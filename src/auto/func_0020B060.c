#include "m2c_prelude.h"
M2C_UNK func_00209130(void *, s32);                 /* extern */
M2C_UNK func_0020AFF0();                            /* extern */

/** @sly2-annot addr=0x20B060
 * Purpose: same then resolve
 * Info: proposed name SetCid120AttackBindingK2Resolve (T2); class CID120; TU stepguard
 * Notes: Cid78->Cid120 (CID 0x78=120, agent-default(hex))
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020B060(void *arg0) {
    func_0020AFF0();
    func_00209130(arg0 + 0xB0, M2C_FIELD(arg0, s32 *, 0xAC));
}
