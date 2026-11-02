#include "m2c_prelude.h"
M2C_UNK func_0020AA40();                            /* extern */
M2C_UNK func_0020AAA8(s32);                         /* extern */

/** @sly2-annot addr=0x20AA78
 * Purpose: 0x20AA40 then 0x20AAA8
 * Info: proposed name AddStepguardAttackOidResolve (T2); TU stepguard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020AA78(s32 arg0) {
    func_0020AA40();
    func_0020AAA8(arg0);
}
