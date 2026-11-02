#include "m2c_prelude.h"
s32 FTaskComplete(s32);                             /* extern */

/** @sly2-annot addr=0x163400
 * Purpose: True if the job's end task (+0x68) is complete (FTaskComplete)
 * Info: proposed name FJobComplete (T1); TU game
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
s32 FJobComplete(void *arg0) {
    s32 temp_a0;
    s32 var_v0;

    temp_a0 = M2C_FIELD(arg0, s32 *, 0x68);
    var_v0 = 0;
    if (temp_a0 != 0) {
        var_v0 = FTaskComplete(temp_a0) != 0;
    }
    return var_v0;
}
