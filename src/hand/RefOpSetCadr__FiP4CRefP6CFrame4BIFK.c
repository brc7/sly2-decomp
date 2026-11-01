typedef struct CRef { int m_tagk; void *m_p; } CRef;
void InitTag__4CRef(CRef *pref, int tagk);
void func_00129658(CRef *pref, CRef *prefSrc);
void func_00256C58(void *p, void *q);
CRef *RefOpSetCadr__FiP4CRefP6CFrame4BIFK(CRef *ret, void *pbasic, CRef *aref, int cref, int k)
{
    char *p = aref[0].m_p;
    if (k == 0x23)
        func_00129658((CRef *)(p + 8), &aref[1]);
    else if (aref[1].m_tagk == 0x1F)
        func_00256C58(p + 0x10, aref[1].m_p);
    else
        func_00256C58(p + 0x10, 0);
    InitTag__4CRef(ret, 0x24);
    return ret;
}
