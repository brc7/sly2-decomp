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

{{TUS}}

## 5. Key functions

{{KEYFUNCS:0x1E1E30 0x1E3890 0x1B3BD0 max=50 minc=0.7}}

## Open questions
- Real file name of the RELAY/JSG TU (must sort between the 0x1DE280 TU and shape, so not "jsg").
- RELAYCK names for the ~70 kinds not yet seen live (18 are runtime-confirmed).
- 0x1B2820 (returns 0 per movie); whether 0x1B3A48 is Sly 1's movie queue function.
- Experiment: break 0x1B3700 during a subtitled cutscene; break `FireRelay` when a cutscene starts and dump the RELAYCMD array.
