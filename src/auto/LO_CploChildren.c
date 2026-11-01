#include "m2c_prelude.h"
/** @sly2-annot addr=0x1A5BE8
 * Purpose: LO vt28 default: no children (return 0)
 * Info: proposed name LO_CploChildren (T1); class LO slot28; TU lo
 * Notes: names_v2 v1 proposed GetLightDiffuse_Old__FP5LIGHTPf (0.002)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r4_17F090
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
s32 LO_CploChildren(void) {
    return 0;
}
