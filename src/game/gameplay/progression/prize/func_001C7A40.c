#include "m2c_prelude.h"
/** @sly2-annot addr=0x1C7A40
 * Purpose: small setter
 * Info: proposed name Dprize_7A40 (T2); TU prize
 * Confidence: low (draft) [T2 0.15] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
s32 func_001C7A40(void *arg0) {
    s32 *temp_v1;
    s32 var_v0;

    temp_v1 = M2C_FIELD(arg0, s32 **, 0x244);
    var_v0 = -1;
    if (temp_v1 != NULL) {
        var_v0 = *temp_v1;
    }
    return var_v0;
}
