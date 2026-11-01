# World (SW): level loading, transitions, hands-off, callbacks

> **What changed in v2.** Runtime round 5b caught a level load in progress (S56), the front-end episode menu (S55), the pause menu (S54)
> and the safehouse (S48); round 6 produced the first non-zero lock counters. The transition order below is corrected from the matched
> code and agrees with S56; the "checkpoint manager" section of the draft is REFUTED (0x20DF80 = `SetSwTransition`).

The world object and everything that happens when a level is loaded, run and left.
Related: [world file format](../formats/world-file.md), [object-model](object-model.md), [save-memcard](save-memcard.md).

## 1. SW, the world singleton

- `g_sw` = D_003DD490, class SW (CID 0x1, BASIC child, 0x4790 bytes, vtable 0x2DC018, 9 slots) `[C:runtime]`.
  Constructed by the static initializer of sw.cpp via `SW::SW` 0x20D9D0 and re-constructed in place on every level
  change `[C:runtime S56 + code]` (`ExecuteSwTransition` memsets 0x4790 bytes and calls `SW::SW`). SW is **not** an LO in Sly 2 (Sly 1: LO child) `[C:static]`.
- SW vtable slots 8 and 9 point at `RenderSw` 0x1CDA80 and `DrawSw` 0x157FA0 `[H r5 render, r3 draw.md]`; UpdateSw 0x226550 is called directly from main.

Selected SW fields (more in the pages that own them):

| Offset | Field | Tag |
|---|---|---|
| +0x10 | root DL of objects (ibDle 0x24) | [C:runtime] |
| +0x20 | busy (activation-candidate) list of LOROOTs, cursor +0x39A4 | [C:verified] |
| +0x2C | active-roots list (roots that are live) | [C:verified] |
| +0x38 / +0x1838 | oid hash (512) / second 512-bucket table | [C:runtime] |
| +0x3038 | 199 per-class SLs (12 bytes each) | [C:runtime] |
| +0x398C | LOROOT pool | [C:runtime] |
| +0x39D0..+0x39E8 | load scratch buffers, load list cursor (+0x39E4) | [H r2 brx.md] |
| +0x3A18 | save-variable schema | [C:runtime] |
| +0x3AFC..+0x3BC4 | progression slots (task count, current job/task, save-var keys, HUD widget records) | [C:runtime] partly; see [game-progression](game-progression.md) |
| +0x3BF0 / +0x3BF4 / +0x3C38 / +0x3C40 | transition request flag / requested TRANS (0x4C: obfuscated FILEREC 0x40 + oid + ...) / request oid (0x189) / current level's record | [C:runtime] |
| +0x3C90 / +0x3CA8 | deferred-callback pool / list | [H r2b callback.md] |
| +0x3D58 | a stamp counter (~145 increments per frame; XF/matrix stamps) — REFUTED as "frame stamp" | [C:runtime] |
| +0x3D60 | frame counter (bumped by RenderSw) | [C:runtime] |
| +0x3D78 / +0x3D7C / +0x3D80 / +0x3D84 | hands-off / **no-pause** / letterbox / input lock counters (section 5) | [C:runtime] |
| +0x3E00 | clear colour; +0x3E10 HSV, +0x3E20 midtone, +0x3E24 shadow (illumination) | [H r7 sw.md] |
| +0x3EB8 / +0x3EBC / +0x3EFC / +0x3F00 | PO count / PO array / current PO / override | [C:runtime] |
| +0x3F04 / +0x3F08 / +0x3F10 | world-entry (= PROXYSRC) count / array (0x6C each, name at +0x1C, aplo[] +4, count +8) / current index while loading | [C:runtime] |
| +0x3F14 / +0x3F18 / +0x3F20 | update table count / dead count / array of `{grf, plo}` | [C:runtime] |
| +0x455C.. / +0x45CC | cells and leaves / pathzone BSP | [H r2 brx.md, r5b path.md] |
| +0x4640 / +0x4644 | localized strings | [H r2 brx.md] |
| +0x4764 / +0x4768 | clue bottles collected / total (HUD) | [C:runtime] |
| +0x476C | countdown end time | [C:runtime] (REFUTED: +0x4764..0x4770 "timer state") |

