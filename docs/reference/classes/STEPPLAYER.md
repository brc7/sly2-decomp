# STEPPLAYER (CID 52, 0x34)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> Lilly STEPPLAYER : STEP ends 0x1320; abstract (pure virtuals); parent of JT/MURRAY/BENTLEY

| | |
|---|---|
| Name | `STEPPLAYER` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [STEPPLAYER](STEPPLAYER.md) → [STEP](STEP.md) → [PO](PO.md) → [SO](SO.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | [JT](JT.md), [MURRAY](MURRAY.md), [BENTLEY](BENTLEY.md) |
| Instance size (`cb`) | 0x1320 (4896 bytes), +0x1060 over STEP |
| Descriptor | `D_002BAEF8` (record 52 of D_002BAAE8; cid range 52..56) |
| Factory (`pfnNew`) | (none: not created by PloNew) |
| Descriptor getter (slot 6) | `GetStepplayerClassDesc` `0x113D50` |
| vtable | `D_002CBE68`, 106 entries (slots 0..105; slot 0 null), 18 new, 26 overridden |
| Level instances | none (not a world-file object class) |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | **override** | [`HandleStepplayerMessage`](../../../src/game/gameplay/player/stepplayer/HandleStepplayerMessage.c) | `0x140F90` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetStepplayerClassDesc`](../../../src/game/app/class_registry/GetStepplayerClassDesc.c) | `0x113D50` | Pclassinfo |
| 7 | Init | **override** | [`InitStepplayer`](../../../src/game/gameplay/player/stepplayer/InitStepplayer.c) | `0x13FF70` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | inherited (SO) | `SetSoParent` | `0x1F21B8` | SetParent |
| 11 | AddHierarchy | inherited (SO) | `AddSoHierarchy` | `0x1F2C30` | AddHierarchy |
| 12 | RemoveHierarchy | **override** | [`RemoveHierarchyStepplayer?`](../../../src/game/gameplay/player/stepplayer/func_00141928.c) | `0x141928` | RemoveHierarchy |
| 13 | UpdateLoroot | inherited (SO) | `UpdateSoLoroot` | `0x1F2E78` | UpdateLoroot |
| 14 | LoadFromBrx | inherited (STEP) | `LoadStepFromBrx` | `0x2039C0` | LoadFromBrx |
| 15 | PostLoad | **override** | [`PostLoadStepplayer`](../../../src/game/gameplay/player/stepplayer/PostLoadStepplayer.c) | `0x1400B8` | VUnk14 |
| 16 | Update | **override** | [`UpdateStepplayer`](../../../src/game/gameplay/player/stepplayer/UpdateStepplayer.c) | `0x1403D0` | VUnk15 |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (SO) | `UpdateSoXfWorld?` | `0x1F3980` | VUnk16 |
| 18 | UpdateLoXfWorldHierarchy | inherited (SO) | `UpdateSoXfWorldHierarchy` | `0x22FBE0` | UpdateXfWorldHierarchy |
| 19 | RenderSelf | **override** | [`RenderStepplayerSelf`](../../../src/game/gameplay/player/stepplayer/RenderStepplayerSelf.c) | `0x1425B0` | RenderSelf |
| 20 | RenderLoAll | inherited (SO) | `RenderSoAll?` | `0x1F3CC8` | VUnk19 |
| 21 | SetLoFlag8 | inherited (SO) | `SetSoFlag8?` | `0x1F3B28` | VUnk20 |
| 22 | OnRemove | inherited (SO) | `OnSoRemove` | `0x1F3E00` | Freeze |
| 23 | OnAdd | inherited (SO) | `OnSoAdd` | `0x1F3EA8` | Unfreeze |
| 24 |  | inherited (LO) | `func_00210330` | `0x210330` | VUnk23 |
| 25 | SetIdProxy | inherited (SO) | `SetSoIdProxy` | `0x1CA308` | SetIdProxy |
| 26 | ResetLoForProxy | inherited (SO) | `ResetSoForProxy?` | `0x1CB518` | VUnk25 |
| 27 | LO_VUnk27Proxy | inherited (SO) | `SO_VUnk27Proxy?` | `0x1CB5D0` | VUnk26 |
| 28 | CploChildren | inherited (LO) | `LO_CploChildren` | `0x1A5BE8` | CploChildren |
| 29 | TranslateToPos | **override** | [`TranslateStepplayerToPos`](../../../src/game/gameplay/player/stepplayer/TranslateStepplayerToPos.c) | `0x143CF8` | TranslateToPos |
| 30 | RotateToMat | inherited (STEP) | `RotateStepToMat` | `0x2041F8` | RotateToMat |
| 31 | PmatWorld | inherited (SO) | `SO_PmatWorld` | `0x1F3FA0` | PmatWorld |
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
| 50 | GetSoCpdefi | **override** | [`GetStepplayerCpdefi`](../../../src/game/gameplay/player/stepplayer/GetStepplayerCpdefi.c) | `0x141640` | GetCpdefi |
| 51 | SO_PxfFromId | inherited (SO) | `SO_PxfFromId?` | `0x22D778` | PxfFromIxf |
| 52 | CloneXfRef | inherited (SO) | `CloneXfRef?` | `0x1F4CA0` | CloneXfRef |
| 53 | SO_PtreeNew | inherited (SO) | `SO_PtreeNew?` | `0x1F56F8` | VUnk52 |
| 54 | SO_vt54_Nop | inherited (STEP) | `OnStepSoContactFreed` | `0x204C60` | VUnk53 |
| 55 | SO_vt55_Nop | inherited (STEP) | `AdjustStepNewXp` | `0x204250` | AdjustNewXp |
| 56 | SO_vt56_Nop | inherited (SO) | `SO_vt56_Nop?` | `0x1F4FB8` | VUnk55 |
| 57 | SO_vt57_Nop | inherited (STEP) | `AdjustStepXpVelocity` | `0x2044B0` | VUnk56 |
| 58 | ScheduleSoTouchEvents | **override** | [`STEPPLAYER_VUnk57?`](../../../src/game/gameplay/player/stepplayer/func_00140D70.c) | `0x140D70` | VUnk57 |
| 59 | SO_vt59_Nop | **override** | [`STEPPLAYER_VUnk58?`](../../../src/game/gameplay/player/stepplayer/func_00140D00.c) | `0x140D00` | VUnk58 |
| 60 | SO_vt60_Nop | inherited (STEP) | `AddStepCustomXps` | `0x204AC8` | VUnk59 |
| 61 | FIgnoreSoIntersection | **override** | [`FIgnoreIntersection__10STEPPLAYERP2SO`](../../../src/game/gameplay/player/stepplayer/FIgnoreIntersection__10STEPPLAYERP2SO.c) | `0x144388` | FIgnoreIntersection |
| 62 | FIsSoFixed | inherited (SO) | `FIsSoFixed?` | `0x1F5040` | VUnk61 |
| 63 | SO_vt63_Ret0 | inherited (STEP) | `FStepVt63?` | `0x204248` | CpartExtra |
| 64 | ResetSoPhys | inherited (SO) | `ResetSoPhys?` | `0x1F3590` | InitPhys |
| 65 | ApplySoForcesXf | inherited (SO) | `ApplySoForcesXf?` | `0x1F9E70` | VUnk64 |
| 66 | ProjectSoForce | inherited (SO) | `ProjectSoForce?` | `0x1FA328` | VUnk65 |
| 67 | ApplySoQueuedMotion | inherited (SO) | `ApplySoQueuedMotion?` | `0x1FA528` | VUnk66 |
| 68 | ApplySoContactImpulse | **override** | [`STEPPLAYER_VUnk67?`](../../../src/game/gameplay/player/stepplayer/func_00140DB0.c) | `0x140DB0` | VUnk67 |
| 69 | GetSoVelocityAtPos | inherited (SO) | `GetSoVelocityAtPos?` | `0x1FADD0` | VUnk68 |
| 70 | ApplySoPendingVelocity | inherited (STEP) | `PresetStepAccel` | `0x204108` | VUnk69 |
| 71 | AddSoExternalAccelerations | inherited (SO) | `AddSoExternalAccelerations?` | `0x2297F0` | VUnk70 |
| 72 |  | **override** | [`ResolveStepplayerXaContact?`](../../../src/game/gameplay/player/hurt.cpp) | `0x1A28B0` | VUnk71 |
| 73 | FPosInSo | inherited (SO) | `FPosInSo?` | `0x1F72F8` | VUnk72 |
| 74 | InflictSoContactZaps | inherited (SO) | `InflictSoContactZaps?` | `0x1F7410` | ProcessSoZapContacts |
| 75 | FZprHasField44 | **override** | [`STEPPLAYER_FTakeDamage`](../../../src/game/gameplay/player/stepplayer/STEPPLAYER_FTakeDamage.c) | `0x140F50` | FTakeDamage |
| 76 | FInvulnerablePo | **override** | [`STEPPLAYER_FInvulnerable`](../../../src/game/gameplay/player/stepplayer/STEPPLAYER_FInvulnerable.c) | `0x140ED8` | FInvulnerable |
| 77 | JthsCurrentPo | **override** | [`JthsCurrent__10STEPPLAYER`](../../../src/game/gameplay/player/stepplayer/JthsCurrent__10STEPPLAYER.c) | `0x140F70` | JthsCurrent |
| 78 | OnPoActive | **override** | [`OnStepplayerActive`](../../../src/game/gameplay/player/stepplayer/OnStepplayerActive.c) | `0x144480` | OnActive |
| 79 | SetPoCtlState | **override** | [`SetStepplayerCtlState`](../../../src/game/gameplay/player/stepplayer/SetStepplayerCtlState.c) | `0x1406E8` | SetCtlState |
| 80 | FCanPoBeHealed | **override** | [`FCanBeHealed__10STEPPLAYER`](../../../src/game/gameplay/player/stepplayer/FCanBeHealed__10STEPPLAYER.c) | `0x144090` | FCanBeHealed |
| 81 | AddPoHpMp | **override** | [`AddStepplayerHpMp`](../../../src/game/gameplay/player/stepplayer/AddStepplayerHpMp.c) | `0x144268` | AddHpMp |
| 82 | AddStepAxisXps | **override** | [`STEPPLAYER_VUnk81?`](../../../src/game/gameplay/player/stepplayer/func_00140C30.c) | `0x140C30` | VUnk81 |
| 83 | AddStepDialogSmRefs | **override** | [`STEPPLAYER_VUnk82?`](../../../src/game/gameplay/player/stepplayer/func_001442D0.c) | `0x1442D0` | VUnk82 |
| 84 | RemoveStepDialogSmRefs | **override** | [`STEPPLAYER_VUnk83?`](../../../src/game/gameplay/player/stepplayer/func_00144338.c) | `0x144338` | VUnk83 |
| 85 | GrfStepGroundProbe | **override** | [`STEPPLAYER_VUnk84?`](../../../src/game/gameplay/player/stepplayer/func_00143C78.c) | `0x143C78` | VUnk84 |
| 86 | FStepVt86 | **override** | [`STEPPLAYER_VUnk85?`](../../../src/game/gameplay/player/stepplayer/func_00144418.c) | `0x144418` | VUnk85 |
| 87 | StepVt87Nop | inherited (STEP) | `StepVt87Nop?` | `0x205D10` | VUnk86 |
| 88 | SetJtPlayerState (pure in STEPPLAYER) | **new** | [`__pure_virtual`](../../../src/sdk/libgcc/__pure_virtual.c) | `0x23C820` | VUnk87 |
| 89 | GetJtPlayerState (pure in STEPPLAYER) | **new** | [`__pure_virtual`](../../../src/sdk/libgcc/__pure_virtual.c) | `0x23C820` | VUnk88 |
| 90 | JT_vt90 (pure in STEPPLAYER) | **new** | [`__pure_virtual`](../../../src/sdk/libgcc/__pure_virtual.c) | `0x23C820` | VUnk89 |
| 91 | JT_vt91 (pure in STEPPLAYER) | **new** | [`__pure_virtual`](../../../src/sdk/libgcc/__pure_virtual.c) | `0x23C820` | VUnk90 |
| 92 | JT_FFaceTarget (pure in STEPPLAYER) | **new** | [`__pure_virtual`](../../../src/sdk/libgcc/__pure_virtual.c) | `0x23C820` | VUnk91 |
| 93 | OnStepplayerHeroDied | **new** | [`OnStepplayerHeroDied`](../../../src/game/gameplay/player/stepplayer/OnStepplayerHeroDied.c) | `0x140EB0` | OnHeroDied |
| 94 | JT_vt94 (pure in STEPPLAYER) | **new** | [`__pure_virtual`](../../../src/sdk/libgcc/__pure_virtual.c) | `0x23C820` | VUnk93 |
| 95 | SetStepplayerPlayerInteractMode | **new** | [`SetStepplayerPlayerInteractMode`](../../../src/game/gameplay/player/stepplayer/SetStepplayerPlayerInteractMode.c) | `0x140678` | SetPlayerInteractMode |
| 96 | GetStepplayerStunHold | **new** | [`GetStepplayerStunHold`](../../../src/game/gameplay/player/stepplayer/GetStepplayerStunHold.c) | `0x143220` | GetStunHold |
| 97 | GetStepplayerCmFocusPos | **new** | [`GetStepplayerCmFocusPos`](../../../src/game/gameplay/player/stepplayer/GetStepplayerCmFocusPos.c) | `0x141588` | GetCmFocusPos |
| 98 | STEPPLAYER_VUnk97 | **new** | [`STEPPLAYER_VUnk97?`](../../../src/game/gameplay/player/stepplayer/func_001409B8.c) | `0x1409B8` | VUnk97 |
| 99 | FCanUseGadget__10STEPPLAYERi | **new** | [`FCanUseGadget__10STEPPLAYERi`](../../../src/game/gameplay/player/stepplayer/FCanUseGadget__10STEPPLAYERi.c) | `0x141C00` | FCanUseGadget |
| 100 | StartStepplayerGadget | **new** | [`StartStepplayerGadget`](../../../src/game/gameplay/player/stepplayer/StartStepplayerGadget.c) | `0x141D38` | StartGadget |
| 101 | UpdateStepplayerGadget | **new** | [`UpdateStepplayerGadget`](../../../src/game/gameplay/player/stepplayer/UpdateStepplayerGadget.c) | `0x141DD0` | VSlot101 |
| 102 | StopStepplayerGadget | **new** | [`StopStepplayerGadget`](../../../src/game/gameplay/player/stepplayer/StopStepplayerGadget.c) | `0x141E28` | VSlot102 |
| 103 | JT_vt103 (pure in STEPPLAYER) | **new** | [`__pure_virtual`](../../../src/sdk/libgcc/__pure_virtual.c) | `0x23C820` | VSlot103 |
| 104 | STEPPLAYER_VUnk104 | **new** | [`STEPPLAYER_VUnk104?`](../../../src/game/gameplay/player/stepplayer/func_00141E30.c) | `0x141E30` | VSlot104 |
| 105 | OnStepplayerGadgetsChanged | **new** | [`OnStepplayerGadgetsChanged?`](../../../src/game/gameplay/player/stepplayer/func_00141E70.c) | `0x141E70` | VSlot105 |

## Fields

From `include/sly2/stepplayer.h` (`struct STEPPLAYER : STEP`). STEPPLAYER: from src/game/gameplay/player/air/UpdateJtAirInput.cpp (15 copies in Lilly ports)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 5 header asserts agree, 0 disagree. Size from header: 0x1320.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x2C0 | `struct JTAIR` | `jtair` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4C0 | `struct JTGRAB` | `jtgrab` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x9E0 | `struct JTPLACES` | `jtplaces` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xDF0 | `struct JTHURT` | `jthurt` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xF00 | `struct JTMODE` | `jtmode` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xF50 | `struct JTPIPE` | `jtpipe` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xFD0 | `struct JTGOAL` | `jtgoal` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1000 | `uint` | `grfUnk1000` | unknown (padding/placeholder) |  |
| 0x1004 | `undefined4[3]` | `unk_0x1004` | unknown (padding/placeholder) |  |
| 0x1010 | `qword` | `dposUnk1010` | unknown (padding/placeholder) |  |
| 0x1020 | `float` | `gUnk1020` | unknown (padding/placeholder) |  |
| 0x1024 | `struct TN *` | `ptnUnk1024` | unknown (padding/placeholder) |  |
| 0x1028 | `struct SM *` | `psmUnk1028` | unknown (padding/placeholder) |  |
| 0x102C | `struct SM *` | `psmUnk102c` | unknown (padding/placeholder) |  |
| 0x1030 | `struct ASEGA *` | `pasegaUnk1030` | unknown (padding/placeholder) |  |
| 0x1034 | `struct SMA *` | `psmaUnk1034` | unknown (padding/placeholder) |  |
| 0x1038 | `int` | `nUnk1038` | unknown (padding/placeholder) |  |
| 0x103C | `float` | `gUnk103c` | unknown (padding/placeholder) |  |
| 0x1040 | `int` | `nUnk1040` | unknown (padding/placeholder) |  |
| 0x1044 | `int` | `fUnk1044` | unknown (padding/placeholder) |  |
| 0x1048 | `int` | `nUnk1048` | unknown (padding/placeholder) |  |
| 0x104C | `float` | `gUnk104c` | unknown (padding/placeholder) |  |
| 0x1050 | `float` | `gUnk1050` | unknown (padding/placeholder) |  |
| 0x1054 | `undefined4[3]` | `unk_0x1054` | unknown (padding/placeholder) |  |
| 0x1060 | `qword` | `qwUnk1060` | unknown (padding/placeholder) |  |
| 0x1070 | `qword` | `qwUnk1070` | unknown (padding/placeholder) |  |
| 0x1080 | `undefined4` | `unk_0x1080` | unknown (padding/placeholder) |  |
| 0x1084 | `struct JSG *` | `pjsgScript` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1088 | `void *` | `pvUnk1088` | unknown (padding/placeholder) |  |
| 0x108C | `int` | `nUnk108c` | unknown (padding/placeholder) |  |
| 0x1090 | `LOREF *` | `aplorefUnk1090` | unknown (padding/placeholder) |  |
| 0x1094 | `int` | `cplorefUnk1094` | unknown (padding/placeholder) |  |
| 0x1098 | `float[4]` | `agUnk1098` | unknown (padding/placeholder) |  |
| 0x10A8 | `struct SMPA` | `smpaUnk10a8` | unknown (padding/placeholder) |  |
| 0x10B8 | `void *` | `precUnk10b8` | unknown (padding/placeholder) |  |
| 0x10BC | `undefined4[3]` | `unk_0x10bc` | unknown (padding/placeholder) |  |
| 0x10C8 | `int` | `nUnk10c8` | unknown (padding/placeholder) |  |
| 0x10CC | `undefined4[2]` | `unk_0x10cc` | unknown (padding/placeholder) |  |
| 0x10D4 | `float` | `gUnk10d4` | unknown (padding/placeholder) |  |
| 0x10D8 | `float` | `gUnk10d8` | unknown (padding/placeholder) |  |
| 0x10DC | `LO *` | `ploInteract` | asserted (CONFIRMED runtime) | +0x10DC [R] interaction target (guard to pickpocket, etc.; GuardEvalInteractMode); was ploUnk10dc |
| 0x10E0 | `int` | `grfUnk10e0` | unknown (padding/placeholder) |  |
| 0x10E4 | `int` | `nUnk10e4` | unknown (padding/placeholder) |  |
| 0x10E8 | `LO *` | `ploFightTarget` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x10EC | `int` | `nInteractMode` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x10F0 | `int` | `nUnk10f0` | unknown (padding/placeholder) |  |
| 0x10F4 | `undefined4` | `unk_0x10f4` | unknown (padding/placeholder) |  |
| 0x10F8 | `LO *` | `ploLean` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x10FC | `int` | `nLeanKind` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1100 | `undefined4[8]` | `unk_0x1100` | unknown (padding/placeholder) |  |
| 0x1120 | `RAW8[3]` | `araw1120` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1138 | `float[4]` | `agdtIdle` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1148 | `int` | `nIdleKind` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x114C | `float` | `tIdle` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1150 | `undefined4[12]` | `unk_0x1150` | unknown (padding/placeholder) |  |
| 0x1180 | `GADGETSLOT[3]` | `agadgetslot` | asserted (CONFIRMED runtime) | +0x1180 [R] the 3 equipped gadgets {idGadget, nState, tState} (L1/R1/R2 slots) |
| 0x11A4 | `undefined4[3]` | `unk_0x11a4` | unknown (padding/placeholder) |  |
| 0x11B0 | `struct JTTHROW` | `jtthrow` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x12F0 | `int` | `nUnk12f0` | unknown (padding/placeholder) |  |
| 0x12F4 | `int` | `cUnk12f4` | unknown (padding/placeholder) |  |
| 0x12F8 | `struct EMITTER *` | `pemitterGrab` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x12FC | `int *` | `pmpCur` | asserted (CONFIRMED runtime) | +0x12FC [R] -> save var <char>.mpCur (gadget power) |
| 0x1300 | `int` | `mpMax` | header (Lilly-ported; offset computed, unasserted) | +0x1300 [R] max gadget power |
| 0x1304 | `float` | `tGadgetUsed` | header (Lilly-ported; offset computed, unasserted) | +0x1304 [R] clock.t of the last gadget use |
| 0x1308 | `float` | `gMpFrac` | header (Lilly-ported; offset computed, unasserted) | +0x1308 [R] fractional mp drain accumulator |
| 0x130C | `int` | `nUnk130c` | unknown (padding/placeholder) |  |
| 0x1310 | `struct ASEGA *` | `pasegaGadgetOverlay` | header (Lilly-ported; offset computed, unasserted) | +0x1310 [R] ASEGA of the gadget overlay animation; was pasegaUnk1310 |
| 0x1314 | `int` | `idGadgetOverlay` | asserted (CONFIRMED runtime) | +0x1314 [R] gadget id of the overlay animation; was nUnk1314 |
| 0x1318 | `int` | `nUnk1318` | unknown (padding/placeholder) |  |
| 0x131C | `float` | `tUnk131c` | unknown (padding/placeholder) |  |

Inherited fields: see [STEP](STEP.md).

## Related docs

[engine/player.md](../../engine/player.md) (19), [reference/class-tree.md](../../reference/class-tree.md) (6), [engine/camera.md](../../engine/camera.md) (2), [engine/overview.md](../../engine/overview.md) (2), [engine/splice-scripting.md](../../engine/splice-scripting.md) (2), [README.md](../../README.md) (1), [engine/animation.md](../../engine/animation.md) (1), [engine/game-progression.md](../../engine/game-progression.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
