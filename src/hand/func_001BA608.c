typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern void *D_002E1E40;
struct POLY { float c0, c1, c2; float pad; float gMin, gMax; float dt; };
struct S { char pad[0xE8]; void *xE8; void *xEC; };
struct O { char pad[0xF64]; struct S *pF64; char pad2[0x1070 - 0xF68]; struct POLY poly; };
struct Q1 { char pad[0xC4]; float gC4; };
struct Q2 { char pad[0x38]; float g38; };
struct P { char pad[0x10]; struct O *po; char pad2[0x30 - 0x14]; int x30; float g34; };
void UpdateBhv(struct P *p);
int FBhvBusyAbove(struct P *p);
qword func_001A62E8(void *p);
int FCheckVolPoint(void *p, qword v);
void func_001BA828(struct P *p, int n);
int FStepguardHasSgf(struct O *po, int n);
void *func_00205458(struct O *po, int n);
static inline float Dist(qword a, qword b) {
    qword d;
    float f;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
static inline float GEval(struct POLY *pp, float x) { return pp->c0 + x * (pp->c1 + x * pp->c2); }
static inline float GClamp(float g, float lo, float hi) { float r; if (g < lo) r = lo; else if (hi < g) r = hi; else r = g; return r; }
void func_001BA608(struct P *p)
{
    int s;
    UpdateBhv(p);
    for (;;) {
        if (FBhvBusyAbove(p))
            break;
        s = p->x30;
        switch (s) {
        case 0: {
            struct S *ps = p->po->pF64;
            if (!FCheckVolPoint(ps->xEC, func_001A62E8(D_002E1E40)))
                func_001BA828(p, 1);
            break;
        }
        case 1: {
            struct S *ps = p->po->pF64;
            if (FCheckVolPoint(ps->xE8, func_001A62E8(D_002E1E40)))
                func_001BA828(p, 0);
            else if (p->po->poly.dt < D_002DDED0.t - p->g34)
                func_001BA828(p, 2);
            break;
        }
        case 2:
            break;
        }
        if (s == p->x30)
            break;
    }
    if (p->x30 == 0 && FStepguardHasSgf(p->po, 9)) {
        float d = Dist(func_001A62E8(p->po), func_001A62E8(D_002E1E40));
        struct O *po = p->po;
        float r = GClamp(GEval(&po->poly, d), po->poly.gMin, po->poly.gMax);
        struct Q1 *q1 = func_00205458(p->po, 0xA7);
        struct Q2 *q2 = func_00205458(p->po, 0xB8);
        if (q1)
            q1->gC4 = r;
        else if (q2)
            q2->g38 = r;
    }
}
