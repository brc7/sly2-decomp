# BENTLEY (CID 55, 0x37)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> Lilly BENTLEY : STEPPLAYER ~0x1880; own code TU 0x134648 ("bentley")

| | |
|---|---|
| Name | `BENTLEY` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [BENTLEY](BENTLEY.md) → [STEPPLAYER](STEPPLAYER.md) → [STEP](STEP.md) → [PO](PO.md) → [SO](SO.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x1880 (6272 bytes), +0x560 over STEPPLAYER |
| Descriptor | `D_002BAF34` (record 55 of D_002BAAE8; cid range 55..56) |
| Factory (`pfnNew`) | [`NewBentley`](../../../src/game/app/class_registry.cpp) `0x111728` |
| Descriptor getter (slot 6) | `GetBentleyClassDesc` `0x113D80` |
| vtable | `D_002CB230`, 106 entries (slots 0..105; slot 0 null), 0 new, 22 overridden |
| vtable stored by | [`NewBentley`](../../../src/game/app/class_registry.cpp) (vptr +0xC) |
| Level instances | 42 objects in 42 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | **override** | [`HandleBentleyMessage`](../../../src/game/gameplay/player/bentley.cpp) | `0x135AD8` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetBentleyClassDesc`](../../../src/game/app/class_registry.cpp) | `0x113D80` | Pclassinfo |
| 7 | Init | **override** | [`InitBentley`](../../../src/game/gameplay/player/bentley.cpp) | `0x134648` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | inherited (SO) | `SetSoParent` | `0x1F21B8` | SetParent |
| 11 | AddHierarchy | inherited (SO) | `AddSoHierarchy` | `0x1F2C30` | AddHierarchy |
| 12 | RemoveHierarchy | inherited (STEPPLAYER) | `RemoveHierarchyStepplayer?` | `0x141928` | RemoveHierarchy |
| 13 | UpdateLoroot | inherited (SO) | `UpdateSoLoroot` | `0x1F2E78` | UpdateLoroot |
| 14 | LoadFromBrx | **override** | [`LoadBentleyFromBrx`](../../../src/game/gameplay/player/bentley.cpp) | `0x134770` | LoadFromBrx |
| 15 | PostLoad | **override** | [`PostBentleyLoad`](../../../src/game/gameplay/player/bentley.cpp) | `0x1347A8` | VUnk14 |
| 16 | Update | **override** | [`UpdateBentley`](../../../src/game/gameplay/player/bentley.cpp) | `0x134DA8` | VUnk15 |
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
| 50 | GetSoCpdefi | **override** | [`GetBentleyCpdefi`](../../../src/game/gameplay/player/bentley.cpp) | `0x1358A8` | GetCpdefi |
| 51 | SO_PxfFromId | inherited (SO) | `SO_PxfFromId?` | `0x22D778` | PxfFromIxf |
| 52 | CloneXfRef | inherited (SO) | `CloneXfRef?` | `0x1F4CA0` | CloneXfRef |
| 53 | SO_PtreeNew | inherited (SO) | `SO_PtreeNew?` | `0x1F56F8` | VUnk52 |
| 54 | SO_vt54_Nop | inherited (STEP) | `OnStepSoContactFreed` | `0x204C60` | VUnk53 |
| 55 | SO_vt55_Nop | **override** | [`BENTLEY_VUnk54?`](../../../src/game/gameplay/player/bentley.cpp) | `0x135A48` | AdjustNewXp |
| 56 | SO_vt56_Nop | inherited (SO) | `SO_vt56_Nop?` | `0x1F4FB8` | VUnk55 |
| 57 | SO_vt57_Nop | inherited (STEP) | `AdjustStepXpVelocity` | `0x2044B0` | VUnk56 |
| 58 | ScheduleSoTouchEvents | inherited (STEPPLAYER) | `STEPPLAYER_VUnk57?` | `0x140D70` | VUnk57 |
| 59 | SO_vt59_Nop | inherited (STEPPLAYER) | `STEPPLAYER_VUnk58?` | `0x140D00` | VUnk58 |
| 60 | SO_vt60_Nop | inherited (STEP) | `AddStepCustomXps` | `0x204AC8` | VUnk59 |
| 61 | FIgnoreSoIntersection | inherited (STEPPLAYER) | `FIgnoreIntersection__10STEPPLAYERP2SO` | `0x144388` | FIgnoreIntersection |
| 62 | FIsSoFixed | inherited (SO) | `FIsSoFixed?` | `0x1F5040` | VUnk61 |
| 63 | SO_vt63_Ret0 | inherited (STEP) | `FStepVt63?` | `0x204248` | CpartExtra |
| 64 | ResetSoPhys | inherited (SO) | `ResetSoPhys?` | `0x1F3590` | InitPhys |
| 65 | ApplySoForcesXf | inherited (SO) | `ApplySoForcesXf?` | `0x1F9E70` | VUnk64 |
| 66 | ProjectSoForce | inherited (SO) | `ProjectSoForce?` | `0x1FA328` | VUnk65 |
| 67 | ApplySoQueuedMotion | inherited (SO) | `ApplySoQueuedMotion?` | `0x1FA528` | VUnk66 |
| 68 | ApplySoContactImpulse | inherited (STEPPLAYER) | `STEPPLAYER_VUnk67?` | `0x140DB0` | VUnk67 |
| 69 | GetSoVelocityAtPos | inherited (SO) | `GetSoVelocityAtPos?` | `0x1FADD0` | VUnk68 |
| 70 | ApplySoPendingVelocity | **override** | [`BENTLEY_VUnk69?`](../../../src/game/gameplay/player/bentley.cpp) | `0x135940` | VUnk69 |
| 71 | AddSoExternalAccelerations | inherited (SO) | `AddSoExternalAccelerations?` | `0x2297F0` | VUnk70 |
| 72 |  | inherited (STEPPLAYER) | `ResolveStepplayerXaContact?` | `0x1A28B0` | VUnk71 |
| 73 | FPosInSo | inherited (SO) | `FPosInSo?` | `0x1F72F8` | VUnk72 |
| 74 | InflictSoContactZaps | inherited (SO) | `InflictSoContactZaps?` | `0x1F7410` | ProcessSoZapContacts |
| 75 | FZprHasField44 | inherited (STEPPLAYER) | `STEPPLAYER_FTakeDamage` | `0x140F50` | FTakeDamage |
| 76 | FInvulnerablePo | **override** | [`BENTLEY_FInvulnerable`](../../../src/game/gameplay/player/bentley.cpp) | `0x135F78` | FInvulnerable |
| 77 | JthsCurrentPo | inherited (STEPPLAYER) | `JthsCurrent__10STEPPLAYER` | `0x140F70` | JthsCurrent |
| 78 | OnPoActive | inherited (STEPPLAYER) | `OnStepplayerActive` | `0x144480` | OnActive |
| 79 | SetPoCtlState | inherited (STEPPLAYER) | `SetStepplayerCtlState` | `0x1406E8` | SetCtlState |
| 80 | FCanPoBeHealed | inherited (STEPPLAYER) | `FCanBeHealed__10STEPPLAYER` | `0x144090` | FCanBeHealed |
| 81 | AddPoHpMp | inherited (STEPPLAYER) | `AddStepplayerHpMp` | `0x144268` | AddHpMp |
| 82 | AddStepAxisXps | inherited (STEPPLAYER) | `STEPPLAYER_VUnk81?` | `0x140C30` | VUnk81 |
| 83 | AddStepDialogSmRefs | inherited (STEPPLAYER) | `STEPPLAYER_VUnk82?` | `0x1442D0` | VUnk82 |
| 84 | RemoveStepDialogSmRefs | inherited (STEPPLAYER) | `STEPPLAYER_VUnk83?` | `0x144338` | VUnk83 |
| 85 | GrfStepGroundProbe | **override** | [`BENTLEY_VUnk84?`](../../../src/game/gameplay/player/bentley.cpp) | `0x136CC0` | VUnk84 |
| 86 | FStepVt86 | inherited (STEPPLAYER) | `STEPPLAYER_VUnk85?` | `0x144418` | VUnk85 |
| 87 | StepVt87Nop | inherited (STEP) | `StepVt87Nop?` | `0x205D10` | VUnk86 |
| 88 | SetJtPlayerState (pure in STEPPLAYER) | **override** | [`SetBentleyPlayerState`](../../../src/game/gameplay/player/bentley.cpp) | `0x135C20` | VUnk87 |
| 89 | GetJtPlayerState (pure in STEPPLAYER) | **override** | [`BENTLEY_PlayerStateCur`](../../../src/game/gameplay/player/bentley.cpp) | `0x135D08` | VUnk88 |
| 90 | JT_vt90 (pure in STEPPLAYER) | **override** | [`BENTLEY_VUnk89?`](../../../src/game/gameplay/player/bentley.cpp) | `0x135D80` | VUnk89 |
| 91 | JT_vt91 (pure in STEPPLAYER) | **override** | [`BENTLEY_VUnk90?`](../../../src/game/gameplay/player/bentley.cpp) | `0x135E30` | VUnk90 |
| 92 | JT_FFaceTarget (pure in STEPPLAYER) | **override** | [`BENTLEY_VUnk91?`](../../../src/game/gameplay/player/bentley.cpp) | `0x135E38` | VUnk91 |
| 93 | OnStepplayerHeroDied | inherited (STEPPLAYER) | `OnStepplayerHeroDied` | `0x140EB0` | OnHeroDied |
| 94 | JT_vt94 (pure in STEPPLAYER) | **override** | [`BENTLEY_VUnk93?`](../../../src/game/gameplay/player/bentley.cpp) | `0x135FF0` | VUnk93 |
| 95 | SetStepplayerPlayerInteractMode | inherited (STEPPLAYER) | `SetStepplayerPlayerInteractMode` | `0x140678` | SetPlayerInteractMode |
| 96 | GetStepplayerStunHold | inherited (STEPPLAYER) | `GetStepplayerStunHold` | `0x143220` | GetStunHold |
| 97 | GetStepplayerCmFocusPos | inherited (STEPPLAYER) | `GetStepplayerCmFocusPos` | `0x141588` | GetCmFocusPos |
| 98 | STEPPLAYER_VUnk97 | **override** | [`BENTLEY_VUnk97?`](../../../src/game/gameplay/player/bentley.cpp) | `0x135E40` | VUnk97 |
| 99 | FCanUseGadget__10STEPPLAYERi | **override** | [`BENTLEY_FCanUseGadget`](../../../src/game/gameplay/player/bentley.cpp) | `0x136870` | FCanUseGadget |
| 100 | StartStepplayerGadget | **override** | [`StartBentleyGadget`](../../../src/game/gameplay/player/bentley.cpp) | `0x136A90` | StartGadget |
| 101 | UpdateStepplayerGadget | inherited (STEPPLAYER) | `UpdateStepplayerGadget` | `0x141DD0` | VSlot101 |
| 102 | StopStepplayerGadget | inherited (STEPPLAYER) | `StopStepplayerGadget` | `0x141E28` | VSlot102 |
| 103 | JT_vt103 (pure in STEPPLAYER) | **override** | [`BENTLEY_PchzSaveVarRoot`](../../../src/game/gameplay/player/bentley.cpp) | `0x136CB0` | VSlot103 |
| 104 | STEPPLAYER_VUnk104 | inherited (STEPPLAYER) | `STEPPLAYER_VUnk104?` | `0x141E30` | VSlot104 |
| 105 | OnStepplayerGadgetsChanged | **override** | [`OnBentleyGadgetsChanged`](../../../src/game/gameplay/player/bentley.cpp) | `0x136C50` | VSlot105 |

## Fields

From `include/sly2/players.h` (`struct BENTLEY : STEPPLAYER`). BENTLEY: from src/game/gameplay/cutscene/relay/func_001E1E30.cpp (2 copies in Lilly ports)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 1 header asserts agree, 0 disagree. Size from header: 0x1880.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x1320 | `undefined4[44]` | `unk_0x1320` | unknown (padding/placeholder) |  |
| 0x13D0 | `int` | `nState` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x13D4 | `undefined4[3]` | `unk_0x13d4` | unknown (padding/placeholder) |  |
| 0x13E0 | `BLDIR[8]` | `abldirA` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x14E0 | `ASEGBL *` | `pasegblA` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x14E4 | `undefined4[3]` | `unk_0x14e4` | unknown (padding/placeholder) |  |
| 0x14F0 | `BLDIR[8]` | `abldirB` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x15F0 | `ASEGBL *` | `pasegblB` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x15F4 | `undefined4[3]` | `unk_0x15f4` | unknown (padding/placeholder) |  |
| 0x1600 | `undefined4[44]` | `unk_0x1600` | unknown (padding/placeholder) |  |
| 0x16B0 | `int` | `nUnk16b0` | unknown (padding/placeholder) |  |
| 0x16B4 | `JTPOCKET *` | `ppocketCur` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x16B8 | `undefined4` | `unk_0x16b8` | unknown (padding/placeholder) |  |
| 0x16BC | `int` | `nUnk16bc` | unknown (padding/placeholder) |  |
| 0x16C0 | `undefined4[20]` | `unk_0x16c0` | unknown (padding/placeholder) |  |
| 0x1710 | `struct BTBLENDS` | `btblends` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x17F0 | `undefined4[4]` | `unk_0x17f0` | unknown (padding/placeholder) |  |
| 0x1800 | `int` | `nUnk1800` | unknown (padding/placeholder) |  |
| 0x1804 | `undefined4[5]` | `unk_0x1804` | unknown (padding/placeholder) |  |
| 0x1818 | `union BENTLEY::(anonymous at /tmp/gencls_g2fnul5h/sly2/players.h:135:5) ` | `union BENTLEY::(anonymous at /tmp/gencls_g2fnul5h/sly2/players.h:135:5) ` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1820 | `undefined4` | `unk_0x1820` | unknown (padding/placeholder) |  |
| 0x1824 | `JTPOCKET[6]` | `apocket` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x186C | `undefined4` | `unk_0x186c` | unknown (padding/placeholder) |  |
| 0x1870 | `int` | `nUnk1870` | unknown (padding/placeholder) |  |
| 0x1874 | `int` | `nUnk1874` | unknown (padding/placeholder) |  |
| 0x1878 | `undefined4[2]` | `unk_0x1878` | unknown (padding/placeholder) |  |

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

[engine/player.md](../../engine/player.md) (7), [engine/guards-ai.md](../../engine/guards-ai.md) (1), [engine/overview.md](../../engine/overview.md) (1), [formats/level-objects.md](../../formats/level-objects.md) (1), [reference/class-tree.md](../../reference/class-tree.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
