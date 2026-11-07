# Recipe: add a HUD widget, dynamic text, screen or pause-menu option

**Goal.** Put something new on screen: a counter or gauge in the HUD, a line of text that shows a live value, a new
menu or dialog with its own input, or a new option in the pause menu.

**Systems.** [ui-and-hud.md](../systems/ui-and-hud.md) (read sections 1, 3, 4, 6 and 8 first),
[rendering.md](../systems/rendering.md) (the HUD pass), [progression-and-saves.md](../systems/progression-and-saves.md)
section 5 (save variables, for persistent options). Mod build: [README.md](README.md#how-a-mod-is-built).

## The two layers

- A **widget** (`WIDGET`, an `LO` subclass, `engine/presentation/hud.cpp`) is the look: a level object with sprite
  groups, animations and text sprites, loaded from the world file, updated and drawn while live.
- A **screen** (the `SCREENBASE` / `SCREEN` family, `include/sly2/screen.h`, `engine/presentation/screen.cpp`,
  `src/game/ui/`) is the logic: a global C++ object constructed at boot that survives level changes, binds to its widget
  by oid after each load, reads input while it has focus and drives the widget.

Pick the smallest piece that does the job:

| You want | Touch |
|---|---|
| show/hide or animate an existing widget from a script or relay | level Splice / RELAY commands only (no code) |
| a new HUD element with fixed art | a new widget in the level data; drive it with `ShowWidget` / `HideWidget` and the animation calls |
| a text that shows a live value | a new text-table (`STRINGGEN`) entry with a getter, referenced by a text sprite |
| a new menu, dialog or input-driven overlay | a new screen plus a widget |
| a new pause-menu setting | an item in the options page table of `ui/menu.cpp` and a save variable |

## Recipe A: a dynamic text

Text sprites name their text by a source kind and an id: kind 0 is a localized string of the world
(`FindSwLocalizedString`), kind 1 is an entry of the text table, a global array of `STRINGGEN` records
`{text id, getter, context}`. `RenderSpriteText` (`hud.cpp`) looks the id up with a linear search and calls the getter
to fill a text buffer, then lays the text out as rich text.

1. Write a getter with the table's signature: `void Build<Name>Text(char *out, void *context, void *owner)`. The
   owner is the sprite's owning widget; the context is whatever the entry stores. Write at most the buffer
   `RenderSpriteText` provides (a fixed local array) including the terminator. Rich-text escapes work in the output
   (`~RRGGBB` colour, `&N` font; ui-and-hud.md section 5). Existing getters to copy: `BuildCoinCountText`,
   `BuildTimerText`.
2. Add the entry. The table is built at static-init time (`hud.cpp`'s static initializer copies its records from a
   constant array and builds one at run time), and its length is a literal in the static initializer **and** in
   `RenderSpriteText`'s search loop. Grow the constant array and change both lengths together.
3. Pick a text id no other entry uses, and give the widget's text sprite kind 1 and that id (level data).

## Recipe B: a widget

1. **Art.** Widgets are authored in the level data: sprite tables, animation ids, size, a font flag
   (`LoadWidgetFromBrx`, `LoadSpriteTable`). Copying an existing widget record of the same subclass is the practical
   start; no tool in the repository writes world files yet.
2. **Activation.** A HUD widget must be live to update and draw; HUD widgets are kept always-active by their busy
   setting (object-model.md section 4.4). If yours appears only in some places, show and hide it instead of moving it
   in and out of the world.
3. **Driving it.** `ShowWidget` (slot 49) and `HideWidget` (slot 50) set a request; `UpdateWidget` plays the appear and
   disappear animations. Use `ShowWidgetArgs` to pass argument text ids, `SetWidgetAnimPhase` for meters (seek by
   fraction), `PlayWidgetAnim` / `SwitchWidgetAnim` for state changes, `SetWidgetExclusive` to hide lower-priority
   widgets while shown.
4. **New behaviour.** If the widget needs per-frame logic of its own (count something, follow a value), make it a
   `WIDGET` subclass and override `Update` (slot 16), calling `UpdateWidget` first. That is a new class:
   [new-object-class.md](new-object-class.md). The item counter and the job banner are existing subclasses to read.

## Recipe C: a screen

1. **Class.** Derive from `SCREEN` (which adds the widget pointer, a mode and post-load/reset slots) or from
   `SCREENBASE` for something without a widget. The slots: 7 Update, 8 Draw, 9 focus change (the focused screen gets
   the joypad), 10 Show, 11 Hide, and on `SCREEN` 12 PostLoad (resolve the widget; `PostLoadScreen` does it from the
   widget oid), 13 Reset, 14 SetMode.
2. **Instance.** A global object, like the existing screens. Its constructor runs from the C++ global constructors at
   boot (engine.md section 3).
3. **Register it in every list by hand.** There is no screen registry: each lifecycle function calls every screen
   explicitly.
   - `PostLoadScreens` (after a level load): call your post-load so it finds its widget.
   - `ResetScreens` (teardown): call your reset so it drops the widget pointer.
   - `UpdateScreens` (per frame, after the world): call your update at the right place in the fixed order (the pause
     menu first, the title screen last).
   - `DrawUi`: only if it draws outside its widget (most screens draw only through widgets).
4. **Input.** Show the screen (slot 10 puts it at the front of the shown list); `UpdateScreenFocus` gives the focused
   screen the joypad and calls slot 9 on the screens that gain and lose focus. Read the stick as an 8-way direction
   with `UpdateScreenStickDir` like the other menus.
5. **Freezing the game.** A screen that stops play must stop the clock through a clock rate slot (`SetClockRate`,
   engine.md section 4) and take hands-off (`IncrementSwHandsOff`), and undo both on **every** exit path, as
   `HidePauseMenu` does.

## Recipe D: a pause-menu option

1. The options page is built by `OpenPauseOptionsPage` (`ui/menu.cpp`) from a constant table of `ITEM` records
   `{kind, select, ..., update, init, ...}`, filtered by item kind and copied into a fixed buffer. Add a record to the
   table, raise the literal count in the copy loop and the size of the destination buffer to match.
2. Write the item callbacks: `init` reads the current value, `update` handles left/right while selected, `select`
   applies it. The existing `OnPauseItem*` callbacks are the models.
3. Persist the value in a save variable (`GetSaveVarByName` / `SetSaveVarByName`); adding a new save variable changes
   the save schema (progression-and-saves.md section 11).
4. The page's look (labels, cursor, animations) comes from the pause-menu widget in the front-end and level data; a new
   item needs a label there.

## Gotchas

- **The match.** Every table above (the text table, the options page table, the screen calls) is original code or
  data; build as a mod.
- **Screens outlive levels, widgets do not.** A screen holding a widget pointer across a level change crashes. Reset
  must clear it and post-load must re-find it; both happen only if you added the calls in step C3.
- **Widgets only work while live.** A widget outside the world neither updates nor draws, and `ShowWidget` on it has no
  visible effect.
- **PS2 screen coordinates.** Layout is in the PS2's 512-wide coordinate space with half-height pixels;
  `BuildRichTextMatrix` applies aspect and offset. A port rescales, but a mod on PS2 must stay inside it.
- **Text buffer size.** Getters write into a fixed stack buffer in `RenderSpriteText`; overrunning it corrupts the
  stack.

## How to test

- A dynamic text: point an existing text sprite at your id (a one-field change in the level data, or a live memory
  edit of the sprite's id in PCSX2 over PINE, which works without a mod build only if the id already exists) and
  watch it update.
- A screen: change level several times while it is shown and hidden; a missing reset shows up as a crash right after
  the next load.
- Pause the game with your screen open and closed: game time must stop and resume, and control must return.
