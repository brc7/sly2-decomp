#include "m2c_prelude.h"
M2C_UNK func_00141E70();                            /* extern */
s32 func_00141EA8(s32, M2C_UNK);                    /* extern */
s32 func_001C40C0(s32);                             /* extern */
M2C_UNK func_001F6978(s32, M2C_UNK);                /* extern */
M2C_UNK func_001F69F8(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x136C50
 * Purpose: BENTLEY vt105: base, then enable feature 0x24D if current player and gadget 0xC equipped, else
 *   disable
 * Info: proposed name OnBentleyGadgetsChanged (T1); class BENTLEY slot105; TU bentley
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_00136C50(s32 arg0) {
    func_00141E70();
    if ((func_001C40C0(arg0) != 0) && (func_00141EA8(arg0, 0xC) != 0)) {
        func_001F6978(arg0, 0x24D);
        return;
    }
    func_001F69F8(arg0, 0x24D);
}
