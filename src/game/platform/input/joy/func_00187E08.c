#include "m2c_prelude.h"
/** @sly2-annot addr=0x187E08
 * Purpose: Clear mask from held (x10B4) and handled (x10BC)
 * Info: proposed name ClearJoyBtn (T2); TU joy
 * Notes: names_v2 v1 proposed SetJoyBtnHandled__FP3JOYUs (0.200)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/main-loop-startup.md#joy-0x187030-0x188488
 * @end-sly2-annot */
void func_00187E08(void *arg0, s32 arg1) {
    s32 temp_a1;

    temp_a1 = ~arg1;
    M2C_FIELD(arg0, s32 *, 0x10B4) = (s32) (M2C_FIELD(arg0, s32 *, 0x10B4) & temp_a1);
    M2C_FIELD(arg0, s32 *, 0x10BC) = (s32) (M2C_FIELD(arg0, s32 *, 0x10BC) & temp_a1);
}
