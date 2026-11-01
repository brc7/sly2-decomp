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

SO-only slots 48-74 `[H r6 so.md]`: 49 tree bounds, 50 GetCpdefi, 53 PtreeNew, 54 contact notify, 55 AdjustNewXp (contact elasticity),
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

## 6. Source files

| Span | File (label) | Notes |
|---|---|---|
| 0x1336F0-0x1344F0 | bbmark |  |
| 0x14C6A0-0x14CFE0 | clip |  |
| 0x166270-0x166920 | geom |  |
| 0x1A7C18-0x1AA978 | mark |  |
| 0x1BC050-0x1BE398 | tu_1BC050 (cid 0x0E + convex volumes) | was labelled jt |
| 0x1BF110-0x1C2920 | path |  |
| 0x1D6848-0x1D7238 | tu_1D6848 (launch kinematics) |  |
| 0x1E5D80-0x1EA568 | shape |  |
| 0x1F2108-0x1F8DD8 | so |  |
| 0x1F8DD8-0x1FAF70 | solve | was labelled so |
| 0x227BB8-0x227FE8 | vol |  |
| 0x228B48-0x229EF0 | water |  |

## 7. Key functions

| Addr | Name | Purpose | Conf | Status |
|---|---|---|---|---|
| [0x1336F0](../../src/hand/func_001336F0.c) | `AddSwAaobrObject` | Allocate the object's OXA node (pool g_sw+0x3CE0), append to SW lists, and add min/max endpoints on each of 3 axes to the sweep arrays (count += 2) | 0.75 | HYP |
| [0x1337D0](../../src/hand/func_001337D0.c) | `RemoveSwAaobrObject` | Remove an object from the broadphase: for each pair notify both (vt54), free XP lists, remove from SW lists and delete its endpoints from the 3 axis a | 0.75 | HYP |
| [0x133C68](../../src/auto/func_00133C68.c) | `PoxAddSw` | Allocate OX from pool g_sw+0x3CF8, psoOther = other, push on poxa list | 0.80 | HYP |
| [0x133CB8](../../src/hand/func_00133CB8.c) | `PoxRemoveSw` | Unlink and return the OX whose other matches | 0.75 | HYP |
| [0x133E80](../../src/hand/func_00133E80.c) | `UpdateSwAaox` | If dirty: insertion-sort the 3 endpoint arrays; when a min/max pair crosses call UpdateSwPox(..,0) (sweep and prune) | 0.75 | HYP |
| [0x134320](../../src/hand/func_00134320.c) | `RecalcSwOxfFilterForObject` | If the object has a broadphase node (+0xEC): refresh (0x1F4BF8) and UpdateSwPox against every other node | 0.75 | HYP |
| [0x14C6A0](../../src/hand/func_0014C6A0.c) | `ClsgClipEdgeToObject` | Clip segment against an SO: ClsgClipEdgeToObjectPruned(pso, pso->bsp (+0x74), ...) | 0.80 | HYP |
| [0x14C6D8](../../src/hand/func_0014C6D8.c) | `ClsgClipEdgeToObjectPruned` | If a BSP is given ClsgClipEdgeToBsp, else if the object is a sphere (bit 49) ClsgClipEdgeToSphere with its center/radius | 0.75 | HYP |
| [0x14C748](../../src/hand/func_0014C748.cpp) | `ClsgClipEdgeToSphere` | Segment/sphere intersection: quadratic in u, clamp to [0,1], emit up to 2 LSG entries | 0.75 | HYP |
| [0x166270](../../src/hand/func_00166270.c) | `CloneGeom` | Copies a GEOM (points + surface normals) into the SW heap and optionally transforms the points by a matrix and rotates/renormalizes the normals, recom | 0.80 | VERIFIED |
| [0x1A7C18](../../src/hand/func_001A7C18.c) | `PxpAdd` | Allocate an XP contact (slotheap) between two objects at pos/normal with distance g and kind, convert to local frames, compute movement, link into *pp | 0.75 | VERIFIED |
| [0x1A7E60](../../src/hand/func_001A7E60.c) | `RecalcSwXpPos` | For each XP in the array recompute world positions of its two sides (ConvertXfPos) | 0.70 | VERIFIED |
| [0x1A7FC0](../../src/hand/func_001A7FC0.c) | `RecalcSwXpMovement` | For each XP recompute the relative velocity at the contact (GetXfVelocity of both sides) | 0.70 | VERIFIED |
| [0x1A86B8](../../src/hand/func_001A86B8.c) | `CplsgFindEdgePath` | Breadth search (64 nodes) for a chain of LSGs connecting two positions along shared edges | 0.55 | VERIFIED |
| [0x1A8B50](../../src/hand/func_001A8B50.cpp) | `MarkSoContactsBspBsp` | Edge/edge and vertex/face contacts between two BSP shapes: clip edges both ways, CategorizeAlsg, embedded vertex XPs, edge paths and closest points (k | 0.65 | VERIFIED |
| [0x1BF398](../../src/hand/func_001BF398.c) | `CbskFromG` | Plane side class of a distance: 0 front, 1 back, 2 on (epsilon D_002C0C00) | 0.80 | HYP |
| [0x1BF3D8](../../src/hand/func_001BF3D8.cpp) | `ClsgClipEdgeToCbsp` | Clip segment against the pathzone BSP, writing crossed leaf spans (LSG rows 96 B) | 0.75 | HYP |
| [0x1BF970](../../src/auto/func_001BF970.c) | `IcgvFromPcgv` | Index of a graph vertex: (pcgv - pcg->acgv) >> 5 | 0.85 | VERIFIED |
| [0x1BF980](../../src/hand/func_001BF980.cpp) | `FindPathAStar` | A* over the pathzone vertex graph | 0.75 | HYP |
| [0x1BFF28](../../src/hand/func_001BFF28.cpp) | `CposFindPath` | Route between two points over the pathzone graph: locate triangles, A*, simplify, copy out, min spacing | 0.75 | VERIFIED |
| [0x1C0378](../../src/auto/func_001C0378.c) | `InitPathzone` | CID 17 vt7: InitXfm, ids -1, init DL | 0.75 | HYP |
| [0x1C03C0](../../src/hand/func_001C03C0.c) | `LoadPathzoneFromBrx` | CID 17 vt14: LoadXfmFromBrx, read vertices (pos + edge list), triangles, BSP nodes; HookupCg | 0.80 | HYP |
| [0x1C0D70](../../src/hand/func_001C0D70.c) | `PpathzoneFromPos` | Walk the world zone BSP (SW+0x45CC) to the pathzone containing pos; default world pathzone SW+0x45C8 | 0.55 | HYP |
| [0x1E5DF8](../../src/hand/func_001E5DF8.c) | `LoadShapeFromBrx` | SHAPE::LoadFromBrx (vt14): XFM xf, mode(+0xE0: 0 linear, 1 curve), fClosed(+0xE4), count, then times/dists/points (+tangent arrays +0x140/+0x144 for c | 0.75 | HYP |
| [0x1F2108](../../src/hand/func_001F2108.c) | `InitSo` | SO vt7 Init: InitLo, SetLoBusyk(0), clear DL +0x64, dhp bytes +0x148/+0x149 = -10/-20, gAlpha(+0x12C)=1, +0x13C=D_002C3B90, inherit +0x148 bit35 from  | 0.85 | VERIFIED |
| [0x1F2758](../../src/hand/func_001F2758.c) | `GetSoIgnoreLocked` | Return bit 34 of +0xD0 (fIgnoreLocked) | 0.75 | HYP |
| [0x1F2768](../../src/hand/SetSoIgnoreLocked__FP2SOi.cpp) | `SetSoIgnoreLocked` | Set +0xD0 bit34 (fIgnoreLocked) | 0.85 | PLAUSIBLE |
| [0x1F2798](../../src/hand/func_001F2798.c) | `PsoFindPhysRoot` | Walk up the parent chain to the first SO that has non-fixed physics (SOPHYS modes != 3) or flag +0x148 bit34; returns last visited | 0.45 | VERIFIED |
| [0x1F2A60](../../src/hand/func_001F2A60.cpp) | `ShowSoPart` | If part hidden (fl bit50): clear it and, when SO in world, AddSoPartToTree | 0.50 | VERIFIED |
| [0x1F2B20](../../src/hand/func_001F2B20.c) | `AddSoParts` | AddSoPartToTree for every visible SOPART (+0xE0 count / +0xE4 array, 0xB0 stride) | 0.55 | VERIFIED |
| [0x1F31E0](../../src/hand/func_001F31E0.cpp) | `LoadSoFromBrx` | SO vt14: LoadLo base, read model/globset, SOPART array (XF id, flags, radius, local geom, BSP...), physics block (matrix, mass), options, bounds | 0.85 | VERIFIED |
| [0x1F35F8](../../src/hand/func_001F35F8.c) | `AdjustSoMomint` | Scale SOPHYS inertia matrix by r and recompute its inverse (zero if singular) | 0.55 | VERIFIED |
| [0x1F3C70](../../src/auto/func_001F3C70.c) | `RenderSoSelf` | SO vt19: if visible in current vis mask (+0x2D & D_002DDF54) draw globset (+0x80) with alpha +0x12C | 0.65 | VERIFIED |
| [0x1F3E00](../../src/hand/func_001F3E00.c) | `OnSoRemove` | SO vt22: LO::Freeze, unregister (0x1D6310), vt22 children, if tree owner: vt49 + remove from D_003E11AC | 0.55 | VERIFIED (arbitration/conflict fix; was FreezeSo) |
| [0x1F3EA8](../../src/hand/func_001F3EA8.c) | `OnSoAdd` | SO vt23: LO::Unfreeze, re-register, vt23 children, add tree to D_003E11AC and vt49 | 0.55 | VERIFIED (arbitration/conflict fix; was UnfreezeSo) |
| [0x1F4558](../../src/hand/func_001F4558.cpp) | `GetAngularVelocity__2SO` | SO vt34: world angular velocity of rotation XF (kind 6), zero otherwise | 0.90 | VERIFIED |
| [0x1F4CA0](../../src/hand/func_001F4CA0.c) | `CloneXfRef` | SO vt52: re-point an XF* from base SO's XF block to the clone's (also via parent) | 0.55 | VERIFIED |
| [0x1F4FC8](../../src/hand/func_001F4FC8.c) | `FIgnoreSoIntersection` | SO vt61: true if (pa flag +0x148 bit26 or pa fixed) and pb fixed | 0.35 | VERIFIED |
| [0x1F57C0](../../src/auto/func_001F57C0.c) | `SetSoCnstrTorque` | Same for rotation XF into SOPHYS+0x90 | 0.60 | VERIFIED |
| [0x1F5860](../../src/hand/func_001F5860.c) | `GetSoDvGravity` | SOPHYS+0x80 (gravity vector) or zero | 0.50 | VERIFIED |
| [0x1F58F0](../../src/hand/func_001F58F0.c) | `GetSoCpdefi` | SO vt50: camera default info: base pos/velocity, radHome = atan2(-m01,-m00) of upright matrix, sAdjust 100, grfcpd 0, tMoveLast = g_clock.t | 0.85 | HYP |
| [0x1F6068](../../src/hand/func_001F6068.cpp) | `Clone__2SOP2LO` | SO vt41 Clone: copy XF block, model, parts, physics for a clone | 0.90 | VERIFIED |
| [0x1F66F0](../../src/hand/func_001F66F0.c) | `CloneSoHierarchy` | SO vt40: vt41 Clone then PloCloneLo each non-removed child | 0.50 | VERIFIED |
| [0x1F6AC8](../../src/hand/func_001F6AC8.c) | `ShowSoPartsById` | ShowSoPart for parts whose id (+0xC) == k | 0.45 | VERIFIED |
| [0x1F7598](../../src/hand/func_001F7598.c) | `InflictZap` | Deliver a zap record to responders: dedupe via recent list D_003E15C0, call target vt75, log with time, send messages 0xB to both | 0.45 | HYP |
| [0x1F77C0](../../src/hand/func_001F77C0.c) | `ExpireSwZapRecords` | Drop zap-history entries older than 0.25 s from g_sw list | 0.45 | VERIFIED |
| [0x1F78D8](../../src/hand/func_001F78D8.c) | `GrfzpdFromSoGrfzpk` | OR of per-kind response bytes SO+0x110[26] selected by grfzpk | 0.40 | VERIFIED |
| [0x1F7D90](../../src/hand/func_001F7D90.c) | `SetSoFadeRate` | If rate != 0: Add (vt8) and set grf bit 2; store +0x130 | 0.50 | VERIFIED |
| [0x1F8198](../../src/hand/func_001F8198.c) | `PsaaFindSo` | Find SAA in +0xA4 list with +0x18 == n | 0.35 | VERIFIED |
| [0x1F84D0](../../src/hand/SetSoIceable__FP2SOi.cpp) | `SetSoIceable` | Set +0x148 bit33 (fIceable) | 0.85 | HYP |
| [0x1F8500](../../src/hand/func_001F8500.c) | `GetSoIceable` | Return +0x148 bit33 | 0.75 | HYP |
| [0x1F8740](../../src/auto/func_001F8740.c) | `HANDLE::Decref` | --cref; at 0 destroy and free to pool D_003E15CC | 0.40 | VERIFIED |
| [0x1F8DD8](../../src/hand/func_001F8DD8.c) | `SolveSw` | Per-frame physics solve over the world: pre-pass 0x134040, vt17 on entries with (grf&0x24)==4, then solve contact groups (0x1F8F80) | 0.70 | HYP |
| [0x1F9410](../../src/hand/func_001F9410.cpp) | `SolveSwGroup` | Solve one contact group: build DZG rows from contacts, set targets, SolveDzg, apply impulses, notify objects (vt 65/66/67) | 0.75 | HYP |
| [0x1FA810](../../src/hand/func_001FA810.c) | `ApplySoConstraintWorld` | Return a vector constrained by a CONSTR: kind 3 locked -> zero, 2 -> projected on (world-transformed) normal, 1 -> plane-projected | 0.55 | VERIFIED |
| [0x227BB8](../../src/hand/func_00227BB8.c) | `LoadVolFromBrx` | LoadXfmFromBrx, ReadGeom, ReadBspc, then bounding sphere of the geometry points | 0.75 | VERIFIED |
| [0x227E20](../../src/auto/func_00227E20.c) | `OnCid23Add` | OnLoAdd + AppendDlEntry(&D_003E1480) | 0.55 | VERIFIED |
| [0x227E58](../../src/auto/func_00227E58.c) | `OnCid23Remove` | OnLoRemove + RemoveDlEntry(&D_003E1480) | 0.55 | VERIFIED |
| [0x2297F0](../../src/auto/func_002297F0.c) | `SO::AddExternalAccelerations (default, empty)` | empty default | 0.80 | HYP |

Selected 59 of 395 named functions (confidence >= 0.75, verified, or flagged). Full list: [reference/functions/physics-collision.md](../reference/functions/physics-collision.md).

## Open questions
- SO slots 65-72 and 49-60 naming; `fIgnoreLocked` label on 0x1F2768 is suspect (verifier r4r6).
- Exact SW offsets of the sweep-and-prune endpoint arrays (+0x3CD4 region) and the object cap.
- Experiment: break 0x1F9410 while pushing a crate (once per frame per group); break 0x1C0D70 as a guard crosses rooms.
