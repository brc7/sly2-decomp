#include "m2c_prelude.h"
M2C_UNK FAddAshOid(s32);                         /* extern */

/** @sly2-annot addr=0x148AF8
 * Purpose: FAddAshOid on the 'on' list (+4)
 * Info: proposed name AddBtnAseg (T1); TU button
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void AddBtnAseg(s32 arg0) {
    FAddAshOid(arg0 + 4);
}
