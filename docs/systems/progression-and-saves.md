# Progression and saves

How the game tracks what the player has done: missions (JOBs) made of steps (TASKs) linked into a dependency graph,
the game event bus that drives them, the save blob of named variables that holds all persistent progress, the
memory-card manager that writes it, and the dynamic difficulty ("suck") measure. The names JOB and TASK are the
team's; Sucker Punch's own names are not known. Save-variable names, however, are the game's own (they ship in every
world file).

## 1. The main types and globals

- **`JOB`** (`include/sly2/task.h`): an `LO` subclass, one per mission. Holds its start and end tasks, a counter, an
  elapsed time, a completion phase, lists of game-event ids that complete or fail it, visibility and state-machine
  records, an optional completion widget, and references to its save variables (`m<N>_<name>`).
- **`TASK`** (`include/sly2/task.h`): an `LO` subclass, one per mission step. Holds its state and the time it entered
  it, its owning job, prerequisite and successor tasks, the character it is played with (the oid of the playable object:
  Sly, Bentley, Murray or a minigame object), a mode, visibility and state-machine records, and references to its save
  variables (`t<N>_<name>`).
- **The game record** `g_game` (`world/state/game.cpp`): the oids of the current job, task and pending task, the game
  event handler (`HandleGameEvent`), and the startup sequence queue. The world mirrors the pointers
  (`SW::currentJob`, `currentTask`, `pendingTask`; set through `SetSwJobCur`, `SetSwTaskCur`, `SetSwTaskPending`) and
  keeps the list of tasks in the active state (`SW::dlTaskActive`).
- **The save blob** `g_gs` (`include/sly2/save.h`, type `GS`): one flat block of memory holding every persistent
  variable. Its layout is not fixed in code: it comes from the **save-variable schema** loaded from every world file
  into `SW::savevars` (a symbol table of names and a trie of typed entries, `world/state/save.cpp`).
- **The save manager** (`ui/memcard.cpp`, type `SAVE`): three save slots, the memory-card worker thread (`MEMCARD`),
  the card state machine and the dialog widgets.
- **The difficulty object** (`world/state/difficulty.cpp`, type `DIFFICULTY`): handles to the saved difficulty samples
  and a parameter table.

## 2. Tasks: states and transitions

A task's state is one of: unset (only until its post-load restores it), 0 locked, 1 available, 2 complete within the
running job, 3 complete after the job has finished. Two more values are requests, not states: 4 activate and 5
deactivate.

**Changing state**, from the safest entry point down:

- `RequestTaskState(task, state, ...)`: the validated request used by scripts, exits, warps and relays. 4 activates
  (`ActivateTask`), 5 deactivates; 0..3 are accepted only one step up or when the prerequisites are met
  (`AreTaskPrereqsMet`); anything else is rejected (`RejectTaskState`, which sends the reject event to the task and its
  job).
- `ForceTaskState` (and the script-facing `ForceTaskState0..3`): the same without the checks, then `UpdateCurrentJob`.
- `SetTaskStateCascade(task, state)`: the main change. It collects successors whose state must move with it
  (`CollectTaskCascade`: completing a task in one job raises "complete in job" to "complete" across jobs), sets the
  state (`SetTaskStatePropagate`), updates the episode percentage save variable, tells the job when its start or end
  task completes (`OnJobTaskComplete`), and autosaves. A re-entrancy guard makes nested calls no-ops.
- `SetTaskState(task, state)`: the low level. Broadcasts the state-change game events, maintains the active-task list
  and counts, writes the task's save variables (`UpdateTaskSaveVars`), sends the task its state events, and runs the
  follow-up activate or deactivate.
- `SetTaskStatePropagate` then recomputes every successor (`RecomputeTaskState`: a successor is locked until all its
  prerequisites are complete, then available).

**Current and pending task.** `BeginTask` / `EndTask` make a task current or end it (with its music, its "active"
visuals and its activate / deactivate events); `ActivateTask` / `DeactivateTask` wrap them. A task with a warp becomes
the **pending** task: the one to resume after a level change. `GoToTask(task)` switches to the task's job, makes the
task available and current, selects its character and wipes to its level and warp; `StartTaskByOid` does the same by
oid, storing the task as pending and warping when it is not in the loaded level.

