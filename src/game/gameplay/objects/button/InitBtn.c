#include "m2c_prelude.h"
/** @sly2-annot addr=0x148718
 * Purpose: BTN defaults: state -1, rates 1.0
 * Info: proposed name InitBtn (T1); TU button
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void InitBtn(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x90) = -1;
    M2C_FIELD(arg0, f32 *, 0x8C) = 1.0f;
    M2C_FIELD(arg0, f32 *, 0x98) = 1.0f;
}
