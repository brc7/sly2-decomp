typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } VU;
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern float D_002BD350;
extern struct { float a; float b; } D_002BD358;
extern qword D_002A2DA0;
struct Q { char pad[0x330]; int x330; };
struct R { char pad[0xDF4]; int xDF4; char pad2[0x1714 - 0xDF8]; int x1714; };
struct P { char pad[0x10]; struct R *p10; char pad2[0x30 - 0x14]; int x30; float x34; struct Q *p38; };
void UpdateBhv(struct P *);
int func_001A61A0(struct Q *);
qword func_001A62E8(void *);
qword *func_001A6318(void *);
void func_00146650(struct P *, int);

void func_001462A8(struct P *p)
{
    VU d;
    qword a, b, c, l, one, n;
    float g;
    UpdateBhv(p);
    if (p->x30 == -1)
        return;
    if (p->p10->xDF4 != -1 || !func_001A61A0(p->p38) || p->p38->x330) {
        func_00146650(p, -1);
        return;
    }
    a = func_001A62E8(p->p38);
    b = func_001A62E8(p->p10);
    { qword *pk = &D_002A2DA0;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(c) : "j"(a), "j"(b));
    d.q = c;
    d.f[2] = 0.0f;
    {
        register qword l1 __asm__("vf1");
        register qword r2 __asm__("vf2");
        float gl;
        __asm__("vmul.xyz %0, %1, %1" : "=j"(l1) : "j"(d.q));
        __asm__("vaddw.x %0, $vf0, $vf0w\n\tvadday.x ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(r2), "+j"(l1));
        __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(r2) : "j"(l1));
        l = r2;
        __asm__("qmfc2.ni %0, %1" : "=r"(gl) : "j"(l));
        if (gl < 1e-4f) {
        d.q = *pk;
    } else {
        __asm__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(n) : "j"(d.q), "j"(l));
        d.q = n;
    }
    }
    }
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(l)); /* gcc emits the qmfc2 and schedules it */
    for (;;) {
        int state = p->x30;
        switch (state) {
        case 0:
            if (g < D_002BD350) {
                qword *pm = func_001A6318(p->p10);
                register qword t __asm__("vf2");
                qword one2;
                float dot;
                t = *pm;
                __asm__("vmul.xyz %0, %0, %1" : "+j"(t) : "j"(d.q));
                __asm__("vaddw.x %0, $vf0, $vf0w\n\tvadday.x ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(one2), "+j"(t));
                __asm__("qmfc2.ni %0, %1" : "=r"(dot) : "j"(t));
                if (0.8f < dot)
                    func_00146650(p, 2);
            } else if (D_002BD358.b < g) {
                func_00146650(p, 1);
            }
            break;
        case 1:
            if (g < D_002BD358.a)
                func_00146650(p, 0);
            break;
        case 2:
            if (p->p10->x1714 == -1 && 0.1f < D_002DDED0.t - p->x34)
                func_00146650(p, -1);
            break;
        }
        if (p->x30 == state)
            break;
    }
}
