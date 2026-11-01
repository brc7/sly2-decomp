typedef struct CRef { int m_tagk; int m_tag; } CRef;
CRef *CRef_ctor_CPair(CRef *ret, int *p)
{
    ret->m_tag = (int)p;
    ret->m_tagk = 0x1F;
    (*p)++;
    return ret;
}
