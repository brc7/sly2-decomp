struct RGBA { unsigned char r, g, b, a; };
struct CFont;
class CRichText {
public:
    char *m_achz;
    char *m_pchCur;
    CFont *m_pfontCur;
    CFont *m_pfontBase;
    char pad[0x150 - 0x10];
    int m_unknown;
    RGBA m_rgbaCur;
    RGBA m_rgbaSet;
    RGBA m_rgbaBase;
    void Reset();
};
/** @sly2-annot addr=0x15F970
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
void CRichText::Reset()
{
    m_pchCur = m_achz;
    m_pfontCur = m_pfontBase;
    m_rgbaSet = m_rgbaBase;
    m_rgbaCur = m_rgbaSet;
    m_unknown = 0;
}
