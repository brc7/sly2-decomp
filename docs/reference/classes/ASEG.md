# ASEG (CID 109, 0x6D)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> InitAseg/LoadAsegFromBrx in slots; Lilly ASEG : LO

| | |
|---|---|
| Name | `ASEG` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [ASEG](ASEG.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | [ASEGBL](ASEGBL.md) |
| Instance size (`cb`) | 0xC0 (192 bytes), +0x70 over LO |
| Descriptor | `D_002BB36C` (record 109 of D_002BAAE8; cid range 109..111) |
| Factory (`pfnNew`) | [`NewAseg`](../../../src/game/app/class_registry/NewAseg.c) `0x1127E8` |
| Descriptor getter (slot 6) | `GetAsegClassDesc` `0x1140E0` |
| vtable | `D_002CAD78`, 49 entries (slots 0..48; slot 0 null), 0 new, 4 overridden |
| vtable stored by | [`NewAseg`](../../../src/game/app/class_registry/NewAseg.c) (vptr +0xC) |
| Level instances | 38560 objects in 42 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetAsegClassDesc`](../../../src/game/app/class_registry/GetAsegClassDesc.c) | `0x1140E0` | Pclassinfo |
| 7 | Init | **override** | [`InitAseg`](../../../src/game/engine/anim/aseg.cpp) | `0x12F248` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | inherited (LO) | `SetLoParent?` | `0x1A5900` | SetParent |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` | AddHierarchy |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` | RemoveHierarchy |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` | UpdateLoroot |
| 14 | LoadFromBrx | **override** | [`LoadAsegFromBrx`](../../../src/game/engine/anim/aseg.cpp) | `0x12F2A8` | LoadFromBrx |
| 15 | PostLoad | **override** | [`PostAsegLoad`](../../../src/game/engine/anim/aseg.cpp) | `0x12F8F0` | VUnk14 |
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
| 29 | TranslateToPos | inherited (LO) | `TranslateLoToPos` | `0x1A6110` | TranslateToPos |
| 30 | RotateToMat | inherited (LO) | `RotateLoToMat` | `0x1A6118` | RotateToMat |
| 31 | PmatWorld | inherited (LO) | `LO_PmatWorld` | `0x1A62D8` | PmatWorld |
| 32 | LO_PxfLocal | inherited (LO) | `LO_PxfLocal?` | `0x1A6B30` | PxfLocal |
| 33 | GetVelocity | inherited (LO) | `GetLoVelocity?` | `0x1A6120` | GetVelocity |
| 34 | GetAngularVelocity | inherited (LO) | `GetLoAngularVelocity` | `0x1A6138` | GetAngularVelocity |
| 35 | SetVelocity | inherited (LO) | `SetLoVelocity?` | `0x1A6130` | SetVelocity |
| 36 | SetAngularVelocity | inherited (LO) | `SetLoAngularVelocity` | `0x1A6148` | SetAngularVelocity |
| 37 | ApplyLoProxyHierarchy | inherited (LO) | `ApplyLoProxyHierarchy?` | `0x1A6450` | ApplyProxy |
| 38 | MatchLoXf | inherited (LO) | `MatchLoXf?` | `0x1A6B88` | VUnk37 |
| 39 | SetLoXf | inherited (LO) | `SetLoXf?` | `0x1A6BC8` | VUnk38 |
| 40 | CloneHierarchy | inherited (LO) | `CloneLoHierarchy` | `0x1A6AD0` | CloneHierarchy |
| 41 | Clone | inherited (LO) | `CloneLo` | `0x1A6730` | Clone |
| 42 | SetLoSBusy | inherited (LO) | `SetLoSBusy?` | `0x1A6C48` | VUnk41 |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` | VUnk42 |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` | VUnk43 |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` | VUnk44 |
| 46 | SetVisMask | inherited (LO) | `SetLoVisMask?` | `0x1A6DC0` | SetVisMask |
| 47 | SetGrf | inherited (LO) | `SetLoGrf?` | `0x1A66C8` | VUnk46 |
| 48 | ClearLoGrf | inherited (LO) | `ClearLoGrf?` | `0x1A66F8` | VUnk47 |

## Fields

From `include/sly2/aseg.h` (`struct ASEG : LO`). ASEG: from src/game/gameplay/player/stepplayer.cpp (16 copies in Lilly ports)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 3 header asserts agree, 0 disagree. Size from header: 0xC0.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x50 | `struct DL` | `dlAsega` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x5C | `int` | `cchn` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x60 | `CHN *` | `achn` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x64 | `int` | `covr` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x68 | `OVR *` | `aovr` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x6C | `int` | `nPriority` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x70 | `int` | `nWorldHold` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x74 | `float` | `tMax` | asserted (clip 0x11D tMax 0.833 (anim_lab, CONFIRMED parse)) | +0x74 [R] clip length (s) (anim_lab: 0.833 for 0x11D, parse) |
| 0x78 | `float` | `svtMaster` | asserted (ApplyAseg copies it (CONFIRMED code+runtime)) | +0x78 [R] playback-speed multiplier copied into ASEGA::svtMaster (option 109 HYPOTHESIS) |
| 0x7C | `uint32_t[1]` | `_pad_263` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x80 | `struct CLQ` | `clqMasterSuck` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x90 | `struct LM` | `lmMasterSuck` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x98 | `int` | `ceaStart` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x9C | `EA *` | `aeaStart` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xA0 | `int` | `ceaFrame` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xA4 | `EA *` | `aeaFrame` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xA8 | `int` | `ceaEnd` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xAC | `EA *` | `aeaEnd` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xB0 | `ASEGSTART *` | `pasegstart` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xB4 | `uint32_t[3]` | `_pad_273` | header (Lilly-ported; offset computed, unasserted) |  |

Inherited fields: see [LO](LO.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 1104 |
| L03_nightclub_ep02 | 829 |
| L04_nightclub_ep03 | 857 |
| L05_nightclub_ep04 | 815 |
| L06_nightclub_ep05 | 784 |
| L07_nightclub_ep06 | 829 |
| L08_palace_ep00 | 980 |
| L09_palace_ep07 | 873 |
| L10_palace_ep08 | 896 |
| L11_palace_ep09 | 1321 |
| L12_temple_ep00 | 1358 |
| L13_temple_ep07 | 957 |
| L14_prison_ep00 | 1087 |
| L15_prison_ep07 | 955 |
| L16_prison_ep10 | 725 |
| L17_castle_ep00 | 1223 |
| L19_castle_ep11 | 778 |
| L20_castle_ep12 | 810 |
| L21_castle_ep13 | 811 |
| L22_castle_ep04 | 1088 |
| L23_castle_ep14 | 777 |
| L24_castle_ep15 | 778 |
| L25_castle_ep16 | 780 |
| L26_castle_ep17 | 779 |
| L27_train_ep00 | 1067 |
| L28_train_ep18 | 907 |
| L29_train_ep19 | 923 |
| L30_train_ep20 | 1045 |
| L31_train_ep21 | 885 |
| L32_sawmill_ep00 | 1079 |
| L33_sawmill_ep22 | 790 |
| L34_sawmill_ep23 | 787 |
| L35_sawmill_ep24 | 896 |
| L36_sawmill_ep25 | 742 |
| L37_sawmill_ep26 | 864 |
| L38_blimp_ep00 | 1053 |
| L39_blimp_ep07 | 989 |
| L40_blimp_ep27 | 818 |
| L41_blimp_ep28 | 735 |
| L42_blimp_ep29 | 734 |
| L43_blimp_ep26 | 1055 |
| world_elf2A38A8 | 997 |

## Related docs

[engine/animation.md](../../engine/animation.md) (12), [engine/guards-ai.md](../../engine/guards-ai.md) (4), [engine/minigames.md](../../engine/minigames.md) (4), [formats/level-objects.md](../../formats/level-objects.md) (3), [formats/splice-scripts.md](../../formats/splice-scripts.md) (3), [engine/overview.md](../../engine/overview.md) (2), [engine/world-sw.md](../../engine/world-sw.md) (2), [reference/class-tree.md](../../reference/class-tree.md) (2)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
