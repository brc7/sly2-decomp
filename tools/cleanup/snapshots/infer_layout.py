#!/usr/bin/env python3
"""Step 2: field typing per class from the objects of step 1.
For every 4-byte slot of each class's instance (size: class descriptor, else header sizeof, else 0x40), over the
instances of all snapshots (the world-listed ones where the class has any, at most CAP per snapshot), classify the
word: zero, vptr, object pointer (exact base, target class from its vptr), interior pointer into an object
(class+offset), code pointer, pointer into a global/static symbol, other heap pointer, small int, -1, float
(plausible range), other (bitfield/hash). Merge with the header's flattened fields; write layouts.json."""
import os, sys, json, bisect, collections
import numpy as np
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import snapcfg
if __name__ == '__main__': snapcfg.parse(__doc__)
import common as C
from snapshot_objects import hdr_chain, vptr_off

CAP = 300
RAMTOP = 0x2000000
UNKNOWN_PREFIX = ('pad', '_pad', 'unknown', 'unk', 'fUnk', 'field_', 'gUnk', 'pUnk', 'nUnk')

def class_chain(label, cid):
    ch = hdr_chain(label) if label in C.LAYOUT else [label]
    if cid is not None:
        for k in C.cid_chain(cid):
            n = C.cname(k)
            if n not in ch: ch.append(n)
    return ch
def lca(labels, cl):
    chains = [class_chain(l, cl[l]['cid'] if l in cl else None) for l in labels]
    if not chains: return None
    inter = chains[0]
    for c in chains[1:]: inter = [x for x in inter if x in c]
    return inter[0] if inter else None

def sym_of(w):
    i = bisect.bisect_right(SYMA, w) - 1
    if i < 0: return None
    a = SYMA[i]; n = C.SYM[a]
    return n if w - a < max(C.SYMSIZE.get(a, 0), 1) or w == a else n + '+?'

SYMA = sorted(a for a in C.SYM if 0x100000 <= a < C.IMAGE_END)
CODE_END = max(C.FUNCS) + 0x400

def is_unknown_name(n):
    last = n.split('.')[-1]
    return last.startswith(UNKNOWN_PREFIX) or last.lower().startswith(('pad', 'unknown', '_pad'))

def header_slots(label):
    """offset -> (name, type, isptr, known) for each 4-byte word the header's flattened fields cover."""
    out = {}
    if label not in C.LAYOUT: return out
    for off, sz, nm, ty, isp in C.flat_fields(label):
        for o in range(off - off % 4, off + max(sz, 1), 4):
            if o in out and out[o][3]: continue
            out[o] = (nm, ty, isp, not is_unknown_name(nm) and ty not in ('undefined4', 'undefined1'))
    return out

