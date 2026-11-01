// (Claude agent, laneB10) MATCH: the !fShuffle reassign goes through an inline SetSpn(SPN*, NODE*) so &spRes is substituted directly (no pseudo for gcse to share with pspHead).
struct NODE;
struct CRef {
    int m_tagk; union { int n; float g; void *pv; NODE *pnode; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    CRef(NODE *p);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
extern char D_004520A8[];
extern "C" {
void func_0011CD98(void *pheap, void *pv);
void func_002550F8(void *pv);
NODE *func_00256B48();
int NRandInRange__Fii(int nLo, int nHi);
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
    void operator delete(void *pv) { func_002550F8(pv); func_0011CD98(D_004520A8, pv); }
};
struct SPN {
    NODE *p;
    void Set(NODE *pn);
    ~SPN() { NODE *pn = p; if (pn && --pn->cref == 0) delete pn; }
    void operator=(NODE *pn);
    NODE *operator->() { return p; }
    int pad[3];
};
extern "C" CRef RefOpShuffle(void *pbasic, CRef *aref);
static inline void SetSpn(SPN *psp, NODE *pn) { *psp = pn; }
static inline void InsertAfter(SPM *psp, SPN &sp) { sp->pnext = psp->p; *psp = sp.p; }
CRef RefOpShuffle(void *pbasic, CRef *aref)
{
    if (aref[0].m_tagk == 0) return CRef(0);
    if (aref[0].m_tagk != 0x1F) return CRef(0);
    CRef *apref[128];
    int c = 0;
    int iNil = -1;
    SPN spRes;
    SPM *pspHead = (SPM *)&spRes;
    NODE *pn;
    for (pn = aref[0].u.pnode; pn; pn = pn->pnext.p) {
        if (pn->ref.m_tagk == 0) iNil = c;
        else apref[c++] = &pn->ref;
    }
    int fShuffle = c >= 2 && iNil < 1;
    if (fShuffle) {
        CRef *prefFirst = apref[0];
        int i;
        for (i = 0; i < c; i++) {
            int j = NRandInRange__Fii(0, c - 1);
            CRef *pref = apref[j];
            apref[j] = apref[i];
            apref[i] = pref;
        }
        if (apref[0] == prefFirst) {
            apref[0] = apref[c - 1];
            apref[c - 1] = prefFirst;
        }
        iNil = c;
    }
    spRes.p = 0;
    NODE *pnLast = 0;
    SPM *psp = pspHead;
    int i;
    for (i = 0; i < c; i++) {
        *psp = func_00256B48();
        NODE *pnNew = psp->p;
        pnNew->ref = *apref[i];
        psp = &pnNew->pnext;
        if (i == iNil - 1) pnLast = pnNew;
    }
    if (pnLast && pnLast != spRes.p) {
        if (!fShuffle) {
            SPN sp;
            sp.Set(spRes.p);
            SetSpn(&spRes, spRes->pnext.p);
            InsertAfter(&pnLast->pnext, sp);
        }
        if (spRes.p != pnLast) {
            SPN sp;
            NODE *pnNew = func_00256B48();
            sp.Set(pnNew);
            sp->ref = CRef(0);
            InsertAfter(&pnLast->pnext, sp);
        }
    }
    return CRef(spRes.p);
}
