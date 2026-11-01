struct S { char pad[8]; void *x8; char padc[0x7C]; float f88; };
void func_00215CA8(struct S *p, int i);
int func_00215D40(struct S *p);
float func_00216B80(void *p, int ch);
/** @sly2-annot addr=0x2161C8
 * Purpose: widest line
 * Info: proposed name CRichText2_DxMaxLine (T2); class CRichText; TU hud
 * Notes: names_v2 C proposed GetExtents__9CRichTextPfT1f (0.188) | COLLISION: CRichText_DxMaxLine already
 *   used at 0x15FF40 (existing symbol); renamed CRichText_DxMaxLine_2161C8 | v3: superseded
 *   CRichText_DxMaxLine_2161C8 | top50 finding 8: hud-TU rich text (colours +0x14/+0x18) is a second class;
 *   Reset__9CRichText 0x15F970 keeps CRichText. CRichText2 is a placeholder class word (T2); resolves the v2
 *   T3 collision | T2: placeholder token CRichText2
 * Confidence: low (draft) [T2 0.70] - evidence: semantic review; static naming pass; sources:
 *   r7c_hud+top50_round(CRichText2 class split)
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
float func_002161C8(struct S *p)
{
    float fMax = 0.0f;
    float fCur = fMax;
    int ch;
    func_00215CA8(p, 0);
    while ((ch = func_00215D40(p)) != 0) {
        if (ch == 10) {
            fMax = (fMax > fCur) ? fMax : fCur;
            fCur = 0.0f;
        } else {
            fCur += func_00216B80(p->x8, ch) * p->f88;
        }
    }
    fMax = (fMax > fCur) ? fMax : fCur;
    return fMax;
}
