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
    CRef &operator=(void *pv);
    CRef &operator=(qword q);
    CRef &operator=(bool f);
    CRef &operator=(int n);
    CRef &operator=(float g);
};
extern "C" CRef func_00201360(int c, void *aref, int *ak, ...);
extern "C" { void func_00132CC8(void *p, int n, void *pu); CRef RefThunkBasicCUSTOM_CLQ_PROPERTY(void *p, int c, void *aref); }
CRef RefThunkBasicCUSTOM_CLQ_PROPERTY(void *p, int c, void *aref)
{
    int ak[2] = {0x15, 9};
    union { qword q; } u;
    int n;
    CRef ref = func_00201360(2, aref, ak, &n, &u);
    ref.Decref();
    ref.InitTag(0x24);
    func_00132CC8(p, n, &u);
    return ref;
}
