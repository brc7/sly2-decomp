#include "m2c_prelude.h"
M2C_UNK func_002090D0(s32, s32);                    /* extern */

/** @sly2-annot addr=0x20AF38
 * Purpose: release binding then set oid/arg
 * Info: proposed name SetCid120AttackBinding (T2); class CID120; TU stepguard
 * Notes: Cid78->Cid120 (CID 0x78=120, agent-default(hex))
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020AF38(void *arg0, s32 arg1, s32 arg2) {
    func_002090D0(arg0 + 0xB0, M2C_FIELD(arg0, s32 *, 0xAC));
    M2C_FIELD(arg0, s32 *, 0xB4) = arg1;
    M2C_FIELD(arg0, s32 *, 0xBC) = arg2;
}
