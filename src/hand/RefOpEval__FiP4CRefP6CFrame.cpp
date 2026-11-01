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
    SPM &operator=(NODE *pn);
};
struct NODE {
    int cref;
    int x4;
    CRef ref;
    SPM pnext;
    void operator delete(void *pv) { func_002550F8(pv); func_0011CD98(D_004520A8, pv); }
};
struct SPN {
    NODE *p;
    void Set(NODE *pn);
    ~SPN() { NODE *pn = p; if (pn && --pn->cref == 0) delete pn; }
    NODE *operator->() { return p; }
};
extern "C" CRef RefEval__FP5CPairP6CFrame(NODE *pn, CRef *aref);
extern "C" CRef RefOpEval__FiP4CRefP6CFrame(void *pbasic, CRef *pref, CRef *aref);
CRef RefOpEval__FiP4CRefP6CFrame(void *pbasic, CRef *pref, CRef *aref)
{
    SPN sp;
    sp.Set(func_00256B48());
    sp->ref = *pref;
    return RefEval__FP5CPairP6CFrame(sp.p, aref);
}
