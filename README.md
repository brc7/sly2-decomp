# Sly 2: Band of Thieves — matching decompilation

A matching decompilation of *Sly 2: Band of Thieves* for the PlayStation 2 (NTSC-U, `SCUS_973.16`): C/C++ source
that, compiled with the original PS2 compilers, rebuilds the game's executable byte for byte. On top of the match, the
code is being turned into readable, documented source, for a faithful port, for mods, and for learning from the
game's design.

This is a fork of [Komasia/sly2-decomp](https://github.com/Komasia/sly2-decomp). The matching work was done by its
contributors (Dimitri/Komasia, NaGa, Lilly and others, working with AI assistants); see [CREDITS.md](CREDITS.md).

Target: `SCUS_973.16` (NTSC-U, SHA-1 `de2af92a54de71e33edc8a160c31c0337a95e6f5`).

## Status

- **Every function matches.** Each function compiles to code identical to the original, and every call and data
  reference in it points at the right target (relocation-aware check). `python3 tools/progress.py` prints the totals.
- **The full link reproduces the ELF.** `tools/link.py` links the compiled objects into an executable with no
  differing bytes; some data, the IOP modules, the VU microcode and the SDK sources that do not build on every host
  are still taken from your copy of the original as byte ranges
  ([docs/guides/full-link.md](docs/guides/full-link.md)).
- **Readability is ongoing.** Most functions have descriptive names and doc comments; names marked `(inferred)` still
  need confirmation. `python3 tools/readability.py` reports the current state. Naming and cleanup never change the
  match.

## What you need

- **Your own legally obtained copy of the game** (USA, SCUS-97316) as a disc image. The repo contains no game data,
  no game code and no compiler binaries.
- **A host**: macOS on Apple Silicon or x86-64 Linux (WSL works). Windows without WSL is untested.
- **Python 3** with `rabbitizer` (`python3 -m pip install rabbitizer`).
- **Docker** for the Sony SDK files that need the Linux-hosted compilers (on Apple Silicon the game code itself is
  compiled natively through wibo under Rosetta 2).
- **LLVM 18 `ld.lld` and `llvm-objcopy`** for the full link (macOS: `brew install llvm lld`).

The setup steps per host, and why the toolchain is unusual, are in [docs/guides/setup.md](docs/guides/setup.md);
the Linux one-command setup is in [SETUP.md](SETUP.md).

## Quick start

```sh
git clone https://github.com/brc7/sly2-decomp && cd sly2-decomp

# Extract the executable from your disc image (checks the SHA-1, writes disc/SCUS_973.16)
python3 tools/extract_elf.py "/path/to/Sly 2 - Band of Thieves (USA).iso"

# Toolchain: Docker image, then compilers into tools/ (on macOS also the native wibo)
tools/dev.sh image
tools/dev.sh setup

# Check one file: prints MATCH per function, or a side-by-side diff
python3 tools/matchtool.py check src/game/core/binstream/bis.cpp

# Check everything: all sources, matched.csv, doc links and the full link (--no-link skips the link)
tools/verify.sh

# Only the full link (output in build/link/)
python3 tools/link.py
```

On Linux without Docker, `bash tools/setup_all.sh "<ISO>"` does the setup natively ([SETUP.md](SETUP.md)).
`tools/doctor.sh` diagnoses a broken environment.

## Repository map

| Path | Contents |
|---|---|
| `src/game/` | the game code, one file per original source file (translation unit), functions in their original order. Grouped by layer, lowest first: `platform`, `core`, `splice`, `object`, `world`, `engine`, `level`, `actor`, `gameplay`, `ui`, `app` ([docs/architecture.md](docs/architecture.md)) |
| `src/sdk/` | Sony SDK and runtime libraries (libkernel, libc, libm, libgraph, libcdvd, 989snd, ...); each file names its compiler in a header comment |
| `src/generated/` | generated wrappers for the Splice scripting runtime |
| `include/sly2/` | shared headers for the game's types (classes, structs, field layouts) |
| `config/` | every function's address and size (`functions.csv`), names (`symbol_addrs.txt`), the verified match list (`matched.csv`), dead-code scraps between functions (`junk.csv`) |
| `tools/` | matching, verification, link, rename and setup tools; `tools/layout/units.csv` maps functions to translation units |
| `docs/` | engine overview, system guides, file formats, contributor guides |

## Where to go next

- [docs/README.md](docs/README.md): the map of all documentation.
- [docs/engine.md](docs/engine.md): how the engine works, with a guide per subsystem in
  [docs/systems/](docs/README.md#systems-guides) (objects, world and levels, player, AI, camera, rendering, sound,
  Splice scripting, saves, UI, ...).
- [docs/guides/naming-workflow.md](docs/guides/naming-workflow.md): naming and comment conventions, and how to rename
  safely.
- [docs/decisions.md](docs/decisions.md): the design decisions and compromises the matching code depends on.
- [docs/guides/port-plan.md](docs/guides/port-plan.md): the plan for a native port.
- [docs/MATCHING_NOTES.md](docs/MATCHING_NOTES.md): compiler facts and matching techniques.

## Contributing

- **Every change must keep the match.** Renames, comments and rewrites are only accepted when every function and data
  symbol still compiles to the same bytes. Check the files you touch with `tools/matchtool.py check`.
- **Rename functions and globals with `tools/rename.py`**, never by hand: names are linker symbols, and the tool
  updates config and every caller and re-verifies the objects. Struct fields go through `tools/fieldrename.py`.
- **Run `tools/verify.sh` before pushing** a branch; it must report every check passing.
- Follow the naming and comment conventions in [docs/guides/naming-workflow.md](docs/guides/naming-workflow.md).

## Legal

This is an unofficial fan project. It is not affiliated with, endorsed by or connected to Sony Interactive
Entertainment or Sucker Punch Productions. *Sly 2: Band of Thieves* and related names are their trademarks. The
repository contains no game assets, game code or copyrighted binaries: to build or check anything you need your own
legally obtained copy of the game.

**License:** intended MIT. The upstream decompilation this fork builds on has no license file; we're confirming with its
contributors before adding one.
