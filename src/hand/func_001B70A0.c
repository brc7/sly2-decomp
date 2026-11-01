typedef int qword __attribute__((mode(TI)));
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
struct O { char pad[0x26C]; char x26C[0xFEC - 0x26C]; void *xFEC; };
struct S { char pad[0x50]; int x50; char pad2[0x5C - 0x54]; void *x5C; };
struct P { char pad[0x150]; void *pl; char pad2[0x10DC - 0x154]; struct O *p10DC; char pad3[0x180C - 0x10E0]; int x180C; };
extern float D_002C09AC, D_002C09B8;
extern char D_002C09B0[], D_002C09C0[];
int func_00209A68(struct O *po, int n);
int GrfPressedWithin__5POCTLUif(void *pl, int k, float g);
int GrfConsumePressed__5POCTLUif(void *pl, int k, float g);
struct S *func_001EEEE0(void *p, int n);
qword func_001A62E8(void *p);
int func_0011D910(void *p, float g);
void func_0017B1A8(struct S *p, int n);
void func_001B7050(struct P *p, void *q);
void func_001B6078(struct P *p, int n);
void func_001B86B0(struct P *p, int n);
static inline float LenXY(qword v) {
    qword d;
    float f;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetZ2(qword t) { __asm__("pextuw %0,$0,%0" : "+r"(t)); return GetX(t); }
static inline qword ToGpr(qword v) { qword r; __asm__("qmfc2.ni %0, %1" : "=r"(r) : "j"(v)); return r; }
int func_001B70A0(struct P *p)
{
    union { qword q; float f[4]; } u;
    struct S *ps;
    if (!p->p10DC)
        return 0;
    if (!func_00209A68(p->p10DC, 0x17) || !GrfPressedWithin__5POCTLUif(p->pl, 0xA0, 0.15f))
        goto ret0;
    ps = func_001EEEE0(p->p10DC->x26C, 0xB1);
    if (ps->x50 >= 3)
        return 0;
    if (ps->x5C != p) {
    ret0:
        return 0;
    }
    {
            int f = 0, g;
            qword a = func_001A62E8(p->p10DC);
            qword d = func_001A62E8(p);
            __asm__("vsub.xyzw %0, %1, %0" : "+j"(d) : "j"(a));
            u.q = d;
            if (LenXY(d) < D_002C09AC && func_0011D910(D_002C09B0, GetZ(d)) && p->p10DC->xFEC)
                f = GrfConsumePressed__5POCTLUif(p->pl, 0x20, 0.15f) != 0;
            if (f) {
                func_0017B1A8(ps, 2);
                func_001B7050(p, p->p10DC->xFEC);
                return 1;
            }
            d = u.q;
            g = 0;
            if (LenXY(d) < D_002C09B8 && func_0011D910(D_002C09C0, GetZ(d)) && p->x180C)
                g = GrfConsumePressed__5POCTLUif(p->pl, 0x80, 0.15f) != 0;
    if (!g)
        goto ret0;
    }
    func_001B6078(p, 0xB);
    func_001B86B0(p, 4);
    return 1;
}
