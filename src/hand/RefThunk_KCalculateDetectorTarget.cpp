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
extern "C" { int func_001541D8(void *p, int, int); CRef RefThunk_KCalculateDetectorTarget(void *p, int c, void *aref); }
CRef RefThunk_KCalculateDetectorTarget(void *p, int c, void *aref)
{
    int ak[2] = {0x2002, 0x106b};
    int v0;
    int v1;
    CRef ref = func_00201360(2, aref, ak, &v0, &v1);
    ref = func_001541D8(p, v0, v1);
    return ref;
}
