# World (SW): level loading, transitions, hands-off, callbacks, checkpoints

The world object and everything that happens when a level is loaded, run and left.
Related: [world file format](../formats/world-file.md), [object-model](object-model.md), [save-memcard](save-memcard.md).

## 1. SW, the world singleton

- `g_sw` = D_003DD490, class SW (CID 0x1, BASIC child, 0x4790 bytes, vtable 0x2DC018, 9 slots) `[C:runtime]`.
  Constructed by the static initializer of sw.cpp via `SW::SW` 0x20D9D0 and re-constructed in place on every level
  change `[H r7 sw.md]`. SW is **not** an LO in Sly 2 (Sly 1: LO child) `[C:static]`.
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
| +0x3BF0 | pending transition request (flag + header 0x40 + oid + key) | [H r7 sw.md] |
| +0x3C90 / +0x3CA8 | deferred-callback pool / list | [H r2b callback.md] |
| +0x3D58 | a stamp counter (~145 increments per frame; XF/matrix stamps) — REFUTED as "frame stamp" | [C:runtime] |
| +0x3D60 | frame counter (bumped by RenderSw) | [C:runtime] |
| +0x3D78 / +0x3D7C / +0x3D80 / +0x3D84 | hands-off / user lock / letterbox / input lock counters (all 0 in free roam) | [H r7 sw.md], consistent [C:runtime] |
| +0x3E00 | clear colour; +0x3E10 HSV, +0x3E20 midtone, +0x3E24 shadow (illumination) | [H r7 sw.md] |
| +0x3EB8 / +0x3EFC / +0x3F00 | PO list (16) / current PO / override | [H r5 po.md] |
| +0x3F04 / +0x3F08 / +0x3F10 | world-entry (proxy source) count / array / current index (-2 after load) | [C:runtime] (count/array) |
| +0x3F14 / +0x3F18 / +0x3F20 | update table count / dead count / array of `{grf, plo}` | [C:runtime] |
| +0x455C.. / +0x45CC | cells and leaves / pathzone BSP | [H r2 brx.md, r5b path.md] |
| +0x4640 / +0x4644 | localized strings | [H r2 brx.md] |
| +0x4764 / +0x4768 | clue bottles collected / total (HUD) | [C:runtime] |
| +0x476C | countdown end time | [C:runtime] (REFUTED: +0x4764..0x4770 "timer state") |

## 2. Loading a level

`ExecuteSwTransition(g_sw)` 0x20E890 runs at the top of every frame and does nothing unless SW+0x3BF0 is set
`[C:static main]`. With a request pending, in order `[H r7 sw.md]`:

1. `FlushFrames` (drain the render thread), game event 1 (teardown), job abort if flagged.
2. `DeleteSw` 0x20E068 (also deletes the Splice frame, `DeleteSwSplice` 0x1FF830), `memset(g_sw)`, `SW::SW` again; world heap reset.
3. Open the world file: FILEREC -> sector range, 0xD8000-byte spool, `SetDecompress` (LZSS into the scratchpad) `[C:static disc_toc.md]`.
4. `LoadSwFromBrx` 0x13DA18 (below), bracketed by the loading-screen thread (`BeginLoadingScreen` 0x1C9058) `[H r5 prog.md]`.
5. `PostSwLoad` 0x20E740: drain the post-load callback queue, vt15 PostLoad, bind HUD/grab-marker objects, create a
   default pathzone if the level has none, game events 4 and 5 `[H r7 sw.md, r5b path.md]`.
6. Restore checkpoint / current character from the save var `oidPoCurrent`; camera reset.

### LoadSwFromBrx 0x13DA18 (matched hand file)

Reads the decompressed world stream in this order `[C:static]`, verified end-to-end on two sample worlds `[C:verified]` (format: [world-file.md](../formats/world-file.md)):
1. `BeginSpliceLoad`; `S16 cwe` world entries (0x6C bytes each at SW+0x3F08) with name block, flags, root oid and copy count `c8`.
2. `LoadSoundSectionFromBrx(&g_sndmgr)` 0x1FD0E8.
3. Save-variable schema (`LoadSaveSchemaFromBrx` 0x1D7B88 into SW+0x3A18).
4. Level table (`LoadLevelTableFromBrx` 0x20F318).
5. Localized strings (0x13DF80), cell BSP (0x13E0E0).
6. Options of g_sw, then of g_cm (`LoadOptionsFromBrx` 0x13F810).
7. Align(0x10); Splice frame setup (0x1FF7E8), save-var resolve (`ResolveSwSaveVars` 0x20E3C0), scratch allocations, GS buffers.
8. Per world entry: blob header, Splice refs (`LoadSwSpliceFromBrx` 0x1FF858), shaders/textures (0x1EB748), a small
   table (0x1C0EC0), **the object list** (`LoadSwObjectsFromBrx` 0x13FB40), then `c8-1` clones of the list (`PloCloneLo`).
   Unless the entry's deferred flag (x5C) is set, vt9 is called on every object `[H r2 brx.md]`.

