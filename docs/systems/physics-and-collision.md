# Physics and collision

How solid objects move and collide: the SO class and its physics record, the broad phase, contact generation against
spheres and BSP meshes, the per-frame solve with the DZG constraint solver, how walking characters sit on top of it,
water and external accelerations, trigger volumes, shapes (paths), navigation zones and path finding, and damage
delivery ("zaps"). For the overview see
[engine.md section 7.6](../engine.md#76-physics-collision-and-navigation-enginephysics); for the collision data in the
world file see [formats/level-objects.md](../formats/level-objects.md) and
[formats/meshes-scene.md](../formats/meshes-scene.md). The characters built on this are in [player.md](player.md) and
[ai-guards.md](ai-guards.md).

## 1. Purpose and shape

The engine has one rigid-body world. Every object with collision geometry is an **SO** (solid object). Some SOs are
*physical* (they own a physics record and can be pushed, fall and bounce), the rest are fixed or animated geometry that
others collide with. Once per frame the world rebuilds the contacts between touching objects, groups objects that touch,
solves each group's contact impulses as a bounded linear system (the DZG solver, inherited from Sly 1), and integrates
positions and rotations. Characters do not bypass this: heroes and guards are SOs whose per-object hooks steer the
solve (desired velocity, floor friction, constraints).

Code lives in `src/game/engine/physics/`:

- `so.cpp`: the SO class (transforms, parts, trees, physics records, the contact slots, zaps, fades).
- `bbmark.cpp` (broad phase), `mark.cpp` (narrow phase: contact generation), `solve.cpp` (the per-frame solve and the
  impulse/acceleration helpers), `dzg.cpp` (contact rows on top of the solver in `core/math/dzgcore.cpp`).
- `geom.cpp` (collision geometry in local and world space), `clip.cpp` (segment clipping and line tests),
  `vol.cpp` (trigger volumes), `water.cpp` (water bodies), `shape.cpp` (paths and curves), `path.cpp` (navigation
  zones and A*), the convex-obstacle unit (`IsPosInCnvo`, `MergeCid14Obstacles`), the sticky-mesh unit
  (`UpdateJtMeshAttachment`) and the launcher unit (`LaunchSoFromLauncher`).
- Shared maths in `src/game/core/math/`: `bsp.cpp` (plane BSP trees: clip an edge, locate a point), `dzgcore.cpp`
  (the active-set constraint solver), `crout.cpp` (the factorisation it uses).
- `src/game/actor/step.cpp`: how a walking body uses the solver (section 4).

Types are in `include/sly2/so.h` (SO, SOPHYS, CONSTR, SOPART, SOTREE, XF, XA, GLOBSET) and `include/sly2/dzg.h`. The
contact record XP and the BSP geometry types are still private to `mark.cpp` and its users.

## 2. Main types

- **SO**: an LO with a hierarchy of transforms (XF), a model (GLOBSET), child objects, collision parts (SOPART), an
  optional physics record (SOPHYS), membership in a physics tree (SOTREE), per-frame contact velocity deltas, a per-kind
  zap response table, alpha/fade state, a bounds record, and an external-acceleration (XA) list.
- **XF**: one transform of the object's hierarchy, with local and world matrices, actuators that animate it, and a
  *role*. The object's position and rotation XFs carry the roles that matter to physics: velocity-driven roles are
  integrated by the solver, rest roles are skipped, animated roles follow their actuators (role meanings are
  (inferred); see `IsXfRoleVelocity`, `IsXfRoleAtRest`).
- **SOPHYS**: mass, local inertia and its inverse, linear and angular velocity, gravity, and two **CONSTR** records
  (one for translation, one for rotation). A constraint kind is free, plane (remove motion along an axis), axis (keep
  only motion along it) or locked (`ApplyConstr`). An object with both locked is treated as fixed by grouping.
- **SOPART**: one collision part: the XF it moves with, a sphere radius or a BSP mesh (GEOM local and world copies plus
  a plane BSP), a hidden flag, and the zap kinds and damage it delivers or accepts.
- **SOTREE**: a physics tree: a root SO and everything attached to it, solved as one body for collision; holds bounds,
  the part list and the contact lists, and links into the world's awake and all-tree lists.
- **AAOBR** / pair link (POX): the broad-phase box of a tree and a record that two boxes overlap.
- **XP**: a contact point between parts of two trees: position, normal, distance, friction, elasticity, kind, the
  two sides' objects and parts. Floor contacts of walking characters are a dedicated kind.
- **DZ / DZX / DZG**: one constraint row, its contact record, and the solver state.

## 3. Lifecycle

**Load.** `LoadSoFromBrx` reads transforms, the model, collision parts (geometry and BSP) and the mass block; physical
objects get a SOPHYS from `InitSoPhysics`. `PostSoLoad` finishes references. Collision geometry is copied into the world
heap and shared where possible (`CloneGeom`).

**Activation.** `OnSoAdd` / `OnSoRemove` (slots 23/22) register and unregister the object with rendering and the broad
phase (`AddSwAaobrObject` / `RemoveSwAaobrObject`), so only live objects collide. `AddSoHierarchy` /
`RemoveSoHierarchy` build and tear down trees when objects are attached to each other.

**Per frame.** In `UpdateSw`, after every live object's slot 16 has run (where gameplay sets intent: velocities,
accelerations, target headings), the world calls `SolveSw`:

