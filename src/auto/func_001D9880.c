#include "m2c_prelude.h"
M2C_UNK func_0011A380(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_003D9E70;

/** @sly2-annot addr=0x1D9880
 * Purpose: Return whether the screen is in the shown-screen list D_003D9E70
 * Info: proposed name FScreenShown (T1); TU screen
 * Notes: top50_round (0.65): Body FFindDlEntry(&D_003D9E70,p) (m2c drops the return). ShowScreen 0x1D98B0 /
 *   HideScreen 0x1D9910 (SCREEN slots 10/11) add/remove from the same list. Callers: pause-menu page tests,
 *   memcard dialog flow
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: r6b_rest+top50_round
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001D9880(s32 arg0) {
    func_0011A380(&D_003D9E70, arg0);
}
