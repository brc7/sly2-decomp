typedef int TI __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; void *m_p; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void InitTag__4CRef(CRef *pref, int tagk);
TI func_001A6FC0(void *a, void *b, TI v);
CRef *RefOpConvertObjectVector__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    TI *pv;
    InitTag__4CRef(&ref, 0x1A);
    pv = ref.m_p;
    *pv = func_001A6FC0(aref[0].m_p, aref[1].m_p, *(TI *)aref[2].m_p);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
