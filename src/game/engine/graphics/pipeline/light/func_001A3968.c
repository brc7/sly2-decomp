typedef int qword __attribute__((mode(TI)));
typedef struct { float x, y, z; } V3;
struct S;
typedef struct { short d; short pad; V3 *(*pfn)(struct S *); } VTE;
struct S {
    int x0, x4, x8;
    VTE *vt;
    char pad10[0xE0 - 0x10];
    int xE0;
    char padE4[0xF0 - 0xE4];
    char xF0[8];
    float xF8;
    int xFC;
    union { qword q; struct { float x, y, z, w; } f; } x100;
    float x110, x114;
    int pad118[2];
    int x120;
    float x124, x128, x12C;
    float x130, x134, x138, x13C;
    V3 x140;
    float x14C, x150, x154;
    int pad158[3];
    float x164, x168;
};
void func_001A38A8(float a, float b, float c, float d, float *pa, float *pb);
void func_0011DFA0(void *p, V3 *pv);
void func_001ED2D8(V3 *pv);
qword func_001A62E8(struct S *p);
void FitLinearFunction__FffffPfT4(float a, float b, float c, float d, float *pa, float *pb);
void func_001A3B38(struct S *p);
static inline float Max1(float a, float b) { return (a > b) ? a : b; }
/** @sly2-annot addr=0x1A3968
 * Purpose: Recompute the light's shading parameters: ConvertAngleStrength, HSV->RGB color, FitLinearFunction
 *   falloff, position; then InvalidateLight
 * Info: proposed name RebuildLight (T1); TU light
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A3968(struct S *p)
{
    V3 v;
    float g;
    float a;
    g = Max1(p->x150, p->xF8);
    a = (p->x154 > 30.0f) ? p->x154 : 30.0f;
    func_001A38A8(a, 0.0f, 0.0f, g, &p->x124, &p->x128);
    func_0011DFA0(p->xF0, &v);
    func_001ED2D8(&v);
    p->x130 = v.x;
    p->x134 = v.y;
    p->x138 = v.z;
    p->x13C = p->x150;
    if (0.0f < g) {
        p->x130 = p->x130 / g;
        p->x134 = p->x134 / g;
        p->x138 = p->x138 / g;
        p->x13C = p->x13C / g;
    }
    p->x128 = -p->x128;
    switch (p->x120 = p->xE0) {
    case 0:
        {
            struct F4 { float x, y, z, w; } *pv = (struct F4 *)&p->x100;
            pv->z = 0.0f;
            pv->y = 0.0f;
            pv->x = 0.0f;
        }
        p->x110 = 1.0f;
        p->x114 = 0.0f;
        p->x140 = *p->vt[31].pfn((struct S *)((char *)p + p->vt[31].d));
        break;
    case 1:
        p->x100.q = func_001A62E8(p);
        p->x140 = *(V3 *)&p->x100;
        FitLinearFunction__FffffPfT4(p->x164, 1.0f, p->x168, 0.0f, &p->x12C, &p->x14C);
        p->x110 = p->x12C;
        p->x114 = p->x14C;
        break;
    }
    g = Max1(p->xF8, p->x150);
    p->x110 = p->x110 * g;
    p->x114 = p->x114 * g;
    func_001A3B38(p);
}
