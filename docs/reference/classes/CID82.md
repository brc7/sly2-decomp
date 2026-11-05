# CID82 (CID 82, 0x52)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> no identification; cid_table label '? (MISSILE child)'

| | |
|---|---|
| Name | `CID82` — placeholder (decimal CID) |
| Parent chain | [CID82](CID82.md) → [MISSILE](MISSILE.md) → [BOMB](BOMB.md) → [SO](SO.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x290 (656 bytes), +0x20 over MISSILE |
| Descriptor | `D_002BB150` (record 82 of D_002BAAE8; cid range 82..83) |
| Factory (`pfnNew`) | [`NewCid82`](../../../src/game/app/class_registry.cpp) `0x111F48` |
| Descriptor getter (slot 6) | `GetCid82ClassDesc` `0x113F30` |
| vtable | `D_002D3AA0`, 79 entries (slots 0..78; slot 0 null), 0 new, 4 overridden |
| vtable stored by | [`NewCid82`](../../../src/game/app/class_registry.cpp) (vptr +0xC) |
| Level instances | 20 objects in 14 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address |
|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` |
| 5 | HandleMessage | inherited (BOMB) | `HandleBombMessage` | `0x13AAE8` |
| 6 | Pclassinfo | **override** | [`GetCid82ClassDesc`](../../../src/game/app/class_registry.cpp) | `0x113F30` |
| 7 | Init | **override** | [`InitSplmiss`](../../../src/game/gameplay/objects/missile.cpp) | `0x1B21F0` |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` |
| 10 | SetParent | inherited (SO) | `SetSoParent` | `0x1F21B8` |
| 11 | AddHierarchy | inherited (SO) | `AddSoHierarchy` | `0x1F2C30` |
| 12 | RemoveHierarchy | inherited (MISSILE) | `OnMissileRemove__FP7MISSILE` | `0x1B1E38` |
| 13 | UpdateLoroot | inherited (SO) | `UpdateSoLoroot` | `0x1F2E78` |
| 14 | LoadFromBrx | inherited (MISSILE) | `LoadMissileFromBrx__FP7MISSILEP18CBinaryInputStream` | `0x1B1E00` |
| 15 | PostLoad | inherited (BOMB) | `PostBombLoad` | `0x13AAA0` |
| 16 | Update | inherited (BOMB) | `UpdateBomb` | `0x13AB58` |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | inherited (SO) | `UpdateSoXfWorld?` | `0x1F3980` |
| 18 | UpdateLoXfWorldHierarchy | inherited (SO) | `UpdateSoXfWorldHierarchy` | `0x22FBE0` |
| 19 | RenderSelf | inherited (SO) | `RenderSoSelf` | `0x1F3C70` |
| 20 | RenderLoAll | inherited (SO) | `RenderSoAll?` | `0x1F3CC8` |
| 21 | SetLoFlag8 | inherited (SO) | `SetSoFlag8?` | `0x1F3B28` |
| 22 | OnRemove | inherited (SO) | `OnSoRemove` | `0x1F3E00` |
| 23 | OnAdd | inherited (SO) | `OnSoAdd` | `0x1F3EA8` |
| 24 |  | inherited (LO) | `func_00210330` | `0x210330` |
| 25 | SetIdProxy | inherited (SO) | `SetSoIdProxy` | `0x1CA308` |
| 26 | ResetLoForProxy | inherited (BOMB) | `ResetBomb` | `0x13BB80` |
| 27 | LO_VUnk27Proxy | inherited (SO) | `SO_VUnk27Proxy?` | `0x1CB5D0` |
| 28 | CploChildren | inherited (LO) | `LO_CploChildren` | `0x1A5BE8` |
| 29 | TranslateToPos | inherited (SO) | `TranslateSoToPos` | `0x1F40A0` |
| 30 | RotateToMat | inherited (SO) | `RotateSoToMat` | `0x1F42E0` |
| 31 | PmatWorld | inherited (SO) | `SO_PmatWorld` | `0x1F3FA0` |
| 32 | LO_PxfLocal | inherited (SO) | `SO_PxfLocal?` | `0x1F3FB0` |
| 33 | GetVelocity | inherited (SO) | `GetSoVelocity` | `0x1F4440` |
| 34 | GetAngularVelocity | inherited (SO) | `GetSoAngularVelocity` | `0x1F4558` |
| 35 | SetVelocity | inherited (SO) | `SetSoVelocity` | `0x1F4488` |
| 36 | SetAngularVelocity | inherited (SO) | `SetSoAngularVelocity` | `0x1F46D8` |
| 37 | ApplyLoProxyHierarchy | inherited (SO) | `ApplySoProxy?` | `0x1F4010` |
| 38 | MatchLoXf | inherited (SO) | `MatchSoXf?` | `0x1F49C8` |
| 39 | SetLoXf | inherited (SO) | `SetSoXf?` | `0x1F4A80` |
| 40 | CloneHierarchy | inherited (SO) | `CloneSoHierarchy` | `0x1F66F0` |
| 41 | Clone | inherited (SO) | `CloneSo` | `0x1F6068` |
| 42 | SetLoSBusy | inherited (SO) | `SetSoSBusy?` | `0x1F3118` |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` |
| 46 | SetVisMask | inherited (SO) | `SetSoVisMask` | `0x1A6DC8` |
| 47 | SetGrf | inherited (SO) | `SetSoGrf` | `0x1F2FE0` |
| 48 | ClearLoGrf | inherited (SO) | `ClearSoGrf` | `0x1F3078` |
| 49 | UpdateSoTreeBounds | inherited (SO) | `UpdateSoTreeBounds` | `0x1F50E0` |
| 50 | GetSoCpdefi | inherited (SO) | `GetSoCpdefi` | `0x1F58F0` |
| 51 | SO_PxfFromId | inherited (SO) | `SO_PxfFromId?` | `0x22D778` |
| 52 | CloneXfRef | inherited (SO) | `CloneXfRef?` | `0x1F4CA0` |
| 53 | SO_PtreeNew | inherited (SO) | `SO_PtreeNew?` | `0x1F56F8` |
| 54 | SO_vt54_Nop | inherited (SO) | `SO_vt54_Nop?` | `0x1F5988` |
| 55 | SO_vt55_Nop | inherited (SO) | `SO_vt55_Nop?` | `0x1F4FB0` |
| 56 | SO_vt56_Nop | inherited (SO) | `SO_vt56_Nop?` | `0x1F4FB8` |
| 57 | SO_vt57_Nop | inherited (SO) | `SO_vt57_Nop?` | `0x1F4FC0` |
| 58 | ScheduleSoTouchEvents | inherited (BOMB) | `BOMB_VUnk57?` | `0x13AEB0` |
| 59 | SO_vt59_Nop | inherited (SO) | `SO_vt59_Nop?` | `0x1F4FA8` |
| 60 | SO_vt60_Nop | inherited (BOMB) | `AddBombCustomXps` | `0x13ADE8` |
| 61 | FIgnoreSoIntersection | inherited (MISSILE) | `FIgnoreMissileIntersection__FP7MISSILEP2SO` | `0x1B1F40` |
| 62 | FIsSoFixed | inherited (SO) | `FIsSoFixed?` | `0x1F5040` |
| 63 | SO_vt63_Ret0 | inherited (SO) | `SO_vt63_Ret0?` | `0x1F5998` |
| 64 | ResetSoPhys | inherited (SO) | `ResetSoPhys?` | `0x1F3590` |
| 65 | ApplySoForcesXf | inherited (SO) | `ApplySoForcesXf?` | `0x1F9E70` |
| 66 | ProjectSoForce | inherited (SO) | `ProjectSoForce?` | `0x1FA328` |
| 67 | ApplySoQueuedMotion | inherited (SO) | `ApplySoQueuedMotion?` | `0x1FA528` |
| 68 | ApplySoContactImpulse | inherited (SO) | `ApplySoContactImpulse?` | `0x1FAD00` |
| 69 | GetSoVelocityAtPos | inherited (SO) | `GetSoVelocityAtPos?` | `0x1FADD0` |
| 70 | ApplySoPendingVelocity | **override** | [`PresetSplmissAccel?`](../../../src/game/gameplay/objects/missile.cpp) | `0x1B2468` |
| 71 | AddSoExternalAccelerations | inherited (SO) | `AddSoExternalAccelerations?` | `0x2297F0` |
| 72 |  | inherited (SO) | `func_002297F8` | `0x2297F8` |
| 73 | FPosInSo | inherited (SO) | `FPosInSo?` | `0x1F72F8` |
| 74 | InflictSoContactZaps | inherited (SO) | `InflictSoContactZaps?` | `0x1F7410` |
| 75 | FZprHasField44 | inherited (BOMB) | `FAbsorbBombWkr` | `0x13B2F0` |
| 76 | CloneBomb | inherited (BOMB) | `CloneBomb?` | `0x13AA70` |
| 77 | ApplyBombThrow | inherited (MISSILE) | `ExplodeMissile?` | `0x1B1F10` |
| 78 | FireMissile | **override** | [`FireSplmiss`](../../../src/game/gameplay/objects/missile.cpp) | `0x1B2228` |

## Fields

No struct for `CID82` in `include/sly2` yet. Size is known exactly (0x290); the bytes 0x270..0x290 are this class's own. Inherited fields: see [MISSILE](MISSILE.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L08_palace_ep00 | 1 |
| L11_palace_ep09 | 1 |
| L12_temple_ep00 | 3 |
| L14_prison_ep00 | 2 |
| L15_prison_ep07 | 2 |
| L17_castle_ep00 | 2 |
| L20_castle_ep12 | 1 |
| L22_castle_ep04 | 2 |
| L23_castle_ep14 | 1 |
| L24_castle_ep15 | 1 |
| L25_castle_ep16 | 1 |
| L26_castle_ep17 | 1 |
| L29_train_ep19 | 1 |
| L30_train_ep20 | 1 |

## Related docs

[engine/minigames.md](../../engine/minigames.md) (1), [reference/class-tree.md](../../reference/class-tree.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
