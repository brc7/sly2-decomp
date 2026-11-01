typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void CRef_SetBasic(CRef *pref, void *p);
extern char D_003DD490[];
CRef *RefOpFindWorldObject__FiP4CRefP6CFrame(CRef *ret)
{
    CRef ref;
    ref.m_tagk = -1;
    CRef_SetBasic(&ref, D_003DD490);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
