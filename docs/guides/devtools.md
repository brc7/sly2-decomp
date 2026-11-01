# Developer tools (devtools)

Status as of 2026-11-01. The devtools are **process tooling**: throwaway, vibe-coded viewers and editors built so that a human can check our claims
by eye or by experiment. They are not product code and are not in the decomp repo; on the Mac they live under
`~/Documents/code/src/sly2/devtools/<tool>/`. Each tool has a `README.txt` with exact commands, inputs, outputs, what was verified and known gaps.
Everything under a tool's `out/` and `screens/` is **game-derived**: keep it out of git.

Common conventions:
- Inputs: the extracted disc (`research/assets/out/`: decompressed `worlds/*.brx`, `sound/`, `misc/`, `manifest.json`), the retail ELF
  (`disc/SCUS_973.16`, for option tables and code scans) and PCSX2 save-state RAM (`research/savestates/`). python3 + numpy only.
- Every claim a tool shows carries a tag (confirmed with how, or hypothesis) and its source, and most tools have **right / wrong / unsure** buttons
  that export a verdicts JSON to hand back. Those verdicts are how human review feeds into these docs.
- Shared overlay format for 3D layers (`layers/<level>/<layer>.json`, kinds lines/points/mesh/boxes, viewer coords = game (x, z, -y) x 0.01) lets
  level_viewer show other tools' output.
- Tags in these docs: a finding a tool reports is written `[C:<kind> <tool>]`. Where v3 re-checked a finding independently against the ELF
  disassembly or the save-state RAM, the tag says `v3` (e.g. `[C:static v3 disassembly]`, `[C:runtime v3]`). `[C:live]` = confirmed in the running
  game over PINE ([live-pine](live-pine.md)).

## Tools at a glance

