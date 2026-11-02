#include "m2c_prelude.h"
M2C_UNK func_001F1900();                            /* extern */

/** @sly2-annot addr=0x209870
 * Purpose: Store flag word +0x2C0 and re-evaluate pickpocket state (0x1F1900)
 * Info: proposed name SetStepguardGrfsgf (T1); TU stepguard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void SetStepguardGrfsgf(void *arg0, s64 arg1) {
    M2C_FIELD(arg0, s64 *, 0x2C0) = arg1;
    func_001F1900();
}
