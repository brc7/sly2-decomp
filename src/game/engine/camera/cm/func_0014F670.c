#include "m2c_prelude.h"
M2C_UNK RecalcCmFrustrum__FP2CM();                  /* extern */

/** @sly2-annot addr=0x14F670
 * Purpose: sFarClip(+0x50)=f; RecalcCmFrustrum
 * Info: proposed name SetCmFarClip (T1); TU cm
 * Notes: names_v2 v1 proposed SetCmCut__FP2CMPf (0.000)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014F670(void *arg0, f32 fparg0) {
    M2C_FIELD(arg0, f32 *, 0x50) = fparg0;
    RecalcCmFrustrum__FP2CM();
}
