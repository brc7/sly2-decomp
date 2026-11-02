#include "m2c_prelude.h"
/** @sly2-annot addr=0x206488
 * Purpose: Splice setter: +0x338 = (n+1)*10
 * Info: proposed name SetStepguardHitPoints (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00206488(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0x338) = (s32) ((arg1 + 1) * 0xA);
}