| Tool | What it is for | How it verifies claims |
|---|---|---|
| [code_explorer](#code_explorer) | browse every function: address, TU, names and tiers, match status, C/asm, callers/callees, docs that mention it | disassembler cross-checked with llvm-objdump; caller counts vs raw `jal` sites; verdict buttons |
| [symbols_export](#symbols_export) | names for the PCSX2 debugger, Ghidra, IDA, Binary Ninja; `addr2name.py` | validate.py: uniqueness, ranges, 160 random functions read in disassembly; CLASSINFO getters; vtable words in RAM |
| [level_viewer](#level_viewer) | fly through any of 43 levels, click any object, see fields, scripts, class info; compare with in-game screenshots | game cameras from save states vs screenshots; XFM placement vs RAM (787/842 in S02) |
| [entity_viewer](#entity_viewer) | one card per entity type (guard species, players, pickups, bosses): model, skeleton, clips, claims | each claim quoted from its source file; attack sets vs runtime; models vs known characters |
| [anim_lab](#anim_lab) | play the game's own animation clips on its skeleton and skinned mesh, with events and hit windows | FK/IK vs RAM world matrices (S04/S05/S68); clip events vs runtime7b |
| [texture_browser](#texture_browser) | every texture x CLUT pair, variants, users, dedupe stats, suspicious-decode list | counts vs whole_game; decoder parity; heuristics with negative controls |
| [audio_browser](#audio_browser) | play and cross-reference every sound (dialogue, music, ambience, SFX, movie audio) | signal self-tests with negative controls; g50 vs file sizes; references from objects, scripts and code |
| [splice_browser](#splice_browser) | every Splice script, RELAY cutscene and JOB/TASK graph, decompiled with text ids resolved | byte-identical round trip; listings parity; RELAY decode vs live S59; graph vs runtime6 |
| [save_editor](#save_editor) | decode the save blob from any RAM dump; progression view; var <-> event map; PINE edit jobs; card files | 73 states, schema hash; events in info.txt; preset results re-decoded |
| [ram_inspector](#ram_inspector) | one HTML report per save state (objects, fields, BHV stacks, save vars) and diffs between states | 18 checks against published runtime findings; input-format parity tests |
| [pine_toolkit](#pine_toolkit) | drive the live game over PINE: dashboard, recipes (write -> observe), warp, unlock, prepare jobs | mock PINE server serving real RAM; then live jobs ([live-pine](live-pine.md)) |
| [collision_paths](#collision_paths) | collision, navmesh, path, grab and volume layers per level (overlays for level_viewer) | all records parse to cb; placement vs RAM in 3 states; lasers 52/52 vs RAM; overlays vs game-camera renders |
| render_lab | re-render a captured frame with the documented relight model | still in progress (no README); out/S02/relight_check.json |

## code_explorer

Static HTML explorer of the decomp: every function with address, size, TU, current and proposed name (tier, confidence, purpose, evidence from
master_names_v3), byte-match status, C source or asm, callers/callees, the docs that mention it, a "least certain areas" heatmap, class hierarchy
with vtable slots and overrides. **Verification**: its disassembler output was checked against llvm-objdump; caller counts against raw `jal` sites
(PpoHero 0x1C4840: 59 sites -> 50 callers). Use it to check any name or purpose in these docs.

## symbols_export

One generator for every debugger format (PCSX2 `.sym`, Ghidra / IDA / Binary Ninja scripts, `symbols.json`, `sly2_types.h`) with the tier in every name
(T1 `Name`, T2 `Name?` / `T2_Name`, T3 `Name??`). `addr2name.py` resolves addresses, including struct fields (`g_sw.aslCid[0].head`).
**Verification**: validate.py (unique names, all functions inside .text without overlap, sscanf-compatible `.sym`, 1,528 static asserts on struct
layouts, 160 random functions read in disassembly). Findings folded into these docs (re-checked in v3): **CLASSINFO stride 0x14** (getters at 0x113A10 +
0x10*cid return `g_aclassinfo + 0x14*cid`) and **g_sw / g_cm are objects in place** (vtable words at +0xC) - see
[object-model](../engine/object-model.md#devtools-findings-v3-2026-11-01). Debugger/disassembler scripts have only run against mock APIs `[H]`.

## level_viewer

WebGL viewer for all 43 world files: groups (guards, coins, clue bottles, lasers, lights, triggers, paths...), per-object inspector with parsed fields,
option names and the attached Splice script, game cameras from save states with side-by-side screenshots. **Verification**: from the S02/S04 game camera the
render lines up with the PCSX2 screenshot; the XFM placement rule is checked against RAM (matWorld at XFM+0xD0; 787/842 live XFMs in S02 within 2 units).
Placement rules for every record class: [meshes-scene](../formats/meshes-scene.md#devtools-findings-v3-2026-11-01).

## entity_viewer

Gallery of 158 entity types (one per world-entry template): textured bind-pose model, XF skeleton, ASEG clip list, and a claims panel where each claim
is quoted from its source (file:line) and tagged. **Verification**: claims are located by quoted needles so a doc edit that removes one is reported;
the per-species CID 0x78 attack entries that pass the level load filter equal the attack oids seen at runtime (rhino, goat, cobra, rat, boar) - see
[guards-ai](../engine/guards-ai.md#devtools-findings-v3-2026-11-01).

## anim_lab

Offline three.js player for character clips: skinned mesh + skeleton, IK, event markers, SOPART hit windows, playback speed from clip option 109.
**Verification**: FK from RAM locals reproduces RAM world matrices (S68, 0.005 units); IK reproduces Sly's 4 chains to 1e-4; skinned Sly posed from S04 RAM
overlays his silhouette in the S04 screenshot. Format notes: devtools/anim_lab/ANIM_FORMAT.txt; summary in
[animation](../engine/animation.md#devtools-findings-v3-2026-11-01).

## texture_browser

Every bitmap x CLUT pair of every level with variants, users, shaders, duplicates, an alpha view and a suspicious-decode list. **Verification**: counts equal
whole_game (37,508 bitmaps, 71,774 pairs); vectorised decode pixel-identical to the reference decoder on 2,103 pairs; noise/stride heuristics measured
against garbage and mis-strided controls. Findings (no stored mips; 10.7x dedupe): [textures](../formats/textures.md#devtools-findings-v3-2026-11-01).

## audio_browser

One page per disc: dialogue (VAGp), music (VPK), ambience, SFX (989snd banks), movie audio, each with waveform, loop region, metadata and the objects,
scripts and code call sites that reference the sound. **Verification**: 23 signal checks, each with a negative control (misaligned or mis-interleaved decodes
must fail); g50 vs FILEREC size for 7,970 stream records. Findings (g50 = seconds, SFX ids 0-146 global, CID43 emitter option 99, music ids without
files, odd-rate tones): [sound](../engine/sound.md#devtools-findings-v3-2026-11-01), [audio format](../formats/audio.md#devtools-findings-v3-2026-11-01).

## splice_browser

Searchable browser of all 10,930 Splice entries with builtins, oids, options, text ids resolved to English strings, RELAY cutscenes as timelines and
JOB/TASK graphs per episode. **Verification**: byte-identical decode/re-encode; 10,900/10,930 listings identical to whole_game after name normalisation;
RELAY 0x6E3 decodes to the same 21 commands as the live list in S59; the m1_sat graph reproduces runtime6's chain. Finding folded in: TASK +0x5C (eopid
1265) is the **objective text**, not music ([game-progression](../engine/game-progression.md)).

## save_editor

Decodes g_gs (0x3D4A60, 0x5400 bytes + 8-byte schema fingerprint, 4,472 vars) from any RAM dump; progression per episode; var <-> event table
(out/var_meanings.csv, 47 curated rows with tags); presets and single-var edits as PINE jobs; memory-card save files. **Verification**: schema identical in
all 73 states; every non-zero blob byte covered by a variable; meanings confirmed against info.txt events (coins, loot, clues, HP, character, ThiefNet).
Findings: card file = 0x5408 bytes, **no checksum** ([save-memcard](../engine/save-memcard.md)); afClue per episode in the hub level; vault flag levels;
episode menu can unlock down to 1 ([game-progression](../engine/game-progression.md#devtools-findings-v3-2026-11-01)). Presets are `[H]` until run live.

## ram_inspector

CLI + HTML reports for a save state or a PINE dump: object tree, per-class decoders with confidence tags, BHV stacks, save vars, and diffs between states.
**Verification**: 18/18 checks against published runtime findings (S02, S64-S73 and two diffs); .p2s (zstd/deflate/stored), raw and PINE partial dumps give
identical models. Use it to re-check any `[C:runtime]` claim.

## pine_toolkit

Live control of PCSX2 over PINE: dashboard (level, clock, player, guards, tasks), 14 verification recipes (write -> observe -> verdict -> restore),
warp / reload / unlock / save vars / gadgets / prepare job. Works directly on the socket or through the agent file queue (`pine_jobs`). **Verification**:
17 unit tests and the 5 TESTPLAN jobs against a mock PINE server serving real save-state RAM (proves addresses and encodings, not game reactions),
then the live session of 2026-11-01. See [live-pine](live-pine.md).

## collision_paths

Writes the data the renderer never shows (collision hulls, navmeshes, paths, pipes/rails/wall grabs, volumes, warps, spawns, lasers from RAM) as overlay
layers, plus its own overlay.html alignment checker with verdict buttons; format note FORMAT.txt. **Verification**: all 121,153 records parse to cb;
placement vs RAM S02 1637/1692, S63 609/620, S65 1303/1404 (misses = moving objects); hulls hug the geometry from exact game cameras; laser lengths
52/52. Finding: PATHZONE points are world-space ([physics-collision](../engine/physics-collision.md#devtools-findings-v3-2026-11-01)).

## Modding pipeline (not a devtool, related)

modding/MODDING.txt: level writer, LZSS compressor and disc patcher, verified offline 43/43. First live test: SFX lost and the car did not move -
open issue in [open-questions](../open-questions.md#modding-live-test-2026-11-01).
