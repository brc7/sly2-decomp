# TASK (CID 133, 0x85)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> TASK (save t<N>_<name>)

| | |
|---|---|
| Name | `TASK` — medium: team coinage (v1 identified) |
| Parent chain | [TASK](TASK.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x110 (272 bytes), +0xC0 over LO |
| Descriptor | `D_002BB54C` (record 133 of D_002BAAE8; cid range 133..134) |
| Factory (`pfnNew`) | [`NewTask`](../../../src/game/app/class_registry.cpp) `0x112D70` |
| Descriptor getter (slot 6) | `GetTaskClassDesc` `0x114260` |
| vtable | `D_002CF1E8`, 49 entries (slots 0..48; slot 0 null), 0 new, 5 overridden |
| vtable stored by | [`NewTask`](../../../src/game/app/class_registry.cpp) (vptr +0xC) |
| Level instances | 4080 objects in 42 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetTaskClassDesc`](../../../src/game/app/class_registry/GetTaskClassDesc.c) | `0x114260` | Pclassinfo |
| 7 | Init | **override** | [`InitTask`](../../../src/game/world/state/game/InitTask.c) | `0x1635A0` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | inherited (LO) | `SetLoParent?` | `0x1A5900` | SetParent |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` | AddHierarchy |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` | RemoveHierarchy |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` | UpdateLoroot |
| 14 | LoadFromBrx | **override** | [`LoadTaskFromBrx`](../../../src/game/world/state/game/LoadTaskFromBrx.cpp) | `0x1635F8` | LoadFromBrx |
| 15 | PostLoad | **override** | [`PostTaskLoad`](../../../src/game/world/state/game/PostTaskLoad.c) | `0x1638F8` | VUnk14 |
| 16 | Update | **override** | [`UpdateTask`](../../../src/game/world/state/game/UpdateTask.c) | `0x163B48` | VUnk15 |
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

From `include/sly2/task.h` (`struct TASK : LO`). CONFIRMED runtime

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 4 header asserts agree, 0 disagree. Size from header: 0x110.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x50 | `undefined4` | `unk_0x50` | unknown (padding/placeholder) |  |
| 0x54 | `int` | `nState` | asserted (CONFIRMED runtime) | +0x54 [R] -1 unset, 0 locked, 1 available, 2 done (in job), 3 done |
| 0x58 | `float` | `tState` | header (Lilly-ported; offset computed, unasserted) | +0x58 [R] clock.t when nState was entered |
| 0x5C | `undefined4[8]` | `unk_0x5c` | unknown (padding/placeholder) |  |
| 0x7C | `JOB *` | `pjob` | asserted (CONFIRMED runtime) | +0x7C [R] owning JOB |
| 0x80 | `undefined4[5]` | `unk_0x80` | unknown (padding/placeholder) |  |
| 0x94 | `int` | `cptaskPrereq` | header (Lilly-ported; offset computed, unasserted) | +0x94 [R] prerequisite tasks |
| 0x98 | `TASK **` | `aptaskPrereq` | header (Lilly-ported; offset computed, unasserted) | +0x98 [R] |
| 0x9C | `undefined4` | `unk_0x9c` | unknown (padding/placeholder) |  |
| 0xA0 | `int` | `cptaskSucc` | header (Lilly-ported; offset computed, unasserted) | +0xA0 [R] successor tasks |
| 0xA4 | `TASK **` | `aptaskSucc` | asserted (CONFIRMED runtime) | +0xA4 [R] |
| 0xA8 | `undefined4[6]` | `unk_0xa8` | unknown (padding/placeholder) |  |
| 0xC0 | `int` | `nCharacter` | header (Lilly-ported; offset computed, unasserted) | +0xC0 [R] character the task is played with |
| 0xC4 | `undefined4[12]` | `unk_0xc4` | unknown (padding/placeholder) | +0xC8..+0xE8: {kind, prec} save-name references (one of them names the task) |
| 0xF4 | `int` | `nMode` | header (Lilly-ported; offset computed, unasserted) | +0xF4 [R] |
| 0xF8 | `undefined4[6]` | `unk_0xf8` | unknown (padding/placeholder) |  |

Inherited fields: see [LO](LO.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 99 |
| L03_nightclub_ep02 | 99 |
| L04_nightclub_ep03 | 99 |
| L05_nightclub_ep04 | 99 |
| L06_nightclub_ep05 | 99 |
| L07_nightclub_ep06 | 99 |
| L08_palace_ep00 | 93 |
| L09_palace_ep07 | 93 |
| L10_palace_ep08 | 93 |
| L11_palace_ep09 | 93 |
| L12_temple_ep00 | 82 |
| L13_temple_ep07 | 82 |
| L14_prison_ep00 | 110 |
| L15_prison_ep07 | 110 |
| L16_prison_ep10 | 110 |
| L17_castle_ep00 | 102 |
| L19_castle_ep11 | 102 |
| L20_castle_ep12 | 102 |
| L21_castle_ep13 | 102 |
| L22_castle_ep04 | 102 |
| L23_castle_ep14 | 102 |
| L24_castle_ep15 | 102 |
| L25_castle_ep16 | 102 |
| L26_castle_ep17 | 102 |
| L27_train_ep00 | 72 |
| L28_train_ep18 | 72 |
| L29_train_ep19 | 72 |
| L30_train_ep20 | 72 |
| L31_train_ep21 | 72 |
| L32_sawmill_ep00 | 122 |
| L33_sawmill_ep22 | 122 |
| L34_sawmill_ep23 | 122 |
| L35_sawmill_ep24 | 122 |
| L36_sawmill_ep25 | 122 |
| L37_sawmill_ep26 | 122 |
| L38_blimp_ep00 | 99 |
| L39_blimp_ep07 | 99 |
| L40_blimp_ep27 | 99 |
| L41_blimp_ep28 | 99 |
| L42_blimp_ep29 | 99 |
| L43_blimp_ep26 | 99 |
| world_elf2A38A8 | 16 |

## Related docs

[engine/game-progression.md](../../engine/game-progression.md) (19), [formats/splice-scripts.md](../../formats/splice-scripts.md) (8), [engine/splice-scripting.md](../../engine/splice-scripting.md) (4), [open-questions.md](../../open-questions.md) (4), [engine/bosses.md](../../engine/bosses.md) (3), [engine/overview.md](../../engine/overview.md) (3), [guides/devtools.md](../../guides/devtools.md) (3), [reference/save-variables.md](../../reference/save-variables.md) (3)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
