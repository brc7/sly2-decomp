struct S { char pad[8]; void *x8; char pad2[0x7C]; float x88; };
void func_00215CA8(struct S *p, int a);
int func_00215D40(struct S *p);
float func_00216B80(void *p, int ch);
/** @sly2-annot addr=0x215ED8
 * Purpose: sum of glyph widths * scale
 * Info: proposed name CRichText_Dx (T1); class CRichText; TU hud
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
float CRichText_Dx(struct S *p)
{
    float sum = 0.0f;
    int ch;
    func_00215CA8(p, 0);
    while ((ch = func_00215D40(p)) != 0)
        sum += func_00216B80(p->x8, ch) * p->x88;
    return sum;
}
