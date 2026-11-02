struct VTE { short d; short pad; void (*pfn)(); };
struct S { char pad[0x20]; struct VTE *vt; };
/** @sly2-annot addr=0x182CD8
 * Purpose: Call slot B (closest param) then slot C (pos at that param)
 * Info: proposed name GetGrabspotPosNear (T2); TU grabspot
 * Notes: names_v2 C proposed SetStepRunTarget__FP4STEPffP5BLRUNP6ASEGBL (0.638)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00182CD8(struct S *p, int a, float *pg, int b, int c)
{
    float g;
    struct VTE *e;
    e = &p->vt[2];
    ((void (*)(void *, int, float *))e->pfn)((char *)p + e->d, a, &g);
    e = &p->vt[3];
    ((void (*)(void *, int, int, int, float))e->pfn)((char *)p + e->d, a, b, c, g);
    if (pg) *pg = g;
}
