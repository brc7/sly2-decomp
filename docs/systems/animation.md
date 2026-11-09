# Animation

How objects move without physics: transform slots (XF), the actions (ACT) that drive them, animation clips (ASEG) and
their playing instances (ASEGA), blends, state machines (SM/SMA), IK, tails/ropes, transform constraints and space
warps. For the short summary see [engine.md section 7.5](../engine.md#75-animation-engineanim); for the SO side
(where XFs live) see [physics-and-collision.md](physics-and-collision.md).

## 1. The model in one paragraph

An SO's "skeleton" is its **table of XFs**. An XF is one degree-of-freedom group of one joint (translate, rotate,
scale, shear or pivot), not a bone; a joint is a stack of XFs, and an XF's world matrix is its local matrix times its
parent's world matrix (row vectors). Each XF has a **role** (at rest, velocity-driven, chasing a goal, spring, curve,
IK, chain, constraint, ...) and a **priority-sorted list of actions** (`ACT`); the head action is the one in effect.
Animation clips do not write matrices: an `ASEG` clip, when applied, creates one action per channel on the XFs it
animates, and those actions feed values into the per-frame slot update (`UpdateSoSlots`). State machines (`SM`) choose
which clip plays next. So almost everything in this system is "put an action on an XF and let the slot update evaluate
it".

## 2. Main types

| Type | What it is | Where |
|---|---|---|
| `XF` | a transform slot of an SO: component kind, role, value/velocity/rest state, local and world matrix, parent, head of its action list | `include/sly2/so.h`, `engine/anim/xf.cpp` |
| `ACT` | an action on one XF: owner object, target XF, priority, next action, role; virtual slots attach, detach, switch, update, goal, clone | `include/sly2/act.h`, `engine/anim/act.cpp` |
| ACT kinds | position act, look-at act, path act, offset act (wraps another act), IK act, chain act, matrix/Bezier acts, the clip channel act (ACTSEG) | `act.cpp`, `ik.cpp`, `tail.cpp`, `asega.cpp` |
| `AC` / `ACG` families | vector and float curves: constant, two keyed (Bezier/Hermite) kinds, two per-component kinds, blended | `act.cpp` (`PacNewKind`, `PacNewKind2`) |
| `ASEG` | an animation clip (an LO loaded from the level): channels keyed by object oid + XF component, overrides, labels, start/frame/end events, master speed, optional auto-start | `include/sly2/aseg.h`, `engine/anim/aseg.cpp` |
| `ASEGA` | a clip **applied** to an object hierarchy: refcounted, local time, speed (own x master), priority, event cursor, hands-off hold, the channel acts it created | `aseg.h`, `engine/anim/asega.cpp` |
| `EA` | one clip event at a local time (sound, rumble, hands-off, other clips, Splice call, emitter, message, show/hide...) | `aseg.h` |
| `ASEGBL` | a blended clip: several clips with weights and sync tables (`MRSG`) | `aseg.h`, `engine/anim/blend.cpp` |
| `SM` | a state machine (an LO from the level): states (`SMS`, each an oid), transitions (`SMT`: from, to, clip, weight, flags) and a precomputed state-distance table | `aseg.h`, `engine/anim/sm.cpp` |
| `SMA` | an SM **applied** to an object: refcounted, current state, goal, running transition and its ASEGA, transition mask, speed | `aseg.h`, `sm.cpp` |
| IK record | two-bone IK: first bone, second bone, parent XFs and lengths | `engine/anim/ik.cpp` |
| chain | a tail / rope: a chain of XFs simulated with damping | `engine/anim/tail.cpp` |
| transform constraint | destination object's position and/or rotation follow a source's | `engine/anim/constraint.cpp` (`ApplyXfmConstraint`) |
| `WR` | a space warp (an LO): wave and noisy-bend entries applied to the vertices of warp groups | `engine/anim/wr.cpp` |

Two refcounted "application" objects (`ASEGA`, `SMA`) are the important ownership boundary: the level owns `ASEG` and
`SM`; gameplay code owns applications through counted slots.

## 3. Lifecycle

### Load

- `LoadSoXfsFromBrx` reads an SO's XF table (component kinds, parents, rest values, initial roles). XF ids in level data
  resolve through a context stack while nested objects load (`PushXfContext`, `ResolveXfId`).
- SO loading also calls `LoadSoIkFromBrx` (IK records), `LoadSoChainsFromBrx` (tails/ropes) and
  `LoadXfmConstraintsFromBrx` (constraints); each installs an action of the matching role on its XFs.
- `LoadAsegFromBrx` reads clips: per channel a curve from the curve factories (`PacNewKind`, `PacNewKind2`), times in
  1/60 s ticks. `SM::LoadFromBrx` reads states and transitions and builds the distance table.
- Post-load: `ASEG::PostLoad` / `SM::PostLoad` (the slot-15 post-load hooks) start auto-start clips and
  machines through deferred world callbacks (`PostAsegLoadCallback`, `SetSmPendingApply`).

### Per frame

