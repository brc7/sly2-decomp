"""Pass 2: name/type-implied kind vs observed values per declared field (aggregated over every live class containing
it), plus naming suggestions for unknown words. Output: WORK/namecheck/namecheck.json (checks 1 and 3). Run through namecheck.py."""
import sys, os, re, json, glob, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import numpy as np
import snapcfg
snapcfg.parse(__doc__)
import common as C

HERE = os.path.join(C.WORK, 'namecheck'); CACHE = os.path.join(HERE, 'cache')
LN = json.load(open(os.path.join(CACHE, 'labels.json'))); LID = {l: i for i, l in enumerate(LN)}
KN = ['zero', 'vptr', 'obj', 'objint', 'code', 'global', 'heap', 'int', 'negint', 'float', 'other']
PTRK = {1, 2, 3, 4, 5, 6}
L = C.LAYOUT
CL = json.load(open(os.path.join(C.WORK, 'classes.json')))['classes']
UNK = ('pad', '_pad', 'unknown', 'unk', 'fUnk', 'field_', 'gUnk', 'pUnk', 'nUnk', 'qwUnknown', 'vecUnknown')
ESZ = {'char': 1, 'uchar': 1, 'bool': 1, 'undefined1': 1, 'byte': 1, 'short': 2, 'ushort': 2, 'undefined2': 2}
SIGNED = {'char', 'short', 'int', 'long'}
GLOB = {'g_clock': 'CLOCK', 'g_cm': 'CM', 'g_game': 'GAME', 'g_gs': 'SAVEDATA', 'g_joy': 'JOY', 'g_save': 'SAVE',
        'g_sndmgr': 'SNDMGR', 'g_sw': 'SW'}

def chain(t):
    out = []
    x = t
    while x and x not in out:
        out.append(x); x = L.get(x, {}).get('base')
    cid = CL.get(t, {}).get('cid')
    if cid is not None and cid in C.CLASSES:
        for k in C.cid_chain(cid):
            n = C.cname(k)
            if n not in out: out.append(n)
    return out
POLY = set(LN) | {t for t in L if 'BASIC' in chain(t)}

def is_unk(n): return n.startswith(UNK) or n.lower().startswith(('unknown', 'pad', '_pad'))

def flat(t, base=0, depth=0, path=''):
    """[(abs_off, size, local_name, type, isptr, decl_type, rel_off, path)]; embedded structs expanded."""
    out = []
    Lt = L.get(t)
    if not Lt or depth > 12: return out
    if Lt.get('base'): out += flat(Lt['base'], base, depth + 1, path)
    for off, sz, nm, ty, isp in Lt['fields']:
        if ty.startswith('CRefPtr'): isp = True
        E = L.get(ty)
        if not isp and E and E['fields'] and E['size'] and sz % E['size'] == 0 and depth < 6:
            n = sz // E['size']
            for i in range(min(n, 64)):
                out += flat(ty, base + off + i * E['size'], depth + 1, path + nm + ('[]' if n > 1 else '') + '.')
        else:
            out.append((base + off, sz, nm, ty, isp, t, off, path + nm))
    return out

def category(nm, ty, isp):
    """expected kind from name / type. returns (cat, reason)."""
    last = nm
    if isp or ty.endswith('*') or ty.startswith('PFN'): return 'ptr', 'pointer type'
    if ty == 'float' or ty == 'VECTOR' or ty == 'VECTOR4':
        if re.search(r'(^t[A-Z0-9]|[tT]ime|^dt[A-Z])', last): return 'time', 'float time name'
        if re.search(r'(pos|Pos|vel|Vel|^vec|Vec|^v[A-Z]|^dpos)', last): return 'vec', 'vector name'
        return 'float', 'float type'
    if ty == 'qword' and re.search(r'(pos|Pos|vel|Vel|^vec|Vec|^v[A-Z])', last): return 'vec', 'vector name (qword)'
    if re.match(r'(f|is|has|can)[A-Z]', last) or ty == 'bool': return 'bool', 'bool name/type'
    if re.match(r'^(c|num|n)[A-Z]', last) or re.search(r'(Count|count)$', last): return 'count', 'count name'
    if re.match(r'^p[A-Z]|^pfn|Ptr$', last): return 'ptrname', 'pointer name, non-pointer type'
    if re.search(r'(state|State|kind|Kind|mode|Mode)$', last):
        return 'enum', 'state/kind/mode name'
    if ty in ('int', 'uint', 'uint32_t', 'short', 'ushort', 'char', 'uchar', 'long', 'ulong', 'OID', 'CID', 'byte') \
            or (ty not in L and re.match(r'^[A-Z]+$', ty) and ty not in ('qword',)):
        return 'int', 'integer type'
    return None, None

