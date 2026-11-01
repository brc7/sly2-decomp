#include "m2c_prelude.h"
M2C_UNK func_0017B750(s32, M2C_UNK);                /* extern */
M2C_UNK func_001EEF78();                            /* extern */

/** @sly2-annot addr=0x17B5F0
 * Purpose: BHVSHOOT Close: state -1
 * Info: proposed name CloseBhvshoot (T1); class BHVSHOOT slot9; TU guardbhv
 * Notes: BHV class word normalised: CloseBhvshoot -> CloseBhv173 | v3 BHV class word (runtime6 P2: BHV194 =
 *   follow, BHV168 = walk-to-goal child): CloseBhv173 -> CloseBhvshoot | runtime7a: BHVSHOOT role confirmed
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)+runtime
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017B5F0(s32 arg0) {
    func_001EEF78();
    func_0017B750(arg0, -1);
}