## 2. Loading a level

A level change is requested by filling a TRANS record: `SetSwTransition(SW*, TRANS*)` 0x20DF80 sets SW+0x3BF0 = 1 and copies the 0x4C-byte record
to SW+0x3BF4 (callers: the wipe functions, always with g_sw) `[C:verified arbitration]`, `[C:runtime S56]`. The 0x40-byte file part is an
**XOR-obfuscated FILEREC**: `lba = (w0 ^ w13) + 0xCA0`, `size = w1 ^ w15` (the other words are filler text); decoded values match the disc manifest
exactly (hub 353856/10938886 = L02, L01 350144/1949759, L32 730464/10965082) `[C:runtime]`. The request oid (+0x3C38) is 0x189 in every state.

**Live warp by writing the request** `[C:live]` (PINE, 2026-11-01): writing SW+0x3BF0 = 1 (pending), +0x3BF4 flags = 0, +0x3BF8 the 16-word
masked FILEREC of the target level, +0x3C38 warp oid 0x189, +0x3C3C = -1 makes the next `ExecuteSwTransition` load that level: Paris -> India L08
loaded into the safehouse character select. Layout re-checked `[C:static v3 disassembly]` of `SetSwTransition` 0x20DF80 (`sw 1, 0x3BF0(a0)`, then a
0x4C-byte copy to +0x3BF4) and `WipeToWorldWarp` 0x22B490 (TRANS = {flags; CHK 16 words; oid; b}, so +0x3BF4 flags, +0x3BF8..+0x3C37 FILEREC,
+0x3C38 oid, +0x3C3C b); every save state holds +0x3C38 = 0x189 and +0x3C3C = -1 after a load `[C:runtime v3]`. Flag value 2 (`FSameChk`, same level) is set by
`WipeToWorldWarp` itself; a raw write with flags 0 behaves as a normal level change. The current level index D_003E1110 (= SW+0x3C80) reads `Lnn`
(2 in Paris S02, 8 in India S65-S73) `[C:runtime v3]`, `[C:live]`.

`ExecuteSwTransition(g_sw)` 0x20E890 runs at the top of every frame and does nothing unless SW+0x3BF0 is set. Corrected order `[C:static]`
(matched code), consistent with the S56 snapshot `[C:runtime]`:

1. `FlushFrames` (drain the render thread), then `SetPhase(2)` (phase D_003D3980 = 2 during the load, 3 in normal frames).
2. **Open the new world file first**: FILEREC -> sector range, 0xD8000-byte spool (D_002F7980), `SetDecompress` (LZSS window 0x4000 in the scratchpad
   0x70000000). The stream is a `CBinaryAsyncStream` on the main stack (0x471DB0 in S56).
3. `DeleteSw` 0x20E068 (also deletes the Splice frame and clears g_pjt/g_pmurray/g_pbentley/the PO list), `ResetClock` (g_clock.t = 0),
   `AddGrfusr(8)` (pad blocked: `g_grfjoyt` = 0; `UpdateUi` removes bit 8 0.1 s after the load).
4. `memset(g_sw, 0, 0x4790)` + `SW::SW`; `CMpeg::ExecuteOids` (queued movies); camera reset.
5. `LoadSwFromBrx` 0x13DA18 (below). It starts the **loading screen** itself (`BeginLoadingScreen` 0x1C9058 with a local start-time record).
6. `PostSwLoad` 0x20E740: post-load callbacks, vt15 PostLoad, HUD/grab-marker binding, default pathzone, `SetupCm` (empty in Sly 2), game event 4.
7. Current character from the save var `oidPoCurrent`; camera; game event 5; `ClearPhase`; **wipe-in** (`0x22B420` re-finds widget 0x30A and sets the wipe to k 2).

Save variables are re-resolved before the objects load (`episodeCurrent`, `widCurrent` = level index); all TASKs are state -1 until PostSwLoad `[C:runtime S56]`.

