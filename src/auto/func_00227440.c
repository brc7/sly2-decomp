#include "m2c_prelude.h"
extern f32 D_002DDED0;

/** @sly2-annot addr=0x227440
 * Purpose: if changed: store flag 0x110, time 0x114 = g_clock.t
 * Info: proposed name SetCid5ZoneInside (T2); class CID5; TU vis
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/rendering.md#vis-0x227350-0x2275b8
 * @end-sly2-annot */
void func_00227440(void *arg0, s32 arg1) {
    if (arg1 != M2C_FIELD(arg0, s32 *, 0x110)) {
        M2C_FIELD(arg0, s32 *, 0x110) = arg1;
        M2C_FIELD(arg0, f32 *, 0x114) = (f32) D_002DDED0;
    }
}
