# How the Sly 2 engine works

A cross-cutting overview of the engine as recovered by the decomp: boot, the frame loop, the level lifecycle, the object
model, and one section per subsystem. Detail that belongs to one file or struct lives in that file's header comment or
next to the struct in `include/sly2/`; this page keeps what spans files. Names are the code's current names; where an
address helps, it is given (`0x1A7518`). `D_XXXXXXXX` is a global the code has not named.

This page condenses the per-subsystem research pages that used to live in `docs/engine/` (plus `docs/README.md`,
`docs/reference/glossary.md` and `docs/open-questions.md`). Those pages hold more runtime tables (per-state field dumps,
save-state numbers, provenance) than fit here; they remain in git history:
`git show 3493d772:docs/engine/<page>.md` (pages: animation, bosses, camera, core-library, cutscenes, game-progression,
gameplay-objects, guards-ai, hud-ui, main-loop-startup, minigames, object-model, overview, particles-effects,
physics-collision, player, rendering, save-memcard, sound, splice-scripting, world-sw).

## 1. How knowledge is marked

Facts carry a tag saying how we know them. The same tags appear in source and header comments.

| Tag | Meaning |
|---|---|
| `[C:runtime]` | observed in PCSX2 save-state RAM (states S02-S74: Paris and India hubs, combat, all three characters, a level load, the pause menu, ThiefNet, the front end, a full mission); a state or round may be named, e.g. `[C:runtime S56]` |
| `[C:live]` | confirmed in the running game over PINE: a value was written or read and the game visibly reacted |
| `[C:verified]` | re-derived from the code by an independent verifier (incl. the slot-22/23 arbitration) |
| `[C:parse]` | a parser following the matched loader consumed every byte of the game's files (all 43 world files) |
| `[C:static]` | read directly from matched code, ELF data or the VU disassembly, and structurally certain (a vtable slot, a decoded table, a call order) |
| `[C:data]` | read from level data (object records, task records, Splice code, string tables) |
| `[H]` | hypothesis: inferred from code shape, Sly 1 similarity, callers or link order (a provenance label such as `r4 lo.md` names an out-of-repo write-up) |
| `REFUTED` | an earlier claim a later check contradicted; kept so nobody re-introduces it |

Matching does not mean understood: every function compiles to the original bytes, so the code is certain, but a name or
purpose is a hypothesis until a verifier, a runtime check or a structural fact backs it. Code comments mark such names
`(inferred)`.

Conventions: offsets are `+0x18` from the object start; "slot N" / `vtN` is C++ virtual slot N. gcc 2.95 vtable entries are
8 bytes `{s16 delta; s16 pad; void *pfn}`, slot 0 is a null header, so slot N's function pointer is at `vtable + 8*N + 4`.
CID numbers are hex (`CID 0x47`) or decimal placeholders (`CID71`, `BHV171`) for classes whose Sucker Punch name is unknown.

## 2. The shape of the program

- One statically linked EE executable, `SCUS_973.16`: game code 0x100000-0x2319E8, Sony's 989snd client library
  0x2319E8-~0x233130, Sony SDK libraries above that `[C:static]`.
- Game code is C++ compiled by ee-gcc 2.95.3 (`-O2 -G0 -ffast-math`) and linked **in alphabetical file order** after a core
  block (crt0, Splice thunks, class factories, core library, Splice runtime) `[C:static]`. That order is how most file names
  were recovered `[H]`.
- Almost every translation unit ends with compiler stubs that register the Splice slot-heap pools
  (`__static_initialization_and_destruction_0` + `_GLOBAL_$I`, plus `_GLOBAL_$D` with global destructors). These ~209
  boundaries split the code into ~210 TUs `[C:static]`; each TU is one file under `src/game/` (see
  [architecture.md](architecture.md)).
- The design descends from Sly Cooper 1 (same vocabulary: LO, SO, SW, CM, BRX, Splice), but the object model was rebuilt with
  real C++ classes and virtuals, every class ID was renumbered and most structs changed layout. Sly 1 is a source of
  **names**, not offsets.

## 3. Boot

`_start` (crt0) -> `GameMain` 0x1A7518 ([`app/main.cpp`](../src/game/app/main.cpp)):

1. `__main` 0x23B7D8 runs the C++ global constructors (`g_sw`, `g_cm`, `g_sndmgr`, the screen objects, ...).
2. `Startup` 0x1A7970 runs the 19-entry startup table D_002BFCE0 under a `CProg` progress bar, ticking the clock between
   entries; prints build "0813.0032"/"america" `[C:verified]`. Entries in order: `StartupDma`, `StartupThread`, `StartupSif`
   (IOP modules, `ioprp300.img`), `StartupGs`, `StartupMemMgr`, `StartupCd`, `SetCdPollStateIdle`, `StartupFont`,
   `StartupJoy`, `StartupFrame` (render thread), `StartupLoadingScreen`, `StartupClock`, `StartupVU0`, `StartupVU1`,
   `StartupSound`, `StartupBlips`, `StartupHudGifPackets`, `StartupMemcard`, `StartupMpeg`, NULL `[C:static]`. There is no
   camera entry: `SetupCm` 0x14D470 is empty in Sly 2 but keeps Sly 1's two call sites (main, `PostSwLoad`) `[C:verified]`.
3. `StartupGame` 0x1613A8: language from `sceScfGetLanguage`, game event 0 (startup), then the boot step queue
   (`BeginStartupSequence` 0x1A4E08: logo movie, memcard check, warp to the boot world) `[H]`.
4. The frame loop, forever.

## 4. The frame loop

From the matched `GameMain` body `[C:static]`:

```
for (;;) {
  1  ExecuteSwTransition(g_sw)      0x20E890  only if a level change is pending (section 5)
  2  UpdateJoy(&g_joy)              0x1871C0  pad read
  3  UpdateCd()                     0x14BA38  disc check (CRC of one sector) / CD service
  4  SetupCm(&g_cm)                 0x14D470  empty in Sly 2
  5  MarkClockTick(&g_clock, 0)     0x14D0B8  dt from the EE cycle counter, clamped to [1/60, 1/30] s
  6  UpdateSw(g_sw)                 0x226550  the world update (below)
  7  UpdateScreens(D_003D9E70)      0x1D9648  pause / menus / memcard UI; Start (0x800) toggles pause
  8  OpenFrame()                    0x160828  start this frame's DMA chains
  9  if (IsFrameCaptureInactive())  0x1477B0  overlay D_002DD644 absent or allows rendering
        RenderSw(g_sw)              0x1CDA80  vt19 RenderSelf on live objects
        DrawSw(g_sw)                0x157FA0  sort the render list, emit the VIF1 chain (section 7.3)
        DrawUi(D_003D9E70)          0x1D9820  HUD lists + GIF/VIF flush
 10  if overlay: DrawFrameCaptureScreen  0x1477D8  full-screen transition pass
 11  CloseFrame()                   0x160A98  finalize, hand the frame to the render thread
 12  D_002BE538++                             global frame counter, never reset
}
```

The render thread (`FrameRenderLoop` 0x160B70) waits for a closed frame, DMAs its VIF1 then GIF chains, optionally blends
the previous frame back in (`BlendPrevFrame`), and swaps GS buffers, so the CPU builds frame N+1 while the GPU draws frame
N. The overlay pointer was NULL in all 55 save states, so in practice the world always renders, including under the pause
menu `[C:runtime]`. During a level load the main thread is blocked in `ExecuteSwTransition` and frames come from the
loading-screen thread. Other threads: the memcard worker (`MemcardThreadMain` 0x1AAF18) and an MPEG display thread.

