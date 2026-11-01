typedef struct CRef { int m_tagk; int m_tag; } CRef;
void func_002013D8(CRef *pref, void *p, int n, int c, int *pc, int *ai);
int GrfFromStates(int c, int *ai);
void func_00129EC8(CRef *pref);
int func_00162050(void *p)
{
    int ai[8];
    CRef ref;
    int c;
    int grf;
    func_002013D8(&ref, p, 0x1050, 6, &c, ai);
    grf = GrfFromStates(c, ai);
    func_00129EC8(&ref);
    return grf;
}
