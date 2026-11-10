#!/usr/bin/env python3
"""Step 3: resolve the class behind each type-'c' view (fix == 'c') of the view-plan JSON (--views).
 (a) vtjoin: the view is `this` of a virtual (or one caller level), from vtjoin.json (--vtjoin; optional);
 (b) reached through a typed field or global: the view pointer comes from `EXPR` (a cast `(V *)EXPR`, an initializer
     `V *x = EXPR`, or a member `V *m` of another local view W); EXPR's base has a known type (header type, global,
     resolved local view) and the field's pointee class comes from the snapshots (layouts.json slot targets, aggregated
     over the holder's descendants), or the global's own address/pointee in the snapshots;
 (c) layout compatibility: the view's typed members (ptr/float/int at census offsets) match the inferred slots of
     the classes seen alive; resolved when the compatible classes share one ancestor that is itself big enough.
Iterates (b) over views resolved in earlier rounds. Accuracy check on census tier-A views of seen classes.
Needs classes.json + layouts.json (steps 1-2) in WORK. Writes WORK/view_resolution.json."""
import os, sys, re, json, collections
import numpy as np
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import snapcfg
if __name__ == '__main__': snapcfg.parse(__doc__)
import common as C
import cparse
from snapshot_objects import hdr_chain

P = json.load(open(snapcfg.need('views', 'view-plan JSON')))
_vj = snapcfg.get('vtjoin')
VJ = json.load(open(_vj)) if _vj and os.path.exists(_vj) else {'type_c': []}   # vtjoin.py output; optional
LAY = json.load(open(os.path.join(C.WORK, 'layouts.json')))['classes']
CL = json.load(open(os.path.join(C.WORK, 'classes.json')))['classes']
PTRK = ('obj', 'objint', 'heap', 'global', 'code', 'vptr', 'ptr')

# ---------------------------------------------------------------- class tree helpers
def chain(l):
    ch = hdr_chain(l) if l in C.LAYOUT else [l]
    cid = CL.get(l, {}).get('cid')
    if cid is not None:
        for k in C.cid_chain(cid):
            n = C.cname(k)
            if n not in ch: ch.append(n)
    return ch
CHAIN = {l: chain(l) for l in set(LAY) | set(CL)}
def ch(l): return CHAIN.get(l) or chain(l)
def lca(ls):
    ls = [l for l in ls if l]
    if not ls: return None
    inter = ch(ls[0])
    for l in ls[1:]: inter = [x for x in inter if x in ch(l)]
    return inter[0] if inter else None
def related(a, b): return a in ch(b) or b in ch(a)
def size_of(l):
    if l in LAY: return LAY[l]['size']
    if l in C.LAYOUT: return C.LAYOUT[l]['size']
    return None
DESC = collections.defaultdict(list)      # T -> seen classes whose chain includes T
for l in LAY:
    for t in ch(l): DESC[t].append(l)

