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
CRef func_00127C00(CRef *aref, void *pv, CRef *pref);
CRef func_00126540(P *p, CRef *aref);
}
/** @sly2-annot addr=0x126540
 * Purpose: (define sym expr): evaluate, CFrame::RefAddBinding (0x127C00)
 * Info: proposed name RefEvalDefine (T1); TU eval
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18
 * @end-sly2-annot */
CRef func_00126540(P *p, CRef *aref) { CRef ref; Q *pq = p->pr->pq; ref = func_00127838(pq->pv10, aref); ref = func_00127C00(aref, pq->pv0C, &ref); return ref; }
