#include "m2c_prelude.h"
M2C_UNK func_0017E6B0(void *, M2C_UNK);             /* extern */
M2C_UNK func_001EEF78();                            /* extern */
M2C_UNK func_001EF038(void *);                      /* extern */
M2C_UNK func_00209970(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17E410
 * Purpose: BHVPATHLINK Close: stop, clear busy, lower flag 0x2C
 * Info: proposed name CloseBhvpathlink (T1); class BHVPATHLINK slot9; TU guardbhv
 * Notes: names_v2 v1 hint not used: LoadStepguardFromBrx__FP9STEPGUARDP18CBinaryInputStream | runtime2/4
 *   confirmed the BHVPATHLINK role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.75] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017E410(void *arg0) {
    func_001EEF78();
    func_0017E6B0(arg0, -1);
    func_001EF038(arg0);
    func_00209970(M2C_FIELD(arg0, s32 *, 0x10), 0x2C);
}
