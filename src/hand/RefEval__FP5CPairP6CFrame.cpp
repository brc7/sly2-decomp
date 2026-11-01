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
struct NODE { int x0, x4; CRef ref; NODE *px10; };
extern "C" {
CRef RefEvalSymbol__FP5CPairP6CFrame(NODE *p, CRef *aref);
CRef RefEvalSet__FP5CPairP6CFrame(NODE *p, CRef *aref);
CRef RefEvalDefine__FP5CPairP6CFrame(NODE *p, CRef *aref);
CRef RefEvalAssert__FP5CPairP6CFrame(NODE *p, CRef *aref);
CRef func_001266C8(NODE *p, CRef *aref);
CRef RefEvalIf__FP5CPairP6CFrame(NODE *p, CRef *aref);
CRef RefEvalOr__FP5CPairP6CFrame(NODE *p, CRef *aref);
CRef RefEvalAnd__FP5CPairP6CFrame(NODE *p, CRef *aref);
CRef RefEvalCond__FP5CPairP6CFrame(NODE *p, CRef *aref);
CRef RefEvalCase__FP5CPairP6CFrame(NODE *p, CRef *aref);
CRef RefEvalLet__FP5CPairP6CFrame(NODE *p, CRef *aref);
CRef RefEvalWhile__FP5CPairP6CFrame(NODE *p, CRef *aref);
CRef RefEvalBegin__FP5CPairP6CFrame(NODE *p, CRef *aref);
CRef RefEvalLambda__FP5CPairP6CFrame(NODE *p, CRef *aref);
CRef RefEvalApply__FP5CPairP6CFrame(NODE *p, CRef *aref);
CRef RefEval__FP5CPairP6CFrame(NODE *p, CRef *aref);
}
/** @sly2-annot addr=0x127838
 * Purpose: Core evaluator: self-evaluating atoms, symbols, special-form dispatch on head TAGK (1..18), else
 *   apply; import (17) returns Nil, quote (18) returns datum
 * Info: proposed name RefEval (T1); TU eval
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/overview.md#6-subsystem-map, docs/engine/splice-scripting.md#3-the-evaluator,
 *   docs/engine/splice-scripting.md#7-key-functions
 * @end-sly2-annot */
CRef RefEval__FP5CPairP6CFrame(NODE *p, CRef *aref)
{
    switch (p->ref.m_tagk) {
    default:
        return p->ref;
    case 0x18:
        return RefEvalSymbol__FP5CPairP6CFrame(p, aref);
    case 0x1F: {
        NODE *pn = (NODE *)p->ref.u.pv;
        switch (pn->ref.m_tagk) {
        case 1: return RefEvalSet__FP5CPairP6CFrame(p, aref);
        case 2: return RefEvalDefine__FP5CPairP6CFrame(p, aref);
        case 5: return RefEvalAssert__FP5CPairP6CFrame(p, aref);
        case 6: return func_001266C8(p, aref);
        case 7: return RefEvalIf__FP5CPairP6CFrame(p, aref);
        case 8: return RefEvalOr__FP5CPairP6CFrame(p, aref);
        case 9: return RefEvalAnd__FP5CPairP6CFrame(p, aref);
        case 10: return RefEvalCond__FP5CPairP6CFrame(p, aref);
        case 12: return RefEvalCase__FP5CPairP6CFrame(p, aref);
        case 13: return RefEvalLet__FP5CPairP6CFrame(p, aref);
        case 14: return RefEvalWhile__FP5CPairP6CFrame(p, aref);
        case 16: return RefEvalBegin__FP5CPairP6CFrame(p, aref);
        case 15: return RefEvalLambda__FP5CPairP6CFrame(p, aref);
        case 18: return pn->px10->ref;
        case 17: return CRef(-1);
        }
        return RefEvalApply__FP5CPairP6CFrame(p, aref);
    }
    }
}
