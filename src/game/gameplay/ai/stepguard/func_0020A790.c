#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(void *, void *, s32);       /* extern */
s32 func_0020A658();                                /* extern */

/** @sly2-annot addr=0x20A790
 * Purpose: Remove guard from record
 * Info: proposed name RemoveTargetRecordGuard (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020A790(void *arg0) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    void *temp_a0;

    temp_v0 = func_0020A658();
    temp_v1 = M2C_FIELD(arg0, s32 *, 0x28) - 1;
    temp_a0 = arg0 + ((temp_v0 * 4) + 0x2C);
    temp_v0_2 = temp_v1 - temp_v0;
    M2C_FIELD(arg0, s32 *, 0x28) = temp_v1;
    CopyAb__FPvT0Ui(temp_a0, temp_a0 + 4, temp_v0_2 * 4);
}
