#!/usr/bin/env python3
"""View census: find every private "view" of a shared header type in the game sources, by layout and by usage.

Usage:
  census_all.py [--root REPO] [--out DIR] [--src-dir src/game] [--config CONFIG.json] [--report-data]

Reads DIR/offsets.json (made by dump_offsets.py with the real compiler) and writes DIR/census.json (one record per
definition), DIR/globalviews.json (extern declarations of a shared global with another type), DIR/census_tiers.json
(records with a tier, the work list for the next rounds) and DIR/REPORT.md (totals by tier, family, file).
--report-data re-tiers an existing census.json without rescanning. Defaults: REPO = this repository, DIR =
REPO/build/census. CONFIG (optional) tunes the usage check and grouping for one game:
  {"globals": {"SW": ["g_sw"], ...},            names of the global instance of a type (extern-view check)
   "extra_tokens": {"JOB": "currentJob|\\bpjob\\w*"},   extra regexes that count as usage of a type
   "value_types": ["VECTOR", "MATRIX4", ...],     small value types reported apart from object views
   "families": {"lo.h": "base", "STEPGUARD": "AI", ...}}   header base name or type (or a base in its chain) -> family
Without a config, families are the header base names.

For every struct/class/union defined in a src/game .cpp/.c/.inc file it computes a crude layout (sizes of header types
from offsets.json) and classifies the definition:
  name    - its name is a shared type's name, or one with a view suffix (V, View, Island, _view, 2...);
  layout  - >= 75% of its named fields sit at offsets/sizes of a shared type (inherited and embedded members flattened),
            with at least one distinctive hit, AND (usage check) the struct's name meets a token of that type in one
            statement of its region (per-function namespace or file): the type's name, its global (g_x / address
            name), a Hungarian pointer name (pso, psoFoo), or a function name carrying the type (UpdateSo, ...);
  vte     - a vtable-entry record (short d; short pad; pfn) re-declaring arrayv.h's VTE;
  layout? - layout hit without usage evidence (reported separately, not counted as a view);
  local   - none of the above.
Orthogonal tags: hand-vptr (a vtable-pointer member used to call through slots), padbase (a leading pad standing in
for a base class), island (*Island).
"""
import argparse, collections, json, os, re, sys
_ap = argparse.ArgumentParser(add_help=True, description='View census (see module docstring).')
_ap.add_argument('--root', default=os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))
_ap.add_argument('--out')
_ap.add_argument('--src-dir', default='src/game')
_ap.add_argument('--config')
_ap.add_argument('--report-data', action='store_true')
if any(a in ('-h', '--help') for a in sys.argv[1:]):
    print(__doc__); sys.exit(0)
ARGS = _ap.parse_args()
W = os.path.abspath(ARGS.root)
O = os.path.abspath(ARGS.out or os.path.join(W, 'build', 'census'))
SRC = ARGS.src_dir
T = json.load(open(O + '/offsets.json'))
CFG = json.load(open(ARGS.config)) if ARGS.config else {}
GLOBALS = CFG.get('globals', {})
EXTRA = CFG.get('extra_tokens', {})
VALUE_TYPES = set(CFG.get('value_types', []))
FAMILY_MAP = CFG.get('families', {})

TSZ = {'char': 1, 'uchar': 1, 'u8': 1, 's8': 1, 'bool': 1, 'byte': 1, 'short': 2, 'ushort': 2, 'u16': 2, 's16': 2,
       'int': 4, 'uint': 4, 'float': 4, 'long': 8, 'ulong': 8, 'double': 8, 'unsigned': 4, 'signed': 4,
       'undefined4': 4, 'undefined2': 2, 'undefined1': 1, 'undefined': 1, 'undefined8': 8, 'u32': 4, 's32': 4,
       'u64': 8, 's64': 8, 'OID': 4, 'CID': 4, 'MSGID': 4, 'qword': 16, 'u128': 16, 'GRFEOPID': 4, 'SVKEY': 8,
       'size_t': 4, 'SYMID': 4, 'LABEL': 4, 'TICK': 8, 'GRFZON': 4, 'grfjt': 8, 'JTS': 4, 'JTBS': 4}
for k, v in T.items():
    TSZ.setdefault(k, v['size'])

# ---------- shared types: flattened field maps
FLAT = {}
def flat(t, depth=0):
    if t in FLAT: return FLAT[t]
    v = T[t]; out = []
    b = v.get('base')
    if b in T and depth < 12: out += flat(b, depth + 1)
    for o, z, n, tw, ptr in v['fields']:
        out.append((o, z, n))
        if not ptr and tw in T and tw != t and depth < 4 and T[tw]['size'] <= z:
            for so, sz, sn in flat(tw, depth + 1):
                if so < T[tw]['size']: out.append((o + so, sz, n + '.' + sn))
    FLAT[t] = out
    return out
