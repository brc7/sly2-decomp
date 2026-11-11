"""Byte-level comparison of two MIPS ELF32 relocatable objects (the round harness's object check).

Stronger than objcmp.py's views and matchtool's masked compare: every allocated section is compared byte for byte,
relocations by (section, offset, type, target symbol name), and named symbols by (value, size, type, binding,
section). .mdebug and .comment (paths, compiler banner) are ignored.

Verdicts:
  IDENTICAL   nothing differs
  RENAMED     bytes and relocation positions identical; only symbol names differ, through a consistent one-to-one
              map whose new names are not already known under another address (see classify_renames). A real
              rename (rename.py) or a renumbered local label ($LC12 -> $LC13) looks like this.
  DIFF        anything else; first_diff() describes the first difference (section, function, a few instructions)
A relocation that now names another existing symbol (a call resolving to another overload or virtual) is a DIFF
even though matchtool still reports MATCH: that is the case this module exists to catch.
"""
import re
import struct

SHT_PROGBITS, SHT_SYMTAB, SHT_NOBITS, SHT_REL = 1, 2, 8, 9
SHF_ALLOC, SHF_EXEC = 2, 4
IGNORED = ('.mdebug', '.comment', '.pdr', '.gnu.attributes')
RTYPE = {2: 'R_MIPS_32', 4: 'R_MIPS_26', 5: 'R_MIPS_HI16', 6: 'R_MIPS_LO16', 7: 'R_MIPS_GPREL16'}


def norm(name):
    """objcmp's normalisation: the hash suffix g++ appends to _GLOBAL_$I$/$D$ names depends on the build path."""
    return re.sub(r'(_GLOBAL_\$[ID]\$\S*?\.c(?:pp)?)\w{6}', r'\1', name)


