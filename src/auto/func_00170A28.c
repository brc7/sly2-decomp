#include "m2c_prelude.h"
M2C_UNK func_00170A58(s32);                         /* extern */
M2C_UNK func_001A5C48();                            /* extern */

/** @sly2-annot addr=0x170A28
 * Purpose: cid 0x71 PostLoad: LO PostLoad then PrepareCid71Targets
 * Info: proposed name PostCid113Load (T2); class CID113 slot15; TU tu_1709A8
 * Notes: Cid71->Cid113 (CID 0x71=113, vtable)
 * Confidence: low (draft) [T2 0.65] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_00170A28(s32 arg0) {
    func_001A5C48();
    func_00170A58(arg0);
}
