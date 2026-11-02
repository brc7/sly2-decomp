#include "m2c_prelude.h"
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17E8E8
 * Purpose: BHVRELOCATE Close: lower flag 0x2D
 * Info: proposed name CloseBhv191 (T2); class BHV191 slot9; TU guardbhv
 * Notes: names_v2 v1 hint not used: OnStepguardAdd__FP9STEPGUARD | BHV class word normalised:
 *   CloseBhvrelocate -> CloseBhv191
 * Confidence: low (draft) [T2 0.50] - evidence: static naming pass; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017E8E8(void *arg0) {
    CloseBhv();
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x2D);
}
