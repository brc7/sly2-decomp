struct RGBA { unsigned char r, g, b, a; };
class CRichText {
public:
    char pad[0x154];
    RGBA m_rgbaCur;
    int pad158;
    RGBA m_rgbaBase;
    void SetBaseColor(RGBA *rgba);
};
/** @sly2-annot addr=0x15F9A8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
void CRichText::SetBaseColor(RGBA *rgba)
{
    m_rgbaBase = *rgba;
    m_rgbaCur = m_rgbaBase;
}
