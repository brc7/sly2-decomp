#include "m2c_prelude.h"
M2C_UNK func_0014F610();                            /* extern */

/** @sly2-annot addr=0x14F630
 * Purpose: Option setter g_aeopid[164]: SetCmNearClip and the active policy's saved near clip (pcplcy+8)
 * Info: proposed name SetCmBaseNearClip (T1); TU cm
 * Confidence: mid (draft) [T1 0.50] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014F630(void *arg0, f32 fparg0) {
    void *temp_v0;

    func_0014F610();
    temp_v0 = M2C_FIELD(arg0, void **, 0x314);
    if (temp_v0 != NULL) {
        M2C_FIELD(temp_v0, f32 *, 8) = fparg0;
    }
}
