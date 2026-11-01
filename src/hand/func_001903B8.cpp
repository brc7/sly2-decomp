typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
union U { qword q; float f[4]; };
struct PP {
    char pad[0x1A8]; float x1A8;
    char pad1[0x18C4 - 0x1AC]; void *x18C4;
    char pad2[0x1910 - 0x18C8]; qword x1910;
};
extern "C" {
qword func_001A62E8(void *p);
void func_001FAC30(PP *p, qword q);
void func_001F59A0(PP *p, M3 *pm, float *pg);
void func_001903B8(PP *p);
}
M3 BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(qword a, qword b);
M3 func_0011B500(qword a, float g);
extern qword D_002A2DC0;
extern float D_002BF548[];
static inline qword Sub(qword a, qword b)
{
    qword r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline M3 Mul(const M3 &a, const M3 &b) return r
{
    __asm__("vmulax.xyz ACC, %1, %4x\n\tvmadday.xyz ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
        : "=j"(r.r[0]) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(b.r[0]));
    __asm__("vmulax.xyz ACC, %1, %4x\n\tvmadday.xyz ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
        : "=j"(r.r[1]) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(b.r[1]));
    __asm__("vmulax.xyz ACC, %1, %4x\n\tvmadday.xyz ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
        : "=j"(r.r[2]) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(b.r[2]));
}
void func_001903B8(PP *p)
{
    U v, d;
    M3 m1, m2, m;
    func_001FAC30(p, p->x1910);
    qword a;
    __asm__("qmtc2.ni %1, %0" : "=j"(a) : "r"(func_001A62E8(p->x18C4)));
    v.q = a;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(a) : "j"(a), "j"(func_001A62E8(p)));
    d.q = a;
    m1 = BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(D_002A2DC0, a);
    m2 = func_0011B500(D_002A2DC0, p->x1A8);
    m = Mul(m1, m2);
    func_001F59A0(p, &m, D_002BF548);
}
