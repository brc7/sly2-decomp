# The world and levels

`SW` ("the world") is the single object that owns a loaded level: every object, every lookup list, the pools the
subsystems allocate from, the transition request and a cache of save-variable handles. This guide covers the program's
frame loop, the per-frame world update and its order, how a level is chosen, loaded, activated and torn down, and how
warps, exits and wipes request a level change. The objects inside the world are described in
[object-model.md](object-model.md).

## 1. The main types and globals

- **`SW`** (`include/sly2/sw.h`, global `g_sw`, defined in `world/scene/sw.cpp`). A `BASIC` child, not an `LO`: it has a
  Splice frame and options but no oid or parent. It is re-initialised in place for every level. Field groups by name:
  - object structure: `dlChild` (top-level objects), `roots` (all hierarchy roots), `busyRoots` (active roots), the oid
    hash `objectsByOid`, the proxy-record hash `aslPxrOid`, one list per class `objectsByClass`, the per-frame update table
    (`aloupd` and its counts), and many per-class lists (`dlTarget`, `dlStepguard`, `dlStimulus`, ...);
  - pools (`SLOTHEAP` members): root records, callbacks, Splice event callbacks, physics trees and contacts, animation
    applications, state machines, stimuli, relay interpreters, and more;
  - loading state: the world-entry table (`awe`, count `cwe`), the proxy sources (`aproxysrc`), the "new objects" array
    filled by `CreateLo` during a load, the load filter;
  - progression: the save-variable store `savevars`, cached save-variable handles (`psvp...` fields such as the coin
    count and the settings keys), the current job, task and pending task (`currentJob`, `currentTask`,
    `pendingTask`), the active task list;
  - the transition request: `isTransitionPending`, `grfTransition`, `destinationLevel`, `oidWarpDest`, `oidWarpContext`, and
    the current level's file record `currentLevel`;
  - players: `playerObjects` (the playable objects of the level), `currentPlayer` (the one with control), `overridePlayer`;
  - control counters: `handsOffCount`, `noPauseCount`, letterbox and input-lock counts;
  - lighting (illumination HSV, midtone, light scale) and the HUD stacks.
- **`FILEREC`**: the 64-byte, XOR-obfuscated record that names a world file on the disc
  ([formats/disc-records.md](../formats/disc-records.md)). Levels are identified by their file record; there are no
  file names.
- **`TRANS`** (`level/wipe.cpp`): a transition request: flags, target `FILEREC`, warp oid and one more argument.
- **World entry** (`WE`, the `awe` array): one block of the world file with its objects, Splice table, textures and copy
  count ([formats/world-file.md](../formats/world-file.md)).
- **Level table**: per level its file record, world, episode and entry flag (`LoadLevelTableFromBrx`); queries
  `GetCurrentWorld`, the current level and episode getters, `GetWorldEntry`, the level-index lookups (`world/scene/sw.cpp`).
  Worlds group levels into episodes (Paris, India, Prague, Canada, ...); a world's "entry" record is its hub level.
- Other singletons the frame loop drives: `g_cm` (camera), the clock `g_clock` (`core/time/clock.cpp`), the game
  record `g_game` (`world/state/game.cpp`), the wipe (`level/wipe.cpp`), the screen list (pause menu and other screens,
  `engine/presentation/screen.cpp`), the sound manager, the memory-card manager.

## 2. Boot and the frame loop

`_start` calls `GameMain` (`app/main.cpp`): the C++ global constructors (`__main`, which also constructs `g_sw`), then
`Startup`, which runs the boot stage table (DMA, threads, IOP modules, GS, memory, CD, font, pad, render thread,
loading screen, clock, VU microcode, sound, HUD packets, memory card, MPEG) under a progress bar, then `StartupGame`
(`world/state/game.cpp`): language from the console, game event 0, and `BeginStartupSequence`.

**The startup step queue** (`app/tu_1A4E08.cpp`) is a short list of front-end steps run one after another: play a
movie, warp to a level, start a task, enter the memory-card UI. At boot it holds the intro movie, the memory-card
check and the warp to the first level. `RunNextStartupStep` runs one step; a movie step warps to an empty level (a
transition with no file) that plays it, and the end of that empty load runs the next step. The game also queues steps
after a job (`ContinueAfterJob`).

**One iteration of the frame loop**, in order:

