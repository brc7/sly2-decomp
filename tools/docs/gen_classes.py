#!/usr/bin/env python3
"""gen_classes.py -- generate the Sly 2 CLASS REFERENCE (docs/reference/classes/*, docs/reference/hidden-types.md).

Re-runnable against any future repo state. Everything structural (hierarchy, sizes, vtables, overrides, vtable
references) is read from the retail ELF; names come from the repo (config/symbol_addrs.txt, the
Get<Cls>ClassDesc getters), optionally enriched by master_names (proposals, marked '?'), the docs class-tree page,
the whole-game object census, the BHV map and the Sly 1 decomp. Field offsets are computed by clang from the repo's
include/sly2 headers and checked against the headers' own compile-time asserts.

Usage (all arguments have defaults matching the 2026-11 workspace):
  python3 gen_classes.py --repo REPO --elf SCUS_973.16 --out OUTDIR [--master DIR] [--levels CSV] [--bhv CSV]
                         [--sly1 DIR] [--no-clang]
Outputs OUTDIR/docs/reference/classes/{index.md,<NAME>.md}, OUTDIR/docs/reference/hidden-types.md,
OUTDIR/data/*.csv (machine-readable tables) and prints a link check.
"""
import argparse, bisect, csv, os, re, shutil, struct, subprocess, sys, tempfile
from collections import defaultdict, Counter

AP = argparse.ArgumentParser()
AP.add_argument('--repo', default='/home/claude/main_snapshot')
AP.add_argument('--elf', default='/home/claude/ref/sly2-decomp/disc/SCUS_973.16')
AP.add_argument('--master', default='/home/claude/ref/reports/master_names_v3')
AP.add_argument('--levels', default='/home/claude/ref/reports/whole_game/objects_by_level.csv')
AP.add_argument('--bhv', default='/home/claude/ref/reports/tu_naming/r9_bhv_screen/bhv_class_map.csv')
AP.add_argument('--sly1', default='/home/claude/ref/sly1')
AP.add_argument('--out', default='/home/claude/ref/reports/class_reference')
AP.add_argument('--no-clang', action='store_true')
A = AP.parse_args()

# ----------------------------------------------------------------------------------------------- ELF access
D = open(A.elf, 'rb').read()
def _phdrs():
    e_phoff, = struct.unpack_from('<I', D, 0x1C); n, = struct.unpack_from('<H', D, 0x2C)
    out = []
    for i in range(n):
        t, o, va, pa, fsz, msz, fl, al = struct.unpack_from('<8I', D, e_phoff + 32 * i)
        if t == 1: out.append((va, o, fsz))
    return out
PH = _phdrs()
def off(a):
    for va, o, fsz in PH:
        if va <= a < va + fsz: return a - va + o
    raise ValueError(hex(a))
def w32(a): return struct.unpack_from('<I', D, off(a))[0]
def vent(a): return struct.unpack_from('<hhI', D, off(a))
def _sections():
    shoff, = struct.unpack_from('<I', D, 0x20); shn, shstr = struct.unpack_from('<HH', D, 0x30)
    hdr = lambda i: struct.unpack_from('<10I', D, shoff + 40 * i)
    so = hdr(shstr)[4]; out = {}
    for i in range(shn):
        h = hdr(i); nm = D[so + h[0]:D.index(b'\0', so + h[0])].decode()
        out[nm] = (h[3], h[3] + h[5])
    return out
SEC = _sections()
TEXT = SEC['.text']
def intext(p): return TEXT[0] <= p < TEXT[1]
DESC_TABLE = 0x2BAAE8; DESC_STRIDE = 0x14; NCLS = 199
def h(x): return ('-0x%X' % -x) if x < 0 else '0x%X' % x
GETTER0 = 0x113A10   # Get<Cls>ClassDesc for cid c is at GETTER0 + 0x10*c (verified below, instruction by instruction)

# ----------------------------------------------------------------------------------------------- repo names
def read_symbols(path):
    N = {}
    for l in open(path):
        m = re.match(r'\s*([A-Za-z_$.][\w$.]*)\s*=\s*0x([0-9A-Fa-f]+);', l)
        if m: N[int(m.group(2), 16)] = m.group(1)
    return N
SYM = read_symbols(os.path.join(A.repo, 'config/symbol_addrs.txt'))
FUNCS = []
for r in csv.DictReader(open(os.path.join(A.repo, 'config/functions.csv'))):
    FUNCS.append((int(r['Start'], 16), int(r['End'], 16), r['Name']))
FUNCS.sort()
FSTART = [f[0] for f in FUNCS]
MATCHED = {}
mp = os.path.join(A.repo, 'config/matched.csv')
if os.path.exists(mp):
    for r in csv.DictReader(open(mp)):
        try: MATCHED[int(r['addr'], 16)] = r
        except Exception: pass
MASTER = {}
mpath = os.path.join(A.master or '', 'master_names.csv')
if A.master and os.path.exists(mpath):
    for r in csv.DictReader(open(mpath)):
        MASTER[int(r['addr'], 16)] = r
def is_placeholder(n): return n is None or n.startswith('func_') or n.startswith('D_') or n.startswith('junk_')
def fname(a):
    """repo name; else master proposal + '?'; else func_XXXXXXXX"""
    n = SYM.get(a) or (MATCHED.get(a) or {}).get('name')
    if n and not is_placeholder(n): return n
    m = MASTER.get(a)
    if m and not is_placeholder(m['final_name']): return m['final_name'] + '?'
    return 'func_%08X' % a
def fsrc(a):
    r = MATCHED.get(a)
    return r['source'] if r and r.get('source') else None
def func_of(a):
    i = bisect.bisect_right(FSTART, a) - 1
    return FUNCS[i][0] if i >= 0 and a < FUNCS[i][1] else None

# ----------------------------------------------------------------------------------------------- descriptors
def decode_la(fa, n=4):
    """value loaded by lui/addiu (or lui+lw-free) pair in the first n instructions of a function"""
    hi = {}
    for k in range(n):
        i = w32(fa + 4 * k); op = i >> 26; rs = (i >> 21) & 31; rt = (i >> 16) & 31; imm = i & 0xFFFF
        if op == 0x0F: hi[rt] = imm << 16
        elif op == 0x09 and rs in hi: return (hi[rs] + (imm - 0x10000 if imm & 0x8000 else imm)) & 0xFFFFFFFF
    return None
CLS = {}
for c in range(NCLS):
    a = DESC_TABLE + DESC_STRIDE * c
    sup, cid, cmax, cb, pfn = struct.unpack_from('<5I', D, off(a))
    assert cid == c, (c, cid)
    par = None if sup == 0 else (sup - DESC_TABLE) // DESC_STRIDE
    g = GETTER0 + 0x10 * c
    CLS[c] = dict(cid=c, addr=a, parent=par, cidmax=cmax, cb=cb, pfnNew=pfn, getter=g,
                  getter_ok=(decode_la(g) == a), children=[])
for c in range(NCLS):
    if CLS[c]['parent'] is not None: CLS[CLS[c]['parent']]['children'].append(c)
GETTERS = {CLS[c]['getter']: c for c in CLS}
# name from the repo getter Get<Cls>ClassDesc -> CLS
for c in CLS:
    gn = SYM.get(CLS[c]['getter'], '')
    m = re.match(r'Get(\w+)ClassDesc$', gn)
    CLS[c]['name'] = m.group(1).upper() if m else 'CID%d' % c
    CLS[c]['getter_name'] = gn or 'func_%08X' % CLS[c]['getter']
NAME2CID = {CLS[c]['name']: c for c in CLS}
assert len(NAME2CID) == NCLS, 'class names not unique'
def chain(c):
    out = []
    while c is not None: out.append(c); c = CLS[c]['parent']
    return out
def subtree(c):
    out = [c]
    for k in CLS[c]['children']: out += subtree(k)
    return out

# optional enrichment: master class names (v3), docs class-tree identification
V3 = {}
for fn in ('class_names_v3.csv', 'class_names.csv'):
    p = os.path.join(A.master or '', fn)
    if A.master and os.path.exists(p):
        for r in csv.DictReader(open(p)): V3[int(r['cid_dec'])] = r
        break
V3T = {}
p = os.path.join(A.master or '', 'cid_table_v3.csv')
if A.master and os.path.exists(p):
    for r in csv.DictReader(open(p)): V3T[int(r['cid'], 16)] = r
TREE_ID = {}
p = os.path.join(A.repo, 'docs/reference/class-tree.md')
if os.path.exists(p):
    for l in open(p):
        m = re.match(r'\|\s*0x[0-9A-F]+ \((\d+)\)\s*\|([^|]*)\|([^|]*)\|([^|]*)\|([^|]*)\|', l)
        if m: TREE_ID[int(m.group(1))] = dict(ident=m.group(4).strip(), tag=m.group(5).strip())
def name_conf(c):
    v = V3.get(c, {}); st = v.get('status', '')
    n = CLS[c]['name']
    if re.match(r'(CID|BHV|WIDGET)\d+$', n): return 'placeholder (decimal CID)'
    if 'Sly 1' in st or 'Lilly' in st: return 'high: Sucker Punch name (Sly 1 / Lilly)'
    m = re.search(r'\((high|medium-high|medium|low|runtime)\)', st)
    if m: return ('high' if m.group(1) == 'runtime' else m.group(1)) + ': team coinage (' + st + ')'
    if st: return 'medium: team coinage (' + st + ')'
    return 'unknown (no master_names data)'
def v3_name(c):
    v = V3.get(c)
    return v['class_name'] if v else None

# ----------------------------------------------------------------------------------------------- vtables
def scan_vtables(minlen=3):
    res = []
    for sname in ('.rodata', '.data'):
        lo, hi = SEC[sname]; a = (lo + 3) & ~3
        while a < hi - 16:
            if vent(a) == (0, 0, 0) and vent(a + 8)[1] == 0 and intext(vent(a + 8)[2]):
                n = 1; b = a + 8
                while b + 8 <= hi:
                    dl, pad, p = vent(b)
                    if pad != 0 or not intext(p) or abs(dl) > 0x4000: break
                    n += 1; b += 8
                if n >= minlen: res.append((a, n)); a = b; continue
            a += 4
    return res
VTS = scan_vtables()
VT = {}   # addr -> dict(addr, n, slots[list of pfn], deltas, cid (by slot-6 getter) or None)
for a, n in VTS:
    sl = [vent(a + 8 * s)[2] for s in range(n)]; dl = [vent(a + 8 * s)[0] for s in range(n)]
    cid = GETTERS.get(sl[6]) if n > 6 else None; alt = False
    if cid is None and n > 6:
        try: da = decode_la(sl[6], 3)
        except Exception: da = None
        if da is not None and DESC_TABLE <= da < DESC_TABLE + DESC_STRIDE * NCLS and (da - DESC_TABLE) % DESC_STRIDE == 0:
            cid = (da - DESC_TABLE) // DESC_STRIDE; alt = True   # a second copy of the getter returns this class's record
    VT[a] = dict(addr=a, n=n, slots=sl, deltas=dl, cid=cid, alt_getter=alt)
