typedef int VU_VECTOR __attribute__((mode(TI)));
typedef struct MATRIX3 { float mat[3][4]; } __attribute__((aligned(16))) MATRIX3;

extern VU_VECTOR D_002A2DC0;
MATRIX3 BuildOrthonormalMatrixZ(VU_VECTOR vecX, VU_VECTOR vecY);
void *func_0023320C(void *pvDst, const void *pvSrc, unsigned int cb);

MATRIX3 *LoadLookAtMatrix(MATRIX3 *pmat, VU_VECTOR vec)
{
    MATRIX3 mat = BuildOrthonormalMatrixZ(vec, D_002A2DC0);
    func_0023320C(pmat, &mat, sizeof(MATRIX3));
    return pmat;
}
