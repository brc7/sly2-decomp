# CID27 (CID 27, 0x1B)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> table grabspot (VOL + GRABSPOT; hide under)

| | |
|---|---|
| Name | `CID27` — placeholder (decimal CID) |
| Parent chain | [CID27](CID27.md) → [VOL](VOL.md) → [XFM](XFM.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x160 (352 bytes), +0x30 over VOL |
| Descriptor | `D_002BAD04` (record 27 of D_002BAAE8; cid range 27..28) |
| Factory (`pfnNew`) | [`NewCid27`](../../../src/hand/NewCid27.cpp) `0x111090` |
| Descriptor getter (slot 6) | `GetCid27ClassDesc` `0x113BC0` |
| vtable | `D_002D1B60`, 51 entries (slots 0..50; slot 0 null), 0 new, 5 overridden |
| vtable stored by | [`NewCid27`](../../../src/hand/NewCid27.cpp) (vptr +0xC) |
| Level instances | 159 objects in 28 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` |
| 6 | Pclassinfo | **override** | [`GetCid27ClassDesc`](../../../src/auto/GetCid27ClassDesc.c) | `0x113BC0` |
| 7 | Init | **override** | [`InitCid27?`](../../../src/game/gameplay/player/grabspot/func_001843C0.c) | `0x1843C0` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (XFM) | `SetXfmParent` | `0x2315F0` |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` |
| 14 | LoadFromBrx | inherited (VOL) | `LoadVolFromBrx` | `0x227BB8` |
| 15 | PostLoad | **override** | [`PostCid27Load?`](../../../src/game/gameplay/player/grabspot/func_001843F0.c) | `0x1843F0` |
| 16 | Update | inherited (LO) | `UpdateLo` | `0x1A5E28` |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (LO) | `LO_vt17_InheritFlag4?` | `0x1A5E80` |
| 18 | UpdateLoXfWorldHierarchy | inherited (LO) | `UpdateLoXfWorldHierarchy` | `0x22FBD8` |
| 19 | RenderSelf | inherited (LO) | `RenderLoSelf` | `0x1A5ED8` |
| 20 | RenderLoAll | inherited (LO) | `RenderLoAll?` | `0x1A5EE0` |
| 21 | SetLoFlag8 | inherited (LO) | `SetLoFlag8?` | `0x1A6E80` |
| 22 | OnRemove | **override** | [`OnCid27Remove?`](../../../src/game/gameplay/player/grabspot/func_001844D8.c) | `0x1844D8` |
| 23 | OnAdd | **override** | [`OnCid27Add?`](../../../src/game/gameplay/player/grabspot/func_001844A0.c) | `0x1844A0` |
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
| 41 | Clone | inherited (XFM) | `CloneXfm` | `0x2311B0` |
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

No struct for `CID27` in `include/sly2` yet. Size is known exactly (0x160); the bytes 0x130..0x160 are this class's own. Inherited fields: see [VOL](VOL.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 4 |
| L03_nightclub_ep02 | 8 |
| L04_nightclub_ep03 | 1 |
| L05_nightclub_ep04 | 1 |
| L06_nightclub_ep05 | 4 |
| L08_palace_ep00 | 4 |
| L09_palace_ep07 | 8 |
| L10_palace_ep08 | 6 |
| L12_temple_ep00 | 6 |
| L13_temple_ep07 | 8 |
| L14_prison_ep00 | 13 |
| L15_prison_ep07 | 9 |
| L16_prison_ep10 | 5 |
| L17_castle_ep00 | 15 |
| L20_castle_ep12 | 7 |
| L22_castle_ep04 | 4 |
| L23_castle_ep14 | 3 |
| L28_train_ep18 | 4 |
| L29_train_ep19 | 4 |
| L30_train_ep20 | 7 |
| L32_sawmill_ep00 | 4 |
| L33_sawmill_ep22 | 6 |
| L34_sawmill_ep23 | 6 |
| L35_sawmill_ep24 | 2 |
| L36_sawmill_ep25 | 2 |
| L38_blimp_ep00 | 8 |
| L39_blimp_ep07 | 6 |
| L40_blimp_ep27 | 4 |

## Related docs

[engine/player.md](../../engine/player.md) (3), [formats/level-objects.md](../../formats/level-objects.md) (1), [reference/class-tree.md](../../reference/class-tree.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
