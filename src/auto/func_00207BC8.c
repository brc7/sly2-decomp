#include "m2c_prelude.h"
s32 func_00207B10(s32);                             /* extern */
M2C_UNK func_00207B70(s32, s32, s32, s32);          /* extern */
M2C_UNK func_00209130(s32, s32);                    /* extern */
s32 func_00209C18(s32, s32);                        /* extern */

/** @sly2-annot addr=0x207BC8
 * Purpose: Same as 0x207B70 then resolve it (0x209130)
 * Info: proposed name SetStepguardAnimBindingResolve (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00207BC8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_s1;

    temp_s1 = func_00209C18(arg0, func_00207B10(arg1));
    func_00207B70(arg0, arg1, arg2, arg3);
    func_00209130(temp_s1, arg0);
}
