#include "m2c_prelude.h"
/** @sly2-annot addr=0x11A1F8
 * Purpose: DL init: ibDle at +8
 * Info: proposed name InitDl (T1); TU dl
 * Notes: names_v2 v1 proposed ClearDl__FP2DL (0.000)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r1_116918+verify_side
 * See: docs/reference/functions/core-library.md#dl-0x11a050-0x11a550
 * @end-sly2-annot */
void InitDl__FP2DLi(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 8) = arg1;
}
