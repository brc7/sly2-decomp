#include "m2c_prelude.h"
M2C_UNK func_0012D020(s32);                         /* extern */
M2C_UNK func_0012E210(s32);                         /* extern */

/** @sly2-annot addr=0x12E460
 * Purpose: If *ppact: DetachActFromAlo, free it (0x12E210), *ppact=NULL
 * Info: proposed name ReleaseAct (T1); TU act
 * Notes: top50_round (0.70): Body (m2c). Paired with PactNew* by the same callers (SetBentleyHoverState,
 *   SetMurrayLiftSubstate, SetJtHandsTarget, OnMissileRemove) to drop an actuator they own. Sly 1 RetractAct
 *   is the detach; this is the owning-point
 * Confidence: mid (draft) [T1 0.70] - evidence: Sly 1 / Lilly reference; semantic review; sources:
 *   r1_116918+top50_round
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600,
 *   docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8,
 *   docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550
 * @end-sly2-annot */
void ReleaseAct(s32 *arg0) {
    s32 temp_s0;

    temp_s0 = *arg0;
    if (temp_s0 != 0) {
        func_0012D020(temp_s0);
        func_0012E210(temp_s0);
        *arg0 = 0;
    }
}
