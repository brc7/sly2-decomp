struct X;
class CSave {
public:
    X *m_p0;
    X *m_p4;
    int m_n8;
    int m_nC;
    inline CSave(X *px, int nF4, int nF8);
    inline CSave(X *px, int nF0, int nF4, int fOwner);
    inline ~CSave();
};
struct X {
    char pad[0x54]; int n54;
    char pad1[0x7C - 0x58]; X *f7c;
    char pad2[0xB8 - 0x80]; int fB8; int fBC; int fC0;
    char pad3[0xF0 - 0xC4]; int fF0; int fF4; int fF8;
};
inline CSave::CSave(X *px, int nF4, int nF8)
{
    m_p0 = 0;
    m_p4 = px;
    if (px) {
        m_n8 = px->fF4;
        px->fF4 = nF4;
        m_nC = m_p4->fF8;
        m_p4->fF8 = nF8;
    }
}
inline CSave::CSave(X *px, int nF0, int nF4, int fOwner)
{
    m_p0 = px;
    m_p4 = 0;
    if (px) {
        m_n8 = px->fF0;
        px->fF0 = nF0;
        m_nC = m_p0->fF4;
        m_p0->fF4 = nF4;
    }
}
inline CSave::~CSave()
{
    if (m_p0) {
        m_p0->fF0 = m_n8;
        m_p0->fF4 = m_nC;
    }
    if (m_p4) {
        m_p4->fF4 = m_n8;
        m_p4->fF8 = m_nC;
    }
}
struct CRef { int m_tagk; int m_tag; };
struct G { char pad[0x3B0C]; X *f3b0c; char pad2[0x3E38 - 0x3B10]; int f3e38; };
extern G D_003DD490;
extern char D_002A5238[];
extern "C" void AbortJob(X *p);
extern "C" void SetCurrentJob(X *p);
extern "C" void SetTaskStateCascade(X *p, int n);
extern "C" void ActivateTask(X *p);
extern "C" void func_001D8B20(CRef *pref, CRef *parg, void *pv, int n);
extern "C" void func_00129EC8(CRef *pref);
extern "C" int IlevelCur__Fv();
extern "C" void *func_00210990(int n);
extern "C" void func_0022B490(G *pg, void *p, int a, int b, int c, int d);
extern "C" void GoToTask(X *p)
{
    CSave save(p, 2, 0);
    CSave saveOwner(p->f7c, 2, 0, 1);
    if (p->f7c != D_003DD490.f3b0c) {
        if (D_003DD490.f3b0c) {
            CSave saveCur(D_003DD490.f3b0c, 2, 0, 1);
            AbortJob(D_003DD490.f3b0c);
        }
        if (p->f7c)
            SetCurrentJob(p->f7c);
    }
    SetTaskStateCascade(p, 1);
    ActivateTask(p);
    if (p->fC0 != -1) {
        CRef refRet;
        CRef ref;
        ref.m_tagk = 0x15;
        ref.m_tag = p->fC0;
        func_001D8B20(&refRet, &ref, D_002A5238, 0);
        func_00129EC8(&refRet);
        func_00129EC8(&ref);
    }
    int id = p->fB8 == -1 ? 0x189 : p->fB8;
    int k = 4;
    if (D_003DD490.f3e38 && (p->fBC == -1 || p->fBC == IlevelCur__Fv()))
        k = 0x24;
    func_0022B490(&D_003DD490, func_00210990(p->fBC), id, -1, k, 0x30A);
}
