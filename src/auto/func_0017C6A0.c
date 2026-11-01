#include "m2c_prelude.h"
M2C_UNK func_0017C928(s32, M2C_UNK);                /* extern */
M2C_UNK StartBhv();                            /* extern */

/** @sly2-annot addr=0x17C6A0
 * Purpose: BHVESCORT Start: state 0
 * Info: proposed name StartBhv183 (T2); class BHV183 slot8; TU guardbhv
 * Notes: BHV class word normalised: StartBhvescort -> StartBhv183
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017C6A0(s32 arg0) {
    StartBhv();
    func_0017C928(arg0, 0);
}
