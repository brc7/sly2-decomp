struct NODE;
struct OBJ;
struct CRef {
    int m_tagk; union { int n; float g; void *pv; NODE *pnode; } u;
    CRef() { m_tagk = -1; }
    CRef(int n, int tagk) { m_tagk = tagk; u.n = n; }
    CRef(const CRef &r);
    CRef(NODE *p);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(int n);
    CRef &operator=(NODE *p);
    CRef &operator=(OBJ *p);
    void SetTag(int tagk) { Decref(); InitTag(tagk); }
};
extern char D_004520A8[];
struct G;
extern G D_003DD490;
struct CLOCK { float t; float dt; };
extern CLOCK D_002DDED0;
struct R { char pad[0xC]; int n; };
extern "C" {
void func_0011CD98(void *pheap, void *pv);
void func_002550F8(void *pv);
NODE *func_00256B48();
CRef CRef_RefCoerceF32(CRef *pref);
R *func_00133040(G *pg, int n, NODE *pnode, float t);
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
extern "C" CRef RefOpScheduleCallback__FiP4CRefP6CFrame(int c, CRef *aref);
CRef RefOpScheduleCallback__FiP4CRefP6CFrame(int c, CRef *aref) { CRef res(-1, 0x15); SPN sp; OBJ *pobj = (OBJ *)aref[1].u.pv; NODE *pnNew = func_00256B48(); sp.Set(pnNew); sp->ref = func_00256B48(); NODE *pn = sp->ref.u.pnode; pn->ref = pobj; if (c >= 3) { NODE *pnT = pn; for (NODE *pw = aref[2].u.pnode; pw; pw = pw->pnext.p) { pnT->SetNext(func_00256B48()); pnT = pnT->pnext.p; CRef *pref = &pnT->ref; *pref = func_00256B48(); NODE *pc = pref->u.pnode; pc->ref.SetTag(0x12); pc->SetNext(func_00256B48()); pc->pnext.p->ref = pw->ref; } } G *pg = &D_003DD490; float t = D_002DDED0.t; R *pr = func_00133040(pg, 0, sp.p, t + CRef_RefCoerceF32(aref).u.g); res = pr->n; return res; }