1. `RecalcSwXpAll`: flush pending bounds updates, re-sort the broad-phase axes, create/destroy pair links, and regenerate
   contacts for the pairs that changed (`AddSoXps` in `mark.cpp`), then refresh contact positions and relative
   velocities.
2. Slot 17 (`UpdateSoXfWorld`) on every awake object that needs it: part matrices, soft-mesh parts, velocities copied
   into the physics record, then **slot 70, the physics preset** (`PresetSoAccel` and its overrides) for objects whose
   position or rotation is velocity-driven, then part geometry.
3. `SolveSwContactGroups` with the resting-contact pass: for each dynamic tree not yet grouped, flood-fill the cluster
   of physical objects it touches (`BuildContactGroup`), collect the cluster's contacts and solve it (`SplitSwGroup`
   -> `SolveSwGroup`).
4. A second `RecalcSwXpAll` and a second group solve with the impact and position passes.

`SolveSwGroup` builds a DZG over the group's contacts (`InitDzgXp`), adds one row per contact (`AppendDzgDz`; each row's
coefficients come from the two objects' responses to a unit impulse, slots 67/68), sets each row's target from
restitution or penetration, solves with friction (`SolveDzgFriction`), applies the impulses (`ApplyDzg`), and notifies the
objects through slots 65 (resting), 66 (impact) and 67 (position push-out). Integration renormalises rotations and
updates part geometry; world geometry is refreshed lazily by frame stamp (`UpdateGeomWorld`).

When the game clock is stopped (pause), `UpdateSw` skips the solve and updates only the objects marked to update while
paused (inferred).

**Sleep.** Trees that stop moving drop off the awake list; anything that disturbs them calls `WakeSo` (removing a pair
link wakes both sides). Code that teleports or reconfigures an object should go through `TranslateSoToPos` /
`RotateSoToMat` (slots 29/30), which also refresh bounds, instead of writing matrices.

## 4. Characters on the solver

STEP (`step.cpp`) is the bridge between gameplay and physics:

- **Phys part.** At post-load a STEP builds an extra collision part from one of its mesh parts (`BuildStepPhysPart`),
  records its lowest point as the base height, and adds helper points. `SetStepPhys` switches the active shape (stand,
  crouch, ...); `UpdateStepPhys` blends the body part toward it so shape changes do not pop.
