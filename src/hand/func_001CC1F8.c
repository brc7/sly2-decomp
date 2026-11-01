typedef int qword __attribute__((mode(TI)));
struct S { int pad[5]; void *p14; void *p18; int pad2[3]; void *p28; void *p2C; };
void func_001E67B8(void *p, float t, qword *pq, qword *tangent);
void FindClosestPointOnLineSegment(qword q, qword a, qword b, qword *position, float *projection, float *distance);
void func_001CC1F8(struct S *p, qword q, float *c, float *d, float t)
{
    qword a, b;
    func_001E67B8(p->p18, t, &a, 0);
    func_001E67B8(p->p2C, t, &b, 0);
    FindClosestPointOnLineSegment(q, a, b, 0, c, d);
}
