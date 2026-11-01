typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void InitTag__4CRef(CRef *pref, int tagk);
void func_00129658(CRef *pref, CRef *prefSrc);
CRef *func_00127DB8(void *a, void *b, int c);
CRef *RefSetBinding__6CFrameUiP4CRef(CRef *ret, void *a, void *b, CRef *pref)
{
    CRef ref;
    CRef *p;
    ref.m_tagk = -1;
    p = func_00127DB8(a, b, 1);
    if (p)
        func_00129658(p, pref);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
