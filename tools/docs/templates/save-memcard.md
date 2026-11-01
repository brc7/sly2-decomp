# Save data and memory card

Sly 2 keeps all persistent progress in **one flat blob** addressed by **named variables** whose layout comes from a schema
shipped in every world file. The memory-card code just writes that blob (three slots) through a worker thread.
Related: [reference/save-variables.md](../reference/save-variables.md) (the variables), [game-progression](game-progression.md).

## 1. The save blob g_gs (save.cpp, 0x1D7B88-0x1D93D8)

- `g_gs` = D_003D4A60: 0x5400 bytes of variables, then +0x5400 schema version and +0x5404 schema checksum `[C:runtime]` (address, size), `[H r6 save.md]` (trailer).
- **Schema** at `g_sw+0x3A18` (D_003E0EA8), loaded by `LoadSaveSchemaFromBrx` 0x1D7B88 from the world file `[C:runtime]`:
  - a 53-bucket case-insensitive hash (h = h\*33 + toupper(c)) of `{hash, name, sym, next}`: 960 symbols;
  - a trie of 5,461 10-byte entries `{s16 sym; s8 kind (-1 = interior); u8 cbits (array length / packed bits); u16 ib (byte offset in g_gs); u16 iChild; u16 iSibling}`.
  Walking it yields 4,472 named variables, and every byte that changed between save states maps to one `[C:runtime]`.
- Kinds select int / float / bool (packed bits when `cbits` != 0) / vector / ... ; element sizes from table D_002C2D90 (4,4,4,4,4,16,16,8,12,4,4,0) `[C:runtime]`.
- **Coins live** `[C:live]`: `loot.ccoin` = g_gs+0xA0 = 0x3D4B00; writing 777 over PINE made the HUD show 777. RAM cross-check `[C:runtime v3]`: 6 in S02,
  2698 in S71, 498 in S72 (after the 2,200-coin ThiefNet purchase).
- If the schema checksum/version differs from the blob's trailer, `ResetGsIfSchemaChanged` 0x1D7C80 wipes the blob `[H r6 save.md]`.
- Access: script builtins 131/132 and C++ through varargs helpers `RefGetSaveVarV("name", ..., NULL)` 0x1D8A38 / `RefSetSaveVarV` 0x1D8B20 /
  `PvSaveVarV` 0x1D8C10 returning `CRef`s or pointers (20 callers) `[H r6 save.md, r2b stepplayer.md]`. `ResolveSwSaveVars` 0x20E3C0
  precomputes keys into SW slots (+0x3B1C dt, +0x3B20 loot.ccoin, +0x3B2C[11] options, +0x3B58/+0x3B7C per-episode) `[C:runtime]`.
- `ResetGs` wipes progress but keeps the 11 option variables; `ApplyGsSettings` pushes options to hardware (sound mode, volumes,
  screen offset, vibration) `[H r6 save.md]`. Option keys k0..k10 = `flags.lang`, `nMixerMode`, `uMusicVolume`, `uEffectsVolume`,
  `fUseHeadset`, `dxScreenCenter`, `dyScreenCenter`, `fNoVibration`, `fReverseLookAround`, `fReverseAim`, `fAutosaveWarned` `[C:runtime]`.
- "% complete" shown in the memcard UI = 9 weighted progress vars + `fVault` bits `[H r6 save.md]`. Per-episode `percent_complete` = state-3 TASKs with
  successors / all TASKs with successors (`NEpisodePercentComplete` 0x161660) `[C:runtime runtime6]`.
- Save variables that change during play: `dt`, `episode.<w>.dt`, hp, `loot.ccoin`, `mplootkc`, `afClue`, and the task/job records (`t1_sat_dish1.state` etc.,
  `is_complete` already 1 at task state 2), `powerUps.mppukfAcquired` (g_gs+0x98..+0x9F; bits set by ThiefNet purchases, coins taken from `loot.ccoin`) `[C:runtime]`. A death reload keeps every save var `[C:runtime S29-S30]`.

## 2. Memory card (memcard.cpp, 0x1AA978-0x1AE130)

