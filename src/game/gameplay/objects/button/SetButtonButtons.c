#include "m2c_prelude.h"
M2C_UNK SetBtnButtons(s32);                         /* extern */

/** @sly2-annot addr=0x1490A0
 * Purpose: SetBtnButtons(+0x150, n)
 * Info: proposed name SetButtonButtons (T1); TU button
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void SetButtonButtons(s32 arg0) {
    SetBtnButtons(arg0 + 0x150);
}
