typedef struct CRef { int m_tagk; union { int n; float g; void *p; } u; } CRef;
void Decref__4CRef(CRef *pref);
void func_00129DA8(CRef *pref, CRef *prefSrc);
/** @sly2-annot addr=0x123380
 * Purpose: Splice builtin "clq-evaluate": a0+t(a1+t a2)
 * Info: proposed name RefOpClqEvaluate (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_00123380(CRef *ret, void *pbasic, CRef *aref)
{
    float *a = (float *)aref[0].u.p;
    CRef ref;
    float t, g;
    func_00129DA8(&ref, &aref[1]);
    t = ref.u.g;
    g = a[0] + t * (a[1] + t * a[2]);
    ret->m_tagk = 0x16;
    ret->u.g = g;
    Decref__4CRef(&ref);
    return ret;
}
