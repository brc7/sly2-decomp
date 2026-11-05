# STEP (CID 51, 0x33)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> Lilly STEP : PO ends 0x2C0; own code TU 0x203948 ("step")

| | |
|---|---|
| Name | `STEP` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [STEP](STEP.md) → [PO](PO.md) → [SO](SO.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | [STEPPLAYER](STEPPLAYER.md), [STEPGUARD](STEPGUARD.md), [DANCER](DANCER.md) |
| Descendants | 17 classes (CID 52..68) |
| Instance size (`cb`) | 0x2C0 (704 bytes), +0x140 over PO |
| Descriptor | `D_002BAEE4` (record 51 of D_002BAAE8; cid range 51..69) |
| Factory (`pfnNew`) | [`NewStep`](../../../src/game/app/class_registry.cpp) `0x111680` |
| Descriptor getter (slot 6) | `GetStepClassDesc` `0x113D40` |
| vtable | `D_002DA110`, 88 entries (slots 0..87; slot 0 null), 6 new, 17 overridden |
| vtable stored by | [`NewStep`](../../../src/game/app/class_registry.cpp) (vptr +0xC) |
| Level instances | none (not a world-file object class) |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | inherited (PO) | `HandlePoMessage` | `0x1C3E20` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetStepClassDesc`](../../../src/game/app/class_registry/GetStepClassDesc.c) | `0x113D40` | Pclassinfo |
| 7 | Init | **override** | [`InitStep`](../../../src/game/actor/step.cpp) | `0x203948` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | inherited (SO) | `SetSoParent` | `0x1F21B8` | SetParent |
| 11 | AddHierarchy | inherited (SO) | `AddSoHierarchy` | `0x1F2C30` | AddHierarchy |
| 12 | RemoveHierarchy | inherited (SO) | `RemoveSoHierarchy` | `0x1F2D48` | RemoveHierarchy |
| 13 | UpdateLoroot | inherited (SO) | `UpdateSoLoroot` | `0x1F2E78` | UpdateLoroot |
| 14 | LoadFromBrx | **override** | [`LoadStepFromBrx`](../../../src/game/actor/step.cpp) | `0x2039C0` | LoadFromBrx |
| 15 | PostLoad | **override** | [`PostStepLoad`](../../../src/game/actor/step.cpp) | `0x203A38` | VUnk14 |
| 16 | Update | **override** | [`UpdateStep`](../../../src/game/actor/step.cpp) | `0x203FC0` | VUnk15 |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (SO) | `UpdateSoXfWorld?` | `0x1F3980` | VUnk16 |
| 18 | UpdateLoXfWorldHierarchy | inherited (SO) | `UpdateSoXfWorldHierarchy` | `0x22FBE0` | UpdateXfWorldHierarchy |
| 19 | RenderSelf | **override** | [`RenderStepSelf`](../../../src/game/actor/step.cpp) | `0x203AD0` | RenderSelf |
| 20 | RenderLoAll | inherited (SO) | `RenderSoAll?` | `0x1F3CC8` | VUnk19 |
| 21 | SetLoFlag8 | inherited (SO) | `SetSoFlag8?` | `0x1F3B28` | VUnk20 |
| 22 | OnRemove | inherited (SO) | `OnSoRemove` | `0x1F3E00` | Freeze |
| 23 | OnAdd | inherited (SO) | `OnSoAdd` | `0x1F3EA8` | Unfreeze |
| 24 |  | inherited (LO) | `func_00210330` | `0x210330` | VUnk23 |
| 25 | SetIdProxy | inherited (SO) | `SetSoIdProxy` | `0x1CA308` | SetIdProxy |
| 26 | ResetLoForProxy | inherited (SO) | `ResetSoForProxy?` | `0x1CB518` | VUnk25 |
| 27 | LO_VUnk27Proxy | inherited (SO) | `SO_VUnk27Proxy?` | `0x1CB5D0` | VUnk26 |
| 28 | CploChildren | inherited (LO) | `LO_CploChildren` | `0x1A5BE8` | CploChildren |
| 29 | TranslateToPos | **override** | [`TranslateStepToPos`](../../../src/game/actor/step.cpp) | `0x2041C0` | TranslateToPos |
| 30 | RotateToMat | **override** | [`RotateStepToMat`](../../../src/game/actor/step.cpp) | `0x2041F8` | RotateToMat |
| 31 | PmatWorld | inherited (SO) | `SO_PmatWorld` | `0x1F3FA0` | PmatWorld |
| 32 | LO_PxfLocal | inherited (SO) | `SO_PxfLocal?` | `0x1F3FB0` | PxfLocal |
| 33 | GetVelocity | inherited (SO) | `GetSoVelocity` | `0x1F4440` | GetVelocity |
| 34 | GetAngularVelocity | inherited (SO) | `GetSoAngularVelocity` | `0x1F4558` | GetAngularVelocity |
| 35 | SetVelocity | inherited (SO) | `SetSoVelocity` | `0x1F4488` | SetVelocity |
| 36 | SetAngularVelocity | inherited (SO) | `SetSoAngularVelocity` | `0x1F46D8` | SetAngularVelocity |
| 37 | ApplyLoProxyHierarchy | inherited (SO) | `ApplySoProxy?` | `0x1F4010` | ApplyProxy |
| 38 | MatchLoXf | **override** | [`MatchStepXf`](../../../src/game/actor/step.cpp) | `0x2052E0` | VUnk37 |
| 39 | SetLoXf | inherited (SO) | `SetSoXf?` | `0x1F4A80` | VUnk38 |
| 40 | CloneHierarchy | inherited (SO) | `CloneSoHierarchy` | `0x1F66F0` | CloneHierarchy |
| 41 | Clone | **override** | [`CloneStep`](../../../src/game/actor/step.cpp) | `0x203A08` | Clone |
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
| 54 | SO_vt54_Nop | **override** | [`OnStepSoContactFreed`](../../../src/game/actor/step.cpp) | `0x204C60` | VUnk53 |
| 55 | SO_vt55_Nop | **override** | [`AdjustStepNewXp`](../../../src/game/actor/step.cpp) | `0x204250` | AdjustNewXp |
| 56 | SO_vt56_Nop | inherited (SO) | `SO_vt56_Nop?` | `0x1F4FB8` | VUnk55 |
| 57 | SO_vt57_Nop | **override** | [`AdjustStepXpVelocity`](../../../src/game/actor/step.cpp) | `0x2044B0` | VUnk56 |
| 58 | ScheduleSoTouchEvents | **override** | [`AdjustStepXps?`](../../../src/game/actor/step.cpp) | `0x204768` | VUnk57 |
| 59 | SO_vt59_Nop | inherited (SO) | `SO_vt59_Nop?` | `0x1F4FA8` | VUnk58 |
| 60 | SO_vt60_Nop | **override** | [`AddStepCustomXps`](../../../src/game/actor/step.cpp) | `0x204AC8` | VUnk59 |
| 61 | FIgnoreSoIntersection | inherited (SO) | `FIgnoreSoIntersection` | `0x1F4FC8` | FIgnoreIntersection |
| 62 | FIsSoFixed | inherited (SO) | `FIsSoFixed?` | `0x1F5040` | VUnk61 |
| 63 | SO_vt63_Ret0 | **override** | [`FStepVt63?`](../../../src/game/actor/step.cpp) | `0x204248` | CpartExtra |
| 64 | ResetSoPhys | inherited (SO) | `ResetSoPhys?` | `0x1F3590` | InitPhys |
| 65 | ApplySoForcesXf | inherited (SO) | `ApplySoForcesXf?` | `0x1F9E70` | VUnk64 |
| 66 | ProjectSoForce | inherited (SO) | `ProjectSoForce?` | `0x1FA328` | VUnk65 |
| 67 | ApplySoQueuedMotion | inherited (SO) | `ApplySoQueuedMotion?` | `0x1FA528` | VUnk66 |
| 68 | ApplySoContactImpulse | inherited (SO) | `ApplySoContactImpulse?` | `0x1FAD00` | VUnk67 |
| 69 | GetSoVelocityAtPos | inherited (SO) | `GetSoVelocityAtPos?` | `0x1FADD0` | VUnk68 |
| 70 | ApplySoPendingVelocity | **override** | [`PresetStepAccel`](../../../src/game/actor/step.cpp) | `0x204108` | VUnk69 |
| 71 | AddSoExternalAccelerations | inherited (SO) | `AddSoExternalAccelerations?` | `0x2297F0` | VUnk70 |
| 72 |  | inherited (SO) | `func_002297F8` | `0x2297F8` | VUnk71 |
| 73 | FPosInSo | inherited (SO) | `FPosInSo?` | `0x1F72F8` | VUnk72 |
| 74 | InflictSoContactZaps | inherited (SO) | `InflictSoContactZaps?` | `0x1F7410` | ProcessSoZapContacts |
| 75 | FZprHasField44 | inherited (SO) | `FZprHasField44?` | `0x1F7588` | FTakeDamage |
| 76 | FInvulnerablePo | inherited (PO) | `FInvulnerablePo` | `0x1C3E90` | FInvulnerable |
| 77 | JthsCurrentPo | inherited (PO) | `JthsCurrentPo` | `0x1C3ED0` | JthsCurrent |
| 78 | OnPoActive | inherited (PO) | `OnPoActive` | `0x1C3FF0` | OnActive |
| 79 | SetPoCtlState | inherited (PO) | `SetPoCtlState` | `0x1C3ED8` | SetCtlState |
| 80 | FCanPoBeHealed | inherited (PO) | `FCanPoBeHealed` | `0x1C41B8` | FCanBeHealed |
| 81 | AddPoHpMp | inherited (PO) | `AddPoHpMp` | `0x1C4228` | AddHpMp |
| 82 | AddStepAxisXps | **new** | [`AddStepAxisXps`](../../../src/game/actor/step.cpp) | `0x205008` | VUnk81 |
| 83 | AddStepDialogSmRefs | **new** | [`AddStepDialogSmRefs?`](../../../src/game/actor/step.cpp) | `0x205478` | VUnk82 |
| 84 | RemoveStepDialogSmRefs | **new** | [`RemoveStepDialogSmRefs?`](../../../src/game/actor/step.cpp) | `0x205588` | VUnk83 |
| 85 | GrfStepGroundProbe | **new** | [`GrfStepGroundProbe?`](../../../src/game/actor/step.cpp) | `0x2040F8` | VUnk84 |
| 86 | FStepVt86 | **new** | [`FStepVt86?`](../../../src/game/actor/step.cpp) | `0x205D08` | VUnk85 |
| 87 | StepVt87Nop | **new** | [`StepVt87Nop?`](../../../src/game/actor/step.cpp) | `0x205D10` | VUnk86 |

## Fields

From `include/sly2/step.h` (`struct STEP : PO`). STEP: from src/game/gameplay/player/jt.cpp (15 copies in Lilly ports)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 2 header asserts agree, 0 disagree. Size from header: 0x2C0.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x180 | `float` | `gUnk180` | unknown (padding/placeholder) |  |
| 0x184 | `undefined4[3]` | `unk_0x184` | unknown (padding/placeholder) |  |
| 0x190 | `struct VECTOR` | `vUnk190` | unknown (padding/placeholder) |  |
| 0x19C | `undefined4` | `unk_0x19c` | unknown (padding/placeholder) |  |
| 0x1A0 | `float` | `uGait` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1A4 | `int` | `nUnk1a4` | unknown (padding/placeholder) |  |
| 0x1A8 | `float` | `radTarget` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1AC | `undefined4` | `unk_0x1ac` | unknown (padding/placeholder) |  |
| 0x1B0 | `struct MATRIX3` | `matTarget` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1E0 | `qword` | `qwUnk1e0` | unknown (padding/placeholder) |  |
| 0x1F0 | `float` | `rSpin` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1F4 | `struct VECTOR` | `vSpin` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x200 | `float` | `rUnk200` | unknown (padding/placeholder) |  |
| 0x204 | `int` | `nUnk204` | unknown (padding/placeholder) |  |
| 0x208 | `XP *` | `pxpFloor` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x20C | `float` | `tUnk20c` | unknown (padding/placeholder) |  |
| 0x210 | `float` | `gUnk210` | unknown (padding/placeholder) |  |
| 0x214 | `float` | `gUnk214` | unknown (padding/placeholder) |  |
| 0x218 | `qword *` | `pqwUnk218` | unknown (padding/placeholder) |  |
| 0x21C | `qword *` | `pqwUnk21c` | unknown (padding/placeholder) |  |
| 0x220 | `float` | `gUnk220` | unknown (padding/placeholder) |  |
| 0x224 | `undefined4[3]` | `unk_0x224` | unknown (padding/placeholder) |  |
| 0x230 | `qword` | `qwUnk230` | unknown (padding/placeholder) |  |
| 0x240 | `qword` | `qwUnk240` | unknown (padding/placeholder) |  |
| 0x250 | `undefined4[2]` | `unk_0x250` | unknown (padding/placeholder) |  |
| 0x258 | `SOPART *` | `ppartUnk258` | unknown (padding/placeholder) |  |
| 0x25C | `undefined4` | `unk_0x25c` | unknown (padding/placeholder) |  |
| 0x260 | `float` | `rUnk260` | unknown (padding/placeholder) |  |
| 0x264 | `int` | `idUnk264` | unknown (padding/placeholder) |  |
| 0x268 | `SOPART *` | `precUnk268` | unknown (padding/placeholder) |  |
| 0x26C | `struct BHVSTK` | `bhvstk` | asserted (guard behaviour stack at STEP+0x26C (CONFIRMED runtime)) |  |
| 0x294 | `void *` | `pvUnk294` | unknown (padding/placeholder) |  |
| 0x298 | `int` | `nUnk298` | unknown (padding/placeholder) |  |
| 0x29C | `undefined4` | `unk_0x29c` | unknown (padding/placeholder) |  |
| 0x2A0 | `int[6]` | `acUnk2a0` | unknown (padding/placeholder) |  |

Inherited fields: see [PO](PO.md).

## Related docs

[engine/player.md](../../engine/player.md) (10), [engine/guards-ai.md](../../engine/guards-ai.md) (6), [reference/class-tree.md](../../reference/class-tree.md) (4), [engine/overview.md](../../engine/overview.md) (2), [README.md](../../README.md) (1), [engine/gameplay-objects.md](../../engine/gameplay-objects.md) (1), [engine/object-model.md](../../engine/object-model.md) (1), [engine/sound.md](../../engine/sound.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
