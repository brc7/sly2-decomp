# JOB (CID 132, 0x84)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> JOB (save m<N>_<name>)

| | |
|---|---|
| Name | `JOB` — medium: team coinage (v1 identified) |
| Parent chain | [JOB](JOB.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x100 (256 bytes), +0xB0 over LO |
| Descriptor | `D_002BB538` (record 132 of D_002BAAE8; cid range 132..133) |
| Factory (`pfnNew`) | [`NewJob`](../../../src/game/app/class_registry/NewJob.c) `0x112D38` |
| Descriptor getter (slot 6) | `GetJobClassDesc` `0x114250` |
| vtable | `D_002CF378`, 49 entries (slots 0..48; slot 0 null), 0 new, 5 overridden |
| vtable stored by | [`NewJob`](../../../src/game/app/class_registry/NewJob.c) (vptr +0xC) |
| Level instances | 388 objects in 42 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetJobClassDesc`](../../../src/game/app/class_registry/GetJobClassDesc.c) | `0x114250` | Pclassinfo |
| 7 | Init | **override** | [`InitJob`](../../../src/game/world/state/game.cpp) | `0x162448` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | inherited (LO) | `SetLoParent?` | `0x1A5900` | SetParent |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` | AddHierarchy |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` | RemoveHierarchy |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` | UpdateLoroot |
| 14 | LoadFromBrx | **override** | [`LoadJobFromBrx`](../../../src/game/world/state/game.cpp) | `0x1624A8` | LoadFromBrx |
| 15 | PostLoad | **override** | [`PostJobLoad`](../../../src/game/world/state/game.cpp) | `0x162780` | VUnk14 |
| 16 | Update | **override** | [`UpdateJob`](../../../src/game/world/state/game.cpp) | `0x163078` | VUnk15 |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (LO) | `LO_vt17_InheritFlag4?` | `0x1A5E80` | VUnk16 |
| 18 | UpdateLoXfWorldHierarchy | inherited (LO) | `UpdateLoXfWorldHierarchy` | `0x22FBD8` | UpdateXfWorldHierarchy |
| 19 | RenderSelf | inherited (LO) | `RenderLoSelf` | `0x1A5ED8` | RenderSelf |
| 20 | RenderLoAll | inherited (LO) | `RenderLoAll?` | `0x1A5EE0` | VUnk19 |
| 21 | SetLoFlag8 | inherited (LO) | `SetLoFlag8?` | `0x1A6E80` | VUnk20 |
| 22 | OnRemove | inherited (LO) | `OnLoRemove` | `0x1A6150` | Freeze |
| 23 | OnAdd | inherited (LO) | `OnLoAdd` | `0x1A6178` | Unfreeze |
| 24 |  | inherited (LO) | `func_00210330` | `0x210330` | VUnk23 |
| 25 | SetIdProxy | inherited (LO) | `SetLoIdProxy` | `0x1CA118` | SetIdProxy |
| 26 | ResetLoForProxy | inherited (LO) | `ResetLoForProxy?` | `0x1CB4E0` | VUnk25 |
| 27 | LO_VUnk27Proxy | inherited (LO) | `LO_VUnk27Proxy?` | `0x1CB5C8` | VUnk26 |
| 28 | CploChildren | inherited (LO) | `LO_CploChildren` | `0x1A5BE8` | CploChildren |
| 29 | TranslateToPos | inherited (LO) | `TranslateLoToPos` | `0x1A6110` | TranslateToPos |
| 30 | RotateToMat | inherited (LO) | `RotateLoToMat` | `0x1A6118` | RotateToMat |
| 31 | PmatWorld | inherited (LO) | `LO_PmatWorld` | `0x1A62D8` | PmatWorld |
| 32 | LO_PxfLocal | inherited (LO) | `LO_PxfLocal?` | `0x1A6B30` | PxfLocal |
| 33 | GetVelocity | inherited (LO) | `GetLoVelocity?` | `0x1A6120` | GetVelocity |
| 34 | GetAngularVelocity | inherited (LO) | `GetLoAngularVelocity` | `0x1A6138` | GetAngularVelocity |
| 35 | SetVelocity | inherited (LO) | `SetLoVelocity?` | `0x1A6130` | SetVelocity |
| 36 | SetAngularVelocity | inherited (LO) | `SetLoAngularVelocity` | `0x1A6148` | SetAngularVelocity |
| 37 | ApplyLoProxyHierarchy | inherited (LO) | `ApplyLoProxyHierarchy?` | `0x1A6450` | ApplyProxy |
| 38 | MatchLoXf | inherited (LO) | `MatchLoXf?` | `0x1A6B88` | VUnk37 |
| 39 | SetLoXf | inherited (LO) | `SetLoXf?` | `0x1A6BC8` | VUnk38 |
| 40 | CloneHierarchy | inherited (LO) | `CloneLoHierarchy` | `0x1A6AD0` | CloneHierarchy |
| 41 | Clone | inherited (LO) | `CloneLo` | `0x1A6730` | Clone |
| 42 | SetLoSBusy | inherited (LO) | `SetLoSBusy?` | `0x1A6C48` | VUnk41 |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` | VUnk42 |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` | VUnk43 |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` | VUnk44 |
| 46 | SetVisMask | inherited (LO) | `SetLoVisMask?` | `0x1A6DC0` | SetVisMask |
| 47 | SetGrf | inherited (LO) | `SetLoGrf?` | `0x1A66C8` | VUnk46 |
| 48 | ClearLoGrf | inherited (LO) | `ClearLoGrf?` | `0x1A66F8` | VUnk47 |

## Fields

From `include/sly2/task.h` (`struct JOB : LO`). 

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 3 header asserts agree, 0 disagree. Size from header: 0x100.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x50 | `int` | `fDisabled` | header (Lilly-ported; offset computed, unasserted) | +0x50 [R] |
| 0x54 | `undefined4[4]` | `unk_0x54` | unknown (padding/placeholder) |  |
| 0x64 | `TASK *` | `ptaskStart` | asserted (CONFIRMED runtime) | +0x64 [R] first task of the job |
| 0x68 | `TASK *` | `ptaskEnd` | header (Lilly-ported; offset computed, unasserted) | +0x68 [R] final task |
| 0x6C | `undefined4[4]` | `unk_0x6c` | unknown (padding/placeholder) |  |
| 0x7C | `int` | `cCount` | header (Lilly-ported; offset computed, unasserted) | +0x7C [R] counter shown by the job (ram_inspector "count") |
| 0x80 | `undefined4[9]` | `unk_0x80` | unknown (padding/placeholder) |  |
| 0xA4 | `float` | `dtElapsed` | header (Lilly-ported; offset computed, unasserted) | +0xA4 [R] elapsed time in the job |
| 0xA8 | `undefined4[7]` | `unk_0xa8` | unknown (padding/placeholder) |  |
| 0xC4 | `int` | `nPhase` | asserted (CONFIRMED runtime) | +0xC4 [R] completion phase |
| 0xC8 | `undefined4[2]` | `unk_0xc8` | unknown (padding/placeholder) |  |
| 0xD0 | `int` | `ksvName` | header (Lilly-ported; offset computed, unasserted) | +0xD0 [R] save-name reference {kind, prec} (job save-variable name) |
| 0xD4 | `void *` | `psvName` | header (Lilly-ported; offset computed, unasserted) | +0xD4 [R] |
| 0xD8 | `undefined4[10]` | `unk_0xd8` | unknown (padding/placeholder) |  |

Inherited fields: see [LO](LO.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 11 |
| L03_nightclub_ep02 | 11 |
| L04_nightclub_ep03 | 11 |
| L05_nightclub_ep04 | 11 |
| L06_nightclub_ep05 | 11 |
| L07_nightclub_ep06 | 11 |
| L08_palace_ep00 | 12 |
| L09_palace_ep07 | 12 |
| L10_palace_ep08 | 12 |
| L11_palace_ep09 | 12 |
| L12_temple_ep00 | 10 |
| L13_temple_ep07 | 10 |
| L14_prison_ep00 | 9 |
| L15_prison_ep07 | 9 |
| L16_prison_ep10 | 9 |
| L17_castle_ep00 | 8 |
| L19_castle_ep11 | 8 |
| L20_castle_ep12 | 8 |
| L21_castle_ep13 | 8 |
| L22_castle_ep04 | 8 |
| L23_castle_ep14 | 8 |
| L24_castle_ep15 | 8 |
| L25_castle_ep16 | 8 |
| L26_castle_ep17 | 8 |
| L27_train_ep00 | 8 |
| L28_train_ep18 | 8 |
| L29_train_ep19 | 8 |
| L30_train_ep20 | 8 |
| L31_train_ep21 | 8 |
| L32_sawmill_ep00 | 10 |
| L33_sawmill_ep22 | 10 |
| L34_sawmill_ep23 | 10 |
| L35_sawmill_ep24 | 10 |
| L36_sawmill_ep25 | 10 |
| L37_sawmill_ep26 | 10 |
| L38_blimp_ep00 | 9 |
| L39_blimp_ep07 | 9 |
| L40_blimp_ep27 | 9 |
| L41_blimp_ep28 | 9 |
| L42_blimp_ep29 | 9 |
| L43_blimp_ep26 | 9 |
| world_elf2A38A8 | 1 |

## Related docs

[engine/game-progression.md](../../engine/game-progression.md) (12), [formats/splice-scripts.md](../../formats/splice-scripts.md) (7), [engine/bosses.md](../../engine/bosses.md) (3), [engine/overview.md](../../engine/overview.md) (3), [engine/splice-scripting.md](../../engine/splice-scripting.md) (3), [open-questions.md](../../open-questions.md) (3), [guides/devtools.md](../../guides/devtools.md) (2), [reference/class-tree.md](../../reference/class-tree.md) (2)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
