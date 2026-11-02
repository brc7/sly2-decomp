#include "m2c_prelude.h"
/** @sly2-annot addr=0x209840
 * Purpose: unless locked, copy a 3x3 into owner's matTarget +0x1B0
 * Info: proposed name SetStepguardMoverMatTarget (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00209840(void *arg0, void *arg1) {
    void *temp_a0;

    if (M2C_FIELD(arg0, s32 *, 4) == 0) {
        temp_a0 = M2C_FIELD(arg0, void **, 0xC);
        M2C_FIELD(temp_a0, s128 *, 0x1B0) = (s128) M2C_FIELD(arg1, s128 *, 0);
        M2C_FIELD(temp_a0, s128 *, 0x1C0) = (s128) M2C_FIELD(arg1, s128 *, 0x10);
        M2C_FIELD(temp_a0, s128 *, 0x1D0) = (s128) M2C_FIELD(arg1, s128 *, 0x20);
    }
}
