#include "m2c_prelude.h"
/** @sly2-annot addr=0x221960
 * Purpose: tnfn.vecFocus.x = y = u, z = 1.0
 * Info: proposed name SetTnFocusRatio (T1); TU tn
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
void func_00221960(void *arg0, f32 fparg0) {
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0x118);
    M2C_FIELD(temp_v1, f32 *, 0x74) = fparg0;
    M2C_FIELD(temp_v1, f32 *, 0x70) = fparg0;
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x118), f32 *, 0x78) = 1.0f;
}
