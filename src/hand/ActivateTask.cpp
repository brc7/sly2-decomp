struct X { char pad[0xF0]; int fF0; int fF4; int fF8; };
class CSave {
public:
    X *m_p0;
    X *m_p4;
    int m_n8;
    int m_nC;
    CSave(X *px, int nF4, int nF8)
    {
        m_p4 = px;
        m_p0 = 0;
        m_n8 = px->fF4;
        px->fF4 = nF4;
        m_nC = m_p4->fF8;
        m_p4->fF8 = nF8;
    }
    ~CSave()
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
};
struct G { char pad[0x3B10]; X *f3b10; };
extern G D_003DD490;
extern "C" int FIsCurrentTask(X *px);
extern "C" void DeactivateTask(X *px);
extern "C" void BeginTask(X *px);
extern "C" void ActivateTask(X *px)
{
    if (FIsCurrentTask(px)) return;
    if (D_003DD490.f3b10) {
        CSave save(D_003DD490.f3b10, px->fF4, px->fF8);
        DeactivateTask(D_003DD490.f3b10);
    }
    BeginTask(px);
}
