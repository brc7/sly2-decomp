#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E6630
 * Purpose: return times[c-1]
 * Info: proposed name UMaxShape (T1); TU shape
 * Notes: names_v2 B proposed UMaxCrv__FP3CRV (0.856)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
f32 func_001E6630(void *arg0) {
    return M2C_FIELD(((M2C_FIELD(arg0, s32 *, 0xE8) * 4) + M2C_FIELD(arg0, s32 *, 0xEC)), f32 *, -4);
}
