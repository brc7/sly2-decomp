# WIDGET (CID 126, 0x7E)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> WIDGET

| | |
|---|---|
| Name | `WIDGET` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [WIDGET](WIDGET.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | [WIDGET127](WIDGET127.md), [WIDGET128](WIDGET128.md), [WIDGET129](WIDGET129.md), [WIDGET130](WIDGET130.md), [WIDGET131](WIDGET131.md) |
| Instance size (`cb`) | 0x110 (272 bytes), +0xC0 over LO |
| Descriptor | `D_002BB4C0` (record 126 of D_002BAAE8; cid range 126..132) |
| Factory (`pfnNew`) | [`NewWidget`](../../../src/game/app/class_registry/NewWidget.c) `0x112BE8` |
| Descriptor getter (slot 6) | `GetWidgetClassDesc` `0x1141F0` |
| vtable | `D_002DAF30`, 51 entries (slots 0..50; slot 0 null), 2 new, 5 overridden |
| vtable stored by | [`WIDGET_ctor?`](../../../src/game/engine/presentation/hud.cpp) (vptr +0xC) |
| Level instances | 1659 objects in 43 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` |
| 6 | Pclassinfo | **override** | [`GetWidgetClassDesc`](../../../src/game/app/class_registry/GetWidgetClassDesc.c) | `0x1141F0` |
| 7 | Init | inherited (LO) | `InitLo` | `0x1A5270` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (LO) | `SetLoParent?` | `0x1A5900` |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` |
| 14 | LoadFromBrx | **override** | [`LoadWidgetFromBrx?`](../../../src/game/engine/presentation/hud.cpp) | `0x212298` |
| 15 | PostLoad | **override** | [`PostWidgetLoad?`](../../../src/game/engine/presentation/hud.cpp) | `0x2123C8` |
| 16 | Update | **override** | [`UpdateWidget?`](../../../src/game/engine/presentation/hud.cpp) | `0x212C50` |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (LO) | `LO_vt17_InheritFlag4?` | `0x1A5E80` |
| 18 | UpdateLoXfWorldHierarchy | inherited (LO) | `UpdateLoXfWorldHierarchy` | `0x22FBD8` |
| 19 | RenderSelf | **override** | [`RenderWidget?`](../../../src/game/engine/presentation/hud.cpp) | `0x213300` |
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
| 49 | ShowWidget | **new** | [`ShowWidget?`](../../../src/game/engine/presentation/hud.cpp) | `0x2128E8` |
| 50 | HideWidget | **new** | [`HideWidget?`](../../../src/game/engine/presentation/hud.cpp) | `0x2129E8` |

## Fields

No struct for `WIDGET` in `include/sly2` yet. Size is known exactly (0x110); the bytes 0x50..0x110 are this class's own. Inherited fields: see [LO](LO.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 38 |
| L03_nightclub_ep02 | 38 |
| L04_nightclub_ep03 | 38 |
| L05_nightclub_ep04 | 38 |
| L06_nightclub_ep05 | 38 |
| L07_nightclub_ep06 | 38 |
| L08_palace_ep00 | 41 |
| L09_palace_ep07 | 37 |
| L10_palace_ep08 | 37 |
| L11_palace_ep09 | 38 |
| L12_temple_ep00 | 43 |
| L13_temple_ep07 | 40 |
| L14_prison_ep00 | 38 |
| L15_prison_ep07 | 37 |
| L16_prison_ep10 | 35 |
| L17_castle_ep00 | 43 |
| L19_castle_ep11 | 39 |
| L20_castle_ep12 | 39 |
| L21_castle_ep13 | 39 |
| L22_castle_ep04 | 41 |
| L23_castle_ep14 | 39 |
| L24_castle_ep15 | 39 |
| L25_castle_ep16 | 39 |
| L26_castle_ep17 | 39 |
| L27_train_ep00 | 36 |
| L28_train_ep18 | 36 |
| L29_train_ep19 | 37 |
| L30_train_ep20 | 38 |
| L31_train_ep21 | 36 |
| L32_sawmill_ep00 | 41 |
| L33_sawmill_ep22 | 40 |
| L34_sawmill_ep23 | 40 |
| L35_sawmill_ep24 | 40 |
| L36_sawmill_ep25 | 40 |
| L37_sawmill_ep26 | 40 |
| L38_blimp_ep00 | 42 |
| L39_blimp_ep07 | 39 |
| L40_blimp_ep27 | 39 |
| L41_blimp_ep28 | 39 |
| L42_blimp_ep29 | 39 |
| L43_blimp_ep26 | 39 |
| world_elf2A37E8 | 29 |
| world_elf2A38A8 | 38 |

## Related docs

[engine/hud-ui.md](../../engine/hud-ui.md) (11), [reference/class-tree.md](../../reference/class-tree.md) (11), [engine/minigames.md](../../engine/minigames.md) (8), [engine/bosses.md](../../engine/bosses.md) (5), [engine/overview.md](../../engine/overview.md) (2), [formats/level-objects.md](../../formats/level-objects.md) (2), [formats/world-file.md](../../formats/world-file.md) (2), [README.md](../../README.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
