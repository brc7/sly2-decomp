# TARGET (CID 18, 0x12)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> Lilly TARGET : XFM ends 0x100; CID19 : TARGET

| | |
|---|---|
| Name | `TARGET` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [TARGET](TARGET.md) → [XFM](XFM.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | [HND](HND.md) |
| Instance size (`cb`) | 0x100 (256 bytes), +0x20 over XFM |
| Descriptor | `D_002BAC50` (record 18 of D_002BAAE8; cid range 18..20) |
| Factory (`pfnNew`) | [`NewTarget`](../../../src/auto/NewTarget.c) `0x110E88` |
| Descriptor getter (slot 6) | `GetTargetClassDesc` `0x113B30` |
| vtable | `D_002DB518`, 51 entries (slots 0..50; slot 0 null), 0 new, 6 overridden |
| vtable stored by | [`NewTarget`](../../../src/auto/NewTarget.c) (vptr +0xC) |
| Level instances | 102 objects in 38 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetTargetClassDesc`](../../../src/auto/GetTargetClassDesc.c) | `0x113B30` | Pclassinfo |
| 7 | Init | **override** | [`InitTarget`](../../../src/auto/InitTarget.c) | `0x21D278` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | inherited (XFM) | `SetXfmParent` | `0x2315F0` | SetParent |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` | AddHierarchy |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` | RemoveHierarchy |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` | UpdateLoroot |
| 14 | LoadFromBrx | inherited (XFM) | `LoadXfmFromBrx` | `0x231170` | LoadFromBrx |
| 15 | PostLoad | **override** | [`PostTargetLoad`](../../../src/hand/PostTargetLoad.c) | `0x21D2C8` | VUnk14 |
| 16 | Update | inherited (LO) | `UpdateLo` | `0x1A5E28` | VUnk15 |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (LO) | `LO_vt17_InheritFlag4?` | `0x1A5E80` | VUnk16 |
| 18 | UpdateLoXfWorldHierarchy | inherited (LO) | `UpdateLoXfWorldHierarchy` | `0x22FBD8` | UpdateXfWorldHierarchy |
| 19 | RenderSelf | inherited (LO) | `RenderLoSelf` | `0x1A5ED8` | RenderSelf |
| 20 | RenderLoAll | inherited (LO) | `RenderLoAll?` | `0x1A5EE0` | VUnk19 |
| 21 | SetLoFlag8 | inherited (LO) | `SetLoFlag8?` | `0x1A6E80` | VUnk20 |
| 22 | OnRemove | **override** | [`OnTargetRemove`](../../../src/auto/OnTargetRemove.c) | `0x21D320` | Freeze |
| 23 | OnAdd | **override** | [`OnTargetAdd`](../../../src/auto/OnTargetAdd.c) | `0x21D358` | Unfreeze |
| 24 |  | inherited (LO) | `func_00210330` | `0x210330` | VUnk23 |
| 25 | SetIdProxy | inherited (LO) | `SetLoIdProxy` | `0x1CA118` | SetIdProxy |
| 26 | ResetLoForProxy | inherited (LO) | `ResetLoForProxy?` | `0x1CB4E0` | VUnk25 |
| 27 | LO_VUnk27Proxy | inherited (LO) | `LO_VUnk27Proxy?` | `0x1CB5C8` | VUnk26 |
| 28 | CploChildren | inherited (LO) | `LO_CploChildren` | `0x1A5BE8` | CploChildren |
| 29 | TranslateToPos | inherited (XFM) | `TranslateXfmToPos` | `0x2312A0` | TranslateToPos |
| 30 | RotateToMat | inherited (XFM) | `RotateXfmToMat` | `0x231300` | RotateToMat |
| 31 | PmatWorld | inherited (XFM) | `XFM_PmatWorld` | `0x231440` | PmatWorld |
| 32 | LO_PxfLocal | inherited (XFM) | `XFM_PxfLocal?` | `0x231550` | PxfLocal |
| 33 | GetVelocity | inherited (XFM) | `GetXfmVelocity?` | `0x231468` | GetVelocity |
| 34 | GetAngularVelocity | inherited (XFM) | `GetXfmAngularVelocity?` | `0x2314D0` | GetAngularVelocity |
| 35 | SetVelocity | inherited (LO) | `SetLoVelocity?` | `0x1A6130` | SetVelocity |
| 36 | SetAngularVelocity | inherited (LO) | `SetLoAngularVelocity` | `0x1A6148` | SetAngularVelocity |
| 37 | ApplyLoProxyHierarchy | inherited (XFM) | `ApplyXfmProxy?` | `0x231598` | ApplyProxy |
| 38 | MatchLoXf | inherited (LO) | `MatchLoXf?` | `0x1A6B88` | VUnk37 |
| 39 | SetLoXf | inherited (LO) | `SetLoXf?` | `0x1A6BC8` | VUnk38 |
| 40 | CloneHierarchy | inherited (LO) | `CloneLoHierarchy` | `0x1A6AD0` | CloneHierarchy |
| 41 | Clone | **override** | [`CloneTarget`](../../../src/hand/CloneTarget.c) | `0x21D390` | Clone |
| 42 | SetLoSBusy | inherited (LO) | `SetLoSBusy?` | `0x1A6C48` | VUnk41 |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` | VUnk42 |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` | VUnk43 |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` | VUnk44 |
| 46 | SetVisMask | inherited (LO) | `SetLoVisMask?` | `0x1A6DC0` | SetVisMask |
| 47 | SetGrf | inherited (LO) | `SetLoGrf?` | `0x1A66C8` | VUnk46 |
| 48 | ClearLoGrf | inherited (LO) | `ClearLoGrf?` | `0x1A66F8` | VUnk47 |
| 49 | XFM_PxfFromId | inherited (XFM) | `XFM_PxfFromId?` | `0x22D750` | VSlot49 |
| 50 |  | inherited (XFM) | `func_00231220` | `0x231220` | VSlot50 |

## Fields

From `include/sly2/xfm.h` (`struct TARGET : XFM`). TARGET: from src/hand/UpdateJtLean__FP2JT.cpp (5 copies in Lilly ports)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 1 header asserts agree, 0 disagree. Size from header: 0x100.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0xE0 | `struct DLE` | `dleTarget` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xE8 | `int` | `grftak` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xEC | `float` | `sRadiusTarget` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xF0 | `int` | `fHitTest` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xF4 | `int` | `nUnkF4` | unknown (padding/placeholder) |  |
| 0xF8 | `uint32_t[2]` | `_pad_22` | header (Lilly-ported; offset computed, unasserted) |  |

Inherited fields: see [XFM](XFM.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 7 |
| L03_nightclub_ep02 | 2 |
| L04_nightclub_ep03 | 1 |
| L05_nightclub_ep04 | 1 |
| L07_nightclub_ep06 | 3 |
| L08_palace_ep00 | 3 |
| L09_palace_ep07 | 4 |
| L10_palace_ep08 | 2 |
| L11_palace_ep09 | 6 |
| L12_temple_ep00 | 7 |
| L13_temple_ep07 | 3 |
| L14_prison_ep00 | 3 |
| L15_prison_ep07 | 5 |
| L17_castle_ep00 | 3 |
| L19_castle_ep11 | 1 |
| L20_castle_ep12 | 1 |
| L21_castle_ep13 | 1 |
| L22_castle_ep04 | 3 |
| L23_castle_ep14 | 1 |
| L24_castle_ep15 | 1 |
| L25_castle_ep16 | 1 |
| L26_castle_ep17 | 1 |
| L27_train_ep00 | 5 |
| L28_train_ep18 | 2 |
| L29_train_ep19 | 2 |
| L30_train_ep20 | 2 |
| L31_train_ep21 | 2 |
| L32_sawmill_ep00 | 6 |
| L33_sawmill_ep22 | 1 |
| L35_sawmill_ep24 | 3 |
| L37_sawmill_ep26 | 2 |
| L38_blimp_ep00 | 5 |
| L39_blimp_ep07 | 2 |
| L40_blimp_ep27 | 3 |
| L41_blimp_ep28 | 1 |
| L42_blimp_ep29 | 1 |
| L43_blimp_ep26 | 4 |
| world_elf2A38A8 | 1 |

## Related docs

[engine/object-model.md](../../engine/object-model.md) (2), [reference/class-tree.md](../../reference/class-tree.md) (2), [engine/gameplay-objects.md](../../engine/gameplay-objects.md) (1), [engine/overview.md](../../engine/overview.md) (1), [engine/player.md](../../engine/player.md) (1), [formats/level-objects.md](../../formats/level-objects.md) (1), [formats/meshes-scene.md](../../formats/meshes-scene.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
