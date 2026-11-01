#include "m2c_prelude.h"
s32 func_00207B10(s32);                             /* extern */
void *func_00209C18(s32, s32);                      /* extern */

/** @sly2-annot addr=0x207B70
 * Purpose: Splice: ensure the anim binding for a flag set and set its oid/arg (+4/+0xC)
 * Info: proposed name SetStepguardAnimBinding (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00207B70(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *temp_v0;

    temp_v0 = func_00209C18(arg0, func_00207B10(arg1));
    M2C_FIELD(temp_v0, s32 *, 4) = arg2;
    M2C_FIELD(temp_v0, s32 *, 0xC) = arg3;
}
