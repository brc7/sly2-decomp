#include "m2c_prelude.h"
M2C_UNK func_00153DE0(s32);                         /* extern */
M2C_UNK func_001A5E28();                            /* extern */

/** @sly2-annot addr=0x153DB0
 * Purpose: LO::Update (0x1A5E28) then class update 0x153DE0
 * Info: proposed name UpdateDetector (T1); class DETECTOR slot16; TU detector
 * Notes: Cid10->Detector (CID 0x10=16, vtable)
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/guards-ai.md#detector-0x153d58-0x154a60
 * @end-sly2-annot */
void func_00153DB0(s32 arg0) {
    func_001A5E28();
    func_00153DE0(arg0);
}
