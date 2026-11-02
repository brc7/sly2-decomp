typedef struct CRef { int m_tagk; union { int n; float g; } u; } CRef;
void Decref__4CRef(CRef *pref);
void CRef_RefCoerceF32(CRef *pref, CRef *prefSrc);
float GLimitLm__FP2LMf(int n, float g);
/** @sly2-annot addr=0x1235A8
 * Purpose: Splice builtin "lm-limit": GLimitLm
 * Info: proposed name RefOpLmLimit (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpLmLimit__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    int n = aref[0].u.n;
    CRef ref;
    float g;
    CRef_RefCoerceF32(&ref, &aref[1]);
    g = GLimitLm__FP2LMf(n, ref.u.g);
    ret->u.g = g;
    ret->m_tagk = 0x16;
    Decref__4CRef(&ref);
    return ret;
}
