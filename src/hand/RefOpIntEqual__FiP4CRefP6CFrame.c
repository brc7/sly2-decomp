typedef struct CRef { int m_tagk; int m_tag; } CRef;
CRef *RefOpIntEqual__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    int a = aref[0].m_tag;
    int b = aref[1].m_tag;
    ret->m_tagk = 0x17;
    ret->m_tag = a == b;
    return ret;
}
