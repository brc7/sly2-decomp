typedef struct CRef { int m_tagk; union { int n; float g; void *pv; } u; } CRef;
typedef struct LM { float gMin; float gMax; } LM;
void Decref__4CRef(CRef *pref);
void func_00129DA8(CRef *pref, CRef *prefSrc);
static __inline__ float GLimit(float g, float gMin, float gMax)
{
    if (g < gMin)
        return gMin;
    if (gMax < g)
        return gMax;
    return g;
}
/** @sly2-annot addr=0x1233F0
 * Purpose: Splice builtin "clq-evaluate-lm": clq eval clamped to lm
 * Info: proposed name RefOpClqEvaluateLm (T1); TU bif
 * Notes: names_v2 C proposed RefOpClqEvaluate__FiP4CRefP6CFrame (0.638)
 * Confidence: high [T1 0.93] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_001233F0(CRef *ret, void *pbasic, CRef *aref)
{
    float *c = (float *)aref[0].u.pv;
    LM *plm = (LM *)aref[1].u.pv;
    CRef ref;
    float g;
    func_00129DA8(&ref, &aref[2]);
    g = ref.u.g;
    ret->u.g = GLimit(c[0] + g * (c[1] + g * c[2]), plm->gMin, plm->gMax);
    ret->m_tagk = 0x16;
    Decref__4CRef(&ref);
    return ret;
}
