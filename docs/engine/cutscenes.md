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
- Fixes: existing names `EnsureSggAlarm` and `FWipingAseg` are RELAY alarm-list helpers `[H r6b fixes]`.

## 2. FMV playback (mpeg.cpp 0x1B2820-0x1B4438)

Sly 1 mpeg.c almost 1:1 (anchor `Reset__15CQueueOutputIop`) `[C:verified]` (CMpeg/CMpegAudio methods, r3r5 verifier).
Flow `[H r5 mpeg.md]`: queued by events (`CMpeg::ExecuteOids` 0x1B3B08 refuses if `CbFreeMem()` <= 0x41FC3F) -> `CMpeg::Execute` 0x1B3BD0 runs
its own loop (clock ticked paused, UpdateJoy, UpdateCd) -> `sceMpeg` decode with callbacks -> per frame spool the async stream and
`RenderPicture` (with **subtitles**, new in Sly 2, 0x1B3700) handing frames to a display thread. Audio: `CMpegAudio` (CMpeg+0x180) -> `CByteQueue`
-> IOP via `CQueueOutputIop`; video to the IPU via `CQueueOutputIpu`. Movies are raw MPEG-PS files located by FILERECs (boot logo 0x2A3868, others 0x2A3968/0x2A39A8) `[C:static disc_toc.md]`.

## 3. Screen transitions

- **Overlay** D_002DD644 drawn by main after the world (`DrawOverlay` 0x1477D8), states 0..2; while in state 2/3 the world render is skipped `[H r4 main.md]`.
- **Frame-capture transition** (CID 0x68, TU 0x1471C0): on start (Splice message 0x601) it captures the framebuffer to a texture, takes hands-off,
  changes the clock rate, and main draws two quads moved by child objects (split/peel effect) `[H r2b cid68.md]` (low confidence).
