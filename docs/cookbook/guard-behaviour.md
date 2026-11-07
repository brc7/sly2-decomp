# Recipe: add a guard behaviour

**Goal.** Make guards do something new: a new reaction (taunt, call for help), a new movement pattern, a new mode
the guard's brain can switch to, or a new step in a patrol script.

**Systems.** [ai-guards.md](../systems/ai-guards.md) (read sections 3, 4, 6, 10 and 11 first),
[object-model.md](../systems/object-model.md) (class ids), [animation.md](../systems/animation.md) (clips and state
machines a behaviour plays), [physics-and-collision.md](../systems/physics-and-collision.md) section 8 (navigation).
Mod build: [README.md](README.md#how-a-mod-is-built).

## Choose the smallest change

| You want | Change | Code? |
|---|---|---|
| a different patrol, pauses, animations on a path | the guard's patrol script (`AddGuardScript*` options in the level's Splice) | no |
| a different clip for an existing situation | an animation binding for that guard-flag set (`SetStepguardAnimBinding` options) | no |
| a new step a patrol script can take | a new guard-script command (below, "Variant: a script command") | yes |
| a new reaction or sub-task inside an existing mode | a new BHV class pushed from that mode's state switch | yes |
| a new top-level state of the guard's brain | a new BHV class plus a new `GUARD_MODE_*` | yes |

## What to touch

| Piece | Where |
|---|---|
| behaviour class (BHV subclass) | declaration in `include/sly2/classes.h` (where `BHVIDLE`, `BHVANIM` and the others are) or a view in your `.cpp`; code in `gameplay/ai/guardbhv.cpp` or a new file |
| placement factory | `ConstructBhv` in `gameplay/ai/bhvstk.cpp`: one `case` per behaviour class id |
| slot size | `GetBhvSlotSize` in `bhvstk.cpp` (every behaviour must fit) |
| registry | factory, descriptor getter and descriptor row in `app/class_registry.cpp` ([new-object-class.md](new-object-class.md)) |
| guard flag (optional) | the SGF bit the behaviour raises; selects animation bindings |
| entry point | `SetBhvguardMode` / `UpdateBhvguard` / `ResumeBhvguard` (`guardbhv.cpp`, `GUARD_MODE_*` enum at its top), or another behaviour's `Set*State` switch, or `ExecuteBhvScriptCommand` |

Behaviours are not level objects: they are `BASIC`-derived and constructed **in place** in a preallocated slot of the
guard's stack, not through `CreateLo`. `PushBhvstk` clears the slot, calls `ConstructBhv` (which clears the class's
size and installs its vtable), links the behaviour to its owner and the one below, and calls slot 7 (OnPush). The
caller then sets fields and calls slot 8 (Start) itself.

```cpp
// guardbhv.cpp, SetBhvguardMode: the push-configure-Start pattern every entry point follows
o = PushStepBehavior(behavior->guard, CID_BHVCOMBAT);
o->combatTarget = m->u.s.arg;
StartPushedBhv(o);
```

## Steps

1. **Class id.** Behaviour classes are the last subtree of the class table (`BHV` and its descendants), so a new
   behaviour can be **appended**: give it the next id after the last class, extend the end-of-subtree id of `BHV` and
   of `BASIC` to include it, and grow the tables sized by the class count (new-object-class.md step 4). This keeps
   `IsBasicDerivedFrom(bhv, BHV's id)` and `FindStepBehavior` working. If the new class derives from another behaviour
   (for example `BHVANIM`, to play a bound animation), that parent's range does not include an appended id; check
   the code that tests for that parent with `IsBasicDerivedFrom` or `FindStepBehavior` and decide whether your
   behaviour should match.

2. **Declare it.** Derive from `BHV` for a pure controller, or from `BHVANIM` when it plays one bound animation (the
   way `BHVIDLE` and `BHVSTRIKE` do). Override only the slots you need; the `BHV` defaults are empty:
   7 OnPush, 8 Start, 9 Close, 10 Update, 11 UpdateMotion, 12 ground-probe flags, 13 HandleMessage, 14 steering hook
   (heroes only). `BHVIDLE` is the minimal model: its OnPush calls the `BHVANIM` OnPush and sets its guard flag.

3. **Fit the slot.** The instance must fit in `GetBhvSlotSize()` bytes. If it does not, raise that size; the slots
   are allocated from the world heap at load (`PostBhvstkLoad`), so a larger size only costs memory.

