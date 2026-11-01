#include "m2c_prelude.h"
M2C_UNK func_0017A8C8(void *, M2C_UNK);             /* extern */
M2C_UNK func_001EEF78();                            /* extern */
M2C_UNK func_00209970(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17A7B8
 * Purpose: BHVTHROWN Close: state -1, lower flag 0x16
 * Info: proposed name CloseBhv176 (T2); class BHV176 slot9; TU guardbhv
 * Notes: BHV class word normalised: CloseBhvthrown -> CloseBhv176
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017A7B8(void *arg0) {
    func_001EEF78();
    func_0017A8C8(arg0, -1);
    func_00209970(M2C_FIELD(arg0, s32 *, 0x10), 0x16);
}
