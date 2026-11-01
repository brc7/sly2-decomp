#include "m2c_prelude.h"
M2C_UNK func_001A3968(s32);                         /* extern */
M2C_UNK RotateXfmToMat();                            /* extern */

/** @sly2-annot addr=0x1A3580
 * Purpose: LIGHT vt30: XFM RotateToMat then RebuildLight
 * Info: proposed name RotateLightToMat (T1); class LIGHT slot30; TU light
 * Notes: names_v2 v1 proposed OnLightAdd__FP5LIGHT (0.000)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A3580(s32 arg0) {
    RotateXfmToMat();
    func_001A3968(arg0);
}
