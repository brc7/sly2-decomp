/* func_0011E630 (Claude agent, lane A23, 2026-10-23), from v6. The u/v dots (DotG) end with one `mtc1` asm from the qword
   so gcc emits the qmfc2 moves itself; the commented barrier sits after ba (not ca). One pin (d). */
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
extern qword D_002A2DC0;
static inline qword Sub(qword a, qword b)
{
    qword r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword Cross(qword a, qword b)
{
    qword r;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline float Dot(qword a, qword b)
{
    qword d, one;
    float f;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
static inline float DotG(qword a, qword b)
{
    qword d, one;
    float f;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    __asm__("mtc1 %1, %0" : "=f"(f) : "r"(d));
    return f;
}
/** @sly2-annot addr=0x11E630
 * Purpose: Closest points between two infinite lines
 * Info: proposed name FindClosestPointBetweenLines (T1); TU vec
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2+verdicts_A1
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#vec-0x11e198-0x11f440
 * @end-sly2-annot */
void func_0011E630(qword a, qword b, qword c, qword pos, float *puv, float *pdist, qword *pnormal)
{
    qword ca, ba, pc, n;
    float lenSq;
    float eps = 1e-4f;
    ca = Sub(c, a);
    ba = Sub(b, a);
    __asm__ volatile(""); /* emulates toolchain order */
    pc = Sub(pos, c);
    n = Cross(ba, pc);
    lenSq = Dot(n, n);
    if (lenSq < eps) {
        puv[0] = puv[1] = 0.0f;
        n = ca;
    } else {
        float u = DotG(Cross(ca, pc), n) / lenSq;
        float v = DotG(Cross(ca, ba), n) / lenSq;
        puv[0] = u;
        puv[1] = v;
        if (Dot(ca, n) < 0.0f) {
            U s;
            float m = -1.0f;
            __asm__("qmtc2.ni %1, %0" : "=j"(s.q) : "r"(m));
            __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(n) : "j"(n), "j"(s.q));
        }
    }
    {
        qword def = D_002A2DC0;
        qword t;
        qword one, dd;
        float l;
        __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(dd), "=&j"(one) : "j"(n));
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(t) : "j"(dd));
        __asm__("qmfc2.ni %0, %1" : "=r"(l) : "j"(t));
        if (l < 1e-4f) {
            t = def;
        } else {
            __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(t) : "j"(t), "j"(n));
        }
        n = t;
    }
    {
        register qword d __asm__("vf1"); /* pin */
        qword one;
        int nd;
        __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(d), "=&j"(one) : "j"(n), "j"(ca));
        __asm__("qmfc2.ni %0, %1" : "=r"(nd) : "j"(d));
        *(int *)pdist = nd;
    }
    *pnormal = n;
}
