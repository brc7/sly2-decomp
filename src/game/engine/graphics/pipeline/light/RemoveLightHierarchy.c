#include "m2c_prelude.h"
M2C_UNK func_001A3B38(s32);                         /* extern */
M2C_UNK RemoveLoHierarchy();                            /* extern */

/** @sly2-annot addr=0x1A34B0
 * Purpose: LIGHT vt12: RemoveLoHierarchy then InvalidateLight
 * Info: proposed name RemoveLightHierarchy (T1); class LIGHT slot12; TU light
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void RemoveLightHierarchy(s32 arg0) {
    RemoveLoHierarchy();
    func_001A3B38(arg0);
}
