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
void GcPair_Unlink(void *pv);
NODE *PvNewPair();
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
    void operator delete(void *pv) { GcPair_Unlink(pv); func_0011CD98(D_004520A8, pv); }
};
struct SPN {
    NODE *p;
    void Set(NODE *pn);
    ~SPN() { NODE *pn = p; if (pn && --pn->cref == 0) delete pn; }
    NODE *operator->() { return p; }
};
extern "C" void func_00121578(SPN *psp, CRef **ppa, CRef **ppb);
extern "C" CRef RefEval__FP5CPairP6CFrame(NODE *pn, CRef *aref);
extern "C" CRef RefOpForEach__FiP4CRefP6CFrame(void *pbasic, CRef *aref, CRef *arefCall);
/** @sly2-annot addr=0x121C20
 * Purpose: Splice builtin "for-each": same, discards results
 * Info: proposed name RefOpForEach (T1); TU bif
 * Notes: names_v2 C proposed RefOpClq__FiP4CRefP6CFrame (0.638)
 * Confidence: high [T1 0.93] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef RefOpForEach__FiP4CRefP6CFrame(void *pbasic, CRef *aref, CRef *arefCall)
{
    if (aref[1].m_tagk == 0x1F) {
        SPN sp;
        CRef *pa;
        CRef *pb;
        NODE *pn;
        func_00121578(&sp, &pa, &pb);
        *pa = aref[0];
        pn = (NODE *)aref[1].u.pv;
        while (pn) {
            *pb = pn->ref;
            CRef ref = RefEval__FP5CPairP6CFrame(sp.p, arefCall);
            pn = pn->pnext.p;
        }
    }
    return CRef(0x24);
}
