#include "m2c_prelude.h"
M2C_UNK FAddAshOid(s32);                         /* extern */

/** @sly2-annot addr=0x148B18
 * Purpose: FAddAshOid on the 'off' list (+0x48)
 * Info: proposed name AddBtnOffAseg (T1); TU button
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void AddBtnOffAseg(s32 arg0) {
    FAddAshOid(arg0 + 0x48);
}
