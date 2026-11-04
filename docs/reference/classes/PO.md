# PO (CID 50, 0x32)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> Lilly PO : SO ends 0x180; own code TU 0x1C3D70 ("po")

| | |
|---|---|
| Name | `PO` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [PO](PO.md) → [SO](SO.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | [STEP](STEP.md), [TURRET](TURRET.md), [SAFEHOUSE](SAFEHOUSE.md), [HACK](HACK.md), [ROBOTTANK](ROBOTTANK.md), [VAULT](VAULT.md) |
| Descendants | 28 classes (CID 51..78) |
| Instance size (`cb`) | 0x180 (384 bytes), +0x30 over SO |
| Descriptor | `D_002BAED0` (record 50 of D_002BAAE8; cid range 50..79) |
| Factory (`pfnNew`) | [`NewPo`](../../../src/game/app/class_registry/NewPo.cpp) `0x111628` |
| Descriptor getter (slot 6) | `GetPoClassDesc` `0x113D30` |
| vtable | `D_002D5898`, 82 entries (slots 0..81; slot 0 null), 6 new, 5 overridden |
| vtable stored by | [`NewPo`](../../../src/game/app/class_registry/NewPo.cpp) (vptr +0xC) |
| Level instances | 12 objects in 12 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | **override** | [`HandlePoMessage`](../../../src/game/actor/po.cpp) | `0x1C3E20` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetPoClassDesc`](../../../src/game/app/class_registry/GetPoClassDesc.c) | `0x113D30` | Pclassinfo |
| 7 | Init | **override** | [`InitPo`](../../../src/game/actor/po.cpp) | `0x1C3D70` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | inherited (SO) | `SetSoParent` | `0x1F21B8` | SetParent |
| 11 | AddHierarchy | inherited (SO) | `AddSoHierarchy` | `0x1F2C30` | AddHierarchy |
| 12 | RemoveHierarchy | inherited (SO) | `RemoveSoHierarchy` | `0x1F2D48` | RemoveHierarchy |
| 13 | UpdateLoroot | inherited (SO) | `UpdateSoLoroot` | `0x1F2E78` | UpdateLoroot |
| 14 | LoadFromBrx | **override** | [`LoadPoFromBrx`](../../../src/game/actor/po.cpp) | `0x1C3DC8` | LoadFromBrx |
| 15 | PostLoad | inherited (SO) | `PostSoLoad` | `0x1F36A8` | VUnk14 |
| 16 | Update | inherited (SO) | `UpdateSo` | `0x1F36E0` | VUnk15 |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (SO) | `UpdateSoXfWorld?` | `0x1F3980` | VUnk16 |
| 18 | UpdateLoXfWorldHierarchy | inherited (SO) | `UpdateSoXfWorldHierarchy` | `0x22FBE0` | UpdateXfWorldHierarchy |
| 19 | RenderSelf | inherited (SO) | `RenderSoSelf` | `0x1F3C70` | RenderSelf |
| 20 | RenderLoAll | inherited (SO) | `RenderSoAll?` | `0x1F3CC8` | VUnk19 |
| 21 | SetLoFlag8 | inherited (SO) | `SetSoFlag8?` | `0x1F3B28` | VUnk20 |
| 22 | OnRemove | inherited (SO) | `OnSoRemove` | `0x1F3E00` | Freeze |
| 23 | OnAdd | inherited (SO) | `OnSoAdd` | `0x1F3EA8` | Unfreeze |
| 24 |  | inherited (LO) | `func_00210330` | `0x210330` | VUnk23 |
| 25 | SetIdProxy | inherited (SO) | `SetSoIdProxy` | `0x1CA308` | SetIdProxy |
| 26 | ResetLoForProxy | inherited (SO) | `ResetSoForProxy?` | `0x1CB518` | VUnk25 |
| 27 | LO_VUnk27Proxy | inherited (SO) | `SO_VUnk27Proxy?` | `0x1CB5D0` | VUnk26 |
| 28 | CploChildren | inherited (LO) | `LO_CploChildren` | `0x1A5BE8` | CploChildren |
| 29 | TranslateToPos | inherited (SO) | `TranslateSoToPos` | `0x1F40A0` | TranslateToPos |
| 30 | RotateToMat | inherited (SO) | `RotateSoToMat` | `0x1F42E0` | RotateToMat |
| 31 | PmatWorld | inherited (SO) | `SO_PmatWorld` | `0x1F3FA0` | PmatWorld |
| 32 | LO_PxfLocal | inherited (SO) | `SO_PxfLocal?` | `0x1F3FB0` | PxfLocal |
| 33 | GetVelocity | inherited (SO) | `GetSoVelocity` | `0x1F4440` | GetVelocity |
| 34 | GetAngularVelocity | inherited (SO) | `GetSoAngularVelocity` | `0x1F4558` | GetAngularVelocity |
| 35 | SetVelocity | inherited (SO) | `SetSoVelocity` | `0x1F4488` | SetVelocity |
| 36 | SetAngularVelocity | inherited (SO) | `SetSoAngularVelocity` | `0x1F46D8` | SetAngularVelocity |
| 37 | ApplyLoProxyHierarchy | inherited (SO) | `ApplySoProxy?` | `0x1F4010` | ApplyProxy |
| 38 | MatchLoXf | inherited (SO) | `MatchSoXf?` | `0x1F49C8` | VUnk37 |
| 39 | SetLoXf | inherited (SO) | `SetSoXf?` | `0x1F4A80` | VUnk38 |
| 40 | CloneHierarchy | inherited (SO) | `CloneSoHierarchy` | `0x1F66F0` | CloneHierarchy |
| 41 | Clone | **override** | [`ClonePo`](../../../src/game/actor/po.cpp) | `0x1C3DF8` | Clone |
| 42 | SetLoSBusy | inherited (SO) | `SetSoSBusy?` | `0x1F3118` | VUnk41 |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` | VUnk42 |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` | VUnk43 |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` | VUnk44 |
| 46 | SetVisMask | inherited (SO) | `SetSoVisMask` | `0x1A6DC8` | SetVisMask |
| 47 | SetGrf | inherited (SO) | `SetSoGrf` | `0x1F2FE0` | VUnk46 |
| 48 | ClearLoGrf | inherited (SO) | `ClearSoGrf` | `0x1F3078` | VUnk47 |
| 49 | UpdateSoTreeBounds | inherited (SO) | `UpdateSoTreeBounds` | `0x1F50E0` | VUnk48 |
| 50 | GetSoCpdefi | inherited (SO) | `GetSoCpdefi` | `0x1F58F0` | GetCpdefi |
| 51 | SO_PxfFromId | inherited (SO) | `SO_PxfFromId?` | `0x22D778` | PxfFromIxf |
| 52 | CloneXfRef | inherited (SO) | `CloneXfRef?` | `0x1F4CA0` | CloneXfRef |
| 53 | SO_PtreeNew | inherited (SO) | `SO_PtreeNew?` | `0x1F56F8` | VUnk52 |
| 54 | SO_vt54_Nop | inherited (SO) | `SO_vt54_Nop?` | `0x1F5988` | VUnk53 |
| 55 | SO_vt55_Nop | inherited (SO) | `SO_vt55_Nop?` | `0x1F4FB0` | AdjustNewXp |
| 56 | SO_vt56_Nop | inherited (SO) | `SO_vt56_Nop?` | `0x1F4FB8` | VUnk55 |
| 57 | SO_vt57_Nop | inherited (SO) | `SO_vt57_Nop?` | `0x1F4FC0` | VUnk56 |
| 58 | ScheduleSoTouchEvents | inherited (SO) | `ScheduleSoTouchEvents?` | `0x1F4F10` | VUnk57 |
| 59 | SO_vt59_Nop | inherited (SO) | `SO_vt59_Nop?` | `0x1F4FA8` | VUnk58 |
| 60 | SO_vt60_Nop | inherited (SO) | `SO_vt60_Nop?` | `0x1F5990` | VUnk59 |
| 61 | FIgnoreSoIntersection | inherited (SO) | `FIgnoreSoIntersection` | `0x1F4FC8` | FIgnoreIntersection |
| 62 | FIsSoFixed | inherited (SO) | `FIsSoFixed?` | `0x1F5040` | VUnk61 |
| 63 | SO_vt63_Ret0 | inherited (SO) | `SO_vt63_Ret0?` | `0x1F5998` | CpartExtra |
| 64 | ResetSoPhys | inherited (SO) | `ResetSoPhys?` | `0x1F3590` | InitPhys |
| 65 | ApplySoForcesXf | inherited (SO) | `ApplySoForcesXf?` | `0x1F9E70` | VUnk64 |
| 66 | ProjectSoForce | inherited (SO) | `ProjectSoForce?` | `0x1FA328` | VUnk65 |
| 67 | ApplySoQueuedMotion | inherited (SO) | `ApplySoQueuedMotion?` | `0x1FA528` | VUnk66 |
| 68 | ApplySoContactImpulse | inherited (SO) | `ApplySoContactImpulse?` | `0x1FAD00` | VUnk67 |
| 69 | GetSoVelocityAtPos | inherited (SO) | `GetSoVelocityAtPos?` | `0x1FADD0` | VUnk68 |
| 70 | ApplySoPendingVelocity | inherited (SO) | `ApplySoPendingVelocity?` | `0x1F9B20` | VUnk69 |
| 71 | AddSoExternalAccelerations | inherited (SO) | `AddSoExternalAccelerations?` | `0x2297F0` | VUnk70 |
| 72 |  | inherited (SO) | `func_002297F8` | `0x2297F8` | VUnk71 |
| 73 | FPosInSo | inherited (SO) | `FPosInSo?` | `0x1F72F8` | VUnk72 |
| 74 | InflictSoContactZaps | inherited (SO) | `InflictSoContactZaps?` | `0x1F7410` | ProcessSoZapContacts |
| 75 | FZprHasField44 | inherited (SO) | `FZprHasField44?` | `0x1F7588` | FTakeDamage |
| 76 | FInvulnerablePo | **new** | [`FInvulnerablePo`](../../../src/game/actor/po.cpp) | `0x1C3E90` | FInvulnerable |
| 77 | JthsCurrentPo | **new** | [`JthsCurrentPo`](../../../src/game/actor/po.cpp) | `0x1C3ED0` | JthsCurrent |
| 78 | OnPoActive | **new** | [`OnPoActive`](../../../src/game/actor/po.cpp) | `0x1C3FF0` | OnActive |
| 79 | SetPoCtlState | **new** | [`SetPoCtlState`](../../../src/game/actor/po.cpp) | `0x1C3ED8` | SetCtlState |
| 80 | FCanPoBeHealed | **new** | [`FCanPoBeHealed`](../../../src/game/actor/po.cpp) | `0x1C41B8` | FCanBeHealed |
| 81 | AddPoHpMp | **new** | [`AddPoHpMp`](../../../src/game/actor/po.cpp) | `0x1C4228` | AddHpMp |

## Fields

From `include/sly2/po.h` (`struct PO : SO`). PO: from src/game/gameplay/player/jt/JtChooseGroundState.cpp (15 copies in Lilly ports)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 1 header asserts agree, 0 disagree. Size from header: 0x180.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x150 | `POCTL *` | `pctl` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x154 | `uint` | `grfbtnForce` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x158 | `undefined4` | `unk_0x158` | unknown (padding/placeholder) |  |
| 0x15C | `int` | `nUnk15c` | unknown (padding/placeholder) |  |
| 0x160 | `undefined4[2]` | `unk_0x160` | unknown (padding/placeholder) |  |
| 0x168 | `PO *` | `ppoUnk168` | unknown (padding/placeholder) |  |
| 0x16C | `TN *` | `ptnCam` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x170 | `undefined4` | `unk_0x170` | unknown (padding/placeholder) |  |
| 0x174 | `void *` | `pvUnk174` | unknown (padding/placeholder) |  |
| 0x178 | `undefined4` | `unk_0x178` | unknown (padding/placeholder) |  |
| 0x17C | `int` | `nUnk17c` | unknown (padding/placeholder) |  |

Inherited fields: see [SO](SO.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L08_palace_ep00 | 1 |
| L14_prison_ep00 | 1 |
| L17_castle_ep00 | 1 |
| L29_train_ep19 | 1 |
| L30_train_ep20 | 1 |
| L32_sawmill_ep00 | 1 |
| L33_sawmill_ep22 | 1 |
| L34_sawmill_ep23 | 1 |
| L35_sawmill_ep24 | 1 |
| L36_sawmill_ep25 | 1 |
| L37_sawmill_ep26 | 1 |
| world_elf2A38A8 | 1 |

## Related docs

[engine/player.md](../../engine/player.md) (28), [engine/minigames.md](../../engine/minigames.md) (11), [reference/class-tree.md](../../reference/class-tree.md) (9), [engine/object-model.md](../../engine/object-model.md) (4), [engine/world-sw.md](../../engine/world-sw.md) (4), [README.md](../../README.md) (3), [engine/bosses.md](../../engine/bosses.md) (3), [engine/gameplay-objects.md](../../engine/gameplay-objects.md) (3)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
