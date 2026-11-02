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

| Span | File (label) | Conf | Notes |
|---|---|---|---|
| 0x1AA978-0x1AE150 | memcard | high | ends with $D stub 0x1AE130 |
| 0x1D7B88-0x1D93D8 | save | high |  |

## 5. Key functions

| Addr | Name | Purpose | Conf | Tier | Evidence |
|---|---|---|---|---|---|
| [0x1AA978](../../src/auto/func_001AA978.c) | `StartupMemcard` | Startup-table entry: reset D_002DEB30 (0x1613F8), clear D_004425D8, InitSave(&g_save,0,0) | 0.85 | T1 | verified |
| [0x1AA9B8](../../src/auto/FUN_0018b780.c) | `FillMcDirInfo` | memset + fill a save-directory info block from sceMcGetDir results | 0.60 | existing | arbitration; replaces existing FUN_0018b780 |
| [0x1AAB48](../../src/hand/FUN_0018b910__Fi.cpp) | `McResFromSyncState` | sceMcSync state -> internal result: 0 -> 0x1D (done), >0 -> 0x1E, <0 -> 0x1F | 0.50 | existing | arbitration; replaces existing FUN_0018b910__Fi |
| [0x1AAB68](../../src/hand/FUN_0018b930__Fi.cpp) | `McResFromSyncCmd` | sceMcSync cmd result -> 0x21 (0), 0x22 (1), 0x20 (-1), else 0x1F | 0.50 | existing | arbitration; replaces existing FUN_0018b930__Fi |
| [0x1AABC0](../../src/hand/FUN_0018b988.c) | `MsgFromMcError` | (kind 1..21, sceMc error) -> message id for the UI | 0.60 | existing | arbitration; replaces existing FUN_0018b988 |
| [0x1AAED8](../../src/auto/func_001AAED8.c) | `InitMemcard` | MEMCARD init: SemaCreate(1,1), thread id -1, prio 0x20 | 0.75 | T1 | verified |
| [0x1AAF18](../../src/hand/func_001AAF18.c) | `MemcardThreadMain` | Memcard worker thread body: WaitSema, call the queued SAVE member-function job on g_save, clear job, signal, ExitThread | 0.60 | T1 |  |
| [0x1AAF90](../../src/hand/func_001AAF90.c) | `CreateMemcardThread` | Wait for previous job, store pointer-to-member job {0xFFFF0000,pfn}, CreateThread+StartThread | 0.85 | T1 | verified |
| [0x1AB040](../../src/auto/func_001AB040.c) | `SetMemcardThreadHighPrio` | If thread valid ChangeThreadPriority(id,1); then wait/signal sema (block until job finishes) | 0.60 | T1 | verified |
| [0x1AB088](../../src/auto/FMemcardThreadValid__FP7MEMCARD.c) | `FMemcardThreadValid__FP7MEMCARD` | Thread id (+4) != -1 | 0.90 | existing | verified |
| [0x1AB098](../../src/hand/FUN_0018bc90.c) | `McSyncResult` | Poll sceMcSync(0,&cmd,&res) and map to result; on error map through MsgFromMcError | 0.60 | existing | arbitration; replaces existing FUN_0018bc90 |
| [0x1AB1A0](../../src/hand/func_001AB1A0.c) | `UpdateSave` | Per-frame save state machine: skip during transitions/when thread busy; poll card (job D_002A6BA0) or advance state (0x1AB418); autosave trigger with  | 0.80 | T1 | verified |
| [0x1AB398](../../src/auto/CompareSaves__FP4SAVET0.c) | `CompareSaves__FP4SAVET0` | existing | 0.85 | existing | verified |
| [0x1AB6C0](../../src/auto/func_001AB6C0.c) | `McJobFormat` | Thread job: sceMcFormat(port,slot); result -> SAVE next state 2/3 and UI state | 0.75 | T1 |  |
| [0x1AB750](../../src/hand/FUN_0018c1e8.cpp) | `FOpenSaveIconFile` | Build a disc FLOC (lba=(w0^w13)+0xCA0,size=w1^w15) for a file entry and open it via 0x14B670 | 0.35 | existing | arbitration; replaces existing FUN_0018c1e8 |
| [0x1ABA10](../../src/hand/FUN_0018c420.c) | `McJobGetDir` | Thread job: sceMcGetInfo + sceMcGetDir of save dir, read slot headers (0x1AC4C8) | 0.60 | existing | arbitration; replaces existing FUN_0018c420 |
| [0x1ABB10](../../src/hand/func_001ABB10.c) | `BuildSaveFilePath` | sprintf('/%s/%s%d', 'BASCUS-97316YAOTWTD!', 'save', i) | 0.85 | T1 | verified |
| [0x1ABB60](../../src/hand/func_001ABB60.c) | `McJobSaveSlot` | Thread job: card info check, CreateSaveDir, write one slot file (0x1AC420) | 0.85 | T1 | verified |
| [0x1ABE78](../../src/hand/FUN_0018c888.c) | `ReadMcDir` | sceMcGetDir into table, then FillMcDirInfo | 0.50 | existing | arbitration; replaces existing FUN_0018c888 |
| [0x1ABFC0](../../src/hand/FUN_0018c9c0.c) | `ReadMcDirWrapper` | wrapper of 0x1ABE78 | 0.35 | existing | arbitration; replaces existing FUN_0018c9c0 |
| [0x1AC008](../../src/hand/FUN_0018ca10.c) | `WriteIconSys` | Build PS2 icon.sys (memset, title 'Sly 2', sprintf) and write it | 0.60 | existing | verified, arbitration; replaces existing FUN_0018ca10 |
| [0x1AC260](../../src/hand/func_001AC260.cpp) | `load_file_from_disc` | Open disc file '%s/%s' via BIS, read into alloc, write to card with sceMcOpen/Write/Close | 0.60 | T1 | verified |
| [0x1AC420](../../src/hand/FUN_0018d2e8.c) | `WriteSaveSlotFile` | BuildSaveFilePath + McWriteFile | 0.40 | existing | arbitration; replaces existing FUN_0018d2e8 |
| [0x1AC468](../../src/hand/FUN_0018d328.c) | `WriteSaveSlotFileReset` | ResetGs then BuildSaveFilePath + McWriteFile | 0.35 | existing | arbitration; replaces existing FUN_0018d328 |
| [0x1AC748](../../src/auto/func_001AC748.c) | `McMkdir` | sceMcMkdir wrapper + sync | 0.70 | T1 |  |
| [0x1AC7A0](../../src/auto/func_001AC7A0.c) | `McOpen` | sceMcOpen wrapper + sync | 0.85 | T1 | verified |
| [0x1AC808](../../src/auto/func_001AC808.c) | `McClose` | sceMcClose wrapper + sync | 0.70 | T1 |  |
| [0x1AC850](../../src/hand/func_001AC850.c) | `McRead` | sceMcRead + McSyncResult | 0.70 | T1 |  |
| [0x1AC8A8](../../src/hand/func_001AC8A8.c) | `McWrite` | sceMcWrite + McSyncResult | 0.70 | T1 |  |
| [0x1AC900](../../src/hand/FUN_0018d368.c) | `McWriteFile` | McOpen(create) + McWrite + McClose | 0.60 | existing | arbitration; replaces existing FUN_0018d368 |
| [0x1ACA40](../../src/auto/func_001ACA40.c) | `McDelete` | sceMcDelete wrapper | 0.85 | T1 | verified |
| [0x1ACB58](../../src/hand/func_001ACB58.c) | `SetMcDialogState` | x40 = state; if shown (x24) call vt10 and child vt49(-1) | 0.85 | T1 | verified |
| [0x1ACBB8](../../src/hand/func_001ACBB8.c) | `ClearMcDialogState` | x40=-1; vt11 and child vt50 | 0.85 | T1 | verified |
| [0x1ACCC8](../../src/hand/func_001ACCC8.c) | `CMemcardDialog_Update` | vt7: when dialog fully shown >0.1s handle pad input (0x187D80) per KMcDialogButtons, start jobs (CreateMemcardThread), transition (0x1ACFE0) | 0.85 | T1 | verified |
| [0x1ADE70](../../src/hand/func_001ADE70.c) | `HideMcDialogWidget` | small | 0.60 | T1 | verified |
| [0x1ADF78](../../src/auto/func_001ADF78.c) | `CMcStatus_SetMode` | vt14 same body as 0x1ACC18 | 0.30 | T2 | runtime-corrected |
| [0x1D7B88](../../src/game/world/state/save/func_001D7B88.c) | `LoadSaveSchemaFromBrx` | Load save-variable schema: U16 symbol count -> hash table of (U16 id, string) names; U16 entry count -> c*10-byte trie entries; then reset g_gs if sch | 0.90 | T1 | runtime, format-doc |
| [0x1D7C80](../../src/game/world/state/save/func_001D7C80.c) | `ResetGsIfSchemaChanged` | Byte-sum the schema trie and read U16 version at end; if differs from GS+0x5400/+0x5404 memset GS 0x5400 bytes and store new version/sum | 0.70 | T1 |  |
| [0x1D7D30](../../src/game/world/state/save/func_001D7D30.c) | `FGsMatchesSchema` | True if GS stored version/checksum (+0x5400/+0x5404) equal schema's | 0.75 | T1 |  |
| [0x1D7DA0](../../src/game/world/state/save/func_001D7DA0.c) | `FFindSaveVar` | Walk the schema trie (10-byte ENT {short sym; s8 kind; u8 bits; u16 ib; u16 child; u16 sibling}) along a key of symbol ids; outputs entry + array inde | 0.75 | T1 |  |
| [0x1D7E80](../../src/game/world/state/save/func_001D7E80.c) | `PvFromSaveVar` | Address of a save var in a GS blob: pb + ib + index*size[kind] | 0.85 | T1 | verified |
| [0x1D7ED8](../../src/game/world/state/save/func_001D7ED8.cpp) | `RefGetSaveVar` | Read a save var as a Splice CRef: switch on kind (int/float/bool or bitfield/vector/qw/v2/v3...) | 0.75 | T1 |  |
| [0x1D80C8](../../src/game/world/state/save/func_001D80C8.cpp) | `RefSetSaveVar` | Write a CRef into a save var by kind (bitfield set/clear for packed bools) | 0.75 | T1 |  |
| [0x1D82B8](../../src/game/world/state/save/func_001D82B8.cpp) | `NCompletionPercentGs` | Game completion percent: weighted sum of 9 progress vars (weights D_002C2DC0) plus 0x1A per set 'fVault' bool/bit; returns 0 / 100 / clamp(1..99) | 0.85 | T1 | verified |
| [0x1D8640](../../src/game/world/state/save/func_001D8640.c) | `UHashPchzCi` | Case-insensitive string hash h=h*33+toupper(c) | 0.75 | T1 |  |
| [0x1D8690](../../src/game/world/state/save/func_001D8690.c) | `AddSaveSymbol` | Insert (name,id) into 53-bucket chained hash table, taking entries from the end of the preallocated array | 0.70 | T1 |  |
| [0x1D8720](../../src/game/world/state/save/func_001D8720.c) | `NFindSaveSymbol` | Lookup name -> symbol id in 53-bucket hash (strcmp-ish 0x2342D0), -1 if absent | 0.75 | T1 |  |
| [0x1D87C0](../../src/game/world/state/save/func_001D87C0.c) | `BuildSaveKeyV` | Turn a NULL-terminated varargs list of strings into a key (short array of symbol ids; non-symbols via atoi as array indices) | 0.70 | T1 |  |
| [0x1D8A38](../../src/game/world/state/save/func_001D8A38.cpp) | `RefGetSaveVarV` | CRef GetSaveVar(const char *name, ...): build key on stack frame and read from g_gs D_003D4A60 | 0.90 | T1 | runtime, verified |
| [0x1D8B20](../../src/game/world/state/save/func_001D8B20.cpp) | `RefSetSaveVarV` | CRef SetSaveVar(const CRef &val, const char *name, ...) into g_gs | 0.85 | T1 | verified |
| [0x1D8FF8](../../src/game/world/state/save/func_001D8FF8.cpp) | `ApplyGsSettings` | Apply option vars from GS: vibration(?), sound playback mode, music/sfx volumes (1-g), a flag at D_003DD050, screen X/Y offset (GS DISPLAY DX=g+636, D | 0.70 | T1 |  |

Selected 51 of 98 named functions (T1 with confidence >= 0.7, or runtime/verified/parse/arbitration evidence, or cited in the text). Full list: [reference/functions/save-memcard.md](../reference/functions/save-memcard.md).

## Open questions
- SAVE state enum (+8/+0xC values 2,3,5,6) and dialog states (0x11..0x2E; 0x22 = no card). Experiment: pull the card at the slot-select screen, break `SetMcDialogState` 0x1ACB58.
- Meaning of kinds 5-10 in the schema (element sizes 16, 16, 8, 12, 4, 4).
- Does the schema checksum ever differ between world files in retail (it is identical in the two samples)?
- Experiment: patch 0x1D7C80 to skip the memset and confirm progress survives a schema mismatch.
