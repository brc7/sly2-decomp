#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E6330
 * Purpose: +0x178=0, cached segment=-1
 * Info: proposed name InvalidateShapeCache (T1); TU shape
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void InvalidateShapeCache(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x178) = 0;
    M2C_FIELD(arg0, s32 *, 0x148) = -1;
}
