// vec.cpp -- vector math on VU0 quadwords: building vectors from cylindrical/spherical coordinates and back, normals,
// angles between vectors, closest points between lines, segments and points, a ray/triangle test, length limits,
// PosSmooth, and componentwise min/max over arrays. VU0/EE asm throughout.
// Vectors are VU0 quadwords (x, y, z, w; z up); angles are radians. The closest-point routines return line
// parameters (0..1 along a segment) and optional distances/normals through out-pointers, so callers pass null for
// what they do not need. Entry points: the functions below, used by physics, camera, AI and Splice builtins.
// TU 0x11E198..0x11F440 (22 functions, address order).
// Each function's private asm helpers are kept as matched, in a namespace named after the function: several have the
// same name with different asm (operand order, allocation-only operands, clobbers), which matters to the match.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_vec
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_vec
#include "sly2/basic.h"
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/scrap.h"

// 0x11F2E8 StaticInit_vec and GlobalConstructors_vec: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* The quadword these files use (int TImode; sly2/basic.h, needed by PosSmooth, has an unsigned one). */
typedef int qwordi __attribute__((mode(TI)));
extern "C" {
extern qwordi D_002A2DA0;
extern qwordi g_normalZ;
extern qwordi D_002A2DD0;
extern qwordi D_002A2DF0;
extern qwordi D_002A2E00;
float NormalizeAngle__Ff(float g);
float acosf(float g);
float atan2f(float y, float x);
float fabsf(float);
qwordi GetNormalVector(qwordi v);
void CalculateSinCos__FfPfT1(float g, float *ps, float *pc);
void FindClosestPointBetweenLines(qwordi a, qwordi b, qwordi c, qwordi pos, float *puv, float *pdist, qwordi *pnormal);
}
float SmoothValue(float gCur, float gTarget, float dt, SMP *psmp, float *pdgNext);
//////// 0x11E1A0 SetVectorCylind
namespace V_SetVectorCylind {
typedef int qword __attribute__((mode(TI)));
// Builds a vector from cylindrical coordinates: angle `rad` around z, `radius` in the xy plane, height z.
// Also known as: SetVectorCylind (upstream readable / Sly 1 name; SetVectorCylind__Ffff).
extern "C" qword SetVectorCylind(float rad, float radius, float z)
{
    union { qword q; float f[4]; } v; float gA, gB; CalculateSinCos__FfPfT1(rad, &gA, &gB); v.q = QwordFromFloats<qword>(gB * radius, gA * radius, z); return v.q;
}
}

//////// 0x11E208 SetVectorSphere
namespace V_SetVectorSphere {
typedef int qword __attribute__((mode(TI)));
// Builds a vector from spherical coordinates: pan around z, tilt above the xy plane, length r.
// Also known as: SetVectorSphere (upstream readable / Sly 1 name; SetVectorSphere__Ffff).
extern "C" qword SetVectorSphere(float radPan, float radTilt, float r)
{
    union { qword q; float f[4]; } v;
    float sp, cp, st, ct;
    CalculateSinCos__FfPfT1(radPan, &sp, &cp);
    CalculateSinCos__FfPfT1(radTilt, &st, &ct);
    v.q = QwordFromFloats<qword>(ct * cp * r, ct * sp * r, st * r);
    return v.q;
}
}

//////// 0x11E290 SplitVectorAlongAxis
namespace V_SplitVectorAlongAxis {
typedef int qword __attribute__((mode(TI)));
// Splits a along the unit vector b: returns a . b, and optionally the parallel part (a . b) b and the
// perpendicular part a - (a . b) b.
// Also known as: GSplitVector (upstream readable / Sly 1 name; GSplitVector__FG9VU_VECTORT0P6VECTORT2).
extern "C" float SplitVectorAlongAxis(qword a, qword b, qword *outParallel, qword *outPerpendicular)
{
    qword d, p;
    float g;
    d = VuDot(a, b);
#ifdef SLY2_EE_ASM
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(p) : "j"(b), "j"(d));
#else
    p = VuScaleByX(b, d);
#endif
    if (outParallel) {
#ifdef SLY2_EE_ASM
        __asm__("sq %1, %0" : "=m"(*outParallel) : "r"(p));
#else
        *outParallel = p;
#endif
    }
    if (outPerpendicular) {
        qword r;
        r = VuSub(a, p);
        *outPerpendicular = r;
    }
#ifdef SLY2_EE_ASM
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(d));
#else
    VuCopyBits(g, d);
#endif
    return g;
}
}

//////// 0x11E2D8 GetNormalVectors
namespace V_GetNormalVectors {
/* GetNormalVectors(normal, a, b, M3 rows *pm): builds basis rows 0/1. VF registers pinned with hard-register locals (Claude agent). */
typedef int qword __attribute__((mode(TI)));
#ifdef SLY2_EE_ASM
#define CROSS(r, a, b) __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b))
#define DOT(d, one, v) __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=j"(one), "=j"(d) : "j"(v))
#else
#define CROSS(r, a, b) ((r) = VuCross((a), (b)))
#define DOT(d, one, v) ((d) = VuDot((v), (v)))
#endif
// Builds two unit vectors perpendicular to `a` (outBasis[0] = normalize(b x a), or c x a when b is parallel to a;
// outBasis[1] = normalize(a x outBasis[0])): a tangent frame around a normal.
// Also known as: BuildOrthonormalVectors (upstream readable / Sly 1 name;
//   BuildOrthonormalVectors__FG9VU_VECTORN20P6VECTOR).
extern "C" void GetNormalVectors(qword a, qword b, qword c, qword *outBasis)
{
    register qword vb __asm__("vf1") = b;
    register qword va __asm__("vf3") = a;
    register qword vc __asm__("vf4");
    register qword x __asm__("vf6");
    CROSS(x, vb, va);
    vc = c;
    {
        register qword d __asm__("vf1");
        register qword one __asm__("vf2");
        DOT(d, one, x);
        if (QwordX(d) < 1.0e-4f)
            CROSS(x, vc, va);
    }
    {
        register qword y __asm__("vf1");
        register qword d0 __asm__("vf2");
        register qword one0 __asm__("vf4");
        register qword d1 __asm__("vf3");
        register qword one1 __asm__("vf5");
        register qword r0 __asm__("vf2");
        register qword r1 __asm__("vf3");
        CROSS(y, va, x);
        DOT(d0, one0, x);
        DOT(d1, one1, y);
#ifdef SLY2_EE_ASM
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d0) : "j"(d0));
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d1) : "j"(d1));
        __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r0) : "j"(d0), "j"(x));
        __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r1) : "j"(d1), "j"(y));
