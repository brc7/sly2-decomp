#include "m2c_prelude.h"
/** @sly2-annot addr=0x11A1E8
 * Purpose: pvFirst == NULL
 * Info: proposed name FIsSlEmpty (T1); TU dl
 * Notes: names_v2 v1 proposed InitDl__FP2DLi (0.008)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/core-library.md#dl-0x11a050-0x11a550
 * @end-sly2-annot */
s32 FIsSlEmpty(s32 *arg0) {
    return *arg0 == 0;
}
