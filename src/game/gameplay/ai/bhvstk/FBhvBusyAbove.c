#include "m2c_prelude.h"
/** @sly2-annot addr=0x1EF0A8
 * Purpose: Return cBusyAbove>0 (a behaviour higher on the stack is busy)
 * Info: proposed name FBhvBusyAbove (T1); TU bhvstk
 * Notes: names_v2 C proposed IsSwHandsOff__FP2SW (0.638)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850
 * @end-sly2-annot */
s32 FBhvBusyAbove(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x20) > 0;
}
