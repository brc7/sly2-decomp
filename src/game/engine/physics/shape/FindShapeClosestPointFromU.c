#include "m2c_prelude.h"
M2C_UNK func_001E6A30();                            /* extern */
M2C_UNK func_001E6DB8();                            /* extern */

/** @sly2-annot addr=0x1E6ED8
 * Purpose: Dispatch on mode to the linear/curve closest-point search
 * Info: proposed name FindShapeClosestPointFromU (T1); TU shape
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void FindShapeClosestPointFromU(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0xE0) == 0) {
        func_001E6A30();
        return;
    }
    func_001E6DB8();
}
