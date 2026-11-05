# BLIPG (CID 28, 0x1C)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> own code TU 0x138F08 right after blend (alphabetical "blip")

| | |
|---|---|
| Name | `BLIPG` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [BLIPG](BLIPG.md) → [XFM](XFM.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x460 (1120 bytes), +0x380 over XFM |
| Descriptor | `D_002BAD18` (record 28 of D_002BAAE8; cid range 28..29) |
| Factory (`pfnNew`) | [`NewBlipg`](../../../src/game/app/class_registry.cpp) `0x1110D8` |
| Descriptor getter (slot 6) | `GetBlipgClassDesc` `0x113BD0` |
| vtable | `D_002CB5D8`, 52 entries (slots 0..51; slot 0 null), 1 new, 7 overridden |
| vtable stored by | [`NewBlipg`](../../../src/game/app/class_registry.cpp) (vptr +0xC), [`PblipgNew`](../../../src/game/engine/fx/blip.cpp) (vptr +0xC) |
| Level instances | none (not a world-file object class) |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | **override** | [`DeleteBlipg?`](../../../src/game/engine/fx/blip.cpp) | `0x139318` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` |
| 6 | Pclassinfo | **override** | [`GetBlipgClassDesc`](../../../src/game/app/class_registry/GetBlipgClassDesc.c) | `0x113BD0` |
| 7 | Init | **override** | [`InitBlipg__FP5BLIPG`](../../../src/game/engine/fx/blip.cpp) | `0x1391C8` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (XFM) | `SetXfmParent` | `0x2315F0` |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` |
| 12 | RemoveHierarchy | **override** | [`OnBlipgRemove__FP5BLIPG`](../../../src/game/engine/fx/blip.cpp) | `0x139250` |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` |
| 14 | LoadFromBrx | inherited (XFM) | `LoadXfmFromBrx` | `0x231170` |
| 15 | PostLoad | inherited (LO) | `PostLoLoad` | `0x1A5C48` |
| 16 | Update | **override** | [`UpdateBlipg`](../../../src/game/engine/fx/blip.cpp) | `0x1398E0` |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | **override** | [`ProjectBlipgTransform`](../../../src/game/engine/fx/blip.cpp) | `0x139DC0` |
| 18 | UpdateLoXfWorldHierarchy | inherited (LO) | `UpdateLoXfWorldHierarchy` | `0x22FBD8` |
| 19 | RenderSelf | **override** | [`RenderBlipgSelf`](../../../src/game/engine/fx/blip.cpp) | `0x13A1E8` |
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
| 51 | SubscribeBlipgObject | **new** | [`SubscribeBlipgObject`](../../../src/game/engine/fx/blip.cpp) | `0x139900` |

## Fields

No struct for `BLIPG` in `include/sly2` yet. Size is known exactly (0x460); the bytes 0xE0..0x460 are this class's own. Inherited fields: see [XFM](XFM.md).

## Related docs

[engine/particles-effects.md](../../engine/particles-effects.md) (6), [engine/object-model.md](../../engine/object-model.md) (1), [engine/overview.md](../../engine/overview.md) (1), [formats/level-objects.md](../../formats/level-objects.md) (1), [reference/class-tree.md](../../reference/class-tree.md) (1), [reference/glossary.md](../../reference/glossary.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
