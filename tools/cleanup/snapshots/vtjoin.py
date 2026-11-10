#!/usr/bin/env python3
"""vtjoin: estimate how many type-'c' views (view-plan JSON, --views) get a class mechanically.
Read-only on the tree; writes WORK/vtjoin.json + WORK/vtjoin_REPORT.md. Needs --elf and --views (see snapcfg).
 1. vtables from the ELF (_vt$ symbols; 8-byte entries {short delta, short pad, u32 pfn}, entry 0 = header).
 2. view -> function; view must be bound to the function's 1st argument (strict) to use the function's class.
 3. one level of caller propagation through jal sites that pass the caller's own a0 through.
"""
import json, re, struct, os, sys, collections, bisect, random
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import snapcfg
snapcfg.parse(__doc__)
WT = snapcfg.get('root')
ELFP = os.path.realpath(snapcfg.need('elf', 'game ELF'))
OUT = snapcfg.get('work'); os.makedirs(OUT, exist_ok=True)
P = json.load(open(snapcfg.need('views', 'view-plan JSON')))

d = open(ELFP, 'rb').read()
phoff, = struct.unpack_from('<I', d, 28); nph, = struct.unpack_from('<H', d, 44)
PH = [struct.unpack_from('<8I', d, phoff + 32 * i) for i in range(nph)]
def rd(va, n):
    for p in PH:
        if p[0] == 1 and p[2] <= va and va + n <= p[2] + p[4]:
            return d[p[1] + va - p[2]: p[1] + va - p[2] + n]
    return None

# ---- symbols
name2a, a2name, vts = {}, {}, []
for ln in open(f'{WT}/config/symbol_addrs.txt'):
    m = re.match(r'\s*(\S+)\s*=\s*(0x[0-9A-Fa-f]+);(.*)', ln)
    if not m: continue
    n, a = m.group(1), int(m.group(2), 16)
    if n.startswith('_vt$'):
        sz = re.search(r'size:(0x[0-9A-Fa-f]+)', m.group(3)); vts.append((n, a, int(sz.group(1), 16) if sz else 0))
    elif 'type:func' in m.group(3):
        name2a.setdefault(n, a); a2name.setdefault(a, n)
fstarts = {}
for ln in list(open(f'{WT}/config/functions.csv'))[1:]:
    n, s, e, _ = ln.strip().split(','); s, e = int(s, 16), int(e, 16)
    name2a.setdefault(n, s); a2name.setdefault(s, n); fstarts[s] = e
for ln in list(open(f'{WT}/config/matched.csv'))[1:]:
    f = ln.strip().split(','); name2a.setdefault(f[0], int(f[1], 16))
SORTED = sorted(fstarts)

# ---- class tree from headers (first base)
parent = {}
for root, _, fs in os.walk(f'{WT}/include'):
    for f in fs:
        for m in re.finditer(r'^struct\s+([A-Z]\w*)\s*:\s*(?:public\s+)?([A-Z]\w*)', open(os.path.join(root, f), errors='ignore').read(), re.M):
            parent.setdefault(m.group(1), m.group(2))
for v in P['views']:                     # census type chains fill links the header regex misses
    tc = v.get('type_chain') or []
    for a, b in zip(tc, tc[1:]): parent.setdefault(a, b)
def chain(c):
    out = [c]
    while out[-1] in parent and parent[out[-1]] not in out: out.append(parent[out[-1]])
    return out

# ---- vtables
def demangle_vt(n):
    parts, s = [], n[4:]
    for comp in s.split('$'):
        m = re.match(r'(\d+)(\w+)', comp); parts.append(m.group(2)[:int(m.group(1))] if m else comp)
    return parts
