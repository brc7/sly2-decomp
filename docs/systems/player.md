# Player characters

How Sly, Bentley and Murray work: the class chain from PO to the three heroes, the per-frame update, the state machines,
input, the shared movement components (air, grab, hurt, hang, pipe, cane, throw, ...), gadgets, and how the AI partner
drives a hero that is not under player control. For the overview see
[engine.md section 7.10](../engine.md#710-player-characters-actor-gameplayplayer); for the object model (slots, OnAdd /
OnRemove, proxies) see [engine.md section 6](../engine.md#6-the-object-model). Guards share the STEP base and the
behaviour stack described here: see [ai-guards.md](ai-guards.md). The physics the characters stand on is in
[physics-and-collision.md](physics-and-collision.md).

## 1. Purpose and shape

A hero is a solid object (SO) that a controller drives, that walks on legs, and that owns a set of movement
*components*: self-contained sub-records that each implement one kind of movement (jumping, grabbing, climbing a pipe,
being hurt, ...). Sly 1 had these as sub-structs of Sly's class JT; Sly 2 lifted them into a shared base, STEPPLAYER,
so Bentley and Murray reuse the same air, grab, hurt, pipe and throw code with their own state machines on top.

Code lives in two places:

- `src/game/actor/` holds the two shared bases: `po.cpp` (PO and its controller POCTL, plus the world's "current
  player" selection) and `step.cpp` (STEP: walking body, heading control, floor contact, behaviour-stack accessors).
- `src/game/gameplay/player/` holds STEPPLAYER (`stepplayer.cpp`), the three heroes (`jt.cpp`, `bentley.cpp`,
  `murray.cpp`) and one file per component (`air.cpp`, `grab.cpp`, `hurt.cpp`, `hang.cpp`, `pipe_jt.cpp`,
  `cane.cpp`, `throw.cpp`, `mode.cpp`, `places.cpp`, `jtgoal.cpp`, `jthands.cpp`), plus the grabbable world objects
  (`grabspot.cpp`, `pipe.cpp`, `rail.cpp`, `hnd.cpp`) and animation helpers (`dirblend.cpp`, the GAIT/LEAP unit).

The types are declared in `include/sly2/po.h`, `step.h`, `stepplayer.h` (STEPPLAYER and every component record),
`jt.h` (JT) and `players.h` (MURRAY, BENTLEY). The button bits are named in
`src/game/gameplay/player/player_input.h`.

## 2. The class chain

```
SO -> PO -> STEP -> STEPPLAYER -> JT (Sly)
                               -> MURRAY
                               -> BENTLEY
              STEP -> STEPGUARD (guards, see ai-guards.md)
```

- **PO** (player object) is an SO with a **POCTL** controller. POCTL is a BASIC child the PO owns; each frame it turns
  the pad into camera-relative polar stick values (heading and deflection), a held-button word `grfbtn`, and a
  timestamp per button so input can be buffered (`GrfPressedWithin`, `GrfConsumePressed`, `ClearPoctlJoyPressed`).
  PO adds the virtuals for invulnerability (`IsPoInvulnerable`), health state, activation, control hand-off
  (`SetPoCtlState`), healing and `AddPoHpMp`. `po.cpp` also owns the world's player bookkeeping: the current PO
  (`GetCurrentPo`, `SwitchToPo`), followers and the PO list used for switching and the HUD.
- **STEP** is the walking body. It keeps a target heading (`SetStepTargetHeading`, `FaceStepToward`, current heading
  from `GetStepHeading`), turns toward it in its physics preset, keeps a cached floor contact found by a ground probe,
  smooths its collision ("phys") part toward the active shape so crouch/stand changes blend, and owns the behaviour
  stack `BHVSTK` (`PushStepBehavior`, `FindStepBehavior`).
- **STEPPLAYER** is abstract. It embeds the components (section 5), the three gadget slots, HP/MP bindings, the
  interaction target (guard to pickpocket or take down), idle/lean state and the script hooks. Its virtuals beyond
  STEP are the hero interface the components call: set/get the generic player state, apply per-state step parameters,
  can-use-gadget / start gadget, stun hold, camera focus, hero died, interact mode.
- **JT, MURRAY, BENTLEY** each override Init / LoadFromBrx / PostLoad / Update / HandleMessage, the physics preset,
  the camera definition (`GetCpdefi`), the gadget virtuals, and the player-state mapping.

`g_pjt` is always Sly, not "the current player". The PO that has control is the world's current PO (`GetHeroPo`),
which is published as the save variable `oidPoCurrent`; heroes that are not current are out of the world.

## 3. Lifecycle

**Load.** Heroes are ordinary level objects: `PloNew` + slot 14 `LoadFromBrx` (`LoadJtFromBrx`,
`LoadMurrayFromBrx`, `LoadBentleyFromBrx`, each chaining down to `LoadStepFromBrx` and `LoadPoFromBrx`). Slot 7
`Init` (`InitJt`, `InitMurray`, `InitBentley` -> `InitStepplayer` -> `STEP::Init` -> `InitPo`) sets defaults and
initialises every component (`InitJtPipe`, `InitThrow`, `InitJtgoal`, `InitJtHands`, ...).

**Post-load.** Slot 15 (`PostJtLoad`, `PostMurrayLoad`, `PostBentleyLoad` -> `PostLoadStepplayer` -> `PostStepLoad`)
resolves object references, looks up animations (ASEGs) and state machines by oid, builds the step's phys part
(`BuildStepPhysPart`), runs each component's post-load (`PostJtLoadPipe`, `PostJtThrowLoad`, `PostJtLoadSwing`,
`PostJtLoadGait`, the hurt table), binds HP and MP to the per-character save variables (`BindStepplayerHpMp`) and
loads the gadget slots (`LoadStepplayerGadgets`).

**Per frame.** Only live objects are updated (OnAdd/OnRemove, engine.md section 6). `UpdateSw` first refreshes every
POCTL (`UpdatePoctl`), then calls slot 16 on each live object. For a hero that is:

1. `UpdateStep` (via `UpdateStepplayer`): SO update, phys-part smoothing (`UpdateStepPhys`) and the turn-rate factor.
2. `UpdateStepplayer`: gadget slots (`UpdateStepplayerGadgets`), hint acknowledge; when not hurt, the walk-to goal
   (`UpdateJtgoal`) and then either the **top behaviour's input slot** (the AI partner, section 8) or the script ride
   control; interaction candidates among nearby guards (`UpdateStepplayerInteractCandidates`), the water hazard
   (`UpdateStepplayerWater`), the hide blend, the interact target pick (`PickHeroInteractTarget`), the gadget menu
   button, and finally `UpdateBhvstk`.
3. The character's own update. For Sly, `UpdateJt` runs `UpdateJtPre` when the hero has a controller: gather grab
   candidates (`GatherJtGrabCandidates`), mode input, **`ChooseJtGroundState`**, cane input, then a per-state switch
   that calls the active component's per-frame function (`UpdateJtActiveJump`, `UpdateJtActiveHang`,
   `UpdateJtActiveHide`, ...) and sets the desired velocity/heading. `UpdateJt` then runs per-state animation upkeep
   (gait blend and footsteps, directional blends, hang IK goals, mesh attachment), the cane update, the hand-reach test
   that grabs a rope, pole or hook, and `UpdateJtLean`. Murray (`UpdateMurray`) and Bentley (`UpdateBentley`) follow
   the same outline with `UpdateMurrayStateMachine` / `UpdateBentleyStateMachine` in place of `ChooseJtGroundState`.

