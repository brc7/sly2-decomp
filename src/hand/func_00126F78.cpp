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
CRef func_00127838(Q *pq, CRef *aref);
CRef func_00126F78(P *p, CRef *aref);
}
/** @sly2-annot addr=0x126F78
 * Purpose: (while cond body)
 * Info: proposed name RefEvalWhile (T1); TU eval
 * Confidence: high [T1 0.92] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18
 * @end-sly2-annot */
CRef func_00126F78(P *p, CRef *aref)
{
    CRef ref;
    Q *pqCond = p->pr->pq;
    Q *pqBody = pqCond->pnext;
    for (;;) {
        ref = func_00127838(pqCond, aref);
        if (ref.m_tagk == 0x17 && ref.u.n == 0)
            break;
        ref = func_00127838(pqBody, aref);
    }
    ref.Decref();
    ref.InitTag(0x24);
    return ref;
}
