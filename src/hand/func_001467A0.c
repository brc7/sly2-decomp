typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
struct PO { char pad[0x2DC]; int x2DC; float g2E0; char pad2[0xDF4 - 0x2E4]; int xDF4; char pad3[0x18C8 - 0xDF8]; int x18C8; char pad4[0x18D8 - 0x18CC]; int x18D8; };
struct T { char pad[0x330]; int x330; };
struct P { char pad[0x10]; struct PO *p10; char pad2[0x30 - 0x14]; int x30; float x34; struct T *p38; };
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern qword D_002A2DA0;
extern float D_002BD360;
extern float D_002BD368;
extern float D_002BD36C;
extern float D_002BD370;
void func_001EEF98(struct P *p);
int func_001A61A0(struct T *p);
qword func_001A62E8(void *p);
struct M3 *func_001A6318(void *p);
void func_00146E18(struct P *p, int n);
float GRandInRange__Fff(float gMin, float gMax);

static __inline__ float Dot(qword a, qword b)
{
    qword r, t;
    float g;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(r), "=j"(t) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(r));
    return g;
}

static __inline__ float SafeNormalize(qword *pv, qword *pvDefault)
{
    register qword v __asm__("vf3") = *pv;
    qword t, l;
    float g;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(t), "=j"(l) : "j"(v));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(t), "0"(l));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
    if (g < 0.0001f)
        *pv = *pvDefault;
    else
    {
        qword r;
        __asm__("vdiv Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, Q" : "=&j"(r) : "j"(l), "j"(v));
        *pv = r;
    }
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
    return g;
}

void func_001467A0(struct P *p)
{
    union { qword q; float f[4]; } d;
    float g;
    int state;
    func_001EEF98(p);
    if (p->x30 == -1)
        return;
    if (p->p10->xDF4 != -1 || !func_001A61A0(p->p38) || p->p38->x330) {
        func_00146E18(p, -1);
        return;
    }
    {
        qword a = func_001A62E8(p->p38);
        qword b = func_001A62E8(p->p10);
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d.q) : "j"(a), "j"(b));
        d.f[2] = 0.0f;
        g = SafeNormalize(&d.q, &D_002A2DA0);
    }
    do {
        state = p->x30;
        switch (state) {
        case 0:
            if (g < D_002BD360) {
                struct M3 *pm = func_001A6318(p->p10);
                if (0.8f < Dot(pm->r[0], d.q)) {
                    if (GRandInRange__Fff(0.0f, 1.0f) < 0.3f)
                        func_00146E18(p, 3);
                    else
                        func_00146E18(p, 2);
                }
            } else if (D_002BD36C < g) {
                func_00146E18(p, 1);
            }
            break;
        case 1:
            if (g < D_002BD368)
                func_00146E18(p, 0);
            break;
        case 2:
            if (p->p10->x18D8 == -1 && 0.1f < D_002DDED0.t - p->x34)
                func_00146E18(p, -1);
            break;
        case 3: {
            float t = D_002DDED0.t;
            if (0.1f < t - p->x34) {
                struct PO *po = p->p10;
                if (po->x2DC != -1) {
                    if (po->x2DC == 1 && D_002BD370 < t - po->g2E0)
                        func_00146E18(p, 4);
                } else
                    func_00146E18(p, -1);
            }
            break;
        }
        case 4:
            if (0.1f < D_002DDED0.t - p->x34 && p->p10->x18C8 != 13)
                func_00146E18(p, -1);
            break;
        }
    } while (p->x30 != state);
}
