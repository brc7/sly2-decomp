struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct W;
extern char D_004520A8[];
extern "C" {
void func_0011CD98(void *pheap, void *pv);
void func_002550F8(void *pv);
W *func_00256B48();
CRef CRef_CloneTo(CRef *pref, void *a, void *b);
}
struct SPM {
    W *p;
    ~SPM();
    SPM &operator=(W *pw);
};
struct W {
    int cref;
    int x4;
    CRef ref;
    SPM pnext;
    void SetNext(W *pw) { pnext = pw; }
    void operator delete(void *pv) { func_002550F8(pv); func_0011CD98(D_004520A8, pv); }
};
struct SPW {
    W *p;
    void Set(W *pw);
    SPW() {}
    SPW(W *pw) { Set(pw); }
    SPW(const SPW &);
    ~SPW() { W *pw = p; if (pw && --pw->cref == 0) delete pw; }
    W *operator->() { return p; }
};
extern "C" SPW CPair_CloneTo(W *src, void *a, void *b);
SPW CPair_CloneTo(W *src, void *a, void *b)
{
    SPW sp;
    sp.Set(func_00256B48());
    sp->ref = CRef_CloneTo(&src->ref, a, b);
    if (src->pnext.p)
        sp->SetNext(CPair_CloneTo(src->pnext.p, a, b).p);
    return SPW(sp.p);
}
