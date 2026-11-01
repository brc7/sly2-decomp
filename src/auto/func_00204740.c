#include "m2c_prelude.h"
M2C_UNK atan2f(f32, f32);                           /* extern */

/** @sly2-annot addr=0x204740
 * Purpose: Current heading = atan2(xf.mat[0][1], xf.mat[0][0])
 * Info: proposed name RadStepHeading (T1); TU step
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void func_00204740(void *arg0) {
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg0, void **, 0x60);
    atan2f(M2C_FIELD(temp_v0, f32 *, 0x44), M2C_FIELD(temp_v0, f32 *, 0x40));
}