**Physics.** The update only decides *intent* (state, target heading, desired velocity). Movement happens later in the
frame when `SolveSw` runs: each awake object's slot 17 calls its slot 70 physics preset (`PresetJtAccel`,
`PresetMurrayAccel`, `PresetBentleyAccel`), which runs the STEP base (steering toward the target heading) and then the
active component's preset (`PresetJtAccelJump`, `PresetJtAccelHang`, `PresetJtAccelPipe`, `PresetJtAccelHide`,
`PresetJtAccelZap`, ...). Contact hooks (slots 55 to 58, `AdjustStepNewXp`, `AdjustJtXp`, `AdjustStepXps`) tune friction
and closing speed on the floor; components that pin the hero to geometry add constraint contacts
(`UpdateJtInternalXpsPipe`, `UpdateJtInternalXpsHide`). See physics-and-collision.md section 4.

**Render.** Slot 19 (`RenderStepplayerSelf`) draws the model through the SO path plus hand IK (`RenderJtHangIk`) and
effect overlays.

**Teardown.** Heroes are removed with the world on a level change; switching characters takes the old hero out of the
world (OnRemove) rather than freeing it. `OnStepplayerActive` and `SetStepplayerCtlState` handle activation and control
hand-off; becoming the current player clears the behaviour stack, so AI-partner behaviours never survive into player
control.

## 4. State machines

### Sly (JT)

