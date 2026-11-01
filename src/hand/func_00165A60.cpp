struct X { char pad[0xF0]; int fF0; int fF4; int fF8; };
class CSave {
public:
    X *m_p0;
    X *m_p4;
    int m_n8;
    int m_nC;
    CSave(X *px, int nF4, int nF8)
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
struct S { char pad[0x54]; int n; char pad2[0xA0 - 0x58]; int c; X **apx; char pad3[0xF4 - 0xA8]; int fF4; int fF8; };
extern "C" void func_001657D0(S *p, int n);
extern "C" void func_00165B40(X *px, int n);
/** @sly2-annot addr=0x165A60
 * Purpose: SetTaskState then recompute each successor
 * Info: proposed name SetTaskStatePropagate (T1); TU game
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void func_00165A60(S *p, int n)
{
    int i;
    if (p->n == n) return;
    func_001657D0(p, n);
    for (i = 0; i < p->c; i++) {
        X *px = p->apx[i];
        CSave save(px, p->fF4, p->fF8);
        func_00165B40(px, p->n);
    }
}
