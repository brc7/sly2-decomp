# Physics, collision and navigation

Related: [object-model](object-model.md), [player](player.md), [core-library](core-library.md) (BSP, DZG, Crout math).

## 1. SO, the solid object (so.cpp 0x1F2108-0x1F8DD8, 174 functions)

SO (CID 0x30, 0x150 bytes, LO child, vtable 0x2D9978, 75 slots) is the base of every physical/collidable object `[C:runtime]`.
Sly 2 split Sly 1's huge SO into separate records `[H r6 so.md]` (offsets `[C:runtime]` where marked):

| Off | Field | Tag |
|---|---|---|
| +0x50 / +0x54 | `cxf` / `XF *axf` (0x100-byte transform records) | [C:runtime] |
| +0x58 / +0x5C / +0x60 | position / rotation / root XF (world matrix of root at XF+0x40, position row at XF+0x70) | [C:runtime] (+0x60) |
| +0x64 / +0x70 | child DL / first removed child | [C:runtime] |
| +0x80 | GLOBSET (model; 0x60 bytes) | [H] |
| +0xE0 / +0xE4 | collision parts SOPART[] (0xB0 each: geometry, BSP, radius, zap kinds) | [H] |
| +0xE8 | SOPHYS (0xE0: inertia, gravity +0x80, constraints, mass +0xD0) — 462 of 1,196 SOs have one; JT mass 0.25, gravity z -2275 | [C:runtime] |
| +0xEC | contact-tree record per physics root | [H] |
| +0x110 | 26-entry zap response table (by zap-kind bit) | [H] |
| +0x12C / +0x130 | alpha (1.0) / alpha rate: fading built into SO; UpdateSo removes the object at alpha 0 | [C:runtime] (+0x12C) |
| +0x138 | external-acceleration (XA) list (water currents...) | [H r8 water.md] |
| +0x13C | float 400.0 (REFUTED "= const address 0x2C3B90") | [C:runtime] |
| +0x148 | flags: damage bytes (10/20), zap mode, fIceable, sphere-tree flag | [H] |

SO-only slots 49-75 (LO vtable 0x2D3540 ends at slot 48 = `ClearLoGrf` 0x1A66F8; SO vtable 0x2D9978 has 75 slots, 49 = 0x1F50E0, 75 = 0x1F7588) `[C:static v3.1 re-check]`; roles `[H r6 so.md]`: 49 tree bounds, 50 GetCpdefi, 53 PtreeNew, 54 contact notify, 55 AdjustNewXp (contact elasticity),
58 touch, 61 `FIgnoreIntersection` (tri-state 1/2/3, sides combined with max), 62 FIsSoFixed, 64 ResetPhys, 68 contact impulse,
70 PresetAccel, 71/72 XA hooks, 73 FPosInSo, 74 contact zaps, 75 FTakeDamage (last SO slot).
Caution: the old Sly 1-alignment names in 0x1F6C08-0x1F8C40 are shifted almost everywhere; do not promote them `[H r6]`.

## 2. Contacts and solving

| Stage | Where | What | Tag |
|---|---|---|---|
| Broadphase | bbmark.cpp 0x1336F0 | 3-axis sweep-and-prune over SW endpoint arrays, insertion sort; crossings create/destroy pair records (OX) with keep/force/unpair; Sly 1 bbmark.c counterpart | [H r2b bbmark.md] |
| Contact generation | mark.cpp 0x1A7C18 | `PxpAdd`, `RecalcSwXpPos/Movement`, BSP-BSP, sphere-BSP, sphere-sphere contacts, `AddSoXps` (Sly 1 mark.cpp order) | [C:verified] (RecalcSwXp*) |
| Solve | solve.cpp 0x1F8DD8 | `SolveSw` (per frame from UpdateSw): flood-fill contact groups, collect up to 64 contacts, `SolveSwGroup` 0x1F9410 with the DZG LCP solver | [H r7 solve.md] |
| LCP solver | dzg.cpp 0x11A550 / 0x1585D8 | `SolveDzg`, `SolveDzgFric`, inequality solver (Sly 1 dzg.c) | [H r1, r3b] |
| Constraints / impulses | solve.cpp second half | constraint projection, impulse -> dv/dw, velocity at a point | [H r7] |

## 3. Zaps (damage delivery)

`InflictZap` 0x1F7598 delivers a zap record (kind bits, damage), dedupes through a 0.25 s history expired by UpdateSw (0x1F77C0), and messages
both sides (message 0xB); each SO answers through its 26-entry response table `[H r6 so.md]`. Zap kinds seen: 0x1000 laser beam, 0x4000 guard stuck watch,
0x80000/0x100000 special in STEPPLAYER vt86 `[H r6b, r7c, r2b]`.

