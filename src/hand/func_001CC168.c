typedef int qword __attribute__((mode(TI)));
struct S { int pad[5]; void *p14; void *p18; int pad2[3]; void *p28; void *p2C; };
void EvaluateShapeU(void *p, float t, qword *pq, qword *tangent);
void FindClosestPointOnLineSegment(qword q, qword a, qword b, qword *position, float *projection, float *distance);
void func_001CC168(struct S *p, qword q, float *c, float *d, float t)
{
    qword a, b;
    EvaluateShapeU(p->p14, t, &a, 0);
    EvaluateShapeU(p->p28, t, &b, 0);
    FindClosestPointOnLineSegment(q, a, b, 0, c, d);
}
