# CID36 (CID 36, 0x24)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> carry grabspot (child of CID35)

| | |
|---|---|
| Name | `CID36` — placeholder (decimal CID) |
| Parent chain | [CID36](CID36.md) → [CID35](CID35.md) → [XFM](XFM.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x1A0 (416 bytes), +0x20 over CID35 |
| Descriptor | `D_002BADB8` (record 36 of D_002BAAE8; cid range 36..37) |
| Factory (`pfnNew`) | [`NewCid36`](../../../src/game/app/class_registry.cpp) `0x1112A8` |
| Descriptor getter (slot 6) | `GetCid36ClassDesc` `0x113C50` |
| vtable | `D_002D1D00`, 52 entries (slots 0..51; slot 0 null), 0 new, 4 overridden |
| vtable stored by | [`NewCid36`](../../../src/game/app/class_registry.cpp) (vptr +0xC) |
| Level instances | 204 objects in 40 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` |
| 6 | Pclassinfo | **override** | [`GetCid36ClassDesc`](../../../src/game/app/class_registry.cpp) | `0x113C50` |
| 7 | Init | **override** | [`InitCid36?`](../../../src/game/gameplay/player/grabspot.cpp) | `0x184268` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (XFM) | `SetXfmParent` | `0x2315F0` |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` |
| 14 | LoadFromBrx | inherited (XFM) | `LoadXfmFromBrx` | `0x231170` |
| 15 | PostLoad | **override** | [`PostCid36Load?`](../../../src/game/gameplay/player/grabspot.cpp) | `0x1842B0` |
| 16 | Update | inherited (LO) | `UpdateLo` | `0x1A5E28` |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (LO) | `LO_vt17_InheritFlag4?` | `0x1A5E80` |
| 18 | UpdateLoXfWorldHierarchy | inherited (LO) | `UpdateLoXfWorldHierarchy` | `0x22FBD8` |
| 19 | RenderSelf | inherited (LO) | `RenderLoSelf` | `0x1A5ED8` |
| 20 | RenderLoAll | inherited (LO) | `RenderLoAll?` | `0x1A5EE0` |
| 21 | SetLoFlag8 | inherited (LO) | `SetLoFlag8?` | `0x1A6E80` |
| 22 | OnRemove | inherited (CID35) | `OnCid35Remove?` | `0x183FF0` |
| 23 | OnAdd | inherited (CID35) | `OnCid35Add?` | `0x183FB8` |
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
| 51 | TriggerCid35 | **override** | [`Cid35OnGrab?`](../../../src/game/gameplay/player/grabspot.cpp) | `0x184330` |

## Fields

No struct for `CID36` in `include/sly2` yet. Size is known exactly (0x1A0); the bytes 0x180..0x1A0 are this class's own. Inherited fields: see [CID35](CID35.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 15 |
| L03_nightclub_ep02 | 11 |
| L04_nightclub_ep03 | 6 |
| L05_nightclub_ep04 | 1 |
| L06_nightclub_ep05 | 5 |
| L07_nightclub_ep06 | 8 |
| L08_palace_ep00 | 8 |
| L09_palace_ep07 | 9 |
| L10_palace_ep08 | 8 |
| L11_palace_ep09 | 6 |
| L12_temple_ep00 | 9 |
| L13_temple_ep07 | 12 |
| L14_prison_ep00 | 3 |
| L15_prison_ep07 | 5 |
| L17_castle_ep00 | 8 |
| L19_castle_ep11 | 1 |
| L20_castle_ep12 | 2 |
| L21_castle_ep13 | 1 |
| L22_castle_ep04 | 1 |
| L23_castle_ep14 | 1 |
| L24_castle_ep15 | 1 |
| L25_castle_ep16 | 1 |
| L26_castle_ep17 | 1 |
| L27_train_ep00 | 11 |
| L28_train_ep18 | 6 |
| L29_train_ep19 | 7 |
| L30_train_ep20 | 6 |
| L31_train_ep21 | 7 |
| L32_sawmill_ep00 | 10 |
| L33_sawmill_ep22 | 2 |
| L34_sawmill_ep23 | 3 |
| L35_sawmill_ep24 | 2 |
| L37_sawmill_ep26 | 1 |
| L38_blimp_ep00 | 8 |
| L39_blimp_ep07 | 3 |
| L40_blimp_ep27 | 5 |
| L41_blimp_ep28 | 1 |
| L42_blimp_ep29 | 1 |
| L43_blimp_ep26 | 4 |
| world_elf2A38A8 | 4 |

## Related docs

[engine/gameplay-objects.md](../../engine/gameplay-objects.md) (1), [reference/class-tree.md](../../reference/class-tree.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
