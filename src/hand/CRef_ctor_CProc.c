typedef struct CRef { int m_tagk; int m_tag; } CRef;
CRef *CRef_ctor_CProc(CRef *ret, int *p)
{
    ret->m_tag = (int)p;
    ret->m_tagk = 0x20;
    (*p)++;
    return ret;
}