- **Ground probe.** Instead of resting on raw mesh contacts, a STEP adds a custom floor contact each step
  (`AddStepCustomXps`, slot 60) from a ray cast whose behaviour comes from slot 85's probe flags (enabled, cast from the
  top helper point, keep the hit normal). Guards override the flags per behaviour (`GetStepguardGroundProbeFlags`, which
  asks the behaviours' slot 12); a "no base" option turns the probe off. The resulting contact is cached as the step's floor (`pxpFloor`, null when airborne).
- **Contact hooks.** Slot 55 (`AdjustStepNewXp`) gives new floor contacts the step's friction and a force limit from its
  weight; slots 56/57 (`AdjustStepXpVelocity`, `AdjustStepXps`) smooth the closing speed on the floor and limit its
  change per step so the body settles without bouncing. Heroes add their own adjustments (`AdjustJtXp`,
  `HandleStepplayerContactImpulse`).
- **Steering.** Slot 70 (`PresetStepAccel`) runs the SO preset (pending velocity change, external accelerations) and
  then turns the body toward the target heading with spin smoothing, or applies a fixed angular velocity. Hero and
  guard presets call it first and then add their component- or behaviour-specific accelerations.
- **Constraints.** Components that pin a character to geometry add constraint contacts or project movement: pipe
  climbing (`UpdateJtInternalXpsPipe`), wall press, sticky meshes (`SolveMeshConstraints`, `UpdateJtMeshAttachment`),
  guards attached to rails and pipes (`AddStepguardAttachContacts`).
- **Ignoring collisions.** `FIgnoreIntersection` (slot 61) answers per pair; the broad phase combines both objects'
  answers and only pairs them when neither asks to ignore the other. Heroes use it to pass through a thrown object
  for a moment (Murray after a throw); guards ignore some objects while dead and their holder's group while juggled. When the answer changes, call
  `RecalcSwOxfFilterForObject` so existing pairs are re-evaluated.

## 5. Water, external accelerations and launchers

- **XA list.** An SO keeps a list of external-acceleration sources. Before solving, the preset asks each source for a
  force (`ApplySoExternalAccelerations` calls each owner's slot 71; WATER forwards to the object's slot 72,
  `AddSoWaterAcceleration`). `AddSoXa` / `RemoveSoXa` manage the list.
- **WATER** is an SO that tracks the objects inside it and mirrors each into that object's XA list, so the solver asks
  the water every step for buoyancy, current drag, righting torque and wake (`AddSoWaterAcceleration`,
  `GetWaterSubmergedFraction`, `CalculateWaterCurrent`). It emits the splash event when something enters fast. Heroes
  treat deep or hazardous water as damage through the hurt component (`UpdateStepplayerWater`, player.md section 5).
- **Launchers** throw an SO toward a point: ballistic arcs at a chosen elevation or the velocity that reaches a point
  in a given time (`CalculateBallisticLaunchVelocity`, `CalculateVelocityToReachPos`, `LaunchSoFromLauncher`). Guards'
  ranged attacks use them.
- **Impulse helpers** for gameplay: `ApplySoImpulse`, `AddSoAcceleration`, `AddSoAngularAcceleration`,
  `GetSoVelocityAtPos`, the `AccelSoToward*` family, `SetSoVelocity` / `GetSoVelocity` (slots 35/33).

## 6. Damage: zaps

Damage is delivered as a **zap** record (ZPR): source object, source part, target, a bit set of **zap kinds** (melee,
projectile, laser, water, crossbow dart, juggle, Murray takedown, ...), and the damage amount. Kind bits observed
`[C:runtime]`: 0x2 Murray takedown, 0x100 guard projectile, 0x200 guard melee, 0x1000 laser, 0x2000 water, 0x80000
crossbow dart, 0x200000 juggle (floors hp at 5).

- **Delivery.** `DeliverZapToSo` walks from the target up its parent chain. For each object it skips the zap if an
  identical one (same source, part, kinds and responder) is still in the world's recent-zap history, computes the
  object's response from its per-kind response table (`GetSoZapResponseFlags`), and asks the object's slot 75
  (`FTakeDamage`: `CanSoTakeDamage` for plain SOs, `TakeStepguardDamage`, `TakeStepplayerDamage`). The first object that
  accepts it is logged in the history; if the response is damaging, both sides get a message and Splice events so
  scripts can react. `ExpireSwZapRecords` drops history entries after a short window, which is what keeps one swing or
  one beam from hitting every frame.
- **Sources.** Parts carry zap kinds and damage (`SetSoPartsZapKindsById`); contact-based damage is gathered from the
  tree's zap contacts (`InflictSoContactZaps`, slot 74) and from deferred touch callbacks (`HandleSoTouchZaps`), with
  the object's zap mode (`SetSoZapMode`) deciding how its parts damage what they touch. Bombs, laser rigs, guard
  attacks and Murray's moves build zap records directly.
- **Responses.** `SetSoZapResponse` sets the per-kind response bytes; `FindZapResponder` finds the first object in a
  chain that responds. Characters add their own filters: invulnerability (slot 76), damage veto (slot 86), and the guards'
  hurt/knockdown/KO reaction tables (ai-guards.md section 6).

## 7. Geometry, volumes and shapes

- **GEOM** (`geom.cpp`) is the collision mesh format shared by parts, volumes and shapes: points, surface planes, edges.
  `UpdateGeomWorld` transforms it to world space (points via a VU0 microprogram, normals by the inverse transpose) when
  its owner moved.
- **BSP** (`core/math/bsp.cpp`): each mesh part and volume has a plane BSP (null children are inside/outside leaves).
  Contact generation clips the other part's edges against it (`ClsgClipEdgeToBsp`), point queries locate leaves
  (`FindBspLeafContainingPoint`).
- **Narrow phase** (`mark.cpp`): `AddSoXps` pairs the parts of two trees and dispatches by shape: sphere/sphere,
  sphere/BSP, BSP/BSP and swept vertices; contacts are appended with `AddXp`.
- **Line tests** (`clip.cpp`): clip a segment against objects (through their BSP or bounding sphere) into span lists,
  combine span lists, and return the first object hit (`PsoHitTestLineObjects`). Vision, ground probes and attack
  checks are built on these with per-use object filters.
- **VOL** (`vol.cpp`): a trigger volume (an XFM with a convex-cell BSP): `IsPointInVol`, `IsSphereInVol`. Used by guard
  detectors, buttons, exits, hazard checks and Splice. Active volumes are kept on a world list.
- **SHAPE** (`shape.cpp`): polylines or Hermite curves with a parameter `u` and arc length `s` per point (Sly 1's S/U API:
  `SFromShapeU`, `UFromShapeS`, `SMaxShape`, `EvaluateShapeS` / `EvaluateShapeU`, `FindShapeClosestPoint*`). Shapes are
  patrol paths, camera and platform paths, PIPE and RAIL, rope chains (`SetShapeChainMode`), and the outlines of convex
  obstacles.

## 8. Navigation: path zones and path finding

- **PATHZONE** (`path.cpp`) holds a navigation graph: walkable triangles with a 2D BSP for point location and vertices
  with edge costs. `FindPathPoints` locates the start and goal triangles, runs A*, simplifies the result by line of
  sight and returns the points; `FindPathzonePathCurve` builds a curve from it.
- **The zone BSP.** A world can carry a BSP whose leaves are path zones; `FindPathzoneAtPos` walks it to find the zone
  containing a point, falling back to a default zone that `PostLoadSwPathzones` creates when the world has none.
- **Obstacles.** Convex polygons (the convex-obstacle unit: `IsPosInCnvo`, `ClipEdgeToCnvo`) register in the zone that
  contains them (`AddPathzoneObstacle` / `RemovePathzoneObstacle`), kept in sorted begin/end event arrays per axis for
  sweep-and-prune queries; overlapping ones merge into one hull (`MergeCid14Obstacles`, `UpdatePendingCid14Obstacles`
  after each solve). Every change bumps a world navigation version so planners know to re-plan. `IsPosFreeInPathzone`
  answers "walkable and outside every obstacle".
- **Path links** (PATHLINK objects) connect zones with walk or leap segments. `FindPathlinkRoute` runs A* over zones
  connected by links; guards' BHVGOTO walks a route zone by zone and hands each link segment to BHVPATHLINK
  (ai-guards.md section 8).

## 9. Invariants and gotchas

- **Intent in Update, motion in the solve.** Velocities and accelerations set in slot 16 are consumed by slot 70 inside
  `SolveSw`. Writing an object's world matrix directly bypasses bounds, contacts and sleep; use the translate/rotate
  slots or the velocity setters.
- **Only live objects collide.** Objects out of the world have no broad-phase box; a guard or prop that streamed out
  will not block or be hit.
- **Fixed means both constraints locked.** Grouping skips physical objects whose translation and rotation are both
  locked; a half-locked object still takes part.
- **Wake what you disturb.** Code that changes an object's state without contacts (teleport, re-parent, enabling
  collision) should wake it, or a sleeping tree will ignore the change.
