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
CRef func_00127838(NODE *p, FRAME *pframe);
CRef func_00127C00(FRAME *pframe, int sym, CRef *pref);
void func_00127B18(FRAME *pframe, FRAME *pframeParent);
CRef func_00126DB8(P *p, FRAME *pframe);
}
/** @sly2-annot addr=0x126DB8
 * Purpose: (let ((sym val)...) body): new CFrame (0x256DD0), RefAddBinding each, then body
 * Info: proposed name RefEvalLet (T1); TU eval
 * Confidence: high [T1 0.92] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18
 * @end-sly2-annot */
CRef func_00126DB8(P *p, FRAME *pframe)
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
            refVal = func_00127838(pb->pnext, pframe);
            refT = func_00127C00(spf.p, pb->ref.u.n, &refVal);
        }
        func_00127B18(spf.p, pframe);
    } else {
        spf = pframe;
    }
    NODE *pq;
    for (pq = pargs->pnext; pq; pq = pq->pnext)
        ref = func_00127838(pq, spf.p);
    return ref;
}
