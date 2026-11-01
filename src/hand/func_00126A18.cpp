// func_00126A18: walks a clause list; first clause whose test is not false (tagk 0x17 with value 0) wins
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct NODE { int x0, x4; CRef ref; NODE *pnext; };
struct P { char pad[0xC]; struct { char pad[0x10]; NODE *pcl; } *pv; };
extern "C" {
CRef func_00127838(NODE *p, void *penv);
CRef func_00126A18(P *p, void *penv);
}
/** @sly2-annot addr=0x126A18
 * Purpose: (cond clauses...) with else (tag 0xB)
 * Info: proposed name RefEvalCond (T1); TU eval
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18
 * @end-sly2-annot */
CRef func_00126A18(P *p, void *penv)
{
    CRef ref;
    CRef test;
    NODE *pcl;
    ref.Decref();
    ref.InitTag(0x24);
    pcl = p->pv->pcl;
    while (pcl) {
        NODE *pc = (NODE *)pcl->ref.u.pv;
        int f;
        if (pc->ref.m_tagk == 0xB) {
            f = 1;
        } else {
            test = func_00127838(pc, penv);
            f = 0;
            if (test.m_tagk != 0x17 || test.u.n)
                f = 1;
            if (f && !pc->pnext)
                ref = test;
        }
        if (f) {
            for (pc = pc->pnext; pc; pc = pc->pnext)
                ref = func_00127838(pc, penv);
            pcl = 0;
        } else {
            pcl = pcl->pnext;
        }
    }
    return ref;
}
