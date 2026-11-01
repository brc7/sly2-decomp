#include "m2c_prelude.h"
/** @sly2-annot addr=0x1F0528
 * Purpose: cref(+0x10)++
 * Info: proposed name IncrefSma (T1); class SMA slot1; TU sm
 * Notes: names_v2 v1 proposed OidFromSmIsms__FP2SMi (0.023)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: semantic_tu_docs/sm
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
void IncrefSma(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x10) = (s32) (M2C_FIELD(arg0, s32 *, 0x10) + 1);
}