def elem_values(M, K, T, off, sz, ty, isp):
    """yield (values int64, kinds, targets) per element (flattened over instances)."""
    esz = 4 if isp else ESZ.get(ty, 4 if sz >= 4 else sz)
    if ty in ('qword', 'VECTOR', 'VECTOR4'): esz = 4
    n = max(1, min(sz // max(esz, 1), 64))
    vals, ks, ts = [], [], []
    for i in range(n):
        o = off + i * esz; w = o // 4
        if w >= M.shape[1]: break
        v = M[:, w].astype(np.int64)
        if esz < 4:
            v = (v >> (8 * (o % 4))) & ((1 << (8 * esz)) - 1)
            if ty in SIGNED: v = np.where(v >= (1 << (8 * esz - 1)), v - (1 << (8 * esz)), v)
            k = np.where(v == 0, 0, 7).astype(np.int8); t = np.full(len(v), -1, np.int16)
        else:
            k = K[:, w]; t = T[:, w]
        vals.append(v); ks.append(k); ts.append(t)
    return np.concatenate(vals), np.concatenate(ks), np.concatenate(ts), esz

def main():
    snaps = sorted(glob.glob(os.path.join(CACHE, '*.npz')))
    FL = {}           # label -> flat list, union words
    agg = collections.defaultdict(lambda: dict(v=[], k=[], t=[], s=[], a=[], cls=collections.Counter()))
    unk = collections.defaultdict(lambda: dict(v=[], k=[], t=[], s=[], a=[], cls=collections.Counter()))
    clock = {}
    for si, fn in enumerate(snaps):
        sn = os.path.basename(fn)[:-4]
        Z = np.load(fn)
        clock[sn] = float(Z['M_g_clock'][0, 0:1].view(np.float32)[0])
        for key in Z.files:
            if not key.startswith('M_'): continue
            lab = key[2:]; ty0 = GLOB.get(lab, lab)
            M, K, T, A = Z[key], Z['K_' + lab], Z['T_' + lab], Z['A_' + lab]
            if ty0 not in FL:
                ff = flat(ty0)
                # union detection: words covered by fields of conflicting categories
                cov = collections.defaultdict(set)
                for f in ff:
                    cat = 'p' if f[4] else ('f' if f[3] in ('float', 'VECTOR', 'VECTOR4') else 'i')
                    for o in range(f[0] - f[0] % 4, f[0] + max(f[1], 1), 4): cov[o].add(cat)
                FL[ty0] = (ff, {o for o, c in cov.items() if len(c) > 1})
            ff, uw = FL[ty0]
            named_words = set()
            for (aoff, sz, nm, ty, isp, decl, roff, path) in ff:
                for o in range(aoff - aoff % 4, aoff + max(sz, 1), 4): named_words.add(o) if not is_unk(nm) else None
                if is_unk(nm) and ty in ('undefined4', '?', 'undefined1', 'int', 'uint', 'float', 'qword') and not isp:
                    for o in range(aoff - aoff % 4, aoff + sz, 4):
                        if o // 4 >= M.shape[1] or o in uw: continue
                        d = unk[(decl, roff + (o - aoff), path + ('' if o == aoff else '+%d' % (o - aoff)))]
                        d['v'].append(M[:, o // 4].astype(np.int64)); d['k'].append(K[:, o // 4]); d['t'].append(T[:, o // 4])
                        d['s'].append(np.full(len(A), si)); d['a'].append(A); d['cls'][lab] += len(A)
                    continue
                if is_unk(nm): continue
                cat, why = category(nm, ty, isp)
                if cat is None: continue
                if any(o in uw for o in range(aoff - aoff % 4, aoff + max(sz, 1), 4)): cat = 'union:' + cat
                v, k, t, esz = elem_values(M, K, T, aoff, sz, ty, isp)
                d = agg[(decl, roff, path, nm, ty, bool(isp), cat)]
                d['v'].append(v); d['k'].append(k); d['t'].append(t)
                d['s'].append(np.repeat(np.full(len(A), si), len(v) // max(len(A), 1)))
                d.setdefault('l', []).append(np.full(len(v), LID.get(lab, -1) if lab in LID else -10 - list(GLOB).index(lab)))
                d['a'].append(np.tile(A, len(v) // max(len(A), 1))); d['cls'][lab] += len(A)
    names = [os.path.basename(f)[:-4] for f in snaps]
    clk = np.array([clock[n] for n in names])
    res, conf, uns = [], [], []
    for (decl, roff, path, nm, ty, isp, cat), d in agg.items():
        v = np.concatenate(d['v']); k = np.concatenate(d['k']); t = np.concatenate(d['t'])
        s = np.concatenate(d['s']); a = np.concatenate(d['a'])
        nz = k != 0; n = len(v); nnz = int(nz.sum())
        if nnz < 4: continue
        u = v & 0xFFFFFFFF
        sentinel = (u == 0x7F7FFFFF) | (u == 0xFF7FFFFF) | (u == 0x7F800000) | (u == 0xFF800000)
        minus1 = (u == 0xFFFFFFFF) | (v == -1)
        nsn = len(set(s[nz].tolist()))
        kc = collections.Counter(KN[x] for x in k[nz].tolist())
        u32 = (v & 0xFFFFFFFF).astype(np.uint32); fv = u32.view(np.float32)
        base = cat.split(':')[-1]; bad = np.zeros(n, bool); note = ''
        isfl = k == 9; isptr = np.isin(k, list(PTRK)); smallint = (k == 7) | (k == 8)
        if base == 'ptr':
            bad = nz & ~isptr
            tgt = collections.Counter(LN[x] for x in t[(k == 2)].tolist() if x >= 0)
            if ty in POLY and tgt:
                wrong = {x: c for x, c in tgt.items() if ty not in chain(x)}
                tot = sum(tgt.values()); wr = sum(wrong.values())
                if wr / tot > 0.2:
                    note = 'points to %s (%d/%d), not %s' % (', '.join('%s:%d' % kv for kv in collections.Counter(wrong).most_common(4)), wr, tot, ty)
                    bad = bad | ((k == 2) & np.isin(t, [LID[x] for x in wrong]))
            elif ty not in POLY and ty in L and tgt and sum(tgt.values()) > 0.8 * nnz and nnz >= 8:
                lc = tgt.most_common(1)[0][0]
                note = 'type %s is a plain struct but %d%% of values are exact %s objects' % (ty, 100 * sum(tgt.values()) // nnz, lc)
            d['tgt'] = dict(tgt.most_common(5))
        elif base == 'bool':
            vv = v[nz]
            bad = nz & ~(v == 1)
            if minus1[nz].mean() > 0.5: note = 'mostly -1 (all-ones: BOOL -1 / mask?)'
            elif bad.any():
                bv = v[bad]
                if (k[bad] == 7).all() or (bv > 0).all() and (bv < (1 << 16)).all(): note = 'multi-bit values (flag word / bit-field?)'
        elif base == 'count':
            bad = nz & ~((v > 0) & (u < (1 << 24)) & ~np.isin(k, [2, 3, 6]) if ty not in ('float',) else isfl) & ~minus1
            if ty in ESZ: bad = nz & (v < -1)
            if minus1[nz].any(): note = '-1 sentinel seen (%d%%)' % (100 * minus1[nz].mean())
        elif base == 'ptrname':
            bad = nz & ~isptr
        elif base == 'enum':
            if ty in ESZ: bad = nz & ((v < -1) | (v > 255))
            else: bad = nz & ~((k == 7) & (v < 4096) | minus1)
        elif base == 'int':
            if ty not in ESZ: bad = nz & (isfl & (np.abs(fv) > 1e-3) & ((v & 0xFFFFFFFF) > 0x10000000)) & ~isptr
            note = ''
        elif base in ('float', 'time', 'vec'):
            fin = np.isfinite(fv) & (np.abs(fv) < 1e12)
            bad = nz & (isptr & (k != 7) | ~fin | (k == 7)) & ~sentinel   # small-int bit patterns are denormals
            if sentinel[nz].any(): note = (note + ' FLT_MAX sentinel %d%%' % (100 * sentinel[nz].mean())).strip()
            if base == 'time':
                pos = fv[nz & fin]
                stamp = float(np.mean(pos <= clk[s[nz & fin]] + 0.5)) if len(pos) else 0
                pos = pos
                neg = float(np.mean(pos < 0)) if len(pos) else 0
                d['time'] = dict(le_clock=round(stamp, 3), neg=round(neg, 3))
        if base == 'int' and not bad.any(): pass
        nb = int(bad.sum())
        frac = nb / max(nnz, 1)
        ex_bad = sorted(set(hex(int(x) & 0xFFFFFFFF) for x in v[bad][:200]))[:5]
        ex_ok = sorted(set(int(x) if base not in ('float', 'time', 'vec') else round(float(y), 3)
                           for x, y in zip(v[nz & ~bad][:200], fv[nz & ~bad][:200])))[:6]
        lab_ = np.concatenate(d['l'])
        def lname(x): return LN[x] if x >= 0 else list(GLOB)[-10 - x]
        bcl = collections.Counter(lname(x) for x in lab_[bad].tolist())
        nzcl = collections.Counter(lname(x) for x in lab_[nz].tolist())
        bad_sn = sorted(set(names[x] for x in s[bad].tolist()))[:6]
        ent = dict(field='%s::%s' % (decl, path), decl=decl, off='+0x%X' % roff, type=ty + ('*' if isp else ''),
                   check=cat, n=n, nonzero=nnz, snapshots=nsn, bad=nb, bad_frac=round(frac, 3),
                   kinds=dict(kc.most_common(5)), classes=dict(d['cls'].most_common(4)), ex_bad=ex_bad, ex_ok=ex_ok,
                   bad_snaps=bad_sn, note=note, bad_by_class=dict(bcl.most_common(4)), nz_by_class=dict(nzcl.most_common(4)))
        for x in ('tgt', 'time'):
            if x in d: ent[x] = d[x]
        score = frac * (1 - 1 / np.sqrt(nnz)) * min(1, nsn / 3)
        if cat.startswith('union'): score *= 0.3
        ent['score'] = round(float(score), 3)
        (res if (frac >= 0.2 and nb >= 3) or (note and 'points to' in note) or (note and base == 'ptr') else conf).append(ent)
    # unknown suggestions
    for (decl, roff, path), d in unk.items():
        v = np.concatenate(d['v']); k = np.concatenate(d['k']); t = np.concatenate(d['t'])
        s = np.concatenate(d['s']); a = np.concatenate(d['a'])
        nz = k != 0; nnz = int(nz.sum()); nsn = len(set(s[nz].tolist()))
        if nnz < 6 or nsn < 2: continue
        fv = (v & 0xFFFFFFFF).astype(np.uint32).view(np.float32)
        sug = None; ev = ''; c = 0
        objt = collections.Counter(LN[x] for x in t[k == 2].tolist() if x >= 0)
        if (k[nz] == 2).mean() >= 0.9:
            tl = [x for x in objt]
            ch = [chain(x) for x in tl]
            inter = [x for x in ch[0] if all(x in cc for cc in ch[1:])] if ch else []
            X = inter[0] if inter else objt.most_common(1)[0][0]
            sug = 'p%s (%s*)' % (X.lower(), X); c = (k[nz] == 2).mean(); ev = 'targets ' + ', '.join('%s:%d' % kv for kv in objt.most_common(3))
        elif (k[nz] == 4).mean() >= 0.9:
            sug = 'pfn... (function pointer)'; c = (k[nz] == 4).mean(); ev = 'code pointers'
        elif (k[nz] == 9).mean() >= 0.9:
            fin = nz & (k == 9); ff = fv[fin]; cs = clk[s[fin]]
            le = float(np.mean((ff > 0) & (ff <= cs + 0.5)))
            # monotone with clock for the same object across snapshots
            pairs = 0; mono = 0; slope1 = 0
            byo = collections.defaultdict(list)
            for vv, ss, aa in zip(ff, s[fin], a[fin]): byo[int(aa)].append((clk[ss], vv))
            for lst in byo.values():
                lst = sorted(set(lst))
                for (c0, v0), (c1, v1) in zip(lst, lst[1:]):
                    if c1 - c0 < 0.05: continue
                    pairs += 1; mono += v1 >= v0 - 1e-4
                    if abs((v1 - v0) / (c1 - c0) - 1) < 0.05: slope1 += 1
            st = float(np.std(ff)); dist = len(set(np.round(ff, 4).tolist()))
            if le >= 0.9 and pairs >= 3 and mono / pairs >= 0.9 and np.median(ff) > 1:
                sug = 't... (absolute time stamp, <= g_clock.time)'; c = le * mono / pairs
                ev = 'le_clock %.2f, monotone %d/%d pairs, slope~1 %d' % (le, mono, pairs, slope1)
            elif dist == 1:
                sug = 'constant float %.4g (tuning parameter)' % ff[0]; c = 0.5; ev = 'single value'
            else:
                sug = 'float'; c = 0.3; ev = 'distinct %d, median %.3g' % (dist, float(np.median(ff)))
        elif np.isin(v[nz], [1]).all() and (~nz).sum() > 0:
            sug = 'f... (bool)'; c = 0.8; ev = 'only 0/1 (%d zero, %d one)' % ((~nz).sum(), nnz)
        elif ((k[nz] == 7) | (v[nz] == 0xFFFFFFFF)).all():
            dist = sorted(set(v[nz].tolist()))
            if len(dist) <= 12: sug = 'enum/state'; c = 0.5; ev = 'values ' + str([x if x != 0xFFFFFFFF else -1 for x in dist][:8])
            else: sug = 'int/count'; c = 0.4; ev = '%d distinct small ints, max %d' % (len(dist), max(dist))
        elif (k[nz] == 6).mean() >= 0.9:
            sug = 'p... (heap struct pointer)'; c = 0.6; ev = 'heap pointers (no object start)'
        if sug and c >= 0.5:
            uns.append(dict(field='%s::%s' % (decl, path), decl=decl, off='+0x%X' % roff, suggestion=sug, conf=round(float(c), 2),
                            nonzero=nnz, snapshots=nsn, evidence=ev, classes=dict(d['cls'].most_common(3))))
    # layout-level clusters: many failing fields in one declaring struct / embedded instance, or concentrated in one class
    grp = collections.defaultdict(lambda: [0, 0])
    def gkey(e): return e['decl'] + ('::' + e['field'].split('::', 1)[1].split('.')[0] + '.' if '.' in e['field'].split('::', 1)[1] else '')
    for e in res: grp[gkey(e)][0] += 1
    for e in res + conf: grp[gkey(e)][1] += 1
    def domcls(e):
        b = e['bad_by_class']; tot = sum(b.values())
        if not tot: return None
        c, n = max(b.items(), key=lambda kv: kv[1])
        return c if n >= 0.8 * e['bad'] and c != e['decl'] else None
    dc = collections.Counter(domcls(e) for e in res if domcls(e))
    clusters = {}
    for g, (f, t) in grp.items():
        if f >= 4 and f / t >= 0.35: clusters[g] = dict(failing=f, checked=t, kind='struct')
    for c, n in dc.items():
        if n >= 5: clusters['class:' + c] = dict(failing=n, kind='subclass-concentrated')
    naming = []
    lay = collections.defaultdict(list)
    for e in res:
        g = gkey(e); c = domcls(e)
        if g in clusters: e['cluster'] = g
        elif c and 'class:' + c in clusters: e['cluster'] = 'class:' + c
        (lay[e['cluster']] if 'cluster' in e else naming).append(e)
    for g in clusters: clusters[g]['examples'] = [x['field'].split('::', 1)[1] for x in lay[g][:6]]
    res = naming
    res.sort(key=lambda e: -e['score']); uns.sort(key=lambda e: (-('t...' in e['suggestion']) - (e['suggestion'].startswith('p') and 'heap' not in e['suggestion']), -e['conf'] * np.log1p(e['nonzero'])))
    out = dict(snapshots=len(snaps), layout_clusters=clusters, clustered_mismatches=[e for v in lay.values() for e in v], mismatches=res, confirmed=conf, unknown_suggestions=uns,
               summary=dict(fields_checked=len(res) + len(conf) + sum(len(v) for v in lay.values()), clustered=sum(len(v) for v in lay.values()), clusters=len(clusters), mismatches=len(res), confirmed=len(conf),
                            by_check=dict(collections.Counter(e['check'] for e in res + conf)),
                            mismatch_by_check=dict(collections.Counter(e['check'] for e in res)),
                            unknown_suggestions=len(uns)))
    json.dump(out, open(os.path.join(HERE, 'namecheck.json'), 'w'), indent=1)
    print(json.dumps(out['summary']))

if __name__ == '__main__':
    main()
