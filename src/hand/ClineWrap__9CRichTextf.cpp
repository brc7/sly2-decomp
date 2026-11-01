// (Claude agent) newline kept in a local (two copies of 10 in the target); the register pin only swaps s2/s3 with the hoisted compare constant.
struct CFont;
extern "C" {
extern const unsigned char D_002AA6A1[];
float func_0015F0F0(CFont *pfont, int ch);
}
class CRichText {
public:
    unsigned char *m_achz;
    unsigned char *m_pchCur;
    CFont *m_pfontCur;
    int ClineWrap(float dxMax);
};
extern "C" void func_0015F970(CRichText *p);
extern "C" int func_0015F9D0(CRichText *p);
/** @sly2-annot addr=0x15FD68
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
int CRichText::ClineWrap(float dxMax)
{
    int cline = 1;
    float dx = 0.0f;
    int ch;
    register int chNl __asm__("$18") = 10;
    func_0015F970(this);
    while ((ch = func_0015F9D0(this)) != 0) {
        if (ch == '\n') {
            cline++;
            dx = 0.0f;
        } else {
            dx += func_0015F0F0(m_pfontCur, ch);
        }
        if (dxMax != 0.0f && dx > dxMax) {
            unsigned char *pch;
            for (pch = m_pchCur - 1; pch >= m_achz && *pch != '\n'; pch--) {
                if ((*pch == ' ' || *pch == '\t') && !(D_002AA6A1[pch[1]] & 0x10)) {
                    cline++;
                    m_pchCur = pch + 1;
                    dx = 0.0f;
                    *pch = chNl;
                    break;
                }
            }
            if (dx != 0.0f && *m_pchCur) {
                *m_pchCur = chNl;
                cline++;
                dx = 0.0f;
            }
        }
    }
    return cline;
}
