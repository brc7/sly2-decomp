# SHAPE (CID 10, 0x0A)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> Lilly SHAPE : XFM ends 0x190; CID11/12/13 : SHAPE in Lilly port

| | |
|---|---|
| Name | `SHAPE` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [SHAPE](SHAPE.md) → [XFM](XFM.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | [PIPE](PIPE.md), [CID12](CID12.md), [RAIL](RAIL.md), [CID14](CID14.md) |
| Instance size (`cb`) | 0x190 (400 bytes), +0xB0 over XFM |
| Descriptor | `D_002BABB0` (record 10 of D_002BAAE8; cid range 10..15) |
| Factory (`pfnNew`) | [`NewShape`](../../../src/game/app/class_registry/NewShape.c) `0x110C98` |
| Descriptor getter (slot 6) | `GetShapeClassDesc` `0x113AB0` |
| vtable | `D_002D9040`, 51 entries (slots 0..50; slot 0 null), 0 new, 5 overridden |
| vtable stored by | [`NewShape`](../../../src/game/app/class_registry/NewShape.c) (vptr +0xC) |
| Level instances | 3140 objects in 42 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetShapeClassDesc`](../../../src/game/app/class_registry/GetShapeClassDesc.c) | `0x113AB0` | Pclassinfo |
| 7 | Init | **override** | [`InitShape`](../../../src/game/engine/physics/shape/InitShape.c) | `0x1E5D80` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | inherited (XFM) | `SetXfmParent` | `0x2315F0` | SetParent |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` | AddHierarchy |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` | RemoveHierarchy |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` | UpdateLoroot |
| 14 | LoadFromBrx | **override** | [`LoadShapeFromBrx`](../../../src/game/engine/physics/shape/LoadShapeFromBrx.c) | `0x1E5DF8` | LoadFromBrx |
| 15 | PostLoad | inherited (LO) | `PostLoLoad` | `0x1A5C48` | VUnk14 |
| 16 | Update | inherited (LO) | `UpdateLo` | `0x1A5E28` | VUnk15 |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | **override** | [`UpdateShapeXfWorld?`](../../../src/game/engine/physics/shape/func_001E6340.c) | `0x1E6340` | VUnk16 |
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
| 41 | Clone | **override** | [`CloneShape`](../../../src/game/engine/physics/shape/CloneShape.c) | `0x1E5DB8` | Clone |
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

From `include/sly2/xfm.h` (`struct SHAPE : XFM`). SHAPE: from src/game/gameplay/player/jt/UpdateJtLean__FP2JT.cpp (3 copies in Lilly ports)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 1 header asserts agree, 0 disagree. Size from header: 0x190.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0xE0 | `CRVK` | `crvk` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xE4 | `int` | `fClosed` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xE8 | `int` | `ccv` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xEC | `float *` | `mpicvu` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xF0 | `float *` | `mpicvs` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xF4 | `VECTOR4 *` | `mpicvpos` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xF8 | `undefined4[2]` | `unk_0xf8` | unknown (padding/placeholder) |  |
| 0x100 | `struct MATRIX4` | `matWorldCache` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x140 | `VECTOR4 *` | `mpicvposA` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x144 | `VECTOR4 *` | `mpicvposB` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x148 | `int` | `nUnk148` | unknown (padding/placeholder) |  |
| 0x14C | `float[11]` | `agsSeg` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x178 | `int` | `nUnk178` | unknown (padding/placeholder) |  |
| 0x17C | `void *` | `pvUnk17c` | unknown (padding/placeholder) |  |
| 0x180 | `struct SHAPERANGES *` | `pranges` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x184 | `undefined4[3]` | `unk_0x184` | unknown (padding/placeholder) |  |

Inherited fields: see [XFM](XFM.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 105 |
| L03_nightclub_ep02 | 28 |
| L04_nightclub_ep03 | 46 |
| L05_nightclub_ep04 | 42 |
| L06_nightclub_ep05 | 17 |
| L07_nightclub_ep06 | 14 |
| L08_palace_ep00 | 244 |
| L09_palace_ep07 | 56 |
| L10_palace_ep08 | 54 |
| L11_palace_ep09 | 21 |
| L12_temple_ep00 | 171 |
| L13_temple_ep07 | 65 |
| L14_prison_ep00 | 460 |
| L15_prison_ep07 | 92 |
| L16_prison_ep10 | 3 |
| L17_castle_ep00 | 444 |
| L19_castle_ep11 | 9 |
| L20_castle_ep12 | 11 |
| L21_castle_ep13 | 15 |
| L22_castle_ep04 | 41 |
| L23_castle_ep14 | 7 |
| L24_castle_ep15 | 7 |
| L25_castle_ep16 | 11 |
| L26_castle_ep17 | 4 |
| L27_train_ep00 | 330 |
| L28_train_ep18 | 21 |
| L29_train_ep19 | 44 |
| L30_train_ep20 | 45 |
| L31_train_ep21 | 40 |
| L32_sawmill_ep00 | 216 |
| L33_sawmill_ep22 | 14 |
| L34_sawmill_ep23 | 26 |
| L35_sawmill_ep24 | 39 |
| L36_sawmill_ep25 | 3 |
| L37_sawmill_ep26 | 5 |
| L38_blimp_ep00 | 211 |
| L39_blimp_ep07 | 11 |
| L40_blimp_ep27 | 7 |
| L41_blimp_ep28 | 5 |
| L42_blimp_ep29 | 45 |
| L43_blimp_ep26 | 32 |
| world_elf2A38A8 | 79 |

## Related docs

[reference/class-tree.md](../../reference/class-tree.md) (7), [engine/physics-collision.md](../../engine/physics-collision.md) (4), [engine/guards-ai.md](../../engine/guards-ai.md) (3), [engine/overview.md](../../engine/overview.md) (1), [formats/level-objects.md](../../formats/level-objects.md) (1), [formats/meshes-scene.md](../../formats/meshes-scene.md) (1), [reference/tu-map.md](../../reference/tu-map.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
