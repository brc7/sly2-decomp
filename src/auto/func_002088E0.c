#include "m2c_prelude.h"
M2C_UNK func_002089E8();                            /* extern */
M2C_UNK func_00208B30(void *);                      /* extern */

/** @sly2-annot addr=0x2088E0
 * Purpose: Detector count=0, clear +0x394/+0x3A4/+0x3A8/+0x3C0
 * Info: proposed name ClearStepguardTargetLists (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_002088E0(void *arg0) {
    M2C_FIELD(M2C_FIELD(arg0, void **, 0xF64), s32 *, 0xFC) = 0;
    M2C_FIELD(arg0, s32 *, 0x394) = 0;
    M2C_FIELD(arg0, s32 *, 0x3A4) = 0;
    func_002089E8();
    func_00208B30(arg0);
}
