#include "m2c_prelude.h"
M2C_UNK ActivateCplcy();                            /* extern */

/** @sly2-annot addr=0x151BF0
 * Purpose: CPLCY::Activate; +0x14=psoFocus arg, +0x10=pv arg
 * Info: proposed name ActivateCptwoshot (T1); TU cplcy
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void ActivateCptwoshot(void *arg0, s32 arg1, s32 arg2) {
    ActivateCplcy();
    M2C_FIELD(arg0, s32 *, 0x14) = arg1;
    M2C_FIELD(arg0, s32 *, 0x10) = arg2;
}
