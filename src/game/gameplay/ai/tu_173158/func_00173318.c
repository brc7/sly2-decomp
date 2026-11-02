#include "m2c_prelude.h"
/** @sly2-annot addr=0x173318
 * Purpose: Appends spawn selector kind 1 (a curve, sampled)
 * Info: proposed name AddCid119SpawnCurve (T2); class CID119; TU tu_173158
 * Notes: Cid77->Cid119 (CID 0x77=119, agent-default(hex)) | verifier PLAUSIBLE: Placeholder class. Appends
 *   {kind=1, arg} to 0x1C-stride array +0x54 (max 128); data-ref only (splice). 'spawn'/'curve' not shown by
 *   this body. | T2 forced: verifier: placeholder name
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest+verdicts_followups
 * See: docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898
 * @end-sly2-annot */
void func_00173318(void *arg0, s32 arg1) {
    s32 temp_a1;
    void *temp_v1;

    temp_a1 = M2C_FIELD(arg0, s32 *, 0x50);
    if (temp_a1 < 0x80) {
        M2C_FIELD(arg0, s32 *, 0x50) = (s32) (temp_a1 + 1);
        temp_v1 = M2C_FIELD(arg0, s32 *, 0x54) + (temp_a1 * 0x1C);
        M2C_FIELD(temp_v1, s32 *, 0) = 1;
        M2C_FIELD(temp_v1, s32 *, 4) = arg1;
    }
}
