#include "m2c_prelude.h"
M2C_UNK func_00230598(s32, s32);                    /* extern */

/** @sly2-annot addr=0x1F88D8
 * Purpose: optionally refresh XF kind (arg==1) and return XF (+0x24)
 * Info: proposed name PxfFromXfh (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
s32 func_001F88D8(void *arg0, s32 arg1) {
    if (arg1 == 1) {
        func_00230598(M2C_FIELD(arg0, s32 *, 0x20), M2C_FIELD(arg0, s32 *, 0x24));
    }
    return M2C_FIELD(arg0, s32 *, 0x24);
}
