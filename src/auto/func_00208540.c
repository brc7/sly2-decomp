#include "m2c_prelude.h"
s32 func_001F8278(s32);                             /* extern */
M2C_UNK func_0020A730(s32, void *);                 /* extern */
M2C_UNK func_0020A790(s32, void *);                 /* extern */

/** @sly2-annot addr=0x208540
 * Purpose: Change current target +0x38C: leave the old target's shared engagement record +0x3CC, join the new
 *   one's (EnsureGlobalById 0x1F8278 / 0x20A730)
 * Info: proposed name SetStepguardTarget (T1); TU stepguard
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#tu_1553f0-0x1553f0-0x1564e8,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00208540(void *arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v0_2;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x38C);
    if (arg1 != temp_v0) {
        if (temp_v0 != 0) {
            func_0020A790(M2C_FIELD(arg0, s32 *, 0x3CC), arg0);
            M2C_FIELD(arg0, s32 *, 0x3CC) = 0;
        }
        M2C_FIELD(arg0, s32 *, 0x38C) = arg1;
        if (arg1 != 0) {
            temp_v0_2 = func_001F8278(arg1);
            M2C_FIELD(arg0, s32 *, 0x3CC) = temp_v0_2;
            func_0020A730(temp_v0_2, arg0);
        }
    }
}
