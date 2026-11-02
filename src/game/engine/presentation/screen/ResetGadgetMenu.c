#include "m2c_prelude.h"
M2C_UNK func_001DA7B0();                            /* extern */
M2C_UNK ClearGadgetMenu(s32);                         /* extern */

/** @sly2-annot addr=0x1DAA48
 * Purpose: Gadget menu reset (slot 13)
 * Info: proposed name ResetGadgetMenu (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void ResetGadgetMenu(s32 arg0) {
    func_001DA7B0();
    ClearGadgetMenu(arg0);
}
