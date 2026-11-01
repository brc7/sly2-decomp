struct S { char pad[0xE4]; int f; int c; float *ag; };
float func_0011D898(float g, float gMax);
/** @sly2-annot addr=0x1E6660
 * Purpose: If closed, wrap u into [times[0],times[c-1]] (GModPositive)
 * Info: proposed name UWrapShape (T1); TU shape
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
float func_001E6660(struct S *p, float g)
{
    float gMin, gMax;
    if (!p->f) return g;
    gMin = p->ag[0];
    gMax = p->ag[p->c - 1];
    if (gMin <= g && g <= gMax) return g;
    return gMin + func_0011D898(g - gMin, gMax - gMin);
}