### LoadSwObjectsFromBrx 0x13FB40 (matched)
Per object `[C:static]`: `S8 k` (+ `k` S16 filter ids when k>0), `S16 cid`, `S32 oid`, `S16 isplice`, `S32 cb`, then `cb` bytes.
- `k = -1/-2`: skip if the save state says the object is already gone (save query 0x805/0x405 on `(oid, IdProxy(iwe))`).
- `k > 0`: `FMatchLoadFilter` 0x13FAD0 against D_003E1110 (= current level index `[C:runtime]`); misses are skipped
  by `cb`, except JOB/TASK (CID 0x84/0x85), which are always created.
- PROXY-derived classes go through `LoadProxyFromBrx` 0x1C9B08; everything else `PloNew` + vt14 `LoadFromBrx`, which
  must consume exactly `cb` bytes.

## 3. Level table and world identity

`LoadLevelTableFromBrx` 0x20F318 stores 44 level records (FILEREC, world 0..8, "episode" kind, entry flag, a 4-entry
list) in D_003E1C40..D_003E1C4C; the 9 entry levels go to `g_aWorldEntryRecord` D_003E1C50 `[C:static]`, contents
`[C:verified samples]`. World names come from `g_apchzWorld` D_002A39E8 (intro, nightclub, palace, temple, prison,
castle, train, sawmill, blimp) `[C:static]`. Helpers: `IlevelCur`, `NWorldCur`, `NEpisodeCur`, `NEpisodeBest` `[H r7 sw.md]`.

## 4. Transitions: warps, exits, wipes, boot queue

