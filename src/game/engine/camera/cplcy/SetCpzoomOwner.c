#include "m2c_prelude.h"
M2C_UNK SnapCpzoom();                            /* extern */

/** @sly2-annot addr=0x150E20
 * Purpose: ploOwner(+0x10)=plo; if posed, SnapCpzoom; fPose=0
 * Info: proposed name SetCpzoomOwner (T1); TU cplcy
 * Confidence: mid (draft) [T1 0.50] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void SetCpzoomOwner(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0x10) = arg1;
    if (M2C_FIELD(arg0, s32 *, 0x40) != 0) {
        SnapCpzoom();
    }
    M2C_FIELD(arg0, s32 *, 0x40) = 0;
}
