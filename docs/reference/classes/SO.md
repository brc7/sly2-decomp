# SO (CID 48, 0x30)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> Clone__2SO/GetAngularVelocity__2SO in slots; LoadFromBrx 0x1F31E0 (940 B vs Sly1 LoadSoFromBrx 944 B); Lilly SO : LO ends 0x150. Parent is now LO, not ALO

| | |
|---|---|
| Name | `SO` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [SO](SO.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | [MS](MS.md), [PO](PO.md), [BOMB](BOMB.md), [BUTTON](BUTTON.md), [SENSOR](SENSOR.md), [BARRIER](BARRIER.md), [RIPG](RIPG.md), [BARREL](BARREL.md), [WATER](WATER.md), [DPRIZE](DPRIZE.md), [CID96](CID96.md), [SKY](SKY.md), [CID98](CID98.md), [CID99](CID99.md), [CID100](CID100.md), [BOOTY](BOOTY.md), [CID104](CID104.md), [CID105](CID105.md), [CID106](CID106.md), [WAYPT](WAYPT.md), [CID108](CID108.md) |
| Descendants | 60 classes (CID 49..108) |
| Instance size (`cb`) | 0x150 (336 bytes), +0x100 over LO |
| Descriptor | `D_002BAEA8` (record 48 of D_002BAAE8; cid range 48..109) |
| Factory (`pfnNew`) | [`NewSo`](../../../src/game/app/class_registry/NewSo.cpp) `0x111588` |
| Descriptor getter (slot 6) | `GetSoClassDesc` `0x113D10` |
| vtable | `D_002D9978`, 76 entries (slots 0..75; slot 0 null), 27 new, 36 overridden |
| vtable stored by | [`JT_ctor?`](../../../src/game/app/class_registry/JT_ctor.c) (vptr +0xC), [`MURRAY_ctor?`](../../../src/game/app/class_registry/MURRAY_ctor.c) (vptr +0xC), [`NewAccmiss`](../../../src/game/app/class_registry/NewAccmiss.cpp) (vptr +0xC), [`NewBarrel`](../../../src/game/app/class_registry/NewBarrel.cpp) (vptr +0xC), [`NewBarrier`](../../../src/game/app/class_registry/NewBarrier.cpp) (vptr +0xC), [`NewBentley`](../../../src/game/app/class_registry/NewBentley.cpp) (vptr +0xC), [`NewBomb`](../../../src/game/app/class_registry/NewBomb.cpp) (vptr +0xC), [`NewBooty`](../../../src/game/app/class_registry/NewBooty.cpp) (vptr +0xC), [`NewButton`](../../../src/game/app/class_registry/NewButton.cpp) (vptr +0xC), [`NewCid100`](../../../src/game/app/class_registry/NewCid100.cpp) (vptr +0xC), [`NewCid101`](../../../src/game/app/class_registry/NewCid101.cpp) (vptr +0xC), [`NewCid102`](../../../src/game/app/class_registry/NewCid102.cpp) (vptr +0xC), [`NewCid104`](../../../src/game/app/class_registry/NewCid104.cpp) (vptr +0xC), [`NewCid105`](../../../src/game/app/class_registry/NewCid105.cpp) (vptr +0xC), [`NewCid106`](../../../src/game/app/class_registry/NewCid106.cpp) (vptr +0xC), [`NewCid108`](../../../src/game/app/class_registry/NewCid108.cpp) (vptr +0xC), [`NewCid82`](../../../src/game/app/class_registry/NewCid82.cpp) (vptr +0xC), [`NewCid96`](../../../src/game/app/class_registry/NewCid96.cpp) (vptr +0xC), [`NewCid98`](../../../src/game/app/class_registry/NewCid98.cpp) (vptr +0xC), [`NewCid99`](../../../src/game/app/class_registry/NewCid99.cpp) (vptr +0xC), [`NewClue`](../../../src/game/app/class_registry/NewClue.cpp) (vptr +0xC), [`NewCoin`](../../../src/game/app/class_registry/NewCoin.cpp) (vptr +0xC), [`NewDancer`](../../../src/game/app/class_registry/NewDancer.cpp) (vptr +0xC), [`NewHack`](../../../src/game/app/class_registry/NewHack.cpp) (vptr +0xC), [`NewHackbullet`](../../../src/game/app/class_registry/NewHackbullet.cpp) (vptr +0xC), [`NewHackenemy`](../../../src/game/app/class_registry/NewHackenemy.cpp) (vptr +0xC), [`NewHacklock`](../../../src/game/app/class_registry/NewHacklock.cpp) (vptr +0xC), [`NewHackship`](../../../src/game/app/class_registry/NewHackship.cpp) (vptr +0xC), [`NewHackwall`](../../../src/game/app/class_registry/NewHackwall.cpp) (vptr +0xC), [`NewKey`](../../../src/game/app/class_registry/NewKey.cpp) (vptr +0xC), [`NewLasen`](../../../src/game/app/class_registry/NewLasen.cpp) (vptr +0xC), [`NewLoot`](../../../src/game/app/class_registry/NewLoot.cpp) (vptr +0xC), [`NewMedicine`](../../../src/game/app/class_registry/NewMedicine.cpp) (vptr +0xC), [`NewMissile`](../../../src/game/app/class_registry/NewMissile.cpp) (vptr +0xC), [`NewMs`](../../../src/game/app/class_registry/NewMs.cpp) (vptr +0xC), [`NewPo`](../../../src/game/app/class_registry/NewPo.cpp) (vptr +0xC), [`NewRipg`](../../../src/game/app/class_registry/NewRipg.cpp) (vptr +0xC), [`NewRobottank`](../../../src/game/app/class_registry/NewRobottank.cpp) (vptr +0xC), [`NewSafehouse`](../../../src/game/app/class_registry/NewSafehouse.cpp) (vptr +0xC), [`NewSky`](../../../src/game/app/class_registry/NewSky.cpp) (vptr +0xC), [`NewSo`](../../../src/game/app/class_registry/NewSo.cpp) (vptr +0xC), [`NewStep`](../../../src/game/app/class_registry/NewStep.cpp) (vptr +0xC), [`NewTurret`](../../../src/game/app/class_registry/NewTurret.cpp) (vptr +0xC), [`NewVault`](../../../src/game/app/class_registry/NewVault.cpp) (vptr +0xC), [`NewWater`](../../../src/game/app/class_registry/NewWater.cpp) (vptr +0xC), [`NewWaypt`](../../../src/game/app/class_registry/NewWaypt.cpp) (vptr +0xC), [`PripgNew?`](../../../src/game/engine/fx/rip/PripgNew.c) (vptr +0xC), [`STEPGUARD_ctor?`](../../../src/game/app/class_registry/STEPGUARD_ctor.c) |
| Level instances | 8723 objects in 43 of 43 world files |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetSoClassDesc`](../../../src/game/app/class_registry/GetSoClassDesc.c) | `0x113D10` | Pclassinfo |
| 7 | Init | **override** | [`InitSo`](../../../src/game/engine/physics/so/InitSo.c) | `0x1F2108` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | **override** | [`SetSoParent`](../../../src/game/engine/physics/so/SetSoParent.cpp) | `0x1F21B8` | SetParent |
| 11 | AddHierarchy | **override** | [`AddSoHierarchy`](../../../src/game/engine/physics/so/AddSoHierarchy.c) | `0x1F2C30` | AddHierarchy |
| 12 | RemoveHierarchy | **override** | [`RemoveSoHierarchy`](../../../src/game/engine/physics/so/RemoveSoHierarchy.c) | `0x1F2D48` | RemoveHierarchy |
| 13 | UpdateLoroot | **override** | [`UpdateSoLoroot`](../../../src/game/engine/physics/so/UpdateSoLoroot.c) | `0x1F2E78` | UpdateLoroot |
| 14 | LoadFromBrx | **override** | [`LoadSoFromBrx`](../../../src/game/engine/physics/so/LoadSoFromBrx.cpp) | `0x1F31E0` | LoadFromBrx |
| 15 | PostLoad | **override** | [`PostSoLoad`](../../../src/game/engine/physics/so/PostSoLoad.c) | `0x1F36A8` | VUnk14 |
| 16 | Update | **override** | [`UpdateSo`](../../../src/game/engine/physics/so/UpdateSo.c) | `0x1F36E0` | VUnk15 |
| 17 | (Sly 1: ProjectTransform; LO: inherit flag 4) | **override** | [`UpdateSoXfWorld?`](../../../src/game/engine/physics/so/func_001F3980.c) | `0x1F3980` | VUnk16 |
| 18 | UpdateLoXfWorldHierarchy | **override** | [`UpdateSoXfWorldHierarchy`](../../../src/game/engine/anim/xf/UpdateSoXfWorldHierarchy.c) | `0x22FBE0` | UpdateXfWorldHierarchy |
| 19 | RenderSelf | **override** | [`RenderSoSelf`](../../../src/game/engine/physics/so/RenderSoSelf.c) | `0x1F3C70` | RenderSelf |
| 20 | RenderLoAll | **override** | [`RenderSoAll?`](../../../src/game/engine/physics/so/func_001F3CC8.c) | `0x1F3CC8` | VUnk19 |
| 21 | SetLoFlag8 | **override** | [`SetSoFlag8?`](../../../src/game/engine/physics/so/func_001F3B28.c) | `0x1F3B28` | VUnk20 |
| 22 | OnRemove | **override** | [`OnSoRemove`](../../../src/game/engine/physics/so/OnSoRemove.c) | `0x1F3E00` | Freeze |
| 23 | OnAdd | **override** | [`OnSoAdd`](../../../src/game/engine/physics/so/OnSoAdd.c) | `0x1F3EA8` | Unfreeze |
| 24 |  | inherited (LO) | `func_00210330` | `0x210330` | VUnk23 |
| 25 | SetIdProxy | **override** | [`SetSoIdProxy`](../../../src/game/object/proxy/SetSoIdProxy.c) | `0x1CA308` | SetIdProxy |
| 26 | ResetLoForProxy | **override** | [`ResetSoForProxy?`](../../../src/game/object/proxy/func_001CB518.c) | `0x1CB518` | VUnk25 |
| 27 | LO_VUnk27Proxy | **override** | [`SO_VUnk27Proxy?`](../../../src/game/object/proxy/func_001CB5D0.cpp) | `0x1CB5D0` | VUnk26 |
| 28 | CploChildren | inherited (LO) | `LO_CploChildren` | `0x1A5BE8` | CploChildren |
| 29 | TranslateToPos | **override** | [`TranslateSoToPos`](../../../src/game/engine/physics/so/TranslateSoToPos.cpp) | `0x1F40A0` | TranslateToPos |
| 30 | RotateToMat | **override** | [`RotateSoToMat`](../../../src/game/engine/physics/so/RotateSoToMat.cpp) | `0x1F42E0` | RotateToMat |
| 31 | PmatWorld | **override** | [`SO_PmatWorld`](../../../src/game/engine/physics/so/SO_PmatWorld.c) | `0x1F3FA0` | PmatWorld |
| 32 | LO_PxfLocal | **override** | [`SO_PxfLocal?`](../../../src/game/engine/physics/so/func_001F3FB0.c) | `0x1F3FB0` | PxfLocal |
| 33 | GetVelocity | **override** | [`GetSoVelocity`](../../../src/game/engine/physics/so/GetSoVelocity.c) | `0x1F4440` | GetVelocity |
| 34 | GetAngularVelocity | **override** | [`GetSoAngularVelocity`](../../../src/game/engine/physics/so/GetSoAngularVelocity.cpp) | `0x1F4558` | GetAngularVelocity |
| 35 | SetVelocity | **override** | [`SetSoVelocity`](../../../src/game/engine/physics/so/SetSoVelocity.c) | `0x1F4488` | SetVelocity |
| 36 | SetAngularVelocity | **override** | [`SetSoAngularVelocity`](../../../src/game/engine/physics/so/SetSoAngularVelocity.cpp) | `0x1F46D8` | SetAngularVelocity |
| 37 | ApplyLoProxyHierarchy | **override** | [`ApplySoProxy?`](../../../src/game/engine/physics/so/func_001F4010.c) | `0x1F4010` | ApplyProxy |
| 38 | MatchLoXf | **override** | [`MatchSoXf?`](../../../src/game/engine/physics/so/func_001F49C8.cpp) | `0x1F49C8` | VUnk37 |
| 39 | SetLoXf | **override** | [`SetSoXf?`](../../../src/game/engine/physics/so/func_001F4A80.cpp) | `0x1F4A80` | VUnk38 |
| 40 | CloneHierarchy | **override** | [`CloneSoHierarchy`](../../../src/game/engine/physics/so/CloneSoHierarchy.c) | `0x1F66F0` | CloneHierarchy |
| 41 | Clone | **override** | [`CloneSo`](../../../src/game/engine/physics/so/CloneSo.cpp) | `0x1F6068` | Clone |
| 42 | SetLoSBusy | **override** | [`SetSoSBusy?`](../../../src/game/engine/physics/so/func_001F3118.cpp) | `0x1F3118` | VUnk41 |
| 43 | LO_vt43 | inherited (LO) | `LO_vt43?` | `0x1A6EE8` | VUnk42 |
| 44 | LO_vt44 | inherited (LO) | `LO_vt44?` | `0x1A6EF0` | VUnk43 |
| 45 | LO_vt45 | inherited (LO) | `LO_vt45?` | `0x1A6EF8` | VUnk44 |
| 46 | SetVisMask | **override** | [`SetSoVisMask`](../../../src/game/object/lo/SetSoVisMask.c) | `0x1A6DC8` | SetVisMask |
| 47 | SetGrf | **override** | [`SetSoGrf`](../../../src/game/engine/physics/so/SetSoGrf.c) | `0x1F2FE0` | VUnk46 |
| 48 | ClearLoGrf | **override** | [`ClearSoGrf`](../../../src/game/engine/physics/so/ClearSoGrf.c) | `0x1F3078` | VUnk47 |
| 49 | UpdateSoTreeBounds | **new** | [`UpdateSoTreeBounds`](../../../src/game/engine/physics/so/UpdateSoTreeBounds.cpp) | `0x1F50E0` | VUnk48 |
| 50 | GetSoCpdefi | **new** | [`GetSoCpdefi`](../../../src/game/engine/physics/so/GetSoCpdefi.c) | `0x1F58F0` | GetCpdefi |
| 51 | SO_PxfFromId | **new** | [`SO_PxfFromId?`](../../../src/game/engine/anim/xf/func_0022D778.c) | `0x22D778` | PxfFromIxf |
| 52 | CloneXfRef | **new** | [`CloneXfRef?`](../../../src/game/engine/physics/so/func_001F4CA0.c) | `0x1F4CA0` | CloneXfRef |
| 53 | SO_PtreeNew | **new** | [`SO_PtreeNew?`](../../../src/game/engine/physics/so/func_001F56F8.c) | `0x1F56F8` | VUnk52 |
| 54 | SO_vt54_Nop | **new** | [`SO_vt54_Nop?`](../../../src/game/engine/physics/so/func_001F5988.c) | `0x1F5988` | VUnk53 |
| 55 | SO_vt55_Nop | **new** | [`SO_vt55_Nop?`](../../../src/game/engine/physics/so/func_001F4FB0.c) | `0x1F4FB0` | AdjustNewXp |
| 56 | SO_vt56_Nop | **new** | [`SO_vt56_Nop?`](../../../src/game/engine/physics/so/func_001F4FB8.c) | `0x1F4FB8` | VUnk55 |
| 57 | SO_vt57_Nop | **new** | [`SO_vt57_Nop?`](../../../src/game/engine/physics/so/func_001F4FC0.c) | `0x1F4FC0` | VUnk56 |
| 58 | ScheduleSoTouchEvents | **new** | [`ScheduleSoTouchEvents?`](../../../src/game/engine/physics/so/func_001F4F10.c) | `0x1F4F10` | VUnk57 |
| 59 | SO_vt59_Nop | **new** | [`SO_vt59_Nop?`](../../../src/game/engine/physics/so/func_001F4FA8.c) | `0x1F4FA8` | VUnk58 |
| 60 | SO_vt60_Nop | **new** | [`SO_vt60_Nop?`](../../../src/game/engine/physics/so/func_001F5990.c) | `0x1F5990` | VUnk59 |
| 61 | FIgnoreSoIntersection | **new** | [`FIgnoreSoIntersection`](../../../src/game/engine/physics/so/FIgnoreSoIntersection.c) | `0x1F4FC8` | FIgnoreIntersection |
| 62 | FIsSoFixed | **new** | [`FIsSoFixed?`](../../../src/game/engine/physics/so/func_001F5040.c) | `0x1F5040` | VUnk61 |
| 63 | SO_vt63_Ret0 | **new** | [`SO_vt63_Ret0?`](../../../src/game/engine/physics/so/func_001F5998.c) | `0x1F5998` | CpartExtra |
| 64 | ResetSoPhys | **new** | [`ResetSoPhys?`](../../../src/game/engine/physics/so/func_001F3590.c) | `0x1F3590` | InitPhys |
| 65 | ApplySoForcesXf | **new** | [`ApplySoForcesXf?`](../../../src/game/engine/physics/solve/func_001F9E70.cpp) | `0x1F9E70` | VUnk64 |
| 66 | ProjectSoForce | **new** | [`ProjectSoForce?`](../../../src/game/engine/physics/solve/func_001FA328.c) | `0x1FA328` | VUnk65 |
| 67 | ApplySoQueuedMotion | **new** | [`ApplySoQueuedMotion?`](../../../src/game/engine/physics/solve/func_001FA528.cpp) | `0x1FA528` | VUnk66 |
| 68 | ApplySoContactImpulse | **new** | [`ApplySoContactImpulse?`](../../../src/game/engine/physics/solve/func_001FAD00.c) | `0x1FAD00` | VUnk67 |
| 69 | GetSoVelocityAtPos | **new** | [`GetSoVelocityAtPos?`](../../../src/game/engine/physics/solve/func_001FADD0.c) | `0x1FADD0` | VUnk68 |
| 70 | ApplySoPendingVelocity | **new** | [`ApplySoPendingVelocity?`](../../../src/game/engine/physics/solve/func_001F9B20.c) | `0x1F9B20` | VUnk69 |
| 71 | AddSoExternalAccelerations | **new** | [`AddSoExternalAccelerations?`](../../../src/game/engine/physics/water/AddSoExternalAccelerations.c) | `0x2297F0` | VUnk70 |
| 72 |  | **new** | [`func_002297F8`](../../../src/game/engine/physics/water/func_002297F8.cpp) | `0x2297F8` | VUnk71 |
| 73 | FPosInSo | **new** | [`FPosInSo?`](../../../src/game/engine/physics/so/func_001F72F8.c) | `0x1F72F8` | VUnk72 |
| 74 | InflictSoContactZaps | **new** | [`InflictSoContactZaps?`](../../../src/game/engine/physics/so/func_001F7410.c) | `0x1F7410` | ProcessSoZapContacts |
| 75 | FZprHasField44 | **new** | [`FZprHasField44?`](../../../src/game/engine/physics/so/func_001F7588.c) | `0x1F7588` | FTakeDamage |

## Fields

From `include/sly2/so.h` (`struct SO : LO`). SO: from src/game/gameplay/player/jt/JtChooseGroundState.cpp (23 copies in Lilly ports)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 4 header asserts agree, 0 disagree. Size from header: 0x150.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x50 | `int` | `cxf` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x54 | `XF *` | `axf` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x58 | `XF *` | `pxfUnk58` | unknown (padding/placeholder) |  |
| 0x5C | `XF *` | `pxfUnk5c` | unknown (padding/placeholder) |  |
| 0x60 | `XF *` | `pxfRoot` | asserted (position = pxfRoot->matWorld row 3 (CONFIRMED runtime)) |  |
| 0x64 | `struct DL` | `dlChild` | asserted (child DL at SO+0x64 (CONFIRMED runtime)) |  |
| 0x70 | `LO *` | `ploRemovedFirst` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x74 | `undefined4[3]` | `unk_0x74` | unknown (padding/placeholder) |  |
| 0x80 | `struct GLOBSET` | `globset` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xE0 | `int` | `cpart` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xE4 | `SOPART *` | `apart` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xE8 | `SOPHYS *` | `psophys` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xEC | `SOTREE *` | `ptree` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xF0 | `qword` | `qwDvContact` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x100 | `qword` | `qwDwContact` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x110 | `uchar[26]` | `agrfZpk` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x12A | `undefined1[2]` | `unk_0x12a` | unknown (padding/placeholder) |  |
| 0x12C | `float` | `uAlpha` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x130 | `float` | `duAlpha` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x134 | `struct SOBOUNDS *` | `psobounds` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x138 | `XA *` | `pxa` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x13C | `float` | `gUnk13c` | asserted (holds 400.0f (CONFIRMED runtime)) |  |
| 0x140 | `int` | `nStampUnk140` | unknown (padding/placeholder) |  |
| 0x144 | `undefined4` | `unk_0x144` | unknown (padding/placeholder) |  |
| 0x148 | `char` | `bUnk148` | unknown (padding/placeholder) |  |
| 0x149 | `char` | `bUnk149` | unknown (padding/placeholder) |  |
| 0x14A bits 0-3 | `ulong` | `unk_0x14a` | unknown (padding/placeholder) |  |
| 0x14A bits 4-7 | `long long` | `nUnk14a` | unknown (padding/placeholder) |  |
| 0x14B bits 0-1 | `ulong` | `nZapMode` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x14B bits 2-2 | `ulong` | `fIgnoreFixed` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x14B bits 3-4 | `ulong` | `unk_0x14c` | unknown (padding/placeholder) |  |
| 0x14B bits 5-5 | `ulong` | `fNoJtPlace` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x14B bits 6-8 | `ulong` | `unk_0x14d` | unknown (padding/placeholder) |  |
| 0x14C bits 1-1 | `ulong` | `fHurtRespawns` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x14C bits 2-2 | `ulong` | `fTreeRoot` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x14C bits 3-3 | `ulong` | `fUnk35` | unknown (padding/placeholder) |  |
| 0x14C bits 4-4 | `ulong` | `fHasIkLimbs` | header (Lilly-ported; offset computed, unasserted) |  |

Inherited fields: see [LO](LO.md).

## Level instances

From the whole-game object census (every object record of all 43 world files).

| Level | Objects |
|---|---|
| L02_nightclub_ep00 | 345 |
| L03_nightclub_ep02 | 193 |
| L04_nightclub_ep03 | 229 |
| L05_nightclub_ep04 | 184 |
| L06_nightclub_ep05 | 190 |
| L07_nightclub_ep06 | 153 |
| L08_palace_ep00 | 287 |
| L09_palace_ep07 | 193 |
| L10_palace_ep08 | 174 |
| L11_palace_ep09 | 164 |
| L12_temple_ep00 | 350 |
| L13_temple_ep07 | 239 |
| L14_prison_ep00 | 358 |
| L15_prison_ep07 | 223 |
| L16_prison_ep10 | 102 |
| L17_castle_ep00 | 336 |
| L19_castle_ep11 | 113 |
| L20_castle_ep12 | 137 |
| L21_castle_ep13 | 146 |
| L22_castle_ep04 | 265 |
| L23_castle_ep14 | 121 |
| L24_castle_ep15 | 121 |
| L25_castle_ep16 | 129 |
| L26_castle_ep17 | 122 |
| L27_train_ep00 | 275 |
| L28_train_ep18 | 173 |
| L29_train_ep19 | 201 |
| L30_train_ep20 | 205 |
| L31_train_ep21 | 176 |
| L32_sawmill_ep00 | 318 |
| L33_sawmill_ep22 | 157 |
| L34_sawmill_ep23 | 149 |
| L35_sawmill_ep24 | 159 |
| L36_sawmill_ep25 | 100 |
| L37_sawmill_ep26 | 124 |
| L38_blimp_ep00 | 378 |
| L39_blimp_ep07 | 241 |
| L40_blimp_ep27 | 165 |
| L41_blimp_ep28 | 150 |
| L42_blimp_ep29 | 133 |
| L43_blimp_ep26 | 407 |
| world_elf2A37E8 | 2 |
| world_elf2A38A8 | 336 |

## Related docs

[engine/physics-collision.md](../../engine/physics-collision.md) (31), [reference/class-tree.md](../../reference/class-tree.md) (23), [formats/meshes-scene.md](../../formats/meshes-scene.md) (13), [engine/gameplay-objects.md](../../engine/gameplay-objects.md) (11), [engine/object-model.md](../../engine/object-model.md) (9), [formats/level-objects.md](../../formats/level-objects.md) (9), [engine/player.md](../../engine/player.md) (8), [engine/animation.md](../../engine/animation.md) (7)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
