# HND (CID 19, 0x13)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> HND (hook; REVIEW proposes HND)

| | |
|---|---|
| Name | `HND` — medium-high: team coinage (v3 identified (medium-high)) |
| Parent chain | [HND](HND.md) → [TARGET](TARGET.md) → [XFM](XFM.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x140 (320 bytes), +0x40 over TARGET |
| Descriptor | `D_002BAC64` (record 19 of D_002BAAE8; cid range 19..20) |
| Factory (`pfnNew`) | [`NewHnd`](../../../src/game/app/class_registry/NewHnd.cpp) `0x110EC0` |
| Descriptor getter (slot 6) | `GetHndClassDesc` `0x113B40` |
| vtable | `D_002D2978`, 51 entries (slots 0..50; slot 0 null), 0 new, 2 overridden |
| vtable stored by | [`NewHnd`](../../../src/game/app/class_registry/NewHnd.cpp) (vptr +0xC) |
| Level instances | 74 objects in 12 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` |
| 6 | Pclassinfo | **override** | [`GetHndClassDesc`](../../../src/game/app/class_registry/GetHndClassDesc.c) | `0x113B40` |
| 7 | Init | **override** | [`InitHnd`](../../../src/game/gameplay/player/hnd/InitHnd.c) | `0x184F50` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (XFM) | `SetXfmParent` | `0x2315F0` |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` |
| 14 | LoadFromBrx | inherited (XFM) | `LoadXfmFromBrx` | `0x231170` |
| 15 | PostLoad | inherited (TARGET) | `PostTargetLoad` | `0x21D2C8` |
| 16 | Update | inherited (LO) | `UpdateLo` | `0x1A5E28` |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (LO) | `LO_vt17_InheritFlag4?` | `0x1A5E80` |
| 18 | UpdateLoXfWorldHierarchy | inherited (LO) | `UpdateLoXfWorldHierarchy` | `0x22FBD8` |
| 19 | RenderSelf | inherited (LO) | `RenderLoSelf` | `0x1A5ED8` |
| 20 | RenderLoAll | inherited (LO) | `RenderLoAll?` | `0x1A5EE0` |
| 21 | SetLoFlag8 | inherited (LO) | `SetLoFlag8?` | `0x1A6E80` |
| 22 | OnRemove | inherited (TARGET) | `OnTargetRemove` | `0x21D320` |
| 23 | OnAdd | inherited (TARGET) | `OnTargetAdd` | `0x21D358` |
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
| 41 | Clone | inherited (TARGET) | `CloneTarget` | `0x21D390` |
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

No struct for `HND` in `include/sly2` yet. Size is known exactly (0x140); the bytes 0x100..0x140 are this class's own. Inherited fields: see [TARGET](TARGET.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L14_prison_ep00 | 1 |
| L15_prison_ep07 | 1 |
| L17_castle_ep00 | 1 |
| L19_castle_ep11 | 1 |
| L27_train_ep00 | 20 |
| L28_train_ep18 | 1 |
| L30_train_ep20 | 4 |
| L32_sawmill_ep00 | 40 |
| L33_sawmill_ep22 | 1 |
| L34_sawmill_ep23 | 1 |
| L36_sawmill_ep25 | 2 |
| L38_blimp_ep00 | 1 |

## Related docs

[open-questions.md](../../open-questions.md) (4), [reference/class-tree.md](../../reference/class-tree.md) (4), [engine/player.md](../../engine/player.md) (3), [engine/gameplay-objects.md](../../engine/gameplay-objects.md) (1), [reference/glossary.md](../../reference/glossary.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
