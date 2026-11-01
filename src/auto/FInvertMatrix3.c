#include "m2c_prelude.h"
M2C_UNK FInvertMatrix__FiPfT1(M2C_UNK, M2C_UNK, s32, s32); /* extern */

/** @sly2-annot addr=0x11B318
 * Purpose: FInvertMatrix(stride 4, n 3, src, dst)
 * Info: proposed name FInvertMatrix3 (T1); TU mat
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8
 * @end-sly2-annot */
void FInvertMatrix3(s32 arg0, s32 arg1) {
    FInvertMatrix__FiPfT1(4, 3, arg0, arg1);
}
