#include "m2c_prelude.h"
M2C_UNK func_00148A78(s32);                         /* extern */

/** @sly2-annot addr=0x149718
 * Purpose: SetBtnButtons on the volbtn's BTN
 * Info: proposed name SetVolbtnButtons (T1); TU button
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void func_00149718(s32 arg0) {
    func_00148A78(arg0 + 0x130);
}
