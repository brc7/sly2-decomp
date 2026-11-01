#include "m2c_prelude.h"
M2C_UNK RotateSoToMat();                            /* extern */
M2C_UNK func_00204660(void *, f32);                 /* extern */
f32 RadStepHeading(void *);                          /* extern */

/** @sly2-annot addr=0x2041F8
 * Purpose: STEP vt30: RotateSoToMat then radTarget = current heading (SetStepRadTarget) and POCTL+0x18 =
 *   heading
 * Info: proposed name RotateStepToMat (T1); class STEP slot30; TU step
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void RotateStepToMat(void *arg0) {
    void *temp_v0;

    RotateSoToMat();
    func_00204660(arg0, RadStepHeading(arg0));
    temp_v0 = M2C_FIELD(arg0, void **, 0x150);
    if (temp_v0 != NULL) {
        M2C_FIELD(temp_v0, f32 *, 0x18) = (f32) M2C_FIELD(arg0, f32 *, 0x1A8);
    }
}