for c in CLS: CLS[c]['vtables'] = sorted(a for a in VT if VT[a]['cid'] == c)
# implementation usage across all vtables (for 'generic' detection)
IMPL_USERS = defaultdict(set)
for a, v in VT.items():
    for s, p in enumerate(v['slots']):
        if s: IMPL_USERS[p].add(a)

def canonical_slots(c):
    """(slots, kind): own vtable; for multi-vtable classes the abstract one (only table with __pure_virtual) or the
    shortest table that agrees with the per-slot majority; else the consensus (None where they disagree)"""
    vts = CLS[c]['vtables']
    if len(vts) == 1: return VT[vts[0]]['slots'], 'own'
    if not vts: return [], 'none'
    prim = [a for a in vts if not VT[a]['alt_getter']]
    if len(prim) == 1:
        CLS[c]['canon'] = prim[0]; return VT[prim[0]]['slots'], 'canonical %s (the table whose slot 6 is the registered getter; the others use a duplicate getter copy)' % h(prim[0])
    maj = {}
    for s in range(1, max(VT[a]['n'] for a in vts)):
        cnt = Counter(VT[a]['slots'][s] for a in vts if s < VT[a]['n'])
        p, k = cnt.most_common(1)[0]
        if k * 2 > len(vts): maj[s] = p
    agree = [a for a in vts if all(maj.get(s) == VT[a]['slots'][s] for s in range(1, VT[a]['n']))]
    if agree:
        a = min(agree, key=lambda a: VT[a]['n'])
        CLS[c]['canon'] = a; return VT[a]['slots'], 'canonical %s (shortest table agreeing with the majority of the %d tables in every slot)' % (h(a), len(vts))
    pv = [a for a in vts if any(fname(p) == '__pure_virtual' for p in VT[a]['slots'][1:])]
    if len(pv) == 1:
        CLS[c]['canon'] = pv[0]; return VT[pv[0]]['slots'], 'canonical %s (the only abstract table: has __pure_virtual slots)' % h(pv[0])
    n = min(VT[a]['n'] for a in vts)
    out = []
    for s in range(n):
        vals = {VT[a]['slots'][s] for a in vts}
        out.append(vals.pop() if len(vals) == 1 else None)
    return out, 'consensus of %d vtables' % len(vts)
for c in CLS: CLS[c]['slots'], CLS[c]['slots_kind'] = canonical_slots(c)

# slot roles: object-model.md (BASIC/LO), guards-ai.md (BHV); others derived from the introducing class's impl
ROLE_LO = {1: 'Incref-style hook', 2: 'Decref-style hook', 3: 'HandleSpliceEvent', 4: 'SendMessage',
           5: 'HandleMessage', 6: 'Pclassinfo', 7: 'Init', 8: 'Add', 9: 'Remove', 10: 'SetParent', 11: 'AddHierarchy',
           12: 'RemoveHierarchy', 13: 'UpdateLoroot', 14: 'LoadFromBrx', 15: 'PostLoad', 16: 'Update',
           17: '(Sly 1: ProjectTransform; LO: inherit flag 4)', 19: 'RenderSelf', 22: 'OnRemove', 23: 'OnAdd',
           25: 'SetIdProxy', 28: 'CploChildren', 29: 'TranslateToPos', 30: 'RotateToMat', 31: 'PmatWorld',
           33: 'GetVelocity', 34: 'GetAngularVelocity', 35: 'SetVelocity', 36: 'SetAngularVelocity',
           40: 'CloneHierarchy', 41: 'Clone', 46: 'SetVisMask', 47: 'SetGrf'}
ROLE_BHV = {7: 'Init', 8: 'Start', 9: 'Close', 10: 'Update', 11: 'UpdateMotion', 12: 'GrfGroundProbe',
            13: 'FHandleMessage', 14: 'UpdateInput', 15: 'SetState'}
ROLE_SRC_LO = 'object-model.md section 2'
def intro_class(c, s):
    """class in c's chain that introduced slot s (first ancestor, top-down, whose vtable has index s)"""
    for k in reversed(chain(c)):
        if len(CLS[k]['slots']) > s: return k
    return c
LO_CID = NAME2CID.get('LO', 2); BHV_CID = NAME2CID.get('BHV')
def role(c, s):
    ch = chain(c)
    if s <= 6: return ROLE_LO.get(s, ''), 'BASIC (object-model.md)'
    if LO_CID in ch and s in ROLE_LO: return ROLE_LO[s], ROLE_SRC_LO
    if BHV_CID is not None and BHV_CID in ch and s in ROLE_BHV: return ROLE_BHV[s], 'guards-ai.md section 3'
    k = intro_class(c, s); sl = CLS[k]['slots']
    p = sl[s] if s < len(sl) else None
    if p is not None:
        n = fname(p)
        if not n.startswith('func_') and n != '__pure_virtual':
            return n.rstrip('?'), 'name of %s\'s implementation' % CLS[k]['name']
        if n == '__pure_virtual':
            for d in subtree(k)[1:]:
                sd = CLS[d]['slots']
                if s < len(sd) and sd[s] is not None:
                    nd = fname(sd[s])
                    if not nd.startswith('func_') and nd != '__pure_virtual':
                        return nd.rstrip('?') + ' (pure in %s)' % CLS[k]['name'], 'name of %s\'s implementation' % CLS[d]['name']
    return '', ''
def owner_of_impl(c, s, p):
    """highest ancestor in c's chain whose slot s has the same impl p"""
    own = c
    for k in chain(c)[1:]:
        sl = CLS[k]['slots']
        if s < len(sl) and sl[s] == p: own = k
        else: break
    return own

# header virtual declarations (positional slot names) and fields --------------------------------------------
HDR_DIR = os.path.join(A.repo, 'include/sly2')
HSTRUCT = {}  # name -> dict(bases, virtuals, members{name: comment}, file, src_note)
ASSERTS = defaultdict(dict)  # struct -> {field or '__size__': value}
ASSERT_NOTES = defaultdict(dict)
for fn in sorted(os.listdir(HDR_DIR)) if os.path.isdir(HDR_DIR) else []:
    txt = open(os.path.join(HDR_DIR, fn)).read()
    for m in re.finditer(r'typedef char assert_\w+\[(?:sizeof\((\w+)\)|SLY2_OFFSETOF\((\w+),\s*([\w.]+)\))\s*==\s*(0x[0-9A-Fa-f]+|\d+)[^;]*;[ \t]*(?:/\*(.*?)\*/)?', txt):
        if m.group(1): ASSERTS[m.group(1)]['__size__'] = int(m.group(4), 0); ASSERT_NOTES[m.group(1)]['__size__'] = (m.group(5) or '').strip()
        else: ASSERTS[m.group(2)][m.group(3)] = int(m.group(4), 0); ASSERT_NOTES[m.group(2)][m.group(3)] = (m.group(5) or '').strip()
    for m in re.finditer(r'(?:/\*((?:(?!\*/).)*)\*/\s*)?^(struct|class|union) (\w+)(?: : ([^\n{]+))?\s*\n\{(.*?)\n\};', txt, re.S | re.M):
        note, kind, name, bases, body = m.groups()
        bl = [b.strip().split()[-1] for b in bases.split(',')] if bases else []
        virt = []; mem = {}; order = []
        for line in body.split('\n'):
            ls = line.strip()
            if ls.startswith('virtual'):
                vm = re.search(r'(\w+)\s*\(', ls)
                if vm: virt.append(vm.group(1))
                continue
            mm = re.match(r'(.*?)\b(\w+)\s*(?:\[[^\]]*\])*\s*(?::\s*\d+)?\s*;\s*(?:/\*(.*?)\*/)?', ls)
            if mm and not ls.startswith('//'):
                mem[mm.group(2)] = (mm.group(3) or '').strip(); order.append(mm.group(2))
        HSTRUCT[name] = dict(bases=bl, virtuals=virt, members=mem, order=order, file=fn,
                             note=re.sub(r'\s+', ' ', note or '').strip())
def header_slots(name, memo={}):
    """positional slot names from header virtual decls: base slots + new names, slot 1 first. None if no header."""
    if name in memo: return memo[name]
    h = HSTRUCT.get(name)
    if not h: memo[name] = None; return None
    base = []
    if h['bases']:
        b = header_slots(h['bases'][0])
        if b is None: memo[name] = None; return None
        base = list(b)
    else:
        base = [None]  # slot 0
    for v in h['virtuals']:
        if v not in base: base.append(v)
    memo[name] = base
    return base

