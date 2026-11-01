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
    CRef &operator=(bool f);
};
struct Q { char pad[0x10]; Q *pnext; };
struct R { char pad[0x10]; Q *pq; };
struct P { char pad[0xC]; R *pr; };
extern "C" {
CRef RefEval__FP5CPairP6CFrame(Q *pq, CRef *aref);
CRef RefEvalOr__FP5CPairP6CFrame(P *p, CRef *aref);
}
CRef RefEvalOr__FP5CPairP6CFrame(P *p, CRef *aref)
{
    CRef ref;
    Q *pq;
    for (pq = p->pr->pq; pq; pq = pq->pnext) {
        ref = RefEval__FP5CPairP6CFrame(pq, aref);
        if (ref.m_tagk != 0x17 || ref.u.n != 0)
            return ref;
    }
    ref = false;
    return ref;
}
