// ClosestPointOnCgt: closest point of a navigation triangle to p (wave-3 lane 4). T holds pointers to the three corners.
// If p lies outside an edge in the XY plane (z of cross(p - v0, v1 - v0) > 0, tested a-b, b-c, c-a in turn), the result
// is the closest point on that edge (FindClosestPointOnLineSegment); otherwise p is projected along D_002A2DC0 (world up) onto the
// triangle's plane.
// Keys:
//  * The inside block works on VU_VECTOR copies of the first and third corners made right after the loads. The
//    const-reference conversion constructor makes them struct pass-through copies that cse1 and gcse's copy propagation
//    leave alone, so they are live together with a/c through the edge tests and get their own VU registers: the
//    original's `vmove vf8,vf6` / `vmove vf7,vf3` (plain `qword aa = a` copies are always propagated away).
//  * The plane normal's z is read with pextuw on an "r" input and the dot product with mtc1 on an "r" input, so gcc
//    emits the qmfc2 moves itself as reloads. Then the inside block needs only the v0/v1 spill registers, which keeps
//    reload's round-robin choice for the three edge tests at v1, v0, v1, and lets sched2 put the mtc1 before the lqc2.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
struct T { qword *a, *b, *c; };
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
extern "C" {
extern qword D_002A2DC0; // (0, 0, 1, 0)
void FindClosestPointOnLineSegment(qword pos, qword p0, qword p1, qword *ppos, float *pt, float *pdist);
qword ClosestPointOnCgt(T *t, qword p);
}
static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Cross(qword a, qword b) { qword r; __asm__("vopmula.xyz ACC,%1,%2\n\tvopmsub.xyz %0,%2,%1" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline float Z(qword a) { float z; __asm__("qmfc2.ni %0,%1\n\tpextuw %0,$zero,%0" : "=r"(z) : "j"(a)); return z; }
static inline float ZOf(qword q) { float z; __asm__("pextuw %0,$zero,%1" : "=r"(z) : "r"(q)); return z; }
static inline float XOf(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static inline qword Dot(qword a, qword b)
{
    qword d = a, one;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "0"(d), "j"(b));
    return d;
}
static inline qword AddScaled(qword a, qword b, float g)
{
    U u; qword s, r;
    __asm__("qmtc2.ni %1,%0" : "=j"(s) : "r"(g));
    u.q = s;
    __asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x" : "=j"(r) : "j"(a), "j"(b), "j"(u.q));
    return r;
}

/** @sly2-annot addr=0x1BFDB0
 * Purpose: Closest point of a navigation triangle to p (edge clamp or vertical projection)
 * Info: proposed name ClosestPointOnCgt (T1); TU path
 * Notes: names_v2 v1 proposed InitJt__FP2JT (0.000)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
qword ClosestPointOnCgt(T *t, qword p)
{
    U result;
    qword a = *t->a, b = *t->b, c = *t->c;
    qword ab = Sub(b, a);
    VU_VECTOR va(a), vc(c);
    if (0.0f < Z(Cross(Sub(p, a), ab))) FindClosestPointOnLineSegment(p, a, b, &result.q, 0, 0);
    else if (0.0f < Z(Cross(Sub(p, b), Sub(c, b)))) FindClosestPointOnLineSegment(p, b, c, &result.q, 0, 0);
    else if (0.0f < Z(Cross(Sub(p, c), Sub(a, c)))) FindClosestPointOnLineSegment(p, c, a, &result.q, 0, 0);
    else {
        qword n = Cross(ab, Sub(vc.q, va.q));
        qword dv = Dot(Sub(va.q, p), n);
        float nz = ZOf(n);
        float dot = XOf(dv);
        float g = dot / nz;
        result.q = AddScaled(p, D_002A2DC0, g);
    }
    return result.q;
}
