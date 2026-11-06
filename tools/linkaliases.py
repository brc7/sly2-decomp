#!/usr/bin/env python3
"""Emit linker-script aliases for symbols the matched objects reference but no object or config entry defines.

usage: python3 tools/linkaliases.py [--redefine DIR] [obj.o ...] > aliases.ld

Two sources of aliases:
  1. config/link_aliases.txt, a table of `symbol = target;` lines (`[source] symbol = target;`: one object only, written
     with --redefine DIR for objcopy). Empty and deleted since data/vtables-2 (every name is bound in source or
     config); read if it exists.
  2. Address names: every undefined D_XXXXXXXX / func_XXXXXXXX (also as an alias target) that no given object
     defines becomes `name = <config symbol at that address>;` when config/symbol_addrs.txt or
     config/functions.csv names the address, else `name = 0xXXXXXXXX;`. The absolute form is only valid in a link
     that keeps the original data image at its original address (unbuilt data spliced from the ELF).

With objects, only aliases for symbols that are undefined in them (and not defined by any of them) are printed;
without objects, the whole table is printed and no address names are derived.
"""
import os, re, struct, sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
ADDR_NAME = re.compile(r'(?:D|func)_([0-9A-F]{8})$')


def elf_symbols(path):
    """(undefined, defined) global symbol names of an ELF32 little-endian relocatable object"""
    d = open(path, 'rb').read()
    shoff, = struct.unpack_from('<I', d, 0x20)
    shentsize, shnum = struct.unpack_from('<HH', d, 0x2E)
    secs = [struct.unpack_from('<10I', d, shoff + i * shentsize) for i in range(shnum)]
    und, dfn = set(), set()
    for s in secs:
        if s[1] != 2:                       # SHT_SYMTAB
            continue
        strtab = secs[s[6]][4]
        for i in range(1, s[5] // 16):
            name, value, size, info, other, shndx = struct.unpack_from('<IIIBBH', d, s[4] + i * 16)
            if info >> 4 == 0:              # STB_LOCAL
                continue
            nm = d[strtab + name:d.index(b'\0', strtab + name)].decode()
            (und if shndx == 0 else dfn).add(nm)
    return und, dfn


def config_names():
    """address -> preferred config name (symbol_addrs.txt first, then functions.csv)"""
    names = {}
    for line in open(os.path.join(ROOT, 'config', 'symbol_addrs.txt')):
        m = re.match(r'\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+);', line)
        if m and not ADDR_NAME.match(m.group(1)):
            names.setdefault(int(m.group(2), 16), m.group(1))
    import csv
    for r in csv.DictReader(open(os.path.join(ROOT, 'config', 'functions.csv'), newline='')):
        if not ADDR_NAME.match(r['Name']):
            names.setdefault(int(r['Start'], 16), r['Name'])
    return names


def read_table():
    glob, scoped = {}, {}
    path = os.path.join(ROOT, 'config', 'link_aliases.txt')
    if not os.path.exists(path):                # the table is gone (data/vtables-2): address names only
        return glob, scoped
    for line in open(path):
        line = line.split('#', 1)[0].strip()
        if not line:
            continue
        m = re.fullmatch(r'(?:\[(\S+)\]\s*)?(\S+)\s*=\s*(\S+)\s*;', line)
        if not m:
            sys.exit(f'link_aliases.txt: cannot parse {line!r}')
        src, sym, tgt = m.groups()
        if src:
            scoped.setdefault(src, {})[sym] = tgt
        else:
            glob[sym] = tgt
    return glob, scoped


def q(sym):
    """quote a name for a linker script (unquoted names may not contain '$')"""
    return sym if re.fullmatch(r'[A-Za-z_.][\w.]*|0x[0-9A-F]+', sym) else f'"{sym}"'


def main():
    args = sys.argv[1:]
    redefine = None
    if '--redefine' in args:
        i = args.index('--redefine'); redefine = args[i + 1]; del args[i:i + 2]
    glob, scoped = read_table()
    und, dfn = set(), set()
    for o in args:
        u, d = elf_symbols(o); und |= u; dfn |= d
    if args:
        und -= dfn
        glob = {s: t for s, t in glob.items() if s in und}
    out = [f'{q(s)} = {q(t)};' for s, t in sorted(glob.items())]
    if args:
        names = config_names()
        want = {s for s in und if ADDR_NAME.match(s)} | {t for t in glob.values() if ADDR_NAME.match(t)}
        want |= {t for m in scoped.values() for t in m.values() if ADDR_NAME.match(t)}
        for s in sorted(want - dfn):
            a = int(ADDR_NAME.match(s).group(1), 16)
            out.append(f'{s} = {q(names.get(a, f"0x{a:08X}"))};')
    print('\n'.join(out))
    if redefine:
        os.makedirs(redefine, exist_ok=True)
        for src, m in scoped.items():
            with open(os.path.join(redefine, src.replace('/', '__') + '.redefine'), 'w') as f:
                f.writelines(f'{s} {t}\n' for s, t in sorted(m.items()))
    print(f'{len(glob)} table aliases, {len(out) - len(glob)} address names, '
          f'{sum(len(m) for m in scoped.values())} per-object renames in {len(scoped)} objects', file=sys.stderr)


if __name__ == '__main__':
    main()
