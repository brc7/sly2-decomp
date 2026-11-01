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
extern "C" { void func_001DF908(void *p, int, float, float, float, int, int, float, float, float, int); CRef RefThunk_AddLasenBeam(void *p, int c, void *aref); }
CRef RefThunk_AddLasenBeam(void *p, int c, void *aref)
{
    int ak[10] = {0x6, 0x1, 0x1, 0x1, 0x100b, 0x6, 0x1, 0x1, 0x1, 0x100b};
    int v0;
    float v1;
    float v2;
    float v3;
    int v4;
    int v5;
    float v6;
    float v7;
    float v8;
    int v9;
    CRef ref = func_00201360(10, aref, ak, &v0, &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8, &v9);
    ref.Decref();
    ref.InitTag(0x24);
    func_001DF908(p, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
    return ref;
}
