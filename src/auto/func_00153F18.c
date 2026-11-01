#include "m2c_prelude.h"
M2C_UNK func_00154080(void *, s32 *, M2C_UNK);      /* extern */
s32 func_00154180();                                /* extern */

/** @sly2-annot addr=0x153F18
 * Purpose: Adds an object id to the detector target list (max 8, entries of 16 bytes at +0x100, count at
 *   +0xFC) with state 0
 * Info: proposed name AddDetectorTarget (T1); TU detector
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/guards-ai.md#detector-0x153d58-0x154a60
 * @end-sly2-annot */
void func_00153F18(void *arg0, s32 arg1) {
    s32 *temp_a1;
    u32 temp_v1;

    if (func_00154180() == 0) {
        temp_v1 = M2C_FIELD(arg0, u32 *, 0xFC);
        if (temp_v1 < 8U) {
            M2C_FIELD(arg0, u32 *, 0xFC) = (u32) (temp_v1 + 1);
            temp_a1 = arg0 + ((temp_v1 * 0x10) + 0x100);
            *temp_a1 = arg1;
            func_00154080(arg0, temp_a1, 0);
        }
    }
}
