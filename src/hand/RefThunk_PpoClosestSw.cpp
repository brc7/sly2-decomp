typedef int qword __attribute__((mode(TI)));
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(void *pv);
};
extern "C" {
CRef func_00201360(int c, void *aref, int *ak, void *an);
void *PpoClosestSw(void *p, qword q);
CRef RefThunk_PpoClosestSw(void *p, int c, void *aref);
}
CRef RefThunk_PpoClosestSw(void *p, int c, void *aref)
{
    union { qword q; } u;
    int k = 2;
    CRef ref = func_00201360(1, aref, &k, &u);
    ref = PpoClosestSw(p, u.q);
    return ref;
}
