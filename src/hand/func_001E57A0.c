typedef int qword __attribute__((mode(TI)));
struct T { qword v0; qword v10; float f20; };
struct A { int n0; int n4; int pad8[2]; struct T t; float f40; int n44; };
struct SUB { struct T *apt[2]; float f8C; int n90; int n94; void *p98; int n9C; };
struct P { char pad[0x80]; float f80; struct SUB sub; };
struct G2 { char pad[0x30]; qword v30; };
struct G { char pad[0x20]; float f20; char pad24[0x60 - 0x24]; struct G2 m60; char pad2[0x500 - 0xA0]; char m500[0x10]; int n510; };
extern qword D_002A2DD0;
extern float D_002A8124;
extern struct { float t; float dt; } D_002DDED0;
extern struct G D_002DDF40;
extern struct G2 D_002DDFA0;
int func_0011D4D0(float, float, float);
float RadNormalize__Ff(float);
qword func_0011BFF0(void *);
int func_0014FD50(void *);
void func_0014E4C8(void *, int, int, void *, int, int);
void func_0014E328(void *, int, void *, int, void *);
static inline float X(qword q) { float f; __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q)); return f; }
static inline float Y(qword q) { float f; __asm__("prot3w %0, %1" : "=r"(f) : "r"(q)); return f; }
static inline float Z(qword q) { float f; __asm__("pextuw %0, $0, %1" : "=r"(f) : "r"(q)); return f; }
static inline int FNear(qword a, qword b)
{
    return func_0011D4D0(X(b), X(a), 0.0001f) && func_0011D4D0(Y(b), Y(a), 0.0001f) && func_0011D4D0(Z(b), Z(a), 0.0001f);
}
void func_001E57A0(struct P *p, struct A *pa, struct A *pb, float t)
{
    float tBase;
    int i;
    float *pga, *pgb;
    if (pa->f40 == 0.0f) {
        p->f80 = D_002A8124;
    } else {
        pb->n4 = 0;
        p->f80 = D_002DDED0.t + t;
    }
    p->sub.apt[0] = &pa->t;
    p->sub.apt[1] = &pb->t;
    p->sub.f8C = t;
    p->sub.n90 = 1;
    p->sub.n94 = pa->n44;
    p->sub.p98 = p;
    p->sub.n9C = 1;
    pga = (float *)&pa->t.v10;
    pgb = (float *)&pb->t.v10;
    for (i = 2; i >= 0; i--) {
        *pgb = *pga + RadNormalize__Ff(*pgb - *pga);
        pga++;
        pgb++;
    }
    {
        struct T **ppt = p->sub.apt;
        unsigned j;
        qword *zero=&D_002A2DD0;
        for (j = 0; j < 2; j++, ppt++) {
            struct T *pt = *ppt;
            if (FNear(pt->v0, *zero))
                pt->v0 = D_002DDFA0.v30;
            if (FNear(pt->v10, *zero))
                pt->v10 = func_0011BFF0(&D_002DDFA0);
            if (pt->f20 == 0.0f)
                pt->f20 = D_002DDF40.f20;
        }
    }
    if (p->sub.f8C == 0.0f)
        p->sub.f8C = D_002A8124;
    if (func_0014FD50(D_002DDF40.m500))
        func_0014E4C8(&D_002DDF40, 2, 10, D_002DDF40.m500, 0, D_002DDF40.n510);
    func_0014E328(&D_002DDF40, 10, D_002DDF40.m500, 0, &p->sub);
}
