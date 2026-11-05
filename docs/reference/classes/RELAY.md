# RELAY (CID 114, 0x72)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> RELAY (cutscene / sequence script)

| | |
|---|---|
| Name | `RELAY` — medium: team coinage (v1 identified) |
| Parent chain | [RELAY](RELAY.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0xB0 (176 bytes), +0x60 over LO |
| Descriptor | `D_002BB3D0` (record 114 of D_002BAAE8; cid range 114..115) |
| Factory (`pfnNew`) | [`NewRelay`](../../../src/game/app/class_registry/NewRelay.c) `0x112900` |
| Descriptor getter (slot 6) | `GetRelayClassDesc` `0x114130` |
| vtable | `D_002D8C48`, 49 entries (slots 0..48; slot 0 null), 0 new, 5 overridden |
| vtable stored by | [`NewRelay`](../../../src/game/app/class_registry/NewRelay.c) (vptr +0xC) |
| Level instances | 3871 objects in 42 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` |
| 6 | Pclassinfo | **override** | [`GetRelayClassDesc`](../../../src/game/app/class_registry/GetRelayClassDesc.c) | `0x114130` |
| 7 | Init | **override** | [`InitRelay?`](../../../src/game/gameplay/cutscene/relay.cpp) | `0x1DFAD8` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (LO) | `SetLoParent?` | `0x1A5900` |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` |
| 14 | LoadFromBrx | **override** | [`LoadRelayFromBrx?`](../../../src/game/gameplay/cutscene/relay.cpp) | `0x1DFB10` |
| 15 | PostLoad | inherited (LO) | `PostLoLoad` | `0x1A5C48` |
| 16 | Update | **override** | [`UpdateRelay?`](../../../src/game/gameplay/cutscene/relay.cpp) | `0x1DFC50` |
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
| 41 | Clone | **override** | [`CloneRelay?`](../../../src/game/gameplay/cutscene/relay.cpp) | `0x1DFBF8` |
| 42 | SetLoSBusy | inherited (LO) | `SetLoSBusy?` | `0x1A6C48` |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` |
| 46 | SetVisMask | inherited (LO) | `SetLoVisMask?` | `0x1A6DC0` |
| 47 | SetGrf | inherited (LO) | `SetLoGrf?` | `0x1A66C8` |
| 48 | ClearLoGrf | inherited (LO) | `ClearLoGrf?` | `0x1A66F8` |

## Fields

No struct for `RELAY` in `include/sly2` yet. Size is known exactly (0xB0); the bytes 0x50..0xB0 are this class's own. Inherited fields: see [LO](LO.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 98 |
| L03_nightclub_ep02 | 82 |
| L04_nightclub_ep03 | 90 |
| L05_nightclub_ep04 | 78 |
| L06_nightclub_ep05 | 80 |
| L07_nightclub_ep06 | 79 |
| L08_palace_ep00 | 88 |
| L09_palace_ep07 | 79 |
| L10_palace_ep08 | 74 |
| L11_palace_ep09 | 70 |
| L12_temple_ep00 | 110 |
| L13_temple_ep07 | 109 |
| L14_prison_ep00 | 121 |
| L15_prison_ep07 | 118 |
| L16_prison_ep10 | 117 |
| L17_castle_ep00 | 102 |
| L19_castle_ep11 | 74 |
| L20_castle_ep12 | 83 |
| L21_castle_ep13 | 76 |
| L22_castle_ep04 | 80 |
| L23_castle_ep14 | 76 |
| L24_castle_ep15 | 76 |
| L25_castle_ep16 | 76 |
| L26_castle_ep17 | 76 |
| L27_train_ep00 | 72 |
| L28_train_ep18 | 64 |
| L29_train_ep19 | 52 |
| L30_train_ep20 | 65 |
| L31_train_ep21 | 53 |
| L32_sawmill_ep00 | 169 |
| L33_sawmill_ep22 | 154 |
| L34_sawmill_ep23 | 161 |
| L35_sawmill_ep24 | 158 |
| L36_sawmill_ep25 | 155 |
| L37_sawmill_ep26 | 153 |
| L38_blimp_ep00 | 84 |
| L39_blimp_ep07 | 74 |
| L40_blimp_ep27 | 71 |
| L41_blimp_ep28 | 70 |
| L42_blimp_ep29 | 76 |
| L43_blimp_ep26 | 71 |
| world_elf2A38A8 | 57 |

## Related docs

[engine/cutscenes.md](../../engine/cutscenes.md) (17), [engine/bosses.md](../../engine/bosses.md) (15), [engine/minigames.md](../../engine/minigames.md) (10), [engine/game-progression.md](../../engine/game-progression.md) (7), [formats/splice-scripts.md](../../formats/splice-scripts.md) (4), [guides/devtools.md](../../guides/devtools.md) (4), [engine/overview.md](../../engine/overview.md) (2), [engine/player.md](../../engine/player.md) (2)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
