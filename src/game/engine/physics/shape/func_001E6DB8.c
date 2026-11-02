typedef int qword __attribute__((mode(TI)));
struct X { char pad[0xE4]; int fWrap; int c; float *ag; };
struct SPACE;
struct CTX { struct X *p; struct SPACE *space; int pad[2]; qword q; };
extern float D_002A8130;
extern float D_002A8134;
float func_001E69A0(void *pv, float g);
void func_0011D9E0(float (*pfn)(void *, float), void *pv, float g, float dg, float gMin, float gMax, float *pg, float *pgOut);
float UWrapShape(struct X *p, float g);
void EvaluateShapeU(struct X *p, float g, qword *a, qword *b);
float SFromShapeU(struct X *p, float g);
/** @sly2-annot addr=0x1E6DB8
 * Purpose: Closest point on a curved shape by MinimizeRange around a start parameter (step = segment/10)
 * Info: proposed name FindShapeClosestPointCurveFromU (T2); TU shape
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void func_001E6DB8(struct X *p, qword q, struct SPACE *space, qword *a, qword *b, float *pg, float *pgOut, float g)
{
    struct CTX ctx;
    float gRes;
    ctx.space = space; ctx.q = q; ctx.p = p;
    func_0011D9E0(func_001E69A0, &ctx, g, (p->ag[1] - p->ag[0]) * 0.1f, p->fWrap ? D_002A8130 : p->ag[0], p->fWrap ? D_002A8134 : p->ag[p->c - 1], &gRes, 0);
    gRes = UWrapShape(p, gRes);
    if (a || b) EvaluateShapeU(p, gRes, a, b);
    if (pg) *pg = gRes;
    if (pgOut) *pgOut = SFromShapeU(p, gRes);
}
