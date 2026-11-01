typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void InitTag__4CRef(CRef *pref, int tagk);
void func_001DAF38(int a, int b);
CRef *RefOpSetPowerUpAcquired(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    InitTag__4CRef(&ref, 0x24);
    func_001DAF38(aref[0].m_tag, aref[1].m_tag);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
