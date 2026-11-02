#include "m2c_prelude.h"
M2C_UNK func_00141E70();                            /* extern */
s32 GrfbtnGadget(s32, M2C_UNK);                    /* extern */
s32 FIsPoCur(s32);                             /* extern */
M2C_UNK ShowSoGlobsById(s32, M2C_UNK);                /* extern */
M2C_UNK HideSoGlobsById(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x136C50
 * Purpose: BENTLEY vt105: base, then enable feature 0x24D if current player and gadget 0xC equipped, else
 *   disable
 * Info: proposed name OnBentleyGadgetsChanged (T1); class BENTLEY slot105; TU bentley
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void OnBentleyGadgetsChanged(s32 arg0) {
    func_00141E70();
    if ((FIsPoCur(arg0) != 0) && (GrfbtnGadget(arg0, 0xC) != 0)) {
        ShowSoGlobsById(arg0, 0x24D);
        return;
    }
    HideSoGlobsById(arg0, 0x24D);
}
