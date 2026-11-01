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
extern "C" { void * func_00130598(void *p, int, int, float, float, int); CRef RefThunkAsegENSURE(void *p, int c, void *aref); }
CRef RefThunkAsegENSURE(void *p, int c, void *aref)
{
    int ak[5] = {0x2030, 0x100c, 0x1, 0x1, 0x100b};
    int v0;
    int v1;
    float v2;
    float v3;
    int v4;
    CRef ref = func_00201360(5, aref, ak, &v0, &v1, &v2, &v3, &v4);
    ref = func_00130598(p, v0, v1, v2, v3, v4);
    return ref;
}
