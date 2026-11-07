# EXPLO (CID 32, 0x20)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> child of EXPL with one child; Sly1 tree shape

| | |
|---|---|
| Name | `EXPLO` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [EXPLO](EXPLO.md) → [EXPL](EXPL.md) → [XFM](XFM.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | [EXPLS](EXPLS.md) |
| Instance size (`cb`) | 0xF0 (240 bytes), +0x10 over EXPL |
| Descriptor | `D_002BAD68` (record 32 of D_002BAAE8; cid range 32..34) |
| Factory (`pfnNew`) | [`NewExplo`](../../../src/game/app/class_registry.cpp) `0x1111A8` |
| Descriptor getter (slot 6) | `GetExploClassDesc` `0x113C10` |
| vtables | 2 tables report this class: `D_002CA098` (52), `D_002CE680` (52) (see [hidden types B1](../hidden-types.md#b1-registered-getters-returned-by-more-than-one-vtable)). Slot table below: canonical 0x2CA098 (the table whose slot 6 is the registered getter; the others use a duplicate getter copy) |
| Level instances | 52 objects in 42 of 43 world files |

- **Hidden-type note (HYPOTHESIS):** 2 vtables report this class from slot 6 (unregistered subclasses that do not override Pclassinfo) — [hidden-types B](../hidden-types.md#b-vtables-and-descriptor-like-tables-not-claimed-by-a-registered-class)

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` |
| 6 | Pclassinfo | **override** | [`GetExploClassDesc`](../../../src/game/app/class_registry.cpp) | `0x113C10` |
| 7 | Init | inherited (XFM) | `InitXfm` | `0x231100` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (XFM) | `SetXfmParent` | `0x2315F0` |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` |
| 14 | LoadFromBrx | **override** | [`LoadExploFromBrx?`](../../../src/game/engine/fx/emitter.cpp) | `0x259030` |
| 15 | PostLoad | **override** | [`BindExplo?`](../../../src/game/engine/fx/emitter.cpp) | `0x259170` |
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
| 28 | CploChildren | inherited (LO) | `LO_CploChildren` | `0x1A5BE8` |
| 29 | TranslateToPos | inherited (XFM) | `TranslateXfmToPos` | `0x2312A0` |
| 30 | RotateToMat | inherited (XFM) | `RotateXfmToMat` | `0x231300` |
| 31 | PmatWorld | inherited (XFM) | `GetXfmWorldMatrix` | `0x231440` |
| 32 | LO_PxfLocal | inherited (XFM) | `XFM_PxfLocal?` | `0x231550` |
| 33 | GetVelocity | inherited (XFM) | `GetXfmVelocity?` | `0x231468` |
| 34 | GetAngularVelocity | inherited (XFM) | `GetXfmAngularVelocity?` | `0x2314D0` |
| 35 | SetVelocity | inherited (LO) | `SetLoVelocity?` | `0x1A6130` |
| 36 | SetAngularVelocity | inherited (LO) | `SetLoAngularVelocity` | `0x1A6148` |
| 37 | ApplyLoProxyHierarchy | inherited (XFM) | `ApplyXfmProxy?` | `0x231598` |
| 38 | MatchLoXf | inherited (LO) | `MatchLoXf?` | `0x1A6B88` |
| 39 | SetLoXf | inherited (LO) | `SetLoXf?` | `0x1A6BC8` |
| 40 | CloneHierarchy | inherited (LO) | `CloneLoHierarchy` | `0x1A6AD0` |
| 41 | Clone | **override** | [`CloneExplo?`](../../../src/game/engine/fx/emitter.cpp) | `0x2590B8` |
| 42 | SetLoSBusy | inherited (LO) | `SetLoSBusy?` | `0x1A6C48` |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` |
| 46 | SetVisMask | inherited (LO) | `SetLoVisMask?` | `0x1A6DC0` |
| 47 | SetGrf | inherited (LO) | `SetLoGrf?` | `0x1A66C8` |
| 48 | ClearLoGrf | inherited (LO) | `ClearLoGrf?` | `0x1A66F8` |
| 49 | XFM_PxfFromId | inherited (XFM) | `XFM_PxfFromId?` | `0x22D750` |
| 50 |  | inherited (XFM) | `func_00231220` | `0x231220` |
| 51 | ExplodeExplExplso__FP4EXPLP6EXPLSO | inherited (EXPL) | `ExplodeExplExplso__FP4EXPLP6EXPLSO` | `0x15C1E0` |

## Fields

No struct for `EXPLO` in `include/sly2` yet. Size is known exactly (0xF0); the bytes 0xE0..0xF0 are this class's own. Inherited fields: see [EXPL](EXPL.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 2 |
| L03_nightclub_ep02 | 1 |
| L04_nightclub_ep03 | 1 |
| L05_nightclub_ep04 | 1 |
| L06_nightclub_ep05 | 1 |
| L07_nightclub_ep06 | 2 |
| L08_palace_ep00 | 1 |
| L09_palace_ep07 | 1 |
| L10_palace_ep08 | 2 |
| L11_palace_ep09 | 1 |
| L12_temple_ep00 | 1 |
| L13_temple_ep07 | 1 |
| L14_prison_ep00 | 1 |
| L15_prison_ep07 | 1 |
| L16_prison_ep10 | 1 |
| L17_castle_ep00 | 1 |
| L19_castle_ep11 | 1 |
| L20_castle_ep12 | 1 |
| L21_castle_ep13 | 1 |
| L22_castle_ep04 | 1 |
| L23_castle_ep14 | 2 |
| L24_castle_ep15 | 2 |
| L25_castle_ep16 | 2 |
| L26_castle_ep17 | 2 |
| L27_train_ep00 | 3 |
| L28_train_ep18 | 1 |
| L29_train_ep19 | 1 |
| L30_train_ep20 | 1 |
| L31_train_ep21 | 1 |
| L32_sawmill_ep00 | 1 |
| L33_sawmill_ep22 | 1 |
| L34_sawmill_ep23 | 1 |
| L35_sawmill_ep24 | 1 |
| L36_sawmill_ep25 | 1 |
| L37_sawmill_ep26 | 1 |
| L38_blimp_ep00 | 1 |
| L39_blimp_ep07 | 1 |
| L40_blimp_ep27 | 1 |
| L41_blimp_ep28 | 1 |
| L42_blimp_ep29 | 1 |
| L43_blimp_ep26 | 2 |
| world_elf2A38A8 | 1 |

## Related docs

[engine/core-library.md](../../engine/core-library.md) (2), [engine/particles-effects.md](../../engine/particles-effects.md) (2), [reference/class-tree.md](../../reference/class-tree.md) (2), [formats/level-objects.md](../../formats/level-objects.md) (1), [reference/glossary.md](../../reference/glossary.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
