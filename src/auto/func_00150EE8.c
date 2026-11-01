#include "m2c_prelude.h"
/** @sly2-annot addr=0x150EE8
 * Purpose: uZoomTarget(+0x18)=g
 * Info: proposed name SetCpzoomUZoomTarget (T1); TU cplcy
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void func_00150EE8(void *arg0, f32 fparg0) {
    M2C_FIELD(arg0, f32 *, 0x18) = fparg0;
}
