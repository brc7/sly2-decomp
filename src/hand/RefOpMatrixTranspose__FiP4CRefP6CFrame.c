typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void InitTag__4CRef(CRef *pref, int tagk);
typedef struct MATRIX4 { float m[4][4]; } __attribute__((aligned(16))) MATRIX4;
void TransposeMatrix4(MATRIX4 *pmat, int n);
void CRef_ctor_MATRIX4(CRef *pref, MATRIX4 *pmat);
CRef *RefOpMatrixTranspose__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    MATRIX4 mat;
    TransposeMatrix4(&mat, aref->m_tag);
    CRef_ctor_MATRIX4(&ref, &mat);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
