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
extern "C" CRef func_00201360(int c, void *aref, int *ak, void *an);
struct VT { short d; short pad; void *pfn; };
struct LO { char pad[0xC]; VT *pvt; };
extern "C" CRef RefThunkSoPOINT_IN_BSP_QUICK(LO *p, int c, void *aref);
CRef RefThunkSoPOINT_IN_BSP_QUICK(LO *p, int c, void *aref)
{
    union { qword q; } u;
    int k = 2;
    CRef ref = func_00201360(1, aref, &k, &u);
    VT *pvt = &p->pvt[73];
    ref = ((int (*)(void *, qword))pvt->pfn)((char *)p + pvt->d, u.q) != 0;
    return ref;
}
