#include "m2c_prelude.h"
M2C_UNK SetBhvcombatState(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x1757D0
 * Purpose: BHVCOMBAT Close: state -1, lower flag 0x1D
 * Info: proposed name CloseBhvcombat (T1); class BHVCOMBAT slot9; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVCOMBAT role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void CloseBhvcombat(void *arg0) {
    CloseBhv();
    SetBhvcombatState(arg0, -1);
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x1D);
}
