typedef struct CRef { int m_tagk; union { int n; float g; void *pv; } u; } CRef;
void Decref__4CRef(CRef *pref);
void InitTag__4CRef(CRef *pref, int tagk);
void CRef_RefCoerceF32(CRef *pref, CRef *prefSrc);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
extern float D_002DDED0;
CRef *RefOpDeferObjectUpdate__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    CRef refT;
    char *p;
    float g;
    InitTag__4CRef(&ref, 0x24);
    p = (char *)aref[0].u.pv;
    CRef_RefCoerceF32(&refT, &aref[1]);
    g = refT.u.g;
    Decref__4CRef(&refT);
    *(float *)(p + 0x30) = D_002DDED0 + g;
    func_001291A8(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