FM = {}
for t in T:
    m = {}
    for o, z, n in flat(t):
        if not re.match(r'(_?pad|unk_|unknown|_)', n.split('.')[-1]): m.setdefault(o, set()).add(z)
    FM[t] = m
def chain(t):
    out = []
    while t in T and t not in out:
        out.append(t); t = T[t].get('base')
    return out

def family(t):
    ch = chain(t) or [t]; h = T[t]['header'] if t in T else ''
    hb = os.path.basename(h)
    for c in ch:
        if c in FAMILY_MAP: return FAMILY_MAP[c]
    if hb in FAMILY_MAP: return FAMILY_MAP[hb]
    return hb or 'unknown'

def tok_re(t):
    low = t.lower(); camel = t[0] + t[1:].lower()
    parts = [r'\b%s\b' % re.escape(t), r'\bp%s(?:[A-Z0-9_]\w*)?\b' % re.escape(low),
             r'[A-Z][a-z0-9]+%s(?=[A-Z_(]|\b)' % re.escape(camel), r'\b%s[A-Z]\w*\s*\(' % re.escape(camel)]
    for g in GLOBALS.get(t, []): parts.append(r'\b%s\b' % g)
    if t in EXTRA: parts.append(EXTRA[t])
    return re.compile('|'.join(parts))
TOK = {}
def tok(t):
    if t not in TOK: TOK[t] = tok_re(t)
    return TOK[t]

# ---------- local layout calculator (from vu_f4e/census.py, sizes extended to all header types)
def ev(x):
    x = re.sub(r'(0x[0-9A-Fa-f]+|\d+)[uUlL]*', r'\1', x)
    x = re.sub(r'sizeof\s*\(\s*(\w+)\s*\)', lambda m: str(TSZ.get(m.group(1), 'X')), x)
    try: return int(eval(x, {}, {}))
    except Exception: return None
def body_of(s, i):
    d = 1; j = i
    while d and j < len(s):
        if s[j] == '{': d += 1
        elif s[j] == '}': d -= 1
        j += 1
    return s[i:j - 1], j
def layout(body, structs, base=None):
    off = 0; fields = []; hasv = False
    if base:
        if base in structs: bf, bsz, bv = structs[base]
        elif base in T: bf, bsz, bv = [(o, z, '^' + n) for o, z, n in flat(base)], T[base]['size'], True
        else: return None
        fields = list(bf); off = bsz; hasv = bv
    while True:
        m = re.search(r'\)\s*(const\s*)?(:[^{;]*)?\{', body)
        if not m: break
        b, j = body_of(body, m.end()); body = body[:m.start()] + ');' + body[j:]
    ownv = re.search(r'\bvirtual\b', body) is not None
    stmts = []; k = 0
    while k < len(body):
        m = re.match(r'\s*(public|private|protected)\s*:', body[k:])
        if m: k += m.end(); continue
        m = re.match(r'\s*(union|struct)\s*(\w*)\s*\{', body[k:])
        if m:
            b, j = body_of(body, k + m.end())
            rest = re.match(r'\s*([^;]*);', body[j:])
            stmts.append(('U' if m.group(1) == 'union' else 'S', b, rest.group(1).strip() if rest else ''))
            k = j + (rest.end() if rest else 0); continue
        m = re.match(r'\s*enum\b[^{;]*\{', body[k:])
        if m:
            b, j = body_of(body, k + m.end()); k = body.find(';', j) + 1 or len(body); continue
        e = body.find(';', k)
        if e < 0: break
        stmts.append(('D', body[k:e].strip(), '')); k = e + 1
    for kind, st, decl in stmts:
        if kind in 'US':
            if kind == 'U':
                sub = []; mx = 0; al = 1
                for part in re.split(r';', st):
                    part = part.strip()
                    if not part: continue
                    if re.match(r'(struct|union)\s*\w*\s*\{', part):    # nested struct in union: crude
                        r2 = None
                        return None
                    r = layout_one(part, structs)
                    if r is None: continue
                    if r == 'bad': return None
                    sz, names = r
                    for nm, cnt in names: sub.append((sz, nm)); mx = max(mx, sz * cnt)
                    al = max(al, min(sz, 16 if sz >= 16 else 4))
                if off % min(al, 4): off += min(al, 4) - off % min(al, 4)
                fields += [(off, z, n) for z, n in sub]; off += mx
            else:
                r = layout(st, structs)
                if r is None: return None
                cnt = 1; nm = ''
                mm = re.match(r'(\w*)\s*((\[[^\]]*\])*)', decl)
                if mm:
                    nm = mm.group(1)
                    for a in re.findall(r'\[([^\]]*)\]', mm.group(2)):
                        v = ev(a)
                        if v is None: return None
                        cnt *= v
                if off % 4: off += 4 - off % 4
                for o, z, n in r[0]: fields.append((off + o, z, (nm + '.' if nm else '') + n))
                off += r[1] * cnt
            continue
        if not st or re.match(r'(typedef|enum|friend|static|virtual|using|inline|template|operator)\b', st) or '(' in st:
            continue
        r = layout_one(st, structs)
        if r is None: continue
        if r == 'bad': return None
        sz, names = r
        al = min(sz, 4) if sz < 16 else 4
        if sz == 8 and re.match(r'\s*(long|ulong|double|u64|s64|undefined8)\b', st): al = 8
        if sz == 16 and re.match(r'\s*(qword|u128|VECTOR|VECTOR4|MATRIX4|QW)\b', st): al = 16
        if al and off % al: off += al - off % al
        for nm, cnt in names:
            fields.append((off, sz, nm)); off += sz * cnt
    if ownv and not hasv:
        fields.insert(0, (off, 4, '__vptr')) if False else fields.append((off, 4, '__vptr')); off += 4; hasv = True
    if off % 4: off += 4 - off % 4
    return fields, off, hasv
