typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern qword D_002A2DD0;
extern float D_002C09F0;
struct VT { short d; short pad; void *pfn; };
struct OBJ { struct VT *vt; };
struct R { int x0; struct OBJ *po; };
struct S14 { char pad[0x74]; float g74; };
struct ANIM { char pad[0x14]; struct S14 *p14; char pad2[0x40 - 0x18]; float g40; };
struct PL { char pad[0x30]; int x30; };
struct P {
    char pad0[0x150]; struct PL *pl; char pad1[0x190 - 0x154]; qword v190; char pad1b[0x1A8 - 0x1A0]; float g1A8; char pad2[0x1B0 - 0x1AC];
    qword m1B0[3]; char pad3[0x1030 - 0x1E0]; struct ANIM *panim; char x1034[0x10DC - 0x1034]; void *p10DC; char pad5[0x10E4 - 0x10E0];
    int x10E4; char pad6[0x17FC - 0x10E8]; void *ap17FC[5]; int x1810; float g1814; int x1818; int x181C; int x1820;
    char pad7[0x1830 - 0x1824]; qword v1830; char pad8[0x190C - 0x1840]; void *x190C;
};
void func_00130658(void *pp);
qword func_001A62E8(void *p);
float func_002458F0(float y, float x);
float RadNormalize__Ff(float g);
void func_00204660(struct P *p, float g);
void func_00204CA8(struct P *p);
void func_001F15B8(void *p);
void func_001305B8(void *q, struct P *p, float g, float h, int n, struct ANIM **ppanim);
float *func_0012F938(void *q, int n);
void func_00131A90(struct ANIM *pa, float t);
struct R *func_0012FF10(struct S14 *p, struct P *pp, int a, int b);
void func_001431D0(struct P *p);
static inline qword MulM3(qword *m, qword v)
{
    __asm__("vmulax.xyz $ACC, %1, %0x\n\tvmadday.xyz $ACC, %2, %0y\n\tvmaddz.xyz %0, %3, %0z" : "+j"(v) : "j"(m[0]), "j"(m[1]), "j"(m[2]));
    return v;
}
void func_001B86B0(struct P *p, int k)
{
    void *q;
    float g;
    if (k == p->x1810)
        return;
    p->x1810 = k;
    p->g1814 = D_002DDED0.t;
    if (k == -1)
        return;
    p->x1820 = 0;
    p->x1818 = 0;
    p->x181C = 0;
    if (p->x190C)
        func_00130658(&p->x190C);
    q = p->ap17FC[k];
    p->v190 = D_002A2DD0;
    if (p->p10DC && p->x10E4 >= 2 && !(p->pl->x30 & 8)) {
        qword d;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(func_001A62E8(p->p10DC)), "j"(func_001A62E8(p)));
        g = func_002458F0(GetY(d), GetX(d));
        if (__builtin_fabsf(RadNormalize__Ff(g - p->g1A8)) < D_002C09F0) {
            func_00204660(p, g);
            func_00204CA8(p);
        }
    }
    g = 0.0f;
    func_001F15B8(p->x1034);
    func_001305B8(q, p, g, 1.0f, 1, &p->panim);
    if (q) {
        float *pg = func_0012F938(q, 0x1AC);
        if (pg)
            g = *pg;
    }
    if (g != 0.0f)
        func_00131A90(p->panim, D_002DDED0.t + g / p->panim->g40);
    if (p->panim) {
        struct R *r = func_0012FF10(p->panim->p14, p, 1, 1);
        if (r) {
            qword a, b, pos, t, v;
            ((void (*)(void *, qword *, float, float, int))r->po->vt[3].pfn)((char *)r->po + r->po->vt[3].d, &a, 0.0f, 0.0f, 0);
            ((void (*)(void *, qword *, float, float, int))r->po->vt[3].pfn)((char *)r->po + r->po->vt[3].d, &b, p->panim->p14->g74, 0.0f, 0);
            pos = func_001A62E8(p);
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(t) : "j"(b), "j"(a));
            t = MulM3(p->m1B0, t);
            __asm__("vadd.xyzw %0, %1, %2" : "=j"(v) : "j"(pos), "j"(t));
            p->v1830 = v;
        } else {
            p->v1830 = func_001A62E8(p);
        }
    }
    func_001431D0(p);
}
