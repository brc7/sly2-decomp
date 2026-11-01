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
extern "C" CRef func_00127838(NODE *pn, CRef *aref);
extern "C" CRef func_00121D20(void *pbasic, CRef *pref, CRef *aref);
/** @sly2-annot addr=0x121D20
 * Purpose: Splice builtin "eval": wraps arg in list and applies 0x127838
 * Info: proposed name RefOpEval (T1); TU bif
 * Notes: names_v2 B proposed RefOpLm__FiP4CRefP6CFrame (0.856)
 * Confidence: high [T1 0.93] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_00121D20(void *pbasic, CRef *pref, CRef *aref)
{
    SPN sp;
    sp.Set(func_00256B48());
    sp->ref = *pref;
    return func_00127838(sp.p, aref);
}
