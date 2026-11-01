#include "m2c_prelude.h"
M2C_UNK func_0017E9B0(s32);                         /* extern */
M2C_UNK func_001EEF70();                            /* extern */

/** @sly2-annot addr=0x17E8B8
 * Purpose: BHVRELOCATE Start: go to the point
 * Info: proposed name StartBhv191 (T2); class BHV191 slot8; TU guardbhv
 * Notes: names_v2 v1 hint not used: PostStepguardLoadCallback__FP9STEPGUARD5MSGIDPv | BHV class word
 *   normalised: StartBhvrelocate -> StartBhv191
 * Confidence: low (draft) [T2 0.50] - evidence: static naming pass; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017E8B8(s32 arg0) {
    func_001EEF70();
    func_0017E9B0(arg0);
}
