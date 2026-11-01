struct X { char pad[0xE8]; int c; float *ag; };
extern float D_002A813C;
void func_001E67B8(struct X *p, int a, int b, float g);
float func_001E63C0(struct X *p, float g);
/** @sly2-annot addr=0x1E7410
 * Purpose: Pick the vertex parameter minimizing a callback; evaluate pos/dir and s there
 * Info: proposed name FindShapeBestVertex (T2); TU shape
 * Notes: names_v2 B proposed RenderCrvcSegment__FP4CRVCiP7MATRIX4P2CMG4RGBAi (0.856)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void func_001E7410(struct X *p, float (*pfn)(float, void *), void *pv, int a, int b, float *pg, float *pgOut)
{
    int i;
    float gBest = 0.0f;
    float dMin = D_002A813C;
    for (i = 0; i < p->c; i++) {
        float g = p->ag[i];
        float d = pfn(g, pv);
        if (d < dMin) {
            dMin = d;
            gBest = g;
        }
    }
    if (a || b) func_001E67B8(p, a, b, gBest);
    if (pg) *pg = gBest;
    if (pgOut) *pgOut = func_001E63C0(p, gBest);
}
