#include "m2c_prelude.h"
M2C_UNK RecalcCmFrustrum__FP2CM();                  /* extern */

/** @sly2-annot addr=0x14F730
 * Purpose: rMRD ratio (+0x18)=f; RecalcCmFrustrum (which derives +0x1C = max(r/fov,1))
 * Info: proposed name SetCmMrdRatio (T1); TU cm
 * Notes: names_v2 v1 proposed ClearCmCut__FP2CM (0.000)
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmMrdRatio__FP2CMf(void *arg0, f32 fparg0) {
    M2C_FIELD(arg0, f32 *, 0x18) = fparg0;
    RecalcCmFrustrum__FP2CM();
}
