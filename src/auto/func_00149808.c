#include "m2c_prelude.h"
M2C_UNK func_00148B38(s32);                         /* extern */

/** @sly2-annot addr=0x149808
 * Purpose: SetBtnRsmg
 * Info: proposed name SetVolbtnRsmg (T1); TU button
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void func_00149808(s32 arg0) {
    func_00148B38(arg0 + 0x130);
}
