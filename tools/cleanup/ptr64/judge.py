#!/usr/bin/env python3
"""judge.py FACTS.json OUT.json [--snap-dir DIR] [--views F] [--evidence F | --no-evidence] -- decide 64-bit
danger per view (see dt.py for facts).

Model: an accessed direct field f of view V sits at the right 64-bit offset iff, over the bytes from the end of V's
real (header) bases to the end of f, V declares a wide leaf (pointer/long) exactly where the real object R has one.
R's wide slots: V's bases (exact, they are header types); the object's class C when known: layouts.json slot =
pointer if the header says so or pointer kinds are >= 90% of nonzero samples; else C's header (clang dump).
Unknown class: bytes V types explicitly are trusted; an aligned word fully inside a pad is unknown -> 'unverifiable'.
Statuses: dangerous (an accessed field moves on 64-bit), unverifiable, safe, unused (no field accessed).

Inputs: FACTS.json from dt.py; --snap-dir = the snapshot pipeline's work dir (classes.json, layouts.json,
view_resolution.json; default build/snapshots); --views = view-plan JSON (list or {"views": [...]}, records with
file/view/namespace/real_type; optional: supplies the census label and the in_census flag); --evidence = hand
judgements {"FILE::VIEW": {"cls": CLASS} | {"nonptr": true, "why": "..."}} for views the snapshots cannot settle
(a record whose pad bytes hold no pointers: data/file/hardware records). Keep the evidence file under version
control next to the work it justifies, each entry with a "why".
"""
import argparse, json, os, sys, re, collections
_REPO = os.path.realpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..'))
_ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
_ap.add_argument('facts'); _ap.add_argument('out')
_ap.add_argument('--snap-dir', default=os.environ.get('SNAP_WORK', os.path.join(_REPO, 'build', 'snapshots')))
_ap.add_argument('--views', default=os.environ.get('SNAP_VIEWS'))
_ap.add_argument('--evidence', default=os.path.join(os.path.dirname(os.path.abspath(__file__)), 'evidence.json'), help='default: evidence.json next to this script'); _ap.add_argument('--no-evidence', action='store_true')
_A = _ap.parse_args()
F = json.load(open(_A.facts)); OUT = _A.out
EV = json.load(open(_A.evidence)) if _A.evidence and os.path.exists(_A.evidence) and not _A.no_evidence else {}   # (file::view) -> {cls|nonptr, why}
V = F['views']; H = F['headers']
LAY = json.load(open(os.path.join(_A.snap_dir, 'layouts.json')))['classes']
CLS = json.load(open(os.path.join(_A.snap_dir, 'classes.json')))['classes']
_vr = os.path.join(_A.snap_dir, 'view_resolution.json')
VR = json.load(open(_vr))['views'] if os.path.exists(_vr) else []
P64 = json.load(open(_A.views)) if _A.views else []
if isinstance(P64, dict): P64 = P64.get('views', [])
PTRK = {'obj', 'ptr', 'heap', 'objint', 'vptr', 'global', 'code'}
BYCID = {c['cid']: k for k, c in CLS.items() if c.get('cid') is not None}

def chain(c):
    out = [c]
    while c in CLS and CLS[c].get('parent') is not None and BYCID.get(CLS[c]['parent']) not in out + [None]:
        c = BYCID[CLS[c]['parent']]; out.append(c)
    return out[::-1]

def hdr_chain(c):
    out = [c]
    while c in H and H[c]['bases']:
        c = H[c]['bases'][0][1]; out.append(c)
    return out[::-1]

def milestone(f, fn):
    p = f[len('src/game/'):]
    early = bool(re.search(r'FromBrx|^Load|PostLoad|^Init|^New[A-Z]|^Construct|Load$|^Bind', fn or ''))
    if p.startswith(('platform/', 'core/', 'object/', 'level/', 'app/', 'splice/', 'world/scene/', 'world/state/game',
                     'world/state/difficulty', 'engine/physics/so.cpp', 'engine/graphics/pipeline/glob', 'engine/graphics/pipeline/shd')):
        return 1
    if early: return 1
    if p.startswith(('engine/graphics/', 'engine/presentation/', 'engine/font/', 'engine/camera/')): return 2
    if p.startswith(('engine/sound/', 'ui/', 'gameplay/cutscene/mpeg', 'gameplay/cutscene/framecapture', 'world/state/save')): return 4
    return 3

def slot_ptr(s):
    h = s.get('header') or {}
    if h.get('isptr'): return True
    return s['kind'] in PTRK and s['conf'] >= 0.9 and s.get('nonzero', 1) > 0

def vshift(c):
    """clang puts a vptr first; g++ 2.95 after the root's own members. Only BASIC roots give the same pointer slots."""
    hc = hdr_chain(c) if c in H else [c]
    root = hc[0]
    return root in H and any(n == '__vptr' for (n, o, t) in H[root]['direct']) and root != 'BASIC'

