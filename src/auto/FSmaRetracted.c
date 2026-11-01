#include "m2c_prelude.h"
s32 FFindDlEntry__FP2DLPv(M2C_UNK *, s32);                  /* extern */
extern M2C_UNK D_003E126C;

/** @sly2-annot addr=0x1F0638
 * Purpose: True if the SMA is not in the live list D_003E126C
 * Info: proposed name FSmaRetracted (T1); TU sm
 * Notes: names_v2 v1 proposed GetSmaGoal__FP3SMAP3OID (0.004)
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; static naming pass; sources:
 *   semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
s32 FSmaRetracted(s32 arg0) {
    return FFindDlEntry__FP2DLPv(&D_003E126C, arg0) == 0;
}