- **EXIT** (CID 0x1A, a VOL child): player-in-volume state machine that starts a wipe once `tWipe` passes; loads level
  file records (`LoadExitFromBrx` 0x2282B0). **WARP** = CID 0x2A (XFM with RSMG records; `SetWarpRsmg` 0x2288D8)
  `[C:verified]` (r7r8 verifier; CONFLICTS #11).
- **WIPE**: a global wipe state machine; `WipeToWorldWarp` 0x22B490 fills the transition request, with checkpoint handling `[H r8 wipe.md]`.
- **Overlay** D_002DD644: while its state is 2/3 the world is not rendered, so a full-screen wipe/fade can cover a load `[H r4 main.md]`.
- **Boot step queue** (0x1A4E08.., 16 entries): movie, world warp (boot world, oid 0x189), start task, memcard UI phase; run by
  StartupGame, ExecuteSwTransition and ContinueAfterJob `[H r4b bootseq.md]`. The game sequence queue D_002E1E88
  in game.cpp has the same command kinds `[H r3b game.md]`; whether these are one structure is open.
- **Phase memory**: `SetPhase`/`ClearPhase` 0x1C2A78/0x1C2BA8 (FlushCache, phase buffers) called by main and transitions `[H r5 phasemem]`.

## 5. Hands-off and locks

`IncrementSwHandsOff` 0x20EFC0 / `DecrementSwHandsOff` 0x20F030 / `FSwHandsOff` 0x20F0B0 manage SW+0x3D78 (with a user
flag `grfusr` 2); a second counter at +0x3D7C uses grfusr 4; letterbox (`AcquireSwLetterbox`) +0x3D80; input lock
+0x3D84 `[C:verified]` for 0x20EFC0 (verify B: 0x20F0C0 is the second counter, not IncrementSwHandsOff). Cutscenes,
job banners, the rhythm game and the recon shutter take hands-off; `FInvulnerablePo` returns true while hands-off `[H r5 po.md]`.

## 6. Deferred callbacks and the Splice schedule

- `PostSwCallback` 0x149A50 appends `{object, C++ pointer-to-member, arg}` from pool SW+0x3C90 to the list SW+0x3CA8;
  `EnsureSwCallback` 0x149AB8 dedupes; `ClearSwCallbacks` 0x149B58 removes by mask (1 object, 2 member, 4 arg) `[H r2b callback.md]` (Sly 1 counterparts exact).
- `ProcessSwCallbacks` 0x149C80 drains the list (repeating while callbacks post new ones), then runs every Splice
  scheduled callback whose time has come (time-sorted list D_003E0E80, filled by `ScheduleSwSpliceCallback` 0x133040) `[H r2b callback.md]`.
- Examples: barrier impact -> Splice event 5 (D_002A4790 = {-1, 0x132298}); STEPPLAYER booty restore (D_002A4D10) `[H r2b]`.

## 7. Checkpoints

The Sly 1 checkpoint manager moved into sw.cpp: `RestoreChkmgrFromCheckpoint` 0x20DF80 (existing name; its 2-argument
signature contradicts Sly 1, CONFLICTS #10) `[H r7 sw.md]`. Checkpoint restore runs inside ExecuteSwTransition after
load. Task-level checkpoints appear in save-variable names (`t8_tntbarrel_checkpoint1`, ...) `[C:runtime]`.

## 8. Other SW services

- Spatial queries `IntersectSwBoundingBox`/`Sphere` (max 256 results) `[H r7 sw.md]`.
- Illumination `SetSwIllum*`/`UpdateSwIllum` (HSV, midtone, shadow) `[H r7 sw.md]`.
- RSMG triggers `FAddRsmg`/`TriggerRsmg` (Sly 1 button.c; `FAddRsmg` has an extra first arg, CONFLICTS #10) `[H r7 sw.md]`.
- ASEG event actions (sound start/stop, show/hide with fade, object toggles) called by the ASEGA event switch 0x131290 `[H r7 sw.md]`.

## 9. Source files

| Span | File (label) | Notes |
|---|---|---|
| 0x13DA18-0x13E368 | brx_sw? (LoadSwFromBrx) | file name unknown, sorts bq<x<brx |
| 0x13E368-0x13FF70 | brx |  |
| 0x149A50-0x14A008 | callback? |  |
| 0x1A4E08-0x1A5270 | bootseq? (step queue) |  |
| 0x1C2A78-0x1C2D20 | phasemem |  |
| 0x20D9D0-0x210E90 | sw | was labelled button |
| 0x226550-0x226AB8 | update |  |
| 0x227FE8-0x228B48 | warp? (EXIT/WARP) | was labelled stepguard |
| 0x22B040-0x22B720 | wipe |  |

## 10. Key functions

| Addr | Name | Purpose | Conf | Status |
|---|---|---|---|---|
| [0x13DA18](../../src/hand/func_0013DA18.cpp) | `LoadSwFromBrx` | Top-level level-body reader: proxy-source table, sound, save vars, level table, langstrs, cells, g_sw/g_cm options, then per proxy source: refs+shader | 0.92 | HYP |
| [0x13DF80](../../src/hand/func_0013DF80.c) | `LoadSwLangstrsFromBrx` | Reads localized string table: U8 clang + lang ids, S16 c x {S16 id; clang x string16}; keeps the g_nLang (D_002E1E54) string into SW.alangstr (+0x4640 | 0.80 | HYP |
| [0x13E0E0](../../src/hand/func_0013E0E0.c) | `LoadSwCellsFromBrx` | Reads cell BSP: U16 c x {VECTOR4 plane; S16 front; S16 back}; negative child = new leaf index; allocs SW.acell (+0x455C, 0x20 each) and SW.aleaf (clea | 0.75 | HYP |
| [0x13E368](../../src/hand/func_0013E368.c) | `PchzFindSwLangstr` | Linear search of SW.alangstr by id; returns string or "" (D_002A4BE0) if id==-1 or not found | 0.80 | HYP |
| [0x13E3C8](../../src/hand/func_0013E3C8.c) | `PloNew` | Create object: D_002BAAE8[cid].pfnNew(); set psoParent(+0x10), oid(+0x18), refcounted pframe from isplice; append to SW oid and cid hash lists; vt7 In | 0.95 | HYP |
| [0x13E510](../../src/hand/func_0013E510.c) | `PloCloneLo` | Clone object: PloNew(Pclassinfo(po)->cid, psoParent, po->oid, -1) then vt40 (clone-from, which for SO also clones children via PloCloneLo)  | 0.88 | HYP |
| [0x13E590](../../src/hand/func_0013E590.c) | `PostLoadOidField` | LORUN callback: *(OID-resolved LO*)(plo + ib) = PloFindSwObject(g_sw, 4, oid, plo) | 0.85 | HYP |
| [0x13E5E0](../../src/hand/func_0013E5E0.cpp) | `PostLoadOidSetter` | LORUN callback: finds object by oid (PloFindSwObject mode 4) and calls the stored pointer-to-member setter with it | 0.85 | HYP |
| [0x13E690](../../src/hand/func_0013E690.c) | `RefFromBrx` | Reads one typed method-option argument into a CRef by otyp: 0 bool(U8), 1 float, 5/6 S32, 7/14..23 S16, 2/9 3xF32 vectors, 10 2xF32, 0x1xxx U32, else  | 0.85 | HYP |
| [0x13E818](../../src/hand/func_0013E818.cpp) | `LoadOptionFromBrx` | Reads one option value and applies it via g_aeopid record (method/field/setter forms; oid refs deferred to LORUN) | 0.95 | HYP |
| [0x13F810](../../src/hand/func_0013F810.c) | `LoadOptionsFromBrx` | Loop: S16 eopid until <0; LoadOptionFromBrx(pv, &g_aeopid[eopid] (D_002AC880 stride 0x28), grf & 0x2580, pbis) | 0.95 | HYP |
| [0x13F888](../../src/auto/func_0013F888.c) | `__19CLoadCollectorArray` | CLoadCollectorArray ctor: vptr(+0)=D_002CBE50, cMax=c=0, a=0 | 0.88 | HYP |
| [0x13F8A8](../../src/hand/func_0013F8A8.c) | `CLoadCollectorArray::Add` | Append c LO* to growable array (capacity doubles, min 32, PvAllocStack 0x1AE318 + CopyAb) | 0.85 | HYP |
| [0x13FB40](../../src/hand/func_0013FB40.c) | `LoadSwObjectsFromBrx` | Reads an object list: S16 cobj x {S8 k; [filter]; S16 cid; S32 oid; S16 isplice; S32 cb; payload via vt14 LoadFromBrx}, proxies (FIsCidDerivedFrom(cid | 0.95 | HYP |
| [0x149A50](../../src/hand/func_00149A50.c) | `PostSwCallback` | Append a deferred callback {object, pointer-to-member, arg} from pool g_sw+0x3C90 to the SW callback list (+0x3CA8) | 0.85 | HYP |
| [0x149AB8](../../src/hand/func_00149AB8.c) | `EnsureSwCallback` | PostSwCallback unless an identical {obj, member pointer, arg} entry is already queued | 0.85 | HYP |
| [0x149B58](../../src/hand/func_00149B58.c) | `ClearSwCallbacks` | Remove queued callbacks matching by grf bits: 1 = object, 2 = member pointer, 4 = arg; fix tail | 0.85 | HYP |
| [0x149C80](../../src/hand/func_00149C80.cpp) | `ProcessSwCallbacks` | Drain the deferred-callback list (calling each member function, repeating while new ones are posted), then run all Splice scheduled callbacks whose ti | 0.80 | HYP |
| [0x1C2A78](../../src/hand/func_001C2A78.c) | `SetPhase` | if phase != g_phase(D_003D3980): FlushCache, set g_phase and (Sly2) re-point phase memory buffers | 0.85 | HYP |
| [0x1C2BA8](../../src/auto/func_001C2BA8.c) | `ClearPhase` | SetPhase(PHASE_None=0) | 0.80 | HYP |
| [0x20D9D0](../../src/hand/func_0020D9D0.cpp) | `SW::SW` | SW constructor: vt D_002DC018, ~30 CSplotheap pools, ~60 DLs, 512+199+512 hash buckets, illum HSV (180,255,255), fog/fov defaults | 0.70 | HYP |
| [0x20E068](../../src/hand/func_0020E068.c) | `DeleteSw` | Tear down the current world before loading: game callback(1), free view/joy state, ResetSndmgr, clear ~15 global object pointers, FreeSw | 0.60 | HYP |
| [0x20E3C0](../../src/hand/func_0020E3C0.cpp) | `ResolveSwSaveVars` | Look up save-variable paths the game reads (language, sound flags, job/world, per-world dt/percent_complete), store current language/job/world, raise  | 0.80 | VERIFIED |
| [0x20E740](../../src/hand/func_0020E740.c) | `PostSwLoad` | After the BRX is read: reset transient pools, bind HUD/aux objects, run post-load callbacks (vt15) until queue empty, StartupCm-like camera reset, gam | 0.55 | HYP |
| [0x20E890](../../src/hand/func_0020E890.cpp) | `ExecuteSwTransition` | Level transition: if a request is pending, open level file, DeleteSw, memset+construct g_sw, InitSwLoadBuffers, LoadSwFromBrx, PostSwLoad, restore che | 0.55 | HYP |
| [0x20EFC0](../../src/hand/func_0020EFC0.c) | `IncrementSwHandsOff` | ++SW+0x3D78 (first -> AddGrfusr(2)); grf&1 AcquireSwLetterbox, grf&2 ++count 0x3D84 | 0.80 | HYP |
| [0x20F030](../../src/hand/func_0020F030.c) | `DecrementSwHandsOff` | --SW+0x3D78 (last -> RemoveGrfusr(2), SetJoyBtnHandled); grf&1 ReleaseSwLetterbox, grf&2 --0x3D84 | 0.80 | HYP |
| [0x20F0B0](../../src/auto/func_0020F0B0.c) | `FSwHandsOff` | SW+0x3D78 > 0 | 0.75 | VERIFIED |
| [0x20F160](../../src/auto/func_0020F160.c) | `ReleaseSwLetterbox` | --SW+0x3D80; last -> 0x1A30D0 (vt50 hide) | 0.50 | VERIFIED |
| [0x20F2F8](../../src/auto/func_0020F2F8.c) | `SetSwIllum` | Store midtone +0x3E20 (ctor default 90) and UpdateSwIllum | 0.50 | VERIFIED |
| [0x20F318](../../src/hand/func_0020F318.cpp) | `LoadLevelTableFromBrx` | Read level table: clevel x {file record, world, episode, fEntry, n level numbers}; fills g_alevellist/g_anWorldOfLevel/g_anEpisodeOfLevel/world entry  | 0.85 | HYP |
| [0x20F5D0](../../src/hand/func_0020F5D0.c) | `LoadSwSharedHeaderFromBrx` | Read (cb, id) of a cross-level shared data block; flag reload if id changed or forced | 0.35 | VERIFIED |
| [0x20F948](../../src/hand/func_0020F948.c) | `FFilterSoPair` | Intersection filter: reject self/flagged; accept unless relationship 0x134310(a,b) is 1 or 3 | 0.25 | VERIFIED |
| [0x20FAA0](../../src/hand/func_0020FAA0.c) | `IntersectSwBoundingBox` | Collect up to 256 SOs whose groups overlap the AABB(min/max of two points), optional filter; result in slot-allocated array | 0.75 | HYP |
| [0x20FC60](../../src/hand/func_0020FC60.c) | `IntersectSwBoundingSphere` | Same for a sphere (center, radius) | 0.75 | VERIFIED |
| [0x20FEC0](../../src/hand/FAddRsmg__FP4RSMGiPii3OIDN24.c) | `FAddRsmg` | Add/update a (root,SM)->trigger/untrigger goal record | 0.70 | VERIFIED |
| [0x2108E8](../../src/hand/func_002108E8.c) | `StopSwAsegSound` | ASEG event 1: StopSnd stored continuous sound (optionally matching id) and release ref | 0.55 | VERIFIED |
| [0x210B98](../../src/auto/func_00210B98.c) | `PrecWorldEntry` | &g_arecWorldEntry[iworld] (64 bytes) if valid, else NULL | 0.40 | VERIFIED |
| [0x226550](../../src/hand/func_00226550.c) | `UpdateSw` | per-frame world update: callbacks, objects (0x2267C0), lights, sound, camera ... | 0.75 | HYP |
| [0x2282B0](../../src/hand/func_002282B0.c) | `LoadExitFromBrx` | LoadVolFromBrx then blob + U8 count of entries | 0.50 | VERIFIED (arbitration/conflict fix; was LoadWarpFromBrx) |
| [0x2288D8](../../src/hand/func_002288D8.c) | `SetWarpRsmg` | FAddRsmg(g_sw, p+0xE4, 4, p+0xE0, ...) when fOnTrigger | 0.40 | VERIFIED (arbitration/conflict fix; was SetCid41Rsmg) |
| [0x22B050](../../src/auto/func_0022B050.c) | `InitWipe (WIPE ctor)` | wipes fields, state -1, SetWipeWipes(0) | 0.50 | VERIFIED |
| [0x22B490](../../src/hand/func_0022B490.c) | `WipeToWorldWarp` | request wipe to a level/warp (dedupe same CHK) | 0.45 | HYP |

Selected 43 of 122 named functions (confidence >= 0.75, verified, or flagged). Full list: [reference/functions/world-sw.md](../reference/functions/world-sw.md).

## Open questions
- The real file name of the TU holding LoadSwFromBrx (sorts between bq and brx).
- What vt9 on every object after a world entry load means when x5C is clear; WE fields x60/x14.
- The exact call order inside ExecuteSwTransition and PostSwLoad has not been traced at runtime (no load in the save states).
- The class created as CID 0x74 in PostSwLoad; D_00443740 == 1 state in 0x20E040.
- Whether the boot step queue (0x1A4E08) and the game sequence queue (D_002E1E88) are the same object.
