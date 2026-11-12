# Bosses and minigames

How the boss fights and the minigames (hacking, turret, RC combat, dance, the safehouse) are put together. They add
few new engine mechanisms: bosses are guards, scripts and relays; minigames are playable objects that take the
controller. For the summary see [engine.md section 7.15](../engine.md#715-bosses-and-minigames); the machinery they
reuse is in [ai-guards.md](ai-guards.md), [player.md](player.md), [cutscenes-and-sequences.md](cutscenes-and-sequences.md)
and [progression-and-saves.md](progression-and-saves.md).

## 1. Purpose and shape

- **A boss** is a mission (a JOB, or tasks inside a heist JOB) built from generic parts: a `STEPGUARD` subclass or a
  plain assembly of objects, guard scripts and behaviours, a health-bar widget, game events for damage and defeat,
  relays for the phases and the cutscenes, boss music, and proxy copies so a retry gets a fresh boss after the level
  reloads. The fight's rules mostly live in the level's Splice; native code supplies the boss class's special
  movement or senses.
- **A minigame** is a playable object (`PO` subclass) that becomes the controlled object for a while. Control is
  handed over by a relay command (`RELAYCK_SetCtlState`, which calls the PO's `SetCtlState(1)`) or by the task that
  names the playable object it is played with. The minigame object reads its `POCTL` like a hero does, and the level's
  scripts and relays decide when it starts, how it scores and what completing it does to the task.

Code: boss classes in `src/game/gameplay/ai/` (one small file per named character: `carmelita.cpp`, `dimitri.cpp`, `neela.cpp`, `rajan.cpp`, `contessa.cpp`), minigame
objects in `src/game/gameplay/objects/` (`turret.cpp`, `rhythm.cpp`, `hack.cpp`, `robottank.cpp`) and
`src/game/gameplay/ai/safehouse.cpp`. The hacking classes are declared in `include/sly2/hack.h`, the robot tank and its
ammo pickup in `include/sly2/tank.h`. Each file's header comment names its classes and entry points.

## 2. Main types

| Class (class id) | What it is | File |
|---|---|---|
| `DIMITRI` (0x3B) | boss guard with a table of authored escape routes; two behaviours of its own (escape, attack) | the unit defining `FindDimitriFarthestRoute` |
| `RAJAN` (0x3D) | boss guard: hears one stimulus source at double range; a camera policy framing him with Murray | `rajan.cpp` |
| `CONTESSA` (0x40) | boss guard: Init and Update overrides that fit her body to the terrain while she crawls | `contessa.cpp` |
| `HACK` family (0x47-0x4C: `HACK`, `HACKSHIP`, `HACKENEMY`, `HACKBULLET`, `HACKLOCK`, `HACKWALL`) | Bentley's hacking minigame: the ship (a singleton), enemies, bullets, locks and walls, each an `SO` with health and entry tables bound by oid | `hack.cpp` |
| `TURRET` (0x45) | player-controlled turret (a PO): aim from the stick or a target, zoom, fire through a launcher; the camera follows with the `CPTURRET` policy | `turret.cpp`, `engine/camera/cplcy.cpp` |
| `ROBOTTANK` (0x4D) | the RC combat car (a PO) with wheels, weapons, ammo and an AI mode; CID108 is its ammo pickup | the unit defining `CollectRobottankPickup` |
| `DANCER` (0x44) and the rhythm manager (`RHYTHMMGR`, was CID121) | the dance minigame: dancers playing a move clip per cue, and a manager timing cues, judging presses and drawing the note track | `rhythm.cpp` |
| `SAFEHOUSE` (0x46) | the safehouse character select: steps through the selectable characters and binds buttons to relays (ThiefNet) | the unit defining `UpdateSafehouse` |

## 3. Bosses

Each fight in the retail game (from engine.md section 7.15, which lists the job and level of each):

- **Dimitri**: the `DIMITRI` class. His escape behaviour waits a few frames, picks the route whose near end is
  farthest from the hero (`FindDimitriFarthestRoute`) and runs his attack behaviour along it, or falls back to normal
  combat (`BHVCOMBAT`) when the hero is close. The guard mode `GUARD_MODE_FLEE` pushes the escape behaviour
  ([ai-guards.md section 6](ai-guards.md#6-modes-combat-and-turn-taking)).
- **Rajan**: the `RAJAN` class (sensing override, camera) plus Splice for the phases; played first as Sly, then as
  Murray.
- **The Contessa**: the `CONTESSA` class plus Splice, two battles.
- **Neyla (the RC duel)**, **Jean Bison** and **Clock-La** have no boss class of their own: Splice and relays drive
  generic objects (state machines, targets, solid objects), and the job script counts hits and advances phase tasks.

Common machinery:

- **Hit points and defeat**: `STEPGUARD`'s hit points and KO handling; damage and defeat are broadcast as game events
  (progression-and-saves.md section 4) that the job script listens to (event 17), and a health-bar widget shows the
  value.
- **Phases** are tasks of the boss job; relays play the transitions and set the next task state.
- **Retries**: death reloads the level ([world-and-levels.md](world-and-levels.md)); the boss is a proxy copy, so the
  reload places a fresh one, and the job restarts from its pending task.

## 4. Minigames

- **Entering.** A relay's `RELAYCK_SetCtlState` command makes the minigame PO controlled (`SetCtlState(1)`, the PO's
  control-state virtual), or the current task names the PO it is played with and becoming the current task selects it
  ([player.md section 8](player.md#8-the-ai-partner-and-scripted-control)). The heroes leave control the same way a
  character switch takes them out.
- **Hacking** (`HACK` family): the ship moves with the left stick and aims and fires with the right; enemies, locks and
  walls are configured by Splice setters (`SetHack*Entry`, `AddHackwallSegment`, `AddHacklockRsmg`); destroyed pieces
  die through a scheduled world callback (`StartHackDying`). Locks drive state machines when broken, which is how the
  level knows the hack progressed.
- **Turret**: `UpdateTurret` aims within pitch and yaw ranges from the stick or a target and fires through a launcher;
  `OnTurretActive` switches it on and off with control.
- **RC combat**: the robot tank runs in player or AI mode (`SetRobottankTactic`); wheels, weapons and attachments are
  added by Splice setters.
- **Dance**: the rhythm manager gets cues from relays (the builders `AddRelayRhythmCue` and
  `AddRelayRhythmSequence` in `relay.cpp`, executed in `NextJsgCommand` through `PushRhythmManagerCue` and
  `SetRhythmManagerPrompts`), and judges button presses against the **music time** (`GetMusicTime`), not the game clock;
  dancers play the move clip for each cue and follow its root motion.
- **Safehouse**: `UpdateSafehouse` reads the stick selection and steps the character-select state machine
  (`SetCharSelectState`, `StepCharSelect`) over the entries added by `AddSafehouseEntry`.

## 5. Invariants and gotchas

- **Control must come back.** A minigame that takes control through a relay must give it back (another
  `RELAYCK_SetCtlState`, or the task change that selects the hero); a stopped relay does not restore the previous PO
  by itself.
- **Proxy copies make retries work.** Boss state that is not in save variables is reset by the level reload; state you
  add to a boss must live in the object (reset with the copy) or in a save variable (if it must survive), never in a
  global.
- **Music time is the dance clock.** A port or mod that changes stream timing breaks the rhythm judgement before
  anything else (sound-and-music.md section 8).
- **Fights are mostly data.** Changing a boss's rules usually means changing the job's Splice and relays, not code;
  read the decoded scripts before the class.
- Most minigame classes are still partly placeholder-named and their fields `(inferred)`; the file header comments are
  the most reliable summary.

## 6. How to

**Change a boss's health or phases.** Health is the guard's hit points (set by options at load); phases are the job's
tasks and the relays between them. Both are level data.

**Add a minigame.** Write a PO subclass ([cookbook/new-object-class.md](../cookbook/new-object-class.md)) that reads its
`POCTL` in `Update` and reports progress through a game event or a task request; hand control to it with a relay's
`RELAYCK_SetCtlState` and back at the end; give it a camera policy if the follow camera does not fit
([camera.md](camera.md)).

**Find what a boss fight does.** Start at the job in the decoded script (the job and task names in engine.md section
7.15), follow its event-17 handler and its tasks' relays, then read the boss class for what the scripts do not cover.

## 7. What a port must replace

Nothing specific: bosses and minigames are game logic on top of the shared systems. The dance minigame depends on
accurate music stream positions, and the hacking and turret views on the same rendering and camera as the rest of the
game.

## 8. Where to look

- `src/game/gameplay/ai/`: the boss classes (`dimitri.cpp`, `contessa.cpp`, `rajan.cpp`, `carmelita.cpp`, `neela.cpp`)
  and `safehouse.cpp` (`UpdateSafehouse`).
- `src/game/gameplay/objects/`: `turret.cpp`, `rhythm.cpp`, `hack.cpp` (classes in `include/sly2/hack.h`) and `robottank.cpp`
  (`CollectRobottankPickup`; classes in `include/sly2/tank.h`).
- `src/game/engine/camera/cplcy.cpp`: `CPTURRET` and the other special camera policies.
- `src/game/gameplay/cutscene/relay.cpp`: `RELAYCK_SetCtlState` and the rhythm commands.