def clang_layouts():
    if A.no_clang or not shutil.which('clang++') or not os.path.isdir(HDR_DIR): return {}, 'clang not run'
    tmp = tempfile.mkdtemp(prefix='gencls_')
    os.makedirs(os.path.join(tmp, 'sly2'))
    for f in ('common.h',):
        s = open(os.path.join(A.repo, 'include', f)).read()
        s = s.replace('typedef unsigned long ulong;', 'typedef unsigned long long ulong;')
        open(os.path.join(tmp, f), 'w').write(s)
    for fn in os.listdir(HDR_DIR):
        s = open(os.path.join(HDR_DIR, fn)).read()
        s = re.sub(r'typedef unsigned long long qword __attribute__\(\(mode\(TI\)\)\);',
                   'typedef struct { unsigned int w[4]; } __attribute__((aligned(16))) qword;', s)
        s = re.sub(r'(?<!long )\blong\b(?! long)', 'long long', s)   # EE: long is 64-bit, pointers 32-bit
        s = re.sub(r'typedef char assert_\w+\[[^;]*;', '', s)
        open(os.path.join(tmp, 'sly2', fn), 'w').write(s)
    src = ''.join('#include "sly2/%s"\n' % f for f in sorted(os.listdir(HDR_DIR)))
    src += 'int __sz[] = {' + ''.join('(int)sizeof(%s),' % n for n, h in HSTRUCT.items()
                                      if not n.startswith('CRefPtr') and n not in ('CGcObj',)) + '0};\n'
    open(os.path.join(tmp, 'all.cpp'), 'w').write(src)
    p = subprocess.run(['clang++', '--target=mipsel-linux-gnu', '-fsyntax-only', '-w', '-I', tmp, '-Xclang',
                        '-fdump-record-layouts', os.path.join(tmp, 'all.cpp')], capture_output=True, text=True)
    shutil.rmtree(tmp, ignore_errors=True)
    out = {}
    blocks = p.stdout.split('*** Dumping AST Record Layout')
    for b in blocks[1:]:
        lines = b.strip('\n').split('\n')
        hm = re.match(r'\s*0 \| (?:struct|class|union) (\S+)$', lines[0])
        if not hm: continue
        nm = hm.group(1); fields = []; vptr_own = False; has_base = False
        for l in lines[1:]:
            m = re.match(r'\s*(\d+)(?::(\d+)-(\d+))? \|( +)(.*)$', l)
            if not m:
                sm = re.search(r'sizeof=(\d+)', l)
                if sm: size = int(sm.group(1))
                continue
            depth = (len(m.group(4)) - 1) // 2
            if depth != 1: continue
            body = m.group(5)
            if '(primary base)' in body or '(base)' in body: has_base = True; continue
            if 'vtable pointer' in body: vptr_own = True; continue
            mm = re.match(r'(.*?)\s*(\w+)$', body)
            fields.append(dict(off=int(m.group(1)), bits=(int(m.group(2)), int(m.group(3))) if m.group(2) else None,
                               type=mm.group(1).strip() if mm else body, name=mm.group(2) if mm else body))
        sm = re.search(r'sizeof=(\d+)', b); size = int(sm.group(1)) if sm else None
        note = ''
        bm = re.search(r'\n\s*0 \|   (?:struct|class) (\w+) \((?:primary )?base\)', b)
        if bm and fields and bm.group(1) in out and out[bm.group(1)]['size']:
            # Itanium reuses a non-POD base's tail padding; EE GCC 2.9x never does: undo
            delta = out[bm.group(1)]['size'] - min(f['off'] for f in fields)
            if delta > 0:
                for f in fields: f['off'] += delta
                note = 'tail-padding reuse undone (+%d)' % delta
        if vptr_own and not has_base and fields:
            # EE GCC 2.9x puts the vptr after the first polymorphic class's own fields; clang puts it at 0.
            for f in fields: f['off'] -= 4
            note = 'vptr moved after own fields (EE GCC 2.9x rule)'
        elif vptr_own and has_base:
            note = 'polymorphic class over a non-polymorphic base: clang vptr placement differs; offsets unreliable'
        out[nm] = dict(fields=fields, size=size, note=note)
    return out, 'clang %s' % ('ok' if p.returncode == 0 else 'FAILED: ' + p.stderr[:200])
LAYOUT, LAYOUT_STATUS = clang_layouts()
def layout_check(nm):
    """(n_ok, n_bad, bad list) of header asserts vs computed layout"""
    L = LAYOUT.get(nm); ok = 0; bad = []
    if not L: return 0, 0, []
    offs = {f['name']: f['off'] for f in L['fields']}
    for k, v in ASSERTS.get(nm, {}).items():
        if k == '__size__': got = L['size']
        else:
            got = offs.get(k.split('.')[0])
            if '.' in k: continue
        if got is None: continue
        if got == v: ok += 1
        else: bad.append((k, v, got))
    return ok, len(bad), bad

# ----------------------------------------------------------------------------------------------- code references
GP = 0x2E5270
def stfmt(r):
    if r['store'] is None: return '?'
    if r['base'] == 28: return 'global %s' % h(GP + r['store'])
    return h(r['store']) if r['store'] < 0 else '+' + h(r['store'])
def scan_refs(targets):
    out = []
    for fs, fe, _ in FUNCS:
        if fe <= fs or not intext(fs): continue
        hi = {}; a = fs
        while a < fe:
            i = w32(a); op = i >> 26; rs = (i >> 21) & 31; rt = (i >> 16) & 31; imm = i & 0xFFFF
            simm = imm - 0x10000 if imm & 0x8000 else imm
            if op == 0x0F: hi[rt] = imm << 16
            elif op in (0x09, 0x0D) and rs in hi:
                v = (hi[rs] + simm) & 0xFFFFFFFF if op == 0x09 else hi[rs] | imm
                if v in targets:
                    st = None; sb = None
                    for k in range(1, 10):
                        b = a + 4 * k
                        if b >= fe: break
                        j = w32(b)
                        if j >> 26 == 0x2B and ((j >> 16) & 31) == rt:
                            st = j & 0xFFFF; st = st - 0x10000 if st & 0x8000 else st; sb = (j >> 21) & 31; break
                    out.append(dict(func=fs, at=a, target=v, store=st, base=sb))
                hi.pop(rt, None)
            a += 4
    return out
REFS = scan_refs(set(VT) | {CLS[c]['addr'] for c in CLS})
VT_REFS = defaultdict(list); F_VT = defaultdict(list)
for r in REFS:
    if r['target'] in VT: VT_REFS[r['target']].append(r); F_VT[r['func']].append(r)

# ----------------------------------------------------------------------------------------------- levels, docs, bhv
LEVELS = defaultdict(list)
if A.levels and os.path.exists(A.levels):
    for r in csv.DictReader(open(A.levels)):
        try: LEVELS[int(r['cid'])].append((r['level'], int(r['count_all']), r.get('identification', '')))
        except Exception: pass
BHVMAP = {}
if A.bhv and os.path.exists(A.bhv):
    for r in csv.DictReader(open(A.bhv)): BHVMAP[int(r['cid_dec'])] = r
DOCS = os.path.join(A.repo, 'docs')
DOCTXT = {}
for root, ds, fs in os.walk(DOCS):
    for f in fs:
        if f.endswith('.md'):
            p = os.path.join(root, f); rel = os.path.relpath(p, DOCS)
            if rel.startswith('reference/functions') or rel.startswith('reference/classes'): continue
            DOCTXT[rel] = open(p, errors='replace').read()
def doc_mentions(word, extra=()):
    words = [word] + [x for x in extra if x and x != word]
    hits = []
    for rel, t in DOCTXT.items():
        n = sum(len(re.findall(r'(?<![\w`])`?%s`?(?![\w])' % re.escape(wd), t)) for wd in words)
        if n: hits.append((n, rel))
    hits.sort(key=lambda x: (-x[0], x[1]))
    return hits

# ----------------------------------------------------------------------------------------------- mangled names
def parse_mangled(sym):
    """GCC 2.9x: returns (owner_class or None, [type names in params]) or None"""
    m = re.match(r'^(.*?[^_])__(.*)$', sym) or re.match(r'^()__(\d.*)$', sym)
    if sym.startswith('_$_'):
        m2 = re.match(r'_\$_(\d+)(\w+)', sym);
        if m2: return m2.group(2)[:int(m2.group(1))], []
    if not m: return None
    rest = m.group(2); owner = None; types = []; i = 0
    def read_name(i):
        j = i
        while j < len(rest) and rest[j].isdigit(): j += 1
        if j == i: return None, i
        n = int(rest[i:j]); return rest[j:j + n], j + n
    if rest and rest[0].isdigit():
        owner, i = read_name(0)
        if owner is None: return None
    elif rest.startswith('Q'):
        pass
    elif rest.startswith('F'): i = 1
    else: return None
    while i < len(rest):
        ch = rest[i]
        if ch == 'Q' and i + 1 < len(rest) and rest[i + 1].isdigit():
            k = int(rest[i + 1]); i += 2
            for _ in range(k):
                n, i = read_name(i)
                if n: types.append(n)
        elif ch in 'TN' and i + 1 < len(rest) and rest[i + 1].isdigit():
            i += 2 if ch == 'T' else 3
        elif ch.isdigit():
            n, i = read_name(i)
            if n: types.append(n)
            else: i += 1
        elif ch == 't': return owner, types  # templates: stop
        else: i += 1
    return owner, types
MANGLED = {}  # sym -> (addr, source)
for a, n in SYM.items():
    if '__' in n: MANGLED[n] = (a, 'symbol_addrs')
for a, r in MATCHED.items():
    if '__' in r['name'] and r['name'] not in MANGLED: MANGLED[r['name']] = (a, 'matched.csv')
for a, r in MASTER.items():
    mn = r.get('mangled_or_blank') or ''
    if mn and mn not in MANGLED: MANGLED[mn] = (a, 'master_names (proposal)')
TYPEUSE = defaultdict(lambda: dict(owner=[], param=[]))
for sym, (a, src) in MANGLED.items():
    pm = parse_mangled(sym)
    if not pm: continue
    owner, types = pm
    if owner: TYPEUSE[owner]['owner'].append((sym, a, src))
    for t in set(types): TYPEUSE[t]['param'].append((sym, a, src))

SLY1_TYPES = Counter()
p1s = os.path.join(A.sly1 or '', 'config/symbol_addrs.txt')
if A.sly1 and os.path.exists(p1s):
    for a, n in read_symbols(p1s).items():
        if '__' in n:
            pm = parse_mangled(n)
            if pm:
                if pm[0]: SLY1_TYPES[pm[0]] += 1
                for t in set(pm[1]): SLY1_TYPES[t] += 1
SLY2_NAMES_IN_HDR = set()
# ----------------------------------------------------------------------------------------------- output helpers
OUT = A.out
CDIR = os.path.join(OUT, 'docs/reference/classes'); os.makedirs(CDIR, exist_ok=True)
DDIR = os.path.join(OUT, 'data'); os.makedirs(DDIR, exist_ok=True)
for f in os.listdir(CDIR):
    if f.endswith('.md'): os.remove(os.path.join(CDIR, f))
def clink(c, frm='classes'):
    n = CLS[c]['name']
    return '[%s](%s.md)' % (n, n) if frm == 'classes' else '[%s](classes/%s.md)' % (n, n)
def flink(a, frm_depth=3):
    """function name with a link to its source file in the repo, when matched.csv knows it"""
    n = fname(a); s = fsrc(a)
    if s: return '[`%s`](%s%s)' % (n, '../' * frm_depth, s)
    return '`%s`' % n
def esc(s): return (s or '').replace('|', '\\|').replace('\n', ' ')
def sizestr(c):
    cb = CLS[c]['cb']; p = CLS[c]['parent']
    return '%s (%d bytes)%s' % (h(cb), cb, '' if p is None else ', +%s over %s' % (h(cb - CLS[p]['cb']), CLS[p]['name']))

# ----------------------------------------------------------------------------------------------- forensics
def generic(p, fam):
    """impl used by vtables outside the vtable set fam"""
    return bool(IMPL_USERS[p] - fam)
