#include "m2c_prelude.h"
M2C_UNK InitBhv();                            /* extern */
M2C_UNK AddStepguardSgf(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17C638
 * Purpose: BHVESCORT Init: raise flag 0x26, state -1
 * Info: proposed name InitBhv183 (T2); class BHV183 slot7; TU guardbhv
 * Notes: BHV class word normalised: InitBhvescort -> InitBhv183
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017C638(void *arg0) {
    InitBhv();
    M2C_FIELD(arg0, s32 *, 0x50) = -1;
    AddStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x26);
}
