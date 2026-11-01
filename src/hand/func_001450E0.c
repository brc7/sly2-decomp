typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct CLQ { float g0, g1, g2, gPad; };
struct SO { char pad[0x2C0]; ulong grf; };
struct LIST { char pad[0x28]; int c; struct SO *a[1]; };
struct PO { char pad[0x1088]; struct LIST *plist; };
struct P { char pad[0x10]; struct PO *p10; char pad2[0x30 - 0x14]; void *p30; char pad3[0x90 - 0x34]; struct SO *p90; };
extern float D_002A4E30;
extern struct CLQ D_002BD300;
extern struct CLQ D_002BD310;
extern struct CLQ D_002BD320;
qword func_001A62E8(void *p);
ulong GrfFromSgf(int n);
int FStepguardHasSgf(struct SO *p, int n);
struct LIST *func_001F8240(void *p);

static __inline__ float GDist(qword a, qword b)
{
    qword d, t;
    float g;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z" : "+j"(d), "=j"(t));
    __asm__("vsqrt Q, %0x\n"
        "vwaitq\n"
        "vaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
static __inline__ float GFromClq(float g, struct CLQ *pclq) { return pclq->g0 + g * (pclq->g1 + g * pclq->g2); }

struct SO *func_001450E0(struct P *p)
{
    struct SO *psoBest = 0;
    float gBest = D_002A4E30;
    qword pos;
    ulong grf;
    struct LIST *plist;
    int i;
    func_001A62E8(p->p10);
    pos = func_001A62E8(p->p30);
    grf = GrfFromSgf(7) | GrfFromSgf(0x10) | GrfFromSgf(0x16) | GrfFromSgf(0x17) |
          GrfFromSgf(0x2A) | GrfFromSgf(0x2B);
    plist = p->p10->plist;
    for (i = 0; i < plist->c; i++) {
        struct SO *pso = plist->a[i];
        float g;
        if (grf & pso->grf)
            continue;
        g = GDist(pos, func_001A62E8(pso));
        if (pso == p->p90)
            g = GFromClq(g, &D_002BD300);
        if (FStepguardHasSgf(pso, 3))
            g = GFromClq(g, &D_002BD320);
        if (g < gBest) {
            psoBest = pso;
            gBest = g;
        }
    }
    plist = func_001F8240(p->p30);
    if (plist) {
        for (i = 0; i < plist->c; i++) {
            struct SO *pso = plist->a[i];
            float g;
            if (grf & pso->grf)
                continue;
            g = GDist(pos, func_001A62E8(pso));
            g = GFromClq(g, &D_002BD310);
            if (pso == p->p90)
                g = GFromClq(g, &D_002BD300);
            if (FStepguardHasSgf(pso, 3))
                g = GFromClq(g, &D_002BD320);
            if (g < gBest) {
                psoBest = pso;
                gBest = g;
            }
        }
    }
    return psoBest;
}
