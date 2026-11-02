# CM (CID 135, 0x87)

[Class reference index](index.md) · [Class tree](../class-tree.md) · [Object model](../../engine/object-model.md)

> CM

| | |
|---|---|
| Name | `CM` — high: Sucker Punch name (Sly 1 / Lilly) |
| Parent chain | [CM](CM.md) → [BASIC](BASIC.md) |
| Children | (none) |
| Instance size (`cb`) | 0x630 (1584 bytes), +0x620 over BASIC |
| Descriptor | `D_002BB574` (record 135 of D_002BAAE8; cid range 135..136) |
| Factory (`pfnNew`) | [`NewCm`](../../../src/auto/NewCm.c) `0x112DA8` |
| Descriptor getter (slot 6) | `GetCmClassDesc` `0x114280` |
| vtable | `D_002CA4D0`, 7 entries (slots 0..6; slot 0 null), 0 new, 1 overridden |
| vtable stored by | [`CM_ctor?`](../../../src/game/engine/camera/cm/func_0014D3D8.c) (vptr +0xC) |
| Level instances | none (not a world-file object class) |

## Virtual table

Status: **new** = slot first appears in this class; **override** = differs from the parent; inherited slots name the class that owns the implementation. `?` = proposed name (master_names), not yet in the repo.

| Slot | Role | Status | Implementation | Address | Header decl |
|---|---|---|---|---|---|
| 1 | Incref-style hook | inherited (BASIC) | `BASIC_VUnk00?` | `0x132B00` | VUnk00 |
| 2 | Decref-style hook | inherited (BASIC) | `BASIC_VUnk01?` | `0x132B08` | VUnk01 |
| 3 | HandleSpliceEvent | inherited (BASIC) | `HandleBasicSpliceEvent` | `0x1331F8` | HandleSpliceEvent |
| 4 | SendMessage | inherited (BASIC) | `SendLoMessage` | `0x133508` | SendMessage |
| 5 | HandleMessage | inherited (BASIC) | `HandleBasicMessage` | `0x133590` | HandleMessage |
| 6 | Pclassinfo | **override** | [`GetCmClassDesc`](../../../src/auto/GetCmClassDesc.c) | `0x114280` | Pclassinfo |

## Fields

From `include/sly2/cm.h` (`struct CM : CMBASE`). CM: from src/game/engine/camera/cm/func_0014DC58.cpp (6 copies in Lilly ports)

Offsets computed by clang from the header (EE ABI: 32-bit pointers, 64-bit long, 16-byte qword); 9 header asserts agree, 0 disagree. Size from header: 0x630.

