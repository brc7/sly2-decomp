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
struct R;
struct Q { char pad[0x8]; int x8; R *pr; struct S *ps; };
struct R { char pad[0x10]; Q *pq; };
struct S { char pad[0xC]; R *pr; };
struct P { char pad[0xC]; R *pr; };
extern char D_002A43B0[];
extern "C" {
CRef RefEval__FP5CPairP6CFrame(Q *pq, CRef *aref);
void func_0021EC18(char *psz);
CRef RefOpPrint__FiP4CRefP6CFrame(int n, int *p, CRef *aref);
CRef RefEvalAssert__FP5CPairP6CFrame(P *p, CRef *aref);
}
CRef RefEvalAssert__FP5CPairP6CFrame(P *p, CRef *aref)
{
    CRef ref;
    Q *pq = p->pr->pq;
    S *ps = pq->ps;
    ref = RefEval__FP5CPairP6CFrame(pq, aref);
    if (ref.u.n == 0) {
        Q *pq2 = ps->pr->pq;
        func_0021EC18(D_002A43B0);
        RefOpPrint__FiP4CRefP6CFrame(1, &pq2->x8, aref);
    }
    ref.Decref();
    ref.InitTag(0x24);
    return ref;
}
