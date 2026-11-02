typedef struct CRef { int m_tagk; union { int n; float g; void *pv; } u; } CRef;
typedef struct LO { char pad[0x10]; struct LO *pnext; } LO;
typedef struct BASIC { char pad[0x18]; LO *plo; } BASIC;
void Decref__4CRef(CRef *pref);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
CRef *func_00129658(CRef *pref, CRef *prefSrc);
CRef *RefEval__FP5CPairP6CFrame(CRef *ret, LO *plo, CRef *aref);
/** @sly2-annot addr=0x127190
 * Purpose: Evaluates each body expression of a proc in order, returns the last
 * Info: proposed name RefEvalLambdaBody (T1); TU eval
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18
 * @end-sly2-annot */
CRef *RefEvalLambdaBody__FP5CPairP6CFrame(CRef *ret, BASIC *pbasic, CRef *aref)
{
    CRef ref;
    CRef refT;
    LO *plo;
    ref.m_tagk = -1;
    plo = pbasic->plo;
    do {
        RefEval__FP5CPairP6CFrame(&refT, plo, aref);
        func_00129658(&ref, &refT);
        Decref__4CRef(&refT);
        plo = plo->pnext;
    } while (plo);
    func_001291A8(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