## 4. Geometry and queries

- clip.cpp (0x14C6A0): `PsoHitTestLineObjects` (segment vs objects); 0x14C9F8 is not ClsgClipEdgeToSphere `[C:verified]` but an LSG-list merge (`ClsgMergeAlsg`), and the sphere clip is 0x14C748 `[H r2b clip.md]`.
- geom.cpp (0x166270): `CloneGeom`, `UpdateGeomWorld` (points via VU0, inverse-transpose normals), `CpedgeFromGeomPoint` `[H r3 geom.md]`.
- bsp.cpp (0x118668): `ClsgClipEdgeToBsp`, `PruneBsp` (both verified), point-in-BSP `[C:verified]`.
- vol.cpp (0x227BB8): VOL (CID 0x16) geometry + BSP + bounding sphere; point/sphere containment, used widely by guards and camera `[H r8 vol.md]`.
- shape.cpp (0x1E5D80): SHAPE (CID 0xA) paths (polyline or Hermite, time and distance tables) absorbed Sly 1's CRV curve code; closest-point
  queries used by pipe/rail/grab code, BHVs and scripts; optional rope "chain" `[H r6b shape.md]`.
- water.cpp (0x228B48): WATER (CID 0x59) + the XA external-acceleration list (SO slots 71/72) `[H r8 water.md]`.
- Launch kinematics (tu_1D6848): `VecLaunchBallistic`, `VecToReachPosInDt`, launcher objects `[H r6]`; `DtBallisticMax` in the air TU.
- Convex volumes (tu_1BC050, CID 0x0E, 180 instances): segment clipping against convex volumes, registered as pathzone obstacles `[H r5 SUMMARY, r5b path.md]`.

## 5. Navigation (path.cpp 0x1BF110-0x1C2920)

- **PATHZONE** (CID 0x11, XFM child): 2D navigation graph (CG) with BSP point location, A* (`FindPathAStar`), path smoothing, closest point — Sly 1
  path.cpp one-for-one for the first 20 functions `[H r5b path.md]`.
- New in Sly 2: a **world zone BSP** whose leaves are pathzones (SW+0x45CC; `PpathzoneFromPos` 0x1C0D70 is called by UpdateSo so every SO knows
  its zone); **dynamic obstacles** (per-zone sweep-and-prune, nav version counter SW+0x3D70); **inter-zone routing** over CID 0x7C link objects
  (`FindPathlinkRoute` 0x1C21D8, A* over zones, used by guard AI) `[H r5b]`. Hub counts: 51 pathzones, 262 CID 0x7C links `[C:runtime]`.

## Devtools findings (v3, 2026-11-01)

From [devtools/collision_paths](../guides/devtools.md#collision_paths) (README + FORMAT.txt; tags as given by the tool, not re-derived in v3):
- Collision hulls, PATHZONE navmeshes + adjacency, PATHLINKs, SHAPE paths, pipes, rails, wall grabs, volumes, markers and spawns are exported as
  overlay layers for all 43 levels; all 121,153 object records parse with payloads ending at cb `[C:parse collision_paths]`.
- Placement vs RAM (oid within 2 units): S02/L02 1637/1692, S63/L03 609/620, S65/L08 1303/1404; pipes, rails, wall grabs, convex shapes, TN, VOLBTN,
  table grabs, carry grabspots and LASEN match 100 %; misses are dynamic objects `[C:runtime collision_paths]`. Spot-check v3: verify_S02.txt shows
  LIGHT 248/248, PROXY 385/385, PIPE 16/16, RAIL 21/21.
- **PATHZONE points are world-space** (centroids sit at the XFM translation, 48/49; applying the transform puts zones 60-70 m off) `[C:runtime+visual collision_paths]`.
- Laser beam lengths recomputed from data equal RAM for 52/52 beams (S63, S64) `[C:runtime collision_paths]`.

## 6. Source files

{{TUS}}

## 7. Key functions

{{KEYFUNCS:0x1F7598 0x1F8DD8 0x1F9410 0x1C0D70 max=70 minc=0.75}}

## Open questions
- SO slots 65-72 and 49-60 naming; `fIgnoreLocked` label on 0x1F2768 is suspect (verifier r4r6).
- Exact SW offsets of the sweep-and-prune endpoint arrays (+0x3CD4 region) and the object cap.
- Experiment: break 0x1F9410 while pushing a crate (once per frame per group); break 0x1C0D70 as a guard crosses rooms.
