# Setting up the Sly 2 decomp

This repository contains **only our own source code and tools**. It does not contain the game, its code, or any
compiler binaries. You need your own copy of *Sly 2: Band of Thieves* (USA, SCUS-97316) and a Linux (or WSL) machine.

## Quick start (one command)
On Linux or WSL (Windows: install WSL first):
```
git clone https://github.com/Komasia/sly2-decomp && cd sly2-decomp
bash tools/setup_all.sh "/path/to/Sly 2 - Band of Thieves (USA).iso"
```
This downloads every compiler and tool the build uses into `tools/` (they are third-party programs, so they are
fetched from decomp.me / GitHub rather than stored in this repo), installs the Python packages and the 32-bit C library
the PS2 compilers need, extracts `disc/SCUS_973.16` from **your own** disc image, and writes the disassembly. Then `python3 tools/matchtool.py check src/game/core/binstream/bis.cpp` should print MATCH lines.
**Already set up?** From your existing clone just run (no ISO needed, since you already have `disc/SCUS_973.16`):
```
git pull
bash tools/setup_all.sh
```
It checks each item, downloads only what is missing (for example the new ee-gcc 2.9 / 3.2 compilers) and prints a
summary. Nothing existing is overwritten.

The sections below explain what each piece is, if you want to do it by hand.

## 1. Game executable (from your own disc)
```
python3 tools/extract_elf.py "/path/to/Sly 2 - Band of Thieves (USA).iso"
```
It writes `disc/SCUS_973.16` and checks SHA-1 `de2af92a54de71e33edc8a160c31c0337a95e6f5`.

## 2. Python packages
```
pip install rabbitizer spimdisasm splat64[mips]
git clone https://github.com/matt-kempster/m2c ~/m2c     # only needed for tools/show.py drafts
```
`tools/matchtool.py` needs `rabbitizer` on PYTHONPATH.

## 3. Compilers (put them under `tools/`)
| What | Where it goes | Get it from |
|---|---|---|
| wibo (runs the Windows compilers on Linux) | `tools/wibo-i686` (chmod +x) | https://github.com/decompals/wibo/releases/download/1.0.0/wibo-i686 |
| PS2 compiler bundle (game code: **ee-gcc 2.95.3-136**) | extract to `tools/compilers/bundle/` | https://github.com/decompme/compilers/releases/download/compilers/ps2_compilers.tar.xz |
| ee-gcc 2.95.3-114 (only for its SN assembler `ps2eeas.exe`) | extract to `tools/compilers/2.95.3-114/` | https://github.com/decompme/compilers/releases/download/compilers/ee-gcc2.95.3-114.tar.gz |
| ee-gcc 2.96 (Sony SDK library code, GNU as) | extract to `tools/compilers/ee-gcc2.96/` | https://github.com/decompme/compilers/releases/download/compilers/ee-gcc2.96.tar.xz |
| More PS2 compilers (Sony SDK libraries: ee-gcc 2.9 does `mult1` and tail calls) | extract each to `tools/compilers/extra/<name>/` | `https://github.com/decompme/compilers/releases/download/compilers/<name>.tar.xz` for ee-gcc2.9-990721, ee-gcc2.9-991111, ee-gcc2.9-991111a, ee-gcc2.9-991111-01, ee-gcc3.2-040921; `.tar.gz` for ee-gcc2.9-991111-01-dtls13010, ee-gcc3.2-030926 |

ee-gcc 2.96 is a 32-bit Linux program: on Ubuntu/Debian `sudo dpkg --add-architecture i386 && sudo apt install libc6:i386`
(or use `tools/setup_i386.sh` with Ubuntu's `libc6_*_i386.deb`).

## 4. Optional: disassembly for drafts
```
python3 -m splat split config/sly2.yaml      # writes asm/ (used by tools/show.py)
```

## 5. Check it works
```
export PYTHONPATH=<path to rabbitizer etc.>
python3 tools/matchtool.py check src/game/core/binstream/bis.cpp      # every function should print MATCH
python3 tools/progress.py                            # rebuilds config/matched.csv and prints totals
```

## Notes
- Compiler facts, per-file compiler headers (`SLY2_CC:`, `SLY2_CFLAGS:`, `SLY2_ASM:` …) and the matching techniques
  used are written up in `docs/MATCHING_NOTES.md`.
- `python3 tools/show.py NAME` prints a function's original assembly next to an m2c draft.
