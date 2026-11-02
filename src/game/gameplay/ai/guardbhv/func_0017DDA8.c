#include "m2c_prelude.h"
M2C_UNK func_0017E278(void *, M2C_UNK);             /* extern */
M2C_UNK StartBhv();                            /* extern */
M2C_UNK SetStepguardTarget(s32, s32);                    /* extern */

/** @sly2-annot addr=0x17DDA8
 * Purpose: BHVSTALK Start: state 0 (alert anim at target) and set target
 * Info: proposed name StartBhv189 (T2); class BHV189 slot8; TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded:
 *   SetAloLookAtTiltFunction (names_v2:B(unverified)) | BHV class word normalised: StartBhvstalk ->
 *   StartBhv189
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017DDA8(void *arg0) {
    StartBhv();
    func_0017E278(arg0, 0);
    SetStepguardTarget(M2C_FIELD(arg0, s32 *, 0x10), M2C_FIELD(arg0, s32 *, 0x38));
}
