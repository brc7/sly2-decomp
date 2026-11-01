#include "m2c_prelude.h"
M2C_UNK AddBtnAseg(s32);                         /* extern */

/** @sly2-annot addr=0x149778
 * Purpose: AddBtnAseg
 * Info: proposed name AddVolbtnAseg (T1); TU button
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void AddVolbtnAseg(s32 arg0) {
    AddBtnAseg(arg0 + 0x130);
}
