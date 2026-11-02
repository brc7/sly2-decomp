typedef struct CRef { int m_tagk; union { int n; float g; void *pv; } u; } CRef;
void Decref__4CRef(CRef *pref);
void SetTag__4CRef4TAGK(CRef *pref, int tagk);
void CRef_RefCoerceF32(CRef *pref, CRef *prefSrc);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
/** @sly2-annot addr=0x122168
 * Purpose: Splice builtin "lm": 2 floats -> tag 0x1D
 * Info: proposed name RefOpLm (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_B+verify_side
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpLm__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    int i;
    float *pg;
    SetTag__4CRef4TAGK(&ref, 0x1D);
    pg = (float *)ref.u.pv;
    for (i = 0; i < 2; i++) {
        CRef tmp;
        CRef_RefCoerceF32(&tmp, &aref[i]);
        pg[i] = tmp.u.g;
        Decref__4CRef(&tmp);
    }
    func_001291A8(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
