#include "m2c_prelude.h"
M2C_UNK SetBhvpipeState(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17CED0
 * Purpose: BHVPIPE Close: state -1, lower flag 0x27
 * Info: proposed name CloseBhvpipe (T1); class BHVPIPE slot9; TU guardbhv
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void CloseBhvpipe(void *arg0) {
    CloseBhv();
    SetBhvpipeState(arg0, -1);
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x27);
}
