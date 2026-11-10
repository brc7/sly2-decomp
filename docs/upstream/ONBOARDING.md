# From upstream main to readable-brc7

A delta guide for people and agents who know upstream main (be803ab88, or the `readable` branch built on it) and
want to work on `readable-brc7`. Both trees match the same ELF byte for byte. This page lists what changed and links
to the docs that explain each part. For the name differences, see [NAME_CONFLICTS.md](NAME_CONFLICTS.md) and
[name_crosswalk.csv](name_crosswalk.csv).

## What changed

| area | upstream main | readable-brc7 | read |
|---|---|---|---|
| layout | one file per function: `src/auto`, `src/hand`, `src/lib` | one file per original translation unit: `src/game/<layer>/<subsystem>/<unit>.cpp` (211 files); the SDK is per function under `src/sdk/<lib>/`; generated splice bindings are `src/generated/splicewrap/*.inc`, included by class_registry.cpp | [architecture.md](../architecture.md), [tu-merging.md](../guides/tu-merging.md) |
| headers | a few shared headers, plus a private struct "view" per function | shared types with their real layouts in `include/sly2/*.h`, with offset static-asserts; registry classes in `sly2/classes.h`; most per-function views replaced by the shared type | [decisions.md](../decisions.md) section 1, tu-merging.md "Views unified" |
| islands | none | where no natural spelling matches, a small documented form kept only to reproduce codegen (`<Type>Island` views, addend labels) | decisions.md 1.4, tu-merging.md "Matching-only islands" |
| virtual calls | manual vtable loads and calls | registry classes declare real virtual members, each with an asm label naming the function the ELF vtable holds; calls go through them; vptr stores use the macros in `sly2/vtables.h` | decisions.md 1.7, tu-merging.md "Emitting vtables (key functions)" |
| linkage | each function in its own object | symbols bound with asm labels; linkonce copies emitted by the TU that references them; static initializers named per TU | decisions.md section 2 |
| names | Sly 1 / Hungarian names where known, `func_XXXXXXXX` otherwise | descriptive names and a doc comment per function; upstream names as "Also known as:" lines | [naming-workflow.md](../guides/naming-workflow.md), NAME_CONFLICTS.md |
| verification | per-function matchtool, progress | matchtool on the whole TU, an object comparison against a baseline, a real full link of the ELF from source, and `tools/verify.sh` | below; [full-link.md](../guides/full-link.md) |

The tree has a quirk that per-function files never showed: in rare asm functions, GCC 2.95 hashes the source path
and line into the code it generates. Moving or merging a function can change its stack slots. When a moved function
misses by a few stack offsets, read tu-merging.md "Path-hash quirk (asm functions)" first.

## Where to start

1. [docs/README.md](../README.md) maps all the docs. [engine.md](../engine.md) is the engine overview and links to
   one systems guide per subsystem.
2. [cleanup-playbook.md](../guides/cleanup-playbook.md) explains how this tree was produced and why. The agent
   checklist is [skills/decomp-cleanup/SKILL.md](../../skills/decomp-cleanup/SKILL.md).
3. Before editing, read [decisions.md](../decisions.md), and [tu-merging.md](../guides/tu-merging.md) "Rules" and
   "Mismatch causes and fixes".

## Mapping per-function work onto this tree

Functions are keyed by address on both sides. To move a change from an upstream file such as
`src/hand/func_0012ABCD.c`:
1. Find the address in upstream's `config/matched.csv`. In this tree's `config/matched.csv`, the row with the same
   `addr` gives our symbol and, in the `source` column, the TU file that defines the function.
   `tools/layout/units.csv` maps the address to its unit too, and `config/functions.csv` gives the size.
2. Open that file and search for our name, or for upstream's name: it appears in an "Also known as:" line when the
   names differ. [name_crosswalk.csv](name_crosswalk.csv) gives both names for every address.
3. Port the change into the TU. Use the shared types from `include/sly2` rather than a private view; if you need a
   field the shared type lacks, add it at its offset with a static-assert. Renames of functions and globals always go
   through `tools/rename.py`, never by hand (naming-workflow.md, section 3).
4. Upstream changes that only rename something map to a `tools/rename.py` row (`address,new_name`). Changes to a
   signature need the declaration and every caller in this tree changed together.

## Verifying a change

Each check and what passing means:
1. `python3 tools/matchtool.py check <file>`: every function and data symbol of the TU reports MATCH. A TU is
   checked whole.
2. `tools/cleanup/check.sh BASE.json <changed files>`: the same matchtool run on every file that includes a changed
   header, plus a comparison against a baseline recorded at your base commit with `tools/cleanup/objcmp.py record`.
   The baseline covers nm symbols, sizes, sections and relocations by symbol, which matchtool cannot see.
   Expect 0 differences, apart from the renames you intended.
3. Before merging, run `tools/verify.sh`:
   - progress from a clean cache: 6662/6662, and `config/matched.csv` unchanged;
   - a full link with `tools/link.py`: every section 100%, 8120/8120 symbols at their config address;
   - `tools/linkcheck.py`.

   Then run `python3 tools/cleanup/docnames.py`.

[SETUP.md](../../SETUP.md) and [setup.md](../guides/setup.md) cover the toolchain. Some SDK sources build only with
the Linux compiler path (Docker, see verify.sh).