- **Collision filters are cached in pairs.** After changing what `FIgnoreIntersection` answers, call
  `RecalcSwOxfFilterForObject`.
- **Zap history dedupes.** A repeated identical zap inside the history window is dropped silently; give distinct hits
  distinct sources or parts, or wait out the window.
- **Navigation changes are versioned.** Adding or removing obstacles bumps the navigation version; long-lived plans must
  check it (BHVGOTO does).
- **Hand-tuned code.** Many functions in this directory are VU0 asm or register-pinned for the matching build; their
  doc comments describe the maths. A port should reimplement them from the description rather than translate the asm.
- Role values of XFs, some contact kinds and several SO flag bits are (inferred); check the doc comment next to the
  function before relying on one.

## 10. How to

**Make a prop physical.** Give it a mass block in the level data (or create a SOPHYS with `InitSoPhysics`), set its
constraints (free, or locked rotation for an upright crate), make sure its position XF is in a velocity role, and wake
it. The solver will group it with whatever it touches.

**Push an object from gameplay.** Call `ApplySoImpulse` for an instant hit at a point, or `AddSoAcceleration` /
`AddSoAngularAcceleration` from the object's (or a controller's) slot-70 preset for continuous force; for currents and
fields add an XA source the way WATER does.

**Add a damaging object.** Give its parts zap kinds and damage, or build a zap record and call `DeliverZapToSo` when it
hits. Make targets respond by setting their response bytes (`SetSoZapResponse`) and, for new behaviour, overriding
slot 75.

