# Live game access over PINE

PINE is PCSX2's IPC protocol (read/write EE memory, save/load states) on a local socket. This page covers the bridge workflow used on 2026-11-01
and the addresses **confirmed in the running game** that night (`[C:live]`). Each live fact below was also re-checked in v3 against the matched
code (disassembly of the retail ELF) and/or the save-state RAM dumps; the check is named next to it.
The original PINE job logs (`research/pine_jobs`) are on the Mac and were not available when this page was written; the facts below are the record.

Tools: [devtools/pine_toolkit](devtools.md#pine_toolkit) (client, dashboard, recipes, warp), [save_editor](devtools.md#save_editor) (save-var jobs),
[ram_inspector](devtools.md#ram_inspector) (decode a dump). Older background: [playtesting](playtesting.md) (pnach, patched ELF, save states).

## 1. Setup

1. PCSX2 2.x: Settings > Advanced > **Enable PINE**, slot 28011 (default). The socket is `$TMPDIR/pcsx2.sock` (non-default slot: `pcsx2.sock.<slot>`).
2. Boot Sly 2 (SCUS-97316, CRC 07652DD9) and get into gameplay.
3. **One client at a time**: PCSX2's PINE server serves a single connection. Tools that hold the socket starve others; pine_toolkit's dashboard and
   reference bridge reconnect per request/job.

## 2. Two ways to drive it

- **DIRECT** (a human at the Mac): `python3 jobs.py read32 0x3D4B00`, `python3 jobs.py sv loot.ccoin 9999`, `python3 dash.py` (dashboard at
  http://127.0.0.1:8765), `python3 progression.py warp L08`.
- **QUEUE** (an agent without a socket): the agent writes a job JSON into `pine_jobs/inbox/<id>.json`; a bridge process on the Mac (`pine_bridge.py`, or
  pine_toolkit's `pine_bridge_ref.py`) runs it against PCSX2 and writes `pine_jobs/outbox/<id>.json`.

Job format (pine_toolkit README):
```
inbox:  {"id", "stop_on_error", "ops": [{"op":"read32","addr":A}, {"op":"write32","addr":A,"value":V},
         {"op":"dump","addr","len","name"}, {"op":"watch","addr","width","s","hz"}, {"op":"sleep","s"},
         {"op":"savestate"|"loadstate","slot"}, {"op":"version"|"status"|"title"|"game_id"|"uuid"}]}
outbox: {"id", "ok", "results": [{"op","ok","value"|"error"}]}
```
Floats are written as their raw IEEE-754 u32 (0.25 = 0x3E800000). Always save a state before writing, and restore after an experiment.

## 3. Confirmed addresses (2026-11-01)

| Address | What | Live result | Independent check (v3) |
|---|---|---|---|
| 0x3D4B00 | `loot.ccoin` = g_gs (0x3D4A60) + 0xA0 | wrote 777, the HUD showed 777 `[C:live]` | RAM: 6 in S02, 2698 in S71, 498 in S72 (after a 2,200-coin purchase) `[C:runtime]` |
| 0x2DDED0 | CLOCK: +0 t, +4 dt, +8 `rDt`, rate slots `art[0..8]` at +0x18..+0x38 | see section 4 `[C:live]` | `RecalcClockRate` 0x14D1F8 disassembly; S71/S72 RAM `[C:static]` `[C:runtime]` |
| 0x3E1080 (g_sw+0x3BF0) | transition request pending flag | write 1 -> transition runs `[C:live]` | `SetSwTransition` 0x20DF80: `sw 1, 0x3BF0(a0)` `[C:static]` |
| g_sw+0x3BF4 | TRANS flags (write 0) | `[C:live]` | first word of the 0x4C-byte TRANS copy `[C:static]` |
| g_sw+0x3BF8..+0x3C37 | masked FILEREC of the target level (16 words) | `[C:live]` | `WipeToWorldWarp` 0x22B490 struct {flags; CHK[16]; oid; b} `[C:static]` |
| g_sw+0x3C38 | warp oid 0x189 | `[C:live]` | 0x189 in every save state; `InitExit` sets oidWarp 0x189 `[C:runtime]` |
| g_sw+0x3C3C | -1 | `[C:live]` | -1 in every save state `[C:runtime]` |
| 0x3E1110 (g_sw+0x3C80) | current level index = disc level number Lnn | read back after the warp `[C:live]` | 2 in Paris S02, 8 in India S65-S73 `[C:runtime]` |

g_sw = 0x3DD490 (an object in place: vt_SW 0x2DC018 at +0xC).

## 4. Clock experiment

- Writing **`art[1]` = 0.25 alone** (0x2DDED0 + 0x1C) had **no effect**: the game rate stayed 1.0.
- Writing **`rDt` (+8) = 0.25 as well**: game time t advanced at **0.246x** real time.
- Why `[C:static]`: `rDt` is a cached product. `RecalcClockRate` 0x14D1F8 sets +8 = 1.0, multiplies it by the 9 slots +0x18..+0x38 and stores slot 8 to
  +0xC; only `SetClockRate` 0x14D1D0 (`slot[i] = r`, then `jal RecalcClockRate`) calls it. `MarkClockTick` 0x14D0B8 uses the cached `rDt`.
  Note `rDt` includes slot 8 (v2 said slots 0-7; corrected in [main-loop-startup](../engine/main-loop-startup.md)).
- Consistent with RAM `[C:runtime]`: S72 (Thief Reflexes active) art[1] = 0.25, rDt = 0.25, dt = 1/240; S71 (ThiefNet) art[0] = 0, rDt = 0.
- This confirms pine_toolkit's prediction (recipe `clock_slot_norecalc` predicted no slow-down; `clock_rdt` predicted 1/4 speed).

## 5. Level warp

Write, in one job: g_sw+0x3BF4 = 0; the 16 words of the target's masked FILEREC at g_sw+0x3BF8; g_sw+0x3C38 = 0x189; g_sw+0x3C3C = -1; then
g_sw+0x3BF0 = 1 last. The next frame's `ExecuteSwTransition` 0x20E890 loads the level. **Result**: Paris -> India L08, which loaded into the
safehouse character select `[C:live]`. FILEREC words: copy them from the level table of the loaded world (each world masks its copy with its own key
words but all decode to the same lba/size; [world-sw](../engine/world-sw.md)), or use `progression.py warp Lnn`, which reads them from
pine_toolkit `data/levels.json`. Save vars survive the warp (the save blob is not touched).

**live_lab session (07:06-07:33 UTC)** `[C:live]`: the same warp works for all 6 episode hubs tried (L12, L14, L17, L27, L32, L38; each loads in
5-9 s into the episode safehouse character select, level index = target, pending flag back to 0). **L37 drops straight into the Jean Bison boss
arena** with Sly ready to fight; L43 loads the blimp deck (boss start not seen). L41 er_bentley loads **with Sly**: the player comes from
`oidPoCurrent`, not the level. Bridge note: results come back as `"0x.."` strings, so `progression.py wait_level` (int compare) times out;
normalise with `int(v, 0)`.

Still `[H]`: warping from inside a cutscene (hands-off flags) and the `prepare <job>` flow.

## 5a. Input injection (pine_input)

Two paths, both `[C:live]` (reports/pine_input/inject.py):

| Path | How | Live result |
|---|---|---|
| A: force mask | OR a button mask into PO+0x154; it is OR'd into the POCTL buttons each frame | CROSS (0x40) held 0.15 s: jts 0 -> 2 within one frame, z +215; clear the mask after |
| B: freeze `g_joy` | write `joys` (g_joy 0x2DFC00 +0x100C) = 0 so the pad is no longer read, then write the stick/button fields yourself | stick (0, 1) for 45 frames moved Sly about 405 units; START pauses and unpauses; SQUARE/RIGHT/CROSS drive menus (memcard dialog, character select) |
| restore | `joys` = 3, force mask 0 | `tRead` advances again |

**Pause sets dt = 0** `[C:live]`: START via path B: dt 0.0167 -> 0.0 -> 0.0167.

**After any loadstate** PCSX2 shows "Memory Card removed - Auto-save disabled" and the game **pauses (rDt 0)** until SQUARE is pressed
`[C:live]`. Dismiss it before timing anything (it caused one false clock FAIL). Side benefit: auto-save is off. Also: `savestate` is asynchronous,
so wait at least 8 s before copying the file.

Other recipes run live: `loot.ccoin` 777 -> 1234; `sly.hpCur` via STEPPLAYER+0xE00 and `mpCur` via +0x12FC; `rDt` = 0.25 gives 0.249x; SW+0x3D78
hands-off counter sticks (visual effect not seen); guard +0x33C hp 40 -> 1 sticks; poking BHV156+0x30 mode = 4 does not change behaviour (the
prediction holds). Double-jump apex: see [player](../engine/player.md).

## 6. Next live checks

From pine_toolkit TESTPLAN and the open questions: jts 14 (triangle) context; gadgets_all / equip_slot / episodes_cheat / char_switch (save-var writes, not run); `unlock_all_episodes` (D_002BE26C); `reach_m1_boss` on an
early save; reading the moved car's world matrix in the modded ISO ([open-questions](../open-questions.md#modding-live-test-2026-11-01)); a
`BroadcastGameEvent` 0x1619A0 logger for game event ids.