FOR_A = []   # dict(parent, members, slots[(s, impl)], exclusive_count, kind)
for P in CLS:
    kids = [k for k in CLS[P]['children'] if len(CLS[k]['vtables']) == 1]
    if len(kids) < 2: continue
    ps = CLS[P]['slots']
    groups = defaultdict(list)  # frozenset(kids) -> [(s, p)]
    maxn = max(len(CLS[k]['slots']) for k in kids)
    for s in range(1, maxn):
        if s == 6: continue
        byimpl = defaultdict(list)
        for k in kids:
            sl = CLS[k]['slots']
            if s < len(ps) and ps[s] is None: continue
            if s < len(sl) and (s >= len(ps) or sl[s] != ps[s]): byimpl[sl[s]].append(k)
        for p, ks in byimpl.items():
            if len(ks) >= 2:
                # include the descendants of ks in the family for the exclusivity test
                groups[frozenset(ks)].append((s, p))
    for ks, sl in groups.items():
        fam = set()
        for k in ks:
            for d in subtree(k): fam |= set(CLS[d]['vtables'])
        excl = [(s, p) for s, p in sl if not generic(p, fam)]
        new = [(s, p) for s, p in sl if s >= len(ps)]
        FOR_A.append(dict(parent=P, members=sorted(ks), slots=sl, excl=excl, new=new,
                          allkids=(len(ks) == len(kids))))
FOR_A.sort(key=lambda g: (-len(g['excl']), -len(g['slots']), g['parent']))
def score_a(g):
    if len(g['excl']) >= 3 or len(g['new']) >= 1 and len(g['excl']) >= 2: return 'strong'
    if len(g['excl']) >= 1: return 'medium'
    return 'weak'

# (b) unclaimed vtables
UNCL = [a for a in sorted(VT) if VT[a]['cid'] is None]
def nearest_registered(a):
    sl = VT[a]['slots']; best = (0, None)
    for b, v in VT.items():
        if v['cid'] is None: continue
        same = sum(1 for s in range(1, min(len(sl), v['n'])) if v['slots'][s] == sl[s])
        if same > best[0]: best = (same, b)
    return best
def owner_words(a):
    cnt = Counter()
    for p in VT[a]['slots'][1:]:
        n = fname(p).rstrip('?')
        pm = parse_mangled(n) if '__' in n else None
        if pm and pm[0]: cnt[pm[0]] += 3; continue
        if pm and pm[1]: cnt[pm[1][0]] += 2; continue
        m = re.match(r'([A-Z][A-Z0-9]+)_', n) or re.match(r'C([A-Z][a-z]+\w*?)_', n)
        if m: cnt[m.group(1)] += 1
        else:
            m = re.match(r'(?:Init|Post|Update|Render|Project|Touch|Activate|Deactivate|Reset|Load|Evaluate|Clone|Apply|Get|On)([A-Z][a-z0-9]+)', n)
            if m: cnt[m.group(1).upper()] += 1
    return cnt.most_common(3)
# families of unclaimed vtables: connected by shared non-trivial impls
def families(addrs):
    par = {a: a for a in addrs}
    def f(x):
        while par[x] != x: par[x] = par[par[x]]; x = par[x]
        return x
    byimpl = defaultdict(list)
    for a in addrs:
        for p in set(VT[a]['slots'][1:]):
            if fname(p) != '__pure_virtual': byimpl[p].append(a)
    for p, L in byimpl.items():
        if len(IMPL_USERS[p]) > 20: continue
        for b in L[1:]: par[f(b)] = f(L[0])
    byfunc = defaultdict(list)
    for a in addrs:
        for r in VT_REFS.get(a, []): byfunc[r['func']].append(a)
    for fn_, L in byfunc.items():
        for b in L[1:]: par[f(b)] = f(L[0])
    g = defaultdict(list)
    for a in addrs: g[f(a)].append(a)
    return sorted(g.values(), key=lambda L: L[0])
UFAM = families(UNCL)
MULTI = [c for c in CLS if len(CLS[c]['vtables']) > 1]

# descriptor-like tables elsewhere
def scan_desc_like():
    found = []
    for sname in ('.rodata', '.data'):
        lo, hi = SEC[sname]; a = (lo + 3) & ~3
        while a < hi - 0x14 * 3:
            run = 0; b = a
            while b < hi - 0x14:
                sup, x, y, cb, pfn = struct.unpack_from('<5I', D, off(b))
                okrec = (sup == 0 or lo <= sup < hi) and x < 0x400 and x < y <= 0x400 and 0 < cb < 0x10000 \
                        and cb % 4 == 0 and (pfn == 0 or intext(pfn))
                if not okrec: break
                run += 1; b += 0x14
            if run >= 3 and not (DESC_TABLE <= a < DESC_TABLE + DESC_STRIDE * NCLS):
                found.append((a, run)); a = b
            else: a += 4
    return found
DESC_LIKE = scan_desc_like()

# (d) functions writing >1 vtable
FOR_D = []
for f, L in F_VT.items():
    vts = []
    for r in sorted(L, key=lambda r: r['at']):
        if not vts or vts[-1]['target'] != r['target']: vts.append(r)
    if len({r['target'] for r in vts}) >= 2: FOR_D.append((f, vts))
FOR_D.sort()

# (e) Sly 1
SLY1_CID = {}; SLY1_PAR = {}
p1 = os.path.join(A.sly1 or '', 'include')
if A.sly1 and os.path.isdir(p1):
    t = open(os.path.join(p1, 'cid.h')).read() if os.path.exists(os.path.join(p1, 'cid.h')) else ''
    for l in t.split('\n'):
        m = re.match(r'\s*CID_(\w+)\s*=\s*(\d+)\s*,?\s*(?://\s*(.*))?$', l)
        if m and m.group(1) != 'Nil': SLY1_CID[m.group(1)] = (int(m.group(2)), (m.group(3) or '').strip())
    for fn in os.listdir(p1):
        if fn.endswith('.h'):
            for m in re.finditer(r'^(?:struct|class) (\w+) : public (\w+)', open(os.path.join(p1, fn), errors='replace').read(), re.M):
                SLY1_PAR[m.group(1)] = (m.group(2), fn)
SLY1_NOTES = {  # hand-curated, with source
    'BLOT': 'Sly 2 HUD is the registered WIDGET family (class-tree.md: "WIDGET (Sly 1\'s BLOT HUD family)")',
    'CTR': 'counters are WIDGET subclasses in Sly 2 (class-tree.md)',
    'ACG': 'Sly 2 animation curves are the AC* vtable family (B2), named differently',
    'ACP': 'see AC* vtable family (B2)', 'ACR': 'see AC* vtable family (B2)', 'ACS': 'see AC* vtable family (B2)',
}
def sly1_chain(n):
    out = []
    while n in SLY1_PAR and n not in out: n = SLY1_PAR[n][0]; out.append(n)
    return out

# ----------------------------------------------------------------------------------------------- class pages
HIDDEN_NOTES = defaultdict(list)
for i, g in enumerate(FOR_A):
    if score_a(g) == 'weak': continue
    for k in g['members'] + [g['parent']]:
        HIDDEN_NOTES[k].append('candidate unregistered intermediate under %s shared by %s (%s, %d exclusive identical slots) — [hidden-types A%d](../hidden-types.md#a%d)'
                               % (CLS[g['parent']]['name'], ', '.join(CLS[m]['name'] for m in g['members']), score_a(g), len(g['excl']), i + 1, i + 1))
for c in MULTI:
    HIDDEN_NOTES[c].append('%d vtables report this class from slot 6 (unregistered subclasses that do not override Pclassinfo) — [hidden-types B](../hidden-types.md#b-vtables-and-descriptor-like-tables-not-claimed-by-a-registered-class)' % len(CLS[c]['vtables']))

