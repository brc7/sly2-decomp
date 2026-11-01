# Sly 2 engine documentation (first draft)

This tree documents how the Sly 2: Band of Thieves engine (NTSC-U `SCUS_973.16`) works, as recovered from the
matching decompilation in this repo. It is written for two readers:

- an engineer who knows software well but is new to the PS2 and to decompilation;
- agents (human or LLM) who need to find their way around ~6,600 functions, most of which still have
  address-based names (`func_XXXXXXXX`).

> **Status: draft for review.** Assembled on 2026-10-31 from ~100 per-TU write-ups, verifier passes, a
> slot-22/23 arbitration and a save-state runtime analysis. Most names in here are **proposals** that are not yet
> in `config/symbol_addrs.txt`. Read the confidence tags.

## Index

| Page | What it covers |
|---|---|
| [engine/overview.md](engine/overview.md) | **Start here.** Boot, the frame loop, the level lifecycle, the object model, the subsystem map |
| [engine/main-loop-startup.md](engine/main-loop-startup.md) | `main`, the startup table, the frame loop in order, the clock, pad input, cheats, rumble |
| [engine/object-model.md](engine/object-model.md) | BASIC/LO/XFM, class descriptors and vtables, messages vs Splice events, OnAdd/OnRemove activation, proxies, lookup |
| [engine/world-sw.md](engine/world-sw.md) | The world object `g_sw`, level loading (BRX), transitions, hands-off, deferred callbacks, checkpoints |
| [engine/splice-scripting.md](engine/splice-scripting.md) | The Splice (Scheme-like) script runtime, builtin table, object options, events |
| [engine/rendering.md](engine/rendering.md) | Render passes, models (GLOBSET), DMA/VIF/GIF, GS memory, the render thread, textures |
| [engine/camera.md](engine/camera.md) | The camera `g_cm`, the policy stack, camera policies, tunnels |
| [engine/player.md](engine/player.md) | PO, STEP, STEPPLAYER and its components, Sly (JT), Bentley, Murray, gadgets |
| [engine/guards-ai.md](engine/guards-ai.md) | STEPGUARD, the behaviour (BHV) stack, perception, pickpocketing, guard scripts and spawners |
| [engine/game-progression.md](engine/game-progression.md) | Jobs and tasks, the game event bus, difficulty, collectibles, booty |
| [engine/save-memcard.md](engine/save-memcard.md) | The save blob `g_gs`, the save-variable schema, the memory-card worker |
| [engine/sound.md](engine/sound.md) | The sound manager, sound instances, 989snd, the SUKR streaming module |
| [engine/hud-ui.md](engine/hud-ui.md) | WIDGET HUD, text table, screens and menus, fonts, loading screen |
| [engine/particles-effects.md](engine/particles-effects.md) | Emitters, RIP particles, BLIPs, trails |
| [engine/physics-collision.md](engine/physics-collision.md) | SO physics, broadphase, contacts, the solver, zaps, navigation (path), volumes |
| [engine/animation.md](engine/animation.md) | ASEG/ASEGA, blends, actuators, XF slots, state machines (SM/SMA), IK, gait |
| [engine/cutscenes.md](engine/cutscenes.md) | RELAY/JSG scripted sequences, FMV playback, screen transitions |
| [engine/core-library.md](engine/core-library.md) | Heaps, slot heaps, lists, math, the binary input stream, CD, threads, text |
| [engine/gameplay-objects.md](engine/gameplay-objects.md) | Smaller gameplay classes (buttons, barriers, bombs, turrets, RC car, ...) |
| [formats/disc-records.md](formats/disc-records.md) | How files are found on disc (64-byte FILERECs, no TOC) |
| [formats/compression-lzss.md](formats/compression-lzss.md) | The whole-file LZSS compression |
| [formats/world-file.md](formats/world-file.md) | The world (level) file format |
| [reference/class-tree.md](reference/class-tree.md) | All 199 classes (CIDs), sizes, vtables, identifications |
| [reference/tu-map.md](reference/tu-map.md) | Address range -> source file (translation unit) |
| [reference/functions/](reference/functions/) | Generated per-subsystem function lists (every proposed name) |
| [reference/glossary.md](reference/glossary.md) | Sucker Punch naming conventions and abbreviations |
| [reference/save-variables.md](reference/save-variables.md) | Named save variables and their addresses |
| [open-questions.md](open-questions.md) | Open questions and suggested experiments, by subsystem |

## How knowledge is marked

Every claim carries one tag. The tag says how we know, not how important the claim is.

