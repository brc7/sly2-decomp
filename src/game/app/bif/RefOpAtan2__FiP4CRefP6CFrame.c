typedef struct CRef { int m_tagk; union { int n; float g; } u; } CRef;
float atan2f(float y, float x);
/** @sly2-annot addr=0x1226B0
 * Purpose: Splice builtin "atan2": atan2f(a0,a1)
 * Info: proposed name RefOpAtan2 (T1); TU bif
 * Notes: names_v2 B proposed RefOpVectorDotProduct__FiP4CRefP6CFrame (0.994)
 * Confidence: high [T1 0.93] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpAtan2__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    float ag[2];
    int i;
    float g;
    for (i = 0; i < 2; i++) {
        CRef *p = &aref[i];
        if (p->m_tagk == 0x16) ag[i] = p->u.g; else ag[i] = p->u.n;
    }
    g = atan2f(ag[0], ag[1]);
    ret->u.g = g;
    ret->m_tagk = 0x16;
    return ret;
}