1. `ExecuteSwTransition(&g_sw)`: does nothing unless a transition is pending (section 4).
2. `UpdateJoy`: read the pads.
3. `UpdateCd`: disc service.
4. `SetupCm`: empty in Sly 2 (kept from Sly 1).
5. `MarkClockTick`: compute this frame's `dt` (clamped) and the scaled game time.
6. `UpdateSw(&g_sw)`: the world update (below).
7. `UpdateScreens`: pause menu, menus, memory-card dialogs; this is also where the wipe advances (`UpdateWipe`).
8. `OpenFrame`, then (unless a frame-capture transition says otherwise) `RenderSw`, `DrawSw`, `DrawUi`; optionally
   `DrawFrameCaptureScreen`; `CloseFrame` hands the frame to the render thread.
9. The global frame counter increments.

**The world update** is `SW::Update` (`level/update.cpp`, called as `UpdateSw`):

1. `ProcessSwCallbacks`: run queued world callbacks, then due scheduled Splice callbacks.
2. `UpdateSwBusyList`: incremental activation by distance (fires `OnAdd`/`OnRemove`, see
   [object-model.md](object-model.md)).
3. If the game clock is stopped (`dt` is zero: pause, job banner...), only `UpdateSwObjects` runs: the update-table
   entries flagged to update while paused, plus the animation applications flagged likewise.
4. Otherwise, in order: every controller (`UpdatePoctl`); zap-history expiry; slot 16 `Update` on every update-table
   entry that asks for updates; the guard override pass; every running animation application (`UpdateAsega`); noise
   stimulus expiry; `ProcessSwCallbacks`; physics (`SolveSw`); pending path obstacles; shader animation
   (`UpdateShaders`); `ProcessSwCallbacks` again.
5. Always: the memory-card state machine (`UpdateSave`), the sound manager (`UpdateSndmgr`), the final camera update
   (`UpdateCmLast`, only when the camera clock advanced, so the camera keeps working under slow motion and pause), the
   timer HUD, and the Splice collector's recycle cursor.

Because callbacks run at the start and twice after object updates, work posted from an `Update` usually runs in the
same frame, after physics or before it depending on when it was posted.

## 3. Level lifecycle overview

```
request:   WipeToWorldWarp / SetSwTransition  -> g_sw.isTransitionPending, request fields
next frame: ExecuteSwTransition
             open file stream (LZSS) -> DeleteSw (old world) -> memset + SW_ctor (fresh g_sw)
             -> InitSwLoadBuffers -> LoadSwFromBrx -> PostSwLoad
             -> pick the current player, trigger the arrival warp, cut the camera
             -> game event 5 -> resume the wipe -> (empty level) next startup step
frames:    UpdateSw ... objects activate by distance, scripts run
```

## 4. Requesting a level change

Everything ends in **`SetSwTransition(world, TRANS*)`**, which copies the request into `SW` and sets
`isTransitionPending`. The request is consumed at the top of the next frame. (In `sw.cpp` the function body still
carries a Sly 1 "checkpoint manager" view name; the symbol and behaviour are `SetSwTransition`. Sly 2 has no checkpoint
record.)

- **`WipeToWorldWarp(world, filerec, warpOid, arg, flags, wipeWidgetOid)`** (`level/wipe.cpp`) is the common entry
  point: it builds a `TRANS` for the target level (the current level when `filerec` is null), marks "same level" when
  the target is the current file, and either starts a wipe or calls `SetSwTransition` directly. Callers include exits,
  job failure (`FailJob`), going to a task in another level (`GoToTask`, `StartTaskByOid`), cheats, the pause menu and
  the startup queue.
- **The wipe** (one global `WIPE`): states idle, wiping out, wiping in, transition pending. `ActivateWipe` stores the
  request and plays the wipe widget's "out" animation; `UpdateWipe` moves to "pending" when it ends, which hands the
  stored request to `SetSwTransition`; after the load `ResumeWipeAfterLoad` plays the "in" animation. A request made
  while a wipe-out or a pending transition is in progress is ignored; `IsSwTransitionBusy` reports that state.
- **EXIT** (`level/exit_warp.cpp`, a volume): `UpdateExit` runs a small state machine (player inside, left, off, armed,
  warping). Entering an armed exit calls `TakeExit`: apply the exit's task states, take control away (hands-off), send
  the Splice trigger event, then when the wipe time arrives queue the exit's movies and wipe to its target world.
- **WARP** (`level/exit_warp.cpp`, a point): where the player lands. After a load, `ExecuteSwTransition` finds the
  requested warp (`FindWarpByOid`, which also matches a warp's alias oids; well-known aliases exist for the default
  entrance) and calls `TriggerWarp`: move paired objects to their targets, trigger its state-machine records, apply its
  task states, re-arm the exits, cut the camera, send the trigger event. `PutHeroAtWarp` places the hero on a warp
  without a load (also callable from scripts and relays).
