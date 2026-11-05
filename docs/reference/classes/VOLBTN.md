# VOLBTN (CID 24, 0x18)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> VOLBTN (REVIEW, high)

| | |
|---|---|
| Name | `VOLBTN` — high: team coinage (v3 identified (high)) |
| Parent chain | [VOLBTN](VOLBTN.md) → [VOL](VOL.md) → [XFM](XFM.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | [CID25](CID25.md) |
| Instance size (`cb`) | 0x2D0 (720 bytes), +0x1A0 over VOL |
| Descriptor | `D_002BACC8` (record 24 of D_002BAAE8; cid range 24..26) |
| Factory (`pfnNew`) | [`NewVolbtn`](../../../src/game/app/class_registry.cpp) `0x110FE8` |
| Descriptor getter (slot 6) | `GetVolbtnClassDesc` `0x113B90` |
| vtable | `D_002CC7B0`, 51 entries (slots 0..50; slot 0 null), 0 new, 7 overridden |
| vtable stored by | [`NewVolbtn`](../../../src/game/app/class_registry.cpp) (vptr +0xC) |
| Level instances | 482 objects in 39 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` |
| 6 | Pclassinfo | **override** | [`GetVolbtnClassDesc`](../../../src/game/app/class_registry.cpp) | `0x113B90` |
| 7 | Init | **override** | [`InitVolbtn`](../../../src/game/gameplay/objects/button.cpp) | `0x1492F0` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (XFM) | `SetXfmParent` | `0x2315F0` |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` |
| 12 | RemoveHierarchy | **override** | [`RemoveVolbtnHierarchy?`](../../../src/game/gameplay/objects/button.cpp) | `0x149880` |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` |
| 14 | LoadFromBrx | **override** | [`LoadVolbtnFromBrx`](../../../src/game/gameplay/objects/button.cpp) | `0x149358` |
| 15 | PostLoad | **override** | [`PostVolbtnLoad`](../../../src/game/gameplay/objects/button.cpp) | `0x149398` |
| 16 | Update | **override** | [`UpdateVolbtn`](../../../src/game/gameplay/objects/button.cpp) | `0x149440` |
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
| 41 | Clone | **override** | [`CloneVolbtn`](../../../src/game/gameplay/objects/button.cpp) | `0x1493F8` |
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

No struct for `VOLBTN` in `include/sly2` yet. Size is known exactly (0x2D0); the bytes 0x130..0x2D0 are this class's own. Inherited fields: see [VOL](VOL.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 20 |
| L03_nightclub_ep02 | 13 |
| L04_nightclub_ep03 | 12 |
| L05_nightclub_ep04 | 10 |
| L06_nightclub_ep05 | 2 |
| L07_nightclub_ep06 | 5 |
| L08_palace_ep00 | 31 |
| L09_palace_ep07 | 9 |
| L10_palace_ep08 | 4 |
| L11_palace_ep09 | 2 |
| L12_temple_ep00 | 25 |
| L13_temple_ep07 | 20 |
| L14_prison_ep00 | 40 |
| L15_prison_ep07 | 13 |
| L16_prison_ep10 | 1 |
| L17_castle_ep00 | 54 |
| L19_castle_ep11 | 1 |
| L20_castle_ep12 | 4 |
| L21_castle_ep13 | 6 |
| L22_castle_ep04 | 8 |
| L23_castle_ep14 | 2 |
| L24_castle_ep15 | 3 |
| L25_castle_ep16 | 3 |
| L26_castle_ep17 | 3 |
| L27_train_ep00 | 27 |
| L29_train_ep19 | 8 |
| L30_train_ep20 | 11 |
| L31_train_ep21 | 4 |
| L32_sawmill_ep00 | 47 |
| L34_sawmill_ep23 | 4 |
| L35_sawmill_ep24 | 7 |
| L36_sawmill_ep25 | 1 |
| L38_blimp_ep00 | 17 |
| L39_blimp_ep07 | 3 |
| L40_blimp_ep27 | 4 |
| L41_blimp_ep28 | 5 |
| L42_blimp_ep29 | 4 |
| L43_blimp_ep26 | 19 |
| world_elf2A38A8 | 30 |

## Related docs

[engine/bosses.md](../../engine/bosses.md) (5), [reference/class-tree.md](../../reference/class-tree.md) (3), [engine/gameplay-objects.md](../../engine/gameplay-objects.md) (2), [engine/minigames.md](../../engine/minigames.md) (2), [engine/physics-collision.md](../../engine/physics-collision.md) (1), [formats/level-objects.md](../../formats/level-objects.md) (1), [open-questions.md](../../open-questions.md) (1), [reference/tu-map.md](../../reference/tu-map.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
