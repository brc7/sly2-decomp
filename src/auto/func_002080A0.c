#include "m2c_prelude.h"
M2C_UNK func_00209130(s32, s32);                    /* extern */

/** @sly2-annot addr=0x2080A0
 * Purpose: Resolve all 26 entries of a table (0x209130)
 * Info: proposed name ResolveZapReactions (T1); TU stepguard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7b_guard+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_002080A0(s32 arg0, s32 arg1) {
    s32 var_s0;
    s32 var_s1;

    var_s0 = arg1;
    var_s1 = 0x19;
    do {
        func_00209130(var_s0, arg0);
        var_s1 -= 1;
        var_s0 += 0x10;
    } while (var_s1 >= 0);
}
