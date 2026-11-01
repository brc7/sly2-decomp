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
CRef func_00201360(int c, void *aref, int *ak, int *an);
int func_00132B10(void *p, int n);
CRef RefThunkBasicDERIVED_FROMQ(void *p, int c, void *aref);
}
CRef RefThunkBasicDERIVED_FROMQ(void *p, int c, void *aref)
{
    int k = 7;
    int n;
    CRef ref = func_00201360(1, aref, &k, &n);
    ref = func_00132B10(p, n) != 0;
    return ref;
}