- **WIPE** and `WipeToWorldWarp`: see [world-sw](world-sw.md#4-transitions-warps-exits-wipes-boot-queue). **Letterbox**: [hud-ui](hud-ui.md).
- Camera blends during cut-scenes: CPCAMBLEND with a CAMMOVE `{pposeFrom, pposeTo, dt, fCutOnLeave, iCurve}` `[H r2 cplcy.md]`.

## 4. Source files

| Span | File (label) | Notes |
|---|---|---|
| 0x1471C0-0x147E20 | cid 0x68 TU | frame-capture transition |
| 0x1B2820-0x1B4438 | mpeg |  |
| 0x1DFAD8-0x1E5D80 | relay TU (RELAY + JSG) | Lilly: jsg.c; was labelled stepguard |

## 5. Key functions

| Addr | Name | Purpose | Conf | Status |
|---|---|---|---|---|
| [0x1B2940](../../src/hand/func_001B2940.c) | `StartupMpeg` | Create mpeg semaphore and start the display thread | 0.80 | VERIFIED |
| [0x1B29D0](../../src/hand/func_001B29D0.c) | `CQueueOutputIop::Init` | Init IOP output queue from audio params (0x10 = streaming mode), allocate IOP buffer (0x232EE8), Reset | 0.75 | HYP |
| [0x1B2EB8](../../src/hand/func_001B2EB8.c) | `CQueueOutputIop::Update` | Read IOP playback position and update read/write cursors | 0.75 | HYP |
| [0x1B2FD0](../../src/auto/func_001B2FD0.c) | `CQueueOutputIop::FAsyncDrain` | returns 0 | 0.80 | VERIFIED |
| [0x1B2FD8](../../src/hand/func_001B2FD8.c) | `CQueueOutputIpu::CbWrite` | Kick IPU_TO DMA (0x1000B400/10/20) with up to 0x1000 bytes | 0.85 | HYP |
| [0x1B3038](../../src/auto/func_001B3038.c) | `CQueueOutputIpu::FAsyncDrain` | returns 1 | 0.80 | VERIFIED |
| [0x1B3040](../../src/hand/func_001B3040.c) | `CMpegAudio::Init` | Stop game sound/music, allocate IOP voice resources for movie audio | 0.80 | VERIFIED |
| [0x1B3130](../../src/hand/func_001B3130.c) | `CMpegAudio::Reset` | reset byte queues and IOP queue | 0.80 | VERIFIED |
| [0x1B3190](../../src/hand/func_001B3190.c) | `CMpegAudio::Close` | release IOP resources | 0.80 | VERIFIED |
| [0x1B31E0](../../src/hand/func_001B31E0.c) | `CMpegAudio::FAccept` | accept demuxed audio packet: pick stream id, init queue from header, CbFill | 0.80 | HYP |
| [0x1B32B8](../../src/hand/func_001B32B8.c) | `CMpegAudio::Update` | drain byte queue to IOP per state | 0.80 | VERIFIED |
| [0x1B3388](../../src/hand/func_001B3388.c) | `FMpegAcceptVideo` | sceMpeg video stream callback: CbFill video queue | 0.80 | VERIFIED |
| [0x1B33C8](../../src/auto/func_001B33C8.c) | `FMpegAcceptAudio` | audio stream callback -> CMpegAudio::FAccept(pmpeg+0x180) | 0.80 | VERIFIED |
| [0x1B33F0](../../src/hand/func_001B33F0.c) | `FMpegDecodeVideo` | decode callback: feed IPU from video queue | 0.75 | VERIFIED |
| [0x1B34D0](../../src/hand/func_001B34D0.c) | `FMpegDecoderIdle` | idle callback: spool stream + audio update | 0.75 | HYP |
| [0x1B3508](../../src/auto/func_001B3508.c) | `FMpegDecoderError` | returns 1 | 0.80 | VERIFIED |
| [0x1B3510](../../src/hand/func_001B3510.cpp) | `BuildMpegGifs` | build GIF packets to upload decoded RGB32 picture | 0.75 | VERIFIED |
| [0x1B3B08](../../src/hand/func_001B3B08.cpp) | `CMpeg::ExecuteOids` | Play every queued movie (Execute until done), unless CbFreeMem<=0x41FC3F; called during level transition | 0.75 | HYP |
| [0x1B3BD0](../../src/hand/func_001B3BD0.cpp) | `CMpeg::Execute` | Movie loop: open async stream, Start, per frame MarkClockTick(paused)/UpdateJoy/UpdateCd, decode, Finish | 0.85 | VERIFIED |
| [0x1B3EC0](../../src/hand/func_001B3EC0.c) | `CMpeg::Start` | init byte queue, CMpegAudio::Init, sceMpegInit/Create, add callbacks | 0.80 | HYP |
| [0x1B4140](../../src/hand/func_001B4140.c) | `CMpeg::Finish` | audio reset/close, sceMpegReset/Delete | 0.80 | VERIFIED |
| [0x1B41D0](../../src/hand/func_001B41D0.c) | `CMpeg::Update` | spool async stream and demux into sceMpeg (returns 2 at EOF) | 0.70 | HYP |
| [0x1DFAD8](../../src/hand/func_001DFAD8.c) | `InitRelay` | RELAY::Init (vt7): InitLo; crelaycmdMax(+0x58)=-1; InitDl(&dlJsg +0x5C, ibDle 0xE4) | 0.80 | HYP |
| [0x1DFB10](../../src/hand/func_001DFB10.cpp) | `LoadRelayFromBrx` | RELAY::LoadFromBrx (vt14): build commands into a 0xA000-byte temp buffer (0x200 x 0x50), start with BlockBegin(-1), fAppendEnd=1, LoadLoFromBrx (the o | 0.75 | HYP |
| [0x1DFBF8](../../src/hand/func_001DFBF8.c) | `CloneRelay` | RELAY::Clone (vt41): CloneLo; if the relay has a private command limit (+0x80) reallocate its own copy of the command array | 0.70 | HYP |
| [0x1DFC50](../../src/hand/func_001DFC50.c) | `UpdateRelay` | RELAY::Update (vt16): UpdateLo; while firing advance tFired (real-time dt if grf 0x100), UpdateJsg on every running script; StopRelay when none remain | 0.80 | HYP |
| [0x1DFD58](../../src/hand/func_001DFD58.c) | `FireRelay` | Start the relay (if not already firing): ploFirer=arg, fFiring=1, send itself msg 5, append BlockEnd if fAppendEnd, spawn a JSG for the top block (Pjs | 0.75 | HYP |
| [0x1DFEB8](../../src/hand/func_001DFEB8.c) | `StopRelay` | If firing: clear firer/state, stop every JSG (0x1E5628), clear the alarm list, release LO busy (0x1A6CA8(p,2)), send itself msg 6 | 0.75 | HYP |
| [0x1E01F8](../../src/hand/func_001E01F8.c) | `PrelaycmdAddRelay` | Append a zeroed 0x50-byte RELAYCMD {relayck, fAsync} to the relay (limit crelaycmdMax or 0x200); on overflow return a scratch command D_003DA740 | 0.75 | HYP |
| [0x1E1B58](../../src/auto/func_001E1B58.c) | `JSG::AddRef (IncrefJsg)` | cref++ (+0x10) | 0.70 | HYP |
| [0x1E1B68](../../src/auto/func_001E1B68.c) | `JSG::Release (DecrefJsg)` | --cref; at 0 release sound ref, destruct and free to the JSG slot heap D_003E1560 | 0.70 | HYP |
| [0x1E1C40](../../src/hand/func_001E1C40.cpp) | `UpdateJsg` | Run a script: start at prelaycmdFirst if not started, then keep stepping (NextJsgCommand) while the current command is fAsync or complete (FIsJsgComma | 0.80 | HYP |
| [0x1E1D48](../../src/hand/func_001E1D48.c) | `ReleaseJsgJtIfComplete` | If the command holding the character (prelaycmdJt +0x40) is complete (StepCmd1D only when its goal is -1) release it (SetJsgJt(p,0,0)) | 0.75 | HYP |
| [0x1E1E30](../../src/hand/func_001E1E30.cpp) | `NextJsgCommand` | Advance the script to the next command and execute it by kind | 0.90 | HYP |
| [0x1E3890](../../src/hand/func_001E3890.cpp) | `FIsJsgCommandComplete` | Whether the given command has finished, by kind | 0.90 | HYP |

Selected 35 of 180 named functions (confidence >= 0.7, verified, or flagged). Full list: [reference/functions/cutscenes.md](../reference/functions/cutscenes.md).

## Open questions
- Real file name of the RELAY/JSG TU (must sort between the 0x1DE280 TU and shape, so not "jsg").
- RELAYCK enum names for all ~90 kinds (currently from Lilly's enum, prefixed AddRelay by the team).
- 0x1B2820 (returns 0 per movie); whether 0x1B3A48 is Sly 1's movie queue function.
- Experiment: break 0x1B3700 during a subtitled cutscene; break `FireRelay` when a cutscene starts and dump the RELAYCMD array.