#else
        d0 = VuSqrtX(d0);
        d1 = VuSqrtX(d1);
        r0 = VuDivByX(x, d0);
        r1 = VuDivByX(y, d1);
#endif
        outBasis[0] = r0;
        outBasis[1] = r1;
    }
}
}

//////// 0x11E390 GetNormalVector
namespace V_GetNormalVector {
// GetNormalVector MATCH (Claude agent, lane A10, 2026-10-23), from v3: y/z extracts are asm prot3w/pextuw into a plain int, then a
// union int->float so gcc emits the mtc1 itself (its latency gives the target schedule). x stays an asm mtc1 from the qword. One pin (|y| in $f0).
typedef int qword __attribute__((mode(TI)));
// y lane of q as a float.
#ifdef SLY2_EE_ASM
static __inline__ float GetY(qword q) { qword t; float y; __asm__("prot3w %0,%1" : "=r"(t) : "r"(q)); __asm__("mtc1 %1,%0" : "=f"(y) : "r"(t)); return y; }
#else
static __inline__ float GetY(qword q) { qword t; float y; VuCopyBits(t, VuProt3w(q)); VuCopyBits(y, t); return y; }
#endif
// z lane of q as a float.
#ifdef SLY2_EE_ASM
static __inline__ float GetZ(qword q) { qword t; float z; __asm__("pextuw %0,$0,%1" : "=r"(t) : "r"(q)); __asm__("mtc1 %1,%0" : "=f"(z) : "r"(t)); return z; }
#else
static __inline__ float GetZ(qword q) { qword t; float z; VuCopyBits(t, VuPextuw0(q)); VuCopyBits(z, t); return z; }
#endif
// Unit vector along v.
static inline qword Normalize(qword v)
{
    qword d, l, r;
    d = VuDot(v, v);
#ifdef SLY2_EE_ASM
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l), "j"(v));
#else
    l = VuSqrtX(d);
    r = VuDivByX(v, l);
#endif
    return r;
}
// Returns some unit vector perpendicular to v (crosses v with the axis of its smallest component).
// Also known as: PerpendicularVector (upstream readable / Sly 1 name; PerpendicularVector__FG9VU_VECTOR).
extern "C" qword GetNormalVector(qword v)
{
    float x, y, z;
    float ax;
    qword w; QWROW u;
    { int ci; union { int i; float f; } c;
#ifdef SLY2_EE_ASM
    __asm__("prot3w %0,%1" : "=r"(ci) : "r"(v));
#else
    VuCopyBits(ci, VuProt3w(v));
#endif
    c.i = ci;
#ifdef SLY2_EE_ASM
    __asm__("mtc1 %1,%0" : "=f"(x) : "r"(v));
#else
    VuCopyBits(x, v);
#endif
    y = c.f;
#ifdef SLY2_EE_ASM
    __asm__("pextuw %0,$0,%1" : "=r"(ci) : "r"(v));
#else
    VuCopyBits(ci, VuPextuw0(v));
#endif
    c.i = ci;
    z = c.f; }
    ax = fabsf(x);


    {
    register float ay __asm__("$f0") = fabsf(y); /* pin */
    float az = fabsf(z);
        if (ax <= ay) {
        if (ax <= az)
            w = u.q = QwordFromFloats<qword>(0.0f, z, -y);
        else
            w = u.q = QwordFromFloats<qword>(y, -x, 0.0f);
    } else {
        if (ay <= az)
            w = u.q = QwordFromFloats<qword>(z, 0.0f, -x);
        else
            w = u.q = QwordFromFloats<qword>(y, -x, 0.0f);
    }
    }
    return Normalize(VuCross(v, w));
}
}

//////// 0x11E4B0 AngleBetweenVectors
namespace V_AngleBetweenVectors {
/* AngleBetweenVectors (Claude agent): angle between vectors + optional normalized axis. Length's float comes from an mtc1 asm fed by gcc's own qmfc2 ("r"(qword)), and the axis length vector (pinned vf1) is passed as a GPR qword to the qmtc2, so the move is shared like the original. */
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
// Length of the xyz part of v as a float.
static inline float Length(qword v)
{
    qword d;
    float f;
    d = VuDot(v, v);
#ifdef SLY2_EE_ASM
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "+j"(d));
    __asm__("mtc1 %1, %0" : "=f"(f) : "r"(d));
#else
    d = VuSqrtX(d);
    VuCopyBits(f, d);
#endif
    return f;
}
// Dot product of the xyz parts of a and b as a float.
static inline float Dot(qword a, qword b)
{
    qword d;
    float f;
    d = VuDot(a, b);
    f = VuToFloat(d);
    return f;
}
// Length of the xyz part of v (x lane).
static inline qword LenV(qword v)
{
    register qword d __asm__("vf1");
    d = VuDot(v, v);
#ifdef SLY2_EE_ASM
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "+j"(d));
#else
    d = VuSqrtX(d);
#endif
    return d;
}
// Return v scaled by g.x.
static inline qword Scale(qword v, qword g)
{
    U u;
    qword r;
#ifdef SLY2_EE_ASM
    { qword u_q; __asm__("qmtc2.ni %1, %0" : "=j"(u_q) : "r"(g)); u.q = u_q; }   /* asm output into a scalar, then the union (see the merge guide) */
    __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(r) : "j"(v), "j"(u.q));
#else
    { qword u_q; u_q = VuWiden<__typeof__(u_q)>(g); u.q = u_q; }   /* asm output into a scalar, then the union (see the merge guide) */
    r = VuDivByX(v, u.q);
