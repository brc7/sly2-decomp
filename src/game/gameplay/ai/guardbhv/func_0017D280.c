#include "m2c_prelude.h"
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17D280
 * Purpose: BHVATTACH19 Close: lower flag 0x28
 * Info: proposed name CloseBhv187 (T2); class BHV187 slot9; TU guardbhv
 * Notes: BHV class word normalised: CloseBhvattach19 -> CloseBhv187
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017D280(void *arg0) {
    CloseBhv();
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x28);
}
