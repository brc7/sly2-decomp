typedef int qword __attribute__((mode(TI)));
struct LO;
typedef struct { short d; short pad; qword (*pfn)(struct LO *); } VTE;
struct LO { int x0, x4, x8; VTE *vt; char pad10[0xD0 - 0x10]; float xD0; char padD4[0x1A8 - 0xD4]; float x1A8; char pad1AC[0x208 - 0x1AC]; int x208; };
struct S { struct LO *po; int x4; float x8; char pad[0x90 - 0xC]; int x90; float x94, x98, x9C; };
struct CLOCK { float t, dt; };
extern struct CLOCK D_002DDED0;
extern float D_002BFAA0[3], D_002BFAB0[3], D_002BFAC0[];
extern float D_002BBB60[2];
void func_001F4BF8(struct LO *po);
float func_00244E10(float g);
float func_002458F0(float y, float x);
float func_0011D2D0(void *p, int n, float a, float b, float c);
void func_00204660(struct LO *po, float g);
int func_0019A098(struct LO *po, int n, float g);
void func_001A15D0(struct S *s);
static inline float GClamp(float g, float lo, float hi)
{
    float r;
    if (g < lo) r = lo;
    else if (hi < g) r = hi;
    else r = g;
    return r;
}
/** @sly2-annot addr=0x1A1358
 * Purpose: jts 4 per-frame update: smooth heading (RadSmooth), timers, run pending callback, FCheckJtStuck
 *   escape
 * Info: proposed name UpdateJtActiveZap (T1); TU hurt
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
void func_001A1358(struct S *s)
{
    if (s->x90 != -1) {
        float u;
        float w;
        float a;
        if (s->x4 == -1) {
            float dt = D_002DDED0.t - s->x98;
            if (s->x9C < dt) {
                s->x90 = -1;
                s->po->xD0 = 0.0f;
                func_001F4BF8(s->po);
                goto done;
            }
            u = dt / s->x9C;
        } else {
            u = 0.0f;
        }
        w = func_00244E10((D_002DDED0.t - s->x94) * 10.0f);
        a = GClamp(D_002BFAA0[0] + u * (D_002BFAA0[1] + u * D_002BFAA0[2]), D_002BBB60[0], D_002BBB60[1]);
        s->po->xD0 = a * GClamp(D_002BFAB0[0] + w * (D_002BFAB0[1] + w * D_002BFAB0[2]), D_002BBB60[0], D_002BBB60[1]);
    }
done:
    switch (s->x4) {
    case -1:
        break;
    case 7: {
        union { qword q; float f[4]; } v;
        float g;
        v.q = s->po->vt[33].pfn((struct LO *)((char *)s->po + s->po->vt[33].d));
        g = func_002458F0(v.f[1], v.f[0]) + 3.1415927f;
        func_00204660(s->po, func_0011D2D0(D_002BFAC0, 0, s->po->x1A8, g, D_002DDED0.dt));
        if (0.25f < D_002DDED0.t - s->x8 && s->po->x208)
            func_001A15D0(s);
        else if (func_0019A098(s->po, 0, s->x8))
            func_001A15D0(s);
        break;
    }
    }
}
