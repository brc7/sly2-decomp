# Recipe: add a sound or music cue

**Goal.** Make something audible at a new moment: a sound effect when an object does something, a music change for
a situation, a voiced line with subtitles, or ambience in an area.

**Systems.** [sound-and-music.md](../systems/sound-and-music.md) (read sections 1-6 and 8 first),
[formats/audio.md](../formats/audio.md) (banks and streams), [animation.md](../systems/animation.md) section 5 (clip
sound events), [splice-scripting.md](../systems/splice-scripting.md) (script access). Mod build:
[README.md](README.md#how-a-mod-is-built).

## First: is the sound already in the level?

Everything the game plays is referenced by **id through the current level's sound section**
(`LoadSoundSectionFromBrx`, the first thing `LoadSwFromBrx` reads): which 989snd bank holds the level's effects, the
effect-id table, the ambience stream, and the dialogue and music stream records. So:

- **An effect** id plays only if the level's table maps it to a bank sound; `CreateSfxSound` returns null otherwise
  (`FindSoundForSfxId` finds no entry). An effect another level uses but this one does not is not available here.
- **Music and dialogue** are stream records in the same section, found by id (`FindMusic`, `FindDialogStream`).
- **A brand-new sample** means a new or edited 989snd bank and new level sound-section entries. The bank and stream
  formats are documented in formats/audio.md; no tool in the repository writes them yet.

Cueing existing sounds at new moments needs only code or script; new audio content needs data tools.

## What to touch

| Cue | Code entry points (`engine/sound/sound.cpp` unless noted) | No-code route |
|---|---|---|
| effect, one-shot or loop | `CreateSfxSound(sfxId)`, then `SetSndSourceLo` / `SetSndSourcePos`, `SetSndVolume`, `SetSndPan`, `SetSndPitch`; `StopSnd(snd, fadeSeconds)` | the `start-sound` builtin (`BuiltinStartSound`: creates the effect and attaches it to an object); sound events on an animation clip |
| ambience | `AMB` objects (`engine/sound/amb.cpp`) start their loops in `OnAmbAdd` and stop them in `OnAmbRemove` | place an `AMB` with its sound list in the level |
| music change | `PushMusic(id)` / `StopMusic(id)`; intensity within a track with `AddMusicLayer0Request` / `RemoveMusicLayer0Request` and `AddMusicLayer2Request` / `RemoveMusicLayer2Request` | RELAY music commands (`gameplay/cutscene/relay.cpp`) |
| voiced line | `StartDialog(id)`, `IsDialogPlaying`, `StopDialog` | RELAY dialogue commands |
| volume rules | `SetSndmgrChannelVolume(mgr, channel, volume, category)` | the options menu |

## Steps: an effect from native code

1. **Create.** `CreateSfxSound(sfxId)` takes a `BANKSND` from the pool; it starts at the next flush, not now.
   **Check for null**: the pool may be exhausted or the level may not have the effect.
2. **Place.** `SetSndSourceLo(snd, object)` to follow an object (volume and pan then come from distance and direction
   in `UpdateSndPositional`), or `SetSndSourcePos` for a fixed point. Leave both unset for a non-positional sound.
3. **Own it.** If you need the sound after this frame (a loop, or a sound you will stop), keep a counted reference;
   otherwise the manager frees it when its callbacks finish. One-shots stop themselves; loops run until `StopSnd`.
4. **Stop it with its owner.** For a loop attached to an object, stop it in the object's `OnRemove` (slot 22) so it
   does not outlive the object's activity, and start it again in `OnAdd` if needed.
5. **Where to call from.** An `Update` (slot 16), a state change, a message handler. Effects tied to a pose belong on
   the animation clip as sound events instead (no code: the clip plays them).

## Steps: music for a situation

1. For a different track: `PushMusic(id)` when the situation starts and `StopMusic(id)` when it ends. The stack is
   bounded and only the top plays; pair every push with a stop.
2. For more or less intensity within the level's track: raise and lower the layer request counters.
   `UpdateMusicMix` picks the layer from the counters and the player's state and cross-fades, so requests must be
   balanced (every add has a remove on every exit path), or the music sticks in that layer.

## Steps: a voiced line

1. `StartDialog(id)` stops the current line, starts the stream, sets the subtitle and binds up to six lip-sync roles
   to the speaker objects' state machines.
2. The speaker needs a state machine providing the mouth states; a line spoken by an object without one plays audio
   and subtitles but no lip-sync.
3. In a cutscene, use the RELAY dialogue command instead, which waits for the line
   ([level-event-task.md](level-event-task.md)).

## Gotchas

- **The match.** Calling any of these from new places changes game code; build as a mod. Level-data routes (an `AMB`,
  a clip sound event, a RELAY command, a Splice call) change data only, once a world-file writer exists.
- **Nothing happens until the flush.** Setters only set request bits; `UpdateSndmgr` flushes them near the end of the
  world update. Querying a sound right after starting it shows the request, not the playback.
- **Async callbacks.** Status replies arrive on a later frame through callbacks that hold a reference to the sound.
  Do not assume a one-shot has finished, or started, in the same frame.
- **Streams pause on disc access** and fade or cut while game time is stopped; bank effects do not. A cue meant to
  play under the pause menu must be a bank effect.
- **Headset microphone is gameplay.** Do not reuse the microphone noise path for audio work: it makes the hero audible
  to guards (sound-and-music.md section 7).

## How to test

- In a mod build, trigger your cue in a level whose sound table has the effect (the decoded sound section of the world
  file lists the ids), first non-positional, then attached to an object while you walk away from it to check the
  falloff.
- For loops: leave the area and come back, change level, and pause; the loop must stop with its owner and not leak
  pool entries (a leak eventually makes `CreateSfxSound` return null everywhere).
- For music layers: enter and leave the situation repeatedly, including dying inside it (the level reload resets the
  manager), and listen for a stuck layer.
