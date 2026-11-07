# Recipe: add a level script event, a mission step or a scripted sequence

**Goal.** Make a level react to something new: an object that runs script when triggered, a new step (TASK) in a
mission (JOB), a native event that missions can complete or fail on, or a cutscene-style sequence (RELAY).

**Systems.** [progression-and-saves.md](../systems/progression-and-saves.md) (sections 2-4 and 10 first),
[splice-scripting.md](../systems/splice-scripting.md) section 6 (events),
[cutscenes-and-sequences.md](../systems/cutscenes-and-sequences.md) (RELAY and JSG),
[world-and-levels.md](../systems/world-and-levels.md) (load order, deferred work). Mod build:
[README.md](README.md#how-a-mod-is-built).

## The pieces and how they talk

- **Objects run script through Splice events.** An object whose frame binds an event symbol gets
  `(event self args...)` called when native code sends that event through slot 3. Buttons, exits, warps and volumes
  send trigger (5) and untrigger (6); `PostLoLoad` sends post-load (0); `UpdateLo` sends update (1) when the object's
  next update time comes.
- **Missions are JOB and TASK objects** (`include/sly2/task.h`, both `LO` subclasses) placed in the level, configured
  by options at load (prerequisites, successors, the character, visibility and state-machine records) and driven by
  their own scripts. Task state changes go through `RequestTaskState` (validated) and `SetTaskStateCascade`.
- **The game event bus** (`BroadcastGameEvent`) delivers an integer event to the game record, the difficulty
  object, the active tasks of the current job and the current job. Tasks and jobs receive it as Splice event 17; a
  job also checks its lists of completing and failing event ids (`HandleJobGameEvent`).
- **RELAYs** are command lists built by the level's Splice at load (`AddRelay*` builders) and fired by scripts or
  objects (`FireRelay`); each running block is a JSG stepping through commands until one is not complete.

Most level logic is data: scripts, task records and relay command lists in the world file. Code changes are needed
only for a new kind of event, command or rule.

## Recipe A: run script when something happens (data only)

1. Pick the sender: a `BUTTON` or `VOLBTN` (trigger when pressed or entered), an `EXIT`/`WARP`, a volume, a task
   state change (events 19 / 20 on the task), a game event (17 on tasks and jobs).
2. In the object's script entry, bind the event: `(define <event> (lambda (self args...) ...))`. Chain the previous
   binding if the entry is shared (`find-binding-or-noop`).
3. Do the work with builtins and options: find objects (`find-object` and friends), set options (`seto!`), request task
   states, fire relays, start sounds.

Remember that objects with the same script index share the frame, and that an unposted event runs inside the native
code that sent it.

## Recipe B: add a mission step (TASK)

1. **Place a TASK** in the level with its job, its prerequisite tasks and successors, the character it is played as,
   and its save variables (task records are options; formats/level-objects.md and the decoded task records of an
   existing level are the template).
2. **Gate the world.** Use visibility records (`AddTaskStateVisibility`) so the step's trigger objects exist only in
   the right task states; that is how the game enforces objective order.
3. **Script it.** Handle event 19 (activate) to start the step (fire a relay, show a hint, set the camera), and
   request the next state when its goal is met (`RequestTaskState` with "complete" from a trigger handler; a trigger grab
   spot can request it directly, `FireTriggerGrabspot`).
4. **Persist.** Task state is written to its save variables by `SetTaskState`; a new task needs its variables in the
   save schema (progression-and-saves.md section 11: changing the schema wipes existing saves unless the check is
   adapted).
5. **Restore.** After a load, `RestoreTasksAfterLoad` re-derives states in topological order
   (`SortTasksTopologically`); prerequisites must form a DAG.

## Recipe C: a new native game event (code)

Use this when a native condition (a guard state, a gadget, a physics event) should be visible to mission scripts.

1. Choose an integer id not used by any sender (the known ids are listed in progression-and-saves.md section 4; the
   ids have no symbolic names, so search for `BroadcastGameEvent` callers to see the used ones).
2. Call `BroadcastGameEvent(&g_game, id, arg)` where the condition happens. It runs synchronously through the game
   record, difficulty, tasks and job; if your caller is iterating world lists, post the broadcast as a world callback
   (`PostSwCallback`) instead.
3. Scripts receive it as event 17 on the active tasks and the current job; a job can also list the id among its
   completing or failing events.

## Recipe D: a scripted sequence

**With existing commands (data only).** A RELAY's script entry calls the `AddRelay*` builders at load to append
commands: blocks (`AddRelayBlockBegin` / `AddRelayBlockEnd`), waits (`AddRelayWaitTimer`, `AddRelayWaitLine`,
`AddRelayWaitBlocks`), actor commands (`AddRelayWalkTo`, `AddRelayTeleportTo`, `AddRelayPlayAseg`), camera
(`AddRelayCutCamera`, `AddRelayCamBlendTo`), sound and dialogue (`AddRelaySound`, `AddRelayLine`, `AddRelayStartMusic`),
widgets, hands-off (`AddRelayHandsOff`) and task states (`AddRelaySetTaskState`). Fire it with `FireRelay` from a task's
activate handler or a trigger.

**A new command kind (code).**

1. Add a value to the `RELAYCK` enum (`include/sly2/basic.h`). Append; the enum values are stored in the commands
   built at load, not in the world file (the builders write them), so appending is enough.
2. Write a builder `AddRelay<Name>(relay, args...)` that calls `AddRelayCommand(relay, RELAYCK_<Name>, fAsync)` and
   fills the command's argument block (the `RELAYCMD` union in `relay.cpp`).
3. Execute it in `NextJsgCommand` (`relay.cpp`): the switch first resolves the command's actor oid for the kinds that
   have one, then performs the command.
4. If it takes time, add a case to `IsJsgCommandComplete` returning nonzero when done; the JSG stays on the command
   until then. Commands that are not async complete immediately.
5. Expose the builder to scripts as a method option of `RELAY` ([splice-command.md](splice-command.md)), like
   `RefThunk_AddRelayButtonPrompt`.
6. Release anything the command holds in the JSG's cleanup (`RemoveJsg` releases everything a script holds; add yours
   there if it holds references, hands-off counts or locks).

## Gotchas

- **The match.** Recipes C and D's code half change original code and data; recipes A and B and D's data half change
  only level data (which needs a world-file writer, not in the repository yet).
- **Counters must balance.** Hands-off, no-pause, letterbox and input-lock are counters (world-and-levels.md
  section 7). A relay or handler that increments one must decrement it on every path, including when the relay is
  stopped early (`StopRelay`) or the level reloads.
- **Cutscenes complete tasks themselves.** A task's script usually fires a relay whose last command sets the next task
  state; if you stop the relay early, nothing completes the task.
- **Death reloads the level.** Everything not in save variables is lost; there are no checkpoints other than tasks.
- **Re-entrancy.** `SetTaskStateCascade` ignores nested calls; a handler that requests another state from inside a
  state change has no effect. Post it instead.
- **Task order is data.** Prerequisites and visibility records, not code, decide what the player can do next.

## How to test

- Read the decoded script and task records of the level first; most "why didn't it fire" problems are a missing
  binding, a task not in the state the handler expects, or a visibility record hiding the trigger.
- Inspect task states live: they are save variables (`<task>.state`, formats/save-variables.md) readable over PINE.
- In a mod build, broadcast your game event from a debug hook and watch the job react; for a relay command, fire a
  relay that uses it in a quiet spot, then stop it early and confirm counters return to zero.
