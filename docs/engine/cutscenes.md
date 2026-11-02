# Cutscenes, scripted sequences and movies

Related: [splice-scripting](splice-scripting.md), [camera](camera.md) (blend policies), [sound](sound.md) (dialogue streams), [world-sw](world-sw.md) (wipes).

## 1. RELAY and JSG: the sequence engine (TU 0x1DFAD8-0x1E5D80, file name unknown; Lilly: jsg.c)

- **RELAY** (CID 0x72, LO child, 0xB0, vtable 0x2D8C48): a level-script object holding a flat array of 0x50-byte **RELAYCMD**s (max 0x200
  or a per-relay limit), grouped in nested BlockBegin/BlockEnd blocks `[H r6b relay.md]` (Lilly layouts confirmed by code).
- Level Splice builds the array at load time through ~90 builtins, one per command kind RELAYCK 0x00-0x5A (`AddRelay<Kind>`, team names;
  reached from method thunks such as 0x106F08) `[H r6b]`.
- `FireRelay` spawns a **JSG** interpreter (0xF0, slot heap D_003E1560, vtable 0x2D8C08) for the top block; `UpdateRelay` runs `UpdateJsg` on all
  live JSGs each frame; a JSG steps through commands (`NextJsgCommand` 0x1E1E30) while each is async or complete (`FIsJsgCommandComplete`
  0x1E3890; both matched with names) and can spawn more JSGs. `RemoveJsg` is the single teardown that releases everything a script holds `[C:static]` (matched names), rest `[H r6b]`.
- Command kinds cover: actors and teleport; player movement (GoTo/WalkTo/FollowPath push BHVs on the actor: BHV166 family, BHV193);
  animations (PlayAseg); state machines (ApplySm); widgets; hands-off; camera cuts/blends/tweens; sound, music and dialogue lines; rumble;
  hint boxes; alarms; labels and gotos `[H r6b]`.
- Murray's button-mash struggle state is entered from relay code `[H r5b murray.md]`.

**Runtime-confirmed layouts** `[C:runtime runtime6]` (four live relays in the first mission): RELAY +0x54 command array, +0x58 command count,
+0x70 tFired (elapsed), +0x74 firing; RELAYCMD 0x50 bytes `{kind, fAsync, 0, 0, args...}`; JSG +0x28/+0x2C/+0x30 first/max/current command, +0x34 current
command start time, +0x38/+0x3C hands-off flag/grf, +0x84 an embedded CAMMOVE. The Lilly RELAYCK order is right for the 18 kinds seen; their arguments
match live objects: `SetTaskState(0x6E2, 2)` / `(0x6E2, 5)`, `ApplySm(context, SM 0x17DE2, 0x1512)`, `FireRelay(0x6B3)` (waits for the child relay when not
async), `HandsOff(1|0, grf 3)`, `RemoveObject`, `PlayAseg`, `TeleportTo`, `StartJtMode(7, 8)` (puts Sly in recon mode, JTMODE kind 1, with Bentley speaking),
`Line(0x513..)`, `HintBox(0x2F4)`, `Face`, `CamBlendFrom/To`, `TweenTarget`, `SetContext`.

Example (S59, dish-1 cutscene RELAY 0x6E3, 23 commands): `HandsOff(1,3)`, `RemoveObject(0x1E446)`, `TeleportTo(0x1E44C)`, `CamBlendFrom`, `SetContext(0x1E44B)`,
`PlayAseg(0xEC36)`, ..., `ApplySm(0x1E44B, SM 0x17DE2, 0x1512)` (dish destroyed), camera blends, `HintBox(0x2F4)`, `Line(0x51A)`, `SetTaskState(0x6E2, 2)`,
`SetTaskState(0x6E2, 5)`: **the cutscene itself completes the task**. Cutscene RELAYs have no Splice entry; their commands come from the object records
(METHOD options), while the TASK's Splice evt19 fires them (`FireRelayPlayer`).