ALLROWS = []
def class_page(c):
    C = CLS[c]; n = C['name']; L = []
    sl = C['slots']; P = C['parent']; ps = CLS[P]['slots'] if P is not None else []
    L.append('# %s (CID %d, 0x%02X)\n' % (n, c, c))
    ident = TREE_ID.get(c, {}).get('ident') or (V3.get(c, {}).get('evidence', '') if c in V3 else '')
    L.append('[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)\n')
    if ident: L.append('> %s\n' % esc(ident))
    L.append('| | |\n|---|---|')
    L.append('| Name | `%s` — %s |' % (n, name_conf(c)))
    vn = v3_name(c)
    if vn and vn != n: L.append('| Proposed name (master_names) | `%s` (not yet in the repo getter name) |' % vn)
    ch = chain(c)
    L.append('| Parent chain | %s |' % ' → '.join(clink(k) for k in ch))
    kids = C['children']
    L.append('| Children | %s |' % (', '.join(clink(k) for k in kids) if kids else '(none)'))
    st = subtree(c)
    if len(st) > 1 + len(kids): L.append('| Descendants | %d classes (CID %d..%d) |' % (len(st) - 1, c + 1, C['cidmax'] - 1))
    L.append('| Instance size (`cb`) | %s |' % sizestr(c))
    L.append('| Descriptor | `D_%08X` (record %d of D_%08X; cid range %d..%d) |' % (C['addr'], c, DESC_TABLE, c, C['cidmax']))
    L.append('| Factory (`pfnNew`) | %s |' % (flink(C['pfnNew']) + ' `%s`' % h(C['pfnNew']) if C['pfnNew'] else '(none: not created by PloNew)'))
    L.append('| Descriptor getter (slot 6) | `%s` `%s`%s |' % (C['getter_name'], h(C['getter']), '' if C['getter_ok'] else ' (getter does not load this record!)'))
    vts = C['vtables']
    if len(vts) == 1:
        v = VT[vts[0]]
        newc = sum(1 for s in range(len(ps), len(sl))) if P is not None else len(sl) - 1
        ovc = sum(1 for s in range(1, min(len(ps), len(sl))) if sl[s] != ps[s])
        L.append('| vtable | `D_%08X`, %d entries (slots 0..%d; slot 0 null), %d new, %d overridden |' % (vts[0], v['n'], v['n'] - 1, newc, ovc))
        refs = VT_REFS.get(vts[0], [])
        if refs:
            L.append('| vtable stored by | %s |' % ', '.join(sorted({'%s (vptr %s)' % (flink(r['func']), stfmt(r)) if r['store'] is not None else flink(r['func']) for r in refs})))
    elif vts:
        L.append('| vtables | %d tables report this class: %s (see [hidden types B1](../hidden-types.md#b1-registered-getters-returned-by-more-than-one-vtable)). Slot table below: %s |' % (len(vts), ', '.join('`D_%08X` (%d)' % (a, VT[a]['n']) for a in vts), C['slots_kind']))
    else:
        L.append('| vtable | none found |')
    lv = LEVELS.get(c, [])
    tot = sum(x[1] for x in lv)
    L.append('| Level instances | %s |' % ('%d objects in %d of 43 world files' % (tot, len(lv)) if lv else 'none (not a world-file object class)'))
    L.append('')
    for note in HIDDEN_NOTES.get(c, []): L.append('- **Hidden-type note (HYPOTHESIS):** ' + note)
    if HIDDEN_NOTES.get(c): L.append('')
    if c in BHVMAP:
        b = BHVMAP[c]
        L.append('**Behaviour map (r9):** placeholder `%s`, proposed `%s` (confidence %s), guard flag `%s`. %s\n' %
                 (b['placeholder'], b['proposed_class'], b['confidence'], b['guard_flag'], esc(b['role'])))
    # vtable
    if sl:
        L.append('## Virtual table\n')
        L.append('Status: **new** = slot first appears in this class; **override** = differs from the parent; '
                 'inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.\n')
        hs = header_slots(n)
        hs_ok = hs is not None and len(hs) == len(sl)
        L.append('| Slot | Role | Status | Implementation | Address |%s' % (' Header decl |' if hs_ok else ''))
        L.append('|---|---|---|---|---|%s' % ('---|' if hs_ok else ''))
        for s in range(1, len(sl)):
            p = sl[s]
            r, rsrc = role(c, s)
            if p is None: L.append('| %d | %s | varies | (differs between the %d vtables) | |%s' % (s, esc(r), len(vts), ' |' if hs_ok else '')); continue
            if P is None or s >= len(ps): stt = '**new**'
            elif ps[s] == p: stt = 'inherited (%s)' % CLS[owner_of_impl(c, s, p)]['name']
            else:
                anc = [k for k in ch[2:] if s < len(CLS[k]['slots']) and CLS[k]['slots'][s] == p]
                stt = '**override**' + (' (= %s impl)' % CLS[anc[0]]['name'] if anc else '')
            impl = flink(p) if stt.startswith('**') else '`%s`' % fname(p)
            L.append('| %d | %s | %s | %s | `%s` |%s' % (s, esc(r), stt, impl, h(p), (' %s |' % (hs[s] or '') if hs_ok else '')))
        if hs is not None and not hs_ok:
            L.append('\nHeader `include/sly2/%s` declares %d virtual slots for `%s`, the ELF vtable has %d: header virtual order not used.'
                     % (HSTRUCT[n]['file'], len(hs) - 1, n, len(sl) - 1))
        L.append('')
    # fields
    L.append('## Fields\n')
    hsx = HSTRUCT.get(n)
    if hsx:
        lay = LAYOUT.get(n); okc, badc, bad = layout_check(n)
        L.append('From `include/sly2/%s` (`struct %s%s`). %s' % (hsx['file'], n, ' : ' + ', '.join(hsx['bases']) if hsx['bases'] else '',
                 esc(hsx['note'])))
        if lay:
            sz = lay['size']
            L.append('\nOffsets computed by clang from the header (%s); %d header asserts agree, %d disagree.%s Size from header: %s%s.\n'
                     % ('EE ABI: 32-bit pointers, 64-bit long, 16-byte qword' + ('; ' + lay['note'] if lay['note'] else ''), okc, badc,
                        (' Disagreements: ' + ', '.join('%s header-assert %s vs computed %s' % (k, h(v), h(g)) for k, v, g in bad)) if bad else '',
                        h(sz) if sz else '?', '' if sz == C['cb'] else ' (**differs from descriptor cb %s**)' % h(C['cb'])))
            L.append('| Offset | Type | Field | Confidence | Note |\n|---|---|---|---|---|')
            rows = list(lay['fields'])
            if lay['note'].startswith('vptr moved') and rows:
                rows.append(dict(off=max(f['off'] for f in rows) + 4, bits=None, type='vtable *', name='(vptr)', vptr=True))
            for f in rows:
                if f.get('vptr'):
                    L.append('| %s | `vtable *` | (vptr) | computed (EE GCC 2.9x places the vptr after the first polymorphic class\'s own fields) | 8-byte entries; slot 6 = Pclassinfo |' % h(f['off'])); continue
                fo = h(f['off']) + ('' if not f['bits'] else ' bits %d-%d' % f['bits'])
                asrt = ASSERTS.get(n, {}).get(f['name'])
                if asrt is not None and asrt == f['off']: conf = 'asserted (%s)' % esc(ASSERT_NOTES[n].get(f['name'], '') or 'compile-time')
                elif asrt is not None: conf = '**assert %s disagrees**' % h(asrt)
                elif f['name'].startswith('unk') or 'Unk' in f['name'] or f['name'].startswith('pad'): conf = 'unknown (padding/placeholder)'
                else: conf = 'header (Lilly-ported; offset computed, unasserted)'
                L.append('| %s | `%s` | `%s` | %s | %s |' % (fo, esc(f['type']), f['name'], conf, esc(hsx['members'].get(f['name'], ''))))
        else:
            L.append('\n(Layout not computed: %s.) Members in declaration order: %s' % (LAYOUT_STATUS, ', '.join('`%s`' % m for m in hsx['order'])))
        if P is not None: L.append('\nInherited fields: see %s.' % clink(P))
    else:
        L.append('No struct for `%s` in `include/sly2` yet. Size is known exactly (%s); the bytes %s..%s are this class\'s own.%s'
                 % (n, h(C['cb']), h(CLS[P]['cb']) if P is not None else '0', h(C['cb']),
                    (' Inherited fields: see %s.' % clink(P)) if P is not None else ''))
    L.append('')
    # levels
    if lv:
        L.append('## Level instances\n')
        L.append('From the whole-game object census (every object record of all 43 world files).\n')
        L.append('| Level | Objects |\n|---|---|')
        for lvl, cnt, _ in sorted(lv, key=lambda x: x[0]): L.append('| %s | %d |' % (lvl, cnt))
        L.append('')
    # docs
    extra = [vn] if vn and vn != n else []
    dm = doc_mentions(n, extra)
    L.append('## Related docs\n')
    if dm:
        L.append(', '.join('[%s](../../%s) (%d)' % (rel, rel, k) for k, rel in dm[:8]) + '\n')
    else: L.append('(no mentions of `%s` in docs/)\n' % n)
    L.append('---\n*Generated by `gen_classes.py` from the ELF (descriptor table D_%08X, vtables), the repo names and headers. Re-run after renames.*' % DESC_TABLE)
    open(os.path.join(CDIR, n + '.md'), 'w').write('\n'.join(L) + '\n')
    ALLROWS.append(dict(cid=c, name=n, v3=vn or '', name_conf=name_conf(c), parent=CLS[P]['name'] if P is not None else '',
                        cb=h(C['cb']), vtables='/'.join(h(a) for a in vts), nslots=len(sl),
                        new=(len(sl) - len(ps)) if (P is not None and len(vts) == 1) else '',
                        overrides=sum(1 for s in range(1, min(len(ps), len(sl))) if sl[s] is not None and sl[s] != ps[s]) if P is not None else '',
                        factory=fname(C['pfnNew']) if C['pfnNew'] else '', levels=len(lv), objects=tot,
                        header='include/sly2/' + hsx['file'] if hsx else ''))
for c in range(NCLS): class_page(c)
with open(os.path.join(DDIR, 'classes.csv'), 'w', newline='') as f:
    wr = csv.DictWriter(f, fieldnames=list(ALLROWS[0])); wr.writeheader(); wr.writerows(ALLROWS)
with open(os.path.join(DDIR, 'layout_checks.csv'), 'w', newline='') as f:
    wr = csv.writer(f); wr.writerow(['struct', 'header', 'computed_size', 'asserts_ok', 'asserts_bad', 'bad_detail', 'note'])
    for nm in sorted(HSTRUCT):
        okc, badc, bad = layout_check(nm); L0 = LAYOUT.get(nm) or {}
        wr.writerow([nm, HSTRUCT[nm]['file'], h(L0['size']) if L0.get('size') else '', okc, badc,
                     ' '.join('%s:%s/%s' % (k, h(v), h(g)) for k, v, g in bad), L0.get('note', '')])
with open(os.path.join(DDIR, 'vtables.csv'), 'w', newline='') as f:
    wr = csv.writer(f); wr.writerow(['vtable', 'entries', 'class_by_slot6', 'stored_by', 'vptr_offsets'])
    for a in sorted(VT):
        v = VT[a]; refs = VT_REFS.get(a, [])
        wr.writerow([h(a), v['n'], CLS[v['cid']]['name'] if v['cid'] is not None else '',
                     ' '.join(sorted({fname(r['func']) for r in refs})), ' '.join(sorted({str(r['store']) for r in refs if r['store'] is not None}))])
with open(os.path.join(DDIR, 'vtable_slots.csv'), 'w', newline='') as f:
    wr = csv.writer(f); wr.writerow(['vtable', 'class', 'slot', 'impl', 'name'])
    for a in sorted(VT):
        v = VT[a]
        for s, p in enumerate(v['slots']):
            if s: wr.writerow([h(a), CLS[v['cid']]['name'] if v['cid'] is not None else '', s, h(p), fname(p)])

# ----------------------------------------------------------------------------------------------- index.md
def mermaid(root, maxdepth=99, stop=(), title=None, only=None):
    out = ['```mermaid', 'graph LR']
    def node(c):
        n = CLS[c]['name']; return '  c%d["%s<br/>%s"]' % (c, n, h(CLS[c]['cb']))
    seen = set()
    def walk(c, d):
        seen.add(c); out.append(node(c))
        if d >= maxdepth or (c in stop and c != root):
            if CLS[c]['children']: out.append('  c%d -.-> c%dmore(["+%d more"])' % (c, c, len(subtree(c)) - 1))
            return
        for k in CLS[c]['children']:
            if only and k not in only: continue
            walk(k, d + 1); out.append('  c%d --> c%d' % (c, k))
    walk(root, 0)
    for c in seen: out.append('  click c%d "%s.md"' % (c, CLS[c]['name']))
    out.append('```')
    return '\n'.join(out)
I = []
I.append('# Class reference\n')
I.append('Doxygen-style reference for every class Sly 2 registers in its class-descriptor table, plus the C++ classes and '
         'polymorphic structs that live outside it. Structure (hierarchy, sizes, vtables, overrides) is read from the retail '
         'ELF `SCUS_973.16` and is exact `[C:static, ELF data]`; names come from the repo and carry the confidence shown on '
         'each page. Generated by `gen_classes.py` (re-run after renames). See also [object model](../../engine/object-model.md), '
         '[class tree](../class-tree.md) and [hidden types](../hidden-types.md).\n')
I.append('## How the data was obtained\n')
nvt = len(VT); ncl = sum(1 for a in VT if VT[a]['cid'] is not None)
I.append('- **Descriptors.** `D_%08X` holds %d records of 0x14 bytes `{CLASSINFO *pclassinfoSuper; CID cid; CID cidMax; int cb; LO *(*pfnNew)();}`. '
         'CIDs are a pre-order numbering: `cidMax` is one past the last descendant. All %d getters `Get<Cls>ClassDesc` (0x%X + 0x10*cid) '
         'were decoded and %s.' % (DESC_TABLE, NCLS, NCLS, GETTER0, 'each loads its own record' if all(CLS[c]['getter_ok'] for c in CLS) else 'SOME DO NOT load their record'))
