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
extern "C" CRef RefOpCons__FiP4CRefP6CFrame(void *pbasic, CRef *aref);
/** @sly2-annot addr=0x120B08
 * Purpose: Splice builtin "cons": new pair node (ref=a0, next=a1 if list)
 * Info: proposed name RefOpCons (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_B+verify_side
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef RefOpCons__FiP4CRefP6CFrame(void *pbasic, CRef *aref)
{
    SPN sp;
    sp.Set(func_00256B48());
    sp->ref = aref[0];
    CRef *pref = &aref[1];
    if (pref->m_tagk == 0x1F)
        sp->pnext = (NODE *)pref->u.pv;
    return CRef(sp.p);
}
