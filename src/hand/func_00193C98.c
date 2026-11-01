typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct O { char pad[0xE4]; int xe4; };
struct Q { int x0; int x4; float x8; int xc; U x10; qword x20; };
struct HIT { qword a; U b; qword c[4]; };
struct P { char pad[0x4D4]; struct O *x4d4; int pad2[2]; float x4e0; int x4e4; };
extern qword D_002A2DD0;
void func_0012DA30(void *r, qword q);
float func_001E6620(struct O *o);
float func_001E6648(struct O *o);
float GModPositive__Fff(float g, float hi);
void func_001E6F10(struct O *o, float g, U *pv, int n);
qword func_001A6F00(int n, struct P *p, qword q);
int func_0014C748(qword a, float g, qword b, qword c, int n, struct HIT *pv);
static __inline__ float GClamp(float g, float lo, float hi)
{
    if (g < lo)
        return lo;
    if (g > hi)
        return hi;
    return g;
}
void func_00193C98(struct P *p, struct Q *q, void *r)
{
    struct O *o;
    float g, lo, hi, t;
    U v;
    struct HIT hit;
    qword d;
    if (!q->x4) {
        func_0012DA30(r, D_002A2DD0);
        return;
    }
    o = p->x4d4;
    g = q->x10.f;
    if (p->x4e4)
        g = -g;
    lo = func_001E6620(o);
    hi = func_001E6648(o);
    if (o->xe4)
        t = GModPositive__Fff(p->x4e0 + g, hi);
    else
        t = GClamp(p->x4e0 + g, lo, hi);
    func_001E6F10(o, t, &v, 0);
    v.q = func_001A6F00(0, p, v.q);
    if (func_0014C748(q->x20, q->x8, q->x10.q, v.q, 1, &hit) > 0) {
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(hit.b.q), "j"(q->x10.q));
        func_0012DA30(r, d);
    }
}