#endif
    return r;
}
// Returns the angle between a and b (acos of the normalised dot product) and optionally the unit rotation axis
// a x b.
// Also known as: RadBetweenVectors (upstream readable / Sly 1 name; RadBetweenVectors__FG9VU_VECTORT0P6VECTOR).
extern "C" float AngleBetweenVectors(qword a, qword b, qword *outAxis)
{
    float la = Length(a);
    float lb = Length(b);
    if (la < 1e-4f || lb < 1e-4f) {
        if (outAxis)
            *outAxis = D_002A2DA0;
        return 0.0f;
    }
    if (outAxis) {
        qword axis = VuCross(a, b);
        qword dl = LenV(axis);
        float l;
#ifdef SLY2_EE_ASM
        __asm__("mtc1 %1, %0" : "=f"(l) : "r"(dl));
#else
        VuCopyBits(l, dl);
#endif
        if (l > 1e-8f)
            axis = Scale(axis, dl);
        else
            axis = GetNormalVector(a);
        *outAxis = axis;
    }
    return acosf(Dot(a, b) / (la * lb));
}
}

//////// 0x11E630 FindClosestPointBetweenLines
namespace V_FindClosestPointBetweenLines {
/* FindClosestPointBetweenLines (Claude agent, lane A23, 2026-10-23), from v6. The u/v dots (DotG) end with one `mtc1` asm from the qword
   so gcc emits the qmfc2 moves itself; the commented barrier sits after ba (not ca). One pin (d). */
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
// Dot product of the xyz parts of a and b as a float.
static inline float Dot(qword a, qword b)
{
    qword d;
    float f;
    d = VuDot(a, b);
    f = VuToFloat(d);
    return f;
}
// Dot product of the xyz parts of a and b as a float (GPR read).
static inline float DotG(qword a, qword b)
{
    qword d;
    float f;
    d = VuDot(a, b);
#ifdef SLY2_EE_ASM
    __asm__("mtc1 %1, %0" : "=f"(f) : "r"(d));
#else
    VuCopyBits(f, d);
#endif
    return f;
}
// Closest points between the infinite lines through a, b and through c, pos: writes the two line parameters, the
// distance between the lines (along the normal) and the unit common normal, oriented from the first line toward
// the second. Parallel lines give parameters 0 and the normal of c - a. All three outputs are required.
// Also known as: ClosestLineParams (upstream readable / Sly 1 name; ClosestLineParams__FG9VU_VECTORN30P6VECTORN24).
extern "C" void FindClosestPointBetweenLines(qword a, qword b, qword c, qword pos, float *outParams, float *outDistance, qword *outNormal)
{
    qword ca, ba, pc, n;
    float lenSq;
    float eps = 1e-4f;
    ca = VuSub(c, a);
    ba = VuSub(b, a);
    __asm__ volatile(""); /* emulates toolchain order */
    pc = VuSub(pos, c);
    n = VuCross(ba, pc);
    lenSq = Dot(n, n);
    if (lenSq < eps) {
        outParams[0] = outParams[1] = 0.0f;
        n = ca;
    } else {
        float u = DotG(VuCross(ca, pc), n) / lenSq;
        float v = DotG(VuCross(ca, ba), n) / lenSq;
        outParams[0] = u;
        outParams[1] = v;
        if (Dot(ca, n) < 0.0f) {
            U s;
            float m = -1.0f;
            { qword s_q; s_q = VuFromFloat<qword>(m); s.q = s_q; }   /* asm output into a scalar, then the union (see the merge guide) */
#ifdef SLY2_EE_ASM
            __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(n) : "j"(n), "j"(s.q));
#else
            n = VuScaleByX(n, s.q);
#endif
        }
    }
    {
        qword def = g_normalZ;
        qword t;
        qword dd;
        float l;
        dd = VuDot(n, n);
#ifdef SLY2_EE_ASM
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(t) : "j"(dd));
#else
        t = VuSqrtX(dd);
#endif
        l = VuToFloat(t);
        if (l < 1e-4f) {
            t = def;
        } else {
#ifdef SLY2_EE_ASM
            __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(t) : "j"(t), "j"(n));
#else
            t = VuDivByX(n, t);
#endif
        }
        n = t;
    }
    {
        register qword d __asm__("vf1"); /* pin */
        int nd;
        d = VuDot(n, ca);
#ifdef SLY2_EE_ASM
        __asm__("qmfc2.ni %0, %1" : "=r"(nd) : "j"(d));
#else
        VuCopyBits(nd, d);
#endif
        *(int *)outDistance = nd;
    }
    *outNormal = n;
}
}

//////// 0x11E7C8 FindClosestPointBetweenLineSegments
namespace V_FindClosestPointBetweenLineSegments {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
// Broadcast g into a VU0 register.
static inline qword VF(float g)
{
    U s;
    { qword s_q; s_q = VuFromFloat<qword>(g); s.q = s_q; }   /* asm output into a scalar, then the union (see the merge guide) */
    return s.q;
}
// Length of v (x lane).
static inline qword Len(qword v)
{
    qword d, l;
    d = VuDot(v, v);
#ifdef SLY2_EE_ASM
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
#else
    l = VuSqrtX(d);
#endif
    return l;
}
// Closest points between two segments (the line solution clamped to the segments); outputs as for
// FindClosestPointBetweenLines.
// Also known as: DistSegments (upstream readable / Sly 1 name; DistSegments__FG9VU_VECTORN30P6VECTORN24).
extern "C" void FindClosestPointBetweenLineSegments(qword a, qword b, qword c, qword pos, float *outParams, float *outDistance, qword *outNormal)
{
    float v;
    qword w0, w1, w2, w3, n, l;
    qword *pdef;
    FindClosestPointBetweenLines(a, b, c, pos, outParams, outDistance, outNormal);
    outParams[0] = Clamp(outParams[0], 0.0f, 1.0f);
    v = Clamp(outParams[1], 0.0f, 1.0f);
    outParams[1] = v;
    w0 = VF(outParams[0] - 1.0f);
    w1 = VF(-outParams[0]);
    w2 = VF(1.0f - v);
    w3 = VF(v);
#ifdef SLY2_EE_ASM
    __asm__("vmulax.xyzw $ACC, %5, %1x\n\tvmaddax.xyzw $ACC, %6, %2x\n\tvmaddax.xyzw $ACC, %7, %3x\n\tvmaddx.xyzw %0, %8, %4x"
            : "=j"(n) : "j"(w0), "j"(w1), "j"(w2), "j"(w3), "j"(a), "j"(b), "j"(c), "j"(pos));
#else
    /* n = ((a w0 + b w1) + c w2) + pos w3, all four lanes (the closest points' difference) */
    {
        VuLanes pa = VuGet(a), pb = VuGet(b), pcl = VuGet(c), pp = VuGet(pos);
        float k0 = QwordX(w0), k1 = QwordX(w1), k2 = QwordX(w2), k3 = QwordX(w3);
        n = VuMake<qword>(((pa.x * k0 + pb.x * k1) + pcl.x * k2) + pp.x * k3, ((pa.y * k0 + pb.y * k1) + pcl.y * k2) + pp.y * k3,
                          ((pa.z * k0 + pb.z * k1) + pcl.z * k2) + pp.z * k3, ((pa.w * k0 + pb.w * k1) + pcl.w * k2) + pp.w * k3);
    }
#endif
    *outNormal = n;
    pdef = &D_002A2DA0;
    l = Len(n);
    if (QwordX(l) < 1e-4f) {
        *outNormal = *pdef;
    } else {
        qword r;
#ifdef SLY2_EE_ASM
        __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(r) : "j"(n), "j"(l));
#else
        r = VuDivByX(n, l);
#endif
        *outNormal = r;
    }
#ifdef SLY2_EE_ASM
    { int nl; __asm__("qmfc2.ni %0, %1" : "=r"(nl) : "j"(l)); *(int *)outDistance = nl; }
#else
    { int nl; VuCopyBits(nl, l); *(int *)outDistance = nl; }
#endif
}
}

