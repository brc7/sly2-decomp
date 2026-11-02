struct RGBA { unsigned char r, g, b, a; };
struct S { int x0; int x4; int x8; int xc; int x10; struct RGBA c1; struct RGBA c0; unsigned int x1c; };
void func_002160D8(struct S *p);
/** @sly2-annot addr=0x215CA8
 * Purpose: Reset the HUD rich-text cursor: optional new text, pchCur=achz, font cur=base, colour cur=base,
 *   colour word 0x80000000|alpha, UpdateColor
 * Info: proposed name CRichText2_Reset (T2); class CRichText; TU hud
 * Notes: names_v2 C proposed ShowBlot__FP4BLOT (0.438) | COLLISION: CRichText_Reset already used at 0x15F970
 *   (existing symbol); renamed CRichText_Reset_215CA8 | v3: superseded CRichText_Reset_215CA8 | top50_round
 *   (0.50): Same role as Sly 1 CRichText::Reset (m_pchCur=m_achz; m_pfontCur=m_pfontBase;
 *   m_rgbaCur=m_rgbaBase). But this is a second class: Reset__9CRichText already exists at 0x15F970 with a
 *   different layout (colours at +0x150..+0 | T2: placeholder token CRichText2
 * Confidence: low (draft) [T2 0.50] - evidence: Sly 1 / Lilly reference; semantic review; static naming pass;
 *   sources: r7c_hud+top50_round
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00215CA8(struct S *p, int a)
{
    if (a)
        p->x0 = a;
    p->x4 = p->x0;
    p->x8 = p->xc;
    p->c1 = p->c0;
    p->x1c = 0x80000000;
    ((unsigned char *)&p->x1c)[3] = p->c1.a;
    func_002160D8(p);
}
