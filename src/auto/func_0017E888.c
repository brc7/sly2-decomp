#include "m2c_prelude.h"
M2C_UNK func_001EEF68();                            /* extern */
M2C_UNK func_00209890(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17E888
 * Purpose: BHVRELOCATE Init: raise flag 0x2D
 * Info: proposed name InitBhv191 (T2); class BHV191 slot7; TU guardbhv
 * Notes: BHV class word normalised: InitBhvrelocate -> InitBhv191
 * Confidence: low (draft) [T2 0.85] - evidence: static analysis; sources: r9_bhv_screen+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017E888(void *arg0) {
    func_001EEF68();
    func_00209890(M2C_FIELD(arg0, s32 *, 0x10), 0x2D);
}
