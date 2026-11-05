# CID14 (CID 14, 0x0E)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> convex polygon SHAPE / pathzone obstacle

| | |
|---|---|
| Name | `CID14` — placeholder (decimal CID) |
| Parent chain | [CID14](CID14.md) → [SHAPE](SHAPE.md) → [XFM](XFM.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x1F0 (496 bytes), +0x60 over SHAPE |
| Descriptor | `D_002BAC00` (record 14 of D_002BAAE8; cid range 14..15) |
| Factory (`pfnNew`) | [`NewCid14`](../../../src/game/app/class_registry.cpp) `0x110DA8` |
| Descriptor getter (slot 6) | `GetCid14ClassDesc` `0x113AF0` |
| vtable | `D_002D4DE0`, 51 entries (slots 0..50; slot 0 null), 0 new, 7 overridden |
| vtable stored by | [`NewCid14`](../../../src/game/app/class_registry.cpp) (vptr +0xC) |
| Level instances | 179 objects in 36 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` |
| 6 | Pclassinfo | **override** | [`GetCid14ClassDesc`](../../../src/game/app/class_registry.cpp) | `0x113AF0` |
| 7 | Init | **override** | [`InitCid14?`](../../../src/game/engine/physics/tu_1BC050.cpp) | `0x1BC050` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (XFM) | `SetXfmParent` | `0x2315F0` |
| 11 | AddHierarchy | **override** | [`AddCid14Hierarchy?`](../../../src/game/engine/physics/tu_1BC050.cpp) | `0x1BC160` |
| 12 | RemoveHierarchy | **override** | [`RemoveCid14Hierarchy?`](../../../src/game/engine/physics/tu_1BC050.cpp) | `0x1BC198` |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` |
| 14 | LoadFromBrx | **override** | [`LoadCid14FromBrx?`](../../../src/game/engine/physics/tu_1BC050.cpp) | `0x1BC088` |
| 15 | PostLoad | **override** | [`PostCid14Load?`](../../../src/game/engine/physics/tu_1BC050.cpp) | `0x1BC570` |
| 16 | Update | inherited (LO) | `UpdateLo` | `0x1A5E28` |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (SHAPE) | `UpdateShapeXfWorld?` | `0x1E6340` |
| 18 | UpdateLoXfWorldHierarchy | inherited (LO) | `UpdateLoXfWorldHierarchy` | `0x22FBD8` |
| 19 | RenderSelf | inherited (LO) | `RenderLoSelf` | `0x1A5ED8` |
| 20 | RenderLoAll | inherited (LO) | `RenderLoAll?` | `0x1A5EE0` |
| 21 | SetLoFlag8 | inherited (LO) | `SetLoFlag8?` | `0x1A6E80` |
| 22 | OnRemove | inherited (LO) | `OnLoRemove` | `0x1A6150` |
| 23 | OnAdd | inherited (LO) | `OnLoAdd` | `0x1A6178` |
| 24 |  | inherited (LO) | `func_00210330` | `0x210330` |
| 25 | SetIdProxy | inherited (LO) | `SetLoIdProxy` | `0x1CA118` |
| 26 | ResetLoForProxy | inherited (LO) | `ResetLoForProxy?` | `0x1CB4E0` |
| 27 | LO_VUnk27Proxy | inherited (LO) | `LO_VUnk27Proxy?` | `0x1CB5C8` |
| 28 | CploChildren | inherited (LO) | `LO_CploChildren` | `0x1A5BE8` |
| 29 | TranslateToPos | inherited (XFM) | `TranslateXfmToPos` | `0x2312A0` |
| 30 | RotateToMat | inherited (XFM) | `RotateXfmToMat` | `0x231300` |
| 31 | PmatWorld | inherited (XFM) | `XFM_PmatWorld` | `0x231440` |
| 32 | LO_PxfLocal | inherited (XFM) | `XFM_PxfLocal?` | `0x231550` |
| 33 | GetVelocity | inherited (XFM) | `GetXfmVelocity?` | `0x231468` |
| 34 | GetAngularVelocity | inherited (XFM) | `GetXfmAngularVelocity?` | `0x2314D0` |
| 35 | SetVelocity | inherited (LO) | `SetLoVelocity?` | `0x1A6130` |
| 36 | SetAngularVelocity | inherited (LO) | `SetLoAngularVelocity` | `0x1A6148` |
| 37 | ApplyLoProxyHierarchy | inherited (XFM) | `ApplyXfmProxy?` | `0x231598` |
| 38 | MatchLoXf | inherited (LO) | `MatchLoXf?` | `0x1A6B88` |
| 39 | SetLoXf | inherited (LO) | `SetLoXf?` | `0x1A6BC8` |
| 40 | CloneHierarchy | inherited (LO) | `CloneLoHierarchy` | `0x1A6AD0` |
| 41 | Clone | **override** | [`CloneCid14?`](../../../src/game/engine/physics/tu_1BC050.cpp) | `0x1BC0D0` |
| 42 | SetLoSBusy | inherited (LO) | `SetLoSBusy?` | `0x1A6C48` |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` |
| 46 | SetVisMask | inherited (LO) | `SetLoVisMask?` | `0x1A6DC0` |
| 47 | SetGrf | inherited (LO) | `SetLoGrf?` | `0x1A66C8` |
| 48 | ClearLoGrf | inherited (LO) | `ClearLoGrf?` | `0x1A66F8` |
| 49 | XFM_PxfFromId | inherited (XFM) | `XFM_PxfFromId?` | `0x22D750` |
| 50 |  | inherited (XFM) | `func_00231220` | `0x231220` |

## Fields

No struct for `CID14` in `include/sly2` yet. Size is known exactly (0x1F0); the bytes 0x190..0x1F0 are this class's own. Inherited fields: see [SHAPE](SHAPE.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 23 |
| L03_nightclub_ep02 | 7 |
| L04_nightclub_ep03 | 9 |
| L05_nightclub_ep04 | 8 |
| L06_nightclub_ep05 | 2 |
| L07_nightclub_ep06 | 6 |
| L08_palace_ep00 | 4 |
| L09_palace_ep07 | 6 |
| L10_palace_ep08 | 6 |
| L11_palace_ep09 | 4 |
| L12_temple_ep00 | 3 |
| L13_temple_ep07 | 9 |
| L14_prison_ep00 | 5 |
| L15_prison_ep07 | 2 |
| L16_prison_ep10 | 1 |
| L17_castle_ep00 | 6 |
| L20_castle_ep12 | 2 |
| L22_castle_ep04 | 2 |
| L23_castle_ep14 | 1 |
| L27_train_ep00 | 7 |
| L28_train_ep18 | 6 |
| L29_train_ep19 | 5 |
| L30_train_ep20 | 6 |
| L31_train_ep21 | 6 |
| L32_sawmill_ep00 | 9 |
| L33_sawmill_ep22 | 3 |
| L34_sawmill_ep23 | 4 |
| L35_sawmill_ep24 | 2 |
| L37_sawmill_ep26 | 1 |
| L38_blimp_ep00 | 7 |
| L39_blimp_ep07 | 4 |
| L40_blimp_ep27 | 3 |
| L41_blimp_ep28 | 1 |
| L42_blimp_ep29 | 1 |
| L43_blimp_ep26 | 4 |
| world_elf2A38A8 | 4 |

## Related docs

[formats/level-objects.md](../../formats/level-objects.md) (2), [engine/physics-collision.md](../../engine/physics-collision.md) (1), [reference/class-tree.md](../../reference/class-tree.md) (1), [reference/tu-map.md](../../reference/tu-map.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
