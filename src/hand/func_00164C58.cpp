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
extern "C" void func_00164858(X *px, int a, int b);
extern "C" void func_001649D0();
/** @sly2-annot addr=0x164C58
 * Purpose: ForceTaskState(p,3) then UpdateCurrentJob
 * Info: proposed name ForceTaskState3 (T2); TU game
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void func_00164C58(X *px)
{
    CSave save(px);
    func_00164858(px, 3, 0);
    func_001649D0();
}
