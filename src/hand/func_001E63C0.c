struct X { char pad[0xE0]; int xE0; char pad1[0xF0 - 0xE4]; float *ag; char pad2[0x14C - 0xF4]; float agSub[11]; };
int func_001E7538(struct X *p, float *pa, float *pb);
void func_001E5FC8(struct X *p, int i);
static __inline__ float GLerp(float a, float b, float t) { return (1.0f - t) * a + t * b; }
/** @sly2-annot addr=0x1E63C0
 * Purpose: Arc length at parameter u (interpolated from the distance table, with per-segment sub-samples for
 *   curves)
 * Info: proposed name SFromShapeU (T1); TU shape
 * Notes: names_v2 B proposed IposFindAposG__FfiPfiT2T2 (0.819)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
float func_001E63C0(struct X *p)
{
    float a, b;
    float g;
    int i = func_001E7538(p, &a, &b);
    if (p->xE0 == 0) { g = GLerp(p->ag[i], p->ag[i + 1], a / b); } else { float u; int j; func_001E5FC8(p, i); u = a * 10.0f / b; j = (int)u; g = GLerp(p->agSub[j], p->agSub[j + 1], u - j); g += p->ag[i]; }
    return g;
}