def layout_one(st, structs):
    st = re.sub(r'\b(const|volatile|mutable)\b', '', st).strip()
    if not st: return None
    if re.search(r'\w\s*:\s*\d+\s*$', st): return 'bad'
    m = re.match(r'((?:unsigned|signed|struct|class|union)\s+)*(\w+)\s*(.*)$', st, re.S)
    if not m: return 'bad'
    t, rest = m.group(2), m.group(3)
    pre = m.group(1) or ''
    if t in ('long', 'short', 'char', 'int') and pre.strip() in ('unsigned', 'signed'): pass
    if t in ('unsigned', 'signed') and not rest: return None
    if t == 'long' and rest.startswith('long'): rest = rest[4:]
    out = []; sz0 = None
    for d in rest.split(','):
        mm = re.match(r'\s*(\**)\s*(\w+)\s*((\[[^\]]*\])*)\s*(=.*)?$', d.strip(), re.S)
        if not mm: return 'bad'
        ptr, nm, arr = mm.group(1), mm.group(2), mm.group(3)
        if ptr: sz = 4
        elif t in structs: sz = structs[t][1]
        elif t in TSZ: sz = TSZ[t]
        else: return 'bad'
        cnt = 1
        for a in re.findall(r'\[([^\]]*)\]', arr):
            v = ev(a)
            if v is None: return 'bad'
            cnt *= v
        if sz0 is None: sz0 = sz
        elif sz != sz0: return 'bad'
        out.append((nm, cnt))
    return sz0, out

def strip(s):
    s = re.sub(r'/\*.*?\*/', lambda m: re.sub(r'[^\n]', ' ', m.group(0)), s, flags=re.S)
    s = re.sub(r'//[^\n]*', lambda m: ' ' * len(m.group(0)), s)
    s = re.sub(r'"(\\.|[^"\\\n])*"', lambda m: '"' + ' ' * (len(m.group(0)) - 2) + '"', s)
    return s

SUFFIX = re.compile(r'^(?:[A-Z]_)?(\w+?)(?:V\d*|View|VIEW|Island|ISLAND|_view|_V|_v|Local|LOCAL|Part|PART|_\w+|\d+)$')
def name_type(n):
    if n in T: return n
    m = SUFFIX.match(n)
    if m and m.group(1) in T and len(m.group(1)) >= 2: return m.group(1)
    m = re.match(r'^(\w+?)_?V$', n)
    if m and m.group(1) in T: return m.group(1)
    return None

DEF = re.compile(r'\b(struct|class|union)\s+(\w+)\s*(:\s*(?:public\s+|private\s+|protected\s+)?(\w+)[^{;]*)?\{')
def regions(s):
    """namespace spans [(start,end,name)] (innermost lookup by position)"""
    out = []
    for nm in re.finditer(r'\bnamespace\s+(\w*)\s*\{', s):
        b, j = body_of(s, nm.end()); out.append((nm.start(), j, nm.group(1)))
    return out
def region_of(regs, pos, n):
    best = (0, n, '')
    for a, b, nm in regs:
        if a <= pos < b and (b - a) < (best[1] - best[0]): best = (a, b, nm)
    return best

def is_vte_layout(fields, size):
    return size == 8 and len(fields) in (2, 3) and fields[0][1] == 2 and fields[-1][0] == 4 and fields[-1][1] == 4