Sly's state is the pair `jts` (movement state, enum `JTS`) and `jtbs` (body sub-state, enum `JTBS`), with the times they
were entered (`tJts`, `tJtbs`). There is a third, independent axis, `jtas`, for cane/attack animations (section 6).
States seen at runtime include stand, run, air, hang, hurt, interact, grab, pipe, pickpocket, walk, surface climb,
hang-follow and throw/aim; several enumerators are still unnamed (`JTS_UnkNN`).

- **`SetJtState(jt, jts, jtbs)`** is the only place a state changes. It returns at once when nothing changes;
  otherwise it runs the *exit* actions of the old state (leaving the air: landing sound and dust, `SetJtAirState(-1)`;
  leaving a hang: untrigger and unsubscribe the hook, detach, reset the camera tunnel; ...), remembers what Sly left
  when he goes airborne (so a jump off a rope or pipe does not immediately re-grab it), picks the animation, state
  machine, sound and channel mask for the new state, stamps the entry time, calls slot 90
  (`ApplyJtStateStepParams`: per-state step tuning), and finally retracts the previous animation and applies the new
  ASEG or SM. Code that needs Sly in a state calls this function; it never writes `jts` directly.
- **`ChooseJtGroundState`** runs every frame and, from the current state, the stick, the buttons and the floor,
  decides transitions between the grounded states (stand, run, walk, fight stance, ...). It loops until the state is
  stable, so one frame can chain several transitions.
- Components switch Sly between broad states through the **generic player state** (slot 88, `SetJtPlayerState`, and
  its inverse slot 89, `GetJtPlayerState`): generic 0 stand, 1 air, 2 grab, 3 hurt, 4 an unnamed state, 5 scripted
  animation, 6 scripted state-machine animation, 7 throw, 8 pipe. For example the air component calls the owner's
  slot 88 with 1 when a jump starts and 0 when it lands; the grab component calls it with 2. This indirection is what
  lets the same component code run inside all three heroes.

### Murray and Bentley

Each has one integer state switched by **`SetMurrayState`** / **`SetBentleyState`** and evaluated each frame by
**`UpdateMurrayStateMachine`** / **`UpdateBentleyStateMachine`**. The enums are at the top of `murray.cpp`
(`MurrayState`) and `bentley.cpp` (`BentleyState`):

- Murray: idle, run, jump, flop (air slam), lift, carry, struggle, throw, toss, strafe, walk, attack (punch combo,
  uppercut, takedown finisher), power move, grab, hurt, aimed throw, the scripted states, pipe and turnbuckle.
  Attacks are a sub-machine (`SetMurrayAttack`, `UpdateMurrayAttack`, `TryMurrayTakedown`); lifting and carrying use
  `StartMurrayLiftGrabspot` / `StartMurrayLiftGuard`, `UpdateMurrayLift`, `ThrowMurrayCarried`, `DropMurrayCarried`.
- Bentley: idle, run, air, grab, hurt, near-target, gadget (with a sub-state), walk, hover, the scripted states, throw
  (aiming a Trigger Bomb), pipe and adrenaline. His dart aim is `UpdateBentleyAim` and `FireBentleyDart`; the hover pack
  has its own machine (`SetBentleyHoverState`).

Their slot-88 overrides (`SetMurrayPlayerState`, `SetBentleyPlayerState`) map the same generic player states onto
these enums, so a component that asks for "air" gets `MURRAY_JUMP` or `BENTLEY_AIR`.

## 5. The movement components

Each component is a record embedded in STEPPLAYER whose first word points back to its owner. Its functions take the
record (`JTAIR *`, `JTGRAB *`, ...) and follow a common pattern: an `Init*` / post-load pair, a `Set*State` state switch,
an `UpdateJtActive*` per-frame function called from the owner's per-state switch, a `PresetJtAccel*` physics preset
called from the owner's slot 70, and sometimes an `UpdateJtInternalXps*` constraint hook.

- **Air** (`JTAIR`, `air.cpp`): every launch into the air goes through `SetJtAirJump` (jump kind, launch presets,
  double-jump availability); `SetJtAirState` switches air sub-states; `UpdateJtAirInput` and `UpdateJtActiveJump` run
  per frame and `PresetJtAccelJump` integrates the arc. Landing target choice is `ChooseJtLanding`; ledge and edge
  catches are `FindJtBestEdge` / `FCheckJtSingleEdge`; `IsJtStuck` detects being wedged. Also the jump-target object
  (JMT).
