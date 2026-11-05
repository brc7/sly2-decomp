# MEDICINE (CID 93, 0x5D)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> MEDICINE (health refill)

| | |
|---|---|
| Name | `MEDICINE` — medium: team coinage (v2 identified (whole-game parse)) |
| Parent chain | [MEDICINE](MEDICINE.md) → [DPRIZE](DPRIZE.md) → [SO](SO.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x250 (592 bytes), +0x20 over DPRIZE |
| Descriptor | `D_002BB22C` (record 93 of D_002BAAE8; cid range 93..94) |
| Factory (`pfnNew`) | [`NewMedicine`](../../../src/game/app/class_registry.cpp) `0x112268` |
| Descriptor getter (slot 6) | `GetMedicineClassDesc` `0x113FE0` |
| vtable | `D_002D66B8`, 77 entries (slots 0..76; slot 0 null), 0 new, 4 overridden |
| vtable stored by | [`NewMedicine`](../../../src/game/app/class_registry.cpp) (vptr +0xC) |
| Level instances | 42 objects in 42 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (DPRIZE) | `HandleDprizeMessage?` | `0x1C5098` |
| 6 | Pclassinfo | **override** | [`GetMedicineClassDesc`](../../../src/game/app/class_registry.cpp) | `0x113FE0` |
| 7 | Init | **override** | [`InitMedicine?`](../../../src/game/gameplay/progression/prize.cpp) | `0x1C67D8` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (SO) | `SetSoParent` | `0x1F21B8` |
| 11 | AddHierarchy | inherited (SO) | `AddSoHierarchy` | `0x1F2C30` |
| 12 | RemoveHierarchy | inherited (SO) | `RemoveSoHierarchy` | `0x1F2D48` |
| 13 | UpdateLoroot | inherited (SO) | `UpdateSoLoroot` | `0x1F2E78` |
| 14 | LoadFromBrx | inherited (DPRIZE) | `LoadDprizeFromBrx?` | `0x1C4EF8` |
| 15 | PostLoad | inherited (DPRIZE) | `PostDprizeLoad?` | `0x1C4F60` |
| 16 | Update | **override** | [`UpdateMedicine?`](../../../src/game/gameplay/progression/prize.cpp) | `0x1C6880` |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (DPRIZE) | `ProjectDprizeTransform?` | `0x1C5148` |
| 18 | UpdateLoXfWorldHierarchy | inherited (SO) | `UpdateSoXfWorldHierarchy` | `0x22FBE0` |
| 19 | RenderSelf | inherited (DPRIZE) | `RenderDprizeSelf?` | `0x1C5FA0` |
| 20 | RenderLoAll | inherited (SO) | `RenderSoAll?` | `0x1F3CC8` |
| 21 | SetLoFlag8 | inherited (SO) | `SetSoFlag8?` | `0x1F3B28` |
| 22 | OnRemove | inherited (SO) | `OnSoRemove` | `0x1F3E00` |
| 23 | OnAdd | inherited (SO) | `OnSoAdd` | `0x1F3EA8` |
| 24 |  | inherited (LO) | `func_00210330` | `0x210330` |
| 25 | SetIdProxy | inherited (SO) | `SetSoIdProxy` | `0x1CA308` |
| 26 | ResetLoForProxy | inherited (SO) | `ResetSoForProxy?` | `0x1CB518` |
| 27 | LO_VUnk27Proxy | inherited (SO) | `SO_VUnk27Proxy?` | `0x1CB5D0` |
| 28 | CploChildren | inherited (LO) | `LO_CploChildren` | `0x1A5BE8` |
| 29 | TranslateToPos | inherited (SO) | `TranslateSoToPos` | `0x1F40A0` |
| 30 | RotateToMat | inherited (SO) | `RotateSoToMat` | `0x1F42E0` |
| 31 | PmatWorld | inherited (SO) | `SO_PmatWorld` | `0x1F3FA0` |
| 32 | LO_PxfLocal | inherited (SO) | `SO_PxfLocal?` | `0x1F3FB0` |
| 33 | GetVelocity | inherited (SO) | `GetSoVelocity` | `0x1F4440` |
| 34 | GetAngularVelocity | inherited (SO) | `GetSoAngularVelocity` | `0x1F4558` |
| 35 | SetVelocity | inherited (SO) | `SetSoVelocity` | `0x1F4488` |
| 36 | SetAngularVelocity | inherited (SO) | `SetSoAngularVelocity` | `0x1F46D8` |
| 37 | ApplyLoProxyHierarchy | inherited (SO) | `ApplySoProxy?` | `0x1F4010` |
| 38 | MatchLoXf | inherited (SO) | `MatchSoXf?` | `0x1F49C8` |
| 39 | SetLoXf | inherited (SO) | `SetSoXf?` | `0x1F4A80` |
| 40 | CloneHierarchy | inherited (SO) | `CloneSoHierarchy` | `0x1F66F0` |
| 41 | Clone | inherited (SO) | `CloneSo` | `0x1F6068` |
| 42 | SetLoSBusy | inherited (SO) | `SetSoSBusy?` | `0x1F3118` |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` |
| 46 | SetVisMask | inherited (SO) | `SetSoVisMask` | `0x1A6DC8` |
| 47 | SetGrf | inherited (SO) | `SetSoGrf` | `0x1F2FE0` |
| 48 | ClearLoGrf | inherited (SO) | `ClearSoGrf` | `0x1F3078` |
| 49 | UpdateSoTreeBounds | inherited (SO) | `UpdateSoTreeBounds` | `0x1F50E0` |
| 50 | GetSoCpdefi | inherited (SO) | `GetSoCpdefi` | `0x1F58F0` |
| 51 | SO_PxfFromId | inherited (SO) | `SO_PxfFromId?` | `0x22D778` |
| 52 | CloneXfRef | inherited (SO) | `CloneXfRef?` | `0x1F4CA0` |
| 53 | SO_PtreeNew | inherited (SO) | `SO_PtreeNew?` | `0x1F56F8` |
| 54 | SO_vt54_Nop | inherited (SO) | `SO_vt54_Nop?` | `0x1F5988` |
| 55 | SO_vt55_Nop | inherited (SO) | `SO_vt55_Nop?` | `0x1F4FB0` |
| 56 | SO_vt56_Nop | inherited (SO) | `SO_vt56_Nop?` | `0x1F4FB8` |
| 57 | SO_vt57_Nop | inherited (SO) | `SO_vt57_Nop?` | `0x1F4FC0` |
| 58 | ScheduleSoTouchEvents | inherited (SO) | `ScheduleSoTouchEvents?` | `0x1F4F10` |
| 59 | SO_vt59_Nop | inherited (SO) | `SO_vt59_Nop?` | `0x1F4FA8` |
| 60 | SO_vt60_Nop | inherited (SO) | `SO_vt60_Nop?` | `0x1F5990` |
| 61 | FIgnoreSoIntersection | inherited (SO) | `FIgnoreSoIntersection` | `0x1F4FC8` |
| 62 | FIsSoFixed | inherited (SO) | `FIsSoFixed?` | `0x1F5040` |
| 63 | SO_vt63_Ret0 | inherited (SO) | `SO_vt63_Ret0?` | `0x1F5998` |
| 64 | ResetSoPhys | inherited (SO) | `ResetSoPhys?` | `0x1F3590` |
| 65 | ApplySoForcesXf | inherited (SO) | `ApplySoForcesXf?` | `0x1F9E70` |
| 66 | ProjectSoForce | inherited (SO) | `ProjectSoForce?` | `0x1FA328` |
| 67 | ApplySoQueuedMotion | inherited (SO) | `ApplySoQueuedMotion?` | `0x1FA528` |
| 68 | ApplySoContactImpulse | inherited (SO) | `ApplySoContactImpulse?` | `0x1FAD00` |
| 69 | GetSoVelocityAtPos | inherited (SO) | `GetSoVelocityAtPos?` | `0x1FADD0` |
| 70 | ApplySoPendingVelocity | inherited (SO) | `ApplySoPendingVelocity?` | `0x1F9B20` |
| 71 | AddSoExternalAccelerations | inherited (SO) | `AddSoExternalAccelerations?` | `0x2297F0` |
| 72 |  | inherited (SO) | `func_002297F8` | `0x2297F8` |
| 73 | FPosInSo | inherited (SO) | `FPosInSo?` | `0x1F72F8` |
| 74 | InflictSoContactZaps | inherited (SO) | `InflictSoContactZaps?` | `0x1F7410` |
| 75 | FZprHasField44 | inherited (DPRIZE) | `DPRIZE_VUnk75?` | `0x1C5DF0` |
| 76 | SetDprizeDprizes | **override** | [`SetMedicineDprizes?`](../../../src/game/gameplay/progression/prize.cpp) | `0x1C68F0` |

## Fields

No struct for `MEDICINE` in `include/sly2` yet. Size is known exactly (0x250); the bytes 0x230..0x250 are this class's own. Inherited fields: see [DPRIZE](DPRIZE.md).

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

[reference/class-tree.md](../../reference/class-tree.md) (2), [engine/game-progression.md](../../engine/game-progression.md) (1), [engine/gameplay-objects.md](../../engine/gameplay-objects.md) (1), [open-questions.md](../../open-questions.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
