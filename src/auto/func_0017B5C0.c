#include "m2c_prelude.h"
M2C_UNK func_0017B750(s32, M2C_UNK);                /* extern */
M2C_UNK func_001EEF70();                            /* extern */

/** @sly2-annot addr=0x17B5C0
 * Purpose: BHVSHOOT Start: state 0
 * Info: proposed name StartBhvshoot (T1); class BHVSHOOT slot8; TU guardbhv
 * Notes: no usable identifier; purpose only | BHV class word normalised: StartBhvshoot -> StartBhv173 | v3
 *   BHV class word (runtime6 P2: BHV194 = follow, BHV168 = walk-to-goal child): StartBhv173 -> StartBhvshoot
 *   | runtime7a: BHVSHOOT state 0 aiming at JT since 68.378 (S67)
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources:
 *   verify:verdicts_A1+verdicts_A1+r9_bhv_screen+runtime(class role)+runtime
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017B5C0(s32 arg0) {
    func_001EEF70();
    func_0017B750(arg0, 0);
}
