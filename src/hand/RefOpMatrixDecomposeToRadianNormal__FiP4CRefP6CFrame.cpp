struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    CRef(struct NODE *p);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(float g);
    void SetTag(int tagk) { Decref(); InitTag(tagk); }
};
struct NODE;
extern char D_004520A8[];
extern "C" {
void func_0011CD98(void *pheap, void *pv);
void func_002550F8(void *pv);
NODE *func_00256B48();
}
struct SPM {
    NODE *p;
    ~SPM();
    void operator=(NODE *pn);
};
struct NODE {
    int cref;
    int x4;
    CRef ref;
    SPM pnext;
    void SetNext(NODE *pn) { pnext = pn; }
    void operator delete(void *pv) { func_002550F8(pv); func_0011CD98(D_004520A8, pv); }
};
struct SPN {
    NODE *p;
    void Set(NODE *pn);
    ~SPN() { NODE *pn = p; if (pn && --pn->cref == 0) delete pn; }
    NODE *operator->() { return p; }
};
extern "C" void func_0011BCF8(void *pv, float *pg, void *pvec);
extern "C" CRef RefOpMatrixDecomposeToRadianNormal__FiP4CRefP6CFrame(void *pbasic, CRef *aref);
CRef RefOpMatrixDecomposeToRadianNormal__FiP4CRefP6CFrame(void *pbasic, CRef *aref)
{
    SPN sp;
    struct { float g, h, i; } g;
    sp.Set(func_00256B48());
    sp->SetNext(func_00256B48());
    CRef *pref = &sp->pnext.p->ref;
    pref->SetTag(0x1A);
    func_0011BCF8(aref[0].u.pv, &g.g, pref->u.pv);
    sp->ref = g.g;
    return CRef(sp.p);
}
