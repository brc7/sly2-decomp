#include "m2c_prelude.h"
s128 func_001A62E8(s32);                            /* extern */

/** @sly2-annot addr=0x1D0158
 * Purpose: RIPT 4 Project: snap position to followed LO (PosLoWorld)
 * Info: proposed name ProjectGlintTransform (T2); TU rip
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void func_001D0158(void *arg0) {
    M2C_FIELD(arg0, s128 *, 0x80) = (s128) M2C_FIELD(arg0, s128 *, 0x70);
    M2C_FIELD(arg0, s128 *, 0x70) = func_001A62E8(M2C_FIELD(arg0, s32 *, 0xF0));
}
