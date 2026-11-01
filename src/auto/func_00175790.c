#include "m2c_prelude.h"
M2C_UNK func_00176458(void *, M2C_UNK);             /* extern */
M2C_UNK func_001EEF70();                            /* extern */
M2C_UNK func_00208540(s32, s32);                    /* extern */

/** @sly2-annot addr=0x175790
 * Purpose: BHVCOMBAT Start: state 3 (pursue) and SetStepguardTarget(target +0x30)
 * Info: proposed name StartBhvcombat (T1); class BHVCOMBAT slot8; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVCOMBAT role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_00175790(void *arg0) {
    func_001EEF70();
    func_00176458(arg0, 3);
    func_00208540(M2C_FIELD(arg0, s32 *, 0x10), M2C_FIELD(arg0, s32 *, 0x30));
}