**Visibility and state machines.** A task or job carries records that tie world objects to its state:
`ApplyStateLoVisibility` adds an object to the world when the state's bit is in the record's mask and removes it
otherwise (this is how only the available task's trigger spot exists, which fixes the objective order);
`TriggerStateRsmg` drives a state machine to a goal on a state change. Scripts create these records at load
(`AddTaskStateVisibility`, `AddJobStateVisibility`, `AddTaskStateRsmg`, `AddJobStateRsmg`).

**Ordering.** `SortTasksTopologically` (via `VisitTaskTopo` and `CompareTasks`) orders all tasks by prerequisite depth;
the post-load restore walks tasks in this order so that derived states are computed after their prerequisites.

## 3. Jobs

A job's state is derived from its tasks (`GetJobState`: the start task's state while it is below "complete", otherwise
the end task's, at least 1).

- **Becoming current.** When the start task completes, `OnJobTaskComplete` calls `ActivateJob`, which makes the job
  current (`SetCurrentJob` -> `BeginJob`: counts the start, broadcasts its game event, applies active visuals and
  state-machine records, sends the activate event). `UpdateCurrentJob` re-derives the current job from task states
  (the job whose start task is complete and whose end task is not).
- **Completing.** When the end task completes, `CompleteJob` runs the completion sequence if the job has a completion
  widget: `SetJobCompletionPhase` takes control from the player, `UpdateJob` steps through the widget's intro and outro
  labels (the banner stops the game clock through a clock-rate slot). At the end `FinishJob` marks the end task
  complete, broadcasts "job finished", ends the job (`EndJob`) and autosaves; `ContinueAfterJob` then runs the next
  startup-queue step if any, or activates the next task, or takes the level's exit.
- **Failing and restarting.** A job lists game-event ids that complete it and ids that fail it; `HandleJobGameEvent`
  checks them. `FailJob` records the job for abort and wipes back to the current world's entrance; on reload `DeleteSw`
  calls `AbortJob` (reset its tasks, make the start task available, end the job). `RestartJob` goes to the pending
  task or the start task. Death is reported as a game event that the job's scripts and `HandleJobGameEvent` turn into a
  restart.
- `IsCurrentJob`, `IsJobComplete`, `IsCurrentTask`, `IsTaskComplete` are exposed to scripts as options.

## 4. The game event bus

`BroadcastGameEvent(&g_game, id, arg)` delivers an integer event, in order, to:

1. the game record (`HandleGameEvent`): world teardown (1) saves widget state; reset (3) applies pending save data and
   reloads the save slots; post-load (4) restores tasks (`RestoreTasksAfterLoad`); transition done (5) re-resolves the
   current job, task and pending task from the saved oids and announces the job;
2. the difficulty object (`HandleDifficultyMessage`, section 7);
3. every task on the active list that belongs to the current job (`SendTaskGameEvent`);
4. the current job (`HandleJobGameEvent`).

Senders include the level load (events 0..5, see [world-and-levels.md](world-and-levels.md)), job and task state
changes, the hero's death, guards being damaged or defeated, pause, gadget use, and every LO trigger / untrigger
(`HandleLoSpliceEvent`). The ids are integers without symbolic names in the code. Known ids: 0/1/2/4/5 startup /
teardown / load begin / post-load / transition done; 16 hero died (jobs restart); 21 guard damaged; 23/24 guard
defeated; 0x20 job finished; 0x28/0x2C pause shown/hidden; 0x3C gadget used.

**How tasks and jobs hear about it.** The code comments call these "messages", but `SendTaskGameEvent`,
`HandleJobGameEvent`, `BeginTask`, `EndTask` and friends call vtable slot 3, `HandleSpliceEvent`: they are **Splice
events** handled by the level scripts. The game event is Splice event 17; activate and deactivate are 19 and 20;
reject, state set and timeout use neighbouring ids. A job's script typically handles event 17 to restart on death and
to advance its tasks; a task's script handles 19 to fire relays, show hints and request the next states. See
[splice-scripting.md](splice-scripting.md).

## 5. Save variables

**The schema.** `LoadSaveSchemaFromBrx` reads, from the head of every world file, the symbol table (names and symbol
ids, hashed by `HashSaveSymbolName` into `SVSYMTAB`) and the trie of entries (each a symbol, a kind, a bit count or
array length, a byte offset into the blob, and child / sibling links). Kinds are integer, float, packed boolean bits,
object id and a few vector-like kinds. The same symbol ids double as Splice symbols, which is why save variables and
script variables share names. If the loaded schema's version or checksum differs from the one stored with the blob,
`ResetGsIfSchemaChanged` wipes the blob.

**Keys.** A variable is named by a path of symbols and array indices, for example `loot.ccoin`,
`episode.<world>.level.<name>.afClue`, `<task>.state`. In code a path becomes an `SVKEY` (`BuildSaveKeyV`, `NewSaveKey`,
`AllocSaveKey`, `ConcatSaveKeys`); `FindSaveVar` walks the trie.

**Access.**

- From native code: `GetSaveVarByName("name", ..., NULL)` / `SetSaveVarByName(value, "name", ..., NULL)` read and write a
  variable of the global blob as a Splice value; `GetSaveVarAddressV` returns its address for direct access. The
  non-V forms take a schema, a key and a blob.
- Hot variables are resolved once per level: `ResolveSwSaveVars` (`world/scene/sw.cpp`) caches handles (`SVPATH`) in
  `SW` for the coin count, play time, current world and episode, the settings keys, and per-world play time and
  completion; it also raises `episodeBest` when the current world is a new best.
- From scripts: the save-variable builtins with a list of symbols ([splice-scripting.md](splice-scripting.md)).
- Engine state bound directly to save variables: character health and gadget power point into the blob
  (`<char>.hpCur`, `mpCur`), the owned gadgets are bits of `powerUps.mppukfAcquired`, the current character is
  `oidPoCurrent`.

**Jobs and tasks in the blob.** Each job and task has a record of save variables (`focus_count`, `is_complete`,
`state`, time, the job's `suck_value`, the task's `from_memcard`, ...; list in
[formats/save-variables.md](../formats/save-variables.md)). `UpdateTaskSaveVars` and the job's equivalent write them on
state changes; `RestoreTaskState` and `PostJobLoad` read them back after a load. The world itself keeps nothing, so
**reloading a level is how the game resets everything except progress**.

**Resets and settings.** `ResetSaveGs(blob, fKeepSettings)` clears the blob, optionally keeping the settings
variables (language, sound mode and volumes, headset, screen offset, vibration, camera and aim inversion, the autosave
warning flag); `ApplyGsSettings` pushes those settings into the engine. `InitGame` resets the game record, the blob and
the difficulty for a new game.

**Completion.** `GetCompletionPercent` (save menu: a weighted sum of per-world progress variables plus vault bonuses)
and `GetEpisodePercentComplete` (the episode menu: complete tasks over tasks that have successors, so a job counts only
after its banner finishes) are separate measures.

## 6. Memory card

The save manager in `ui/memcard.cpp` is initialised by the boot stage `StartupMemcard` (`InitSave`, `InitMemcard`).

- **Worker thread.** Card operations run on a worker thread: `CreateMemcardThread` queues one job (a pointer to a
  `SAVE` member function such as `McJobGetInfo`, `McJobFormat`, `McJobCreateSave`, `McJobSaveSlot`, `McJobAutosave`)
  and `MemcardThreadMain` runs it. Each job calls the Sony `sceMc*` functions through small synchronous wrappers
  (`McOpen`, `McWrite`, `McReadFile`, `McWriteFile`, ...).
- **State machine.** `UpdateSave`, called from every world update, polls the card and advances the save state
  (`SetSaveState`); it is skipped during transitions or while the worker is busy, and it debounces autosave requests.
- **Saving.** `AutosaveGame(force)` (called by `FinishJob` and task completions) does nothing during a transition or
  without a current slot; otherwise it starts `StartAutosaveJob`, or shows the "no card" dialog. The slot's data is the
  blob with every task's `from_memcard` flag set (`BuildMemcardGsMask`), followed by the schema fingerprint; there is no
  checksum of the data, no compression and no encryption. Files live in one save directory with an `icon.sys` and an
  icon copied from the disc (`CreateSaveDir`, `WriteIconSys`, `BuildSaveFilePath`).
- **Loading.** The front-end save pages (`StartNewGameInSlot`, `LoadGameFromSlot`, `SelectSaveSlotToSave`) set the
  current slot; loading resets the blob and starts the game from the slot (`StartGameFromSave`): a new
  game goes to the first task, a saved game to its episode (or the hub when the episode is complete). Slot data reaches the
  blob through `ApplyPendingSaveData`, which the game record runs on the reset event (inferred sequence).
- **Dialogs.** `CMemcardDialog` and its helpers (`SetMcDialogState`, `CMemcardDialog_Update`, `OnMcDialogChoice`) show
  card errors and confirmations; `GetMcErrorMessageId` maps card error codes to message ids.

## 7. Difficulty: the "suck" measure

Sucker Punch's term (the save variables are `suck_history.mission_0..2` and each job's `suck_value`) measures how much
the player is struggling; higher presumably means easier play (inferred from its consumers). In `difficulty.cpp`:

