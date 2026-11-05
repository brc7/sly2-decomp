#!/usr/bin/env python3
"""Sly 2 function matcher.

  matchtool.py check  <src.cpp> [func ...]   compile src, compare every (or listed) function against SCUS_973.16
  matchtool.py target <func>                 print the original disassembly of one function

A function is identified by its symbol name in the compiled object (mangled C++ names are fine)
and is looked up in config/symbol_addrs.txt + config/functions.csv for its address/size.
Comparison masks only the fields that the object's relocations will fill in at link time
(jal targets, %hi/%lo of globals, %gp_rel), so a MATCH here means byte-identical after linking.
`check` also compares every data symbol the object defines whose address is known (D_XXXXXXXX names, or
`// type:data` lines in config/symbol_addrs.txt) against the ELF: see obj_data/compare_data.
"""
import os, re, struct, subprocess, sys, csv, tempfile
# Extra Python search paths (e.g. a local rabbitizer checkout), os.pathsep-separated.
for _p in filter(None, os.environ.get('SLY2_PYTHONPATH', '').split(os.pathsep)):
    if _p not in sys.path and os.path.isdir(_p): sys.path.insert(0, _p)
import rabbitizer

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ELF = os.path.join(ROOT, 'disc', 'SCUS_973.16')
TEXT_VA, TEXT_OFF, TEXT_SIZE = 0x100000, 0x80, 0x159df0
CC = os.environ.get('SLY2_CC', os.path.join(ROOT, 'tools', 'compilers', 'bundle', 'ee-gcc2.95.3-136', 'bin', 'ee-gcc.exe'))  # Sly 2 compiler
WIBO = os.environ.get('SLY2_WIBO', os.path.join(ROOT, 'tools', 'wibo_cc.py'))
WIBO_CMD = [sys.executable, WIBO] if os.name == 'nt' else [WIBO]   # native Windows: no wibo, wibo_cc.py runs the .exe files
M2C_INC = os.environ.get('SLY2_M2C_INC', os.path.expanduser('~/m2c'))     # directory holding m2c_macros.h (needed by raw m2c .c files)
# -fno-rtti: the game was built without RTTI (the typeinfo slot of every vtable in the ELF is 0, and no __tf*/__rtti_*
# code exists); with RTTI g++ fills that slot and emits typeinfo functions for each key TU. C++ only (see compile_src).
CFLAGS = os.environ.get('SLY2_CFLAGS', '-x c++ -O2 -G0 -ffast-math -mvu0-use-vf0-vf31 -fno-exceptions -fno-rtti').split() + ['-B' + os.path.join(ROOT, 'tools/compilers/bundle/ee-gcc2.95.3-136/lib/gcc-lib/ee/2.95.3') + '/']
INCLUDES = ['-I' + os.path.join(ROOT, 'include')] + os.environ.get('SLY2_INC', '').split()

rabbitizer.config.regNames_gprAbiNames = rabbitizer.Abi.O32
rabbitizer.config.misc_omit0XOnSmallImm = False

def load_functions():
    funcs = {}
    p = os.path.join(ROOT, 'config', 'functions.csv')
    if os.path.exists(p):
        for r in csv.DictReader(open(p)):
            a = int(r['Start'], 16); funcs[r['Name']] = (a, int(r['End'], 16) - a)
    by_addr = {a: (n, s) for n, (a, s) in funcs.items()}
    for line in open(os.path.join(ROOT, 'config', 'symbol_addrs.txt')):
        m = re.match(r'(\S+)\s*=\s*0x([0-9A-Fa-f]+);', line)
        if m:
            a = int(m.group(2), 16)
            if a in by_addr:
                funcs[m.group(1)] = (a, by_addr[a][1])
    return funcs

def load_junk():
    """Address ranges of dead-code scraps between functions (config/junk.csv): not functions, never counted."""
    p = os.path.join(ROOT, 'config', 'junk.csv')
    if not os.path.exists(p): return []
    return [(int(r['Start'], 16), int(r['End'], 16)) for r in csv.DictReader(open(p))]

def is_junk(addr, size, junk=None):
    return any(lo <= addr and addr + size <= hi for lo, hi in (junk if junk is not None else load_junk()))

