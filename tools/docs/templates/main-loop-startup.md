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

{{TUS}}

## 8. Key functions

{{KEYFUNCS:max=60 minc=0.6}}

## Open questions
- What D_002DD644 (overlay) is; never non-NULL in the snapshots.
- Unverified startup-table entries 0x21F100, 0x14B660, 0x15EF00, 0x1C90D0, 0x14CFE0, 0x1FCEA0, 0x138FC0, 0x211528, 0x1B2940.
- Experiment: breakpoint 0x1A7518 loop head and count iterations per vsync; NOP the RenderSw call (world should freeze while UI draws).
