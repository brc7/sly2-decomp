#include "m2c_prelude.h"
/** @sly2-annot addr=0x14F5F8
 * Purpose: Option setter g_aeopid[162] (field +0x24): fovTarget=f and the active policy's saved fov
 *   (acpr[0].pcplcy+4) so Deactivate restores it
 * Info: proposed name SetCmBaseFovTarget (T1); TU cm
 * Confidence: mid (draft) [T1 0.50] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014F5F8(void *arg0, f32 fparg0) {
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg0, void **, 0x314);
    M2C_FIELD(arg0, f32 *, 0x24) = fparg0;
    if (temp_v0 != NULL) {
        M2C_FIELD(temp_v0, f32 *, 4) = fparg0;
    }
}