- `GetDifficulty` returns the current job's value while a job is current, otherwise the average of the three history
  samples.
- `HandleDifficultyMessage` reacts to game events: the reset event binds the three history keys; the "job ended" event
  shifts the history window and stores the job's value as the newest sample; the hero's death and one other event add
  one delta from the parameter table, and a task of the current job completing adds another (the field names of the
  parameter table are inferred: deaths raise the value, progress lowers it).
- `ChangeDifficulty` adds a delta; `SetDifficulty` clamps to the object's range and stores into the current sample;
  `UpdateGameTimers` eases it when a job takes long.
- Consumers: `ScaleByDifficulty` (interpolates a gameplay value between two ends), `SprayDifficultyPrizes` (more prizes
  from breakables when struggling), the animation playback speed of segments authored with a difficulty curve
  (`engine/anim/aseg.cpp`), and the `get-difficulty` builtin for scripts.

## 8. Collectibles and other progress

Progress that is not a task lives in save variables written by gameplay code: coins (`loot.ccoin`, read and written
through `GetCoinCount` / `SetCoinCount`), loot counts, clue bottles and vaults per level, owned gadgets, booty. The code
is in `src/game/gameplay/progression/` (`prize.cpp` for coins, clues, vaults and prize sprays, with the classes in
`include/sly2/prize.h`; `booty.cpp`;
`cheat.cpp`; `waypoint.cpp`) and ThiefNet purchases in the front-end screens.

