#include "m2c_prelude.h"
/** @sly2-annot addr=0x11A3B0
 * Purpose: pvFirst == NULL
 * Info: proposed name FIsDlEmpty (T1); TU dl
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#dl-0x11a050-0x11a550
 * @end-sly2-annot */
s32 FIsDlEmpty__FP2DL(s32 *arg0) {
    return *arg0 == 0;
}
