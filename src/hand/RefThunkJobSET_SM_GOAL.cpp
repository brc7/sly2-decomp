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
extern "C" { void func_00162728(void *p, int, int, int, int, int); CRef RefThunkJobSET_SM_GOAL(void *p, int c, void *aref); }
CRef RefThunkJobSET_SM_GOAL(void *p, int c, void *aref)
{
    int ak[5] = {0x1050, 0x6, 0x6, 0x6, 0xf};
    int v0;
    int v1;
    int v2;
    int v3;
    int v4;
    CRef ref = func_00201360(5, aref, ak, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    func_00162728(p, v0, v1, v2, v3, v4);
    return ref;
}
