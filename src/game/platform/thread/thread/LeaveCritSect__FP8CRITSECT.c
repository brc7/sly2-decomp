#include "m2c_prelude.h"
M2C_UNK SignalSema(s32);                            /* extern */

/** @sly2-annot addr=0x21F0C0
 * Purpose: decrement enter count; at 0 clear owner and SignalSema
 * Info: proposed name LeaveCritSect (T1); TU thread
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8+verdicts_A2
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#thread-0x21efd0-0x21f2e0
 * @end-sly2-annot */
void LeaveCritSect__FP8CRITSECT(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0) - 1;
    M2C_FIELD(arg0, s32 *, 0) = temp_v0;
    if (temp_v0 == 0) {
        M2C_FIELD(arg0, s32 *, 4) = -1;
        SignalSema(M2C_FIELD(arg0, s32 *, 8));
    }
}
