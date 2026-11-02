# TN (CID 15, 0x0F)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> Lilly TN : XFM ends 0x1C0; own code in TU 0x220D40 ("tn" after thread)

| | |
|---|---|
| Name | `TN` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [TN](TN.md) → [XFM](XFM.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x1C0 (448 bytes), +0xE0 over XFM |
| Descriptor | `D_002BAC14` (record 15 of D_002BAAE8; cid range 15..16) |
| Factory (`pfnNew`) | [`NewTn`](../../../src/game/app/class_registry/NewTn.c) `0x110DE0` |
| Descriptor getter (slot 6) | `GetTnClassDesc` `0x113B00` |
| vtable | `D_002DB6B8`, 51 entries (slots 0..50; slot 0 null), 0 new, 8 overridden |
| vtable stored by | [`NewTn`](../../../src/game/app/class_registry/NewTn.c) (vptr +0xC) |
| Level instances | 619 objects in 42 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetTnClassDesc`](../../../src/game/app/class_registry/GetTnClassDesc.c) | `0x113B00` | Pclassinfo |
| 7 | Init | **override** | [`InitTn?`](../../../src/game/engine/camera/tn/InitTn.c) | `0x220ED0` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | inherited (XFM) | `SetXfmParent` | `0x2315F0` | SetParent |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` | AddHierarchy |
| 12 | RemoveHierarchy | **override** | [`RemoveTnHierarchy?`](../../../src/game/engine/camera/tn/RemoveTnHierarchy.c) | `0x220F88` | RemoveHierarchy |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` | UpdateLoroot |
| 14 | LoadFromBrx | **override** | [`LoadTnFromBrx?`](../../../src/game/engine/camera/tn/LoadTnFromBrx.c) | `0x220FD0` | LoadFromBrx |
| 15 | PostLoad | inherited (LO) | `PostLoLoad` | `0x1A5C48` | VUnk14 |
| 16 | Update | **override** | [`UpdateTn?`](../../../src/game/engine/camera/tn/UpdateTn.c) | `0x221388` | VUnk15 |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (LO) | `LO_vt17_InheritFlag4?` | `0x1A5E80` | VUnk16 |
| 18 | UpdateLoXfWorldHierarchy | inherited (LO) | `UpdateLoXfWorldHierarchy` | `0x22FBD8` | UpdateXfWorldHierarchy |
| 19 | RenderSelf | inherited (LO) | `RenderLoSelf` | `0x1A5ED8` | RenderSelf |
| 20 | RenderLoAll | inherited (LO) | `RenderLoAll?` | `0x1A5EE0` | VUnk19 |
| 21 | SetLoFlag8 | inherited (LO) | `SetLoFlag8?` | `0x1A6E80` | VUnk20 |
| 22 | OnRemove | **override** | [`OnTnRemove?`](../../../src/game/engine/camera/tn/OnTnRemove.c) | `0x2213D8` | Freeze |
| 23 | OnAdd | inherited (LO) | `OnLoAdd` | `0x1A6178` | Unfreeze |
| 24 |  | inherited (LO) | `func_00210330` | `0x210330` | VUnk23 |
| 25 | SetIdProxy | inherited (LO) | `SetLoIdProxy` | `0x1CA118` | SetIdProxy |
| 26 | ResetLoForProxy | inherited (LO) | `ResetLoForProxy?` | `0x1CB4E0` | VUnk25 |
| 27 | LO_VUnk27Proxy | inherited (LO) | `LO_VUnk27Proxy?` | `0x1CB5C8` | VUnk26 |
| 28 | CploChildren | **override** | [`FGetTnPshape?`](../../../src/game/engine/camera/tn/func_00221120.c) | `0x221120` | CploChildren |
| 29 | TranslateToPos | inherited (XFM) | `TranslateXfmToPos` | `0x2312A0` | TranslateToPos |
| 30 | RotateToMat | inherited (XFM) | `RotateXfmToMat` | `0x231300` | RotateToMat |
| 31 | PmatWorld | inherited (XFM) | `XFM_PmatWorld` | `0x231440` | PmatWorld |
| 32 | LO_PxfLocal | inherited (XFM) | `XFM_PxfLocal?` | `0x231550` | PxfLocal |
| 33 | GetVelocity | inherited (XFM) | `GetXfmVelocity?` | `0x231468` | GetVelocity |
| 34 | GetAngularVelocity | inherited (XFM) | `GetXfmAngularVelocity?` | `0x2314D0` | GetAngularVelocity |
| 35 | SetVelocity | inherited (LO) | `SetLoVelocity?` | `0x1A6130` | SetVelocity |
| 36 | SetAngularVelocity | inherited (LO) | `SetLoAngularVelocity` | `0x1A6148` | SetAngularVelocity |
| 37 | ApplyLoProxyHierarchy | inherited (XFM) | `ApplyXfmProxy?` | `0x231598` | ApplyProxy |
| 38 | MatchLoXf | inherited (LO) | `MatchLoXf?` | `0x1A6B88` | VUnk37 |
| 39 | SetLoXf | inherited (LO) | `SetLoXf?` | `0x1A6BC8` | VUnk38 |
| 40 | CloneHierarchy | inherited (LO) | `CloneLoHierarchy` | `0x1A6AD0` | CloneHierarchy |
| 41 | Clone | **override** | [`CloneTn?`](../../../src/game/engine/camera/tn/CloneTn.c) | `0x221140` | Clone |
| 42 | SetLoSBusy | inherited (LO) | `SetLoSBusy?` | `0x1A6C48` | VUnk41 |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` | VUnk42 |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` | VUnk43 |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` | VUnk44 |
| 46 | SetVisMask | inherited (LO) | `SetLoVisMask?` | `0x1A6DC0` | SetVisMask |
| 47 | SetGrf | inherited (LO) | `SetLoGrf?` | `0x1A66C8` | VUnk46 |
| 48 | ClearLoGrf | inherited (LO) | `ClearLoGrf?` | `0x1A66F8` | VUnk47 |
| 49 | XFM_PxfFromId | inherited (XFM) | `XFM_PxfFromId?` | `0x22D750` | VSlot49 |
| 50 |  | inherited (XFM) | `func_00231220` | `0x231220` | VSlot50 |

## Fields

From `include/sly2/xfm.h` (`struct TN : XFM`). TN: from src/game/engine/camera/tn/func_00221DD8.cpp

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 1 header asserts agree, 0 disagree. Size from header: 0x1C0.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0xE0 | `LO *` | `ploHero` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xE4 | `struct GEOM` | `geom` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xFC | `struct BSPC` | `bspc` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x10C | `SHAPE *` | `pshape` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x110 | `int` | `ccpd` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x114 | `CPD *` | `acpd` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x118 | `CPD *` | `pcpd` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x11C | `int` | `fCpdsLoaded` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x120 | `int` | `fCylind` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x124 | `int` | `nUnk124` | unknown (padding/placeholder) |  |
| 0x128 | `int` | `nPriorityTn` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x12C | `int` | `fCircle` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x130 | `float` | `radPanLimit` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x134 | `TNS` | `tns` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x138 | `float` | `tTns` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x13C | `TNS` | `tnsWant` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x140 | `float` | `uProgress` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x144 | `undefined4[3]` | `unk_0x144` | unknown (padding/placeholder) |  |
| 0x150 | `struct VECTOR4` | `posCenter` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x160 | `float` | `sRadius` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x164 | `int` | `grftnm` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x168 | `int` | `fPullEnter` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x16C | `int` | `fPullIdle` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x170 | `float` | `dtPullEnter` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x174 | `float` | `gPullEnter` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x178 | `float` | `dtPullIdle` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x17C | `float` | `gPullIdle` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x180 | `int` | `nTurnAxis` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x184 | `int` | `nUnk184` | unknown (padding/placeholder) |  |
| 0x188 | `float` | `radFov` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x18C | `int` | `fNoCamClip` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x190 | `LO *` | `ploPull` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x194 | `undefined4[3]` | `unk_0x194` | unknown (padding/placeholder) |  |
| 0x1A0 | `struct CLQ` | `clqPull` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1B0 | `struct LM` | `lmPull` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1B8 | `undefined4[2]` | `unk_0x1b8` | unknown (padding/placeholder) |  |

Inherited fields: see [XFM](XFM.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 26 |
| L03_nightclub_ep02 | 9 |
| L04_nightclub_ep03 | 10 |
| L05_nightclub_ep04 | 15 |
| L06_nightclub_ep05 | 6 |
| L07_nightclub_ep06 | 6 |
| L08_palace_ep00 | 35 |
| L09_palace_ep07 | 15 |
| L10_palace_ep08 | 12 |
| L11_palace_ep09 | 7 |
| L12_temple_ep00 | 19 |
| L13_temple_ep07 | 10 |
| L14_prison_ep00 | 53 |
| L15_prison_ep07 | 13 |
| L16_prison_ep10 | 5 |
| L17_castle_ep00 | 41 |
| L19_castle_ep11 | 7 |
| L20_castle_ep12 | 9 |
| L21_castle_ep13 | 8 |
| L22_castle_ep04 | 22 |
| L23_castle_ep14 | 6 |
| L24_castle_ep15 | 6 |
| L25_castle_ep16 | 7 |
| L26_castle_ep17 | 6 |
| L27_train_ep00 | 35 |
| L28_train_ep18 | 11 |
| L29_train_ep19 | 14 |
| L30_train_ep20 | 21 |
| L31_train_ep21 | 17 |
| L32_sawmill_ep00 | 35 |
| L33_sawmill_ep22 | 6 |
| L34_sawmill_ep23 | 7 |
| L35_sawmill_ep24 | 11 |
| L36_sawmill_ep25 | 8 |
| L37_sawmill_ep26 | 6 |
| L38_blimp_ep00 | 35 |
| L39_blimp_ep07 | 8 |
| L40_blimp_ep27 | 7 |
| L41_blimp_ep28 | 7 |
| L42_blimp_ep29 | 7 |
| L43_blimp_ep26 | 9 |
| world_elf2A38A8 | 22 |

## Related docs

[engine/camera.md](../../engine/camera.md) (5), [engine/bosses.md](../../engine/bosses.md) (4), [engine/player.md](../../engine/player.md) (3), [formats/level-objects.md](../../formats/level-objects.md) (2), [engine/minigames.md](../../engine/minigames.md) (1), [engine/overview.md](../../engine/overview.md) (1), [engine/physics-collision.md](../../engine/physics-collision.md) (1), [formats/meshes-scene.md](../../formats/meshes-scene.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
