typedef int qword __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; int m_tag; } CRef;
typedef struct MATRIX4 { qword a[4]; } MATRIX4;
void func_0012A0B0(CRef *pref, int tagk);
void ConvertAloMat4(MATRIX4 *pmat, int a, int b, int c);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
void func_00129EC8(CRef *pref);
CRef *RefOpConvertObjectMatrix__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    MATRIX4 mat;
    MATRIX4 *pmat;
    func_0012A0B0(&ref, 0x1B);
    pmat = (MATRIX4 *)ref.m_tag;
    ConvertAloMat4(&mat, aref[0].m_tag, aref[1].m_tag, aref[2].m_tag);
    *pmat = mat;
    func_001291A8(ret, &ref);
    func_00129EC8(&ref);
    return ret;
}
