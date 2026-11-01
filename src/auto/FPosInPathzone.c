#include "m2c_prelude.h"
s32 PcgtPointInCbspQuick(s32);                             /* extern */

/** @sly2-annot addr=0x1C0920
 * Purpose: PcgtPointInCbspQuick(+0xE0) != 0
 * Info: proposed name FPosInPathzone (T1); TU path
 * Notes: names_v2 B proposed CposFindPathzonePath__FP8PATHZONEP6VECTORT1iT1 (0.856)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
s32 FPosInPathzone(s32 arg0) {
    return PcgtPointInCbspQuick(arg0 + 0xE0) != 0;
}
