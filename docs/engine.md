# How the Sly 2 engine works

A cross-cutting overview of the engine as recovered by the decomp: boot, the frame loop, the level lifecycle, the object
model, and one short section per subsystem. Each subsystem section is a summary; **the systems guides in
[systems/](README.md#systems-guides) hold the detail** (types, lifecycle, invariants, recipes, where to look). Detail that
belongs to one file or struct lives in that file's header comment or next to the struct in `include/sly2/`. Names are
the code's current names. `D_XXXXXXXX` is a global the code has not named.

The per-subsystem research pages this page and the guides condense (more runtime tables, per-state field dumps,
save-state numbers, provenance) remain in git history: `git show 3493d772:docs/engine/<page>.md` (pages: animation,
bosses, camera, core-library, cutscenes, game-progression, gameplay-objects, guards-ai, hud-ui, main-loop-startup,
minigames, object-model, overview, particles-effects, physics-collision, player, rendering, save-memcard, sound,
splice-scripting, world-sw).

## 1. How knowledge is marked

Facts carry a tag saying how we know them. The same tags appear in source and header comments.

| Tag | Meaning |
|---|---|
| `[C:runtime]` | observed in PCSX2 save-state RAM (hubs, combat, all three characters, a level load, the pause menu, ThiefNet, the front end, a full mission); a state or round may be named, e.g. `[C:runtime S56]` |
| `[C:live]` | confirmed in the running game over PINE: a value was written or read and the game visibly reacted |
| `[C:verified]` | re-derived from the code by an independent verifier |
| `[C:parse]` | a parser following the matched loader consumed every byte of the game's files (all world files) |
| `[C:static]` | read directly from matched code, ELF data or the VU disassembly, and structurally certain (a vtable slot, a decoded table, a call order) |
| `[C:data]` | read from level data (object records, task records, Splice code, string tables) |
| `[H]` | hypothesis: inferred from code shape, Sly 1 similarity, callers or link order (a provenance label such as `r4 lo.md` names an out-of-repo write-up) |
| `REFUTED` | an earlier claim a later check contradicted; kept so nobody re-introduces it |

Matching does not mean understood: every function compiles to the original bytes, so the code is certain, but a name or
purpose is a hypothesis until a verifier, a runtime check or a structural fact backs it. Code comments mark such names
`(inferred)`.

Conventions: "slot N" / `vtN` is C++ virtual slot N. gcc 2.95 vtable entries are 8 bytes `{s16 delta; s16 pad; void
*pfn}` and slot 0 is a null header. CID numbers are hex (`CID 0x47`) or decimal placeholders (`CID148`, `BHV171`) for
classes whose Sucker Punch name is unknown.

## 2. The shape of the program

- One statically linked EE executable, `SCUS_973.16`: game code, then Sony's 989snd client library, then the Sony SDK
  libraries `[C:static]`.
- Game code is C++ compiled by ee-gcc 2.95.3 (`-O2 -G0 -ffast-math`) and linked **in alphabetical file order** after a core
  block (crt0, Splice thunks, class factories, core library, Splice runtime) `[C:static]`. That order is how most file names
  were recovered `[H]`.
- Almost every translation unit ends with compiler stubs that register the Splice slot-heap pools
  (`__static_initialization_and_destruction_0` + `_GLOBAL_$I`, plus `_GLOBAL_$D` with global destructors). These stubs
  mark the TU boundaries `[C:static]`; each TU is one file under `src/game/` (see [architecture.md](architecture.md)).
- The design descends from Sly Cooper 1 (same vocabulary: LO, SO, SW, CM, BRX, Splice), but the object model was rebuilt with
  real C++ classes and virtuals, every class ID was renumbered and most structs changed layout. Sly 1 is a source of
  **names**, not offsets.

## 3. Boot

`_start` (crt0) -> `GameMain` ([`app/main.cpp`](../src/game/app/main.cpp)):

1. `__main` runs the C++ global constructors (`g_sw`, `g_cm`, `g_sndmgr`, the screen objects, ...).
2. `Startup` runs the startup table under a `CProg` progress bar, ticking the clock between entries, and prints the
   build string `[C:verified]`. Entries in order: `StartupDma`, `StartupThread`, `StartupSif` (IOP modules,
   `ioprp300.img`), `StartupGs`, `StartupMemMgr`, `StartupCd`, `SetCdPollStateIdle`, `StartupFont`, `StartupJoy`,
   `StartupFrame` (render thread), `StartupLoadingScreen`, `StartupClock`, `StartupVU0`, `StartupVU1`, `StartupSound`,
   `StartupBlips`, `StartupHudGifPackets`, `StartupMemcard`, `StartupMpeg` `[C:static]`. There is no camera entry:
   `SetupCm` is empty in Sly 2 but keeps Sly 1's two call sites (main, `PostSwLoad`) `[C:verified]`.
3. `StartupGame`: language from `sceScfGetLanguage`, game event 0 (startup), then the boot step queue
   (`BeginStartupSequence`: logo movie, memcard check, warp to the boot world).
4. The frame loop, forever.

Detail: [systems/world-and-levels.md section 2](systems/world-and-levels.md#2-boot-and-the-frame-loop).

## 4. The frame loop

From the matched `GameMain` body `[C:static]`:

```
for (;;) {
  1  ExecuteSwTransition(g_sw)      only if a level change is pending (section 5)
  2  UpdateJoy(&g_joy)              pad read
  3  UpdateCd()                     disc check / CD service
  4  SetupCm(&g_cm)                 empty in Sly 2
  5  MarkClockTick(&g_clock, 0)     dt from the EE cycle counter, clamped to [1/60, 1/30] s
  6  UpdateSw(g_sw)                 the world update (below)
  7  UpdateScreens(...)             pause / menus / memcard UI; Start toggles pause
  8  OpenFrame()                    start this frame's DMA chains
  9  if (IsFrameCaptureInactive())  no full-screen capture overlay, or it allows rendering
        RenderSw(g_sw)              vt19 RenderSelf on live objects
        DrawSw(g_sw)                sort the render list, emit the VIF1 chain (section 7.3)
        DrawUi(...)                 HUD lists + GIF/VIF flush
 10  if overlay: DrawFrameCaptureScreen   full-screen transition pass
 11  CloseFrame()                   finalize, hand the frame to the render thread
 12  global frame counter++         never reset
}
```

The render thread (`FrameRenderLoop`) waits for a closed frame, DMAs its VIF1 then GIF chains, optionally blends the
previous frame back in (`BlendPrevFrame`), and swaps GS buffers, so the CPU builds frame N+1 while the GPU draws frame N.
In practice the world always renders, including under the pause menu `[C:runtime]`. During a level load the main thread
is blocked in `ExecuteSwTransition` and frames come from the loading-screen thread. Other threads: the memcard worker
(`MemcardThreadMain`) and an MPEG display thread.

**`UpdateSw`** (once per frame, if dt > 0): deferred SW callbacks and due Splice callbacks (`ProcessSwCallbacks`); the
busy list, which decides which roots are live and fires OnAdd/OnRemove (`UpdateSwBusyList`) `[C:verified]`; vt16
`Update` on every live object `[C:runtime]`; animation applications and noise stimuli; physics (`SolveSw`); lights,
shader animation, zap-history expiry; then `UpdateSave`, `UpdateSndmgr`, the camera and the HUD stack. The exact order
is in [systems/world-and-levels.md](systems/world-and-levels.md#2-boot-and-the-frame-loop).

**Clock** (`g_clock`, [`core/time/clock.cpp`](../src/game/core/time/clock.cpp)): reads the CP0 Count register, extends it
to 64 bits and clamps dt. Game time is scaled by the product of **9 rate slots**: slot 0 pause menu / ThiefNet, slot 1
power-up gadgets (Thief Reflexes 0.25, Time Rush 1.35), slot 7 widget pause (job banner), slot 8 camera. The product is
**cached** and recomputed only by `SetClockRate` -> `RecalcClockRate`, so writing a slot alone has no effect
`[C:live]`; a port or trainer must call `SetClockRate`. The camera uses dt x slot 8 only, so it runs during pause and slow
motion. A separate pause-independent real time drives the loading screen, episode menu and ThiefNet. `ResetClock` sets
t = 0 on every level load.

**Input**: `g_joy` (JOY: held and pressed button words). Gameplay never reads the pad directly: each playable object owns
a POCTL (section 7.10). Cheat codes advance on presses and fire one second after the last press
([`gameplay/progression/cheat.cpp`](../src/game/gameplay/progression/cheat.cpp)). Rumble has a global mode (follow option
/ forced on / off) gated by the save var `flags.fNoVibration`.

## 5. The world and the level lifecycle

There is exactly one world object, `g_sw` (class SW, CID 0x1, a BASIC child) `[C:runtime]`. SW is **not** an LO in Sly 2.
It owns every pool, object list and hash table of the level, and is re-constructed in place on every level change. Its
fields are grouped by role in `include/sly2/sw.h` and described in
[systems/world-and-levels.md section 1](systems/world-and-levels.md#1-the-main-types-and-globals).

### Loading a level

A change is requested by `SetSwTransition(SW*, TRANS*)`, which marks a transition pending and copies the `TRANS`
(flags, target FILEREC, warp oid, argument). The FILEREC is XOR-obfuscated
([formats/disc-records.md](formats/disc-records.md)). Writing the request directly over PINE warps to any level
`[C:live]`. `WipeToWorldWarp` is the usual caller (via the wipe, below).

`ExecuteSwTransition` `[C:static]`: drain the render thread; **open the new world file first** (an async CD stream with
LZSS decompression); `DeleteSw` (also deletes the Splice frame and clears the player pointers), `ResetClock`, block the
pad; zero and re-construct `g_sw`; `LoadSwFromBrx`, which starts the loading screen itself; `PostSwLoad` (post-load
callbacks, vt15 PostLoad, bind save vars, default pathzone, game event 4); pick the current character from the save var
`oidPoCurrent`; game event 5; wipe in. Step by step:
[systems/world-and-levels.md section 5](systems/world-and-levels.md#5-loading-a-level-executeswtransition).

`LoadSwFromBrx` reads the decompressed stream in order: world-entry table, sound section, save-variable schema, level
table, localized strings, cell BSP, options of `g_sw` then `g_cm`, Splice frame setup and save-var resolve, then **per
world entry** Splice refs, shaders/textures, zone BSP, the object list (`LoadSwObjectsFromBrx`) and its clones
(`CloneLoWithParent`). World entries are **proxy sources**: entries flagged to stay out of the world have their objects removed
again and appear only through PROXY copies. Each object record carries its class, oid, Splice index, load filter and
byte size, and vt14 `LoadFromBrx` must consume exactly that many bytes. Byte layouts:
[formats/world-file.md](formats/world-file.md) and [formats/level-objects.md](formats/level-objects.md).

**Level identity.** The level table names each level by FILEREC, world and level-name index (path symbols spell
`episode.<world>.level.<name>`); worlds are intro, nightclub (Paris), palace (India), temple, prison (Prague), castle,
train, sawmill (Canada), blimp. The disc's world files are the `Lnn` levels plus L00 (playable prologue) and L01
(front-end/menu world) `[C:parse]`. World-entry name prefixes follow the world: `f_` Paris, `i_` India, `p_` Prague,
`c_` Canada, `a_` blimp, `jb_` L00.

### Transitions

- **EXIT** (CID 0x1A, a VOL child): a player-in-volume state machine that starts a wipe; **WARP** (CID 0x2A): warp
  destinations matched by id or alias list (`FindWarpByOid`); CID41 (0x29): in-level teleport destination.
- **WIPE**: idle, out animation, in animation, fire -> `SetSwTransition`.
- The boot step queue (movie, world warp, start task, memcard UI phase) is run by `StartupGame`, `ExecuteSwTransition`
  and the job flow `[H]`.
- **Death reloads the level** `[C:runtime]`: SW and clock reset, save variables kept. There is **no checkpoint record**
  (REFUTED: a "checkpoint manager" in sw.cpp; that function is `SetSwTransition`). "Checkpoints" are ordinary TASKs.

### Hands-off and locks

Four SW counters, each paired with a `g_grfusr` bit `[C:runtime]`: **hands-off** (`IncrementSwHandsOff` /
`DecrementSwHandsOff` / `IsSwHandsOff`; RELAY hands-off commands (`AddRelayHandsOff`), the job banner, the pause menu), **no-pause**
(`IncrementSwNoPause`; RELAY hands-off, recon mode), **letterbox** (cutscenes) and **input lock** (cutscenes, job banner,
recon mode). Observed `[handsoff, nopause, letterbox, input]`: free roam [0,0,0,0]; recon dialogue [0,1,0,1]; cutscene
[1,1,1,1]. `IsPoInvulnerable` is true while hands-off. **Deferred callbacks**: `PostSwCallback` queues
`{object, pointer-to-member, arg}`; `ProcessSwCallbacks` drains the queue, then runs Splice callbacks scheduled with
`ScheduleSwSpliceCallback`. Detail: [systems/world-and-levels.md section 7](systems/world-and-levels.md#7-control-counters-and-deferred-work).

## 6. The object model

```
BASIC (0x0)                       slots 1-6: refcount hooks, Splice event, messages, class info
 ├─ SW (0x1)                      the world (not an LO in Sly 2)
 ├─ CM (0x87), SAA family, POCTL, ASEGA, SMA, BHV (0x9B) + behaviours, screens, ...
 └─ LO (0x2) level object         oid, parent, hierarchy, Splice frame, activation, options
     ├─ XFM (0x3) LO + matrix     lightweight classes: LIGHT, PROXY, SHAPE (PIPE, RAIL), TN, TARGET, VOL, EMITTER, ...
     ├─ ASEG, SM, WR, WIDGET (HUD), JOB/TASK (0x84/0x85), ...
     └─ SO (0x30) solid object    XF array, collision parts, physics record, model (GLOBSET), zaps, fading
         ├─ MS, BOMB/MISSILE, BARRIER, RIPG, pickups (DPRIZE family), ...
         └─ PO (0x32) playable    POCTL controller, HP/MP, character switching
             └─ STEP (0x33)       heading/turn, ground contact, behaviour stack (BHVSTK)
                 ├─ STEPPLAYER (0x34)  shared movement components, gadgets
                 │    └─ JT (0x35) Sly, MURRAY (0x36), BENTLEY (0x37)
                 └─ STEPGUARD (0x38)   perception, flags, engagement; GOMER (0x39) + subclasses
```

The essentials (detail in [systems/object-model.md](systems/object-model.md)):

- **Registry.** The class list with sizes and factories is `include/sly2/classes.h` and
  [`app/class_registry.cpp`](../src/game/app/class_registry.cpp). CIDs are numbered in **pre-order** of the class tree, so
  "is derived from" is a range test (`IsBasicDerivedFrom`) `[C:runtime]`.
- **Layout.** The vptr follows BASIC's three data words (Splice frame, sidebag of extra script options, message
  subscribers), not at offset 0 as in Sly 1. Non-BASIC helpers (HEAP, CBinaryInputStream, RIPs, camera policies) put the
  vptr elsewhere.
- **Virtual interface.** Slot indices hold across every LO subclass `[C:static]`, so naming a subclass's Init/Load/Update
  is mechanical: 3 HandleSpliceEvent, 4/5 SendMessage/HandleMessage, 7 Init, 8/9 Add/Remove, 14/15/16
  LoadFromBrx/PostLoad/Update, 19 RenderSelf, **22/23 OnRemove/OnAdd**, 40/41 CloneHierarchy/Clone, 48+ class-specific.
  REFUTED: Clone at slot 40 (it is 41); slot 3 = SendMessage (it is 4).
- **Messages vs Splice events.** Slot 4 delivers a numeric message to the object's own slot 5 and every subscriber; slot 3
  runs or schedules the script bound to an event symbol. `vt3(k,0,0,0,0)` calls are Splice events (5 trigger,
  6 untrigger), not messages.
- **Creation.** `CreateLo(cid, parent, oid, isplice)`: factory, link into the oid hash and class list, vt7 Init, vt11
  AddHierarchy if the parent is in the world, vt23 OnAdd if now live. `BindLoOidTable` resolves OID references into
  pointer fields at PostLoad.
- **Activation** `[C:verified]`. `IsLoInWorld` is true only when the object is in the world hierarchy **and** its LOROOT is
  active. Slot 23 fires when that flips on, slot 22 when it flips off; they always alternate. It flips on structural
  changes and on **proximity**: each frame `UpdateSwBusyList` decides per root whether it is active (always, never, or
  within its activation radius of the player or the camera). **Only live objects get Update and RenderSelf**; guards and
  pickups are pooled and toggled, never freed. Sly 1's separate `FreezeAlo` is merged into this.
- **Proxies.** A world entry is a proxy source loaded several times; PROXY objects (CID 0x6/0x7) take a spare copy on
  demand (`AcquireProxySpare`). `AcquireProxyObject` is the generic spawn path (thrown objects, markers, coin sprays).
- **Finding objects.** `FindSwObject` resolves OIDs through the oid hash; `FindSwObjectsByClass` walks the
  per-class lists. Runtime-spawned BLIPG/RIPG objects are on class lists but not in the oid hash.
- **XFM** (CID 0x3): LO + parent XF, local and world matrices; `GetLoWorldPos` / `GetLoWorldMatrix`.

## 7. Subsystems

Directory names are under `src/game/`; the layer rules are in [architecture.md](architecture.md). Each section ends with
its guide.

### 7.1 Core library (`platform/memory`, `core/*`)

No systems guide yet; this section is the reference.

- **Two bump heaps, no free lists.** The world heap grows up from `_end` and is reset at level unload (`FreeSw`); all `new`
  goes there (zeroed, 16-aligned). The stack heap grows down from the top of RAM, scoped by an RAII helper. `malloc`
  returns NULL, `free`/`delete` are no-ops. `GetFreeMemorySize` (the gap between the heaps) gates movie playback.
- **Slot heaps** (`slotheap.cpp`): fixed-size pools with lazy block allocation, used by every TU's static init (Splice pools)
  and many engine pools (LOROOTs, callbacks, sounds, stimuli, JSGs).
- **Containers** (`dl.cpp`): Sly 1's doubly linked list (`DL`, iterator fix-up via `g_pdliFirst`) plus a new singly linked
  list SL used by the SW class lists, oid hash and scheduled callbacks. `CByteQueue` for streaming.
- **Math**: mat/vec/util/bez/crout/bsp/dzg (LCP) in Sly 1 order. Sly 2 passes VU vectors **by value** where Sly 1 passed
  `VECTOR*`, so Sly 1 mangled signatures must not be applied blindly `[C:verified]`.
- **Streams and CD**: `CBinaryInputStream` (typed readers, `Align`, LZSS `SetDecompress`); CD-backed reads for all
  world loads (`CBinaryInputStream::OpenCdSectors`); `CBinaryAsyncStream` spools FMV. The only sector reader is `sceCdRead` through a retry
  wrapper. There are no file names on disc (`GetFileIdName` returns "fid:#unknown"); files are found by FILEREC
  ([formats/disc-records.md](formats/disc-records.md)).

### 7.2 Splice scripting (`splice/`, `app/bif.cpp`, `object/spliceobj.cpp`)

Splice is Sucker Punch's Scheme-like level-scripting language. Levels carry compiled Splice; objects carry a frame of
bindings; scripts react to events, read and write object **options** (numbered properties and methods, polymorphic per
class), and build RELAY sequences and JOB/TASK records. Native binding thunks live in `src/generated/splicewrap/`; the
builtin table is in `bif.cpp`; the evaluator is Sly 1's eval.cpp one-to-one, with quirks a port must keep (`if` tests
the payload word, `=` compares raw payloads, unbound symbols evaluate to nil) `[C:static]`. Value tags were renumbered
from Sly 1 `[C:verified]`.

Guide: [systems/splice-scripting.md](systems/splice-scripting.md) (values, frames, evaluator, options, events, builtins,
GC). Serialized form: [formats/splice-scripts.md](formats/splice-scripts.md).

### 7.3 Rendering (`engine/graphics/*`)

Objects queue **render-list entries** (RPLs); `DrawSw` sorts them into **kinds**, each kind maps to a **pass**; a pass hook
uploads a **VU1 microprogram** and GS state; per-entry DMA chains feed vertices to it. (PS2 primer: the EE builds DMA
chains; VIF1 unpacks data into VU1 memory and starts microprograms, which XGKICK GIF packets to the GS rasteriser: 4 MB
local memory, **no stencil buffer**, blend `(A-B)*C+D`.) The kind-to-pass table and the per-pass begin/end hooks in
[`draw.cpp`](../src/game/engine/graphics/pipeline/draw.cpp) are **the main extension point for a native renderer**.

Notable facts: lighting ("relight") recomputes two colour sets per vertex only when lights change, and there is **no
banding in the VU code** (the cel look comes from the art, the steep light curve and the outlines); outlines are
silhouette edges drawn as GS lines; shadow volumes count in the framebuffer's **green channel** because there is no
stencil buffer. Particles (EMITTER, RIPG, BLIPG, explosions, trails) are covered in the same guide.

Guide: [systems/rendering.md](systems/rendering.md) (render lists, passes and kinds, relight, shaders and textures,
culling, particles, render targets, what a port must replace). Port checklist:
[guides/port-plan.md](guides/port-plan.md#66-render-port-checklist). Formats: [formats/textures.md](formats/textures.md),
[formats/meshes-scene.md](formats/meshes-scene.md).

### 7.4 Camera (`engine/camera/`)

`g_cm` (CM, CID 0x87, BASIC child, not an LO) holds fov, clip planes, matrices and fog; level options set them. The
camera is a **priority stack of policies** (`SetCmPolicy` / `RevokeCmPolicy`); each frame the top policy places the camera
using the focus object's camera definition (CPDEFI, from SO slot 50) softened by adjust springs, then cm.cpp builds the
view/projection matrices and frustum and clips the eye against collision. A policy is a small C++ object (slots Init,
Activate, Deactivate, Set, Revoke, Update).

The bottom entry, **CMPART1, is an empty placeholder** pushed at priority 0 by `InitCm`; its Update does nothing. **The
real follow camera is CPTN**, pushed at priority 1 by `OnPoActive` when a PO becomes the active player, and pushed again
above that while the hero is inside a TN tunnel volume. Higher priorities: CPLOOK binocucom, the recon camera,
CPTURRET, CPCAMBLEND cutscene blends, CPCAMERA fixed marker camera (safehouse), a hide-under-table policy, CPTWOSHOT.

Guide: [systems/camera.md](systems/camera.md) (who pushes what at which priority, the follow camera, collision, look and
zoom).

### 7.5 Animation (`engine/anim/`)

The **skeleton is the SO's XF table**: an XF is one degree-of-freedom group (translate, rotate, scale, ...), not a bone.
**ASEG** (CID 0x6D) is a clip loaded from the level whose channels **replace** XF values; **ASEGA** (CID 0x86) is a
playing application of a clip to an object tree, and its events start sounds and toggle objects. Blends (ASEGBL, gait,
directional), **SM** (CID 0x6F, states and transitions from the level) and **SMA** (CID 0x88, plays toward a goal state
using precomputed distance tables, new in Sly 2), two-bone IK and rope chains complete the system.

Guide: [systems/animation.md](systems/animation.md).

### 7.6 Physics, collision and navigation (`engine/physics/`)

**SO** (CID 0x30) carries the XF array, collision parts, an optional physics record, a zap response table and an
external-acceleration list. Pipeline: sweep-and-prune broad phase (`bbmark.cpp`) -> contact generation (`mark.cpp`: BSP
and spheres) -> `SolveSw` groups contacts and solves each group with the DZG LCP solver. Damage is delivered as
**zaps** (`DeliverZapToSo`) with a short dedupe history. Navigation: PATHZONE graphs with A* (Sly 1 path.cpp) plus, new
in Sly 2, a zone BSP and inter-zone routing over pathlink objects used by guard AI.

Guide: [systems/physics-and-collision.md](systems/physics-and-collision.md) (solver, characters, water, zaps, volumes
and shapes, navigation).

### 7.7 Sound (`engine/sound/`, `src/sdk/989snd`)

Sly 2 rewrote Sly 1's C sound code as C++ classes with a request queue. Effects come from a per-level 989snd bank;
music, dialogue and ambience stream from disc through Sucker Punch's IOP module (RPC 'SUKR'). `g_sndmgr` owns the pools,
bank, music stack, dialogue and ambience streams and a category x channel volume matrix (channels: 0 ambience, 1 music,
2 dialogue, 3 bank effects). **SND** (CID 0x97, refcounted) keeps request bits that `UpdateSnd` turns into virtual calls
once per frame. The rhythm minigame times against music time (`GetMusicTime`), not the game clock. SNDMGR's headset option
drives the **USB-headset microphone**: dialogue plays through the headset and the mic level makes the hero emit a noise
guards hear.

Guide: [systems/sound-and-music.md](systems/sound-and-music.md). Formats: [formats/audio.md](formats/audio.md).

### 7.8 Particles and effects (`engine/fx/`)

EMITTER (CID 0x1D) keeps Sly 1's emitter structure and spawns RIPs and BLIPs; the same file holds the explosion family
EXPL/EXPLG/EXPLO/EXPLS. RIPG (CID 0x57) owns RIP particles whose types got real C++ vtables; BLIPG (CID 0x1C) sprite
particle groups; trails (CID 0x69) deform a mesh along a ring of past matrices.

Guide: [systems/rendering.md section 8](systems/rendering.md#8-particles-and-effects-enginefx).

### 7.9 HUD and UI (`engine/presentation/`, `engine/font/`, `ui/`)

**WIDGET** (CID 0x7E, LO child; Sly 1's BLOT) and subclasses are HUD elements with a show/hide state machine (the
job-completion banner pauses the game clock through rate slot 7; the objective counter recounts complete tasks every
frame and stores the count nowhere). **Screens** are a BASIC-derived family (no CIDs) on an active list updated by
`UpdateScreens`: pause menu (clock slot 0 and hands-off; the world keeps rendering), ThiefNet, gadget menu, memcard
dialogs. Text: STRINGGEN getters, `CFont`/`CRichText`, the text table.

Guide: [systems/ui-and-hud.md](systems/ui-and-hud.md).

### 7.10 Player characters (`actor/`, `gameplay/player/`)

`SO -> PO (0x32) -> STEP (0x33) -> STEPPLAYER (0x34, abstract) -> JT (Sly) / MURRAY / BENTLEY`. STEPPLAYER is new: Sly 1's JT
sub-structs were lifted into a shared base, so all three run the same movement components (air, grab, hurt, pipe,
throw, ...). `g_pjt` is **always Sly** (REFUTED: "the current player"); the PO with control is `GetHeroPo`, published as
the save var `oidPoCurrent` (7 Sly, 8 Bentley, 9 Murray). Input reaches a hero only through its **POCTL** (CID 0x89); AI
partners write a non-current hero's POCTL directly from a behaviour on its stack. Gadgets are described by
`g_agadgetdesc`; owned = a bit in `powerUps.mppukfAcquired`. Thief Reflexes is `SetClockRate(slot 1, 0.25)`.

Guide: [systems/player.md](systems/player.md) (class chain, state machines, components, input, grab spots, AI partner,
gadgets).

### 7.11 Guards and AI (`gameplay/ai/`)

Sly 2 replaced Sly 1's per-guard state machine with a **stack of behaviour objects** (BHV, `BHVSTK`) per character, plus
STEPGUARD (CID 0x38), which hosts perception, targeting, flags and engagement rules. The guard flag word always equals the
union of the flags of the behaviours on the stack `[C:runtime]`. The root BHVGUARD picks the mode (calm/patrol,
suspicious/search, alert/combat, juggled, KO, script); two alerted guards **take turns attacking**; perception is a
vision DETECTOR plus refcounted noise stimuli. Only a few guards are in the world at a time; the guard manager respawns
inactive ones.

Guide: [systems/ai-guards.md](systems/ai-guards.md) (behaviour stack, perception, combat, patrols, scripts, adding a
behaviour).

### 7.12 Game progression (`world/state/game.cpp`, `gameplay/progression/`)

**JOB** (CID 0x84) and **TASK** (CID 0x85) are team names for a mission and a mission step; save names follow
`m<N>_<name>` = JOB, `t<N>_<name>` = TASK. Tasks form a dependency graph with states (locked, available, complete within
the job, complete after the job) `[C:runtime]`; visibility records put objects in the world only in some task states, so
objective order is fixed. `BroadcastGameEvent` is the game event bus (startup, post-load, hero died, guard defeated, job
finished, ...). Difficulty averages saved samples (`GetDifficulty`); `ScaleByDifficulty` scales a count by it. Collectibles are the DPRIZE family
(`prize.cpp`).

Guide: [systems/progression-and-saves.md](systems/progression-and-saves.md).

### 7.13 Save data and memory card (`world/state/save.cpp`, `ui/memcard.cpp`)

All persistent progress is **one flat blob** `g_gs` addressed by **named variables** whose layout comes from a schema in
every world file `[C:runtime]`; a changed schema wipes the blob (`ResetGsIfSchemaChanged`). Access is through
`GetSaveVarByName` / `SetSaveVarByName` / `GetSaveVarAddressV` and the Splice save-var builtins. Memory-card files `save0..2`
are the raw blob plus the schema fingerprint, with **no checksum, compression or encryption** `[C:static]`.

Guide: [systems/progression-and-saves.md](systems/progression-and-saves.md#5-save-variables). Variable list:
[formats/save-variables.md](formats/save-variables.md).

### 7.14 Cutscenes, sequences and movies (`gameplay/cutscene/`)

No systems guide yet; this section is the reference.

- **RELAY** (CID 0x72) holds a flat array of commands in nested blocks, built at load from object records (METHOD
  options) or Splice through one builtin per command kind. `FireRelay` spawns a **JSG** interpreter (slot heap) that steps
  through commands while each is async or complete (`NextJsgCommand`, `IsJsgCommandComplete`) and can spawn more JSGs;
  `RemoveJsg` releases everything a script holds. Commands cover actors and teleports, movement (pushing BHVs), clips,
  state machines, widgets, hands-off, camera cuts/blends/tweens, sound and dialogue lines, hint boxes, task states and
  controller hand-off (`SetCtlState`). **Cutscenes complete tasks themselves**; a TASK's Splice fires them. Camera blends
  push CPCAMBLEND (angles in degrees, vertical fov in radians).
- **FMV**: no FMV is used for mission sequences; the disc holds 2 movies. `CMpeg` is Sly 1's mpeg.c almost 1:1 plus
  subtitles (`DrawMpegSubtitles`); it refuses to play when free memory is short.
- **Transitions**: the frame-capture transition (CID 0x68: capture the framebuffer, split/peel effect), wipes (section 5),
  letterbox.

### 7.15 Bosses and minigames

No systems guide yet; this section is the reference.

Six boss fights, each a JOB or tasks inside a heist JOB; all reuse generic machinery: STEPGUARD hit points, game events,
a health-bar WIDGET, guard scripts (CID 0x71), boss music, and PROXY copies so retries respawn a fresh boss.

| Boss | Job | Level | Played as | Driver |
|---|---|---|---|---|
| Dimitri | `m1_boss` | L05 | Sly | DIMITRI class: authored flee routes away from the player (`FindDimitriFarthestRoute`) |
| Rajan | `m3_boss` | L12 | Sly, then Murray | RAJAN class (sense range, camera focus) + Splice |
| The Contessa | `m5_heist` boss tasks | L17 | Sly | CONTESSA class + Splice, two battles |
| Neyla (RC duel) | `m6_heist` | L30 | Sly / RC chopper | Splice + RELAYs over generic SOs |
| Jean Bison | `m7_boss` | L37 | Bentley | **pure Splice**: the job script counts indirect hits and advances phase tasks |
| Clock-La | `m8_turret`, `m8_boss` | L38, L43 | Sly (+ scripted Murray/Bentley) | **no actor class**: an assembly of SOs, SMs and targets driven by Splice |

Minigames are entered by handing the controller to another PO (`SetCtlState` from a RELAY, or the task's controlled-PO
field): **Bentley hacking** (HACK family CIDs 0x47-0x4C: ship, enemies, bullets, lock, walls; left stick moves, right
stick aims and fires), **mounted turret** (TURRET 0x45 + CPTURRET), **RC combat** (ROBOTTANK 0x4D; player or AI mode),
**dance** (DANCER 0x44 + rhythm manager CID121, judged against music time), the **safehouse** character select and
ThiefNet (SAFEHOUSE 0x46). The RC chopper and the drivable tank are plain POs whose drive code is not identified yet.

Smaller gameplay classes (`gameplay/objects/`): BARRIER, BOMB, MISSILE/ACCMISS (projectiles derived from BOMB), BUTTON and
VOLBTN (fire Splice events 5/6), SENSOR and laser rigs, TARGET, breakable props (CID98, staged recorded actions),
tossable gadget objects (CID100-102), critters (CID96), the screen tint/fade (CID116), the sky dome (SKY).

## 8. Where Sly 2 differs from Sly 1

| Area | Sly 1 | Sly 2 |
|---|---|---|
| Classes | C function-pointer tables, vptr at +0, ALO class | real C++ virtuals (vptr +0xC), no ALO (split into LO + XFM), SW and CM are BASIC children |
| Activation | OnAdd/OnRemove + separate FreezeAlo | one OnAdd/OnRemove pair driven by structure and proximity |
| Level file | one object tree, objects without sizes | world entries instanced `c8` times; every object record carries its byte size |
| Guards | SGS state machine in STEPGUARD | BHV classes on a per-character stack; STEPGUARD hosts perception and flags |
| Progress | scattered game state | one flat blob of named variables from a schema in the world file |
| Missions | - | JOB/TASK dependency graph + a game event bus |
| Scripting | Splice | same language, renumbered tags, a larger builtin table, RELAY/JSG sequences on top |
| Player | JT with sub-structs | STEPPLAYER base shared by Sly, Murray and Bentley |

## 9. Names the research disputes

The research pages argued for these readings; the code keeps a different name for now. Check before renaming.

| Code name | Research reading |
|---|---|
| `AllocSw__4DMASi` | allocates from the stack heap (`AllocStack`) |
| `InitZpr__FP3ZPR3ZPKP2LO` | GS block allocator init (`InitGsb`) |
| `SetStepSpinRatio__FP4STEPf` | sets the spin ratio of the CID96 critters |
| `EnsureSggAlarm__FP3SGGP5ALARM` | RELAY alarm id set used by command kinds 4/5 |
| `AddBootyEntryObject__FP5ALARM3OID` | adds a BOOTY entry by oid (CID 0x67 is BOOTY, not ALARM) |
| `SetFrameCaptureState` | the DANCER's screen-grab state (split/flip transition) |
| `GetHeroPo` / `GetCurrentPo` | `GetHeroPo` returns the PO that has control |

## 10. Glossary

Sucker Punch's Hungarian style, inherited from Sly 1 (whose symbols shipped), still shows in type names and the remaining
Hungarian function names. **(team)** marks project coinage.

Prefixes: `P`/`p` pointer (returns or is), `F`/`f` boolean, `C`/`c` count, `G`/`g` float, `I`/`i` index, `N`/`n` integer,
`A`/`a` array, `Grf`/`grf` flag bits, `Mp`/`mp` map indexed by an enum, `Rad` radians, `S`/`s` distance, `T`/`t` time (s),
`Dt` time delta, `U`/`u` 0..1 parameter, `Pchz` C string, `Ref` returns a Splice `CRef`, `Pv` `void*`, `g_` global, `s_` file
static. Methods read `<Verb><Class><Noun>`: `FindSwObject` = "pointer to LO: find SW object".

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
| JTS, JTBS, JTCS | Sly's state, sub-state, cane state (JTCS is Sly 1's; Sly 2 has no separate cane-state type) |
| BRX, BIS | the world file format and its loaders (`Load...FromBrx`); binary input stream |
| LM, CLQ, SMP | limits, quadratic coefficients, smoothing parameters (Splice value kinds) |
| DL/DLE, SL/SLE | doubly / singly linked list and entry |

PS2 terms: **EE** main MIPS CPU; **IOP** I/O processor (CD, sound, memory card) reached over SIF RPC; **IRX** IOP module;
**VU0/VU1** vector units (VU1 runs vertex microprograms); **VIF/GIF** DMA interfaces to VU1 / the GS; **GS** rasteriser with
4 MB local memory; **scratchpad** 16 KB fast RAM; **IPU** MPEG decoder; **LBA** 2048-byte sector index.
