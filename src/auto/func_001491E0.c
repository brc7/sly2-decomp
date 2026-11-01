#include "m2c_prelude.h"
M2C_UNK func_00148AF8(s32);                         /* extern */

/** @sly2-annot addr=0x1491E0
 * Purpose: AddBtnAseg(+0x150, oid)
 * Info: proposed name AddButtonAseg (T1); TU button
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void func_001491E0(s32 arg0) {
    func_00148AF8(arg0 + 0x150);
}
