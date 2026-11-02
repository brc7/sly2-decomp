#include "m2c_prelude.h"
/** @sly2-annot addr=0x13C478
 * Purpose: id of the current entry or -1
 * Info: proposed name IdBootyCur (T1); TU booty
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
s32 IdBootyCur(void *arg0) {
    s32 *temp_v1;
    s32 var_v0;

    temp_v1 = M2C_FIELD(arg0, s32 **, 0x158);
    var_v0 = -1;
    if (temp_v1 != NULL) {
        var_v0 = *temp_v1;
    }
    return var_v0;
}
