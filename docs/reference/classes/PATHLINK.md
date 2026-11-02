# PATHLINK (CID 124, 0x7C)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> PATHLINK (BHVPATHLINK walks it; REVIEW)

| | |
|---|---|
| Name | `PATHLINK` — medium-high: team coinage (v3 identified (medium-high)) |
| Parent chain | [PATHLINK](PATHLINK.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x160 (352 bytes), +0x110 over LO |
| Descriptor | `D_002BB498` (record 124 of D_002BAAE8; cid range 124..125) |
| Factory (`pfnNew`) | [`NewPathlink`](../../../src/hand/NewPathlink.c) `0x112B30` |
| Descriptor getter (slot 6) | `GetPathlinkClassDesc` `0x1141D0` |
| vtable | `D_002D5120`, 49 entries (slots 0..48; slot 0 null), 0 new, 5 overridden |
| vtable stored by | [`NewPathlink`](../../../src/hand/NewPathlink.c) (vptr +0xC) |
| Level instances | 2429 objects in 33 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` |
| 6 | Pclassinfo | **override** | [`GetPathlinkClassDesc`](../../../src/auto/GetPathlinkClassDesc.c) | `0x1141D0` |
| 7 | Init | **override** | [`InitPathlink`](../../../src/game/engine/physics/path/InitPathlink.c) | `0x1C2048` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (LO) | `SetLoParent?` | `0x1A5900` |
| 11 | AddHierarchy | **override** | [`AddPathlinkHierarchy`](../../../src/game/engine/physics/path/AddPathlinkHierarchy.c) | `0x1C2078` |
| 12 | RemoveHierarchy | **override** | [`RemovePathlinkHierarchy`](../../../src/game/engine/physics/path/RemovePathlinkHierarchy.c) | `0x1C20B0` |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` |
| 14 | LoadFromBrx | inherited (LO) | `LoadLoFromBrx` | `0x1A5C28` |
| 15 | PostLoad | **override** | [`PostPathlinkLoad`](../../../src/game/engine/physics/path/PostPathlinkLoad.c) | `0x1C2170` |
| 16 | Update | inherited (LO) | `UpdateLo` | `0x1A5E28` |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (LO) | `LO_vt17_InheritFlag4?` | `0x1A5E80` |
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
| 29 | TranslateToPos | inherited (LO) | `TranslateLoToPos` | `0x1A6110` |
| 30 | RotateToMat | inherited (LO) | `RotateLoToMat` | `0x1A6118` |
| 31 | PmatWorld | inherited (LO) | `LO_PmatWorld` | `0x1A62D8` |
| 32 | LO_PxfLocal | inherited (LO) | `LO_PxfLocal?` | `0x1A6B30` |
| 33 | GetVelocity | inherited (LO) | `GetLoVelocity?` | `0x1A6120` |
| 34 | GetAngularVelocity | inherited (LO) | `GetLoAngularVelocity` | `0x1A6138` |
| 35 | SetVelocity | inherited (LO) | `SetLoVelocity?` | `0x1A6130` |
| 36 | SetAngularVelocity | inherited (LO) | `SetLoAngularVelocity` | `0x1A6148` |
| 37 | ApplyLoProxyHierarchy | inherited (LO) | `ApplyLoProxyHierarchy?` | `0x1A6450` |
| 38 | MatchLoXf | inherited (LO) | `MatchLoXf?` | `0x1A6B88` |
| 39 | SetLoXf | inherited (LO) | `SetLoXf?` | `0x1A6BC8` |
| 40 | CloneHierarchy | inherited (LO) | `CloneLoHierarchy` | `0x1A6AD0` |
| 41 | Clone | inherited (LO) | `CloneLo` | `0x1A6730` |
| 42 | SetLoSBusy | inherited (LO) | `SetLoSBusy?` | `0x1A6C48` |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` |
| 46 | SetVisMask | inherited (LO) | `SetLoVisMask?` | `0x1A6DC0` |
| 47 | SetGrf | inherited (LO) | `SetLoGrf?` | `0x1A66C8` |
| 48 | ClearLoGrf | inherited (LO) | `ClearLoGrf?` | `0x1A66F8` |

## Fields

No struct for `PATHLINK` in `include/sly2` yet. Size is known exactly (0x160); the bytes 0x50..0x160 are this class's own. Inherited fields: see [LO](LO.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 262 |
| L03_nightclub_ep02 | 6 |
| L04_nightclub_ep03 | 23 |
| L05_nightclub_ep04 | 44 |
| L06_nightclub_ep05 | 8 |
| L07_nightclub_ep06 | 9 |
| L08_palace_ep00 | 313 |
| L09_palace_ep07 | 43 |
| L10_palace_ep08 | 36 |
| L12_temple_ep00 | 246 |
| L13_temple_ep07 | 8 |
| L14_prison_ep00 | 329 |
| L15_prison_ep07 | 58 |
| L17_castle_ep00 | 194 |
| L19_castle_ep11 | 4 |
| L23_castle_ep14 | 2 |
| L24_castle_ep15 | 2 |
| L25_castle_ep16 | 6 |
| L26_castle_ep17 | 4 |
| L27_train_ep00 | 323 |
| L28_train_ep18 | 8 |
| L29_train_ep19 | 7 |
| L31_train_ep21 | 2 |
| L32_sawmill_ep00 | 213 |
| L33_sawmill_ep22 | 6 |
| L34_sawmill_ep23 | 6 |
| L35_sawmill_ep24 | 35 |
| L37_sawmill_ep26 | 6 |
| L38_blimp_ep00 | 219 |
| L39_blimp_ep07 | 1 |
| L40_blimp_ep27 | 2 |
| L41_blimp_ep28 | 2 |
| L42_blimp_ep29 | 2 |

## Related docs

[reference/class-tree.md](../../reference/class-tree.md) (2), [formats/level-objects.md](../../formats/level-objects.md) (1), [open-questions.md](../../open-questions.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
