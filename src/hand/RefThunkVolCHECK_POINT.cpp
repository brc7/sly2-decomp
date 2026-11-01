typedef int qword __attribute__((mode(TI)));
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(bool f);
};
extern "C" {
CRef func_00201360(int c, void *aref, int *ak, void *a0);
int FCheckVolPoint(void *p, qword q);
CRef RefThunkVolCHECK_POINT(void *p, int c, void *aref);
}
CRef RefThunkVolCHECK_POINT(void *p, int c, void *aref)
{
    int ak[1] = {2};
    union { qword q; } u;
    CRef ref = func_00201360(1, aref, ak, &u);
    ref = FCheckVolPoint(p, u.q) != 0;
    return ref;
}
