#include "m2c_prelude.h"
M2C_UNK ConvertXfVec(s32, s32, s128);              /* extern */

/** @sly2-annot addr=0x1F4870
 * Purpose: Same for rotation XF (+0x5C)
 * Info: proposed name GetSoRotXfVelocityLocal (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4870(void *arg0) {
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg0, void **, 0x5C);
    ConvertXfVec(M2C_FIELD(temp_v0, s32 *, 0xA0), M2C_FIELD(arg0, s32 *, 0x60), M2C_FIELD(temp_v0, s128 *, 0xB0));
}
