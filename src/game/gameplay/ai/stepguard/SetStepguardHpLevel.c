#include "m2c_prelude.h"
/** @sly2-annot addr=0x2064C0
 * Purpose: Splice setter: +0x33C = (n+1)*10
 * Info: proposed name SetStepguardHpLevel (T1); TU stepguard
 * Notes: superseded: SetStepguardHitPoints2 (r7b_guard) | runtime4: splice setter writes (n+1)*10 to guard
 *   +0x33C (current hp; +0x338 = max hp)
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources: r7b_guard+runtime
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void SetStepguardHpLevel(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0x33C) = (s32) ((arg1 + 1) * 0xA);
}
