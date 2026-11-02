struct NODE;
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(NODE *pn);
    void SetTag(int tagk) { Decref(); InitTag(tagk); }
};
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
    SPN() {}
    SPN(NODE *pn) { Set(pn); }
    SPN(const SPN &);
    ~SPN() { NODE *pn = p; if (pn && --pn->cref == 0) delete pn; }
    NODE *operator->() { return p; }
};
extern "C" SPN func_00121578(CRef **ppref1, CRef **ppref2);
/** @sly2-annot addr=0x121578
 * Purpose: Allocates a 3-node call list (proc a b) and returns pointers to the two arg slots, for
 *   map/filter/for-each
 * Info: proposed name SpnNewApplyList2 (T2); TU bif
 * Notes: names_v2 B proposed RefOpMatrix__FiP4CRefP6CFrame (0.897)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r1_116918
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
SPN func_00121578(CRef **ppref1, CRef **ppref2)
{
    SPN sp;
    sp.Set(func_00256B48());
    sp->ref = func_00256B48();
    NODE *pn1 = (NODE *)sp->ref.u.pv;
    CRef *pref1 = &pn1->ref;
    *pref1 = func_00256B48();
    pn1->SetNext(func_00256B48());
    NODE *pn2 = (NODE *)pref1->u.pv;
    pn2->ref.SetTag(0x12);
    pn2->SetNext(func_00256B48());
    *ppref1 = &pn2->pnext.p->ref;
    CRef *pref2 = &pn1->pnext.p->ref;
    *pref2 = func_00256B48();
    NODE *pn3 = (NODE *)pref2->u.pv;
    pn3->ref.SetTag(0x12);
    pn3->SetNext(func_00256B48());
    *ppref2 = &pn3->pnext.p->ref;
    return SPN(sp.p);
}
