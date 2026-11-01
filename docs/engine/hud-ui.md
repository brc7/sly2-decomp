# HUD and UI: widgets, text, screens, menus, fonts, loading screen

Related: [game-progression](game-progression.md), [save-memcard](save-memcard.md) (memcard dialogs), [rendering](rendering.md).

## 1. WIDGET HUD objects (hud TU 0x210EB0-0x21BB88, file name unknown)

Sly 2's version of Sly 1's BLOT HUD family. WIDGET = CID 0x7E (LO child, 0x110), base vtable 0x2DAF30, with subclasses 0x7F-0x83; 41 widget-family
objects live in the Paris hub `[C:runtime]` (count), `[H r7c hud.md]` (design).
- State at +0x58: 0 unloaded, 1 hidden, 2 visible (looping anim +0x70, default 0x310), 3 appearing (+0x74, 0x30F), 4 disappearing (+0x78, 0x311);
  request +0x64 (1 hide, 2 show) set by 0x212A50 (existing name `SetTimerTimers` is wrong) `[C:runtime]` (coin/clue widgets: state 2, and 1 while pickpocketing).
- Vtable: 14 `LoadWidgetFromBrx`, 15 `PostWidgetLoad`, 16 `UpdateWidget`, 19 `RenderWidget`, new 49 `ShowWidget(idAnim)`, 50 `HideWidget` `[H r7c]` (>=0.8).
- Sprite data loaded immediately or streamed on first show (list D_003E106C); exclusive list D_003E1060 suppresses lower widgets `[H r7c]`.
- Subclasses: 0x7F modal hands-off box (dismiss mask 0xF0 after 0.5 s); 0x81 objective counter ("Widget Item 0/1/2" buffers);
  0x83 = WIDGET131 episode menu (methods in tu_15CD68) `[H r7c, r3b]`.
- SW HUD stack: four 12-byte records at SW+0x3BA0/+0x3BAC/+0x3BB8/+0x3BC4 (oids 0x371, 0x36D coins, 0x36E clues, 0x36F countdown) `[C:runtime]`
  (REFUTED "4 pointers"); 0x20F7E0 slides visible ones into a stacked layout `[H r7c]`.

HUD values `[C:runtime]` unless noted:

| HUD element | Source |
|---|---|
| coins | STRINGGEN id 0x2CF, getter `BuildCoinCountText` 0x210ED8 reads save var `loot.ccoin` via SW+0x3B20 |
| clue bottles | "%d/%d" of SW+0x4764 / SW+0x4768 (`BuildClueCountText`) |
| countdown | SW+0x476C - g_clock.t (`BuildTimerText`) [H r7c] |
| health meter | widget oid 0x36B, loop anim per character (Sly 0x33D, also 0x340 seen), `SetWidgetAnimPhase` 0x212E40(w, 0x34F, 1 - hp/hpMax); driver 0x1D9968 (PARTIAL: anim 0x340 in S02/S03 not in the doc list) |

**Text table** STRINGGEN: 108 entries `{id, get(pchz, context, widget), context}` at D_003E1C88, copied from rodata 0x2A8B90 by the static init 0x21AA60;
sprite text elements of kind 1 look up their id here, kind 0 uses a plain localized string `[H r7c]`. Rich text: `CRichText` (0x98 bytes) `[H r7c]`.

## 2. Screens and menus (screen.cpp 0x1D93D8, menu? 0x1AE928)

- A BASIC-derived screen family (12 vtables 0x2CB1F0..0x2D86D0; no CID of their own) replaces Sly 1's BLOT screens. Global screen objects
  (D_003D9E80, D_003DA0F0, D_003DA150, D_003DA0C0, ...) come from the TU's static init. Shared slots: 7 Update, 8 Draw?, 10 Show, 11 Hide
  (active list D_003D9E70 with focus switching), 12 PostLoad, 13 Reset, 14 SetState `[H r6b screen.md]` (partial pass).
- `UpdateUi(D_003D9E70)` 0x1D9648 (main loop step 7): Start (0x800) toggles pause, memcard UI updates; `DrawUi` 0x1D9820 flushes HUD lists `[H r4 main.md]`.
- **Pause menu** (menu?, D_003D9ED0): Show = clock rate 0 + hands-off + game event 0x28, Hide = rate 1 + event 0x2C; page stack; item tables
  0x2BFEC8..0x2C01AC with callbacks: volume sliders, video options (`ApplyGsSettings`), save/load, character moves pages, restart job
  (`FailJob`), return to hideout (`WipeToWorldWarp`) `[H r5b small_tus_r5b.md]`.

## 3. Fonts, language, letterbox, loading screen