//////// 0x11E958 CalculateVectorPanTilt
namespace V_CalculateVectorPanTilt {
typedef int qword __attribute__((mode(TI)));
// Returns v's pan (atan2(y, x)) and tilt (atan2(z, |xy|)), each normalised to (-pi, pi]; both optional.
// Also known as: PanTiltFromVector (upstream readable / Sly 1 name; PanTiltFromVector__FG9VU_VECTORPfT1).
extern "C" void CalculateVectorPanTilt(qword v, float *outPan, float *outTilt)
{
    if (outPan)
        *outPan = NormalizeAngle__Ff(atan2f(QwordY(v), QwordX(v)));
    if (outTilt)
        *outTilt = NormalizeAngle__Ff(atan2f(QwordZ(v), SqrtFloat(QwordX(v) * QwordX(v) + QwordY(v) * QwordY(v))));
}
}

//////// 0x11E9F8 LimitVectorLength
namespace V_LimitVectorLength {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
// Returns v scaled down to maxLength if it is longer, else v.
// Also known as: LimitVectorLength (upstream readable / Sly 1 name; LimitVectorLength__FG9VU_VECTORf).
extern "C" qword LimitVectorLength(qword v, float maxLength)
{
    qword d, l;
    float g;
    d = VuDot(v, v);
#ifdef SLY2_EE_ASM
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(l));
#else
    l = VuSqrtX(d);
    VuCopyBits(g, l);
#endif
    if (maxLength < g) {
        U s;
        qword r;
        float q;
#ifdef SLY2_EE_ASM
        __asm__ __volatile__("div.s %0, %1, %2" : "=f"(q) : "f"(maxLength), "f"(g));
#else
        q = maxLength / g;
#endif
        { qword s_q; s_q = VuFromFloat<qword>(q); s.q = s_q; }   /* asm output into a scalar, then the union (see the merge guide) */
#ifdef SLY2_EE_ASM
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(s.q));
#else
        r = VuScaleByX(v, s.q);
#endif
        return r;
    }
    return v;
}
}

//////// 0x11EA60 ClosestPointsOfLines
namespace V_ClosestPointsOfLines {
/* ClosestPointsOfLines (Claude agent, lane A28, 2026-10-24): from fresh9 (13). ba via a vsub without the "memory" clobber (qmtc2 b order), and every dot readback via F3, a mtc1 asm with an allocation-only "j"(q) input next to the "r"(q) one, so the dot result stays a VU pseudo and gcc emits the qmfc2 itself. No pins. */
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
// Return a - b.
static inline qword Sub(qword a, qword b)
{
    qword r;
#ifdef SLY2_EE_ASM
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b) : "memory");
#else
    r = VuSub(a, b);
#endif
    return r;
}
// Cross product a x b.
static inline qword Cross(qword a, qword b)
{
    qword r;
#ifdef SLY2_EE_ASM
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b) : "memory");
#else
    r = VuCross(a, b);
#endif
    return r;
}
// Cross product a x b (temporary variant).
static inline qword CrossT(qword a, qword b)
{
#ifdef SLY2_EE_ASM
    __asm__("vopmula.xyz $ACC, %0, %1\n\tvopmsub.xyz %0, %1, %0" : "+j"(a) : "j"(b) : "memory");
#else
    a = VuCross(a, b);
#endif
    return a;
}
// Dot product of a and b (x lane).
static inline qword DotQ(qword a, qword b)
{
    qword d, one;
#ifdef SLY2_EE_ASM
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b) : "memory");
#else
    d = VuDot(a, b);
#endif
    return d;
}
// Dot product of the xyz parts of a and b as a float.
static inline float Dot(qword a, qword b)
{
    int n;
    float g;
#ifdef SLY2_EE_ASM
    __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(n), "=f"(g) : "j"(DotQ(a, b)) : "memory");
#else
    VuCopyBits(n, DotQ(a, b)); VuCopyBits(g, DotQ(a, b));
#endif
    return g;
}
// x lane of q as a float.
static inline float F3(qword q)
{
    float g;
#ifdef SLY2_EE_ASM
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q), "j"(q)); /* "j"(q): allocation-only operand */
#else
    VuCopyBits(g, q);
