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
    char pad2[0xF0 - 0x80]; int fF0; int fF4; int fF8;
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
extern "C" int func_001645D0(X *p);
extern "C" void func_00164DF0(X *p, int n);
extern "C" void func_00163C28(X *p);
extern "C" void func_00163CE0(X *p);
/** @sly2-annot addr=0x164858
 * Purpose: Unvalidated counterpart of RequestTaskState
 * Info: proposed name ForceTaskState (T1); TU game
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void func_00164858(X *p, int n, int nF8)
{
    CSave save(p, p->fF4, nF8);
    CSave saveOwner(p->f7c, p->fF4, nF8, 1);
    if (n == -1)
        n = 4;
    if (p->n54 != -1) {
        switch (n) {
        case 4:
            if (p->n54 == 0 && func_001645D0(p))
                func_00164DF0(p, 1);
            func_00163C28(p);
            break;
        case 5:
            func_00163CE0(p);
            break;
        case 0:
        case 1:
        case 2:
        case 3:
            func_00164DF0(p, n);
            break;
        }
    }
}
