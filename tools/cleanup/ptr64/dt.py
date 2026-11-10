#!/usr/bin/env python3
"""dt.py [--long4] [-j N] ROOT OUT.json [TU ...] -- exact 64-bit danger test for TU-private record views (facts pass).

Per src/game TU: clang -fdump-record-layouts (x32 = 4-byte pointers, and x86_64) + -ast-dump (x32).
A view = record defined in the .cpp itself whose layout clang computed. Accessed = named by a MemberExpr.
Per view: direct fields (32/64-bit offsets), bases, flattened 'wide' slots (pointer/long leaves, 32-bit offsets).
Danger is decided in judge.py (needs the object's class); this script only extracts facts.

EE data model: 4-byte pointers but 8-byte `long` (the EE is a 64-bit MIPS; the PS2 gcc uses long = 64 bits). By
default the x32 layouts are computed from the preprocessed TU with plain `long` rewritten to `long long`, so `long`
is not a hazard by itself. --long4 keeps clang's x32 long = 4 bytes and flags long leaves as wide (older model).
ROOT = decomp checkout (needs clang++ with x32 support); TUs default to every src/game .cpp/.c. Writes OUT.json
{"views": [...], "headers": {...}}.
"""
import argparse, os, re, sys, json, subprocess, concurrent.futures, tempfile
_ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
_ap.add_argument('root'); _ap.add_argument('out'); _ap.add_argument('tus', nargs='*')
_ap.add_argument('--long4', action='store_true', help='4-byte long (clang x32 default) instead of the EE 8-byte long')
_ap.add_argument('-j', type=int, default=2, help='parallel TUs (default 2)')
_A = _ap.parse_args()
ROOT, OUT = _A.root, _A.out
TMP = tempfile.mkdtemp(prefix='dt_long8_')
FL = ['-fsyntax-only', '-x', 'c++', '-std=gnu++98', '-fno-exceptions', '-fno-rtti', '-Iinclude', '-w', '-ferror-limit=0']
T32, T64 = 'x86_64-linux-gnux32', 'x86_64-linux-gnu'

LONG8 = not _A.long4
_pp = {}
def pp8(tu):
    """EE model: 4-byte pointers, 8-byte long. Preprocess for x32 and rewrite plain long as long long."""
    if tu in _pp: return _pp[tu]
    txt = subprocess.run(['clang++', '-target', T32, '-E', '-x', 'c++', '-std=gnu++98', '-Iinclude', '-w', tu], cwd=ROOT, capture_output=True, text=True, errors='replace').stdout
    out = []
    for l in txt.split('\n'):
        if not l.startswith('#'):
            l = re.sub(r'\blong\s+long\b', '@LL@', l); l = re.sub(r'\blong\s+double\b', '@LD@', l)
            l = re.sub(r'\blong\b', 'long long', l).replace('@LL@', 'long long').replace('@LD@', 'long double')
        out.append(l)
    fn = os.path.join(TMP, tu.replace('/', '__') + '.ii')
    open(fn, 'w').write('\n'.join(out))
    _pp[tu] = fn; return fn

def clang(tu, tgt, extra):
    src = pp8(tu) if (LONG8 and tgt == T32 and '-fdump-record-layouts' in extra) else tu
    fl = FL if src == tu else [f for f in FL if f != '-x' and f != 'c++'] + ['-x', 'c++']
    return subprocess.run(['clang++', '-target', tgt] + fl + extra + [src], cwd=ROOT, capture_output=True, text=True, errors='replace').stdout

