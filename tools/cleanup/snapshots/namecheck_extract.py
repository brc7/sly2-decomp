"""Pass 1: per snapshot, per header class: raw words, word kinds and pointee labels for up to CAP live instances.
Also typed globals, and for player/guard classes the SOPHYS block behind SO::physics (+0xE8).
Output: WORK/namecheck/cache/<snap>.npz (game-derived data; work directory only). Needs objects/ from step 1.
Run through namecheck.py."""
import sys, os, json, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import numpy as np
import snapcfg
if __name__ == '__main__': snapcfg.parse(__doc__, lambda ap: ap.add_argument('only', nargs='*', help='snapshot names to (re)extract'))
import common as C

OUT = os.path.join(C.WORK, 'namecheck', 'cache'); os.makedirs(OUT, exist_ok=True)
CAP, RAMTOP = 24, 0x2000000
GLOBALS = {'g_clock': ('CLOCK', 0x2DDED0), 'g_cm': ('CM', 0x2DDF40), 'g_game': ('GAME', 0x2DEB30),
           'g_gs': ('SAVEDATA', 0x3D4A60), 'g_joy': ('JOY', 0x2DFC00), 'g_save': ('SAVE', 0x2E65E0),
           'g_sndmgr': ('SNDMGR', 0x3DD040), 'g_sw': ('SW', 0x3DD490)}
KN = ['zero', 'vptr', 'obj', 'objint', 'code', 'global', 'heap', 'int', 'negint', 'float', 'other']
DEREF = {0xE8: 224}   # SO::physics -> SOPHYS

def main():
    vts = C.find_vtables(); vlabel = {a: v['label'] for a, v in vts.items()}
    LN = sorted(set(vlabel.values())); LID = {l: i for i, l in enumerate(LN)}
    vset = np.array(sorted(vts), dtype=np.int64)
    json.dump(LN, open(os.path.join(OUT, 'labels.json'), 'w'))
    CODE_END = max(C.FUNCS) + 0x400
    def size_of(l):
        return C.LAYOUT[l]['size'] if l in C.LAYOUT and C.LAYOUT[l]['size'] else 0x40
    only = set(snapcfg.parse(__doc__).only)
    for name, path, h in C.snapshots():
        if only and name not in only: continue
        fn = os.path.join(OUT, name + '.npz')
        if os.path.exists(fn) and not only: continue
        ram = C.load_ram(path)
        objs = np.load(os.path.join(C.WORK, 'objects', name + '.npy'))
        objs = objs[np.argsort(objs[:, 0])]
        oaddr = objs[:, 0]; olab = np.array([LID[vlabel[int(v)]] for v in objs[:, 1]])
        osize = np.array([size_of(LN[i]) for i in olab], dtype=np.int64)
        def classify(M):
            zero = M == 0; isvt = np.isin(M, vset)
            pos = np.searchsorted(oaddr, M, side='right') - 1; posc = np.clip(pos, 0, len(oaddr) - 1)
            exact = (pos >= 0) & (oaddr[posc] == M)
            interior = (pos >= 0) & ~exact & (M < oaddr[posc] + osize[posc]) & (M > oaddr[posc])
            inimg = (M >= 0x100000) & (M < C.IMAGE_END); code = inimg & (M < CODE_END)
            heap = (M >= C.IMAGE_END) & (M < RAMTOP) & (M % 4 == 0)
            sm = (M > 0) & (M < 0x10000); neg = M >= 0xFFFF0000
            fv = M.astype(np.uint32).view(np.float32); af = np.abs(fv)
            fl = np.isfinite(fv) & (af > 1e-5) & (af < 1e7)
            K = np.select([zero, isvt, exact, interior, code, inimg, heap, sm, neg, fl], list(range(10)), default=10)
            T = np.where(exact | interior, olab[posc], -1)
            T = np.where(isvt, -2, T)
            return K.astype(np.int8), T.astype(np.int16)
        D = {}
        for l, idx in collections.defaultdict(list, {}).items(): pass
        by = collections.defaultdict(list)
        for i, li in enumerate(olab): by[int(li)].append(i)
        for li, idx in by.items():
            lab = LN[li]
            if lab not in C.LAYOUT or not (C.LAYOUT[lab]['fields'] or C.LAYOUT[lab].get('base')): continue
            idx = np.array(idx); lst = idx[objs[idx, 2] == 1]
            if len(lst): idx = lst
            if len(idx) > CAP: idx = idx[np.linspace(0, len(idx) - 1, CAP).astype(int)]
            S = size_of(lab); nw = S // 4
            base = oaddr[idx]; base = base[base + S <= RAMTOP]
            if not len(base): continue
            M = ram[(base[:, None] >> 2) + np.arange(nw)[None, :]].astype(np.int64)
            K, T = classify(M)
            D['M_' + lab] = M.astype(np.uint32); D['K_' + lab] = K; D['T_' + lab] = T; D['A_' + lab] = base
            if (lab in ('JT', 'MURRAY', 'BENTLEY') or 'GUARD' in lab) and nw > 0x3B:
                for off, sz in DEREF.items():
                    p = M[:, off // 4]
                    ok = (p >= C.IMAGE_END) & (p + sz < RAMTOP) & (p % 4 == 0)
                    R = np.zeros((len(p), sz // 4), dtype=np.uint32)
                    for r in np.nonzero(ok)[0]: R[r] = ram[p[r] // 4: p[r] // 4 + sz // 4]
                    D['P%X_%s' % (off, lab)] = R
        for g, (ty, a) in GLOBALS.items():
            S = C.LAYOUT[ty]['size']; M = ram[a // 4: a // 4 + S // 4].astype(np.int64)[None, :]
            K, T = classify(M)
            D['M_' + g] = M.astype(np.uint32); D['K_' + g] = K; D['T_' + g] = T; D['A_' + g] = np.array([a])
        np.savez_compressed(fn, **D)
        print(name, len([k for k in D if k.startswith('M_')]), flush=True)

if __name__ == '__main__':
    main()