**Locks taken by sequences** `[C:runtime]`: `SetJsgHandsOff` 0x1E50E0 = `IncrementSwHandsOff(grf)` + `IncrementSwNoPause` 0x20F0C0; with grf 3 the four SW
counters read [1,1,1,1] (hands-off, no-pause, letterbox widget 0x373 visible, input). Recon-mode dialogue (`StartJtMode` kind 1) takes only
no-pause + input [0,1,0,1]. See [world-sw](world-sw.md#5-hands-off-and-locks). Camera: the relay's camera blends push **CPCAMBLEND at CM+0x500**
(priority 10, pv = JSG+0x84); recon mode pushes the recon camera CM+0x520 at priority 8.
- Fixes: `EnsureSggAlarm` -> `EnsureRelayAlarm` 0x1E0020, `FWipingAseg` -> `FRelayHasAlarm` 0x1E00E8: RELAY +0x84/+0x88 integer-id sets used by RELAYCK 5/4 `[C:verified arbitration]`.

## 2. FMV playback (mpeg.cpp 0x1B2820-0x1B4438)

**No FMV is used for mission sequences**: every scripted sequence observed (S57-S62) is an in-engine RELAY and `CMpeg` n = 0 in every snapshot; the disc holds only 2 movies `[C:runtime]`.
Sly 1 mpeg.c almost 1:1 (anchor `Reset__15CQueueOutputIop`) `[C:verified]` (CMpeg/CMpegAudio methods, r3r5 verifier).
Flow `[H r5 mpeg.md]`: queued by events (`CMpeg::ExecuteOids` 0x1B3B08 refuses if `CbFreeMem()` <= 0x41FC3F) -> `CMpeg::Execute` 0x1B3BD0 runs
its own loop (clock ticked paused, UpdateJoy, UpdateCd) -> `sceMpeg` decode with callbacks -> per frame spool the async stream and
`RenderPicture` (with **subtitles**, new in Sly 2, 0x1B3700) handing frames to a display thread. Audio: `CMpegAudio` (CMpeg+0x180) -> `CByteQueue`
-> IOP via `CQueueOutputIop`; video to the IPU via `CQueueOutputIpu`. Movies are raw MPEG-PS files located by FILERECs (boot logo 0x2A3868, others 0x2A3968/0x2A39A8) `[C:static disc_toc.md]`.

## 3. Screen transitions

- **Overlay** D_002DD644 drawn by main after the world (`DrawOverlay` 0x1477D8); while in state 2/3 the world render is skipped `[H r4 main.md]`. It is NULL in all 55 snapshots, including mid-load, so it is not the loading cover `[C:runtime]`.
- **Frame-capture transition** (CID 0x68, TU 0x1471C0): on start (Splice message 0x601) it captures the framebuffer to a texture, takes hands-off,
  changes the clock rate, and main draws two quads moved by child objects (split/peel effect) `[H r2b cid68.md]` (low confidence).
- **WIPE** and `WipeToWorldWarp`: see [world-sw](world-sw.md#4-transitions-warps-exits-wipes-boot-queue). **Letterbox**: [hud-ui](hud-ui.md).
- Camera blends during cut-scenes: CPCAMBLEND with a CAMMOVE `{pposeFrom, pposeTo, dt, fCutOnLeave, iCurve}` `[H r2 cplcy.md]`.

## Devtools findings (v3, 2026-11-01)

- **Static RELAY decode = live command list** `[C:runtime splice_browser]`: RELAY 0x6E3 (dish-1 cutscene, L02) decoded from the record's METHOD options
  gives 21 commands equal to live commands 1-21 of 23 in save state S59 (same kinds, order, args: HandsOff 1,3; RemoveObject 0x1E446; TeleportTo 0x1E44C; ...;
  ApplySm 0x1E44B 0x17DE2 0x1512; HintBox 0x2F4; Line 0x51A; SetTaskState 0x6E2 2 / 5); the runtime adds BlockBegin/BlockEnd around them. Confirms
  live scripts S59 ([devtools/splice_browser](../guides/devtools.md#splice_browser)). Text ids decode to English strings (hint 810 = "Find and reprogram all 3 satellite dishes.").

## Camera command argument format (cutscene_player, v3.1)

From `devtools/cutscene_player/docs/relay_commands.txt`, checked against RELAY 0x6E3 live in S59 and RELAY 0x6B2 in S57.
- **CamBlendFrom / CamBlendTo (pose, angles, fov, dt, ...)** `[C:runtime]`: arg0 = position in game units; arg1 = **[roll, pitch, yaw] in degrees** (stored
  in RAM as radians); pitch positive = looking down; yaw = atan2(fwd.y, fwd.x) in game xy; +roll tilts toward camera-right; arg2 = **vertical fov in radians**
  (1.1 = default 63.03 deg). At S59 all six quantities (pos, roll, pitch, yaw, fov) match one blend fraction f = 0.204 between From#10 and To#13.
- Semantics `[H]` (strongly supported): `CamBlendFrom(pose, fov, dt)` starts a dt-second linear blend toward the pose of the **next** `CamBlendTo` in the list
  (look-ahead); a sync To waits for the running blend; a To with no pending From cuts. To arg3/arg4 are ints, probably `fCutOnLeave` / `iCurve` of CAMMOVE
  ([camera](camera.md)).
- **TweenTarget(pos, angles, dt)** (recon/binocucom): same pose convention, fov 1.0 rad; RELAY 0x6B2 matches S57 exactly `[C:runtime]`; dt = tween seconds `[H]`.
- Async flags live at RELAYCMD +4 in RAM but are **not in the level record** `[C:runtime, 0x6E3 only]`.

## 4. Source files

| Span | File (label) | Conf | Notes |
|---|---|---|---|
| 0x1471C0-0x147E20 | tu_1471C0 | low | CID 0x68 frame-capture transition; frame-capture transition |
| 0x1B2820-0x1B4438 | mpeg | high |  |
| 0x1DFAD8-0x1E5D80 | relay | low | RELAY + JSG script runner (Lilly jsg.c); Lilly: jsg.c; was labelled stepguard |

## 5. Key functions

| Addr | Name | Purpose | Conf | Tier | Evidence |
|---|---|---|---|---|---|
| [0x1B2940](../../src/game/gameplay/cutscene/mpeg/func_001B2940.c) | `StartupMpeg` | Create mpeg semaphore and start the display thread | 0.85 | T1 | verified |
| [0x1B29D0](../../src/game/gameplay/cutscene/mpeg/func_001B29D0.c) | `CQueueOutputIop_Init` | Init IOP output queue from audio params (0x10 = streaming mode), allocate IOP buffer (0x232EE8), Reset | 0.75 | T1 |  |
| [0x1B2EB8](../../src/game/gameplay/cutscene/mpeg/func_001B2EB8.c) | `CQueueOutputIop_Update` | Read IOP playback position and update read/write cursors | 0.75 | T1 |  |
| [0x1B2FD0](../../src/game/gameplay/cutscene/mpeg/func_001B2FD0.c) | `CQueueOutputIop_FAsyncDrain` | returns 0 | 0.80 | T1 | verified |
| [0x1B2FD8](../../src/game/gameplay/cutscene/mpeg/func_001B2FD8.c) | `CQueueOutputIpu_CbWrite` | Kick IPU_TO DMA (0x1000B400/10/20) with up to 0x1000 bytes | 0.85 | T1 |  |
| [0x1B3038](../../src/game/gameplay/cutscene/mpeg/func_001B3038.c) | `CQueueOutputIpu_FAsyncDrain` | returns 1 | 0.80 | T1 | verified |
| [0x1B3040](../../src/game/gameplay/cutscene/mpeg/func_001B3040.c) | `CMpegAudio_Init` | Stop game sound/music, allocate IOP voice resources for movie audio | 0.80 | T1 | verified |
| [0x1B3130](../../src/game/gameplay/cutscene/mpeg/func_001B3130.c) | `CMpegAudio_Reset` | reset byte queues and IOP queue | 0.85 | T1 | verified |
| [0x1B3190](../../src/game/gameplay/cutscene/mpeg/func_001B3190.c) | `CMpegAudio_Close` | release IOP resources | 0.80 | T1 | verified |
| [0x1B31E0](../../src/game/gameplay/cutscene/mpeg/func_001B31E0.c) | `CMpegAudio_FAccept` | accept demuxed audio packet: pick stream id, init queue from header, CbFill | 0.80 | T1 |  |
| [0x1B32B8](../../src/game/gameplay/cutscene/mpeg/func_001B32B8.c) | `CMpegAudio_Update` | drain byte queue to IOP per state | 0.80 | T1 | verified |
| [0x1B3388](../../src/game/gameplay/cutscene/mpeg/func_001B3388.c) | `FMpegAcceptVideo` | sceMpeg video stream callback: CbFill video queue | 0.80 | T1 | verified |
| [0x1B33C8](../../src/game/gameplay/cutscene/mpeg/func_001B33C8.c) | `FMpegAcceptAudio` | audio stream callback -> CMpegAudio::FAccept(pmpeg+0x180) | 0.80 | T1 | verified |
| [0x1B33F0](../../src/game/gameplay/cutscene/mpeg/func_001B33F0.c) | `FMpegDecodeVideo` | decode callback: feed IPU from video queue | 0.85 | T1 | verified |
| [0x1B34D0](../../src/game/gameplay/cutscene/mpeg/func_001B34D0.c) | `FMpegDecoderIdle` | idle callback: spool stream + audio update | 0.75 | T1 |  |
| [0x1B3508](../../src/game/gameplay/cutscene/mpeg/func_001B3508.c) | `FMpegDecoderError` | returns 1 | 0.80 | T1 | verified |
| [0x1B3510](../../src/game/gameplay/cutscene/mpeg/func_001B3510.cpp) | `BuildMpegGifs` | build GIF packets to upload decoded RGB32 picture | 0.75 | T1 | verified |
| [0x1B3B08](../../src/game/gameplay/cutscene/mpeg/func_001B3B08.cpp) | `CMpeg_ExecuteOids` | Play every queued movie (Execute until done), unless CbFreeMem<=0x41FC3F; called during level transition | 0.75 | T1 |  |
| [0x1B3BD0](../../src/game/gameplay/cutscene/mpeg/func_001B3BD0.cpp) | `CMpeg_Execute` | Movie loop: open async stream, Start, per frame MarkClockTick(paused)/UpdateJoy/UpdateCd, decode, Finish | 0.85 | T1 | verified |
| [0x1B3EC0](../../src/game/gameplay/cutscene/mpeg/func_001B3EC0.c) | `CMpeg_Start` | init byte queue, CMpegAudio::Init, sceMpegInit/Create, add callbacks | 0.80 | T1 |  |
| [0x1B4058](../../src/game/gameplay/cutscene/mpeg/func_001B4058.c) | `CMpeg_Update` | get decoded picture, BuildMpegGifs, subtitles, signal display thread | 0.80 | T1 | arbitration |
| [0x1B4140](../../src/game/gameplay/cutscene/mpeg/func_001B4140.c) | `CMpeg_Finish` | audio reset/close, sceMpegReset/Delete | 0.80 | T1 | verified |
| [0x1B41D0](../../src/game/gameplay/cutscene/mpeg/func_001B41D0.c) | `CMpeg_CbDemuxed` | spool async stream and demux into sceMpeg (returns 2 at EOF) | 0.85 | T1 | arbitration |
| [0x1DFAD8](../../src/game/gameplay/cutscene/relay/func_001DFAD8.c) | `InitRelay` | RELAY::Init (vt7): InitLo; crelaycmdMax(+0x58)=-1; InitDl(&dlJsg +0x5C, ibDle 0xE4) | 0.85 | T1 | verified |
| [0x1DFB10](../../src/game/gameplay/cutscene/relay/func_001DFB10.cpp) | `LoadRelayFromBrx` | RELAY::LoadFromBrx (vt14): build commands into a 0xA000-byte temp buffer (0x200 x 0x50), start with BlockBegin(-1), fAppendEnd=1, LoadLoFromBrx (the o | 0.80 | T1 | parse |
| [0x1DFBF8](../../src/game/gameplay/cutscene/relay/func_001DFBF8.c) | `CloneRelay` | RELAY::Clone (vt41): CloneLo; if the relay has a private command limit (+0x80) reallocate its own copy of the command array | 0.70 | T1 |  |
| [0x1DFC50](../../src/game/gameplay/cutscene/relay/func_001DFC50.c) | `UpdateRelay` | RELAY::Update (vt16): UpdateLo; while firing advance tFired (real-time dt if grf 0x100), UpdateJsg on every running script; StopRelay when none remain | 0.80 | T1 |  |
| [0x1DFD58](../../src/game/gameplay/cutscene/relay/func_001DFD58.c) | `FireRelay` | Start the relay (if not already firing): ploFirer=arg, fFiring=1, send itself msg 5, append BlockEnd if fAppendEnd, spawn a JSG for the top block (Pjs | 0.75 | T1 |  |
| [0x1DFEB8](../../src/game/gameplay/cutscene/relay/func_001DFEB8.c) | `StopRelay` | If firing: clear firer/state, stop every JSG (0x1E5628), clear the alarm list, release LO busy (0x1A6CA8(p,2)), send itself msg 6 | 0.75 | T1 |  |
| [0x1E0020](../../src/game/gameplay/cutscene/relay/EnsureSggAlarm__FP3SGGP5ALARM.cpp) | `EnsureRelayAlarm` | Add an ALARM to the relay's alarm list (+0x84 calarm, +0x88 apalarm[8]) if absent and room | 0.75 | existing | arbitration; replaces existing EnsureSggAlarm__FP3SGGP5ALARM |
| [0x1E00E8](../../src/game/gameplay/cutscene/relay/FWipingAseg__FP4ASEG.c) | `FRelayHasAlarm` | Return whether the alarm list (+0x84/+0x88) contains x | 0.75 | existing | arbitration; replaces existing FWipingAseg__FP4ASEG |
| [0x1E01F8](../../src/game/gameplay/cutscene/relay/func_001E01F8.c) | `PrelaycmdAddRelay` | Append a zeroed 0x50-byte RELAYCMD {relayck, fAsync} to the relay (limit crelaycmdMax or 0x200); on overflow return a scratch command D_003DA740 | 0.75 | T1 |  |
| [0x1E0290](../../src/game/gameplay/cutscene/relay/func_001E0290.c) | `AddRelayBlockBegin` | Splice builtin: append a RELAYCK 0x0 (BlockBegin) command to the relay (PrelaycmdAddRelay(prelay,0x0,fAsync)) and fill its args | 0.85 | T1 | verified |
| [0x1E0450](../../src/game/gameplay/cutscene/relay/func_001E0450.c) | `AddRelaySetCtlState` | Splice builtin: append a RELAYCK 0x7 (SetCtlState) command to the relay (PrelaycmdAddRelay(prelay,0x7,fAsync)) and fill its args | 0.85 | T1 | verified |
| [0x1E06F0](../../src/game/gameplay/cutscene/relay/func_001E06F0.c) | `AddRelayHandsOff` | Splice builtin: append a RELAYCK 0x12 (HandsOff) command to the relay (PrelaycmdAddRelay(prelay,0x12,fAsync)) and fill its args | 0.85 | T1 | verified |
| [0x1E0B40](../../src/game/gameplay/cutscene/relay/func_001E0B40.c) | `AddRelayFace` | Splice builtin: append a RELAYCK 0x23 (Face) command to the relay (PrelaycmdAddRelay(prelay,0x23,fAsync)) and fill its args | 0.85 | T1 | verified |
| [0x1E0ED8](../../src/game/gameplay/cutscene/relay/func_001E0ED8.c) | `AddRelayAddObject` | Splice builtin: append a RELAYCK 0x2C (AddObject) command to the relay (PrelaycmdAddRelay(prelay,0x2C,fAsync)) and fill its args | 0.85 | T1 | verified |
| [0x1E1710](../../src/game/gameplay/cutscene/relay/func_001E1710.c) | `AddRelayUnk49` | Splice builtin: append a RELAYCK 0x49 (Unk49) command to the relay (PrelaycmdAddRelay(prelay,0x49,fAsync)) and fill its args | 0.50 | T2 | verified |
| [0x1E1B58](../../src/game/gameplay/cutscene/relay/func_001E1B58.c) | `AddJsgRef` | cref++ (+0x10) | 0.70 | T1 |  |
| [0x1E1B68](../../src/game/gameplay/cutscene/relay/func_001E1B68.c) | `ReleaseJsg` | --cref; at 0 release sound ref, destruct and free to the JSG slot heap D_003E1560 | 0.70 | T1 |  |
| [0x1E1C40](../../src/game/gameplay/cutscene/relay/func_001E1C40.cpp) | `UpdateJsg` | Run a script: start at prelaycmdFirst if not started, then keep stepping (NextJsgCommand) while the current command is fAsync or complete (FIsJsgComma | 0.80 | T1 |  |
| [0x1E1D48](../../src/game/gameplay/cutscene/relay/func_001E1D48.c) | `ReleaseJsgJtIfComplete` | If the command holding the character (prelaycmdJt +0x40) is complete (StepCmd1D only when its goal is -1) release it (SetJsgJt(p,0,0)) | 0.75 | T1 |  |
| [0x1E1E30](../../src/game/gameplay/cutscene/relay/func_001E1E30.cpp) | `NextJsgCommand` | Advance the script to the next command and execute it by kind | 0.90 | T1 |  |
| [0x1E3890](../../src/game/gameplay/cutscene/relay/func_001E3890.cpp) | `FIsJsgCommandComplete` | Whether the given command has finished, by kind | 0.90 | T1 |  |

Selected 44 of 180 named functions (T1 with confidence >= 0.7, or runtime/verified/parse/arbitration evidence, or cited in the text). Full list: [reference/functions/cutscenes.md](../reference/functions/cutscenes.md).

## Open questions
- Real file name of the RELAY/JSG TU (must sort between the 0x1DE280 TU and shape, so not "jsg").
- RELAYCK names for the ~70 kinds not yet seen live (18 are runtime-confirmed).
- 0x1B2820 (returns 0 per movie); whether 0x1B3A48 is Sly 1's movie queue function.
- Experiment: break 0x1B3700 during a subtitled cutscene; break `FireRelay` when a cutscene starts and dump the RELAYCMD array.