| Offset | Type | Field | Confidence | Note |
|---|---|---|---|---|
| 0x10 | `struct VISZONE *` | `pzonVis` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x14 | `uint` | `grfvis` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x18 | `float` | `rFovCullRef` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1C | `float` | `rCullScale` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x20 | `float` | `fov` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x24 | `float` | `fovTarget` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x28 | `float` | `dfov` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x2C | `float` | `rUnk2c` | unknown (padding/placeholder) |  |
| 0x30 | `float` | `rUnk30` | unknown (padding/placeholder) |  |
| 0x34 | `float` | `rUnk34` | unknown (padding/placeholder) |  |
| 0x38 | `float` | `rUnk38` | unknown (padding/placeholder) |  |
| 0x3C | `float` | `rAspect` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x40 | `float` | `gTanHalfX` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x44 | `float` | `gTanHalfY` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x48 | `float` | `sNearCorner` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x4C | `float` | `sNearClip` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x50 | `float` | `sFarClip` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x54 | `uint32_t[3]` | `_pad_558` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x60 | `union CM::(anonymous at /tmp/gencls_g2fnul5h/sly2/cm.h:212:7) ` | `union CM::(anonymous at /tmp/gencls_g2fnul5h/sly2/cm.h:212:7) ` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xA0 | `float[12]` | `agFrustumXyz` | header (Lilly-ported; offset computed, unasserted) |  |
| 0xD0 | `float[12]` | `agFrustumWideXyz` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x100 | `struct MATRIX4` | `matProj` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x140 | `struct MATRIX4` | `matViewProj` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x180 | `struct MATRIX4` | `matViewProjInv` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x1C0 | `qword[4]` | `aplaneFrustum` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x200 | `qword[4]` | `aplaneFrustumWide` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x240 | `float` | `sNearFog` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x244 | `float` | `sFarFog` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x248 | `float` | `uFogMax` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x24C | `struct RGBA` | `rgbaFog` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x250 | `float[4]` | `agRecipFit` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x260 | `SO *` | `psoAdjust` | asserted (CONFIRMED runtime) | +0x260 [R] camera focus SO = the current player in gameplay (runtime3/6) |
| 0x264 | `uint32_t[3]` | `_pad_581` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x270 | `struct CPDEFI` | `cpdefi` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x2C0 | `qword` | `dposAdjust` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x2D0 | `struct VECTOR` | `vAdjust` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x2DC | `uint32_t[1]` | `_pad_585` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x2E0 | `qword` | `dposFocus` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x2F0 | `struct VECTOR` | `vFocus` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x2FC | `uint32_t[1]` | `_pad_588` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x300 | `int` | `nCut` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x304 | `float` | `radCutPan` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x308 | `float` | `radCutTilt` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x30C | `int` | `ccpr` | asserted (CONFIRMED runtime) | +0x30C [R] depth of the policy stack acpr (ram_inspector "policy stack count"); was cmodeHero |
| 0x310 | `CPR[8]` | `acpr` | asserted (policy stack acpr[8] of 16-byte CPR (CONFIRMED runtime)) | +0x310 [R] policy stack {prio, pcplcy, psoFocus, pv}, 16 bytes each; top = highest prio |
| 0x390 | `float` | `tModeStart` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x394 | `uint32_t[3]` | `_pad_595` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x3A0 | `struct CMPART1` | `part3a0` | asserted (bottom policy CMPART1 (CONFIRMED runtime)) |  |
| 0x3B0 | `struct CMPART2` | `part3b0` | asserted (upper policy with TN pv (CONFIRMED runtime)) |  |
| 0x450 | `struct CPLOOK` | `cplook` | asserted (CONFIRMED runtime (vptr seen in policy stack)) | +0x450 [R] look-around policy (prio 7, focus JT in S31; vptr 0x2CD4E0) |
| 0x4A8 | `struct CPCAMERA` | `part4a8` | header (Lilly-ported; offset computed, unasserted) | +0x4A8 [R] fixed-camera policy (prio 10 on a CID 0x2C marker, runtime7a) |
| 0x4C0 | `struct CPHANG` | `part4c0` | header (Lilly-ported; offset computed, unasserted) | +0x4C0 [R offset / S type] embedded policy pushed at prio 10 |
| 0x500 | `struct CPCAMBLEND` | `cpcamblend` | asserted (CONFIRMED runtime) | +0x500 [R] cutscene blend policy (prio 10, pv = a CAMMOVE in the JSG); was part500 |
| 0x520 | `struct CPZOOM` | `cpzoom` | asserted (CONFIRMED runtime) | +0x520 [R] recon camera (prio 8 while JT mode kind 1, binocucom) |
| 0x5F0 | `struct CPTURRET` | `part5f0` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x600 | `struct CPTWOSHOT` | `part600` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x618 | `float` | `tJolt` | header (Lilly-ported; offset computed, unasserted) |  |
| 0x61C | `int` | `nUnk61c` | unknown (padding/placeholder) |  |
| 0x620 | `float` | `rUnk620` | unknown (padding/placeholder) |  |
| 0x624 | `float` | `rUnk624` | unknown (padding/placeholder) |  |

Inherited fields: see [BASIC](BASIC.md).

## Related docs

[engine/camera.md](../../engine/camera.md) (13), [engine/overview.md](../../engine/overview.md) (4), [engine/player.md](../../engine/player.md) (4), [engine/main-loop-startup.md](../../engine/main-loop-startup.md) (3), [open-questions.md](../../open-questions.md) (3), [reference/class-tree.md](../../reference/class-tree.md) (3), [engine/cutscenes.md](../../engine/cutscenes.md) (2), [engine/rendering.md](../../engine/rendering.md) (2)

---
*Generated by `gen_classes.py` from the ELF (descriptor table D_002BAAE8, vtables), the repo names and headers. Re-run after renames.*
