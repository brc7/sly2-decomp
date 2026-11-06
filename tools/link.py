#!/usr/bin/env python3
"""Prototype full link of SCUS_973.16 from the matched sources (docs/guides/full-link.md).

usage: python3 tools/link.py [--no-compile | --check-tools]

  1. compile every source of config/matched.csv that builds here (matchtool's compile_src, plus -ffunction-sections;
     no -fdata-sections: each object's data stays in its .data/.rodata in source order, as in a real link) into
     build/link/obj/; --no-compile reuses the objects already there
  2. place every object section at the address its symbols and relocations give it in the ELF, cover everything
     else with byte ranges cut from your disc/SCUS_973.16 (build/link/blobs.o, never committed), and write
     build/link/sly2.ld from tools/link.ld.in
  3. link with ld.lld and compare build/link/sly2.elf with the ELF section by section
"""
import collections, csv, functools, os, re, shutil, struct, subprocess, sys
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import matchtool, linkaliases


@functools.lru_cache(None)
def llvm_tool(name):
    """path of an LLVM tool: $LLVM_BIN, then PATH, then Homebrew's keg-only llvm/lld (not on PATH by default)"""
    dirs = [os.environ['LLVM_BIN']] if os.environ.get('LLVM_BIN') else []
    found = shutil.which(name)
    if found and not dirs:
        return found
    for prefix in ('/opt/homebrew/opt', '/usr/local/opt'):
        dirs += [f'{prefix}/llvm/bin', f'{prefix}/lld/bin']
    for d in dirs:
        if os.path.isfile(os.path.join(d, name)):
            return os.path.join(d, name)
    if found:
        return found
    sys.exit(f'link.py: {name} not found. Install LLVM (macOS: `brew install llvm lld` -- Homebrew ships ld.lld in its own '
             f'`lld` formula since LLVM 20; Debian/Ubuntu: `apt install lld llvm`) or set LLVM_BIN to the directory that '
             f'contains it.')

OUT = os.path.join(ROOT, 'build', 'link')
ELF = matchtool.ELF


def sources():
    """(buildable, unbuildable) source paths of config/matched.csv"""
    srcs = sorted({r['source'] for r in csv.DictReader(open(os.path.join(ROOT, 'config', 'matched.csv')))})
    ok, skip = [], []
    for s in srcs:
        head = open(os.path.join(ROOT, s), errors='ignore').read(2000)
        cc = re.search(r'SLY2_CC:\s*(\S+)', head)
        native = cc and os.path.exists(os.path.join(ROOT, cc.group(1), 'bin', 'ee-gcc'))
        (skip if native or re.search(r'SLY2_ASM:\s*gnu-reor', head) else ok).append(s)
    return ok, skip


def obj_path(src):
    return os.path.join(OUT, 'obj', src + '.o')


def compile_all(srcs):
    # one section per function and per data object, so each lands at its own address even where a source's order
    # differs from the ELF's (codegen is unchanged: every function still MATCHes in these objects).  Appended to
    # matchtool's include list because that is the one argument list every compile path of compile_src passes on.
    matchtool.INCLUDES += ['-ffunction-sections']
    def one(s):
        o = obj_path(s)
        os.makedirs(os.path.dirname(o), exist_ok=True)
        try:
            shutil.move(matchtool.compile_src(s), o)   # relative path, as matchtool is run (the path-hash quirk)
        except SystemExit:
            print(f'FAILED: {s}', file=sys.stderr)
    with ThreadPoolExecutor(os.cpu_count() or 2) as ex:
        for i, _ in enumerate(ex.map(one, srcs)):
            if i % 50 == 0:
                print(f'compiled {i}/{len(srcs)}', file=sys.stderr)


# ---- ELF reading ----
def elf_secs(path):
    """[(name, type, flags, addr, offset, size, link, info, align)] of an ELF32-LE file"""
    d = open(path, 'rb').read()
    shoff, = struct.unpack_from('<I', d, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2E)
    hdr = [struct.unpack_from('<10I', d, shoff + i * shentsize) for i in range(shnum)]
    so = hdr[shstrndx][4]
    return d, [(d[so + h[0]:d.index(b'\0', so + h[0])].decode(),) + h[1:9] for h in hdr]


