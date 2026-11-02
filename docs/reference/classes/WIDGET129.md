# WIDGET129 (CID 129, 0x81)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> WIDGET: objective counter (computed per frame)

| | |
|---|---|
| Name | `WIDGET129` — placeholder (decimal CID) |
| Parent chain | [WIDGET129](WIDGET129.md) → [WIDGET](WIDGET.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x220 (544 bytes), +0x110 over WIDGET |
| Descriptor | `D_002BB4FC` (record 129 of D_002BAAE8; cid range 129..130) |
| Factory (`pfnNew`) | [`NewWidget129`](../../../src/game/app/class_registry/NewWidget129.c) `0x112C90` |
| Descriptor getter (slot 6) | `GetWidget129ClassDesc` `0x114220` |
| vtable | `D_002DAD90`, 51 entries (slots 0..50; slot 0 null), 0 new, 3 overridden |
| vtable stored by | [`NewWidget129`](../../../src/game/app/class_registry/NewWidget129.c) (vptr +0xC) |
| Level instances | 212 objects in 41 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` |
| 6 | Pclassinfo | **override** | [`GetWidget129ClassDesc`](../../../src/game/app/class_registry/GetWidget129ClassDesc.c) | `0x114220` |
| 7 | Init | inherited (LO) | `InitLo` | `0x1A5270` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (LO) | `SetLoParent?` | `0x1A5900` |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` |
| 14 | LoadFromBrx | inherited (WIDGET) | `LoadWidgetFromBrx?` | `0x212298` |
| 15 | PostLoad | **override** | [`PostWidget129Load?`](../../../src/game/engine/presentation/hud/func_00211EC0.c) | `0x211EC0` |
| 16 | Update | **override** | [`UpdateWidget129?`](../../../src/game/engine/presentation/hud/func_00211F90.c) | `0x211F90` |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (LO) | `LO_vt17_InheritFlag4?` | `0x1A5E80` |
| 18 | UpdateLoXfWorldHierarchy | inherited (LO) | `UpdateLoXfWorldHierarchy` | `0x22FBD8` |
| 19 | RenderSelf | inherited (WIDGET) | `RenderWidget?` | `0x213300` |
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
| 49 | ShowWidget | inherited (WIDGET) | `ShowWidget?` | `0x2128E8` |
| 50 | HideWidget | inherited (WIDGET) | `HideWidget?` | `0x2129E8` |

## Fields

No struct for `WIDGET129` in `include/sly2` yet. Size is known exactly (0x220); the bytes 0x110..0x220 are this class's own. Inherited fields: see [WIDGET](WIDGET.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 4 |
| L03_nightclub_ep02 | 4 |
| L04_nightclub_ep03 | 4 |
| L05_nightclub_ep04 | 4 |
| L06_nightclub_ep05 | 4 |
| L07_nightclub_ep06 | 4 |
| L08_palace_ep00 | 5 |
| L09_palace_ep07 | 5 |
| L10_palace_ep08 | 5 |
| L11_palace_ep09 | 5 |
| L12_temple_ep00 | 3 |
| L13_temple_ep07 | 3 |
| L14_prison_ep00 | 6 |
| L15_prison_ep07 | 6 |
| L16_prison_ep10 | 6 |
| L17_castle_ep00 | 7 |
| L19_castle_ep11 | 7 |
| L20_castle_ep12 | 7 |
| L21_castle_ep13 | 7 |
| L22_castle_ep04 | 7 |
| L23_castle_ep14 | 7 |
| L24_castle_ep15 | 7 |
| L25_castle_ep16 | 7 |
| L26_castle_ep17 | 7 |
| L27_train_ep00 | 3 |
| L28_train_ep18 | 3 |
| L29_train_ep19 | 3 |
| L30_train_ep20 | 3 |
| L31_train_ep21 | 3 |
| L32_sawmill_ep00 | 4 |
| L33_sawmill_ep22 | 4 |
| L34_sawmill_ep23 | 4 |
| L35_sawmill_ep24 | 4 |
| L36_sawmill_ep25 | 4 |
| L37_sawmill_ep26 | 4 |
| L38_blimp_ep00 | 7 |
| L39_blimp_ep07 | 7 |
| L40_blimp_ep27 | 7 |
| L41_blimp_ep28 | 7 |
| L42_blimp_ep29 | 7 |
| L43_blimp_ep26 | 7 |

## Related docs

[engine/game-progression.md](../../engine/game-progression.md) (1), [engine/hud-ui.md](../../engine/hud-ui.md) (1), [engine/minigames.md](../../engine/minigames.md) (1), [reference/class-tree.md](../../reference/class-tree.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
