typedef struct MATRIX4 { float mat[4][4]; } __attribute__((aligned(16))) MATRIX4;

MATRIX4 func_0011C318(void *pv);
MATRIX4 func_0011C168(MATRIX4 *pmatA, MATRIX4 *pmatB);

/** @sly2-annot addr=0x11C3D0
 * Purpose: dmat = inverse(a) * b
 * Info: proposed name CalculateDmat4 (T1); TU mat
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
void func_0011C3D0(void *pv, MATRIX4 *pmat, MATRIX4 *pmatOut)
{
    MATRIX4 matRes;
    MATRIX4 matT;
    matT = func_0011C318(pv);
    matRes = func_0011C168(pmat, &matT);
    *pmatOut = matRes;
}
