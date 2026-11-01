#include "m2c_prelude.h"
M2C_UNK func_0017CD00(void *, M2C_UNK);             /* extern */
M2C_UNK func_001EEF78();                            /* extern */
M2C_UNK func_00209970(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17CB40
 * Purpose: BHVRAIL Close: state -1, lower flag 0x1B
 * Info: proposed name CloseBhvrail (T1); class BHVRAIL slot9; TU guardbhv
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017CB40(void *arg0) {
    func_001EEF78();
    func_0017CD00(arg0, -1);
    func_00209970(M2C_FIELD(arg0, s32 *, 0x10), 0x1B);
}
