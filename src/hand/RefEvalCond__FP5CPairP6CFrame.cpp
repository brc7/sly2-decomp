// RefEvalCond: walks a clause list; first clause whose test is not false (tagk 0x17 with value 0) wins
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
CRef RefEval__FP5CPairP6CFrame(NODE *p, void *penv);
CRef RefEvalCond__FP5CPairP6CFrame(P *p, void *penv);
}
CRef RefEvalCond__FP5CPairP6CFrame(P *p, void *penv)
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
            test = RefEval__FP5CPairP6CFrame(pc, penv);
            f = 0;
            if (test.m_tagk != 0x17 || test.u.n)
                f = 1;
            if (f && !pc->pnext)
                ref = test;
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