## 9. How it connects

- **Object model**: JOB and TASK are ordinary LOs loaded from world files (they bypass the load filter, so every level
  has the whole graph). [object-model.md](object-model.md).
- **World**: load events drive restore; `FailJob`, `GoToTask` and `StartTaskByOid` request transitions; the world caches
  save handles. [world-and-levels.md](world-and-levels.md).
- **Splice**: level scripts configure jobs and tasks and react to their events; scripts read and write save variables.
  [splice-scripting.md](splice-scripting.md).
- **Cutscenes**: relays set task states, hand the controller to other characters and complete tasks themselves
  (`gameplay/cutscene/relay.cpp`).
- **HUD**: the objective counter recounts complete tasks every frame; the job banner is the completion widget.

## 10. Invariants and gotchas

- The save blob's layout is data, not code: never hard-code blob offsets in new code; resolve variables by name (once
  per level if hot), because a different schema moves everything.
- A schema change wipes the blob on the next load (`ResetGsIfSchemaChanged`). A mod that adds save variables must
  account for that with existing saves.
- Task state writes are not idempotent: `SetTaskStateCascade` broadcasts events, autosaves and can complete a job. Use
  `RequestTaskState` unless you know the validation must be skipped.
- Re-entrancy: a state change triggered from inside another state change is dropped by the cascade guard.
- "Complete" has two values (2 within the running job, 3 after it finished); percentages and successors treat them
  differently.
- Game events reach only the current job and its active tasks; a task of another job does not hear them.
- The "message" vocabulary in `game.cpp` comments means Splice events (slot 3), not `SendLoMessage` messages.
- There are no checkpoint records: retrying a section reloads the level and restores tasks from the blob.

## 11. Recipes

**Inspect progress at run time.** Read the blob through the schema names in
[formats/save-variables.md](../formats/save-variables.md); task states are `<task>.state`, the current character
`oidPoCurrent`, coins `loot.ccoin`.

**Complete or unlock a task from code.** Find the task (`FindSwObject` with its oid) and call `RequestTaskState(task,
2, ...)` to complete it, or 1 to make it available; use `ForceTaskState` only for debugging tools.

**Jump to a mission.** `GoToTask(task)` if the task is in the loaded level, otherwise `StartTaskByOid`, which warps.

**Add a persistent value (port or mod build).** Add the variable to the schema in the world files (every file carries
the schema) and access it by name with `GetSaveVarByName` / `SetSaveVarByName`; existing saves are wiped by the schema
check unless that check is adapted.

**React to mission progress natively.** Hook `HandleGameEvent` (game-wide) or the job and task functions above; for
level-specific behaviour, prefer a script handler on the task.

## 12. Where to look

- `src/game/world/state/game.cpp`: the game record, the event bus, JOB and TASK, task state changes, `GoToTask`,
  start-from-save.
- `src/game/world/state/save.cpp`: the schema, keys, typed access, resets, settings, completion percentage.
- `src/game/world/state/difficulty.cpp`: the suck measure.
- `src/game/ui/memcard.cpp`: the save manager, worker thread, card jobs, dialogs, save pages.
- `src/game/world/scene/sw.cpp`: `ResolveSwSaveVars`.
- `src/game/gameplay/progression/`: prizes, booty, cheats, waypoints.
- `include/sly2/task.h`, `save.h`, `sw.h`, `game.h` (the game-progress record `g_game`), `prize.h`.
- [formats/save-variables.md](../formats/save-variables.md) and [engine.md](../engine.md) sections 7.12-7.13.
