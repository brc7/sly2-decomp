#!/usr/bin/env python3
"""Full link of SCUS_973.16 from the matched sources (docs/guides/full-link.md).

usage: python3 tools/link.py [--no-compile] [--per-function] [--sdk] | --check-tools

  1. compile every source of config/matched.csv that builds here with matchtool's compile_src, as the original build
     did: one .text/.data/.rodata/.bss per object plus its .gnu.linkonce.* sections (build/link/obj-tu/).  An object
     whose .text is not yet one run at its ELF address (linkonce copies kept as plain functions, junk.csv scraps
     inside, padding) falls back to a -ffunction-sections build of the same source (build/link/obj/), each function
     placed on its own; the reason per object goes to build/link/tutext.txt.  --per-function builds every object that
     way (the old prototype mode); --no-compile reuses the objects already there.
  2. place every object section at the address its symbols and relocations give it in the ELF, fill what no object
     covers with byte ranges cut from your disc/SCUS_973.16 (build/link/blobs.o, never committed), and write
     build/link/sly2.ld from tools/link.ld.in: sections in address order, an address assignment only where the
     linker's own layout would not land there
  3. link with ld.lld and compare build/link/sly2.elf with the ELF section by section
"""
import bisect, collections, csv, functools, os, re, shutil, struct, subprocess, sys
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import matchtool, linkaliases

OUT = os.path.join(ROOT, 'build', 'link')
ELF = matchtool.ELF
LINKONCE = (0x254F00, 0x259DF0)                              # the ELF's .gnu.linkonce.t area (end of .text)


@functools.lru_cache(None)
def llvm_tool(name):
    """path of an LLVM tool: $LLVM_BIN, then PATH, then Homebrew's keg-only llvm/lld (not on PATH by default)"""
    dirs = [os.environ['LLVM_BIN']] if os.environ.get('LLVM_BIN') else []
    found = shutil.which(name)
    if found and not dirs:
        return found
    dirs += [f'{p}/{k}/bin' for p in ('/opt/homebrew/opt', '/usr/local/opt') for k in ('llvm', 'lld')]
    for d in dirs:
        if os.path.isfile(os.path.join(d, name)):
            return os.path.join(d, name)
    if found:
        return found
    sys.exit(f'link.py: {name} not found. Install LLVM (macOS: `brew install llvm lld` -- Homebrew ships ld.lld in its own '
             f'`lld` formula since LLVM 20; Debian/Ubuntu: `apt install lld llvm`) or set LLVM_BIN to the directory that '
             f'contains it.')


def sources(sdk=False):
    """(buildable, unbuildable) source paths of config/matched.csv.  The SDK sources assembled by GNU as
    (`SLY2_ASM: gnu-reorder*`) or compiled by the native ee-gcc 2.96 need i386 Linux binaries: unbuildable unless
    --sdk asks for them (gnu_reorder runs GNU as through Docker on macOS; native Linux needs /lib/ld-linux.so.2)"""
    srcs = sorted({r['source'] for r in csv.DictReader(open(os.path.join(ROOT, 'config', 'matched.csv')))})
    loader = os.path.exists('/lib/ld-linux.so.2')
    ok, skip = [], []
    for s in srcs:
        head = open(os.path.join(ROOT, s), errors='ignore').read(2000)
        cc = re.search(r'SLY2_CC:\s*(\S+)', head)
        native = cc and os.path.exists(os.path.join(ROOT, cc.group(1), 'bin', 'ee-gcc'))
        gnu_as = re.search(r'SLY2_ASM:\s*gnu-reor', head)
        (skip if (native and not (sdk and loader)) or (gnu_as and not sdk) else ok).append(s)
    return ok, skip


def obj_path(src, per_function):
    return os.path.join(OUT, 'obj' if per_function else 'obj-tu', src + '.o')


