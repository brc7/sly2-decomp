# Particles and effects

Related: [rendering](rendering.md), [object-model](object-model.md) (spawned objects are not in the oid hash).

## 1. Emitters (emitter.cpp 0x159118-0x15CD68)

EMITTER = CID 0x1D (629 instances in the Paris hub) `[C:runtime]`; vtable slots 5 HandleMessage 0x1599A8, 7 Init 0x1591B0, 15 PostLoad 0x159500, 16 Update 0x159C80 `[H r3 emitter.md]`.
Sly 1 emitter.c order is largely kept: `InitEmitb`, `LoadEmitmeshFromBrx`, `ChooseEmitoPos`, `CalculateEmitvx`, `ChooseEmitVelocity`,
`EmitRips`, `EmitBlips`, `OriginateParticles`, `EmitParticles`, ... `[H r3]`. The same TU holds the explosion family EXPL (0x1E, abstract),
EXPLG (0x1F), EXPLO (0x20), EXPLS (0x21): `ExplodeExpl`, `LoadExplgFromBrx`, `CloneExplg`; EXPLS is the most common class in the hub (1,501) `[C:runtime]` counts, `[H r3]`.
Wrong existing proposal: 0x1594D8 is a Splice-exported callback that registers an object in a 4-slot SW array, not `BindEmitterCallback` `[C:verified]` (WRONG verdict).

## 2. RIP particles (rip.cpp 0x1CDCD0-0x1D1370)

- RIPG (CID 0x57, SO child, vtable 0x2D77D8) owns a DL of RIPs (+0x15C) created by emitters; SW pools at +0x403C/+0x4054 `[H r5 rip.md]`.
  RIPGs are spawned at runtime (not in the oid hash) and come and go between frames `[C:runtime]`.
- Sly 2 turned Sly 1's VTRIP function tables into real C++ vtables at RIP+0xEC, built by placement construction (`PripConstructRipt` 0x1CE6D0); RIPT renumbered to 11 types `[H r5]`.
  Vtable order (same as Sly 1 VTRIP): 1 Init, 2 PostEmit, 3 OnRemove, 4 Project, 5 Update, 6 Render, 7 Touch, 8 FBounce.

| RIPT | vtable | size | Type | Conf |
|---|---|---|---|---|
| 0 | 0x2D7508 | 0xF0 | Bublet? | low |
| 1 | 0x2D7558 | 0x100 | ? (spawns child rips) | - |
| 2 | 0x2D7468 | 0x100 | Remit | med |
| 3 | 0x2D7738 | 0x100 | Flying | med-high |
| 4 | 0x2D75F8 | 0x100 | Glint? | low |
| 5 | 0x2D74B8 | 0xF0 | Leaf | med |
| 6 | 0x2D7648 | 0x110 | Match | high |
| 7 | 0x2D7788 | 0xF0 | plain | - |
| 8 | 0x2D7698 | 0x100 | Shrapnel | high |
| 9 | 0x2D75A8 | 0x110 | Smack | med |
| 10 | 0x2D76E8 | 0x100 | Stuck | med |

Fixes: 0x1CF518 is `RenderRip`, not UpdateSmack `[C:verified]`; `UpdateRipg` 0x1CE1C8 (existing `FUN_0019e420` is a Sly 1 placeholder) `[H r5 fixes]`.

## 3. BLIPs (blip.cpp 0x138F08)

BLIPG (CID 0x1C) particle groups (smoke, sparkles), Sly 1 blip.c; slots 2 Delete, 7 Init, 12 RemoveHierarchy, 16 Update, 17 ProjectTransform, 19 RenderSelf, 51 SubscribeObject `[H r2b blip.md]` (six functions verified). Spawned at runtime `[C:runtime]`.

## 4. Other effects

- Trail (CID 0x69, tu_21BB88): keeps a 60 Hz ring of its world matrices and deforms its mesh to follow the history (motion trail / streamer), fading via SO alpha `[H r8 trail_cid105.md]`.
- Shader animations (SAA family) and frame blending: see [rendering](rendering.md).
- Frame-capture transition (CID 0x68): see [cutscenes](cutscenes.md).

