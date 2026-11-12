#!/usr/bin/env python3
"""Virtual-slot signature check: do all overrides of one vtable slot take the same arguments?

A virtual call passes the slot's full argument list. On the EE an override declared with fewer parameters still
works when it only forwards: the arguments it does not name stay in their registers (a2..t1, f12..) for its tail
call. Natively they are dropped (STEPPLAYER's slot 82, AddStepplayerAxisXps, declared one qword of three and tail-called
the STEP base: the port flung Sly off the intro pipe). This tool groups the functions that fill each slot and lists the
slots whose native-visible declarations disagree on the argument count, or put an integer where another override has a
pointer.

  vslotcheck.py [--root REPO] [--elf ELF] [--out DIR]

Slots come from the game's vtables themselves: every `_vt$<n><CLASS>` in config/symbol_addrs.txt is read from the
ELF (default disc/SCUS_973.16; g++ 2.95 entries are {short delta, short index, pfn}, 8 bytes, entry 0 the unused
typeinfo slot), and each pfn is named through the config's function symbols. Entry i of a class's vtable belongs to
the topmost ancestor (struct X : Y in include/, primary bases) whose vtable has an entry i: a family is every function
in that slot across the ancestor's descendants, i.e. every override of one virtual. Declarations: sretcheck.py's native_groups (asm label, C name, canonical mangled name; EE-only
branches and file-local statics skipped; address-only `void f()` declarations ignored). Functions found in more than
one family (__pure_virtual, shared stubs) are left out. Counts include the object pointer.

Outputs (DIR, default build/signatures/): vslot.csv (one row per declaration in a listed family) and vslot.md. A
listed family is a lead, not a verdict: an override that takes fewer arguments and never reads or forwards the rest
(a stub, a default that returns 0) is harmless. Exit 0 (a report).
"""
import argparse
import collections
import csv
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sigcheck as S   # noqa: E402
import sretcheck as R  # noqa: E402


def read_elf(path):
    """(bytes, [(vaddr, offset, filesz)]) of the PT_LOAD segments of a 32-bit little-endian ELF"""
    b = open(path, 'rb').read()
    phoff, = struct.unpack_from('<I', b, 0x1C)
    phentsize, phnum = struct.unpack_from('<HH', b, 0x2A)
    segs = []
    for i in range(phnum):
        t, off, va, _, fsz = struct.unpack_from('<IIIII', b, phoff + i * phentsize)
        if t == 1: segs.append((va, off, fsz))
    return b, segs


