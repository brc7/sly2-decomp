#include "m2c_prelude.h"
/** @sly2-annot addr=0x20F0B0
 * Purpose: SW+0x3D78 > 0
 * Info: proposed name FSwHandsOff (T1); TU sw
 * Notes: names_v2 C proposed GetSwPlayerSuck__FP2SWPf (0.200)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/world-sw.md#5-hands-off-and-locks, docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
s32 func_0020F0B0(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x3D78) > 0;
}
