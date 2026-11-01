#include "m2c_prelude.h"
/** @sly2-annot addr=0x1308A8
 * Purpose: cref(+0x10)++
 * Info: proposed name IncrefAsega (T1); class ASEGA slot1; TU asega
 * Notes: names_v2 v1 proposed CloneAsegbl__FP6ASEGBLP2LO (0.001)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r1_116918
 * See: docs/engine/animation.md#6-source-files,
 *   docs/reference/functions/animation.md#asega-0x1308a8-0x131f70,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
void func_001308A8(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x10) = (s32) (M2C_FIELD(arg0, s32 *, 0x10) + 1);
}
