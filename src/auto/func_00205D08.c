#include "m2c_prelude.h"
/** @sly2-annot addr=0x205D08
 * Purpose: STEP vt86: returns 0 (base; STEPGUARD override 0x207530 tests lethal zap)
 * Info: proposed name FStepVt86 (T2); class STEP slot86; TU step
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
s32 func_00205D08(void) {
    return 0;
}