#endif
    return g;
}
// Dot product of a and b as a float (single asm block).
static inline float DotS(qword a, qword b)
{
    return F3(DotQ(a, b));
}
// Closest points of the infinite lines through a, b and c, d: writes the two line parameters to outParams[0..1]
// and the unit common normal (oriented away from a towards c) to outNormal; returns 0 if the lines are parallel.
// Used for edge/edge contacts (mark.cpp).
// Also known as: FClosestPointsLines (upstream readable / Sly 1 name; FClosestPointsLines__FG9VU_VECTORN30PfP6VECTOR).
extern "C" int ClosestPointsOfLines(qword a, qword b, qword c, qword d, float *outParams, qword *outNormal)
{
    qword ca = Sub(c, a);
    qword ba = VuSub(b, a);
    qword dc = Sub(d, c);
    qword n = Cross(ba, dc);
    qword ca2, n2, c1, lq, c2;
    float lenSq;
    float g, s;
#ifdef SLY2_EE_ASM
    __asm__("vmove.xyzw %0, %1" : "=j"(ca2) : "j"(ca) : "memory");
    __asm__("vmove.xyzw %0, %1" : "=j"(n2) : "j"(n), "j"(ca) : "memory"); /* "j"(ca): allocation-only operand */
#else
    ca2 = ca;
    n2 = n;
#endif
    c1 = Cross(ca2, ba);
    lq = DotQ(n2, n2);
#ifdef SLY2_EE_ASM
    __asm__("vopmula.xyz $ACC, %0, %2\n\tvopmsub.xyz %0, %2, %0" : "=j"(c2) : "0"(ca2), "j"(dc) : "memory");
#else
    c2 = VuCross(ca2, dc);
#endif
    {
        int k;
#ifdef SLY2_EE_ASM
        __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(k), "=f"(lenSq) : "j"(lq) : "memory");
#else
        VuCopyBits(k, lq); VuCopyBits(lenSq, lq);
#endif
    }
    if (lenSq < 1e-6f)
        return 0;
    if (outParams) {
        qword du = VuDot(c2, n2);
        qword dv = VuDot(c1, n2);
        outParams[0] = F3(du) / lenSq;
        outParams[1] = F3(dv) / lenSq;
    }
    g = DotS(ca, n2);
    s = 1.0f;
    if (g < 0.0f)
        s = -1.0f;
    if (outNormal) {
        float l;
        U t;
        qword r;
#ifdef SLY2_EE_ASM
        __asm__("sqrt.s %0, %1" : "=&f"(l) : "f"(lenSq));
#else
        l = SqrtFloat(lenSq);
#endif
        { qword t_q; t_q = VuFromFloat<qword>(s / l); t.q = t_q; }   /* asm output into a scalar, then the union (see the merge guide) */
#ifdef SLY2_EE_ASM
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(n), "j"(t.q));
#else
        r = VuScaleByX(n, t.q);
#endif
        *outNormal = r;
    }
    return 1;
}
}

//////// 0x11EBB0 IntersectRayTriangle
namespace V_IntersectRayTriangle {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float af[4]; } V;
// Dot product of the xyz parts of a and b as a float.
static inline float Dot(qword a, qword b)
{
    qword d;
    int n;
    float g;
    d = VuDot(a, b);
#ifdef SLY2_EE_ASM
    __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(n), "=f"(g) : "j"(d));
#else
    VuCopyBits(n, d); VuCopyBits(g, d);
#endif
    return g;
}
// Dot product of a and b as a float (single asm block).
static inline float DotS(qword a, qword b)
{
    qword d;
    register int n __asm__("$2");
    float g;
    d = VuDot(a, b);
#ifdef SLY2_EE_ASM
    __asm__("qmfc2.ni %0, %1" : "=r"(n) : "j"(d));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(n));
#else
    VuCopyBits(n, d);
    VuCopyBits(g, n);
#endif
    return g;
}
// 0x11EBA0..0x11EBAC: 2 dead-code scraps (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x10");
SLY2_SCRAP("addiu $sp, $sp, 0x10");

// Ray/triangle intersection (Moller-Trumbore, determinant epsilon 1e-6): returns whether the ray from orig along
// dir hits triangle v0 v1 v2, with the ray parameter in *outT and barycentrics in outUv.
extern "C" int IntersectRayTriangle(qword dir, qword orig, qword v0, qword v1, qword v2, float *outT, float *outUv)
{
    V e1, e2, pvec, tvec, qvec;
    float det, inv, u, v, t;
    e1.q = VuSub(v1, v0);
    e2.q = VuSub(v2, v0);
    pvec.q = VuCross(dir, e2.q);
    det = Dot(e1.q, pvec.q);
    if (det > -1e-6f && det < 1e-6f)
        return 0;
    inv = 1.0f / det;
    tvec.q = VuSub(orig, v0);
    u = DotS(tvec.q, pvec.q) * inv;
    if (u < 0.0f || u > 1.0f)
        return 0;
    qvec.q = VuCross(tvec.q, e1.q);
    v = Dot(dir, qvec.q) * inv;
    if (v < 0.0f || u + v > 1.0f)
        return 0;
    t = Dot(e2.q, qvec.q) * inv;
    if (t < 0.0f)
        return 0;
    if (outT)
        *outT = t;
    if (outUv) {
        outUv[0] = u;
        outUv[1] = v;
    }
    return 1;
}
}

//////// 0x11ED40 FindClosestPointOnLine
namespace V_FindClosestPointOnLine {
// FindClosestPointOnLine (Claude agent, lane A22, 2026-10-23): matched from build/hand_wip/FindClosestPointOnLine.v4.c.
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
// Dot product of the xyz parts of a and b as a float.
static inline float Dot(qword a, qword b)
{
    qword d;
    float g;
    d = VuDot(a, b);
    /* "$4" clobber: a0 (pos's argument register) is dead here; scheduling only, it gives pos's qmtc2 one more dependent so it wins the tie */
#ifdef SLY2_EE_ASM
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(d) : "$4");
#else
    VuCopyBits(g, d);
#endif
    return g;
}
// 0x11ED38: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x60");