TAGGED = {'LO': [0x40]}   # header unions of a flag word and a pointer (lo.h LO+0x40 unknown40/ploupd): pointer-sized on 64-bit
def class_slots(c):
    k = class_slots0(c)
    if not k: return k
    extra = set(o for b, offs in TAGGED.items() if b in (hdr_chain(c) if c in H else [c]) or b == c for o in offs if o < k[0])
    return k[0], k[1] | extra, k[2]

def class_slots0(c):
    """-> (size, set of wide offsets, source) or None"""
    if vshift(c):
        if c not in LAY: return None
        L = LAY[c]
        return L['size'], set(s['off'] for s in L['slots'] if s['kind'] in PTRK and s['conf'] >= 0.9 and s.get('nonzero', 1) > 0), 'layouts-observed'
    if c in LAY:
        L = LAY[c]
        return L['size'], set(s['off'] for s in L['slots'] if slot_ptr(s)), 'layouts'
    if c in H:
        return H[c]['size'], set(H[c]['wide']), 'header'
    return None

# class evidence per view
vr_idx = collections.defaultdict(list)
for x in VR: vr_idx[(x['file'], x['view'])].append(x)
p64_idx = collections.defaultdict(list)
for x in P64: p64_idx[(x['file'], x['view'])].append(x)

def view_base_end(v):
    be = 0
    for off, b in v['bases']:
        if b in H: be = max(be, off + H[b]['size'])
    return be

def consistent(v, c):
    """the view's own pointer leaves (past its header bases) all land on pointer slots of class c"""
    k = class_slots(c)
    own = set(o for o in v['wide'] if o >= view_base_end(v))
    return bool(k and own and own <= k[1])

def basic_vptr(v):
    """a hand vtable pointer at +0xC with only pad bytes before it: the object is-a BASIC (vptr after 3 pointer members)"""
    d = v['direct']
    if v['bases']: return False
    vt = [x for x in d if x['off'] == 0xC and x['wide'] and re.search(r'vt|vtable|VT', x['name'] + ' ' + x['type'])]
    if not vt: return False
    return all(x['off'] >= 0xC or (not x['wide'] and re.match(r'^(char|unsigned char|uchar|u8)\[', x['type'])) for x in d)

def evidence(v):
    e = EV.get(v['file'] + '::' + v['name'])
    if e and e.get('cls'): return e['cls'], None, None, 'manual'
    ns, _, nm = v['name'].rpartition('::')
    ex = None; bound = None; how = None
    for x in vr_idx.get((v['file'], nm), []):
        fn = x['function'] or ''
        if ns and not fn.startswith(ns) and not ns.startswith(fn.split('__')[0]): continue
        if not ns and fn != '<file>': continue
        fin = x.get('final')
        if not fin: continue
        meths = x.get('methods', {})
        good = [m for m in ('a', 'b', 'c') if m in meths and not (m == 'c' and meths[m].get('ambiguous'))]
        if fin['conf'] != 'conflict' and (fin['method'] == 'a' or fin['conf'] == 'high' or (fin['conf'] == 'medium' and fin['method'] in ('b', 'c') and consistent(v, fin['cls']))):
            ex = fin['cls']; how = 'snapshot-' + fin['method']
        else:
            bound = fin['cls']; how = 'bound-' + fin['method']
        break
    lab = None
    for x in p64_idx.get((v['file'], nm), []):
        if (x['namespace'] or '') == ns and x.get('real_type'): lab = x['real_type']; break
    return ex, bound, lab, how

