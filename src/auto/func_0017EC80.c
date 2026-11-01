#include "m2c_prelude.h"
M2C_UNK func_001EEF78();                            /* extern */
M2C_UNK func_00209970(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17EC80
 * Purpose: BHVPATROLPATH Close: lower flag 0x2E
 * Info: proposed name CloseBhvpatrolpath (T1); class BHVPATROLPATH slot9; TU guardbhv
 * Notes: names_v2 v1 hint not used: FReachedStepguardGoal__FP9STEPGUARD | runtime2/4 confirmed the
 *   BHVPATROLPATH role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017EC80(void *arg0) {
    func_001EEF78();
    func_00209970(M2C_FIELD(arg0, s32 *, 0x10), 0x2E);
}
