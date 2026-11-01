#include "m2c_prelude.h"
M2C_UNK FInvertMatrix__FiPfT1(M2C_UNK, M2C_UNK, s32, s32); /* extern */

/** @sly2-annot addr=0x11C2F0
 * Purpose: FInvertMatrix(4,4,src,dst)
 * Info: proposed name FInvertMatrix4 (T1); TU mat
 * Notes: names_v2 v1 proposed LoadLookAtMatrix__FP6VECTORP7MATRIX3 (0.002)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
void func_0011C2F0(s32 arg0, s32 arg1) {
    FInvertMatrix__FiPfT1(4, 4, arg0, arg1);
}
