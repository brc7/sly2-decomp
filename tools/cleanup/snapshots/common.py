"""Shared loaders for the snapshot pipeline (read-only on the tree; game-derived data stays in the work directory).
ELF segments, symbols, the class descriptor table (D_002BAAE8, CLASSDESC[199]), vtables, header layouts
(census offsets.json) and the deduplicated snapshot list. Paths come from snapcfg (see snapcfg.py)."""
import os, re, struct, json, collections, hashlib, glob
import numpy as np
import snapcfg

WT = snapcfg.get('root')                      # decomp checkout (read-only)
ELFP = os.path.realpath(snapcfg.need('elf', 'game ELF'))
WORK = snapcfg.get('work'); os.makedirs(WORK, exist_ok=True)
RAMDIR = os.path.join(WORK, 'ram')
G_SW = 0x3DD490
CLASSDESC_TAB, NCID = 0x2BAAE8, 199
VPTR_OFF = 0xC

_d = open(ELFP, 'rb').read()
_phoff, = struct.unpack_from('<I', _d, 28); _nph, = struct.unpack_from('<H', _d, 44)
PH = [struct.unpack_from('<8I', _d, _phoff + 32 * i) for i in range(_nph)]
TEXT = (0x100000, 0x100000 + 0x1dd230)          # first PT_LOAD (code + rodata/data early part)
IMAGE_END = max(p[2] + p[5] for p in PH if p[0] == 1)   # end of .bss

def rd(va, n):
    for p in PH:
        if p[0] == 1 and p[2] <= va and va + n <= p[2] + p[4]:
            return _d[p[1] + va - p[2]: p[1] + va - p[2] + n]
    return None
def u32(va):
    b = rd(va, 4); return struct.unpack('<I', b)[0] if b else None

# ---- symbols
SYM, SYMSIZE, FUNCS = {}, {}, {}
for ln in open(f'{WT}/config/symbol_addrs.txt'):
    m = re.match(r'\s*(\S+)\s*=\s*(0x[0-9A-Fa-f]+);(.*)', ln)
    if not m: continue
    n, a = m.group(1), int(m.group(2), 16)
    sz = re.search(r'size:(0x[0-9A-Fa-f]+)', m.group(3))
    if 'type:func' in m.group(3): FUNCS.setdefault(a, n)
    else:
        SYM.setdefault(a, n)
        if sz: SYMSIZE[a] = int(sz.group(1), 16)
for ln in list(open(f'{WT}/config/functions.csv'))[1:]:
    n, s, e, _ = ln.strip().split(','); FUNCS.setdefault(int(s, 16), n)

def demangle_vt(n):
    parts = []
    for comp in n[4:].split('$'):
        m = re.match(r'(\d+)(\w+)', comp); parts.append(m.group(2)[:int(m.group(1))] if m else comp)
    return parts

# ---- header layouts (32-bit offsets from the compiler dump)
LAYOUT = json.load(open(os.path.join(snapcfg.need('census', 'census dir (run tools/cleanup/census/dump_offsets.py)'), 'offsets.json')))
def flat_fields(t, base=0, depth=0, seen=None):
    """flatten a header type: [(off, size, name, type, isptr)] incl. bases and embedded structs (one level of names)."""
    out = []
    L = LAYOUT.get(t)
    if not L or depth > 12: return out
    if L.get('base'): out += flat_fields(L['base'], base, depth + 1)
    for off, sz, nm, ty, isp in L['fields']:
        if ty.startswith('CRefPtr'): isp = True
        E = LAYOUT.get(ty)
        if not isp and E and E['fields'] and E['size'] and sz % E['size'] == 0 and depth < 6:
            n = sz // E['size']
            for i in range(min(n, 1024)):
                sub = flat_fields(ty, base + off + i * E['size'], depth + 1)
                tag = nm if n == 1 else '%s[%d]' % (nm, i)
                out += [(o, s_, tag + '.' + n2, t2, p2) for o, s_, n2, t2, p2 in sub] or [(base + off + i * E['size'], E['size'], tag, ty, isp)]
        else:
            out.append((base + off, sz, nm, ty, isp))
    return out

# ---- class descriptor table
def getter_names():
    src = open(f'{WT}/src/game/app/class_registry.cpp', errors='ignore').read()
    idx2getter = {}
    for m in re.finditer(r'(Get\w*ClassDesc)\s*\(\s*\)\s*\{[^}]*?D_002BAAE8\s*\[\s*(\d+)\s*\]', src):
        idx2getter.setdefault(int(m.group(2)), m.group(1))
    return idx2getter
