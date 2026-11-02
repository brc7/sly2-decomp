# GOMER (CID 57, 0x39)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> GOMER (instances are swarmers)

| | |
|---|---|
| Name | `GOMER` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [GOMER](GOMER.md) → [STEPGUARD](STEPGUARD.md) → [STEP](STEP.md) → [PO](PO.md) → [SO](SO.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x1010 (4112 bytes), +0x0 over STEPGUARD |
| Descriptor | `D_002BAF5C` (record 57 of D_002BAAE8; cid range 57..58) |
| Factory (`pfnNew`) | [`NewGomer`](../../../src/auto/NewGomer.c) `0x1117A8` |
| Descriptor getter (slot 6) | `GetGomerClassDesc` `0x113DA0` |
| vtable | `D_002CF520`, 90 entries (slots 0..89; slot 0 null), 0 new, 2 overridden |
| vtable stored by | [`NewGomer`](../../../src/auto/NewGomer.c) (vptr +0xC) |
| Level instances | 66 objects in 34 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (STEPGUARD) | `HandleStepguardMessage` | `0x2075D0` |
| 6 | Pclassinfo | **override** | [`GetGomerClassDesc`](../../../src/auto/GetGomerClassDesc.c) | `0x113DA0` |
| 7 | Init | **override** | [`RenderGomerSelf__FP5GOMERP2CMP2RO`](../../../src/game/gameplay/ai/gomer/RenderGomerSelf__FP5GOMERP2CMP2RO.cpp) | `0x16F148` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (SO) | `SetSoParent` | `0x1F21B8` |
| 11 | AddHierarchy | inherited (SO) | `AddSoHierarchy` | `0x1F2C30` |
| 12 | RemoveHierarchy | inherited (STEPGUARD) | `RemoveStepguardHierarchy` | `0x2064D8` |
| 13 | UpdateLoroot | inherited (SO) | `UpdateSoLoroot` | `0x1F2E78` |
| 14 | LoadFromBrx | inherited (STEP) | `LoadStepFromBrx` | `0x2039C0` |
| 15 | PostLoad | inherited (STEPGUARD) | `PostStepguardLoad` | `0x2061D8` |
| 16 | Update | inherited (STEPGUARD) | `UpdateStepguard` | `0x207350` |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (SO) | `UpdateSoXfWorld?` | `0x1F3980` |
| 18 | UpdateLoXfWorldHierarchy | inherited (SO) | `UpdateSoXfWorldHierarchy` | `0x22FBE0` |
| 19 | RenderSelf | inherited (STEPGUARD) | `RenderStepguardSelf` | `0x207690` |
| 20 | RenderLoAll | inherited (SO) | `RenderSoAll?` | `0x1F3CC8` |
| 21 | SetLoFlag8 | inherited (SO) | `SetSoFlag8?` | `0x1F3B28` |
| 22 | OnRemove | inherited (STEPGUARD) | `OnStepguardRemove` | `0x2079A0` |
| 23 | OnAdd | inherited (STEPGUARD) | `OnStepguardAdd` | `0x2079E0` |
| 24 |  | inherited (LO) | `func_00210330` | `0x210330` |
| 25 | SetIdProxy | inherited (SO) | `SetSoIdProxy` | `0x1CA308` |
| 26 | ResetLoForProxy | inherited (STEPGUARD) | `ResetStepguardToHome` | `0x206518` |
| 27 | LO_VUnk27Proxy | inherited (STEPGUARD) | `ClearStepguardState` | `0x206748` |
| 28 | CploChildren | inherited (LO) | `LO_CploChildren` | `0x1A5BE8` |
| 29 | TranslateToPos | inherited (STEP) | `TranslateStepToPos` | `0x2041C0` |
| 30 | RotateToMat | inherited (STEP) | `RotateStepToMat` | `0x2041F8` |
| 31 | PmatWorld | inherited (SO) | `SO_PmatWorld` | `0x1F3FA0` |
| 32 | LO_PxfLocal | inherited (SO) | `SO_PxfLocal?` | `0x1F3FB0` |
| 33 | GetVelocity | inherited (SO) | `GetSoVelocity` | `0x1F4440` |
| 34 | GetAngularVelocity | inherited (SO) | `GetSoAngularVelocity` | `0x1F4558` |
| 35 | SetVelocity | inherited (SO) | `SetSoVelocity` | `0x1F4488` |
| 36 | SetAngularVelocity | inherited (SO) | `SetSoAngularVelocity` | `0x1F46D8` |
| 37 | ApplyLoProxyHierarchy | inherited (SO) | `ApplySoProxy?` | `0x1F4010` |
| 38 | MatchLoXf | inherited (STEP) | `MatchStepXf` | `0x2052E0` |
| 39 | SetLoXf | inherited (SO) | `SetSoXf?` | `0x1F4A80` |
| 40 | CloneHierarchy | inherited (SO) | `CloneSoHierarchy` | `0x1F66F0` |
| 41 | Clone | inherited (STEPGUARD) | `CloneStepguard` | `0x206100` |
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
| 54 | SO_vt54_Nop | inherited (STEP) | `OnStepSoContactFreed` | `0x204C60` |
| 55 | SO_vt55_Nop | inherited (STEP) | `AdjustStepNewXp` | `0x204250` |
| 56 | SO_vt56_Nop | inherited (SO) | `SO_vt56_Nop?` | `0x1F4FB8` |
| 57 | SO_vt57_Nop | inherited (STEP) | `AdjustStepXpVelocity` | `0x2044B0` |
| 58 | ScheduleSoTouchEvents | inherited (STEP) | `AdjustStepXps?` | `0x204768` |
| 59 | SO_vt59_Nop | inherited (STEPGUARD) | `AddStepguardAttachXps?` | `0x209C88` |
| 60 | SO_vt60_Nop | inherited (STEP) | `AddStepCustomXps` | `0x204AC8` |
| 61 | FIgnoreSoIntersection | inherited (STEPGUARD) | `FIgnoreStepguardIntersection` | `0x206380` |
| 62 | FIsSoFixed | inherited (SO) | `FIsSoFixed?` | `0x1F5040` |
| 63 | SO_vt63_Ret0 | inherited (STEP) | `FStepVt63?` | `0x204248` |
| 64 | ResetSoPhys | inherited (SO) | `ResetSoPhys?` | `0x1F3590` |
| 65 | ApplySoForcesXf | inherited (SO) | `ApplySoForcesXf?` | `0x1F9E70` |
| 66 | ProjectSoForce | inherited (SO) | `ProjectSoForce?` | `0x1FA328` |
| 67 | ApplySoQueuedMotion | inherited (SO) | `ApplySoQueuedMotion?` | `0x1FA528` |
| 68 | ApplySoContactImpulse | inherited (SO) | `ApplySoContactImpulse?` | `0x1FAD00` |
| 69 | GetSoVelocityAtPos | inherited (SO) | `GetSoVelocityAtPos?` | `0x1FADD0` |
| 70 | ApplySoPendingVelocity | inherited (STEP) | `PresetStepAccel` | `0x204108` |
| 71 | AddSoExternalAccelerations | inherited (SO) | `AddSoExternalAccelerations?` | `0x2297F0` |
| 72 |  | inherited (SO) | `func_002297F8` | `0x2297F8` |
| 73 | FPosInSo | inherited (SO) | `FPosInSo?` | `0x1F72F8` |
| 74 | InflictSoContactZaps | inherited (SO) | `InflictSoContactZaps?` | `0x1F7410` |
| 75 | FZprHasField44 | inherited (STEPGUARD) | `FTakeStepguardDamage` | `0x207468` |
| 76 | FInvulnerablePo | inherited (STEPGUARD) | `FInvulnerableStepguard` | `0x207598` |
| 77 | JthsCurrentPo | inherited (STEPGUARD) | `JthsCurrentStepguard` | `0x20B3D0` |
| 78 | OnPoActive | inherited (PO) | `OnPoActive` | `0x1C3FF0` |
| 79 | SetPoCtlState | inherited (PO) | `SetPoCtlState` | `0x1C3ED8` |
| 80 | FCanPoBeHealed | inherited (PO) | `FCanPoBeHealed` | `0x1C41B8` |
| 81 | AddPoHpMp | inherited (PO) | `AddPoHpMp` | `0x1C4228` |
| 82 | AddStepAxisXps | inherited (STEP) | `AddStepAxisXps` | `0x205008` |
| 83 | AddStepDialogSmRefs | inherited (STEP) | `AddStepDialogSmRefs?` | `0x205478` |
| 84 | RemoveStepDialogSmRefs | inherited (STEP) | `RemoveStepDialogSmRefs?` | `0x205588` |
| 85 | GrfStepGroundProbe | inherited (STEPGUARD) | `GrfStepguardGroundProbe?` | `0x20B090` |
| 86 | FStepVt86 | inherited (STEPGUARD) | `FLethalStepguardZap?` | `0x207530` |
| 87 | StepVt87Nop | inherited (STEPGUARD) | `SetStepguardPlayable?` | `0x1F1F38` |
| 88 | PloChooseStepguardTarget | inherited (STEPGUARD) | `PloChooseStepguardTarget` | `0x2082D0` |
| 89 | FStepguardSenseStimulus | inherited (STEPGUARD) | `FStepguardSenseStimulus` | `0x208BE0` |

## Fields

No struct for `GOMER` in `include/sly2` yet. Size is known exactly (0x1010); the bytes 0x1010..0x1010 are this class's own. Inherited fields: see [STEPGUARD](STEPGUARD.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 2 |
| L03_nightclub_ep02 | 1 |
| L04_nightclub_ep03 | 1 |
| L07_nightclub_ep06 | 1 |
| L08_palace_ep00 | 3 |
| L09_palace_ep07 | 3 |
| L10_palace_ep08 | 3 |
| L11_palace_ep09 | 1 |
| L12_temple_ep00 | 4 |
| L13_temple_ep07 | 2 |
| L14_prison_ep00 | 3 |
| L15_prison_ep07 | 2 |
| L17_castle_ep00 | 3 |
| L19_castle_ep11 | 3 |
| L20_castle_ep12 | 2 |
| L21_castle_ep13 | 2 |
| L22_castle_ep04 | 2 |
| L23_castle_ep14 | 2 |
| L24_castle_ep15 | 2 |
| L25_castle_ep16 | 2 |
| L26_castle_ep17 | 2 |
| L27_train_ep00 | 3 |
| L28_train_ep18 | 1 |
| L29_train_ep19 | 2 |
| L30_train_ep20 | 1 |
| L31_train_ep21 | 2 |
| L32_sawmill_ep00 | 2 |
| L35_sawmill_ep24 | 1 |
| L37_sawmill_ep26 | 1 |
| L38_blimp_ep00 | 2 |
| L39_blimp_ep07 | 1 |
| L40_blimp_ep27 | 2 |
| L41_blimp_ep28 | 1 |
| L43_blimp_ep26 | 1 |

## Related docs

[engine/guards-ai.md](../../engine/guards-ai.md) (7), [engine/bosses.md](../../engine/bosses.md) (2), [reference/class-tree.md](../../reference/class-tree.md) (2), [engine/overview.md](../../engine/overview.md) (1), [formats/level-objects.md](../../formats/level-objects.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
