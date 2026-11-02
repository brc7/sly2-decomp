#include "m2c_prelude.h"
/** @sly2-annot addr=0x14B670
 * Purpose: size (+4) of a file record if kind (+8) == 2 else 0
 * Info: proposed name CbFileRecord (T2); TU tu_14B660
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/core-library.md#tu_14b660-0x14b660-0x14b7f8,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
s32 func_0014B670(void *arg0) {
    s32 var_a1;

    var_a1 = 0;
    if (M2C_FIELD(arg0, s32 *, 8) == 2) {
        var_a1 = M2C_FIELD(arg0, s32 *, 4);
    }
    return var_a1;
}
