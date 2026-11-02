#include "m2c_prelude.h"
/** @sly2-annot addr=0x1732E0
 * Purpose: Appends spawn selector kind 0 (an LO spot) with oid (max 128)
 * Info: proposed name AddCid119SpawnLo (T2); class CID119; TU tu_173158
 * Notes: Cid77->Cid119 (CID 0x77=119, agent-default(hex))
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898
 * @end-sly2-annot */
void func_001732E0(void *arg0, s32 arg1) {
    s32 temp_a2;
    void *temp_v1;

    temp_a2 = M2C_FIELD(arg0, s32 *, 0x50);
    if (temp_a2 < 0x80) {
        M2C_FIELD(arg0, s32 *, 0x50) = (s32) (temp_a2 + 1);
        temp_v1 = M2C_FIELD(arg0, s32 *, 0x54) + (temp_a2 * 0x1C);
        M2C_FIELD(temp_v1, s32 *, 4) = arg1;
        M2C_FIELD(temp_v1, s32 *, 0) = 0;
    }
}
