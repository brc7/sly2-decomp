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
struct VTE { short d; short pad; void (*pfn)(void *, int, void *); };
struct LO { char pad[0xC]; VTE *pvt; };
extern "C" { CRef RefThunk_FireMissile(LO *p, int c, void *aref); }
CRef RefThunk_FireMissile(LO *p, int c, void *aref)
{
    int ak[2] = {0x2030, 2};
    union { qword q; } u;
    int n;
    CRef ref = func_00201360(2, aref, ak, &n, &u);
    ref.Decref();
    ref.InitTag(0x24);
    VTE *pvte = &p->pvt[78];
    pvte->pfn((char *)p + pvte->d, n, &u);
    return ref;
}
