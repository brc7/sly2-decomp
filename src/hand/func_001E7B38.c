struct S { char pad[0xE4]; int fWrap; };
float func_001E6648(struct S *p);
/** @sly2-annot addr=0x1E7B38
 * Purpose: b-a, wrapped to the shorter way round on closed shapes
 * Info: proposed name DsShape (T2); TU shape
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
float func_001E7B38(struct S *p, float a, float b)
{
    float d = b - a;
    if (p->fWrap) {
        float r = func_001E6648(p);
        if (d > r * 0.5f) d -= r;
        else if (d < r * -0.5f) d += r;
    }
    return d;
}
