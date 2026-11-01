# BHV (CID 155, 0x9B)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> BHV (behaviour base)

| | |
|---|---|
| Name | `BHV` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [BHV](BHV.md) → [BASIC](BASIC.md) |
| Children | [BHVGUARD](BHVGUARD.md), [BHVANIM](BHVANIM.md), [BHVATTACK](BHVATTACK.md), [BHVCOMBAT](BHVCOMBAT.md), [BHVPATH](BHVPATH.md), [BHV169](BHV169.md), [BHVPATROL](BHVPATROL.md), [BHV171](BHV171.md), [BHVLEAP](BHVLEAP.md), [BHV173](BHV173.md), [BHV174](BHV174.md), [BHV175](BHV175.md), [BHV176](BHV176.md), [BHVJUGGLED](BHVJUGGLED.md), [BHV178](BHV178.md), [BHV179](BHV179.md), [BHV180](BHV180.md), [BHV181](BHV181.md), [BHV182](BHV182.md), [BHV183](BHV183.md), [BHVRAIL](BHVRAIL.md), [BHVPIPE](BHVPIPE.md), [BHVGOTO](BHVGOTO.md), [BHV187](BHV187.md), [BHV188](BHV188.md), [BHV189](BHV189.md), [BHVPATHLINK](BHVPATHLINK.md), [BHV191](BHV191.md), [BHVPATROLPATH](BHVPATROLPATH.md), [BHV193](BHV193.md), [BHVFOLLOW](BHVFOLLOW.md), [BHV195](BHV195.md), [BHV196](BHV196.md), [BHV197](BHV197.md), [BHV198](BHV198.md) |
| Descendants | 43 classes (CID 156..198) |
| Instance size (`cb`) | 0x30 (48 bytes), +0x20 over BASIC |
| Descriptor | `D_002BB704` (record 155 of D_002BAAE8; cid range 155..199) |
| Factory (`pfnNew`) | [`NewBhv`](../../../src/auto/NewBhv.c) `0x113028` |
| Descriptor getter (slot 6) | `GetBhvClassDesc` `0x1143C0` |
| vtable | `D_002D9480`, 15 entries (slots 0..14; slot 0 null), 8 new, 1 overridden |
| vtable stored by | [`NewBhv`](../../../src/auto/NewBhv.c) (vptr +0xC) |
| Level instances | none (not a world-file object class) |

- **Hidden-type note (HYPOTHESIS):** candidate unregistered intermediate under BHV shared by BHVANIM, BHVLEAP, BHV175 (medium, 2 exclusive identical slots) — [hidden-types A1](../hidden-types.md#a1)
- **Hidden-type note (HYPOTHESIS):** candidate unregistered intermediate under BHV shared by BHVANIM, BHVLEAP, BHV175, BHVJUGGLED (medium, 1 exclusive identical slots) — [hidden-types A2](../hidden-types.md#a2)

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (BASIC) | `HandleBasicSpliceEvent` | `0x1331F8` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetBhvClassDesc`](../../../src/auto/GetBhvClassDesc.c) | `0x1143C0` | Pclassinfo |
| 7 | Init | **new** | [`InitBhv`](../../../src/auto/InitBhv.c) | `0x1EEF68` | OnPush |
| 8 | Start | **new** | [`StartBhv`](../../../src/auto/StartBhv.c) | `0x1EEF70` | Start |
| 9 | Close | **new** | [`CloseBhv`](../../../src/auto/CloseBhv.c) | `0x1EEF78` | VSlot9 |
| 10 | Update | **new** | [`UpdateBhv`](../../../src/auto/UpdateBhv.c) | `0x1EEF98` | VSlot10 |
| 11 | UpdateMotion | **new** | [`UpdateBhvMotion`](../../../src/auto/UpdateBhvMotion.c) | `0x1EEFB8` | VSlot11 |
| 12 | GrfGroundProbe | **new** | [`BHV_GrfGroundProbe?`](../../../src/auto/func_001EEFA0.c) | `0x1EEFA0` | VSlot12 |
| 13 | FHandleMessage | **new** | [`BHV_FHandleMessage`](../../../src/auto/BHV_FHandleMessage.c) | `0x1EF0B8` | VSlot13 |
| 14 | UpdateInput | **new** | [`UpdateBhvInput?`](../../../src/auto/func_001EF0C0.c) | `0x1EF0C0` | VSlot14 |

## Fields

From `include/sly2/step.h` (`struct BHV : BASIC`). BHV: from src/hand/func_001E1E30.cpp (9 copies in Lilly ports)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 1 header asserts agree, 0 disagree. Size from header: 0x30.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x10 | `STEP *` | `pstep` | header (Lilly-ported; offset computed, unasserted) | +0x10 [R] owner STEP (== the guard/partner whose bhvstk holds it, all states) |
| 0x14 | `BHV *` | `pbhvPrev` | header (Lilly-ported; offset computed, unasserted) | +0x14 [S] |
| 0x18 | `BHV *` | `pbhvNext` | header (Lilly-ported; offset computed, unasserted) | +0x18 [S] |
| 0x1C | `int` | `fBusy` | header (Lilly-ported; offset computed, unasserted) | +0x1C [R] behaviour busy (shown with '*' in ram_inspector stacks) |
| 0x20 | `int` | `cBusyAbove` | header (Lilly-ported; offset computed, unasserted) | +0x20 [R] number of busy behaviours above this one in the stack |
| 0x24 | `undefined4[3]` | `unk_0x24` | unknown (padding/placeholder) |  |

Inherited fields: see [BASIC](BASIC.md).

## Related docs

[reference/class-tree.md](../../reference/class-tree.md) (39), [engine/guards-ai.md](../../engine/guards-ai.md) (11), [engine/player.md](../../engine/player.md) (4), [guides/naming-workflow.md](../../guides/naming-workflow.md) (4), [open-questions.md](../../open-questions.md) (3), [README.md](../../README.md) (2), [engine/overview.md](../../engine/overview.md) (2), [guides/devtools.md](../../guides/devtools.md) (2)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
