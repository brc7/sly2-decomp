#include "m2c_prelude.h"
M2C_UNK func_0011A050(s32, M2C_UNK);                /* extern */
M2C_UNK InitLo();                            /* extern */

/** @sly2-annot addr=0x1709F8
 * Purpose: cid 0x71 (LO child, 0x60) Init: LO Init, then InitDl of the command list at +0x50 (link +0x18)
 * Info: proposed name InitCid113 (T2); class CID113 slot7; TU tu_1709A8
 * Notes: Cid71->Cid113 (CID 0x71=113, vtable)
 * Confidence: low (draft) [T2 0.75] - evidence: static analysis; sources: r3b_rest+verdicts_tu_r3r5
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_001709F8(s32 arg0) {
    InitLo();
    func_0011A050(arg0 + 0x50, 0x18);
}
