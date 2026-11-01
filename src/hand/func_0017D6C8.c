typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002BE758, D_002BE75C;
struct W { int pad[0x28 / 4]; int n28; };
struct G { char pad[0x45CC]; struct W *p45CC; };
extern struct G D_003DD490;
struct T { char pad[0x30]; void *p30; float f34; };
struct V { int pad[2]; float f8; };
struct A { char pad[0x218]; struct V *p218; char pad2[0xFE8 - 0x21C]; int nFE8; };
struct S {
    char pad[0x10]; struct A *p10; int pad2; struct T *p18; char pad3[0x14]; int n30;
    char pad4[0x80 - 0x34]; int n80; int n84; char pad5[8]; float a90[4]; float fA0; int nA4;
};
void UpdateBhv(struct S *p);
int FBhvBusyAbove(struct S *p);
int func_00178D00(struct T *t);
void func_0017D450(struct S *p, int n);
int func_001792E8(struct T *t);
float SMaxShape(void *p);
void EvaluateShapeS(void *p, float f, float *pv, int n);
int func_0017E570(struct T *t);
int func_0017EAE8(struct T *t);
void func_0017DA28(struct S *p);
void func_0017D6C8(struct S *p) {
    int s;
    UpdateBhv(p);
    if (!(p->p10->nFE8 & 2)) return;
    while (!FBhvBusyAbove(p)) {
        s = p->n30;
        switch (s) {
        case 1:
        case 2:
            {
                struct T *t = p->p18;
                if (s == 2 && func_00178D00(t)) {
                    func_0017D450(p, 3);
                } else if (D_002DDED0.t - p->fA0 > D_002BE758 || p->nA4 != D_003DD490.p45CC->n28) {
                    func_0017DA28(p);
                } else if (func_001792E8(t)) {
                    float f = t->f34 + D_002BE75C;
                    float hi = SMaxShape(t->p30);
                    p->n80 = 0;
                    p->n84 = 0;
                    EvaluateShapeS(t->p30, f < hi ? f : hi, p->a90, 0);
                    p->a90[2] = p->p10->p218->f8;
                    func_0017D450(p, 5);
                }
            }
            break;
        case 3:
            if (func_0017E570(p->p18)) func_0017DA28(p);
            break;
        case 5:
            if (func_0017EAE8(p->p18)) func_0017DA28(p);
            break;
        }
        if (s != p->n30) continue;
        break;
    }
}
