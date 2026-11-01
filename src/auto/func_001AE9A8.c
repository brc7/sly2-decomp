#include "m2c_prelude.h"
M2C_UNK func_00187E78(s32);                         /* extern */
M2C_UNK func_001D98A8();                            /* extern */

/** @sly2-annot addr=0x1AE9A8
 * Purpose: Screen base vt9 (0x1D98A8) then reset joystick selection repeat if flag
 * Info: proposed name PAUSEMENU_vt9 (T2); class PAUSEMENU; TU menu
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001AE9A8(void *arg0, s32 arg1) {
    func_001D98A8();
    if (arg1 != 0) {
        func_00187E78(M2C_FIELD(arg0, s32 *, 0x10));
    }
}
