#include "m2c_prelude.h"
M2C_UNK SetPauseMenuState(void *, M2C_UNK);             /* extern */
M2C_UNK func_001AF660();                            /* extern */

/** @sly2-annot addr=0x1AE970
 * Purpose: Pop to root (0x1AF660), state 0, drop widget
 * Info: proposed name ResetPauseMenu (T2); TU menu
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001AE970(void *arg0) {
    func_001AF660();
    SetPauseMenuState(arg0, 0);
    M2C_FIELD(arg0, s32 *, 0x20) = 0;
}