def target_words(addr, size):
    elf = open(ELF, 'rb').read()
    off = addr - TEXT_VA + TEXT_OFF
    return list(struct.unpack('<%dI' % (size // 4), elf[off:off + size]))

# ---- minimal ELF32-LE object reader ----
def read_obj(path):
    d = open(path, 'rb').read()
    shoff, = struct.unpack_from('<I', d, 0x20); shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2E)
    secs = []
    for i in range(shnum):
        name, typ, flags, addr, off, size, link, info, align, entsize = struct.unpack_from('<10I', d, shoff + i * shentsize)
        secs.append(dict(name=name, type=typ, flags=flags, off=off, size=size, link=link, info=info, entsize=entsize))
    shstr = secs[shstrndx]
    for s in secs:
        s['name'] = d[shstr['off'] + s['name']:d.index(b'\0', shstr['off'] + s['name'])].decode()
    symtab = next(s for s in secs if s['type'] == 2)
    strtab = secs[symtab['link']]
    syms = []
    for i in range(symtab['size'] // 16):
        nm, val, size, info, other, shndx = struct.unpack_from('<IIIBBH', d, symtab['off'] + i * 16)
        n = d[strtab['off'] + nm:d.index(b'\0', strtab['off'] + nm)].decode()
        syms.append(dict(name=n, value=val, size=size, type=info & 0xf, shndx=shndx))
    relocs = {}  # section index -> {offset: type}
    for s in secs:
        if s['type'] == 9:  # SHT_REL
            r = relocs.setdefault(s['info'], {})
            for i in range(s['size'] // 8):
                off, inf = struct.unpack_from('<II', d, s['off'] + i * 8)
                r[off] = inf & 0xff
    return d, secs, syms, relocs

def obj_functions(path):
    d, secs, syms, relocs = read_obj(path)
    out = {}
    for s in syms:
        if s['type'] == 2 and s['size'] > 0 and s['shndx'] < len(secs):
            sec = secs[s['shndx']]
            words = list(struct.unpack_from('<%dI' % (s['size'] // 4), d, sec['off'] + s['value']))
            rel = {(o - s['value']) // 4: t for o, t in relocs.get(s['shndx'], {}).items() if s['value'] <= o < s['value'] + s['size']}
            out[s['name']] = (words, rel)
    return out

# ---- data symbols ----
# A data symbol is checked when the object DEFINES it (in .data/.rodata/.bss/.sdata/... or as COMMON) and its address
# is known: a `name = 0xADDR; // type:data size:0xN` line in config/symbol_addrs.txt (size optional) or the
# D_XXXXXXXX naming convention. Initialized data is compared byte for byte with the ELF, words covered by a
# relocation masked as in code (R_MIPS_32 fills the whole word); .bss/COMMON has no bytes, so only its placement and
# size are checked. The ELF has no symbol table, so a symbol's true extent is known only from a size: in
# symbol_addrs.txt; without one, a definition that is too SHORT cannot be detected.
_D_NAME = re.compile(r'D_([0-9A-F]{8})$')

def load_data_addrs():
    out = {}
    for line in open(os.path.join(ROOT, 'config', 'symbol_addrs.txt')):
        m = re.match(r'(\S+)\s*=\s*0x([0-9A-Fa-f]+);.*type:data', line)
        if m:
            sz = re.search(r'size:0x([0-9A-Fa-f]+)', line)
            out[m.group(1)] = (int(m.group(2), 16), int(sz.group(1), 16) if sz else None)
    return out

def data_addr(name, known):
    if name in known: return known[name]
    m = _D_NAME.match(name)
    return (int(m.group(1), 16), None) if m else None

def elf_sections():
    """(name, va, size, nobits) of the ELF's allocated sections."""
    d = open(ELF, 'rb').read()
    shoff, = struct.unpack_from('<I', d, 0x20); shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2E)
    hdr = [struct.unpack_from('<10I', d, shoff + i * shentsize) for i in range(shnum)]
    stro = hdr[shstrndx][4]
    return [(d[stro + h[0]:d.index(b'\0', stro + h[0])].decode(), h[3], h[5], h[1] == 8)
            for h in hdr if h[2] & 2 and h[3]]   # SHF_ALLOC with an address

def obj_data(path):
    """name -> (bytes or None for .bss/COMMON, size, {byte offset: reloc type}, section name) for every data symbol
    the object defines."""
    d, secs, syms, relocs = read_obj(path)
    out = {}
    starts = {}   # section -> sorted symbol offsets, for the size of a file-local .bss object (.lcomm has no size)
    for s in syms:
        if s['name'] and s['type'] in (0, 1) and s['shndx'] < len(secs): starts.setdefault(s['shndx'], set()).add(s['value'])
    for s in syms:
        if s['type'] not in (0, 1) or not s['name'] or s['name'] in ('gcc2_compiled.', '__gnu_compiled_cplusplus'): continue
        if s['size'] == 0 and s['shndx'] < len(secs) and secs[s['shndx']]['type'] == 8:
            s = dict(s, size=min([v for v in starts[s['shndx']] if v > s['value']] + [secs[s['shndx']]['size']]) - s['value'])
        if s['size'] == 0: continue
        if s['shndx'] == 0xFFF2:   # SHN_COMMON
            out[s['name']] = (None, s['size'], {}, 'COMMON'); continue
        if s['shndx'] == 0 or s['shndx'] >= len(secs): continue
        sec = secs[s['shndx']]
        if not sec['flags'] & 2 or sec['flags'] & 4: continue   # not SHF_ALLOC, or SHF_EXECINSTR
        if sec['type'] == 8:       # SHT_NOBITS
            out[s['name']] = (None, s['size'], {}, sec['name']); continue
        b = d[sec['off'] + s['value']:sec['off'] + s['value'] + s['size']]
        rel = {o - s['value']: t for o, t in relocs.get(s['shndx'], {}).items() if s['value'] <= o < s['value'] + s['size']}
        out[s['name']] = (b, s['size'], rel, sec['name'])
    return out

def compare_data(name, b, size, rel, secname, verbose=True, known=None):
    """Like compare() for one data symbol. Returns (score, (headline, lines)) or (None, message)."""
    a = data_addr(name, load_data_addrs() if known is None else known)
    if a is None:
        return None, f'{name}: no address known (data)'
    addr, tsize = a
    sec = next((s for s in elf_sections() if s[1] <= addr < s[1] + s[2]), None)
    head = f'{name} @0x{addr:06X} [{secname} 0x{size:X}]: '
    problems = []
    if sec is None:
        return 0.0, (head + 'address is in no section of the ELF', [])
    if addr + size > sec[1] + sec[2]:
        problems.append(f'runs 0x{addr + size - sec[1] - sec[2]:X} bytes past the end of {sec[0]}')
    if tsize is not None and tsize != size:
        problems.append(f'size 0x{size:X}, symbol_addrs.txt says 0x{tsize:X}')
    if (b is None) != sec[3]:
        problems.append(f'ours is {"zero-initialized (bss)" if b is None else "initialized data"}, target is in {sec[0]}')
    lines = []
    if b is not None and not sec[3]:
        elf = open(ELF, 'rb').read()
        off = addr - TEXT_VA + TEXT_OFF
        t = elf[off:off + size]
        masked = set()
        for o in rel: masked.update(range(o, o + 4))   # every data relocation here fills a whole word (R_MIPS_32)
        bad = [i for i in range(size) if i not in masked and (i >= len(t) or t[i] != b[i])]
        if bad:
            rows = sorted({i // 4 for i in bad})
            problems.append(f'{len(bad)}/{size} bytes differ')
            if verbose:
                for r in rows[:32]:
                    tw, ow = t[4*r:4*r+4], b[4*r:4*r+4]
                    lines.append(f'!! {addr + 4*r:08X}  {tw.hex():8s} | {ow.hex()}')
                if len(rows) > 32: lines.append(f'   ... {len(rows) - 32} more differing words')
        score = 1.0 - len(bad) / size
    else:
        score = 1.0
    if problems:
        return (min(score, 0.999) if score == 1.0 else score), (head + '; '.join(problems), lines)
    return 1.0, (head + 'MATCH' + (' (bss: placement/size only)' if b is None else ''), lines)

def apply_aliases(src, fns):
    """Per-TU symbol aliases for compiler-generated local functions whose object names are not unique or depend on the
    source path, e.g. in a real TU:
      // SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_letterbox
      // SLY2_ALIAS: _GLOBAL_$I$*=_GLOBAL_$I$letterbox          (trailing * matches any suffix)
    The right-hand name is then looked up in config/symbol_addrs.txt like any other symbol."""
    try:
        head = open(src, errors='ignore').read(4000)
    except OSError:
        return fns
    for a, b in re.findall(r'SLY2_ALIAS:\s*(\S+?)=(\S+)', head):
        for n in list(fns):
            if n == a or (a.endswith('*') and n.startswith(a[:-1])):
                fns[b] = fns.pop(n)
    return fns

# R_MIPS_26=4 (jal/j), HI16=5, LO16=6, GPREL16=7, LITERAL=8, GOT16=9, PC16=10, CALL16=11
def mask(word, rtype):
    if rtype == 4: return word & 0xFC000000
    if rtype in (5, 6, 7, 8, 9, 11): return word & 0xFFFF0000
    return word

def disasm(word, va):
    return rabbitizer.Instruction(word, vram=va, category=rabbitizer.InstrCategory.R5900).disassemble()

LENIENT = os.environ.get('SLY2_LENIENT', '0') == '1'   # until the Sly 2 compiler is identified

def same_lenient(a, b):
    # sq<->sd (op 0x1F vs 0x3F) and lq<->ld (0x1E vs 0x37) with identical rs/rt/imm
    ops = {(0x1F, 0x3F), (0x3F, 0x1F), (0x1E, 0x37), (0x37, 0x1E)}
    return (a >> 26, b >> 26) in ops and (a & 0x03FFFFFF) == (b & 0x03FFFFFF)

IMM_OPS = {0x02, 0x03, 0x08, 0x09, 0x0D, 0x0F, 0x18, 0x19, 0x1E, 0x1F, 0x20, 0x21, 0x23, 0x24, 0x25, 0x27, 0x28, 0x29,
           0x2B, 0x31, 0x36, 0x37, 0x39, 0x3E, 0x3F}

def aligned_diffs(tgt, words):
    """Differences after aligning the two instruction streams (an extra/missing instruction counts once instead of
    shifting everything after it).  Immediates of address-forming ops are ignored for the alignment."""
    import difflib
    def key(w):
        op = w >> 26
        if op in (0x02, 0x03): return w & 0xFC000000
        if op in IMM_OPS: return w & 0xFFFF0000
        return w
    a = [key(w) for w in tgt]; b = [key(w) for w in words]
    m = sum(bl.size for bl in difflib.SequenceMatcher(None, a, b, autojunk=False).get_matching_blocks())
    return max(len(a), len(b)) - m

def compare(name, words, rel, verbose=True):
    funcs = load_functions()
    if name not in funcs:
        return None, f'{name}: no address known (add it to config/symbol_addrs.txt)'
    addr, size = funcs[name]
    tgt = target_words(addr, size)
    # trailing nops/alignment padding in the target are not part of the function body
    while len(tgt) > len(words) and tgt[-1] == 0: tgt.pop()
    n = max(len(tgt), len(words)); diffs = []; lenient_hits = []
    for i in range(n):
        a = tgt[i] if i < len(tgt) else None; b = words[i] if i < len(words) else None
        r = rel.get(i)
        if a is None or b is None or (mask(a, r) != mask(b, r) if r is not None else a != b):
            if LENIENT and a is not None and b is not None and same_lenient(a, b):
                lenient_hits.append(i)
                continue
            diffs.append(i)
    score = 1.0 - len(diffs) / n if n else 0
    aligned = aligned_diffs(tgt, words) if diffs else 0
    lines = []
    if verbose and diffs:
        for i in range(n):
            a = tgt[i] if i < len(tgt) else None; b = words[i] if i < len(words) else None
            mark = '  ' if i not in diffs else '!!'
            lines.append(f'{mark} {addr + 4*i:08X}  {disasm(a, addr+4*i) if a is not None else "-":40s} | {disasm(b, addr+4*i) if b is not None else "-"}')
    return score, (f'{name} @0x{addr:06X}: {("MATCH*" if lenient_hits else "MATCH") if not diffs else f"{len(diffs)}/{n} instructions differ ({score:.1%}); aligned {aligned}"}', lines)

def compile_src(src):
    out = tempfile.mktemp(suffix='.o')
    flags = CFLAGS
    if src.endswith('.c'):
        flags = ['c' if x == 'c++' else x for x in CFLAGS if x != '-fno-rtti']   # C++-only flag
        flags = flags + ['-I' + M2C_INC]
    cc = CC
    # Per-file overrides (used for Sony SDK library code built with other compilers/flags):
    #   // SLY2_CC: tools/compilers/bundle/ee-gcc2.95.2-274          (compiler directory, relative to project root)
    #   // SLY2_CFLAGS: -O2 -G0                                         (replaces the default optimisation flags)
    try:
        head = open(src, errors='ignore').read(2000)
    except OSError:
        head = ''
    import re as _re
    native = False
    m = _re.search(r'SLY2_CC:\s*(\S+)', head)
    if m and os.path.exists(os.path.join(ROOT, m.group(1), 'bin', 'ee-gcc')):
        # native 32-bit Linux toolchain (ee-gcc 2.96, used for Sony SDK libraries); needs /lib/ld-linux.so.2
        d = os.path.join(ROOT, m.group(1)); native = True
        cc = os.path.join(d, 'bin', 'ee-gcc')
        libdir = [os.path.join(d, 'lib', 'gcc-lib', 'ee', v) for v in sorted(os.listdir(os.path.join(d, 'lib', 'gcc-lib', 'ee')))][0]
        flags = [x for x in flags if not x.startswith('-B')] + ['-B' + libdir + '/', '-B' + os.path.join(d, 'ee', 'bin') + '/']
        m = None
    if m:
        d = os.path.join(ROOT, m.group(1))
        cc = os.path.join(d, 'bin', 'ee-gcc.exe')
        libdir = [os.path.join(d, 'lib', 'gcc-lib', 'ee', v) for v in sorted(os.listdir(os.path.join(d, 'lib', 'gcc-lib', 'ee')))][0]
        flags = [x for x in flags if not x.startswith('-B')] + ['-B' + libdir + '/']
    m = _re.search(r'SLY2_CFLAGS:\s*(.+)', head)
    if m:
        lang = ['-x', 'c'] if src.endswith('.c') else ['-x', 'c++']
        flags = lang + m.group(1).split() + [x for x in flags if x.startswith('-B') or x.startswith('-I')]
    _am = _re.search(r'SLY2_ASM:\s*(gnu-reorder-db|gnu-reorder|gnu-reorg)\b', head)
    if _am:
        # gnu-reorder    : no gcc delay-slot filling; GNU as fills slots (libkernel)
        # gnu-reorder-db : gcc fills slots (reorg), GNU as fills what is left (libm, libmpeg, printf, alarm)
        # gnu-reorg      : as gnu-reorder-db, but short single-block loop slots are left empty (libgcc, libgraph)
        asm_mode = _am.group(1)
        # SDK path: compile to .s without delay-slot filling, then GNU-as reorder assembly (tools/gnu_reorder.py)
        import gnu_reorder
        s_out = out + '.s'
        import wibo_cc; real = wibo_cc.default_wibo()
        scmd = ([] if native else [real]) + [cc, '-S'] + INCLUDES + [x for x in flags] + (['-fno-delayed-branch'] if asm_mode == 'gnu-reorder' else []) + [src, '-o', s_out]
        if native: scmd = [x for x in scmd if x != '-mvu0-use-vf0-vf31']
        p = subprocess.run(scmd, capture_output=True, text=True, cwd=ROOT, timeout=300)
        if not p.returncode and os.path.exists(s_out) and _re.search(r'SLY2_SAVES:\s*sd\b', head):
            # Sony's SDK compiler saved/restored registers with 64-bit sd/ld (in 16-byte slots); the ee-gcc we have
            # emits 128-bit sq/lq there.  Emulate that compiler difference at assembly time (SDK C code has no
            # other 128-bit loads/stores, and any wrong rewrite would simply fail to match).
            body = open(s_out).read()
            # only register saves/restores (stack-pointer based); real 128-bit data stores stay sq/lq
            body = _re.sub(r'^(\s+)sq(\s+\$\w+\s*,\s*-?\w*\(\$sp\))', r'\1sd\2', body, flags=_re.M)
            body = _re.sub(r'^(\s+)lq(\s+\$\w+\s*,\s*-?\w*\(\$sp\))', r'\1ld\2', body, flags=_re.M)
            open(s_out, 'w').write(body)
        if p.returncode or not os.path.exists(s_out) or gnu_reorder.assemble(s_out, out, ROOT, asm_mode, divnop=bool(_re.search(r'SLY2_DIVNOP', head)), noloopfix=bool(_re.search(r'SLY2_NOLOOPFIX', head))):
            print(p.stdout + p.stderr); sys.exit(1)
        return out
    cmd = ([] if native else WIBO_CMD) + [cc, '-c'] + INCLUDES + flags + [src, '-o', out]
    if native:
        cmd = [x for x in cmd if x != '-mvu0-use-vf0-vf31']
    p = subprocess.run(cmd, capture_output=True, text=True, cwd=ROOT, timeout=300)
    if p.returncode != 0 or not os.path.exists(out):
        print(p.stdout + p.stderr); sys.exit(1)
    return out

def main():
    if len(sys.argv) < 3: print(__doc__); return
    if sys.argv[1] == 'target':
        a, s = load_functions()[sys.argv[2]]
        for i, w in enumerate(target_words(a, s)): print(f'{a+4*i:08X}  {disasm(w, a+4*i)}')
        return
    obj = compile_src(sys.argv[2]); fns = apply_aliases(sys.argv[2], obj_functions(obj))
    data = obj_data(obj); known = load_data_addrs()
    want = sys.argv[3:] or list(fns) + [n for n in data if data_addr(n, known)]
    for f in want:
        if f in fns: score, res = compare(f, *fns[f])
        elif f in data: score, res = compare_data(f, *data[f], known=known)
        else: print(f'{f}: not in object'); continue
        if score is None: print(res); continue
        print(res[0]); [print(l) for l in res[1]]

if __name__ == '__main__':
    main()