def compile_all(srcs, per_function):
    """-ffunction-sections only for the per-function objects (codegen is unchanged: every function MATCHes in both);
    appended to matchtool's include list, the one argument list every compile path of compile_src passes on"""
    flag = '-ffunction-sections'
    matchtool.INCLUDES = [x for x in matchtool.INCLUDES if x != flag] + ([flag] if per_function else [])
    def one(s):
        o = obj_path(s, per_function)
        os.makedirs(os.path.dirname(o), exist_ok=True)
        if os.path.exists(o):
            os.remove(o)                               # no stale object if the compile fails
        try:
            shutil.move(matchtool.compile_src(s), o)   # relative path, as matchtool is run (the path-hash quirk)
        except SystemExit:
            print(f'FAILED: {s}', file=sys.stderr)
    with ThreadPoolExecutor(os.cpu_count() or 2) as ex:
        for i, _ in enumerate(ex.map(one, srcs)):
            if i % 50 == 0:
                print(f'compiled {i}/{len(srcs)}{" (per function)" if per_function else ""}', file=sys.stderr)


# ---- ELF reading ----
def elf_secs(path):
    """[(name, type, flags, addr, offset, size, link, info, align)] of an ELF32-LE file"""
    d = open(path, 'rb').read()
    shoff, = struct.unpack_from('<I', d, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2E)
    hdr = [struct.unpack_from('<10I', d, shoff + i * shentsize) for i in range(shnum)]
    so = hdr[shstrndx][4]
    return d, [(d[so + h[0]:d.index(b'\0', so + h[0])].decode(),) + h[1:9] for h in hdr]


