#include "m2c_prelude.h"
M2C_UNK func_001EEF68();                            /* extern */
M2C_UNK func_00209890(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17D2B0
 * Purpose: BHVATTACH39 Init: raise flag 0x29
 * Info: proposed name InitBhv188 (T2); class BHV188 slot7; TU guardbhv
 * Notes: BHV class word normalised: InitBhvattach39 -> InitBhv188
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017D2B0(void *arg0) {
    func_001EEF68();
    func_00209890(M2C_FIELD(arg0, s32 *, 0x10), 0x29);
}
