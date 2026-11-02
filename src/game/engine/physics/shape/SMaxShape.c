#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E6648
 * Purpose: return dists[c-1] (total length)
 * Info: proposed name SMaxShape (T1); TU shape
 * Notes: names_v2 B proposed SMaxCrv__FP3CRV (0.856)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r6b_rest+verdicts_followups
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
f32 SMaxShape(void *arg0) {
    return M2C_FIELD(((M2C_FIELD(arg0, s32 *, 0xE8) * 4) + M2C_FIELD(arg0, s32 *, 0xF0)), f32 *, -4);
}
