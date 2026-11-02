#include "m2c_prelude.h"
M2C_UNK RebuildLight();                            /* extern */

/** @sly2-annot addr=0x1A3610
 * Purpose: Store float x150 and rebuild
 * Info: proposed name SetLightMidtoneStrength (T2); TU light
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A3610(void *arg0, f32 fparg0) {
    M2C_FIELD(arg0, f32 *, 0x150) = fparg0;
    RebuildLight();
}