- **Grab** (`JTGRAB`, `grab.cpp`): the world keeps a short list of grab candidates that `GatherJtGrabCandidates`
  refreshes each frame from the grab spots near the hero (section 7) and filters by a per-character mask. Grabbing
  goes through `SetJtGrabState` / `SetJtGrabKind`; input is `UpdateJtGrabInput`; the hide (wall press) state is
  `UpdateJtActiveHide` with its own preset. The grab component also handles landing on rails.
- **Hands** (`JTHANDS`, `jthands.cpp`): fits two hand clips to the grab target every frame (`UpdateJtHands`) and
  attaches / releases held objects (`AttachJtHandsObject`, `ReleaseJtHandsActs`).
- **Hang / swing** (`hang.cpp`, ported from Sly 1's stephang): swinging from a hook (HND) as a pendulum
  (`CalculateJtHangAccel`, `AnticipateJtForce` for moving anchors), `UpdateJtActiveHang`, `PresetJtAccelHang`, hand IK.
  Hang state is Sly-specific and lives in JT's `JTANIM` record.
- **Pipe** (`JTPIPE`, `pipe_jt.cpp`): climbing a PIPE shape: `SetJtPipeState` (attach, idle, climb, turn, release),
  `UpdateJtActivePipe` (stick to climb speed), `PresetJtAccelPipe`, and the constraint `UpdateJtInternalXpsPipe` that
  keeps the hero on the pipe.
- **Hurt** (`JTHURT`, `hurt.cpp`): damage in and the hurt state machine. `TakeStepplayerDamage` (slot 75) forwards to
  `TakeJtDamage`, which lets the owner veto (`ShouldStepplayerVetoDamage`), filters by zap kind, scales by difficulty,
  decrements HP, broadcasts the game event, plays the hit clip and sound and starts knock-back
  (`StartJtKnockback`). `SetJtHurtState` runs the sub-states including the death states; when the hurt animation ends a
  stored callback runs (`SetJtHurtCallback` / `RunJtHurtCallback`), for example returning the hero to a safe place.
  `IsJtInvulnerable` and `IsStepplayerInvulnerable` filter zaps (hands-off, cheat, state-dependent kinds).
  `UpdateStepplayerWater` inflicts drowning/hazard damage when the hero is inside a hazard volume.
- **Places** (`JTPLACES`, `places.cpp`): remembers recent safe footholds (`UpdateJtPlaces`) and puts the hero back on
  the best one after a fall or hazard (`ReturnJtToSafePlace`, `ReturnJtToPosition`); `RemoveSwPlacesOf` forgets places
  on an object that leaves the world.
- **Mode** (`JTMODE`, `mode.cpp`): the look (binocucom, kind 0) and recon (kind 1) modes with their HUD:
  `StartJtMode` / `EndJtMode`, `SetJtModeState`, `UpdateJtMode`. Recon selection (picking photo targets, RECONTGT
  objects) is in the unit that defines `UpdateReconSel`.
- **Goal** (`JTGOAL`, `jtgoal.cpp`): a scripted walk-to point (`SetJtgoal`, `UpdateJtgoal`) with anti-stuck logic that
  fades blocking guards or warps the hero onto the goal. While `IsJtgoalHolding` is true the AI input and the
  behaviour stack are skipped.
- **Throw** (`JTTHROW`, `throw.cpp`): carrying and aiming an object, the predicted arc and marker, `SetJtThrowState`,
  `UpdateJtThrow`, `ReleaseJtThrowObject`.
- **Gait / leap / directional blends** (`dirblend.cpp` and the unit that defines `UpdateGaitBlend`): walk/run blend and
  footsteps (`UpdateGaitBlend`, `UpdateGaitFootsteps`), LEAP jumps between two points (`StartLeap`, `UpdateLeap`),
  and solving eight directional clips so root motion matches a requested velocity (`UpdateDirectionalBlend`,
  `UpdateDirectionalAcceleration`).

Not every component is meaningful for every hero (hang and cane are Sly's), but the records exist in all three.

## 6. Input and Sly's cane

Gameplay never reads the pad directly. The world updates each POCTL before object updates; a hero reads its own POCTL
(`pctl`) through the stick fields and the button helpers. `GrfConsumePressed(mask, dt)` returns a press that happened
within `dt` seconds and consumes it, which is how jump and attack presses are buffered across state changes. The bit
meanings are in `player_input.h` (`BUTTON_JUMP`, `BUTTON_ACTION`, `BUTTON_ATTACK`, `BUTTON_ALT_ATTACK`,
`BUTTON_RUN`, the three gadget buttons, look, hint and gadget menu); the physical buttons for the gadget and look bits
are (inferred). A PO whose control state is not "current" still has a POCTL: the AI partner and scripts write into it
(section 8). Hands-off and the input lock (engine.md section 5) make the hero ignore input and be invulnerable.

The cane (`cane.cpp`) is an overlay on Sly's ground states rather than a `jts` state: `SetJtCaneState` switches the
attack state (`jtas`) with its animation, sound and timing window, `UpdateJtActiveCane` reads input, and
`ChooseJtAttackTarget` (with sweep, rush and smash variants) picks what the swing homes in on.

## 7. Grab spots and grabbable objects

A **GRABSPOT** is a mix-in embedded in grabbable classes (walls, poles, hold points, barrels, mission trigger spots,
carry spots, the table, PIPE, RAIL, HND). It has its own small set of virtuals: grab position, closest parameter, wrap
range and sparkle position (for example `GetPipeGrabPosition`, `GetRailClosestParam`, `GetHndGrabPosition`).
`InitGrabspot` sets defaults; each grabbable class adds itself to a per-class world grab list in OnAdd and removes
itself in OnRemove, so only live spots are candidates. `GetGrabspotFromLo` finds the spot of an object. PIPE and RAIL
are SHAPEs (polylines/curves, see physics-and-collision.md section 7); HND is a TARGET with a grab spot. Trigger grab
spots double as mission trigger spots: grabbing one fires a task request (`FireTriggerGrabspot`).

## 8. The AI partner and scripted control

When a hero is not the current player it is usually out of the world. When it is in the world but not under player
control (Murray following Sly, a partner walking to a mark in a cutscene), it is driven through the same behaviour
stack the guards use. `UpdateStepplayer` calls the top behaviour's input slot with the hero's POCTL; the behaviour
writes stick and button values into it, and the hero's normal state machine reacts as if a player were holding the
pad. Example: following Sly pushes a follow behaviour and, under it, a walk-to-goal behaviour; Splice starts it with
`StartStepplayerFollow`. The behaviours that drive heroes (follow, walk-to-goal, approach-and-interact) live in the
unit that defines them under `src/game/gameplay/ai/` (search for `StartStepplayerFollow`'s callees). There is no guard
root behaviour on a hero's stack.

Scripts (RELAY/JSG sequences, engine.md section 7.14) take over a hero through the ride/script hook
(`UpdateJsgJtControl`), the walk-to goal (`SetJtgoal`), the scripted animation states (`PlayStepplayerScriptAnim`,
generic player states 5 and 6) and controller hand-off (`SetCtlState` on another PO for minigames).

## 9. Gadgets, HP and MP

- STEPPLAYER has three gadget slots (`GADGETSLOT`: gadget id, state, time). `UpdateStepplayerGadgets` runs per frame:
  on a buffered press of the slot's button it asks the hero's slot 99 (`FCanUseGadget`) and checks the gadget's MP
  cost against the current MP; failure plays the error sound, success moves the slot to the gadget's mode (hold or
  one-shot) through `SetGadgetSlotState`, which calls slot 100 (`StartGadget`). While a hold gadget's button stays down
  slot 101 (`UpdateStepplayerGadget`) runs every frame; releasing it, or the hero becoming unable to use it, calls
  slot 102 (`StopStepplayerGadget`).
- The per-gadget description records (mode, owner, price, MP cost, unlock episode, name, description, icon) are the
  `GADGETDESC` table defined in `stepplayer.cpp`. Ownership is a bit per gadget in the save data (`FPowerupAcquired`).
- Each hero decides usability per state: `JT::FCanUseGadget` asks the active component (`FJtairCanUseGadget`,
  `FJtgrabCanUseGadget`, `FJtpipeCanUseGadget`) and refuses during a mode; `CanBentleyUseGadget` and
  Murray's `FCanUseGadget` override have their own rules. The effect itself is in `UseJtGadget`, `StartBentleyGadget` and
  `UseMurrayGadget`; time-based gadgets work through the clock's rate slots (`SetClockRate`), and the timed guard
  override (all guards frozen) is `SetGuardOverride` / `UpdateSwGuardOverride`.
- HP and MP live in the save blob: `BindStepplayerHpMp` points the hurt component's HP and the MP pointer at the
  character's save variables, so health persists across level reloads. `AddStepplayerHpMp` heals, `DrainStepplayerMp`
  spends.

## 10. Invariants and gotchas

- **Change state only through the setter.** `SetJtState`, `SetMurrayState`, `SetBentleyState` and each component's
  `Set*State` run exit/entry actions and swap animations. Writing the state field directly skips the animation swap and
  leaves stale references (hooks, attachments, camera tunnels).
- **Components talk to their owner through virtuals.** A component must not assume it runs inside JT: use the owner's
  generic player state (slot 88) and the STEPPLAYER virtuals. Code that downcasts to JT belongs in `jt.cpp` or is
  guarded by a class test.
- **Intent in Update, motion in the preset.** Velocities written in slot 16 are consumed by the slot-70 preset during
  `SolveSw`. Teleports go through `TranslateToPos` / `TranslateStepplayerToPos` so the step's cached base and the
  broad-phase box follow.
- **`JTS_Max` sizes an array in JT.** JT holds one record per movement state indexed by `jts`; adding a state means
  that array grows and JT's layout changes (fine in a port, not in the matching build).
- **Hurt gates input.** While the hurt component is active (`nState` not -1), `UpdateStepplayer` skips the AI input and
  the walk-to goal.
- **Hands-off means invulnerable** (`IsPoInvulnerable`), and `IsJtgoalHolding` freezes the behaviour stack.
- **Becoming current clears the behaviour stack** (`SetStepplayerCtlState`), so do not keep state in a partner
  behaviour that must survive a character switch.
- Several JT fields, enumerators and STEPPLAYER virtuals are still offset-named or `VUnkNN`; their meaning is
  documented next to the function that uses them. Treat names marked (inferred) in the code as hypotheses.

## 11. How to

**Add a new move for Sly.**
1. Add a `JTS` value (see the gotcha above) and, if it needs a body sub-state, `JTBS` values.
2. In `SetJtState`, add the exit actions when leaving it and the entry actions (animation or SM, sound, channel mask)
   when entering it; add its step parameters to `ApplyJtStateStepParams`.
3. Decide who enters it: a transition in `ChooseJtGroundState` (grounded moves from input), a component (through
   `SetJtState` or the generic player state), or a message in `HandleJtMessage`.
4. Add its per-frame work to the switch in `UpdateJtPre` (input, desired velocity) and, if it animates, to `UpdateJt`.
5. Add its physics to `PresetJtAccel`, and its gadget rules to `JT::FCanUseGadget`.
6. If scripts or other components must request it generically, extend `SetJtPlayerState` / `GetJtPlayerState` and the
   partner overrides.

**Add a new component shared by all heroes.** Put a record in STEPPLAYER with an owner back-pointer, write
`Init*` / post-load / `Set*State` / `UpdateJtActive*` / `PresetJtAccel*`, call them from `InitStepplayer` /
`PostLoadStepplayer`, and from each hero's per-state update and slot-70 preset; switch the owner's state through
slot 88, adding a generic player state if none fits.

**Add a gadget.** Add its description record (mode, MP cost, ...) to the gadget table, accept it in the hero's
`FCanUseGadget` override, implement the effect in the hero's `StartGadget` override (and slots 101/102 for hold
gadgets), and give it an ownership bit so ThiefNet can sell it.

**Debug "the hero does nothing".** Check, in order: is the PO current and its POCTL in control state 1
(`SetPoCtlState`); is the world hands-off or input-locked; is the hurt component active; is a walk-to goal holding; is
a behaviour on the stack writing the POCTL; is the object live (in the world and its root active).

## 12. Where to look

- `src/game/actor/po.cpp`, `step.cpp`: PO, POCTL, STEP.
- `src/game/gameplay/player/stepplayer.cpp`: the shared base, gadgets, HP/MP, interaction, the AI-input hook.
- `src/game/gameplay/player/jt.cpp`, `murray.cpp`, `bentley.cpp`: the three heroes and their state machines.
- `src/game/gameplay/player/air.cpp`, `grab.cpp`, `hurt.cpp`, `hang.cpp`, `pipe_jt.cpp`, `cane.cpp`, `throw.cpp`,
  `mode.cpp`, `places.cpp`, `jtgoal.cpp`, `jthands.cpp`, `dirblend.cpp`: the components.
- `src/game/gameplay/player/grabspot.cpp`, `pipe.cpp`, `rail.cpp`, `hnd.cpp`: grabbable objects.
- `include/sly2/po.h`, `step.h`, `stepplayer.h`, `jt.h`, `players.h`; `src/game/gameplay/player/player_input.h`.
- Every file starts with a header comment listing its entry points; start there.
