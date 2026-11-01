#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E6620
 * Purpose: return dists[0] (+0xF0)
 * Info: proposed name SMinShape (T1); TU shape
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
f32 SMinShape(void *arg0) {
    return *M2C_FIELD(arg0, f32 **, 0xF0);
}
