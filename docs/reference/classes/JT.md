# JT (CID 53, 0x35)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> FCanUseGadget__2JT in slot 99; Lilly JT : STEPPLAYER ends 0x2060

| | |
|---|---|
| Name | `JT` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [JT](JT.md) → [STEPPLAYER](STEPPLAYER.md) → [STEP](STEP.md) → [PO](PO.md) → [SO](SO.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x2060 (8288 bytes), +0xD40 over STEPPLAYER |
| Descriptor | `D_002BAF0C` (record 53 of D_002BAAE8; cid range 53..54) |
| Factory (`pfnNew`) | [`NewJt`](../../../src/game/app/class_registry.cpp) `0x1116D8` |
| Descriptor getter (slot 6) | `GetJtClassDesc` `0x113D60` |
| vtable | `D_002D2E98`, 108 entries (slots 0..107; slot 0 null), 2 new, 31 overridden |
| vtable stored by | [`JT_ctor?`](../../../src/game/app/class_registry.cpp) (vptr +0xC) |
| Level instances | 42 objects in 42 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | **override** | [`HandleJtMessage`](../../../src/game/gameplay/player/jt.cpp) | `0x188F20` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetJtClassDesc`](../../../src/game/app/class_registry.cpp) | `0x113D60` | Pclassinfo |
| 7 | Init | **override** | [`InitJt`](../../../src/game/gameplay/player/jt.cpp) | `0x188488` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | inherited (SO) | `SetSoParent` | `0x1F21B8` | SetParent |
| 11 | AddHierarchy | inherited (SO) | `AddSoHierarchy` | `0x1F2C30` | AddHierarchy |
| 12 | RemoveHierarchy | inherited (STEPPLAYER) | `RemoveHierarchyStepplayer?` | `0x141928` | RemoveHierarchy |
| 13 | UpdateLoroot | inherited (SO) | `UpdateSoLoroot` | `0x1F2E78` | UpdateLoroot |
| 14 | LoadFromBrx | **override** | [`LoadJtFromBrx`](../../../src/game/gameplay/player/jt.cpp) | `0x1885B8` | LoadFromBrx |
| 15 | PostLoad | **override** | [`PostJtLoad`](../../../src/game/gameplay/player/jt.cpp) | `0x188608` | VUnk14 |
| 16 | Update | **override** | [`UpdateJt`](../../../src/game/gameplay/player/jt.cpp) | `0x18BCE0` | VUnk15 |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | **override** | [`JT_vt17?`](../../../src/game/gameplay/player/jt.cpp) | `0x18A278` | VUnk16 |
| 18 | UpdateLoXfWorldHierarchy | inherited (SO) | `UpdateSoXfWorldHierarchy` | `0x22FBE0` | UpdateXfWorldHierarchy |
| 19 | RenderSelf | **override** | [`RenderJtSelf`](../../../src/game/gameplay/player/jt.cpp) | `0x18A238` | RenderSelf |
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
| 49 | UpdateSoTreeBounds | **override** | [`JT_vt49?`](../../../src/game/gameplay/player/jt.cpp) | `0x18A2E0` | VUnk48 |
| 50 | GetSoCpdefi | **override** | [`JT_vt50?`](../../../src/game/gameplay/player/jt.cpp) | `0x18A428` | GetCpdefi |
| 51 | SO_PxfFromId | inherited (SO) | `SO_PxfFromId?` | `0x22D778` | PxfFromIxf |
| 52 | CloneXfRef | inherited (SO) | `CloneXfRef?` | `0x1F4CA0` | CloneXfRef |
| 53 | SO_PtreeNew | inherited (SO) | `SO_PtreeNew?` | `0x1F56F8` | VUnk52 |
| 54 | SO_vt54_Nop | inherited (STEP) | `OnStepSoContactFreed` | `0x204C60` | VUnk53 |
| 55 | SO_vt55_Nop | **override** | [`AdjustJtXp?`](../../../src/game/gameplay/player/jt.cpp) | `0x188CC8` | AdjustNewXp |
| 56 | SO_vt56_Nop | inherited (SO) | `SO_vt56_Nop?` | `0x1F4FB8` | VUnk55 |
| 57 | SO_vt57_Nop | **override** | [`AdjustJtXpVelocity?`](../../../src/game/gameplay/player/jt.cpp) | `0x188E78` | VUnk56 |
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
| 70 | ApplySoPendingVelocity | **override** | [`PresetJtAccel?`](../../../src/game/gameplay/player/jt.cpp) | `0x188960` | VUnk69 |
| 71 | AddSoExternalAccelerations | inherited (SO) | `AddSoExternalAccelerations?` | `0x2297F0` | VUnk70 |
| 72 |  | inherited (STEPPLAYER) | `ResolveStepplayerXaContact?` | `0x1A28B0` | VUnk71 |
| 73 | FPosInSo | inherited (SO) | `FPosInSo?` | `0x1F72F8` | VUnk72 |
| 74 | InflictSoContactZaps | inherited (SO) | `InflictSoContactZaps?` | `0x1F7410` | ProcessSoZapContacts |
| 75 | FZprHasField44 | inherited (STEPPLAYER) | `STEPPLAYER_FTakeDamage` | `0x140F50` | FTakeDamage |
| 76 | FInvulnerablePo | **override** | [`JT_FIgnoreZap?`](../../../src/game/gameplay/player/hurt.cpp) | `0x1A21A0` | FInvulnerable |
| 77 | JthsCurrentPo | inherited (STEPPLAYER) | `JthsCurrent__10STEPPLAYER` | `0x140F70` | JthsCurrent |
| 78 | OnPoActive | **override** | [`OnJtActive?`](../../../src/game/gameplay/player/jt.cpp) | `0x18E3A0` | OnActive |
| 79 | SetPoCtlState | inherited (STEPPLAYER) | `SetStepplayerCtlState` | `0x1406E8` | SetCtlState |
| 80 | FCanPoBeHealed | inherited (STEPPLAYER) | `FCanBeHealed__10STEPPLAYER` | `0x144090` | FCanBeHealed |
| 81 | AddPoHpMp | inherited (STEPPLAYER) | `AddStepplayerHpMp` | `0x144268` | AddHpMp |
| 82 | AddStepAxisXps | inherited (STEPPLAYER) | `STEPPLAYER_VUnk81?` | `0x140C30` | VUnk81 |
| 83 | AddStepDialogSmRefs | inherited (STEPPLAYER) | `STEPPLAYER_VUnk82?` | `0x1442D0` | VUnk82 |
| 84 | RemoveStepDialogSmRefs | inherited (STEPPLAYER) | `STEPPLAYER_VUnk83?` | `0x144338` | VUnk83 |
| 85 | GrfStepGroundProbe | **override** | [`JT_vt85?`](../../../src/game/gameplay/player/jt.cpp) | `0x18DEE0` | VUnk84 |
| 86 | FStepVt86 | inherited (STEPPLAYER) | `STEPPLAYER_VUnk85?` | `0x144418` | VUnk85 |
| 87 | StepVt87Nop | **override** | [`SetJtPlayable?`](../../../src/game/gameplay/player/jt.cpp) | `0x189530` | VUnk86 |
| 88 | SetJtPlayerState (pure in STEPPLAYER) | **override** | [`SetJtPlayerState`](../../../src/game/gameplay/player/jt.cpp) | `0x18D568` | VUnk87 |
| 89 | GetJtPlayerState (pure in STEPPLAYER) | **override** | [`GetJtPlayerState`](../../../src/game/gameplay/player/jt.cpp) | `0x18D620` | VUnk88 |
| 90 | JT_vt90 (pure in STEPPLAYER) | **override** | [`JT_vt90?`](../../../src/game/gameplay/player/jt.cpp) | `0x18C060` | VUnk89 |
| 91 | JT_vt91 (pure in STEPPLAYER) | **override** | [`JT_vt91?`](../../../src/game/gameplay/player/jt.cpp) | `0x18D548` | VUnk90 |
| 92 | JT_FFaceTarget (pure in STEPPLAYER) | **override** | [`JT_FFaceTarget`](../../../src/game/gameplay/player/jt.cpp) | `0x189458` | VUnk91 |
| 93 | OnStepplayerHeroDied | **override** | [`OnJtDeath?`](../../../src/game/gameplay/player/hurt.cpp) | `0x1A2388` | OnHeroDied |
| 94 | JT_vt94 (pure in STEPPLAYER) | **override** | [`JT_vt94?`](../../../src/game/gameplay/player/jt.cpp) | `0x1894F8` | VUnk93 |
| 95 | SetStepplayerPlayerInteractMode | inherited (STEPPLAYER) | `SetStepplayerPlayerInteractMode` | `0x140678` | SetPlayerInteractMode |
| 96 | GetStepplayerStunHold | **override** | [`JT_vt96?`](../../../src/game/gameplay/player/jt.cpp) | `0x18DE80` | GetStunHold |
| 97 | GetStepplayerCmFocusPos | **override** | [`GetJtCameraTarget?`](../../../src/game/gameplay/player/jt.cpp) | `0x18A350` | GetCmFocusPos |
| 98 | STEPPLAYER_VUnk97 | **override** | [`JT_vt98?`](../../../src/game/gameplay/player/jt.cpp) | `0x189488` | VUnk97 |
| 99 | FCanUseGadget__10STEPPLAYERi | **override** | [`FCanUseGadget__2JTi`](../../../src/game/gameplay/player/jt.cpp) | `0x18D760` | FCanUseGadget |
| 100 | StartStepplayerGadget | **override** | [`UseJtGadget`](../../../src/game/gameplay/player/jt.cpp) | `0x18DA48` | StartGadget |
| 101 | UpdateStepplayerGadget | inherited (STEPPLAYER) | `UpdateStepplayerGadget` | `0x141DD0` | VSlot101 |
| 102 | StopStepplayerGadget | **override** | [`EndJtGadget?`](../../../src/game/gameplay/player/jt.cpp) | `0x18DD78` | VSlot102 |
| 103 | JT_vt103 (pure in STEPPLAYER) | **override** | [`JT_vt103?`](../../../src/game/gameplay/player/jt.cpp) | `0x18DE08` | VSlot103 |
| 104 | STEPPLAYER_VUnk104 | inherited (STEPPLAYER) | `STEPPLAYER_VUnk104?` | `0x141E30` | VSlot104 |
| 105 | OnStepplayerGadgetsChanged | inherited (STEPPLAYER) | `OnStepplayerGadgetsChanged?` | `0x141E70` | VSlot105 |
| 106 | JT_vt106 | **new** | [`JT_vt106?`](../../../src/game/gameplay/player/jt.cpp) | `0x18D500` | VSlot106 |
| 107 | AddJtExternalAccelerations | **new** | [`AddJtExternalAccelerations`](../../../src/game/gameplay/player/hang.cpp) | `0x1904E0` | VSlot107 |

## Fields

From `include/sly2/jt.h` (`struct JT : STEPPLAYER`). JT: from src/game/gameplay/player/jt.cpp (10 copies in Lilly ports)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 7 header asserts agree, 0 disagree. Size from header: 0x2060.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x1320 | `undefined4[44]` | `unk_0x1320` | unknown (padding/placeholder) |  |
| 0x13D0 | `undefined4[64]` | `unk_0x13d0` | unknown (padding/placeholder) |  |
| 0x14D0 | `ASEGBL *` | `pasegblUnk14d0` | unknown (padding/placeholder) |  |
| 0x14D4 | `ASEG *` | `pasegUnk14d4` | unknown (padding/placeholder) |  |
| 0x14D8 | `ASEG *` | `pasegUnk14d8` | unknown (padding/placeholder) |  |
| 0x14DC | `undefined4[14]` | `unk_0x14dc` | unknown (padding/placeholder) |  |
| 0x1514 | `int` | `nUnk1514` | unknown (padding/placeholder) |  |
| 0x1518 | `ASEG *` | `pasegUnk1518` | unknown (padding/placeholder) |  |
| 0x151C | `undefined4` | `unk_0x151c` | unknown (padding/placeholder) |  |
| 0x1520 | `XMG[2]` | `axmgUnk1520` | unknown (padding/placeholder) |  |
| 0x1720 | `SOPART *[25]` | `aprecJts` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1784 | `undefined4[19]` | `unk_0x1784` | unknown (padding/placeholder) |  |
| 0x17D0 | `struct JTHANDS` | `hands` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1880 | `int` | `cqwUnk1880` | unknown (padding/placeholder) |  |
| 0x1884 | `undefined4[3]` | `unk_0x1884` | unknown (padding/placeholder) |  |
| 0x1890 | `qword[3]` | `aqwUnk1890` | unknown (padding/placeholder) |  |
| 0x18C0 | `LO *` | `ploAttached` | asserted (CONFIRMED runtime) | +0x18C0 [R] object Sly is attached to (ledge/pole/rope owner); was ploUnk18c0 |
| 0x18C4 | `CID19 *` | `phndHang` | header (Lilly-ported; offset computed, unasserted) | +0x18C4 [R] current HND (hang/hook object, CID 0x13); was phndUnk18c4 |
| 0x18C8 | `JTS` | `jts` | asserted (0 standing / 11 pickpocket (CONFIRMED runtime)) |  |
| 0x18CC | `float` | `tJts` | asserted (CONFIRMED runtime) | +0x18CC [R] clock.t when jts was entered (t - tJts = time in state); was tUnk18cc |
| 0x18D0 | `JTBS` | `jtbs` | asserted (-1 in all states (CONFIRMED runtime)) | +0x18D0 [R] body state; -1 in all observed states |
| 0x18D4 | `float` | `tJtbs` | header (Lilly-ported; offset computed, unasserted) | +0x18D4 [S] clock.t when jtbs was entered (by analogy with tJts); was tUnk18d4 |
| 0x18D8 | `int` | `jtas` | asserted (CONFIRMED runtime) |  |
| 0x18DC | `float` | `tJtas` | header (Lilly-ported; offset computed, unasserted) | +0x18DC [R] clock.t when jtas was entered; was tUnk18dc |
| 0x18E0 | `struct JTANIM` | `janim` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1970 | `undefined4[9]` | `unk_0x1970` | unknown (padding/placeholder) |  |
| 0x1994 | `ASEG *` | `pasegHang` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1998 | `float` | `gUnk1998` | unknown (padding/placeholder) |  |
| 0x199C | `float` | `gUnk199c` | unknown (padding/placeholder) |  |
| 0x19A0 | `SO *` | `psoUnk19a0` | unknown (padding/placeholder) |  |
| 0x19A4 | `SO *` | `psoUnk19a4` | unknown (padding/placeholder) |  |
| 0x19A8 | `LO *` | `ploUnk19a8` | unknown (padding/placeholder) |  |
| 0x19AC | `float` | `gUnk19ac` | unknown (padding/placeholder) |  |
| 0x19B0 | `float` | `tUnk19b0` | unknown (padding/placeholder) |  |
| 0x19B4 | `undefined4[3]` | `unk_0x19b4` | unknown (padding/placeholder) |  |
| 0x19C0 | `void *` | `precUnk19c0` | unknown (padding/placeholder) |  |
| 0x19C4 | `void *` | `precUnk19c4` | unknown (padding/placeholder) |  |
| 0x19C8 | `void *` | `precUnk19c8` | unknown (padding/placeholder) |  |
| 0x19CC | `void *` | `precUnk19cc` | unknown (padding/placeholder) |  |
| 0x19D0 | `void *` | `precUnk19d0` | unknown (padding/placeholder) |  |
| 0x19D4 | `void *` | `precUnk19d4` | unknown (padding/placeholder) |  |
| 0x19D8 | `void *` | `precUnk19d8` | unknown (padding/placeholder) |  |
| 0x19DC | `void *` | `precUnk19dc` | unknown (padding/placeholder) |  |
| 0x19E0 | `void *` | `precUnk19e0` | unknown (padding/placeholder) |  |
| 0x19E4 | `ACT5 *` | `pact5Unk19e4` | unknown (padding/placeholder) |  |
| 0x19E8 | `ACT5 *` | `pact5Unk19e8` | unknown (padding/placeholder) |  |
| 0x19EC | `ACT5 *` | `pact5Unk19ec` | unknown (padding/placeholder) |  |
| 0x19F0 | `ACT5 *` | `pact5Unk19f0` | unknown (padding/placeholder) |  |
| 0x19F4 | `ACT5 *` | `pact5Unk19f4` | unknown (padding/placeholder) |  |
| 0x19F8 | `ACT5 *` | `pact5Unk19f8` | unknown (padding/placeholder) |  |
| 0x19FC | `ACT5 *` | `pact5Unk19fc` | unknown (padding/placeholder) |  |
| 0x1A00 | `int` | `nUnk1a00` | unknown (padding/placeholder) |  |
| 0x1A04 | `undefined4[6]` | `unk_0x1a04` | unknown (padding/placeholder) |  |
| 0x1A1C | `ASEGBL *` | `pasegblUnk1a1c` | unknown (padding/placeholder) |  |
| 0x1A20 | `undefined4[6]` | `unk_0x1a20` | unknown (padding/placeholder) |  |
| 0x1A38 | `ASEGBL *` | `pasegblUnk1a38` | unknown (padding/placeholder) |  |
| 0x1A3C | `undefined4[6]` | `unk_0x1a3c` | unknown (padding/placeholder) |  |
| 0x1A54 | `ASEGBL *` | `pasegblUnk1a54` | unknown (padding/placeholder) |  |
| 0x1A58 | `undefined4[6]` | `unk_0x1a58` | unknown (padding/placeholder) |  |
| 0x1A70 | `ASEGBL *` | `pasegblUnk1a70` | unknown (padding/placeholder) |  |
| 0x1A74 | `ASEG *` | `pasegUnk1a74` | unknown (padding/placeholder) |  |
| 0x1A78 | `ASEG *` | `pasegUnk1a78` | unknown (padding/placeholder) |  |
| 0x1A7C | `ASEG *` | `pasegUnk1a7c` | unknown (padding/placeholder) |  |
| 0x1A80 | `ASEG *` | `pasegUnk1a80` | unknown (padding/placeholder) |  |
| 0x1A84 | `ASEG *` | `pasegUnk1a84` | unknown (padding/placeholder) |  |
| 0x1A88 | `SOPART *` | `precUnk1a88` | unknown (padding/placeholder) |  |
| 0x1A8C | `SOPART *` | `precUnk1a8c` | unknown (padding/placeholder) |  |
| 0x1A90 | `SOPART *` | `precUnk1a90` | unknown (padding/placeholder) |  |
| 0x1A94 | `SOPART *` | `precUnk1a94` | unknown (padding/placeholder) |  |
| 0x1A98 | `SOPART *` | `precUnk1a98` | unknown (padding/placeholder) |  |
| 0x1A9C | `SOPART *` | `precUnk1a9c` | unknown (padding/placeholder) |  |
| 0x1AA0 | `SO *` | `psoUnk1aa0` | unknown (padding/placeholder) |  |
| 0x1AA4 | `float` | `tUnk1aa4` | unknown (padding/placeholder) |  |
| 0x1AA8 | `struct EXPL *` | `pexplDustLand` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1AAC | `struct EXPL *` | `pexplDustGrab` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1AB0 | `undefined4[4]` | `unk_0x1ab0` | unknown (padding/placeholder) |  |
| 0x1AC0 | `undefined4[64]` | `unk_0x1ac0` | unknown (padding/placeholder) |  |
| 0x1BC0 | `ASEGBL *` | `pasegblUnk1bc0` | unknown (padding/placeholder) |  |
| 0x1BC4 | `SM *` | `psmUnk1bc4` | unknown (padding/placeholder) |  |
| 0x1BC8 | `ASEG *` | `pasegPickpocket` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1BCC | `LO *` | `ploPickpocket` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1BD0 | `LO *` | `ploUnk1bd0` | unknown (padding/placeholder) |  |
| 0x1BD4 | `int` | `nUnk1bd4` | unknown (padding/placeholder) |  |
| 0x1BD8 | `undefined4[22]` | `unk_0x1bd8` | unknown (padding/placeholder) |  |
| 0x1C30 | `undefined4[44]` | `unk_0x1c30` | unknown (padding/placeholder) |  |
| 0x1CE0 | `ASEG *[13]` | `apasegSwing` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1D14 | `ASEG *` | `pasegAttackA` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1D18 | `ASEG *` | `pasegAttackB` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1D1C | `ASEG *` | `pasegUnk1d1c` | unknown (padding/placeholder) |  |
| 0x1D20 | `SO *` | `psoUnk1d20` | unknown (padding/placeholder) |  |
| 0x1D24 | `SM *` | `psmUnk1d24` | unknown (padding/placeholder) |  |
| 0x1D28 | `SMA *` | `psmaUnk1d28` | unknown (padding/placeholder) |  |
| 0x1D2C | `int` | `nUnk1d2c` | unknown (padding/placeholder) |  |
| 0x1D30 | `int` | `nUnk1d30` | unknown (padding/placeholder) |  |
| 0x1D34 | `ASEG *` | `pasegUnk1d34` | unknown (padding/placeholder) |  |
| 0x1D38 | `undefined4[2]` | `unk_0x1d38` | unknown (padding/placeholder) |  |
| 0x1D40 | `int` | `nUnk1d40` | unknown (padding/placeholder) |  |
| 0x1D44 | `undefined4[51]` | `unk_0x1d44` | unknown (padding/placeholder) |  |
| 0x1E10 | `undefined4[64]` | `unk_0x1e10` | unknown (padding/placeholder) |  |
| 0x1F10 | `ASEGBL *` | `pasegblUnk1f10` | unknown (padding/placeholder) |  |
| 0x1F14 | `SM *` | `psmUnk1f14` | unknown (padding/placeholder) |  |
| 0x1F18 | `LO *` | `ploUnk1f18` | unknown (padding/placeholder) |  |
| 0x1F1C | `undefined4[17]` | `unk_0x1f1c` | unknown (padding/placeholder) |  |
| 0x1F60 | `undefined4[12]` | `unk_0x1f60` | unknown (padding/placeholder) |  |
| 0x1F90 | `ASEGBL *` | `pasegblUnk1f90` | unknown (padding/placeholder) |  |
| 0x1F94 | `float` | `gUnk1f94` | unknown (padding/placeholder) |  |
| 0x1F98 | `float` | `gUnk1f98` | unknown (padding/placeholder) |  |
| 0x1F9C | `undefined4[5]` | `unk_0x1f9c` | unknown (padding/placeholder) |  |
| 0x1FB0 | `JTPOCKET[4]` | `apocket` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1FE0 | `int` | `nUnk1fe0` | unknown (padding/placeholder) |  |
| 0x1FE4 | `int` | `nUnk1fe4` | unknown (padding/placeholder) |  |
| 0x1FE8 | `ASEG *` | `pasegUnk1fe8` | unknown (padding/placeholder) |  |
| 0x1FEC | `undefined4` | `unk_0x1fec` | unknown (padding/placeholder) |  |
| 0x1FF0 | `undefined4` | `unk_0x1ff0` | unknown (padding/placeholder) |  |
| 0x1FF4 | `undefined4[7]` | `unk_0x1ff4` | unknown (padding/placeholder) |  |
| 0x2010 | `PROXY *` | `pproxyUnk2010` | unknown (padding/placeholder) |  |
| 0x2014 | `JTAIRBL[3]` | `ablLean` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x2038 | `ASEGBL *` | `pasegblUnk2038` | unknown (padding/placeholder) |  |
| 0x203C | `undefined4[2]` | `unk_0x203c` | unknown (padding/placeholder) |  |
| 0x2044 | `class CRefPtr<struct SND>` | `psndGadget` | asserted (CONFIRMED runtime) | +0x2044 [R] looping gadget sound (ram_inspector) |
| 0x2048 | `undefined4` | `unk_0x2048` | unknown (padding/placeholder) |  |
| 0x204C | `struct EXPL *` | `pexplUnk204c` | unknown (padding/placeholder) |  |
| 0x2050 | `ASEG *` | `pasegPouncePowerUp` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x2054 | `undefined4[3]` | `unk_0x2054` | unknown (padding/placeholder) |  |

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

[engine/player.md](../../engine/player.md) (28), [engine/guards-ai.md](../../engine/guards-ai.md) (6), [engine/world-sw.md](../../engine/world-sw.md) (3), [reference/save-variables.md](../../reference/save-variables.md) (3), [engine/animation.md](../../engine/animation.md) (2), [engine/object-model.md](../../engine/object-model.md) (2), [reference/glossary.md](../../reference/glossary.md) (2), [engine/main-loop-startup.md](../../engine/main-loop-startup.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
