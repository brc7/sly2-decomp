#include "m2c_prelude.h"
M2C_UNK TriggerBtn(s32);                         /* extern */

/** @sly2-annot addr=0x149738
 * Purpose: TriggerBtn
 * Info: proposed name TriggerVolbtn (T1); TU button
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void TriggerVolbtn(s32 arg0) {
    TriggerBtn(arg0 + 0x130);
}
