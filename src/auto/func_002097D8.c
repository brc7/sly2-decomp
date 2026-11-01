#include "m2c_prelude.h"
/** @sly2-annot addr=0x2097D8
 * Purpose: mover[1] = 0
 * Info: proposed name UnlockStepguardMover (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_002097D8(void *arg0) {
    M2C_FIELD(arg0, s32 *, 4) = 0;
}
