#include "m2c_prelude.h"
M2C_UNK func_0011C8E8(M2C_UNK, M2C_UNK, s32, s32);  /* extern */

/** @sly2-annot addr=0x11C1D8
 * Purpose: Transpose (0x11C8E8 with 4,4)
 * Info: proposed name TransposeMatrix4 (T1); TU mat
 * Notes: names_v2 v1 proposed LoadMatrixFromPosRot__FP6VECTORP7MATRIX3P7MATRIX4 (0.638)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r1_116918+verify_side
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
s32 func_0011C1D8(s32 arg0, s32 arg1) {
    func_0011C8E8(4, 4, arg1, arg0);
    return arg0;
}
