typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void InitTag__4CRef(CRef *pref, int tagk);
void *func_001FBB90(int n);
void func_001FB2F8(void *p, int n);
void CRef_ctor_BASIC(CRef *pref, void *p);
CRef *RefOpStartSound__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    int n = aref[1].m_tag;
    void *p = func_001FBB90(aref[0].m_tag);
    func_001FB2F8(p, n);
    CRef_ctor_BASIC(ret, p);
    return ret;
}
