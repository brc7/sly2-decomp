#include "m2c_prelude.h"
M2C_UNK func_001DA7B0();                            /* extern */
M2C_UNK func_001DB820(s32);                         /* extern */

/** @sly2-annot addr=0x1DB7F0
 * Purpose: ThiefNet shop reset (slot 13)
 * Info: proposed name ResetThiefnet (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DB7F0(s32 arg0) {
    func_001DA7B0();
    func_001DB820(arg0);
}
