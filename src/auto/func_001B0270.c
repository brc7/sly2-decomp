#include "m2c_prelude.h"
M2C_UNK func_001AF660(M2C_UNK *);                   /* extern */
extern M2C_UNK D_003D9ED0;

/** @sly2-annot addr=0x1B0270
 * Purpose: Item callback wrapper of PopPauseMenuToRoot
 * Info: proposed name PauseItemPopToRoot (T2); TU menu
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001B0270(void) {
    func_001AF660(&D_003D9ED0);
}