From `UpdateSw` ([engine.md section 4](../engine.md#4-the-frame-loop)):

1. Live objects' Update (LO slot 16) run first: gameplay code sets goals, applies clips, changes SM goals.
2. The world's live ASEGA list: **`UpdateAsega`** for each: advance local time by the effective speed, wrap (loop) or
   reflect (ping-pong) via `WrapAsegaTime`, dispatch crossed events (`HandleAsegaEvents` -> `HandleAsegaEvent`),
   retract a finished non-looping clip (`RetractAsega`). A finished transition clip sends "animation finished" to its
   SMA (`HandleSmaMessage`), which ends the transition and chooses the next.
3. Physics and the SO transform step: SO slot 17 (`UpdateSoXfWorld` in `engine/physics/so.cpp`) calls
   **`UpdateSoSlots`**, which for every XF evaluates the head action's value, applies the role (integrate velocity,
   chase with smoothing limits, spring-damper, Hermite to a target time, IK solve, chain step, constraint), stamps it,
   and rebuilds the world matrix from the parent. Settled slots drop to a resting role (`SettleXfSlot`) so idle objects
   cost nothing; an SO with no awake slots stops being woken.
4. Rendering reads the XF world matrices (skinning matrices are taken from them in `BuildModelDrawPackets`, see
   [rendering.md](rendering.md)).

### Teardown

Clips and machines are world-heap objects and go with the level. Applications are pooled in the world; releasing the
last reference (`DecrefAsega`, `DecrefSma`) returns them to their pool.

## 4. XF roles and actions

`UpdateSoSlots`' doc comment in `xf.cpp` lists every role. The families:

- **Resting** roles: value fixed (`IsXfRoleAtRest`).
- **Velocity** roles: constant acceleration on a velocity, or continuous rotation (`IsXfRoleVelocity`).
- **Actuator value** roles: take the head action's value directly (clip channels replace the XF value).
- **Chase** roles: move toward the action's goal under a speed limit, optionally arriving by a given time.
- **Spring** role: spring-damper toward the rest value, settling into a rest role.
- **Delegated** roles: IK, transform constraint and chain roles call their unit's per-frame function.

`AddXfAct` inserts an action sorted by priority; when it becomes the head, the slot wakes and the old head detaches.
`RemoveXfAct` and `SortXfActs` keep the list consistent. `SetXfRole` captures the current velocity when switching into a
moving role, so motion is continuous across role changes. Frame conversion helpers (`TransformXfPos`, `ConvertXfVec`,
`ConvertXfMat`, `GetRelativeXfMatrix`) and velocity queries (`GetXfVelocity`, `GetXfAngularVelocity`,
`AccumulateXfChainVelocity`) are what gameplay code uses to reason about animated parts.

## 5. Clips: ASEG and ASEGA

Entry points for gameplay code (all in `aseg.cpp`):

- `ApplyAsegCore` / `ApplyAsegToObject`: start a clip on a root object; returns the new ASEGA.
- `EnsureAseg`: reuse the clip's existing application on that root (`FindAsegPlayerForObject`) or start one, then seek.
- `ReplaceCurrentAseg(..., &slot)`: the common pattern: a "current clip" slot that releases the previous application and
  holds a reference to the new one.
- `FindAsegLabel` / `GetAsegLabelTime`: named times inside a clip (used to line up blends and seeks).
- `PredictAsegTransform`: where a clip would put an object at time t, without applying it (used for planning moves).

Controls on an application (`asega.cpp`): `SeekAsega` (absolute, relative or to a label), `SetAsegaSpeed`,
`SetAsegaMasterSpeed`, `SetAsegaPriority` (re-prioritises all its channel acts so it can override or yield to other
animation on the same XFs), `SetAsegaHandsOff` (holds the world's hands-off count while playing), `RetractAsega`.

**Channels replace, they do not add.** A channel's act sets the XF value. Layering is done by priority (the higher
application wins per XF), by blends (section 6), or by animating different XFs.

**Events** fire when the time cursor crosses them, in either direction of play. A clip started at t > 0 replays the
state-changing events up to the start time (`HandleAsegaEventsFastForward`) so the world matches. Events are the bridge to sound
(start/stop), effects, Splice scripts, rumble, show/hide with fade, and other clips.

## 6. Blends

- **ASEGBL** (`blend.cpp`): `EnsureAsegBlendDynamic` finds or creates a blend of a set of clips with weights;
  `RecalculateAsegBlend` re-blends after weight changes. Each clip gets an `MRSG` table of sync points so clips of
  different lengths stay phase-aligned (`FillBlendSyncTables`, `CalculateBlendSyncTable`, `FindSyncInterval`); the dominant clip supplies
  events. Blended channels evaluate positions/rotations with VU0 code. Users: grab, hang, throw and locomotion code in
  `gameplay/player/`.
- **Directional blend** (`gameplay/player/dirblend.cpp`): eight directional clips whose weights are solved so the root
  motion matches the requested velocity.
- **GAIT, LEAP, DIRANIM**: gameplay-side helpers (walk/run weights and footsteps, animation-driven jumps, best of
  directional clips) built on the above; see [player.md](player.md).

## 7. State machines: SM and SMA

`SM` is level data; `SMA` runs it on an object.

- Start: `ApplySmCore` and wrappers, or the slot pattern `ReplaceSma(&slot, ...)` / `ReleaseSma`.
- Drive: `SetSmaGoal` (by state oid; may break off a running transition if it allows breaking), `SetSmaGoalIfReachable`,
  `SeekSma` (re-choose immediately), `SnapSma` (jump without animating), `SkipSma` (advance through transitions by time),
  `SetSmaMask` (restrict usable transitions), `SetSmaSvt` (speed).
- Query: `GetSmaCurrentState`, `GetSmaGoal`, `GetSmaNext`, `EstimateSmaTimeToState` (estimated time to reach a state),
  `CountSmTransitionHops`.
- **Choice**: `ChooseSmaTransition` wanders (`ChooseRandomSmTransition`, weighted random) with no goal, otherwise takes a
  transition one hop closer to the goal (`ChooseSmTransitionTowardGoal`, weighted among equals), and plays the transition's clip.
- **Notifications**: entering a state calls `NotifySmaSpliceOnEnterState`: a message to listeners and a Splice event
  with the from/to state oids, which is how level scripts react to machines.

SMs drive buttons, doors, task visibility records, guard animation bindings, player state changes and dialogue
lip-sync (the sound manager sets the speaker roles' SM goals; see [sound-and-music.md](sound-and-music.md)).

## 8. IK, chains, constraints, warps

- **IK** (`ik.cpp`): `SolveIk` solves two bones by the law of cosines after forward kinematics; `UpdateSoIkBoneLengths`
  keeps lengths current. The hang code calls it directly for hand placement.
- **Chains** (`tail.cpp`): `UpdateChainLink` runs per link; the first call in a frame simulates the whole chain from that
  link to the tip (damped, VU0 code), later calls read the result.
- **Constraints**: `ApplyXfmConstraint` sets the destination's local position/rotation so its world transform equals
  the source's (with offsets); used to attach props to hands and similar.
- **WR** (`wr.cpp`): a deformer, not a skeleton feature. Up to a fixed number of wave/bend entries over a few warp
  matrices (`UpdateWrMatrixes` per frame), applied per vertex to warp groups registered at glob load
  (`AddWrWarpGroup`). Noise from `UNoise`.

## 9. Invariants and gotchas

- **Only the head action matters.** An action lower in an XF's list is ignored until those above it go; forgetting to
  retract an application leaves its channels overriding everything at that priority.
- **Release applications through their slots.** `ReplaceCurrentAseg`, `ReplaceSma` and `ReleaseSma` handle refcounts; holding a
  raw ASEGA/SMA pointer without a reference can dangle after the clip ends.
- **Events are crossed, not polled.** Seeking backward or wrapping can fire events again; zero speed fires none.
- **Clip time is game time** times speed; anything that must stay in sync with audio (lip-sync, rhythm) uses stream time
  instead ([sound-and-music.md](sound-and-music.md)).
- **Settling matters for performance**: objects whose slots never settle stay awake and keep the SO update busy.
- Act kinds, curve classes and some role numbers still have provisional names (`ACT1`, `ACT7`...); match by
  behaviour, not name, and check the doc comments' `(inferred)` marks.

## 10. How to

**Play a clip on an object from code.** Keep an `ASEGA *` slot on your object and call `ReplaceCurrentAseg(aseg, root, t,
speed, flags, &slot)`; read `IsAsegaRetracted` or use an event to know when it ends; pass a null clip to clear the slot.

**Animate a state change.** Author an SM with states and transition clips, apply it once (`ReplaceSma`), then only set
goals (`SetSmaGoal`); the machine routes and plays clips. Listen for state entry by message or Splice event.

**Drive one joint procedurally.** Create a position/look-at/path act (`act.cpp` creators) on the XF with a priority above
the clip that also animates it, or set the XF's role (`SetXfRole`) for spring or chase motion.

## 11. What a port must replace

The logic is portable C++. The PS2-specific parts are **VU0 inline asm and microprograms** in `xf.cpp` (world matrix
products), `blend.cpp`, `ik.cpp`, `tail.cpp` and `wr.cpp`, which become plain C++ or SIMD. Timing assumes the game clock
(clamped frame delta); a port that decouples simulation from frame rate must feed `UpdateAsega` and `UpdateSoSlots` a
consistent dt.

## 12. Where to look

- `src/game/engine/anim/`: `xf.cpp` (slots, `UpdateSoSlots`), `act.cpp` (acts and curves), `aseg.cpp`, `asega.cpp`,
  `blend.cpp`, `sm.cpp`, `ik.cpp`, `tail.cpp`, `wr.cpp`, and `constraint.cpp` (transform constraints).
- `src/game/engine/physics/so.cpp`: `UpdateSoXfWorld` (SO slot 17), which calls `UpdateSoSlots`.
- `src/game/gameplay/player/`: `dirblend.cpp` and the GAIT/LEAP users; `include/sly2/leap.h`.
- `include/sly2/aseg.h` (ASEG, ASEGA, ASEGBL, SM, SMA, EA), `include/sly2/act.h`, `include/sly2/so.h` (XF).
