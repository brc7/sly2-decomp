#include "m2c_prelude.h"
M2C_UNK UntriggerBtn(s32);                         /* extern */

/** @sly2-annot addr=0x1490E0
 * Purpose: UntriggerBtn(+0x150)
 * Info: proposed name UntriggerButton (T1); TU button
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void UntriggerButton(s32 arg0) {
    UntriggerBtn(arg0 + 0x150);
}
