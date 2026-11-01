#include "m2c_prelude.h"
M2C_UNK func_001A3968();                            /* extern */

/** @sly2-annot addr=0x1A3630
 * Purpose: Store float x154 (deg, clamped >=30 in RebuildLight) and rebuild
 * Info: proposed name SetLightMidtoneAngle (T2); TU light
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A3630(void *arg0, f32 fparg0) {
    M2C_FIELD(arg0, f32 *, 0x154) = fparg0;
    func_001A3968();
}