**`UpdateSw`** (once per frame, if dt > 0) `[H]`, table and order `[C:runtime]`/`[C:verified]` where noted:
1. deferred SW callbacks, then due Splice scheduled callbacks (`ProcessSwCallbacks` 0x149C80);
2. the busy list: decide which roots are near the player/camera, fire OnAdd/OnRemove (`UpdateSwBusyList` 0x147F40) `[C:verified]`;
3. vt16 `Update` on every live object in the SW update table (SW+0x3F20) `[C:runtime]`;
4. world sub-lists (ASEGA animation applications, noise stimuli), physics `SolveSw` 0x1F8DD8;
5. lights, shader animation, zap-history expiry, then `UpdateSave`, `UpdateSndmgr` 0x1FD830, the camera and the HUD stack
   (`UpdateSwTimerHud` 0x20F7E0).

**Clock** (`g_clock`, [`core/time/clock.cpp`](../src/game/core/time/clock.cpp)): reads the CP0 Count register, extends it to
64 bits, converts at 294.912 MHz and clamps dt. Game time is scaled by the product of **9 rate slots** `art[0..8]`
(+0x18..+0x38): slot 0 pause menu / ThiefNet, slot 1 power-up gadgets (Thief Reflexes 0.25, Time Rush 1.35), slot 7 widget
pause (job banner), slot 8 camera. `rDt` (+8) is a **cached** product recomputed only by `SetClockRate` 0x14D1D0 ->
`RecalcClockRate` 0x14D1F8, so writing a slot alone has no effect `[C:live]`; a port or trainer must call `SetClockRate`.
The camera uses dt x art[8] only, so it runs during pause and slow motion. +0x3C is a pause-independent real time (loading
screen, episode menu, ThiefNet). `ResetClock` sets t = 0 on every level load.

**Input**: `g_joy` (JOY: +0x10B4 held, +0x10B8 pressed). Gameplay never reads the pad directly: each playable object owns a
POCTL (section 7.10). 15 cheat codes (records at 0x2BD4E0) advance on presses and fire one second after the last press
([`gameplay/progression/cheat.cpp`](../src/game/gameplay/progression/cheat.cpp)). Rumble has a global mode
(follow option / forced on / off) gated by the save var `flags.fNoVibration`.

## 5. The world and the level lifecycle

There is exactly one world object, `g_sw` (class SW, CID 0x1, a BASIC child, 0x4790 bytes, at 0x3DD490) `[C:runtime]`. SW is
**not** an LO in Sly 2. It owns every pool, object list and hash table of the level, and is re-constructed in place on every
level change. SW slots 8/9 are `RenderSw`/`DrawSw`; `UpdateSw` is called directly. Selected fields (more in
`include/sly2/sw.h`): +0x10 root object DL; +0x20 busy-candidate list of LOROOTs; +0x2C active-roots list; +0x38 oid hash
(512 SL buckets, `(oid*0x95675) & 0x1FF`); +0x3038 199 per-class SLs (12 bytes each); +0x3A18 save-variable schema;
+0x3AFC..+0x3BC4 progression slots; +0x3BF0/+0x3BF4 transition request; +0x3D78..+0x3D84 lock counters; +0x3EB8.. PO
array and current PO (+0x3EFC); +0x3F04/+0x3F08 world entries; +0x3F14..+0x3F20 update table.

### Loading a level

A change is requested by `SetSwTransition(SW*, TRANS*)` 0x20DF80: SW+0x3BF0 = 1 and a 0x4C-byte TRANS copied to +0x3BF4
(`{flags; 16-word FILEREC; oid; b}`). The FILEREC part is XOR-obfuscated: `lba = (w0 ^ w13) + 0xCA0`, `size = w1 ^ w15`
`[C:runtime]`. Writing the request directly over PINE warps to any level `[C:live]`. `WipeToWorldWarp` 0x22B490 is the usual
caller (via the wipe, below).

`ExecuteSwTransition` 0x20E890, order from the matched code `[C:static]`, consistent with a mid-load save state:
1. `FlushFrames` (drain the render thread); `SetPhase(2)` (phase memory, D_003D3980).
2. **Open the new world file first**: FILEREC -> sector range, a 0xD8000-byte spool, `CBinaryAsyncStream` with LZSS
   decompression through a 0x4000-byte scratchpad window.
3. `DeleteSw` 0x20E068 (also deletes the Splice frame and clears the player pointers and PO list), `ResetClock`, block the pad.
4. `memset(g_sw, 0, 0x4790)` + `SW_ctor` 0x20D9D0; queued movies; camera reset.
5. `LoadSwFromBrx` 0x13DA18, which starts the **loading screen** itself (`BeginLoadingScreen` 0x1C9058). The loading-screen
   thread draws an 80x80 quad at (320,240), scale 1 + 0.1 sin(2 pi t), 1 s fade-in, and ticks the clock and frame counter.
6. `PostSwLoad` 0x20E740: drain post-load callbacks, vt15 PostLoad on objects, bind save vars, default pathzone, game event 4.
7. Current character from the save var `oidPoCurrent`; game event 5; `ClearPhase`; wipe-in (`ResumeWipeAfterLoad`).

`LoadSwFromBrx` reads the decompressed stream in this order `[C:static]` (format: [formats/world-file.md](formats/world-file.md)):
world-entry table (0x6C bytes each, copy count `c8`); sound section (`LoadSoundSectionFromBrx`); save-variable schema
(`LoadSaveSchemaFromBrx`); level table (`LoadLevelTableFromBrx`); localized strings; cell BSP; options of `g_sw` then
`g_cm` (`LoadOptionsFromBrx`); Splice frame setup and save-var resolve (`ResolveSwSaveVars`); then **per world entry**:
Splice refs, shaders/textures (`LoadShadersFromBrx`), zone BSP, the object list (`LoadSwObjectsFromBrx` 0x13FB40) and `c8`
clones of it (`PloCloneLo`). World entries are **proxy sources**: entries flagged to stay out of the world have their loaded
objects removed again and appear only through PROXY copies ([formats/level-objects.md](formats/level-objects.md)).

Each object record is `S8 k` (+ k S16 load-filter ids), `S16 cid`, `S32 oid`, `S16 isplice`, `S32 cb`, then `cb` bytes.
`k < 0` skips objects the save says are gone; `k > 0` is matched against the current level index D_003E1110
(`FMatchLoadFilter`), except JOB/TASK which are always created. PROXY classes go through `LoadProxyFromBrx`; everything
else is `PloNew` + vt14 `LoadFromBrx`, which must consume exactly `cb` bytes.

**Level identity.** The level table holds 44 records (FILEREC, world 0..8, level-name index, path symbols spelling
`episode.<world>.level.<name>`); worlds are intro, nightclub (Paris), palace (India), temple, prison (Prague), castle,
train, sawmill (Canada), blimp. The disc has **43 world files**: 41 `Lnn` levels + L00 (playable prologue) + L01
(front-end/menu world) `[C:parse]`. World-entry name prefixes follow the world: `f_` Paris, `i_` India, `p_` Prague,
`c_` Canada, `a_` blimp, `jb_` L00.

### Transitions

- **EXIT** (CID 0x1A, a VOL child): a player-in-volume state machine that starts a wipe; **WARP** (CID 0x2A): warp
  destinations matched by id or alias list (`FindWarpByOid`); CID41 (0x29): in-level teleport destination.
- **WIPE** (D_00443740): k 0 idle, 1 out animation, 2 in animation, 3 fire -> `SetSwTransition`. Widget oid 0x30A in every world.
- The boot step queue (16 entries: movie, world warp, start task, memcard UI phase) is run by `StartupGame`,
  `ExecuteSwTransition` and the job flow `[H]`.
- **Death reloads the level** `[C:runtime]`: SW and clock reset, save variables kept. There is **no checkpoint record**
  (REFUTED: a "checkpoint manager" in sw.cpp; 0x20DF80 is `SetSwTransition`). "Checkpoints" are ordinary TASKs.

### Hands-off and locks

Four counters, each paired with a `g_grfusr` bit; every non-zero value seen at runtime is accounted for `[C:runtime]`:

