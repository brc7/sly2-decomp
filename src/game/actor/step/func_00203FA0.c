#include "m2c_prelude.h"
/** @sly2-annot addr=0x203FA0
 * Purpose: Option setter for STEP+0x204 (bool); setting it clears the ground contact +0x208
 * Info: proposed name SetStepFNoBase (T2); TU step
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void func_00203FA0(void *arg0, s32 arg1) {
    if (M2C_FIELD(arg0, s32 *, 0x204) != arg1) {
        M2C_FIELD(arg0, s32 *, 0x204) = arg1;
        if (arg1 != 0) {
            M2C_FIELD(arg0, s32 *, 0x208) = 0;
        }
    }
}
