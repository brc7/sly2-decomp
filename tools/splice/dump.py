#!/usr/bin/env python3
"""Extract and pretty-print the Splice level scripts of Sly 2 (PS2, NTSC-U SCUS-97316) from your own disc image.

Usage:
  python3 tools/splice/dump.py DISC LEVEL... [-o OUTDIR] [--json] [--raw] [--width N] [--stats]
  python3 tools/splice/dump.py DISC --list

  DISC    a 2048-byte-sector ISO image, or a directory of sparse extents named <lba>_<sectors>.bin
  LEVEL   level-table indices (0 = prologue, 1 = front end, 2 = Paris hub, ...), ranges like 2-7, or "all"
  -o DIR  write one file per level (L02_nightclub.scm, and L02_nightclub.json with --json) into DIR;
          without -o a single level is printed to stdout
  --json  also write the decoded AST as JSON (with -o), or print JSON instead of Scheme (without -o)
  --raw   no name resolution: symbols sN, builtins #bif:N, no annotations
  --stats print validation statistics (tags, builtins, arity, allocation counts, round trip) to stderr
  --list  print the level table and exit

Everything is read from the disc: the executable's static file records locate the boot world, whose level table
locates every level; world files are LZSS-decompressed and walked in loader order (LoadSwFromBrx) to each world
entry's Splice table (LoadSwSpliceFromBrx -> DeserializePair). Names come from this repository: builtin spellings
from docs/reference/splice-language.md (checked against the builtin table in src/game/app/bif.cpp), class and option
labels from src/game/app/class_registry_options.inc (via tools/splice_options.py), symbol names 30-989 from the
level's own save-schema table. The format is documented in docs/reference/splice-dump.md.

The output is game content: write it outside the repository. Stdlib only, Python 3.8+.
"""
import argparse, json, math, os, re, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.abspath(os.path.join(HERE, '..', '..'))
SECTOR = 2048


class FormatError(Exception):
    pass