**Loading screen** `[C:runtime S56]` + code: `LoadingScreenThread` draws an 80x80 quad at (320,240), scaled 1 + 0.1 sin(2 pi t) (1 Hz, +-10%), alpha =
clamp(t,0,1) (1 s fade-in), t from `g_clock+0x3C` (the pause-independent clock). While the main thread is blocked in LoadSwFromBrx, frames come from this
thread, which ticks the clock paused and bumps `g_cframe` itself (+312 frames while the SW frame counter stays 0). In S56 the load had run 1.18 s:
WEs 0..216 of 281 populated, 5620 objects, 0x62E000 of 17.9 MB decompressed. The overlay D_002DD644 is NULL in all 55 dumps, so it is **not** the load
cover (its role is open).

### LoadSwFromBrx 0x13DA18 (matched hand file)

Reads the decompressed world stream in this order `[C:static]`, parsed to the last byte on all 43 world files `[C:parse]` (format: [world-file.md](../formats/world-file.md)):
1. `BeginSpliceLoad`; `S16 cwe` world entries (0x6C bytes each at SW+0x3F08) with name block, flags, root oid and copy count `c8`.
2. `LoadSoundSectionFromBrx(&g_sndmgr)` 0x1FD0E8.
3. Save-variable schema (`LoadSaveSchemaFromBrx` 0x1D7B88 into SW+0x3A18).
4. Level table (`LoadLevelTableFromBrx` 0x20F318).
5. Localized strings (0x13DF80), cell BSP (0x13E0E0).
6. Options of g_sw, then of g_cm (`LoadOptionsFromBrx` 0x13F810).
7. Align(0x10); Splice frame setup (0x1FF7E8), save-var resolve (`ResolveSwSaveVars` 0x20E3C0), scratch allocations, GS buffers.
8. Per world entry: blob header, Splice refs (`LoadSwSpliceFromBrx` 0x1FF858), shaders/textures (`LoadShadersFromBrx` 0x1EB748), the zone BSP
   (0x1C0EC0), **the object list** (`LoadSwObjectsFromBrx` 0x13FB40), Align(0x10), then `c8` clones of the list (`PloCloneLo`, 0x13E510).
   World entries are the **proxy sources** (PROXYSRC: WE.x10 = source id, c8 = copies, x60 = fUnk60, x5C != 0 = copies stay in the world): for
   entries with x5C = 0 the loaded objects are removed again (vt9) and enter the world only through PROXY records `[C:parse]` (level parser
   predicts live class counts within 0.1%) + `[C:runtime]` (placement check). Full layout: [formats/level-objects.md](../formats/level-objects.md).

### LoadSwObjectsFromBrx 0x13FB40 (matched)
Per object `[C:static]`: `S8 k` (+ `k` S16 filter ids when k>0), `S16 cid`, `S32 oid`, `S16 isplice`, `S32 cb`, then `cb` bytes.
- `k = -1/-2`: skip if the save state says the object is already gone (save query 0x805/0x405 on `(oid, IdProxy(iwe))`).
- `k > 0`: `FMatchLoadFilter` 0x13FAD0 against D_003E1110 (= current level index `[C:runtime]`); misses are skipped
  by `cb`, except JOB/TASK (CID 0x84/0x85), which are always created.
- PROXY-derived classes go through `LoadProxyFromBrx` 0x1C9B08; everything else `PloNew` + vt14 `LoadFromBrx`, which
  must consume exactly `cb` bytes.

## 3. Level table and world identity

