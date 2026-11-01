#include "m2c_prelude.h"
/** @sly2-annot addr=0x2064A0
 * Purpose: Splice getter for +0x33C: max(v-1,0)+1
 * Info: proposed name GetStepguardHitPoints2 (T2); TU stepguard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
s32 func_002064A0(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x33C) - 1;
    return ((temp_v0 <= -1) ? 0 : temp_v0) + 1;
}
