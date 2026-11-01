typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
void InitTag__4CRef(CRef *pref, int tagk);
CRef *RefOpPrintSidebag__FiP4CRefP6CFrame(CRef *ret)
{
    CRef ref;
    ref.m_tagk = -1;
    InitTag__4CRef(ret, 0x24);
    Decref__4CRef(&ref);
    return ret;
}
