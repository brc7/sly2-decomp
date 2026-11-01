typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
typedef struct { qword r[3]; } M3;
struct X { char pad[0x14]; void *p14; char pad1[0xA4 - 0x18]; float gA4; float gA8; };
extern qword D_002A5000;
qword func_001A62E8(void *p);
qword func_00150780(struct X *p);
float func_002458F0(float y, float x);
void func_0011B720(M3 *pm, float a, float b);
static inline qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline qword Add(qword a, qword b) { qword d; __asm__("vadd.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline qword Len(qword v)
{
    qword d, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(v), "j"(v));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(d) : "j"(d));
    return d;
}
static inline qword Normalize(qword v)
{
    qword l = Len(v), r;
    __asm__("vdiv Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, Q" : "=j"(r) : "j"(l), "j"(v));
    return r;
}
static inline float GFromQ(qword v) { float g; __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(v)); return g; }
static inline qword VecMul(qword v, float g)
{
    qword r;
    U s;
    __asm__("qmtc2.ni %1, %0" : "=j"(s.q) : "r"(g));
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(s.q));
    return r;
}
static inline qword MulM3(M3 *pm, qword v)
{
    qword r;
    __asm__("vmulax.xyz ACC, %2, %1x\n\tvmadday.xyz ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z" : "=j"(r) : "j"(v), "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]));
    return r;
}
/** @sly2-annot addr=0x1507F8
 * Purpose: Eye: if ploFocus(+0x14), behind center looking at focus (updates radPan +0xA4); else center +
 *   Rot(radPan,radTilt)*offset
 * Info: proposed name PosCpzoomEye (T1); TU cplcy
 * Confidence: mid (draft) [T1 0.50] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
qword func_001507F8(struct X *p)
{
    qword r;
    if (p->p14) {
        U n;
        qword a = func_001A62E8(p->p14);
        n.q = Normalize(Sub(a, func_00150780(p)));
        p->gA4 = func_002458F0(n.f[1], n.f[0]);
        r = Add(func_00150780(p), VecMul(n.q, -GFromQ(Len(D_002A5000))));
    } else {
        M3 m;
        qword v = D_002A5000;
        func_0011B720(&m, p->gA4, p->gA8);
        r = Add(func_00150780(p), MulM3(&m, v));
    }
    return r;
}
