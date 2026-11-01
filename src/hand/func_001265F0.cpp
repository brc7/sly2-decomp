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
CRef func_00127838(Q *pq, CRef *aref);
void func_0021EC18(char *psz);
CRef func_001202D0(int n, int *p, CRef *aref);
CRef func_001265F0(P *p, CRef *aref);
}
/** @sly2-annot addr=0x1265F0
 * Purpose: (assert expr ...): evaluates; if false prints message (printf) ; returns Void
 * Info: proposed name RefEvalAssert (T1); TU eval
 * Confidence: high [T1 0.88] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18
 * @end-sly2-annot */
CRef func_001265F0(P *p, CRef *aref)
{
    CRef ref;
    Q *pq = p->pr->pq;
    S *ps = pq->ps;
    ref = func_00127838(pq, aref);
    if (ref.u.n == 0) {
        Q *pq2 = ps->pr->pq;
        func_0021EC18(D_002A43B0);
        func_001202D0(1, &pq2->x8, aref);
    }
    ref.Decref();
    ref.InitTag(0x24);
    return ref;
}
