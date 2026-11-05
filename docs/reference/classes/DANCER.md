# DANCER (CID 68, 0x44)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> DANCER (rhythm-game performer)

| | |
|---|---|
| Name | `DANCER` — medium: team coinage (v2 identified (whole-game parse)) |
| Parent chain | [DANCER](DANCER.md) → [STEP](STEP.md) → [PO](PO.md) → [SO](SO.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x410 (1040 bytes), +0x150 over STEP |
| Descriptor | `D_002BB038` (record 68 of D_002BAAE8; cid range 68..69) |
| Factory (`pfnNew`) | [`NewDancer`](../../../src/game/app/class_registry.cpp) `0x111A68` |
| Descriptor getter (slot 6) | `GetDancerClassDesc` `0x113E50` |
| vtable | `D_002CD748`, 88 entries (slots 0..87; slot 0 null), 0 new, 6 overridden |
| vtable stored by | [`NewDancer`](../../../src/game/app/class_registry.cpp) (vptr +0xC) |
| Level instances | 1 objects in 1 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | **override** | [`HandleDancerMessage?`](../../../src/game/gameplay/objects/rhythm.cpp) | `0x152250` |
| 6 | Pclassinfo | **override** | [`GetDancerClassDesc`](../../../src/game/app/class_registry/GetDancerClassDesc.c) | `0x113E50` |
| 7 | Init | inherited (STEP) | `InitStep` | `0x203948` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (SO) | `SetSoParent` | `0x1F21B8` |
| 11 | AddHierarchy | inherited (SO) | `AddSoHierarchy` | `0x1F2C30` |
| 12 | RemoveHierarchy | **override** | [`RemoveHierarchyDancer?`](../../../src/game/gameplay/objects/rhythm.cpp) | `0x1520C8` |
| 13 | UpdateLoroot | inherited (SO) | `UpdateSoLoroot` | `0x1F2E78` |
| 14 | LoadFromBrx | inherited (STEP) | `LoadStepFromBrx` | `0x2039C0` |
| 15 | PostLoad | **override** | [`PostLoadDancer?`](../../../src/game/gameplay/objects/rhythm.cpp) | `0x152100` |
| 16 | Update | **override** | [`UpdateDancer?`](../../../src/game/gameplay/objects/rhythm.cpp) | `0x152148` |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (SO) | `UpdateSoXfWorld?` | `0x1F3980` |
| 18 | UpdateLoXfWorldHierarchy | inherited (SO) | `UpdateSoXfWorldHierarchy` | `0x22FBE0` |
| 19 | RenderSelf | inherited (STEP) | `RenderStepSelf` | `0x203AD0` |
| 20 | RenderLoAll | inherited (SO) | `RenderSoAll?` | `0x1F3CC8` |
| 21 | SetLoFlag8 | inherited (SO) | `SetSoFlag8?` | `0x1F3B28` |
| 22 | OnRemove | inherited (SO) | `OnSoRemove` | `0x1F3E00` |
| 23 | OnAdd | inherited (SO) | `OnSoAdd` | `0x1F3EA8` |
| 24 |  | inherited (LO) | `func_00210330` | `0x210330` |
| 25 | SetIdProxy | inherited (SO) | `SetSoIdProxy` | `0x1CA308` |
| 26 | ResetLoForProxy | inherited (SO) | `ResetSoForProxy?` | `0x1CB518` |
| 27 | LO_VUnk27Proxy | inherited (SO) | `SO_VUnk27Proxy?` | `0x1CB5D0` |
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
| 41 | Clone | inherited (STEP) | `CloneStep` | `0x203A08` |
| 42 | SetLoSBusy | inherited (SO) | `SetSoSBusy?` | `0x1F3118` |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` |
| 46 | SetVisMask | inherited (SO) | `SetSoVisMask` | `0x1A6DC8` |
| 47 | SetGrf | inherited (SO) | `SetSoGrf` | `0x1F2FE0` |
| 48 | ClearLoGrf | inherited (SO) | `ClearSoGrf` | `0x1F3078` |
| 49 | UpdateSoTreeBounds | inherited (SO) | `UpdateSoTreeBounds` | `0x1F50E0` |
| 50 | GetSoCpdefi | **override** | [`GetDancerCpdefi`](../../../src/game/gameplay/objects/rhythm.cpp) | `0x152350` |
| 51 | SO_PxfFromId | inherited (SO) | `SO_PxfFromId?` | `0x22D778` |
| 52 | CloneXfRef | inherited (SO) | `CloneXfRef?` | `0x1F4CA0` |
| 53 | SO_PtreeNew | inherited (SO) | `SO_PtreeNew?` | `0x1F56F8` |
| 54 | SO_vt54_Nop | inherited (STEP) | `OnStepSoContactFreed` | `0x204C60` |
| 55 | SO_vt55_Nop | inherited (STEP) | `AdjustStepNewXp` | `0x204250` |
| 56 | SO_vt56_Nop | inherited (SO) | `SO_vt56_Nop?` | `0x1F4FB8` |
| 57 | SO_vt57_Nop | inherited (STEP) | `AdjustStepXpVelocity` | `0x2044B0` |
| 58 | ScheduleSoTouchEvents | inherited (STEP) | `AdjustStepXps?` | `0x204768` |
| 59 | SO_vt59_Nop | inherited (SO) | `SO_vt59_Nop?` | `0x1F4FA8` |
| 60 | SO_vt60_Nop | inherited (STEP) | `AddStepCustomXps` | `0x204AC8` |
| 61 | FIgnoreSoIntersection | inherited (SO) | `FIgnoreSoIntersection` | `0x1F4FC8` |
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
| 75 | FZprHasField44 | inherited (SO) | `FZprHasField44?` | `0x1F7588` |
| 76 | FInvulnerablePo | inherited (PO) | `FInvulnerablePo` | `0x1C3E90` |
| 77 | JthsCurrentPo | inherited (PO) | `JthsCurrentPo` | `0x1C3ED0` |
| 78 | OnPoActive | inherited (PO) | `OnPoActive` | `0x1C3FF0` |
| 79 | SetPoCtlState | inherited (PO) | `SetPoCtlState` | `0x1C3ED8` |
| 80 | FCanPoBeHealed | inherited (PO) | `FCanPoBeHealed` | `0x1C41B8` |
| 81 | AddPoHpMp | inherited (PO) | `AddPoHpMp` | `0x1C4228` |
| 82 | AddStepAxisXps | inherited (STEP) | `AddStepAxisXps` | `0x205008` |
| 83 | AddStepDialogSmRefs | inherited (STEP) | `AddStepDialogSmRefs?` | `0x205478` |
| 84 | RemoveStepDialogSmRefs | inherited (STEP) | `RemoveStepDialogSmRefs?` | `0x205588` |
| 85 | GrfStepGroundProbe | inherited (STEP) | `GrfStepGroundProbe?` | `0x2040F8` |
| 86 | FStepVt86 | inherited (STEP) | `FStepVt86?` | `0x205D08` |
| 87 | StepVt87Nop | inherited (STEP) | `StepVt87Nop?` | `0x205D10` |

## Fields

No struct for `DANCER` in `include/sly2` yet. Size is known exactly (0x410); the bytes 0x2C0..0x410 are this class's own. Inherited fields: see [STEP](STEP.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L11_palace_ep09 | 1 |

## Related docs

[engine/minigames.md](../../engine/minigames.md) (5), [reference/class-tree.md](../../reference/class-tree.md) (2), [engine/bosses.md](../../engine/bosses.md) (1), [engine/gameplay-objects.md](../../engine/gameplay-objects.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
