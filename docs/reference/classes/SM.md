# SM (CID 111, 0x6F)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> LoadFromBrx__2SM/PostSmLoad in slots; Lilly SM : LO

| | |
|---|---|
| Name | `SM` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [SM](SM.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x80 (128 bytes), +0x30 over LO |
| Descriptor | `D_002BB394` (record 111 of D_002BAAE8; cid range 111..112) |
| Factory (`pfnNew`) | [`NewSm`](../../../src/game/app/class_registry/NewSm.c) `0x112858` |
| Descriptor getter (slot 6) | `GetSmClassDesc` `0x114100` |
| vtable | `D_002D97A8`, 49 entries (slots 0..48; slot 0 null), 0 new, 4 overridden |
| vtable stored by | [`NewSm`](../../../src/game/app/class_registry/NewSm.c) (vptr +0xC) |
| Level instances | 2603 objects in 42 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetSmClassDesc`](../../../src/game/app/class_registry/GetSmClassDesc.c) | `0x114100` | Pclassinfo |
| 7 | Init | **override** | [`InitSm`](../../../src/game/engine/anim/sm/InitSm.c) | `0x1EFA90` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | inherited (LO) | `SetLoParent?` | `0x1A5900` | SetParent |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` | AddHierarchy |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` | RemoveHierarchy |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` | UpdateLoroot |
| 14 | LoadFromBrx | **override** | [`LoadSmFromBrx__FP2SMP18CBinaryInputStream`](../../../src/game/engine/anim/sm/LoadSmFromBrx__FP2SMP18CBinaryInputStream.cpp) | `0x1EFAC0` | LoadFromBrx |
| 15 | PostLoad | **override** | [`PostSmLoad__FP2SM`](../../../src/game/engine/anim/sm/PostSmLoad__FP2SM.cpp) | `0x1F00C8` | VUnk14 |
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

From `include/sly2/aseg.h` (`struct SM : LO`). SM: from src/game/engine/anim/sm/LoadSmFromBrx__FP2SMP18CBinaryInputStream.cpp

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 1 header asserts agree, 0 disagree. Size from header: 0x80.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x50 | `int` | `csms` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x54 | `SMS *` | `asms` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x58 | `int` | `csmt` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x5C | `SMT *` | `asmt` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x60 | `int` | `csmrt` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x64 | `SMRT *` | `asmrt` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x68 | `struct DL` | `dlSma` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x74 | `SMINIT *` | `psminit` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x78 | `undefined4[2]` | `unk_0x78` | unknown (padding/placeholder) | HYPOTHESIS: Lilly SM ends at 0x78 but the class descriptor says 0x80 |

Inherited fields: see [LO](LO.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 83 |
| L03_nightclub_ep02 | 53 |
| L04_nightclub_ep03 | 62 |
| L05_nightclub_ep04 | 51 |
| L06_nightclub_ep05 | 51 |
| L07_nightclub_ep06 | 55 |
| L08_palace_ep00 | 71 |
| L09_palace_ep07 | 54 |
| L10_palace_ep08 | 55 |
| L11_palace_ep09 | 81 |
| L12_temple_ep00 | 96 |
| L13_temple_ep07 | 62 |
| L14_prison_ep00 | 85 |
| L15_prison_ep07 | 73 |
| L16_prison_ep10 | 42 |
| L17_castle_ep00 | 90 |
| L19_castle_ep11 | 47 |
| L20_castle_ep12 | 51 |
| L21_castle_ep13 | 59 |
| L22_castle_ep04 | 73 |
| L23_castle_ep14 | 50 |
| L24_castle_ep15 | 49 |
| L25_castle_ep16 | 50 |
| L26_castle_ep17 | 49 |
| L27_train_ep00 | 72 |
| L28_train_ep18 | 56 |
| L29_train_ep19 | 57 |
| L30_train_ep20 | 67 |
| L31_train_ep21 | 52 |
| L32_sawmill_ep00 | 85 |
| L33_sawmill_ep22 | 53 |
| L34_sawmill_ep23 | 51 |
| L35_sawmill_ep24 | 61 |
| L36_sawmill_ep25 | 47 |
| L37_sawmill_ep26 | 54 |
| L38_blimp_ep00 | 87 |
| L39_blimp_ep07 | 73 |
| L40_blimp_ep27 | 52 |
| L41_blimp_ep28 | 44 |
| L42_blimp_ep29 | 44 |
| L43_blimp_ep26 | 92 |
| world_elf2A38A8 | 64 |

## Related docs

[engine/bosses.md](../../engine/bosses.md) (8), [engine/animation.md](../../engine/animation.md) (5), [engine/minigames.md](../../engine/minigames.md) (5), [engine/game-progression.md](../../engine/game-progression.md) (3), [engine/player.md](../../engine/player.md) (3), [engine/sound.md](../../engine/sound.md) (3), [formats/level-objects.md](../../formats/level-objects.md) (3), [engine/cutscenes.md](../../engine/cutscenes.md) (2)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
