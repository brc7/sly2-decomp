#include "m2c_prelude.h"
M2C_UNK func_0017B8B0(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17B870
 * Purpose: BHVTARGETS Close: state -1, lower flag 0x20
 * Info: proposed name CloseBhv178 (T2); class BHV178 slot9; TU guardbhv
 * Notes: BHV class word normalised: CloseBhvtargets -> CloseBhv178
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017B870(void *arg0) {
    CloseBhv();
    func_0017B8B0(arg0, -1);
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x20);
}
