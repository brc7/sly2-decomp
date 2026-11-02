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
void func_0015F970(CRichText *p);
int func_0015F9D0(CRichText *p);
float func_0015F0F0(CFont *pfont, int ch);
void func_0015F9A8(CRichText *p, RGBA *prgba);
void SetupDraw(CFont *pfont, CTextBox *ptb, GIFS *pgifs);
void func_0015F3E8(CFont *pfont, GIFS *pgifs);
float func_0015F480(CFont *pfont, int ch, float x, float y, unsigned int rgba, GIFS *pgifs);
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
    func_0015F970(this);
    {
        int chT;
        while ((chT = func_0015F9D0(this)) != 0) {
            if (chT == '\n')
                iline++;
            else
                adx[iline] += func_0015F0F0(m_pfontCur, chT);
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
    func_0015F9A8(this, &rgba);
    pfontSetup = 0;
    func_0015F970(this);
    pdx = adx;
    while ((ch = func_0015F9D0(this)) != 0) {
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
            func_0015F3E8(m_pfontCur, pgifs);
            m_fDirty = 0;
        }
        x += func_0015F480(m_pfontCur, ch, x, y, m_rgbaCur, pgifs);
    }
    CleanupDraw(m_pfontCur, ptbClip, pgifs);
}
