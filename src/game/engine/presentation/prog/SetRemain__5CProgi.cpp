extern "C" void ClearFrameBuffers__Fv(void);
class CProg {
public:
    int m_fActive; int m_nMax; int m_nTarget; int m_nRemain; char m_f10; char pad[0x3f]; int m_cRetry;
    void SetRemain(int nRemain);
    void Draw();
};
/** @sly2-annot addr=0x1C8D40
 * Purpose: update remaining count
 * Info: proposed name SetRemain__5CProgi (EXISTING); class CProg; TU prog
 * Notes: agent/verifier proposes CProg_SetRemain for this existing name (see existing_name_changes.csv)
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r5_1AE130
 * See: docs/reference/functions/hud-ui.md#prog-0x1c8cc0-0x1c9940
 * @end-sly2-annot */
void CProg::SetRemain(int nRemain)
{
    if (m_fActive != 0) {
        if (m_nTarget == 0) {
            m_nTarget = nRemain;
            m_nMax = nRemain << 1;
            ClearFrameBuffers__Fv();
        } else if (m_nRemain == nRemain) {
            m_cRetry = m_cRetry + 1;
        } else {
            m_cRetry = 0;
        }
        m_nRemain = nRemain;
        Draw();
    }
}
