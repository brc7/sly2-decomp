#!/usr/bin/env python3
"""Check code-like names cited in docs/**/*.md and README.md against the current tree.

Usage: docnames.py [REPO] [--all] [--file=PATTERN] [--comments] [--allow FILE]
  REPO     repository root (default: the one this script is in)
  --allow  reviewed exceptions, one or more tokens per line, '#' comments (default: docnames_allow.txt beside this
           script). Add a token only after checking it is a deliberate non-symbol (example, former name, proposal).
Run after renames: docs that cite a renamed function, type, field or file show up here. Exit status 1 on misses.
Known names: symbol_addrs.txt (mangled names split), identifiers in src/ and include/ code (comments stripped),
plus path components. Cited names: backticked spans (identifiers and paths), PascalCase/camelCase function-like
tokens and ALLCAPS type-like tokens in prose. Paths are checked against the file tree.
"""
import os, re, sys, subprocess

if any(a in ('-h', '--help') for a in sys.argv[1:]):
    print(__doc__); sys.exit(0)
_pos = [a for i, a in enumerate(sys.argv[1:], 1) if not a.startswith('-') and sys.argv[i - 1] not in ('--allow', '--file')]
REPO = os.path.abspath(_pos[0] if _pos else os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..'))
ARGS = sys.argv[1:]
ALLOW_FILE = ARGS[ARGS.index('--allow') + 1] if '--allow' in ARGS else \
    os.path.join(os.path.dirname(os.path.abspath(__file__)), 'docnames_allow.txt')
IDENT = re.compile(r'[A-Za-z_][A-Za-z0-9_]*')


def strip_c_comments(s):
    out, i, n = [], 0, len(s)
    while i < n:
        c = s[i]
        if s.startswith('/*', i):
            j = s.find('*/', i + 2); i = n if j < 0 else j + 2; out.append(' ')
        elif s.startswith('//', i):
            j = s.find('\n', i); i = n if j < 0 else j
        elif c in '"\'':
            j = i + 1
            while j < n and s[j] != c:
                j += 2 if s[j] == '\\' else 1
            out.append(s[i:j + 1]); i = j + 1
        else:
            out.append(c); i += 1
    return ''.join(out)


known = set()
symnames = []
for line in open(os.path.join(REPO, 'config/symbol_addrs.txt')):
    m = re.match(r'\s*(\S+)\s*=', line)
    if not m:
        continue
    name = m.group(1)
    known.add(name)
    symnames.append(name)
    for part in re.split(r'__|\$|\d+', name):
        if part:
            known.add(part)
for f in ('functions.csv', 'matched.csv'):
    p = os.path.join(REPO, 'config', f)
    if os.path.exists(p):
        for tok in IDENT.findall(open(p, errors='replace').read()):
            known.add(tok)

files = subprocess.run(['git', '-C', REPO, 'ls-files'], capture_output=True, text=True).stdout.split()
paths = set(files)
for f in files:
    parts = f.split('/')
    for i in range(1, len(parts)):
        paths.add('/'.join(parts[:i]))
basenames = {}
for p in paths:
    basenames.setdefault(os.path.basename(p), []).append(p)

toolknown = set()
commentknown = set()
for f in files:
    if f.startswith('tools/') and os.path.splitext(f)[1] in ('.py', '.sh', '.in', '') and '/ghidra/' not in f:
        try:
            for l in open(os.path.join(REPO, f), errors='replace'):
                code = l.split('#')[0] if not l.lstrip().startswith(('"', "'")) else ''
                toolknown.update(IDENT.findall(code))
        except (OSError, IsADirectoryError):
            pass
for f in files:
    if not (f.startswith('src/') or f.startswith('include/')):
        continue
    ext = os.path.splitext(f)[1]
    try:
        s = open(os.path.join(REPO, f), errors='replace').read()
    except OSError:
        continue
    if ext in ('.c', '.cpp', '.h', '.inc', '.hpp'):
        commentknown.update(IDENT.findall(s))
        s = strip_c_comments(s)
        if ext == '.inc':
            s = '\n'.join(l.split('#')[0] for l in s.split('\n')) if not f.startswith('src/game') else s
    elif ext == '.md':
        continue
    for tok in IDENT.findall(s):
        known.add(tok)
        # demangled pieces inside asm labels
        for part in re.split(r'__|\$|\d+', tok):
            if part:
                known.add(part)

# prose words that look code-like but are not symbols
STOP = set('''PS2 PS3 PS4 GCC GNU ELF ISO VU VU0 VU1 GS GIF DMA EE IOP SPU SPU2 IRX USB CD DVD SDK API CPU GPU RAM
VRAM PC OK TODO FIXME NB TU TUs ABI CRC LZSS BRX WAL CSV HTML JSON YAML PNG CI MIPS FPU COP0 COP1 COP2 SIMD MMI
RGB RGBA XYZ UV ID IDs AI HUD UI NPC FMV PAL NTSC USA US EU SCUS SCES CLI GDB PCSX2 PINE ASCII UTF macOS MacBook
GitHub LLVM JIT IR OO II III IV ABC NOTE WIP II VIF VIF0 VIF1 GIFtag SPR TLB BSS DSL DLL XML URL SHA MD5 OS VM
PlayStation SafeHouse ProjectCane SlyTools Ghidra Docker DockerFile Dockerfile Python CMake Makefile README
SuckerPunch Sucker Punch Sly Bentley Murray Carmelita Clockwerk Dimitri Neyla JavaScript WebGL OpenGL Vulkan Metal
SDL SDL2 SDL3 FFmpeg MPEG IPU VAG ADPCM PCM BGM SFX FX MP3 WAV DDS TGA BMP OBJ FBX glTF GLB NaN NULL FALSE TRUE
INI LOD LODs FOV NDC RPC SIF IOPRP GPR GPRs MSB LSB LBA DMAC FIFO MPEG2 ROM BIOS PSU MCD OSD SCE Sce HDD
iPhone YouTube LTS GameCube Xbox PSP Vita RenderDoc WebAssembly
WSL NaGa NOTE_INSN_LOOP_BEG REG_EQUAL RTL EABI SPECIFIC REGISTRY CONFIRMED HYPOTHESIS COMMON RTTI CFLAGS DVP REFUTED
RAII LCP SGS VPK RIFF ADSR1 ADSR2 XOR NUL OPAQUE REPEAT COLOR_0 PSMT4 PSMT8 RGBA32 BITBLTBUF DPSM MODULATE PARTLY AND
PROGBITS OLD TOTAL TYPES QEMU WARNING RELIGHT BACKGROUND CUTOUT CEL BORDER FOREGROUND PACKED REGLIST DIRECT PATH2
PATH3 END THEN ELSE HSV FAILED KEEP TRIGGER ThiefNet KHR_materials_unlit baseColorFactor alphaMode camelCase
PascalCase UPPER_SNAKE VAGp KPV ModuleNotFoundError LLVM_BIN Library JOBS DATA_SOURCES FILE RRGGBB SUKR SUKR_ MRD
x411C7D26 u32BE''' .split())


TOP = ('src/', 'include/', 'tools/', 'docs/', 'config/', 'game/', 'sly2/', 'sdk/', 'generated/', 'build/')
EXT = r'\.(c|cpp|h|inc|py|sh|md|txt|csv|tsv|yaml|ld|in|s|S|json|dylib)$'


def looks_path(s):
    if s.startswith('http') or not re.match(r'^[\w.\-/*{}~]+$', s) or re.match(r'^[/.]+$', s):
        return False
    if re.search(EXT, s) and not re.match(r'^\.\w+(/\.\w+)*$', s):
        return True
    return '/' in s and (s.startswith(TOP) or s.endswith('/') or s.startswith(('~', '.')))


def check_path(s):
    s = s.rstrip('/').rstrip(',.')
    if s.startswith('./'):
        s = s[2:]
    if any(ch in s for ch in '*{}<>'):
        return True
    if s in paths:
        return True
    if '/' not in s:
        return s in basenames
    # relative tail match (e.g. sly2/act.h, game/app/x.cpp)
    for p in paths:
        if p.endswith('/' + s):
            return True
    if s.startswith(('build/', '/', '~', 'disc/', 'asm/', 'tools/compilers/', 'tools/cc/', 'tools/wibo-')):  # .gitignore'd / host
        return True
    return False


known_sorted = None


def ident_ok(t):
    global known_sorted
    if t in known or t in STOP or t in toolknown:
        return True
    if re.search(r'(NN|XX)$', t):
        rx = re.compile('^' + re.escape(t[:-2]) + '[0-9A-Fa-f]{2}$')
        if any(rx.match(k) for k in known):
            return True
    if re.match(r'^[A-Z][A-Z0-9_]{4,}$', t) and any(t in k for k in symnames):
        return True
    if '_PREFIX' in t and (t[0].islower() or not t.endswith('_PREFIX')):
        if t[0].islower() or t.endswith('_PREFIX') is False and re.search(r'\d', t):
            return True
        rx = re.compile('^' + '.*'.join(map(re.escape, t.split('_PREFIX'))) + '$')
        return any(rx.match(k) for k in known)
    if t.endswith('_PREFIX'):
        t = t[:-7]
        if t.endswith('_'):
            t = t[:-1]
        if t in known or t in toolknown:
            return True
        t += '_' if False else ''
        if len(t) > 2:
            if known_sorted is None:
                known_sorted = sorted(known)
            import bisect
            i = bisect.bisect_left(known_sorted, t)
            return i < len(known_sorted) and known_sorted[i].startswith(t)
        return False
    if t.endswith('_') and len(t) > 3:
        if known_sorted is None:
            known_sorted = sorted(known)
        import bisect
        i = bisect.bisect_left(known_sorted, t)
        return i < len(known_sorted) and known_sorted[i].startswith(t)
    return False


doc_files = sorted(f for f in files if (f.startswith('docs/') and f.endswith('.md')) or f == 'README.md')
only = [a.split('=', 1)[1] for a in ARGS if a.startswith('--file=')]
if only:
    doc_files = [f for f in doc_files if any(o in f for o in only)]
EXC_FILES = set()
if '--all' not in ARGS:
    EXC_FILES = {'docs/guides/tu-merging.md'}  # historical log; reported separately with --all
    EXC_FILES |= {f for f in doc_files if f.startswith('docs/archive/')}  # verbatim history moved out of tu-merging.md

PROSE_FUNC = re.compile(r'(?<![\w/.`-])([A-Z][a-z0-9]+(?:[A-Z][A-Za-z0-9]*)+|[a-z]+[A-Z][A-Za-z0-9]*)(?![\w/.-])')
PROSE_TYPE = re.compile(r'(?<![\w/.`-])([A-Z][A-Z0-9_]{2,})(?![\w/.-])')
CAMEL_OK = re.compile(r'^(g_|s_|D_|func_|sub_)')

SKIPRE = re.compile(r'^((0x|D_|func_|sub_|jtbl_|lbl_|L|x)[0-9A-Fa-f]+|[LSMUVTKR][0-9_]+|CID\\d+|S\\d\\d|[DLS]_?X+|func_X+|sN+|Lnn|vtN)$')
misses = {}
for f in doc_files:
    if f in EXC_FILES:
        continue
    infence = False
    for ln, line in enumerate(open(os.path.join(REPO, f), encoding='utf-8'), 1):
        if line.lstrip().startswith('```'):
            infence = not infence
            continue
        cands = []
        # strip links' URLs
        text = re.sub(r'\]\(([^)]*)\)', ']', line)
        text = re.sub(r'https?://\S+', '', text)
        spans = re.findall(r'`([^`]+)`', text) if not infence else [text]
        prose = re.sub(r'`[^`]+`', ' ', text) if not infence else ''
        for sp in spans:
            if not infence:
                sp = re.sub(r'\(([^()]*)\)', lambda m: '()' if re.match(r'^[\w\s,*&.]*$', m.group(1)) else m.group(0), sp)
                sp = re.sub(r'(\w)\*', r'\1_PREFIX', sp)
                sp = re.sub(r'\S*(<\w+>|\.\.\.)\S*', ' ', sp)  # placeholder patterns
            for word in re.split(r'[\s,;()]+', sp):
                if not word:
                    continue
                w = word.strip('"\'')
                if looks_path(w):
                    if not check_path(w):
                        cands.append(('path', w))
                    continue
                if infence:
                    continue
                for t in IDENT.findall(w):
                    if len(t) < 3 or t.isdigit() or t.islower():
                        continue
                    if SKIPRE.match(t):
                        continue
                    if not ident_ok(t):
                        cands.append(('ident', t))
        for m in PROSE_FUNC.finditer(prose):
            t = m.group(1)
            if not ident_ok(t):
                cands.append(('prose', t))
        for m in PROSE_TYPE.finditer(prose):
            t = m.group(1)
            if not ident_ok(t) and not SKIPRE.match(t):
                cands.append(('ptype', t))
        if re.search(r'not in (this|the) repo|outside the repo|research workspace', line):
            cands = [c for c in cands if c[0] != 'path']
        for kind, t in cands:
            misses.setdefault((kind, t), []).append('%s:%d' % (f, ln))

ALLOW = set()
allow_p = ALLOW_FILE
if os.path.exists(allow_p):
    for l in open(allow_p):
        ALLOW.update(l.split('#')[0].split())
un = {k: v for k, v in misses.items() if k[1] not in ALLOW}
shown = 0
for (kind, t), locs in sorted(un.items(), key=lambda kv: kv[1][0]):
    if kind != 'path' and t in commentknown:
        kind = 'C-' + kind
        if '--comments' not in ARGS and re.match(r'^[A-Z][A-Z0-9_]*$', t):
            continue  # concept/type label also used by source comments
    shown += 1
    print('%-6s %-32s %s' % (kind, t, ' '.join(locs[:4]) + (' ...' if len(locs) > 4 else '')))
print('# %d unexplained misses (%d allowed)' % (shown, len(misses) - len(un)))
sys.exit(1 if shown else 0)
