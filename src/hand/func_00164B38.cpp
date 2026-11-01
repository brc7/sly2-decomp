struct X { char pad[0xF0]; int fF0; int fF4; int fF8; };
class CSave {
public:
    X *m_p0;
    X *m_p4;
    int m_n8;
    int m_nC;
    CSave(X *px)
    {
        m_p0 = 0;
        m_p4 = px;
        if (px) {
            m_n8 = px->fF4;
            px->fF4 = 4;
            m_nC = px->fF8;
            px->fF8 = 0;
        }
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
extern "C" void ForceTaskState(X *px, int a, int b);
extern "C" void UpdateCurrentJob();
extern "C" void func_00164B38(X *px)
{
    CSave save(px);
    ForceTaskState(px, 1, 0);
    UpdateCurrentJob();
}