## 5. Source files

| Span | File (label) | Notes |
|---|---|---|
| 0x138F08-0x13A9E8 | blip |  |
| 0x159118-0x15CD68 | emitter |  |
| 0x1CDCD0-0x1D1370 | rip |  |
| 0x21BB88-0x21C820 | trail? (cid 0x69) |  |

## 6. Key functions

| Addr | Name | Purpose | Conf | Status |
|---|---|---|---|---|
| [0x138F08](../../src/hand/func_00138F08.cpp) | `BuildBlipAqwGifs` | Build static blip GIF packet for blend mode n | 0.90 | VERIFIED |
| [0x138FC0](../../src/auto/func_00138FC0.c) | `StartupBlips` | Build the two blip GIF packets (BuildBlipAqwGifs(1,&D_002DD5C0), (2,&D_002DD600)) | 0.85 | VERIFIED |
| [0x138FF8](../../src/hand/func_00138FF8.c) | `PblipNew` | Allocate a BLIP (+spline block) and append to the blipg list | 0.85 | VERIFIED |
| [0x139098](../../src/auto/func_00139098.c) | `RemoveBlip` | Move blip from its group list to the pending-free list | 0.85 | VERIFIED |
| [0x1390D8](../../src/hand/func_001390D8.c) | `PblipgNew` | Allocate a BLIPG from pool g_sw+0x4070 (0x460 bytes, vptr 0x2CB5D8), add to the cid 0x1C list, call vt7 Init, vt11 AddHierarchy, vt15 PostLoad | 0.80 | HYP |
| [0x1391C8](../../src/hand/func_001391C8.c) | `InitBlipg` | BLIPG vt7 Init | 0.85 | VERIFIED |
| [0x139250](../../src/hand/func_00139250.c) | `OnBlipgRemove` | BLIPG vt12: remove all blips, base remove, queue group for freeing | 0.85 | VERIFIED |
| [0x139AB8](../../src/hand/func_00139AB8.cpp) | `ProjectBlipgTransformAccel` | VU-accelerated particle advance with bounds | 0.85 | VERIFIED |
| [0x139DC0](../../src/hand/func_00139DC0.cpp) | `ProjectBlipgTransform` | BLIPG vt17: per-blip transform/projection (non-accelerated path) | 0.75 | HYP |
| [0x13A1E8](../../src/hand/func_0013A1E8.cpp) | `RenderBlipgSelf` | BLIPG vt19 RenderSelf: build VU1 DMA for the blip group | 0.75 | HYP |
| [0x159118](../../src/hand/func_00159118.c) | `InitEmitb` | Zeroes an EMITB and installs the default EMITO/EMITV objects (D_002DE5B0/D_002DE5F0) | 0.85 | HYP |
| [0x1591B0](../../src/hand/func_001591B0.c) | `InitEmitter` | EMITTER::Init: XFM init, default indices -1, g=10.0, ClearDl of particle list, register for update, flag 2 | 0.85 | HYP |
| [0x15A808](../../src/hand/func_0015A808.cpp) | `ChooseEmitoPos` | Chooses a particle origin position/direction from the EMITO shape (point/sphere/box/skeleton) | 0.65 | VERIFIED |
| [0x15AD48](../../src/hand/func_0015AD48.c) | `ConvertEmitoPosVec` | Transforms a chosen particle position (and velocity, depending on mode) from the reference SO space to world space | 0.70 | VERIFIED |
| [0x15B0C8](../../src/hand/func_0015B0C8.c) | `ChooseEmitvVelocityAge` | Chooses per-particle velocity (ChooseEmitVelocity) and age | 0.65 | VERIFIED |
| [0x15B1B0](../../src/hand/func_0015B1B0.cpp) | `EmitRips` | Spawns RIP particles (PripNewRipg, SetRipgEmitb, SetEmitdvEmitb) for originated positions | 0.70 | VERIFIED |
| [0x15B428](../../src/hand/func_0015B428.cpp) | `EmitBlips` | Spawns BLIP particles for originated positions | 0.65 | VERIFIED |
| [0x15B7E8](../../src/hand/func_0015B7E8.cpp) | `OriginateParticles` | Computes origin position/velocity/age for c particles (CalculateEmitvx, ChooseEmitoPos, ChooseEmitvVelocityAge, ConvertEmitoPosVec) | 0.70 | VERIFIED |
| [0x15BD20](../../src/hand/func_0015BD20.cpp) | `EmitParticles` | Emits c particles: OriginateParticles, then EmitRips (rip emitters) or [OriginateSplineSinkParticles] + EmitBlips | 0.75 | VERIFIED |
| [0x15C150](../../src/hand/func_0015C150.c) | `ExplodeExpl` | Explodes an EXPL with default (zero) parameters via its explode virtual (slot 51) | 0.75 | HYP |
| [0x15C198](../../src/hand/func_0015C198.c) | `ExplodeExplParams` | Explodes an EXPL with explicit flags/reference/vector/origin/scale/radius (packs an EXPLSO, calls slot 51) | 0.75 | HYP |
| [0x15C1E0](../../src/auto/func_0015C1E0.c) | `ExplodeExplExplso` | Empty base implementation of the EXPL explode virtual (slot 51) | 0.50 | VERIFIED |
| [0x15C1E8](../../src/hand/func_0015C1E8.c) | `LoadExplgFromBrx` | EXPLG::LoadFromBrx: XFM load, reads child explosion list into apo | 0.80 | HYP |
| [0x1CDDC8](../../src/hand/func_001CDDC8.c) | `InitRipg` | RIPG::Init (vt slot 7): InitSo + init rip DL (+0x15C ib 0xC) + flags | 0.85 | VERIFIED |
| [0x1CDE70](../../src/auto/SetRipgEmitb__FP4RIPGP5EMITB.c) | `SetRipgEmitb` | copy emitb rate into RIPG | 0.90 | VERIFIED |
| [0x1CDE80](../../src/hand/OnRipgRemove__FP4RIPG.cpp) | `OnRipgRemove` | RIPG vt slot 12 (OnRemove): release rips / clear SW default RIPG | 0.90 | HYP |
| [0x1CDF88](../../src/hand/func_001CDF88.c) | `ProjectRipgTransform` | RIPG vt slot 17: iterate rips, run due ones' project step, accumulate min/max bounds, grow by size, set center/radius | 0.70 | VERIFIED |
| [0x1CE170](../../src/auto/func_001CE170.c) | `UpdateRipgBounds` | RIPG vt slot 49: copy RIPG bounds (+0x170..+0x1A0) into its geometry/bound record (+0xEC), set dirty flag D_003E115C, call SO bound update 0x1F4BA8 | 0.50 | VERIFIED |
| [0x1CE1C8](../../src/hand/FUN_0019e420.c) | `UpdateRipg` | RIPG::Update (vt slot 16): AddRipRef/update each rip via its vtable, ReleaseRip, then BounceRipgRips if active | 0.80 | HYP |
| [0x1CE2E8](../../src/hand/func_001CE2E8.cpp) | `BounceRipgRips` | Collide live rips with nearby SO shapes (scratch stack alloc, BSP query) calling each rip's FBounce virtual | 0.85 | VERIFIED |
| [0x1CE608](../../src/hand/RenderRipgSelf__FP4RIPGP2CMP2RO.cpp) | `RenderRipgSelf` | RIPG vt slot 19 RenderSelf: render each rip via vtable | 0.90 | HYP |
| [0x1CE888](../../src/hand/func_001CE888.c) | `PripNewRipg` | Allocate a RIP from the rip pool (D_003E1144?), construct by type, link into RIPG dlRip, AddRipRef | 0.85 | VERIFIED |
| [0x1CE920](../../src/hand/func_001CE920.c) | `InitRip` | RIP default Init (vt entry 1): tCreated=g_clock.t, scale=1, pos/posPrev, identity matrix | 0.85 | VERIFIED |
| [0x1CE980](../../src/auto/RemoveRip__FP3RIP.c) | `RemoveRip` | unlink from RIPG dl + ReleaseRip | 0.90 | HYP |
| [0x1CE9C0](../../src/auto/AddRipRef__FP3RIP.c) | `AddRipRef` | cRef++ | 0.90 | HYP |
| [0x1CE9D0](../../src/hand/func_001CE9D0.c) | `ReleaseRip` | Decref; at 0 call OnRemove virtual, notify subscribed LOs (msg 8) and free subscription nodes, free rip | 0.85 | VERIFIED |
| [0x1CEA88](../../src/auto/TouchRip__FP3RIPi.c) | `TouchRip` | if fTouching RemoveRip (default vt entry 7) | 0.90 | HYP |
| [0x1CEAA8](../../src/hand/ForceRipFade__FP3RIPf.cpp) | `ForceRipFade` | start fade | 0.90 | VERIFIED |
| [0x1CEB08](../../src/hand/func_001CEB08.cpp) | `FBounceRip` | RIP default FBounce (vt entry 8): reflect velocity off an SO contact, friction/elasticity, ForceRipFade | 0.75 | VERIFIED |
| [0x1CF518](../../src/hand/func_001CF518.cpp) | `RenderRip` | RIP default Render (vt entry 6): FRenderRipPosMat, else RemoveRip | 0.75 | VERIFIED |
| [0x1CFAB8](../../src/hand/PostShrapnelEmit__FP8SHRAPNELP5EMITB.cpp) | `PostShrapnelEmit` | SHRAPNEL (RIPT 8) PostEmit | 0.85 | VERIFIED |
| [0x1CFC88](../../src/hand/RenderShrapnel__FP8SHRAPNELP2CM.cpp) | `RenderShrapnel` | SHRAPNEL Render | 0.85 | HYP |
| [0x1CFCD8](../../src/hand/InitMatch__FP5MATCHP6VECTORfP2SO.cpp) | `InitMatch` | MATCH (RIPT 6) Init | 0.80 | HYP |
| [0x1CFD30](../../src/hand/func_001CFD30.cpp) | `LaunchMatch` | Aim match at its target object: take target pose, decompose rotation, set launch velocity | 0.65 | VERIFIED |
| [0x1CFE88](../../src/hand/func_001CFE88.cpp) | `ProjectMatchTransform` | MATCH Project: Hermite-follow target pose | 0.70 | VERIFIED |
| [0x1D0038](../../src/hand/func_001D0038.c) | `UpdateMatch` | MATCH Update: LaunchMatch / RemoveRip | 0.70 | VERIFIED |
| [0x1D0190](../../src/hand/func_001D0190.cpp) | `RenderGlint` | RIPT 4 Render: large custom draw (fades; RemoveRip when done) | 0.45 | VERIFIED |
| [0x1D04E8](../../src/hand/func_001D04E8.c) | `InitSmack` | SMACK (RIPT 9) Init | 0.70 | VERIFIED |
| [0x1D05A0](../../src/hand/func_001D05A0.c) | `ProjectSmackTransform` | SMACK Project | 0.70 | VERIFIED |
| [0x1D0800](../../src/hand/func_001D0800.cpp) | `UpdateSmack` | SMACK Update (RemoveRip when done) | 0.65 | VERIFIED |

Selected 50 of 127 named functions (confidence >= 0.75, verified, or flagged). Full list: [reference/functions/particles-effects.md](../reference/functions/particles-effects.md).

## Open questions
- RIPT 0, 1, 4, 7 identities; EXPLG vs EXPLO roles.
- Experiment: break 0x15BD20 while an emitter is active (a0 = particles per frame); break `RenderBlipgSelf` 0x13A1E8 during smoke.
