# WR (CID 115, 0x73)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> UpdateWrMatrixes in slot 16; wr TU 0x22B720; size 0x900 (Sly1 WR ~0x8F0)

| | |
|---|---|
| Name | `WR` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [WR](WR.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x900 (2304 bytes), +0x8B0 over LO |
| Descriptor | `D_002BB3E4` (record 115 of D_002BAAE8; cid range 115..116) |
| Factory (`pfnNew`) | [`NewWr`](../../../src/game/app/class_registry/NewWr.c) `0x112938` |
| Descriptor getter (slot 6) | `GetWrClassDesc` `0x114140` |
| vtable | `D_002DCF00`, 49 entries (slots 0..48; slot 0 null), 0 new, 3 overridden |
| vtable stored by | [`NewWr`](../../../src/game/app/class_registry/NewWr.c) (vptr +0xC) |
| Level instances | 49 objects in 20 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` |
| 6 | Pclassinfo | **override** | [`GetWrClassDesc`](../../../src/game/app/class_registry/GetWrClassDesc.c) | `0x114140` |
| 7 | Init | **override** | [`InitWr?`](../../../src/game/engine/anim/wr.cpp) | `0x22B720` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (LO) | `SetLoParent?` | `0x1A5900` |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` |
| 14 | LoadFromBrx | inherited (LO) | `LoadLoFromBrx` | `0x1A5C28` |
| 15 | PostLoad | inherited (LO) | `PostLoLoad` | `0x1A5C48` |
| 16 | Update | **override** | [`UpdateWrMatrixes__FP2WR`](../../../src/game/engine/anim/wr.cpp) | `0x22C028` |
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

No struct for `WR` in `include/sly2` yet. Size is known exactly (0x900); the bytes 0x50..0x900 are this class's own. Inherited fields: see [LO](LO.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 2 |
| L04_nightclub_ep03 | 2 |
| L08_palace_ep00 | 5 |
| L12_temple_ep00 | 8 |
| L13_temple_ep07 | 2 |
| L17_castle_ep00 | 2 |
| L19_castle_ep11 | 2 |
| L21_castle_ep13 | 2 |
| L22_castle_ep04 | 2 |
| L26_castle_ep17 | 2 |
| L32_sawmill_ep00 | 4 |
| L35_sawmill_ep24 | 2 |
| L37_sawmill_ep26 | 1 |
| L38_blimp_ep00 | 2 |
| L39_blimp_ep07 | 2 |
| L40_blimp_ep27 | 1 |
| L41_blimp_ep28 | 1 |
| L42_blimp_ep29 | 1 |
| L43_blimp_ep26 | 4 |
| world_elf2A38A8 | 2 |

## Related docs

[engine/overview.md](../../engine/overview.md) (1), [formats/level-objects.md](../../formats/level-objects.md) (1), [reference/class-tree.md](../../reference/class-tree.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
