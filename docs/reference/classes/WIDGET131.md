# WIDGET131 (CID 131, 0x83)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> WIDGET: episode menu

| | |
|---|---|
| Name | `WIDGET131` — placeholder (decimal CID) |
| Parent chain | [WIDGET131](WIDGET131.md) → [WIDGET](WIDGET.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x140 (320 bytes), +0x30 over WIDGET |
| Descriptor | `D_002BB524` (record 131 of D_002BAAE8; cid range 131..132) |
| Factory (`pfnNew`) | [`NewWidget131`](../../../src/game/app/class_registry.cpp) `0x112CF8` |
| Descriptor getter (slot 6) | `GetWidget131ClassDesc` `0x114240` |
| vtable | `D_002CEEB8`, 51 entries (slots 0..50; slot 0 null), 0 new, 4 overridden |
| vtable stored by | [`NewWidget131`](../../../src/game/app/class_registry.cpp) (vptr +0xC) |
| Level instances | 1 objects in 1 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` |
| 6 | Pclassinfo | **override** | [`GetWidget131ClassDesc`](../../../src/game/app/class_registry.cpp) | `0x114240` |
| 7 | Init | **override** | [`InitWidget131?`](../../../src/game/ui/tu_15CD68.cpp) | `0x15CD68` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (LO) | `SetLoParent?` | `0x1A5900` |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` |
| 14 | LoadFromBrx | inherited (WIDGET) | `LoadWidgetFromBrx?` | `0x212298` |
| 15 | PostLoad | **override** | [`PostWidget131Load?`](../../../src/game/ui/tu_15CD68.cpp) | `0x15CDA8` |
| 16 | Update | **override** | [`UpdateWidget131?`](../../../src/game/ui/tu_15CD68.cpp) | `0x15CF30` |
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

No struct for `WIDGET131` in `include/sly2` yet. Size is known exactly (0x140); the bytes 0x110..0x140 are this class's own. Inherited fields: see [WIDGET](WIDGET.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| world_elf2A37E8 | 1 |

## Related docs

[engine/hud-ui.md](../../engine/hud-ui.md) (2), [engine/game-progression.md](../../engine/game-progression.md) (1), [engine/main-loop-startup.md](../../engine/main-loop-startup.md) (1), [engine/world-sw.md](../../engine/world-sw.md) (1), [formats/level-objects.md](../../formats/level-objects.md) (1), [formats/world-file.md](../../formats/world-file.md) (1), [reference/class-tree.md](../../reference/class-tree.md) (1), [reference/tu-map.md](../../reference/tu-map.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
