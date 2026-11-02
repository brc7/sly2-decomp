# ASEGBL (CID 110, 0x6E)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> Lilly ASEGBL : ASEG (Sly1: LO sibling)

| | |
|---|---|
| Name | `ASEGBL` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [ASEGBL](ASEGBL.md) → [ASEG](ASEG.md) → [LO](LO.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0xE0 (224 bytes), +0x20 over ASEG |
| Descriptor | `D_002BB380` (record 110 of D_002BAAE8; cid range 110..111) |
| Factory (`pfnNew`) | [`NewAsegbl`](../../../src/game/app/class_registry/NewAsegbl.c) `0x112820` |
| Descriptor getter (slot 6) | `GetAsegblClassDesc` `0x1140F0` |
| vtable | `D_002CA300`, 49 entries (slots 0..48; slot 0 null), 0 new, 1 overridden |
| vtable stored by | [`NewAsegbl`](../../../src/game/app/class_registry/NewAsegbl.c) (vptr +0xC) |
| Level instances | none (not a world-file object class) |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (LO) | `HandleLoSpliceEvent` | `0x1A61E0` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetAsegblClassDesc`](../../../src/game/app/class_registry/GetAsegblClassDesc.c) | `0x1140F0` | Pclassinfo |
| 7 | Init | inherited (ASEG) | `InitAseg` | `0x12F248` | Init |
| 8 | Add | inherited (LO) | `AddLo` | `0x1A5478` | Add |
| 9 | Remove | inherited (LO) | `RemoveLo` | `0x1A5630` | Remove |
| 10 | SetParent | inherited (LO) | `SetLoParent?` | `0x1A5900` | SetParent |
| 11 | AddHierarchy | inherited (LO) | `AddLoHierarchy` | `0x1A5800` | AddHierarchy |
| 12 | RemoveHierarchy | inherited (LO) | `RemoveLoHierarchy` | `0x1A5848` | RemoveHierarchy |
| 13 | UpdateLoroot | inherited (LO) | `UpdateLoroot?` | `0x1A5B80` | UpdateLoroot |
| 14 | LoadFromBrx | inherited (ASEG) | `LoadAsegFromBrx` | `0x12F2A8` | LoadFromBrx |
| 15 | PostLoad | inherited (ASEG) | `PostAsegLoad` | `0x12F8F0` | VUnk14 |
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

From `include/sly2/aseg.h` (`struct ASEGBL : ASEG`). ASEGBL: from src/game/gameplay/player/stepplayer/HeroInteractDistance.cpp (16 copies in Lilly ports)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 1 header asserts agree, 0 disagree. Size from header: 0xE0.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0xC0 | `int` | `cbBl` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xC4 | `int` | `cbl` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xC8 | `void *` | `abl` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xCC | `int` | `cmrsgc` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xD0 | `MRSGC *` | `amrsgc` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xD4 | `int` | `iblSolo` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xD8 | `ASEG *` | `pasegEvents` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xDC | `uint32_t[1]` | `_pad_333` | header (Lilly-ported; offset computed, unasserted) |  |

Inherited fields: see [ASEG](ASEG.md).

## Related docs

[engine/animation.md](../../engine/animation.md) (4), [formats/level-objects.md](../../formats/level-objects.md) (1), [reference/class-tree.md](../../reference/class-tree.md) (1), [reference/glossary.md](../../reference/glossary.md) (1)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
