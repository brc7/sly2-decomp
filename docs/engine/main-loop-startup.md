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

**Disputed name** (CONFLICTS #3): 0x14D470 `StartupCm__Fv` is an empty function called every frame with `&g_cm`; it
sits where Sly 1 calls `SetupCm`. Semantic audit and r4 say "SetupCm slot, misnamed"; r2 says the evidence conflicts.
Status: open `[H]`.

## 2. Clock (clock.cpp, 0x14CFE0-0x14D3D8)

`g_clock` = D_002DDED0 `[C:runtime]` (+0 t, +4 dt; t equals the save var `episode.<world>.dt`, dt = 1/60 in the save states).
- Reads the EE CP0 Count register, extends it to 64 bits with a wrap counter, converts ticks to seconds
  (x 1/294.912 MHz), clamps the frame delta to [1/60, 1/30] s `[H semantic clock.md]` (constant 3.3908420e-9 and the clamp are `[C:static]`).
- Game time is scaled by the product of **9 rate multiplier slots** inside CLOCK (Sly 1 had two globals); a second
  accumulated time ignores pause (used by cheat timing) `[H semantic clock.md]`. Gadgets use a rate slot for slow motion (`JT::UseGadget`), menus set rate 0.
- `MarkClockTick(&g_clock, fPaused)` 0x14D0B8 is called once per frame by main `[C:static]`.

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

| Span | File (label) | Notes |
|---|---|---|
| 0x14BEA8-0x14C548 | cheat | was labelled splotheap |
| 0x14CFE0-0x14D3D8 | clock | was labelled clip |
| 0x187030-0x188488 | joy |  |
| 0x1A7518-0x1A7C18 | main |  |
| 0x1D5150-0x1D57A8 | rumble |  |

## 8. Key functions

| Addr | Name | Purpose | Conf | Status |
|---|---|---|---|---|
| [0x14BEA8](../../src/hand/func_0014BEA8.c) | `AddCode` | If not already in the list (0x11A1B8 ≈ FFindDlEntry), append it (AppendDlEntry 0x11A070); `ijbcCur = (current buttons D_002E0CB4 == ajbc[0])` | 0.85 | HYP |
| [0x14BF18](../../src/auto/UnmarkAll__10CSplotheap.c) | `_ResetCodes` | Sets `ijbcCur = 0` on every code | 0.85 | HYP |
| [0x14BF48](../../src/hand/func_0014BF48.c) | `_MatchCodes` | Only for `g_joy` (D_002DFC00), on a new non-zero press (`joy+0x10B4`), and only while D_003DA0B8 is set: for each unfinished code, advance `ijbcCur` i | 0.85 | HYP |
| [0x14BFF8](../../src/hand/func_0014BFF8.c) | `UpdateCodes` | When `tCheck` has passed: drop codes flagged `fRemove`; pick the completed code with the largest `cjbc`; `StartSound(0x34)` via 0x1FBB90/0x1FB2F8; `pf | 0.85 | HYP |
| [0x14C1F0](../../src/hand/func_0014C1F0.c) | `CheatSetFlag` | `*pf = 1` | 0.85 | HYP |
| [0x14C200](../../src/auto/func_0014C200.c) | `CheatBuyItem1F?` | If 0x1D82B8(&D_003E0EA8, ...) ≥ 100, 0x1DAF38(0x1F, 1); else play error sound 0x3E | 0.60 | HYP |
| [0x14C258](../../src/auto/func_0014C258.c) | `ClearCheatItems` | If D_002E1E40: find item 0x1F in it (0x141E78), remove it (0x141FC8), then 0x1DAF38(0x1F, 0) | 0.60 | HYP |
| [0x14C2B0](../../src/hand/func_0014C2B0.c) | `CheatGiveGadget` | 0x1DAF38(nParam, 1); 0x19F0A8(g_sw) (refresh) | 0.60 | HYP |
| [0x14C2D8](../../src/auto/func_0014C2D8.c) | `StartupCodes` | `InitDl(pdl, 0x18)` (DLE at CODE+0x18), then AddCode for the 15 records 0x2BD4E0..0x2BD7F0 | 0.85 | HYP |
| [0x14CFE0](../../src/hand/func_0014CFE0.c) | `StartupClock` | Seeds the tick history from CP0 Count, `g_clock.tick = TickNow()`, sets all 9 rate slots and `rDt`/`rDtReal` to 1, calls ResetClock | 0.85 | HYP |
| [0x14D068](../../src/hand/func_0014D068.c) | `ResetClock` | `t = 0`; resets slots 0,1,2,4-8 to 1.0 (**not slot 3**), recomputes the rate | 0.60 | HYP |
| [0x14D0B8](../../src/hand/func_0014D0B8.c) | `MarkClockTick` | `dt = clamp(Δtick·1/294.912MHz, 1/60, 1/30)`; `dtReal = dt·rDtReal`, `tReal += dtReal`; `dt40 = dt·slot[3]`; if not paused, `dt = dt40·rDt`, `t += dt` | 0.85 | HYP |
| [0x14D1D0](../../src/hand/func_0014D1D0.c) | `SetClockRate` | `slot[islot] = r; RecalcClockRate()` | 0.85 | HYP |
| [0x14D1F8](../../src/hand/func_0014D1F8.c) | `RecalcClockRate` | `rDt = Π slot[0..8]`, `rDtReal = slot[8]` | 0.85 | HYP |
| [0x14D240](../../src/hand/func_0014D240.c) | `TickNow` | 64-bit tick: CP0 Count masked to 32 bits; if less than last, wrap counter++ (state at +0x50/+0x58) | 0.85 | HYP |
| [0x187030](../../src/auto/func_00187030.c) | `StartupJoy` | UpdateGrfjoytFromGrfusr, scePadInit(0), InitJoy(&g_joy D_002DFC00, 0, 0), then init D_002DDEC0 (0x14C2D8) | 0.90 | HYP |
| [0x187078](../../src/auto/func_00187078.c) | `AddGrfusr` | g_grfusr (D_002DFBC4)  | 0.90 | VERIFIED |
| [0x1870A0](../../src/auto/func_001870A0.c) | `RemoveGrfusr` | g_grfusr &= ~mask; UpdateGrfjoytFromGrfusr | 0.90 | VERIFIED |
| [0x1870D0](../../src/hand/func_001870D0.c) | `UpdateGrfjoytFromGrfusr` | g_grfjoyt (D_002DFBC0) = 5/0/5/4/7 from grfusr bits 0x10/0xC/1/2; if not bit 2 reset D_002E0CBC=0xFFFF | 0.85 | HYP |
| [0x187150](../../src/hand/func_00187150.c) | `InitJoy` | SetJoyJoys(searching), store port/slot, scePadPortOpen, enable rumble, gain 1.0 | 0.85 | HYP |
| [0x1875B8](../../src/hand/func_001875B8.c) | `SetJoyJoys` | Change pad state/kind: InitRumble when kind becomes >=3 (dualshock), clear analog block on kind 4 changes, reset button state when state changes to/fr | 0.75 | VERIFIED |
| [0x187DE0](../../src/auto/func_00187DE0.c) | `SetJoyBtnHandled` | Clear mask from pressed (x10B8) and add held&mask to handled (x10BC) | 0.75 | HYP |
| [0x187E28](../../src/auto/func_00187E28.c) | `TriggerJoyRumbleRumk` | If rumble enabled and pad is a ready dualshock (joys>=3, joyk==3) TriggerRumbleRumk on the JOY's RUMBLE | 0.80 | VERIFIED |
| [0x187E78](../../src/hand/func_00187E78.c) | `StartJoySelection` | Reset the menu selection repeat state (two fields = 0) | 0.55 | VERIFIED |
| [0x187E88](../../src/hand/func_00187E88.c) | `DxSelectionJoy` | Menu left/right with auto-repeat from d-pad 0x8000/0x2000 or stick (mag>=0.8,  | 0.70 | HYP |
| [0x1880D8](../../src/hand/func_001880D8.c) | `DySelectionJoy` | Menu up/down with auto-repeat from d-pad 0x1000/0x4000 or stick | 0.70 | HYP |
| [0x1A7518](../../src/hand/func_001A7518.c) | `main` | Program entry: store argc/argv, Startup(), StartGame-like init 0x1613A8, then the infinite frame loop (see tu_docs/main.md) | 0.95 | VERIFIED |
| [0x1A7630](../../src/hand/func_001A7630.c) | `StartupSif` | IOP bring-up: sceSifInitRpc, reboot IOP with ioprp300.img unless D_002E2058, sceCdInit/SetCdMmode, sceFsReset, alloc IOP heap and sceSifLoadModuleBuff | 0.90 | VERIFIED |
| [0x1A7890](../../src/auto/StartupVU0__Fv.c) | `StartupVU0` | DMA VU0 microcode (D_00259DF0) via sceDmaSend/sceDmaSync | 0.95 | VERIFIED |
| [0x1A7900](../../src/auto/StartupVU1__Fv.c) | `StartupVU1` | DMA VU1 microcode (D_0025E050) | 0.95 | VERIFIED |
| [0x1A7970](../../src/hand/func_001A7970.c) | `Startup` | Run the 19-entry startup table D_002BFCE0 with a CProg progress bar (shown only after StartupGs), MarkClockTick(g_clock,1) per stage; prints build "08 | 0.95 | VERIFIED |
| [0x1D5150](../../src/auto/InitRumble__FP6RUMBLEii.c) | `InitRumble` | Init RUMBLE: state Idle (or Stop if already active), store port/slot | 0.90 | VERIFIED |
| [0x1D51B8](../../src/hand/UpdateRumble__FP6RUMBLE.cpp) | `UpdateRumble` | Step rumble pattern instructions by real dt, stop when done | 0.90 | VERIFIED |
| [0x1D5310](../../src/hand/TriggerRumbleRumk__FP6RUMBLE4RUMKf.cpp) | `TriggerRumbleRumk` | Trigger predefined pattern D_002C2260[rumk] | 0.90 | VERIFIED |
| [0x1D5340](../../src/auto/TriggerRumbleRumpat__FP6RUMBLEP6RUMPATf.c) | `TriggerRumbleRumpat` | Start a rumble pattern for dt if pad active and FRumbleEnabled | 0.90 | HYP |
| [0x1D53C0](../../src/hand/SetRumbleRums__FP6RUMBLE4RUMS.cpp) | `SetRumbleRums` | Rumble state machine transition (scePadSetActDirect) | 0.90 | HYP |
| [0x1D5468](../../src/hand/StopRumbleActuators__FP6RUMBLE.cpp) | `StopRumbleActuators` | Zero both motors | 0.90 | VERIFIED |
| [0x1D55B8](../../src/hand/func_001D55B8.c) | `FRumbleEnabled` | Mode 2 -> 1; 0/3 -> 0; else !save var D_003E0FD8 | 0.60 | HYP |

Selected 38 of 48 named functions (confidence >= 0.6, verified, or flagged). Full list: [reference/functions/main-loop-startup.md](../reference/functions/main-loop-startup.md).

## Open questions
- 0x14D470: empty SetupCm stub or a real StartupCm? (CONFLICTS #3).
- Unverified startup-table entries 0x21F100, 0x14B660, 0x15EF00, 0x1C90D0, 0x14CFE0, 0x1FCEA0, 0x138FC0, 0x211528, 0x1B2940.
- Experiment: breakpoint 0x1A7518 loop head and count iterations per vsync; NOP the RenderSw call (world should freeze while UI draws).
