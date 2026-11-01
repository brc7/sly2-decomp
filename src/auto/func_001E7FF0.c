#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E7FF0
 * Purpose: chain ? chain->+0x4C : -1
 * Info: proposed name GetShapeChainMode (T2); TU shape
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
s32 func_001E7FF0(void *arg0) {
    s32 var_v0;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0x17C);
    var_v0 = -1;
    if (temp_v1 != NULL) {
        var_v0 = M2C_FIELD(temp_v1, s32 *, 0x4C);
    }
    return var_v0;
}
