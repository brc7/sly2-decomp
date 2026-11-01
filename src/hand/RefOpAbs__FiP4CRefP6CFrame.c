typedef struct CRef { int m_tagk; int m_tag; } CRef;
void SetS32__4CRefi(CRef *pref, int n);
void SetF32__4CReff(CRef *pref, float g);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
void func_00129EC8(CRef *pref);
CRef *RefOpAbs__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    ref.m_tagk = -1;
    if (aref->m_tagk == 0x15) {
        int n = aref->m_tag;
        SetS32__4CRefi(&ref, n < 0 ? -n : n);
    } else {
        float g = *(float *)&aref->m_tag;
        if (g < 0.0f) g = -g;
        SetF32__4CReff(&ref, g);
    }
    func_001291A8(ret, &ref);
    func_00129EC8(&ref);
    return ret;
}
