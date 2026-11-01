#include "m2c_prelude.h"
M2C_UNK func_001EEF68();                            /* extern */
M2C_UNK func_00209890(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17EB48
 * Purpose: BHVPATROLPATH Init: raise flag 0x2E, param -1
 * Info: proposed name InitBhvpatrolpath (T1); class BHVPATROLPATH slot7; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVPATROLPATH role on live guards (S02-S41)
 * Confidence: high [T1 0.90] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+verdicts_followups+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017EB48(void *arg0) {
    func_001EEF68();
    M2C_FIELD(arg0, f32 *, 0x38) = -1.0f;
    func_00209890(M2C_FIELD(arg0, s32 *, 0x10), 0x2E);
}
