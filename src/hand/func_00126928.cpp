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
CRef func_00127838(Q *pq, CRef *aref);
CRef func_00126928(P *p, CRef *aref);
}
/** @sly2-annot addr=0x126928
 * Purpose: (and ...): stops at first #f, else last value
 * Info: proposed name RefEvalAnd (T1); TU eval
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18
 * @end-sly2-annot */
CRef func_00126928(P *p, CRef *aref)
{
    CRef ref;
    Q *pq;
    ref = true;
    for (pq = p->pr->pq; pq; pq = pq->pnext) {
        ref = func_00127838(pq, aref);
        if (ref.m_tagk == 0x17 && ref.u.n == 0)
            return ref;
    }
    return ref;
}