HDR_NAMES = set(LAYOUT)
def canon(getter):
    n = getter[3:-9].upper()
    return n
CLASSES = {}          # cid -> dict
_g = getter_names()
_fn2a = {v: k for k, v in FUNCS.items()}
for i in range(NCID):
    a = CLASSDESC_TAB + 0x14 * i
    par, cid, last, size, pfnnew = struct.unpack('<5I', rd(a, 20))
    nm = canon(_g[i]) if i in _g else 'CID%d' % cid
    CLASSES[cid] = dict(cid=cid, name=nm, desc=a, parent_desc=par, last=last, size=size, pfnNew=pfnnew,
                        getter=_g.get(i), getter_addr=_fn2a.get(_g.get(i)), in_headers=nm in HDR_NAMES)
DESC2CID = {c['desc']: k for k, c in CLASSES.items()}
for c in CLASSES.values():
    c['parent'] = DESC2CID.get(c['parent_desc'])
GETTER2CID = {c['getter_addr']: k for k, c in CLASSES.items() if c['getter_addr']}
def cid_chain(cid):
    out = [cid]
    while CLASSES[out[-1]]['parent'] is not None and CLASSES[out[-1]]['parent'] not in out:
        out.append(CLASSES[out[-1]]['parent'])
    return out
def cname(cid): return CLASSES[cid]['name'] if cid in CLASSES else '?'

# ---- vtables: named primary _vt$ tables + any table whose slot 6 (GetClassInfo) is a descriptor getter
def find_vtables():
    vts = {}
    named = {}
    for a, n in SYM.items():
        if n.startswith('_vt$'):
            parts = demangle_vt(n); named[a] = (parts[0], parts[1] if len(parts) > 1 else None)
    # scan the image's data for tables: word at v+8*6+4 == a getter address, v 8-aligned
    lo, hi = 0x100000, IMAGE_END
    for p in PH:
        if p[0] != 1: continue
        seg = _d[p[1]: p[1] + p[4]]
        arr = np.frombuffer(seg[: len(seg) // 4 * 4], dtype='<u4')
        hits = np.nonzero(np.isin(arr, np.array(list(GETTER2CID), dtype=np.uint32)))[0]
        for h in hits:
            va = p[2] + 4 * int(h) - (8 * 6 + 4)
            if va % 8: continue
            # slot 1..5 must be code pointers too
            ok = all(TEXT[0] <= (u32(va + 8 * k + 4) or 0) < TEXT[1] for k in range(1, 6))
            if not ok: continue
            cid = GETTER2CID[int(arr[h])]
            nm = named.get(va)
            vts[va] = dict(addr=va, cid=cid, sym=SYM.get(va), named_class=nm[0] if nm and not nm[1] else None,
                           getter_class=cname(cid))
    for a, (cls, sec) in named.items():
        if a not in vts and sec is None:
            vts[a] = dict(addr=a, cid=None, sym=SYM.get(a), named_class=cls, getter_class=None)
    for v in vts.values():
        v['label'] = v['named_class'] or v['getter_class'] or v['sym']
    return vts

# ---- snapshots (dedup by md5; prefer canonical names)
def snapshots():
    """RAM images in WORK/ram (*.bin, 32 MB eeMemory dumps), deduplicated by md5 (cached in WORK/ram_md5.txt)."""
    groups = collections.OrderedDict()
    idx = os.path.join(WORK, 'ram_md5.txt')
    known = {os.path.basename(l.split()[1]) for l in open(idx)} if os.path.exists(idx) else set()
    with open(idx, 'a') as f:
        for p in sorted(glob.glob(os.path.join(RAMDIR, '*.bin'))):
            if os.path.basename(p) not in known:
                f.write('%s  %s\n' % (hashlib.md5(open(p, 'rb').read()).hexdigest(), p))
    for ln in open(idx):
        h, p = ln.split(); groups.setdefault(h, []).append(os.path.basename(p))
    def rank(n): return (n.startswith('dump_'), '.bin.' in n, n.startswith('x'), n)
    out = []
    for h, ns in groups.items():
        n = sorted(ns, key=rank)[0]
        out.append((n.replace('.eeMemory.bin', '').lstrip('x') if n.startswith('xS') else n.replace('.eeMemory.bin', ''),
                    os.path.join(RAMDIR, n), h))
    return sorted(out)

def load_ram(path):
    return np.fromfile(path, dtype='<u4')
