# LO (CID 2, 0x02)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> PloNew/LoadSwObjectsFromBrx vt slot 7 Init / 14 LoadFromBrx; Lilly LO : BASIC ends 0x50

| | |
|---|---|
| Name | `LO` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [LO](LO.md) → [BASIC](BASIC.md) |
| Children | [XFM](XFM.md), [SO](SO.md), [ASEG](ASEG.md), [SM](SM.md), [CID112](CID112.md), [CID113](CID113.md), [RELAY](RELAY.md), [WR](WR.md), [CID116](CID116.md), [CID117](CID117.md), [CID118](CID118.md), [CID119](CID119.md), [CID120](CID120.md), [CID121](CID121.md), [CID122](CID122.md), [CID123](CID123.md), [PATHLINK](PATHLINK.md), [CID125](CID125.md), [WIDGET](WIDGET.md), [JOB](JOB.md), [TASK](TASK.md) |
| Descendants | 131 classes (CID 3..133) |
| Instance size (`cb`) | 0x50 (80 bytes), +0x40 over BASIC |
| Descriptor | `D_002BAB10` (record 2 of D_002BAAE8; cid range 2..134) |
| Factory (`pfnNew`) | [`NewLo`](../../../src/game/app/class_registry/NewLo.c) `0x110AD8` |
| Descriptor getter (slot 6) | `GetLoClassDesc` `0x113A30` |
| vtable | `D_002D3540`, 49 entries (slots 0..48; slot 0 null), 42 new, 2 overridden |
| vtable stored by | [`NewLo`](../../../src/game/app/class_registry/NewLo.c) (vptr +0xC) |
| Level instances | 178 objects in 42 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | **override** | [`HandleLoSpliceEvent`](../../../src/game/object/lo.cpp) | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetLoClassDesc`](../../../src/game/app/class_registry/GetLoClassDesc.c) | `0x113A30` | Pclassinfo |
| 7 | Init | **new** | [`InitLo`](../../../src/game/object/lo.cpp) | `0x1A5270` | Init |
| 8 | Add | **new** | [`AddLo`](../../../src/game/object/lo.cpp) | `0x1A5478` | Add |
| 9 | Remove | **new** | [`RemoveLo`](../../../src/game/object/lo.cpp) | `0x1A5630` | Remove |
| 10 | SetParent | **new** | [`SetLoParent?`](../../../src/game/object/lo.cpp) | `0x1A5900` | SetParent |
| 11 | AddHierarchy | **new** | [`AddLoHierarchy`](../../../src/game/object/lo.cpp) | `0x1A5800` | AddHierarchy |
| 12 | RemoveHierarchy | **new** | [`RemoveLoHierarchy`](../../../src/game/object/lo.cpp) | `0x1A5848` | RemoveHierarchy |
| 13 | UpdateLoroot | **new** | [`UpdateLoroot?`](../../../src/game/object/lo.cpp) | `0x1A5B80` | UpdateLoroot |
| 14 | LoadFromBrx | **new** | [`LoadLoFromBrx`](../../../src/game/object/lo.cpp) | `0x1A5C28` | LoadFromBrx |
| 15 | PostLoad | **new** | [`PostLoLoad`](../../../src/game/object/lo.cpp) | `0x1A5C48` | VUnk14 |
| 16 | Update | **new** | [`UpdateLo`](../../../src/game/object/lo.cpp) | `0x1A5E28` | VUnk15 |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | **new** | [`LO_vt17_InheritFlag4?`](../../../src/game/object/lo.cpp) | `0x1A5E80` | VUnk16 |
| 18 | UpdateLoXfWorldHierarchy | **new** | [`UpdateLoXfWorldHierarchy`](../../../src/game/engine/anim/xf.cpp) | `0x22FBD8` | UpdateXfWorldHierarchy |
| 19 | RenderSelf | **new** | [`RenderLoSelf`](../../../src/game/object/lo.cpp) | `0x1A5ED8` | RenderSelf |
| 20 | RenderLoAll | **new** | [`RenderLoAll?`](../../../src/game/object/lo.cpp) | `0x1A5EE0` | VUnk19 |
| 21 | SetLoFlag8 | **new** | [`SetLoFlag8?`](../../../src/game/object/lo.cpp) | `0x1A6E80` | VUnk20 |
| 22 | OnRemove | **new** | [`OnLoRemove`](../../../src/game/object/lo.cpp) | `0x1A6150` | Freeze |
| 23 | OnAdd | **new** | [`OnLoAdd`](../../../src/game/object/lo.cpp) | `0x1A6178` | Unfreeze |
| 24 |  | **new** | [`func_00210330`](../../../src/game/world/scene/sw.cpp) | `0x210330` | VUnk23 |
| 25 | SetIdProxy | **new** | [`SetLoIdProxy`](../../../src/game/object/proxy.cpp) | `0x1CA118` | SetIdProxy |
| 26 | ResetLoForProxy | **new** | [`ResetLoForProxy?`](../../../src/game/object/proxy.cpp) | `0x1CB4E0` | VUnk25 |
| 27 | LO_VUnk27Proxy | **new** | [`LO_VUnk27Proxy?`](../../../src/game/object/proxy.cpp) | `0x1CB5C8` | VUnk26 |
| 28 | CploChildren | **new** | [`LO_CploChildren`](../../../src/game/object/lo.cpp) | `0x1A5BE8` | CploChildren |
| 29 | TranslateToPos | **new** | [`TranslateLoToPos`](../../../src/game/object/lo.cpp) | `0x1A6110` | TranslateToPos |
| 30 | RotateToMat | **new** | [`RotateLoToMat`](../../../src/game/object/lo.cpp) | `0x1A6118` | RotateToMat |
| 31 | PmatWorld | **new** | [`LO_PmatWorld`](../../../src/game/object/lo.cpp) | `0x1A62D8` | PmatWorld |
| 32 | LO_PxfLocal | **new** | [`LO_PxfLocal?`](../../../src/game/object/lo.cpp) | `0x1A6B30` | PxfLocal |
| 33 | GetVelocity | **new** | [`GetLoVelocity?`](../../../src/game/object/lo.cpp) | `0x1A6120` | GetVelocity |
| 34 | GetAngularVelocity | **new** | [`GetLoAngularVelocity`](../../../src/game/object/lo.cpp) | `0x1A6138` | GetAngularVelocity |
| 35 | SetVelocity | **new** | [`SetLoVelocity?`](../../../src/game/object/lo.cpp) | `0x1A6130` | SetVelocity |
| 36 | SetAngularVelocity | **new** | [`SetLoAngularVelocity`](../../../src/game/object/lo.cpp) | `0x1A6148` | SetAngularVelocity |
| 37 | ApplyLoProxyHierarchy | **new** | [`ApplyLoProxyHierarchy?`](../../../src/game/object/lo.cpp) | `0x1A6450` | ApplyProxy |
| 38 | MatchLoXf | **new** | [`MatchLoXf?`](../../../src/game/object/lo.cpp) | `0x1A6B88` | VUnk37 |
| 39 | SetLoXf | **new** | [`SetLoXf?`](../../../src/game/object/lo.cpp) | `0x1A6BC8` | VUnk38 |
| 40 | CloneHierarchy | **new** | [`CloneLoHierarchy`](../../../src/game/object/lo.cpp) | `0x1A6AD0` | CloneHierarchy |
| 41 | Clone | **new** | [`CloneLo`](../../../src/game/object/lo.cpp) | `0x1A6730` | Clone |
| 42 | SetLoSBusy | **new** | [`SetLoSBusy?`](../../../src/game/object/lo.cpp) | `0x1A6C48` | VUnk41 |
| 43 | LO_vt43 | **new** | [`LO_vt43?`](../../../src/game/object/lo.cpp) | `0x1A6EE8` | VUnk42 |
| 44 | LO_vt44 | **new** | [`LO_vt44?`](../../../src/game/object/lo.cpp) | `0x1A6EF0` | VUnk43 |
| 45 | LO_vt45 | **new** | [`LO_vt45?`](../../../src/game/object/lo.cpp) | `0x1A6EF8` | VUnk44 |
| 46 | SetVisMask | **new** | [`SetLoVisMask?`](../../../src/game/object/lo.cpp) | `0x1A6DC0` | SetVisMask |
| 47 | SetGrf | **new** | [`SetLoGrf?`](../../../src/game/object/lo.cpp) | `0x1A66C8` | VUnk46 |
| 48 | ClearLoGrf | **new** | [`ClearLoGrf?`](../../../src/game/object/lo.cpp) | `0x1A66F8` | VUnk47 |

## Fields

From `include/sly2/lo.h` (`struct LO : BASIC`). LO: from src/game/gameplay/player/mode.cpp (27 copies in Lilly ports)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 7 header asserts agree, 0 disagree. Size from header: 0x50.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x10 | `SO *` | `psoParent` | asserted (vptr at +0xC precedes it (CONFIRMED runtime, 8162/8162 objects)) |  |
| 0x14 | `LOROOT *` | `ploroot` | asserted (LOROOT(+0x14)->plo = topmost ancestor (CONFIRMED runtime FINDINGS)) |  |
| 0x18 | `OID` | `oid` | asserted (matched code (PloNew 0x13E3C8 family)) |  |
| 0x1C | `struct SLE` | `sleOid` | asserted (oid hash SL ibNext 0x1C (CONFIRMED runtime)) |  |
| 0x20 | `struct SLE` | `sleCid` | asserted (per-class SL ibNext 0x20 (CONFIRMED runtime)) |  |
| 0x24 | `struct DLE` | `dleChild` | asserted (child DL ibDle 0x24 (CONFIRMED runtime)) |  |
| 0x2C | `char` | `cpaloFindSwObjects` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x2D | `uchar` | `grfvis` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x2E bits 0-3 | `long long` | `nUnk48` | unknown (padding/placeholder) |  |
| 0x2E bits 4-7 | `long long` | `busyk` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x2F bits 0-0 | `ulong` | `fRemoved` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x2F bits 1-1 | `ulong` | `fUnk57` | unknown (padding/placeholder) |  |
| 0x2F bits 2-2 | `ulong` | `fUnk58` | unknown (padding/placeholder) |  |
| 0x2F bits 3-3 | `ulong` | `fUnk59` | unknown (padding/placeholder) |  |
| 0x2F bits 4-4 | `ulong` | `fInWorld` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x30 | `float` | `gUnk30` | unknown (padding/placeholder) |  |
| 0x34 | `LORUN *` | `plorunFirst` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x38 | `float` | `sBusy` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3C | `PXR *` | `ppxr` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x40 | `int` | `grfUnk40` | unknown (padding/placeholder) |  |
| 0x44 | `undefined4[3]` | `unk_0x44` | unknown (padding/placeholder) |  |

Inherited fields: see [BASIC](BASIC.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 7 |
| L03_nightclub_ep02 | 6 |
| L04_nightclub_ep03 | 6 |
| L05_nightclub_ep04 | 6 |
| L06_nightclub_ep05 | 6 |
| L07_nightclub_ep06 | 6 |
| L08_palace_ep00 | 3 |
| L09_palace_ep07 | 3 |
| L10_palace_ep08 | 3 |
| L11_palace_ep09 | 3 |
| L12_temple_ep00 | 3 |
| L13_temple_ep07 | 3 |
| L14_prison_ep00 | 4 |
| L15_prison_ep07 | 4 |
| L16_prison_ep10 | 4 |
| L17_castle_ep00 | 5 |
| L19_castle_ep11 | 5 |
| L20_castle_ep12 | 5 |
| L21_castle_ep13 | 5 |
| L22_castle_ep04 | 5 |
| L23_castle_ep14 | 5 |
| L24_castle_ep15 | 5 |
| L25_castle_ep16 | 5 |
| L26_castle_ep17 | 6 |
| L27_train_ep00 | 5 |
| L28_train_ep18 | 3 |
| L29_train_ep19 | 4 |
| L30_train_ep20 | 4 |
| L31_train_ep21 | 4 |
| L32_sawmill_ep00 | 3 |
| L33_sawmill_ep22 | 3 |
| L34_sawmill_ep23 | 3 |
| L35_sawmill_ep24 | 3 |
| L36_sawmill_ep25 | 3 |
| L37_sawmill_ep26 | 3 |
| L38_blimp_ep00 | 5 |
| L39_blimp_ep07 | 4 |
| L40_blimp_ep27 | 4 |
| L41_blimp_ep28 | 4 |
| L42_blimp_ep29 | 4 |
| L43_blimp_ep26 | 4 |
| world_elf2A38A8 | 2 |

## Related docs

[engine/object-model.md](../../engine/object-model.md) (42), [reference/class-tree.md](../../reference/class-tree.md) (25), [engine/game-progression.md](../../engine/game-progression.md) (9), [engine/overview.md](../../engine/overview.md) (7), [engine/guards-ai.md](../../engine/guards-ai.md) (6), [engine/physics-collision.md](../../engine/physics-collision.md) (5), [engine/world-sw.md](../../engine/world-sw.md) (5), [engine/animation.md](../../engine/animation.md) (4)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
