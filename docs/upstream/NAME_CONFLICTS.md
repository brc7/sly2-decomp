# Name conflicts between upstream `readable` and `readable-brc7`

For the upstream maintainer (Komasia) and his agents. Two cleanup efforts named the same functions independently:
upstream's `readable` branch (908f3f33d) and this fork's `readable-brc7`. Both start from upstream main be803ab88,
and both keep every function byte-identical. This page compares the two naming styles, gives the size of the overlap
per subsystem, and sets out three ways to reconcile them. Each one can be applied mechanically with
`tools/rename.py` on this tree. The full address-keyed table is [name_crosswalk.csv](name_crosswalk.csv). For the
rest of what differs between the trees, see [ONBOARDING.md](ONBOARDING.md).

## The two naming philosophies

**Upstream readable: original names.** Upstream keeps the Sucker Punch names used by the Sly 1 decomp and the
SafeHouse port (credited in upstream's CREDITS.md; names taken from SafeHouse are marked `// name: SafeHouse port`).
The style is Hungarian: the prefix gives the return type (`F` bool, `N` int, `Pv` pointer, `Rad` angle, `Clq`/`Lm`
for the curve and limit types), followed by terse type abbreviations. Those names carry over between Sly 1, Sly 2 and
SafeHouse, so one search finds the same function in all three. Upstream readable also gives many functions C++
signatures, so 2,193 of its names in conflict are mangled.

**readable-brc7: descriptive names.** This fork follows [naming-workflow.md](../guides/naming-workflow.md): functions
are PascalCase verb phrases, predicates start with `Is`/`Has`/`Can`/`Should`, and the Hungarian prefixes go. Sly 1
names count as evidence about what a function means, not as a style to keep. Type names (`LM`, `TARGETREC`) and
signatures stay as the matching code needs them. Script bindings follow the `RefThunk_<native>` convention.

| address | upstream readable | readable-brc7 |
|---|---|---|
| 0x11D910 | `FCheckLm__FP2LMf` | `IsWithinLimits__FP2LMf` |
| 0x11D3F8 | `NRandInRange__Fii` | `RandomIntInRange__Fii` |
| 0x116AF8 | `PvRealloc__11CLinearHeapPvii` | `Realloc__11CLinearHeapPvii` |
| 0x20A690 | `RadTargetrec__FP9TARGETREC` | `GetTargetRecordFacing` |
| 0x144418 | `FDamageFatal__FP5ACTORP5CTMSG` | `IsStepplayerZapLethal` |
| 0x20B3D0 | `NGuardStatus__FP5GUARD` | `GetStepguardHealthState` |
| 0x1AE1D0 | `PvAllocSwImpl__Fi` | `AllocFromWorldHeap` |
| 0x1014E8 | `RefXfHandle__FP2LOiPv` | `RefThunk_NewSoXfHandle` |

## How much overlaps

Functions (6,662, keyed by address; "ours" is this branch after the work described below):

| status | count | meaning |
|---|---|---|
| conflict | 2,063 | both sides named it, with different base names |
| signature-only | 167 | same base name, different symbol (upstream changed the parameter types or the linkage) |
| same | 694 | identical symbol |
| ours-only | 3,123 | named here, still `func_XXXXXXXX` upstream |
| ported, ported-converted | 21, 9 | named only upstream; this branch took the name, or converted it to our style (below) |
| not-ported | 1 | 0x238CC8: upstream calls it `sceSifFreeIopHeap`, a name this branch already uses at 0x238D40 |
| both-placeholder | 584 | neither side has a name (249 are generated splice bindings in class_registry.cpp) |

Upstream's 90 globals: 62 now carry upstream's name on this branch too (here they were only `D_XXXXXXXX` linker
symbols), 3 were already the same, and 1 kept our name (`g_pdliFirst`; upstream has `s_pdliFirst`). In 23 conflicts
upstream gives a vtable a label (`g_vtexplo`) where this branch uses g++'s real symbol (`_vt$5EXPLO`), and one was
not ported (`g_clqZero`, which nothing references).

Conflicts per subsystem (directory under src/game, or src/sdk library; columns are function counts):

| subsystem | conflict | signature-only | same | ours-only | upstream-only | both-placeholder | total |
|---|---|---|---|---|---|---|---|
| gameplay/ai | 355 | 12 | 1 | 253 | 0 | 18 | 639 |
| gameplay/player | 306 | 22 | 0 | 276 | 0 | 34 | 638 |
| engine/physics | 228 | 14 | 3 | 187 | 0 | 15 | 447 |
| gameplay/objects | 181 | 23 | 6 | 182 | 1 | 21 | 414 |
| engine/anim | 144 | 11 | 14 | 82 | 0 | 12 | 263 |
| app | 108 | 0 | 5 | 722 | 8 | 249 | 1092 |
| engine/sound | 69 | 1 | 0 | 47 | 0 | 4 | 121 |
| engine/camera | 66 | 12 | 9 | 57 | 0 | 5 | 149 |
| world/state | 57 | 8 | 0 | 67 | 0 | 3 | 135 |
| engine/presentation | 57 | 1 | 5 | 173 | 0 | 5 | 241 |
| core/math | 56 | 4 | 18 | 27 | 1 | 7 | 113 |
| splice | 52 | 0 | 9 | 83 | 0 | 12 | 156 |
| object | 50 | 9 | 4 | 145 | 0 | 9 | 217 |
| gameplay/progression | 49 | 6 | 0 | 50 | 0 | 4 | 109 |
| engine/graphics | 48 | 17 | 86 | 141 | 0 | 16 | 308 |
| actor | 39 | 7 | 0 | 34 | 0 | 2 | 82 |
| ui | 38 | 1 | 6 | 83 | 0 | 5 | 133 |
| gameplay/cutscene | 33 | 0 | 2 | 148 | 0 | 3 | 186 |
| engine/fx | 31 | 13 | 28 | 77 | 1 | 4 | 154 |
| world/scene | 25 | 1 | 0 | 45 | 0 | 3 | 74 |
| level | 13 | 2 | 1 | 21 | 0 | 6 | 43 |
| platform (memory, runtime, input, cdvd) | 27 | 1 | 54 | 33 | 0 | 14 | 129 |
| core (binstream, containers, time, locale) | 17 | 1 | 39 | 68 | 0 | 13 | 138 |
| engine/font | 8 | 1 | 8 | 9 | 0 | 2 | 28 |
| src/sdk (all libraries) | 6 | 0 | 390 | 112 | 20 | 117 | 645 |

The "upstream-only" column counts ported, ported-converted and not-ported together. Almost 60% of the conflicts
are in gameplay (AI, player, objects), physics and animation. In those subsystems upstream adopted SafeHouse names in
its R6 batches, while this branch had named the same functions in its own rounds. SDK names barely conflict, because
both sides use Sony's names.

## The crosswalk file

[name_crosswalk.csv](name_crosswalk.csv) has one row per function (6,662) and per upstream global (90), with the
columns `address,kind,upstream_base,upstream_readable,ours,status`:
- `upstream_base` is the name on upstream main be803ab88, the common ancestor.
- `upstream_readable` and `ours` are the linker symbols on the two branches.
- `status` is one of the values in the tables above.

The rows come from joining the three `config/matched.csv` files on `addr`. For globals they come from upstream
readable's `config/symbol_addrs.txt` and this branch's linked ELF. To refresh the file after more renames on either
side, redo that join.

## What readable-brc7 already did

- **Ported upstream-only names.** These are addresses that only upstream had named, renamed on this tree with
  `tools/rename.py`:
  - 19 libkernel syscall stubs (`CreateThread`, `StartThread`, `DeleteSema`, `sceSifSetDma`, ...; each checked
    against its syscall number);
  - `ClipSegmentBsp` and `MarkWheel`;
  - 63 globals (`g_sw`, `g_cm`, `g_game`, `g_clock`, `g_mpbifkbif`, the camera-jolt statics, the 989snd command
    buffer, ...).
- **Converted to our style.** Nine upstream names were converted to our convention, and upstream's name is kept as
  the alias:
  - `RefSoeHandle`, `RefSoundMethod` and `RefXfhLook` became `RefThunk_NewSoPartHandle`, `RefThunk_PauseSnd` and
    `RefThunk_SetXfHandleActuator4`;
  - six emitter accessor templates became `ResolveEmitterRefs`, `EnsureEmitterRecord0`/`EnsureEmitterRecord1`,
    `SetEmitterOriginKind` and `SetEmitterRecord0PayloadKind`/`SetEmitterRecord1PayloadKind` (upstream: `ResolveEmits`, `OffKind0`/`OffKind1`,
    `SetPositionKind`, `SetKind0Payload`/`SetKind1Payload`).
- **Aliases.** All 2,240 functions whose upstream name differs from ours now have this line in their doc comment:
  "Also known as: <upstream base name> (upstream readable / Sly 1 name[; <upstream symbol>])". A search for either
  name finds the definition. The symbol is included when its signature part differs from ours. 20 of the upstream
  names are Ghidra `FUN_xxxxxxxx` labels, kept because they point at the SafeHouse/Sly 1 function.
- **Not changed.** Every other conflict keeps our name (the maintainer's decision for this branch).
- **One suspected wrong name.** Our `s1_GetThreadId` at 0x2348F0 is syscall 45, which is `ReleaseWaitThread`, as
  upstream has it.

## Three strategies

Every strategy below is a batch of `tools/rename.py` runs on this tree, taking rows of the crosswalk as input.
`rename.py --csv FILE` takes rows of `address,new_name`. For a C++ function it changes only the base name and keeps
this tree's signature. It rebuilds every file it touches and undoes any rename that changes a file's match or symbol
table. Three cases need a token-exact edit by hand:
- function templates and members whose base name is shared, which the tool refuses;
- splice bindings defined in `src/generated/splicewrap/*.inc` (rename the `.inc` file with the function);
- the `#line` pin path in robottank.cpp, which must keep its old file name (path-hash quirk in
  [tu-merging.md](../guides/tu-merging.md#path-hash-quirk-asm-functions)).

After each batch, run the gates from [ONBOARDING.md](ONBOARDING.md#verifying-a-change).
`tools/cleanup/docnames.py` then lists every doc that cites an old name.

**A. Keep this branch's names and keep the aliases (the current state).**
- Trade-offs:
  - Nothing to rename.
  - Code reads as English, and the docs, systems guides and cookbook already use these names.
  - Upstream names stay searchable through the aliases and the crosswalk.
  - Contributors who know Sly 1 or SafeHouse have to learn a second vocabulary.
  - New upstream names arriving later need the same alias pass.
- Mechanics:
  - Nothing to do now.
  - For future upstream names, port the upstream-only ones (status `upstream-only` → `rename.py --csv`) and rerun
    the alias pass (comment-only, so `check.sh` must show 0 differences).

**B. Adopt the original names wherever upstream has one.**
- Trade-offs:
  - One vocabulary shared with Sly 1, SafeHouse and upstream.
  - The Hungarian prefixes go against naming-workflow.md, which would have to change.
  - Around 2,230 renames, then doc updates: the systems guides, cookbook and engine.md cite our names, and
    docnames.py flags each one.
  - The 167 signature-only rows keep our signature: matching upstream's mangled name there means changing the
    parameter types, which is not a rename.
- Mechanics:
  1. Take the rows with status `conflict` or `signature-only`. Write `address,<base of upstream_readable>` to a CSV.
  2. Run `rename.py --csv` one subsystem at a time, so each batch rebuilds fewer files.
  3. Flip the alias lines to name ours instead ("Also known as: IsWithinLimits ...").
  4. Update the docs from docnames.py's list.
- Effort: expect several hours of recompiles on a 2-core machine.

**C. Mix per subsystem.**
- Trade-offs:
  - Original names where code is shared with Sly 1 and SafeHouse: the engine core, platform, splice VM and math
    (core/, platform/, splice/, object/, engine/graphics: about 200 conflicts).
  - Descriptive names where Sly 2 code is new and most read by modders: gameplay: AI, player, objects (about
    850 conflicts).
  - Smaller churn than B, and each name follows the style of the code around it.
  - The cost is two styles in one tree, and a boundary that has to be written down and kept.
- Mechanics:
  1. Filter the crosswalk rows by our source path (the `source` column of `config/matched.csv` for the address).
  2. Feed `rename.py --csv` as in B for the chosen subsystems.
  3. Record the split in naming-workflow.md.
  4. Keep the aliases in the descriptive subsystems.
