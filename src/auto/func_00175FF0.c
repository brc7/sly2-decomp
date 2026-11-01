#include "m2c_prelude.h"
s32 func_001758A0(void *, s32, s32);                /* extern */
extern f32 D_002BE610;
extern f32 D_002DDED0;

/** @sly2-annot addr=0x175FF0
 * Purpose: Try PentryChooseStepguardAttack for the target; on failure push the attack cooldown +0xF04 forward
 * Info: proposed name FBhvcombatChooseAttack (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVCOMBAT role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
s32 func_00175FF0(void *arg0) {
    s32 temp_v0;
    void *temp_a0;
    void *temp_a0_2;

    temp_a0 = M2C_FIELD(arg0, void **, 0x10);
    temp_v0 = func_001758A0(temp_a0, M2C_FIELD(arg0, s32 *, 0x30), M2C_FIELD(temp_a0, s32 *, 0x3CC));
    M2C_FIELD(arg0, s32 *, 0x3C) = temp_v0;
    if (temp_v0 != 0) {
        return 1;
    }
    temp_a0_2 = M2C_FIELD(arg0, void **, 0x10);
    if (M2C_FIELD(temp_a0_2, f32 *, 0xF04) < D_002DDED0) {
        M2C_FIELD(temp_a0_2, f32 *, 0xF04) = (f32) (D_002DDED0 + D_002BE610);
    }
    return 0;
}
