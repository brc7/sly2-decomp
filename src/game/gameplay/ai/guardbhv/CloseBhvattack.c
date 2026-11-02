#include "m2c_prelude.h"
M2C_UNK SetBhvattackState(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x175448
 * Purpose: BHVATTACK Close: end strike, lower flag 0x18
 * Info: proposed name CloseBhvattack (T1); class BHVATTACK slot9; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVATTACK role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void CloseBhvattack(void *arg0) {
    CloseBhv();
    SetBhvattackState(arg0, -1);
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x18);
}