I.append('- **Vtables.** The ELF data sections were scanned for GCC 2.9x vtables: an all-zero entry 0 followed by 8-byte entries '
         '`{s16 delta; s16 index(0); pfn}` pointing into `.text`. %d vtables were found; %d of them are claimed by a registered class '
         'because slot 6 (`Pclassinfo`) points at that class\'s getter. Every delta is %s, so there is no multiple-inheritance this-adjustment anywhere.'
         % (nvt, ncl, 'zero' if all(all(x == 0 for x in v['deltas']) for v in VT.values()) else 'NOT zero'))
I.append('- **Slot numbering** is the entry index from the vtable start (entry 0 null), as in the object model page. '
         'Older tables (`cid_table.csv`, class-tree.md "Slots") count entries 1..N, so LO\'s "48 slots" = indices 1..48.')
I.append('- **Overrides** compare each slot with the parent\'s vtable; inherited slots name the highest ancestor that owns the same function.')
I.append('- **Fields** come from `include/sly2`; offsets are computed by clang with the EE data model and checked against the headers\' own '
         'compile-time asserts (asserted fields are marked). Layout status: %s.' % LAYOUT_STATUS)
hdr_cmp = [(CLS[c]['name'], len(header_slots(CLS[c]['name'])) == len(CLS[c]['slots'])) for c in CLS if header_slots(CLS[c]['name']) is not None and CLS[c]['slots']]
I.append('- **Header cross-check.** For the %d registered classes with a struct in `include/sly2`, the number of virtual slots implied by the '
         'header\'s `virtual` declarations (GCC order: base slots, then new names) matches the ELF vtable length for %d%s.'
         % (len(hdr_cmp), sum(ok for _, ok in hdr_cmp), '' if all(ok for _, ok in hdr_cmp) else ' (mismatches: ' + ', '.join(n for n, ok in hdr_cmp if not ok) + ')'))
I.append('- **Level instances** come from the whole-game census of all object records in the 43 world files.\n')
# summary
I.append('## Subtrees\n')
roots = CLS[0]['children']
I.append('BASIC has %d direct children: %s.\n' % (len(roots), ', '.join(clink(k) for k in roots)))
I.append('### Top level (BASIC and LO, two levels deep)\n')
I.append(mermaid(0, maxdepth=2))
X = NAME2CID.get('XFM'); SO = NAME2CID.get('SO'); PO = NAME2CID.get('PO'); STEP = NAME2CID.get('STEP')
SG = NAME2CID.get('STEPGUARD'); SP = NAME2CID.get('STEPPLAYER')
sec = []
if X is not None: sec.append(('XFM: positioned lightweights (lights, proxies, shapes, volumes, emitters, ...)', X, dict()))
if SO is not None: sec.append(('SO: physical objects (SO, PO, gadgets, pickups, ...); PO subtree collapsed', SO, dict(stop=(PO,) if PO else ())))
if PO is not None: sec.append(('PO: controllable objects → STEP → STEPPLAYER / STEPGUARD', PO, dict()))
for title, r, kw in sec:
    I.append('\n### %s\n' % title); I.append(mermaid(r, **kw))
# LO direct non-XFM/SO children
lo_other = [k for k in CLS[LO_CID]['children'] if k not in (X, SO)]
I.append('\n### LO children that are not XFM or SO (animation, state machines, scripted sequences, widgets/HUD, jobs)\n')
out = ['```mermaid', 'graph LR', '  c%d["LO"]' % LO_CID]
for k in lo_other:
    out.append('  c%d["%s<br/>%s"]' % (k, CLS[k]['name'], h(CLS[k]['cb']))); out.append('  c%d --> c%d' % (LO_CID, k))
    for g in CLS[k]['children']:
        out.append('  c%d["%s<br/>%s"]' % (g, CLS[g]['name'], h(CLS[g]['cb']))); out.append('  c%d --> c%d' % (k, g))
for k in [LO_CID] + lo_other + [g for k in lo_other for g in CLS[k]['children']]: out.append('  click c%d "%s.md"' % (k, CLS[k]['name']))
out.append('```'); I.append('\n'.join(out))
others = [k for k in roots if k != LO_CID]
I.append('\n### BASIC children that are not LO (world, camera, animation/shader helpers, sound, behaviours)\n')
out = ['```mermaid', 'graph LR', '  c0["BASIC"]']
for k in others:
    if CLS[k]['name'] == 'BHV' or len(subtree(k)) > 12:
        out.append('  c%d["%s<br/>%s"]' % (k, CLS[k]['name'], h(CLS[k]['cb']))); out.append('  c0 --> c%d' % k)
        out.append('  c%d -.-> c%dmore(["+%d subclasses"])' % (k, k, len(subtree(k)) - 1)); continue
    for d in subtree(k):
        out.append('  c%d["%s<br/>%s"]' % (d, CLS[d]['name'], h(CLS[d]['cb'])))
        out.append('  c%d --> c%d' % (CLS[d]['parent'], d))
for k in others:
    for d in ([k] if (CLS[k]['name'] == 'BHV' or len(subtree(k)) > 12) else subtree(k)): out.append('  click c%d "%s.md"' % (d, CLS[d]['name']))
out.append('```'); I.append('\n'.join(out))
if BHV_CID is not None:
    I.append('\n### BHV: guard and player behaviours\n'); I.append(mermaid(BHV_CID))
# full table
I.append('\n## All registered classes\n')
I.append('Name confidence: **high** = Sucker Punch name (Sly 1 / Lilly symbol); **medium/low** = team coinage; **placeholder** = `CIDnn`/`BHVnnn` (decimal CID). '
         'New / Ovr = vtable slots added / overridden relative to the parent.\n')
I.append('| CID | Class | Parent | Size | vtable | Slots | New | Ovr | Levels (objects) | Name confidence |\n|---|---|---|---|---|---|---|---|---|---|')
for r in ALLROWS:
    c = r['cid']; depth = len(chain(c)) - 1
    I.append('| %d (0x%02X) | %s%s | %s | %s | %s | %s | %s | %s | %s | %s |' % (
        c, c, '&nbsp;&nbsp;' * depth, clink(c), clink(CLS[c]['parent']) if CLS[c]['parent'] is not None else '', r['cb'],
        ' '.join('`%s`' % x for x in r['vtables'].split('/')) if r['vtables'] else '', r['nslots'] - 1 if r['nslots'] else '',
        r['new'], r['overrides'], ('%d (%d)' % (r['levels'], r['objects'])) if r['levels'] else '', r['name_conf'].split(':')[0]))
# non-registry
I.append('\n## Types outside the registry\n')
I.append('### Classes owning mangled methods (C++ classes; registered names in bold)\n')
I.append('The retail ELF is stripped: every mangled name in this repo was assigned by the decomp team, mostly by carrying a Sly 1 '
         'symbol (Sucker Punch\'s real spelling, from the Sly 1 prototype) over to the matching Sly 2 function. A class word here is therefore '
         'Sucker Punch\'s name **for Sly 1**; for Sly 2 it is as strong as the signature check (see naming-workflow: 14 of 77 carried Sly 1 '
         'signatures contradicted Sly 2\'s arguments). *(proposal)* = only in master_names. **Sly 1 syms** = occurrences in the Sly 1 decomp\'s symbols.\n')
I.append('| Class | Methods (owner) | Used as parameter | Sly 1 syms | vtable(s) | Source of evidence |\n|---|---|---|---|---|---|')
regnames = set(NAME2CID) | {v3_name(c) for c in CLS if v3_name(c)}
owners = sorted(k for k, v in TYPEUSE.items() if v['owner'])
def vts_for_owner(k):
    out = set()
    for sym, a, src in TYPEUSE[k]['owner']:
        for va in IMPL_USERS.get(a, ()):
            if VT[va]['cid'] is None: out.add(va)
    return sorted(out)
for k in owners:
    v = TYPEUSE[k]; srcs = {s for _, _, s in v['owner'] + v['param']}
    vv = vts_for_owner(k)
    I.append('| %s | %d | %d | %d | %s | %s |' % (('**`%s`**' if k in regnames else '`%s`') % k, len(v['owner']), len(v['param']), SLY1_TYPES.get(k, 0), ' '.join('`%s`' % h(a) for a in vv),
             ', '.join(sorted(srcs)).replace('master_names (proposal)', '*(proposal)*')))
I.append('\nParameter-only type names (structs and classes that never own a mangled method), with how many symbols use them. '
         'Registered class names are in **bold**:\n')
pt = sorted((k for k, v in TYPEUSE.items() if not v['owner']), key=lambda k: (-len(TYPEUSE[k]['param']), k))
I.append(', '.join(('**`%s`**' if k in regnames else '`%s`') % k + ' (%d)' % len(TYPEUSE[k]['param']) for k in pt) + '\n')
I.append('### Polymorphic families without a descriptor (vtables found in the ELF)\n')
I.append('Grouped by shared implementations. Owner words are guessed from the slot functions\' names; the vptr offset is where the '
         'storing code writes the vtable (`+0xC` is the BASIC layout). Details: [hidden types B](../hidden-types.md#b-vtables-and-descriptor-like-tables-not-claimed-by-a-registered-class).\n')
I.append('| Family | vtables | Owner words (from slot names) | Stored by (vptr offset) |\n|---|---|---|---|')
for fam in UFAM:
    words = Counter()
    for a in fam:
        for wd, k in owner_words(a): words[wd] += k
    stb = sorted({'%s (%s)' % (fname(r['func']), stfmt(r)) if r['store'] is not None else fname(r['func']) for a in fam for r in VT_REFS.get(a, [])})
    I.append('| %s | %s | %s | %s |' % (', '.join(w for w, _ in words.most_common(3)) or '?', ' '.join('`%s`(%d)' % (h(a), VT[a]['n']) for a in fam),
             ', '.join('%s×%d' % x for x in words.most_common(4)), esc(', '.join(stb[:6]) + (' …' if len(stb) > 6 else '')) or '(no code reference found)'))
if BHVMAP:
    I.append('\n### The behaviour family (BHV, CIDs %d..%d)\n' % (BHV_CID, CLS[BHV_CID]['cidmax'] - 1))
    I.append('BHVs are registered classes (BASIC → BHV) but are not world objects: `PbhvNew` placement-constructs them into the '
             '8-entry `BHVSTK` embedded in every STEP. Slot scheme: 7 Init, 8 Start, 9 Close, 10 Update, 11 UpdateMotion, 12 GrfGroundProbe, '
             '13 FHandleMessage, 14 UpdateInput (see [guards-ai](../../engine/guards-ai.md)). Roles from the r9 behaviour screen:\n')
    I.append('| CID | Class (repo) | Proposed | Conf. | Guard flag | Role |\n|---|---|---|---|---|---|')
    for c in sorted(BHVMAP):
        b = BHVMAP[c]
        I.append('| %d | %s | `%s` | %s | %s | %s |' % (c, clink(c) if c in CLS else b['placeholder'], b['proposed_class'], b['confidence'], b['guard_flag'], esc(b['role'])[:160]))