def main():
    CL = json.load(open(os.path.join(C.WORK, 'classes.json')))
    cl = CL['classes']
    vts = C.find_vtables(); vlabel = {a: v['label'] for a, v in vts.items()}
    vset = np.array(sorted(vts), dtype=np.uint32)
    def size_of(lab):
        c = cl[lab]
        if c.get('size'): return c['size'], 'classdesc'
        if c.get('header_size'): return c['header_size'], 'header'
        return 0x40, 'default'
    LNAMES = sorted(set(vlabel.values())); LID = {l: i for i, l in enumerate(LNAMES)}
    acc = {}       # label -> per-slot Counter of kinds, Counter of targets
    ninst = collections.Counter(); nsnap = collections.Counter()
    for name, path, h in C.snapshots():
        ram = C.load_ram(path)
        objs = np.load(os.path.join(C.WORK, 'objects', name + '.npy'))
        if not len(objs): continue
        order = np.argsort(objs[:, 0]); objs = objs[order]
        oaddr = objs[:, 0]; olab = [vlabel[int(v)] for v in objs[:, 1]]
        labidx = np.array([LID[l] for l in olab])
        osize = np.array([size_of(l)[0] for l in olab], dtype=np.int64)
        bylab = collections.defaultdict(list)
        for i, l in enumerate(olab): bylab[l].append(i)
        for lab, idx in bylab.items():
            idx = np.array(idx)
            lst = idx[objs[idx, 2] == 1]
            if len(lst): idx = lst
            if len(idx) > CAP: idx = idx[np.linspace(0, len(idx) - 1, CAP).astype(int)]
            S, _ = size_of(lab); nw = S // 4
            base = oaddr[idx]
            base = base[base + S <= RAMTOP]
            if not len(base): continue
            M = ram[(base[:, None] >> 2) + np.arange(nw)[None, :]].astype(np.int64)   # N x nw
            ninst[lab] += len(base); nsnap[lab] += 1
            A = acc.setdefault(lab, [dict(k=collections.Counter(), t=collections.Counter(), s=collections.Counter(),
                                          f=[], i=[]) for _ in range(nw)])
            # vectorized kinds
            zero = M == 0
            isvt = np.isin(M, vset)
            pos = np.searchsorted(oaddr, M, side='right') - 1
            posc = np.clip(pos, 0, len(oaddr) - 1)
            exact = (pos >= 0) & (oaddr[posc] == M)
            interior = (pos >= 0) & ~exact & (M < oaddr[posc] + osize[posc]) & (M > oaddr[posc])
            inimg = (M >= 0x100000) & (M < C.IMAGE_END)
            code = inimg & (M < CODE_END)
            heap = (M >= C.IMAGE_END) & (M < RAMTOP) & (M % 4 == 0)
            sm = (M > 0) & (M < 0x10000)
            neg = (M >= 0xFFFF0000)
            fv = M.astype(np.uint32).view(np.float32)
            af = np.abs(fv)
            fl = np.isfinite(fv) & (af > 1e-5) & (af < 1e7)
            KN = ['zero', 'vptr', 'obj', 'objint', 'code', 'global', 'heap', 'int', 'negint', 'float', 'other']
            K = np.select([zero, isvt, exact, interior, code, inimg, heap, sm, neg, fl],
                          list(range(10)), default=10).astype(np.int64)
            for kc in range(11):
                cnt = (K == kc).sum(axis=0)
                for j in np.nonzero(cnt)[0]: A[j]['k'][KN[kc]] += int(cnt[j])
            for kc in (1, 2, 3, 4, 5):
                rr, jj = np.nonzero(K == kc)
                if not len(rr): continue
                if kc == 1: keys = [vlabel[int(w)] for w in M[rr, jj]]
                elif kc == 2: keys = [LNAMES[labidx[p]] for p in posc[rr, jj]]
                elif kc == 3: keys = ['%s+0x%X' % (LNAMES[labidx[p]], int(w) - int(oaddr[p])) for p, w in zip(posc[rr, jj], M[rr, jj])]
                elif kc == 4: keys = [C.FUNCS.get(int(w), 'fn?') for w in M[rr, jj]]
                else: keys = [sym_of(int(w)) for w in M[rr, jj]]
                tgt = 't' if kc <= 3 else 's'
                for j, key in collections.Counter(zip(jj.tolist(), keys)).items():
                    A[j[0]][tgt][j[1]] += key
            for kc, fld in ((7, 'i'), (8, 'i'), (9, 'f')):
                rr, jj = np.nonzero(K == kc)
                for r, j in zip(rr[:4000], jj[:4000]):
                    lst = A[j][fld]
                    if len(lst) < 64:
                        lst.append(float(fv[r, j]) if kc == 9 else int(M[r, j]) - (1 << 32 if kc == 8 else 0))
        print(name, len(bylab), flush=True)
    # ---- propose
    out = {}
    PTRK = ('obj', 'objint', 'global', 'heap', 'code', 'vptr')
    agree = collections.Counter()
    for lab, A in acc.items():
        S, src = size_of(lab); hs = header_slots(lab)
        slots = []
        for j, a in enumerate(A):
            off = 4 * j; k = a['k']; n = sum(k.values()); nz = n - k['zero']
            if nz == 0: kind, conf = 'zero', 1.0
            else:
                ptr = sum(k[x] for x in PTRK)
                top, tc = max(((x, k[x]) for x in k if x != 'zero'), key=lambda t: t[1])
                if ptr / nz >= 0.9 and top in ('obj', 'objint', 'heap') and ptr > tc: top, tc = 'ptr', ptr
                kind, conf = top, round(tc / nz, 3)
            ent = dict(off=off, kind=kind, conf=conf, n=n, nonzero=nz, kinds=dict(k))
            if a['t']:
                ent['targets'] = dict(a['t'].most_common(6))
                if kind in ('obj', 'ptr'):
                    ent['target_lca'] = lca([t for t in a['t'] if '+0x' not in t], cl)
            if a['s']: ent['syms'] = dict(a['s'].most_common(4))
            if a['f']: ent['float_range'] = [round(min(a['f']), 4), round(max(a['f']), 4)]
            if a['i']: ent['int_values'] = sorted(set(a['i']))[:12]
            if off in hs:
                nm, ty, isp, known = hs[off]
                ent['header'] = dict(name=nm, type=ty, isptr=isp, known=known)
                if known and nz >= 5:
                    ip = kind in ('obj', 'objint', 'ptr', 'heap', 'global', 'code', 'vptr')
                    agree['ptr' if isp else 'nonptr', 'inferred_ptr' if ip else 'inferred_nonptr'] += 1
            slots.append(ent)
        # proposed new header fields: slots the header doesn't name, with a consistent non-zero kind
        props = []
        for e in slots:
            h = e.get('header')
            if h and h['known']: continue
            if e['kind'] == 'zero' or e['nonzero'] < 3 or e['conf'] < 0.8: continue
            if e['off'] == vptr_off(lab) and e['kind'] == 'vptr': continue
            t = {'obj': (e.get('target_lca') or '?') + ' *', 'ptr': (e.get('target_lca') or 'void') + ' * (mixed)',
                 'objint': 'interior ptr ' + next(iter(e.get('targets', {'?': 0}))), 'heap': 'void * (heap)',
                 'global': 'ptr to ' + str(next(iter(e.get('syms', {'?': 0})))), 'code': 'pfn',
                 'float': 'float', 'int': 'int/enum', 'negint': 'int (-1 sentinel)', 'other': 'bits/hash',
                 'vptr': 'vptr'}[e['kind']]
            props.append(dict(off=e['off'], inferred=t, conf=e['conf'], evidence=e['nonzero'],
                              header=h['name'] if h else None))
        out[lab] = dict(label=lab, cid=cl[lab]['cid'], size=S, size_source=src, in_headers=lab in C.LAYOUT,
                        instances=ninst[lab], snapshots=nsnap[lab], slots=slots, proposed_fields=props,
                        header_unknown_words=sum(1 for j in range(len(A)) if not (4 * j in hs and hs[4 * j][3])))
    json.dump(dict(classes=out, header_ptr_agreement={'%s/%s' % k: v for k, v in agree.items()}),
              open(os.path.join(C.WORK, 'layouts.json'), 'w'), indent=0)
    print('classes', len(out), 'agreement', dict(agree))

if __name__ == '__main__':
    main()
