typedef struct CRef { int m_tagk; union { int n; float g; void *p; } u; } CRef;
void Decref__4CRef(CRef *pref);
void CRef_RefCoerceF32(CRef *pref, CRef *prefSrc);
CRef *RefOpClqEvaluate__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    float *a = (float *)aref[0].u.p;
    CRef ref;
    float t, g;
    CRef_RefCoerceF32(&ref, &aref[1]);
    t = ref.u.g;
    g = a[0] + t * (a[1] + t * a[2]);
    ret->m_tagk = 0x16;
    ret->u.g = g;
    Decref__4CRef(&ref);
    return ret;
}