class Obj:
    """sections, symbols and relocations of one relocatable object"""
    def __init__(self, path, src):
        self.path, self.src = path, src
        d, self.secs = elf_secs(path)
        self.data = d
        self.syms, self.rels = [], {}
        for s in self.secs:
            if s[1] == 2:                                    # SHT_SYMTAB
                stro = self.secs[s[6]][4]
                for i in range(s[5] // 16):
                    nm, val, size, info, other, shndx = struct.unpack_from('<IIIBBH', d, s[4] + i * 16)
                    self.syms.append((d[stro + nm:d.index(b'\0', stro + nm)].decode(), val, size, info >> 4,
                                      info & 0xF, shndx))
        for s in self.secs:
            if s[1] == 9:                                    # SHT_REL: target section index -> [(off, type, sym)]
                self.rels[s[7]] = [(o, i & 0xFF, i >> 8) for o, i in
                                   (struct.unpack_from('<II', d, s[4] + 8 * k) for k in range(s[5] // 8))]

        # gas rounds every section up to its alignment; with one section per object that padding would cover the
        # next object (a 12-byte struct aligned 8 gets 16 bytes), so a section holding one sized symbol is cut to it
        self.size = {i: sec[5] for i, sec in enumerate(self.secs)}
        self.align, self.absolute = {}, {}                   # patches decided by script(), applied by fixed()
        ends = collections.defaultdict(list)
        for name, val, size, bind, typ, shndx in self.syms:
            if typ in (1, 2) and size and 0 < shndx < 0xFF00:
                ends[shndx].append(val + size)
        for i, e in ends.items():
            sec = self.secs[i]
            if len(e) == 1 and e[0] < sec[5] and '.' in sec[0][1:] and (sec[1] == 8 or not any(d[sec[4] + e[0]:sec[4] + sec[5]])):
                self.size[i] = e[0]

    def word(self, sec, off):
        return struct.unpack_from('<I', self.data, self.secs[sec][4] + off)[0]

    def alloc(self):
        """indices of the allocated sections with contents or size"""
        return [i for i, s in enumerate(self.secs) if s[2] & 2 and s[5] and s[1] in (1, 8)]


class Image:
    """the original ELF: bytes by address"""
    def __init__(self, path):
        self.d, secs = elf_secs(path)
        self.secs = [s for s in secs if s[2] & 2 and s[3]]
        self.gp = struct.unpack_from('<i', self.d, next(s for s in secs if s[0] == '.reginfo')[4] + 20)[0]

    def sec_at(self, a):
        return next((s for s in self.secs if s[3] <= a < s[3] + s[5]), None)

    def bytes(self, a, n):
        s = self.sec_at(a)
        if s is None or s[1] == 8 or a + n > s[3] + s[5]:
            return None
        return self.d[s[4] + a - s[3]:s[4] + a - s[3] + n]

    def word(self, a):
        b = self.bytes(a, 4)
        return struct.unpack('<I', b)[0] if b else None


def sext16(x):
    return (x & 0xFFFF) - ((x & 0x8000) << 1)


# ---- placement: the address of every object section in the ELF ----
def config_addrs():
    """every name config gives an address (functions.csv, symbol_addrs.txt; D_/func_ names by convention)"""
    a = {n: v[0] for n, v in matchtool.load_functions().items()}
    for line in open(os.path.join(ROOT, 'config', 'symbol_addrs.txt')):
        m = re.match(r'\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+);', line)
        if m:
            a.setdefault(m.group(1), int(m.group(2), 16))
    return a


def addr_of(name, addrs):
    if name in addrs:
        return addrs[name]
    m = linkaliases.ADDR_NAME.match(name)
    return int(m.group(1), 16) if m else None


def reloc_values(o, sec, base, img):
    """(symbol index, value of symbol+0 in the ELF) for the relocations of a placed section"""
    rels = o.rels.get(sec, [])
    for i, (off, t, sym) in enumerate(rels):
        w = img.word(base + off)
        if w is None:
            continue
        ow = o.word(sec, off)
        if t == 2:                                           # R_MIPS_32
            yield sym, (w - ow) & 0xFFFFFFFF
        elif t == 4:                                         # R_MIPS_26
            yield sym, (((base + off + 4) & 0xF0000000) | (w & 0x3FFFFFF) << 2) - ((ow & 0x3FFFFFF) << 2)
        elif t == 5:                                         # R_MIPS_HI16: pair with the next LO16 of the symbol
            lo = next((r for r in rels[i + 1:] if r[1] == 6 and r[2] == sym), None)
            lw = lo and img.word(base + lo[0])
            if lw is not None:
                yield sym, (((w & 0xFFFF) << 16) + sext16(lw)) - (((ow & 0xFFFF) << 16) + sext16(o.word(sec, lo[0])))


def place(objs, img):
    """{(object index, section index): base address}, and the sections whose evidence disagreed"""
    addrs = config_addrs()
    votes = collections.defaultdict(collections.Counter)
    gdef = collections.defaultdict(list)                     # global name -> [(obj, sec, value)]
    for k, o in enumerate(objs):
        names = {s[0] for s in o.syms if s[0]}
        alias = {old: new for new, old in matchtool.apply_aliases(os.path.join(ROOT, o.src), {n: n for n in names}).items()}
        for name, val, size, bind, typ, shndx in o.syms:
            if not name or not 0 < shndx < 0xFF00 or typ not in (1, 2):
                continue
            if bind:
                gdef[name].append((k, shndx, val))
            a = addr_of(alias.get(name, name), addrs)
            if a is not None:
                votes[(k, shndx)][a - val] += 10 + size     # a big object outvotes small ones
    base, ext = {}, collections.defaultdict(collections.Counter)   # ext: undefined name -> ELF values
    todo = list(votes)
    while todo:                                              # relocations of placed sections place their targets
        for key in todo:
            base[key] = votes[key].most_common(1)[0][0]
        new = set()
        for k, sec in todo:
            o = objs[k]
            for sym, v in reloc_values(o, sec, base[(k, sec)], img):
                name, val, size, bind, typ, shndx = o.syms[sym]
                tgts = [(k, shndx, val)] if 0 < shndx < 0xFF00 else gdef.get(name, []) if bind else []
                if bind and shndx in (0, 0xFFF2) and not gdef.get(name):   # undefined or COMMON
                    ext[name][v] += 1
                for tk, ts, tv in tgts:
                    if (tk, ts) not in base:
                        votes[(tk, ts)][v - tv] += 1
                        new.add((tk, ts))
        todo = sorted(new)
    conflicts = {key: c for key, c in votes.items() if len(c) > 1}
    return base, conflicts, {n: c.most_common(1)[0][0] for n, c in ext.items()}


# ---- blobs: ELF byte ranges as input sections of one generated object ----
def write_blobs(path, ranges, img):
    """ranges: [(addr, size, sh_flags)] -> an ELF32 relocatable with one section .blob.XXXXXXXX per range"""
    names, body, hdrs = b'\0.shstrtab\0.symtab\0', b'', []
    for a, n, fl in ranges:
        nm = len(names); names += f'.blob.{a:08X}\0'.encode()
        hdrs.append((nm, 1, fl, 0, 0x34 + len(body), n, 0, 0, 1, 0)); body += img.bytes(a, n)
    off = 0x34 + len(body)
    hdrs = [(0,) * 10, (1, 3, 0, 0, off, len(names), 0, 0, 1, 0),
            (11, 2, 0, 0, off + len(names), 16, 1, 1, 4, 16)] + hdrs
    data = body + names + b'\0' * 16
    shoff = 0x34 + len(data)
    eh = b'\x7fELF\1\1\1' + b'\0' * 9 + struct.pack('<HHIIIIIHHHHHH', 1, 8, 1, 0, 0, shoff, 0x20924001, 0x34, 0, 0,
                                                     40, len(hdrs), 1)
    with open(path, 'wb') as f:
        f.write(eh + data + b''.join(struct.pack('<10I', *h) for h in hdrs))


def same_bytes(o, si, b, n, img):
    """the section's bytes equal the ELF's at b, words with a relocation aside"""
    t = img.bytes(b, n)
    if t is None:
        return False
    mine = o.data[o.secs[si][4]:o.secs[si][4] + n]
    masked = {r[0] // 4 for r in o.rels.get(si, [])}
    return all(mine[i] == t[i] for i in range(n) if i // 4 not in masked)


def q(s):
    return '"' + s + '"'


def blob_reasons(img, unbuilt):
    """(start, end, reason) of what config says is at the addresses blobs may cover"""
    out = [(s[3], s[3] + s[5], {'.vutext': 'VU microcode', '.irx': 'IOP modules (.irx)'}.get(s[0], 'data no object defines'
           if s[0] != '.text' else 'text: padding / not in config')) for s in img.secs]
    funcs = {int(r['Start'], 16): int(r['End'], 16) for r in csv.DictReader(open(os.path.join(ROOT, 'config', 'functions.csv')))}
    matched = {int(r['addr'], 16): r['source'] for r in csv.DictReader(open(os.path.join(ROOT, 'config', 'matched.csv')))}
    for a, e in funcs.items():
        src = matched.get(a)
        out.append((a, e, 'text: unmatched function' if src is None else
                    'text: SDK source that does not build here' if src in unbuilt else 'text: matched but not in an object'))
    for a, e in matchtool.load_junk():
        out.append((a, e, 'text: dead code (junk.csv)'))
    return sorted(out, key=lambda r: (r[0], -r[1]))          # later (narrower) rows win


def reasons_of(lo, hi, reasons, acc):
    """add the bytes of [lo, hi) to acc by reason (narrower rows, sorted later, win)"""
    marks = []
    for a, e, why in reasons:
        if e > lo and a < hi:
            marks.append((max(a, lo), min(e, hi), why))
    cuts = sorted({lo, hi} | {m[0] for m in marks} | {m[1] for m in marks})
    for x, y in zip(cuts, cuts[1:]):
        why = [m[2] for m in marks if m[0] <= x and y <= m[1]]
        acc[why[-1] if why else 'other'] += y - x


# ---- linker script ----
def fix_path(o):
    return os.path.join(OUT, 'fix', o.src.replace('/', '__') + '.o')


def fixed(objs):
    """normalize each object for ld.lld (llvm-objcopy re-sorts the symbol table: gcc 2.95's as leaves locals after
    globals), apply the per-object renames of config/link_aliases.txt, and patch what script() decided: section sizes
    cut to their one symbol, alignments lowered to what the ELF's address allows (o.align), and sections whose bytes
    another object already places bound to that address (o.absolute: their symbols become absolute)"""
    red = os.path.join(OUT, 'redefine')
    for o in objs:
        p = fix_path(o)
        os.makedirs(os.path.dirname(p), exist_ok=True)
        r = os.path.join(red, o.src.replace('/', '__') + '.redefine')
        subprocess.run([llvm_tool('llvm-objcopy')] + (['--redefine-syms=' + r] if os.path.exists(r) else []) + [o.path, p], check=True)
        cut = {o.secs[i][0]: n for i, n in o.size.items() if n != o.secs[i][5]}
        if not (cut or o.align or o.absolute):
            continue
        d = bytearray(open(p, 'rb').read())
        shoff, = struct.unpack_from('<I', d, 0x20)
        secs = elf_secs(p)[1]
        for i, sec in enumerate(secs):                       # llvm-objcopy keeps section names and order
            if sec[0] in cut:
                struct.pack_into('<I', d, shoff + 40 * i + 20, cut[sec[0]])
            if sec[0] in o.align:
                struct.pack_into('<I', d, shoff + 40 * i + 32, o.align[sec[0]])
        absi = {i: o.absolute[sec[0]] for i, sec in enumerate(secs) if sec[0] in o.absolute}
        st = next(sec for sec in secs if sec[1] == 2)
        for k in range(st[5] // 16):
            e = st[4] + 16 * k
            val, info, shndx = struct.unpack_from('<I', d, e + 4)[0], d[e + 12], struct.unpack_from('<H', d, e + 14)[0]
            if shndx in absi:
                struct.pack_into('<I', d, e + 4, val + absi[shndx])
                struct.pack_into('<H', d, e + 14, 0xFFF1)    # SHN_ABS
                if info & 0xF == 3:                          # STT_SECTION -> STT_NOTYPE
                    d[e + 12] = info & 0xF0
        open(p, 'wb').write(d)


def script(objs, base, img, unbuilt):
    """the linker script text, the layout [(start, end, owner)], blob bytes by reason"""
    secs = [s for s in img.secs if s[0] != '.stack']
    entries = collections.defaultdict(list)                  # output section -> [(base, size, align, spec, owner)]
    firsts = set()
    for (k, si), b in sorted(base.items()):
        o, s = objs[k], objs[k].secs[si]
        out = img.sec_at(b)
        if out is None or out[0] == '.stack':
            continue
        name = s[0]
        if name.startswith('.gnu.linkonce.'):                # one copy per object: link the first placed one and
            if name in firsts:                               # bind every other object's copy to its address, so
                continue                                     # whichever weak definition ld.lld picks is right
            firsts.add(name)
            for k2, o2 in enumerate(objs):
                if k2 != k and any(x[0] == name for x in o2.secs):
                    o2.absolute[name] = b
        spec = f'KEEP({q(fix_path(o))}({q(name)}))'
        entries[out[0]].append((b, o.size[si], s[8], spec, f'{o.src}({name})', s[1] == 8, (k, si)))
    lines, layout, why, overlaps, dups, misaligned = [], [], collections.Counter(), [], [], []
    blobs, reasons = [], blob_reasons(img, unbuilt)
    for s in secs:
        name, typ, flags, va, off, size = s[:6]
        nobits = typ == 8
        body, cur, seen = [], va, set()
        for b, n, al, spec, owner, nb, key in sorted(entries[name]) + [(va + size, 0, 1, None, None, True, None)]:
            if spec in seen:                                 # several copies of one linkonce section: placed once
                continue
            seen.add(spec)
            if b > cur:
                if nobits:
                    body.append(f'    . = 0x{b:X};')
                else:
                    blobs.append((cur, b - cur, flags))
                    body.append(f'    KEEP({q(os.path.join(OUT, "blobs.o"))}(.blob.{cur:08X}))')
                    reasons_of(cur, b, reasons, why)
                layout.append((cur, b, 'blob', None))
                cur = b
            if spec is None:
                break
            k, si = key or (0, 0)
            if b >= cur and b % al:                          # stricter alignment than the ELF's address allows
                objs[k].align[objs[k].secs[si][0]] = b & -b
                misaligned.append((owner, b, al))
            elif b < cur:                                    # overlaps what is placed
                if nb or same_bytes(objs[k], si, b, n, img):
                    # the same bytes as the ELF at that address (generated per-function files repeating a string of
                    # their one original TU), or no bytes (.bss under a section whose layout differs): not linked;
                    # its symbols are bound to the address
                    objs[k].absolute[objs[k].secs[si][0]] = b
                    dups.append((owner, b, n))
                else:                                        # anything else: left to .unplaced
                    overlaps.append((owner, b, n, key))
                continue
            body.append(f'    . = 0x{b:X};')
            body.append(f'    {spec}')
            layout.append((b, b + n, owner, key))
            cur = b + n
        lines.append(f'  {name} 0x{va:X}{" (NOLOAD)" if nobits else ""} : {{\n' + '\n'.join(body) + '\n  }')
    write_blobs(os.path.join(OUT, 'blobs.o'), blobs, img)
    return '\n'.join(lines), layout, why, overlaps, dups, misaligned


def symbols(objs, img, ext):
    """_gp, linkaliases.py's lines, and absolute addresses for what only unbuilt code/data defines"""
    p = subprocess.run([sys.executable, os.path.join(ROOT, 'tools', 'linkaliases.py'), '--redefine',
                        os.path.join(OUT, 'redefine')] + [o.path for o in objs], capture_output=True, text=True, check=True)
    lines = [f'_gp = 0x{img.gp:X};'] + p.stdout.split('\n')
    defined = {s[0] for o in objs for s in o.syms if s[3] and s[5] and s[5] != 0xFFF2}
    commons = {s[0] for o in objs for s in o.syms if s[3] and s[5] == 0xFFF2}
    assigned = {re.match(r'"?([^"\s]+)', l).group(1) for l in lines if '=' in l}
    rhs = {re.search(r'=\s*"?([^";]+)', l).group(1) for l in lines if '=' in l}
    undef = {s[0] for o in objs for s in o.syms if s[3] and s[5] == 0} | rhs
    addrs = config_addrs()
    extra, derived = [], []
    for n in sorted((undef - defined - assigned) | commons):
        a = addr_of(n, addrs)
        if a is None and n in ext:                           # named by no config file: the ELF's relocated value
            a = ext[n]; derived.append(n)
        if a is not None and not n.startswith('0x'):
            extra.append(f'{linkaliases.q(n)} = 0x{a:08X};')  # COMMON too: its address is the ELF's
    return '\n'.join(lines + extra), derived


# ---- compare ----
def mismatch_class(objs, key, off, base, odd, gdef):
    """why one differing byte of a placed object section differs: (class, detail)"""
    if key is None:
        return 'blob', ''
    k, si = key
    o = objs[k]
    if key in odd:
        return 'in an object section whose internal layout differs from the ELF', f'{o.src}({o.secs[si][0]})'
    r = next((r for r in o.rels.get(si, []) if r[0] == off & ~3), None)
    if r is None:
        return 'bytes differ, no relocation', f'{o.src}({o.secs[si][0]})'
    name, val, size, bind, typ, shndx = o.syms[r[2]]
    t = {2: 'R_MIPS_32', 4: 'R_MIPS_26', 5: 'HI16', 6: 'LO16', 7: 'GPREL16'}.get(r[1], str(r[1]))
    if not 0 < shndx < 0xFF00 and name in gdef:
        k, shndx = gdef[name]
        o = objs[k]
    if 0 < shndx < 0xFF00:
        sec = f'{o.src}({o.secs[shndx][0]})'
        if (k, shndx) in odd:
            return f'{t} to an object section whose internal layout differs from the ELF', sec
        if (k, shndx) not in base:
            return f'{t} to an unplaced object section', sec
        return f'{t} to an object section at its ELF address', sec
    return f'{t} to a symbol no object defines', name


def compare(path, img, layout, objs, base, odd):
    gdef = {s[0]: (k, s[5]) for k, o in enumerate(objs) for s in o.syms if s[3] and 0 < s[5] < 0xFF00}
    d, secs = elf_secs(path)
    got = [s for s in secs if s[2] & 2 and s[1] == 1]
    starts = [l[0] for l in sorted(layout)]
    layout = sorted(layout)
    import bisect
    print(f'{"section":10s} {"bytes":>9s} {"equal":>9s} {"match":>7s} {"from objects":>12s} {"equal":>9s} {"match":>7s}'
          '  first difference')
    worst, classes, detail = collections.Counter(), collections.Counter(), collections.defaultdict(collections.Counter)
    for s in img.secs:
        if s[1] == 8 or s[0] == '.stack' or not s[5]:
            continue
        ref = img.d[s[4]:s[4] + s[5]]
        ours = bytearray(s[5])
        have = bytearray(s[5])
        for g in got:
            lo, hi = max(g[3], s[3]), min(g[3] + g[5], s[3] + s[5])
            if lo < hi:
                ours[lo - s[3]:hi - s[3]] = d[g[4] + lo - g[3]:g[4] + hi - g[3]]
                have[lo - s[3]:hi - s[3]] = b'\1' * (hi - lo)
        bad = [i for i in range(s[5]) if ours[i] != ref[i] or not have[i]]
        first = ''
        if bad:
            a = s[3] + bad[0]
            l = layout[bisect.bisect_right(starts, a) - 1]
            first = f'0x{a:X} in {l[2]}'
        for i in bad:
            a = s[3] + i
            l = layout[bisect.bisect_right(starts, a) - 1]
            worst[(s[0], l[2])] += 1
            c, why = mismatch_class(objs, l[3], a - l[0], base, odd, gdef)
            detail[c][why] += 1
            classes[(s[0], c)] += 1
        objb = sum(min(e, s[3] + s[5]) - max(a, s[3]) for a, e, owner, key in layout
                   if key is not None and a < s[3] + s[5] and e > s[3])
        objbad = sum(1 for i in bad if layout[bisect.bisect_right(starts, s[3] + i) - 1][3] is not None)
        pct = f'{100 * (objb - objbad) / objb:6.2f}%' if objb else '      -'
        print(f'{s[0]:10s} {s[5]:9d} {s[5] - len(bad):9d} {100 * (s[5] - len(bad)) / s[5]:6.2f}% {objb:12d} '
              f'{objb - objbad:9d} {pct}  {first}')
    print('\ndiffering bytes by class:')
    for (sec, c), n in classes.most_common():
        print(f'  {n:8d}  {sec:8s} {c}')
    with open(os.path.join(OUT, 'mismatch.txt'), 'w') as f:
        for c, names in detail.items():
            f.write(c + ':\n' + ''.join(f'  {n:6d} {t}\n' for t, n in names.most_common(30)))
    print('\nmost differing bytes by owner:')
    for (sec, owner), n in worst.most_common(40):
        print(f'  {n:8d}  {sec:8s} {owner}')


def symbol_check(path, img):
    """config names at the wrong address in the output (covers .sbss/.bss, which have no bytes)"""
    d, secs = elf_secs(path)
    addrs = config_addrs()
    st = next(s for s in secs if s[1] == 2)
    stro = secs[st[6]][4]
    ok = bad = 0
    wrong, badnames = collections.Counter(), []
    for i in range(st[5] // 16):
        nm, val, size, info, other, shndx = struct.unpack_from('<IIIBBH', d, st[4] + i * 16)
        n = d[stro + nm:d.index(b'\0', stro + nm)].decode()
        a = addr_of(n, addrs) if info & 0xF in (1, 2) and shndx not in (0, 0xFFF1) else None
        if a is None:
            continue
        sec = img.sec_at(a)
        if val == a:
            ok += 1
        else:
            bad += 1
            wrong[sec[0] if sec else '?'] += 1
            badnames.append(f'{n} 0x{val:X} (config 0x{a:X})')
    open(os.path.join(OUT, 'wrongaddr.txt'), 'w').writelines(b + '\n' for b in badnames)
    print(f'\nsymbols at their config address: {ok} of {ok + bad}; wrong by section: {dict(wrong)} '
          '(all in build/link/wrongaddr.txt): ' + ', '.join(badnames[:12]))


def tu_check(objs, base):
    """how far each object's code is from linking as one TU section: its functions must sit together in the ELF
    (no other object's code between them) and in source order (with -ffunction-sections every function is placed on
    its own, so this is what a link without the flag would trip over)"""
    text = sorted((b, k, i) for (k, i), b in base.items() if objs[k].secs[i][0].startswith('.text')
                  and objs[k].secs[i][5] and 0x100000 <= b < 0x259DF0)
    pos = collections.defaultdict(list)
    for n, (b, k, i) in enumerate(text):
        pos[k].append((n, i))
    ready, order, split = 0, [], []
    for k, ps in pos.items():
        runs = 1 + sum(1 for a, b in zip(ps, ps[1:]) if b[0] != a[0] + 1)
        if runs > 1:
            split.append((runs, objs[k].src))
        elif [i for n, i in ps] != sorted(i for n, i in ps):
            order.append(objs[k].src)
        else:
            ready += 1
    print(f'\nTU check (.text): {ready} objects link as one section; {len(order)} contiguous but in another order '
          f'than the source ({", ".join(order[:8])}); {len(split)} split into several runs by other objects\' code '
          f'({", ".join(f"{s} {g}" for g, s in sorted(split, reverse=True)[:10])})')
    tu_text_check(objs, base, {s for g, s in split})
    return split


def tu_text_check(objs, base, split):
    """the same question as a link without -ffunction-sections asks it: lay each object's .text sections out
    back to back in source order with their alignments (what gas does with one .text) and require one base address
    for all of them.  Objects that fail are split by other objects' code (above), or have ELF bytes inside their
    range that no source emits (junk.csv dead-code scraps), or a gap/padding that differs; per object in
    build/link/tutext.txt"""
    junk = matchtool.load_junk()
    good, bad = 0, collections.Counter()
    lines = []
    for k, o in enumerate(objs):
        secs = [i for i in o.alloc() if o.secs[i][0].startswith('.text') and 0 < o.secs[i][5]]
        if not secs or not any((k, i) in base for i in secs):
            continue
        off, pos = 0, []
        for i in secs:
            al = max(o.secs[i][8], 1)
            off = (off + al - 1) // al * al
            pos.append((i, off))
            off += o.secs[i][5]
        if any((k, i) not in base for i, _ in pos):
            why = 'unplaced section'
        elif len({base[k, i] - p for i, p in pos}) == 1:
            good += 1
            continue
        elif o.src in split:
            why = 'split by other objects\' code'
        else:
            gaps = []
            for (i, p), (j, q) in zip(pos, pos[1:]):
                d = (base[k, j] - base[k, i]) - (q - p)
                if d:
                    end = base[k, i] + o.secs[i][5]
                    inj = any(a < base[k, j] and e > end for a, e in junk)
                    gaps.append(f'0x{end:X}{d:+d}' + (' junk' if inj else ''))
            why = 'dead code (junk.csv) inside' if all(g.endswith('junk') for g in gaps) else 'gap/padding differs'
            why += ': ' + ' '.join(gaps[:6])
        bad[why.split(':')[0]] += 1
        lines.append(f'{o.src}: {why}\n')
    open(os.path.join(OUT, 'tutext.txt'), 'w').writelines(sorted(lines))
    print(f'TU check without -ffunction-sections (.text back to back in source order): {good} objects link as one '
          f'run at their ELF address; {sum(bad.values())} do not: {dict(bad)} (build/link/tutext.txt)')


def main():
    if '--check-tools' in sys.argv:          # quick preflight (tools/verify.sh): exits non-zero if a tool is missing
        for t in ('ld.lld', 'llvm-objcopy'):
            print(f'{t}: {llvm_tool(t)}')
        return
    os.chdir(ROOT)
    ok, skip = sources()
    if '--no-compile' not in sys.argv:
        compile_all(ok)
    objs = [Obj(obj_path(s), s) for s in ok if os.path.exists(obj_path(s))]
    print(f'{len(objs)} objects ({len(ok) - len(objs)} failed to compile), {len(skip)} sources not buildable here',
          file=sys.stderr)
    img = Image(ELF)
    base, conflicts, ext = place(objs, img)
    with open(os.path.join(OUT, 'conflicts.txt'), 'w') as f:
        for (k, si), c in sorted(conflicts.items()):
            f.write(f'{objs[k].src}({objs[k].secs[si][0]}): ' + ' '.join(f'0x{b:X}x{n}' for b, n in c.most_common(6)) + '\n')
    unplaced = [(objs[k].src, objs[k].secs[i][0], objs[k].secs[i][5]) for k, o in enumerate(objs)
                for i in o.alloc() if (k, i) not in base]
    open(os.path.join(OUT, 'unplaced.txt'), 'w').writelines(f'{n:6d} {s}({sec})\n' for s, sec, n in unplaced)
    syms, derived = symbols(objs, img, ext)
    secs, layout, why, overlaps, dups, misaligned = script(objs, base, img, set(skip))
    fixed(objs)
    paths = [fix_path(o) for o in objs]
    tpl = open(os.path.join(ROOT, 'tools', 'link.ld.in')).read()
    ld = os.path.join(OUT, 'sly2.ld')
    open(ld, 'w').write(tpl.replace('@SECTIONS@', secs).replace('@SYMBOLS@', syms))
    out_of_place = {o[3] for o in overlaps}
    elf = os.path.join(OUT, 'sly2.elf')
    if os.path.exists(elf):
        os.remove(elf)
    p = subprocess.run([llvm_tool('ld.lld'), '-m', 'elf32ltsmip', '-T', ld, '--gc-sections', '--noinhibit-exec',
                        '--no-check-sections', '-o', elf] + [os.path.join(OUT, 'blobs.o')] + paths,
                       capture_output=True, text=True)
    open(os.path.join(OUT, 'ld.log'), 'w').write(p.stdout + p.stderr)
    errs = collections.Counter(re.sub(r'[:(].*', '', l) for l in p.stderr.splitlines() if 'error:' in l)
    print(f'ld.lld: exit {p.returncode}, {sum(errs.values())} errors (build/link/ld.log): {dict(errs)}')
    print(f'placed {len(base)} object sections; {len(conflicts)} with disagreeing evidence; '
          f'{len(unplaced)} unplaced ({sum(u[2] for u in unplaced)} bytes, to .unplaced)')
    open(os.path.join(OUT, 'overlaps.txt'), 'w').writelines(f'0x{b:X} {n:5d} {o}\n' for o, b, n, _ in overlaps)
    print(f'{len(dups)} sections overlap another but repeat its ELF bytes or are .bss (bound there, {sum(d[2] for d in dups)} '
          f'bytes); {len(misaligned)} sections aligned more strictly than their ELF address (alignment lowered): '
          + ', '.join(f'{m[0]}@0x{m[1]:X}/{m[2]}' for m in misaligned[:6]))
    print(f'{len(overlaps)} other placed sections overlap others (to .unplaced, {sum(o[2] for o in overlaps)} bytes): '
          + ', '.join(f'{o[0]}@0x{o[1]:X}' for o in overlaps[:12]))
    print(f'{len(derived)} undefined names no config file knows, resolved from the ELF\'s relocated fields: '
          + ' '.join(derived[:20]))
    print('blob bytes by reason: ' + ', '.join(f'{k} {v}' for k, v in why.most_common()))
    if os.path.exists(elf):
        compare(elf, img, layout, objs, base, out_of_place | set(conflicts))
        symbol_check(elf, img)
    tu_check(objs, base)


if __name__ == '__main__':
    main()
