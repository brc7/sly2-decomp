#include "m2c_prelude.h"
/** @sly2-annot addr=0x187DD0
 * Purpose: Return x10B8 (pressed this frame) & mask
 * Info: proposed name GrfJoyPressed (T1); TU joy
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/main-loop-startup.md#joy-0x187030-0x188488
 * @end-sly2-annot */
s32 GrfJoyPressed(void *arg0, s32 arg1) {
    return M2C_FIELD(arg0, s32 *, 0x10B8) & arg1;
}
