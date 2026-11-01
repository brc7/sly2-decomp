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
extern "C" { void *func_001FE5F0(void *p, int a, int b); CRef RefThunk_StartDialog(void *p, int c, void *aref); }
CRef RefThunk_StartDialog(void *p, int c, void *aref)
{
    int ak[2] = {0x10, 0x105D};
    int a, b;
    CRef ref = func_00201360(2, aref, ak, &a, &b);
    ref = func_001FE5F0(p, a, b);
    return ref;
}
