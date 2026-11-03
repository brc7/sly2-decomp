# Main loop, startup, clock and input

Related: [overview](overview.md) (boot and loop diagram), [world-sw](world-sw.md), [rendering](rendering.md).

## 1. main.cpp (0x1A7518-0x1A7C18)

| Addr | Name | Role | Tag |
|---|---|---|---|
| 0x1A7518 | `main` | entry; global ctors, Startup, StartupGame, infinite frame loop | [C:static] (only caller `_start`) |
| 0x1A7630 | `StartupSif` | sceSifInitRpc/sceCdInit/sceFsReset, load the embedded IRX modules (table D_004E5D80.., "ioprp300.img") | [C:verified] |
| 0x1A7890 / 0x1A7900 | `StartupVU0` / `StartupVU1` | upload VU microcode | [C:static] existing symbols |
| 0x1A7970 | `Startup` | run the 19-entry startup table D_002BFCE0 with a CProg progress bar; `MarkClockTick(g_clock,1)` per stage; prints build "0813.0032"/"america" | [C:verified] |

The startup table and the frame loop are listed in [overview.md](overview.md#1-boot) and
[overview.md](overview.md#2-the-frame-loop-exact-order). Differences from Sly 1 `[H r4 main.md]`: no separate
UpdateCodes/UpdateGameState calls (cheats and game state are driven from elsewhere), `UpdateSave` folded into
`UpdateSw`, the clock ticks before `UpdateSw` unconditionally, and an overlay object can suppress world rendering.

**Resolved** (CONFLICTS #3): 0x14D470 is `SetupCm(CM*)` (`SetupCm__FP2CM`), empty in Sly 2; its two callers are exactly Sly 1's SetupCm call sites
(main 0x1A75A0 before `MarkClockTick`, PostSwLoad 0x20E858); the startup table D_002BFCE0 has no CM entry; 0x14F3C0 is an unreferenced epilogue fragment
`[C:verified arbitration]`.

**Main-loop branch per state** `[C:runtime runtime5b]`: the overlay pointer D_002DD644 is NULL in all 55 snapshots, so RenderSw/DrawSw/DrawUi always run
when the loop runs (including the pause menu); during a level load the main thread is blocked in ExecuteSwTransition and frames come from the
loading-screen thread.

## 2. Clock (clock.cpp, 0x14CFE0-0x14D3D8)

`g_clock` = D_002DDED0 `[C:runtime]` (+0 t, +4 dt; t equals the save var `episode.<world>.dt`, dt = 1/60 in the save states).
- Reads the EE CP0 Count register, extends it to 64 bits with a wrap counter, converts ticks to seconds
  (x 1/294.912 MHz), clamps the frame delta to [1/60, 1/30] s `[H semantic clock.md]` (constant 3.3908420e-9 and the clamp are `[C:static]`).
- Game time is scaled by the product of **9 rate multiplier slots** inside CLOCK (Sly 1 had two globals); a second
  accumulated time ignores pause (used by cheat timing) `[H semantic clock.md]`. Gadgets use a rate slot for slow motion (`JT::UseGadget`), menus set rate 0.
- `MarkClockTick(&g_clock, fPaused)` 0x14D0B8 is called once per frame by main `[C:static]`.
- Runtime layout additions `[C:runtime runtime5b/6/7b]` (Lilly CLOCK field names): +0 t, +4 dt, +0xC rtCm, +0x10 dtCm, +0x14 tCm (the camera clock; round 5b called it
  tReal), **+0x3C tReal** (pause-independent; used by the loading screen, WIDGET131 and ThiefNet), +0x40 dtReal; rate slots `art[]`: **slot 0 pause menu / ThiefNet, slot 1
  power-up gadgets (Thief Reflexes 0.25, Time Rush 1.35), slot 7 widget pause (WIDGET128 via the count D_002DD318), slot 8 camera**; `rDt` (+8) = product of **all 9 slots 0-8** and +0xC = slot 8 (`RecalcClockRate` 0x14D1F8: loop of 9 `mul.s` over +0x18..+0x38, then `swc1` slot 8 to +0xC) `[C:static v3 disassembly]` (v2 said slots 0-7: **corrected**), while the
  camera uses dt x art[8] only (so it runs during pause and slow motion). Real-clock ASEGs (flag 0x100) use dtReal (UI animations). `ResetClock` sets t = 0 on every level load;
  `g_cframe` D_002BE538 is never reset (the loading thread bumps it too).
- **Live-confirmed layout and caching rule** `[C:live]` (PINE, 2026-11-01; see [live-pine](../guides/live-pine.md)): CLOCK at 0x2DDED0: +0 t, +4 dt, +8 `rDt`,
  rate slots `art[0..8]` at +0x18..+0x38. `rDt` is a **cached** product: writing `art[1] = 0.25` alone has **no effect** (game rate stays 1.0);
  writing `rDt` (+8) as well makes game t advance at 0.246x real time. Reason `[C:static v3 disassembly]`: only `SetClockRate` 0x14D1D0
  (`slot[i] = r; jal RecalcClockRate`) recomputes `rDt`, and `MarkClockTick` multiplies by the cached value. Save-state cross-check `[C:runtime v3]`:
  S72 (Thief Reflexes) art[1] = 0.25 and rDt = 0.25, dt = 1/240; S71 (ThiefNet) art[0] = 0 and rDt = 0, dt = 0; S02/S65/S73 all slots 1.0, rDt 1.0.
  A port or trainer must call `SetClockRate` (or write both) to change game speed.
- Startup: `SetupCm` 0x14D470 (was `StartupCm`) is empty in Sly 2 but keeps both Sly 1 call sites (main, PostSwLoad); the 19-entry startup table has no CM entry `[C:verified arbitration]`.

## 3. Pad input (joy.cpp, 0x187030-0x188488)

- `g_joy` = D_002DFC00; `g_grfusr` D_002DFBC4; `g_grfjoyt` D_002DFBC0; JOY fields +0x10B4 held, +0x10B8 pressed,
  +0x10BC handled, +0x10CC RUMBLE `[H r4b joy.md]`. `StartupJoy` 0x187030 (startup entry 9), `UpdateJoy` 0x1871C0 (frame step 2) `[C:static]`.
- Gameplay does not read the pad directly: each playable object has a **POCTL** (CID 0x89, 0xE0 bytes, at PO+0x150)
  with filtered stick values, button bits and press timestamps (`GrfPressedWithin`, `GrfConsumePressed`) `[H r5 po.md]` (verified names).
- Button masks seen in code use Sly 1's `PAD_*` values (Up 0x1000, Right 0x2000, Down 0x4000, Left 0x8000, R1 0x8, Start 0x800) `[H semantic cheat.md, r4 main.md]`.

## 4. Cheat codes (cheat.cpp, 0x14BEA8-0x14C548)

15 static code records at 0x2BD4E0 (stride 0x38): 6 button masks, a callback and a parameter. Every new press advances
or resets each code; one second after the last press the longest completed code fires, plays sound 0x34 and resets
`[H semantic cheat.md]` (records decoded from the ELF `[C:static]`). The span was mislabelled `splotheap`; its old names
`UnmarkAll__10CSplotheap`/`FreeGarbage__10CSplotheap` are wrong.

## 5. Rumble (rumble.cpp, 0x1D5150-0x1D57A8)

Existing names `InitRumble`, `UpdateRumble`, `TriggerRumbleRumk/Rumpat`, `SetRumbleRums` match Sly 1. Sly 2 adds a global mode
D_002C2D8C (0 unset, 1 follow save option, 2 forced on, 3 forced off; inferred) and a save-variable gate `[H r6 rumble.md]`.
The option is the save var `flags.fNoVibration` (option key k7) `[C:runtime]`.

## 6. Threads

thread.cpp (0x21EFD0): semaphores, critical sections (`SemaCreate`, `InitCritSect`, `EnterCritSect`), StartupThread `[C:static]` existing names.
Threads in the game: main, the render thread (`FrameRenderLoop`), the loading-screen thread (`LoadingScreenThread` 0x1C9190),
the memcard worker (`MemcardThreadMain` 0x1AAF18), and an MPEG display thread `[H r3 frm.md, r5 prog.md, r4 memcard.md, r5 mpeg.md]`.

## 7. Source files

| Span | File (label) | Conf | Notes |
|---|---|---|---|
| 0x14BEA8-0x14C548 | cheat | high | semantic/tu_docs/cheat.md; tu_table said splotheap; was labelled splotheap |
| 0x14CFE0-0x14D3D8 | clock | high | semantic/tu_docs/clock.md; tu_table said clip; was labelled clip |
| 0x187030-0x188488 | joy | high |  |
| 0x1A7518-0x1A7C18 | main | high |  |
| 0x1D5150-0x1D57A8 | rumble | high |  |

## 8. Key functions

| Addr | Name | Purpose | Conf | Tier | Evidence |
|---|---|---|---|---|---|
| [0x14BEA8](../../src/game/gameplay/progression/cheat.cpp) | `AddCode` | If not already in the list (0x11A1B8 ≈ FFindDlEntry), append it (AppendDlEntry 0x11A070); ijbcCur = (current buttons D_002E0CB4 == ajbc[0]) | 0.85 | T1 |  |
| [0x14BF18](../../src/game/gameplay/progression/cheat.cpp) | `_ResetCodes` | Sets ijbcCur = 0 on every code | 0.80 | existing | arbitration; replaces existing UnmarkAll__10CSplotheap |
| [0x14BF48](../../src/game/gameplay/progression/cheat.cpp) | `_MatchCodes` | Only for g_joy (D_002DFC00), on a new non-zero press (joy+0x10B4), and only while D_003DA0B8 is set: for each unfinished code, advance ijbcCur if the  | 0.85 | T1 |  |
| [0x14BFF8](../../src/game/gameplay/progression/cheat.cpp) | `UpdateCodes` | When tCheck has passed: drop codes flagged fRemove; pick the completed code with the largest cjbc; StartSound(0x34) via 0x1FBB90/0x1FB2F8; pfn(nParam) | 0.85 | T1 |  |
| [0x14C1F0](../../src/game/gameplay/progression/cheat.cpp) | `CheatSetFlag` | *pf = 1 | 0.70 | T1 |  |
| [0x14C258](../../src/game/gameplay/progression/cheat.cpp) | `ClearCheatItems` | If D_002E1E40: find item 0x1F in it (0x141E78), remove it (0x141FC8), then 0x1DAF38(0x1F, 0) | 0.65 | T1 |  |
| [0x14C2B0](../../src/game/gameplay/progression/cheat.cpp) | `CheatGiveGadget` | 0x1DAF38(nParam, 1); 0x19F0A8(g_sw) (refresh) | 0.65 | T1 |  |
| [0x14C2D8](../../src/game/gameplay/progression/cheat.cpp) | `StartupCodes` | InitDl(pdl, 0x18) (DLE at CODE+0x18), then AddCode for the 15 records 0x2BD4E0..0x2BD7F0 | 0.85 | T1 |  |
| [0x14CFE0](../../src/game/core/time/clock/StartupClock.c) | `StartupClock` | Seeds the tick history from CP0 Count, g_clock.tick = TickNow(), sets all 9 rate slots and rDt/rDtReal to 1, calls ResetClock | 0.85 | T1 |  |
| [0x14D068](../../src/game/core/time/clock/ResetClock.c) | `ResetClock` | t = 0; resets slots 0,1,2,4-8 to 1.0 (**not slot 3**), recomputes the rate | 0.65 | T1 |  |
| [0x14D0B8](../../src/game/core/time/clock/MarkClockTick.c) | `MarkClockTick` | dt = clamp(Δtick·1/294.912MHz, 1/60, 1/30); dtReal = dt·rDtReal, tReal += dtReal; dt40 = dt·slot[3]; if not paused, dt = dt40·rDt, t += dt, and pumps  | 0.85 | T1 |  |
| [0x14D1D0](../../src/game/core/time/clock/func_0014D1D0.c) | `SetClockRate` | slot[islot] = r; RecalcClockRate() | 0.70 | T1 |  |
| [0x14D1F8](../../src/game/core/time/clock/RecalcClockRate.c) | `RecalcClockRate` | rDt = Π slot[0..8], rDtReal = slot[8] | 0.85 | T1 |  |
| [0x14D240](../../src/game/core/time/clock/TickNow.c) | `TickNow` | 64-bit tick: CP0 Count masked to 32 bits; if less than last, wrap counter++ (state at +0x50/+0x58) | 0.85 | T1 |  |
| [0x187030](../../src/game/platform/input/joy/StartupJoy__Fv.c) | `StartupJoy` | UpdateGrfjoytFromGrfusr, scePadInit(0), InitJoy(&g_joy D_002DFC00, 0, 0), then init D_002DDEC0 (0x14C2D8) | 0.90 | T1 |  |
| [0x187078](../../src/game/platform/input/joy/AddGrfusr__Fi.c) | `AddGrfusr` | g_grfusr (D_002DFBC4) | 0.90 | T1 | verified |
| [0x1870A0](../../src/game/platform/input/joy/RemoveGrfusr__Fi.c) | `RemoveGrfusr` | g_grfusr &= ~mask; UpdateGrfjoytFromGrfusr | 0.90 | T1 | verified |
| [0x1870D0](../../src/game/platform/input/joy/UpdateGrfjoytFromGrfusr__Fv.c) | `UpdateGrfjoytFromGrfusr` | g_grfjoyt (D_002DFBC0) = 5/0/5/4/7 from grfusr bits 0x10/0xC/1/2; if not bit 2 reset D_002E0CBC=0xFFFF | 0.85 | T1 |  |
| [0x187150](../../src/game/platform/input/joy/InitJoy__FP3JOYii.c) | `InitJoy` | SetJoyJoys(searching), store port/slot, scePadPortOpen, enable rumble, gain 1.0 | 0.85 | T1 |  |
| [0x1875B8](../../src/game/platform/input/joy/SetJoyJoys__FP3JOY4JOYS4JOYK.c) | `SetJoyJoys` | Change pad state/kind: InitRumble when kind becomes >=3 (dualshock), clear analog block on kind 4 changes, reset button state when state changes to/fr | 0.75 | T1 | verified |
| [0x187DE0](../../src/game/platform/input/joy/SetJoyBtnHandled.c) | `SetJoyBtnHandled` | Clear mask from pressed (x10B8) and add held&mask to handled (x10BC) | 0.75 | T1 |  |
| [0x187E28](../../src/game/platform/input/joy/TriggerJoyRumbleRumk__FP3JOY4RUMKf.c) | `TriggerJoyRumbleRumk` | If rumble enabled and pad is a ready dualshock (joys>=3, joyk==3) TriggerRumbleRumk on the JOY's RUMBLE | 0.80 | T1 | verified |
| [0x187E78](../../src/game/platform/input/joy/StartJoySelection__FP3JOY.c) | `StartJoySelection` | Reset the menu selection repeat state (two fields = 0) | 0.55 | T1 | verified |
| [0x187E88](../../src/game/platform/input/joy/DxSelectionJoy.c) | `DxSelectionJoy` | Menu left/right with auto-repeat from d-pad 0x8000/0x2000 or stick (mag>=0.8, | 0.70 | T1 |  |
| [0x1880D8](../../src/game/platform/input/joy/DySelectionJoy.c) | `DySelectionJoy` | Menu up/down with auto-repeat from d-pad 0x1000/0x4000 or stick | 0.70 | T1 |  |
| [0x1A7518](../../src/game/app/main.cpp) | `main` | Program entry: store argc/argv, Startup(), StartGame-like init 0x1613A8, then the infinite frame loop (see tu_docs/main.md) | 0.95 | T1 | verified |
| [0x1A7630](../../src/game/app/main.cpp) | `StartupSif` | IOP bring-up: sceSifInitRpc, reboot IOP with ioprp300.img unless D_002E2058, sceCdInit/SetCdMmode, sceFsReset, alloc IOP heap and sceSifLoadModuleBuff | 0.90 | T1 | verified |
| [0x1A7890](../../src/game/app/main.cpp) | `StartupVU0__Fv` | DMA VU0 microcode (D_00259DF0) via sceDmaSend/sceDmaSync | 0.95 | existing | verified |
| [0x1A7900](../../src/game/app/main.cpp) | `StartupVU1__Fv` | DMA VU1 microcode (D_0025E050) | 0.95 | existing | verified |
| [0x1A7970](../../src/game/app/main.cpp) | `Startup` | Run the 19-entry startup table D_002BFCE0 with a CProg progress bar (shown only after StartupGs), MarkClockTick(g_clock,1) per stage; prints build "08 | 0.95 | T1 | verified |
| [0x1D5150](../../src/game/platform/input/rumble.cpp) | `InitRumble__FP6RUMBLEii` | Init RUMBLE: state Idle (or Stop if already active), store port/slot | 0.90 | existing | verified |
| [0x1D51B8](../../src/game/platform/input/rumble.cpp) | `UpdateRumble__FP6RUMBLE` | Step rumble pattern instructions by real dt, stop when done | 0.90 | existing | verified |
| [0x1D5310](../../src/game/platform/input/rumble.cpp) | `TriggerRumbleRumk__FP6RUMBLE4RUMKf` | Trigger predefined pattern D_002C2260[rumk] | 0.90 | existing | verified |
| [0x1D5468](../../src/game/platform/input/rumble.cpp) | `StopRumbleActuators__FP6RUMBLE` | Zero both motors | 0.90 | existing | verified |
| [0x1D5498](../../src/game/platform/input/rumble.cpp) | `EnsureRumbleModeDefault` | If rumble mode D_002C2D8C == 0 set it to 1 (follow save setting) | 0.50 | existing | arbitration; replaces existing RumbleUnknown1__Fv |
| [0x1D55B8](../../src/game/platform/input/rumble.cpp) | `FRumbleEnabled` | Mode 2 -> 1; 0/3 -> 0; else !save var D_003E0FD8 | 0.60 | T1 |  |

Selected 36 of 48 named functions (T1 with confidence >= 0.6, or runtime/verified/parse/arbitration evidence, or cited in the text). Full list: [reference/functions/main-loop-startup.md](../reference/functions/main-loop-startup.md).

## Open questions
- What D_002DD644 (overlay) is; never non-NULL in the snapshots.
- Unverified startup-table entries 0x21F100, 0x14B660, 0x15EF00, 0x1C90D0, 0x14CFE0, 0x1FCEA0, 0x138FC0, 0x211528, 0x1B2940.
- Experiment: breakpoint 0x1A7518 loop head and count iterations per vsync; NOP the RenderSw call (world should freeze while UI draws).
