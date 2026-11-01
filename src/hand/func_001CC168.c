typedef int qword __attribute__((mode(TI)));
struct S { int pad[5]; void *p14; void *p18; int pad2[3]; void *p28; void *p2C; };
void func_001E67B8(void *p, float t, qword *pq, qword *tangent);
void func_0011EE08(qword q, qword a, qword b, qword *position, float *projection, float *distance);
/** @sly2-annot addr=0x1CC168
 * Purpose: Closest point of q on segment between the two A curves (hand XF path) evaluated at t
 * Info: proposed name ClosestPointOnJtHandsCurveA (T2); TU jthands
 * Notes: names_v2 C proposed RemoveCmFadeObject__FP2CMP3ALO (0.638)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/player.md#jthands-0x1cc158-0x1cd5f0
 * @end-sly2-annot */
void func_001CC168(struct S *p, qword q, float *c, float *d, float t)
{
    qword a, b;
    func_001E67B8(p->p14, t, &a, 0);
    func_001E67B8(p->p28, t, &b, 0);
    func_0011EE08(q, a, b, 0, c, d);
}
