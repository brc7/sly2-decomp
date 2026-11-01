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
extern "C" { void func_001E0310(void *p, CRef *pref); CRef RefThunk_AddRelayWaitBlocks(void *p, int c, void *aref); }
CRef RefThunk_AddRelayWaitBlocks(void *p, int c, void *aref)
{
    int k = 0x8006;
    CRef r;
    CRef ref = func_00201360(1, aref, &k, &r);
    ref.Decref();
    ref.InitTag(0x24);
    func_001E0310(p, &r);
    return ref;
}
