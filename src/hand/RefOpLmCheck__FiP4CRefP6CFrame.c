typedef struct CRef { int m_tagk; union { int n; float g; } u; } CRef;
void Decref__4CRef(CRef *pref);
void CRef_RefCoerceF32(CRef *pref, CRef *prefSrc);
int func_0011D910(int n, float g);
CRef *RefOpLmCheck__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    int n = aref[0].u.n;
    CRef ref;
    int f;
    CRef_RefCoerceF32(&ref, &aref[1]);
    f = func_0011D910(n, ref.u.g) != 0;
    ret->m_tagk = 0x17;
    ret->u.n = f;
    Decref__4CRef(&ref);
    return ret;
}
