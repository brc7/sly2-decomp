#include "m2c_prelude.h"
extern f32 D_002DDED0;

/** @sly2-annot addr=0x178CD8
 * Purpose: BHVPATH SetState(n): store state +0x4C and time
 * Info: proposed name SetBhvpathState (T1); class BHVPATH slot15; TU guardbhv
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void SetBhvpathState(void *arg0, s32 arg1) {
    if (arg1 != M2C_FIELD(arg0, s32 *, 0x4C)) {
        M2C_FIELD(arg0, s32 *, 0x4C) = arg1;
        M2C_FIELD(arg0, f32 *, 0x50) = (f32) D_002DDED0;
    }
}
