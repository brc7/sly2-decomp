typedef int qword __attribute__((mode(TI)));
struct S { char pad[0xE4]; int fE4; };
qword func_001A62E8(void *q);
void FindShapeClosestPoint(struct S *p, qword q, int a, void *pv, int b, int c, float *pf);
float SMaxShape(struct S *p);
static inline float GLimit(float x, float lo, float hi)
{
    if (x < lo) return lo;
    if (hi < x) return hi;
    return x;
}
void func_001C3958(struct S *p, void *q, float *pf)
{
    FindShapeClosestPoint(p, func_001A62E8(q), 0, 0, 0, 0, pf);
    if (p->fE4 == 0) {
        *pf = GLimit(*pf, 100.0f, SMaxShape(p) - 35.0f);
    }
}