- font.cpp (0x15EF00): `CFont`/`CFontBrx`/`CRichText` in Sly 1 order; built-in fonts uploaded to GS memory 0x1000..0x1800 (`StartupFont`, startup entry 8?) `[H r3 font.md]`.
- language (0x1A2BD0): region table D_002BFBD0 (7 regions), current language D_002E1E54, region D_002E1E58; `FourccFromLanguage` -> 'FK$e', 'FK$f', ... (the same "FK$" prefix as world-entry names) `[H r4b language.md]`.
- sjis (0x1EE988): Shift-JIS helpers `[H r6]`.
- letterbox (0x1A3020): show/hide via vt49/vt50 of the letterbox object, from `AcquireSwLetterbox`/`ReleaseSwLetterbox` `[H r4b]`.
- prog.cpp (0x1C8CC0): `CProg` progress bar (used by Startup) and a **loading-screen thread** (`StartupLoadingScreen` 0x1C90D0,
  `LoadingScreenThread` 0x1C9190, pulsing quad) bracketing `LoadSwFromBrx` with `BeginLoadingScreen` 0x1C9058 / end `[C:verified]` (CProg names), `[H r5 prog.md]`.

## 4. Source files

| Span | File (label) | Notes |
|---|---|---|
| 0x15CD68-0x15DC80 | tu_15CD68 (WIDGET131 episode menu) |  |
| 0x15EF00-0x1603B8 | font |  |
| 0x1A2BD0-0x1A3020 | language |  |
| 0x1A3020-0x1A3260 | letterbox |  |
| 0x1AE928-0x1B0B78 | menu? | was labelled step |
| 0x1C8CC0-0x1C9940 | prog |  |
| 0x1D93D8-0x1DE280 | screen | was labelled gc |
| 0x1EE988-0x1EEC60 | sjis |  |
| 0x210E90-0x21BB88 | hud TU (WIDGET, sprites, rich text) | starts 0x210EB0; was labelled screen |

## 5. Key functions

