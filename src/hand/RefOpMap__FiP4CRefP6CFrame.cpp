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
    CRef &operator=(NODE *p);
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
    void operator delete(void *pv) { func_002550F8(pv); func_0011CD98(D_004520A8, pv); }
};
struct SPN {
    NODE *p;
    void Set(NODE *pn);
    ~SPN() { NODE *pn = p; if (pn && --pn->cref == 0) delete pn; }
    NODE *operator->() { return p; }
    int pad[3];
};
extern "C" void func_00121578(SPN *psp, CRef **ppa, CRef **ppb);
extern "C" CRef RefEval__FP5CPairP6CFrame(NODE *pn, CRef *aref);
extern "C" CRef RefOpMap__FiP4CRefP6CFrame(void *pbasic, CRef *aref, CRef *arefCall);
/** @sly2-annot addr=0x121710
 * Purpose: Splice builtin "map": builds (proc a b) via helper 0x121578, eval 0x127838 per elem, collects
 *   results
 * Info: proposed name RefOpMap (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef RefOpMap__FiP4CRefP6CFrame(void *pbasic, CRef *aref, CRef *arefCall) { if (aref[1].m_tagk == 0) return aref[1];
 SPN spRes; spRes.Set(func_00256B48());
 SPN sp; CRef *pa; CRef *pb; func_00121578(&sp, &pa, &pb);
 *pa = aref[0]; NODE *pTail; NODE *pn; pn = (NODE *)aref[1].u.pv; while (pn) { *pb = pn->ref; SPN spNew; NODE *pnNew = func_00256B48(); spNew.Set(pnNew); spNew->ref = RefEval__FP5CPairP6CFrame(sp.p, arefCall);
   if (spRes->ref.m_tagk != 0x1F) spRes->ref = spNew.p; else pTail->pnext = spNew.p;
   NODE *p = spNew.p; pn = pn->pnext.p; pTail = p; } return CRef((NODE *)spRes->ref.u.pv); }
