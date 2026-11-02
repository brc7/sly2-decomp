#include "m2c_prelude.h"
M2C_UNK SetBhvpathlinkState(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK ClearBhvBusy(void *);                      /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17E410
 * Purpose: BHVPATHLINK Close: stop, clear busy, lower flag 0x2C
 * Info: proposed name CloseBhvpathlink (T1); class BHVPATHLINK slot9; TU guardbhv
 * Notes: names_v2 v1 hint not used: LoadStepguardFromBrx__FP9STEPGUARDP18CBinaryInputStream | runtime2/4
 *   confirmed the BHVPATHLINK role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.75] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void CloseBhvpathlink(void *arg0) {
    CloseBhv();
    SetBhvpathlinkState(arg0, -1);
    ClearBhvBusy(arg0);
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x2C);
}
