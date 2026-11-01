typedef int qword __attribute__((mode(TI)));
struct T { char pad[0x218]; qword *pq; };
struct S { char pad[0xE4]; int fE4; };
void FindShapeClosestPoint(struct S *p, qword q, int a, void *pv, int b, int c, float *pf);
float SMaxShape(struct S *p);
void func_001CBAC0(struct S *p, struct T *q, float *pf)
{
    qword buf;
    float g, v;
    FindShapeClosestPoint(p, *q->pq, 0, &buf, 0, 0, pf);
    if (p->fE4 == 0) {
        g = SMaxShape(p) - 50.0f;
        v = *pf;
        if (v < 50.0f) v = 50.0f;
        else if (g < v) v = g;
        *pf = v;
    }
}
