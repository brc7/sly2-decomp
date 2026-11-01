#include "m2c_prelude.h"
/** @sly2-annot addr=0x2087C8
 * Purpose: Drop generated detector entries and clear prefer list
 * Info: proposed name ClearStepguardGeneratedTargets (T2); TU stepguard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_002087C8(void *arg0) {
    void *temp_a1;

    temp_a1 = M2C_FIELD(arg0, void **, 0xF64);
    M2C_FIELD(temp_a1, s32 *, 0xFC) = (s32) (M2C_FIELD(temp_a1, s32 *, 0xFC) - M2C_FIELD(arg0, s32 *, 0x3A4));
    M2C_FIELD(arg0, s32 *, 0x394) = 0;
    M2C_FIELD(arg0, s32 *, 0x3A4) = 0;
}
