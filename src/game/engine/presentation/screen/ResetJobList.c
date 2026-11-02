#include "m2c_prelude.h"
M2C_UNK func_001DA7B0();                            /* extern */
M2C_UNK ClearJobList(s32);                         /* extern */

/** @sly2-annot addr=0x1DCCD8
 * Purpose: Job list reset (slot 13)
 * Info: proposed name ResetJobList (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void ResetJobList(s32 arg0) {
    func_001DA7B0();
    ClearJobList(arg0);
}
