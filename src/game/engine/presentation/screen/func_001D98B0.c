#include "m2c_prelude.h"
M2C_UNK PrependDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK RemoveDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
s32 FFindDlEntry__FP2DLPv(M2C_UNK *, s32);                  /* extern */
M2C_UNK func_001D95B8(M2C_UNK *);                   /* extern */
extern M2C_UNK D_003D9E70;

/** @sly2-annot addr=0x1D98B0
 * Purpose: slot 10: move/append the screen to the active list and refocus
 * Info: proposed name ShowScreen (T2); class SCREEN; TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/hud-ui.md#open-questions, docs/open-questions.md#hud--ui,
 *   docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001D98B0(s32 arg0) {
    if (FFindDlEntry__FP2DLPv(&D_003D9E70, arg0) != 0) {
        RemoveDlEntry__FP2DLPv(&D_003D9E70, arg0);
    }
    PrependDlEntry__FP2DLPv(&D_003D9E70, arg0);
    func_001D95B8(&D_003D9E70);
}
