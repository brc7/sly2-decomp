typedef struct CRef { int m_tagk; union { int n; float g; void *pv; } u; } CRef;
typedef struct LM { float gMin; float gMax; } LM;
void Decref__4CRef(CRef *pref);
void CRef_RefCoerceF32(CRef *pref, CRef *prefSrc);
static __inline__ float GLimit(float g, float gMin, float gMax)
{
    if (g < gMin)
        return gMin;
    if (gMax < g)
        return gMax;
    return g;
}
CRef *RefOpClqEvaluateLm__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    float *c = (float *)aref[0].u.pv;
    LM *plm = (LM *)aref[1].u.pv;
    CRef ref;
    float g;
    CRef_RefCoerceF32(&ref, &aref[2]);
    g = ref.u.g;
    ret->u.g = GLimit(c[0] + g * (c[1] + g * c[2]), plm->gMin, plm->gMax);
    ret->m_tagk = 0x16;
    Decref__4CRef(&ref);
    return ret;
}