| Counter | Functions | Taken by |
|---|---|---|
| SW+0x3D78 hands-off | `IncrementSwHandsOff(SW, grf)` / `DecrementSwHandsOff` / `IsSwHandsOff` | RELAY `HandsOff(1, grf)`, the job banner (grf 2), the pause menu |
| SW+0x3D7C no-pause | `IncrementSwNoPause` / `DecrementSwNoPause` | RELAY hands-off, recon mode |
| SW+0x3D80 letterbox | grf bit 0 of `IncrementSwHandsOff` (`AcquireSwLetterbox`) | cutscenes (letterbox widget 0x373) |
| SW+0x3D84 input lock | grf bit 1; recon mode increments it directly | cutscenes, job banner, recon mode |

Observed `[handsoff, nopause, letterbox, input]`: free roam [0,0,0,0]; recon dialogue [0,1,0,1]; cutscene [1,1,1,1].
`IsPoInvulnerable` is true while hands-off.

**Deferred callbacks**: `PostSwCallback` appends `{object, pointer-to-member, arg}` to SW+0x3CA8; `EnsureSwCallback`
dedupes; `ClearSwCallbacks` removes by mask. `ProcessSwCallbacks` drains the list (repeating while callbacks post new ones),
then runs Splice callbacks scheduled with `ScheduleSwSpliceCallback` whose time has come.

## 6. The object model

```
BASIC (0x0)                       vptr at +0xC; slots 1-6: refcount hooks, Splice event, messages, class info
 ├─ SW (0x1)                      the world (not an LO in Sly 2)
 ├─ CM (0x87), SAA family, POCTL, ASEGA, SMA, BHV (0x9B) + 43 behaviours, screens, ...
 └─ LO (0x2) level object         oid, parent, hierarchy, Splice frame, activation, options
     ├─ XFM (0x3) LO + matrix     44 lightweight classes: LIGHT, PROXY, SHAPE (PIPE, RAIL), TN, TARGET, VOL, EMITTER, ...
     ├─ ASEG, SM, WR, WIDGET (HUD), JOB/TASK (0x84/0x85), ...
     └─ SO (0x30) solid object    XF array, collision parts, physics record, model (GLOBSET), zaps, fading
         ├─ MS, BOMB/MISSILE, BARRIER, RIPG, pickups (DPRIZE family), ...
         └─ PO (0x32) playable    POCTL controller, HP/MP, character switching
             └─ STEP (0x33)       heading/turn, ground contact, behaviour stack (BHVSTK)
                 ├─ STEPPLAYER (0x34)  shared movement components, gadgets
                 │    └─ JT (0x35) Sly, MURRAY (0x36), BENTLEY (0x37)
                 └─ STEPGUARD (0x38)   perception, flags, engagement; GOMER (0x39) + 10 subclasses
```

The class list with sizes and factories is `include/sly2/classes.h` and
[`app/class_registry.cpp`](../src/game/app/class_registry.cpp).

**Descriptors and vtables** `[C:runtime]`:
- D_002BAAE8 holds 199 records `CLASSINFO {pclassinfoSuper, cid, cidMax, cb, pfnNew}`. CIDs are numbered in **pre-order** of the
  class tree, so `cidMax` (end of the subtree) makes "is derived from" a range test (`FIsBasicDerivedFrom` 0x132B10). 73
  classes have no factory (abstract or embedded).
- Factories (0x110AB0-0x1139D8) allocate `cb` bytes from the world heap and store the vtable at +0xC; the 199 descriptor
  getters (0x113A10 + 0x10*cid) are what slot 6 returns.
- **The vptr sits at +0xC** of every BASIC-derived object, after BASIC's three data words: +0x0 `CRefPtr<CFrame>` (Splice frame),
  +0x4 `CRefPtr<CSidebag>` (extra script options), +0x8 subscriber list for messages. Non-BASIC helpers (HEAP,
  CBinaryInputStream, RIPs, camera policies) put the vptr elsewhere.

**LO** (0x50 bytes): +0x10 parent SO; +0x14 LOROOT (shared by a hierarchy; pooled in SW); +0x18 oid; +0x1C/+0x20 oid-hash and
class-list links; +0x24 link in the parent's child DL; +0x28 64-bit flag word (bits 52-55 `busyk`, 56 removed, 60 in-world;
+0x2D `grfvis`); +0x30 next Splice update time; +0x34 deferred-callback list; +0x38 `sBusy` activation radius; +0x3C proxy
root; +0x40 tagged word (odd: flags; even: pointer to its update-table entry).

**The LO virtual interface.** Slot indices hold across every LO subclass (LO 48 slots, XFM 50, SO 75, PO 81, STEP 87,
STEPPLAYER 105, JT 107, STEPGUARD 89) `[C:static]`, so naming a subclass's Init/Load/Update is mechanical:

| Slot | Role | LO implementation |
|---|---|---|
| 3 | HandleSpliceEvent | `HandleBasicSpliceEvent` 0x1331F8 (LO override `HandleLoSpliceEvent` also notifies `g_game` for events 5/6) |
| 4 / 5 | SendMessage / HandleMessage | `SendLoMessage` 0x133508 / empty |
| 6 | class info | descriptor getter |
| 7 | Init | `InitLo` 0x1A5270 |
| 8 / 9 | Add / Remove (into / out of the world) | `AddLo` / `RemoveLo` |
| 10 | SetParent | `LO_SetParent` 0x1A5900 |
| 11 / 12 | AddHierarchy / RemoveHierarchy (set/clear bit 60) | 0x1A5800 / 0x1A5848 |
| 14 / 15 / 16 | LoadFromBrx / PostLoad / Update | `LoadLoFromBrx` / `PostLoLoad` / `UpdateLo` (fires the Splice update event) |
| 17 | copies bit 2 of the parent's +0x40 word (Sly 1's ProjectTransform slot; no matrix math) | 0x1A5E80 |
| 19 | RenderSelf | empty |
| **22 / 23** | **OnRemove / OnAdd** (object stops / starts being live) | `OnLoRemove` / `OnLoAdd` |
| 29 / 30 / 31 | TranslateToPos / RotateToMat / world matrix | |
| 40 / 41 | CloneHierarchy / Clone (copy the body past +0x50) | `CloneLoHierarchy` / `CloneLo` |
| 48+ | class-specific (SO adds 49-75, PO 76-81, ...) | |

REFUTED: Clone at slot 40 (it is 41); slot 3 = SendMessage (it is 4).

**Messages vs Splice events.** Slot 4 delivers a numeric message to the object's own slot 5 and to every subscriber. Slot 3
looks up a script binding for an event symbol in the object's Splice frame and runs or schedules it. `vt3(k,0,0,0,0)`
calls are Splice events (5 trigger, 6 untrigger), not messages.

**Creation.** `PloNew(cid, parent, oid, isplice)` 0x13E3C8: factory, set parent/oid/Splice frame, link into the oid hash and
class list, vt7 Init, vt11 AddHierarchy if the parent is in the world, vt23 OnAdd if now live. Hierarchy lives in DLs
(roots at SW+0x10, children at SO+0x64). `DeferLoRemove` queues a removal; `BindLoOidTable` resolves `{grf, id, ib}` tables
of OID references into pointer fields at PostLoad.

**Activation (OnAdd/OnRemove)** `[C:verified]`. `IsLoInWorld` 0x1A61A0 is true only when bit 60 is set **and** the object's
LOROOT is in the active-roots list SW+0x2C. Slot 23 fires exactly when that flips false->true, slot 22 when it flips back;
they always alternate. It flips on structural changes (`AddLo`, `RemoveLo`, `LO_SetParent`, `PloNew`) and on **proximity**:
each frame `UpdateSwBusyList` walks the busy-candidate list incrementally and, per root, `busyk` decides: 1 always active,
2 never, 0 active if `grfvis & g_cm.grfvis` and the distance to the current player is within `sBusy`, 3 the same against
the camera. Base `OnLoAdd` inserts the object into the SW update table and `OnLoRemove` takes it out, so **only live objects
get Update and RenderSelf**. SO adds itself to the render tree and contact registration; TARGET, PIPE, RAIL, LIGHT, the
grab spots and STEPGUARD add themselves to per-class lists. Sly 1 had OnAdd/OnRemove plus a separate `FreezeAlo`; Sly 2
merged them. In the Paris hub 1,743 of ~8,160 objects were live; guards and pickups are pooled and toggled, never freed.

