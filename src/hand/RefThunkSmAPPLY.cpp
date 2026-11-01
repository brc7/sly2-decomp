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
extern "C" { void *func_001F0380(void *p, int a, int b, int c); CRef RefThunkSmAPPLY(void *p, int c, void *aref); }
CRef RefThunkSmAPPLY(void *p, int c, void *aref)
{
    int ak[3] = {0x2030, 6, 0x100B};
    int a, b, d;
    CRef ref = func_00201360(3, aref, ak, &a, &b, &d);
    ref = func_001F0380(p, a, b, d);
    return ref;
}
