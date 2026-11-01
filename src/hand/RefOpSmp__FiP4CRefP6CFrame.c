typedef struct CRef { int m_tagk; union { int n; float g; void *pv; } u; } CRef;
void Decref__4CRef(CRef *pref);
void InitTag__4CRef(CRef *pref, int tagk);
void CRef_RefCoerceF32(CRef *pref, CRef *prefSrc);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
CRef *RefOpSmp__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    int i;
    float *pg;
    InitTag__4CRef(&ref, 0x1E);
    pg = (float *)ref.u.pv;
    for (i = 0; i < 3; i++) {
        CRef tmp;
        CRef_RefCoerceF32(&tmp, &aref[i]);
        pg[i] = tmp.u.g;
        Decref__4CRef(&tmp);
    }
    func_001291A8(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