**Proxies.** A world entry is a proxy source loaded `c8` times. PROXY objects (CID 0x6/0x7) take a spare copy on demand
(`AcquireProxySpare`) and release it; `idProxy = (isrc << 16) | icopy`. `AcquireProxyObject` 0x1CB250 is the generic spawn
path (thrown objects, markers, coin sprays). Bosses and adds respawn as fresh proxy copies.

**Finding objects.** `PloFindSwObject` 0x15E938 resolves OIDs through the oid hash; `CploFindSwObjectsByClass` walks the
per-class lists. Runtime-spawned BLIPG/RIPG objects are on class lists but not in the oid hash.

**XFM** (CID 0x3, 0xE0): LO + parent XF, `mat` +0x60, `matWorld` +0xA0. `GetLoWorldPos` / `GetLoWorldMatrix` return slot 31's
world matrix (position = row 3). Transform constraints: `LoadXfmConstraintsFromBrx`, `ApplyXfmConstraint`.

## 7. Subsystems

Directory names are under `src/game/`; the layer rules are in [architecture.md](architecture.md).

### 7.1 Core library (`platform/memory`, `core/*`)

- **Two bump heaps, no free lists.** The world heap grows up from `_end` and is reset at level unload (`FreeSw`); all `new`
  goes there (zeroed, 16-aligned). The stack heap grows down from 0x2000000, scoped by an RAII helper. `malloc` returns NULL,
  `free`/`delete` are no-ops. `GetFreeMemorySize` (the gap between the heaps) gates movie playback.
- **Slot heaps** (`slotheap.cpp`): fixed-size pools with lazy block allocation, used by every TU's static init (Splice pools)
  and many engine pools (LOROOTs, callbacks, sounds, stimuli, JSGs).
- **Containers** (`dl.cpp`): Sly 1's doubly linked list (`DL {pvFirst, pvLast, ibDle}`, iterator fix-up via `g_pdliFirst`)
  plus a new singly linked list SL used by the SW class lists, oid hash and scheduled callbacks. `CByteQueue` for streaming.
- **Math**: mat/vec/util/bez/crout/bsp/dzg (LCP) in Sly 1 order. Sly 2 passes VU vectors **by value** where Sly 1 passed
  `VECTOR*`, so Sly 1 mangled signatures must not be applied blindly `[C:verified]`.
- **Streams and CD**: `CBinaryInputStream` (vptr at +0x30; typed readers, `Align`, LZSS `SetDecompress`); a CD-backed subclass
  for all world loads (`FOpenSector`); `CBinaryAsyncStream` spools FMV. The only sector reader is `sceCdRead` through a retry
  wrapper. There are no file names on disc (`GetFileIdName` returns "fid:#unknown"); files are found by FILEREC
  ([formats/disc-records.md](formats/disc-records.md)).

### 7.2 Splice scripting (`splice/`, `app/bif.cpp`, `object/spliceobj.cpp`)

Splice is Sucker Punch's Scheme-like level-scripting language. Levels carry compiled Splice; objects carry a frame of bindings;
scripts react to events, read and write object options, and build RELAY sequences and JOB/TASK records.

- **Layout**: native binding thunks (0x1002A8-0x110A28, `src/generated/splicewrap/`) convert script arguments
  (`RefSetArgsFromSplice`) and call engine functions; the builtin table (169 entries, D_002BBB90, `bif.cpp`); 29
  byte-identical nil-stub builtins (slots 140-168, probably compiled-out dev builtins); evaluator, values, frames, GC, pairs,
  procs, sidebags and serialization in Sly 1's alphabetical order; object options (`spliceobj.cpp`).
- **Values**: `CRef = {TAGK tagk; union {int n; float g; void *pv;}}`. Sly 2 renumbered the tags (one special form inserted at
  6) `[C:verified]`: 0x15 S32, 0x16 F32, 0x17 Bool, 0x18 Symid, 0x19 Bifk, 0x1A Vector, 0x1B Matrix, 0x1C CLQ, 0x1D LM,
  0x1E SMP, 0x1F Pair, 0x20 Proc, 0x21 Basic, 0x22 Method, 0x24 Void, -1 Nil. Special forms: 1 set!, 2 define, 5 assert,
  7 if, 8 or, 9 and, 10 cond, 11 else, 12 case, 13 let, 14 while, 15 lambda, 16 begin, 17 import, 18 quote.