| Tag | Meaning | Typical evidence |
|---|---|---|
| `[C:runtime]` | **Confirmed by observation** in PCSX2 save-state RAM (states S02-S05, Paris hub) | `runtime/FINDINGS.txt`, `runtime/verdicts.csv` |
| `[C:verified]` | **Confirmed by an independent verifier** who re-derived it from the code without reading the proposer's evidence | `verify/verdicts_*.csv`, the slot-22/23 arbitration |
| `[C:static]` | **Read directly from matched code or ELF data** and structurally certain: a vtable slot position, a data table decoded from the ELF, a call order in `main`, a matched hand file that names itself | the source file, ELF tables |
| `[H]` | **Hypothesis.** Inferred from code shape, Sly 1 similarity, callers or link order. The tag names its source write-up, e.g. `[H r4 lo.md]` | a per-TU write-up |
| `REFUTED` | A claim from an earlier draft that a later check contradicted. Kept so nobody re-introduces it | |

Notes on the tags:
- **"Matches" does not mean "understood".** Every function in this repo compiles to the original bytes, so the
  *code* is certain. A *name* or *purpose* is still a hypothesis until a verifier, a runtime check or a structural
  fact (vtable slot, data table) backs it.
- **Calibration of proposer confidence** (from four verifier rounds of 80 rows each): proposals at >= 0.8 were
  97-100% correct; 0.5-0.8 were 80-100% correct (errors were mostly class-identity slips); below 0.5 the
  *purpose line* was right about 85% of the time but the *name* is a draft. Sly 1-alignment proposals
  (`~names` in early tooling) were badly shifted in places and are not used here unless re-checked.
- **Class names**: names like `CID47` or `BHV171` are placeholders for classes whose Sucker Punch name is unknown.
  CID numbers are written in hex with `0x` (`CID 0x47`) or as `CIDnn` in decimal, exactly as `reference/class-tree.md` lists them.
- **Provenance labels** such as `r4 lo.md` or `r7b stepguard.md` refer to the per-TU research write-ups
  (`reports/tu_naming/<round>/tu_docs/<file>.md`) that this draft merges. They are not in the repo; the facts that
  matter were copied here.

## Conventions used in the pages

- Addresses are EE virtual addresses (`0x1A7518`). `D_XXXXXXXX` is a global data symbol at that address.
- Offsets are written `+0x18` (bytes from the start of the object).
- `vtN` / "slot N" = C++ virtual slot N. gcc 2.95 vtable entries are 8 bytes `{s16 delta; s16 pad; void *pfn}`;
  slot 0 is a null header, so slot N's function pointer is at `vtable + 8*N + 4` `[C:runtime]`.
- Function tables link each address to its source file (`src/hand/...` or `src/auto/...`).

## How to contribute

1. **Pick an open question** from [open-questions.md](open-questions.md) or a low-confidence row in
   `reference/functions/`. Most have a suggested PCSX2 experiment.
2. **Work one TU at a time.** Fix the TU boundary first (static-init stubs end every TU), name the entry points,
   compare with the Sly 1 counterpart file by behaviour, then name the rest. The method is in
   `docs/MATCHING_NOTES.md` and in the playbook summarised below.
3. **Tag what you add.** Use the tags above. Write down the experiment that would confirm a hypothesis.
4. **Promote names carefully.** Only names at high confidence (two independent signals agree), or confirmed by an
   experiment, go into `config/symbol_addrs.txt`. Renames never change the match.
5. **When a name turns out wrong, check its neighbours.** Alignment errors come in shifted runs.
6. **Regenerate the generated pages** (`reference/tu-map.md`, `reference/functions/*`, and the function tables
   inside `engine/*.md`) with the generator script rather than editing them by hand.

Playbook in brief (from the semantic-naming audit): the Sly 2 body and its matcher comment decide ~60% of cases;
then name the callees; then compare with the Sly 1 candidate **and its two neighbours**; check the signature;
use constants and ELF data tables; use call-site arguments. Function size and alignment confidence had no
predictive value.

### Runtime analysis toolkit

Save-state RAM tools live with the research reports (`reports/runtime/scripts/`, Python 3 stdlib, run with `python3 -I`):

| Script | Use |
|---|---|
| `slyram.py` | Library + CLI. Typed reads of a 32 MB EE RAM dump (`eeMemory.bin` from a `.p2s`), class resolution through vptr +0xC and vtable slot 6, walks the per-class lists (`g_sw+0x3038`), the oid hash (`g_sw+0x38`), the root DL (`g_sw+0x10`) and SO child DLs (+0x64) |
| `savevars.py` | Decodes the save-variable schema (`g_sw+0x3A18`) and prints every named variable of `g_gs` with its value per state |
| `probe.py` | One-shot state summary: level, Sly, guards with behaviour stacks and flag bits, camera policy stack, progression, clue HUD, save-var diff |
| `make_inventory.py` | Regenerates the object inventory and per-class counts for every state in a directory |
| `sgf_map.py` | Static (ELF) map from guard flag number to the BHV class that raises it |

Key runtime-verified addresses: `g_sw` 0x3DD490, `g_cm` 0x2DDF40, `g_clock` 0x2DDED0, `g_game` 0x2DEB30,
`g_pjt` 0x2E1E40, `g_gs` 0x3D4A60, class descriptors 0x2BAAE8, guard list 0x3E1444, level index 0x3E1110,
`g_cframe` 0x2BE538 `[C:runtime]`.