def agg_slot(T, off):
    """slot statistics at `off` over every seen class in T's subtree (instance-weighted)."""
    k = collections.Counter(); t = collections.Counter()
    for l in DESC.get(T, []):
        sl = LAY[l]['slots']
        if off % 4 or off // 4 >= len(sl): continue
        e = sl[off // 4]
        k.update(e['kinds']); t.update(e.get('targets', {}))
    return k, t

def pointee_from_slot(T, off):
    k, t = agg_slot(T, off)
    nz = sum(v for x, v in k.items() if x != 'zero')
    objs = {x: v for x, v in t.items() if '+0x' not in x}
    if not objs: return None
    tot = sum(objs.values())
    # weighted LCA: drop targets below 3% before taking the common ancestor
    keep = [x for x, v in objs.items() if v >= 0.03 * tot]
    res = lca(keep)
    share = sum(v for x, v in objs.items() if res and res in ch(x)) / max(nz, 1)
    return dict(cls=res, evidence=tot, share=round(share, 3), targets=dict(collections.Counter(objs).most_common(4)))

# ---------------------------------------------------------------- snapshot oracle for globals
SNAP_OBJ = []
def load_oracle():
    vts = C.find_vtables(); vl = {a: v['label'] for a, v in vts.items()}
    for name, path, h in C.snapshots():
        o = np.load(os.path.join(C.WORK, 'objects', name + '.npy'))
        SNAP_OBJ.append((name, path, {int(a): vl[int(v)] for a, v, *_ in o}))
def global_object(addr):
    c = collections.Counter(d.get(addr) for _, _, d in SNAP_OBJ); c.pop(None, None); return c
def global_pointee(addr):
    c = collections.Counter()
    for name, path, d in SNAP_OBJ:
        w = int(np.fromfile(path, dtype='<u4', count=1, offset=addr)[0])
        if w in d: c[d[w]] += 1
        elif w: c['<non-object>'] += 1
    return c
SYMBYNAME = {}
for a, n in C.SYM.items(): SYMBYNAME.setdefault(n, a)

# ---------------------------------------------------------------- header field lookup
def header_member(T, name):
    """-> (off, type, isptr) of a member named `name` in header type T or its bases"""
    t = T
    while t in C.LAYOUT:
        for off, sz, nm, ty, isp in C.LAYOUT[t]['fields']:
            if nm == name: return off, ty, isp or ty.startswith('CRefPtr'), sz
        t = C.LAYOUT[t].get('base')
    return None

# ---------------------------------------------------------------- view parsing / per region
REGCACHE = {}
def region_info(file, ns):
    key = (file, ns)
    if key in REGCACHE: return REGCACHE[key]
    S = cparse.structs_in(file, ns)
    reg, _ = cparse.region(file, ns)
    # variable declarations T *x (params and locals) and extern globals
    decl = {}
    for m in re.finditer(r'(?:struct\s+|const\s+)*\b([A-Za-z_]\w*)\s*(\*+)\s*(?:const\s+)?([A-Za-z_]\w*)\s*(?=[,;=)\[]|__asm__)', reg):
        decl.setdefault(m.group(3), (m.group(1), len(m.group(2))))
    ext = {}
    for m in re.finditer(r'extern\s+(?:"C"\s+)?(?:struct\s+)?([A-Za-z_]\w*)\s*(\**)\s*(\w+)\s*(?:\[[^\]]*\])?\s*(?:__asm__\s*\(\s*"(\w+)"\s*\))?\s*;', reg):
        ext[m.group(3)] = (m.group(1), len(m.group(2)), m.group(4) or m.group(3))
    REGCACHE[key] = (S, reg, decl, ext)
    return REGCACHE[key]

GLOBAL_TYPES = {'g_sw': ('SW', 0, 'D_003DD490')}
_cc = snapcfg.get('census-config')
for t, names in (json.load(open(_cc))['globals'] if _cc else {}).items():
    GLOBAL_TYPES.setdefault(names[0], (t, 0, names[1]))

EXPR = r'(?:\(\s*\(\s*(?:struct\s+)?(\w+)\s*\*\s*\)\s*([\w]+)\s*\)|(&)?\s*([A-Za-z_]\w*))((?:\s*(?:\.|->)\s*\w+|\s*\[[^\]]*\])*)'

def sources_of(v, S, reg, decl, ext):
    V = v['view']; out = []
    # E1: members V *m of other local structs
    for W, s in S.items():
        if W == V: continue
        for mb in s['members']:
            if mb['base'] == V and mb['ptr'] == 1 and 'off' in mb:
                out.append(('member', W, mb['off'], mb['name']))
    # E2: casts and initializers
    for m in re.finditer(r'\(\s*(?:struct\s+)?%s\s*\*\s*\)\s*' % re.escape(V) + EXPR, reg):
        out.append(('expr',) + m.groups())
    for m in re.finditer(r'\b%s\s*\*\s*\w+\s*(?:__asm__\([^)]*\))?\s*=\s*(?:\(\s*(?:struct\s+)?\w+\s*\*\s*\)\s*)?' % re.escape(V) + EXPR, reg):
        out.append(('expr',) + m.groups())
    # extern V g / extern V *g
    for g, (t, nptr, asm) in ext.items():
        if t == V: out.append(('extern', g, nptr, asm))
    return out

def walk_expr(src, v, S, decl, ext, resolved, file, ns):
    """-> dict(cls, how, evidence...) or None"""
    _, ctype, cvar, amp, ident, rest = src
    fields = re.findall(r'(\.|->)\s*(\w+)|\[([^\]]*)\]', rest or '')
    # ---- base type
    if ctype:
        T, isptr = ctype, True; basename = cvar
    else:
        basename = ident
        if ident in ext and ext[ident][0] in S:
            T, isptr = ext[ident][0], ext[ident][1] > 0
            if ident in GLOBAL_TYPES: resolved.setdefault((file, ns, T), GLOBAL_TYPES[ident][0])
        elif ident in GLOBAL_TYPES and ident not in decl:
            T, isptr = GLOBAL_TYPES[ident][0], False
        elif ident in ext:
            T, isptr = ext[ident][0], ext[ident][1] > 0
        elif ident in decl:
            T, isptr = decl[ident][0], decl[ident][1] > 0
        else:
            return None
    if not fields:
        # (V *)x with x a typed pointer: the static type itself (a header class) or a resolved local view
        if amp: return None
        if isptr:
            if (file, ns, T) in resolved: return dict(cls=resolved[(file, ns, T)], how='alias-of-resolved-view', evidence=1)
            if T in LAY or T in DESC: return dict(cls=T, how='alias-of-typed-pointer', evidence=1)
        return None
    # ---- walk
    cur, off = T, 0
    def is_local(t): return t in S
    def obj_class(t):
        if is_local(t): return resolved.get((file, ns, t))
        return t if (t in DESC or t in C.LAYOUT) else None
    holder = obj_class(cur)
    steps = list(fields)
    for i, (op, name, idx) in enumerate(steps):
        last = i == len(steps) - 1
        if idx is not None and not op and not name:
            continue                     # array index: keep the element offset of the array start (approximation)
        if op == '->' and i > 0:
            return None                  # handled by the pointee step below
        if is_local(cur):
            mb = next((m for m in S[cur]['members'] if m['name'] == name), None)
            if not mb or 'off' not in mb: return None
            moff, mty, mptr = mb['off'], mb['base'], mb['ptr'] > 0
        else:
            hm = header_member(cur, name)
            if not hm: return None
            moff, mty, mptr = hm[0], hm[1], hm[2]
            if mty.startswith('CRefPtr'): mptr = True
        off += moff
        nxt = steps[i + 1] if not last else None
        if mptr and (last or (nxt and nxt[0] == '->')):
            static_ok = mty in C.LAYOUT and not is_local(mty) and mty not in ('void', 'char', 'int', 'uint', 'uchar')
            if last:
                if amp: return dict(cls=None, embedded='%s+0x%X' % (holder, off), how='address-of-member') if holder else None
                pe = pointee_from_slot(holder, off) if holder else None
                if pe and pe['cls']:
                    return dict(cls=pe['cls'], how='field-slot', holder=holder, off=off, evidence=pe['evidence'],
                                share=pe['share'], targets=pe['targets'], static=mty)
                if static_ok:
                    k, _ = agg_slot(holder, off) if holder else (collections.Counter(), None)
                    return dict(cls=mty, how='field-static-type', holder=holder, off=off, static=mty,
                                slot_kinds=dict(k) if k else None, evidence=0)
                if holder:
                    k, _ = agg_slot(holder, off)
                    if k: return dict(cls=None, how='field-nonobject', embedded='*(%s+0x%X) %s' % (holder, off, dict(k)))
                return None
            # dereference and continue with the pointee
            pe = pointee_from_slot(holder, off) if holder else None
            stat = mty if static_ok else None
            if is_local(mty) and (file, ns, mty) in resolved: stat = resolved[(file, ns, mty)]
            nxtcls = (pe['cls'] if pe and pe['cls'] else None) or stat
            if not nxtcls: return None
            cur = mty if is_local(mty) else nxtcls; holder = nxtcls if not is_local(mty) else (resolved.get((file, ns, mty)) or nxtcls)
            off = 0
            steps[i + 1] = ('.', nxt[1], None)
            continue
        if last:
            if amp or not mptr:
                return dict(cls=None, embedded='%s+0x%X' % (holder, off), how='embedded-member') if holder else None
        if not mptr:
            # embedded struct member: continue inside it
            if not is_local(cur) and mty in C.LAYOUT: cur = mty
            elif is_local(cur) and is_local(mty): cur = mty
            elif not is_local(cur): cur = mty
            else: return None
    return None

def resolve_b(v, resolved, file, ns):
    S, reg, decl, ext = region_info(file, ns)
    res = []
    for src in sources_of(v, S, reg, decl, ext):
        if src[0] == 'member':
            _, W, off, mname = src
            holder = resolved.get((file, ns, W))
            if not holder: continue
            pe = pointee_from_slot(holder, off)
            if pe and pe['cls']:
                res.append(dict(cls=pe['cls'], how='member-of-resolved-view', holder=holder, via=W, off=off,
                                evidence=pe['evidence'], share=pe['share'], targets=pe['targets']))
        elif src[0] == 'extern':
            _, g, nptr, asm = src
            a = SYMBYNAME.get(asm) or SYMBYNAME.get(g)
            if a is None and re.match(r'D_[0-9A-Fa-f]{8}$', asm): a = int(asm[2:], 16)
            if a is None: continue
            c = global_pointee(a) if nptr else global_object(a)
            objs = {k: n for k, n in c.items() if k != '<non-object>'}
            if objs:
                res.append(dict(cls=lca(list(objs)), how='global-pointee' if nptr else 'global-object', addr=hex(a),
                                evidence=sum(objs.values()), share=round(sum(objs.values()) / max(sum(c.values()), 1), 3),
                                targets=dict(objs)))
        else:
            r = walk_expr(src, v, S, decl, ext, resolved, file, ns)
            if r: res.append(r)
    return res

# ---------------------------------------------------------------- (d) prototype unification (not snapshot based)
BUILTIN = {'i': 'int', 'f': 'float', 'v': 'void', 'c': 'char', 's': 'short', 'l': 'long', 'x': 'llong', 'b': 'bool',
           'd': 'double', 'e': '...', 'w': 'wchar'}
def demangle_params(name):
    """g++ 2.95 mangled name -> [(type, nptr)] including the implicit this of a method."""
    m = re.match(r'^(.*?)__(\d+)([A-Za-z_]\w*)$', name)
    params = []
    if m and not name.startswith('_'):
        cl = m.group(3)[:int(m.group(2))]; rest = m.group(3)[int(m.group(2)):]
        params.append((cl, 1))
        if rest.startswith('C'): rest = rest[1:]
    else:
        m = re.match(r'^(.*?)__F(.*)$', name)
        if not m: return None
        rest = m.group(2)
    i = 0; out = list(params); nparams_base = len(out)
    while i < len(rest):
        nptr = 0
        while i < len(rest) and rest[i] in 'PRCVU' + 'S':
            if rest[i] in 'PR': nptr += 1
            i += 1
        if i >= len(rest): break
        ch_ = rest[i]
        if ch_.isdigit():
            j = i
            while j < len(rest) and rest[j].isdigit(): j += 1
            n = int(rest[i:j]); out.append((rest[j:j + n], nptr)); i = j + n
        elif ch_ == 'T':
            k = int(rest[i + 1]) if i + 1 < len(rest) and rest[i + 1].isdigit() else 0
            out.append(out[nparams_base + k - 1] if 0 < k <= len(out) - nparams_base else ('?', 0)); i += 2
        elif ch_ == 'N':
            c = int(rest[i + 1]); k = int(rest[i + 2])
            for _ in range(c): out.append(out[nparams_base + k - 1] if 0 < k <= len(out) - nparams_base else ('?', 0))
            i += 3
        elif ch_ in BUILTIN: out.append((BUILTIN[ch_], nptr)); i += 1
        elif ch_ == 'Q' or ch_ == 't': return out    # qualified/template: stop
        else: i += 1
    return out

GLOBAL_FN = set(C.FUNCS.values())
PROTO = collections.defaultdict(list)    # fname -> [(file, [(type, nptr)])]
FILE_STRUCTS = {}
def build_proto_index():
    for root, _, fs in os.walk(os.path.join(C.WT, 'src')):
        for f in fs:
            if not f.endswith(('.cpp', '.c', '.inc', '.h')): continue
            path = os.path.join(root, f); rel = os.path.relpath(path, C.WT)
            txt = cparse.strip_comments(open(path, errors='ignore').read())
            FILE_STRUCTS[rel] = set(re.findall(r'\b(?:struct|union|class)\s+(\w+)\s*(?::[^{;]*)?\{', txt))
            for m in re.finditer(r'\b(\w+)\s*\(([^;{}()]*(?:\([^()]*\)[^;{}()]*)*)\)\s*(?:const\s*)?(?:__asm__\s*\(\s*"(\w+)"\s*\))?\s*[;{]', txt):
                fn = m.group(3) or m.group(1)
                if fn in ('if', 'while', 'for', 'switch', 'return', 'sizeof', '__asm__', 'asm'): continue
                if not (m.group(3) or '__' in fn or fn in GLOBAL_FN): continue
                ps = []
                for prm in m.group(2).split(','):
                    prm = re.sub(r'\b(const|struct|union|class|volatile|register)\b', ' ', prm).strip()
                    mm = re.match(r'([A-Za-z_]\w*)\s*(\**)', prm)
                    ps.append((mm.group(1), len(mm.group(2))) if mm else ('?', 0))
                if ps == [('void', 0)]: ps = []
                PROTO[fn].append((rel, ps))
FSIG = {}
RESOLVED_BY_FILE = {}
def fn_param_types(fn):
    if fn in FSIG: return FSIG[fn]
    occ = collections.defaultdict(list)     # k -> [(file, type, nptr, local)]
    for rel, ps in PROTO.get(fn, []):
        for k, (t, n) in enumerate(ps):
            occ[k].append((rel, t, n, t in FILE_STRUCTS.get(rel, ())))
    dm = demangle_params(fn)
    if dm:
        for k, (t, n) in enumerate(dm): occ[k].append(('<mangled>', t, n, False))
    FSIG[fn] = occ
    return occ

def resolve_d(v, resolved, file, ns):
    """V is parameter k of a function whose other declarations (any file) or mangled name give a header class (or a
    resolved local view) at k."""
    S, reg, decl, ext = region_info(file, ns)
    V = v['view']; res = []
    for m in re.finditer(r'\b(\w+)\s*\(([^;{}()]*\b%s\s*\*[^;{}()]*)\)\s*(?:const\s*)?(?:__asm__\s*\(\s*"(\w+)"\s*\))?\s*[;{]' % re.escape(V), reg):
        fn = m.group(3) or m.group(1)
        if not (m.group(3) or '__' in fn or fn in GLOBAL_FN): continue
        params = [re.sub(r'\b(const|struct|union|class|volatile|register)\b', ' ', x).strip() for x in m.group(2).split(',')]
        ks = [k for k, x in enumerate(params) if re.match(r'%s\s*\*(?!\*)' % re.escape(V), x)]
        occ = fn_param_types(fn)
        npar = len(params)
        for k in ks:
            for rel, t, n, local in occ.get(k, []):
                if rel != '<mangled>' and len(next((ps for r2, ps in PROTO[fn] if r2 == rel), [])) not in (npar, 0): continue
                if n != 1: continue
                if local:
                    if rel == file and t == V: continue
                    c = RESOLVED_BY_FILE.get((rel, t))
                    if c: res.append(dict(cls=c, how='proto-resolved-view', fn=fn, k=k, file=rel))
                elif t in C.LAYOUT or t in DESC:
                    if t in ('BASIC',) and False: continue
                    res.append(dict(cls=t, how='proto-header-type' if rel != '<mangled>' else 'mangled-name', fn=fn, k=k, file=rel))
    return res

# ---------------------------------------------------------------- (c) layout compatibility
def view_fields(s):
    out = []
    for mb in s['members']:
        if 'off' not in mb or mb.get('bits'): continue
        k = mb['kind']
        if k in ('ptr', 'float', 'int'):
            for i in range(min(mb['arr'], 8)):
                o = mb['off'] + i * (mb.get('esz') or 4)
                if o % 4 == 0: out.append((o, k, mb['base'] if k == 'ptr' else None))
    return out

def slot_ok(kind, pbase, e):
    """True/False when the slot's snapshot values can say whether a member of this kind fits; None when they cannot.
    Only pointer and float members are informative (views declare unknown words as int freely)."""
    k = e['kinds']; nz = sum(v for x, v in k.items() if x != 'zero')
    if nz < 5 or kind == 'int': return None
    pt = (sum(k.get(x, 0) for x in PTRK) + k.get('negint', 0)) / nz
    fl = k.get('float', 0) / nz
    if kind == 'ptr':
        if pt < 0.8: return False
        if pbase in DESC and e.get('targets'):
            objs = {x: v for x, v in e['targets'].items() if '+0x' not in x}
            if objs and not any(pbase in ch(x) for x in objs): return False
        return True
    if kind == 'float':
        return fl >= 0.6
    return None

MIN_INST = 3
def resolve_c(s, pmap=None):
    vf = view_fields(s)
    if pmap: vf = [(o, k, pmap.get(pb, pb)) for o, k, pb in vf]
    vsize = s.get('size') or 0
    cand = []
    for l, L in LAY.items():
        if L['instances'] < MIN_INST or L['size'] < vsize: continue
        n_inf = mism = 0
        for o, k, pb in vf:
            if o // 4 >= len(L['slots']): mism += 1; continue
            r = slot_ok(k, pb, L['slots'][o // 4])
            if r is None: continue
            w = 2 if (k == 'ptr' and pb in DESC) else 1      # a typed-pointer member pins the slot's target class
            n_inf += w; mism += w * (r is False)
        if n_inf - mism >= 3 and mism <= n_inf // 5:
            cand.append((l, n_inf - 2 * mism, n_inf, mism))
    if not cand: return None, []
    names = [c[0] for c in cand]
    best = max(c[1] for c in cand)
    if best < 4:
        return dict(cls=None, how='weak', n_compatible=len(cand), best=best), names
    top = [c for c in cand if c[1] >= best - 1]
    a = lca([c[0] for c in top])
    if a and (size_of(a) or 0) >= vsize and a not in ('BASIC',):
        conf = 'high' if len(top) <= 3 and best >= 6 else 'medium' if len(top) <= 8 else 'low'
        return dict(cls=a, how='layout-lca', n_compatible=len(cand), n_top=len(top), score=best, conf=conf,
                    top=sorted(c[0] for c in top)[:8]), names
    exact = [c for c in top if LAY[c[0]]['size'] == vsize]
    if len(exact) == 1:
        return dict(cls=exact[0][0], how='layout-exact-size', n_compatible=len(cand), n_top=len(top), score=best,
                    conf='low'), names
    return dict(cls=None, how='ambiguous', n_compatible=len(cand), score=best, top=sorted(c[0] for c in top)[:8]), names

# ---------------------------------------------------------------- main
def main():
    load_oracle(); build_proto_index()
    cviews = [v for v in P['views'] if v['fix'] == 'c']
    vjmap = {(r['file'], r['line'], r['view']): r for r in VJ['type_c']}
    # accuracy set: census tier-A views whose type is a seen class
    acc_views = []
    for e in cparse.CENSUS:
        if e['tier'] == 'A' and e.get('type') in LAY and LAY[e['type']]['instances'] > 0 and e['type'] not in C.CLASSES.get(0, {}).get('name', ''):
            acc_views.append(dict(file=e['file'], line=e['line'], view=e['name'], namespace=e['ns'], real_type=e['type'],
                                  milestone=None, fix='A'))
    allv = [(v, 'c') for v in cviews] + [(v, 'acc') for v in acc_views]
    resolved = {}           # (file, ns, view) -> class (iterated for (b))
    results = {}
    def key(v): return (v['file'], v['namespace'], v['view'])
    # known header-typed views count as resolved holders for the accuracy set only through their real type
    for v, kind in allv:
        S = region_info(v['file'], v['namespace'])[0]
        s = S.get(v['view'])
        r = dict(file=v['file'], line=v['line'], view=v['view'], function=v.get('function'), milestone=v.get('milestone'),
                 set=kind, real_type=v.get('real_type'), candidate_type=v.get('candidate_type'), size=s and s.get('size'),
                 methods={})
        j = vjmap.get((v['file'], v['line'], v['view'])) if kind == 'c' else None
        if j and j.get('status') in ('vtable', 'caller'):
            r['methods']['a'] = dict(cls=j['cls'], how=j['status'] + '/' + j.get('how', ''))
        if s:
            c, names = resolve_c(s)
            if c: r['methods']['c'] = c
        results[(kind,) + key(v)] = r
    for it in range(4):
        before = len(resolved)
        for (kind, f, ns, vn), r in results.items():
            m = r['methods']
            for meth in ('a', 'b', 'd', 'c'):
                if meth in m and m[meth].get('cls'):
                    trusted = meth == 'a' or (meth == 'b' and 'alias-of-typed-pointer' not in m[meth]['how']) or \
                              (meth == 'd' and not m[meth]['conflict']) or (meth == 'c' and m[meth].get('conf') == 'high')
                    if trusted and (kind == 'c' or meth != 'c'): resolved.setdefault((f, ns, vn), m[meth]['cls'])
                    break
        RESOLVED_BY_FILE.clear()
        cnt = collections.Counter((f, vn) for (f, ns, vn) in resolved)
        for (f, ns, vn), c in resolved.items():
            if cnt[(f, vn)] == 1: RESOLVED_BY_FILE[(f, vn)] = c
        for (kind, f, ns, vn), r in results.items():
            v = dict(view=vn)
            S_ = region_info(f, ns)[0]
            if vn in S_ and it > 0:
                pmap = {t: c for (f2, ns2, t), c in resolved.items() if f2 == f and ns2 == ns and t != vn}
                c, _ = resolve_c(S_[vn], pmap)
                if c: r['methods']['c'] = c
            ds = resolve_d(v, resolved, f, ns)
            if ds:
                cl = collections.Counter(x['cls'] for x in ds)
                rel_ok = all(related(a, b) for a in cl for b in cl)
                best = max(cl, key=lambda x: (len(ch(x)), cl[x])) if rel_ok else None
                r['methods']['d'] = dict(cls=best, how='+'.join(sorted(set(x['how'] for x in ds))), conflict=not rel_ok,
                                         n_sources=len(ds), sources=ds[:4])
            bs = resolve_b(v, resolved, f, ns)
            if bs:
                good = [b for b in bs if b.get('cls')]
                if good:
                    cl = collections.Counter(b['cls'] for b in good)
                    best = lca(list(cl)) if len(cl) > 1 else next(iter(cl))
                    hows = sorted(set(b['how'] for b in good))
                    r['methods']['b'] = dict(cls=best, how='+'.join(hows), n_sources=len(good),
                                             conflict=len(cl) > 1, sources=good[:4])
                else:
                    r['methods']['b_embedded'] = [b.get('embedded') for b in bs if b.get('embedded')][:3]
                    r['methods'].pop('b', None)
        if len(resolved) == before and it: break
    # final
    SNAPB = ('field-slot', 'global-', 'member-of-resolved-view', 'alias-of-resolved-view')
    for r in results.values():
        m = r['methods']; cands = []
        if m.get('a', {}).get('cls'): cands.append(('a', m['a']['cls'], 4))
        if m.get('b', {}).get('cls'):
            snap = any(x in m['b']['how'] for x in SNAPB)
            cands.append(('b' if snap else 'b-static', m['b']['cls'], 3 if snap else 1))
        if m.get('d', {}).get('cls'): cands.append(('d', m['d']['cls'], 2))
        if m.get('c', {}).get('cls'): cands.append(('c', m['c']['cls'], 0 if m['c'].get('conf') == 'low' else 1))
        fin = None
        if cands:
            agree = all(related(x[1], y[1]) for x in cands for y in cands)
            if agree:
                pick = max(cands, key=lambda x: (len(ch(x[1])), x[2]))
            else:
                pick = max(cands, key=lambda x: x[2])
            fin = dict(cls=pick[1], method=pick[0], methods=[x[0] for x in cands], agree=agree)
            strong = [x for x in cands if x[2] >= 2]
            if not agree: fin['conf'] = 'conflict'
            elif len(cands) >= 2 and strong: fin['conf'] = 'high'
            elif strong: fin['conf'] = 'medium'
            elif m.get('c', {}).get('conf') in ('high', 'medium') or pick[0] == 'b-static': fin['conf'] = 'medium' if len(cands) >= 2 else 'low'
            else: fin['conf'] = 'low'
        r['final'] = fin
        if r['real_type'] and fin:
            rt = r['real_type']
            r['check'] = 'exact' if fin['cls'] == rt else 'related' if related(fin['cls'], rt) else 'wrong'
        if r['candidate_type'] and fin:
            r['agrees_candidate'] = 'exact' if fin['cls'] == r['candidate_type'] else 'related' if related(fin['cls'], r['candidate_type']) else 'no'
    # ---- summary
    cres = [r for r in results.values() if r['set'] == 'c']
    ares = [r for r in results.values() if r['set'] == 'acc']
    summ = dict(views_c=len(cres),
                resolved_any=sum(1 for r in cres if r['final']),
                resolved_by_final_method=dict(collections.Counter(r['final']['method'] for r in cres if r['final'])),
                method_hits=dict(a=sum(1 for r in cres if 'a' in r['methods']),
                                 d=sum(1 for r in cres if r['methods'].get('d', {}).get('cls')),
                                 b=sum(1 for r in cres if r['methods'].get('b', {}).get('cls')),
                                 c=sum(1 for r in cres if r['methods'].get('c', {}).get('cls')),
                                 c_ambiguous=sum(1 for r in cres if r['methods'].get('c', {}).get('how') == 'ambiguous'),
                                 embedded=sum(1 for r in cres if 'b_embedded' in r['methods'])),
                conf=dict(collections.Counter(r['final']['conf'] for r in cres if r['final'])),
                by_milestone={str(ms): dict(total=sum(1 for r in cres if r['milestone'] == ms),
                                            resolved=sum(1 for r in cres if r['milestone'] == ms and r['final']),
                                            high_med=sum(1 for r in cres if r['milestone'] == ms and r['final'] and r['final']['conf'] in ('high', 'medium')))
                              for ms in sorted(set(r['milestone'] for r in cres))},
                candidate_agreement=dict(collections.Counter(r.get('agrees_candidate') for r in cres if r['final'] and r['candidate_type'])),
                accuracy=dict(n=len(ares)))
    reasons = collections.Counter()
    for r in cres:
        if r['final']: continue
        m = r['methods']; f, vn = r['file'], r['view']
        ns = next(v['namespace'] for v in cviews if v['file'] == f and v['line'] == r['line'] and v['view'] == vn)
        S, reg, decl, ext = region_info(f, ns)
        srcs = sources_of(dict(view=vn), S, reg, decl, ext)
        if 'b_embedded' in m: reasons['embedded record / non-object pointee'] += 1
        elif m.get('c', {}).get('how') == 'ambiguous': reasons['layout ambiguous (several unrelated classes fit)'] += 1
        elif any(x[0] == 'member' for x in srcs): reasons['member of an unresolved view'] += 1
        elif re.search(r'\b%s\s*\*\s*\w*\s*[,)]' % re.escape(vn), reg): reasons['parameter only, no typed declaration elsewhere'] += 1
        elif srcs: reasons['source untyped (call result, void *, asm register)'] += 1
        else: reasons['no usage pattern found'] += 1
    summ['unresolved_reasons'] = dict(reasons.most_common())
    for meth in ('b', 'c', 'd', 'final'):
        cc = collections.Counter()
        for r in ares:
            mm = r['methods'].get(meth) if meth != 'final' else r['final']
            if mm and mm.get('cls'):
                rt = r['real_type']; p = mm['cls']
                cc['exact' if p == rt else 'related' if related(p, rt) else 'wrong'] += 1
            else: cc['unresolved'] += 1
        summ['accuracy'][meth] = dict(cc)
    json.dump(dict(summary=summ, views=[r for r in results.values() if r['set'] == 'c'],
                   accuracy_views=[r for r in results.values() if r['set'] == 'acc']),
              open(os.path.join(C.WORK, 'view_resolution.json'), 'w'), indent=1, default=str)
    print(json.dumps(summ, indent=1))

if __name__ == '__main__':
    main()