`LoadLevelTableFromBrx` 0x20F318 stores 44 level records (FILEREC, world 0..8, level-name index b2 (0 exterior, 7 interior, 26 boss; the manifest's `epNN` labels are this index, not an episode), entry flag, a 4-entry
list of path symbols spelling `episode.<world>.level.<name>`) `[C:runtime runtime7b]` in D_003E1C40..D_003E1C4C; the 9 entry levels go to `g_aWorldEntryRecord` D_003E1C50 `[C:static]`, contents
`[C:verified samples]`. World names come from `g_apchzWorld` D_002A39E8 (intro, nightclub, palace, temple, prison,
castle, train, sawmill, blimp) `[C:static]`. Record 18 (castle, episode 7) is empty: the disc has **43 world files** = 41 `Lnn` levels + L00 (`world_elf2A38A8`, playable prologue `jb_intro_*`, 239 WEs) + L01 (`world_elf2A37E8`, front-end/menu world: WEs `abandon_icon`, `swf_ui`, `dvd_menu`) `[C:parse whole_game]`. WE-name prefixes follow the world: `f_` Paris, `i_` India, `p_` Prague, `c_` Canada, `a_` blimp, `jb_` L00 `[H]`. Helpers: `IlevelCur`, `NWorldCur`, `NEpisodeCur`, `NEpisodeBest` `[H r7 sw.md]`.

## 4. Transitions: warps, exits, wipes, boot queue

- **EXIT** (CID 0x1A, a VOL child, 0x190 bytes): player-in-volume state machine that starts a wipe once `tWipe` passes; loads a FILEREC plus a list of
  FILERECs (`LoadExitFromBrx` 0x2282B0) `[C:parse]`. `InitExit` 0x2281E8 sets +0x150 = 0x189 (oidWarp), +0x154 = -1; `PexitDefault` 0x2280D0 returns the
  EXIT with fDefault (+0x130); `SetExitExits` 0x228358 (g_sw+0x3C84 = this) (pending fixes; the v2 list still says Warp).
- **WARP** = CID 0x2A: warp **destinations**; `PwarpFromOid` 0x227FE8 matches the id or the alias list at +0x16C (Sly 1 oidAlias behind
  `WipeToWorldWarp(oidWarp, oidWarpContext)`), which is why its instances carry `entrance_*` strings `[C:static REVIEW]`. `TeleportSwPlayer` 0x228750 is a
  WARP method (TriggerWarp candidate). **CID41** (0x29) is an in-level teleport destination (its TRIGGER calls the player's `MatchStepXf` then Splice event 5).
- **Episode start** from the front-end world L01: WIDGET131 fires the episode's intro EXIT or `WipeToWorldWarp(..., 0x189, ..., 0x30A)` (see
  [game-progression](game-progression.md#6-objective-counters-job-markers-and-the-episode-menu)) `[C:runtime S55]`.
- **WIPE** D_00443740 `[C:runtime S56]`: +0 k (0 idle, 1 out animation 0x311, 2 in animation 0x30F, 3 fire), +4 active, +8 TRANS (0x4C), +0x54 widget,
  +0x58 widget oid (0x30A in every world). k 3 calls `SetSwTransition`; `WipeToWorldWarp` 0x22B490 fills the TRANS.
- **Overlay** D_002DD644: NULL in all 55 snapshots (including mid-load), so it is not the load cover; possibly the CID104 frame-capture transition `[C:runtime]` (NULL), role open.
- **Boot step queue** (0x1A4E08.., 16 entries): movie, world warp (boot world, oid 0x189), start task, memcard UI phase; run by
  StartupGame, ExecuteSwTransition and ContinueAfterJob `[H r4b bootseq.md]`. The game sequence queue D_002E1E88
  in game.cpp has the same command kinds `[H r3b game.md]`; whether these are one structure is open.
- **Phase memory**: `SetPhase`/`ClearPhase` 0x1C2A78/0x1C2BA8 (FlushCache, phase buffers) called by main and transitions `[H r5 phasemem]`.

## 5. Hands-off and locks

Four counters, each paired with a `grfusr` bit; every non-zero value seen at runtime is accounted for `[C:runtime runtime6]`:

| Counter | Functions | Taken by |
|---|---|---|
| SW+0x3D78 hands-off | `IncrementSwHandsOff(SW, grf)` 0x20EFC0 / `DecrementSwHandsOff` 0x20F030 / `FSwHandsOff` 0x20F0B0 | RELAY `HandsOff(1, grf)` (`SetJsgHandsOff` 0x1E50E0), job banner (grf 2), pause menu |
| SW+0x3D7C no-pause | `IncrementSwNoPause` 0x20F0C0 / `DecrementSwNoPause` 0x20F0F0 (v2 list: `...SwUsrLock`) | RELAY hands-off, JT recon mode (JTMODE kind 1) |
| SW+0x3D80 letterbox | via `IncrementSwHandsOff` grf bit 0 (`AcquireSwLetterbox` 0x20F128) | cutscenes (letterbox widget oid 0x373 in D_002E1E60) |
| SW+0x3D84 input lock | via grf bit 1; JT mode kind 1 increments it directly | cutscenes, job banner, recon mode |

Observed `[handsoff, nopause, letterbox, input]`: free roam [0,0,0,0]; recon dialogue S57 [0,1,0,1]; cutscene S59 [1,1,1,1]; banner + recon outro S61
[1,1,0,2]; pause menu S54 hands-off 1 (grf 0) with `g_grfusr` 2 `[C:runtime]`. `FInvulnerablePo` returns true while hands-off `[H r5 po.md]`.

## 6. Deferred callbacks and the Splice schedule

- `PostSwCallback` 0x149A50 appends `{object, C++ pointer-to-member, arg}` from pool SW+0x3C90 to the list SW+0x3CA8;
  `EnsureSwCallback` 0x149AB8 dedupes; `ClearSwCallbacks` 0x149B58 removes by mask (1 object, 2 member, 4 arg) `[H r2b callback.md]` (Sly 1 counterparts exact).
- `ProcessSwCallbacks` 0x149C80 drains the list (repeating while callbacks post new ones), then runs every Splice
  scheduled callback whose time has come (time-sorted list D_003E0E80, filled by `ScheduleSwSpliceCallback` 0x133040) `[H r2b callback.md]`.
- Examples: barrier impact -> Splice event 5 (D_002A4790 = {-1, 0x132298}); STEPPLAYER booty restore (D_002A4D10) `[H r2b]`.

## 7. Checkpoints (REFUTED)

The draft's "Sly 1 checkpoint manager in sw.cpp" is REFUTED: 0x20DF80 is `SetSwTransition` (sets the transition request; Sly 1 TRANS fSet), not
`RestoreChkmgrFromCheckpoint` `[C:verified arbitration]` `[C:runtime S56]`. A death reloads the level (SW and clock reset, save vars kept); there is no
checkpoint record in RAM `[C:runtime S29-S30]`. Task-level checkpoints are ordinary TASKs (`t1_recon_checkpoint1`, `t8_tntbarrel_checkpoint1`) whose
+0xC0 sets the character; how the respawn point is chosen (start marker vs JTPLACES) is open.

## 8. Other SW services

- Spatial queries `IntersectSwBoundingBox`/`Sphere` (max 256 results) `[H r7 sw.md]`.
- Illumination `SetSwIllum*`/`UpdateSwIllum` (HSV, midtone, shadow) `[H r7 sw.md]`.
- RSMG triggers `FAddRsmg`/`TriggerRsmg` (Sly 1 button.c). `FAddRsmg(SW*, RSMG*, int crsmgMax, int *pcrsmg, int fOnTrigger, OID oidRoot, OID oidSM, OID oidGoal)`, mangled `FAddRsmg__FP2SWP4RSMGiPii3OIDN25` `[C:verified arbitration]`.
- ASEG event actions (sound start/stop, show/hide with fade, object toggles) called by the ASEGA event switch 0x131290 `[H r7 sw.md]`.

## 9. Source files

{{TUS}}

## 10. Key functions

{{KEYFUNCS:0x20E890 0x20E740 0x20E068 0x20D9D0 0x20DF80 0x226550 0x149C80 0x20EFC0 0x20F0C0 0x22B490 0x1C9058 0x13DA18 0x13FB40 max=60}}

## Open questions
- The real file name of the TU holding LoadSwFromBrx (sorts between bq and brx).
- What the overlay D_002DD644 is (NULL in every snapshot).
- Respawn point after a death reload; PostSwLoad and the wipe-in were inferred from code (S56 caught one instant).
- 10 PROXY records in L02 whose idSource matches no WE.x10 in the file.
- Whether the boot step queue (0x1A4E08) and the game sequence queue (D_002E1E88) are the same object.
