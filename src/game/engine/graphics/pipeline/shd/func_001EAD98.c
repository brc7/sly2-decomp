struct H { int a; int b; unsigned short c; unsigned short d; int *ap; };
struct S { struct H h; unsigned int cqw; };
void ColorCorrectRgba(int *p);
/** @sly2-annot addr=0x1EAD98
 * Purpose: Copy a CLUT header, color-correct its colors, compute qword count
 * Info: proposed name CopyClutHeader (T2); TU shd
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
struct S *func_001EAD98(struct S *p, struct H *ph)
{
    int *pn;
    p->h = *ph;
    for (pn = p->h.ap; pn < p->h.ap + p->h.c; pn++)
        ColorCorrectRgba(pn);
    p->cqw = (p->h.c * sizeof(int) + 15) >> 4;
    return p;
}
