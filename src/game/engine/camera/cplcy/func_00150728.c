#include "m2c_prelude.h"
M2C_UNK func_0014FCA8();                            /* extern */
extern M2C_UNK D_002CD498;

/** @sly2-annot addr=0x150728
 * Purpose: CPZOOM ctor (vtable D_002CD498): rZoomMax(+0xB4)=7.0, uZoomTarget(+0x18)=-1, sounds 0
 * Info: proposed name CPZOOM_ctor (T1); class CPZOOM; TU cplcy
 * Notes: runtime6 C7: CM+0x520 = CPZOOM = the recon camera (prio 8 while JTMODE kind 1, S57); ctor called on
 *   cm+0x520 by 0x14D3D8
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); semantic review; sources:
 *   r2_131F70+runtime
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void *func_00150728(void *arg0) {
    func_0014FCA8();
    M2C_FIELD(arg0, M2C_UNK **, 0xC) = &D_002CD498;
    M2C_FIELD(arg0, f32 *, 0xB4) = 7.0f;
    M2C_FIELD(arg0, f32 *, 0x18) = -1.0f;
    M2C_FIELD(arg0, s32 *, 0xC0) = 0;
    M2C_FIELD(arg0, s32 *, 0xC4) = 0;
    return arg0;
}
