struct S { char pad[0xE4]; int f; int c; int nEC; float *ag; };
float func_0011D898(float g, float gMax);
/** @sly2-annot addr=0x1E66C8
 * Purpose: If closed, wrap s into [0,length]
 * Info: proposed name SWrapShape (T1); TU shape
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
float func_001E66C8(struct S *p, float g)
{
    float gMax;
    if (!p->f) return g;
    gMax = p->ag[p->c - 1];
    if (0.0f <= g && g <= gMax) return g;
    return func_0011D898(g, gMax);
}