f2cls = collections.defaultdict(set); vtsize = {}
for n, a, sz in vts:
    parts = demangle_vt(n); cls = parts[0]; sec = parts[1] if len(parts) > 1 else None
    if not sec: vtsize[cls] = sz
    b = rd(a, sz or 8)
    for k in range(1, len(b) // 8):
        delta, _, pfn = struct.unpack_from('<hhI', b, 8 * k)
        if pfn: f2cls[pfn].add((cls if (sec is None or delta) else sec, k))

def resolve_set(C):
    """C: set of classes -> (class, how) or None"""
    if not C: return None
    if len(C) == 1: return next(iter(C)), 'unique'
    inter = None
    for c in C:
        s = chain(c); inter = s if inter is None else [x for x in inter if x in s]
    if inter: return inter[0], 'common-base'
    return None
def vt_class(fa):
    r = resolve_set({c for c, _ in f2cls.get(fa, ())})
    return r
def mangled_class(fn):
    m = re.match(r'^(.+?)__(\d+)([A-Za-z_]\w*)', fn or '')
    return m.group(3)[:int(m.group(2))] if m else None
def known_class(fa):
    r = vt_class(fa)
    if r: return r[0], 'vtable'
    mc = mangled_class(a2name.get(fa))
    return (mc, 'mangled') if mc else None

# ---- view bound to 1st arg?
srccache = {}
def bound_first(v):
    src = srccache.setdefault(v['file'], open(f"{WT}/{v['file']}", errors='ignore').read())
    ns = v['namespace']; i = src.find('namespace %s {' % ns) if ns else -1
    if i < 0: return False, 'no-ns'
    j = src.find('\nnamespace ', i + 10); body = src[i: j if j > 0 else len(src)]
    defs = [m for m in re.finditer(r'^[A-Za-z_][^;{}()\n]*\b(\w+)\s*\(([^;{}]*?)\)\s*(?:__asm__\([^)]*\))?\s*\{', body, re.M)
            if m.group(1) not in ('if', 'while', 'for', 'switch')]
    asm_c = function_cname(body, v['function'])
    defs = [m for m in defs if m.group(1) == asm_c] or defs[-1:]
    if not defs: return False, 'no-def'
    m = defs[0]; params = m.group(2).split(',')
    p0 = re.match(r'\s*(?:const\s+)?(?:struct\s+)?(\w+)\s*\*\s*(\w+)', params[0]) if params and params[0].strip() not in ('', 'void') else None
    if not p0: return False, 'no-ptr-arg0'
    if p0.group(1) == v['view']: return True, 'param'
    # local VIEW *x = (VIEW *)p0  (possibly through the first param name)
    fbody = body[m.end(): m.end() + 4000]
    if re.search(r'\b%s\s*\*\s*\w+\s*=\s*\(\s*(?:struct\s+)?%s\s*\*\s*\)\s*%s\b' % (v['view'], v['view'], p0.group(2)), fbody) or \
       re.search(r'\(\s*\(\s*(?:struct\s+)?%s\s*\*\s*\)\s*%s\s*\)\s*->' % (v['view'], p0.group(2)), fbody):
        return True, 'cast'
    return False, 'other'
def function_cname(body, asm):
    m = re.search(r'(\w+)\s*\([^;{}]*\)\s*__asm__\("%s"\)' % re.escape(asm), body)
    return m.group(1) if m else asm

# ---- call graph with a0 pass-through
def func_of(a):
    i = bisect.bisect_right(SORTED, a) - 1
    return SORTED[i] if i >= 0 and a < fstarts[SORTED[i]] else None
WRT_IMM = {0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x18, 0x19, 0x1A, 0x1B, 0x1E, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x37}
calls = collections.defaultdict(list)   # callee -> [(caller, passes_a0)]
text_lo, text_hi = 0x100000, 0x259DF0
code = rd(text_lo, text_hi - text_lo)
for fs in SORTED:
    if not (text_lo <= fs < text_hi): continue
    S = {4}; a = fs; fe = min(fstarts[fs], text_hi)
    def step(w):
        op = w >> 26; rs = (w >> 21) & 31; rt = (w >> 16) & 31; rdr = (w >> 11) & 31; fn = w & 63
        if op == 0 and fn in (0x25, 0x21, 0x2D) and rt == 0 and rdr:   # move rd, rs
            (S.add if rs in S else S.discard)(rdr)
        elif op == 0 and fn not in (0x08, 0x09, 0x18, 0x19, 0x1A, 0x1B, 0x0C, 0x0D, 0x0F) and rdr: S.discard(rdr)
        elif op == 0 and fn == 0x09: S.discard(rdr or 31)
        elif op in WRT_IMM: S.discard(rt)
        elif op == 0x1C and rdr: S.discard(rdr)
    while a < fe:
        w, = struct.unpack_from('<I', code, a - text_lo)
        if w >> 26 == 3:
            tgt = ((a + 4) & 0xF0000000) | ((w & 0x3FFFFFF) << 2)
            ds, = struct.unpack_from('<I', code, a + 4 - text_lo); step(ds)
            calls[tgt].append((fs, 4 in S))
            for r in (2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25, 31): S.discard(r)
            a += 8; continue
        step(w); a += 4

def by_caller(fa):
    cs = calls.get(fa, [])
    known = [(c, known_class(c)) for c, ok in cs if ok]
    known = [(c, k[0]) for c, k in known if k]
    if not known: return None, len(cs), 0
    r = resolve_set({k for _, k in known})
    return (r[0] if r else None), len(cs), len(known)

# ---- run
def classify(v):
    v = dict(v); fn = v['function']; v['function'] = fn[2:] if fn.startswith('M_') else fn
    if fn == '<file>':
        return dict(file=v['file'], line=v['line'], function=fn, view=v['view'], milestone=v['milestone'],
                    candidate_type=v.get('candidate_type'), real_type=v.get('real_type'), status='unresolved', why='file-scope-view')
    fa = name2a.get(v['function'])
    m = re.match(r'(?:func|FUN)_([0-9A-Fa-f]{8})$', v['function'])
    if fa is None and m: fa = int(m.group(1), 16)
    rec = dict(file=v['file'], line=v['line'], function=v['function'], view=v['view'], milestone=v['milestone'],
               candidate_type=v.get('candidate_type'), real_type=v.get('real_type'), addr=hex(fa) if fa else None)
    if fa is None: rec.update(status='unresolved', why='no-address'); return rec
    b, how = bound_first(v); rec['bound'] = how
    vc = vt_class(fa)
    rec['fn_vtable_class'] = vc[0] if vc else None
    if not b: rec.update(status='unresolved', why='view-not-arg0' + ('(fn-class-known)' if vc else '')); return rec
    if vc: rec.update(status='vtable', cls=vc[0], how=vc[1]); return rec
    mc = mangled_class(v['function'])
    if mc: rec.update(status='mangled', cls=mc); return rec
    c, ncall, nk = by_caller(fa)
    rec.update(ncallers=ncall, nknown_passing=nk)
    if c: rec.update(status='caller', cls=c); return rec
    rec.update(status='unresolved', why='no-known-caller' if not nk else 'callers-disagree'); return rec

views = P['views']
C = [classify(v) for v in views if v['fix'] == 'c']
AB = [classify(v) for v in views if v['fix'] in ('a', 'b', 'd') and v.get('real_type')]

def rel(r, t):
    if r == t: return 'exact'
    if t in chain(r): return 'view-is-base-of-inferred'
    if r in chain(t): return 'view-more-derived'
    return 'mismatch'
acc = collections.Counter(rel(x['cls'], x['real_type']) + '/' + x['status'] for x in AB if x.get('cls'))
cand = collections.Counter(rel(x['cls'], x['candidate_type']) for x in C if x.get('cls') and x['candidate_type'])
tot = collections.Counter(x['status'] for x in C)
perm = collections.defaultdict(collections.Counter)
for x in C: perm[x['milestone']][x['status']] += 1
why = collections.Counter(x.get('why') for x in C if x['status'] == 'unresolved')
bound = collections.Counter(x.get('bound') for x in C)
loose = sum(1 for x in C if x['status'] == 'unresolved' and x.get('fn_vtable_class'))
random.seed(1)
res = [x for x in C if x.get('cls')]
sample = random.sample(res, min(15, len(res)))
abres = [x for x in AB if x.get('cls')]
check = random.sample(abres, min(10, len(abres)))
json.dump(dict(totals=tot, per_milestone=perm, unresolved_why=why, bound=bound, accuracy_on_known=acc,
               agreement_with_candidate_type=cand, vtables=len(vts), funcs_in_vtables=len(f2cls),
               type_c=C, known_check=AB), open(f'{OUT}/vtjoin.json', 'w'), indent=1, default=dict)

L = ['# vtjoin: mechanical class assignment for type-c views', '',
     f'Tree {WT}, ELF {os.path.basename(ELFP)}. {len(vts)} `_vt$` tables parsed (incl. secondary `$GRABSPOT` tables), '
     f'{len(f2cls)} distinct function addresses in vtables; {len(parent)} parent links from include/ headers. Script: vtjoin.py.', '',
     'Layout (empirical, CID152/BASIC): entry 0 = header {0,0}; slot k at +8k = {short delta, short pad, u32 pfn}; slot 6 = GetClassInfo.', '',
     '## Type-c views (%d)' % len(C), '', '| status | count |', '|---|---|']
L += [f'| {k} | {n} |' for k, n in tot.most_common()]
L += ['', 'status: vtable = function is in vtable(s), class = unique or deepest common base; mangled = member-function mangled name; '
      'caller = all known-class callers that pass their own a0 through agree; unresolved otherwise.', '',
      'View binding to arg0 (required for any of the above): ' + ', '.join(f'{k}: {n}' for k, n in bound.most_common()), '',
      f'Unresolved reasons: ' + ', '.join(f'{k}: {n}' for k, n in why.most_common()), '',
      f'Loose upper bound: {loose} more views live in a function with a vtable class but are not (detectably) its `this`.', '',
      '## Per milestone', '', '| M | ' + ' | '.join(sorted(tot)) + ' |', '|---' * (len(tot) + 1) + '|']
for m in sorted(perm): L.append(f'| {m} | ' + ' | '.join(str(perm[m][k]) for k in sorted(tot)) + ' |')
L += ['', '## Accuracy against views whose type is known (fix a/b/d, real_type)', '',
      f'{len(abres)} of {len(AB)} known views get a class by the same method. relation(inferred, real):', '']
L += [f'- {k}: {n}' for k, n in acc.most_common()]
L += ['', 'Agreement of type-c inferences with ptr64 candidate_type: ' + ', '.join(f'{k}: {n}' for k, n in cand.most_common()), '',
      '## Spot-check sample (15 type-c resolved)', '', '| function | view | class | via | candidate |', '|---|---|---|---|---|']
L += [f"| {x['function']} | {x['view']} | {x['cls']} | {x['status']} | {x['candidate_type']} |" for x in sample]
L += ['', '## 10 known-type checks', '', '| function | view | real | inferred | via | relation |', '|---|---|---|---|---|---|']
L += [f"| {x['function']} | {x['view']} | {x['real_type']} | {x['cls']} | {x['status']} | {rel(x['cls'], x['real_type'])} |" for x in check]
L += ['', '## Findings', '',
  '- The bottleneck is not class identification but `this`-binding: only ~81 of 536 type-c views are the function\'s first argument '
  '(declared `VIEW *` arg0, or cast from it). 138 are file-scope views (no single function), ~290 are reached through a field of '
  'another object or a global (e.g. path.cpp RemovePathlinkHierarchy: `((O *)g_sw.pregtree)->n`), so a vtable join cannot type them.',
  '- Of the arg0-bound views, the vtable join + 1-level caller propagation types 24; the remaining ~57 are non-virtual helpers with no '
  'known-class caller passing a0 through (callers are themselves unresolved helpers; deeper propagation would add some).',
  '- Accuracy proxy (same method on 463 views with known real_type): 29 typed; 26 consistent (exact, or the view is a base of the '
  'inferred class); 3 "mismatch": PATHLINK vs LO and ROBOTTANK vs SO look like missing parent links in the header-derived tree, '
  'EMITTER vs EMITB is a genuine miss (view is an embedded member struct bound as arg0). Treat ~90% as an optimistic estimate on n=29.',
  '- ptr64 candidate_type disagrees with all 6 type-c inferences that have one; candidate_type is a size heuristic, not ground truth.',
  '- Next mechanical lever: field-type propagation (view = cast of X->field where X is typed and field\'s header type is a class), '
  'which addresses the ~290 field-reached views; not attempted here.']
open(f'{OUT}/vtjoin_REPORT.md', 'w').write('\n'.join(L) + '\n')
print('\n'.join(L[:40]))