**Add a walkable surface with special friction.** Override slot 55 on the surface's class or on the walker to adjust
new contacts of the floor kind, as STEP does.

**Debug "falls through the floor".** Check that the floor object is live (in the world and its root active), has
collision parts with a BSP, is not ignored by the walker's `FIgnoreIntersection`, and that the walker's probe flags
(slot 85) are not zero in its current state.

## 11. Where to look

- `src/game/engine/physics/so.cpp`, `solve.cpp`, `dzg.cpp`, `bbmark.cpp`, `mark.cpp`: the rigid-body core.
- `src/game/engine/physics/geom.cpp`, `clip.cpp`, `vol.cpp`, `shape.cpp`: geometry, line tests, volumes, paths.
- `src/game/engine/physics/water.cpp`, the launcher unit (`LaunchSoFromLauncher`), the sticky-mesh unit
  (`UpdateJtMeshAttachment`): special bodies.
- `src/game/engine/physics/path.cpp` and the convex-obstacle unit (`MergeCid14Obstacles`): navigation.
- `src/game/core/math/bsp.cpp`, `dzgcore.cpp`, `crout.cpp`: shared maths.
- `src/game/actor/step.cpp`: walking bodies on the solver.
- `include/sly2/so.h`, `include/sly2/dzg.h`.