4. **Construct it.** Add a `case` to `ConstructBhv` that clears the class's size and installs its vtable, following
   the existing cases (some also initialise embedded counted references; copy that if your class has any).

5. **Implement the slots.**
   - OnPush: defaults (state -1), and raise the behaviour's guard flag if it has one (`AddStepguardSgf`).
   - Start: enter the first state; call `SetBhvBusy` if nothing below may re-decide until you finish.
   - Close: pop your child (`PopBhv` on the stored child slot), `ClearBhvBusy` if you set it, lower your flag
     (`RemoveStepguardSgf`). Close runs on every exit, including when a parent pops you.
   - Update: advance your state machine; test the child's completion (`IsBhvanimDone` and the like).
   - HandleMessage: return nonzero only for messages you consume, so damage still reaches `BHVGUARD`.
   - UpdateMotion: only if the behaviour drives the guard's movement; take the mover with
     `SetStepguardMoverDriver`.

6. **State switch.** Write a `Set<Name>State` that pops the current child and pushes the child behaviour for the new
   state (`BHVANIM` with a flag for an animation, `BHVGOTO` to walk somewhere, `BHVWALKPATH` to follow a path), always
   push, set fields, Start.

7. **Entry point.**
   - *A new mode:* add a `GUARD_MODE_*` value, a case in `SetBhvguardMode` that pushes your behaviour, the
     transitions into it in `UpdateBhvguard`, and what happens when it ends (`ResumeBhvguard` picks the mode to
     return to). Mode records are compared with `IsBhvguardModeSame` before switching, so a repeated request is a
     no-op.
   - *A sub-behaviour:* push it from the owning behaviour's state switch.

8. **Animation.** If it needs its own clip, give it a new guard flag and have the level bind an animation to a flag set
   containing it; `FindBestAnimBinding` picks the most specific binding, so guards without the binding fall back to
   their generic clip.

## Variant: a script command

Patrol and scripted segments are guard scripts (`SSG`), lists of commands built from Splice options and executed by
the script behaviour (`gameplay/ai/tu_1709A8.cpp`). To add a command:

1. Add a kind to the `SSGK_*` enum and an `AddGuardScript<Name>` function that appends a command of that kind
   (`AddGuardScriptCommand`) and fills its arguments.
2. Handle the kind in `ExecuteBhvScriptCommand`: do it immediately, or push a child behaviour and let
   `UpdateBhvScript` wait for it (the pattern of the wait, goto and animation commands).
3. Expose `AddGuardScript<Name>` to Splice as a method option of the guard-script class
   ([splice-command.md](splice-command.md), "Add an object option").

## Gotchas

- **The match.** `ConstructBhv`, `GetBhvSlotSize`, the descriptor table and the vtables are original code and data;
  build as a mod.
- **Flag word = union of the stack's flags.** Raise flags only in OnPush/Start and lower them in Close. Setting
  guard flags from outside a behaviour desynchronises animation choice and pickpocketing (ai-guards.md section 10).
- **Busy must clear on every path.** A behaviour that sets busy and is popped without clearing it freezes the guard in
  its mode; put `ClearBhvBusy` in Close.
- **Slots are reused in place.** After a pop, a pointer to a behaviour above you points at whatever is pushed next.
- **Depth is bounded** by the fixed slot array; mode -> goto -> walk path -> anim already uses much of it. Keep
  delegation shallow.
- **Per-file class-id enums.** Each AI file has its own `CID_BHV*` enum copy; add your id to every file that pushes or
  tests it.
- **Guards stream out.** The guard manager (`UpdateGuardManager`) resets the root behaviour of guards that left the
  world; your behaviour must tolerate being popped at any time.

## How to test

- In a mod build, force the new mode on a guard in a quiet spot (from a debug hook, `SetBhvguardMode` with your
  mode record) and watch: the behaviour starts, plays its animation, and returns through `ResumeBhvguard`.
- Hit the guard during the behaviour: damage must still reach `BHVGUARD` (your HandleMessage returns 0 for zap
  messages) and the guard must react.
- Walk away until the guard streams out and back in: it must restart cleanly.
- Without a mod build, the patrol-script and animation-binding routes (first table) can be tried by editing the
  level's Splice data, once a world-file writer exists; reading `UpdateBhvguard` tells you when each mode is entered.
