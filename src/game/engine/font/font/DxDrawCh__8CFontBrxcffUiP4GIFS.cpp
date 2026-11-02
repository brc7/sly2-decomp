// (Claude agent, lane E7) MATCH from the 3-diff wip: ONE register pin, the 0.0f compare constant in $f12 (x's incoming reg;
// without the pin global-alloc gives it $f4).
struct GIFS;
struct GLYPH { unsigned short ch; unsigned short u; unsigned short v; unsigned short dx; };
struct TEX0 {
    unsigned long TBP0 : 14;
    unsigned long TBW : 6;
    unsigned long PSM : 6;
    unsigned long TW : 4;
    unsigned long TH : 4;
    unsigned long TCC : 1;
    unsigned long TFX : 2;
    unsigned long CBP : 14;
    unsigned long CPSM : 4;
    unsigned long CSM : 1;
    unsigned long CSA : 5;
    unsigned long CLD : 3;
};
class CFontBrx {
public:
    int pad0[2];
    int m_dxSpace;
    int m_dyUnscaled;
    float m_rxScale;
    float m_ryScale;
    char pad18[0x48 - 0x18];
    int m_z;
    int pad4C;
    TEX0 m_tex0;
    float DxDrawCh(char ch, float x, float y, unsigned int rgba, GIFS *pgifs);
};
extern "C" {
GLYPH *PglyffFromCh(CFontBrx *pfont, unsigned char ch);
void ListRGBAQ__4GIFSUif(GIFS *pgifs, unsigned int rgba);
void ListUV__4GIFSii(GIFS *pgifs, int u, int v);
void ListXYZF__4GIFSiiii(GIFS *pgifs, int x, int y, int z, int f);
}
/** @sly2-annot addr=0x15F480
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
float CFontBrx::DxDrawCh(char ch, float x, float y, unsigned int rgba, GIFS *pgifs)
{
    GLYPH *pglyph = PglyffFromCh(this, ch);
    if (pglyph) {
        int dyTex = 1 << m_tex0.TH;
        float xRight = x + (float)pglyph->dx * m_rxScale;
        float yBottom = y + (float)m_dyUnscaled * m_ryScale;
        register float zero __asm__("$f12") = 0.0f; // register pin (the 0.0f compare constant)
        if (xRight >= zero && x <= 640.0f && yBottom >= zero && y <= 492.80002f) {
            ListRGBAQ__4GIFSUif(pgifs, (rgba & 0xFFFFFF) | ((((rgba >> 24) + 1) & 0x1FE) << 23));
            ListUV__4GIFSii(pgifs, pglyph->u << 4, (dyTex - pglyph->v) << 4);
            ListXYZF__4GIFSiiii(pgifs, (int)((x * 0.8f + 1792.0f) * 16.0f), (int)((y * 0.45454547f + 1936.0f) * 16.0f), m_z, 0);
            ListUV__4GIFSii(pgifs, (pglyph->u + pglyph->dx) << 4, (dyTex - (pglyph->v + m_dyUnscaled)) << 4);
            ListXYZF__4GIFSiiii(pgifs, (int)((xRight * 0.8f + 1792.0f) * 16.0f), (int)((yBottom * 0.45454547f + 1936.0f) * 16.0f), m_z, 0);
        }
        return m_rxScale * (float)(pglyph->dx + 1);
    }
    return (float)m_dxSpace * m_rxScale;
}
