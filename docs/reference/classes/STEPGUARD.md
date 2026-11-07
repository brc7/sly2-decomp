# STEPGUARD (CID 56, 0x38)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> STEP child with 11 subclasses; own code TU 0x205E70 ("stepguard")

| | |
|---|---|
| Name | `STEPGUARD` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [STEPGUARD](STEPGUARD.md) → [STEP](STEP.md) → [PO](PO.md) → [SO](SO.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | [GOMER](GOMER.md), [GUARD](GUARD.md), [NEELA](NEELA.md), [RAJAN](RAJAN.md), [JEANBISON](JEANBISON.md), [CARMELITA](CARMELITA.md), [CONTESSA](CONTESSA.md), [ARPEGGIO](ARPEGGIO.md), [BEAR](BEAR.md), [BEARCUB](BEARCUB.md) |
| Descendants | 11 classes (CID 57..67) |
| Instance size (`cb`) | 0x1010 (4112 bytes), +0xD50 over STEP |
| Descriptor | `D_002BAF48` (record 56 of D_002BAAE8; cid range 56..68) |
| Factory (`pfnNew`) | [`NewStepguard`](../../../src/game/app/class_registry.cpp) `0x111780` |
| Descriptor getter (slot 6) | `GetStepguardClassDesc` `0x113D90` |
| vtable | `D_002DA568`, 90 entries (slots 0..89; slot 0 null), 2 new, 20 overridden |
| vtable stored by | [`STEPGUARD_ctor?`](../../../src/game/app/class_registry.cpp) (vptr +0xC) |
| Level instances | none (not a world-file object class) |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | **override** | [`HandleStepguardMessage`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x2075D0` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetStepguardClassDesc`](../../../src/game/app/class_registry.cpp) | `0x113D90` | Pclassinfo |
| 7 | Init | **override** | [`InitStepguard`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x205EB8` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | inherited (SO) | `SetSoParent` | `0x1F21B8` | SetParent |
| 11 | AddHierarchy | inherited (SO) | `AddSoHierarchy` | `0x1F2C30` | AddHierarchy |
| 12 | RemoveHierarchy | **override** | [`RemoveStepguardHierarchy`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x2064D8` | RemoveHierarchy |
| 13 | UpdateLoroot | inherited (SO) | `UpdateSoLoroot` | `0x1F2E78` | UpdateLoroot |
| 14 | LoadFromBrx | inherited (STEP) | `LoadStepFromBrx` | `0x2039C0` | LoadFromBrx |
| 15 | PostLoad | **override** | [`PostStepguardLoad`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x2061D8` | VUnk14 |
| 16 | Update | **override** | [`UpdateStepguard`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x207350` | VUnk15 |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (SO) | `UpdateSoXfWorld?` | `0x1F3980` | VUnk16 |
| 18 | UpdateLoXfWorldHierarchy | inherited (SO) | `UpdateSoXfWorldHierarchy` | `0x22FBE0` | UpdateXfWorldHierarchy |
| 19 | RenderSelf | **override** | [`RenderStepguardSelf`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x207690` | RenderSelf |
| 20 | RenderLoAll | inherited (SO) | `RenderSoAll?` | `0x1F3CC8` | VUnk19 |
| 21 | SetLoFlag8 | inherited (SO) | `SetSoFlag8?` | `0x1F3B28` | VUnk20 |
| 22 | OnRemove | **override** | [`OnStepguardRemove`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x2079A0` | Freeze |
| 23 | OnAdd | **override** | [`OnStepguardAdd`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x2079E0` | Unfreeze |
| 24 |  | inherited (LO) | `func_00210330` | `0x210330` | VUnk23 |
| 25 | SetIdProxy | inherited (SO) | `SetSoIdProxy` | `0x1CA308` | SetIdProxy |
| 26 | ResetLoForProxy | **override** | [`ResetStepguardToHome`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x206518` | VUnk25 |
| 27 | LO_VUnk27Proxy | **override** | [`ClearStepguardState`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x206748` | VUnk26 |
| 28 | CploChildren | inherited (LO) | `LO_CploChildren` | `0x1A5BE8` | CploChildren |
| 29 | TranslateToPos | inherited (STEP) | `TranslateStepToPos` | `0x2041C0` | TranslateToPos |
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
| 41 | Clone | **override** | [`CloneStepguard`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x206100` | Clone |
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
| 54 | SO_vt54_Nop | inherited (STEP) | `OnStepSoContactFreed` | `0x204C60` | VUnk53 |
| 55 | SO_vt55_Nop | inherited (STEP) | `AdjustStepNewXp` | `0x204250` | AdjustNewXp |
| 56 | SO_vt56_Nop | inherited (SO) | `SO_vt56_Nop?` | `0x1F4FB8` | VUnk55 |
| 57 | SO_vt57_Nop | inherited (STEP) | `AdjustStepXpVelocity` | `0x2044B0` | VUnk56 |
| 58 | ScheduleSoTouchEvents | inherited (STEP) | `AdjustStepXps?` | `0x204768` | VUnk57 |
| 59 | SO_vt59_Nop | **override** | [`AddStepguardAttachXps?`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x209C88` | VUnk58 |
| 60 | SO_vt60_Nop | inherited (STEP) | `AddStepCustomXps` | `0x204AC8` | VUnk59 |
| 61 | FIgnoreSoIntersection | **override** | [`FIgnoreStepguardIntersection`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x206380` | FIgnoreIntersection |
| 62 | FIsSoFixed | inherited (SO) | `FIsSoFixed?` | `0x1F5040` | VUnk61 |
| 63 | SO_vt63_Ret0 | inherited (STEP) | `FStepVt63?` | `0x204248` | CpartExtra |
| 64 | ResetSoPhys | inherited (SO) | `ResetSoPhys?` | `0x1F3590` | InitPhys |
| 65 | ApplySoForcesXf | inherited (SO) | `ApplySoForcesXf?` | `0x1F9E70` | VUnk64 |
| 66 | ProjectSoForce | inherited (SO) | `ProjectSoForce?` | `0x1FA328` | VUnk65 |
| 67 | ApplySoQueuedMotion | inherited (SO) | `ApplySoQueuedMotion?` | `0x1FA528` | VUnk66 |
| 68 | ApplySoContactImpulse | inherited (SO) | `ApplySoContactImpulse?` | `0x1FAD00` | VUnk67 |
| 69 | GetSoVelocityAtPos | inherited (SO) | `GetSoVelocityAtPos?` | `0x1FADD0` | VUnk68 |
| 70 | ApplySoPendingVelocity | inherited (STEP) | `PresetStepAccel` | `0x204108` | VUnk69 |
| 71 | AddSoExternalAccelerations | inherited (SO) | `AddSoExternalAccelerations?` | `0x2297F0` | VUnk70 |
| 72 |  | inherited (SO) | `func_002297F8` | `0x2297F8` | VUnk71 |
| 73 | FPosInSo | inherited (SO) | `FPosInSo?` | `0x1F72F8` | VUnk72 |
| 74 | InflictSoContactZaps | inherited (SO) | `InflictSoContactZaps?` | `0x1F7410` | ProcessSoZapContacts |
| 75 | FZprHasField44 | **override** | [`FTakeStepguardDamage`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x207468` | FTakeDamage |
| 76 | FInvulnerablePo | **override** | [`FInvulnerableStepguard`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x207598` | FInvulnerable |
| 77 | JthsCurrentPo | **override** | [`JthsCurrentStepguard`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x20B3D0` | JthsCurrent |
| 78 | OnPoActive | inherited (PO) | `OnPoActive` | `0x1C3FF0` | OnActive |
| 79 | SetPoCtlState | inherited (PO) | `SetPoCtlState` | `0x1C3ED8` | SetCtlState |
| 80 | FCanPoBeHealed | inherited (PO) | `FCanPoBeHealed` | `0x1C41B8` | FCanBeHealed |
| 81 | AddPoHpMp | inherited (PO) | `AddPoHpMp` | `0x1C4228` | AddHpMp |
| 82 | AddStepAxisXps | inherited (STEP) | `AddStepAxisXps` | `0x205008` | VUnk81 |
| 83 | AddStepDialogSmRefs | inherited (STEP) | `AddStepDialogSmRefs?` | `0x205478` | VUnk82 |
| 84 | RemoveStepDialogSmRefs | inherited (STEP) | `RemoveStepDialogSmRefs?` | `0x205588` | VUnk83 |
| 85 | GrfStepGroundProbe | **override** | [`GrfStepguardGroundProbe?`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x20B090` | VUnk84 |
| 86 | FStepVt86 | **override** | [`FLethalStepguardZap?`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x207530` | VUnk85 |
| 87 | StepVt87Nop | **override** | [`SetStepguardPlayable?`](../../../src/game/gameplay/ai/pickpocket.cpp) | `0x1F1F38` | VUnk86 |
| 88 | PloChooseStepguardTarget | **new** | [`PloChooseStepguardTarget`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x2082D0` | VSlot87 |
| 89 | FStepguardSenseStimulus | **new** | [`FStepguardSenseStimulus`](../../../src/game/gameplay/ai/stepguard.cpp) | `0x208BE0` | VSlot88 |

## Fields

From `include/sly2/guard.h` (`struct STEPGUARD : STEP`). STEPGUARD: class descriptor cb 0x1010, vtable 89 entries (types/cid_table.csv, CONFIRMED runtime)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword; tail-padding reuse undone (+8)); 9 header asserts agree, 0 disagree. Size from header: 0x1010.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x2C0 | `unsigned long long` | `grfsg` | asserted (CONFIRMED runtime) |  |
| 0x2C8 | `undefined4[26]` | `unk_0x2c8` | unknown (padding/placeholder) |  |
| 0x330 | `int` | `fKO` | header (Lilly-ported; offset computed, unasserted) | +0x330 [R] knocked out |
| 0x334 | `undefined4` | `unk_0x334` | unknown (padding/placeholder) |  |
| 0x338 | `int` | `hpMax` | header (Lilly-ported; offset computed, unasserted) | +0x338 [R] max hp |
| 0x33C | `int` | `hp` | asserted (CONFIRMED runtime) | +0x33C [R] current hp |
| 0x340 | `undefined4[19]` | `unk_0x340` | unknown (padding/placeholder) |  |
| 0x38C | `LO *` | `ploTarget` | asserted (CONFIRMED runtime) | +0x38C [R] current target (Sly while chasing) |
| 0x390 | `undefined4[15]` | `unk_0x390` | unknown (padding/placeholder) |  |
| 0x3CC | `void *` | `pvEngage` | header (Lilly-ported; offset computed, unasserted) | +0x3CC [R] engagement record (non-zero while in combat) [H type] |
| 0x3D0 | `undefined4[13]` | `unk_0x3d0` | unknown (padding/placeholder) |  |
| 0x404 | `GUARDLOOT[8]` | `aloot` | asserted (CONFIRMED runtime) | +0x404 [R] pickpocket loot list |
| 0x484 | `int` | `cloot` | asserted (CONFIRMED runtime) | +0x484 [R] entries used in aloot |
| 0x488 | `undefined4[694]` | `unk_0x488` | unknown (padding/placeholder) |  |
| 0xF60 | `int` | `nFiringWindow` | header (Lilly-ported; offset computed, unasserted) | +0xF60 [R] firing window (shooting guards) |
| 0xF64 | `LO *` | `pdetector` | asserted (CONFIRMED runtime) | +0xF64 [R] vision detector (CID 0x10; its +0xE0 owner == this guard) |
| 0xF68 | `undefined4[30]` | `unk_0xf68` | unknown (padding/placeholder) |  |
| 0xFE0 | `struct DLE` | `dleGuard` | asserted (CONFIRMED runtime) | +0xFE0 [R] link in the in-world guard list (DL at 0x3E1444, ibDle 0xFE0) |
| 0xFE8 | `uint` | `grfCapability` | header (Lilly-ported; offset computed, unasserted) | +0xFE8 [R] capability bits (bit 3 = may shoot) |
| 0xFEC | `undefined4[4]` | `unk_0xfec` | unknown (padding/placeholder) |  |
| 0xFFC | `int` | `cBusy` | asserted (CONFIRMED runtime) | +0xFFC [R] busy count |
| 0x1000 | `undefined4[4]` | `unk_0x1000` | unknown (padding/placeholder) |  |

Inherited fields: see [STEP](STEP.md).

## Related docs

[engine/bosses.md](../../engine/bosses.md) (16), [engine/guards-ai.md](../../engine/guards-ai.md) (14), [reference/class-tree.md](../../reference/class-tree.md) (12), [engine/overview.md](../../engine/overview.md) (4), [engine/object-model.md](../../engine/object-model.md) (2), [engine/splice-scripting.md](../../engine/splice-scripting.md) (2), [reference/glossary.md](../../reference/glossary.md) (2), [README.md](../../README.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
