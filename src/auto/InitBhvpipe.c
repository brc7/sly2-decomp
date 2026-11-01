#include "m2c_prelude.h"
M2C_UNK InitBhv();                            /* extern */
M2C_UNK AddStepguardSgf(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17CE68
 * Purpose: BHVPIPE Init: raise flag 0x27 (on pipe), state -1
 * Info: proposed name InitBhvpipe (T1); class BHVPIPE slot7; TU guardbhv
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void InitBhvpipe(void *arg0) {
    InitBhv();
    M2C_FIELD(arg0, s32 *, 0x30) = -1;
    AddStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x27);
}
