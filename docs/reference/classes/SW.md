# SW (CID 1, 0x01)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> ctor calls 0x20D9D0 (sw TU); size 0x4790 matches Lilly SW (last field 0x475C); parent now BASIC (Sly1: LO)

| | |
|---|---|
| Name | `SW` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [SW](SW.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x4790 (18320 bytes), +0x4780 over BASIC |
| Descriptor | `D_002BAAFC` (record 1 of D_002BAAE8; cid range 1..2) |
| Factory (`pfnNew`) | [`NewSw`](../../../src/auto/NewSw.c) `0x110AB0` |
| Descriptor getter (slot 6) | `GetSwClassDesc` `0x113A20` |
| vtable | `D_002DC018`, 10 entries (slots 0..9; slot 0 null), 3 new, 1 overridden |
| vtable stored by | [`SW_ctor`](../../../src/hand/SW_ctor.cpp) |
| Level instances | none (not a world-file object class) |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (BASIC) | `HandleBasicSpliceEvent` | `0x1331F8` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetSwClassDesc`](../../../src/auto/GetSwClassDesc.c) | `0x113A20` | Pclassinfo |
| 7 | UpdateSw | **new** | [`UpdateSw`](../../../src/hand/UpdateSw.c) | `0x226550` | VSlot7 |
| 8 | RenderSw | **new** | [`RenderSw?`](../../../src/hand/func_001CDA80.c) | `0x1CDA80` | VSlot8 |
| 9 | DrawSw | **new** | [`DrawSw?`](../../../src/hand/func_00157FA0.c) | `0x157FA0` | VSlot9 |

## Fields

From `include/sly2/sw.h` (`struct SW : BASIC`). SW: from src/hand/GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi.cpp (7 copies in Lilly ports)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 18 header asserts agree, 0 disagree. Size from header: 0x4790.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x10 | `struct DL` | `dlChild` | asserted (root DL ibDle 0x24 (CONFIRMED runtime; Lilly name dlChild)) |  |
| 0x1C | `LO *` | `ploRemovedFirst` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x20 | `struct DL` | `dlRoot` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x2C | `struct DL` | `dlBusy` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x38 | `SL[512]` | `aslOid` | asserted (oid hash, 512 SLs (CONFIRMED runtime)) |  |
| 0x1838 | `SL[512]` | `aslPxrOid` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3038 | `SL[199]` | `aslCid` | asserted (per-class SL lists, 12-byte stride (CONFIRMED runtime)) |  |
| 0x398C | `struct SLOTHEAP` | `slotheapLoroot` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x39A4 | `LOROOT *` | `plorootSettled` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x39A8 | `uint` | `grfvisBusy` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x39AC | `uint32_t[1]` | `_pad_185` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x39B0 | `struct VECTOR4` | `posCmBusy` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x39C0 | `struct VECTOR4` | `posHeroBusy` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x39D0 | `void *` | `aUnk39d0` | unknown (padding/placeholder) |  |
| 0x39D4 | `int` | `cUnk39d4` | unknown (padding/placeholder) |  |
| 0x39D8 | `void *` | `aUnk39d8` | unknown (padding/placeholder) |  |
| 0x39DC | `int` | `cUnk39dc` | unknown (padding/placeholder) |  |
| 0x39E0 | `LO **` | `aploNew` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x39E4 | `LO **` | `pploNew` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x39E8 | `SPLCS *` | `psplcs` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x39EC | `SPLCS *` | `psplcsHead` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x39F0 | `struct SL` | `slEvent` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x39FC | `struct SLOTHEAP` | `slotheapEvent` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3A14 | `int` | `fUnk3a14` | unknown (padding/placeholder) |  |
| 0x3A18 | `struct SAVEVARS` | `savevars` | asserted (save-var schema (CONFIRMED runtime)) |  |
| 0x3AFC | `int` | `ctask` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B00 | `uint32_t[3]` | `_pad_202` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B0C | `LO *` | `ploJobCur` | asserted (CONFIRMED runtime) | +0x3B0C [R] current JOB (CID 0x84); mirrors g_game+0x10 oid (runtime FINDINGS, slygame SW_CURTASK) |
| 0x3B10 | `LO *` | `ploTaskCur` | header (Lilly-ported; offset computed, unasserted) | +0x3B10 [R] current TASK (CID 0x85) |
| 0x3B14 | `LO *` | `ploTaskPending` | header (Lilly-ported; offset computed, unasserted) | +0x3B14 [R] pending TASK |
| 0x3B18 | `LO *` | `ploJobFailed` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B1C | `SVPATH *` | `psvpDt` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B20 | `SVPATH *` | `psvpCcoin` | asserted (CONFIRMED runtime) | +0x3B20 [R] -> save var loot.ccoin (g_gs+0xA0) (runtime3b FINDINGS) |
| 0x3B24 | `SVPATH *` | `psvpWidCurrent` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B28 | `SVPATH *` | `psvpEpisodeCurrent` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B2C | `SVPATH *` | `psvpLang` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B30 | `SVPATH *` | `psvpMixerMode` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B34 | `SVPATH *` | `psvpMusicVolume` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B38 | `SVPATH *` | `psvpEffectsVolume` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B3C | `SVPATH *` | `psvpUseHeadset` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B40 | `SVPATH *` | `psvpScreenCenterX` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B44 | `SVPATH *` | `psvpScreenCenterY` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B48 | `SVPATH *` | `psvpNoVibration` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B4C | `SVPATH *` | `psvpReverseLookAround` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B50 | `SVPATH *` | `psvpReverseAim` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B54 | `SVPATH *` | `psvpAutosaveWarned` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B58 | `SVPATH *[9]` | `apsvpEpisodeDt` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3B7C | `SVPATH *[9]` | `apsvpEpisodePercent` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3BA0 | `HUDSTACK[4]` | `ahudstack` | asserted (4 records of 12 bytes (CONFIRMED runtime)) |  |
| 0x3BD0 | `uint32_t[7]` | `_pad_228` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3BEC | `int` | `fUnk3bec` | unknown (padding/placeholder) |  |
| 0x3BF0 | `int` | `fTransitionPending` | asserted (CONFIRMED runtime + code) |  |
| 0x3BF4 | `uint` | `grfTransition` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3BF8 | `struct FILEREC` | `filerecDest` | asserted (CONFIRMED runtime + code) | +0x3BF8 [R+S] requested level FILEREC (0x40, XOR-obfuscated); == filerecCur after load |
| 0x3C38 | `int` | `oidWarpDest` | asserted (CONFIRMED runtime + code) |  |
| 0x3C3C | `int` | `oidWarpContext` | header (Lilly-ported; offset computed, unasserted) | +0x3C3C [S] requested warp context oid (-1); was nUnk3c3c |
| 0x3C40 | `struct FILEREC` | `filerecCur` | asserted (CONFIRMED runtime) | +0x3C40 [R] current level FILEREC (equal to filerecDest in every state); was _pad_235[16] |
| 0x3C80 | `int` | `nLoadFilter` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3C84 | `int` | `fUnk3c84` | unknown (padding/placeholder) |  |
| 0x3C88 | `int` | `nUnk3c88` | unknown (padding/placeholder) |  |
| 0x3C8C | `int` | `nUnk3c8c` | unknown (padding/placeholder) |  |
| 0x3C90 | `struct SLOTHEAP` | `slotheapCallback` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3CA8 | `struct SL` | `slCallback` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3CB4 | `struct SLOTHEAP` | `slotheapRefnode` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3CCC | `int` | `fAaoxUnsorted` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3CD0 | `int` | `caaox` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3CD4 | `struct AAOX *[3]` | `aaaox` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3CE0 | `struct SLOTHEAP` | `slotheapSotree` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3CF8 | `struct SLOTHEAP` | `slotheapXplist` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3D10 | `struct DL` | `dlSoTreeAll` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3D1C | `struct DL` | `dlSoTree` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3D28 | `struct DL` | `dlSoTreeQueue1` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3D34 | `struct DL` | `dlSoTreeQueue2` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3D40 | `struct SLOTHEAP` | `slotheapXp` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3D58 | `int` | `nStampXf` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3D5C | `int` | `nUnk3d5c` | unknown (padding/placeholder) |  |
| 0x3D60 | `int` | `nStampLights` | asserted (CONFIRMED runtime (frame counter)) | +0x3D60 [R] counts up once per world frame (S12->S13: +32 for 0.533 s game time); Lilly name kept |
| 0x3D64 | `uint32_t[1]` | `_pad_256` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3D68 | `int` | `nStimulusSerial` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3D6C | `int` | `cEventWorld` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3D70 | `int` | `nUnk3d70` | unknown (padding/placeholder) |  |
| 0x3D74 | `int` | `nUnk3d74` | unknown (padding/placeholder) |  |
| 0x3D78 | `int` | `cHandsOff` | asserted (CONFIRMED runtime) | +0x3D78 [R] hands-off counter (1 during the safehouse char-select; runtime6 FINDINGS) |
| 0x3D7C | `int` | `cNoPause` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3D80 | `int` | `cUnk3d80` | unknown (padding/placeholder) |  |
| 0x3D84 | `int` | `cUnk3d84` | unknown (padding/placeholder) |  |
| 0x3D88 | `struct DL` | `dlAsega` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3D94 | `struct SLOTHEAP` | `slotheapAsega` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3DAC | `struct SLOTHEAP` | `slotheapAct` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3DC4 | `struct SLOTHEAP` | `slotheapOvra` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3DDC | `struct DL` | `dlSma` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3DE8 | `struct SLOTHEAP` | `slotheapSma` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3E00 | `struct RGBA` | `rgbaClear` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3E04 | `uint32_t[3]` | `_pad_272` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3E10 | `struct VECTOR4` | `hsvLight` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3E20 | `float` | `gLightScale` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3E24 | `float` | `gMidtone` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3E28 | `struct VECTOR` | `vecLightRgb` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3E34 | `float` | `uMidtone` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3E38 | `int` | `fUnk3e38` | unknown (padding/placeholder) |  |
| 0x3E3C | `uint32_t[2]` | `_pad_279` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3E44 | `struct SLOTHEAP` | `slotheapSobounds` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3E5C | `void *` | `pvUnk3e5c` | unknown (padding/placeholder) |  |
| 0x3E60 | `CID116 *` | `pcid116Fade` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3E64 | `uint32_t[18]` | `_pad_283` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3EAC | `struct SL` | `slPoctl` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3EB8 | `uint` | `cpo` | asserted (CONFIRMED runtime) |  |
| 0x3EBC | `PO *[16]` | `apo` | header (Lilly-ported; offset computed, unasserted) | +0x3EBC [R] player objects (oids 8 Bentley, 7 Sly(JT), 9 Murray, 0x4357 CID 0x46 char-select) |
| 0x3EFC | `PO *` | `ppoHero` | asserted (current player, CONFIRMED runtime) | +0x3EFC [R] CURRENT player (PpoCur): follows character switches; g_pjt stays Sly (runtime3) |
| 0x3F00 | `PO *` | `ppoOverride` | asserted (CONFIRMED runtime) | +0x3F00 [R] override player; 0 in all states; current = ppoOverride ? ppoOverride : ppoHero |
| 0x3F04 | `int` | `cproxysrc` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F08 | `PROXYSRC *` | `aproxysrc` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F0C | `PXR *` | `ppxrRoot` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F10 | `int` | `isrcLoading` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F14 | `int` | `cloupd` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F18 | `int` | `cloupdHole` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F1C | `int *` | `piloupdCur` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F20 | `LOUPD *` | `aloupd` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F24 | `struct DL` | `dlTarget` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F30 | `struct DL` | `dlCid11` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F3C | `struct DL` | `dlCid13` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F48 | `struct DL` | `dlCid39` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F54 | `struct DL` | `dlCid88` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F60 | `struct DL` | `dlCid12` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F6C | `struct DL` | `dlCid34` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F78 | `struct DL` | `dlCid38` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F84 | `struct DL` | `dlCid35` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F90 | `struct DL` | `dlCid40` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3F9C | `struct DL` | `dlCid9` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3FA8 | `struct DL` | `dlCid27` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3FB4 | `struct DL` | `dlStepguard` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3FC0 | `uint32_t[6]` | `_pad_310` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3FD8 | `struct DL` | `dlCid47` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3FE4 | `uint32_t[6]` | `_pad_312` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3FFC | `struct DL` | `dlStimulus` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4008 | `struct SLOTHEAP` | `slotheapStimulus` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4020 | `struct STIMK *[7]` | `apstimk` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x403C | `struct SLOTHEAP` | `slotheapUnk403c` | unknown (padding/placeholder) |  |
| 0x4054 | `struct SLOTHEAP` | `slotheapUnk4054` | unknown (padding/placeholder) |  |
| 0x406C | `uint32_t[1]` | `_pad_318` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4070 | `struct SLOTHEAP` | `slotheapUnk4070` | unknown (padding/placeholder) |  |
| 0x4088 | `struct SLOTHEAP` | `slotheapUnk4088` | unknown (padding/placeholder) |  |
| 0x40A0 | `uint32_t[6]` | `_pad_321` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x40B8 | `struct SLOTHEAP` | `slotheapSsge` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x40D0 | `uint32_t[6]` | `_pad_323` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x40E8 | `struct SLOTHEAP` | `slotheapXa` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4100 | `uint32_t[6]` | `_pad_325` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4118 | `struct SLOTHEAP` | `slotheapZprHit` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4130 | `struct DL` | `dlZprHit` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x413C | `uint32_t[263]` | `_pad_328` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4558 | `int` | `ccell` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x455C | `CELL *` | `acell` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4560 | `int` | `cleaf` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4564 | `void *` | `aleaf` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4568 | `uint32_t[24]` | `_pad_333` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x45C8 | `CID17 *` | `pcid17Default` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x45CC | `REGTREE *` | `pregtree` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x45D0 | `struct CLQ` | `clqGuardScale` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x45E0 | `struct LM` | `lmGuardScale` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x45E8 | `uint32_t[20]` | `_pad_338` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4638 | `LO *` | `ploUnk4638` | unknown (padding/placeholder) |  |
| 0x463C | `int` | `cCid37` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4640 | `int` | `clangstr` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4644 | `LANGSTR *` | `alangstr` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4648 | `uint32_t[12]` | `_pad_343` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4678 | `int` | `cjtgrabcand` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x467C | `undefined4` | `unk_0x467c` | unknown (padding/placeholder) |  |
| 0x4680 | `JTGRABCAND[4]` | `ajtgrabcand` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4740 | `int` | `idHintGrab` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4744 | `LO *` | `ploUnk4744` | unknown (padding/placeholder) |  |
| 0x4748 | `int` | `fGuardsFrozen` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x474C | `float` | `tGuardsFrozen` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4750 | `float` | `tLookLast` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4754 | `uint32_t[2]` | `_pad_352` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x475C | `float` | `gUnk475c` | unknown (padding/placeholder) |  |
| 0x4760 | `uint32_t[1]` | `_pad_354` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4764 | `int` | `cClue` | asserted (CONFIRMED runtime) | +0x4764 [R] clue bottles collected in this level (HUD; 0 -> 1 between S04 and S05) |
| 0x4768 | `int` | `cClueTotal` | header (Lilly-ported; offset computed, unasserted) | +0x4768 [R] clue bottles in this level (30 in the Paris hub) |
| 0x476C | `float` | `tHudTimerEnd` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4770 | `int` | `csecHudTimerShown` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4774 | `uint32_t[1]` | `_pad_357` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4778 | `int` | `cpemitTracked` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x477C | `struct EMITTER *[4]` | `apemitTracked` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x478C | `uint32_t[1]` | `_pad_360` | header (Lilly-ported; offset computed, unasserted) |  |

Inherited fields: see [BASIC](BASIC.md).

## Related docs

[engine/world-sw.md](../../engine/world-sw.md) (46), [engine/object-model.md](../../engine/object-model.md) (17), [engine/overview.md](../../engine/overview.md) (17), [engine/hud-ui.md](../../engine/hud-ui.md) (12), [engine/player.md](../../engine/player.md) (11), [engine/rendering.md](../../engine/rendering.md) (10), [engine/game-progression.md](../../engine/game-progression.md) (9), [engine/physics-collision.md](../../engine/physics-collision.md) (9)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
