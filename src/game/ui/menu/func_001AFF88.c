#include "m2c_prelude.h"
M2C_UNK SetPauseMenuState(s32, M2C_UNK);                /* extern */
M2C_UNK ApplyGsSettings(M2C_UNK *);                   /* extern */
extern M2C_UNK D_003D4A60;

/** @sly2-annot addr=0x1AFF88
 * Purpose: 0x1D8FF8(options) then state 9
 * Info: proposed name ResetPauseVideoOptions (T2); TU menu
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001AFF88(s32 arg0) {
    ApplyGsSettings(&D_003D4A60);
    SetPauseMenuState(arg0, 9);
}
