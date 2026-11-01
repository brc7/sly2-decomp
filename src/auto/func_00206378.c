#include "m2c_prelude.h"
/** @sly2-annot addr=0x206378
 * Purpose: Return the guard's vision detector (+0xF64)
 * Info: proposed name PdetectorGetStepguard (T1); TU stepguard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
s32 func_00206378(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0xF64);
}
