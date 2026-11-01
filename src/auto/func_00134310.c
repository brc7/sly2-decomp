#include "m2c_prelude.h"
/** @sly2-annot addr=0x134310
 * Purpose: return max(a,b) for ints; used to merge FIgnore*Intersection results
 * Info: proposed name NMax (T2); TU bbmark
 * Notes: v3: superseded NMaxIgnore | top50_round (0.50): Body (m2c). 12 callers combine ignore codes
 *   (FSwShouldPairObjects, FIgnoreBombIntersection, MURRAY_FIgnoreIntersection, FIgnoreSensorIntersection: r
 *   = NMax(r,1)). Out-of-line in bbmark TU; generic name preferred over NMa | mangled withheld (not T1):
 *   NMax__Fii
 * Confidence: low (draft) [T2 0.50] - evidence: semantic review; sources: r2b_rest+top50_round
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/physics-collision.md#bbmark-0x1336f0-0x1344f0,
 *   docs/reference/functions/player.md#throw-0x21f2e0-0x220d40
 * @end-sly2-annot */
s32 func_00134310(s32 arg0, s32 arg1) {
    return (arg1 >= arg0) ? arg1 : arg0;
}
