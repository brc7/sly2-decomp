typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } V;
extern float D_002C0AC4;
extern char D_003DD020[];
float SWrapShape(void *a, float g);
void EvaluateShapeS(void *a, float t, V *pv, int n);
void *func_001BD310(void *a, void *b, qword q2, qword q3);
int func_001BD560(void *x, void *a, float t, int dir, V *pv, int n);
void FindShapeClosestPoint(void *x, qword q, void *g, int a, int b, int c, float *pf);
int func_001BCCE0(void *x, void *a, float *pf, int n, float f);
int func_001BD718(qword q, void *a1, void *a2, void *obj, int dir, void **pp, float *pf, float g)
{
    float t;
    V v, v2;
    float f0, f1;
    void *x;
    int r;
    if (dir) t = SWrapShape(obj, g + D_002C0AC4);
    else t = SWrapShape(obj, g - D_002C0AC4);
    EvaluateShapeS(obj, t, &v, 0);
    x = func_001BD310(a1, a2, q, v.q);
    if (pp) *pp = x;
    if (!x) return 0;
    r = func_001BD560(x, obj, t, dir, &v2, 0);
    if (r != -1) return r;
    FindShapeClosestPoint(x, q, D_003DD020, 0, 0, 0, &f0);
    FindShapeClosestPoint(x, v2.q, D_003DD020, 0, 0, 0, &f1);
    if (pf) *pf = f1;
    else pf = &f1;
    return func_001BCCE0(x, a1, pf, -1, f0);
}
