typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
int FFindBinding__6CFrameUiiP4CRef(void *a, void *b, int c, CRef *d);
typedef struct BASIC { int pad[3]; void *p; } BASIC;
CRef *RefEvalSymbol__FP5CPairP6CFrame(CRef *ret, BASIC *pbasic, void *aref)
{
    CRef ref;
    ref.m_tagk = -1;
    FFindBinding__6CFrameUiiP4CRef(aref, pbasic->p, 1, &ref);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
