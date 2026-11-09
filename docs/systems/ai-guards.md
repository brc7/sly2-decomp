# Guards and AI

How guards think: the STEPGUARD class, the behaviour stack (BHV / BHVSTK) that replaced Sly 1's per-guard state
machine, the root behaviour that picks calm / search / alert / combat, perception (vision detectors and noise
stimuli), combat and turn-taking, patrols and path following, guard scripts, the guard manager and spawners, and how to
add a behaviour. For the overview see [engine.md section 7.11](../engine.md#711-guards-and-ai-gameplayai). Guards are
STEPs like the heroes, so the walking body, heading control and floor contact are described in
[player.md](player.md#2-the-class-chain); path zones and collision are in
[physics-and-collision.md](physics-and-collision.md).

## 1. Purpose and shape

A guard is a walking character (STEP) whose decisions come from a **stack of behaviour objects**. The bottom of the
stack is always the root behaviour BHVGUARD, which holds the guard's *mode*; above it sits the behaviour for that mode
(patrol, search, combat, script, ...), and above that the smaller behaviours it delegates to (walk a path, play an
animation, strike, leap). Every frame each behaviour on the stack gets an update, top first, and the top-most behaviour
that "owns the mover" turns its intent into motion. STEPGUARD itself is the host: it owns perception, hit points,
targets, the guard flag word, animation bindings, attacks and loot.

Code lives in `src/game/gameplay/ai/`:

- `stepguard.cpp`: STEPGUARD (load, update, damage, flags, animation bindings, target choice, stimulus perception, the
  mover, engagement records, attack objects, the guard manager).
- `bhvstk.cpp`: BHV base methods and BHVSTK (push, pop, update, messages, busy protocol, the placement factory).
- `guardbhv.cpp`: almost every guard behaviour, including BHVGUARD, BHVANIM and its subclasses, BHVPATH /
  BHVWALKPATH, BHVGOTO, BHVPATROL, BHVATTACK / BHVCOMBAT, BHVLEAP, BHVSHOOT, rail, pipe, juggled, path-link and
  patrol-path behaviours.
- `detector.cpp` (vision), `noise.cpp` (stimuli and listeners), `pickpocket.cpp` (loot).
- `bhvsearch.cpp` holds the search behaviour (`UpdateBhvSearch`), `guardscript.cpp` guard scripts (`UpdateBhvScript`,
  `AddGuardScript*`), `spawner.cpp` spawners (`UpdateSpawner`), `watchdog.cpp` the stuck/hazard watchdogs
  (`UpdateStuckWatch`, `UpdateHazardWatch`), `stepbhv.cpp` the partner/hero behaviours (follow, walk-to-goal,
  approach), and one file per named character (`carmelita.cpp`, `dimitri.cpp`, `neela.cpp`, `rajan.cpp`,
  `contessa.cpp`). Search for those entry points; each file's header comment says
  what it holds.

Types: `include/sly2/step.h` (STEP, BHV, BHVSTK), `include/sly2/guard.h` (STEPGUARD, DETECTOR, BHVGUARD's record,
animation bindings). The behaviour subclasses are still TU-private views in the files that implement them.

## 2. Main types

- **STEPGUARD** (STEP subclass): adds the guard flag word, hit points and KO state, the current and forced targets, a
  vision DETECTOR, animation bindings and three zap-reaction tables, a LEAP and a directional-animation helper, attack
  objects, an engagement link, the loot list, the mover, capability bits and a busy count. Two virtuals beyond STEP:
  choose target (`ChooseStepguardTarget`) and sense stimulus (`CanStepguardSenseStimulus`). One subclass per named
  character (GOMER, GUARD, DIMITRI, NEELA, RAJAN, JEANBISON, CARMELITA, CONTESSA, ARPEGGIO, BEAR, BEARCUB); most
  override only Init, target choice or sensing.
- **BHV**: a BASIC with an owner STEP, links to the behaviours below and above, a busy flag and a busy-above count.
  Its virtual slots: 7 OnPush (`InitBhv` in the header), 8 Start, 9 Close, 10 Update, 11 UpdateMotion, 12 ground-probe
  flags, 13 HandleMessage, 14 the input/steering hook used for hero partners. Each behaviour class also usually has a
  `Set*State` switch that pushes its child for the new state.
- **BHVSTK**: owner, depth, and a fixed array of preallocated behaviour slots. The bottom slot is the root.
- **SGF** (guard flag): one bit per guard condition (idle, attacking, KO, moving, leaping, juggled, attack in progress,
  engaged, patrolling, searching, script, on rail, ...). Behaviours raise their flag in OnPush/Start and lower it in
  Close (`AddStepguardSgf` / `RemoveStepguardSgf`), so **the guard's flag word always equals the union of the flags of
  the behaviours on its stack** (runtime-confirmed). The flags also select animations (section 7).
- **DETECTOR**: a guard's vision (an XFM with near and far volumes), tracking a small set of targets each with a state
  0 unseen, 1 seen, 2 lost.
- **NOISE / NOISETYPE / LISTENER**: refcounted stimuli on the world's stimulus list, the templates that define their
  range, height window and lifetime, and passive listener objects (section 5).
- **Engagement record** (TARGETREC): shared per target; the guards attacking the same target join it and take turns
  (section 6).
- **Guard attack** (an LO class, still placeholder-named): one attack definition (range, height, angle, animation
  binding), owned by its guard.

## 3. Lifecycle

**Load and init.** Guards are level objects, usually proxy copies of one template per world entry (engine.md section 6).
`InitStepguard` (slot 7) runs `STEP::Init`, ensures the world's guard manager exists, creates the DETECTOR, and sets
default vision ranges and angles, hit points, flag masks and the stack. Splice options configure the guard at load:
animation bindings (`SetStepguardAnimBinding` and its kind-2/kind-3 variants), zap reactions, attacks, loot, patrol
script, target lists. `PostStepguardLoad` (slot 15) binds object references, sets up components and attacks and
resolves every animation binding. `PostBhvstkLoad` allocates the stack's slots from the world heap. Loot is rolled at
load (`GenerateStepguardLoot`).

**Activation.** `OnStepguardAdd` / `OnStepguardRemove` (slots 23/22) link and unlink the guard on the world's guard list,
so the list always holds exactly the live guards. The **guard manager** (`UpdateGuardManager`) watches guards that left
the world: after a short grace period it either removes them or restarts their root behaviour and resets their detector,
so a guard that streams back in starts fresh. `ResetStepguardToHome` and `ClearStepguardState` (slots 26/27) reset a
proxy copy or spawned guard for reuse; guards are pooled and toggled, never freed.

**Per frame.** `UpdateStepguard` (slot 16):
1. `UpdateStep` (body physics bookkeeping).
2. Stop here when the guard is not in the world or is time-frozen (a gadget special state).
3. Special-state timeouts (`UpdateStepguardSpecial`), target detection (`UpdateStepguardDetectTargets`: collect nearby
   objects of the preferred classes, refresh the detector targets, update the detector, then the engagement records),
   attack animation end, aim (`UpdateStepguardAim`), loot staging, the stuck and hazard watchdogs.
4. `UpdateBhvstk`: every behaviour's Update, top first.
5. `UpdateStepguardMover`: the mover's driver behaviour runs its UpdateMotion (slot 11), turning the chosen animation
   root motion or path following into desired velocity and heading.

Motion itself is applied in the physics solve through STEP's preset, exactly as for heroes.

**Render.** `RenderStepguardSelf` draws the model plus effects such as the hit flash.

**Teardown.** `RemoveStepguardHierarchy` clears the target and the behaviour stack. Everything else goes with the world.

## 4. The behaviour stack

`bhvstk.cpp` implements the stack; read it once, it is short.

- **Push.** `PushBhvstk(stack, cid)` (wrapped by `PushStepBehavior`) takes the next slot, clears it, constructs the
  behaviour of that class in place (`ConstructBhv`, a switch over the behaviour CIDs that installs the vtable and
  clears the class's size), links it to the owner and the behaviour below, and calls **OnPush** (slot 7). It does
  **not** call Start: the caller configures the new behaviour's fields and then calls slot 8 itself. That is why
  `SetBhvguardMode` and the `Set*State` switches all follow the pattern "push, set fields, Start".
- **Pop.** `PopBhv(&slot)` calls the behaviour's **Close** (slot 9), which pops everything above it first, then
  decrements the depth. Popping a mid-stack behaviour therefore unwinds all of its children. `ClearBhvstk` pops the root.
- **Update.** `UpdateBhvstk` calls Update (slot 10) on every behaviour from top to bottom. A parent sees its children's
  results in the same frame (for example `IsBhvanimDone`, `IsBhvcombatDone`).
- **Messages.** `HandleBhvstkMessage` offers a message to each behaviour from the top down and stops at the first whose
  HandleMessage (slot 13) returns nonzero. Damage arrives this way: `TakeStepguardDamage` forwards the zap to the stack
  as a message, which BHVGUARD answers by picking hurt, knockdown, KO, sleep, held or thrown. Animation events (hit
  windows, root-motion translation, mover lock/unlock, busy on/off) arrive as messages to BHVANIM.
- **Busy protocol.** `SetBhvBusy` marks a behaviour busy, increments the guard's busy count and the busy-above count of
  every behaviour below it; `ClearBhvBusy` undoes it. A parent tests `IsBhvBusyAbove` before re-deciding: BHVGUARD's
  Update loops only while nothing above it is busy, so a strike, leap or scripted segment can finish uninterrupted.
- **Find.** `FindStepBehavior` / `FindBhvInBhvstk` return the first stacked behaviour derived from a class.
- **The mover.** Only one behaviour drives motion at a time: `SetStepguardMoverDriver` hands the mover to a behaviour
  (BHVANIM does it when its animation starts, path behaviours when they walk) and `UpdateStepguardMover` calls that
  behaviour's UpdateMotion. The mover can lock the heading (animation events) so steering does not fight an animation.

## 5. Perception

**Vision.** Each guard owns a DETECTOR (created in `InitStepguard`). `UpdateStepguardDetectTargets` feeds it
candidates: objects of the guard's preferred classes within range, sorted by distance, plus designated targets, minus
ignored classes, with forced classes always included (`Add/Remove` helpers for the preferred, ignored and forced class
lists live in `stepguard.cpp`). The detector's update (`UpdateDetectorTargets`) recomputes each target's state with
`CalculateDetectorTargetState` from its near/far volumes, a line test from the eye through the world's occluders, and the
target's own condition (hidden, disguised, invisible: a hero hiding against cover is concealed). State changes go
through `SetDetectorTargetState`, which notifies the detector by message; when the hero becomes seen the game event for
"spotted" is broadcast. `ChooseStepguardTarget` (slot 88) then picks the guard's target: the forced target while it is in
the world, else the best seen target. `SetStepguardTarget` switches targets and moves the guard between engagement
records.

**Hearing.** A NOISE is created at a position from a NOISETYPE (`CreateNoise`, `NewNoise`, `EmitSwNoise` for the
world's generic types, `ReplaceHeldNoise` for noises that follow their emitter). The world builds its noise types at
load (`CreateSwNoiseTypes`). A noise's audible radius ramps up after emission (`GetNoiseRadius`) and it expires after its
type's lifetime (`ExpireNoise`, run from `UpdateSw`). Guards do not poll listeners: `ChooseStepguardStimulus` scans the
world's stimulus list, asks `CanStepguardSenseStimulus` (slot 89: senses enabled, loud enough, not its own; kind 0 by
range, kind 1 by the detector's near volume with a clear line, ...), and ranks candidates with
`CompareStepguardStimuli` (type priority, loudness, time, distance). Subclasses override sensing (Rajan hears one source
at double range). LISTENER objects are separate passive receivers (`UpdateListener`, `CanListenerHearNoise`) used by
level logic. Things that make noise include the hero (landing, hide-blend changes), bombs, combat guards alerting others
(`EmitStepguardStimulus`) and the Alarm Clock gadget.

**Blindness.** `AreStepguardSensesEnabled` refuses perception while the guard's senses are off (a script option) or a
flag that blinds it is set (alerted, hurt, dead, some special states).

## 6. Modes, combat and turn-taking

**BHVGUARD** is the root and the guard's top-level brain. Its record holds a mode (enum `GUARD_MODE_*` at the top of
`guardbhv.cpp`) and the mode's object. `SetBhvguardMode` pops the current mode behaviour and pushes the one for the new
mode: calm pushes BHVPATROL; suspicious pushes the search behaviour watching a stimulus; alert-animation pushes a
BHVANIM with the "spotted" flag; alert pushes BHVCOMBAT; script pushes the script behaviour; hurt, knockdown and KO push
the reaction behaviours; sleep, thrown, juggled, target list, escort, watch, stalk, relocate and the named-character
modes (flee for Dimitri, route chase for Carmelita, chase for Neyla) push theirs.

`UpdateBhvguard` re-decides while nothing above is busy:
- calm (also escort, watch): a target -> alert-animation (then combat); else a sensed stimulus -> suspicious; else stay
  calm (or escort/watch);
- suspicious: a target -> attack; a newer stimulus -> re-aim the search; search finished -> calm;
- alert-animation done -> alert (Dimitri flees instead);
- alert: combat done -> calm; target changed -> re-target or calm (or watch a fellow guard that grabbed the hero);
- script done -> `ResumeBhvguard`, which picks the mode to return to (flee, stalk, target list, chase, combat on the
  current target, investigate after being hit, else patrol).
A designated target list overrides all of these.

**BHVCOMBAT** runs pursue / close-in / melee / ranged / give-up / hands-off sub-states (`SetBhvcombatState`): it pursues
with BHVGOTO, circles, attacks through BHVATTACK (which pushes BHVSTRIKE and, on a confirmed hit, a follow-through
animation), shoots through BHVSHOOT when `CanBhvcombatShoot` allows (capability, range and line of sight), re-plans when
the target moves far or changes path zone, and emits stimuli to alert others. Attack choice is
`ChooseStepguardAttack`: the first enabled attack whose range, height, angle and optional line of sight accept the
target, confirmed by a Splice event. `TryBhvcombatChooseAttack` pushes a cooldown forward on failure.

**Turn-taking.** All guards targeting the same object share an engagement record (`InitTargetRecord`,
`AddTargetRecordGuard`). `DoesTargetRecordAllowGuard` is the token: a guard may attack only when no other engaged guard is
busy (and, for some record kinds, when it is ready or on camera). Records go stale when not refreshed
(`IsTargetRecordStale`), which `HasBhvcombatLostTarget` uses to give up.

**Damage.** A zap delivered to a guard (physics-and-collision.md section 6) reaches `TakeStepguardDamage` (slot 75),
which drops it when invulnerable or from its own team's bomb, then forwards it to the stack. BHVGUARD's handler picks the
reaction from the zap kind, the reaction tables and the hit points: hurt, knockdown, KO (flung, removed shortly after),
sleep, held by Murray (juggled; hp floored so a finisher is needed), thrown. Game events report damage and defeat to
Splice and the progression system.

**Special states.** Gadget and script effects put a guard into a timed special state (`SetStepguardSpecial`, gated by
`CanSetStepguardSpecial`, ended by `UpdateStepguardSpecial`): shrink, time-frozen, rage, and so on. The world-wide guard
override sets one on every guard (`SetGuardOverride`).

## 7. Animation bindings

Guards do not hard-code animations. Splice binds a *set of guard flags* to an animation (an ASEG, an SM, or a kind-2/3
payload such as a leap) with `SetStepguardAnimBinding`. When a BHVANIM-derived behaviour starts, it raises its flag and
looks up the binding whose flag set is the largest subset of the guard's current flags (`FindBestAnimBinding`,
`FindCurrentAnimBinding`), so "walking while carrying" or "striking while on a rail" pick more specific clips when the
level provides them and fall back otherwise. Zap reactions use three per-kind tables (hurt, knockdown, KO) set the same
way. `StartAnimBinding`-style helpers start and stop the bound animation per kind, and the animation's root motion drives
the mover. Changing a flag sends the guard a message so the playing binding can be re-chosen.

## 8. Patrols, paths and scripts

- **Patrol.** BHVPATROL alternates walking the guard's patrol script for a random time and idling, resuming the script
  where it left off. The patrol script is a **guard script** (an LO built from Splice options: `AddGuardScript*`) run by
  the script behaviour (`UpdateBhvScript`, `ExecuteBhvScriptCommand`): commands set context, walk and pause times,
  teleport, send messages, wait, go to or follow a target, look at, play animations, jump to labels, set busy and
  trigger relay alarms. Commands that take time push a child behaviour and the script waits for it.
- **Path following.** BHVPATH follows a SHAPE (polyline or curve; physics-and-collision.md section 7), detecting
  blockers along it; BHVWALKPATH adds the walk animation and speed. BHVPATROLPATH walks along a shape by advancing a goal
  parameter. Rails and pipes have their own behaviours (BHVRAIL, BHVPIPE) and BHVLEAP jumps onto them.
- **Going somewhere.** BHVGOTO plans a route with the navigation system (path zones and A*; across zones through path
  links with `FindPathlinkRoute`), walks it with BHVWALKPATH (running when its caller raised the run flag), re-plans when
  the route is stale or the navigation version changed, and hands path-link segments (walk or leap) to BHVPATHLINK.
- **Search.** The search behaviour reacts, goes to the last known position, faces it, looks around, visits a few search
  points chosen from nearby hiding places, then gives up (`SetBhvSearchState`).
- **Spawners** clone a guard from a template into a free slot at a spawn point the camera cannot see
  (`UpdateSpawner`, `ChooseSpawnerSpawnPoint`). **Watchdogs** relocate a guard that is stuck or standing in a hazard by
  zapping it (`UpdateStuckWatch`, `UpdateHazardWatch`).

## 9. Interaction with the hero

`UpdateStepplayerInteractCandidates` (player side) scans the world's guard list and ranks each guard with
`GuardEvalInteractMode`: engaged with the hero, hero inside the pickpocket glint volume, close, in range, none. The best
candidates become the hero's interact target (pickpocket, takedown). Pickpocketing (`PickpocketStepguard`) hands the
staged loot entry to Sly; `UpdateStepguardPickpocket` drives the glint and pouch state. Murray's uppercut puts a guard
in the juggled mode, and he can then lift the stunned guard (`StartMurrayLiftGuard`).

## 10. Invariants and gotchas

- **Flag word = union of stacked behaviours' flags.** Raise a flag only in a behaviour's OnPush/Start and lower it in its
  Close; never set guard flags from outside a behaviour, or animation choice and pickpocket state go wrong.
- **Push does not start.** After `PushStepBehavior` set the new behaviour's parameters, then call its Start (slot 8).
- **Close pops children.** A behaviour's Close must pop what it pushed (most store their child slot and call `PopBhv` on
  it); the base handles the depth. Do not hold pointers to behaviours above you after a pop: slots are reused in place.
- **Fixed slot size.** Every behaviour is constructed in a preallocated slot of `GetBhvSlotSize()` bytes, sized for the
  largest class. A larger new behaviour needs that size raised.
- **Depth is bounded** by the fixed slot array; deep delegation chains (mode -> goto -> walkpath -> anim) use most of
  it.
- **CIDs are a range test.** Behaviour classes are numbered in pre-order under BHV, and "is derived from" checks
  compare against the subtree's last CID (engine.md section 6). A new behaviour class must get a CID inside BHV's
  subtree (or BHV's range must be extended), plus a factory case in `ConstructBhv` and a descriptor in
  `app/class_registry.cpp`.
- **Busy blocks re-decisions.** A behaviour that sets busy must clear it on every exit path (Close is the safe place),
  or the guard freezes in its current mode.
- **The guard list is the live set.** Iterating all guards (as the player and gadgets do) only sees guards in the world;
  guards that streamed out keep their stack until the guard manager resets them.
- Many behaviour classes are known only by placeholder CIDs; read their file-level views and doc comments rather than
  trusting the class name.

## 11. How to add a behaviour

1. **Class.** Define the behaviour view as a BHV subclass (in `guardbhv.cpp` or a new file), give it a CID inside BHV's
   subtree, a descriptor and factory in `app/class_registry.cpp`, and a case in `ConstructBhv`. Keep it within the
   slot size.
2. **Slots.** Implement OnPush (defaults, state -1, raise its SGF if it has one), Start (initial state, busy if it must
   not be interrupted), Close (pop its child, clear busy, lower its flag), Update (advance its state machine, test its
   child's done-ness), HandleMessage (return nonzero only for messages it consumes; delegate others to the base), and
   UpdateMotion only if it takes the mover (`SetStepguardMoverDriver`). Unused slots default to the empty BHV methods.
3. **State switch.** Write a `Set<Name>State` that pops the current child and pushes the behaviour for the new state
   (BHVANIM with a flag for animations, BHVGOTO / BHVWALKPATH for movement), following the push-configure-Start pattern.
4. **Animation.** If it needs its own clip, give it a new guard flag and have levels bind an animation to that flag set;
   BHVANIM will find it.
5. **Entry.** Push it from a parent: add a `GUARD_MODE_*` and a case in `SetBhvguardMode` (and the transitions in
   `UpdateBhvguard` / `ResumeBhvguard`), or push it from another behaviour's state switch, or expose it as a guard-script
   command.
6. **Hero partners.** For a behaviour that drives a hero instead of a guard, implement slot 14 to write the hero's POCTL
   (player.md section 8); STEPPLAYER calls it on the top behaviour every frame.

## 12. Where to look

- `src/game/gameplay/ai/stepguard.cpp`, `bhvstk.cpp`, `guardbhv.cpp`: the core.
- `src/game/gameplay/ai/detector.cpp`, `noise.cpp`, `pickpocket.cpp`: perception and loot.
- The other files in `src/game/gameplay/ai/`: search, scripts, spawners, watchdogs, partner behaviours and the named
  characters; each header comment lists its entry points.
- `src/game/gameplay/player/stepplayer.cpp` (`UpdateStepplayerInteractCandidates`) and the unit defining
  `SetGuardOverride`: the hero's side of guard interaction.
- `src/game/engine/physics/path.cpp`: navigation used by BHVGOTO; `src/game/engine/physics/shape.cpp`: patrol paths.
- `include/sly2/step.h`, `include/sly2/guard.h`.
