struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct OBJ { int a[4]; OBJ(); ~OBJ(); };
struct X { int a[4]; };
extern char D_003E0EA8[];
extern char D_003D4A60[];
extern "C" {
void FillAsFromPair(void *pv, X *px);
CRef func_001D80C8(void *a, X *px, CRef *pref, void *b);
CRef RefOpSetSaveVar(void *pbasic, CRef *aref);
}
CRef RefOpSetSaveVar(void *pbasic, CRef *aref)
{
    CRef ref;
    OBJ obj;
    X x;
    FillAsFromPair(aref[0].u.pv, &x);
    ref = func_001D80C8(D_003E0EA8, &x, &aref[1], D_003D4A60);
    return ref;
}
