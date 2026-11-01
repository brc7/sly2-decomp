typedef int qword __attribute__((mode(TI)));
struct S { int pad[5]; void *p14; void *p18; int pad2[3]; void *p28; void *p2C; };
void func_001E67B8(void *p, float t, qword *pq, qword *tangent);
void func_0011EE08(qword q, qword a, qword b, qword *position, float *projection, float *distance);
/** @sly2-annot addr=0x1CC1F8
 * Purpose: Same for the B curves (chain top XF)
 * Info: proposed name ClosestPointOnJtHandsCurveB (T2); TU jthands
 * Notes: names_v2 C proposed ConvertCmScreenToWorld (0.638) | verifier PLAUSIBLE: Evaluates the two B shapes
 *   (+0x18, +0x2C) at u=t and returns the closest point on the SEGMENT between them (ClosestPointOnSegmentT).
 *   Purpose only says 'same for the B curves'; name suffix B is a sibli | T2 forced: verifier: role word not
 *   supported by the code
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest+verdicts_followups
 * See: docs/reference/functions/player.md#jthands-0x1cc158-0x1cd5f0
 * @end-sly2-annot */
void func_001CC1F8(struct S *p, qword q, float *c, float *d, float t)
{
    qword a, b;
    func_001E67B8(p->p18, t, &a, 0);
    func_001E67B8(p->p2C, t, &b, 0);
    func_0011EE08(q, a, b, 0, c, d);
}
