#include "m2c_prelude.h"
M2C_UNK RebuildLight();                            /* extern */

/** @sly2-annot addr=0x1A35F0
 * Purpose: Store color vector xF0 (HSV, converted with ConvertUserHsvToUserRgb in RebuildLight) and rebuild
 * Info: proposed name SetLightHighlightColor (T2); TU light
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A35F0(void *arg0, s128 arg1) {
    M2C_FIELD(arg0, s128 *, 0xF0) = arg1;
    RebuildLight();
}
