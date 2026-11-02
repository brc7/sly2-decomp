#include "m2c_prelude.h"
M2C_UNK func_0017C550(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17BFB8
 * Purpose: BHVCIRCLE Close: state -1, lower flag 0x19
 * Info: proposed name CloseBhv179 (T2); class BHV179 slot9; TU guardbhv
 * Notes: BHV class word normalised: CloseBhvcircle -> CloseBhv179
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017BFB8(void *arg0) {
    CloseBhv();
    func_0017C550(arg0, -1);
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x19);
}