LINE = re.compile(r'^\s*(\d+)(?::(\d+)-(\d+))? \|( *)(.*)$')
def parse_layouts(txt):
    """-> {recname: {'size':n, 'align':a, 'lines':[(off, depth, text)]}}"""
    recs = {}
    for blk in txt.split('*** Dumping AST Record Layout')[1:]:
        ls = blk.strip('\n').split('\n')
        rows = []; size = None
        for l in ls:
            m = LINE.match(l)
            if m:
                rows.append((int(m.group(1)), len(m.group(4)), m.group(5).strip(), m.group(2) is not None)); continue
            m = re.search(r'\[sizeof=(\d+), dsize=\d+, align=(\d+)', l)
            if m and size is None: size = (int(m.group(1)), int(m.group(2)))
        if not rows or size is None: continue
        name = rows[0][2]
        name = re.sub(r'^(struct|class|union) ', '', name)
        recs.setdefault(name, {'size': size[0], 'align': size[1], 'rows': rows[1:]})
    return recs

LONGT = re.compile(r'\b(long|DItype|intfrac|s64|uint64_t|UDItype|fractype|u64|u_long|ulong)\b')  # 8 bytes on the EE, 4 on x32
def is_wide(t):
    t = t.split(' (base)')[0]
    if '*' in t or '&' in t: return True
    if not LONG8 and re.search(r'\blong\b', t) and 'long long' not in t: return True
    return False

def leaf_info(rows):
    """direct fields + flattened wide slots from a layout dump. rows: (off, depth, text, bitfield)."""
    direct = []; wide = []; bases = []
    n = len(rows)
    for i, (off, d, txt, bf) in enumerate(rows):
        nxt_deeper = i + 1 < n and rows[i + 1][1] > d
        if d == 3 and not bf:
            if txt.endswith('(base)') or txt.endswith('(primary base)') or txt.endswith('(virtual base)'):
                bases.append((off, re.sub(r'^(struct|class|union) ', '', txt.rsplit(' (', 1)[0])))
            elif txt.endswith('vtable pointer)'):
                direct.append({'name': '__vptr', 'off': off, 'type': 'vptr', 'wide': True})
            else:
                m = re.match(r'^(.*?)\s*([A-Za-z_]\w*)$', txt)
                if m: direct.append({'name': m.group(2), 'off': off, 'type': m.group(1), 'wide': is_wide(m.group(1)), 'rec': nxt_deeper})
        if not nxt_deeper and not bf:
            tt = txt.rsplit(' (', 1)[0] if txt.endswith(')') and '(vtable pointer)' in txt else txt
            if 'vtable pointer)' in txt: wide.append(off); continue
            m = re.match(r'^(.*?)\s*([A-Za-z_]\w*)$', txt)
            ty = m.group(1) if m else txt
            if is_wide(ty):
                dims = [int(x) for x in re.findall(r'\[(\d+)\]', ty)]
                cnt = 1
                for x in dims: cnt *= x
                if dims and '(*)' not in ty.split('[')[0]:
                    wide += [off + 4 * k for k in range(cnt)]  # 32-bit stride
                else: wide.append(off)
    return direct, wide, bases

