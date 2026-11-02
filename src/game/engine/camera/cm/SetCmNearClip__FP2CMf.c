#include "m2c_prelude.h"
M2C_UNK RecalcCmFrustrum__FP2CM();                  /* extern */

/** @sly2-annot addr=0x14F610
 * Purpose: sNearClip(+0x4C)=f; RecalcCmFrustrum
 * Info: proposed name SetCmNearClip (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmNearClip__FP2CMf(void *arg0, f32 fparg0) {
    M2C_FIELD(arg0, f32 *, 0x4C) = fparg0;
    RecalcCmFrustrum__FP2CM();
}
