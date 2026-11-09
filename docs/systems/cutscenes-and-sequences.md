# Cutscenes, sequences and movies

How the game plays scripted sequences: in-engine cutscenes and mission choreography built as **RELAY** command lists and
run by **JSG** scripts, the full-motion movie player, and the full-screen transitions around them (frame capture,
wipes, letterbox). For the summary see [engine.md section 7.14](../engine.md#714-cutscenes-sequences-and-movies-gameplaycutscene);
for how scripts build relays see [splice-scripting.md](splice-scripting.md); for the missions that fire them see
[progression-and-saves.md](progression-and-saves.md).

## 1. Purpose and shape

Sly 2 has almost no pre-rendered video: mission intros, dialogue scenes, reveals and most scripted moments are played
in the engine by **relays**. A relay (`RELAY`, an `LO` subclass) is a flat array of commands (`RELAYCMD`) organised in
nested **blocks**. Firing a relay starts a small interpreter, a **JSG** (a relay script), on its top block; a block
can start other blocks in parallel, wait for them, wait for time, a dialogue line, an alarm or a player state, and
drive actors, the camera, widgets, sounds, task states and control hand-off. Each frame every running JSG executes
commands until one is not complete yet.

Code:

- `gameplay/cutscene/relay.cpp`: RELAY (its virtuals, firing and stopping, alarms, labels), the `AddRelay*` builders
  scripts call to append commands, and the JSG interpreter (`NextJsgCommand`, `IsJsgCommandComplete`, the per-script
  helpers for hands-off, camera blends, actors and focus).
- `gameplay/cutscene/mpeg.cpp`: the movie player `CMpeg` (one global).
- `gameplay/cutscene/framecapture.cpp`: the frame-capture transition (class id 104, still placeholder-named).
- `level/wipe.cpp` (the level-transition wipe) and `engine/presentation/letterbox.cpp` (the letterbox bars).

Types: `RELAY` in `include/sly2/classes.h`; the command kinds are the `RELAYCK` enum in `include/sly2/basic.h`;
`RELAYCMD`, the JSG view and the per-kind argument blocks are declared in `relay.cpp`.

## 2. Main types

| Type | What it is |
|---|---|
| `RELAY` | a level object holding a command array, an optional private command limit, the list of running scripts, the firer, an alarm list, the fired time and a busy mark |
| `RELAYCMD` | one command: its kind (`RELAYCK`), an async flag and an argument block (a union selected by the kind, filled by the kind's builder) |
| `RELAYCK` | the command kinds (`RELAYCK_` + name): blocks (begin, end, stop, wait for blocks), flow (labels, conditional and unconditional jumps, signal, wait for time), alarms (raise, wait), actors (set actor, teleport, go to, walk to, follow a path, climb a pipe, face, hero state, grab, carry and throw, respawn, launch), animation (play a clip, drive a state machine), camera (cut, blend from / to, tweens, focus, tunnel), presentation (widget show / hide, hint box, button prompt, dialogue line and subtitle, sound, music, rumble, explosion), control (hands-off, controller hand-off, clock rate, Sly's look and recon modes) and progression (task state, fire or stop a relay); several kinds are still `Unk*` |
| JSG (relay script) | a refcounted `BASIC` from the world's JSG slot heap, built by `NewRelayJsg`: its relay, its block's command range and current command, its parent script, the actor and context it works on, and what it holds (hands-off, camera blend, step holds, focus) |
| `CMpeg` | the movie player: a queue of movies, the demux/decode loop over libmpeg and the IPU, the display thread, the subtitle line and the movie's audio stream |
| frame-capture object (class id 104) | an `SO` that grabs the current frame into a texture and draws it as two moving quads (a split/flip transition) |
| `WIPE` | the level-transition wipe state machine (idle, out, in, fire) |

## 3. Lifecycle

### Building a relay (level load)

`LoadRelayFromBrx` (slot 14) prepares a temporary command buffer that starts with an implicit block begin (`RELAYCK_BlockBegin`, label -1), runs
the LO load, which applies the object's option list from the world file; the relay's method options are the
`AddRelay*` builders, each appending one command through `AddRelayCommand(relay, kind, fAsync)`. The result is copied
into an exact-size array on the world heap. Scripts can rebuild a relay later: `ResetRelay` stops it and empties it back
to the implicit block, and the builders append again. Past the command limit `AddRelayCommand` returns a scratch
command, so builders never fail (but the command is lost).

### Firing

`FireRelay(relay, firer)` (or `FireRelayFromPlayer`, with the current player as firer) does nothing if the relay is
already firing; otherwise it records the firer, sends itself a "fired" message, closes the command list if needed,
starts a JSG on the top block (`NewRelayJsg`) and runs it once immediately. If the script is still running afterwards
the relay marks itself busy. Relays are fired by scripts (task activate handlers above all), by other relays
(`RELAYCK_FireRelay` command), by alarms and by objects that hold a relay reference.

### Per frame

`UpdateRelay` (slot 16): while firing, advance the fired time (on real time when the relay is flagged to run while the
game clock is paused, so pause-menu-independent sequences keep going), step every running script with
`UpdateRelayScript`, and stop the relay when no script remains.

`UpdateRelayScript` executes commands in order with `NextJsgCommand` as long as the current one is complete
(`IsJsgCommandComplete`). `NextJsgCommand` first resolves the command's actor oid for kinds that act on one
(relative to the script's context), then performs it. Non-async commands complete at once; async ones (waits, moves,
blends, lines) report completion by kind, for example `RELAYCK_WaitBlocks` completes when none of the listed blocks is running
(`FindRelayBlockScript`), `RELAYCK_Line` when the dialogue manager no longer plays that line.

### Stopping

`StopRelay` forgets the firer, removes every running script (`RemoveJsg`, which releases everything the script holds:
hands-off, camera blends, step holds, focus), clears the alarm list, releases the busy mark and sends itself a
"stopped" message. A relay that finishes normally goes through the same release as its scripts end.

## 4. What scripts can drive

- **Actors.** Movement commands make a character walk, run, follow a path, climb a pipe, teleport, face something or
  enter a state. Heroes are driven through their normal state machines: the script holds the hero
  (`AddJsgStepHold` / `RemoveJsgStepHold`) and writes intent through the ride/script hook (`UpdateJsgJtControl` in
  the player code), the walk-to goal (`SetJtgoal`) or the scripted animation states; see
  [player.md](player.md#8-the-ai-partner-and-scripted-control). Guards are driven through their script behaviour
  ([ai-guards.md](ai-guards.md#8-patrols-paths-and-scripts)).
- **Camera.** `RELAYCK_CutCamera` switches immediately; `RELAYCK_CamBlendFrom` / `RELAYCK_CamBlendTo` start a blend (`StartRelayScriptCamBlend`)
  that pushes a `CPCAMBLEND` policy on the camera stack, released with `EndJsgCamBlend` or when the script ends
  ([camera.md](camera.md#3-the-policy-stack)). Blend angles are in degrees and the vertical field of view in radians.
- **Dialogue and sound.** `RELAYCK_Line` starts a voiced line with its subtitle (`StartDialog`) and, when async, waits for it;
  `RELAYCK_Sound`, `RELAYCK_StartMusic` and `RELAYCK_StopMusic` go to the sound manager ([sound-and-music.md](sound-and-music.md#6-dialogue-and-subtitles)).
- **Control.** `RELAYCK_HandsOff` takes the hero's control through the world's hands-off and no-pause counters for as long
  as the script holds it (`SetRelayScriptHandsOff`); `RELAYCK_SetCtlState` hands the controller to another playable object
  (minigames, [bosses-and-minigames.md](bosses-and-minigames.md)); `RELAYCK_SetClockRate` changes a clock rate slot.
- **Progression.** `RELAYCK_SetTaskState` requests a task state: **cutscenes complete tasks themselves**. A task's script fires
  the relay on activation and the relay's last command moves the mission on
  ([progression-and-saves.md](progression-and-saves.md#2-tasks-states-and-transitions)).
- **Alarms and signals.** A relay keeps a short alarm list (`EnsureSggAlarm`, the same block guard groups use);
  `RELAYCK_RaiseAlarm` and `RELAYCK_WaitAlarm` let blocks and guard scripts synchronise. `RELAYCK_Signal` waits for an external object to
  release the script (`GetRelaySignalContext` / `SetRelaySignalContext`).

## 5. Movies

`CMpeg` (`mpeg.cpp`) is Sly 1's movie player with subtitles added (`DrawMpegSubtitles`). Movies are **queued**
(`CMpeg_QueueMovie`) by an exit volume that carries movies, by the startup step queue (the intro) and by the episode
menu (replays), and are **played during the next level transition**: `ExecuteSwTransition` calls `CMpeg_ExecuteOids`
after the old world is deleted, when the world heap is free. The player refuses to play (and drops the queue) when
`GetFreeMemorySize` reports too little memory. Playback demuxes the stream with libmpeg, feeds video to the IPU, sends
decoded frames to the GS from a display thread and streams the audio to the IOP. The disc holds only a couple of
movies; mission sequences are relays.

## 6. Full-screen transitions

- **Wipe** (`level/wipe.cpp`): `WipeToWorldWarp` requests a level change through the wipe widget; `UpdateWipe` (run
  from `UpdateScreens`) plays the out animation and then fires the world transition; `ResumeWipeAfterLoad` plays the
  in animation after the load ([world-and-levels.md](world-and-levels.md#4-requesting-a-level-change)).
- **Frame capture** (class id 104, `framecapture.cpp`): when started (`StartFrameCapture`, from Splice) it copies the
  frame buffer into a texture, takes hands-off, slows the clock and plays a sound, then draws the captured frame as two
  quads placed by two child objects until its animation ends. The main loop asks `IsFrameCaptureInactive` before
  rendering the world and calls `DrawFrameCaptureScreen` for the overlay ([engine.md section 4](../engine.md#4-the-frame-loop)).
- **Letterbox** (`engine/presentation/letterbox.cpp`): `ShowLetterbox` / `HideLetterbox` animate the bars through a
  timer widget; the world's letterbox counter (`AcquireSwLetterbox`) holds them during cutscenes.

## 7. Invariants and gotchas

- **A relay fires once at a time.** `FireRelay` on a firing relay is ignored; to restart, stop it first.
- **Release on every path.** Everything a script takes (hands-off, the camera blend, step holds, focus) is given back
  by `RemoveJsg`; a new command kind that holds something must release it there too, or a stopped cutscene leaves the
  hero frozen or the camera stuck.
- **Cutscenes own task progress.** Stopping a relay early (a skipped scene, a death) skips its `RELAYCK_SetTaskState`; the
  level reload after death restores tasks from save variables, but other early stops leave the mission where it was.
- **Commands resolve oids at run time** relative to the script's context (`RELAYCK_SetContext`), so the same relay works in
  every proxy copy; a wrong context makes actor commands silently find nothing.
- **Command limit.** The builders write into a bounded buffer at load and into the relay's private limit after;
  commands past it go to a scratch record and are lost without an error.
- **Movies need memory and a transition.** A queued movie plays only at the next level transition and only if enough
  memory is free; queuing one mid-level does nothing visible until then.
- Many command kinds and argument fields are still `Unk*` or `(inferred)` in `relay.cpp`; read the builder and the
  `NextJsgCommand` case together before relying on one.

## 8. How to

**Find what a cutscene does.** Find the relay (decoded script of the level: the task's activate handler names it),
read its option list (the `AddRelay*` calls in order) and follow each kind in `NextJsgCommand` and
`IsJsgCommandComplete`.

**Add a command kind.** Append a `RELAYCK` value, write its builder on `AddRelayCommand`, execute it in
`NextJsgCommand`, report completion in `IsJsgCommandComplete` if async, release what it holds in `RemoveJsg`, and expose
the builder as a `RELAY` method option. Recipe: [cookbook/level-event-task.md](../cookbook/level-event-task.md).

**Skip or shorten a sequence (port or mod).** Stop the relay (`StopRelay`) and apply what its remaining commands would
have done that matters for progress (usually the `RELAYCK_SetTaskState` at the end); the release in `RemoveJsg` restores
control and the camera.

## 9. What a port must replace

- The movie player's hardware side: libmpeg demux, the IPU decode, the GS upload from the display thread and the IOP
  audio stream; a host video decoder with the same queue-at-transition behaviour.
- The frame-capture transition's frame-buffer copy (a render-to-texture of the last frame).
- Relays, JSGs, wipes and the letterbox are game logic and port as they are.

## 10. Where to look

- `src/game/gameplay/cutscene/relay.cpp`: RELAY, the builders, the JSG interpreter.
- `src/game/gameplay/cutscene/mpeg.cpp`: `CMpeg`; `src/game/level/exit_warp.cpp` and `src/game/app/startup.cpp`
  (the startup step queue): who queues movies; `src/game/world/scene/sw.cpp` (`ExecuteSwTransition`): where they play.
- `src/game/gameplay/cutscene/framecapture.cpp`: the frame-capture transition.
- `src/game/level/wipe.cpp`, `src/game/engine/presentation/letterbox.cpp`.
- `include/sly2/basic.h` (`RELAYCK`), `include/sly2/classes.h` (`RELAY`).
