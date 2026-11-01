struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
extern char D_003DD490[];
extern "C" {
int func_0015E9C8(void *pg, int k, void *a, int b, int c, void **ap);
CRef RefPairFromAplo__FiPP2LO(int c, void **ap);
CRef RefOpFindClassObjects__FiP4CRefP6CFrame(void *pbasic, CRef *aref);
}
CRef RefOpFindClassObjects__FiP4CRefP6CFrame(void *pbasic, CRef *aref)
{
    CRef ref;
    void *ap[0x180];
    int b = aref[1].u.n;
    int c = func_0015E9C8(D_003DD490, b ? 0x401 : 0x405, aref[0].u.pv, b, 0x180, ap);
    ref = RefPairFromAplo__FiPP2LO(c, ap);
    return ref;
}
