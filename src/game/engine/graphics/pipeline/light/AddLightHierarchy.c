#include "m2c_prelude.h"
M2C_UNK RebuildLight(s32);                         /* extern */
M2C_UNK AddLoHierarchy();                            /* extern */

/** @sly2-annot addr=0x1A3430
 * Purpose: LIGHT vt11: AddLoHierarchy then RebuildLight
 * Info: proposed name AddLightHierarchy (T1); class LIGHT slot11; TU light
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void AddLightHierarchy(s32 arg0) {
    AddLoHierarchy();
    RebuildLight(arg0);
}
