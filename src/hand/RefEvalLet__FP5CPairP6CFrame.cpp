struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct NODE { int x0, x4; CRef ref; NODE *pnext; };
struct P { char pad[0xC]; struct { char pad[0x10]; NODE *pargs; } *pv; };
struct FRAME {
    int cref;
    ~FRAME();
};
struct SPF {
    FRAME *p;
    SPF() { p = 0; }
    ~SPF() { FRAME *pf = p; if (pf && --pf->cref == 0) delete pf; }
    void operator=(FRAME *pf);
};
extern "C" {
FRAME *func_00256DD0();
CRef RefEval__FP5CPairP6CFrame(NODE *p, FRAME *pframe);
CRef CFrame_RefAddBinding(FRAME *pframe, int sym, CRef *pref);
void CFrame_SetSingleParent(FRAME *pframe, FRAME *pframeParent);
CRef RefEvalLet__FP5CPairP6CFrame(P *p, FRAME *pframe);
}
CRef RefEvalLet__FP5CPairP6CFrame(P *p, FRAME *pframe)
{
    CRef ref;
    CRef refVal;
    CRef refT;
    SPF spf;
    NODE *pargs = p->pv->pargs;
    NODE *pn;
    if (pargs->ref.m_tagk == 0x1F) {
        FRAME *pfNew = func_00256DD0();
        spf = pfNew;
        for (pn = (NODE *)pargs->ref.u.pv; pn; pn = pn->pnext) {
            NODE *pb = (NODE *)pn->ref.u.pv;
            refVal = RefEval__FP5CPairP6CFrame(pb->pnext, pframe);
            refT = CFrame_RefAddBinding(spf.p, pb->ref.u.n, &refVal);
        }
        CFrame_SetSingleParent(spf.p, pframe);
    } else {
        spf = pframe;
    }
    NODE *pq;
    for (pq = pargs->pnext; pq; pq = pq->pnext)
        ref = RefEval__FP5CPairP6CFrame(pq, spf.p);
    return ref;
}
