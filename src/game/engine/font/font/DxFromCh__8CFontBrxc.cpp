struct GLYPH { short a; short b; short c; unsigned short dx; };
class CFontBrx {
public:
    int pad[2];
    int m_dxSpace;
    int pad2;
    float m_rxScale;
    float DxFromCh(char ch);
    GLYPH *PglyphFromCh(unsigned char ch);
};
/** @sly2-annot addr=0x15F0F0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
float CFontBrx::DxFromCh(char ch)
{
    GLYPH *pglyph = PglyphFromCh(ch);
    if (pglyph)
        return (float)(pglyph->dx + 1) * m_rxScale;
    return (float)m_dxSpace * m_rxScale;
}
