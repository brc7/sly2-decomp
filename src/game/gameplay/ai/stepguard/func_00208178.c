#include "m2c_prelude.h"
M2C_UNK ReleaseAnimBinding(s32, s32);                    /* extern */

/** @sly2-annot addr=0x208178
 * Purpose: Release all 26 entries of a table (0x2090D0)
 * Info: proposed name ReleaseZapReactions (T2); TU stepguard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00208178(s32 arg0, s32 arg1) {
    s32 var_s0;
    s32 var_s1;

    var_s0 = arg1;
    var_s1 = 0x19;
    do {
        ReleaseAnimBinding(var_s0, arg0);
        var_s1 -= 1;
        var_s0 += 0x10;
    } while (var_s1 >= 0);
}
