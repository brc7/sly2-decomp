class CFontBrx {
public:
    float DxFromCh(char ch);
};
class CRichText {
public:
    char *m_achz;
    char *m_pchCur;
    CFontBrx *m_pfontCur;
    void Reset();
    char ChNext();
    float DxMaxLine();
};
static inline float GMax(float a, float b) { return a > b ? a : b; }
/** @sly2-annot addr=0x15FF40
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name DxMaxLine__9CRichText (EXISTING); TU font
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: pre-existing symbol in symbol_addrs.txt
 * See: docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
float CRichText::DxMaxLine()
{
    float dxMax = 0.0f;
    float dx = dxMax;
    char ch;
    Reset();
    while ((ch = ChNext()) != 0) {
        if (ch == '\n') {
            dxMax = GMax(dxMax, dx);
            dx = 0.0f;
        } else {
            dx += m_pfontCur->DxFromCh(ch);
        }
    }
    dxMax = GMax(dxMax, dx);
    return dxMax;
}
