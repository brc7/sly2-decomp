#include "m2c_prelude.h"
M2C_UNK func_002099F0(s64);                         /* extern */

/** @sly2-annot addr=0x209A68
 * Purpose: flag n set in +0x2C0
 * Info: proposed name FStepguardHasSgf (T1); TU stepguard
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_00209A68(void *arg0) {
    func_002099F0(M2C_FIELD(arg0, s64 *, 0x2C0));
}
