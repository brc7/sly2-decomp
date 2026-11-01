#include "m2c_prelude.h"
s32 func_00187DD0();                                /* extern */
M2C_UNK func_00187DE0(s32, s32);                    /* extern */

/** @sly2-annot addr=0x187D80
 * Purpose: Return pressed&mask and mark those buttons handled (SetJoyBtnHandled)
 * Info: proposed name GrfHandleJoyBtn (T1); TU joy
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/main-loop-startup.md#joy-0x187030-0x188488,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
s32 func_00187D80(s32 arg0, s32 arg1) {
    s32 temp_s2;

    temp_s2 = func_00187DD0();
    func_00187DE0(arg0, arg1);
    return temp_s2;
}
