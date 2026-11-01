#include "m2c_prelude.h"
/** @sly2-annot addr=0x20BB48
 * Purpose: returns pasegbl(+4)->x50
 * Info: proposed name PxGaitBlend (T2); TU tu_20B840
 * Notes: names_v2 C proposed PdleFromDlEntry__FP2DLPv (0.198)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28
 * @end-sly2-annot */
s32 func_0020BB48(void *arg0) {
    return M2C_FIELD(M2C_FIELD(arg0, void **, 4), s32 *, 0x50);
}
