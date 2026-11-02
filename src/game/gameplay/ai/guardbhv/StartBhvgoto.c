#include "m2c_prelude.h"
M2C_UNK SetBhvgotoState(void *, M2C_UNK);             /* extern */
M2C_UNK PlanBhvgoto(void *);                      /* extern */
M2C_UNK StartBhv(void *);                      /* extern */
M2C_UNK AddStepguardSgf(s32, s32);                    /* extern */

/** @sly2-annot addr=0x17D3B0
 * Purpose: BHVGOTO Start: raise the caller-chosen flag (e.g. 0x1C run), stand, plan the route
 * Info: proposed name StartBhvgoto (T1); class BHVGOTO slot8; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVGOTO role on live guards (S02-S41)
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void StartBhvgoto(void *arg0) {
    s32 temp_a1;

    temp_a1 = M2C_FIELD(arg0, s32 *, 0x38);
    if (temp_a1 != -1) {
        AddStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), temp_a1);
    }
    StartBhv(arg0);
    SetBhvgotoState(arg0, 4);
    PlanBhvgoto(arg0);
}