class Obj:
    def __init__(self, path):
        d = open(path, 'rb').read()
        self.path = path
        shoff, = struct.unpack_from('<I', d, 0x20)
        shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2E)
        hdr = [struct.unpack_from('<10I', d, shoff + i * shentsize) for i in range(shnum)]
        so = hdr[shstrndx][4]
        cstr = lambda base, o: d[base + o:d.index(b'\0', base + o)].decode('latin-1')
        self.secs = []
        for h in hdr:
            name, typ, flags, _addr, off, size, link, info = h[:8]
            self.secs.append(dict(name=cstr(so, name), type=typ, flags=flags, size=size, link=link, info=info,
                                  data=d[off:off + size] if typ != SHT_NOBITS else None))
        self.syms = []
        st = next((s for i, s in enumerate(hdr) if s[1] == SHT_SYMTAB), None)
        if st:
            stro = hdr[st[6]][4]
            for k in range(st[5] // 16):
                n, val, size, info, other, shndx = struct.unpack_from('<IIIBBH', d, st[4] + k * 16)
                self.syms.append(dict(name=cstr(stro, n), value=val, size=size, type=info & 0xF, bind=info >> 4,
                                      shndx=shndx))
        self.rels = {}   # target section index -> [(offset, type, symbol name)]
        for s in self.secs:
            if s['type'] == SHT_REL:
                lst = self.rels.setdefault(s['info'], [])
                for i in range(0, len(s['data']), 8):
                    off, inf = struct.unpack_from('<II', s['data'], i)
                    lst.append((off, inf & 0xFF, self.symname(inf >> 8)))

    def symname(self, k):
        if k >= len(self.syms): return '?%d' % k
        s = self.syms[k]
        if s['name']: return norm(s['name'])
        return self.secname(s['shndx'])          # section symbol

    def secname(self, shndx):
        return {0: 'UND', 0xFFF1: 'ABS', 0xFFF2: 'COMMON'}.get(shndx) or (
            self.secs[shndx]['name'] if shndx < len(self.secs) else str(shndx))

    def alloc(self):
        """{name: section index} of allocated sections (duplicates get #n suffixes)"""
        out = {}
        for i, s in enumerate(self.secs):
            if s['flags'] & SHF_ALLOC and not s['name'].startswith(IGNORED):
                k = norm(s['name'])
                while k in out: k += '#'
                out[k] = i
        return out

    def named(self):
        """{name: (value, size, type, bind, section)} of named symbols (file symbols dropped)"""
        out = {}
        for s in self.syms:
            if s['name'] and s['type'] != 4:
                out[norm(s['name'])] = (s['value'], s['size'], s['type'], s['bind'], self.secname(s['shndx']))
        return out

    def defined_globals(self):
        return {norm(s['name']) for s in self.syms if s['name'] and s['bind'] in (1, 2) and s['shndx'] != 0}

    def func_at(self, shndx, off):
        best = None
        for s in self.syms:
            if s['shndx'] == shndx and s['name'] and s['type'] in (1, 2) and s['value'] <= off < s['value'] + max(s['size'], 1):
                if best is None or s['type'] == 2: best = s
        return best


class Result:
    def __init__(self, verdict, renames=(), diffs=(), first=None):
        self.verdict, self.renames, self.diffs, self.first = verdict, list(renames), list(diffs), first

    def __repr__(self):
        return f'Result({self.verdict}, renames={self.renames[:3]}, diffs={self.diffs[:3]})'


def compare(old, new, known_old=None, known_new=None, defined_elsewhere=()):
    """old, new: Obj. known_old/known_new: {name: address} of the config files (symbol_addrs) before and after, used
    to tell a rename from a retarget; defined_elsewhere: names defined by baseline objects."""
    diffs = []
    ao, an = old.alloc(), new.alloc()
    first = None
    for k in sorted(set(ao) | set(an), key=lambda k: (k not in ao, ao.get(k, an.get(k, 0)))):
        if k not in ao or k not in an:
            diffs.append(('section', k, 'only in ' + ('new' if k in an else 'old')))
            first = first or ('section', k, None)
            continue
        so, sn = old.secs[ao[k]], new.secs[an[k]]
        if so['type'] != sn['type'] or so['size'] != sn['size'] or so['data'] != sn['data']:
            off = None
            if so['data'] is not None and sn['data'] is not None:
                off = next((i for i in range(min(len(so['data']), len(sn['data'])))
                            if so['data'][i] != sn['data'][i]), min(len(so['data']), len(sn['data'])))
            diffs.append(('bytes', k, f'size {so["size"]:#x} -> {sn["size"]:#x}' + (f', first at +{off:#x}' if off is not None else '')))
            first = first or ('bytes', k, off)
    # relocations: positions and types must agree; names may differ only through a rename map
    pairs = {}
    for k in set(ao) & set(an):
        ro, rn = sorted(old.rels.get(ao[k], [])), sorted(new.rels.get(an[k], []))
        if [r[:2] for r in ro] != [r[:2] for r in rn]:
            off = next((a[0] for a, b in zip(ro, rn) if a[:2] != b[:2]), (ro or rn)[min(len(ro), len(rn)) - 1][0] if (ro or rn) else 0)
            diffs.append(('relocs', k, f'{len(ro)} -> {len(rn)} relocations, first change at +{off:#x}'))
            first = first or ('relocs', k, off)
            continue
        for a, b in zip(ro, rn):
            if a[2] != b[2]:
                pairs.setdefault(a[2], {}).setdefault(b[2], []).append((k, a[0]))
    # symbols
    no, nn = old.named(), new.named()
    gone, added = set(no) - set(nn), set(nn) - set(no)
    for n in set(no) & set(nn):
        if no[n] != nn[n]:
            diffs.append(('symbol', n, f'{fmt_sym(no[n])} -> {fmt_sym(nn[n])}'))
            first = first or ('symbol', n, None)
    # pair removed with added symbols that have the same attributes
    by_attr = {}
    for n in added: by_attr.setdefault(nn[n], []).append(n)
    for n in sorted(gone):
        cand = by_attr.get(no[n], [])
        if len(cand) == 1:
            pairs.setdefault(n, {}).setdefault(cand[0], []).append(('symtab', 0))
            added.discard(cand[0]); by_attr[no[n]] = []
        else:
            diffs.append(('symbol', n, 'only in old'))
            first = first or ('symbol', n, None)
    for n in sorted(added):
        diffs.append(('symbol', n, 'only in new'))
        first = first or ('symbol', n, None)
    renames, bad = classify_renames(pairs, no, known_old or {}, known_new or {}, set(defined_elsewhere))
    rt_first = None
    for a, b, where, why in bad:
        diffs.insert(0, ('retarget', where[0] if where[0] != 'symtab' else a,
                         f'{a} -> {b} ({why})' + (f' at {where[0]}+{where[1]:#x}' if where[0] != 'symtab' else '')))
        rt_first = rt_first or ('retarget', where[0], where[1] if where[0] != 'symtab' else None)
    if rt_first and (not first or first[0] == 'symbol'):
        first = rt_first     # a relocation that names another symbol explains the symbol-table differences
    if diffs:
        return Result('DIFF', renames, diffs, first)
    return Result('RENAMED' if renames else 'IDENTICAL', renames)


def classify_renames(pairs, old_names, known_old, known_new, defined_elsewhere):
    """pairs: {old name: {new name: [where...]}}. A pair is a rename when the map is one-to-one, and the new name is
    the old one's config entry moved (same address in the new config), or the new name is unknown (not in the old
    config, not already in the old object, not defined by another baseline object)."""
    renames, bad, used = [], [], {}
    for a, m in sorted(pairs.items()):
        for b, where in m.items():
            used.setdefault(b, set()).add(a)
    for a, m in sorted(pairs.items()):
        for b, where in m.items():
            w = where[0]
            if len(m) > 1 or len(used[b]) > 1:
                bad.append((a, b, w, 'not one-to-one')); continue
            if a in known_old and known_new.get(b) == known_old[a]:
                renames.append((a, b)); continue
            if b in known_old:
                bad.append((a, b, w, f'{b} already in the config at {known_old[b]:#x}')); continue
            if b in old_names:
                bad.append((a, b, w, f'{b} already in the old object')); continue
            if b in defined_elsewhere:
                bad.append((a, b, w, f'{b} defined by another object')); continue
            renames.append((a, b))
    return renames, bad


def fmt_sym(t):
    v, s, ty, b, sec = t
    return f'{sec}+{v:#x} size {s:#x} type {ty} bind {b}'


def first_diff(old, new, res, disasm=None, n=6):
    """A few lines describing res.first: the function and instructions around the first differing word."""
    if not res.first:
        return []
    kind, sec, off = res.first
    lines = []
    head = next((d for d in res.diffs if d[1] == sec or (kind == 'symbol' and d[1] == sec)), res.diffs[0])
    if off is None or kind == 'symbol':
        return [f'{head[0]} {head[1]}: {head[2]}'] + [f'  also {d[0]} {d[1]}: {d[2]}' for d in res.diffs[1:3]]
    ao, an = old.alloc(), new.alloc()
    io, inn = ao.get(sec), an.get(sec)
    fn = new.func_at(inn, off) if inn is not None else None
    fo = old.func_at(io, off) if io is not None else None
    where = f'{fn["name"]}+{off - fn["value"]:#x}' if fn else f'+{off:#x}'
    lines.append(f'{kind} {sec} at {where}: {head[2]}')
    so, sn = (old.secs[io] if io is not None else None), (new.secs[inn] if inn is not None else None)
    if so and sn and so['flags'] & SHF_EXEC and disasm:
        relo = {r[0]: r for r in old.rels.get(io, [])}
        reln = {r[0]: r for r in new.rels.get(inn, [])}
        w0 = max(0, (off // 4) - 1)
        for w in range(w0, w0 + n):
            a = so['data'][4 * w:4 * w + 4] if 4 * w + 4 <= len(so['data']) else None
            b = sn['data'][4 * w:4 * w + 4] if 4 * w + 4 <= len(sn['data']) else None
            if a is None and b is None: break
            da = _ins(a, w, relo, disasm, fo); db = _ins(b, w, reln, disasm, fn)
            lines.append(f'  {"!!" if da != db else "  "} +{4 * w:05x} {da:38.38s} | {db}')
    elif so and sn and so['data'] is not None and sn['data'] is not None:
        lo = off & ~3
        lines.append(f'  old {so["data"][lo:lo + 16].hex(" ", 4)}')
        lines.append(f'  new {sn["data"][lo:lo + 16].hex(" ", 4)}')
    return lines


def _ins(b, w, rels, disasm, fn):
    if b is None: return '-'
    word, = struct.unpack('<I', b)
    s = disasm(word, 4 * w)
    r = rels.get(4 * w)
    if r: s += f'  ; {RTYPE.get(r[1], r[1])} {r[2]}'
    return ' '.join(s.split())
