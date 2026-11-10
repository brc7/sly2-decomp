#!/usr/bin/env python3
"""Step 1: object discovery per snapshot.
 (a) heap-wide scan: every aligned word equal to a known vtable address, object = word - vptr offset (0xC for BASIC
     descendants; ACT* 0x18, input streams 0x30, RIP family 0xEC, CLoadCollectorArray 0);
 (b) SW's per-class lists (g_sw.objectsByClass[199], SL {head, tail, linkOffset}) walked from g_sw, to mark objects
     the world still lists (the scan also finds stale copies in freed memory).
Writes objects/<snap>.npy ([addr, vtable, listed, cid_listed]) and classes.json."""
import os, sys, json, collections
import numpy as np
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import snapcfg
if __name__ == '__main__': snapcfg.parse(__doc__)
import common as C

RIP_FAMILY = {'RIP', 'RIPT0', 'RIPT1', 'STUCK', 'SMACK', 'SHRAPNEL', 'REMIT', 'MATCH', 'LEAF', 'GLINT', 'FLYING',
              'RIPG', 'BUBBLE', 'DROPLET', 'ROSE', 'TRAIL', 'SPARK', 'BURST', 'FLAME', 'DEBRIS'}
def hdr_chain(t):
    out = [t]
    while C.LAYOUT.get(out[-1], {}).get('base') and len(out) < 30: out.append(C.LAYOUT[out[-1]]['base'])
    return out
def vptr_off(label):
    """vptr position: 0xC under BASIC; 0xEC for the RIP family; else the first word of the header root type that no
    field covers (AC/ACG 0, ACT 0x18); stream classes 0x30."""
    if label in ('CBinaryInputStream', 'CStreamBis', 'CMemInputStream'): return 0x30
    if label == 'CLoadCollectorArray': return 0
    ch = hdr_chain(label)
    if 'BASIC' in ch or label not in C.LAYOUT: return C.VPTR_OFF if 'RIP' not in ch and label not in RIP_FAMILY else 0xEC
    if 'RIP' in ch or label in RIP_FAMILY: return 0xEC
    root = ch[-1]; L = C.LAYOUT[root]
    cov = set()
    for o, sz, *_ in L['fields']: cov.update(range(o, o + sz))
    for o in range(0, max(L['size'], 4), 4):
        if o not in cov: return o
    return C.VPTR_OFF

SL_BASE = C.G_SW + 12344      # SW::objectsByClass (offsets.json)

def walk_class_lists(ram):
    w = lambda a: int(ram[a >> 2]) if 0 <= a < len(ram) * 4 and a % 4 == 0 else None
    listed = {}
    bad = 0
    for k in range(C.NCID):
        head, tail, lo = (w(SL_BASE + 12 * k + 4 * j) for j in range(3))
        p, n = head, 0
        while p and n < 20000:
            if p in listed: break
            listed[p] = k; n += 1
            nx = w(p + lo)
            if nx is None: bad += 1; break
            p = nx
    return listed, bad

def main():
    vts = C.find_vtables()
    vlabel = {a: v['label'] for a, v in vts.items()}
    voff = {a: vptr_off(v['label']) for a, v in vts.items()}
    vcid = {a: v['cid'] for a, v in vts.items()}
    varr = np.array(sorted(vts), dtype=np.uint32)
    os.makedirs(os.path.join(C.WORK, 'objects'), exist_ok=True)
    snaps = C.snapshots()
    per = collections.defaultdict(lambda: collections.Counter())      # label -> snap -> count
    perl = collections.defaultdict(lambda: collections.Counter())     # listed counts
    meta = {}
    for name, path, h in snaps:
        ram = C.load_ram(path)
        sw_vt = int(ram[(C.G_SW + 0xC) >> 2])
        hits = np.nonzero(np.isin(ram, varr))[0]
        listed, bad = walk_class_lists(ram)
        rows = []
        for i in hits:
            vt = int(ram[i]); a = 4 * int(i) - voff[vt]
            if a < 0x100000: continue
            l = a in listed
            # a listed object must carry a vtable of its list's class subtree; stale scan hits are kept but unflagged
            rows.append((a, vt, int(l), listed.get(a, -1)))
        unk_listed = sum(1 for a in listed if int(ram[(a + 0xC) >> 2]) not in vts)
        arr = np.array(rows, dtype=np.int64).reshape(-1, 4)
        np.save(os.path.join(C.WORK, 'objects', name + '.npy'), arr)
        for a, vt, l, _ in rows:
            per[vlabel[vt]][name] += 1
            if l: perl[vlabel[vt]][name] += 1
        # consistency: listed object's vptr class within the list's cid subtree
        mism = 0
        for a, k in listed.items():
            vt = int(ram[(a + 0xC) >> 2]); c = vcid.get(vt)
            if c is not None and k not in C.cid_chain(c): mism += 1
        meta[name] = dict(md5=h, sw_vptr_ok=vlabel.get(sw_vt) == 'SW', scan_hits=len(rows), listed=len(listed),
                          listed_unknown_vptr=unk_listed, listed_cid_mismatch=mism, list_walk_errors=bad,
                          level=int(ram[(C.G_SW + 0x3C40) >> 2]))
        print(name, meta[name], flush=True)
    classes = {}
    for a, v in sorted(vts.items()):
        lab = v['label']
        c = classes.setdefault(lab, dict(label=lab, vtables=[], cid=v['cid'], vptr_off=voff[a],
                                         size=C.CLASSES[v['cid']]['size'] if v['cid'] is not None and lab == C.cname(v['cid']) else None,
                                         in_headers=lab in C.LAYOUT, header_size=C.LAYOUT.get(lab, {}).get('size')))
        c['vtables'].append(hex(a))
    for lab, c in classes.items():
        c['instances_total'] = sum(per[lab].values()); c['listed_total'] = sum(perl[lab].values())
        c['snapshots_seen'] = len(per[lab]); c['per_snapshot'] = dict(per[lab])
        if c['cid'] is not None:
            c['parent'] = C.cname(C.CLASSES[c['cid']]['parent']) if C.CLASSES[c['cid']]['parent'] is not None else None
    # registry classes never seen (cid with no vtable of its own name)
    seen_labels = {l for l, c in classes.items() if c['instances_total']}
    reg_unseen = [C.CLASSES[k]['name'] for k in C.CLASSES if C.CLASSES[k]['name'] not in seen_labels]
    out = dict(snapshots=meta, classes=classes,
               summary=dict(n_snapshots=len(snaps), n_vtables=len(vts), classes_with_vtable=len(classes),
                            classes_seen=len(seen_labels), registry_classes=len(C.CLASSES),
                            registry_classes_unseen=reg_unseen))
    json.dump(out, open(os.path.join(C.WORK, 'classes.json'), 'w'), indent=1)
    print('classes seen', len(seen_labels), 'of', len(classes), 'vtable classes; registry unseen', len(reg_unseen))

if __name__ == '__main__':
    main()
