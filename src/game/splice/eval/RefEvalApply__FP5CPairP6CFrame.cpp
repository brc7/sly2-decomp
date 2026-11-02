// MATCH (Claude agent, laneB6, 2026-10-23), from v4: the case 0x20 loop reuses i with &aref[i] (no pref pointer), and the empty rest list is aref[i].SetNil(), an inline CRef member { Decref(); InitTag(0); }.
// v4 (Claude agent, laneB5, 2026-10-23): aligned 37 -> 12. (1) `int dummy[2]` declared right after SPF spf in case 0x20 fixes the frame (0x240); (2) separate pnFn = p->ref.u.pnode for the refOp call and a new `NODE *pn = pnFn->pnext.p` gives pn s0 like the target; (3) named `CRef r = CFrame_RefAddBinding(...)` in the case 0x20 loop (temp dies after pparam advance). Left: spList arm (target computes i*8+sp separately in each arm), case 0x20 `addiu s2,sp,0x20` (pref) placed in preheader after blez + loop alignment nop.
// v3 (Claude agent, laneB4, 2026-10-23): from the .cpp draft (38, closer than v2's 54): SPN::operator= as an inline 'SPN &operator=(NODE *pn) { Assign(pn); return *this; }' -> 37. Frame still 0x10 short.
struct NODE;
struct CRef {
    int m_tagk; union { int n; float g; void *pv; NODE *pnode; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    void SetNil() { Decref(); InitTag(0); }
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(NODE *pn);
};
extern char D_004520A8[];
extern "C" {
void func_0011CD98(void *pheap, void *pv);
void GcPair_Unlink(void *pv);
NODE *PvNewPair();
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
    void operator delete(void *pv) { GcPair_Unlink(pv); func_0011CD98(D_004520A8, pv); }
};
struct SPN {
    NODE *p;
    void Set(NODE *pn);
    SPN() { p = 0; }
    SPN(NODE *pn) { Set(pn); }
    ~SPN() { NODE *pn = p; if (pn && --pn->cref == 0) delete pn; }
    void Assign(NODE *pn);
    SPN &operator=(NODE *pn) { Assign(pn); return *this; }
    NODE *operator->() { return p; }
    int pad;
};
struct FRAME {
    int cref;
    ~FRAME();
};
struct SPF {
    FRAME *p;
    int pad;
    void Set(FRAME *pf);
    SPF(FRAME *pf) { Set(pf); }
    ~SPF() { FRAME *pf = p; if (pf && --pf->cref == 0) delete pf; }
};
struct BIF {
    CRef (*pfn)(int c, CRef *aref, FRAME *pframe);
    int cargMin;
    int fRest;
};
struct METHOD {
    int x0, x4;
    void *pv;
    CRef (*pfn)(void *pv, int c, CRef *aref);
    int cargMin;
};
struct LAMBDA {
    int x0, x4;
    FRAME *pframe;
    int cargMin;
    int fRest;
    NODE *pparams;
    NODE *pbody;
};
extern BIF D_002BBB90[];
extern "C" {
FRAME *PvNewFrame();
CRef RefEval__FP5CPairP6CFrame(NODE *p, FRAME *pframe);
CRef CFrame_RefAddBinding(FRAME *pframe, int sym, CRef *pref);
void CFrame_SetSingleParent(FRAME *pframe, FRAME *pframeParent);
CRef RefEvalLambdaBody__FP5CPairP6CFrame(LAMBDA *plambda, FRAME *pframe);
CRef RefEvalApply__FP5CPairP6CFrame(NODE *p, FRAME *pframe);
}

/** @sly2-annot addr=0x1272E0
 * Purpose: Function application: evaluates operator; Bifk -> D_002BBB90 table {pfn,cargMin,fRest}; Method; or
 *   lambda (new frame binding params, rest list); then calls
 * Info: proposed name RefEvalApply (T1); TU eval
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/splice-scripting.md#3-the-evaluator, docs/engine/splice-scripting.md#7-key-functions,
 *   docs/formats/splice-scripts.md#3-names
 * @end-sly2-annot */
CRef RefEvalApply__FP5CPairP6CFrame(NODE *p, FRAME *pframe)
{
    CRef ref;
    BIF *pbif = 0;
    LAMBDA *plambda = 0;
    METHOD *pmeth = 0;
    NODE *pnFn = p->ref.u.pnode;
    CRef refOp = RefEval__FP5CPairP6CFrame(pnFn, pframe);
    int cargMin, fRest;
    if (refOp.m_tagk == 0x19) {
        pbif = &D_002BBB90[refOp.u.n];
        cargMin = pbif->cargMin;
        fRest = pbif->fRest;
    } else if (refOp.m_tagk == 0x22) {
        pmeth = (METHOD *)refOp.u.pv;
        fRest = 0;
        cargMin = pmeth->cargMin;
    } else {
        plambda = (LAMBDA *)refOp.u.pv;
        cargMin = plambda->cargMin;
        fRest = plambda->fRest;
    }
    int carg = cargMin + (fRest != 0);
    CRef aref[32];
    NODE *pn = pnFn->pnext.p;
    int c = cargMin;
    int i;
    for (i = 0; i < c; i++) {
        ref = RefEval__FP5CPairP6CFrame(pn, pframe);
        aref[i] = ref;
        pn = pn->pnext.p;
    }
    if (fRest) {
        SPN spCur(pn);
        SPN spLast;
        SPN spList;
        while (spCur.p) {
            SPN spNew(PvNewPair());
            spNew->ref = RefEval__FP5CPairP6CFrame(spCur.p, pframe);
            if (spLast.p) {
                spLast->pnext = spNew.p;
                spLast = spNew.p;
            } else {
                spLast = spNew.p;
                spList = spLast.p;
            }
            c++;
            spCur = spCur->pnext.p;
        }
        if (spList.p) {
            aref[i] = spList.p;
        } else {
            aref[i].SetNil();
        }
    }
    switch (refOp.m_tagk) {
    case 0x19:
        ref = pbif->pfn(c, aref, pframe);
        break;
    case 0x20: {
        SPF spf(PvNewFrame());
    int dummy[2];
        CFrame_SetSingleParent(spf.p, plambda->pframe);
        NODE *pparam = plambda->pparams;
        for (i = 0; i < carg; i++) {
            if (pparam->ref.m_tagk == 0x14)
                pparam = pparam->pnext.p;
            CRef r = CFrame_RefAddBinding(spf.p, pparam->ref.u.n, &aref[i]);
            pparam = pparam->pnext.p;
        }
        ref = RefEvalLambdaBody__FP5CPairP6CFrame(plambda, spf.p);
        break;
    }
    case 0x22:
        ref = pmeth->pfn(pmeth->pv, c, aref);
        break;
    }
    return ref;
}
