typedef int TI __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void InitTag__4CRef(CRef *pref, int tagk);
void CRef_ctor(CRef *pref, TI vec);
TI func_001A6F00(int a, int b, TI v);
CRef *RefOpConvertObjectPosition__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    CRef_ctor(&ref, func_001A6F00(aref[0].m_tag, aref[1].m_tag, *(TI *)aref[2].m_tag));
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
