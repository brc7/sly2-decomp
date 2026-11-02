struct S { char pad[0x84]; float x84; };
int func_00215F40(struct S *p);
float func_002161C8(struct S *p);
/** @sly2-annot addr=0x216260
 * Purpose: dx = DxMaxLine, dy = lines*lineheight
 * Info: proposed name CRichText_GetExtents (T1); class CRichText; TU hud
 * Notes: names_v2 C proposed PostNoteLoad__FP4NOTE (0.521)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r7c_hud
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void CRichText_GetExtents(struct S *p, float *pa, float *pb)
{
    int n = func_00215F40(p);
    float f = func_002161C8(p);
    float g = n * p->x84;
    if (pa)
        *pa = f;
    if (pb)
        *pb = g;
}
