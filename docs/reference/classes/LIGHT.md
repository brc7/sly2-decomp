# LIGHT (CID 4, 0x04)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> LIGHT (relight inputs: 3 qw from +0x120)

| | |
|---|---|
| Name | `LIGHT` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [LIGHT](LIGHT.md) → [XFM](XFM.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x170 (368 bytes), +0x90 over XFM |
| Descriptor | `D_002BAB38` (record 4 of D_002BAAE8; cid range 4..5) |
| Factory (`pfnNew`) | [`NewLight`](../../../src/auto/NewLight.c) `0x110B48` |
| Descriptor getter (slot 6) | `GetLightClassDesc` `0x113A50` |
| vtable | `D_002D33A0`, 51 entries (slots 0..50; slot 0 null), 0 new, 10 overridden |
| vtable stored by | [`NewLight`](../../../src/auto/NewLight.c) (vptr +0xC) |
| Level instances | 2855 objects in 42 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` |
| 6 | Pclassinfo | **override** | [`GetLightClassDesc`](../../../src/auto/GetLightClassDesc.c) | `0x113A50` |
| 7 | Init | **override** | [`InitLight?`](../../../src/game/engine/graphics/pipeline/light/func_001A3360.c) | `0x1A3360` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (XFM) | `SetXfmParent` | `0x2315F0` |
| 11 | AddHierarchy | **override** | [`AddLightHierarchy?`](../../../src/game/engine/graphics/pipeline/light/func_001A3430.c) | `0x1A3430` |
| 12 | RemoveHierarchy | **override** | [`RemoveLightHierarchy?`](../../../src/game/engine/graphics/pipeline/light/func_001A34B0.c) | `0x1A34B0` |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` |
| 14 | LoadFromBrx | inherited (XFM) | `LoadXfmFromBrx` | `0x231170` |
| 15 | PostLoad | **override** | [`PostLightLoad?`](../../../src/game/engine/graphics/pipeline/light/func_001A3400.c) | `0x1A3400` |
| 16 | Update | inherited (LO) | `UpdateLo` | `0x1A5E28` |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | **override** | [`LIGHT_vt17?`](../../../src/game/engine/graphics/pipeline/light/func_001A3460.c) | `0x1A3460` |
| 18 | UpdateLoXfWorldHierarchy | inherited (LO) | `UpdateLoXfWorldHierarchy` | `0x22FBD8` |
| 19 | RenderSelf | inherited (LO) | `RenderLoSelf` | `0x1A5ED8` |
| 20 | RenderLoAll | inherited (LO) | `RenderLoAll?` | `0x1A5EE0` |
| 21 | SetLoFlag8 | inherited (LO) | `SetLoFlag8?` | `0x1A6E80` |
| 22 | OnRemove | **override** | [`OnLightRemove?`](../../../src/game/engine/graphics/pipeline/light/func_001A34E0.c) | `0x1A34E0` |
| 23 | OnAdd | **override** | [`OnLightAdd?`](../../../src/game/engine/graphics/pipeline/light/func_001A3518.c) | `0x1A3518` |
| 24 |  | inherited (LO) | `func_00210330` | `0x210330` |
| 25 | SetIdProxy | inherited (LO) | `SetLoIdProxy` | `0x1CA118` |
| 26 | ResetLoForProxy | inherited (LO) | `ResetLoForProxy?` | `0x1CB4E0` |
| 27 | LO_VUnk27Proxy | inherited (LO) | `LO_VUnk27Proxy?` | `0x1CB5C8` |
| 28 | CploChildren | inherited (LO) | `LO_CploChildren` | `0x1A5BE8` |
| 29 | TranslateToPos | **override** | [`TranslateLightToPos?`](../../../src/game/engine/graphics/pipeline/light/func_001A3550.c) | `0x1A3550` |
| 30 | RotateToMat | **override** | [`RotateLightToMat?`](../../../src/game/engine/graphics/pipeline/light/func_001A3580.c) | `0x1A3580` |
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

## Fields

No struct for `LIGHT` in `include/sly2` yet. Size is known exactly (0x170); the bytes 0xE0..0x170 are this class's own. Inherited fields: see [XFM](XFM.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 253 |
| L03_nightclub_ep02 | 96 |
| L04_nightclub_ep03 | 84 |
| L05_nightclub_ep04 | 96 |
| L06_nightclub_ep05 | 85 |
| L07_nightclub_ep06 | 29 |
| L08_palace_ep00 | 171 |
| L09_palace_ep07 | 96 |
| L10_palace_ep08 | 60 |
| L11_palace_ep09 | 37 |
| L12_temple_ep00 | 203 |
| L13_temple_ep07 | 130 |
| L14_prison_ep00 | 157 |
| L15_prison_ep07 | 121 |
| L16_prison_ep10 | 3 |
| L17_castle_ep00 | 180 |
| L19_castle_ep11 | 34 |
| L20_castle_ep12 | 44 |
| L21_castle_ep13 | 38 |
| L22_castle_ep04 | 69 |
| L23_castle_ep14 | 22 |
| L24_castle_ep15 | 26 |
| L25_castle_ep16 | 21 |
| L26_castle_ep17 | 29 |
| L27_train_ep00 | 100 |
| L28_train_ep18 | 35 |
| L29_train_ep19 | 36 |
| L30_train_ep20 | 35 |
| L31_train_ep21 | 28 |
| L32_sawmill_ep00 | 90 |
| L33_sawmill_ep22 | 50 |
| L34_sawmill_ep23 | 48 |
| L35_sawmill_ep24 | 23 |
| L36_sawmill_ep25 | 9 |
| L37_sawmill_ep26 | 28 |
| L38_blimp_ep00 | 55 |
| L39_blimp_ep07 | 25 |
| L40_blimp_ep27 | 19 |
| L41_blimp_ep28 | 18 |
| L42_blimp_ep29 | 18 |
| L43_blimp_ep26 | 32 |
| world_elf2A38A8 | 122 |

## Related docs

[engine/rendering.md](../../engine/rendering.md) (6), [engine/object-model.md](../../engine/object-model.md) (4), [formats/meshes-scene.md](../../formats/meshes-scene.md) (2), [reference/class-tree.md](../../reference/class-tree.md) (2), [engine/guards-ai.md](../../engine/guards-ai.md) (1), [engine/overview.md](../../engine/overview.md) (1), [engine/physics-collision.md](../../engine/physics-collision.md) (1), [formats/level-objects.md](../../formats/level-objects.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