# ---------------------------------------------------------------------------------------------------------------------
# Disc access: ISO image or sparse extents; ISO 9660 root directory; the executable
# ---------------------------------------------------------------------------------------------------------------------
class Disc(object):
    """Reads sectors from an ISO file or from a directory of <lba>_<count>.bin extents."""

    def __init__(self, path):
        self.path = path
        self.extents = None
        if os.path.isdir(path):
            self.extents = []
            for fn in os.listdir(path):
                m = re.match(r'^(\d+)_(\d+)\.bin$', fn)
                if m:
                    self.extents.append((int(m.group(1)), int(m.group(2)), os.path.join(path, fn)))
            self.extents.sort()
            if not self.extents:
                raise FormatError('no <lba>_<count>.bin extents in %s' % path)
        else:
            self.f = open(path, 'rb')
            if os.path.getsize(path) % SECTOR:
                raise FormatError('%s is not a 2048-byte-sector ISO (raw 2352-byte BIN/CUE images are not supported; '
                                  'convert to ISO first)' % path)

    def read(self, lba, cb):
        if self.extents is None:
            self.f.seek(lba * SECTOR)
            b = self.f.read(cb)
        else:
            b = None
            for s, n, fn in self.extents:
                if s <= lba and lba * SECTOR + cb <= (s + n) * SECTOR:
                    with open(fn, 'rb') as f:
                        f.seek((lba - s) * SECTOR)
                        b = f.read(cb)
                    break
            if b is None:
                raise FormatError('sectors %d..+%d bytes not present in the sparse extents' % (lba, cb))
        if len(b) != cb:
            raise FormatError('short read at sector %d (%d of %d bytes)' % (lba, len(b), cb))
        return b

    def find_root_file(self, want):
        """(lba, size) of a file in the ISO 9660 root directory whose name matches the regex `want`."""
        pvd = self.read(16, SECTOR)
        if pvd[1:6] != b'CD001':
            raise FormatError('no ISO 9660 volume descriptor at sector 16')
        root = pvd[156:156 + 34]
        lba, size = struct.unpack_from('<I', root, 2)[0], struct.unpack_from('<I', root, 10)[0]
        data = self.read(lba, (size + SECTOR - 1) // SECTOR * SECTOR)
        i = 0
        while i < size:
            n = data[i]
            if n == 0:                     # records do not cross sectors; skip to the next one
                i = (i // SECTOR + 1) * SECTOR
                continue
            name = data[i + 33:i + 33 + data[i + 32]].decode('latin-1')
            if re.match(want, name):
                return struct.unpack_from('<I', data, i + 2)[0], struct.unpack_from('<I', data, i + 10)[0], name
            i += n
        raise FormatError('no file matching %s in the ISO root directory' % want)


class Elf(object):
    def __init__(self, data):
        if data[:4] != b'\x7fELF':
            raise FormatError('executable is not an ELF')
        self.d = data
        phoff, = struct.unpack_from('<I', data, 0x1C)
        phentsize, phnum = struct.unpack_from('<HH', data, 0x2A)
        self.loads = []
        for k in range(phnum):
            p_type, p_off, p_vaddr, _pa, p_filesz = struct.unpack_from('<5I', data, phoff + k * phentsize)
            if p_type == 1:
                self.loads.append((p_vaddr, p_off, p_filesz))

    def read(self, va, cb):
        for v, o, n in self.loads:
            if v <= va and va + cb <= v + n:
                return self.d[o + va - v:o + va - v + cb]
        raise FormatError('vaddr 0x%X not in the executable' % va)

    def cstr(self, va):
        b = self.read(va, 64)
        return b.split(b'\0', 1)[0].decode('latin-1')


# Retail NTSC-U (SCUS-97316) data addresses (docs/formats/disc-records.md section 3)
ELF_NAME = r'^SCUS_973\.16'
VA_BOOT_WORLD_REC = 0x2A38A8     # boot step queue's world record = level 0
VA_WORLD_NAMES = 0x2A39E8        # g_apchzWorld[9]
VA_EOPID = 0x2AC880              # g_aeopid, stride 0x28: otyp +0, grfeopid +4, file word (argument count) +0x18
EOPID_STRIDE, EOPID_COUNT = 0x28, 1449


def filerec(raw):
    """(lba, size, fid) of a 64-byte FILEREC; size 0 = no file."""
    aw = struct.unpack('<16I', raw)
    return (aw[0] ^ aw[13]) + 0xCA0, aw[1] ^ aw[15], aw[2] ^ aw[14]


def lzss(src):
    """Whole-file LZSS (docs/formats/compression-lzss.md), ring zero-filled. A reference names an absolute ring index;
    the byte there is the output byte `distance` back, where distance = ((len(out) - index - 1) mod 0x2000) + 1."""
    out = bytearray()
    i, n = 0, len(src)
    while i < n:
        flags = src[i]
        i += 1
        for _ in range(8):
            if i >= n:
                break
            if flags & 1:
                out.append(src[i])
                i += 1
            else:
                if i + 1 >= n:
                    break
                v = src[i] | (src[i + 1] << 8)
                i += 2
                ln = (v >> 13) + 3
                L = len(out)
                dist = ((L - (v & 0x1FFF) - 1) & 0x1FFF) + 1
                k = L - dist
                if k >= 0 and dist >= ln:
                    out += out[k:k + ln]
                else:
                    for j in range(ln):
                        out.append(out[k + j] if k + j >= 0 else 0)
            flags >>= 1
    return bytes(out)


class Game(object):
    """The disc's executable tables and the level table."""

    def __init__(self, disc):
        self.disc = disc
        lba, size, name = disc.find_root_file(ELF_NAME)
        self.elf_name = name
        self.elf = Elf(disc.read(lba, size))
        self.world_names = []
        for k in range(9):
            self.world_names.append(self.elf.cstr(struct.unpack('<I', self.elf.read(VA_WORLD_NAMES + 4 * k, 4))[0]))
        self.eopids = []
        for k in range(EOPID_COUNT):
            b = self.elf.read(VA_EOPID + k * EOPID_STRIDE, EOPID_STRIDE)
            self.eopids.append((struct.unpack_from('<i', b, 0)[0], struct.unpack_from('<I', b, 4)[0],
                                struct.unpack_from('<i', b, 0x18)[0]))
        lba, size, fid = filerec(self.elf.read(VA_BOOT_WORLD_REC, 64))
        self._cache = {}
        self.levels = parse_head(self.world_data(lba, size), self)['levels']

    def world_data(self, lba, size):
        if (lba, size) not in self._cache:
            self._cache = {(lba, size): lzss(self.disc.read(lba, size))}   # keep one decompressed world
        return self._cache[(lba, size)]

    def level_name(self, lv):
        w = self.world_names[lv['world']] if lv['world'] < len(self.world_names) else 'world%d' % lv['world']
        return 'L%02d_%s%s' % (lv['index'], w, '' if lv['episode'] == 0 else '_ep%d' % lv['episode'])


# ---------------------------------------------------------------------------------------------------------------------
# World file walk (LoadSwFromBrx order; docs/formats/world-file.md, level-objects.md)
# ---------------------------------------------------------------------------------------------------------------------
class Reader(object):
    def __init__(self, data, pos=0):
        self.d, self.pos = data, pos

    def take(self, n):
        if n < 0:
            return b''                     # CBinaryInputStream::Read with a negative count reads nothing
        if self.pos + n > len(self.d):
            raise FormatError('read past end at 0x%X (+%d)' % (self.pos, n))
        b = self.d[self.pos:self.pos + n]
        self.pos += n
        return b

    def u8(self): return self.take(1)[0]
    def s8(self): return struct.unpack('<b', self.take(1))[0]
    def u16(self): return struct.unpack('<H', self.take(2))[0]
    def s16(self): return struct.unpack('<h', self.take(2))[0]
    def u32(self): return struct.unpack('<I', self.take(4))[0]
    def s32(self): return struct.unpack('<i', self.take(4))[0]
    def f32(self): return struct.unpack('<f', self.take(4))[0]
    def skip(self, n): self.take(n)
    def align(self, n): self.pos = (self.pos + n - 1) & -n

    def string16(self):
        n = self.u16()
        return self.take(n).decode('latin-1') if n else None


def _option_value(r, otyp):
    """LoadOptionFromBrx, non-method option: value size by otyp."""
    if otyp == 0: return r.u8()
    if otyp in (5, 6): return r.s32()
    if otyp == 1: return r.f32()
    if otyp == 10: return (r.f32(), r.f32())
    if otyp in (2, 9, 11, 4): return (r.f32(), r.f32(), r.f32())
    if otyp in (3, 12): return tuple(r.f32() for _ in range(4))
    if otyp == 13: return r.u32()
    if otyp == 22: return [r.s16() for _ in range(r.u16())]
    if otyp in (7, 14, 15, 16, 17, 19, 20, 21, 23): return r.s16()
    hi = (otyp & ~0x8000) >> 12
    if hi == 1: return r.u32()
    if hi == 2: return r.s32()
    return None


def _ref_value(r, otyp):
    """RefFromBrx: one method-option argument."""
    if otyp == 0: return r.u8()
    if otyp in (5, 6): return r.s32()
    if otyp == 1: return r.f32()
    if otyp in (7, 14, 15, 16, 17, 19, 20, 21, 23): return r.s16()
    if otyp in (2, 9): return (r.f32(), r.f32(), r.f32())
    if otyp == 10: return (r.f32(), r.f32())
    if ((otyp & ~0x8000) >> 12) == 1: return r.u32()
    return None


def skip_options(r, eopids):
    """LoadOptionsFromBrx: while ((n = S16) >= 0) LoadOptionFromBrx(&g_aeopid[n], grf & 0x2580)."""
    while True:
        n = r.s16()
        if n < 0:
            return
        if n >= len(eopids):
            raise FormatError('option record %d out of range at 0x%X' % (n, r.pos))
        otyp, grf, cref = eopids[n]
        if grf & 0x2580 & 0x401:
            for _ in range(cref):
                t = r.u16()
                if t & 0x8000:
                    for _ in range(r.u8()):
                        _ref_value(r, t & 0x7FFF)
                else:
                    _ref_value(r, t)
        else:
            _option_value(r, otyp)


def parse_head(data, game):
    """The head of a world file up to the first world entry. Returns a dict."""
    r = Reader(data)
    h = {'wes': []}
    for i in range(r.s16()):                                   # world-entry table
        blk = r.take(64)
        x5c, x60, root, x14, ccopy = (r.s32() for _ in range(5))
        name = blk[4:].split(b'\0', 1)[0].decode('latin-1') if blk[:4] == b'FK$X' else 'we%d' % i
        h['wes'].append(dict(index=i, name=name, root_oid=root, copies=ccopy))
    r.skip(64)                                                 # LoadSoundSectionFromBrx
    csnd = r.s16(); r.skip(csnd); r.s16(); r.skip(2 * csnd)
    camb = r.s16()
    if camb > 0:
        r.skip(64)
    r.skip(2 * max(camb, 0))
    clang = r.u8(); r.skip(clang)
    for _ in range(r.s32()):
        r.skip(4)
        for _ in range(clang):
            r.skip(64 + 1 + 4)
        r.skip(8 * r.s32())
    for _ in range(r.s32()):
        r.skip(2 + 64)
    h['symbols'] = {}                                          # LoadSaveSchemaFromBrx: symbol table
    for _ in range(r.u16()):
        k = r.u16()
        h['symbols'][k] = r.string16()
    r.skip(10 * r.u16())
    clevel = r.u32(); r.u32()                                  # LoadLevelTableFromBrx
    h['levels'] = []
    for i in range(clevel):
        lba, size, fid = filerec(r.take(64))
        world, episode, fentry, n = r.u8(), r.u8(), r.u8(), r.u8()
        r.skip(2 * n)
        h['levels'].append(dict(index=i, lba=lba, size=size, fid=fid, world=world, episode=episode))
    clang = r.u8(); r.skip(clang)                              # LoadSwLocalizedStringsFromBrx
    h['strings'] = {}
    for _ in range(r.s16()):
        sid = r.s16()
        h['strings'][sid] = [r.string16() for _ in range(clang)]
    r.skip(20 * r.u16())                                       # LoadSwCellsFromBrx
    skip_options(r, game.eopids)                               # options of g_sw
    skip_options(r, game.eopids)                               # options of g_cm
    r.align(0x10)
    h['body'] = r.pos
    return h


def skip_texture_tables(r, cb_blob):
    """The per-entry texture / CLUT tables after the Splice table (consumed, not interpreted)."""
    for _ in range(r.u16()):
        hd = r.take(0x10)
        cch, c = struct.unpack_from('<b', hd, 5)[0], struct.unpack_from('<H', hd, 8)[0]
        if cch > 0: r.skip(cch * 2)
        if hd[4] != 1: r.skip(c * 4)
    for _ in range(r.u16()):
        hd = r.take(0x18)
        cch, cb = struct.unpack_from('<b', hd, 5)[0], struct.unpack_from('<I', hd, 0x10)[0]
        if cch > 0: r.skip(cch * 2)
        if hd[4] != 1: r.skip(cb)
    for _ in range(r.u16()):
        if r.u16() != 0xFFFF: r.skip(0x20)
    for _ in range(r.u16()):
        hd = r.take(0x18)
        r.s16()
        for _ in range(hd[0x17]):
            z = r.take(8)
            r.skip(2 * z[6] + 2 * z[7])
    r.align(0x10)
    r.skip(cb_blob)
    n = r.u32()
    if n:
        c8, c10 = r.u32(), r.u32()
        r.skip(n * 6 + c8 * 16 + c10 * 4)


def walk(data, game):
    """Returns (head, world entries). Each entry: name, root_oid, copies, fextra, the Splice table's byte range
    [splice_start, splice_end) (including the U32 count), its entries, and its top-level object records."""
    h = parse_head(data, game)
    r = Reader(data, h['body'])
    for we in h['wes']:
        we['start'] = r.pos
        cb_blob, _id, fextra = r.s32(), r.s32(), r.s32()
        we['fextra'] = fextra
        we['splice_start'] = r.pos
        we['entries'] = []
        for i in range(r.u32()):                               # LoadSwSpliceFromBrx
            s = r.pos
            cells = read_list(r, fextra)
            dbg = [r.string16() for _ in range(3)] if fextra else None
            we['entries'].append(dict(index=i, start=s, end=r.pos, cells=cells, debug=dbg))
        we['splice_end'] = r.pos
        skip_texture_tables(r, cb_blob)
        we['objects'] = []                                     # LoadSwObjectsFromBrx (top level only)
        for _ in range(r.s16()):
            k = r.s8()
            if k > 0:
                r.skip(2 * k)
            cid, oid, isplice, cb = r.s16(), r.s32(), r.s16(), r.s32()
            we['objects'].append(dict(cid=cid, oid=oid, isplice=isplice))
            r.skip(cb)
        r.align(0x10)
    h['walk_end'] = r.pos
    return h, h['wes']


# ---------------------------------------------------------------------------------------------------------------------
# The serialized list (DeserializePair, src/game/splice/serialize.cpp)
# ---------------------------------------------------------------------------------------------------------------------
FORMS = {0: 'none', 1: 'set!', 2: 'define', 5: 'assert', 6: 'form6', 7: 'if', 8: 'or', 9: 'and', 10: 'cond',
         11: 'else', 12: 'case', 13: 'let', 14: 'while', 15: 'lambda', 16: 'begin', 17: 'import', 18: 'quote',
         20: 'tag20'}
T_INT, T_FLOAT, T_BOOL, T_SYM, T_BIF, T_VECTOR, T_MATRIX, T_CLQ, T_LM, T_SMP, T_LIST = range(21, 32)
T_BASIC, T_CUT = 33, -1
BOXES = {T_VECTOR: ('vector', 3), T_MATRIX: ('matrix', 16), T_CLQ: ('clq', 3), T_LM: ('lm', 2), T_SMP: ('smp', 3)}
TAG_NAMES = dict(FORMS)
TAG_NAMES.update({T_INT: 'int', T_FLOAT: 'float', T_BOOL: 'bool', T_SYM: 'symbol', T_BIF: 'builtin',
                  T_LIST: 'list', T_BASIC: 'object', T_CUT: 'cut'})
TAG_NAMES.update((t, n) for t, (n, _) in BOXES.items())


def read_list(r, fextra):
    """A list: None for the 0xFF null list, else a python list of cells [tag, value]. Values: int (int, bool, symbol,
    builtin, object), raw 4-byte bytes (float), raw bytes (boxes), a nested list or None (list), None (forms and
    unknown tags: the game's switch reads no payload for them). A 0xFF where a continuation cell is expected leaves the
    game's next pointer null; it is kept as a [T_CUT, None] cell so the bytes re-encode identically."""
    b = r.u8()
    if b == 0xFF:
        return None
    cells = []
    while True:
        if fextra:
            r.skip(4 + 12)                 # debug info per cell (S32 + 6 x S16), dev builds only
        tag = b & 0x7F
        if tag == T_INT:
            v = r.s32()
        elif tag in (T_BOOL, T_SYM, T_BIF, T_BASIC):
            v = r.u32()
        elif tag == T_FLOAT:
            v = r.take(4)
        elif tag in BOXES:
            v = r.take(4 * BOXES[tag][1])
        elif tag == T_LIST:
            v = read_list(r, fextra)
        else:
            v = None
        cells.append([tag, v])
        if not b & 0x80:
            return cells
        b = r.u8()
        if b == 0xFF:
            cells.append([T_CUT, None])
            return cells


def write_list(cells, out):
    """Inverse of read_list (retail layout, no debug info)."""
    if cells is None:
        out.append(0xFF)
        return
    for i, (tag, v) in enumerate(cells):
        if tag == T_CUT:
            out.append(0xFF)
            return
        out.append(tag | (0x80 if i + 1 < len(cells) else 0))
        if tag == T_INT:
            out += struct.pack('<i', v)
        elif tag in (T_BOOL, T_SYM, T_BIF, T_BASIC):
            out += struct.pack('<I', v)
        elif tag == T_FLOAT or tag in BOXES:
            out += v
        elif tag == T_LIST:
            write_list(v, out)


def encode_table(entries):
    """Bytes of one world entry's Splice table (U32 count + entries), retail layout."""
    out = bytearray(struct.pack('<I', len(entries)))
    for cells in entries:
        write_list(cells, out)
    return bytes(out)


# JSON form of the AST: a list is null or an array of cells; a cell is {"tag": N, ...} with one payload key
def cell_to_json(c, names=None):
    tag, v = c
    j = {'tag': tag, 'kind': TAG_NAMES.get(tag, 'unknown')}
    if tag == T_LIST:
        j['list'] = list_to_json(v, names)
    elif tag == T_FLOAT:
        j['float'] = f32_text(v)
        j['bits'] = v[::-1].hex()
    elif tag in BOXES:
        j['floats'] = [f32_text(v[k:k + 4]) for k in range(0, len(v), 4)]
        j['bits'] = [v[k:k + 4][::-1].hex() for k in range(0, len(v), 4)]
    elif tag in (T_INT, T_BOOL, T_BASIC):
        j['value'] = v
    elif tag == T_SYM:
        j['value'] = v
        if names:
            j['name'] = names.sym(v)
    elif tag == T_BIF:
        j['value'] = v
        if names:
            j['name'] = names.bif(v)
    return j


def list_to_json(cells, names=None):
    return None if cells is None else [cell_to_json(c, names) for c in cells]


def list_from_json(j):
    if j is None:
        return None
    out = []
    for c in j:
        tag = c['tag']
        if tag == T_LIST:
            out.append([tag, list_from_json(c['list'])])
        elif tag == T_FLOAT:
            out.append([tag, bytes.fromhex(c['bits'])[::-1]])
        elif tag in BOXES:
            out.append([tag, b''.join(bytes.fromhex(x)[::-1] for x in c['bits'])])
        elif tag in (T_INT, T_BOOL, T_BASIC, T_SYM, T_BIF):
            out.append([tag, c['value']])
        else:
            out.append([tag, None])
    return out


# ---------------------------------------------------------------------------------------------------------------------
# Names (from this repository; everything optional)
# ---------------------------------------------------------------------------------------------------------------------
EVENT_NAMES = {0: 'post-load', 1: 'update', 4: 'label', 5: 'trigger', 6: 'untrigger', 14: 'hit', 17: 'game-event',
               19: 'activate', 20: 'deactivate'}
SELF_SYM = 1002                     # the usual first parameter of handlers; spelled 'self' (our label)
BIF_OPTION = (108, 109, 110, 111)   # addo ensureo seto! geto: (op obj OPTION ...)
BIF_OID = (112, 113, 115, 116)      # find-object(s), find-nearest-object(s): (op OID scope ...)
BIF_OIDLIST = 114                   # (op '(OID ...) scope ...)
BIF_CID = 120                       # find-class-objects: (op CID scope)
BIF_RECEIVER = {117: 1, 118: 152, 119: 135, 137: 154}
# find-world-object -> SW, find-sound-object~ -> CID152 (g_sndmgr: the only class binding StartDialog, option 264),
# find-camera-object -> CM, find-hintbox-object~ -> CID154 (the only class binding PostNotice, option 979)
IDENT = re.compile(r"^[A-Za-z!$%&*/:<=>?^_~+\-][A-Za-z0-9!$%&*/:<=>?^_~+\-.@]*$")
NUMBER = re.compile(r'^[+-]?(\d+\.?\d*|\.\d+)([eE][+-]?\d+)?$')
GENERATED = re.compile(r'^(s\d+|evt\d+|evt-.*|bif\d+)$')


def kebab(cname):
    s = cname[len('Builtin'):] if cname.startswith('Builtin') else cname
    pred = s.startswith('Is') and len(s) > 2 and s[2].isupper()
    if pred:
        s = s[2:]
    s = re.sub(r'(?<=[a-z0-9])(?=[A-Z])', '-', s).lower()
    return s + ('?' if pred else '') + '~'


class Names(object):
    """Spellings of builtins, symbols, classes and options. Builtins: index -> (spelling, C++ name, min, rest)."""

    def __init__(self, symbols=None, raw=False):
        self.raw = raw
        self.bifs, self.problems = self._load_bifs()
        self.symbols = {}
        self.cidname, self.optnames = {}, {}
        if raw:
            return
        reserved = set(FORMS.values()) | set(b[0] for b in self.bifs.values()) | {'self'}
        for k, n in (symbols or {}).items():
            if n and IDENT.match(n) and not NUMBER.match(n) and not GENERATED.match(n) and n not in reserved \
                    and 30 <= k < SELF_SYM:
                self.symbols[k] = n
        self.symbols[SELF_SYM] = 'self'
        for k, n in EVENT_NAMES.items():
            self.symbols[k] = 'evt-' + n
        self._load_options()
        self.symid = dict((n, k) for k, n in self.symbols.items())
        self.bifid = dict((b[0], k) for k, b in self.bifs.items())

    @staticmethod
    def _load_bifs():
        """bif.cpp's table (C++ name, min, rest) plus the spellings of splice-language.md section 3."""
        bifs, problems = {}, []
        try:
            with open(os.path.join(REPO, 'src/game/app/bif.cpp')) as f:
                src = f.read()
            i = src.index('g_mpbifkbif[')
            table = re.findall(r'\{(Builtin\w+), (\d+), (\d+)\}', src[i:src.index('};', i)])
        except (OSError, ValueError):
            return {}, ['src/game/app/bif.cpp builtin table not found: builtins print as #bif:N']
        spell = {}
        try:
            with open(os.path.join(REPO, 'docs/reference/splice-language.md')) as f:
                doc = f.read().split('\n')
            for line in doc:
                m = re.match(r'^\| (\d+) \| `([^`]+)`[^|]*\| (\d+), (\d+) \|.*?`(Builtin\w+)`', line)
                if m:
                    spell[int(m.group(1))] = (m.group(2), m.group(5), int(m.group(3)), int(m.group(4)))
        except OSError:
            problems.append('docs/reference/splice-language.md not found: builtin spellings derived from C++ names')
        for k, (cname, mn, rest) in enumerate(table):
            s = spell.get(k)
            if s and (s[1] != cname or s[2] != int(mn) or s[3] != int(rest)):
                problems.append('builtin %d: splice-language.md says %s %d,%d but bif.cpp has %s %s,%s'
                                % (k, s[1], s[2], s[3], cname, mn, rest))
            name = s[0] if s and s[0] != '?' else kebab(cname)
            bifs[k] = (name, cname, int(mn), int(rest))
        if len(bifs) != 169:
            problems.append('bif.cpp builtin table has %d entries, expected 169' % len(bifs))
        return bifs, problems

    def _load_options(self):
        sys.path.insert(0, os.path.join(REPO, 'tools'))
        try:
            import splice_options as so
            recs, arrays, cidname = so.parse_tables()
            par, structs = so.parse_parents(), so.parse_structs()
        except Exception as e:                      # names are optional
            self.problems.append('option labels unavailable (%s)' % e)
            return
        finally:
            sys.path.pop(0)
        self.cidname = cidname
        root = {}
        for cid in sorted(arrays):
            for _, ri in arrays[cid]:
                root.setdefault(ri, cid)
        label = {}
        for cid in sorted(arrays):
            for opid, ri in arrays[cid]:
                if ri not in label:
                    name, typ, acc = so.describe(recs[ri], cidname[root[ri]], structs, cidname)
                    name = name.split(' / ')[0]
                    m = re.match(r'^RefThunk_?(?:%s)?(\w+)$' % re.escape(cidname[root[ri]]), name, re.I)
                    if m and name.startswith('RefThunk'):
                        name = m.group(1)
                    label[ri] = '%s %s' % (name, acc) if acc.startswith('method') else name
                self.optnames.setdefault(opid, {})[cid] = label[ri]

    def sym(self, k):
        return 's%d' % k if self.raw else self.symbols.get(k, 'evt%d' % k if k < 29 else 's%d' % k)

    def bif(self, k):
        return self.bifs[k][0] if (not self.raw and k in self.bifs) else '#bif:%d' % k

    def cls(self, cid):
        return self.cidname.get(cid, 'cid%d' % cid)

    def opt(self, opid, cid):
        if opid > 0x4D0:
            return 'sidebag'
        by = self.optnames.get(opid)
        if not by:
            return 'no class binds option %d' % opid
        if cid is not None:
            return '%s.%s' % (self.cls(cid), by[cid]) if cid in by else '%s has no option %d' % (self.cls(cid), opid)
        distinct = sorted(set(by.values()))
        if len(distinct) == 1:
            return distinct[0]
        return ' | '.join(distinct[:3]) + (' | ...' if len(distinct) > 3 else '') + ' (by class)'


# ---------------------------------------------------------------------------------------------------------------------
# Pretty printer
# ---------------------------------------------------------------------------------------------------------------------
def f32_text(raw):
    """Shortest decimal that reads back to the same float32 bits, always with '.' or 'e'; #f32:XXXXXXXX otherwise."""
    g = struct.unpack('<f', raw)[0]
    if math.isnan(g) or math.isinf(g):
        return '#f32:%s' % raw[::-1].hex()
    for p in range(1, 10):
        s = repr(float('%.*g' % (p, g)))      # shortest digits, then Python's plain notation (300.0, 0.05, 1e+20)
        if struct.pack('<f', float(s)) == raw:
            break
    if struct.pack('<f', float(s)) != raw:
        return '#f32:%s' % raw[::-1].hex()
    return s


class Atom(object):
    __slots__ = ('s', 'note')

    def __init__(self, s, note=None):
        self.s, self.note = s, note


class Quote(object):
    __slots__ = ('x',)

    def __init__(self, x):
        self.x = x


class Ctx(object):
    def __init__(self, names, oid_cid=None, oid_where=None, strings=None):
        self.n = names
        self.strings = strings or {}
        self.oid_cid = oid_cid or {}
        self.oid_where = oid_where or {}
        self.self_cid = None
        self.selfs = set()
        self.var_cid = {}

    def scan_vars(self, cells):
        """Entry-wide hint: symbols that (define|set!) binds to an expression of one statically known class."""
        self.var_cid = {}
        seen = {}

        def visit(cs):
            if not cs:
                return
            if cs[0][0] in (1, 2) and len(cs) > 2 and cs[1][0] == T_SYM:
                c = receiver_cid(cs[2], self)
                if c is not None:
                    seen.setdefault(cs[1][1], set()).add(c)
            for tag, v in cs:
                if tag == T_LIST:
                    visit(v)
        visit(cells)
        self.var_cid = dict((k, next(iter(v))) for k, v in seen.items() if len(v) == 1)

    def oidnote(self, v):
        if self.n.raw or v not in self.oid_where:
            return None
        return self.oid_where[v]


def receiver_cid(cell, ctx):
    tag, v = cell
    if tag == T_SYM and (v == SELF_SYM or v in ctx.selfs):
        return ctx.self_cid
    if tag == T_SYM:
        return ctx.var_cid.get(v)
    if tag == T_LIST and v and v[0][0] == T_BIF:
        b = v[0][1]
        if b in BIF_RECEIVER:
            return BIF_RECEIVER[b]
        if b in (112, 115) and len(v) > 1 and v[1][0] == T_INT:
            c = ctx.oid_cid.get(v[1][1])
            return None if c is None or ctx.n.cls(c) == 'PROXY' else c    # a proxy stands for the object it instances
    return None


def atom(cell, ctx):
    tag, v = cell
    n = ctx.n
    if tag == T_INT:
        return Atom(str(v))
    if tag == T_FLOAT:
        return Atom(f32_text(v))
    if tag == T_BOOL:
        return Atom('#t' if v == 1 else '#f' if v == 0 else '#bool:%d' % v)
    if tag == T_SYM:
        return Atom(n.sym(v))
    if tag == T_BIF:
        return Atom(n.bif(v))
    if tag == T_BASIC:
        return Atom('#null-obj' if v == 0 else '#obj:%d' % v)
    if tag in BOXES:
        return Atom('#%s(%s)' % (BOXES[tag][0], ' '.join(f32_text(v[k:k + 4]) for k in range(0, len(v), 4))))
    if tag == T_CUT:
        return Atom('#cut')
    if tag == 0:
        return Atom('()')
    if tag in FORMS:
        return Atom(FORMS[tag])
    return Atom('#<unknown tag %d>' % tag)


def to_doc(cells, ctx, hint=None):
    """A list of cells -> a python list of Atom / list / Quote, with annotations on option ids, oids and cids."""
    if cells is None:
        return []
    head = cells[0]
    hb = head[1] if head[0] == T_BIF else None
    # a handler (define evtN (lambda (p0 ...) ...)) or a lambda whose first parameter is self: p0 is the object
    added = None
    if head[0] == 2 and len(cells) > 2 and cells[1][0] == T_SYM and cells[1][1] < 29 and cells[2][0] == T_LIST:
        lam = cells[2][1]
        if lam and lam[0][0] == 15 and len(lam) > 1 and lam[1][0] == T_LIST and lam[1][1] \
                and lam[1][1][0][0] == T_SYM and lam[1][1][0][1] not in ctx.selfs:
            added = lam[1][1][0][1]
            ctx.selfs.add(added)
    # ((geto <hint box> 979) STRING-ID seconds): note the localized string (first language that has it)
    notice = (head[0] == T_LIST and head[1] and len(head[1]) > 2 and head[1][0] == [T_BIF, 111]
              and head[1][2] == [T_INT, 979] and receiver_cid(head[1][1], ctx) == 154)
    out = []
    for i, c in enumerate(cells):
        tag, v = c
        arg = i - 1
        if tag == T_LIST:
            sub = 'oidlist' if (hb == BIF_OIDLIST and arg == 0) else None
            out.append(to_doc(v, ctx, sub) if v is not None else Atom('#nil'))
            continue
        a = atom(c, ctx)
        if tag == T_INT and not ctx.n.raw:
            if hb in BIF_OPTION and arg == 1:
                a.note = ctx.n.opt(v, receiver_cid(cells[1], ctx) if len(cells) > 1 else None)
            elif (hb in BIF_OID and arg == 0) or hint == 'oidlist':
                a.note = ctx.oidnote(v)
            elif hb == BIF_CID and arg == 0:
                a.note = 'class ' + ctx.n.cls(v)
            elif notice and arg == 0 and v in ctx.strings:
                txt = next((x for x in ctx.strings[v] if x), '')
                txt = re.sub(r'\s+', ' ', txt).replace('|#', '|')
                a.note = '"%s"' % (txt[:70] + '...' if len(txt) > 70 else txt)
        out.append(a)
    if added is not None:
        ctx.selfs.discard(added)
    if head[0] == 18 and len(cells) == 2:          # (quote X) -> 'X
        return Quote(out[1])
    return out


def flat(d):
    if isinstance(d, Atom):
        return d.s + (' #|%s|#' % d.note if d.note else '')
    if isinstance(d, Quote):
        return "'" + flat(d.x)
    return '(' + ' '.join(flat(x) for x in d) + ')'


BODY_FORMS = {'define': 2, 'lambda': 2, 'let': 2, 'while': 2, 'if': 2, 'case': 2, 'set!': 2, 'begin': 1, 'cond': 1,
              'and': 1, 'or': 1}


def layout(d, ind, width, lines, prefix=''):
    s = flat(d)
    if isinstance(d, Atom) or (isinstance(d, list) and not d) or len(s) + ind + len(prefix) <= width:
        lines.append(' ' * ind + prefix + s)
        return
    if isinstance(d, Quote):
        layout(d.x, ind, width, lines, prefix + "'")
        return
    head = d[0]
    hs = head.s if isinstance(head, Atom) else None
    nfirst = BODY_FORMS.get(hs, 1) if hs is not None else 0
    if nfirst and len(prefix + '(' + ' '.join(flat(x) for x in d[:nfirst])) + ind > width:
        nfirst = 1
    if not nfirst:                                              # ((datum ...) body...) of case, ((geto ...) args)
        layout(head, ind, width, lines, prefix + '(')
        for x in d[1:]:
            layout(x, ind + 1 + len(prefix), width, lines)
    else:
        lines.append(' ' * ind + prefix + '(' + ' '.join(flat(x) for x in d[:nfirst]))
        for x in d[nfirst:]:
            layout(x, ind + 2, width, lines)
    lines[-1] += ')'


def render_entry(cells, ctx, width):
    if cells is None:
        return ['#nil']
    lines = []
    for c in cells:
        d = to_doc([c], ctx)
        layout(d.x if isinstance(d, Quote) else d[0], 0, width, lines)
    return lines


# ---------------------------------------------------------------------------------------------------------------------
# Statistics and checks
# ---------------------------------------------------------------------------------------------------------------------
class Stats(object):
    def __init__(self):
        self.tags = {}
        self.bifs = {}
        self.syms = set()
        self.cells = 0
        self.lists = 0
        self.arity = []        # (bif, nargs, min, rest) mismatches
        self.unknown = 0

    def visit(self, cells, names):
        if cells is None:
            return
        self.lists += 1
        if cells and cells[0][0] == T_BIF:
            b = cells[0][1]
            nargs = len([c for c in cells[1:] if c[0] != T_CUT])
            info = names.bifs.get(b)
            if info is None or nargs < info[2] or (nargs > info[2] and not info[3]):
                self.arity.append((b, nargs, info[2] if info else None, info[3] if info else None))
        for tag, v in cells:
            self.cells += tag != T_CUT
            self.tags[tag] = self.tags.get(tag, 0) + 1
            if tag == T_BIF:
                self.bifs[v] = self.bifs.get(v, 0) + 1
            elif tag == T_SYM:
                self.syms.add(v)
            elif tag == T_LIST:
                self.visit(v, names)
            elif tag not in FORMS and tag not in (T_INT, T_FLOAT, T_BOOL, T_BASIC, T_CUT) and tag not in BOXES:
                self.unknown += 1


# ---------------------------------------------------------------------------------------------------------------------
# Driver
# ---------------------------------------------------------------------------------------------------------------------
def decode_level(game, lv, raw=False):
    data = game.world_data(lv['lba'], lv['size'])
    head, wes = walk(data, game)
    names = Names(head['symbols'], raw)
    roundtrip = []
    for we in wes:
        orig = data[we['splice_start']:we['splice_end']]
        roundtrip.append(we['fextra'] != 0 or encode_table([e['cells'] for e in we['entries']]) == orig)
    return dict(data=data, head=head, wes=wes, names=names, roundtrip=roundtrip)


def oid_tables(wes, names):
    oid_cid, where = {}, {}
    for we in wes:
        for o in we['objects']:
            oid_cid.setdefault(o['oid'], o['cid'])
            where.setdefault(o['oid'], '%s in %s' % (names.cls(o['cid']), we['name']))
    for we in wes:
        if we['root_oid'] not in where:
            where[we['root_oid']] = 'root of %s' % we['name']
    return oid_cid, where


def render_level(game, lv, dec, width=110):
    names, wes = dec['names'], dec['wes']
    oid_cid, where = oid_tables(wes, names)
    ctx = Ctx(names, oid_cid, where, dec['head']['strings'])
    n_entries = sum(len(we['entries']) for we in wes)
    L = [';;; Splice scripts of level %d (%s), world file fid 0x%X, LBA %d, %d bytes compressed, %d decompressed'
         % (lv['index'], game.level_name(lv), lv['fid'], lv['lba'], lv['size'], len(dec['data'])),
         ';;; %d world entries, %d script entries in %d entries with scripts; round trip %s'
         % (len(wes), n_entries, sum(1 for we in wes if we['entries']),
            'byte-identical' if all(dec['roundtrip']) else 'FAILED for %d entries' % dec['roundtrip'].count(False)),
         ';;; Generated by tools/splice/dump.py from your disc. Game content: do not commit or redistribute.',
         ';;; Legend (docs/reference/splice-dump.md): evt-NAME / evtN = event symbol N (handler: (define evt-NAME',
         ';;;   (lambda (self ...) ...))); self = symbol 1002; sN = symbol N without a name on disc; other words =',
         ';;;   save-schema symbol names; name~ = builtin label not from the original; #|...|# = annotation (option',
         ';;;   label, object of an oid, class of a cid); () = the None value (tag 0), #nil = a null nested list.']
    for p in names.problems:
        L.append(';;; note: ' + p)
    for we in wes:
        if not we['entries']:
            continue
        users = {}
        for o in we['objects']:
            if o['isplice'] >= 0:
                users.setdefault(o['isplice'], []).append(o)
        L.append('')
        L.append(';;; ==== world entry %d %s  root oid %d  copies %d  script table 0x%X..0x%X (%d entries)'
                 % (we['index'], we['name'], we['root_oid'], we['copies'], we['splice_start'], we['splice_end'],
                    len(we['entries'])))
        for e in we['entries']:
            us = users.get(e['index'], [])
            cids = set(o['cid'] for o in us)
            ctx.self_cid = cids.pop() if len(cids) == 1 else None
            ctx.scan_vars(e['cells'])
            who = ', '.join('%s oid %d' % (names.cls(o['cid']), o['oid']) for o in us[:12])
            if len(us) > 12:
                who += ', ... (%d objects)' % len(us)
            L.append('')
            L.append(';; -- entry %d of %s [0x%X, %d bytes]  bound to: %s'
                     % (e['index'], we['name'], e['start'], e['end'] - e['start'],
                        who or 'no top-level object (nested objects or unused)'))
            L.extend(render_entry(e['cells'], ctx, width))
    return '\n'.join(L) + '\n'


def level_json(game, lv, dec):
    names = dec['names']
    return dict(level=lv['index'], name=game.level_name(lv), fid=lv['fid'], lba=lv['lba'], size=lv['size'],
                format='sly2-splice-ast/1',
                world_entries=[dict(index=we['index'], name=we['name'], root_oid=we['root_oid'], copies=we['copies'],
                                    splice_offset=we['splice_start'],
                                    objects=[dict(cid=o['cid'], cls=names.cls(o['cid']), oid=o['oid'],
                                                  entry=o['isplice']) for o in we['objects'] if o['isplice'] >= 0],
                                    entries=[list_to_json(e['cells'], names) for e in we['entries']])
                               for we in dec['wes']])


def stats_report(game, lv, dec, out=sys.stderr):
    st = Stats()
    names, wes = dec['names'], dec['wes']
    for we in wes:
        for e in we['entries']:
            st.visit(e['cells'], names)
    nent = sum(len(we['entries']) for we in wes)
    w = out.write
    w('level %d %s: %d world entries, %d script entries (= frames LoadSpliceEntryFromBrx creates), %d cells (= CPair '
      'allocations by DeserializePair), %d lists, %d distinct symbols, walk end 0x%X of 0x%X\n'
      % (lv['index'], game.level_name(lv), len(wes), nent, st.cells, st.lists, len(st.syms), dec['head']['walk_end'],
         len(dec['data'])))
    w('  tags: %s\n' % ', '.join('%s %d' % (TAG_NAMES.get(t, 'tag%d' % t), n) for t, n in sorted(st.tags.items())))
    w('  unknown tags: %d\n' % st.unknown)
    known = sum(1 for b in st.bifs if b in names.bifs)
    w('  builtins: %d distinct used, %d resolve to a bif.cpp table entry; %d calls; not in table: %s\n'
      % (len(st.bifs), known, sum(st.bifs.values()), sorted(b for b in st.bifs if b not in names.bifs) or 'none'))
    bad = {}
    for b, n, mn, rest in st.arity:
        bad.setdefault((b, n, mn, rest), 0)
        bad[(b, n, mn, rest)] += 1
    w('  calls whose operand count disagrees with bif.cpp (min, rest): %d%s\n'
      % (len(st.arity), ''.join('\n    %s (#%d) with %d operands, table says min %s rest %s: %d calls'
                                % (names.bif(b), b, n, mn, rest, c) for (b, n, mn, rest), c in sorted(bad.items()))))
    named = sum(1 for s in st.syms if s in names.symbols)
    w('  symbols: %d distinct, %d named (save schema / events / self), %d events used\n'
      % (len(st.syms), named, sum(1 for s in st.syms if s < 29)))
    live = [we for we in wes if we['entries']]
    w('  loader allocations: %d SPLCS tables + %d bytes of entry arrays on the world heap, %d frames (entries with '
      'a non-null list), %d CPairs; symbols are plain ids (nothing allocated)\n'
      % (len(live), sum((8 * len(we['entries']) + 15) & ~15 for we in live),
         sum(1 for we in wes for e in we['entries'] if e['cells'] is not None), st.cells))
    w('  round trip (decode -> encode -> compare each world entry\'s table): %d/%d world entries byte-identical, '
      '%d bytes\n' % (dec['roundtrip'].count(True), len(wes),
                      sum(we['splice_end'] - we['splice_start'] for we in wes)))
    for p in names.problems:
        w('  note: %s\n' % p)
    return st


def check_eopids(game, out=sys.stderr):
    """Compare the option records read from the disc's executable with src/game/app/class_registry_options.inc."""
    sys.path.insert(0, os.path.join(REPO, 'tools'))
    try:
        import splice_options as so
        recs = so.parse_tables()[0]
    except Exception as e:
        out.write('option records: repository table unavailable (%s)\n' % e)
        return
    finally:
        sys.path.pop(0)
    bad = [k for k, (otyp, grf, cref) in enumerate(game.eopids)
           if k not in recs or recs[k]['otyp'] != otyp or recs[k]['grf'] != grf
           or (recs[k]['file'][0] == 'int' and recs[k]['file'][1] != cref)]
    out.write('option records: %d read from %s, %d differ from class_registry_options.inc%s\n'
              % (len(game.eopids), game.elf_name, len(bad), (': %s' % bad[:10]) if bad else ''))


def parse_levels(specs, n):
    out = []
    for s in specs:
        if s == 'all':
            out.extend(range(n))
        elif '-' in s:
            a, b = s.split('-', 1)
            out.extend(range(int(a), int(b) + 1))
        else:
            out.append(int(s))
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0],
                                 formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
    ap.add_argument('disc')
    ap.add_argument('levels', nargs='*')
    ap.add_argument('-o', '--out')
    ap.add_argument('--json', action='store_true')
    ap.add_argument('--raw', action='store_true')
    ap.add_argument('--width', type=int, default=110)
    ap.add_argument('--stats', action='store_true')
    ap.add_argument('--list', action='store_true')
    a = ap.parse_args(argv)
    try:
        game = Game(Disc(a.disc))
    except (FormatError, OSError) as e:
        sys.exit('dump.py: %s' % e)
    if a.list or not a.levels:
        for lv in game.levels:
            print('%2d  %-24s world %d episode %2d  fid 0x%02X  LBA %7d  %9d bytes%s'
                  % (lv['index'], game.level_name(lv), lv['world'], lv['episode'], lv['fid'], lv['lba'], lv['size'],
                     '' if lv['size'] else '  (empty record)'))
        return 0
    levels = parse_levels(a.levels, len(game.levels))
    if a.stats:
        check_eopids(game)
    if not a.out and len(levels) != 1:
        sys.exit('dump.py: give -o DIR for more than one level')
    if a.out:
        if os.path.abspath(a.out).startswith(REPO + os.sep):
            sys.stderr.write('warning: %s is inside the repository; the output is game content, do not commit it\n'
                             % a.out)
        os.makedirs(a.out, exist_ok=True)
    rc = 0
    for k in levels:
        lv = game.levels[k]
        if not lv['size']:
            sys.stderr.write('level %d: empty record, skipped\n' % k)
            continue
        try:
            dec = decode_level(game, lv, a.raw)
        except (FormatError, OSError) as e:
            sys.stderr.write('level %d: %s\n' % (k, e))
            rc = 1
            continue
        if a.stats:
            stats_report(game, lv, dec)
        if a.out:
            base = os.path.join(a.out, game.level_name(lv))
            with open(base + '.scm', 'w') as f:
                f.write(render_level(game, lv, dec, a.width))
            if a.json:
                with open(base + '.json', 'w') as f:
                    json.dump(level_json(game, lv, dec), f, separators=(',', ':'))
            sys.stderr.write('wrote %s.scm%s\n' % (base, ' and .json' if a.json else ''))
        elif a.json:
            json.dump(level_json(game, lv, dec), sys.stdout, indent=1)
        else:
            sys.stdout.write(render_level(game, lv, dec, a.width))
        if not all(dec['roundtrip']):
            rc = 1
    return rc


if __name__ == '__main__':
    sys.exit(main())