// Closest point to pos on the infinite line through p0 and p1: optional point, parameter (unclamped) and distance.
// Also known as: ParamSegPoint (upstream readable / Sly 1 name; ParamSegPoint__FG9VU_VECTORN20P6VECTORN23).
extern "C" void FindClosestPointOnLine(qword pos, qword p0, qword p1, qword *outPos, float *outT, float *outDistance)
{
    register qword p0r __asm__("vf5") = p0; /* the one register pin: p0 copy shared by both Subs and the vadda */
    qword d = VuSub(p1, p0r);
    qword d2;
    float n, t;
#ifdef SLY2_EE_ASM
    __asm__("" : "=j"(d2) : "0"(d)); /* empty asm: the d copy (vmove) both dots read */
#else
    d2 = d;
#endif
#ifdef SLY2_EE_ASM
    __asm__("" : "+j"(pos)); /* empty asm, scheduling only: raises the priority of pos's qmtc2 to tie with the d vsub */
#endif
    n = Dot(VuSub(pos, p0r), d2);
    t = n / Dot(d2, d2);
    if (outT)
        *outT = t;
    if (outPos || outDistance) {
        qword s, buf;
        qword c, q;
        s = VuFromFloat<qword>(t);
#ifdef SLY2_EE_ASM
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(c) : "j"(p0r), "j"(d), "j"(s));
        __asm__ __volatile__("sqc2 %1, %0" : "=m"(buf) : "j"(s)); /* dead spill of s, as in the target */
#else
        c = VuAddScaled(p0r, d, s);
        buf = s;
#endif
        if (outPos) {
#ifdef SLY2_EE_ASM
            __asm__("" : "=r"(q) : "0"(c)); /* empty asm: qmfc2 c into a GPR for the store */
#else
            q = c;
#endif
            *outPos = q;
        }
        if (outDistance) {
            qword e, l;
            int g;
            e = VuSub(pos, c);
            e = VuDot(e, e);
#ifdef SLY2_EE_ASM
            __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(e));
            __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
#else
            l = VuSqrtX(e);
            VuCopyBits(g, l);
#endif
            *(int *)outDistance = g;
        }
    }
}
}

//////// 0x11EE08 FindClosestPointOnLineSegment
namespace V_FindClosestPointOnLineSegment {
// FindClosestPointOnLineSegment: closest point on segment p0-p1 to pos (Claude agent). The dead stack copy of the t vector is a
// volatile sqc2 asm (keeps it ahead of beqz); the ppos copy goes vf->gpr via a gcc reload (empty asm with "0") so
// reorg can steal its qmfc2 into the delay slot.
typedef int qword __attribute__((mode(TI)));
// Dot product of the xyz parts of a and b as a float.
static inline float Dot(qword a, qword b)
{
    qword d;
    float g;
    d = VuDot(a, b);
#ifdef SLY2_EE_ASM
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(d));
#else
    VuCopyBits(g, d);
#endif
    return g;
}
/* kept private: the shared GClamp (sly2/util.h) changes FindClosestPointOnLineSegment (60/62 off) */
// Same idiom as sly2/util.h Clamp; kept local because the shared helper changes FindClosestPointOnLineSegment (60/62 instructions off).
static inline float Clamp(float g, float lo, float hi)
{
    if (g < lo) g = lo;
    else if (g > hi) g = hi;
    return g;
}
// Closest point to pos on the segment p0-p1 (parameter clamped to 0..1): optional point, parameter and distance.
// Also known as: PosNearestSegment (upstream readable / Sly 1 name; PosNearestSegment__FG9VU_VECTORN20P6VECTORN23).
extern "C" void FindClosestPointOnLineSegment(qword pos, qword p0, qword p1, qword *outPos, float *outT, float *outDistance)
{
    qword d = VuSub(p1, p0);
    qword e0 = VuSub(pos, p0);
    qword vd = VuDot(d, d);
    qword vn = VuDot(e0, d);
    float t = Clamp(VuToFloat(vn) / VuToFloat(vd), 0.0f, 1.0f);
    if (outT)
        *outT = t;
    if (outPos || outDistance) {
        qword s, buf;
        qword c, q;
        s = VuFromFloat<qword>(t);
#ifdef SLY2_EE_ASM
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(c) : "j"(p0), "j"(d), "j"(s));
        __asm__ __volatile__("sqc2 %1, %0" : "=m"(buf) : "j"(s));
#else
        c = VuAddScaled(p0, d, s);
        buf = s;
#endif
        if (outPos) {
#ifdef SLY2_EE_ASM
            __asm__("" : "=r"(q) : "0"(c)); /* emits nothing: forces gcc movti vf->gpr (qmfc2.ni) */
#else
            q = c;
#endif
            *outPos = q;
        }
        if (outDistance) {
            qword e, l;
            int g;
            e = VuSub(pos, c);
            e = VuDot(e, e);
#ifdef SLY2_EE_ASM
            __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(e));
            __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
#else
            l = VuSqrtX(e);
            VuCopyBits(g, l);
#endif
            *(int *)outDistance = g;
        }
    }
}
}

//////// 0x11EF08 GetVectorCylind
namespace V_GetVectorCylind {
typedef int qword __attribute__((mode(TI)));
// 0x11EF00: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x10");

// Inverse of SetVectorCylind: v's angle around z, xy radius and height (each optional).
// Also known as: ConvertVectorToCylind (upstream readable / Sly 1 name; ConvertVectorToCylind__FG9VU_VECTORPfN21).
extern "C" void GetVectorCylind(qword v, float *outAngle, float *outRadius, float *outZ)
{
    if (outAngle)
        *outAngle = NormalizeAngle__Ff(atan2f(QwordY(v), QwordX(v)));
    if (outRadius) {
        qword d, l;
#ifdef SLY2_EE_ASM
        __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(v));
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
#else
        d = VuDotXY(v);
        l = VuSqrtX(d);
#endif
        {
            int g;
#ifdef SLY2_EE_ASM
            __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
#else
            VuCopyBits(g, l);
#endif
            *(int *)outRadius = g;
        }
    }
    if (outZ)
        *outZ = QwordZ(v);
}
}

