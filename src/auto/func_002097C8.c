#include "m2c_prelude.h"
/** @sly2-annot addr=0x2097C8
 * Purpose: mover[1] = 1 (heading locked)
 * Info: proposed name LockStepguardMover (T1); TU stepguard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7b_guard+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_002097C8(void *arg0) {
    M2C_FIELD(arg0, s32 *, 4) = 1;
}
