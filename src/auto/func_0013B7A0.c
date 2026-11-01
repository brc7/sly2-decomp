#include "m2c_prelude.h"
/** @sly2-annot addr=0x13B7A0
 * Purpose: qsort comparator (pointer difference / element size)
 * Info: proposed name CompareBombVictims (T2); TU bomb
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#bomb-0x13a9e8-0x13bdf0
 * @end-sly2-annot */
s32 func_0013B7A0(s32 arg0, s32 arg1) {
    return (s32) ((arg1 - arg0) * 0x3CF3CF3D) >> 4;
}