- **Card file format** `[C:static]` (devtools/save_editor/MEMCARD_FORMAT.txt; size constant re-checked in v3: `FUN_0018d328.c` passes 0x5408,
  `func_001D84B0.c` copies a 0x5408 buffer): `save0..2` = exactly **0x5408 bytes** = the raw g_gs blob (0x5400) + the 8-byte schema fingerprint
  (0x5098 / 0x2767B4 in all 73 states `[C:runtime]`). **No checksum, compression or encryption**, so an edited blob can be written as a save file.
  On save every `<task>.from_memcard` byte is forced to 1 (`BuildMemcardGsMask` 0x1D84B0). The fingerprint is the "schema checksum" above; it
  is not a checksum of the data.

- **SAVE** `g_save` = D_002E65E0: header (state +8, wanted state +0xC, times), 3 slots at +0x24 (stride 0x5464 = blob + header),
  current slot +0xFD50, MEMCARD record at +0xFD5C (semaphore, thread id +0xFD60 (-1 idle), priority 0x20). `UpdateSave` runs every frame from UpdateSw `[H r4 memcard.md]`; idle state/goal = 6/6 `[C:runtime S57-S62]`.
- **Autosave**: `AutoSaveGame` is called from `FinishJob` after the job-completion banner (WIDGET128) finishes, i.e. not when the last task completes but when its banner label ends `[C:static]`, consistent with S61/S62 `[C:runtime]` (the save itself was not captured).
- **Worker thread**: `CreateMemcardThread` stores a gcc pointer-to-member `{0xFFFF0000, pfn}` and starts `MemcardThreadMain` 0x1AAF18,
  which calls that SAVE method on g_save and exits. Jobs: GetInfo 0x1AC598, GetDir 0x1ABA10, CheckCard 0x1ABBE0, SaveSlot 0x1ABB60,
  CreateSave 0x1AB868, Format 0x1AB6C0, WriteSave 0x1ABC68 (tables 0x2A6BA0..0x2A6E94) `[H r4 memcard.md]`.
- sceMc wrappers (Open/Close/Read/Write/Mkdir/Delete + `McSyncResult` mapping errors to UI messages) `[H r4]`.
- Files: directory `/BASCUS-97316YAOTWTD!`, files `save%d` and `icon.sys` (title "Sly 2"); the icon is copied raw from the disc
  record 0x2A38E8 (`load_sly_icon_from_disc` 0x1AC238) `[C:static]` strings, `[H r4]`.
- UI: a dialog object D_002E6550 (vtable 0x2D3750) and a status object (vtable 0x2D36D0), BASIC-derived screens updated from
  `UpdateUi`; menu table 0x2BFDA0, text providers 0x2A9130 ("%s : %d%%", "episodeBest") `[H r4]`. Class names `CMemcardDialog`/`CMcStatus` are coinage.
- Existing names `FUN_0018xxxx` in this TU are **Sly 1 addresses**, not names `[H r4 fixes]`. `InitSave`, `CompareSaves`, `FMemcardThreadValid` are existing symbols (verified consistent).

## 3. Scratchpad buffers (sprbuf.cpp)

`LoadSprBuf`/`SaveSprBuf` (existing) and `SwapSprBuf` 0x201998 (DMA sync and flip of a double buffer) `[H r7]`: scratchpad staging, unrelated to saves despite the name.

## 4. Source files

{{TUS}}

## 5. Key functions

{{KEYFUNCS:0x1D7B88 0x1D7C80 0x1D8A38 0x1AAF18 max=60 minc=0.7}}

## Open questions
- SAVE state enum (+8/+0xC values 2,3,5,6) and dialog states (0x11..0x2E; 0x22 = no card). Experiment: pull the card at the slot-select screen, break `SetMcDialogState` 0x1ACB58.
- Meaning of kinds 5-10 in the schema (element sizes 16, 16, 8, 12, 4, 4).
- Does the schema checksum ever differ between world files in retail (it is identical in the two samples)?
- Experiment: patch 0x1D7C80 to skip the memset and confirm progress survives a schema mismatch.
