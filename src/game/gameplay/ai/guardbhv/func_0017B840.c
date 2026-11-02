#include "m2c_prelude.h"
M2C_UNK func_0017B8B0(s32, M2C_UNK);                /* extern */
M2C_UNK StartBhv();                            /* extern */

/** @sly2-annot addr=0x17B840
 * Purpose: BHVTARGETS Start: state 0
 * Info: proposed name StartBhv178 (T2); class BHV178 slot8; TU guardbhv
 * Notes: no usable identifier; purpose only | BHV class word normalised: StartBhvtargets -> StartBhv178
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources:
 *   verify:verdicts_A1+verdicts_A1+r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017B840(s32 arg0) {
    StartBhv();
    func_0017B8B0(arg0, 0);
}
