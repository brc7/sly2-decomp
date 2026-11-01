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

{{TUS}}

## 6. Key functions

{{KEYFUNCS:max=50 minc=0.75}}

## Open questions
- RIPT 0, 1, 4, 7 identities; EXPLG vs EXPLO roles.
- Experiment: break 0x15BD20 while an emitter is active (a0 = particles per frame); break `RenderBlipgSelf` 0x13A1E8 during smoke.
