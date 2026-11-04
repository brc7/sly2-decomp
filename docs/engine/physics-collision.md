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

| Span | File (label) | Conf | Notes |
|---|---|---|---|
| 0x1336F0-0x1344F0 | bbmark | medium | Sly1 bbmark sweep-and-prune |
| 0x14C6A0-0x14CFE0 | clip | medium |  |
| 0x166270-0x166920 | geom | high |  |
| 0x1A7C18-0x1AA978 | mark | high |  |
| 0x1BC050-0x1BE398 | tu_1BC050 | low | CID14 convex polygon SHAPE; was labelled jt |
| 0x1BF110-0x1C2920 | path | high | Sly1 path.cpp order |
| 0x1D6848-0x1D7238 | tu_1D6848 | low | object launcher / ballistic math |
| 0x1E5D80-0x1EA568 | shape | high |  |
| 0x1F2108-0x1F8DD8 | so | high |  |
| 0x1F8DD8-0x1FAF70 | solve | high | tu_table said so; was labelled so |
| 0x227BB8-0x227FE8 | vol | high |  |
| 0x228B48-0x229EF0 | water | medium |  |

## 7. Key functions

| Addr | Name | Purpose | Conf | Tier | Evidence |
|---|---|---|---|---|---|
| [0x133C68](../../src/game/engine/physics/bbmark.cpp) | `PoxAddSw` | Allocate OX from pool g_sw+0x3CF8, psoOther = other, push on poxa list | 0.80 | T1 |  |
| [0x14C6A0](../../src/game/engine/physics/clip.cpp) | `ClsgClipEdgeToObject` | Clip segment against an SO: ClsgClipEdgeToObjectPruned(pso, pso->bsp (+0x74), ...) | 0.80 | T1 |  |
| [0x14C9F8](../../src/game/engine/physics/clip.cpp) | `ClsgMergeAlsg` | Merge two sorted LSG interval lists into an output list with a set operation selected by k (0 = union ...) | 0.85 | T1 | verified |
| [0x166270](../../src/game/engine/physics/geom.cpp) | `CloneGeom` | Copies a GEOM (points + surface normals) into the SW heap and optionally transforms the points by a matrix and rotates/renormalizes the normals, recom | 0.85 | T1 | verified |
| [0x1A7C18](../../src/game/engine/physics/mark/PxpAdd.c) | `PxpAdd` | Allocate an XP contact (slotheap) between two objects at pos/normal with distance g and kind, convert to local frames, compute movement, link into *pp | 0.75 | T1 | verified |
| [0x1A7E60](../../src/game/engine/physics/mark/RecalcSwXpPos.c) | `RecalcSwXpPos` | For each XP in the array recompute world positions of its two sides (ConvertXfPos) | 0.85 | T1 | verified |
| [0x1A7FC0](../../src/game/engine/physics/mark/RecalcSwXpMovement.c) | `RecalcSwXpMovement` | For each XP recompute the relative velocity at the contact (GetXfVelocity of both sides) | 0.85 | T1 | verified |
| [0x1A86B8](../../src/game/engine/physics/mark/CplsgFindEdgePath__FP4SURFiiP3LSGPP3LSG.c) | `CplsgFindEdgePath` | Breadth search (64 nodes) for a chain of LSGs connecting two positions along shared edges | 0.55 | T1 | verified |
| [0x1A8B50](../../src/game/engine/physics/mark/MarkSoContactsBspBsp.cpp) | `MarkSoContactsBspBsp` | Edge/edge and vertex/face contacts between two BSP shapes: clip edges both ways, CategorizeAlsg, embedded vertex XPs, edge paths and closest points (k | 0.85 | T1 | verified |
| [0x1BC088](../../src/game/engine/physics/tu_1BC050.cpp) | `LoadCid14FromBrx` | CID 14 vt14: LoadShapeFromBrx, allocate plane array (count-1) | 0.75 | T2 | parse |
| [0x1BC278](../../src/game/engine/physics/tu_1BC050.cpp) | `SgnTurnXY` | Sign of 2D cross product of (a-b),(b-c) with epsilon | 0.45 | T2 | verified |
| [0x1BC2F8](../../src/game/engine/physics/tu_1BC050.cpp) | `ClassifyCid14Polygon` | Classify polygon (too few points / convex CW / CCW / not convex) from turn signs | 0.40 | T2 | verified |
| [0x1BC688](../../src/game/engine/physics/tu_1BC050.cpp) | `ClsgClipEdgeToCnvo` | Clip segment to the convex volume and write the parts outside it (LSG 0x60) | 0.85 | T1 | verified |
| [0x1BD218](../../src/game/engine/physics/tu_1BC050.cpp) | `DetachCid14FromOwner` | Remove from merged owner's DL (with safe DL iterator) and re-queue | 0.35 | T2 | verified |
| [0x1BD8A8](../../src/game/engine/physics/tu_1BC050.cpp) | `FCnvoOverlap` | Two convex polygons overlap (edge clip or containment) | 0.85 | T1 | verified |
| [0x1BDBC8](../../src/game/engine/physics/tu_1BC050.cpp) | `NCmpPosX` | qsort comparator on first float (used to sort hull points by x) | 0.55 | existing | arbitration; replaces existing NCmpWkr__FP3WKRT0 |
| [0x1BF398](../../src/game/engine/physics/path/CbskFromG__Ff.c) | `CbskFromG` | Plane side class of a distance: 0 front, 1 back, 2 on (epsilon D_002C0C00) | 0.80 | T1 |  |
| [0x1BF970](../../src/game/engine/physics/path/IcgvFromPcgv__FP2CGP3CGV.c) | `IcgvFromPcgv` | Index of a graph vertex: (pcgv - pcg->acgv) >> 5 | 0.85 | T1 | verified |
| [0x1BFF28](../../src/game/engine/physics/path/CposFindPath.cpp) | `CposFindPath` | Route between two points over the pathzone graph: locate triangles, A*, simplify, copy out, min spacing | 0.75 | T1 | verified |
| [0x1C03C0](../../src/game/engine/physics/path/LoadPathzoneFromBrx.c) | `LoadPathzoneFromBrx` | CID 17 vt14: LoadXfmFromBrx, read vertices (pos + edge list), triangles, BSP nodes; HookupCg | 0.85 | T1 | parse |
| [0x1C0D70](../../src/game/engine/physics/path/PpathzoneFromPos.c) | `PpathzoneFromPos` | Walk the world zone BSP (SW+0x45CC) to the pathzone containing pos; default world pathzone SW+0x45C8 | 0.55 | T1 |  |
| [0x1C0EC0](../../src/game/engine/physics/path/LoadSwZoneBspFromBrx.c) | `LoadSwZoneBspFromBrx` | Read world zone BSP (if count) into SW+0x45CC | 0.75 | T1 | parse |
| [0x1C0F30](../../src/game/engine/physics/path/LoadZoneBspFromBrx.c) | `LoadZoneBspFromBrx` | Read zone BSP nodes (plane index + 2 children), planes, leaf zone ids | 0.75 | T1 | parse |
| [0x1E5DF8](../../src/game/engine/physics/shape/LoadShapeFromBrx.c) | `LoadShapeFromBrx` | SHAPE::LoadFromBrx (vt14): XFM xf, mode(+0xE0: 0 linear, 1 curve), fClosed(+0xE4), count, then times/dists/points (+tangent arrays +0x140/+0x144 for c | 0.80 | T1 | parse |
| [0x1E6648](../../src/game/engine/physics/shape/SMaxShape.c) | `SMaxShape` | return dists[c-1] (total length) | 0.85 | T1 | verified |
| [0x1E7A28](../../src/game/engine/physics/shape/SetShapeChainMode.c) | `SetShapeChainMode` | n!=-1: ensure chain and enable updates (LO flags); -1: disable; store mode in chain +0x4C | 0.85 | T1 | verified |
| [0x1E8008](../../src/game/engine/physics/shape/SRandomShapeRange.c) | `SRandomShapeRange` | Random s within +-b of a (wrapped or clamped to the shape), distribution chosen by D_002C3650 | 0.85 | T1 | verified |
| [0x1E81C0](../../src/game/engine/physics/shape/InitChain.c) | `InitChain` | Initialise a chain record for a shape (copy points/dists to a private array, set rest lengths) | 0.85 | T1 | verified |
| [0x1E9858](../../src/game/engine/physics/shape/func_001E9858.cpp) | `PostCid106Load` | CID 0x6A (SO child 0x160) vt15: PostSoLoad then rebuild each mesh vertex as (offset in path-local basis, distance along path) | 0.35 | T2 | verified |
| [0x1F2108](../../src/game/engine/physics/so/InitSo.c) | `InitSo` | SO vt7 Init: InitLo, SetLoBusyk(0), clear DL +0x64, dhp bytes +0x148/+0x149 = -10/-20, gAlpha(+0x12C)=1, +0x13C=D_002C3B90, inherit +0x148 bit35 from  | 0.85 | T1 | verified |
| [0x1F2718](../../src/game/engine/physics/so/func_001F2718.c) | `SetSoField148_20` | Set 4-bit field +0x148 bits 20-23 | 0.20 | T2 | verified |
| [0x1F2768](../../src/game/engine/physics/so/SetSoIgnoreLocked__FP2SOi.cpp) | `SetSoIgnoreLocked__FP2SOi` | Set +0xD0 bit34 (fIgnoreLocked) | 0.85 | existing | verified |
| [0x1F2798](../../src/game/engine/physics/so/PsoFindPhysRoot.c) | `PsoFindPhysRoot` | Walk up the parent chain to the first SO that has non-fixed physics (SOPHYS modes != 3) or flag +0x148 bit34; returns last visited | 0.85 | T1 | verified |
| [0x1F2A60](../../src/game/engine/physics/so/ShowSoPart.cpp) | `ShowSoPart` | If part hidden (fl bit50): clear it and, when SO in world, AddSoPartToTree | 0.85 | T1 | verified |
| [0x1F2B20](../../src/game/engine/physics/so/AddSoParts.c) | `AddSoParts` | AddSoPartToTree for every visible SOPART (+0xE0 count / +0xE4 array, 0xB0 stride) | 0.85 | T1 | verified |
| [0x1F31E0](../../src/game/engine/physics/so/LoadSoFromBrx.cpp) | `LoadSoFromBrx` | SO vt14: LoadLo base, read model/globset, SOPART array (XF id, flags, radius, local geom, BSP...), physics block (matrix, mass), options, bounds | 0.90 | T1 | parse, verified |
| [0x1F35F8](../../src/game/engine/physics/so/AdjustSoMomint.c) | `AdjustSoMomint` | Scale SOPHYS inertia matrix by r and recompute its inverse (zero if singular) | 0.85 | T1 | verified |
| [0x1F3C70](../../src/game/engine/physics/so/RenderSoSelf.c) | `RenderSoSelf` | SO vt19: if visible in current vis mask (+0x2D & D_002DDF54) draw globset (+0x80) with alpha +0x12C | 0.85 | T1 | verified |
| [0x1F3E00](../../src/game/engine/physics/so/OnSoRemove.c) | `OnSoRemove` | SO vt22: LO::Freeze, unregister (0x1D6310), vt22 children, if tree owner: vt49 + remove from D_003E11AC | 0.85 | T1 | verified, arbitration |
| [0x1F3EA8](../../src/game/engine/physics/so/OnSoAdd.c) | `OnSoAdd` | SO vt23: LO::Unfreeze, re-register, vt23 children, add tree to D_003E11AC and vt49 | 0.85 | T1 | arbitration |
| [0x1F4558](../../src/game/engine/physics/so/GetSoAngularVelocity.cpp) | `GetSoAngularVelocity` | SO vt34: world angular velocity of rotation XF (kind 6), zero otherwise | 0.90 | T1 | verified |
| [0x1F4CA0](../../src/game/engine/physics/so/func_001F4CA0.c) | `CloneXfRef` | SO vt52: re-point an XF* from base SO's XF block to the clone's (also via parent) | 0.85 | T1 | verified |
| [0x1F4FC8](../../src/game/engine/physics/so/FIgnoreSoIntersection.c) | `FIgnoreSoIntersection` | SO vt61: true if (pa flag +0x148 bit26 or pa fixed) and pb fixed | 0.85 | T1 | verified |
| [0x1F57C0](../../src/game/engine/physics/so/SetSoCnstrTorque.c) | `SetSoCnstrTorque` | Same for rotation XF into SOPHYS+0x90 | 0.85 | T1 | verified |
| [0x1F5860](../../src/game/engine/physics/so/GetSoDvGravity.c) | `GetSoDvGravity` | SOPHYS+0x80 (gravity vector) or zero | 0.85 | T1 | verified |
| [0x1F58F0](../../src/game/engine/physics/so/GetSoCpdefi.c) | `GetSoCpdefi` | SO vt50: camera default info: base pos/velocity, radHome = atan2(-m01,-m00) of upright matrix, sAdjust 100, grfcpd 0, tMoveLast = g_clock.t | 0.85 | T1 |  |
| [0x1F6068](../../src/game/engine/physics/so/CloneSo.cpp) | `CloneSo` | SO vt41 Clone: copy XF block, model, parts, physics for a clone | 0.90 | T1 | verified |
| [0x1F66F0](../../src/game/engine/physics/so/CloneSoHierarchy.c) | `CloneSoHierarchy` | SO vt40: vt41 Clone then PloCloneLo each non-removed child | 0.85 | T1 | verified |
| [0x1F6AC8](../../src/game/engine/physics/so/ShowSoPartsById.c) | `ShowSoPartsById` | ShowSoPart for parts whose id (+0xC) == k | 0.85 | T1 | verified |
| [0x1F7598](../../src/game/engine/physics/so/func_001F7598.c) | `PsoInflictZap` | Deliver a zap record to the first SO in the target chain that accepts it: dedupe against the recent-zap list D_003E15C0, compute GrfzpdFromSoGrfzpk, a | 0.55 | T2 | top50; top50 round; v2 name InflictZap |
| [0x1F77C0](../../src/game/engine/physics/so/ExpireSwZapRecords.c) | `ExpireSwZapRecords` | Drop zap-history entries older than 0.25 s from g_sw list | 0.85 | T1 | verified |
| [0x1F78D8](../../src/game/engine/physics/so/GrfzpdFromSoGrfzpk.c) | `GrfzpdFromSoGrfzpk` | OR of per-kind response bytes SO+0x110[26] selected by grfzpk | 0.85 | T1 | verified |
| [0x1F7D90](../../src/game/engine/physics/so/SetSoFadeRate.c) | `SetSoFadeRate` | If rate != 0: Add (vt8) and set grf bit 2; store +0x130 | 0.85 | T1 | verified |
| [0x1F8198](../../src/game/engine/physics/so/PsaaFindSoOid.c) | `PsaaFindSoOid` | Find SAA in +0xA4 list with +0x18 == n | 0.85 | T1 | verified |
| [0x1F82B8](../../src/game/engine/physics/so/func_001F82B8.c) | `SetSoSideRecordParams` | Ensure object then set x and optional 8-byte key | 0.45 | T2 | verified |
| [0x1F8470](../../src/game/engine/physics/so/func_001F8470.c) | `SetSoPhysParam4` | record.g4 = f | 0.25 | T2 | verified |
| [0x1F8740](../../src/game/engine/physics/so/DecrefXfh.c) | `DecrefXfh` | --cref; at 0 destroy and free to pool D_003E15CC | 0.85 | T1 | verified |
| [0x1F8870](../../src/game/engine/physics/so/func_001F8870.c) | `GetXfhParamAC2` | duplicate of 0x1F87A0 (different vtable) | 0.15 | T2 | verified |
| [0x1F8DD8](../../src/game/engine/physics/solve.cpp) | `SolveSw` | Per-frame physics solve over the world: pre-pass 0x134040, vt17 on entries with (grf&0x24)==4, then solve contact groups (0x1F8F80) | 0.70 | T1 |  |
| [0x1F9410](../../src/game/engine/physics/solve.cpp) | `SolveSwGroup` | Solve one contact group: build DZG rows from contacts, set targets, SolveDzg, apply impulses, notify objects (vt 65/66/67) | 0.75 | T1 |  |
| [0x1F9C48](../../src/game/engine/physics/solve.cpp) | `IntegrateSoGroupXf` | Integrates position/rotation of SOs in a solved group (NormalizeRotateMatrix3, UpdateSoPartGeom) stamped with SW frame counter +0x3D58 | 0.25 | T2 | runtime-corrected |
| [0x1FA810](../../src/game/engine/physics/solve.cpp) | `ApplyConstr` | Return a vector constrained by a CONSTR: kind 3 locked -> zero, 2 -> projected on (world-transformed) normal, 1 -> plane-projected | 0.85 | T1 | verified |
| [0x1FAC30](../../src/game/engine/physics/solve.cpp) | `AddSoAcceleration` | If the position XF is in a velocity role (1-3): xf->dv(+0xF0) += world vector converted into the XF parent space | 0.80 | T1 | top50; top50 round; v2 name AddSoVelocity |
| [0x227BB8](../../src/game/engine/physics/vol.cpp) | `LoadVolFromBrx` | LoadXfmFromBrx, ReadGeom, ReadBspc, then bounding sphere of the geometry points | 0.90 | T1 | parse, verified |
| [0x227E20](../../src/game/engine/physics/vol.cpp) | `OnCid23Add` | OnLoAdd + AppendDlEntry(&D_003E1480) | 0.85 | T2 | verified, arbitration |
| [0x227E58](../../src/game/engine/physics/vol.cpp) | `OnCid23Remove` | OnLoRemove + RemoveDlEntry(&D_003E1480) | 0.85 | T2 | arbitration |
| [0x228D20](../../src/game/engine/physics/water.cpp) | `UpdateWaterXaList` | rebuilds the per-object XA records (add/remove, free unused) | 0.40 | T2 | verified |
| [0x229710](../../src/game/engine/physics/water.cpp) | `AddSoXa` | push XA on SO XA list (0x138) then 0x1F5078 (wake) | 0.85 | T1 |  |
| [0x229738](../../src/game/engine/physics/water.cpp) | `RemoveSoXa` | unlink XA from SO list then 0x1F5078 | 0.85 | T1 |  |
| [0x2297F0](../../src/game/engine/physics/water.cpp) | `AddSoExternalAccelerations` | empty default | 0.80 | T1 | verified |

Selected 70 of 389 named functions (T1 with confidence >= 0.75, or runtime/verified/parse/arbitration evidence, or cited in the text). Full list: [reference/functions/physics-collision.md](../reference/functions/physics-collision.md).

## Open questions
- SO slots 65-72 and 49-60 naming; `fIgnoreLocked` label on 0x1F2768 is suspect (verifier r4r6).
- Exact SW offsets of the sweep-and-prune endpoint arrays (+0x3CD4 region) and the object cap.
- Experiment: break 0x1F9410 while pushing a crate (once per frame per group); break 0x1C0D70 as a guard crosses rooms.
