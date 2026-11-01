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
extern "C" { void ExplodeExplParams(void *p, int, int, qword, qword, float, float); CRef RefThunkExplEXPLODE_OVR(void *p, int c, void *aref); }
CRef RefThunkExplEXPLODE_OVR(void *p, int c, void *aref)
{
    int ak[6] = {0x1026, 0x2002, 0x2, 0x2, 0x1, 0x1};
    union { qword q; } v2;
    union { qword q; } v3;
    int v0;
    int v1;
    float v4;
    float v5;
    CRef ref = func_00201360(6, aref, ak, &v0, &v1, &v2, &v3, &v4, &v5);
    ref.Decref();
    ref.InitTag(0x24);
    ExplodeExplParams(p, v0, v1, v2.q, v3.q, v4, v5);
    return ref;
}