def judge(v):
    acc = set(v['accessed'])
    if v.get('haslong') and any(x['name'] in acc for x in v['direct']):
        return 'unverifiable', [('*', 'long/u64 member: EE long is 8 bytes, the x32 layout is not the PS2 one')], {'cls': None, 'src': None, 'bound': None, 'base_end': 0}
    d = [x for x in v['direct'] if x['name'] in acc]
    if not d: return 'unused', [], None
    base_end = 0
    for off, b in v['bases']:
        if b in H: base_end = max(base_end, off + H[b]['size'])
    ex, bound, lab, how = evidence(v)
    vb = v['bases'][0][1] if v['bases'] else None
    if not vb and basic_vptr(v): vb = 'BASIC'
    nm = v['name'].rpartition('::')[2]
    nmn = re.sub(r'(Island|V)$', '', nm)
    if lab and not ex and lab != nmn:
        k = class_slots(lab)
        own = set(o for o in v['wide'] if o >= base_end)
        lab = None   # census layout labels proved unreliable (DZG for BASIC-family dialogs/SAA): not trusted   # census label without layout evidence: not trusted
    cls = ex or lab
    src = how if ex else (('census-name' if lab == nmn else 'census-layout') if lab else None)
    know = class_slots(cls) if cls else None
    bknow = None
    if not know:
        for b in [vb]:
            if b and class_slots(b): bknow = (b, class_slots(b)); break
    vw = set(v['wide'])
    # extent of each direct field = up to next direct field / sizeof
    offs = sorted(set(x['off'] for x in v['direct']) | {v['size']})
    pads = [x for x in v['direct'] if re.match(r'^(pad|unk|_pad|unknown|padding|filler|gap|res|reserved|_)', x['name'], re.I) or (x['type'].startswith(('char[', 'unsigned char[', 'uchar[', 'u8[', 's8[', 'int[', 'u32[')) and not x['wide'])]
    padw = set()
    for p in pads:
        end = next(o for o in offs if o > p['off'])
        padw |= set(range((p['off'] + 3) // 4 * 4, end - 3, 4))
    e = EV.get(v['file'] + '::' + v['name'])
    if e and e.get('nonptr'): padw = set()   # the object's pad bytes hold no pointers (data/file/hardware record; evidence.json)
    probs = []; unk = []
    for f in d:
        end = next(o for o in offs if o > f['off'])
        lo, hi = base_end, end
        if know:
            size, rw, _ = know
            rwin = set(o for o in rw if lo <= o < hi)
            vwin = set(o for o in vw if lo <= o < hi)
            if hi > size + 3 and cls not in H: unk.append((f['name'], 'view past sizeof(%s)' % cls))
            if rwin != vwin: probs.append((f['name'], sorted(rwin - vwin), sorted(vwin - rwin)))
        else:
            # V's typed bytes trusted; pad words unknown unless covered by a bound's knowledge
            bw = set(); bsize = 0
            if bknow: bsize, bw, _ = bknow[1]
            uw = [o for o in padw if lo <= o < hi and o >= bsize]
            kn = set(o for o in bw if lo <= o < min(hi, bsize))
            vwin = set(o for o in vw if lo <= o < min(hi, bsize))
            if bknow and kn != vwin: probs.append((f['name'], sorted(kn - vwin), sorted(vwin - kn)))
            if uw: unk.append((f['name'], 'pad words %s..%s unknown' % (hex(min(uw)), hex(max(uw)))))
    info = dict(cls=cls, src=src, bound=(bknow[0] if bknow else None), base_end=base_end)
    if probs: return 'dangerous', probs, info
    if unk: return 'unverifiable', unk, info
    return 'safe', [], info

# arrays of records in a view: clang dumps only the array row, so add the element's pointer leaves per element
_byfile = collections.defaultdict(dict)
for _v in V: _byfile[_v['file']][_v['name']] = _v
def rec_wide(fileviews, ns, tname):
    for q in ([ns + '::' + tname] if ns else []) + [tname]:
        r = fileviews.get(q)
        if r: return r['size'], r['wide']
    h = H.get(tname)
    return (h['size'], h['wide']) if h else None
for _v in V:
    ns = _v['name'].rpartition('::')[0]
    for x in _v['direct']:
        m = re.match(r'^(?:struct |union )?([A-Za-z_][\w:]*) ?\[(\d+)\]$', x['type'])
        if not m or m.group(1) in ('int', 'char', 'float', 'short', 'uint', 'u8', 'u32', 'uchar', 'byte', 'unsigned'): continue
        rw = rec_wide(_byfile[_v['file']], ns, m.group(1))
        if not rw or not rw[1]: continue
        add = [x['off'] + k * rw[0] + w for k in range(int(m.group(2))) for w in rw[1]]
        _v['wide'] = sorted(set(_v['wide']) | set(add))

res = []
for v in V:
    st, why, info = judge(v)
    res.append(dict(file=v['file'], view=v['name'], line=v['line'], status=st, why=why, info=info,
                    milestone=milestone(v['file'], v['name'].split('::')[0] if '::' in v['name'] else v.get('func')),
                    in_census=bool(p64_idx.get((v['file'], v['name'].rpartition('::')[2])))))
json.dump(res, open(OUT, 'w'), indent=0)
c = collections.Counter((r['milestone'], r['status']) for r in res if r['in_census'])
print('census views (--views set, still present):')
for m in (1, 2, 3, 4): print(' M%d' % m, {s: c[(m, s)] for s in ('dangerous', 'unverifiable', 'safe', 'unused')})
c2 = collections.Counter(r['status'] for r in res)
print('all main-file records with a computed layout:', dict(c2))
cm = collections.Counter((r['milestone'], r['status']) for r in res)
print('all records by milestone:')
for m in (1, 2, 3, 4): print(' M%d' % m, {s: cm[(m, s)] for s in ('dangerous', 'unverifiable', 'safe', 'unused')})
