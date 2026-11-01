typedef int qword __attribute__((mode(TI)));
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct VECTOR { float x, y, z, w; } __attribute__((aligned(16)));
struct MATRIX3 { VECTOR mat[3]; };
struct MATRIX4 { VECTOR mat[3]; VECTOR pos; };
struct XF { char pad[0x30]; qword pos; };
extern "C" {
void TiltMatUpright(XF *pxf, int f, MATRIX3 *pmat);
VECTOR *func_00256D18(VECTOR *pv, qword q);
CRef func_001232D0(void *pbasic, CRef *aref);
}
CRef func_001232D0(void *pbasic, CRef *aref)
{
    MATRIX3 mat;
    TiltMatUpright((XF *)aref[0].u.pv, 0, &mat);
    CRef ref(0x1B);
    MATRIX4 *pmat = (MATRIX4 *)ref.u.pv;
    func_00256D18(&pmat->pos, ((XF *)aref[0].u.pv)->pos);
    *(MATRIX3 *)pmat = mat;
    pmat->mat[2].w = 0.0f;
    pmat->mat[1].w = 0.0f;
    pmat->mat[0].w = 0.0f;
    pmat->pos.w = 1.0f;
    return ref;
}
