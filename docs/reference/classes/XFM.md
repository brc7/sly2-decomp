# XFM (CID 3, 0x03)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> Lilly XFM : LO ends 0xE0 (pxfParent, nStampWorld, mat, matWorld); own code in xform TU 0x2310E0; base of 44 positioned lightweights (replaces Sly1 ALO role)

| | |
|---|---|
| Name | `XFM` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [XFM](XFM.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | [LIGHT](LIGHT.md), [CID5](CID5.md), [PROXY](PROXY.md), [CID8](CID8.md), [SHAPE](SHAPE.md), [TN](TN.md), [DETECTOR](DETECTOR.md), [PATHZONE](PATHZONE.md), [TARGET](TARGET.md), [CID20](CID20.md), [CID21](CID21.md), [VOL](VOL.md), [BLIPG](BLIPG.md), [EMITTER](EMITTER.md), [EXPL](EXPL.md), [CID34](CID34.md), [CID35](CID35.md), [CID38](CID38.md), [CID39](CID39.md), [CID40](CID40.md), [CID41](CID41.md), [WARP](WARP.md), [CID43](CID43.md), [CID44](CID44.md), [CID45](CID45.md), [CID46](CID46.md), [CID47](CID47.md) |
| Descendants | 44 classes (CID 4..47) |
| Instance size (`cb`) | 0xE0 (224 bytes), +0x90 over LO |
| Descriptor | `D_002BAB24` (record 3 of D_002BAAE8; cid range 3..48) |
| Factory (`pfnNew`) | [`NewXfm`](../../../src/game/app/class_registry/NewXfm.c) `0x110B10` |
| Descriptor getter (slot 6) | `GetXfmClassDesc` `0x113A40` |
| vtable | `D_002DD090`, 51 entries (slots 0..50; slot 0 null), 2 new, 12 overridden |
| vtable stored by | [`NewXfm`](../../../src/game/app/class_registry/NewXfm.c) (vptr +0xC) |
| Level instances | 4619 objects in 42 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetXfmClassDesc`](../../../src/game/app/class_registry/GetXfmClassDesc.c) | `0x113A40` | Pclassinfo |
| 7 | Init | **override** | [`InitXfm`](../../../src/game/object/xform.cpp) | `0x231100` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | **override** | [`SetXfmParent`](../../../src/game/object/xform.cpp) | `0x2315F0` | SetParent |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` | AddHierarchy |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` | RemoveHierarchy |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` | UpdateLoroot |
| 14 | LoadFromBrx | **override** | [`LoadXfmFromBrx`](../../../src/game/object/xform.cpp) | `0x231170` | LoadFromBrx |
| 15 | PostLoad | inherited (LO) | `PostLoLoad` | `0x1A5C48` | VUnk14 |
| 16 | Update | inherited (LO) | `UpdateLo` | `0x1A5E28` | VUnk15 |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (LO) | `LO_vt17_InheritFlag4?` | `0x1A5E80` | VUnk16 |
| 18 | UpdateLoXfWorldHierarchy | inherited (LO) | `UpdateLoXfWorldHierarchy` | `0x22FBD8` | UpdateXfWorldHierarchy |
| 19 | RenderSelf | inherited (LO) | `RenderLoSelf` | `0x1A5ED8` | RenderSelf |
| 20 | RenderLoAll | inherited (LO) | `RenderLoAll?` | `0x1A5EE0` | VUnk19 |
| 21 | SetLoFlag8 | inherited (LO) | `SetLoFlag8?` | `0x1A6E80` | VUnk20 |
| 22 | OnRemove | inherited (LO) | `OnLoRemove` | `0x1A6150` | Freeze |
| 23 | OnAdd | inherited (LO) | `OnLoAdd` | `0x1A6178` | Unfreeze |
| 24 |  | inherited (LO) | `func_00210330` | `0x210330` | VUnk23 |
| 25 | SetIdProxy | inherited (LO) | `SetLoIdProxy` | `0x1CA118` | SetIdProxy |
| 26 | ResetLoForProxy | inherited (LO) | `ResetLoForProxy?` | `0x1CB4E0` | VUnk25 |
| 27 | LO_VUnk27Proxy | inherited (LO) | `LO_VUnk27Proxy?` | `0x1CB5C8` | VUnk26 |
| 28 | CploChildren | inherited (LO) | `LO_CploChildren` | `0x1A5BE8` | CploChildren |
| 29 | TranslateToPos | **override** | [`TranslateXfmToPos`](../../../src/game/object/xform.cpp) | `0x2312A0` | TranslateToPos |
| 30 | RotateToMat | **override** | [`RotateXfmToMat`](../../../src/game/object/xform.cpp) | `0x231300` | RotateToMat |
| 31 | PmatWorld | **override** | [`XFM_PmatWorld`](../../../src/game/object/xform.cpp) | `0x231440` | PmatWorld |
| 32 | LO_PxfLocal | **override** | [`XFM_PxfLocal?`](../../../src/game/object/xform.cpp) | `0x231550` | PxfLocal |
| 33 | GetVelocity | **override** | [`GetXfmVelocity?`](../../../src/game/object/xform.cpp) | `0x231468` | GetVelocity |
| 34 | GetAngularVelocity | **override** | [`GetXfmAngularVelocity?`](../../../src/game/object/xform.cpp) | `0x2314D0` | GetAngularVelocity |
| 35 | SetVelocity | inherited (LO) | `SetLoVelocity?` | `0x1A6130` | SetVelocity |
| 36 | SetAngularVelocity | inherited (LO) | `SetLoAngularVelocity` | `0x1A6148` | SetAngularVelocity |
| 37 | ApplyLoProxyHierarchy | **override** | [`ApplyXfmProxy?`](../../../src/game/object/xform.cpp) | `0x231598` | ApplyProxy |
| 38 | MatchLoXf | inherited (LO) | `MatchLoXf?` | `0x1A6B88` | VUnk37 |
| 39 | SetLoXf | inherited (LO) | `SetLoXf?` | `0x1A6BC8` | VUnk38 |
| 40 | CloneHierarchy | inherited (LO) | `CloneLoHierarchy` | `0x1A6AD0` | CloneHierarchy |
| 41 | Clone | **override** | [`CloneXfm`](../../../src/game/object/xform.cpp) | `0x2311B0` | Clone |
| 42 | SetLoSBusy | inherited (LO) | `SetLoSBusy?` | `0x1A6C48` | VUnk41 |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` | VUnk42 |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` | VUnk43 |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` | VUnk44 |
| 46 | SetVisMask | inherited (LO) | `SetLoVisMask?` | `0x1A6DC0` | SetVisMask |
| 47 | SetGrf | inherited (LO) | `SetLoGrf?` | `0x1A66C8` | VUnk46 |
| 48 | ClearLoGrf | inherited (LO) | `ClearLoGrf?` | `0x1A66F8` | VUnk47 |
| 49 | XFM_PxfFromId | **new** | [`XFM_PxfFromId?`](../../../src/game/engine/anim/xf.cpp) | `0x22D750` | VSlot49 |
| 50 |  | **new** | [`func_00231220`](../../../src/game/object/xform.cpp) | `0x231220` | VSlot50 |

## Fields

From `include/sly2/xfm.h` (`struct XFM : LO`). XFM: from src/game/gameplay/player/jt.cpp (10 copies in Lilly ports)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 2 header asserts agree, 0 disagree. Size from header: 0xE0.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x50 | `XF *` | `pxfParent` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x54 | `int` | `nStampWorld` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x58 | `undefined4[2]` | `unk_0x58` | unknown (padding/placeholder) |  |
| 0x60 | `struct MATRIX4` | `mat` | header (Lilly-ported; offset computed, unasserted) | +0x60 [S] local matrix |
| 0xA0 | `struct MATRIX4` | `matWorld` | asserted (translation at +0xD0 (CONFIRMED runtime)) |  |

Inherited fields: see [LO](LO.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 269 |
| L03_nightclub_ep02 | 100 |
| L04_nightclub_ep03 | 93 |
| L05_nightclub_ep04 | 72 |
| L06_nightclub_ep05 | 66 |
| L07_nightclub_ep06 | 63 |
| L08_palace_ep00 | 224 |
| L09_palace_ep07 | 108 |
| L10_palace_ep08 | 110 |
| L11_palace_ep09 | 117 |
| L12_temple_ep00 | 338 |
| L13_temple_ep07 | 132 |
| L14_prison_ep00 | 282 |
| L15_prison_ep07 | 138 |
| L16_prison_ep10 | 46 |
| L17_castle_ep00 | 303 |
| L19_castle_ep11 | 31 |
| L20_castle_ep12 | 62 |
| L21_castle_ep13 | 32 |
| L22_castle_ep04 | 94 |
| L23_castle_ep14 | 35 |
| L24_castle_ep15 | 32 |
| L25_castle_ep16 | 33 |
| L26_castle_ep17 | 37 |
| L27_train_ep00 | 191 |
| L28_train_ep18 | 92 |
| L29_train_ep19 | 103 |
| L30_train_ep20 | 86 |
| L31_train_ep21 | 69 |
| L32_sawmill_ep00 | 306 |
| L33_sawmill_ep22 | 57 |
| L34_sawmill_ep23 | 68 |
| L35_sawmill_ep24 | 61 |
| L36_sawmill_ep25 | 30 |
| L37_sawmill_ep26 | 65 |
| L38_blimp_ep00 | 144 |
| L39_blimp_ep07 | 89 |
| L40_blimp_ep27 | 63 |
| L41_blimp_ep28 | 36 |
| L42_blimp_ep29 | 53 |
| L43_blimp_ep26 | 88 |
| world_elf2A38A8 | 201 |

## Related docs

[reference/class-tree.md](../../reference/class-tree.md) (34), [engine/object-model.md](../../engine/object-model.md) (9), [formats/level-objects.md](../../formats/level-objects.md) (7), [engine/animation.md](../../engine/animation.md) (4), [reference/tu-map.md](../../reference/tu-map.md) (4), [engine/camera.md](../../engine/camera.md) (3), [engine/physics-collision.md](../../engine/physics-collision.md) (3), [formats/meshes-scene.md](../../formats/meshes-scene.md) (3)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
