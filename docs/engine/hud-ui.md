# HUD and UI: widgets, text, screens, menus, fonts, loading screen

Related: [game-progression](game-progression.md), [save-memcard](save-memcard.md) (memcard dialogs), [rendering](rendering.md).

## 1. WIDGET HUD objects (hud TU 0x210EB0-0x21BB88, file name unknown)

Sly 2's version of Sly 1's BLOT HUD family. WIDGET = CID 0x7E (LO child, 0x110), base vtable 0x2DAF30, with subclasses 0x7F-0x83; 41 widget-family
objects live in the Paris hub `[C:runtime]` (count), `[H r7c hud.md]` (design).
- State at +0x58: 0 unloaded, 1 hidden, 2 visible (looping anim +0x70, default 0x310), 3 appearing (+0x74, 0x30F), 4 disappearing (+0x78, 0x311);
  request +0x64 (1 hide, 2 show) set by 0x212A50 (existing name `SetTimerTimers` is wrong) `[C:runtime]` (coin/clue widgets: state 2, and 1 while pickpocketing).
- Vtable: 14 `LoadWidgetFromBrx`, 15 `PostWidgetLoad`, 16 `UpdateWidget`, 19 `RenderWidget`, new 49 `ShowWidget(idAnim)`, 50 `HideWidget` `[H r7c]` (>=0.8).
- Sprite data loaded immediately or streamed on first show (list D_003E106C); exclusive list D_003E1060 suppresses lower widgets `[H r7c]`.
- Subclasses: 0x7F modal hands-off box (dismiss mask 0xF0 after 0.5 s) `[H r7c]`; **0x80 WIDGET128 = job completion banner**: `fPause` +0xB0 = 1, so
  showing it sets D_002DD318 and clock rate slot 7 = 0 (the game clock stops while it plays; oid 0x6D6, labels 0x4411/0x4412) `[C:runtime S61]`;
  **0x81 WIDGET129 = objective counter**: counts complete tasks of its list B (+0x194/+0x19C) every frame and sprintf's done/total/remaining into
  "Widget Item 0/1/2" D_002C3EC0 (+0/+0x30/+0x60); the count is not stored anywhere `[C:runtime S57-S62]`; 0x83 WIDGET131 = episode menu in the front-end
  world (mode 10 start-episode animation, label 0x539) `[C:runtime S55]`. See [game-progression](game-progression.md#6-objective-counters-job-markers-and-the-episode-menu).
- Letterbox widget: oid 0x373 via D_002E1E60; state 2 only while SW+0x3D80 = 1 `[C:runtime]`. Binocucom widget pairs: Sly 0x35F/0x368, Murray 0x361/0x36A, Bentley 0x360/0x369; per-character health widgets 0x33D-0x341 (Sly), 0x342-0x346 (Murray) `[H]` (red-team #51: oids 0x33D, 0x341 and 0x342 match no live object in S02/S04/S24/S40/S56; 0x346 seen once in S04 off the class lists, vptr 0x2CA8A0; re-derive).
- SW HUD stack: four 12-byte records at SW+0x3BA0/+0x3BAC/+0x3BB8/+0x3BC4 (oids 0x371, 0x36D coins, 0x36E clues, 0x36F countdown) `[C:runtime]`
  (REFUTED "4 pointers"); 0x20F7E0 slides visible ones into a stacked layout `[H r7c]`.

HUD values `[C:runtime]` unless noted:

| HUD element | Source |
|---|---|
| coins | STRINGGEN id 0x2CF, getter `BuildCoinCountText` 0x210ED8 reads save var `loot.ccoin` via SW+0x3B20 |
| clue bottles | "%d/%d" of SW+0x4764 / SW+0x4768 (`BuildClueCountText`) |
| countdown | SW+0x476C - g_clock.t (`BuildTimerText`) [H r7c] |
| health meter | widget oid 0x36B, loop anim per character (Sly 0x33D `[H]` unconfirmed as a live object, also 0x340 seen), `SetWidgetAnimPhase` 0x212E40(w, 0x34F, 1 - hp/hpMax); driver 0x1D9968 (PARTIAL: anim 0x340 in S02/S03 not in the doc list) |

**Text table** STRINGGEN: 108 entries `{id, get(pchz, context, widget), context}` at D_003E1C88, copied from rodata 0x2A8B90 by the static init 0x21AA60;
sprite text elements of kind 1 look up their id here, kind 0 uses a plain localized string `[H r7c]`. Rich text: `CRichText` (0x98 bytes) `[H r7c]`.

## 2. Screens and menus (screen.cpp 0x1D93D8, menu? 0x1AE928)

- A BASIC-derived screen family (12 vtables 0x2CB1F0..0x2D86D0; no CID of their own) replaces Sly 1's BLOT screens. Global screen objects
  (D_003D9E80, D_003DA0F0, D_003DA150, D_003DA0C0, ...) come from the TU's static init. Shared slots: 7 Update, 8 Draw?, 10 Show, 11 Hide
  (active list D_003D9E70 with focus switching), 12 PostLoad, 13 Reset, 14 SetState `[H r6b screen.md]` (partial pass).
- `UpdateUi(D_003D9E70)` 0x1D9648 (main loop step 7): Start (0x800) toggles pause, memcard UI updates; `DrawUi` 0x1D9820 flushes HUD lists `[H r4 main.md]`.
- **Pause menu** PAUSEMENU D_003D9ED0 (vtable 0x2D37D0) `[C:runtime S54]`: on the active screen DL D_003D9E70 `{first, last, ibDle 0x14}` (focus +0xC; empty in every
  other state); depth +0x24, state +0x1E8 (7 = input), page groups at +0xC + 0x1C*depth `{count, items, widget, back, -, index, meta}`; root page 6 items
  (text 0x39 resume, 0x47/0x42, 0x2C options, 0x43 character moves, 0x6A map), widget oid 0x370. Clock rate slot 0 = 0 (dt 0; tReal and t3C keep running),
  hands-off 1, `g_grfusr` 2, the world keeps rendering. D_003D9E80 (HUD screen; +0x34 = current player) is not on the active list.
- **ThiefNet shop** SCREEN378 = D_003DA150 (vtable 0x2D84E8, widget 0x378, size 0x270), opened from the safehouse PO `[C:runtime S71]` + code: opening (`SetScreen378Tab`
  0x1DC260 from -1) sets clock slot 0 = 0 and hands-off; layout +0x28 state, +0x30 dialog (-1 none, 0 confirm, 1 cannot afford), +0x40 tab (-1 closed, 0 gadgets, 1 loot),
  +0x44 tab start (g_clock+0x3C), +0x48 count, +0x4C entries[64] {id, owner icon 0x456 Sly / 0x457 Bentley / 0x458 Murray}, +0x24C cursor, +0x250 first row (6 rows per page),
  +0x254 selected gadget, +0x258 selected loot. **List rule** (`BuildScreen378List` 0x1DBD10): gadget ids 7-42 with `episodeBest >= g_agadgetdesc[i].unlockEpisode` (!= -1) and
  not owned. **Buying** (`UpdateScreen378` 0x1DB870): X opens the confirm dialog (string 0x34) or "not enough coins" (0x35); X again subtracts the price from `loot.ccoin`, sets
  the acquired bit, rebuilds the list, plays sound 0x29; triangle closes, square switches tab. Runtime check: coins 2698 -> 498 for Combat Dodge (600) + Thief Reflexes (1600).
  Loot tab (sell loot for D_002C3288[i] coins) `[H]` (code only).
- Pause menu (static): Show = clock rate 0 + hands-off + game event 0x28, Hide = rate 1 + event 0x2C; page stack; item tables
  0x2BFEC8..0x2C01AC with callbacks: volume sliders, video options (`ApplyGsSettings`), save/load, character moves pages, restart job
  (`FailJob`), return to hideout (`WipeToWorldWarp`) `[H r5b small_tus_r5b.md]`.

## 3. Fonts, language, letterbox, loading screen

- font.cpp (0x15EF00): `CFont`/`CFontBrx`/`CRichText` in Sly 1 order; built-in fonts uploaded to GS memory 0x1000..0x1800 (`StartupFont`, startup entry 8?) `[H r3 font.md]`.
- language (0x1A2BD0): region table D_002BFBD0 (7 regions), current language D_002E1E54, region D_002E1E58; `FourccFromLanguage` -> 'FK$e', 'FK$f', ... (the same "FK$" prefix as world-entry names) `[H r4b language.md]`.
- sjis (0x1EE988): Shift-JIS helpers `[H r6]`.
- letterbox (0x1A3020): show/hide via vt49/vt50 of the letterbox object, from `AcquireSwLetterbox`/`ReleaseSwLetterbox` `[H r4b]`.
- prog.cpp (0x1C8CC0): `CProg` progress bar (used by Startup) and a **loading-screen thread** (`StartupLoadingScreen` 0x1C90D0,
  `LoadingScreenThread` 0x1C9190) started by `LoadSwFromBrx` itself via `BeginLoadingScreen` 0x1C9058: an 80x80 quad at (320,240), scale 1 + 0.1 sin(2 pi t), alpha = clamp(t,0,1), t from `g_clock+0x3C`; the thread ticks the clock paused and bumps `g_cframe` `[C:runtime S56]`. Details: [world-sw](world-sw.md#2-loading-a-level).

## 4. Source files

| Span | File (label) | Conf | Notes |
|---|---|---|---|
| 0x15CD68-0x15DC80 | tu_15CD68 | low | WIDGET131 episode menu |
| 0x15EF00-0x1603B8 | font | high |  |
| 0x1A2BD0-0x1A3020 | language | low |  |
| 0x1A3020-0x1A3260 | letterbox | low |  |
| 0x1AE928-0x1B0B78 | menu | low | pause menu; was labelled step |
| 0x1C8CC0-0x1C9940 | prog | high |  |
| 0x1D93D8-0x1DE2A0 | screen | medium | ends with $D stub 0x1DE280; was labelled gc |
| 0x1EE988-0x1EEC60 | sjis | high | tu_table said shdanim |
| 0x210EB0-0x21BB88 | hud | low | WIDGET+sprite+richtext; tu_table said screen; starts at 0x210EB0; starts 0x210EB0; was labelled screen |

## 5. Key functions

| Addr | Name | Purpose | Conf | Tier | Evidence |
|---|---|---|---|---|---|
| [0x15EF00](../../src/game/engine/font/font/func_0015EF00.c) | `StartupFont` | Font startup: loads/posts the built-in fonts (0x15EF30) and sets the default font pointer D_002DE760 = font[0] | 0.70 | T1 |  |
| [0x15F148](../../src/game/engine/font/font/func_0015F148.c) | `FEnsureLoaded` | Ensures the font texture is uploaded this frame: tracks a per-frame bit in g_pfrmOpen+0x2C when drawing into g_gifs, emits DMA REFs of the upload chai | 0.70 | T1 |  |
| [0x15F8A8](../../src/game/engine/font/font/func_0015F8A8.c) | `PglyffFromCh` | Binary-searches the font glyph table for a character (via remap table D_002BE338) | 0.80 | T1 |  |
| [0x1B0078](../../src/game/ui/menu/func_001B0078.c) | `IdPauseTopPage` | Id of current top page (0x1E/0x1F/0x19 for the three save/load pages) or -1 | 0.85 | T1 | verified |
| [0x1B09A8](../../src/game/ui/menu/func_001B09A8.c) | `PauseItemReturnToHideout` | WipeToWorldWarp(...,0x189) and fail current job | 0.85 | T1 | verified |
| [0x1C8DB8](../../src/game/engine/presentation/prog/func_001C8DB8.c) | `CProg_End` | clear progress state (same stores as Begin) | 0.85 | T1 | verified |
| [0x1C8DD8](../../src/game/engine/presentation/prog/func_001C8DD8.c) | `CProg_Draw` | draw progress bar (GIF/DMA packet) | 0.75 | T1 | verified |
| [0x1D9648](../../src/game/engine/presentation/screen/func_001D9648.c) | `UpdateScreens` | Per-frame: handle the pause/menu buttons (joy D_002DFBC4 flags) and run Update (slot 7) of the HUD screens | 0.40 | T2 |  |
| [0x1D9820](../../src/game/engine/presentation/screen/UpdateRecyclable__3CGc.c) | `DrawUi` | Called from main: run 0x1D9878 on the three main HUD screens, then flush the two HUD DMA streams (D_002DEA00/D_002DEA28) | 0.60 | existing | arbitration; replaces existing UpdateRecyclable__3CGc |
| [0x1DAEA8](../../src/game/engine/presentation/screen/func_001DAEA8.c) | `FPowerupAcquired` | Save var powerUps.mppukfAcquired[n] (has the power-up been bought) | 0.75 | T1 |  |
| [0x1DAF38](../../src/game/engine/presentation/screen/func_001DAF38.cpp) | `SetPowerupAcquired` | Set powerUps.mppukfAcquired[n]; when set broadcast game event 0x3B with n | 0.75 | T1 |  |
| [0x1DC4B0](../../src/game/engine/presentation/screen/func_001DC4B0.c) | `SetThiefnetStickDir` | ThiefNet slot 14: dir 0 -> sell tab, 4 -> buy tab, 1-3 previous row, 5-7 next row (with sounds) | 0.55 | T2 | verified |
| [0x1DD780](../../src/game/engine/presentation/screen/func_001DD780.c) | `FScreen52DShown` | Widget fully shown (+0x58==2 and +0x64==2) | 0.35 | T2 | verified |
| [0x1DDBF0](../../src/game/engine/presentation/screen/func_001DDBF0.cpp) | `BuildTitlePromptText` | Text callback 0x5D3: title prompt (continue vs new game, options, language) from string ids 5/6/8/0x56 | 0.85 | T1 | verified |
| [0x1DDDB8](../../src/game/engine/presentation/screen/func_001DDDB8.c) | `BuildJobListWorldText` | Text callback 0x51B: current episode name | 0.85 | T1 | verified |
| [0x1EE988](../../src/game/engine/font/sjis.cpp) | `WSjisFromCh` | Map one ASCII byte to a 16-bit SJIS code via table | 0.55 | existing | verified, arbitration; replaces existing FUN_001b5b58 |
| [0x1EEA70](../../src/game/engine/font/sjis.cpp) | `ConvertPchzToSjis` | Convert C string (strlen) to array of u16 SJIS codes | 0.55 | existing | arbitration; replaces existing FUN_001b5c40 |
| [0x210EB0](../../src/game/engine/presentation/hud/func_00210EB0.c) | `BuildContextText` | STRINGGEN getter: strcpy(pchz, context) | 0.75 | T1 |  |
| [0x210ED8](../../src/game/engine/presentation/hud/func_00210ED8.c) | `BuildCoinCountText` | STRINGGEN id 0x2CF: sprintf("%d", saved var loot.ccoin) = HUD coin counter text | 0.85 | T1 |  |
| [0x210F50](../../src/game/engine/presentation/hud/func_00210F50.c) | `BuildClueCountText` | STRINGGEN id 0x2D0: sprintf("%d/%d", SW+0x4764, SW+0x4768) = clue bottles collected/total | 0.80 | T1 | runtime-corrected |
| [0x210F80](../../src/game/engine/presentation/hud/func_00210F80.c) | `BuildTimerText` | STRINGGEN id 0x2D1: countdown "%d:%02d" of max(0, SW+0x476C - g_clock.t) | 0.85 | T1 | verified |
| [0x211348](../../src/game/engine/presentation/hud/func_00211348.c) | `BuildWidgetArgText` | STRINGGEN ids 0x2E5-0x2EE: langstr(owner WIDGET->aid[i] at +0xCC) | 0.70 | T1 |  |
| [0x211A80](../../src/game/engine/presentation/hud/func_00211A80.c) | `BeginRpHud` | Pass 5 begin (kind 21): HUD microprogram + constants + DIRECT HUD GS state | 0.85 | T1 | verified |
| [0x212158](../../src/game/engine/presentation/hud/func_00212158.c) | `WIDGET_ctor` | WIDGET ctor: vtable 0x2DAF30, idAnimMain(+0x7C)=-1, 10 arg ids -1, InitSl(+0xFC, 0x68) | 0.85 | T1 | verified |
| [0x212298](../../src/game/engine/presentation/hud/func_00212298.c) | `LoadWidgetFromBrx` | WIDGET LoadFromBrx: default anims 0x310/0x30F/0x311, LoadLoFromBrx, fStreamed, duration, width, height, fFont; immediate or deferred sprite load | 0.90 | T1 | parse |
| [0x2123C8](../../src/game/engine/presentation/hud/func_002123C8.c) | `PostWidgetLoad` | WIDGET PostLoad: font LO 0x374 into SW+0x3BE8, screen centring from width/height, show if flag, busy 1, set LO flags 0x102 | 0.85 | T1 |  |
| [0x2128E8](../../src/game/engine/presentation/hud/func_002128E8.cpp) | `ShowWidget` | WIDGET slot49: request show; main anim k or default appear/loop/disappear 0x30F/0x310/0x311 if present | 0.70 | T1 |  |
| [0x2129E8](../../src/game/engine/presentation/hud/func_002129E8.c) | `HideWidget` | WIDGET slot50: request hide; if no disappear anim go straight to state 1 | 0.70 | T1 |  |
| [0x212A50](../../src/game/engine/presentation/hud/SetTimerTimers__FP5TIMER6TIMERS.c) | `SetWidgetRequest` |  | 0.75 | existing | arbitration; replaces existing SetTimerTimers__FP5TIMER6TIMERS |
| [0x212A70](../../src/game/engine/presentation/hud/func_00212A70.c) | `SetWidgetState` | state machine 0 unloaded,1 hidden,2 visible(loop),3 appearing,4 disappearing; counts SW+0x3BEC, pauses clock when fPause | 0.75 | T1 |  |
| [0x212C50](../../src/game/engine/presentation/hud/func_00212C50.c) | `UpdateWidget` | WIDGET Update: drives SetWidgetState from request/anim completion and duration, advances main sprite | 0.90 | T1 | runtime |
| [0x212E40](../../src/game/engine/presentation/hud/func_00212E40.c) | `SetWidgetAnimPhase` | seek anim id (0x314=main) to fraction g of its length (meters) | 0.60 | T1 |  |
| [0x2130C0](../../src/game/engine/presentation/hud/func_002130C0.c) | `QueueWidgetRenderSetup` | DMA packet with render init payloads (frame/mask GIF data) before widget sprites | 0.85 | T1 | verified |
| [0x213300](../../src/game/engine/presentation/hud/func_00213300.cpp) | `RenderWidget` | WIDGET RenderSelf: flush the sprite render queue with VU microprogram packets | 0.80 | T1 |  |
| [0x2137F0](../../src/game/engine/presentation/hud/func_002137F0.c) | `FWidgetObscured` | some widget in exclusive list D_003E1060 has higher priority (+0x0? field a) | 0.85 | T1 | verified |
| [0x213CD8](../../src/game/engine/presentation/hud/func_00213CD8.cpp) | `SetWidgetAnimXf` | set M23 transform of the sprite that holds anim id | 0.85 | T1 | verified |
| [0x214398](../../src/game/engine/presentation/hud/func_00214398.c) | `LoadSpriteTable` | read sprite count, alloc (n+1)*0x40, read each descriptor | 0.75 | T1 | parse |
| [0x214460](../../src/game/engine/presentation/hud/func_00214460.cpp) | `LoadSpriteGroups` | deserialize sprite groups, reusable frames and animation keys | 0.75 | T1 | parse |
| [0x2149D8](../../src/game/engine/presentation/hud/func_002149D8.cpp) | `LoadSpriteDescriptor` | read one sprite descriptor (kinds 1 geometry,2 polygon,3 shader,4 index,6/7 picture,8 nested) and build packets | 0.75 | T1 | parse |
| [0x215A50](../../src/game/engine/presentation/hud/PglyffFromCh__8CFontBrxc.c) | `IglyffFromCh__8CFontBrxc` |  | 0.60 | existing | arbitration; replaces existing PglyffFromCh__8CFontBrxc |
| [0x215D00](../../src/game/engine/presentation/hud/func_00215D00.c) | `CRichText_SetBaseColor_215D00` | set base and current colour | 0.85 | T3 | verified |
| [0x215E30](../../src/game/engine/presentation/hud/func_00215E30.c) | `CRichText_Cch` | count characters | 0.75 | T1 |  |
| [0x215E80](../../src/game/engine/presentation/hud/func_00215E80.c) | `CRichText_Trim` | truncate after n chars | 0.85 | T1 | verified |
| [0x215ED8](../../src/game/engine/presentation/hud/func_00215ED8.c) | `CRichText_Dx` | sum of glyph widths * scale | 0.70 | T1 |  |
| [0x216260](../../src/game/engine/presentation/hud/func_00216260.c) | `CRichText_GetExtents` | dx = DxMaxLine, dy = lines*lineheight | 0.70 | T1 |  |
| [0x216B40](../../src/game/engine/presentation/hud/func_00216B40.c) | `CFontBrx_PglyffFromCh` | GLYFF* = &glyphs[i] for i = 0x215A50(ch) | 0.60 | T1 | arbitration |
| [0x2174C8](../../src/game/engine/presentation/hud/func_002174C8.c) | `RenderSpriteGeometry` | transformed sprite parameters with optional fade | 0.85 | T1 | verified |
| [0x218710](../../src/game/engine/presentation/hud/func_00218710.c) | `FSpriteAnimActive` | h!=0 && !done | 0.85 | T1 | verified |
| [0x219348](../../src/game/engine/presentation/hud/func_00219348.c) | `ResetSpriteKeyItems` | reset items referenced by a key | 0.85 | T1 | verified |

Selected 49 of 307 named functions (T1 with confidence >= 0.7, or runtime/verified/parse/arbitration evidence, or cited in the text). Full list: [reference/functions/hud-ui.md](../reference/functions/hud-ui.md).

## Open questions
- Which screen class is which (pause, gadget select, map, objectives): break `Show` 0x1D98B0 and read a0's vtable.
- STRINGGEN entries 44-103 point into other TUs and are unnamed.
- Health widget loop anim 0x340 (seen in S02/S03): which character/state?
