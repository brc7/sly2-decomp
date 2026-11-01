struct O { char pad[0x5C]; float *ag; };
struct S { char pad[0xE8]; int c; };
struct O *func_001E7840(struct S *p, int n);
/** @sly2-annot addr=0x1E79A0
 * Purpose: Last entry of the chain's sample array (0 if no points)
 * Info: proposed name SGetShapeChainLength (T2); TU shape
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
float func_001E79A0(struct S *p)
{
    float g;
    if (p->c > 0) g = func_001E7840(p, 0)->ag[p->c - 1];
    else g = 0.0f;
    return g;
}
