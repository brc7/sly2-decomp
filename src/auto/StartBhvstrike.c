#include "m2c_prelude.h"
M2C_UNK StartBhvanim();                            /* extern */

/** @sly2-annot addr=0x1751F0
 * Purpose: BHVSTRIKE Start: mark guard engaged-busy (+0xF3C=1), BHVANIM Start; enable leap payload when the
 *   binding is kind 2
 * Info: proposed name StartBhvstrike (T1); class BHVSTRIKE slot8; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVSTRIKE role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void StartBhvstrike(void *arg0) {
    void *temp_a0;

    M2C_FIELD(M2C_FIELD(arg0, void **, 0x10), s32 *, 0xF3C) = 1;
    StartBhvanim();
    temp_a0 = M2C_FIELD(arg0, void **, 0x40);
    if (M2C_FIELD(temp_a0, s32 *, 0) == 2) {
        M2C_FIELD(M2C_FIELD(temp_a0, void **, 8), s32 *, 0x60) = 1;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x40), void **, 8), s32 *, 0x64) = 1;
        M2C_FIELD(arg0, s32 *, 0x50) = 1;
    }
}
