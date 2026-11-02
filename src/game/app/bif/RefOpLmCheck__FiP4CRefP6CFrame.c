typedef struct CRef { int m_tagk; union { int n; float g; } u; } CRef;
void Decref__4CRef(CRef *pref);
void CRef_RefCoerceF32(CRef *pref, CRef *prefSrc);
int FCheckLm__FP2LMf(int n, float g);
/** @sly2-annot addr=0x123608
 * Purpose: Splice builtin "lm-check?": FCheckLm
 * Info: proposed name RefOpLmCheck (T1); TU bif
 * Notes: names_v2 B proposed RefOpFloor__FiP4CRefP6CFrame (0.969)
 * Confidence: high [T1 0.93] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpLmCheck__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    int n = aref[0].u.n;
    CRef ref;
    int f;
    CRef_RefCoerceF32(&ref, &aref[1]);
    f = FCheckLm__FP2LMf(n, ref.u.g) != 0;
    ret->m_tagk = 0x17;
    ret->u.n = f;
    Decref__4CRef(&ref);
    return ret;
}