//////// 0x11EFB0 GetVectorSphere
namespace V_GetVectorSphere {
/* RadPanTiltRFromVector-style: pan/tilt angles and length of v (Claude agent, 2026-10-17) */
typedef int qword __attribute__((mode(TI)));
// x lane of q as a float (no instruction).
#ifdef SLY2_EE_ASM
static __inline__ float GetX(qword q) { float x; __asm__("" : "=r"(x) : "0"(q)); return x; }
#else
static __inline__ float GetX(qword q) { float x; x = q; return x; }
#endif
// y lane of q as a float.
#ifdef SLY2_EE_ASM
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%0" : "=r"(y) : "0"(q)); return y; }
#else
static __inline__ float GetY(qword q) { float y; VuCopyBits(y, VuProt3w(y)); return y; }
#endif
// z lane of q as a float.
#ifdef SLY2_EE_ASM
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%0" : "=r"(z) : "0"(q)); return z; }
#else
static __inline__ float GetZ(qword q) { float z; VuCopyBits(z, VuPextuw0(z)); return z; }
#endif
// Inverse of SetVectorSphere: v's pan, tilt and length (each optional).
// Also known as: ConvertVectorToSphere (upstream readable / Sly 1 name; ConvertVectorToSphere__FG9VU_VECTORPfN21).
extern "C" void GetVectorSphere(qword v, float *outPan, float *outTilt, float *outLength)
{
    if (outPan) {
        *outPan = NormalizeAngle__Ff(atan2f(GetY(v), GetX(v)));
    }
    if (outTilt) {
        qword d, l;
        float g;
#ifdef SLY2_EE_ASM
        __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(v), "j"(v)); /* 2nd "j"(v) unused: allocation only (keeps v in memory) */
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
#else
        d = VuDotXY(v);
        l = VuSqrtX(d);
#endif
        g = GetZ(v);
        *outTilt = NormalizeAngle__Ff(atan2f(g, GetX(l)));
    }
    if (outLength) {
        qword d, one, l, t;
        int g;
#ifdef SLY2_EE_ASM
        __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w" : "=&j"(one), "=&j"(d) : "j"(v), "j"(v)); /* 2nd "j"(v) unused: allocation only */
        __asm__("vadday.x $ACC, %1, %1y\n\tvmaddz.x %0, %2, %1z" : "=j"(t) : "j"(d), "j"(one));
#else
        t = VuDot(v, v);
#endif
        d = t;
#ifdef SLY2_EE_ASM
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
        __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
#else
        l = VuSqrtX(d);
        VuCopyBits(g, l);
#endif
        *(int *)outLength = g;
    }
}
}

//////// 0x11F088 PosSmooth
namespace V_PosSmooth {
// PosSmooth: ported from Lilly's personal Sly 2 decomp (src/P2/util.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A2DD0 (g_qwZero)
// 0x0011F088 PosSmooth__FUxUxfP3SMPP6VECTOR  (T1258)
// SmoothValue for a position (a 128-bit qword): the distance from posCur to posTarget is what SmoothValue eases to zero (psmp's pace, dt seconds), the direction stays.
// Returns the new position (posCur + dpos * (the distance moved / the distance)); *pv (optional) gets the velocity it arrives with, in the same direction
// (dpos * (SmoothValue's speed / the distance)). Closer than 0.0001: the position is posTarget and *pv is all zero (g_qwZero).
// Callers: STEPPLAYER::GetCpdefi 0x141640 (the focus offset at JT+0x1070 eases back to zero), UpdateJtAirInput (dposUnk1010).
// What decided the match (T1258, search over spellings; the first draft, a plain qword dpos, was 4%): dpos is a VU_VECTOR4 VALUE returned by SubPos (a memory
// temp) and qwDpos is a qword copy of it that every later use reads; the length is the dot product with the vector passed twice (LengthAsQword - with QwLength
// it is 51%): that gives the pseudo enough references for the register allocator to keep it in vf3 with a caller-save (sqc2 / lqc2 sp+0x40) around the
// SmoothValue call, while the temp stays in memory (sp+0x30) for the reloads. Using dpos.qw again after the call, or without the copy, is 7%.
#include "sly2/basic.h"
struct VU_VECTOR4
{
    qword qw;
};
struct VU_FLOAT
{
    qword qw;
};
// Broadcast a float into a VU0 register.
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
#ifdef SLY2_EE_ASM
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "r"(g));
#else
    v.qw = VuWiden<__typeof__(v.qw)>(g);
#endif
    return v;
}
// Length of v (x lane).
static inline qword LengthAsQword(qword v)
{
    qword qw = VuDot(v, v);
#ifdef SLY2_EE_ASM
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
#else
    qw = VuSqrtX(qw);
#endif
    return qw;
}
// Return qwRow scaled by gScale.
static inline qword ScaleVectorByX(qword qwRow, VU_FLOAT gScale)
{
    qword qw;
#ifdef SLY2_EE_ASM
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwRow), "j"(gScale.qw));
#else
    qw = VuScaleByX(qwRow, gScale.qw);
#endif
    return qw;
}
// Return qwBase + qwVec * gScale.
static inline qword AddScaledVector(qword qwBase, qword qwVec, VU_FLOAT gScale)
{
    qword qw;
#ifdef SLY2_EE_ASM
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(qwBase), "j"(qwVec), "j"(gScale.qw));
#else
    qw = VuAddScaled(qwBase, qwVec, gScale.qw);
#endif
    return qw;
}
// Return a - b.
static inline VU_VECTOR4 SubPos(qword a, qword b)
{
    VU_VECTOR4 r;
    r.qw = VuSub(a, b);
    return r;
}
// Moves posCur toward posTarget along the line between them by one SmoothValue step (smoothParams over dt);
// optionally returns the resulting velocity. Returns the target (velocity 0) when within 1e-4 of it.
extern "C" qword PosSmooth(qword posCur, qword posTarget, float dt, SMP *smoothParams, VECTOR *outVelocity)
{
    VU_VECTOR4 dpos = SubPos(posTarget, posCur);
    qword qwDpos = dpos.qw;
    float sDist = VuToFloat(LengthAsQword(qwDpos));
    if (sDist > 0.0001f)
    {
        float svNext;
        float sNext = SmoothValue(0.0f, sDist, dt, smoothParams, &svNext);
        if (outVelocity != 0)
        {
            *(qword *)outVelocity = ScaleVectorByX(qwDpos, VuFloat(svNext / sDist));
        }
        return AddScaledVector(posCur, qwDpos, VuFloat(sNext / sDist));
    }
    if (outVelocity != 0)
    {
        *(qword *)outVelocity = D_002A2DD0;
    }
    return posTarget;
}
}