I.append('\n### Plain structs in include/sly2 (no vtable, no descriptor)\n')
plain = sorted(k for k, v in HSTRUCT.items() if k not in NAME2CID and not v['virtuals'] and not any(b in NAME2CID for b in v['bases']))
poly = sorted(k for k, v in HSTRUCT.items() if k not in NAME2CID and (v['virtuals'] or any(b in NAME2CID for b in v['bases'])))
I.append(', '.join('`%s` (%s)' % (k, HSTRUCT[k]['file']) for k in plain) + '\n')
if poly:
    I.append('Header structs that are polymorphic or derive from a registered class but are **not** registry names (each is a hidden-type '
             'candidate or a placeholder alias; see [hidden types C](../hidden-types.md#c-type-names-not-in-the-registry)): ' +
             ', '.join('`%s`%s' % (k, ' : ' + ','.join(HSTRUCT[k]['bases']) if HSTRUCT[k]['bases'] else '') for k in poly) + '\n')
I.append('---\n*Generated by `gen_classes.py`. Data tables: `data/classes.csv`, `data/vtables.csv`, `data/vtable_slots.csv` in the generator output.*')
open(os.path.join(CDIR, 'index.md'), 'w').write('\n'.join(I) + '\n')

# ----------------------------------------------------------------------------------------------- hidden-types.md
Hd = []
Hd.append('# Hidden types: forensics for classes the registry does not list\n')
Hd.append('[Class reference](classes/index.md) · [Object model](../engine/object-model.md)\n')
Hd.append('The class-descriptor table lists %d classes. Sucker Punch\'s source almost certainly had more types: abstract '
          'intermediates without a descriptor, C++ helper classes, and polymorphic structs. This page collects the evidence. '
          '**Every inference here is a HYPOTHESIS** unless tagged otherwise; the data under each one is exact (ELF). Generated by `gen_classes.py`.\n' % NCLS)
Hd.append('## A. Siblings sharing identical overrides their registered parent lacks\n')
Hd.append('If two or more children of P all put the *same* function in a slot where P has something else (or P has no such slot), '
          'the simplest explanation is an unregistered intermediate class between P and them that defines that slot. '
          'Alternative explanations: the children call a shared helper that was installed directly in each class (C++ allows `virtual f() = X::f`-like reuse only via '
          'inheritance, so identical function *addresses* in unrelated classes usually mean either inheritance or a non-member function cast into the vtable, '
          'which Sucker Punch\'s C-style method tables do). **Exclusive** slots use a function no vtable outside the group\'s subtrees uses; '
          'shared generic stubs (empty functions) are counted but are weak evidence. Strength: strong = ≥3 exclusive slots, or new slots + ≥2 exclusive; medium = ≥1 exclusive.\n')
for i, g in enumerate(FOR_A):
    sc = score_a(g)
    Hd.append('<a id="a%d"></a>\n### A%d. Under %s: {%s} — %s\n' % (i + 1, i + 1, clink(g['parent'], 'ref'), ', '.join(clink(m, 'ref') for m in g['members']), sc))
    selfimpl = [m for m in g['members'] if any(CLS[m]['name'].lower() in fname(p).lower() for s, p in g['slots'])]
    Hd.append('HYPOTHESIS: an unregistered class `%s → ? → {%s}`%s. %d identical slots (%d exclusive, %d beyond the parent\'s vtable).'
              % (CLS[g['parent']]['name'], ', '.join(CLS[m]['name'] for m in g['members']),
                 ' (the group is every child of %s with a vtable, so the slots may instead be abstract in %s)' % (CLS[g['parent']]['name'], CLS[g['parent']]['name']) if g['allkids'] else '',
                 len(g['slots']), len(g['excl']), len(g['new'])))
    if selfimpl and sc != 'weak':
        Hd.append(' Alternative HYPOTHESIS: the shared functions are named after %s, so the other members may C++-derive from %s (or reuse its functions) while their descriptors name %s as parent.'
                  % (', '.join(CLS[m]['name'] for m in selfimpl), CLS[selfimpl[0]]['name'], CLS[g['parent']]['name']))
    if sc == 'weak':
        Hd.append('Slots: ' + ', '.join('%d=`%s`' % (s, fname(p)) for s, p in g['slots'][:12]) + (' …' if len(g['slots']) > 12 else '') + '\n')
        continue
    Hd.append('\n| Slot | Role | Shared implementation | Parent has | Exclusive |\n|---|---|---|---|---|')
    ps = CLS[g['parent']]['slots']
    for s, p in g['slots']:
        r, _ = role(g['members'][0], s)
        Hd.append('| %d | %s | %s `%s` | %s | %s |' % (s, esc(r), flink(p, 2), h(p), ('`%s`' % fname(ps[s])) if s < len(ps) else '(no slot)', 'yes' if (s, p) in g['excl'] else 'no (generic)'))
    Hd.append('')
Hd.append('## B. Vtables and descriptor-like tables not claimed by a registered class\n')
Hd.append('### B1. Registered getters returned by more than one vtable\n')
Hd.append('Slot 6 identifies the class a vtable belongs to (a slot-6 function outside the getter block that returns a descriptor record also counts: '
          'a *duplicate getter*). When several vtables report one registered class, the extra ones are either (i) unregistered subclasses that inherit '
          'that class\'s `Pclassinfo`, or (ii) duplicate emissions of the same vtable with a duplicate getter copy (GCC 2.9x emits a vtable and its inline '
          'virtuals in every TU when the class has no out-of-line virtual; such copies are unreferenced). The table facts are `[C:static]`; the reading is HYPOTHESIS. '
          'Vtables never stored by any code are typical of abstract bases (their constructor was inlined into the derived one and the store eliminated) or of dead duplicates.\n')
for c in MULTI:
    Hd.append('#### %s (CID %d): %d vtables\n' % (clink(c, 'ref'), c, len(CLS[c]['vtables'])))
    Hd.append('| vtable | Entries | Slots differing from the consensus (slot=impl) | Stored by (vptr offset) |\n|---|---|---|---|')
    cons = CLS[c]['slots']
    for a in CLS[c]['vtables']:
        v = VT[a]
        diff = ['%d=`%s`' % (s, fname(v['slots'][s])) for s in range(1, v['n']) if s >= len(cons) or cons[s] is None]
        stb = sorted({'%s (%s)' % (flink(r['func'], 2), stfmt(r)) if r['store'] is not None else flink(r['func'], 2) for r in VT_REFS.get(a, [])})
        if v['alt_getter']: diff = ['slot 6 = duplicate getter `%s` %s returning the same record' % (fname(v['slots'][6]), h(v['slots'][6]))] + \
                                   ['%d=`%s`' % (s, fname(v['slots'][s])) for s in range(1, v['n']) if s != 6 and (s >= len(cons) or cons[s] != v['slots'][s])]
        Hd.append('| `%s`%s%s | %d | %s | %s |' % (h(a), ' (canonical)' if CLS[c].get('canon') == a else '', ' (abstract: has `__pure_virtual`)' if any(fname(p) == '__pure_virtual' for p in v['slots'][1:]) else '', v['n'], esc(', '.join(diff[:10]) + (' …' if len(diff) > 10 else '')), ', '.join(stb) or '(no lui/addiu reference: never stored by code)'))
    Hd.append('')
Hd.append('### B2. Vtables with no registered getter in slot 6\n')
Hd.append('%d vtables, grouped into %d families by shared implementations. *Nearest registered* = the registered vtable sharing the most slot '
          'implementations (a hint for the base class). *Owner words* are read off the slot functions\' names.\n' % (len(UNCL), len(UFAM)))
for i, fam in enumerate(UFAM):
    words = Counter()
    for a in fam:
        for wd, k in owner_words(a): words[wd] += k
    Hd.append('#### B2.%d %s\n' % (i + 1, ' / '.join(w for w, _ in words.most_common(2)) or 'unknown'))
    Hd.append('| vtable | Entries | Slot functions | Stored by (vptr offset) | Nearest registered |\n|---|---|---|---|---|')
    for a in fam:
        v = VT[a]; nr = nearest_registered(a)
        sf = ', '.join('%d=`%s`' % (s, fname(p)) for s, p in enumerate(v['slots']) if s and s <= 10) + (' …' if v['n'] > 11 else '')
        stb = sorted({'%s (%s)' % (flink(r['func'], 2), stfmt(r)) if r['store'] is not None else flink(r['func'], 2) for r in VT_REFS.get(a, [])})
        nrs = '%s (%d/%d slots)' % (clink(VT[nr[1]]['cid'], 'ref'), nr[0], v['n'] - 1) if nr[1] and nr[0] >= 3 else '—'
        Hd.append('| `%s` | %d | %s | %s | %s |' % (h(a), v['n'], esc(sf), ', '.join(stb) or '(no lui/addiu reference: reached through a table or unused)', nrs))
    Hd.append('')
Hd.append('### B3. Descriptor-like tables elsewhere\n')
Hd.append('Scan of `.rodata`/`.data` for runs of ≥3 records shaped like `CLASSINFO` (super pointer or 0, cid < cidMax ≤ 1024, size, '
          'factory in `.text` or 0), outside D_%08X:\n' % DESC_TABLE)
if DESC_LIKE:
    for a, n in DESC_LIKE: Hd.append('- `%s`: %d records (HYPOTHESIS: inspect; shape match only)' % (h(a), n))
else:
    Hd.append('- None found `[C:static scan]`: D_%08X is the only class registry in the executable.' % DESC_TABLE)
Hd.append('')
Hd.append('## C. Type names not in the registry\n')
Hd.append('### C1. From mangled symbols\n')
Hd.append('GCC 2.9x mangled names spell the owner class and parameter types. The retail ELF is stripped, so these manglings were assigned by the '
          'decomp team, usually by carrying over the Sly 1 symbol of a matching function: a type that is in Sly 1\'s symbols but has no Sly 2 '
          'registry entry, header struct or vtable (column *Verdict*) is most likely a **Sly 1 carry-over** and the Sly 2 signature should be re-checked. '
          'Names below are not registered class names (repo or master_names). C-prefixed mixed-case names are C++ classes; ALL-CAPS ones are structs or classes.\n')
