#include "m2c_prelude.h"
/** @sly2-annot addr=0x149420
 * Purpose: qsort comparator (difference / 0x14)
 * Info: proposed name CompareVolbtnObjects (T2); TU button
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
s32 func_00149420(s32 *arg0, s32 *arg1) {
    return (s32) ((*arg0 - *arg1) * 0xCCCCCCCD) >> 4;
}
