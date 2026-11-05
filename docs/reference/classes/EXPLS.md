# EXPLS (CID 33, 0x21)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> child of EXPLO; Sly1 tree shape

| | |
|---|---|
| Name | `EXPLS` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [EXPLS](EXPLS.md) → [EXPLO](EXPLO.md) → [EXPL](EXPL.md) → [XFM](XFM.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x110 (272 bytes), +0x20 over EXPLO |
| Descriptor | `D_002BAD7C` (record 33 of D_002BAAE8; cid range 33..34) |
| Factory (`pfnNew`) | [`NewExpls`](../../../src/game/app/class_registry.cpp) `0x1111E0` |
| Descriptor getter (slot 6) | `GetExplsClassDesc` `0x113C20` |
| vtable | `D_002CE4D0`, 53 entries (slots 0..52; slot 0 null), 1 new, 3 overridden |
| vtable stored by | [`NewExpls`](../../../src/game/app/class_registry.cpp) (vptr +0xC) |
| Level instances | 8381 objects in 42 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | **override** | [`HandleExplsMessage`](../../../src/game/engine/fx/emitter.cpp) | `0x15C5C8` |
| 6 | Pclassinfo | **override** | [`GetExplsClassDesc`](../../../src/game/app/class_registry.cpp) | `0x113C20` |
| 7 | Init | inherited (XFM) | `InitXfm` | `0x231100` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (XFM) | `SetXfmParent` | `0x2315F0` |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` |
| 14 | LoadFromBrx | inherited (EXPLO) | `LoadExploFromBrx?` | `0x259030` |
| 15 | PostLoad | inherited (EXPLO) | `BindExplo?` | `0x259170` |
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
| 41 | Clone | inherited (EXPLO) | `CloneExplo?` | `0x2590B8` |
| 42 | SetLoSBusy | inherited (LO) | `SetLoSBusy?` | `0x1A6C48` |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` |
| 46 | SetVisMask | inherited (LO) | `SetLoVisMask?` | `0x1A6DC0` |
| 47 | SetGrf | inherited (LO) | `SetLoGrf?` | `0x1A66C8` |
| 48 | ClearLoGrf | inherited (LO) | `ClearLoGrf?` | `0x1A66F8` |
| 49 | XFM_PxfFromId | inherited (XFM) | `XFM_PxfFromId?` | `0x22D750` |
| 50 |  | inherited (XFM) | `func_00231220` | `0x231220` |
| 51 | ExplodeExplExplso__FP4EXPLP6EXPLSO | **override** | [`ExplodeExplsExplso`](../../../src/game/engine/fx/emitter.cpp) | `0x15C628` |
| 52 | FireExplsExplso | **new** | [`FireExplsExplso`](../../../src/game/engine/fx/emitter.cpp) | `0x15C690` |

## Fields

No struct for `EXPLS` in `include/sly2` yet. Size is known exactly (0x110); the bytes 0xF0..0x110 are this class's own. Inherited fields: see [EXPLO](EXPLO.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 332 |
| L03_nightclub_ep02 | 242 |
| L04_nightclub_ep03 | 286 |
| L05_nightclub_ep04 | 189 |
| L06_nightclub_ep05 | 205 |
| L07_nightclub_ep06 | 179 |
| L08_palace_ep00 | 286 |
| L09_palace_ep07 | 223 |
| L10_palace_ep08 | 193 |
| L11_palace_ep09 | 188 |
| L12_temple_ep00 | 377 |
| L13_temple_ep07 | 261 |
| L14_prison_ep00 | 339 |
| L15_prison_ep07 | 206 |
| L16_prison_ep10 | 94 |
| L17_castle_ep00 | 299 |
| L19_castle_ep11 | 108 |
| L20_castle_ep12 | 142 |
| L21_castle_ep13 | 127 |
| L22_castle_ep04 | 193 |
| L23_castle_ep14 | 124 |
| L24_castle_ep15 | 119 |
| L25_castle_ep16 | 115 |
| L26_castle_ep17 | 117 |
| L27_train_ep00 | 231 |
| L28_train_ep18 | 160 |
| L29_train_ep19 | 227 |
| L30_train_ep20 | 246 |
| L31_train_ep21 | 165 |
| L32_sawmill_ep00 | 234 |
| L33_sawmill_ep22 | 144 |
| L34_sawmill_ep23 | 110 |
| L35_sawmill_ep24 | 142 |
| L36_sawmill_ep25 | 82 |
| L37_sawmill_ep26 | 130 |
| L38_blimp_ep00 | 337 |
| L39_blimp_ep07 | 237 |
| L40_blimp_ep27 | 181 |
| L41_blimp_ep28 | 142 |
| L42_blimp_ep29 | 123 |
| L43_blimp_ep26 | 254 |
| world_elf2A38A8 | 292 |

## Related docs

[engine/particles-effects.md](../../engine/particles-effects.md) (4), [engine/core-library.md](../../engine/core-library.md) (1), [engine/game-progression.md](../../engine/game-progression.md) (1), [formats/level-objects.md](../../formats/level-objects.md) (1), [formats/world-file.md](../../formats/world-file.md) (1), [reference/class-tree.md](../../reference/class-tree.md) (1), [reference/glossary.md](../../reference/glossary.md) (1), [reference/tu-map.md](../../reference/tu-map.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