def scan(files):
    recs = []
    for p in sorted(files):
        raw = open(p, errors='ignore').read(); s = strip(raw)
        regs = regions(s)
        structs = {}; vtes = set(['VTE', 'VT', 'VE', 'VTENT', 'VTGRAB'])
        for m in DEF.finditer(s):
            kind, name, base = m.group(1), m.group(2), m.group(4)
            body, j = body_of(s, m.end())
            line = s.count('\n', 0, m.start()) + 1
            a, b, nsname = region_of(regs, m.start(), len(s))
            # in a function body?  (crude: preceding unmatched '{' after the region start that is not a namespace/class)
            r = None
            try: r = layout(body if kind != 'union' else body, structs, base) if kind != 'union' else None
            except RecursionError: r = None
            if kind == 'union':
                # union: size = max member size
                mx = 0; ok = True; fl = []
                for part in body.split(';'):
                    if not part.strip() or '(' in part: continue
                    rr = layout_one(part.strip(), structs)
                    if rr in (None, 'bad'): ok = False; break
                    for nm, cnt in rr[1]: mx = max(mx, rr[0] * cnt); fl.append((0, rr[0], nm))
                r = (fl, mx, False) if ok else None
            if r: structs[name] = r
            rec = dict(file=os.path.relpath(p, W), line=line, kind=kind, name=name, base=base, ns=nsname,
                       size=r[1] if r else None, parsed=r is not None, cls='local', type=None, tags=[])
            fields = r[0] if r else []
            if r and is_vte_layout(fields, r[1]): vtes.add(name)
            # ---- tags
            if name.endswith(('Island', 'ISLAND')): rec['tags'].append('island')
            # hand vptr: member of a vtable-entry pointer type or a vt-named pointer, and slot calls in the region
            vm = re.search(r'(?:^|[;{\s])(?:struct\s+)?(\w+)\s*\*+\s*(\w+)\s*;', body)
            vptr_member = None
            for mm in re.finditer(r'(?:^|[;{}])\s*(?:struct\s+)?(\w+)\s*(\*+)\s*(\w+)\s*(?:\[[^\]]*\])?\s*;', body):
                ty, stars, mem = mm.group(1), mm.group(2), mm.group(3)
                if ty in vtes or ty.startswith(('VT', 'VE')) and len(ty) <= 6 or \
                        (ty in ('void', 'char', 'int', 'uint') and re.match(r'(p?vt\w*|vtable\w*|vptr\w*|_vptr\w*|__vptr\w*|vtbl\w*|pvtbl)$', mem)):
                    vptr_member = mem; break
            if vptr_member:
                rec['vptroff'] = next((o for o, z, n in fields if n == vptr_member), None)
                reg = s[a:b]
                if re.search(r'(->|\.)%s\s*(\[|->|\+|\))' % re.escape(vptr_member), reg):
                    rec['tags'].append('hand-vptr')
                else:
                    rec['tags'].append('vptr-member')
            # padded over base
            if fields:
                f0 = fields[0]
                if f0[0] == 0 and f0[1] >= 1 and re.match(r'(_?_?pad\w*|base\w*|_\w*|unk\w*|lo\w*|so\w*|bytes?)$', f0[2]):
                    # full extent of the leading pad (array): next field offset
                    nxt = fields[1][0] if len(fields) > 1 else r[1]
                    if nxt >= 0x10:
                        rec['tags'].append('padbase')
                        cand = [t for t in T if T[t]['size'] == nxt and any(T[d].get('base') == t for d in T) and nxt >= 0x20 and len(fields) > 1]
                        rec['padsz'] = nxt; rec['padtype'] = cand[0] if len(cand) == 1 else ('/'.join(sorted(cand)) if cand else None)
            if base in T and r and len(fields) > len(flat(base)): rec['tags'].append('derived-ext')
            if r and any(re.match(r'_?_?(pad|p\d|unk|_)', n) and z * 1 >= 0 and (j2 - o) >= 0x10
                         for (o, z, n), j2 in zip(fields, [f[0] for f in fields[1:]] + [r[1]]) if o > 0):
                rec['tags'].append('interior-pad')
            rec['sig'] = (r[1], tuple((o, z) for o, z, n in fields)) if r else None
            # ---- classification
            nt = name_type(name)
            if nt and nt not in ('VTE',):
                rec['cls'] = 'name'; rec['type'] = nt
                if r and fields:
                    fm = FM.get(nt, {})
                    nf = [(o, z, n) for o, z, n in fields if not n.startswith('^') and not re.match(r'(_?_?pad|unk|__vptr|_)', n.split('.')[-1])]
                    if nf:
                        ok = [x for x in nf if x[0] in fm and (x[1] in fm[x[0]] or x[1] == 4)]
                        rec['namefit'] = '%d/%d' % (len(ok), len(nf))
                        if len(ok) / len(nf) < 0.5 and r[1] > T[nt]['size'] + 4: rec['tags'].append('name-conflict')
            elif name in vtes or (r and is_vte_layout(fields, r[1])) or re.match(r'^(VTE|VT|VE|VTENT)\d*$', name):
                rec['cls'] = 'vte'; rec['type'] = 'VTE'
            elif r and fields:
                named = [(o, z, n) for o, z, n in fields if not n.startswith('^') and
                         not re.match(r'(_?_?pad|unk|__vptr|p\d+$|pad\w*$|_\w*$|u\d+$|unknown|field_|f_?\d|d_?\d|w\d|b\d|x[0-9A-F]+$|o[0-9A-F]+$)', n.split('.')[-1])]
                inherited_from = base if base in T else None
                cands = []
                for t, fm in FM.items():
                    if t in VALUE_TYPES or t == 'VTE': continue
                    if r[1] > T[t]['size'] + 4: continue
                    if T[t]['size'] < 0x10: continue
                    if not named: continue
                    mt = [(o, z, n) for o, z, n in named if o in fm and (z in fm[o] or (z == 4 and 4 in fm[o]))]
                    if not mt: continue
                    dist = [x for x in mt if x[0] >= max(0x10, min(0x200, T[t]['size'] // 8))]
                    if not dist and len(mt) < 3: continue
                    if len(mt) / len(named) < 0.75: continue
                    cands.append((t, len(mt), len(named)))
                if inherited_from:
                    # derived from a shared type: the extension is a view of a derived type only if one matches
                    cands = [c for c in cands if inherited_from in chain(c[0]) and c[0] != inherited_from]
                if cands:
                    a0, b0 = a, b
                    reg = s[a0:b0]
                    stmts = [st for st in re.split(r'[;{}]', reg) if re.search(r'\b%s\b' % re.escape(name), st)]
                    used = []
                    nsn = re.sub(r'^M_', '', nsname or '')
                    for t, k, n in cands:
                        tr = tok(t)
                        if any(tr.search(st) for st in stmts) or ('padbase' in rec['tags'] and nsn and re.search(r'%s(?=[A-Z_]|$)' % re.escape(t[0] + t[1:].lower()), nsn) and len(t) >= 2):
                            used.append((t, k, n))
                    pool = used or cands
                    pool.sort(key=lambda c: (-c[1], T[c[0]]['size']))
                    rec['type'] = pool[0][0]; rec['match'] = '%d/%d' % (pool[0][1], pool[0][2])
                    rec['cls'] = 'layout' if used else 'layout?'
                    rec['alts'] = len(pool)
            recs.append(rec)
    return recs

def main():
    files = []
    for d, _, fs in os.walk(os.path.join(W, SRC)):
        for f in fs:
            if f.endswith(('.cpp', '.c', '.inc')): files.append(os.path.join(d, f))
    recs = scan(files)
    json.dump(recs, open(O + '/census.json', 'w'), indent=0)
    cnt = collections.Counter(r['cls'] for r in recs)
    print(len(recs), 'definitions', dict(cnt))
    print('tags', collections.Counter(t for r in recs for t in r['tags']))

    # global views: extern declarations of a shared global's names with a type other than the real one
    gv = collections.Counter(); gvf = collections.defaultdict(set)
    for p in files:
        s = strip(open(p, errors='ignore').read())
        for t, names in GLOBALS.items():
            for g in names:
                for m in re.finditer(r'\bextern\s+(?:"C"\s+)?(?:struct\s+)?(\w+)\s*(\**)\s*(\w+)\s*(\[[^\]]*\])*\s*(__asm__\s*\(\s*"(\w+)"\s*\))?\s*;', s):
                    if (m.group(3) == g or m.group(6) == g) and m.group(1) != t:
                        gv[t] += 1; gvf[t].add(os.path.relpath(p, W))
    json.dump({t: [gv[t], sorted(gvf[t])] for t in gv}, open(O + '/globalviews.json', 'w'), indent=0)
    print('global-alias externs', dict(gv))

def tier(r, helpers):
    if r['cls'] in ('name', 'layout') or 'island' in r['tags']: return 'A'
    if 'hand-vptr' in r['tags']: return 'B'
    if 'vptr-member' in r['tags']: return 'B'
    if 'padbase' in r['tags'] or 'derived-ext' in r['tags']: return 'C'
    if 'interior-pad' in r['tags'] and r['cls'] != 'vte': return 'C2'
    if r['cls'] == 'vte': return 'D'
    if r['name'] in helpers: return 'E'
    return 'F'

def report():
    R = json.load(open(O + '/census.json')); G = json.load(open(O + '/globalviews.json'))
    # duplicated helper records: same name and same layout defined in >= 3 files, not otherwise classified
    byname = collections.defaultdict(set)
    for r in R:
        if r['cls'] == 'local' and r['sig'] and not r['tags'] and len(r['name']) > 1:
            byname[(r['name'], json.dumps(r['sig']))].add(r['file'])
    helpers = {}
    for (n, sig), fs in byname.items():
        if len(fs) >= 3: helpers[n] = helpers.get(n, 0) + len(fs)
    for r in R:
        r['tier'] = tier(r, helpers)
        if r['tier'] == 'B' and not r['type']: r['type'] = '(vptr object, type unknown)'
    out = []; P = out.append
    tc = collections.Counter(r['tier'] for r in R)
    json.dump(R, open(O + '/census_tiers.json', 'w'), indent=0)
    write_report(R, G, helpers, tc)
    return R, G, helpers, tc

FAMS = sorted(set(FAMILY_MAP.values())) or sorted({family(t) for t in T})
def fam_of(t):
    if t in T: return family(t)
    if t and t.startswith('(vptr'): return '(vptr object)'
    return '?'
def short(f): return f.replace(SRC.rstrip('/') + '/', '')
def subdir(f):
    parts = f.split('/')
    return '/'.join(parts[2:4]) if len(parts) > 4 else '/'.join(parts[2:3])

def write_report(R, G, helpers, tc):
    P = []; w = P.append
    N = len(R)
    REM = ('A', 'B', 'C', 'C2', 'D')
    rem = [r for r in R if r['tier'] in REM]
    A = [r for r in R if r['tier'] == 'A']
    Bv = [r for r in R if 'hand-vptr' in r['tags'] or 'vptr-member' in r['tags']]
    lq = [r for r in R if r['tier'] == 'F' and r['cls'] == 'layout?']
    w('# View census over all shared types')
    w('')
    w('Measurement, read-only. Script: tools/cleanup/census/census_all.py (layouts of the shared types from '
      'tools/cleanup/census/dump_offsets.py, which asks the game compiler for offsetof/sizeof of every member of every '
      'struct/class/union in include/*.h and include/sly2/*.h: %d types). Raw per-definition data: census_tiers.json.' % len(T))
    w('')
    w('## Method')
    w('')
    w('Every struct/class/union definition in src/game (.cpp/.c/.inc) gets a crude layout (sizes of header types from the '
      'compiler dump; nested structs/unions handled, bitfields and unparseable bodies skipped: %d of %d unparsed). '
      'Each definition then lands in one tier:' % (sum(not r['parsed'] for r in R), N))
    w('')
    w('- **A, view of a named shared type**: its name is a shared type (or one with a view suffix: V, Island, digits...; '
      '`name`), or >= 75% of its named fields sit at that type\'s offsets and sizes (inherited and embedded members '
      'flattened; at least one hit past max(0x10, min(0x200, sizeof/8)) or three hits; the namespace-name evidence only for leading-pad views) **and** the usage check passes: in one statement of its '
      'region (per-function namespace, else file) its name meets the type\'s name, its global (g_x or the address '
      'name), a Hungarian pointer (pso, psoFoo), a function name carrying the type (UpdateSo, ...), or the namespace '
      'name carries it (`layout`). Ties go to the type with the most hits, then the smallest (most-base) type.')
    w('- **B, hand-vptr view** (not in A): holds a vtable pointer (a VTE/VT/VE-typed pointer or a vt/vtable/vptr-named '
      'pointer) that the region indexes or calls through.')
    w('- **C, leading-pad view, target unknown**: first member a pad of >= 0x10 bytes, or derived from a shared type '
      'with private extension fields. Almost always an object view; the target is often a class without a header yet.')
    w('- **C2, interior-pad record**: a pad of >= 0x10 bytes after a real field (partial view or half-known record).')
    w('- **D, vtable-entry record**: a private re-declaration of arrayv.h\'s VTE (short d; short pad; pfn).')
    w('- **E, duplicated helper**: same name and same layout in >= 3 files, no pad (M3, M4, VU_FLOAT, MSG...): not views '
      'of a header type but repeated private copies (a shared helper header would remove them).')
    w('- **F, genuinely local**: none of the above (function-local records, unions for qword/float punning, file-level '
      'records of classes that have no header). Includes %d `layout?` definitions: a layout hit without usage '
      'evidence; a sample of 14 had about 3-4 real views of other types, the rest coincidences of small float/int records.' % len(lq))
    w('')
    w('Precision: sample the A and C tiers by hand before trusting the counts; type attribution inside a hierarchy is approximate.')
    w('')
    w('## Totals')
    w('')
    w('| tier | definitions | share |')
    w('|---|---:|---:|')
    lab = {'A': 'A view of a named shared type', 'B': 'B hand-vptr view (not in A)', 'C': 'C leading-pad / derived-extension view',
           'C2': 'C2 interior-pad record', 'D': 'D private VTE record', 'E': 'E duplicated helper record', 'F': 'F genuinely local'}
    for t in ('A', 'B', 'C', 'C2', 'D', 'E', 'F'):
        w('| %s | %d | %.0f%% |' % (lab[t], tc[t], 100.0 * tc[t] / N))
    w('| all struct/class/union definitions in src/game | %d | |' % N)
    w('')
    av = [r for r in A if r['type'] in VALUE_TYPES]
    w('Tier A splits into %d views of object/record types and %d private re-declarations of small value types '
      '(RGBA, VECTOR, MATRIX3/4, LM, SMP, CLQ, DL/SL...: %s).' % (len(A) - len(av), len(av),
      ', '.join('%s %d' % kv for kv in collections.Counter(r['type'] for r in av).most_common(10))))
    w('')
    w('Category counts (overlapping tags):')
    w('')
    w('- hand-vptr views (a vtable-pointer member): %d (%d of them index/call through it in their region; vptr at +0xC, '
      'BASIC\'s position: %d); %d of them are also tier A.' % (len(Bv), sum('hand-vptr' in r['tags'] for r in R),
      sum(r.get('vptroff') == 12 for r in Bv), sum(r['tier'] == 'A' for r in Bv)))
    pb = [r for r in R if 'padbase' in r['tags']]; pbt = [r for r in pb if r.get('padtype')]
    ct = collections.Counter(r['padtype'] for r in pbt)
    w('- padded-over-base: %d definitions open with a pad of >= 0x10 bytes; %d of those pads are exactly the size of a '
      'shared base class (>= 0x20): %s. Plus %d definitions that derive from a shared type and add private fields.'
      % (len(pb), len(pbt), ', '.join('%s %d' % (k, v) for k, v in ct.most_common(12)), sum('derived-ext' in r['tags'] for r in R)))
    isl = [r for r in R if 'island' in r['tags']]
    ci = collections.Counter(r['name'] for r in isl)
    w('- `*Island` structs: %d in %d files: %s.' % (len(isl), len(set(r['file'] for r in isl)), ', '.join('%s %d' % kv for kv in ci.most_common())))
    w('- private VTE re-declarations (D, plus %d inside other tiers): %d.' % (sum(r['cls'] == 'vte' for r in R) - tc['D'], sum(r['cls'] == 'vte' for r in R)))
    w('- global aliases (not struct definitions; extern declarations of a shared global\'s name or address with another '
      'type): %d in %d files: %s.' % (sum(v[0] for v in G.values()), len(set(f for v in G.values() for f in v[1])),
      ', '.join('%s %d' % (k, v[0]) for k, v in sorted(G.items(), key=lambda kv: -kv[1][0]))))
    w('- duplicated helpers (E) by name: %s.' % ', '.join('%s %d' % kv for kv in sorted(helpers.items(), key=lambda kv: -kv[1])[:20]))
    w('')
    w('## Remaining vs. genuinely local')
    w('')
    nrem = len(rem)
    w('Views still to unify (A+B+C+C2+D): **%d of %d definitions (%.0f%%)**; of these %d (A) name a shared type now and '
      'can be folded onto it, %d (B) are hand-vptr objects (fold onto the polymorphic class once identified), %d '
      '(C+C2) are pad views whose target is often a class without a header (needs the type first), %d (D) are VTE '
      'copies that disappear with the hand-vptr calls. Duplicated helpers (E): %d (%.0f%%). Genuinely local (F): %d '
      '(%.0f%%), fine to keep; with the F `layout?` part counted as 25%% views, the remaining share moves by about %d.'
      % (nrem, N, 100.0 * nrem / N, tc['A'], tc['B'], tc['C'] + tc['C2'], tc['D'], tc['E'], 100.0 * tc['E'] / N,
         tc['F'], 100.0 * tc['F'] / N, len(lq) // 4))
    w('')
    w('**Not in this checkpoint.** The unmerged view branches carry further work measured the same way (each branch '
      'tip against its merge-base with this checkpoint; views = A+B+C+C2+D): names/vu-f1-base-4 -110 '
      '(merge-base 503 commits behind; includes the game.cpp JOB/TASK pad views, 53, that still count here), names/vu-f2-player-3 -139 (386 behind), names/vu-f4-world-5 -121 (511 behind; '
      'in integrate/names-r7, also unmerged; includes the screen.cpp and memcard.cpp task/save conversions), '
      'names/vu-f6-render-4 -42 (1 behind), names/vu-f8-sound-ui-3 -3 (576 behind): about 415 views, an upper '
      'bound since the old bases may overlap with work already redone here. Those branches also remove 2-51 tier-F '
      'definitions each, a sign that F still holds some views the census cannot see.')
    w('')
    # ---- per family
    w('## Per family (tier A by type family; B with a known type included)')
    w('')
    fam = collections.defaultdict(list)
    for r in R:
        if r['tier'] in ('A', 'B') and r['type']: fam[fam_of(r['type'])].append(r)
    w('| family | views | files | types hit | top types |')
    w('|---|---:|---:|---:|---|')
    for f in FAMS + ['?']:
        L = fam.get(f, [])
        if not L: continue
        c = collections.Counter(r['type'] for r in L)
        w('| %s | %d | %d | %d | %s |' % (f, len(L), len(set(r['file'] for r in L)), len(c), ', '.join('%s %d' % kv for kv in c.most_common(8))))
    w('')
    w('Families as defined here: base = basic/lo/xfm/so/po/dli.h; player = STEP, STEPPLAYER, JT and players.h/leap.h, '
      'plus classes.h classes deriving from STEPPLAYER; AI = STEPGUARD, BHV*, guard.h; world = sw/game/save/joy/clock/'
      'task/gs/wr.h; camera = cm.h; render = glob/shd/render/rip/emitter/light/dzg.h, GLOB*/SUBGLOB; anim = act.h, aseg.h, '
      'XF/XA; sound/UI/splice = snd/sndmgr/screen.h and the splice VM headers (cref/pair/varb/cframe/gc/eopid); '
      'objects = other classes.h classes; core/util = heap/bis/dmas/vifs/arrayv/slotheap and the value types.')
    w('')
    w('The C tier (pad views, no confirmed type) by the family of its best layout guess (tentative): %s.' %
      ', '.join('%s %d' % kv for kv in collections.Counter(fam_of(r['type']) if r['type'] else 'no guess' for r in R if r['tier'] in ('C', 'C2')).most_common()))
    w('')
    # ---- per type top 40
    w('## Per shared type (top 40; tiers A and B with a type)')
    w('')
    bt = collections.defaultdict(list)
    for r in R:
        if r['tier'] in ('A', 'B') and r['type']: bt[r['type']].append(r)
    for t, L in sorted(bt.items(), key=lambda kv: -len(kv[1]))[:40]:
        fc = collections.Counter(short(r['file']).split('/')[-1] for r in L)
        how = collections.Counter(r['cls'] if r['tier'] == 'A' else 'vptr' for r in L)
        w('- **%s** (%s) %d [%s]: %s' % (t, fam_of(t), len(L), ' '.join('%s %d' % kv for kv in how.most_common()),
                                        ', '.join('%s%s' % (f, ' x%d' % n if n > 1 else '') for f, n in fc.most_common(12)) + (' +%d files' % (len(fc) - 12) if len(fc) > 12 else '')))
    w('')
    w('%d shared types have at least one A/B view; %d have none.' % (len([t for t in bt if t in T]), len(T) - len([t for t in bt if t in T])))
    w('')
    # ---- top 20 files by remaining views
    w('## Top 20 files by remaining views (A+B+C+C2+D)')
    w('')
    w('| file | remaining | A | B | C+C2 | D | all defs |')
    w('|---|---:|---:|---:|---:|---:|---:|')
    allf = collections.Counter(r['file'] for r in R)
    fr = collections.Counter(r['file'] for r in rem)
    for f, n in fr.most_common(20):
        L = [r for r in R if r['file'] == f]
        g = collections.Counter(r['tier'] for r in L)
        w('| %s | %d | %d | %d | %d | %d | %d |' % (short(f), n, g['A'], g['B'], g['C'] + g['C2'], g['D'], allf[f]))
    w('')
    # ---- definitions by directory and file
    w('## All definitions by directory')
    w('')
    w('| directory | defs | files | remaining views | A |')
    w('|---|---:|---:|---:|---:|')
    dd = collections.defaultdict(list)
    for r in R: dd[subdir(r['file'])].append(r)
    for d, L in sorted(dd.items(), key=lambda kv: -len(kv[1])):
        w('| %s | %d | %d | %d | %d |' % (d, len(L), len(set(r['file'] for r in L)), sum(r['tier'] in REM for r in L), sum(r['tier'] == 'A' for r in L)))
    w('')
    w('## All definitions by file (top 30)')
    w('')
    w('| file | defs | remaining views | genuinely local |')
    w('|---|---:|---:|---:|')
    for f, n in allf.most_common(30):
        w('| %s | %d | %d | %d |' % (short(f), n, fr[f], sum(1 for r in R if r['file'] == f and r['tier'] == 'F')))
    w('')
    w('%d files define at least one struct/class/union; %d files hold at least one remaining view.' % (len(allf), len(fr)))
    open(O + '/REPORT.md', 'w').write('\n'.join(P) + '\n')

if __name__ == '__main__':
    os.makedirs(O, exist_ok=True)
    if ARGS.report_data: report()
    else: main(); report()
