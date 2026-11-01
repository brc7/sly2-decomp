struct S { char pad[0xE4]; int fWrap; };
float func_001E6620(struct S *p);
float func_001E6648(struct S *p);
float func_001E66C8(struct S *p, float g);
static __inline__ float GClamp(float g, float lo, float hi) { if (g < lo) return lo; if (g > hi) return hi; return g; }
/** @sly2-annot addr=0x1E6728
 * Purpose: Closed: SWrapShape; open: clamp s to [SMin,SMax]
 * Info: proposed name SClampShape (T1); TU shape
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
float func_001E6728(struct S *p, float g)
{
    if (p->fWrap) return func_001E66C8(p, g);
    return GClamp(g, func_001E6620(p), func_001E6648(p));
}
