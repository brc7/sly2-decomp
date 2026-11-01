#include "m2c_prelude.h"
M2C_UNK func_0011A2E8(M2C_UNK *, s32);              /* extern */
s32 func_0011A380(M2C_UNK *, s32);                  /* extern */
M2C_UNK func_001D95B8(M2C_UNK *);                   /* extern */
extern M2C_UNK D_003D9E70;

/** @sly2-annot addr=0x1D9910
 * Purpose: slot 11: remove from the active list and refocus
 * Info: proposed name HideScreen (T2); class SCREEN; TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001D9910(s32 arg0) {
    if (func_0011A380(&D_003D9E70, arg0) != 0) {
        func_0011A2E8(&D_003D9E70, arg0);
        func_001D95B8(&D_003D9E70);
    }
}
