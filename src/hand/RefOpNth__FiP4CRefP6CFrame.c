typedef struct CRef { int m_tagk; int m_tag; } CRef;
typedef struct PAIR { int a; int b; CRef ref; struct PAIR *pnext; } PAIR;
void func_0012A0B0(CRef *pref, int tagk);
void func_00129658(CRef *pref, CRef *prefSrc);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
void func_00129EC8(CRef *pref);
CRef *RefOpNth__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    func_0012A0B0(&ref, 0);
    if (aref[0].m_tagk == 0x1F) {
        PAIR *p = (PAIR *)aref[0].m_tag;
        CRef *pn = &aref[1];
        int i;
        for (i = 0; p; p = p->pnext, i++) {
            if (i == pn->m_tag) {
                func_00129658(&ref, &p->ref);
                break;
            }
        }
    }
    func_001291A8(ret, &ref);
    func_00129EC8(&ref);
    return ret;
}
