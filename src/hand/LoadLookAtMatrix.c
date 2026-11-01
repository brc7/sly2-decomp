typedef int VU_VECTOR __attribute__((mode(TI)));
typedef struct MATRIX3 { float mat[3][4]; } __attribute__((aligned(16))) MATRIX3;

extern VU_VECTOR D_002A2DC0;
MATRIX3 BuildOrthonormalMatrixZ(VU_VECTOR vecX, VU_VECTOR vecY);
void *func_0023320C(void *pvDst, const void *pvSrc, unsigned int cb);

/** @sly2-annot addr=0x11B7E0
 * Purpose: BuildOrthonormalMatrixZ(dir, default up)
 * Info: proposed name LoadLookAtMatrix (T1); TU mat
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8
 * @end-sly2-annot */
MATRIX3 *LoadLookAtMatrix(MATRIX3 *pmat, VU_VECTOR vec)
{
    MATRIX3 mat = BuildOrthonormalMatrixZ(vec, D_002A2DC0);
    func_0023320C(pmat, &mat, sizeof(MATRIX3));
    return pmat;
}
