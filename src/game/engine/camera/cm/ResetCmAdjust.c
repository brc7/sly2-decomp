#include "m2c_prelude.h"
s128 CalcCmAdjust();                               /* extern */

/** @sly2-annot addr=0x14E920
 * Purpose: dposAdjust = CalcCmAdjust(); zero vAdjust (+0x2D0) and vFocus (+0x2F0)
 * Info: proposed name ResetCmAdjust (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void ResetCmAdjust(void *arg0) {
    void *temp_v0;
    void *temp_v1;

    M2C_FIELD(arg0, s128 *, 0x2C0) = CalcCmAdjust();
    temp_v1 = arg0 + 0x2F0;
    temp_v0 = arg0 + 0x2D0;
    M2C_FIELD(temp_v0, s32 *, 8) = 0;
    M2C_FIELD(temp_v0, s32 *, 4) = 0;
    M2C_FIELD(arg0, s32 *, 0x2D0) = 0;
    M2C_FIELD(temp_v1, s32 *, 8) = 0;
    M2C_FIELD(temp_v1, s32 *, 4) = 0;
    M2C_FIELD(arg0, s32 *, 0x2F0) = 0;
}
