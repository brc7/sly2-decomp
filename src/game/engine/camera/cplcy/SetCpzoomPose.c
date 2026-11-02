#include "m2c_prelude.h"
M2C_UNK SnapCpzoom();                            /* extern */

/** @sly2-annot addr=0x150EA0
 * Purpose: posTarget(+0x80), eulTarget(+0x90), radFovTarget(+0xA0); if not posed SnapCpzoom; fPose=1
 * Info: proposed name SetCpzoomPose (T1); TU cplcy
 * Confidence: mid (draft) [T1 0.50] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void SetCpzoomPose(void *arg0, s128 arg1, s128 arg2, f32 fparg0) {
    M2C_FIELD(arg0, s128 *, 0x80) = arg1;
    M2C_FIELD(arg0, s128 *, 0x90) = arg2;
    M2C_FIELD(arg0, f32 *, 0xA0) = fparg0;
    if (M2C_FIELD(arg0, s32 *, 0x40) == 0) {
        SnapCpzoom();
    }
    M2C_FIELD(arg0, s32 *, 0x40) = 1;
}
