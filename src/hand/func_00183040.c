typedef int qword __attribute__((mode(TI)));
struct T { char pad[0x218]; qword *pq; };
struct S { char pad[0xE4]; int xe4; };
struct V { float a[4]; };
void FindShapeClosestPoint(struct S *p, qword q, int a, struct V *pv, int b, int c, float *pg);
float SMaxShape(struct S *p);
void func_00183040(struct S *p, struct T *t, float *pg)
{
    struct V v;
    FindShapeClosestPoint(p, *t->pq, 0, &v, 0, 0, pg);
    if (!p->xe4) {
        float lim = SMaxShape(p) - 30.0f;
        float g = *pg;
        if (g < 30.0f) g = 30.0f;
        else if (lim < g) g = lim;
        *pg = g;
    }
}
