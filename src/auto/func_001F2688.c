#include "m2c_prelude.h"
/** @sly2-annot addr=0x1F2688
 * Purpose: Store float at +0x13C and mirror it into the shared bounds record if this SO owns it
 * Info: proposed name SetSoBoundsScale (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F2688(void *arg0, f32 fparg0) {
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0x134);
    M2C_FIELD(arg0, f32 *, 0x13C) = fparg0;
    if ((temp_v1 != NULL) && (M2C_FIELD(temp_v1, s32 *, 0) == arg0)) {
        M2C_FIELD(temp_v1, f32 *, 0x20) = fparg0;
    }
}
