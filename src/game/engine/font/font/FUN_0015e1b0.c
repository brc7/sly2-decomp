struct FONT { int pad[3]; int dy; int pad10; float ry; };
struct RT { int pad[3]; struct FONT *pfont; };
int ClineWrap__9CRichTextf(struct RT *p, float dx);
/** @sly2-annot addr=0x15FEF8
 * Purpose: UNKNOWN - no analysis recorded yet (byte-matched only).
 * Confidence: none - unanalysed; anything inferred from the code here is a guess
 * See: docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
float FUN_0015e1b0(struct RT *p, float dx)
{
    float dy = (float)p->pfont->dy * p->pfont->ry;
    return dy * (float)ClineWrap__9CRichTextf(p, dx);
}
