# Sound and music

How the game plays sound effects, ambience, music and dialogue, and how subtitles follow the voice. For the summary
see [engine.md section 7.7](../engine.md#77-sound-enginesound-srcsdk989snd); for file formats (989snd banks, VPK
streams, VAGp dialogue, the IOP stream module) see [formats/audio.md](../formats/audio.md); for the port's audio plan
see [guides/port-plan.md section 6.4](../guides/port-plan.md#64-sound--coreaudio-k12).

## 1. Shape of the system

Two playback paths, one manager:

- **Bank sounds** (effects): short PS-ADPCM samples in a per-level **989snd bank**, played by Sony's 989snd library on
  the IOP (the sound coprocessor side). The EE talks to it through the 989snd client (`src/sdk/989snd/`, functions
  named `snd_*`).
- **Streamed sounds** (music, dialogue, ambience): read from disc and decoded on the IOP by Sucker Punch's stream
  module (RPC server 'SUKR'); the EE sends it numbered commands.

Both are wrapped by the same C++ classes and a **request queue**: game code never calls the hardware directly. It sets
request bits on a sound object, and once per frame the manager flushes the dirty sounds to the IOP.

## 2. Main types

| Type | What it is |
|---|---|
| `SNDMGR` | the sound manager, the global `g_sndmgr` (a `BASIC` child). Owns the pools of bank and stream sounds, the level's sound table, the dirty list, the music stack, the current dialogue and ambience streams, the dialogue speaker roles, the volume matrix and the music-layer request counters. |
| `SND` | base sound instance (refcounted; class id 0x97): request flag word, volume, pan, pitch, fade time, optional positional source (an LO or a fixed position), voice/stream handle. Virtual slots start, kind, play, stop, pause, continue, apply params, query status, culled, positional update. |
| `BANKSND` | a `SND` playing one effect from the bank. |
| `STREAMSND` | a `SND` playing a stream record; has up to six **sub-groups** (channels of a multi-layer stream) each with volume, pan and up to a few positional sources. |
| `AMB` | ambient emitter (an XFM; class cid 0x2B): a list of looping positional sounds started when it becomes live. |
| subtitle record (`SPEAKERLABEL`, hypothesis name) | the current caption: language-string id, speaker colour, button glyph mask. |
| `SFX` descriptor | per-effect parameters from level data (effect id, audible ranges, volume). |

Header views are mostly TU-private (`engine/sound/sound.cpp` declares its own `SND`/`SNDMGR` views); `include/sly2/snd.h`
and `include/sly2/aseg.h` carry partial declarations.

## 3. Lifecycle

### Boot

`StartupSound` (from the startup table): starts the 989snd system and the stream module, sets every channel/category
volume to 1 and records the start time. `g_sndmgr` itself is a global constructed by `SNDMGR_ctor`.

### Level load and teardown

- `LoadSoundSectionFromBrx` (called first thing by `LoadSwFromBrx`): which bank file the level uses, the kind of each
  sound, the effect-id table, the ambience stream, and the dialogue and music stream records.
- `PostSoundLoad`: loads the bank if it changed (`snd_BankLoadByLoc`), resolves cross references and marks looping sounds.
- `AMB` objects load their sound lists through option setters (`EnsureCurrentAmbSound`, `AddAmbSound`), then register
  linked sounds with the ambience stream's groups (`PostAmbLoad`).
- `ResetSndmgr` (teardown): stops every sound and stream, flushes the hardware queues, clears the dirty list and resets
  volumes.

### Per frame

`UpdateSndmgr` runs near the end of `UpdateSw` ([engine.md section 4](../engine.md#4-the-frame-loop)):

1. Timing; `UpdateSoundListener` (track the listener while a player exists) and `PollSoundManagerStatus` (one
   outstanding IOP status query at a time).
2. **Global pause**: while the game is paused, the top music stream, the ambience stream and the dialogue stream are
   paused; resumed afterwards.
3. `UpdateMusicMix`: pick the music layer and cross-fade (section 5).
4. `UpdateDialog`: fire dialogue events as the stream plays (section 6).
5. Master fade toward silence when the manager is muted (`IsSoundManagerMuted`) and game time is stopped; applied
   through `SetSndmgrChannelVolume`.
6. **Flush**: `UpdateSnd` on every dirty sound, then `snd_FlushSoundCommands`.

`UpdateSnd` turns request bits into virtual calls, in order: positional update, start (play), stop, status query,
parameters (volume/pan/pitch), pause, continue. Async results come back through callbacks (`OnBankSoundStatus`,
`OnStreamSoundStarted`, `OnStreamsndStatus`), each holding a reference to the sound until it fires.

## 4. Playing sounds from game code

- **Effect**: `PsndStartSound(sfxId)` allocates a `BANKSND` from the pool (null if the level has no such effect); it
  starts at the next flush. Configure with `SetSndSourceLo` (follow an object) or `SetSndSourcePos`, `SetSndVolume`,
  `SetSndPan`, `SetSndPitch`; stop with `StopSnd(snd, fadeSeconds)`. Loopers return a voice handle and keep running until
  stopped; one-shots stop themselves when the status query reports them finished.
- **Positional sound**: `UpdateSndPositional` sets volume from distance to the listener (`CalculateSndVolPan`: full
  volume inside the near range, linear falloff to zero at the far range) and pan from direction. A sound that becomes
  inaudible is **culled** (`SetBankSoundCulled` stops it; a looper keeps its parameters and restarts when audible).
- **From animation**: clip events start and stop sounds ([animation.md](animation.md#5-clips-aseg-and-asega)).
- **From scripts**: Splice builtins and thunks wrap `PsndStartSound`, the music calls and `SetSndmgrChannelVolume`;
  `SetAmbienceGroupMix` is a Splice command for the ambience groups.
- **Ambience**: an `AMB` starts its sounds as looping positional bank sounds when it becomes live (`OnAmbAdd`) and stops
  them when it leaves (`OnAmbRemove`), so the busy-list activation radius governs which ambient emitters run. The
  level's ambience **stream** has sub-groups whose volume and pan come from the loudest audible of their sources
  (`AddStreamGroupSource`, `UpdateStreamsndPositional`).

## 5. Music

- **Stack**: `PushMusic(id)` stops the current track, reuses or creates its stream (positioned on the player) and
  pushes it; the stack is a fixed eight deep. `StopMusic(id)` and `FindMusic(id)` work by id. Only the top plays.
- **Layers**: a music stream carries three layers as sub-groups. `UpdateMusicMix` picks layer 0, 1 or 2 from request
  counters and the player's state (calm, sneaking/hiding, alert/action) and cross-fades the three group volumes with a
  power-preserving smoothing on the real clock. Gameplay raises requests with `AddMusicLayer2Request` /
  `RemoveMusicLayer2Request` (AI alert, inferred) and `AddMusicLayer0Request` / `RemoveMusicLayer0Request`.
- **Music time**: `TMusicCur` is the play position of the top track from the stream's status replies. The rhythm
  minigame judges against it, not the game clock.

## 6. Dialogue and subtitles

- `StartDialog(id)`: stops the current line, creates its stream, sets the subtitle (unless suppressed) and binds the
  speaker objects' state machines to the **six lip-sync roles** (the speaker itself in the primary role, the others
  found through role SM oids).
- `UpdateDialog` fires the line's events as the stream's play time passes them: `ApplyDialogEvent` sets the goal state
  of a role's SM (mouth shapes) or plays a gesture. `StopDialog` flushes remaining events, returns mouths to rest and
  clears the line. `IsDialogPlaying`, `GetDialogTime` and `GetDialogStreamDuration` are the queries.
- **Subtitles** (the subtitle unit in `engine/sound/`): `SetSubtitle` / `ClearSubtitle`; `GetSubtitleText` copies the
  language string; `TrimSubtitleToVoiceProgress` cuts it to the fraction of the voice line played so far (the
  typewriter effect); `FormatSubtitleButtonGlyphs` appends rich-text button glyphs; `RgbaFromSpeakerCid` colours the box
  by speaker class. The HUD draws it through text callbacks ([ui-and-hud.md](ui-and-hud.md)). RELAY cutscene commands,
  movies and the letterbox also set subtitles.

## 7. Volumes: channels and categories

The manager keeps a matrix of **8 categories x 4 output channels**; a channel's output is the product of its category
weights, sent with `snd_SetMasterVolume` when it changes by at least 1/1024. `SetSndmgrChannelVolume(mgr, channel,
category, weight)` sets one cell; `ResetSndmgrVolumes` resets them.

Observed use (mapping inferred from callers): channel 1 is music (the options' music volume), channels 0 and 3 are
effects and ambience (the options' effects volume sets both), channel 2 is dialogue. Category 1 holds the user's option
volumes (applied from the save by the options code and the pause menu), category 2 the manager's own mute/pause fade;
setting category 2 to or from zero also pauses or resumes the streams on that channel.

Other global settings: `SetSndmgrPlaybackMode` (mono/stereo option), `SetSndmgrMasterVolume`. The **headset** option is
a save variable (`flags.fUseHeadset`) that the options code copies into the manager; no headset audio path is
identified in the decompiled sound code. The IOP image includes USB audio modules (see
[formats/audio.md](../formats/audio.md)); what the game does with a headset beyond this flag is an open question.

## 8. Invariants and gotchas

- **Nothing happens until the flush.** Setters only mark bits; reading a sound's state right after starting it shows the
  request, not the hardware.
- **References**: the dirty list, async callbacks and the music stack each hold references. Keep your own reference (or
  a counted slot) if you need a sound after the frame; a sound with no owner is freed once its callbacks return.
- **Pools are fixed.** `PsndStartSound` / `PstreamsndNew` return null when the pool is exhausted; callers must check.
- **Effects are per level.** An effect id that is not in this level's table returns null from `PsndFromSfxid`.
- **Streams are never culled** and the music, ambience and dialogue streams are paused by the manager while the game is
  paused; bank sounds are not covered by that rule.
- **Stream time drives sync.** Lip-sync, subtitle pacing and the rhythm minigame all read stream play time; a port must
  report accurate positions.

## 9. How to

**Add a sound to an object.** Hold a counted pointer to a `BANKSND` from `PsndStartSound`, attach it with
`SetSndSourceLo(snd, this)`, and stop it in your OnRemove (slot 22) so it does not outlive the object's activity.

**Add ambience.** Place an `AMB` with sounds (id, range, volume, pitch) in the level; it needs no code.

**Change music for a situation.** `PushMusic` a track, or use the layer request counters for intensity changes within
the current track.

**Play a voiced line with subtitles.** `StartDialog(id)`; the speaker's SM must provide the lip-sync role states.

## 10. What a port must replace

The whole hardware side:

- **989snd client and IOP library** (`src/sdk/989snd/`, `snd_*`): a bank player that parses the SBlk bank, decodes
  PS-ADPCM, implements pitch, ADSR envelopes, volume/pan, loops, groups, master volume per channel and status queries.
- **The SUKR stream module**: a stream server that plays VAGp dialogue and multi-channel VPK music/ambience with
  per-sub-group volume and pan, and reports accurate play positions.
- **SIF RPC** plumbing and the async callback timing (callbacks arrive on a later frame on the PS2).
- **Reverb / playback modes** (`snd_SetPlaybackMode`) and movie sound.
- The headset modules can be stubbed as "not present".

Game-side code (`SNDMGR`, the request queue, music stack and layers, dialogue roles, subtitles) is portable as is.

## 11. Where to look

- `src/game/engine/sound/sound.cpp`: SND, BANKSND, STREAMSND, SNDMGR, music, dialogue, volumes.
- `src/game/engine/sound/amb.cpp`: AMB ambient emitters.
- The subtitle unit in `src/game/engine/sound/` (still named by address): subtitle record and helpers.
- `src/sdk/989snd/`: the 989snd EE client library.
- Callers: `ui/menu.cpp` and `world/state/save.cpp` (option volumes), `gameplay/cutscene/relay.cpp` (dialogue and music
  commands), `engine/anim/asega.cpp` (clip sound events), `app/bif.cpp` and the Splice thunks.
