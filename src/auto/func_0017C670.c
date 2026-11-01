#include "m2c_prelude.h"
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17C670
 * Purpose: BHVESCORT Close: lower flag 0x26
 * Info: proposed name CloseBhv183 (T2); class BHV183 slot9; TU guardbhv
 * Notes: BHV class word normalised: CloseBhvescort -> CloseBhv183
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017C670(void *arg0) {
    CloseBhv();
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x26);
}
