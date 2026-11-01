#include "m2c_prelude.h"
s32 func_00132B10(s32, M2C_UNK);                    /* extern */
M2C_UNK func_0020B478(s32);                         /* extern */

/** @sly2-annot addr=0x1EF038
 * Purpose: Inverse of SetBhvBusy (fBusy=0, STEPGUARD busy count-- via 0x20B478, cBusyAbove-- below)
 * Info: proposed name ClearBhvBusy (T1); TU bhvstk
 * Notes: runtime: busy protocol confirmed
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); sources: r6b_rest+runtime
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850
 * @end-sly2-annot */
void func_001EF038(void *arg0) {
    void *var_a0;

    if (M2C_FIELD(arg0, s32 *, 0x1C) != 0) {
        M2C_FIELD(arg0, s32 *, 0x1C) = 0;
        if (func_00132B10(M2C_FIELD(arg0, s32 *, 0x10), 0x38) != 0) {
            func_0020B478(M2C_FIELD(arg0, s32 *, 0x10));
        }
        var_a0 = M2C_FIELD(arg0, void **, 0x14);
        if (var_a0 != NULL) {
            do {
                M2C_FIELD(var_a0, s32 *, 0x20) = (s32) (M2C_FIELD(var_a0, s32 *, 0x20) - 1);
                var_a0 = M2C_FIELD(var_a0, void **, 0x14);
            } while (var_a0 != NULL);
        }
    }
}