Hd.append('| Type | As owner | As parameter | Sly 1 syms | Example symbol | Evidence | Header struct | Verdict |\n|---|---|---|---|---|---|---|---|')
for k in sorted(TYPEUSE, key=lambda k: (k not in HSTRUCT, k)):
    if k in regnames: continue
    v = TYPEUSE[k]; ex = (v['owner'] + v['param'])[0]
    srcs = sorted({s for _, _, s in v['owner'] + v['param']})
    hasvt = bool(vts_for_owner(k)) if v['owner'] else False
    if k in HSTRUCT or hasvt: verdict = 'Sly 2 type (header struct or own vtable)'
    elif k.startswith('sce'): verdict = 'Sony SDK type'
    elif SLY1_TYPES.get(k) and k in SLY1_CID: verdict = '**Sly 1 class, not registered in Sly 2: likely carry-over signature (HYPOTHESIS)**'
    elif SLY1_TYPES.get(k): verdict = 'also a Sly 1 type (kept or carried over)'
    else: verdict = 'Sly 2-only name (team-assigned or new)'
    Hd.append('| `%s` | %d | %d | %d | `%s` | %s | %s | %s |' % (k, len(v['owner']), len(v['param']), SLY1_TYPES.get(k, 0), ex[0], ', '.join(srcs), ('`%s`' % HSTRUCT[k]['file']) if k in HSTRUCT else '', verdict))
Hd.append('\nRegistered class names that occur in mangled symbols (confirms the spelling as Sucker Punch\'s): ' +
          ', '.join('`%s`' % k for k in sorted(TYPEUSE) if k in regnames) + '\n')
Hd.append('### C2. Header structs that are polymorphic or derive from a registered class but are not registered names\n')
Hd.append('| Struct | Bases | Header | New virtuals | Note |\n|---|---|---|---|---|')
for k in poly:
    v = HSTRUCT[k]
    Hd.append('| `%s` | %s | `%s` | %d | %s |' % (k, ', '.join(v['bases']), v['file'], len(v['virtuals']), esc(v['note'])[:200]))
Hd.append('')
Hd.append('## D. Code that stores more than one vtable\n')
Hd.append('Functions that load (lui/addiu) two or more distinct vtable addresses and store them. Same offset in sequence = an inlined '
          'base-class constructor followed by the derived one (normal C++ construction: evidence the base has a non-trivial constructor). '
          'Different offsets = embedded polymorphic members or sub-objects (HYPOTHESIS about what they are). `?` store = the store was '
          'not within 10 instructions (table-driven or via another register).\n')
def _desc_pair(v):
    a, b = VT[v[0]['target']]['cid'], VT[v[1]['target']]['cid']
    return a is not None and b is not None and a != b and a in chain(b)
seq_same = [(f, v) for f, v in FOR_D if len(v) == 2 and len({(r['store'], r['base']) for r in v}) == 1 and _desc_pair(v)]
Hd.append('### D1. Base-then-derived pairs (same base register and vptr offset; second class derives from the first)\n')
pairs = Counter((v[0]['target'], v[0]['store']) for f, v in seq_same)
for (t, st), k in pairs.most_common():
    lbl = CLS[VT[t]['cid']]['name'] if VT[t]['cid'] is not None else h(t)
    users = [f for f, v in seq_same if v[0]['target'] == t]
    Hd.append('- First vtable `%s` (%s) at +%s, then the derived class\'s: %d functions (%s%s). Fact: these factories construct a `%s` first. '
              'HYPOTHESIS: `%s` has a real constructor (member objects or explicit ctor) that the compiler inlined.' %
              (h(t), lbl, h(st) if st is not None else '?', k, ', '.join(fname(f) for f in users[:8]), ' …' if k > 8 else '', lbl, lbl))
Hd.append('\n### D2. Other multi-vtable writers\n')
Hd.append('| Function | Vtables in load order (class, vptr offset) |\n|---|---|')
for f, v in FOR_D:
    if (f, v) in seq_same: continue
    Hd.append('| %s | %s |' % (flink(f, 2), ', '.join('`%s` %s +%s' % (h(r['target']), (CLS[VT[r['target']]['cid']]['name'] if VT[r['target']]['cid'] is not None else '(unclaimed)'),
                                                                         stfmt(r)[1:] if stfmt(r).startswith('+') else stfmt(r)) for r in v[:14]) + (' …' if len(v) > 14 else '')))
Hd.append('')
Hd.append('## E. Sly 1 class tree comparison\n')
if SLY1_CID:
    Hd.append('Sly 1 names come from `include/cid.h` and the `struct X : public Y` declarations of the Sly 1 decomp (%d CIDs, %d derivations). '
              'An *intermediate* is a Sly 1 class with at least one subclass.\n' % (len(SLY1_CID), len(SLY1_PAR)))
    s1kids = defaultdict(list)
    for k, (p, fn) in SLY1_PAR.items(): s1kids[p].append(k)
    inter = sorted(k for k in s1kids if k in SLY1_CID or k in ('BASIC',))
    Hd.append('| Sly 1 intermediate | Sly 1 parent | Sly 1 children | In Sly 2 registry? | Sly 2 parent | Verdict |\n|---|---|---|---|---|---|')
    for k in inter:
        p1n = SLY1_PAR.get(k, ('',))[0]
        c2 = NAME2CID.get(k)
        if c2 is None:
            for c in CLS:
                if v3_name(c) == k: c2 = c
        p2 = CLS[CLS[c2]['parent']]['name'] if c2 is not None and CLS[c2]['parent'] is not None else ''
        kids1 = sorted(s1kids[k])
        if c2 is not None:
            kids2 = {CLS[x]['name'] for x in CLS[c2]['children']}
            common = [x for x in kids1 if x in kids2]
            verdict = 'survives' + (' (parent changed %s→%s)' % (p1n, p2) if p1n and p2 != p1n else '') + ('; %d/%d Sly 1 children still direct children' % (len(common), len(kids1)))
        else:
            surv = [x for x in kids1 if x in NAME2CID]
            verdict = 'absent from the registry' + ('; Sly 1 children present in Sly 2: ' + ', '.join(clink(NAME2CID[x], 'ref') + ' (under %s)' % CLS[CLS[NAME2CID[x]]['parent']]['name'] for x in surv) if surv else '')
        Hd.append('| `%s` | %s | %s | %s | %s | %s |' % (k, p1n, esc(', '.join(kids1[:10]) + (' …' if len(kids1) > 10 else '')), clink(c2, 'ref') if c2 is not None else 'no', p2, verdict))
    Hd.append('\n**Sly 1 intermediates without a CID** (non-registry families in Sly 1). Evidence that they survive in Sly 2: a header struct, a mangled type name, or '
              'owner words of unclaimed vtables (B2):\n')
    Hd.append('| Sly 1 type | Sly 1 parent | Sly 1 children | Sly 2 header | Sly 2 mangled uses | In B2 vtable owner words | Verdict |\n|---|---|---|---|---|---|---|')
    b2words = Counter()
    for a in UNCL:
        for wd, k in owner_words(a): b2words[wd.upper()] += k
    for k in sorted(x for x in s1kids if x not in SLY1_CID and x != 'BASIC'):
        kids1 = sorted(s1kids[k]); p1n = SLY1_PAR.get(k, ('',))[0]
        inh = k in HSTRUCT; mu = len(TYPEUSE[k]['owner']) + len(TYPEUSE[k]['param']) if k in TYPEUSE else 0
        bw = b2words.get(k.upper(), 0)
        verdict = 'survives (HYPOTHESIS, %s)' % ', '.join(x for x, y in (('header', inh), ('vtables', bw), ('mangled', mu)) if y) if (inh or bw) else ('mangled symbols only (no header/vtable evidence)' if mu else 'no trace by name')
        if k in NAME2CID: verdict = '**now a registered class** in Sly 2 (%s, CID %d)' % (clink(NAME2CID[k], 'ref'), NAME2CID[k])
        if k in SLY1_NOTES: verdict += '; ' + SLY1_NOTES[k]
        Hd.append('| `%s` | %s | %s | %s | %d | %s | %s |' % (k, p1n, esc(', '.join(kids1[:8]) + (' …' if len(kids1) > 8 else '')), 'yes' if inh else '', mu, bw or '', verdict))
    moved = []
    for n2, c in NAME2CID.items():
        if n2 in SLY1_PAR and CLS[c]['parent'] is not None:
            p1n = SLY1_PAR[n2][0]; p2 = CLS[CLS[c]['parent']]['name']
            if p1n != p2: moved.append((n2, p1n, p2))
    Hd.append('\n**Classes present in both games whose parent changed** (Sly 1 → Sly 2): ' + ', '.join('`%s` (%s → %s)' % m for m in sorted(moved)) + '\n')
    only1 = sorted(k for k in SLY1_CID if k not in NAME2CID and k not in {v3_name(c) for c in CLS})
    Hd.append('**Sly 1 CIDs with no Sly 2 registry entry** (%d): %s\n' % (len(only1), ', '.join('`%s`' % k for k in only1)))
    hdr_surv = [k for k in only1 if k in HSTRUCT or k in TYPEUSE]
    if hdr_surv:
        Hd.append('Of these, names still present in Sly 2 headers or mangled symbols (HYPOTHESIS: survive as unregistered types): ' + ', '.join('`%s`' % k for k in hdr_surv) + '\n')
else:
    Hd.append('(Sly 1 decomp not found.)\n')
Hd.append('---\n*Generated by `gen_classes.py`.*')
os.makedirs(os.path.join(OUT, 'docs/reference'), exist_ok=True)
open(os.path.join(OUT, 'docs/reference/hidden-types.md'), 'w').write('\n'.join(Hd) + '\n')

# ----------------------------------------------------------------------------------------------- link check
def check_links():
    bad = []; n = 0
    roots = [os.path.join(OUT, 'docs'), DOCS]
    for root, ds, fs in os.walk(os.path.join(OUT, 'docs')):
        for f in fs:
            if not f.endswith('.md'): continue
            p = os.path.join(root, f); t = open(p).read()
            t = re.sub(r'```.*?```', '', t, flags=re.S)
            for m in re.finditer(r'\]\(([^)\s]+)\)', t):
                tgt = m.group(1).split('#')[0]
                if not tgt or tgt.startswith('http'): continue
                n += 1
                rel = os.path.relpath(os.path.normpath(os.path.join(root, tgt)), os.path.join(OUT, 'docs'))
                cands = [os.path.join(OUT, 'docs', rel), os.path.join(DOCS, rel), os.path.normpath(os.path.join(DOCS, '..', rel[3:])) if rel.startswith('../') else None]
                if not any(c and os.path.exists(c) for c in cands): bad.append((os.path.relpath(p, OUT), m.group(1)))
    return n, bad
n, bad = check_links()
print('classes: %d pages; vtables %d (claimed %d, unclaimed %d in %d families); hidden-type groups A=%d (strong %d, medium %d); D=%d; layout: %s'
      % (NCLS, len(VT), ncl, len(UNCL), len(UFAM), len(FOR_A), sum(score_a(g) == 'strong' for g in FOR_A), sum(score_a(g) == 'medium' for g in FOR_A), len(FOR_D), LAYOUT_STATUS))
print('links checked: %d, broken: %d' % (n, len(bad)))
for b in bad[:30]: print('  BROKEN', b)
