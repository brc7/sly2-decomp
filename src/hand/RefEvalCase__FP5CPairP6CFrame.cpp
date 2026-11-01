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
struct P { char pad[0xC]; struct { char pad[0x10]; NODE *pargs; } *pv; };
extern "C" {
CRef RefEval__FP5CPairP6CFrame(NODE *p, void *penv);
CRef RefOpEqv__FiP4CRefP6CFrame(int carg, CRef *aref, void *penv);
CRef RefEvalCase__FP5CPairP6CFrame(P *p, void *penv);
}
/** @sly2-annot addr=0x126BA0
 * Purpose: (case key clauses...): compares via RefOpEqv 0x120548, else clause
 * Info: proposed name RefEvalCase (T1); TU eval
 * Confidence: high [T1 0.92] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18
 * @end-sly2-annot */
CRef RefEvalCase__FP5CPairP6CFrame(P *p, void *penv)
{
    CRef ref;
    CRef arefT[2];
    CRef test;
    NODE *pkey;
    NODE *pcl;
    ref.Decref();
    ref.InitTag(0x24);
    pkey = p->pv->pargs;
    arefT[0] = RefEval__FP5CPairP6CFrame(pkey, penv);
    pcl = pkey->pnext;
    while (pcl) {
        NODE *pc = (NODE *)pcl->ref.u.pv;
        int f;
        if (pc->ref.m_tagk == 0xB) {
            f = 1;
        } else {
            NODE *pk;
            f = 0;
            pk = (NODE *)pc->ref.u.pv;
            while (pk) {
                arefT[1] = pk->ref;
                test = RefOpEqv__FiP4CRefP6CFrame(2, arefT, penv);
                if (test.m_tagk == 0x17 && test.u.n) {
                    f = 1;
                    pk = 0;
                } else {
                    pk = pk->pnext;
                }
            }
        }
        if (f) {
            for (pc = pc->pnext; pc; pc = pc->pnext)
                ref = RefEval__FP5CPairP6CFrame(pc, penv);
            pcl = 0;
        } else {
            pcl = pcl->pnext;
        }
    }
    return ref;
}
