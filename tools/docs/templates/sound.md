# Sound

Sly 2 rewrote Sly 1's C sound code as C++ classes with a request queue. Sound effects come from a per-level 989snd bank;
music, dialogue and ambience stream from disc through Sucker Punch's own IOP module (`spiop.irx`, RPC 'SUKR'). File formats (989snd 'SBlk' v3 banks,
VPK ambience/music, VAGp dialogue) and the decoder are in [formats/audio.md](../formats/audio.md) `[C:parse+decode]`.
Related: [world file](../formats/world-file.md#32-sound-section-loadsoundsectionfrombrx-func_001fd0e8) (sound section), [cutscenes](cutscenes.md) (movie audio).

## 1. Sound manager (sound.cpp, 0x1FAF70-0x1FF398)

- `g_sndmgr` = D_003DD040, built by sound.cpp's static init via the ctor 0x1FCD38 `[H r7 sound.md]`.
  Contains: pools for bank-SFX instances (0x70 B, +0x1C) and stream instances (0x3C0 B, +0x34); a dirty DL (+0x4C); the bank FILEREC
  and handle (+0xE8), `aisnd` (sfxid -> index) and `anKind` tables; stream lists; a music stack (+0x204, depth 8); dialog stream
  (+0x200); ambience (+0x228); an 8 category x 4 channel volume matrix (+0x13C); global pause flags (+0x58) `[H r7]`.
- `LoadSoundSectionFromBrx` 0x1FD0E8 (named by its matcher comment) reads the level's sound section `[C:static]`.
- `UpdateSndmgr` 0x1FD830 runs from UpdateSw `[H r4 main.md, r7]`.

## 2. Sound instances

- **SND** (CID 0x97, base vtable 0x2D9D00, 0x60 bytes): refcounted; flag word +0x50 is a **request queue**: 1 start, 2 pause, 4 continue,
  8 stop, 0x10/0x20/0x40 vol/pan/pitch dirty, 0x80/0x100 group vol/pan, 0x200 positional, 0x400/0x1000 status query.
  `SetSndFlags` 0x1FB658 links the instance into the dirty list; `UpdateSnd` 0x1FB8E0 turns bits into virtual calls `[H r7]`.
  Slots: 7 Start, 8 GetKind (2 = looping), 9 Play, 10 Stop, 11 Pause, 12 Continue, 13 ApplyParams, 14 QueryStatus, 15 SetCulled, 16 UpdatePositional.
- **BANKSND** (vtable 0x2D9C70, 0x70): bank sounds. Gameplay pattern: `PsndStartSound(sfxid)` 0x1FBB90, then `SetSndSourceAlo` 0x1FB2F8,
  `SetSndRange`, `SetSndVolume`, `SetSndPitch` `[H r7]` (0x1FBB90/0x1FB2F8 are playbook anchors).
- **STREAMSND** (vtable 0x2D9BE0, 0x3C0): IOP-streamed audio via RPC id 0x524B5553 (**'SUKR'**), commands 0..12 `[H r7]`. Kinds: dialog
  (6 lip-sync SM roles on the speaker; STEP slots 83/84 refcount them), music (8-deep stack, layer mix driven by player state), ambience
  (6 groups mixing the loudest of up to 8 positional sources).
- Rhythm minigame timing reads music time from 0x1FE3F8(&g_sndmgr), not the game clock `[H r2 rhythm.md]`.

## 3. 989snd client library (0x2319E8-~0x233130)

Sony's EE-side 989snd: thin wrappers over `snd_SendIOPCommandNoWait/AndWait` (0x232838) with Sly 1-identical command ids 0x06..0x40,
plus Sly 2 commands 0x21 (6 ints + callback), 0x5A, 0x5C (0x30-byte play parameters) `[C:verified]` (r7r8 verifier: ids and payloads match Sly 1 989snd.c).
Command ids: 6 UnloadBank, 8 ResolveBankXREFS, 9/10 Set/GetMasterVolume, 0xB SetPlaybackMode, 0x13 Pause, 0x14 Continue, 0x15 Stop,
0x16/0x17 Pause/ContinueAllSoundsInGroup, 0x19 SoundIsStillPlaying_CB, 0x1A IsSoundALooper, 0x22 AutoVol, 0x36 StreamSafeCheckCDIdle,
0x3B-0x3E movie sound, 0x40 GetTransStatus.
Conflict resolutions: the names_v2 proposals for 0x2322F0-0x232568 are shifted by one; use r8's names decoded by command id (CONFLICTS #8).
`ctx.py` mislabels 0x2322F0 as StopSound; it sends 0x5C, a play call (CONFLICTS #12).

## 4. Voice / subtitle helpers (tu_2275B8)

Classless helpers: speaker-kind -> RGBA table (`RgbaFromSpeakerKind`, verified), label init used by MPEG audio and sound, oid -> name `[H r8 voice_helpers.md]`.

## Devtools findings (v3, 2026-11-01)

From [devtools/audio_browser](../guides/devtools.md#audio_browser) (all 43 worlds parsed; 4 sound files decoded in the sandbox). Not re-derived from code in v3 unless stated.
- **SFX id space**: each level's sound list is sorted; ids **0-146 exist in all 43 levels** (common block); all 37 constant ids passed to
  `PsndStartSound` 0x1FBB90 in code lie in 2..106. Higher ids are per level (csfx shrinks from 690 in the boot world to 290 in L43), so an id > 146
  probably means different sounds in different levels `[C:parse audio_browser]` (set facts); meaning of the block (player/gadget/UI) `[H]`.
- **CID43 = ambient sound emitter**: option eopid 99 = `SetCid43SoundParam(sfxid, sFull, sStop, f, f)`; `OnCid43Add` starts it via
  `PsndStartSound`/`SetSndRange` `[C:parse]` (id), argument meanings 2-5 `[H]`.
- **SW option 934 `SetAmbienceGroup(ambid, vol, pan_deg)`**: 22 levels, always 2 calls, pans mostly 90/270 (L19 320/50): two ambience groups
  panned left/right `[C:parse]`; ambid i = VPK channel i `[H]`.
- **Music ids without a file**: some music ids carry a null FILEREC (lba 3232, size 0; e.g. L02 ids 29-31) yet scripts still `PushMusic` them; the track
  ships with the sibling episode file `[C:parse]`.
- L02 bank: 383 sounds, 381 mapped (csnd): the last bank sounds have no sfx id `[C:parse]`.
- **Odd-rate tones**: with the note-60 pitch formula some TONEs come out above 48 kHz (center_note 220 -> 197,804 Hz); all tones have
  map_low = map_high = 0. The rate formula is suspect for these `[H]` (open: compare an SPU2 dump).
- **Open issue (modded ISO, live test 2026-11-01)**: see [open-questions](../open-questions.md#modding-live-test-2026-11-01) (all SFX missing after the L02 mod).

## 989snd bank and grain notes (sfx_synth, v3.1)

From `devtools/sfx_synth/FORMAT.txt`. Semantics come from OpenGOAL's clean 989snd re-implementation checked against Sly 2 data and one SPU2 state;
the Sly 2 989snd.irx (in SCUS_973.16 at file offset 0x21ED80) was **not** disassembled.
- Bank: `u32 3, u32 nchunks, {off, size}[n]`; chunk 0 = SBlk (v>=2 layout: +0x16 nsound, +0x18 ngrain, +0x1A nvag, +0x1C first sound, +0x20 first grain,
  +0x34 grain data), chunk 1 = SPU sample image `[C:parse]`.
- Sound (12 B): vol, vol group, pan, grain count, instance limit, flags (1 LOOP, 2 SOLO, 8/0x10/0x20 instance limits `[H]`), first-grain offset `[C:parse]`.
- Grain (8 B): `type<<24 | low24`, s32 delay (ticks). L02: 3,028 grains, mostly TONE (1,920), RAND_PB, RAND_PLAY, RAND_DELAY, LFO_SETTINGS; every program is
  well formed under these semantics `[C:data]`. Tick rate 240 Hz `[H]`.
- Tone (0x18 B): priority, vol, **signed** center_note (< 0 = 48 kHz-referenced, >= 0 = PS1 tone scaled 44100/48000), fine, pan, map range, pitch-bend range
  (semitones), ADSR1/2, flags (8 noise, 0x10 reverb only), sample offset. Pitch = sceSdNote2Pitch(...), 4096 = 48 kHz; dominant tuning cn -86 cf 66 = 0x3AC
  = 11,016 Hz `[C:runtime SPU2 state]`. Tones above 48 kHz are real data clamped by the SPU at 0x3FFF (4x) `[H]`.
- Validation: the L02 sample image is resident in SPU2 RAM and 8/8 live SFX voices carry a bank tone's ADSR and an in-range pitch `[C:runtime]`. Not checked:
  envelope timing, LFO speeds, volume law.

## 5. Source files

{{TUS}}

## 6. Key functions

{{KEYFUNCS:0x1FD0E8 0x1FD830 0x1FBB90 0x1FB2F8 0x1FB658 max=60}}

## Open questions
- SUKR RPC commands 0xB and 0xC (polled every frame); 989snd commands 0x18, 0x21, 0x5A, 0x5C. Experiment: break 0x232838 and log a0.
- What the two Splice-driven counters (+0x5C/+0x60) gate (ducking?).
- Mapping the bank's sound indices to game sfx ids (world sound section `aisnd`) and the non-TONE grain types; see [formats/audio.md](../formats/audio.md).
