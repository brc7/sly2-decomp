#include "m2c_prelude.h"
/** @sly2-annot addr=0x208538
 * Purpose: Splice setter: +0x390 = plo (option getter reads +0x38C)
 * Info: proposed name SetStepguardForcedTarget (T2); TU stepguard
 * Notes: verifier PLAUSIBLE: +0x390 = arg (splice property setter, data ref 0x2AF134); ClearStepguardState
 *   zeroes +0x390. 'Forced target' is not demonstrated. | T2 forced: verifier: role word not supported by the
 *   code
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard+verdicts_followups
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00208538(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0x390) = arg1;
}
