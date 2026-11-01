#include "m2c_prelude.h"
M2C_UNK AddBtnOffAseg(s32);                         /* extern */

/** @sly2-annot addr=0x149200
 * Purpose: AddBtnOffAseg(+0x150, oid)
 * Info: proposed name AddButtonOffAseg (T1); TU button
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void AddButtonOffAseg(s32 arg0) {
    AddBtnOffAseg(arg0 + 0x150);
}
