#include "m2c_prelude.h"
M2C_UNK CPLCY_ctor();                            /* extern */
extern M2C_UNK D_002CD408;

/** @sly2-annot addr=0x151610
 * Purpose: CPCAMBLEND ctor (vtable D_002CD408)
 * Info: proposed name CPCAMBLEND_ctor (T1); class CPCAMBLEND; TU cplcy
 * Notes: runtime6 C7: CM+0x500 = CPCAMBLEND (prio 10, pv = JSG+0x84 CAMMOVE) during CamBlendFrom/To (S59);
 *   ctor called on cm+0x500 by 0x14D3D8
 * Confidence: mid (draft) [T1 0.75] - evidence: runtime (save-state RAM); semantic review; sources:
 *   r2_131F70+runtime
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void *CPCAMBLEND_ctor(void *arg0) {
    CPLCY_ctor();
    M2C_FIELD(arg0, M2C_UNK **, 0xC) = &D_002CD408;
    return arg0;
}
