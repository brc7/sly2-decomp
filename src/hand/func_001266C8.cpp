struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct Q { char pad[0x10]; void *pv; };
struct P { char pad[0xC]; Q *pq; };
extern "C" {
CRef RefEval__FP5CPairP6CFrame(void *pv, CRef *aref);
CRef func_001266C8(P *p, CRef *aref);
}
/** @sly2-annot addr=0x1266C8
 * Purpose: New Sly2 special form 6: evaluates its first operand if present, else Void
 * Info: proposed name RefEvalForm6 (T2); TU eval
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/splice-scripting.md#open-questions,
 *   docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18
 * @end-sly2-annot */
CRef func_001266C8(P *p, CRef *aref) { CRef ref; void *pv = p->pq->pv; if (pv) { ref = RefEval__FP5CPairP6CFrame(pv, aref); } else { ref.Decref(); ref.InitTag(0x24); } return ref; }
