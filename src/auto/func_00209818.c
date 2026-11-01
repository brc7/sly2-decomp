#include "m2c_prelude.h"
M2C_UNK func_00204660(s32);                         /* extern */

/** @sly2-annot addr=0x209818
 * Purpose: unless locked, SetStepRadTarget(owner, rad)
 * Info: proposed name SetStepguardMoverRadTarget (T1); TU stepguard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7b_guard+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00209818(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 4) == 0) {
        func_00204660(M2C_FIELD(arg0, s32 *, 0xC));
    }
}
