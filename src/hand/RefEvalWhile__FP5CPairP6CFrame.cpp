struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(float g);
};
struct Q { char pad[0x10]; Q *pnext; };
struct R { char pad[0x10]; Q *pq; };
struct P { char pad[0xC]; R *pr; };
extern "C" {
CRef RefEval__FP5CPairP6CFrame(Q *pq, CRef *aref);
CRef RefEvalWhile__FP5CPairP6CFrame(P *p, CRef *aref);
}
CRef RefEvalWhile__FP5CPairP6CFrame(P *p, CRef *aref)
{
    CRef ref;
    Q *pqCond = p->pr->pq;
    Q *pqBody = pqCond->pnext;
    for (;;) {
        ref = RefEval__FP5CPairP6CFrame(pqCond, aref);
        if (ref.m_tagk == 0x17 && ref.u.n == 0)
            break;
        ref = RefEval__FP5CPairP6CFrame(pqBody, aref);
    }
    ref.Decref();
    ref.InitTag(0x24);
    return ref;
}