//////// 0x11F198 GetCosineBetweenVectors
namespace V_GetCosineBetweenVectors {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
// Length of the xyz part of v as a float.
static inline float Len(qword v)
{
    qword d, one, l;
    float g;
#ifdef SLY2_EE_ASM
    __asm__("vmul.xyz %0, %1, %2\n\tvaddw.x $vf2, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, $vf2, %0z" : "=j"(d) : "j"(v), "j"(v) : "vf2");
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(l));
#else
    d = VuDot(v, v);
    l = VuSqrtX(d);
    VuCopyBits(g, l);
#endif
    return g;
}
// Dot product of the xyz parts of a and b as a float.
static inline float Dot(qword a, qword b)
{
    qword d, one;
    float g;
#ifdef SLY2_EE_ASM
    __asm__("vmul.xyz %0, %1, %2\n\tvaddw.x $vf2, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, $vf2, %0z" : "=j"(d) : "j"(a), "j"(b) : "vf2");
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(d));
#else
    d = VuDot(a, b);
    VuCopyBits(g, d);
#endif
    return g;
}
// Move a float's bits into a GPR qword (mfc1).
#ifdef SLY2_EE_ASM
static inline qword QF(float g) { U u; qword u_q; __asm__("mfc1 %0, %1" : "=r"(u_q) : "f"(g)); u.q = u_q; return u.q; }
#else
static inline qword QF(float g) { U u; qword u_q; u_q = VuWiden<__typeof__(u_q)>(g); u.q = u_q; return u.q; }
#endif
// Returns the cosine of the angle between a and b (dot / (|a| |b|)) as a quadword; 0 if either is shorter than
// 1e-4.
extern "C" qword GetCosineBetweenVectors(qword a, qword b)
{
    float cosine, lengthB;
    cosine = Len(a);   /* |a| until the division */
    if (cosine < 1.0e-4f) {
        cosine = 0.0f;
    } else {
        lengthB = Len(b);
        if (lengthB < 1.0e-4f)
            goto zero;   /* a separate tail; `return QF(0.0f)` here changes the allocation */
        { register float lengthProduct __asm__("$f1") = cosine * lengthB; cosine = Dot(a, b) / lengthProduct; }
    }
    return QF(cosine);
zero:
    return QF(0.0f);
}
}

//////// 0x11F288 VecMinArray
namespace V_VecMinArray {
typedef int qword __attribute__((mode(TI)));

// 0x11F278..0x11F284: 2 dead-code scraps (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x10");
SLY2_SCRAP("addiu $sp, $sp, 0x90");

// Componentwise minimum of `count` vectors.
extern "C" qword VecMinArray(int count, qword *vectors)
{
    qword m = D_002A2DF0;
    int i;
    for (i = 0; i < count; i++) {
        qword v = vectors[i];
#ifdef SLY2_EE_ASM
        __asm__("vmini.xyz %0, %1, %2" : "=j"(m) : "j"(v), "0"(m));
#else
        m = VuMin(v, m);
#endif
    }
    return m;
}
}

//////// 0x11F2B8 VecMaxArray
namespace V_VecMaxArray {
typedef int qword __attribute__((mode(TI)));

// Componentwise maximum of `count` vectors.
extern "C" qword VecMaxArray(int count, qword *vectors)
{
    qword m = D_002A2E00;
    int i;
    for (i = 0; i < count; i++) {
        qword v = vectors[i];
#ifdef SLY2_EE_ASM
        __asm__("vmax.xyz %0, %1, %2" : "=j"(m) : "j"(v), "0"(m));
#else
        m = VuMax(v, m);
#endif
    }
    return m;
}
}

// ---- vec's .rodata (0x2A2DA0..0x2A2E50, after mat's matrices in the first part of .rodata): the axis, zero, one and
// FLT_MAX vectors (VECTOR, then VECTOR4), which many TUs read (Sly 1's g_vecXAxis and kin: names DRAFT). Under asm
// labels so that no declaration has to agree with the definitions' types.
namespace RO_VEC {
extern const float r_D_002A2DA0[4] __asm__("D_002A2DA0") __attribute__((aligned(16))) = {1.0f, 0.0f, 0.0f, 0.0f};   /* x axis */
extern const float r_D_002A2DB0[4] __asm__("D_002A2DB0") __attribute__((aligned(16))) = {0.0f, 1.0f, 0.0f, 0.0f};   /* y axis */
extern const float r_D_002A2DC0[4] __asm__("g_normalZ") __attribute__((aligned(16))) = {0.0f, 0.0f, 1.0f, 0.0f};   /* z axis */
extern const float r_D_002A2DD0[4] __asm__("D_002A2DD0") __attribute__((aligned(16))) = {0.0f, 0.0f, 0.0f, 0.0f};   /* zero */
extern const float r_D_002A2DE0[4] __asm__("D_002A2DE0") __attribute__((aligned(16))) = {1.0f, 1.0f, 1.0f, 0.0f};   /* one */
extern const float r_D_002A2DF0[4] __asm__("D_002A2DF0") __attribute__((aligned(16))) = {3.40282347e+38f, 3.40282347e+38f, 3.40282347e+38f, 0.0f};    /* FLT_MAX */
extern const float r_D_002A2E00[4] __asm__("D_002A2E00") __attribute__((aligned(16))) = {-3.40282347e+38f, -3.40282347e+38f, -3.40282347e+38f, 0.0f}; /* -FLT_MAX */
extern const float r_D_002A2E10[4] __asm__("D_002A2E10") __attribute__((aligned(16))) = {0.0f, 0.0f, 0.0f, 0.0f};   /* VECTOR4 zero */
extern const float r_D_002A2E20[4] __asm__("D_002A2E20") __attribute__((aligned(16))) = {1.0f, 1.0f, 1.0f, 1.0f};   /* VECTOR4 one */
extern const float r_D_002A2E30[4] __asm__("D_002A2E30") __attribute__((aligned(16))) = {3.40282347e+38f, 3.40282347e+38f, 3.40282347e+38f, 3.40282347e+38f};
extern const float r_D_002A2E40[4] __asm__("D_002A2E40") __attribute__((aligned(16))) = {-3.40282347e+38f, -3.40282347e+38f, -3.40282347e+38f, -3.40282347e+38f};
}