- **Death and job failure** reload the current level through these same paths (the job system restarts the job;
  `FailJob` wipes back to the current world's entrance). Persistent state survives because it lives in the save blob,
  not in the world.

## 5. Loading a level: `ExecuteSwTransition`

In order (`world/scene/sw.cpp`):

1. `FlushFrames` (let the render thread drain), open a stack-allocation scope, enter the loading memory phase.
2. Decode the requested file record and **open the new world file first**, as a buffered CD stream with LZSS
   decompression through the scratchpad (`CStreamBis`, `SetDecompress`). If the file cannot be opened, the request is
   dropped and the old world stays.
3. `DeleteSw`: game event 1, abort the job that `FailJob` recorded (if any), reset screens, release the world's Splice frame
   (`DeleteSwSplice`), clear difficulty references, reset camera policies, the sound manager and shader GS state, stop
   rumble, clear the global object pointers, and free the world heap (`FreeSw`).
4. `ResetClock`, block player input, clear the whole `SW` and run `SW_ctor` on it, reset the camera (`InitCm`), store
   the request and the new current file record.
5. `InitSwLoadBuffers`: work buffers, render targets, game event 2.
6. `LoadSwFromBrx(world, stream)` (`level/brx_sw.cpp`), which shows the loading screen and reads, in order: the
   world-entry table; the sound section; the save-variable schema (`LoadSaveSchemaFromBrx`); the level table; the
   localized strings; the cell plane tree; the options of `g_sw` and of `g_cm`; then creates the world's Splice frame
   (`InitSwSpliceFrame`) and resolves the save-variable handles (`ResolveSwSaveVars`, which ends with game event 3).
   Then **per world entry**: its Splice table (`LoadSwSpliceFromBrx`), shaders and textures, zone tree, its object list
   (`LoadSwObjectsFromBrx` into a collector), then the clones (`CloneLoWithParent` per extra copy), and either the objects stay
   or they are removed again to serve as proxy sources. Formats: [formats/world-file.md](../formats/world-file.md),
   [formats/level-objects.md](../formats/level-objects.md).
7. `PostSwLoad`: prime pools, create the screen tint object, select the main `MS` object (purpose inferred), build proxy spares
   (`InitSwProxySpares`), bind the timer HUD, noise types, default light, path zones, booty proxy, grab marker and
   screen hooks; then **run slot 15 (post-load) on every object created during the load, repeating while callbacks or
   post-load handlers create more**; then path-zone link lists, camera setup, game event 4.
8. If a resume time was pending, show the countdown HUD; clear cheat items when the level changed.
9. Find and trigger the arrival warp.
10. If no player has control yet, choose the playable object from the save variable `oidPoCurrent` (with fallbacks),
    add it, give it control, attach it to the warp, and remove every other player-character that has no controller
    state. Which character you play comes from the save, not from the level.
11. Final camera update and cut, **game event 5**, leave the loading phase, `ResumeWipeAfterLoad`, and if the
    transition had no file (a movie or front-end step) run the next startup step.

The order of game events during a load is therefore: 1 (teardown) -> 2 (load begins) -> 3 (save variables resolved)
-> 4 (post-load done) -> 5 (transition done). Game-side reactions (restoring tasks, choosing the current job) hang off
these; see [progression-and-saves.md](progression-and-saves.md).

## 6. Activation inside the world

A loaded object is not necessarily active. Activation is decided per hierarchy root by the busy list each frame
(distance to camera or player, or forced on/off), and only live objects are in the update table and get rendered.
Task visibility records add and remove objects as missions progress. Details: [object-model.md](object-model.md),
sections 4.4 and 4.5.

## 7. Control counters and deferred work

**Hands-off and friends** (`world/scene/sw.cpp`), all counted, so every increment needs a matching decrement:

- `IncrementSwHandsOff(world, grf)` / `DecrementSwHandsOff` / `IsSwHandsOff`: take control from the player. The first
  increment disables input (a `g_grfusr` bit); flag 1 also takes the letterbox (`AcquireSwLetterbox`), flag 2 also
  locks input. While hands-off the player is invulnerable.
- `IncrementSwNoPause` / `DecrementSwNoPause`: block the pause menu (cutscenes, recon mode).
- `AcquireSwLetterbox` / `ReleaseSwLetterbox`: the letterbox bars.
- Input lock: `IncrementSwInputLock` / `DecrementSwInputLock` (inferred names).

Users: relays (cutscenes) take hands-off and no-pause together; the job banner takes hands-off; exits take hands-off
while warping.

**Deferred work**, three kinds:

- **World callbacks** (`world/scene/callback.cpp`): `PostSwCallback(world, object, member function, arg)` queues a call;
  `EnsureSwCallback` queues it only if an identical one is not queued; `ClearSwCallbacks` removes matching ones.
  `ProcessSwCallbacks` drains the queue (repeating while callbacks queue more) and then fires the scheduled Splice
  callbacks whose time has come.
- **Scheduled Splice callbacks**: `ScheduleSwSpliceCallback` (`object/basic.cpp`) inserts a call into a time-sorted list
  on the world; posted Splice events and the `schedule-callback` builtin use it; `CancelSwSpliceCallback` removes one by
  id (the builtin `cancel-callback`).
- **Per-object deferred callbacks** (`LORUN` on each LO): run by that object's post-load (`RunLoDeferredCallbacks`);
  used for oid fixups and deferred removal.

## 8. How it connects

- **Object model**: `SW` owns every list objects link into; `CreateLo`, `AddLo`, the busy list and the update table all
  write `SW` fields.
- **Splice**: the world has the root Splice frame; each world entry's Splice table hangs under it; scheduled
  callbacks live on the world. [splice-scripting.md](splice-scripting.md).
- **Progression and saves**: the schema comes from every world file; `ResolveSwSaveVars` caches handles in `SW`;
  load events drive task restoration. [progression-and-saves.md](progression-and-saves.md).
- **Engine services**: `DeleteSw` and `PostSwLoad` reset or initialise sound, camera, shaders, physics and path zones;
  `UpdateSw` calls physics, animation, sound and camera. This is why `world/scene` depends on higher layers
  ([architecture.md](../architecture.md), "WORLD-UP").

## 9. Invariants and gotchas

- **Every pointer into the old world dies at a transition.** `SW` is cleared and reconstructed, the world heap is
  reset, and globals that point at objects are cleared in `DeleteSw`. A new global that caches an object pointer must
  be cleared there too, or it dangles into the next level.
- A transition never happens immediately; it runs at the top of the next frame, and only one can be pending. Code
  that requests a warp keeps running for the rest of the frame.
- The clock is reset at every load; scheduled times and next-Splice-update times are world-relative.
- `UpdateSw` has two paths: with the game clock stopped, normal `Update` calls do not happen at all. Code that must run
  during pause belongs in a screen update, the camera, or an entry flagged to update while paused.
- The "same level" flag lets a reload of the current file keep some state (the cheat items, for example); a different
  file clears more.
- A transition with no file is legal and is how movies and front-end steps run.
- The old checkpoint vocabulary is wrong for Sly 2: "checkpoints" are ordinary TASK states saved in the save blob.

## 10. Recipes

**Warp somewhere from code.** Call `WipeToWorldWarp(&g_sw, filerec, warpOid, 0, flags, wipeWidgetOid)`. Get the file
record from the level table (`GetWorldEntry(world)` for a world's hub, or the level-index helpers in `sw.cpp`); pass
null for the current level and a wipe widget oid (the standard wipe widget exists in every world) to get the wipe, or
-1 to cut immediately. The warp oid names a WARP or one of its aliases in the target level.

**Run code after every level load.** Hook game event 4 (post-load) or 5 (transition done) in the game record's event
handler (`HandleGameEvent` in `world/state/game.cpp`), or add a step to `PostSwLoad`. Do not hook `LoadSwFromBrx`:
objects are not post-loaded yet and oid references are unresolved.

**Add per-frame world work.** If it belongs to an object, give the object an `Update`. If it is global, add a call in
`SW::Update` at the right point of the order above (before `SolveSw` for forces, after it for reacting to contacts),
and decide whether it must also run while paused.

**Keep a per-class list.** Follow the existing pattern: a `DL` in `SW`, append in the class's `OnAdd` (slot 23), remove
in `OnRemove` (slot 22). The list then holds exactly the live instances.

## 11. Where to look

- `src/game/app/main.cpp`: `GameMain`, `Startup`. `src/game/app/tu_1A4E08.cpp`: the startup step queue.
- `src/game/world/scene/sw.cpp`: `SW_ctor`, `SetSwTransition`, `DeleteSw`, `InitSwLoadBuffers`, `ResolveSwSaveVars`,
  `PostSwLoad`, `ExecuteSwTransition`, hands-off counters, lighting, the update table, level table queries.
- `src/game/world/scene/callback.cpp`: world callbacks.
- `src/game/level/update.cpp`: `SW::Update`, `UpdateSwObjects`.
- `src/game/level/brx_sw.cpp`: `LoadSwFromBrx`, strings, cells.
- `src/game/level/exit_warp.cpp`: EXIT and WARP. `src/game/level/wipe.cpp`: the wipe and `WipeToWorldWarp`.
- `src/game/object/busy.cpp`: activation by distance.
- `include/sly2/sw.h`: the `SW` layout.
- [engine.md](../engine.md) sections 3-5 for the summary; the original research pages (runtime tables, field offsets)
  are in git history (see the top of engine.md).
