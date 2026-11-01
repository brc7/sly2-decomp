typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct G;
extern struct G D_003DD490;
extern qword D_002A2DD0;
extern float D_002A5584;
extern void *D_002F7900;
extern char D_002DFC00[];
struct E4 { char pad[0x1C]; float f1C; };
struct E { int n0; struct E4 *p4; void *p8; };
struct A { char pad[0x1A8]; float f1A8; char pad2[0x334 - 0x1AC]; int n334; char pad3[0x35C - 0x338]; char a35C[4]; char pad4[0xF04 - 0x360]; int nF04; char pad5[0xFD0 - 0xF08]; char aFD0[4]; };
struct S { char pad[0x10]; struct A *p10; char pad2[0x1C]; int n30; char pad3[0xC]; struct E *p40; char pad4[0xC]; int n50; char pad5[8]; int n5C; char pad6[0x20]; union { qword v; float f[4]; } v80; char pad7[0x20]; float fB0; float fB4; };
void func_00174950(struct S *p);
float func_002458F0(float y, float x);
float *func_001A6318(struct A *a);
void func_00209818(void *p, float f);
float RadNormalize__Ff(float f);
qword SetVectorCylind(float a, float b, float c);
qword func_001A62E8(void *p);
void func_0020C6F8(void *p, qword v);
void func_0020AB68(struct A *a, int b, int c, qword v);
void func_001EEFC0(struct S *p);
void func_00156C80(struct G *g, int n, qword v, struct A *a, void *q);
void *PpoHero(struct G *g);
int FFindLoParent(int n, void *o);
void func_00187E28(void *p, int n, float f);
void func_00179328(struct S *p) {
    int f;
    float a;
    struct E *e;
    func_00174950(p);
    {
        qword d, one, l;
        float g;
        __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(p->v80.v));
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
        __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
        f = 0;
        if (g > 1.0e-4f) f = p->n5C != 0x40000;
    }
    if (f) {
        a = func_002458F0(-p->v80.f[1], -p->v80.f[0]);
    } else {
        float *m = func_001A6318(p->p10);
        a = func_002458F0(m[1], func_001A6318(p->p10)[0]);
    }
    func_00209818(p->p10->aFD0, a);
    e = p->p40;
    if (e && e->n0 == 2) {
        qword r = SetVectorCylind(RadNormalize__Ff(p->p10->f1A8 + 3.14159274101257324219e+00f), p->p40->p4->f1C, 0.0f);
        qword pos = func_001A62E8(p->p10);
        qword s;
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(s) : "j"(pos), "j"(r));
        func_0020C6F8(p->p40->p8, s);
    }
    func_0020AB68(p->p10, 0, 0, D_002A2DD0);
    if (p->n30 == 0) func_001EEFC0(p);
    func_00156C80(&D_003DD490, 1, func_001A62E8(p->p10), p->p10, p->p10->a35C);
    p->fB0 = p->fB4 = D_002A5584;
    p->p10->nF04 = 0;
    if (p->p10->n334) {
        void *o = PpoHero(&D_003DD490);
        if (o && FFindLoParent(p->n50, o) && (o == D_002F7900 || (p->n5C & 0x40000))) {
            func_00187E28(D_002DFC00, 3, 1.50000005960464477539e-01f);
            p->fB0 = D_002DDED0.t + 5.00000044703483581543e-02f;
        }
        p->fB4 = D_002DDED0.t + 1.33333340287208557129e-01f;
    }
}
