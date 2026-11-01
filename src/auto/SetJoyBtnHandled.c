#include "m2c_prelude.h"
/** @sly2-annot addr=0x187DE0
 * Purpose: Clear mask from pressed (x10B8) and add held&mask to handled (x10BC)
 * Info: proposed name SetJoyBtnHandled (T1); TU joy
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#joy-0x187030-0x188488
 * @end-sly2-annot */
void SetJoyBtnHandled(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0x10B8) = (s32) (M2C_FIELD(arg0, s32 *, 0x10B8) & ~arg1);
    M2C_FIELD(arg0, s32 *, 0x10BC) = (s32) (M2C_FIELD(arg0, s32 *, 0x10BC) | (M2C_FIELD(arg0, s32 *, 0x10B4) & arg1));
}
