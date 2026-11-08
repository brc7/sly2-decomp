# UI and HUD

The 2-D layer on top of the world: HUD widgets (coin counter, health meter, banners, menus drawn as sprite animations),
the screen objects that own menu logic (pause menu, gadget menu, ThiefNet, job list, title, memory-card dialogs), text
(fonts, rich text, the text table) and full-screen presentation (letterbox, loading screen, progress bar). For the
summary see [engine.md section 7.9](../engine.md#79-hud-and-ui-enginepresentation-enginefont-ui); how the HUD pass fits
into the frame is in [rendering.md](rendering.md#4-passes-kinds-and-the-hook-table).

## 1. Two layers: widgets and screens

- **WIDGET** objects are **level objects** (an LO subclass; class cid 0x7E and subclasses): they are loaded from the
  world file with their sprite animations, have an oid, live in the world, and draw themselves in the HUD render pass.
  A widget is the *look*: animations, layout, texts.
- **Screens** are **global C++ objects** (a `BASIC`-derived family with no class ids), constructed at boot and reused
  across levels. A screen is the *logic*: input handling, state machines, what to show. After each level load a screen
  binds to its widget by oid (`PostLoadScreens`), then drives it with show/hide and animation calls.

So a menu is usually a screen (logic, in `engine/presentation/screen.cpp` or `src/game/ui/`) plus a widget (art, in the
level data, run by `engine/presentation/hud.cpp`).

## 2. Main types

| Type | What it is | Where |
|---|---|---|
| `WIDGET` | HUD element: state (unloaded, hidden, visible, appearing, disappearing), requested state, sprite groups, argument text ids, display timeout, exclusive flag and priority | `engine/presentation/hud.cpp`; class in `include/sly2/widget.h`, its records in `include/sly2/hud.h` |
| WIDGET subclasses | modal message box (WIDGET127), job-completion banner (WIDGET128, pauses the game clock), item/objective counter (WIDGET129), recon selector prompt (WIDGET130), episode menu (WIDGET131); the letterbox bars are a plain WIDGET | `hud.cpp`, the episode-menu unit in `src/game/ui/`; classes in `include/sly2/classes.h` |
| sprite group / sprite item / sprite key | the widget's animated 2-D content: items are text, geometry or picture payloads (or nested sprites); keys hold poses (2-D transform, colours) and sound events over time | `hud.cpp` |
| `STRINGGEN` | the text table: entries `{text id, getter, context}`; a getter builds dynamic text (coin count, timer, subtitle...) | `hud.cpp` (getters `Build*Text`) |
| `SCREEN` family (sly2/screen.h) | global UI controllers: HUD meter, notice banner, gadget menu (`GADGETMENU`), ThiefNet, job list, title screen, timed screen, pause menu, memory-card dialogs | `screen.cpp`, `src/game/ui/menu.cpp`, `src/game/ui/memcard.cpp` |
| `CFont` / `CFontBrx` | bitmap font: glyph table, texture in GS memory, scaling stack | `engine/font/font.cpp` |
| `CRichText` | text with inline escapes, wrapping, layout and drawing through `GIFS` | `font.cpp`; a second copy for sprite text in `hud.cpp` |
| `CProg` | the loading progress bar | `engine/presentation/prog.cpp` |
| `WIPE` | the screen wipe used for level transitions, updated with the screens | `src/game/level/wipe.cpp` (`UpdateWipe`) |

## 3. Lifecycle

### Boot

`StartupFont` (built-in fonts), `StartupLoadingScreen` (loading-screen thread), `StartupHudGifPackets` and
`StartupMemcard` run from the startup table. The screen objects and memory-card dialogs are C++ globals.

### Level load and teardown

- Widgets load with the level (`LoadWidgetFromBrx`: animation ids, the LO part, size, duration, font flag, sprite
  tables via `LoadSpriteTable`; some widgets stream their sprites in on demand). `PostWidgetLoad` finds the font object,
  centres the widget from its size and shows it if flagged.
- `PostLoadScreens` runs every screen's post-load (screen slot 12), which finds its widget (`PostHudMeterLoad`,
  `PostNoticeLoad`, ...). `InitLetterbox` binds the letterbox to its timer widget.
- On teardown `ResetScreens` (slot 13 on each screen) and `ResetLetterbox` forget widget pointers, because the widgets go
  with the old world.
- Fonts that live in the level are posted with `PostFontsLoad`.

### Per frame

1. **World update** (`UpdateSw`): live widgets run `UpdateWidget` (LO slot 16): act on the requested state, advance
   sprite animations (`AdvanceSpriteAnim`, `ApplySpriteKeyPoses`), time out. Widget subclasses add their own logic (the
   item counter recounts its objects every frame and writes done/total; the modal box waits for a button). The SW
   timer HUD (`UpdateSwTimerHud`) slides the coin/clue/countdown widgets into place and ticks the countdown sound.
2. **`UpdateScreens`** (main loop, after the world): Start opens the pause menu (`TryOpenPauseMenu`; also opened
   automatically after a while in some pad state, inferred: a disconnected controller), then the Update of each global
   screen in a fixed order: pause menu, ThiefNet, gadget menu, cheat codes, wipe, HUD meter, notice, job list,
   memory-card dialogs, timed screen, title screen.
3. **Render**: `RenderSw` calls `WIDGET::RenderSelf` on live widgets, which queue their sprite items
   (`QueueSpriteItem`, `RenderSpritePayload`) as HUD-kind render entries; `DrawSw` runs them in the HUD pass
   (`BeginRpHud` / `EndRpHud`, a VU1 microprogram) after the 3-D kinds, and only the foreground kinds draw after it.
4. **`DrawUi`** (main loop, after `DrawSw`): the draw slots of the main screens (for example the gadget-menu slot
   labels), then sends the two HUD DMA streams.

## 4. Widgets in detail

- **Show/hide**: `ShowWidget` (slot 49, loading sprites first if needed) and `HideWidget` (slot 50) set a request;
  `UpdateWidget` performs the appear/disappear animations and state changes. `SetWidgetRequest`, `IsWidgetIdle`,
  `IsWidgetVisible`, `ResetWidgetVisibleTime` are the helpers. `ShowWidgetArgs` / `ShowWidgetArgsFromSplice` pass up to
  ten argument text ids the widget displays.
- **Animation**: each widget has a main animation track and named animation ids; `PlayWidgetAnim`,
  `StopWidgetAnim`, `SwitchWidgetAnim`, `SetWidgetAnimPhase` (meters and gauges seek by fraction), `SetWidgetAnimTime`,
  `SetWidgetXf` / `SetWidgetAnimXf` (2-D transform overrides).
- **Exclusivity**: an exclusive widget hides lower-priority widgets while shown (`SetWidgetExclusive`,
  `IsWidgetObscured`).
- **Sprites**: sprite items are text (rich text drawn at the item's transform), geometry or pictures (a texture packet);
  keys interpolate transforms and colours and can trigger sounds (`FinishSpriteSoundEvents`).
- **3-D in the HUD**: a widget can show a live 3-D object through a render target (`AcquireRenderTarget`; see
  [rendering.md](rendering.md#9-render-targets)).
- Splice and RELAY commands show and hide widgets, so many HUD behaviours live in level scripts.

## 5. Text

- **Text ids** index the world's localized strings; dynamic texts are STRINGGEN entries whose getters build the string
  on demand (`BuildCoinCountText`, `BuildClueCountText`, `BuildTimerText`, `BuildSubtitleText`, ...; screens add their
  own: `BuildThiefnetInfoText`, `BuildJobNameText`, `BuildTitlePromptText`, ...). A new dynamic text is a new table
  entry plus a getter.
- **Rich text escapes** (`CRichText::ReadNextChar`): `~RRGGBB` sets a colour, `~.` restores the base colour, `&N` switches
  font, `&.` restores it, `^` escapes push/pop a scale; doubling a character escapes it. The sprite-text copy in
  `hud.cpp` handles the `&` family.
- **Fonts**: `CFontBrx` holds a glyph table (binary-searched via a remap table), uploads its texture into GS memory
  (`EnsureFontLoaded`, `CFontBrx::SwitchTex0`) and draws sprites per glyph through `GIFS` (`DrawChar`). `PushScaling` /
  `PopScaling` scale relative to the base. `CRichText::Draw` justifies, wraps and clips into a text box.
- **Subtitles** come from the sound system's subtitle record ([sound-and-music.md](sound-and-music.md#6-dialogue-and-subtitles))
  through the subtitle text getters.
- **Shift-JIS** (`engine/font/sjis.cpp`): only for the memory-card icon title (`ConvertStringToSjis`).

## 6. Screens and menus

- **Pause menu** (`src/game/ui/menu.cpp`): a page stack (`PushPauseMenuPage`, `PopPauseMenuPage`,
  `ReplacePauseMenuPage`), each page a list of items with init/update/select callbacks (`OnPauseItem*`), driven by the
  state machine `SetPauseMenuState` / `UpdatePauseMenu`. Opening stops game time (clock rate slot 0) and takes
  hands-off; the world keeps rendering underneath. Option items write save variables and apply them (volumes through
  `SetSndmgrChannelVolume`, screen offset, vibration, headset). `HidePauseMenu` restores time and control.
- **Gadget menu** (`SetGadgetMenuState`, `UpdateGadgetMenu`): freezes the clock and control, assigns owned power-ups to
  the three slots.
- **ThiefNet** (`SetThiefnetTab`, `UpdateThiefnet`, `RefreshThiefnetItems`): buy power-ups unlocked by progress and
  not owned (coins from the save), sell loot.
- **Job list** (`SetJobListPage`, `RefreshJobListEntries`): jobs and their tasks from the JOB/TASK graph.
- **HUD meter** (`SetHudMeterState`): health and gadget-power gauges per character.
- **Notice banner** (`PostNotice`, `CancelNotice`): queued timed messages.
- **Title screen** (`UpdateTitleScreen`, `StartNewGame`) and the **episode menu** widget (`UpdateEpisodeMenu`,
  percent and play-time labels) in the front-end world.
- **Memory-card dialogs** (`src/game/ui/memcard.cpp`): dialog widgets driven by the save state machine.
- **Modal message box** (`ShowModalBoxWidget`): takes control until dismissed.
- Menus read the stick as an 8-way direction (`UpdateScreenStickDir`, screen slot 14).

## 7. Full-screen presentation

- **Letterbox** (`engine/presentation/letterbox.cpp`): `ShowLetterbox` / `HideLetterbox` start and stop a timer widget
  that animates the bars; taken through the world's letterbox count during cutscenes.
- **Loading screen** (`engine/presentation/prog.cpp`): `BeginLoadingScreen` / `EndLoadingScreen` wrap a level load; a
  dedicated thread draws the pulsing logo directly to the GS while the main thread is blocked
  (`LoadingScreenThread`, `DrawLoadingScreenLogo`). `CProg` is the boot progress bar (`SetRemain`, `Draw`), drawn to both
  frame buffers immediately.
- **Wipes, screen tint and the frame-capture transition**: see [rendering.md](rendering.md#8-particles-and-effects-enginefx).

## 8. Invariants and gotchas

- **Screens outlive levels, widgets do not.** A screen must re-find its widget in post-load and drop it in reset; a
  stale widget pointer after a level change is a crash.
- **Widgets only update and draw while live.** A HUD widget's activation follows the normal LO rules; HUD widgets are
  normally kept always-active by their busy setting.
- **Freezing the game** is done with clock rate slots and hands-off counts, not by skipping the world update; menus
  that stop time must restore both on every exit path.
- **Screen coordinates are PS2 GS coordinates** (512-wide, field-height scissor, half-height pixels with a GS offset);
  `BuildRichTextMatrix` applies the pixel aspect and offset.
- Widget subclass names, several sprite helpers and some screen slots are still marked `(inferred)`.

## 9. How to

**Add a HUD element.** Author a widget in the level data (sprites and animation ids), give it an oid, and drive it
from code or Splice with `ShowWidget` / `HideWidget` and the animation calls. For dynamic text, add a STRINGGEN entry
with a `Build...Text` getter and reference its id from the widget.

**Add a menu.** Write a screen (a global object with Update, post-load and reset slots), add it to `UpdateScreens`,
bind its widget in post-load, and take hands-off / stop the clock while it is open.

**Add a pause-menu option.** Add an item with init/select callbacks to the options page builder
(`OpenPauseOptionsPage`); store the value in a save variable so it persists.

## 10. What a port must replace

- **The HUD VU1 microprogram and pass** (`BeginRpHud` / `EndRpHud`) and every `GIFS`-built 2-D packet (fonts, rectangles,
  progress bar, loading logo): a 2-D sprite batch with an orthographic projection.
- **Font textures in GS memory** (`EnsureFontLoaded`, `SwitchTex0`, `PostFontsLoad`'s GS allocation): normal textures.
- **The loading-screen thread** drawing straight to the GS while the main thread loads: a host loading screen or an
  async loader.
- **Resolution and aspect**: the layout assumes the PS2 screen; scale widget transforms to the output size.
- Shift-JIS conversion is only for the PS2 memory-card icon and can be dropped with the memory-card layer.

## 11. Where to look

- `src/game/engine/presentation/`: `hud.cpp` (widgets, sprites, text table, HUD pass), `screen.cpp` (screens,
  `UpdateScreens`, `DrawUi`, gadget menu, ThiefNet, job list, title), `letterbox.cpp`, `prog.cpp`.
- `src/game/engine/font/`: `font.cpp` (CFont, CFontBrx, CRichText), `sjis.cpp`.
- `src/game/ui/`: `menu.cpp` (pause menu), `memcard.cpp` (save UI and dialogs), the episode-menu unit (still named by
  address).
- `src/game/world/scene/sw.cpp`: the SW timer HUD; `include/sly2/screen.h`.
