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
extern "C" { void *func_00156B98(void *p, int a, qword q, int b); CRef RefThunk_EmitSwNoise(void *p, int c, void *aref); }
CRef RefThunk_EmitSwNoise(void *p, int c, void *aref)
{
    int ak[3] = {0x1046, 2, 0x2002};
    union { qword q; } u;
    int a, b;
    CRef ref = func_00201360(3, aref, ak, &a, &u, &b);
    ref = func_00156B98(p, a, u.q, b);
    return ref;
}
