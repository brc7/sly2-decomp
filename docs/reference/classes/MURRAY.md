# MURRAY (CID 54, 0x36)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> Lilly MURRAY : STEPPLAYER ~0x1B00; own code TU 0x1B4728 ("murray"). Sly1 MURRAY was a STEPGUARD

| | |
|---|---|
| Name | `MURRAY` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [MURRAY](MURRAY.md) → [STEPPLAYER](STEPPLAYER.md) → [STEP](STEP.md) → [PO](PO.md) → [SO](SO.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x1B00 (6912 bytes), +0x7E0 over STEPPLAYER |
| Descriptor | `D_002BAF20` (record 54 of D_002BAAE8; cid range 54..55) |
| Factory (`pfnNew`) | [`NewMurray`](../../../src/game/app/class_registry.cpp) `0x111700` |
| Descriptor getter (slot 6) | `GetMurrayClassDesc` `0x113D70` |
| vtable | `D_002D44C8`, 106 entries (slots 0..105; slot 0 null), 0 new, 24 overridden |
| vtable stored by | [`MURRAY_ctor?`](../../../src/game/app/class_registry.cpp) (vptr +0xC) |
| Level instances | 42 objects in 42 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | **override** | [`HandleMurrayMessage`](../../../src/game/gameplay/player/murray.cpp) | `0x1B68B8` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetMurrayClassDesc`](../../../src/game/app/class_registry.cpp) | `0x113D70` | Pclassinfo |
| 7 | Init | **override** | [`InitMurray`](../../../src/game/gameplay/player/murray.cpp) | `0x1B4728` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | inherited (SO) | `SetSoParent` | `0x1F21B8` | SetParent |
| 11 | AddHierarchy | inherited (SO) | `AddSoHierarchy` | `0x1F2C30` | AddHierarchy |
| 12 | RemoveHierarchy | inherited (STEPPLAYER) | `RemoveHierarchyStepplayer?` | `0x141928` | RemoveHierarchy |
| 13 | UpdateLoroot | inherited (SO) | `UpdateSoLoroot` | `0x1F2E78` | UpdateLoroot |
| 14 | LoadFromBrx | **override** | [`LoadMurrayFromBrx`](../../../src/game/gameplay/player/murray.cpp) | `0x1B4888` | LoadFromBrx |
| 15 | PostLoad | **override** | [`PostMurrayLoad`](../../../src/game/gameplay/player/murray.cpp) | `0x1B49E0` | VUnk14 |
| 16 | Update | **override** | [`UpdateMurray`](../../../src/game/gameplay/player/murray.cpp) | `0x1B50B0` | VUnk15 |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (SO) | `UpdateSoXfWorld?` | `0x1F3980` | VUnk16 |
| 18 | UpdateLoXfWorldHierarchy | inherited (SO) | `UpdateSoXfWorldHierarchy` | `0x22FBE0` | UpdateXfWorldHierarchy |
| 19 | RenderSelf | inherited (STEPPLAYER) | `RenderStepplayerSelf` | `0x1425B0` | RenderSelf |
| 20 | RenderLoAll | inherited (SO) | `RenderSoAll?` | `0x1F3CC8` | VUnk19 |
| 21 | SetLoFlag8 | inherited (SO) | `SetSoFlag8?` | `0x1F3B28` | VUnk20 |
| 22 | OnRemove | inherited (SO) | `OnSoRemove` | `0x1F3E00` | Freeze |
| 23 | OnAdd | inherited (SO) | `OnSoAdd` | `0x1F3EA8` | Unfreeze |
| 24 |  | inherited (LO) | `func_00210330` | `0x210330` | VUnk23 |
| 25 | SetIdProxy | inherited (SO) | `SetSoIdProxy` | `0x1CA308` | SetIdProxy |
| 26 | ResetLoForProxy | inherited (SO) | `ResetSoForProxy?` | `0x1CB518` | VUnk25 |
| 27 | LO_VUnk27Proxy | inherited (SO) | `SO_VUnk27Proxy?` | `0x1CB5D0` | VUnk26 |
| 28 | CploChildren | inherited (LO) | `LO_CploChildren` | `0x1A5BE8` | CploChildren |
| 29 | TranslateToPos | inherited (STEPPLAYER) | `TranslateStepplayerToPos` | `0x143CF8` | TranslateToPos |
| 30 | RotateToMat | inherited (STEP) | `RotateStepToMat` | `0x2041F8` | RotateToMat |
| 31 | PmatWorld | inherited (SO) | `GetSoWorldMatrix` | `0x1F3FA0` | PmatWorld |
| 32 | LO_PxfLocal | inherited (SO) | `SO_PxfLocal?` | `0x1F3FB0` | PxfLocal |
| 33 | GetVelocity | inherited (SO) | `GetSoVelocity` | `0x1F4440` | GetVelocity |
| 34 | GetAngularVelocity | inherited (SO) | `GetSoAngularVelocity` | `0x1F4558` | GetAngularVelocity |
| 35 | SetVelocity | inherited (SO) | `SetSoVelocity` | `0x1F4488` | SetVelocity |
| 36 | SetAngularVelocity | inherited (SO) | `SetSoAngularVelocity` | `0x1F46D8` | SetAngularVelocity |
| 37 | ApplyLoProxyHierarchy | inherited (SO) | `ApplySoProxy?` | `0x1F4010` | ApplyProxy |
| 38 | MatchLoXf | inherited (STEP) | `MatchStepXf` | `0x2052E0` | VUnk37 |
| 39 | SetLoXf | inherited (SO) | `SetSoXf?` | `0x1F4A80` | VUnk38 |
| 40 | CloneHierarchy | inherited (SO) | `CloneSoHierarchy` | `0x1F66F0` | CloneHierarchy |
| 41 | Clone | inherited (STEP) | `CloneStep` | `0x203A08` | Clone |
| 42 | SetLoSBusy | inherited (SO) | `SetSoSBusy?` | `0x1F3118` | VUnk41 |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` | VUnk42 |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` | VUnk43 |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` | VUnk44 |
| 46 | SetVisMask | inherited (SO) | `SetSoVisMask` | `0x1A6DC8` | SetVisMask |
| 47 | SetGrf | inherited (SO) | `SetSoGrf` | `0x1F2FE0` | VUnk46 |
| 48 | ClearLoGrf | inherited (SO) | `ClearSoGrf` | `0x1F3078` | VUnk47 |
| 49 | UpdateSoTreeBounds | inherited (SO) | `UpdateSoTreeBounds` | `0x1F50E0` | VUnk48 |
| 50 | GetSoCpdefi | **override** | [`GetMurrayCpdefi`](../../../src/game/gameplay/player/murray.cpp) | `0x1B6728` | GetCpdefi |
| 51 | SO_PxfFromId | inherited (SO) | `SO_PxfFromId?` | `0x22D778` | PxfFromIxf |
| 52 | CloneXfRef | inherited (SO) | `CloneXfRef?` | `0x1F4CA0` | CloneXfRef |
| 53 | SO_PtreeNew | inherited (SO) | `SO_PtreeNew?` | `0x1F56F8` | VUnk52 |
| 54 | SO_vt54_Nop | inherited (STEP) | `OnStepSoContactFreed` | `0x204C60` | VUnk53 |
| 55 | SO_vt55_Nop | **override** | [`AdjustMurrayNewXp`](../../../src/game/gameplay/player/murray.cpp) | `0x1B9130` | AdjustNewXp |
| 56 | SO_vt56_Nop | inherited (SO) | `SO_vt56_Nop?` | `0x1F4FB8` | VUnk55 |
| 57 | SO_vt57_Nop | inherited (STEP) | `AdjustStepXpVelocity` | `0x2044B0` | VUnk56 |
| 58 | ScheduleSoTouchEvents | inherited (STEPPLAYER) | `STEPPLAYER_VUnk57?` | `0x140D70` | VUnk57 |
| 59 | SO_vt59_Nop | inherited (STEPPLAYER) | `STEPPLAYER_VUnk58?` | `0x140D00` | VUnk58 |
| 60 | SO_vt60_Nop | inherited (STEP) | `AddStepCustomXps` | `0x204AC8` | VUnk59 |
| 61 | FIgnoreSoIntersection | **override** | [`MURRAY_FIgnoreIntersection`](../../../src/game/gameplay/player/murray.cpp) | `0x1B8450` | FIgnoreIntersection |
| 62 | FIsSoFixed | inherited (SO) | `FIsSoFixed?` | `0x1F5040` | VUnk61 |
| 63 | SO_vt63_Ret0 | inherited (STEP) | `FStepVt63?` | `0x204248` | CpartExtra |
| 64 | ResetSoPhys | inherited (SO) | `ResetSoPhys?` | `0x1F3590` | InitPhys |
| 65 | ApplySoForcesXf | inherited (SO) | `ApplySoForcesXf?` | `0x1F9E70` | VUnk64 |
| 66 | ProjectSoForce | inherited (SO) | `ProjectSoForce?` | `0x1FA328` | VUnk65 |
| 67 | ApplySoQueuedMotion | inherited (SO) | `ApplySoQueuedMotion?` | `0x1FA528` | VUnk66 |
| 68 | ApplySoContactImpulse | inherited (STEPPLAYER) | `STEPPLAYER_VUnk67?` | `0x140DB0` | VUnk67 |
| 69 | GetSoVelocityAtPos | inherited (SO) | `GetSoVelocityAtPos?` | `0x1FADD0` | VUnk68 |
| 70 | ApplySoPendingVelocity | **override** | [`PresetMurrayAccel`](../../../src/game/gameplay/player/murray.cpp) | `0x1B67F8` | VUnk69 |
| 71 | AddSoExternalAccelerations | inherited (SO) | `AddSoExternalAccelerations?` | `0x2297F0` | VUnk70 |
| 72 |  | inherited (STEPPLAYER) | `ResolveStepplayerXaContact?` | `0x1A28B0` | VUnk71 |
| 73 | FPosInSo | inherited (SO) | `FPosInSo?` | `0x1F72F8` | VUnk72 |
| 74 | InflictSoContactZaps | inherited (SO) | `InflictSoContactZaps?` | `0x1F7410` | ProcessSoZapContacts |
| 75 | FZprHasField44 | inherited (STEPPLAYER) | `STEPPLAYER_FTakeDamage` | `0x140F50` | FTakeDamage |
| 76 | FInvulnerablePo | **override** | [`MURRAY_vt76?`](../../../src/game/gameplay/player/murray.cpp) | `0x1B83B8` | FInvulnerable |
| 77 | JthsCurrentPo | inherited (STEPPLAYER) | `JthsCurrent__10STEPPLAYER` | `0x140F70` | JthsCurrent |
| 78 | OnPoActive | inherited (STEPPLAYER) | `OnStepplayerActive` | `0x144480` | OnActive |
| 79 | SetPoCtlState | inherited (STEPPLAYER) | `SetStepplayerCtlState` | `0x1406E8` | SetCtlState |
| 80 | FCanPoBeHealed | inherited (STEPPLAYER) | `FCanBeHealed__10STEPPLAYER` | `0x144090` | FCanBeHealed |
| 81 | AddPoHpMp | inherited (STEPPLAYER) | `AddStepplayerHpMp` | `0x144268` | AddHpMp |
| 82 | AddStepAxisXps | inherited (STEPPLAYER) | `STEPPLAYER_VUnk81?` | `0x140C30` | VUnk81 |
| 83 | AddStepDialogSmRefs | inherited (STEPPLAYER) | `STEPPLAYER_VUnk82?` | `0x1442D0` | VUnk82 |
| 84 | RemoveStepDialogSmRefs | inherited (STEPPLAYER) | `STEPPLAYER_VUnk83?` | `0x144338` | VUnk83 |
| 85 | GrfStepGroundProbe | **override** | [`MURRAY_vt85?`](../../../src/game/gameplay/player/murray.cpp) | `0x1B8368` | VUnk84 |
| 86 | FStepVt86 | inherited (STEPPLAYER) | `STEPPLAYER_VUnk85?` | `0x144418` | VUnk85 |
| 87 | StepVt87Nop | **override** | [`SetMurrayPlayable?`](../../../src/game/gameplay/player/murray.cpp) | `0x1B81B8` | VUnk86 |
| 88 | SetJtPlayerState (pure in STEPPLAYER) | **override** | [`SetMurrayPlayerState`](../../../src/game/gameplay/player/murray.cpp) | `0x1B7E40` | VUnk87 |
| 89 | GetJtPlayerState (pure in STEPPLAYER) | **override** | [`GetMurrayPlayerState`](../../../src/game/gameplay/player/murray.cpp) | `0x1B7F90` | VUnk88 |
| 90 | JT_vt90 (pure in STEPPLAYER) | **override** | [`ApplyMurrayStateStepParams?`](../../../src/game/gameplay/player/murray.cpp) | `0x1B8008` | VUnk89 |
| 91 | JT_vt91 (pure in STEPPLAYER) | **override** | [`MURRAY_vt91?`](../../../src/game/gameplay/player/murray.cpp) | `0x1B80F0` | VUnk90 |
| 92 | JT_FFaceTarget (pure in STEPPLAYER) | **override** | [`MURRAY_vt92?`](../../../src/game/gameplay/player/murray.cpp) | `0x1B80F8` | VUnk91 |
| 93 | OnStepplayerHeroDied | inherited (STEPPLAYER) | `OnStepplayerHeroDied` | `0x140EB0` | OnHeroDied |
| 94 | JT_vt94 (pure in STEPPLAYER) | **override** | [`MURRAY_vt94?`](../../../src/game/gameplay/player/murray.cpp) | `0x1B8180` | VUnk93 |
| 95 | SetStepplayerPlayerInteractMode | inherited (STEPPLAYER) | `SetStepplayerPlayerInteractMode` | `0x140678` | SetPlayerInteractMode |
| 96 | GetStepplayerStunHold | **override** | [`MURRAY_vt96?`](../../../src/game/gameplay/player/murray.cpp) | `0x1B9B70` | GetStunHold |
| 97 | GetStepplayerCmFocusPos | inherited (STEPPLAYER) | `GetStepplayerCmFocusPos` | `0x141588` | GetCmFocusPos |
| 98 | STEPPLAYER_VUnk97 | **override** | [`MURRAY_vt98?`](../../../src/game/gameplay/player/murray.cpp) | `0x1B8100` | VUnk97 |
| 99 | FCanUseGadget__10STEPPLAYERi | **override** | [`FCanUseGadget__6MURRAYi`](../../../src/game/gameplay/player/murray.cpp) | `0x1B98A0` | FCanUseGadget |
| 100 | StartStepplayerGadget | **override** | [`UseMurrayGadget`](../../../src/game/gameplay/player/murray.cpp) | `0x1B9A08` | StartGadget |
| 101 | UpdateStepplayerGadget | inherited (STEPPLAYER) | `UpdateStepplayerGadget` | `0x141DD0` | VSlot101 |
| 102 | StopStepplayerGadget | inherited (STEPPLAYER) | `StopStepplayerGadget` | `0x141E28` | VSlot102 |
| 103 | JT_vt103 (pure in STEPPLAYER) | **override** | [`MURRAY_PchName`](../../../src/game/gameplay/player/murray.cpp) | `0x1B9B60` | VSlot103 |
| 104 | STEPPLAYER_VUnk104 | inherited (STEPPLAYER) | `STEPPLAYER_VUnk104?` | `0x141E30` | VSlot104 |
| 105 | OnStepplayerGadgetsChanged | inherited (STEPPLAYER) | `OnStepplayerGadgetsChanged?` | `0x141E70` | VSlot105 |

## Fields

From `include/sly2/players.h` (`struct MURRAY : STEPPLAYER`). MURRAY: from src/game/gameplay/player/air.cpp (3 copies in Lilly ports)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 1 header asserts agree, 0 disagree. Size from header: 0x1B00.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x1320 | `undefined4[44]` | `unk_0x1320` | unknown (padding/placeholder) |  |
| 0x13D0 | `int` | `nState` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x13D4 | `undefined4` | `unk_0x13d4` | unknown (padding/placeholder) |  |
| 0x13D8 | `int` | `nUnk13d8` | unknown (padding/placeholder) |  |
| 0x13DC | `undefined4[3]` | `unk_0x13dc` | unknown (padding/placeholder) |  |
| 0x13E8 | `int` | `nUnk13e8` | unknown (padding/placeholder) |  |
| 0x13EC | `undefined4` | `unk_0x13ec` | unknown (padding/placeholder) |  |
| 0x13F0 | `LO *` | `ploCarried` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x13F4 | `undefined4[7]` | `unk_0x13f4` | unknown (padding/placeholder) |  |
| 0x1410 | `float` | `gUnk1410` | unknown (padding/placeholder) |  |
| 0x1414 | `float` | `gUnk1414` | unknown (padding/placeholder) |  |
| 0x1418 | `undefined4[6]` | `unk_0x1418` | unknown (padding/placeholder) |  |
| 0x1430 | `LO *` | `ploUnk1430` | unknown (padding/placeholder) |  |
| 0x1434 | `float` | `tUnk1434` | unknown (padding/placeholder) |  |
| 0x1438 | `undefined4` | `unk_0x1438` | unknown (padding/placeholder) |  |
| 0x143C | `int` | `nUnk143c` | unknown (padding/placeholder) |  |
| 0x1440 | `undefined4[3]` | `unk_0x1440` | unknown (padding/placeholder) |  |
| 0x144C | `int` | `nUnk144c` | unknown (padding/placeholder) |  |
| 0x1450 | `int` | `nUnk1450` | unknown (padding/placeholder) |  |
| 0x1454 | `undefined4` | `unk_0x1454` | unknown (padding/placeholder) |  |
| 0x1458 | `int` | `iCarry` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x145C | `undefined4` | `unk_0x145c` | unknown (padding/placeholder) |  |
| 0x1460 | `undefined4[44]` | `unk_0x1460` | unknown (padding/placeholder) |  |
| 0x1510 | `undefined4[8]` | `unk_0x1510` | unknown (padding/placeholder) |  |
| 0x1530 | `struct SMA *` | `psmaWork` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1534 | `int` | `nWork` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1538 | `undefined4[2]` | `unk_0x1538` | unknown (padding/placeholder) |  |
| 0x1540 | `undefined4[44]` | `unk_0x1540` | unknown (padding/placeholder) |  |
| 0x15F0 | `void *` | `precUnk15f0` | unknown (padding/placeholder) |  |
| 0x15F4 | `void *` | `precUnk15f4` | unknown (padding/placeholder) |  |
| 0x15F8 | `void *` | `precUnk15f8` | unknown (padding/placeholder) |  |
| 0x15FC | `void *` | `precUnk15fc` | unknown (padding/placeholder) |  |
| 0x1600 | `undefined4[4]` | `unk_0x1600` | unknown (padding/placeholder) |  |
| 0x1610 | `PARTLINK *` | `ppartlinkUnk1610` | unknown (padding/placeholder) |  |
| 0x1614 | `PARTLINK *` | `ppartlinkUnk1614` | unknown (padding/placeholder) |  |
| 0x1618 | `undefined4[6]` | `unk_0x1618` | unknown (padding/placeholder) |  |
| 0x1630 | `undefined4[44]` | `unk_0x1630` | unknown (padding/placeholder) |  |
| 0x16E0 | `undefined4[4]` | `unk_0x16e0` | unknown (padding/placeholder) |  |
| 0x16F0 | `BLDIR[8]` | `abldirUnk16f0` | unknown (padding/placeholder) |  |
| 0x17F0 | `ASEGBL *` | `pasegblUnk17f0` | unknown (padding/placeholder) |  |
| 0x17F4 | `int` | `nUnk17f4` | unknown (padding/placeholder) |  |
| 0x17F8 | `undefined4[6]` | `unk_0x17f8` | unknown (padding/placeholder) |  |
| 0x1810 | `int` | `nUnk1810` | unknown (padding/placeholder) |  |
| 0x1814 | `undefined4[7]` | `unk_0x1814` | unknown (padding/placeholder) |  |
| 0x1830 | `qword` | `qwUnk1830` | unknown (padding/placeholder) |  |
| 0x1840 | `int` | `nUnk1840` | unknown (padding/placeholder) |  |
| 0x1844 | `undefined4[3]` | `unk_0x1844` | unknown (padding/placeholder) |  |
| 0x1850 | `undefined4[16]` | `unk_0x1850` | unknown (padding/placeholder) |  |
| 0x1890 | `undefined4[31]` | `unk_0x1890` | unknown (padding/placeholder) |  |
| 0x190C | `struct ASEGA *` | `pasegaUnk190c` | unknown (padding/placeholder) |  |
| 0x1910 | `struct MOVEREC` | `moverec` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1A40 | `undefined4[6]` | `unk_0x1a40` | unknown (padding/placeholder) |  |
| 0x1A58 | `SOPART *[22]` | `aprecState` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1AB0 | `SOPART *[4]` | `aprecCarry` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1AC0 | `undefined4` | `unk_0x1ac0` | unknown (padding/placeholder) |  |
| 0x1AC4 | `int` | `nUnk1ac4` | unknown (padding/placeholder) |  |
| 0x1AC8 | `float` | `tUnk1ac8` | unknown (padding/placeholder) |  |
| 0x1ACC | `struct ACT1 *` | `pact1Unk1acc` | unknown (padding/placeholder) |  |
| 0x1AD0 | `undefined4[12]` | `unk_0x1ad0` | unknown (padding/placeholder) |  |

Inherited fields: see [STEPPLAYER](STEPPLAYER.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 1 |
| L03_nightclub_ep02 | 1 |
| L04_nightclub_ep03 | 1 |
| L05_nightclub_ep04 | 1 |
| L06_nightclub_ep05 | 1 |
| L07_nightclub_ep06 | 1 |
| L08_palace_ep00 | 1 |
| L09_palace_ep07 | 1 |
| L10_palace_ep08 | 1 |
| L11_palace_ep09 | 1 |
| L12_temple_ep00 | 1 |
| L13_temple_ep07 | 1 |
| L14_prison_ep00 | 1 |
| L15_prison_ep07 | 1 |
| L16_prison_ep10 | 1 |
| L17_castle_ep00 | 1 |
| L19_castle_ep11 | 1 |
| L20_castle_ep12 | 1 |
| L21_castle_ep13 | 1 |
| L22_castle_ep04 | 1 |
| L23_castle_ep14 | 1 |
| L24_castle_ep15 | 1 |
| L25_castle_ep16 | 1 |
| L26_castle_ep17 | 1 |
| L27_train_ep00 | 1 |
| L28_train_ep18 | 1 |
| L29_train_ep19 | 1 |
| L30_train_ep20 | 1 |
| L31_train_ep21 | 1 |
| L32_sawmill_ep00 | 1 |
| L33_sawmill_ep22 | 1 |
| L34_sawmill_ep23 | 1 |
| L35_sawmill_ep24 | 1 |
| L36_sawmill_ep25 | 1 |
| L37_sawmill_ep26 | 1 |
| L38_blimp_ep00 | 1 |
| L39_blimp_ep07 | 1 |
| L40_blimp_ep27 | 1 |
| L41_blimp_ep28 | 1 |
| L42_blimp_ep29 | 1 |
| L43_blimp_ep26 | 1 |
| world_elf2A38A8 | 1 |

## Related docs

[engine/player.md](../../engine/player.md) (7), [reference/class-tree.md](../../reference/class-tree.md) (2), [engine/guards-ai.md](../../engine/guards-ai.md) (1), [engine/minigames.md](../../engine/minigames.md) (1), [engine/overview.md](../../engine/overview.md) (1), [formats/level-objects.md](../../formats/level-objects.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