| Addr | Name | Purpose | Conf | Status |
|---|---|---|---|---|
| [0x15CD68](../../src/hand/func_0015CD68.c) | `InitCid83` | LO::Init, four ids = -1, registers itself as the singleton D_003E1C04 | 0.75 | HYP |
| [0x15EF00](../../src/hand/func_0015EF00.c) | `StartupFont` | Font startup: loads/posts the built-in fonts (0x15EF30) and sets the default font pointer D_002DE760 = font[0] | 0.70 | HYP |
| [0x15F148](../../src/auto/func_0015F148.c) | `FEnsureLoaded` | Ensures the font texture is uploaded this frame: tracks a per-frame bit in g_pfrmOpen+0x2C when drawing into g_gifs, emits DMA REFs of the upload chai | 0.70 | HYP |
| [0x15F8A8](../../src/hand/func_0015F8A8.c) | `PglyffFromCh` | Binary-searches the font glyph table for a character (via remap table D_002BE338) | 0.80 | HYP |
| [0x1C8CC0](../../src/hand/__5CProgP4RGBAN31.cpp) | `CProg::CProg` | CProg ctor (4 colors) | 0.90 | HYP |
| [0x1C8D20](../../src/hand/Begin__5CProg.cpp) | `CProg::Begin` | reset progress | 0.90 | HYP |
| [0x1C8D40](../../src/hand/SetRemain__5CProgi.cpp) | `CProg::SetRemain` | update remaining count | 0.90 | HYP |
| [0x1C8DB8](../../src/hand/func_001C8DB8.c) | `CProg::End` | clear progress state (same stores as Begin) | 0.80 | VERIFIED |
| [0x1C8DD8](../../src/hand/func_001C8DD8.c) | `CProg::Draw` | draw progress bar (GIF/DMA packet) | 0.75 | VERIFIED |
| [0x1D9648](../../src/hand/func_001D9648.c) | `UpdateScreens` | Per-frame: handle the pause/menu buttons (joy D_002DFBC4 flags) and run Update (slot 7) of the HUD screens | 0.40 | HYP |
| [0x1EE988](../../src/hand/FUN_001b5b58.c) | `WSjisFromCh` | Map one ASCII byte to a 16-bit SJIS code via table | 0.50 | VERIFIED |
| [0x210EB0](../../src/auto/func_00210EB0.c) | `BuildContextText` | STRINGGEN getter: strcpy(pchz, context) | 0.75 | HYP |
| [0x210ED8](../../src/hand/func_00210ED8.c) | `BuildCoinCountText` | STRINGGEN id 0x2CF: sprintf("%d", saved var loot.ccoin) = HUD coin counter text | 0.85 | HYP |
| [0x210F50](../../src/hand/func_00210F50.c) | `BuildClueCountText` | STRINGGEN id 0x2D0: sprintf("%d/%d", SW+0x4764, SW+0x4768) = clue bottles collected/total | 0.80 | HYP |
| [0x210F80](../../src/hand/func_00210F80.c) | `BuildTimerText` | STRINGGEN id 0x2D1: countdown "%d:%02d" of max(0, SW+0x476C - g_clock.t) | 0.80 | HYP |
| [0x211348](../../src/hand/func_00211348.c) | `BuildWidgetArgText` | STRINGGEN ids 0x2E5-0x2EE: langstr(owner WIDGET->aid[i] at +0xCC) | 0.70 | HYP |
| [0x211C58](../../src/hand/func_00211C58.cpp) | `WIDGET130::WIDGET130` | ctor of CID 0x82: WIDGET ctor then 4 ids and +0x120 = -1 | 0.70 | HYP |
| [0x212158](../../src/hand/func_00212158.c) | `WIDGET::WIDGET` | WIDGET ctor: vtable 0x2DAF30, idAnimMain(+0x7C)=-1, 10 arg ids -1, InitSl(+0xFC, 0x68) | 0.75 | HYP |
| [0x212298](../../src/hand/func_00212298.c) | `LoadWidgetFromBrx` | WIDGET LoadFromBrx: default anims 0x310/0x30F/0x311, LoadLoFromBrx, fStreamed, duration, width, height, fFont; immediate or deferred sprite load | 0.85 | HYP |
| [0x2123C8](../../src/hand/func_002123C8.c) | `PostWidgetLoad` | WIDGET PostLoad: font LO 0x374 into SW+0x3BE8, screen centring from width/height, show if flag, busy 1, set LO flags 0x102 | 0.85 | HYP |
| [0x2128E8](../../src/hand/func_002128E8.cpp) | `ShowWidget` | WIDGET slot49: request show; main anim k or default appear/loop/disappear 0x30F/0x310/0x311 if present | 0.70 | HYP |
| [0x2129E8](../../src/auto/func_002129E8.c) | `HideWidget` | WIDGET slot50: request hide; if no disappear anim go straight to state 1 | 0.70 | HYP |
| [0x212A50](../../src/auto/SetTimerTimers__FP5TIMER6TIMERS.c) | `SetTimerTimers__FP5TIMER6TIMERS` |  |  | existing; fix: SetWidgetRequest (WIDGET show/hide request setter) |
| [0x212A70](../../src/hand/func_00212A70.c) | `SetWidgetState` | state machine 0 unloaded,1 hidden,2 visible(loop),3 appearing,4 disappearing; counts SW+0x3BEC, pauses clock when fPause | 0.75 | HYP |
| [0x212C50](../../src/hand/func_00212C50.c) | `UpdateWidget` | WIDGET Update: drives SetWidgetState from request/anim completion and duration, advances main sprite | 0.85 | HYP |
| [0x212E40](../../src/hand/func_00212E40.c) | `SetWidgetAnimPhase` | seek anim id (0x314=main) to fraction g of its length (meters) | 0.60 | HYP |
| [0x213300](../../src/hand/func_00213300.cpp) | `RenderWidget` | WIDGET RenderSelf: flush the sprite render queue with VU microprogram packets | 0.80 | HYP |
| [0x215A50](../../src/hand/PglyffFromCh__8CFontBrxc.c) | `PglyffFromCh__8CFontBrxc` |  |  | existing (dispute won); fix: IglyffFromCh (keep the existing name unless the verifier agrees) |
| [0x215D00](../../src/hand/func_00215D00.c) | `CRichText::SetBaseColor` | set base and current colour | 0.70 | HYP |
| [0x215D40](../../src/hand/func_00215D40.c) | `CRichText::ChNext` | next printable char, processing &-escape codes | 0.70 | HYP |
| [0x215E30](../../src/hand/func_00215E30.c) | `CRichText::Cch` | count characters | 0.75 | HYP |
| [0x215E80](../../src/hand/func_00215E80.c) | `CRichText::Trim` | truncate after n chars | 0.75 | HYP |
| [0x215ED8](../../src/hand/func_00215ED8.c) | `CRichText::Dx` | sum of glyph widths * scale | 0.70 | HYP |
| [0x2161C8](../../src/hand/func_002161C8.c) | `CRichText::DxMaxLine` | widest line | 0.70 | HYP |
| [0x216260](../../src/hand/func_00216260.c) | `CRichText::GetExtents` | dx = DxMaxLine, dy = lines*lineheight | 0.70 | HYP |

Selected 35 of 239 named functions (confidence >= 0.7, verified, or flagged). Full list: [reference/functions/hud-ui.md](../reference/functions/hud-ui.md).

## Open questions
- Which screen class is which (pause, gadget select, map, objectives): break `Show` 0x1D98B0 and read a0's vtable.
- STRINGGEN entries 44-103 point into other TUs and are unnamed.
- Health widget loop anim 0x340 (seen in S02/S03): which character/state?
