#include "m2c_prelude.h"
M2C_UNK func_00176458(void *, M2C_UNK);             /* extern */
M2C_UNK func_001EEF78();                            /* extern */
M2C_UNK func_00209970(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x1757D0
 * Purpose: BHVCOMBAT Close: state -1, lower flag 0x1D
 * Info: proposed name CloseBhvcombat (T1); class BHVCOMBAT slot9; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVCOMBAT role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_001757D0(void *arg0) {
    func_001EEF78();
    func_00176458(arg0, -1);
    func_00209970(M2C_FIELD(arg0, s32 *, 0x10), 0x1D);
}
