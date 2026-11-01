struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
typedef struct Q { char pad[0xC]; void *pv0C; void *pv10; } Q;
typedef struct R { char pad[0x10]; Q *pq; } R;
typedef struct P { char pad[0xC]; R *pr; } P;
extern "C" {
CRef func_00127838(void *pv, CRef *aref);
CRef func_00127CF0(CRef *aref, void *pv, CRef *pref);
CRef func_00126490(P *p, CRef *aref);
}
/** @sly2-annot addr=0x126490
 * Purpose: (set! sym expr): evaluate expr, CFrame::RefSetBinding (0x127CF0)
 * Info: proposed name RefEvalSet (T1); TU eval
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18
 * @end-sly2-annot */
CRef func_00126490(P *p, CRef *aref) { CRef ref; Q *pq = p->pr->pq; ref = func_00127838(pq->pv10, aref); ref = func_00127CF0(aref, pq->pv0C, &ref); return ref; }
