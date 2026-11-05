# EXPLG (CID 31, 0x1F)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> child of EXPL, same TU; Sly1 tree shape

| | |
|---|---|
| Name | `EXPLG` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [EXPLG](EXPLG.md) → [EXPL](EXPL.md) → [XFM](XFM.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0xF0 (240 bytes), +0x10 over EXPL |
| Descriptor | `D_002BAD54` (record 31 of D_002BAAE8; cid range 31..32) |
| Factory (`pfnNew`) | [`NewExplg`](../../../src/game/app/class_registry.cpp) `0x111170` |
| Descriptor getter (slot 6) | `GetExplgClassDesc` `0x113C00` |
| vtable | `D_002CE828`, 52 entries (slots 0..51; slot 0 null), 0 new, 5 overridden |
| vtable stored by | [`NewExplg`](../../../src/game/app/class_registry.cpp) (vptr +0xC) |
| Level instances | 1958 objects in 42 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` |
| 6 | Pclassinfo | **override** | [`GetExplgClassDesc`](../../../src/game/app/class_registry/GetExplgClassDesc.c) | `0x113C00` |
| 7 | Init | inherited (XFM) | `InitXfm` | `0x231100` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (XFM) | `SetXfmParent` | `0x2315F0` |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` |
| 14 | LoadFromBrx | **override** | [`LoadExplgFromBrx`](../../../src/game/engine/fx/emitter/LoadExplgFromBrx.c) | `0x15C1E8` |
| 15 | PostLoad | inherited (LO) | `PostLoLoad` | `0x1A5C48` |
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
| 28 | CploChildren | **override** | [`CploExplgChildren`](../../../src/game/engine/fx/emitter/CploExplgChildren.c) | `0x15C310` |
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
| 41 | Clone | **override** | [`CloneExplg`](../../../src/game/engine/fx/emitter/CloneExplg.c) | `0x15C320` |
| 42 | SetLoSBusy | inherited (LO) | `SetLoSBusy?` | `0x1A6C48` |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` |
| 46 | SetVisMask | inherited (LO) | `SetLoVisMask?` | `0x1A6DC0` |
| 47 | SetGrf | inherited (LO) | `SetLoGrf?` | `0x1A66C8` |
| 48 | ClearLoGrf | inherited (LO) | `ClearLoGrf?` | `0x1A66F8` |
| 49 | XFM_PxfFromId | inherited (XFM) | `XFM_PxfFromId?` | `0x22D750` |
| 50 |  | inherited (XFM) | `func_00231220` | `0x231220` |
| 51 | ExplodeExplExplso__FP4EXPLP6EXPLSO | **override** | [`ExplodeExplgExplso`](../../../src/game/engine/fx/emitter/ExplodeExplgExplso.cpp) | `0x15C3C0` |

## Fields

No struct for `EXPLG` in `include/sly2` yet. Size is known exactly (0xF0); the bytes 0xE0..0xF0 are this class's own. Inherited fields: see [EXPL](EXPL.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 81 |
| L03_nightclub_ep02 | 54 |
| L04_nightclub_ep03 | 68 |
| L05_nightclub_ep04 | 54 |
| L06_nightclub_ep05 | 46 |
| L07_nightclub_ep06 | 41 |
| L08_palace_ep00 | 87 |
| L09_palace_ep07 | 55 |
| L10_palace_ep08 | 43 |
| L11_palace_ep09 | 42 |
| L12_temple_ep00 | 81 |
| L13_temple_ep07 | 63 |
| L14_prison_ep00 | 73 |
| L15_prison_ep07 | 48 |
| L16_prison_ep10 | 19 |
| L17_castle_ep00 | 72 |
| L19_castle_ep11 | 22 |
| L20_castle_ep12 | 27 |
| L21_castle_ep13 | 26 |
| L22_castle_ep04 | 42 |
| L23_castle_ep14 | 23 |
| L24_castle_ep15 | 30 |
| L25_castle_ep16 | 23 |
| L26_castle_ep17 | 24 |
| L27_train_ep00 | 57 |
| L28_train_ep18 | 35 |
| L29_train_ep19 | 54 |
| L30_train_ep20 | 58 |
| L31_train_ep21 | 39 |
| L32_sawmill_ep00 | 58 |
| L33_sawmill_ep22 | 33 |
| L34_sawmill_ep23 | 25 |
| L35_sawmill_ep24 | 32 |
| L36_sawmill_ep25 | 17 |
| L37_sawmill_ep26 | 28 |
| L38_blimp_ep00 | 86 |
| L39_blimp_ep07 | 55 |
| L40_blimp_ep27 | 40 |
| L41_blimp_ep28 | 33 |
| L42_blimp_ep29 | 26 |
| L43_blimp_ep26 | 63 |
| world_elf2A38A8 | 75 |

## Related docs

[engine/particles-effects.md](../../engine/particles-effects.md) (4), [formats/level-objects.md](../../formats/level-objects.md) (2), [engine/game-progression.md](../../engine/game-progression.md) (1), [engine/minigames.md](../../engine/minigames.md) (1), [formats/world-file.md](../../formats/world-file.md) (1), [reference/class-tree.md](../../reference/class-tree.md) (1), [reference/glossary.md](../../reference/glossary.md) (1), [reference/tu-map.md](../../reference/tu-map.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
