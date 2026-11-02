#include "m2c_prelude.h"
/** @sly2-annot addr=0x1F5040
 * Purpose: SO vt62: true if no SOPHYS or both XF modes (+0xA0/+0xC0) == 3
 * Info: proposed name FIsSoFixed (T2); class SO slot62; TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
s32 func_001F5040(void *arg0) {
    s32 temp_a0;
    s32 var_a1;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0xE8);
    var_a1 = 0;
    if ((temp_v1 == NULL) || ((temp_a0 = M2C_FIELD(temp_v1, s32 *, 0xC0), (temp_a0 == 3)) && (M2C_FIELD(temp_v1, s32 *, 0xA0) == temp_a0))) {
        var_a1 = 1;
    }
    return var_a1;
}
