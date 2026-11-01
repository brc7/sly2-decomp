struct RGBA { unsigned char r, g, b, a; };
struct S {
    int x0; unsigned char *pch; int x8; int xC; int x10;
    struct RGBA c1; struct RGBA c0; struct RGBA c2;
    char pad20[0x40]; int x60;
};
void func_002160D8(struct S *p);
/** @sly2-annot addr=0x215D40
 * Purpose: next printable char, processing &-escape codes
 * Info: proposed name CRichText2_ChNext (T2); class CRichText; TU hud
 * Notes: COLLISION: CRichText_ChNext already used at 0x15F9D0 (existing symbol); renamed
 *   CRichText_ChNext_215D40 | v3: superseded CRichText_ChNext_215D40 | top50 finding 8: hud-TU rich text
 *   (colours +0x14/+0x18) is a second class; Reset__9CRichText 0x15F970 keeps CRichText. CRichText2 is a
 *   placeholder class word (T2); resolves the v2 T3 collision | T2: placeholder token CRichText2
 * Confidence: low (draft) [T2 0.70] - evidence: semantic review; sources: r7c_hud+top50_round(CRichText2
 *   class split)
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
unsigned char func_00215D40(struct S *p)
{
    int fDone = 0;
    p->x60 = 0;
    while (!fDone) {
        unsigned char *pch = p->pch;
        unsigned char c = *pch;
        if (c == '&') {
            if (pch[1] == c) {
                p->pch = pch + 1;
                fDone = 1;
            } else if (pch[1] == '.') {
                p->pch = pch + 2;
                p->x8 = p->xC;
                p->c1 = p->c0;
                p->x60 = 1;
                func_002160D8(p);
            } else {
                p->pch = pch + 2;
                p->x8 = p->x10;
                p->c1 = p->c2;
                p->x60 = 1;
                func_002160D8(p);
            }
        } else {
            fDone = 1;
        }
    }
    return *p->pch++;
}