- **Evaluator** (`RefEval` 0x127838, `RefEvalApply`): Sly 1's eval.cpp one-to-one. Quirks `[C:static]`: `if` takes THEN only
  when the payload word **== 1** whatever the tag; `and`/`or`/`cond`/`not` treat only Bool #f as false; `while` evaluates only
  its first body expression; `+`/`-` accumulate ints in a float; `=` compares raw payload words (`(= 1 1.0)` is #f); tag 0x14
  is the rest-parameter dot; unbound symbols evaluate to nil.
- **Builtins** `{pfn, cargMin, fRest}`: 0-40 identical to Sly 1's 125-entry table; 96 `current-time`; 97-104 zero-arg getters
  (level index, episode, world, current JOB / TASK / pending TASK, difficulty); 105-107 schedule/cancel callback, defer
  update; 108-111 object options; 112-122 find-object family; 131-139 save-var get/set, bitwise ops, power-up flags.
- **Options**: objects expose numbered properties through `g_aeopid` (0x28-byte records: type, flags, field offset or
  getter/setter pointer-to-member). Level files set them at load (`LoadOptionsFromBrx`); scripts use them through the option
  builtins; ids above 0x4D0 go to the sidebag. Option ids are **polymorphic per class** (446 = start follow on STEPPLAYER,
  escort point on STEPGUARD) and do not line up with Sly 1's.
- **Events**: event numbers 0/1 post-load and update, 5 trigger, 6 untrigger, 17 game event, 19 activate, 20 deactivate.
  The serialized form, symbol tables and decoder are in [formats/splice-scripts.md](formats/splice-scripts.md).

### 7.3 Rendering (`engine/graphics/*`)

Objects queue **render-list entries** (RPLs); `DrawSw` sorts them into 26 **kinds**, each kind maps to one of 12 **passes**; a
pass hook uploads a **VU1 microprogram** and GS state; per-entry DMA chains feed vertices to it. (PS2 primer: the EE builds DMA
chains; VIF1 unpacks data into VU1 memory and starts microprograms, which XGKICK GIF packets to the GS rasteriser: 4 MB local
memory, **no stencil buffer**, blend `(A-B)*C+D`.) A port replaces microprograms with shaders and GS register state with
pipeline state; the port checklist is in [guides/port-plan.md](guides/port-plan.md#66-render-port-checklist).

- **Frame**: `OpenFrame` -> `RenderSw` (light-tree update; queue a RELIGHT entry per lit model when illumination changed; vt19
  on live objects; `RenderSoSelf` draws the SO's GLOBSET if `grfvis & g_cm.grfvis`) -> `DrawSw` -> `DrawUi` -> `CloseFrame`.
  Display: field-rendered 512x224 double buffer, 32-bit Z.
- **RPL** (D_003B3980, 0x2000 x 16 bytes, `AllocRenderListEntry`): `{kind; z; per-frame packet; static load-time chain}`;
  z = squared camera distance (HUD: layer << 18).
- **`DrawSw`** ([`draw.cpp`](../src/game/engine/graphics/pipeline/draw.cpp)) `[C:static]`: counting-sort into 26 buckets
  (`CprplPartitionArpl`; far-to-near on buckets 1, 2, 5, 6, 10, 25; near-to-far on 21); clear colour+Z if needed; upload VU1
  globals (viewport, world->clip, camera matrix, ambient, framebuffer words); then per entry `pass = D_002BDD20[kind]` and on a
  pass change call the old pass's end hook and the new pass's begin hook (table D_002BDD88). **This table is the main
  extension point for a native renderer.**

| Kind | Pass | Role |
|---|---|---|
| 0 | P4 RELIGHT | compute-only vertex relighting |
| 1 | P3 BACKGROUND | sky dome and colour fill; no depth test or write |
| 3 / 4 | P0 GLOB / P11 CUTOUT | opaque world / alpha-tested geometry (marks dest alpha) |
| 5 / 6 / 10 | P0 / P11 / P0 | translucent, translucent cutout (faded 3/4), late translucent (glass, water, decals); far-to-near |
| 7 / 8 | P6 / P7 VOLUME | shadow volumes, channels A / B |
| 9 | P1 CEL BORDER | outlines |
| 11, 13, 14 | P0 | late opaque; post-blip; screen tint/flash/fade |
| 12 | P2 BLIP | sprite particles |
| 15-20 | P8/P9 (+P0, P1) | render-to-texture slots (128x128, sampled next frame) |
| 21 | P5 HUD | widgets, rich text, sprites |
| 22-25 | P10 FOREGROUND | objects drawn over the HUD with Z cleared |

- **Vertex formats**: the A family (static, unskinned) is 4 qw per vertex: ST, colour 0, colour 1 (the two relight outputs),
  position with a **W flag word** (magic float carrying strip restart, culling marks and the 8-bit fog value); transformed in
  place into the GIF stream. The B family adds skinning (4 matrices), billboards, UV scroll, morph deltas, view-angle alpha
  and per-object lights (<= 4). Z is "bigger = nearer".
- **Lighting ("relight")** computes two colour sets per vertex only when lights or ambient change: per light
  `I = clamp(k0 + k1*(d + d^3), 0, 1) * att`; light fills the headroom first, ambient only the rest. **There is no banding or
  quantisation in the VU code**: the cel look comes from the art, the steep light curve, that fill rule, and the outlines.
- **Outlines**: per edge record (two endpoints, two opposite vertices) the edge is drawn as one GS line iff the two faces face
  opposite ways on screen (silhouette), offset ~0.5 px; an antialiased colour pass, then a depth-only pass.
- **Shadow volumes**: with no stencil buffer, Sly 2 counts in the framebuffer's **green channel** (+2/-2 per face by winding,
  Z-fail, wrapping), moves the result into destination alpha, then applies a 3-entry shadow CLUT in full-screen sprites. The
  darkening is proportional to the surface's direct light, so ambient-only surfaces are not darkened.
- **Textures**: loaded per world entry by `LoadShadersFromBrx` ([formats/textures.md](formats/textures.md)), deduplicated by
  hash and uploaded to GS blocks (`IgsAllocGsb`); the sky's textures sit in GS memory that overlaps the Z buffer and are
  re-uploaded every frame before a Z clear. Shader animations (SAA family: LOOP, PINGPONG, SHUFFLE, EYES, SCROLLER, CIRCLER)
  switch bitmap frames or CLUTs.
- **Models**: GLOBSET at SO+0x80 (sub-models with subtype mesh / outline edges / shadow volume, authored kind, flags); culled by
  a VU0 sphere test that also yields a fade value ([formats/meshes-scene.md](formats/meshes-scene.md)).

### 7.4 Camera (`engine/camera/`)

`g_cm` (CM, CID 0x87, BASIC child, 0x630 bytes, not an LO): fov (+0x20, 1.1), near/far clip (52 / 52000), matrix and
position, fog. Level options 161-173 set fov, clips, fog, MRD ratio, pose. The camera is a **priority stack of policies**
(`acpr[8]` at CM+0x310, 16-byte `{prio, policy, focus SO, pv}` entries). Each frame the top policy's Update gets the focus
object's camera definition (CPDEFI, from SO slot 50, e.g. `GetStepplayerCpdefi`) softened by adjust springs; cm.cpp then
builds the view/projection matrices and frustum and clips the eye against collision. A policy is a small C++ object
(vptr +0xC; slots Init, Activate, Deactivate, Set, Revoke, Update). Embedded policies: CMPART1 default follow (priority 0),
CPTN tunnel camera driven by TN volumes, CPLOOK binocucom (priority 7), the recon camera (8), CPTURRET (9), CPCAMBLEND
cutscene blends (10), CPCAMERA fixed marker camera (safehouse, 10), a hide-under-table policy (10), CPTWOSHOT.

### 7.5 Animation (`engine/anim/`)

- **Skeleton = the SO's XF table** (`LoadSoXfsFromBrx`): an XF is one degree-of-freedom group (translate, rotate (Euler),
  scale, shear, pivot), not a bone; a joint is a translate -> rotate -> scale stack. Row vectors, world = local . parentWorld.
  XFs are updated per frame by role (velocity, chase, spring, chain, Hermite) through actuator hooks sorted by priority.
  Ropes/tails are XF chains with damping. Two-bone IK (`SolveIk`, law of cosines) runs after FK.
- **ASEG** (CID 0x6D, a clip loaded from the level): channels keyed by SO oid + XF, each adding an actuator whose value
  **replaces** the XF value; curves are constant or cubic Hermite, times in 1/60 s ticks. **ASEGA** (CID 0x86, refcounted) is
  an application of a clip to an object tree, effective speed = speed x master x k; `ApplyAsegCur` is the usual entry.
  ASEGA events (`HandleAsegaEvent`) trigger sound start/stop, show/hide with fade and object toggles.
- **Blends**: ASEGBL blended segments (`EnsureAsegBlendDynamic`), 8-direction locomotion blend, GAIT walk/run weights and
  footsteps, LEAP (animation-driven jump), DIRANIM (best of 4 directional clips).
- **SM** (CID 0x6F): states and weighted transitions loaded from the level. **SMA** (CID 0x88) plays the ASEGA of the current
  transition and picks the next toward a goal state using precomputed state-distance tables (new in Sly 2). SMs drive task
  visibility records, buttons, guard animation bindings, Sly's state changes and dialogue lip-sync.
- Curve and actuator classes (72 functions, vptr at +0) are clear as families only; actuator slot 5 is a goal query, not an update.

### 7.6 Physics, collision and navigation (`engine/physics/`)

- **SO** (CID 0x30, 0x150 bytes, 75 slots): XF array (+0x50/+0x54), child DL, GLOBSET (+0x80), collision parts (0xB0 each),
  an optional physics record SOPHYS (+0xE8: inertia, gravity, mass; 462 of 1,196 SOs have one), a 26-entry zap response table,
  alpha with fade-out removal, an external-acceleration list (water currents). SO-only slot roles include 50 GetCpdefi,
  61 `FIgnoreIntersection` (tri-state), 70 PresetAccel, 71/72 external acceleration, 75 `CanSoTakeDamage`.
- **Pipeline**: broadphase sweep-and-prune over three axes (`bbmark.cpp`) -> contact generation (`mark.cpp`: BSP-BSP,
  sphere-BSP, sphere-sphere) -> `SolveSw` flood-fills contact groups (up to 64 contacts) and solves each with the DZG LCP solver
  (`SolveSwGroup`).
- **Zaps** (damage): `DeliverZapToSo` 0x1F7598 delivers `{source, part, target, kind bits, dhp}`, dedupes through a 0.25 s
  history (`ExpireSwZapRecords`), and each SO answers through its response table. Kinds seen: 0x2 Murray takedown, 0x100 guard
  projectile, 0x200 guard melee, 0x1000 laser, 0x2000 water, 0x80000 crossbow dart, 0x200000 juggle (floors hp at 5).
- **Geometry**: VOL (CID 0x16, containment volumes), SHAPE (CID 0xA, polyline/Hermite paths with closest-point queries; absorbed
  Sly 1's CRV), WATER (CID 0x59), launch kinematics, convex volumes as pathzone obstacles.
- **Navigation** (`path.cpp`): PATHZONE (CID 0x11) navigation graphs with A* (Sly 1 path.cpp). New in Sly 2: a world zone BSP
  whose leaves are pathzones (every SO knows its zone, `FindPathzoneAtPos`), dynamic obstacles, and inter-zone routing over
  pathlink objects (CID 0x7C, `FindPathlinkRoute`) used by guard AI.

### 7.7 Sound (`engine/sound/`, `src/sdk/989snd`)

Sly 2 rewrote Sly 1's C sound code as C++ classes with a request queue. Effects come from a per-level 989snd bank; music,
dialogue and ambience stream from disc through Sucker Punch's IOP module (RPC 'SUKR'). `g_sndmgr` holds instance pools, the
bank, a music stack (8 deep), dialogue and ambience streams and an 8 category x 4 channel volume matrix. **SND** (CID 0x97,
refcounted) keeps a flag word as a **request queue** (start, pause, continue, stop, volume/pan/pitch dirty, positional);
`SetSndFlags` links it into a dirty list and `UpdateSnd` turns bits into virtual calls. BANKSND plays bank sounds
(`PsndStartSound` then `SetSndSourceLo`, range, volume, pitch); STREAMSND streams dialogue (with lip-sync SM roles), music
(layered by player state) and ambience (6 groups mixing the loudest of up to 8 sources). The rhythm minigame times against
music time (`TMusicCur`), not the game clock. The 989snd client uses Sly 1's command ids plus three new ones. Formats:
[formats/audio.md](formats/audio.md).

### 7.8 Particles and effects (`engine/fx/`)

EMITTER (CID 0x1D) keeps Sly 1's emitter.c structure and spawns RIPs and BLIPs; the same file holds the explosion family
EXPL/EXPLG/EXPLO/EXPLS (EXPLS is the most common class in the hub). RIPG (CID 0x57) owns a list of RIP particles whose
11 types got real C++ vtables (Sly 1's VTRIP function tables; slots Init, PostEmit, OnRemove, Project, Update, Render, Touch,
Bounce); RIPs draw through the SO model path (`RenderRip`). BLIPG (CID 0x1C) sprite particle groups render as kind 12.
Trails (CID 0x69) keep a 60 Hz ring of world matrices and deform their mesh to follow it.

### 7.9 HUD and UI (`engine/presentation/`, `engine/font/`, `ui/`)

- **WIDGET** (CID 0x7E, LO child; Sly 1's BLOT) and subclasses 0x7F-0x83: state +0x58 (0 unloaded, 1 hidden, 2 visible,
  3 appearing, 4 disappearing), show/hide requests, slots 49/50 Show/Hide. 0x80 is the job-completion banner (pauses the game
  clock via rate slot 7); 0x81 the objective counter (recounts complete tasks every frame and formats "done/total"; the count
  is stored nowhere); 0x83 the episode menu. The SW HUD stack slides the visible coin/clue/countdown widgets into place.
- **Text**: STRINGGEN (108 `{id, getter, context}` entries at D_003E1C88; e.g. `BuildCoinCountText` reads `loot.ccoin`),
  `CFont`/`CRichText` in Sly 1 order, Shift-JIS helpers, language/region table.
- **Screens**: a BASIC-derived screen family (no CIDs) on an active list D_003D9E70 with focus switching, updated by
  `UpdateScreens`. The pause menu sets clock slot 0 = 0 and hands-off (the world keeps rendering); ThiefNet lists gadgets
  7-42 unlocked by `episodeBest` and not owned, and buying subtracts from `loot.ccoin` and sets the power-up bit
  (`UpdateThiefnet`, `RefreshThiefnetItems`). The loading screen and `CProg` progress bar live in `prog.cpp`.

### 7.10 Player characters (`actor/`, `gameplay/player/`)

`SO -> PO (0x32) -> STEP (0x33) -> STEPPLAYER (0x34, abstract) -> JT (Sly) / MURRAY / BENTLEY`. STEPPLAYER is new: Sly 1's JT
sub-structs were lifted into a shared base, so all three run the same movement components.

- **Who is played**: `g_pjt` is **always Sly** (REFUTED: "the current player"); the PO with control is SW+0x3EFC
  (`GetHeroPo` 0x1C4840), published as the save var `oidPoCurrent` (7 Sly, 8 Bentley, 9 Murray). Which PO you play comes from
  the save, not the level. Non-current players are out of the world, parked at the safehouse.
- **POCTL** (CID 0x89, at PO+0x150): filtered stick (+0x18 heading, +0x1C speed), button bits `grfbtn` (0x40 jump, 0x20
  circle/action, 0x80 square/attack, 0x10 triangle, 0x08 run modifier, 0x04/0x01/0x02 gadget slots, 0x400 look mode), press
  timestamps; state 1 only for the current player. AI partners write a non-current player's POCTL directly.
- **PO** slots 76-81: `IsPoInvulnerable` (cheat bit or hands-off), `JthsCurrentPo`, OnActive, SetCtlState (controller
  hand-off, also to minigame POs), can-be-healed, `AddPoHpMp`.
- **STEP**: heading/turn control, ground contact, gravity and spin ratios, and the behaviour stack BHVSTK at +0x26C shared with
  guards (`PushStepBehavior`, `FindStepBehavior`).
- **STEPPLAYER components** (fixed offsets, live in all three characters): +0x2C0 JTAIR (jump/fall/double jump/homing/ledge
  hang), +0x4C0 JTGRAB (wires, wall press, hiding, "use gadget on object"), +0x9E0 JTPLACES (32 recently stood-on places, used
  to return the player after falling in water), +0xDF0 JTHURT (damage states, death), +0xF00 JTMODE (look mode: kind 0
  binocucom, kind 1 script-started recon), +0xF50 JTPIPE, +0xFD0 JTGOAL (walk-to-goal), +0x1180 three GADGETSLOTs, +0x11B0
  JTTHROW, GAIT blocks. HP/MP are bound to save vars `<char>.hpCur`/`mpCur` (max hp 40 Sly/Bentley, 60 Murray).
- **Gadgets**: `g_agadgetdesc` = 43 x 0x20 `{mode (-1 passive, 1 hold, 2 press), owner oid, coin price, MP cost, unlock
  episode, name, description, icon}`; owned = bit n of `powerUps.mppukfAcquired`.
- **Sly (JT)**: state `jts` (`SetJtState`): 0 stand, 1 run, 2 air, 4 hurt, 5 fight stance, 7 grab, 8 pipe, 11 pickpocket, 12 walk,
  13 air smash. Cane attacks are an overlay on stand/fight stance (`jtcs`), with a timed hit window. Thief Reflexes is
  `SetClockRate(slot 1, 0.25)`: everything including Sly slows; only the camera clock keeps speed.
- **Murray**: one state machine (`UpdateMurrayStateMachine`): idle, run, jump, air slam (zaps contacts below him), lift, carry,
  throw, walk, attack (square combo, triangle uppercut, takedown finisher), gadget states. Takedown: the uppercut puts a guard
  in the juggled behaviour with hp floored at 5; the finisher kills (`TryMurrayTakedown`).
- **Bentley**: state machine at +0x13D0; Trigger Bomb is a pooled tossable (CID 0x65, `SetTossableState`: pooled, held, flight,
  stuck, detonate); the crossbow dart is an ACCMISS fired from the binocucom (`FireBentleyDart`). REFUTED: "Bentley throws
  real BOMBs".
- **AI partner**: `UpdateStepplayer` calls the top BHV's slot 14 with the PO's POCTL; e.g. Murray following Sly runs
  [follow (BHV194), walk-to-goal (BHV168)], started from Splice option 446 (`StartStepplayerFollow`). No guard root BHV on
  player stacks.
- **Grab spots**: GRABSPOT is a mix-in with its own small vtable (wire = RAIL, lamp post = PIPE, walls, the table); carried
  props use trigger grab spots, which also serve as **mission trigger spots** (`FireTriggerGrabspot` -> task request).

### 7.11 Guards and AI (`gameplay/ai/`)

Sly 2 replaced Sly 1's per-guard state machine with a **stack of behaviour objects** (BHV) per character, plus a guard class
that hosts perception, targeting, flags and engagement rules.

- **STEPGUARD** (CID 0x38, 0x1010 bytes, 89 slots; new virtuals: choose target, sense stimulus). Subclasses, one per named
  character: GOMER (swarmers), GUARD (plain guards), DIMITRI, NEELA, RAJAN, JEANBISON, CARMELITA, CONTESSA, ARPEGGIO, BEAR,
  BEARCUB. Species are proxy copies of one template per world entry (hp 40 / 30 goat / 5 cobra). Only a few guards are in the
  world at a time; the guard list (D_003E1444, maintained by `OnStepguardAdd`/`OnStepguardRemove`) holds exactly those, and
  the guard manager (`UpdateGuardManager`) removes or respawns inactive ones.
- **Flag word** (+0x2C0, 64 bits): BHVs raise their flag in Init and lower it in Remove (`AddStepguardSgf`/`RemoveStepguardSgf`),
  so **the word always equals the union of the flags of the BHVs on the stack** `[C:runtime]` (59/59 states).
- **The stack**: `BHVSTK {pstep; cpbhv; apbhv[8]}`; behaviours are placement-new'd by a CID factory (CIDs 0x9C-0xC6), popping
  one pops everything above, `UpdateBhvstk` runs top first and messages stop at the first handler. A busy behaviour increments
  `cBusyAbove` of those below. BHV slots: 7 Init, 8 Start, 9 Close, 10 Update, 13 HandleMessage, 14 UpdateInput, 15 SetState.
- **Awareness**: the root BHVGUARD holds the mode and stack slot 1 is the mode behaviour: 1 calm (BHVPATROL, runs the guard
  script), 2 suspicious (search: go to the last known position, look, give up), 4 alert (BHVCOMBAT: close, melee, ranged,
  pursue, give up), 7 juggled, 11 KO (flung, removed ~1.7 s later), 5 script mode.
- **Combat**: two alerted guards share an engagement record and **take turns attacking** (`DoesTargetRecordAllowGuard`). Attacks
  are CID 0x78 entries (range, height, angle, clip). Ranged attackers throw pooled BOMBs or fire ACCMISS from a launcher
  (`LaunchSoFromLauncher`). Combat guards emit alert stimuli (`EmitStepguardStimulus`). Gadget effects set a special state
  whose value is the gadget id (`SetStepguardSpecial`).
- **Perception**: vision through a DETECTOR (CID 0x10) per guard with per-target states 0 none / 1 seen / 2 lost; hearing
  through refcounted stimulus objects made from templates (radius, height range, ramp time; `CreateNoise`), which searching
  guards watch and re-target to newer ones (the Alarm Clock gadget works this way). Laser rigs (LASEN, CID 0x55) zap whatever
  cuts a beam (kind 0x1000).
- **Pickpocketing**: each guard has a loot list (coin entries and loot items keyed to save vars `loot.mplootkc[k]`); the pouch
  is a CID 0x63 SO child. **Guard scripts** (CID 0x71) are command lists built from Splice and run by BHV169; spawners
  (CID 0x77) re-arm concealed spawn points.

### 7.12 Game progression (`world/state/game.cpp`, `gameplay/progression/`)

- **JOB** (CID 0x84) and **TASK** (CID 0x85) are team names for a mission and a mission step. Save names follow
  **`m<N>_<name>` = JOB, `t<N>_<name>` = TASK** (each has save-var refs such as `focus_count`, `is_complete`, `state`). A TASK has
  a state, prerequisite and successor arrays, an objective text id, a marker, and +0xC0 = **oid of the PO the player controls**
  for the task (7/8/9 or a minigame PO). Task states `[C:runtime]`: -1 unset (until `PostSwLoad`), 0 locked, 1 available,
  2 complete within the running job, 3 complete after the job finished; requests 4/5 activate/deactivate.
- **Visibility records** `{object, context, mask of states, level}` put objects in the world only in some task states; RSMG
  records drive state machines on state changes. Only the available task's trigger spot is in the world, so objective order
  is fixed even when the HUD reads like a free counter.
- **Job completion** `[C:runtime]`: the last task completes -> banner widget with the clock stopped -> when the banner ends,
  `FinishJob`: tasks -> 3, game event 0x20, `AutoSaveGame`, next task or warp.
- **Game event bus**: `BroadcastGameEvent(&g_game, id, arg)` delivers to the game, the difficulty system, every available task
  of the current job and the job; the live consumers are Splice handlers. Known ids: 0/1/2/4/5 startup / teardown / load
  begin / post-load / transition done; 16 hero died (jobs restart); 21 guard damaged; 23/24 guard defeated; 0x20 job finished;
  0x28/0x2C pause shown/hidden; 0x3C gadget used.
- **% complete** (`GetEpisodePercentComplete`) counts TASKs with successors in state 3, so progress counts only after a job's
  banner has finished; the episode menu shows 95% of it + 5 if the episode's vault power-up is owned.
- **Difficulty** averages 3 saved samples (`suck_history.mission_0..2`) changed by event deltas; `NScaleByDifficulty`
  interpolates gameplay values. **Collectibles** (`prize.cpp`): DPRIZE family (coins pooled as 32 proxy copies, keys, medicine,
  loot, clue bottles, vaults); breakable props (CID 0x62) spray prizes from proxy pools (`SprayDprizes`). **BOOTY** (CID 0x67)
  is a carried object.

### 7.13 Save data and memory card (`world/state/save.cpp`, `ui/memcard.cpp`)

All persistent progress is **one flat blob** `g_gs` (0x5400 bytes, then schema version and checksum) addressed by **named
variables** whose layout comes from a schema in every world file (SW+0x3A18; a 53-bucket symbol hash of 960 symbols and a trie of
5,461 entries `{sym, kind, cbits, byte offset, child, sibling}` yielding 4,472 variables) `[C:runtime]`. A changed schema wipes
the blob (`ResetGsIfSchemaChanged`). Access is through `RefGetSaveVarV("name", ..., NULL)` / `RefSetSaveVarV` /
`GetSaveVarAddressV` and Splice builtins 131/132; `ResolveSwSaveVars` caches hot keys in SW slots. `ResetGs` keeps the 11
option variables. The variable list is in [formats/save-variables.md](formats/save-variables.md).

Memory card: files `save0..2` are exactly **0x5408 bytes = the raw blob + the 8-byte schema fingerprint**, with **no checksum,
compression or encryption** `[C:static]`; on save every `<task>.from_memcard` byte is forced to 1 (`BuildMemcardGsMask`).
`g_save` runs a state machine from `UpdateSave` and starts a worker thread per job (get info, check, format, write);
directory `/BASCUS-97316YAOTWTD!`, icon copied raw from disc.

### 7.14 Cutscenes, sequences and movies (`gameplay/cutscene/`)

- **RELAY** (CID 0x72) holds a flat array of 0x50-byte commands in nested blocks, built at load from object records (METHOD
  options) or Splice through ~90 builtins (one per command kind 0x00-0x5A). `FireRelay` spawns a **JSG** interpreter (slot
  heap) that steps through commands while each is async or complete (`NextJsgCommand`, `IsJsgCommandComplete`) and can spawn
  more JSGs; `RemoveJsg` releases everything a script holds. Commands cover actors and teleports, movement (pushing BHVs),
  clips, state machines, widgets, hands-off, camera cuts/blends/tweens, sound and dialogue lines, hint boxes, task states and
  controller hand-off (`SetCtlState`). **Cutscenes complete tasks themselves**; a TASK's Splice fires them. Camera blends push
  CPCAMBLEND (angles in degrees, vertical fov in radians).
- **FMV**: no FMV is used for mission sequences; the disc holds 2 movies. `CMpeg` is Sly 1's mpeg.c almost 1:1 plus subtitles
  (`DrawMpegSubtitles`); it refuses to play when free memory is short.
- **Transitions**: the frame-capture transition (CID 0x68: capture the framebuffer, split/peel effect), wipes (section 5),
  letterbox.

### 7.15 Bosses and minigames

Six boss fights, each a JOB or tasks inside a heist JOB; all reuse generic machinery: STEPGUARD hit points (+0x338 max,
+0x33C current), game events 15/16/21/23/24, a health-bar WIDGET (anim 847), guard scripts (CID 0x71), boss music, and PROXY
copies so retries respawn a fresh boss.

| Boss | Job | Level | Played as | Driver |
|---|---|---|---|---|
| Dimitri | `m1_boss` | L05 | Sly | DIMITRI class: authored flee routes away from the player (`FindDimitriFarthestRoute`) |
| Rajan | `m3_boss` | L12 | Sly, then Murray | RAJAN class (sense range, camera focus) + Splice |
| The Contessa | `m5_heist` boss tasks | L17 | Sly | CONTESSA class + Splice, two battles |
| Neyla (RC duel) | `m6_heist` | L30 | Sly / RC chopper | Splice + RELAYs over generic SOs |
| Jean Bison | `m7_boss` | L37 | Bentley | **pure Splice**: the job script counts 20 indirect hits and advances 4 phase tasks |
| Clock-La | `m8_turret`, `m8_boss` | L38, L43 | Sly (+ scripted Murray/Bentley) | **no actor class**: an assembly of SOs, SMs and targets driven by Splice |

Minigames are entered by handing the controller to another PO (`SetCtlState` from a RELAY, or the task +0xC0 field):
**Bentley hacking** (HACK family CIDs 0x47-0x4C: ship, enemies, bullets, lock, walls; left stick moves, right stick aims and
fires), **mounted turret** (TURRET 0x45 + CPTURRET), **RC combat** (ROBOTTANK 0x4D; player or AI mode), **dance** (DANCER 0x44
+ rhythm manager CID121, judged against music time), the **safehouse** character select and ThiefNet (SAFEHOUSE 0x46). The RC
chopper and the drivable tank are plain POs whose drive code is not identified yet.

Smaller gameplay classes (`gameplay/objects/`): BARRIER, BOMB, MISSILE/ACCMISS (projectiles derived from BOMB), BUTTON and
VOLBTN (fire Splice events 5/6), SENSOR and laser rigs, TARGET, breakable props (CID98, up to 32 stages of recorded actions),
tossable gadget objects (CID100-102), critters (CID96), the screen tint/fade (CID116), the sky dome (SKY).

## 8. Where Sly 2 differs from Sly 1

| Area | Sly 1 | Sly 2 |
|---|---|---|
| Classes | C function-pointer tables, vptr at +0, ALO class | real C++ virtuals (vptr +0xC), no ALO (split into LO + XFM), SW and CM are BASIC children |
| Activation | OnAdd/OnRemove + separate FreezeAlo | one OnAdd/OnRemove pair driven by structure and proximity |
| Level file | one object tree, objects without sizes | world entries instanced `c8` times; every object record carries its byte size |
| Guards | SGS state machine in STEPGUARD | ~43 BHV classes on a per-character stack; STEPGUARD hosts perception and flags |
| Progress | scattered game state | one 0x5400-byte blob of named variables from a schema in the world file |
| Missions | - | JOB/TASK dependency graph + a game event bus |
| Scripting | Splice | same language, renumbered tags, 169-entry builtin table, RELAY/JSG sequences on top |
| Player | JT with sub-structs | STEPPLAYER base shared by Sly, Murray and Bentley |

## 9. Names the research disputes

The research pages argued for these readings; the code keeps a different name for now. Check before renaming.

| Address | Code name | Research reading |
|---|---|---|
| 0x1572E0 | `AllocSw__4DMASi` | allocates from the stack heap (`AllocStack`) |
| 0x1703F8 | `InitZpr__FP3ZPR3ZPKP2LO` | GS block allocator init (`InitGsb`) |
| 0x1B17B8 | `SetStepSpinRatio__FP4STEPf` | sets the spin ratio of the CID96 critters |
| 0x1E0020 | `EnsureSggAlarm__FP3SGGP5ALARM` | RELAY alarm id set used by command kinds 4/5 |
| 0x13C448 | `AddAlarmAlbrk__FP5ALARM3OID` | adds a BOOTY entry by oid (CID 0x67 is BOOTY, not ALARM) |
| 0x147410 | `SetFrameCaptureState` | the DANCER's screen-grab state (split/flip transition) |
| 0x1C4840 / 0x1C4A48 | `GetHeroPo` / `GetCurrentPo` | 0x1C4840 returns SW+0x3EFC, the PO that has control |

## 10. Glossary

Sucker Punch's Hungarian style, inherited from Sly 1 (whose symbols shipped), still shows in type names and the remaining
Hungarian function names. **(team)** marks project coinage.

Prefixes: `P`/`p` pointer (returns or is), `F`/`f` boolean, `C`/`c` count, `G`/`g` float, `I`/`i` index, `N`/`n` integer,
`A`/`a` array, `Grf`/`grf` flag bits, `Mp`/`mp` map indexed by an enum, `Rad` radians, `S`/`s` distance, `T`/`t` time (s),
`Dt` time delta, `U`/`u` 0..1 parameter, `Pchz` C string, `Ref` returns a Splice `CRef`, `Pv` `void*`, `g_` global, `s_` file
static. Methods read `<Verb><Class><Noun>`: `PloFindSwObject` = "pointer to LO: find SW object".

| Abbr. | Meaning |
|---|---|
| BASIC, LO, XFM, SO, PO | script-visible root class; level object; LO + matrix; solid object; playable object |
| STEP, STEPPLAYER, JT, STEPGUARD | walking character; shared player base; Sly; guard base |
| SW, CM, CPLCY, CPDEFI | world; camera; camera policy; camera definition from the focus object |
| BHV, BHVSTK, SGF | behaviour, behaviour stack, guard flag bit |
| XF, SOPART, SOPHYS, GLOBSET | transform record in an SO; collision part; physics record; model |
| ASEG, ASEGA, ASEGBL | animation clip, its application, blended clip |
| SM, SMA, SMS, SMT, RSMG | state machine, application, state, transition, (root, SM, goal) trigger record |
| ACT, SAA | actuator; shader animation |
| ZPK, ZPR, XP, DZG | zap kind, zap record, contact point, LCP contact solver |
| VOL, TN, HND, JMT | volume, tunnel camera volume, swing hook, jump target |
| PATHZONE, CG | navigation zone and its graph |
| PROXY, PROXYSRC, WE, LOROOT | proxy object, proxy source = world entry, per-hierarchy root |
| RIP, RIPG, BLIPG, EMITTER, EXPL* | particle, particle group, sprite particle group, emitter, explosions |
| DPRIZE, WIDGET | dropped prize, HUD element (Sly 1: BLOT) |
| RELAY, JSG, RELAYCMD | scripted-sequence object, its interpreter, a command |
| JOB, TASK **(team)** | mission and mission step |
| GS (`g_gs`), SAVE | the save blob (not the Graphics Synthesizer); memory-card state machine |
| SND, SNDMGR, BANKSND, STREAMSND | sound instance, manager, bank sound, streamed sound |
| POCTL | player-object controller (filtered pad state) |
| FRM, RPL, GSB | a frame for the render thread, render-list entry, GS memory block |
| FILEREC **(team)** | 64-byte disc file record |
| CID, OID, EOPID, SYMID, BIF, TAGK | class id, object id, option id, Splice symbol id, builtin, value tag |
| JTS, JTBS, JTCS | Sly's state, sub-state, cane state |
| BRX, BIS | the world file format and its loaders (`Load...FromBrx`); binary input stream |
| LM, CLQ, SMP | limits, quadratic coefficients, smoothing parameters (Splice value kinds) |
| DL/DLE, SL/SLE | doubly / singly linked list and entry |

PS2 terms: **EE** main MIPS CPU; **IOP** I/O processor (CD, sound, memory card) reached over SIF RPC; **IRX** IOP module;
**VU0/VU1** vector units (VU1 runs vertex microprograms); **VIF/GIF** DMA interfaces to VU1 / the GS; **GS** rasteriser with
4 MB local memory; **scratchpad** 16 KB fast RAM at 0x70000000; **IPU** MPEG decoder; **LBA** 2048-byte sector index.
