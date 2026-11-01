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
CRef RefEvalIf__FP5CPairP6CFrame(P *p, CRef *aref);
}
/** @sly2-annot addr=0x126758
 * Purpose: (if c then [else])
 * Info: proposed name RefEvalIf (T1); TU eval
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/splice-scripting.md#3-the-evaluator,
 *   docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18
 * @end-sly2-annot */
CRef RefEvalIf__FP5CPairP6CFrame(P *p, CRef *aref)
{
    CRef ref;
    Q *pqCond = p->pr->pq;
    Q *pqThen = pqCond->pnext;
    Q *pqElse = pqThen->pnext;
    ref = RefEval__FP5CPairP6CFrame(pqCond, aref);
    if (ref.u.n == 1)
        ref = RefEval__FP5CPairP6CFrame(pqThen, aref);
    else if (pqElse)
        ref = RefEval__FP5CPairP6CFrame(pqElse, aref);
    else {
        ref.Decref();
        ref.InitTag(0x24);
    }
    return ref;
}
