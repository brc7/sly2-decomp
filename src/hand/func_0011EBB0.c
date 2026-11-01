typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float af[4]; } V;
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
    int n;
    float g;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(n), "=f"(g) : "j"(d));
    return g;
}
static inline float DotS(qword a, qword b)
{
    qword d, one;
    register int n __asm__("$2");
    float g;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(n) : "j"(d));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(n));
    return g;
}
/** @sly2-annot addr=0x11EBB0
 * Purpose: Moller-Trumbore ray/triangle test (det eps 1e-6), outputs t and (u,v)
 * Info: proposed name FIntersectRayTriangle (T1); TU vec
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#vec-0x11e198-0x11f440
 * @end-sly2-annot */
int func_0011EBB0(qword dir, qword orig, qword v0, qword v1, qword v2, float *pt, float *puv)
{
    V e1, e2, pvec, tvec, qvec;
    float det, inv, u, v, t;
    e1.q = Sub(v1, v0);
    e2.q = Sub(v2, v0);
    pvec.q = Cross(dir, e2.q);
    det = Dot(e1.q, pvec.q);
    if (det > -1e-6f && det < 1e-6f)
        return 0;
    inv = 1.0f / det;
    tvec.q = Sub(orig, v0);
    u = DotS(tvec.q, pvec.q) * inv;
    if (u < 0.0f || u > 1.0f)
        return 0;
    qvec.q = Cross(tvec.q, e1.q);
    v = Dot(dir, qvec.q) * inv;
    if (v < 0.0f || u + v > 1.0f)
        return 0;
    t = Dot(e2.q, qvec.q) * inv;
    if (t < 0.0f)
        return 0;
    if (pt)
        *pt = t;
    if (puv) {
        puv[0] = u;
        puv[1] = v;
    }
    return 1;
}
