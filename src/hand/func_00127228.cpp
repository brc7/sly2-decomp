struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
typedef struct Q { char pad[0x10]; struct Q *pnext; } Q;
typedef struct R { char pad[0x10]; Q *pq; } R;
typedef struct P { char pad[0xC]; R *pr; } P;
extern "C" {
CRef func_00127838(Q *pq, CRef *aref);
CRef func_00127228(P *p, CRef *aref);
}
/** @sly2-annot addr=0x127228
 * Purpose: (begin ...): evaluate all, return last (Void if empty)
 * Info: proposed name RefEvalBegin (T1); TU eval
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18
 * @end-sly2-annot */
CRef func_00127228(P *p, CRef *aref)
{
    CRef ref;
    Q *pq;
    ref.Decref();
    ref.InitTag(0x24);
    for (pq = p->pr->pq; pq; pq = pq->pnext)
        ref = func_00127838(pq, aref);
    return ref;
}
