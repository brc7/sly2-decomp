#include "m2c_prelude.h"
/** @sly2-annot addr=0x1BBE18
 * Purpose: current stage index valid (0..count-1 or -1)
 * Info: proposed name FCid98StageValid (T2); class CID98; TU tu_1BAB50
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
s32 func_001BBE18(void *arg0) {
    s32 temp_v1;
    s32 var_a1;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0x15C);
    var_a1 = 0;
    if (((temp_v1 >= 0) && (temp_v1 < M2C_FIELD(arg0, s32 *, 0x160))) || (temp_v1 == -1)) {
        var_a1 = 1;
    }
    return var_a1;
}
