#include "m2c_prelude.h"
M2C_UNK SetBtnRsmg(s32);                         /* extern */

/** @sly2-annot addr=0x149270
 * Purpose: SetBtnRsmg(+0x150,...)
 * Info: proposed name SetButtonRsmg (T1); TU button
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void SetButtonRsmg(s32 arg0) {
    SetBtnRsmg(arg0 + 0x150);
}
