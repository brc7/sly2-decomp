typedef struct CRef { int m_tagk; float m_g; } CRef;
CRef *RefOpFloor__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    float g = aref->m_g;
    ret->m_tagk = 0x15;
    *(int *)&ret->m_g = (int)g;
    return ret;
}
