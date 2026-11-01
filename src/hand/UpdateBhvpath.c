typedef int qword __attribute__((mode(TI)));
struct G;
extern struct G D_003DD020;
struct VTE { short d; short pad; void *pfn; };
struct Y { char pad[0x10]; void *p10; char pad2[0x0C]; char a20[1]; };
struct X { char pad[0xDC]; struct Y *pDC; };
struct A { char pad[0xE8]; struct X *pE8; };
struct T { char pad[0x28]; unsigned long fl; char pad2[0x1D8 - 0x30]; int n1D8; };
struct S {
    char pad[0xC]; struct VTE *vt; struct A *p10; char pad1[0x30 - 0x14];
    void *p30; float g34; int n38; char pad2[4]; int n40; char pad3[8]; int n4C;
    char pad4[4]; struct T *p54; int n58; float g5C; float g60; char pad5[4]; float g68; float g6C;
    char pad6[0x80 - 0x70]; qword v80;
};
void UpdateBhv(struct S *p);
void StepBhvpathAlongShape(struct S *p);
qword func_001A62E8(struct A *a);
int func_001BD718(qword q, void *a1, void *a2, void *obj, int dir, struct T **pp, float *pf, float g);
void FindShapeClosestPoint(struct T *po, qword q, struct G *pg, void *pv, int b, int c, float *pf);
float func_001E7B38(struct T *p, float a, float b);
static inline float Dist(qword a, qword b) {
    qword d;
    float f;
    __asm__("vsub.xyzw %0, %2, %1" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
static inline void Set(struct S *p, int n) {
    struct VTE *e = &p->vt[15];
    ((void (*)(void *, int))e->pfn)((char *)p + e->d, n);
}
void UpdateBhvpath(struct S *p) {
    float g;
    qword pos;
    int s;
    UpdateBhv(p);
    StepBhvpathAlongShape(p);
    pos = func_001A62E8(p->p10);
    do {
        s = p->n4C;
        switch (s) {
        case 0:
        case 2:
        case 3:
            if (!p->n40) {
                struct X *x = p->p10->pE8;
                struct Y *y = x ? x->pDC : 0;
                int n = func_001BD718(pos, y ? y->p10 : 0, x->pDC->a20, p->p30, p->n38 == 0, &p->p54, &p->g60, p->g34);
                p->n58 = n;
                switch (n) {
                case 0:
                    break;
                case 3:
                case 4:
                    Set(p, 3);
                    break;
                case 1:
                case 2: {
                    FindShapeClosestPoint(p->p54, pos, &D_003DD020, 0, 0, 0, &g);
                    if (80.0f < __builtin_fabsf(func_001E7B38(p->p54, g, p->g60))) {
                        p->g5C = g;
                        Set(p, 1);
                    } else {
                        p->n58 = 0;
                    }
                    break;
                }
                }
            }
            if (p->n58 == 0) {
                float r = p->n4C == 0 ? p->g68 : p->g6C;
                int f = r < Dist(p->v80, pos);
                if (f) Set(p, 0);
                else Set(p, 2);
            }
            break;
        case 1:
        case 4:
            if (!((int)(p->p54->fl >> 60) & 1) || p->p54->n1D8) {
                Set(p, 0);
            } else if (__builtin_fabsf(func_001E7B38(p->p54, p->g5C, p->g60)) < 80.0f) {
                Set(p, 0);
            } else {
                float r = p->n4C == 1 ? p->g68 : p->g6C;
                int f = r < Dist(p->v80, pos);
                if (f) Set(p, 1);
                else Set(p, 4);
            }
            break;
        }
    } while (p->n4C != s);
}
