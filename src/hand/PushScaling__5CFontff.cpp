struct SCL { float rx; float ry; };
class CFont {
public:
    int pad[4];
    float m_rxScale;
    float m_ryScale;
    int m_cscl;
    SCL m_ascl[1];
    void PushScaling(float rx, float ry);
};
/** @sly2-annot addr=0x15F078
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
void CFont::PushScaling(float rx, float ry)
{
    SCL *pscl = &m_ascl[m_cscl++];
    pscl->rx = m_rxScale;
    pscl->ry = m_rxScale;
    m_rxScale = m_ascl[0].rx * rx;
    m_ryScale = m_ascl[0].ry * ry;
}