def symtab(d, secs):
    """[(name, value, size, bind, type, shndx)]"""
    st = next((s for s in secs if s[1] == 2), None)
    if st is None:
        return []
    stro, out = secs[st[6]][4], []
    for i in range(st[5] // 16):
        nm, val, size, info, other, shndx = struct.unpack_from('<IIIBBH', d, st[4] + i * 16)
        out.append((d[stro + nm:d.index(b'\0', stro + nm)].decode(), val, size, info >> 4, info & 0xF, shndx))
    return out


class Obj:
    """sections, symbols and relocations of one relocatable object"""
    def __init__(self, src, per_function):
        self.src, self.per_function, self.path = src, per_function, obj_path(src, per_function)
        self.data, self.secs = elf_secs(self.path)
        self.syms = symtab(self.data, self.secs)
        self.rels = {s[7]: [(o, i & 0xFF, i >> 8) for o, i in (struct.unpack_from('<II', self.data, s[4] + 8 * k)
                                                               for k in range(s[5] // 8))]
                     for s in self.secs if s[1] == 9}    # SHT_REL: target section -> [(offset, type, symbol)]
        self.size = {i: sec[5] for i, sec in enumerate(self.secs)}
        self.align, self.absolute = {}, {}                   # patches decided by script(), applied by fixed()

    def word(self, sec, off):
        return struct.unpack_from('<I', self.data, self.secs[sec][4] + off)[0]

    def alloc(self):
        """indices of the allocated sections with contents or size"""
        return [i for i, s in enumerate(self.secs) if s[2] & 2 and s[5] and s[1] in (1, 8)]

    def text(self):
        return [i for i in self.alloc() if self.secs[i][0].startswith('.text')]

    def sym_end(self, si):
        """end of the last sized symbol of a section (gas pads every section to its alignment after it)"""
        return max([v + n for nm, v, n, b, t, sh in self.syms if sh == si and t in (1, 2)] or [0])


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
@functools.lru_cache(None)
def config_addrs():
    """every name config gives an address (functions.csv, symbol_addrs.txt; D_/func_ names by convention)"""
    a = {n: v[0] for n, v in matchtool.load_functions().items()}
    for line in open(os.path.join(ROOT, 'config', 'symbol_addrs.txt')):
        m = re.match(r'\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+);', line)
        if m:
            a.setdefault(m.group(1), int(m.group(2), 16))
    return a


def addr_of(name):
    if name in config_addrs():
        return config_addrs()[name]
    m = linkaliases.ADDR_NAME.match(name)
    return int(m.group(1), 16) if m else None


def named(o):
    """(symbol, config address or None) for the defined functions and objects of o, through its SLY2_ALIAS lines"""
    names = {s[0] for s in o.syms if s[0]}
    alias = {old: new for new, old in matchtool.apply_aliases(os.path.join(ROOT, o.src), {n: n for n in names}).items()}
    for s in o.syms:
        if s[0] and 0 < s[5] < 0xFF00 and s[4] in (1, 2):
            yield s, addr_of(alias.get(s[0], s[0]))


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
    """{(object index, section index): base address}, the sections whose evidence disagreed, and the ELF's values of
    undefined names: votes from every symbol with a config address (weighted by size), then propagated through the
    relocations of placed sections (the ELF's relocated field minus the object's addend is the target's address)"""
    votes = collections.defaultdict(collections.Counter)
    gdef = collections.defaultdict(list)                     # global name -> [(obj, sec, value)]
    for k, o in enumerate(objs):
        for (name, val, size, bind, typ, shndx), a in named(o):
            if bind:
                gdef[name].append((k, shndx, val))
            if a is not None:
                votes[(k, shndx)][a - val] += 10 + size     # a big object outvotes small ones
    base, ext = {}, collections.defaultdict(collections.Counter)
    todo = list(votes)
    while todo:
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


# ---- which objects can link whole ----
def tu_verdict(o, img, owner, junk):
    """None if every function of o's .text sits at its config address relative to one base (the object links as
    one .text, the way gas lays it out), else why not: the reason classes of the TU check"""
    t = o.text()
    if not t:
        return None
    if len(t) > 1:
        return 'several .text sections: ' + ' '.join(o.secs[i][0] for i in t)
    fns = sorted((s[1], s[2], a, s[0]) for s, a in named(o) if s[5] == t[0] and s[4] == 2)
    lost = [n for v, sz, a, n in fns if a is None or not img.sec_at(a)]
    if lost:
        return 'code the ELF does not have: ' + ' '.join(lost[:4])
    if len({a - v for v, sz, a, n in fns}) <= 1:
        return None
    if any(LINKONCE[0] <= a < LINKONCE[1] for v, sz, a, n in fns):
        return 'linkonce copies as plain functions: ' + ' '.join(n for v, sz, a, n in fns if LINKONCE[0] <= a < LINKONCE[1])[:200]
    gaps, why = [], set()
    for (v1, s1, a1, n1), (v2, s2, a2, n2) in zip(fns, fns[1:]):
        d = (a2 - a1) - (v2 - v1)
        if d:
            lo, hi = a1 + s1, a2
            if d < 0:
                why.add('functions in another order')
            elif any(lo <= a < hi and src != o.src for a, src in owner):
                why.add("split by other objects' code")
            elif any(a < hi and e > lo for a, e in junk):
                why.add('dead code (junk.csv) inside')
            else:
                why.add('gap/padding differs')
            gaps.append(f'0x{lo:X}{d:+d}')
    return ' + '.join(sorted(why)) + ': ' + ' '.join(gaps[:6])


def tu_check(srcs, img):
    """{source: reason} for the objects that cannot link whole yet; writes build/link/tutext.txt"""
    matched = {int(r['addr'], 16): r['source'] for r in csv.DictReader(open(os.path.join(ROOT, 'config', 'matched.csv')))}
    owner = sorted(matched.items())
    junk = matchtool.load_junk()
    bad = {}
    for s in srcs:
        why = tu_verdict(Obj(s, False), img, owner, junk)
        if why:
            bad[s] = why
    open(os.path.join(OUT, 'tutext.txt'), 'w').writelines(f'{s}: {w}\n' for s, w in sorted(bad.items()))
    return bad


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


def blob_reasons(img, unbuilt):
    """(start, end, reason) of what config says is at the addresses blobs may cover; narrower rows sort later"""
    out = [(s[3], s[3] + s[5], {'.vutext': 'VU microcode', '.irx': 'IOP modules (.irx)', '.text': 'text: padding / not '
            'in config'}.get(s[0], 'data no object defines')) for s in img.secs]
    matched = {int(r['addr'], 16): r['source'] for r in csv.DictReader(open(os.path.join(ROOT, 'config', 'matched.csv')))}
    for r in csv.DictReader(open(os.path.join(ROOT, 'config', 'functions.csv'))):
        src = matched.get(int(r['Start'], 16))
        out.append((int(r['Start'], 16), int(r['End'], 16), 'text: unmatched function' if src is None else
                    'text: SDK source that does not build here' if src in unbuilt else 'text: matched but not in an object'))
    out += [(a, e, 'text: dead code (junk.csv)') for a, e in matchtool.load_junk()]
    return sorted(out, key=lambda r: (r[0], -r[1]))


def reasons_of(lo, hi, reasons, acc):
    """add the bytes of [lo, hi) to acc by reason (the narrowest row covering a byte wins)"""
    marks = [(max(a, lo), min(e, hi), why) for a, e, why in reasons if e > lo and a < hi]
    cuts = sorted({lo, hi} | {m[0] for m in marks} | {m[1] for m in marks})
    for x, y in zip(cuts, cuts[1:]):
        why = [m[2] for m in marks if m[0] <= x and y <= m[1]]
        acc[why[-1] if why else 'other'] += y - x


# ---- linker script ----
def fix_path(o):
    return os.path.join(OUT, 'fix', o.src.replace('/', '__') + '.o')


def q(s):
    return '"' + s + '"'


def fixed(objs):
    """normalize each object for ld.lld (llvm-objcopy re-sorts the symbol table: gcc 2.95's as leaves locals after
    globals), apply the per-object renames of config/link_aliases.txt, and patch what script() decided: section
    padding cut where the next section starts (o.size), alignments lowered to what the ELF's address allows (o.align),
    and sections whose bytes another object already places bound to that address (o.absolute: symbols made absolute)"""
    for o in objs:
        p = fix_path(o)
        os.makedirs(os.path.dirname(p), exist_ok=True)
        r = os.path.join(OUT, 'redefine', o.src.replace('/', '__') + '.redefine')
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


def same_bytes(o, si, b, n, img):
    """the section's bytes equal the ELF's at b, words with a relocation aside"""
    t = img.bytes(b, n)
    if t is None:
        return False
    mine = o.data[o.secs[si][4]:o.secs[si][4] + n]
    masked = {r[0] // 4 for r in o.rels.get(si, [])}
    return all(mine[i] == t[i] for i in range(n) if i // 4 not in masked)


def script(objs, base, img, reasons):
    """the SECTIONS text, the layout [(start, end, owner, key)], blob bytes by reason, and the sections not linked
    (dups: bound to the address whose bytes they repeat; overlaps: to .unplaced) and alignment-lowered ones"""
    entries, first = collections.defaultdict(list), {}
    for (k, si), b in sorted(base.items()):
        out, name = img.sec_at(b), objs[k].secs[si][0]
        if out is None or out[0] == '.stack':
            continue
        if name.startswith('.gnu.linkonce.'):                # one copy per object: link the first placed one and
            if name in first:                                # bind every other object's copy to its address, so
                continue                                     # whichever weak definition ld.lld picks is right
            first[name] = (k, b)
        entries[out[0]].append((b, k, si))
    for k, o in enumerate(objs):
        for s in o.secs:
            if s[0] in first and first[s[0]][0] != k:
                o.absolute[s[0]] = first[s[0]][1]
    lines, layout, why, blobs, st = [], [], collections.Counter(), [], collections.defaultdict(list)
    st['assign'] = st['bss gap'] = 0
    for s in img.secs:
        name, typ, flags, va, off, size = s[:6]
        if name == '.stack':
            continue
        nobits, body, cur, prev = typ == 8, [], va, None
        for b, k, si in sorted(entries[name]) + [(va + size, None, None)]:     # the end of the section last
            if b < cur and prev:                             # the previous section's padding runs into this one:
                po, psi, pb = prev                           # cut it where the ELF starts the next section
                tail = po.data[po.secs[psi][4] + b - pb:po.secs[psi][4] + po.size[psi]]
                if b - pb >= po.sym_end(psi) and (po.secs[psi][1] == 8 or not any(tail)):
                    po.size[psi] = b - pb
                    layout[-1] = (pb, b) + layout[-1][2:]
                    cur = b
            al = 1
            if k is not None:
                o = objs[k]
                sec, n, al = o.secs[si], o.size[si], max(o.secs[si][8], 1)
                owner = f'{o.src}({sec[0]})'
                if b < cur:                                  # overlaps what is placed
                    if sec[1] == 8 or same_bytes(o, si, b, n, img):
                        # no bytes (.bss under a section whose layout differs), or the ELF's bytes at that address:
                        # not linked, its symbols are bound to the address
                        o.absolute[sec[0]] = b
                        st['dups'].append((owner, b, n))
                    else:
                        st['overlaps'].append((owner, b, n, (k, si)))
                    continue
                if b % al:                                   # stricter alignment than the ELF's address allows
                    o.align[sec[0]] = al = b & -b
                    st['misaligned'].append((owner, b, sec[8]))
            natural = (cur + al - 1) // al * al              # where the linker puts it on its own
            if b > cur and not nobits and (b != natural or any(img.bytes(cur, b - cur))):
                blobs.append((cur, b - cur, flags))          # bytes no object covers: cut from the ELF
                body.append(f'    KEEP({q(os.path.join(OUT, "blobs.o"))}(.blob.{cur:08X}))')
                reasons_of(cur, b, reasons, why)
                layout.append((cur, b, 'blob', None))
            elif b != natural and (k is not None or nobits):
                body.append(f'    . = 0x{b:X};')
                st['assign'] += 1
                st['bss gap'] += b - cur
            if k is None:
                break
            body.append(f'    KEEP({q(fix_path(o))}({q(sec[0])}))')
            layout.append((b, b + n, owner, (k, si)))
            prev, cur = (o, si, b), b + n
        lines.append(f'  {name} 0x{va:X}{" (NOLOAD)" if nobits else ""} : {{\n' + '\n'.join(body) + '\n  } =0')   # pad with zeros
    write_blobs(os.path.join(OUT, 'blobs.o'), blobs, img)
    return '\n'.join(lines), layout, why, st


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
    extra, derived = [], []
    for n in sorted((undef - defined - assigned) | commons):
        a = addr_of(n)
        if a is None and n in ext:                           # named by no config file: the ELF's relocated value
            a = ext[n]; derived.append(n)
        if a is not None and not n.startswith('0x'):
            extra.append(f'{linkaliases.q(n)} = 0x{a:08X};')  # COMMON too: its address is the ELF's
    return '\n'.join(lines + extra), derived


# ---- compare ----
def mismatch_class(objs, key, off, base, odd, gdef):
    """why one differing byte differs: (class, the section it is in or its relocation refers to)"""
    if key is None:
        return 'blob', ''
    o, si = objs[key[0]], key[1]
    here = f'{o.src}({o.secs[si][0]})'
    r = None if key in odd else next((r for r in o.rels.get(si, []) if r[0] == off & ~3), None)
    if r is None:
        return ('in an object section whose layout differs from the ELF' if key in odd else
                'bytes differ, no relocation'), here
    name, shndx = o.syms[r[2]][0], o.syms[r[2]][5]
    t = {2: 'R_MIPS_32', 4: 'R_MIPS_26', 5: 'HI16', 6: 'LO16', 7: 'GPREL16'}.get(r[1], str(r[1]))
    tk, ts = (key[0], shndx) if 0 < shndx < 0xFF00 else gdef.get(name, (None, None))
    if tk is None:
        return f'{t} to a symbol no object defines', name
    where = ('an object section whose layout differs' if (tk, ts) in odd else 'an unplaced object section'
             if (tk, ts) not in base else 'an object section at its ELF address')
    return f'{t} to {where}', f'{objs[tk].src}({objs[tk].secs[ts][0]})'


def compare(path, img, layout, objs, base, odd):
    """per section: equal bytes (all, and those object sections supply); differing bytes by class, details by owner
    in build/link/mismatch.txt"""
    gdef = {s[0]: (k, s[5]) for k, o in enumerate(objs) for s in o.syms if s[3] and 0 < s[5] < 0xFF00}
    d, secs = elf_secs(path)
    got = [s for s in secs if s[2] & 2 and s[1] == 1]
    layout = sorted(layout)
    starts = [l[0] for l in layout]
    print(f'{"section":10s} {"bytes":>9s} {"equal":>9s} {"match":>7s} {"from objects":>12s} {"equal":>9s} {"match":>7s}'
          '  first difference')
    classes, detail = collections.Counter(), collections.defaultdict(collections.Counter)
    for s in img.secs:
        if s[1] == 8 or s[0] == '.stack' or not s[5]:
            continue
        ref, ours, have = img.d[s[4]:s[4] + s[5]], bytearray(s[5]), bytearray(s[5])
        for g in got:
            lo, hi = max(g[3], s[3]), min(g[3] + g[5], s[3] + s[5])
            if lo < hi:
                ours[lo - s[3]:hi - s[3]] = d[g[4] + lo - g[3]:g[4] + hi - g[3]]
                have[lo - s[3]:hi - s[3]] = b'\1' * (hi - lo)
        bad = [i for i in range(s[5]) if ours[i] != ref[i] or not have[i]]
        objbad = 0
        for i in bad:
            l = layout[bisect.bisect_right(starts, s[3] + i) - 1]
            c, why = mismatch_class(objs, l[3], s[3] + i - l[0], base, odd, gdef)
            detail[c][f'{why or l[2]}'] += 1
            classes[(s[0], c)] += 1
            objbad += l[3] is not None
        objb = sum(min(e, s[3] + s[5]) - max(a, s[3]) for a, e, owner, key in layout
                   if key is not None and a < s[3] + s[5] and e > s[3])
        first = f'0x{s[3] + bad[0]:X} in {layout[bisect.bisect_right(starts, s[3] + bad[0]) - 1][2]}' if bad else ''
        pct = f'{100 * (objb - objbad) / objb:6.2f}%' if objb else '      -'
        print(f'{s[0]:10s} {s[5]:9d} {s[5] - len(bad):9d} {100 * (s[5] - len(bad)) / s[5]:6.2f}% {objb:12d} '
              f'{objb - objbad:9d} {pct}  {first}')
    print('\ndiffering bytes by class:')
    for (sec, c), n in classes.most_common():
        print(f'  {n:8d}  {sec:8s} {c}')
    with open(os.path.join(OUT, 'mismatch.txt'), 'w') as f:
        for c, names in detail.items():
            f.write(c + ':\n' + ''.join(f'  {n:6d} {t}\n' for t, n in names.most_common(30)))


def symbol_check(path, img):
    """config names at the wrong address in the output (covers .sbss/.bss, which have no bytes)"""
    d, secs = elf_secs(path)
    ok, wrong, badnames = 0, collections.Counter(), []
    for n, val, size, bind, typ, shndx in symtab(d, secs):
        a = addr_of(n) if typ in (1, 2) and shndx not in (0, 0xFFF1) else None
        if a is None:
            continue
        if val == a:
            ok += 1
        else:
            wrong[(img.sec_at(a) or ('?',))[0]] += 1
            badnames.append(f'{n} 0x{val:X} (config 0x{a:X})')
    open(os.path.join(OUT, 'wrongaddr.txt'), 'w').writelines(b + '\n' for b in badnames)
    print(f'\nsymbols at their config address: {ok} of {ok + len(badnames)}; wrong by section: {dict(wrong)} '
          '(all in build/link/wrongaddr.txt): ' + ', '.join(badnames[:12]))


def main():
    if '--check-tools' in sys.argv:          # quick preflight (tools/verify.sh): exits non-zero if a tool is missing
        for t in ('ld.lld', 'llvm-objcopy'):
            print(f'{t}: {llvm_tool(t)}')
        return
    os.chdir(ROOT)
    os.makedirs(OUT, exist_ok=True)
    per_function = '--per-function' in sys.argv
    compile = '--no-compile' not in sys.argv
    srcs, skip = sources('--sdk' in sys.argv)
    img = Image(ELF)
    if compile:
        compile_all(srcs, per_function)
    ok = [s for s in srcs if os.path.exists(obj_path(s, per_function))]
    fallback = {} if per_function else tu_check(ok, img)
    compile_all([s for s in fallback if compile or not os.path.exists(obj_path(s, True))], True)
    objs = [Obj(s, per_function or s in fallback) for s in ok]
    unbuilt = set(skip) | set(srcs) - set(ok)
    print(f'{len(objs)} objects ({len(srcs) - len(ok)} failed to compile), {len(skip)} sources not buildable here',
          file=sys.stderr)
    base, conflicts, ext = place(objs, img)
    unplaced = [(objs[k].src, o.secs[i][0], o.secs[i][5]) for k, o in enumerate(objs) for i in o.alloc() if (k, i) not in base]
    syms, derived = symbols(objs, img, ext)
    secs, layout, why, st = script(objs, base, img, blob_reasons(img, unbuilt))
    fixed(objs)
    ld, elf = os.path.join(OUT, 'sly2.ld'), os.path.join(OUT, 'sly2.elf')
    tpl = open(os.path.join(ROOT, 'tools', 'link.ld.in')).read()
    open(ld, 'w').write(tpl.replace('@SECTIONS@', secs).replace('@SYMBOLS@', syms))
    if os.path.exists(elf):
        os.remove(elf)
    p = subprocess.run([llvm_tool('ld.lld'), '-m', 'elf32ltsmip', '-T', ld, '--gc-sections', '--noinhibit-exec',
                        '--no-check-sections', '-o', elf, os.path.join(OUT, 'blobs.o')] + [fix_path(o) for o in objs],
                       capture_output=True, text=True)
    open(os.path.join(OUT, 'ld.log'), 'w').write(p.stdout + p.stderr)
    errs = collections.Counter(re.sub(r'[:(].*', '', l) for l in p.stderr.splitlines() if 'error:' in l)
    print(f'ld.lld: exit {p.returncode}, {sum(errs.values())} errors (build/link/ld.log): {dict(errs)}')
    if per_function:
        print(f'mode: per function ({len(objs)} objects, -ffunction-sections)')
    else:
        reasons = collections.Counter(w.split(':')[0] for w in fallback.values())
        print(f'mode: real link: {len(objs) - len(fallback)} objects whole (one .text/.data/.rodata/.bss each), '
              f'{len(fallback)} in per-function fallback: {dict(reasons)} (build/link/tutext.txt)')
    with open(os.path.join(OUT, 'sections.txt'), 'w') as f:    # every section not simply linked at its address
        f.writelines(f'evidence disagrees: {objs[k].src}({objs[k].secs[si][0]}): ' + ' '.join(
            f'0x{b:X}x{n}' for b, n in c.most_common(6)) + '\n' for (k, si), c in sorted(conflicts.items()))
        f.writelines(f'unplaced: {s}({sec}) {n} bytes\n' for s, sec, n in unplaced)
        f.writelines(f'{what}: {o} at 0x{b:X} ({n})\n' for what in ('dups', 'overlaps', 'misaligned') for o, b, n, *_ in st[what])
    print(f'placed {len(base)} object sections with {st["assign"]} address assignments (.bss/.sbss gaps no object '
          f'defines: {st["bss gap"]} bytes); not simply placed (build/link/sections.txt): {len(conflicts)} with '
          f'disagreeing evidence, {len(unplaced)} unplaced ({sum(u[2] for u in unplaced)} bytes, to .unplaced), '
          f'{len(st["dups"])} bound to the address whose bytes they repeat, {len(st["overlaps"])} overlapping (to '
          f'.unplaced), {len(st["misaligned"])} with their alignment lowered')
    print(f'{len(derived)} undefined names no config file knows, resolved from the ELF\'s relocated fields')
    print(f'blob bytes by reason ({sum(why.values())}): ' + ', '.join(f'{k} {v}' for k, v in why.most_common()))
    if os.path.exists(elf):
        compare(elf, img, layout, objs, base, {o[3] for o in st['overlaps']} | set(conflicts))
        symbol_check(elf, img)


if __name__ == '__main__':
    main()