NODE = re.compile(r'^([| `-]*)(\w+) (0x[0-9a-f]+)(.*)$')
LOCF = re.compile(r'([\w./+-]+\.(?:cpp|c|h|hpp|inc)):(\d+):\d+')
def parse_ast(txt, mainfile):
    """-> records {qual: {'file':f,'line':n}}, fields {addr: (qual, name)}, accessed set (qual,name)"""
    stack = []  # (depth, kind, name)
    lastfile = None; lastline = 0
    recs = {}; fields = {}; acc = []
    for l in txt.split('\n'):
        m = NODE.match(l)
        if not m: continue
        depth = len(m.group(1)) // 2; kind = m.group(2); addr = m.group(3); rest = m.group(4)
        # location tracking
        firstfile = None; firstline = None
        for mm in re.finditer(r'([\w./+-]+\.(?:cpp|c|h|hpp|inc)):(\d+):\d+|line:(\d+):\d+', rest):
            if mm.group(1): lastfile, lastline = mm.group(1), int(mm.group(2))
            else: lastline = int(mm.group(3))
            if firstfile is None: firstfile, firstline = lastfile, lastline
        while stack and stack[-1][0] >= depth: stack.pop()
        if kind in ('NamespaceDecl', 'CXXRecordDecl', 'RecordDecl', 'FunctionDecl', 'CXXMethodDecl', 'ClassTemplateSpecializationDecl'):
            name = None
            if kind == 'NamespaceDecl':
                mm = re.search(r' (\w+)$', rest); name = mm.group(1) if mm else '(anonymous namespace)'
            elif kind in ('CXXRecordDecl', 'RecordDecl'):
                if ' implicit ' in rest: stack.append((depth, 'skip', None)); continue
                mm = re.search(r'\b(?:struct|class|union) (\w+)( definition)?', rest)
                if mm:
                    name = mm.group(1)
                    if mm.group(2):
                        qual = '::'.join(s[2] for s in stack if s[1] in ('NamespaceDecl', 'CXXRecordDecl', 'RecordDecl') and s[2])
                        qual = (qual + '::' if qual else '') + name
                        if firstfile == mainfile: recs.setdefault(qual, {'line': firstline, 'func': next((s[2] for s in reversed(stack) if s[1] in ('FunctionDecl', 'CXXMethodDecl')), None)})
                else: name = None
            else:
                mm = re.search(r" (~?\w+) '", rest); name = mm.group(1) if mm else None
            stack.append((depth, kind, name)); continue
        if kind == 'FieldDecl':
            mm = re.search(r" (?:referenced )?(\w+) '", rest)
            par = [s for s in stack if s[1] in ('NamespaceDecl', 'CXXRecordDecl', 'RecordDecl')]
            if mm and par and par[-1][1] != 'NamespaceDecl' and par[-1][2]:
                qual = '::'.join(s[2] for s in par if s[2])
                fields[addr] = (qual, mm.group(1))
        elif kind == 'MemberExpr':
            mm = re.search(r'(?:->|\.)(\w+) (0x[0-9a-f]+)', rest)
            if mm: acc.append(mm.group(2))
    accessed = set(fields[a] for a in acc if a in fields)
    return recs, accessed

def run(tu):
    l32 = parse_layouts(clang(tu, T32, ['-Xclang', '-fdump-record-layouts']))
    l64 = parse_layouts(clang(tu, T64, ['-Xclang', '-fdump-record-layouts']))
    recs, accessed = parse_ast(clang(tu, T32, ['-Xclang', '-ast-dump']), tu)
    views = []; hdr = {}
    for name, r in l32.items():
        d, w, b = leaf_info(r['rows'])
        ent = {'name': name, 'size': r['size'], 'align': r['align'], 'direct': d, 'wide': w, 'bases': b,
               'haslong': (not LONG8) and any(LONGT.search(t) and 'long long' not in t for (_, _, t, _) in r['rows'])}
        r64 = l64.get(name)
        if r64:
            d64, _, b64 = leaf_info(r64['rows'])
            ent['size64'] = r64['size']
            o64 = {x['name']: x['off'] for x in d64}
            for x in d: x['off64'] = o64.get(x['name'])
            ent['bases64'] = b64
        if name in recs:
            ent.update(file=tu, line=recs[name]['line'], func=recs[name]['func'])
            ent['accessed'] = sorted(n for (q, n) in accessed if q == name)
            views.append(ent)
        else:
            hdr[name] = {'size': r['size'], 'align': r['align'], 'wide': w, 'bases': b, 'direct': [(x['name'], x['off'], x['type']) for x in d]}
    return tu, views, hdr

tus = sorted(os.path.relpath(os.path.join(dp, f), ROOT) for dp, dn, fn in os.walk(os.path.join(ROOT, 'src/game')) for f in fn if f.endswith(('.cpp', '.c')))
if _A.tus: tus = _A.tus
allv = []; allh = {}
with concurrent.futures.ThreadPoolExecutor(_A.j) as ex:
    for tu, v, h in ex.map(run, tus):
        allv += v
        for k, x in h.items(): allh.setdefault(k, x)
json.dump({'views': allv, 'headers': allh}, open(OUT, 'w'))
print('tus', len(tus), 'views', len(allv), 'header records', len(allh))
