typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } V;
extern float D_002C0AC4;
extern char D_003DD020[];
float func_001E66C8(void *a, float g);
void func_001E6F10(void *a, float t, V *pv, int n);
void *func_001BD310(void *a, void *b, qword q2, qword q3);
int func_001BD560(void *x, void *a, float t, int dir, V *pv, int n);
void func_001E6F60(void *x, qword q, void *g, int a, int b, int c, float *pf);
int func_001BCCE0(void *x, void *a, float *pf, int n, float f);
/** @sly2-annot addr=0x1BD718
 * Purpose: Free point on a shape around parameter g avoiding obstacles
 * Info: proposed name FindShapeFreePoint (T2); TU tu_1BC050
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
int func_001BD718(qword q, void *a1, void *a2, void *obj, int dir, void **pp, float *pf, float g)
{
    float t;
    V v, v2;
    float f0, f1;
    void *x;
    int r;
    if (dir) t = func_001E66C8(obj, g + D_002C0AC4);
    else t = func_001E66C8(obj, g - D_002C0AC4);
    func_001E6F10(obj, t, &v, 0);
    x = func_001BD310(a1, a2, q, v.q);
    if (pp) *pp = x;
    if (!x) return 0;
    r = func_001BD560(x, obj, t, dir, &v2, 0);
    if (r != -1) return r;
    func_001E6F60(x, q, D_003DD020, 0, 0, 0, &f0);
    func_001E6F60(x, v2.q, D_003DD020, 0, 0, 0, &f1);
    if (pf) *pf = f1;
    else pf = &f1;
    return func_001BCCE0(x, a1, pf, -1, f0);
}
