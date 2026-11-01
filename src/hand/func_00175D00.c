typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct G;
extern struct G D_003DD490;
struct VTE { short d; short pad; void *pfn; };
struct T {
    char pad[0xC]; struct VTE *vt; char pad2[0x18]; unsigned long fl28; char pad3[0x2DC - 0x30]; int n2DC;
    char pad4[0x4C8 - 0x2E0]; int n4C8; int n4CC; char pad5[0xF54 - 0x4D0]; int nF54; char pad6[0x18C8 - 0xF58]; int n18C8;
};
struct A { char pad[0xEC]; int nEC; char pad2[0xF00 - 0xF0]; int nF00; char pad3[0xF48 - 0xF04]; char aF48[8]; char aF50[8]; char aF58[8]; char pad4[0xFE8 - 0xF60]; int nFE8; };
struct S { char pad[0x10]; struct A *p10; char pad2[0x1C]; struct T *p30; int n34; float f38; int n3C; };
struct X { int a[4]; };
struct Y { char pad[0x60]; };
struct R2 { struct T *p0; };
struct R1 { char pad[0x14]; struct R2 *p14; };
struct R { int pad; struct R1 *p4; };
union V { qword q; };
int func_00132B10(struct T *t, int n);
qword func_001A62E8(void *p);
float func_00204740(struct A *a);
void GetVectorSphere(qword v, float *a, float *b, float *c);
int func_0011D910(void *p, float f);
float RadNormalize__Ff(float f);
qword *func_001A6318(struct A *a);
void CalculateVectorPanTilt(qword v, int n, float *pf);
void func_001AE478(struct X *x);
void func_001AE4A0(struct X *x, int n);
int func_0020FA10(void);
void func_0020FAA0(struct G *g, qword a, qword b, int (*fn)(void), int z, int *pa, int *pb);
struct R *func_0014CC58(int n, qword a, qword b, int c, int d, struct Y *y);
int func_00175D00(struct S *p) {
    struct X x;
    struct Y y;
    float f[4];
    int na, nb;
    qword pa, pb;
    qword d;
    float g;
    int k;
    struct T *t;
    struct A *a = p->p10;
    if (!(a->nFE8 & 8) || a->nF00 || p->n3C || !p->p30 || !((int)(p->p30->fl28 >> 60) & 1)) return 0;
    if (func_00132B10(p->p30, 0x32)) {
        struct T *q = p->p30;
        struct VTE *e = &q->vt[0x4D];
        if (((int (*)(void *))e->pfn)((char *)q + e->d) == 2) return 0;
    }
    k = 0;
    if (func_00132B10(p->p30, 0x34)) {
        t = p->p30;
        if (t->n4CC != -1) {
            if (t->n4C8 < 5) k = t->n4C8 >= 3;
        }
        if (t->n2DC != -1) {
            if (t->n2DC == 0x11) k = 1;
        }
        if (t->nF54 != -1) k = 1;
        if (func_00132B10(t, 0x35)) {
            switch (t->n18C8) {
            case 3: case 0x15: case 0x16:
                k = 1;
                break;
            }
        }
    }
    pa = func_001A62E8(p->p10);
    pb = func_001A62E8(p->p30);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pb), "j"(pa));
    g = func_00204740(p->p10);
    GetVectorSphere(d, &f[0], &f[1], &f[2]);
    if (!func_0011D910(p->p10->aF48, f[2])) return 0;
    {
        float h = RadNormalize__Ff(f[0] - g);
        if (!func_0011D910(p->p10->aF50, h)) return 0;
    }
    CalculateVectorPanTilt(*func_001A6318(p->p10), 0, &f[3]);
    {
        float h = RadNormalize__Ff(f[1] - f[3]);
        if (!func_0011D910(p->p10->aF58, h)) return 0;
    }
    if (!k) {
        if (p->n34 == 0) {
            if (D_002DDED0.t - p->f38 < 3.0f) return 0;
        } else if (p->n34 != 2) {
            return 0;
        }
    }
    {
        struct R *r;
        func_001AE478(&x);
        func_0020FAA0(&D_003DD490, pa, pb, func_0020FA10, p->p10->nEC, &na, &nb);
        r = func_0014CC58(0, pa, pb, na, nb, &y);
        if (r && r->p4->p14->p0 != p->p30) {
            func_001AE4A0(&x, 2);
            return 0;
        }
        func_001AE4A0(&x, 2);
        return 1;
    }
}