def word(elf, addr):
    b, segs = elf
    for va, off, fsz in segs:
        if va <= addr < va + fsz:
            return struct.unpack_from('<I', b, off + addr - va)[0]
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0], epilog=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--root', default=os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(
        os.path.abspath(__file__))))))
    ap.add_argument('--elf')
    ap.add_argument('--out')
    a = ap.parse_args()
    root = os.path.abspath(a.root)
    out = os.path.abspath(a.out or os.path.join(root, 'build', 'signatures'))
    os.makedirs(out, exist_ok=True)
    elf = read_elf(a.elf or os.path.join(root, 'disc', 'SCUS_973.16'))

    vts, funcs = {}, {}
    for line in open(os.path.join(root, 'config/symbol_addrs.txt')):
        m = re.match(r'\s*(\S+)\s*=\s*(0x[0-9A-Fa-f]+)\s*;(.*)', line)
        if not m: continue
        name, addr, rest = m.group(1), int(m.group(2), 16), m.group(3)
        if name.startswith('_vt$'):
            sz = re.search(r'size:(0x[0-9A-Fa-f]+)', rest)
            if sz: vts[name] = (addr, int(sz.group(1), 16))
        elif 'type:func' in rest:
            funcs.setdefault(addr, name)

    # primary bases from the headers (struct X : Y); a slot belongs to the topmost ancestor whose vtable reaches it
    base = {}
    hdr = os.path.join(root, 'include')
    for dp, _, fs in os.walk(hdr):
        for fn in fs:
            if not fn.endswith('.h'): continue
            txt, _ = S.strip_comments_pp(open(os.path.join(dp, fn), errors='replace').read())
            for c, b_ in re.findall(r'\b(?:struct|class)\s+(\w+)\s*:\s*(?:public\s+|virtual\s+)*(\w+)', txt):
                base.setdefault(c, b_)
    vlen = {}
    for vt, (addr, size) in vts.items():
        m = re.match(r'_vt\$(\d+)(\w+)$', vt)
        if m and len(m.group(2)) == int(m.group(1)):   # entries up to the last named function (the size has padding)
            vlen[m.group(2)] = max([i + 1 for i in range(1, size // 8) if word(elf, addr + 8 * i + 4) in funcs] or [0])

    fam = collections.defaultdict(set)
    for vt, (addr, size) in sorted(vts.items()):
        m = re.match(r'_vt\$(\d+)(\w+)$', vt)
        if not m or len(m.group(2)) != int(m.group(1)): continue    # secondary-base vtables
        cls = m.group(2)
        chain, c = [], cls
        while c and c not in chain:
            chain.append(c); c = base.get(c)
        for i in range(1, size // 8):
            pfn = word(elf, addr + 8 * i + 4)
            if not pfn or pfn not in funcs: continue
            top = cls
            for c in chain:
                if vlen.get(c, 0) > i: top = c
            fam[(top, i)].add((i, funcs[pfn], vt))

    # a function in more than one family (__pure_virtual, shared empty stubs) does not belong to one signature
    nfam = collections.Counter(sym for m in fam.values() for sym in {s_ for _, s_, _ in m})
    L = S.load(root)
    groups = R.native_groups(L)
    rows, listed = [], []
    for root_key, members in fam.items():
        idx = root_key[1]
        syms = sorted({s for _, s, _ in members if nfam[s] == 1})
        if len(syms) < 2: continue
        info = {}
        for s in syms:
            # address-only declarations (`void f()`, the tables' form) say nothing about the arguments
            ds = [dl for dl in groups.get(s, []) if not dl.unparsed and (dl.params or dl.member or dl.variadic
                                                                         or (dl.ret or 'void') != 'void')]
            if ds: info[s] = ds
        counts = {(len(dl.flat()), dl.variadic) for ds in info.values() for dl in ds}
        nfix = {n for n, v in counts if not v}
        mismatch = len(nfix) > 1
        # int vs pointer at the same position across overrides (positions after the object pointer)
        ivp = set()
        bypos = collections.defaultdict(set)
        for ds in info.values():
            for dl in ds:
                for j, t in enumerate(dl.flat()[1:], 1):
                    bypos[j].add('ptr' if S.is_ptr(t) else ('int' if t in S.INTS else 'other'))
        for j, ks in bypos.items():
            if 'ptr' in ks and 'int' in ks: ivp.add(j)
        if not mismatch and not ivp: continue
        classes = sorted({vt[len('_vt$'):].lstrip('0123456789') for _, _, vt in members})
        listed.append(dict(slot=idx, top=root_key[0], classes=classes, syms=syms, counts=sorted(nfix), ivp=sorted(ivp), info=info))
        for s, ds in info.items():
            for dl in ds:
                rows.append(dict(slot=idx, symbol=s, nargs=len(dl.flat()), is_def=int(dl.is_def), file=dl.file,
                                 line=dl.line, decl=dl.sig()))
    listed.sort(key=lambda x: (x['slot'], x['syms']))
    with open(os.path.join(out, 'vslot.csv'), 'w', newline='') as f:
        w = csv.DictWriter(f, ['slot', 'symbol', 'nargs', 'is_def', 'file', 'line', 'decl'], lineterminator='\n')
        w.writeheader(); w.writerows(rows)
    md = ['# Virtual-slot signature check', '',
          'Generated by tools/cleanup/signatures/vslotcheck.py: vtable slots whose overrides (from the ELF\'s vtables) '
          'have native-visible declarations with different argument counts (object pointer included), or an integer '
          'where another override has a pointer.', '',
          f'Families with two or more functions: {sum(1 for m in fam.values() if len({s for _, s, _ in m}) > 1)}; '
          f'listed: {len(listed)} ({sum(1 for x in listed if len(x["counts"]) > 1)} by count, '
          f'{sum(1 for x in listed if x["ivp"])} int-vs-pointer).', '']
    for x in listed:
        md.append(f"## slot {x['slot']} ({x['top']}): counts {x['counts']}" + (f", int/pointer at {x['ivp']}" if x['ivp'] else ''))
        md.append(f"classes: {', '.join(x['classes'][:12])}{' ...' if len(x['classes']) > 12 else ''}")
        md.append('')
        for s in x['syms']:
            for dl in x['info'].get(s, []):
                md.append(f"- `{s}` ({len(dl.flat())}{'+...' if dl.variadic else ''}) {dl.file}:{dl.line} "
                          f"`{dl.sig()}`{' (def)' if dl.is_def else ''}")
            if s not in x['info']:
                md.append(f'- `{s}`: no native-visible declaration parsed')
        md.append('')
    open(os.path.join(out, 'vslot.md'), 'w').write('\n'.join(md) + '\n')
    print(f'{len(listed)} slot families listed ({sum(1 for x in listed if len(x["counts"]) > 1)} by argument count, '
          f'{sum(1 for x in listed if x["ivp"])} int-vs-pointer)')
    print(f'outputs: {out}/vslot.csv, vslot.md')
    return 0


if __name__ == '__main__':
    sys.exit(main())
