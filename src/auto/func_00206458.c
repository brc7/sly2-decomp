#include "m2c_prelude.h"
/** @sly2-annot addr=0x206458
 * Purpose: Splice getter: (+0x338 - 1)/10 clamped >= 0 (max hit count)
 * Info: proposed name GetStepguardHitPoints (T2); TU stepguard
 * Notes: runtime4 CORRECTION: guard +0x338 = max hp, +0x33C = current hp
 * Confidence: low (draft) [T2 0.40] - evidence: runtime (save-state RAM); sources:
 *   r7b_guard+runtime-corrected
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
s32 func_00206458(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x338) - 1;
    return (s32) ((temp_v0 <= -1) ? 0 : temp_v0) / 10;
}
