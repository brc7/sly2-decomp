typedef int qword __attribute__((mode(TI)));
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
extern "C" float RadBetweenVectors(qword a, qword b, void *pvec);
extern "C" CRef func_00122CE0(void *pbasic, CRef *aref);
CRef func_00122CE0(void *pbasic, CRef *aref)
{
    SPN sp;
    sp.Set(func_00256B48());
    sp->SetNext(func_00256B48());
    CRef *pref = &sp->pnext.p->ref;
    pref->SetTag(0x1A);
    sp->ref = RadBetweenVectors(*(qword *)aref[0].u.pv, *(qword *)aref[1].u.pv, pref->u.pv);
    return CRef(sp.p);
}
