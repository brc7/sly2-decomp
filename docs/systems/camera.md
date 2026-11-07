# Camera

The camera system decides where the view is each frame and turns that into the matrices, frustum and fog the renderer
uses. It is a **priority stack of camera policies**: gameplay, cutscenes and level objects push and revoke policies,
and the highest-priority one places the camera. For the one-paragraph summary see
[engine.md section 7.4](../engine.md#74-camera-enginecamera); for how the matrices are consumed see
[rendering.md](rendering.md).

## 1. Main types

| Type | What it is |
|---|---|
| `CM` | the camera; exactly one, the global `g_cm`. A `BASIC` child (not an LO, so it has no oid and is never "live"). Holds fov/near/far/fog, the camera matrix, projection and view-projection matrices, frustum planes, the visibility zone and mask, the focus-adjust springs, the policy stack and the embedded policy objects. |
| `CPLCY` | a camera policy: a small C++ object with virtual slots Init, Activate, Deactivate, Set, Revoke, Update(`CPDEFI*`, `JOY*`) and a reset slot. Its base data (`CPLCYDATA`) remembers the hero it follows and the fov / near clip saved on activation, which Deactivate restores. |
| `CPR` | one stack entry: `{priority, policy, focus SO, argument}`. |
| `CPDEFI` | a *camera definition* supplied by the focus object (SO virtual slot 50, `GetCpdefi`): base position and direction, home heading, adjustment distance, turn axis, last-move time, flags. Players supply theirs through `GetStepplayerCpdefi` (with per-character overrides). |
| `CMPART1` | the bottom policy, pushed at priority 0 by `InitCm`; its Update does nothing (placeholder name). |
| `CMPART2` (CPTN) | the **follow / tunnel camera**: the normal gameplay camera, steered by `TN` volumes when the hero is inside one. |
| `CPLOOK` | look-around and zoom (binocucom). |
| `CPZOOM` | recon camera: blends to a pose and zooms toward a focus object. |
| `CPHANG` | a swaying camera used by the grab code (inferred: hiding / hanging states). |
| `CPCAMBLEND` | cutscene camera move between two poses along a curve (a `CAMMOVE` owned by a RELAY script). |
| `CPCAMERA` | sits on a designer-placed `CAMERA` object (class cid 0x2C) with an optional look-at target. |
| `CPTURRET` | follows a turret's world matrix. |
| `CPTWOSHOT` | frames two objects together (used by a boss fight). |
| `TN` | camera tunnel volume (an XFM with a BSP and a path); per focus class a `CPD` record of camera parameters. |
| `VIS` | visibility zone volume; selects the camera's `grfvis` mask ([rendering.md](rendering.md#7-culling-and-visibility)). |

All policy objects are **embedded in `CM`** (constructed by `CM_ctor`); nothing allocates policies at run time. Pushing
a policy means inserting a `CPR` that points at one of these embedded objects.

Header: `include/sly2/cm.h` (CM, CPDEFI, CPR and every policy class). Code: `src/game/engine/camera/`.

## 2. Lifecycle

- **Construction**: `CM_ctor` runs as a C++ global constructor at boot. `SetupCm` is still called from the main loop but
  is empty in Sly 2.
- **Level load**: `ExecuteSwTransition` calls `InitCm` after constructing the new world: fog defaults, aspect, near/far,
  identity matrix, `RecalcCmFrustrum`, a cut, each embedded policy's Init, and the default policy pushed at priority 0.
  The world file then sets camera options (`LoadOptionsFromBrx` on `g_cm`: fov, clips, fog, MRD ratio, pose) through the
  option accessors (`SetCmBaseFovTarget`, `SetCmBaseNearClip`, `SetCmPos`, `SetCmRotDeg`, ...). After `PostSwLoad`,
  the transition runs one `UpdateCmLast` and `CutCm` so the first frame does not blend from the old level.
- **Per frame**: `UpdateSw` ends with the save, sound and camera updates; the camera runs `UpdateCmLast` only when the
  camera clock delta is positive. The camera uses its own clock rate slot, so it **keeps running during pause menus and
  slow motion** (Thief Reflexes).
- **Render**: `DrawSw` reads `g_cm`'s matrices and fog terms; RenderSelf code reads `g_cm.grfvis` and the frustum.

### `UpdateCmLast`, step by step

1. Take the top `CPR`. If it has a focus object, ask the focus for its `CPDEFI` (SO slot 50) into the camera's cached
   definition; when the focus changed, `ResetCmAdjust`; then `AdjustCmCpdefi` springs the adjustment offsets (focus and
   adjust springs, `ApplyCameraSpring`, critically damped, snapped on a cut).
2. Call the top policy's **Update(cpdefi, joy)**. The policy computes an eye and a target (or a full pose) and writes it
   with `SetCameraLookAt` / `SetCmMat4` / `SetCmFov`.
3. If that Update changed which policy is on top (a policy may push or revoke), repeat with the new top.
4. Smooth fov toward `targetFieldOfView` (snapped on a cut), recompute the frustum when it moved, clear the cut counter.

`SetCmMat4` -> `UpdateCmMat4` rebuilds the view-projection (plus the camera jolt shake for half a second after
`JoltCamera`), its inverse, the normal and wide frustum planes, the VU0 cull rows, and then `UpdateCmVisZone` picks the
best `VIS` zone containing the eye (`SetCmVisZone` copies that zone's mask into `grfvis`).

## 3. The policy stack

- `SetCmPolicy(cm, priority, policy, focusSo, arg)` inserts a `CPR` sorted by priority (stack depth is fixed and small)
  and calls the policy's Set slot. If the new entry is on top, the old top gets Deactivate and the new one Activate
  (with the mode start time recorded).
- `RevokeCmPolicy(cm, mask, priority, policy, focusSo, arg)` removes every entry matching the mask (bits select which
  of policy, focus and argument must match; a priority of -1 matches any) and calls Revoke on each; if the top was
  removed, Deactivate it and Activate the new top.
- `IsCameraPolicyActive` tells a policy whether it is currently on top.

Who pushes what (priority in parentheses; values are the code's constants):

| Policy | Pushed by |
|---|---|
| CMPART1 (0) | `InitCm` |
| CPTN (1) | `OnPoActive` (PO slot 78) when a PO becomes the active player, unless the PO has its own tunnel |
| CPTN (tunnel priority + 2) | `SetTnTns` when the hero enters a `TN` volume; revoked on exit |
| CPTWOSHOT (6) | the Rajan boss code |
| CPLOOK (7) | the player's look mode (binocucom), `gameplay/player/mode.cpp` |
| CPZOOM (8) | the player's recon mode (script-started), `gameplay/player/mode.cpp` |
| CPTURRET (9) | `gameplay/objects/turret.cpp` while the turret is operated |
| CPCAMERA (10) | a `CAMERA` object entering the world hierarchy (`AddCameraHierarchy`, or at load via `PostCameraLoad`) |
| CPHANG (10) | the grab code, `gameplay/player/grab.cpp` |
| CPCAMBLEND (10) | RELAY camera-blend commands, `gameplay/cutscene/relay.cpp` |

`level/exit_warp.cpp` revokes the look and recon policies on a warp. Activate/Deactivate of the base class save and
restore fov target and near clip, so a policy that changes them (look mode sets a short near clip) gives them back.

## 4. The follow camera (CPTN)

`CMPART2::Update` is the most complex function in the system; its doc comment in `tn.cpp` describes it in ten steps.
In short:

- The look-at point is the focus plus a sprung focus offset (`dposFocus`); grab states can pull it toward a grab focus.
- A `TN` tunnel (when inside one) provides forward/backward views from its path (`ProjectPointOntoTnPath`,
  `EvaluateTnPathPos`) and a `CPD` record per focus class (`SelectTnCpdForCid`): distances, tilt limits, pull strengths,
  framing offsets, whether the player may turn the camera, and whether eye clipping is disabled.
- With no tunnel the camera is "free": the right stick turns it (with dead zones near the axes), the hero's motion
  swings it around the turn axis, and tilt eases back into range when the hero moves fast.
- Cuts (`CutCm`, or `SetCmCut` with explicit pan/tilt) skip smoothing for one frame.
- `UpdateCameraLineOfSight` periodically tests points around the hero; if the hero stays hidden long enough the camera
  cuts.
- The eye is clipped against geometry (section 5) and then handed to `SetCameraLookAt`.

## 5. Collision and clipping

The camera does not use the physics solver. It has its own queries against each SO's **camera BSP**:

- `ClipCameraEye`: estimates the eye velocity, gathers camera-colliding SOs in a box around the previous eye, and
  `SquishCmEye` pushes the eye out with a smoothed constraint (`SvSquishCmEyeConstraint`) that keeps the near-plane
  corners clear (`nearCornerDistance`).
- `FindCameraClearInterval`: clips a segment against the SOs and returns the first clear gap; `FindCmClosestClearPos`
  samples tilts and pans around a point for the closest clear eye (used on cuts).

## 6. Look and zoom

- **CPLOOK** (binocucom): anchor above the focus, pan/tilt from the sticks (honouring an invert option), zoom from a
  button axis with zoom sounds whose pitch/volume follow the zoom speed, fov lerp between the policy's fov range, and
  a short near clip. Deactivate cuts the camera so it continues behind the hero. The HUD reads the zoom range for its
  readout.
- **CPZOOM** (recon): an owner and a focus object (`SetCpzoomOwner`, `SetCpzoomFocus`), an optional explicit pose
  (`SetCpzoomPose`) and a zoom target (`SetCpzoomUZoomTarget`); `HasZoomReachedTarget` and friends let scripts wait for
  arrival. Look and zoom share an entry counter.

## 7. Other outputs

- **Projection**: `RecalcCmFrustrum` derives the half-fov tangents, cull scale, near-corner distance and projection
  matrix; `CombineEyeLookAtProj` applies the game-to-camera axis swap. `BuildStereoEyeViewProj` offsets the eye
  sideways (a stereo/debug feature; inferred).
- **Fog**: `SetCmNearFog`, `SetCmFarFog`, `SetCmUFogMax`, `SetCmRgbaFog`; `BuildCmFgfn` turns them into the GS fog
  function the glob microprogram evaluates per vertex.
- **Culling helpers**: `IsInCameraFrustum` (point or sphere in the frustum) and `IsWithinCameraRenderDistance` (within the "maximum
  render distance" with a fade fraction).
- **Screen mapping**: `ConvertCmWorldToScreen` / `ConvertCmScreenToWorld` (used for labels and HUD markers).
- **Visibility mask**: `grfvis` from the current `VIS` zone, compared against each object's `grfvis` by RenderSelf and by
  the busy-list activation rule ([engine.md section 6](../engine.md#6-the-object-model)).

## 8. Invariants and gotchas

- **Only the top policy runs.** Lower entries are dormant until everything above them is revoked; a policy that
  forgets to revoke itself leaves the camera stuck.
- **Revoke with the right mask.** Revoking by focus or argument alone can remove other users' entries; the existing
  callers mostly match on policy plus focus or argument.
- **Policies are singletons.** Two users that push the same embedded policy share its state; the newest Set wins.
- **Camera time is not game time.** Policies use the camera clock delta, so they keep moving under the pause menu.
  Use `g_clock`'s camera rate, not the game rate, when timing camera code.
- **Writing `mat` directly does nothing**: always go through `SetCmMat4` / `SetCameraLookAt` so the derived matrices,
  frustum and vis zone are rebuilt.
- **Cuts are counted**, not boolean: `CutCm` and `SetCmCut` take the maximum, `UpdateCmLast` clears it.
- `CMPART1`, the hang policy's role and the meaning of several CM fields are still placeholders or inferred.

## 9. How to

**Add a camera behaviour.** Add a `CPLCY` subclass with Activate/Deactivate/Update, embed an instance in `CM` (after the
existing ones; check `CM`'s layout asserts) and construct it in `CM_ctor`. Push it with `SetCmPolicy` at a priority that
sits correctly against the table above, and revoke it with the same policy pointer.

**Give an object its own camera framing.** Override SO slot 50 (`GetCpdefi`) to adjust the definition (see the
Bentley and dancer overrides); the follow camera picks it up when the object is the focus.

**Script a fixed shot.** Place a `CAMERA` object; entering the world hierarchy pushes CPCAMERA. For a move between
shots, use the RELAY camera-blend commands (CPCAMBLEND).

## 10. What a port must replace

Almost nothing here is hardware-specific. The exceptions:

- VU0 inline asm in the matrix code (`UpdateCmMat4`, `UpdateCpcamera`, the CPTN helpers) and the transposed VU0 cull rows
  used by the renderer's sphere test: plain C++/SIMD.
- `BuildCmFgfn` produces a GS fog function; a port evaluates fog in its shader from `nearFogDistance`/`farFogDistance`/`maxFog`.
- The projection is built for the PS2's 512x224 field buffer and its pixel aspect; a widescreen or high-resolution port
  changes `aspectRatio` and the projection in `RecalcCmFrustrum`.

## 11. Where to look

- `src/game/engine/camera/cm.cpp`: CM, `InitCm`, `UpdateCmLast`, policy stack, matrices, fog, clipping, vis zone.
- `src/game/engine/camera/cplcy.cpp`: base policy and CPLOOK, CPZOOM, CPHANG, CPCAMBLEND, CPTURRET, CPTWOSHOT.
- `src/game/engine/camera/tn.cpp`: TN volumes, CPD records, the CPTN follow camera and line of sight.
- The `CAMERA` / CPCAMERA unit in `src/game/engine/camera/` (still named by address).
- `src/game/engine/graphics/pipeline/vis.cpp`: VIS zones.
- `include/sly2/cm.h`: all camera types.
- Callers: `actor/po.cpp` (`OnPoActive`), `gameplay/player/mode.cpp`, `gameplay/player/grab.cpp`,
  `gameplay/objects/turret.cpp`, `gameplay/cutscene/relay.cpp`, `level/exit_warp.cpp`.
