struct RGBA { unsigned char r, g, b, a; };
struct GIFS;
struct FONT { char pad[0xC]; int dy; char pad10[4]; float ry; };
struct CFont;
class CTextBox {
public:
    float x, y, dx, dy;
    RGBA rgba;
    int jh;
    int jv;
};
class CRichText {
public:
    unsigned char *m_achz;
    unsigned char *m_pchCur;
    CFont *m_pfontCur;
    FONT *m_pfontBase;
    char pad[0x150 - 0x10];
    int m_fDirty;
    unsigned int m_rgbaCur;
    void Draw(CTextBox *ptb, CTextBox *ptbClip, GIFS *pgifs);
};
extern "C" {
void *func_002333B4(void *, int, unsigned int);
void Reset__9CRichText(CRichText *p);
int ChNext__9CRichText(CRichText *p);
float DxFromCh__8CFontBrxc(CFont *pfont, int ch);
void SetBaseColor__9CRichTextP4RGBA(CRichText *p, RGBA *prgba);
void SetupDraw(CFont *pfont, CTextBox *ptb, GIFS *pgifs);
void SwitchTex0__8CFontBrxP4GIFS(CFont *pfont, GIFS *pgifs);
float DxDrawCh__8CFontBrxcffUiP4GIFS(CFont *pfont, int ch, float x, float y, unsigned int rgba, GIFS *pgifs);
void CleanupDraw(CFont *pfont, CTextBox *ptb, GIFS *pgifs);
}
/** @sly2-annot addr=0x15FFD0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
void CRichText::Draw(CTextBox *ptb, CTextBox *ptbClip, GIFS *pgifs)
{
    float adx[64];
    int fNewLine = 1;
    int iline = 0;
    int ch;
    float x, y;
    CFont *pfontSetup;
    float *pdx;
    RGBA rgba;
    func_002333B4(adx, 0, sizeof(adx));
    Reset__9CRichText(this);
    {
        int chT;
        while ((chT = ChNext__9CRichText(this)) != 0) {
            if (chT == '\n')
                iline++;
            else
                adx[iline] += DxFromCh__8CFontBrxc(m_pfontCur, chT);
        }
    }
    if (ptb->jv == 0) {
        y = ptb->y;
    } else {
        float dy = (float)(iline + 1) * ((float)m_pfontBase->dy * m_pfontBase->ry);
        if (ptb->jv == 2)
            y = ptb->y + ptb->dy - dy;
        else
            y = ptb->y + (ptb->dy - dy) * 0.5f;
    }
    rgba = ptb->rgba;
    SetBaseColor__9CRichTextP4RGBA(this, &rgba);
    pfontSetup = 0;
    Reset__9CRichText(this);
    pdx = adx;
    while ((ch = ChNext__9CRichText(this)) != 0) {
        if (ch == '\n') {
            fNewLine = 1;
            pdx++;
            y += (float)m_pfontBase->dy * m_pfontBase->ry;
            continue;
        }
        if (fNewLine) {
            if (ptb->jh == 0)
                x = ptb->x;
            else {
                float dxLine = *pdx;
                if (ptb->jh == 2)
                    x = ptb->x + ptb->dx - dxLine;
                else
                    x = ptb->x + (ptb->dx - dxLine) * 0.5f;
            }
            fNewLine = 0;
        }
        if (!pfontSetup) {
            SetupDraw(m_pfontCur, ptbClip, pgifs);
            pfontSetup = m_pfontCur;
        } else if (m_fDirty) {
            SwitchTex0__8CFontBrxP4GIFS(m_pfontCur, pgifs);
            m_fDirty = 0;
        }
        x += DxDrawCh__8CFontBrxcffUiP4GIFS(m_pfontCur, ch, x, y, m_rgbaCur, pgifs);
    }
    CleanupDraw(m_pfontCur, ptbClip, pgifs);
}
