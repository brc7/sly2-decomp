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
extern "C" { void func_001E1178(void *p, int, int, int, int, float, float, float); CRef RefThunk_AddRelaySound(void *p, int c, void *aref); }
CRef RefThunk_AddRelaySound(void *p, int c, void *aref)
{
    int ak[7] = {0xe, 0x6, 0x5, 0x5, 0x1, 0x1, 0x1};
    int v0;
    int v1;
    int v2;
    int v3;
    float v4;
    float v5;
    float v6;
    CRef ref = func_00201360(7, aref, ak, &v0, &v1, &v2, &v3, &v4, &v5, &v6);
    ref.Decref();
    ref.InitTag(0x24);
    func_001E1178(p, v0, v1, v2, v3, v4, v5, v6);
    return ref;
}
