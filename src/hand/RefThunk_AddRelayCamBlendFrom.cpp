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
extern "C" { void func_001E1238(void *p, qword, qword, float, float, int); CRef RefThunk_AddRelayCamBlendFrom(void *p, int c, void *aref); }
CRef RefThunk_AddRelayCamBlendFrom(void *p, int c, void *aref)
{
    int ak[5] = {0x2, 0x2, 0x1, 0x1, 0x106d};
    union { qword q; } v0;
    union { qword q; } v1;
    float v2;
    float v3;
    int v4;
    CRef ref = func_00201360(5, aref, ak, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    func_001E1238(p, v0.q, v1.q, v2, v3, v4);
    return ref;
}
