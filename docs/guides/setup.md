# Setting up the build on macOS, Linux and Windows

This repo matches every function of `SCUS_973.16` byte for byte. "Building" here means compiling each source file
with the original 2004-era PS2 compilers and comparing the result with the retail executable
(`tools/matchtool.py`, `tools/progress.py`). See [SETUP.md](../../SETUP.md) for upstream's Linux/WSL notes.

## Why the toolchain is unusual
| Code | Compiler | Kind of binary | Runs via |
|---|---|---|---|
| Game code (~6,300 files: `src/hand`, `src/auto`, `src/splicewrap`, `src/P2`) | SN/Sony `ee-gcc.exe` 2.95.3-136 + SN `ps2eeas.exe` | 32-bit **Windows** | [wibo](https://github.com/decompals/wibo) (minimal Win32 loader) |
| Sony SDK libraries (~280 files in `src/lib`) | ee-gcc 2.96 / 2.9-991111 / 3.2 + GNU as 2.10 | 32-bit **x86 Linux** | directly (needs 32-bit glibc) |
| Mixed SDK files (54, `// SLY2_ASM: gnu-reorder*`) | Windows ee-gcc → GNU as 2.10 | both | wibo, then Linux `as` |

wibo emulates the Windows thread block with custom x86 segment descriptors. That works on x86 Linux and under
**macOS Rosetta 2**, but **not** under Rosetta-for-Linux or QEMU, i.e. not inside Docker on Apple Silicon.

## Apple Silicon (hybrid: native wibo + Docker)
Prereqs: Rosetta 2 (`softwareupdate --install-rosetta --agree-to-license`), Docker Desktop (running), Python 3,
your own copy of *Sly 2: Band of Thieves* (USA, SCUS-97316).
```
python3 tools/extract_elf.py "/path/to/Sly 2 - Band of Thieves (USA).iso"   # writes disc/SCUS_973.16, checks SHA-1
python3 -m pip install rabbitizer
tools/dev.sh image        # build the toolchain image (ubuntu amd64 + 32-bit glibc + python tools)
tools/dev.sh setup        # compilers into tools/ (in Docker) + on macOS: tools/wibo-macos, ~/m2c
```
How the pieces route:
- `python3 tools/matchtool.py check FILE` runs natively; `tools/wibo_cc.py` picks `tools/wibo-macos` on macOS.
- For mixed files the GNU `as` step runs inside the Docker image automatically (`tools/gnu_reorder.py:run_linux_tool`).
- Pure Linux-compiler SDK files only build in Docker: `tools/dev.sh progress`.

Full verification = both runs (results merge in `build/progress_cache.json`):
```
JOBS=8 python3 tools/progress.py     # native: everything wibo can build (~4 min on an M-series Mac)
tools/dev.sh progress                # Docker: the Linux-compiler SDK files
```
**Done means `matched functions: 6662/6662` and no `WARNING` line (exit status 0).** progress.py prints a WARNING
and exits non-zero if any file's result is carried over from the committed `config/matched.csv` instead of being
verified in at least one environment.

Before pushing a branch, `tools/verify.sh` runs all of it in one go — a clean native run, the Docker run, the
`config/matched.csv` diff, the doc link check and the full link (`--no-link` skips it) — and prints a pass/fail summary
(logs in build/logs/verify-*.log).

Docker Desktop's "Use Rosetta for x86_64/amd64 emulation" may be on or off; only Linux-compiler binaries run there.

## x86-64 Linux (and WSL)
Everything works in one environment:
```
tools/dev.sh image && tools/dev.sh setup && tools/dev.sh progress     # Docker
# or natively, per SETUP.md: bash tools/setup_all.sh "<ISO>"; python3 tools/progress.py
```

## Windows (native, no WSL)
Upstream's tools run the `.exe` compilers directly when `os.name == 'nt'`; the Linux-only SDK compilers still need
WSL or Docker. Untested by us.

## Everyday loop
Edit a source file, then `python3 tools/matchtool.py check src/.../file.cpp` (~1 s). Before review: the full
verification above (`tools/verify.sh`). progress.py rewrites `config/matched.csv`; commit it only when a function's
result is meant to change.

## Troubleshooting
| Symptom | Cause / fix |
|---|---|
| `rosetta error: invalid gdt selector` / `qemu: uncaught target signal 11` from wibo | wibo inside Docker on Apple Silicon — expected; use native wibo-macos |
| `m2c_macros.h: No such file` (all of `src/auto`) | clone https://github.com/matt-kempster/m2c to `~/m2c` or set `SLY2_M2C_INC` |
| `ModuleNotFoundError: rabbitizer` | `python3 -m pip install rabbitizer` (or `SLY2_PYTHONPATH` to a checkout) |
| mixed SDK files fail on macOS with a docker error | start Docker Desktop; build the image (`tools/dev.sh image`) |
| `tty[@]: unbound variable` | old dev.sh on macOS bash 3.2 — update |
| `__tf… no address known` | compiler type-info symbol (only without `-fno-rtti`, e.g. an old `SLY2_CFLAGS` env); harmless |

Environment variables: `SLY2_WIBO_BIN` (wibo path), `SLY2_M2C_INC`, `SLY2_PYTHONPATH`, `SLY2_SLY1` (Sly 1 decomp
checkout for tools/show.py), `SLY2_IMAGE` (Docker image), `JOBS`.
