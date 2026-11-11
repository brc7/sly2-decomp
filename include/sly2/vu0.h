/* include/sly2/vu0.h -- VU0 macro-mode vector helpers shared by the game TUs (one definition per idiom).
 * Each helper is one inline asm idiom that the per-function decompilation had pasted into every TU under a private
 * name (Sub/VSub/QwSub, Add/QwAdd/VAdd, AddScaled/QwAddScaledX...). See docs/guides/tu-merging.md "Shared inline
 * helpers" for which spellings were folded in, and which ones must stay private (pinned registers, allocation-only
 * operands, "+j" forms, scheduling nudges). Without SLY2_EE_ASM (sly2/port.h) the same helpers are plain C, below.
 * The helpers are templates over the quadword type only because the TUs spell qword in two ways that are distinct
 * C++ types (`typedef int qword __attribute__((mode(TI)))` in most TUs, `unsigned long long` in sly2/basic.h); the
 * template keeps every TU's own qword. Names are descriptive placeholders (HYPOTHESIS: the originals were operators of
 * a VU_VECTOR class, as in Sly 1; no Sly 1 symbol names these idioms). */
#include "sly2/port.h"

#ifndef SLY2_VU0_H
#define SLY2_VU0_H
#ifdef SLY2_EE_ASM
/* a - b, all four lanes */
template <class QW> inline QW VuSub(QW a, QW b)
{
    QW r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}

/* a - b with the operands taken by reference: a different idiom for the compiler (the inputs are memory references
 * until the asm needs them, which schedules their loads differently); folding these sites into VuSub breaks them */
template <class QW> inline QW VuSubRef(const QW &a, const QW &b)
{
    QW r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}

/* a + b, all four lanes */
template <class QW> inline QW VuAdd(QW a, QW b)
{
    QW r;
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}

/* a + v * s.x, all four lanes (s: a VF register whose x holds the scale) */
template <class QW> inline QW VuAddScaled(QW a, QW v, QW s)
{
    QW r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a), "j"(v), "j"(s));
    return r;
}

/* a . b over xyz, in x of the result (the other lanes are scratch): d = a * b per lane; one.x = vf0.x + vf0.w = 1.0;
 * ACC.x = d.x + d.y; d.x = ACC.x + one.x * d.z. Self dot (squared length) is VuDot(v, v).
 * Placeholder name (HYPOTHESIS: a VU_VECTOR dot-product inline in the original; no Sly 1 symbol names it). */
template <class QW> inline QW VuDot(QW a, QW b)
{
    QW d, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    return d;
}

/* a float into a VF register (qmtc2.ni moves the whole GPR; x holds the float, the other lanes are scratch). The
 * result type is the template argument (VuFromFloat<qword>(g)) since it cannot be deduced; g is a float or, as many
 * per-function sites pass it, its bit pattern in an int. The non-interlocking .ni form only: plain qmtc2 is a
 * different instruction, and the "rV" spelling (the float's memory word as the operand) a different idiom.
 * Placeholder name (HYPOTHESIS: a VU_FLOAT(float) constructor in the original, as Sly 1's VU_FLOAT type suggests). */
template <class QW, class G> inline QW VuFromFloat(G g)
{
    typedef char assert_g_is_a_word[sizeof(G) == 4 ? 1 : -1]; /* a quadword moved to VF is not this idiom */
    QW r;
    __asm__("qmtc2.ni %1, %0" : "=j"(r) : "r"(g));
    return r;
}

/* x of a VF register as a float (qmfc2.ni into a GPR; the compiler adds the mtc1 when the float goes to an FPR).
 * The .ni form only, as VuFromFloat. Placeholder name (HYPOTHESIS: a VU_FLOAT-to-float conversion in the original). */
template <class QW> inline float VuToFloat(QW v)
{
    float g;
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(v));
    return g;
}

/* matrix times vector, the matrix given as its rows (each a VF register): xyz = r0 * v.x + r1 * v.y + r2 * v.z
 * (w of the result is scratch). VuMulMatrix4 adds r3 (the translation row of an affine 4x4: v.w taken as 1).
 * Placeholder names (HYPOTHESIS: operator*(MATRIX3/MATRIX4, VU_VECTOR) in the original). */
template <class QW> inline QW VuMulMatrix3(QW r0, QW r1, QW r2, QW v)
{
    QW r;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r) : "j"(r0), "j"(r1), "j"(r2), "j"(v));
    return r;
}
// Transform point v by the 4x4 matrix with rows r0..r3 (rotation plus translation row r3).
template <class QW> inline QW VuMulMatrix4(QW r0, QW r1, QW r2, QW r3, QW v)
{
    QW r;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(r) : "j"(r0), "j"(r1), "j"(r2), "j"(r3), "j"(v));
    return r;
}

/* ---- Added by the wave-4 helper pass (appended below the earlier asm so their line numbers stay put). ---- */

/* a x b (cross product over xyz; w of the result is scratch): ACC = a.yzx * b.zxy, r = ACC - b.yzx * a.zxy */
template <class QW> inline QW VuCross(QW a, QW b)
{
    QW r;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b));
    return r;
}

/* v * k.x, all four lanes (k: a VF register whose x holds the scale) */
template <class QW> inline QW VuScaleByX(QW v, QW k)
{
    QW r;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(k));
    return r;
}

/* v / d.x, all four lanes (Q = 1 / d.x via vdiv, then v * Q) */
template <class QW> inline QW VuDivByX(QW v, QW d)
{
    QW r;
    __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(r) : "j"(v), "j"(d));
    return r;
}

/* per-lane minimum / maximum over xyz (w of the result is scratch) */
template <class QW> inline QW VuMin(QW a, QW b)
{
    QW r;
    __asm__("vmini.xyz %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
// Per-lane maximum of the xyz parts of a and b.
template <class QW> inline QW VuMax(QW a, QW b)
{
    QW r;
    __asm__("vmax.xyz %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}

/* the zero vector (vf0 - vf0); the type is the template argument: VuZero<qword>() */
template <class QW> inline QW VuZero()
{
    QW r;
    __asm__("vsub.xyzw %0, $vf0, $vf0" : "=j"(r));
    return r;
}

/* sqrt(v.x) in x of the result (the other lanes are scratch) */
template <class QW> inline QW VuSqrtX(QW v)
{
    QW r;
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(v));
    return r;
}

/* |v| over xyz in x of the result (VuDot(v, v), then vsqrt as a second asm statement) */
template <class QW> inline QW VuLength(QW v)
{
    QW d, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(d), "=j"(one) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "0"(d));
    return d;
}

/* |v| over xy (the horizontal length) in x of the result */
template <class QW> inline QW VuLengthXY(QW v)
{
    QW d;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "0"(d));
    return d;
}

/* ---- EE core (not VU0) idioms that sit next to the VU0 ones: a quadword held in a 128-bit GPR, and the FPU. ---- */

/* x, y, z lane of a quadword in a GPR, as a float: mtc1 moves the low word to an FPR; prot3w / pextuw bring lane 1 /
 * lane 2 down to the low word of a GPR (the compiler adds the mtc1 where the float goes to an FPR) */
template <class QW> inline float QwordX(QW q)
{
    float g;
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q));
    return g;
}
// y lane of q as a float (prot3w rotates it into the low word).
template <class QW> inline float QwordY(QW q)
{
    float g;
    __asm__("prot3w %0, %1" : "=r"(g) : "r"(q));
    return g;
}
// z lane of q as a float (pextuw moves it into the low word).
template <class QW> inline float QwordZ(QW q)
{
    float g;
    __asm__("pextuw %0, $0, %1" : "=r"(g) : "r"(q));
    return g;
}

/* (x, y, z, scratch) packed into a quadword GPR; the type is the template argument: QwordFromFloats<qword>(x, y, z) */
template <class QW> inline QW QwordFromFloats(float x, float y, float z)
{
    QW q, t;
    __asm__("mfc1 %0, %2\n\tmfc1 %1, %3\n\tpextlw %0, %1, %0\n\tmfc1 %1, %4\n\tpcpyld %0, %1, %0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}

/* sqrt.s as volatile asm (the per-function sites spell sqrtf this way; the compiler's own sqrtf would differ) */
inline float SqrtFloat(float g)
{
    float r;
    __asm__ __volatile__("sqrt.s %0, %1" : "=&f"(r) : "f"(g));
    return r;
}

/* FPU max.s / min.s / abs.s as asm (the per-function sites spell these instead of the C comparisons, whose code
 * differs) */
inline float FpuMax(float a, float b)
{
    float r;
    __asm__("max.s %0, %1, %2" : "=f"(r) : "f"(a), "f"(b));
    return r;
}
// Smaller of a and b (min.s).
inline float FpuMin(float a, float b)
{
    float r;
    __asm__("min.s %0, %1, %2" : "=f"(r) : "f"(a), "f"(b));
    return r;
}
// Absolute value of g (abs.s).
inline float FpuAbs(float g)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(g));
    return r;
}

#else /* SLY2_EE_ASM */
/* ---- The same helpers in plain C++, for builds without EE assembly (sly2/port.h). ----
 * Each one computes what the asm computes, in the VU0's operation order (one rounding per VU operation, no fused
 * multiply-add: build with -ffp-contract=off), on IEEE floats; the VU's clamping of Inf/NaN and its truncating rounding
 * are not modelled. Lanes the asm leaves as scratch are zero here. Checked against a VU0 model of the asm text
 * (docs/guides/portability.md, "VU0 fallbacks"). */

/* the four float lanes of a quadword (x, y, z, w: the in-memory order) */
struct VuLanes { float x, y, z, w; };
template <class QW> inline VuLanes VuGet(QW q)
{
    VuLanes v;
    __builtin_memcpy(&v, &q, sizeof v);
    return v;
}
// A quadword from four float lanes; the type is the template argument: VuPut<qword>(v).
template <class QW> inline QW VuPut(VuLanes v)
{
    QW q;
    __builtin_memcpy(&q, &v, sizeof q);
    return q;
}
// A quadword from four floats.
template <class QW> inline QW VuMake(float x, float y, float z, float w)
{
    VuLanes v = { x, y, z, w };
    return VuPut<QW>(v);
}
/* register moves between a float / int and a quadword (qmtc2, qmfc2, mtc1, mfc1, sq): the low bytes, as the bits
   are; VuWiden zero-fills the lanes a 32-bit source does not cover */
template <class D, class S> inline void VuCopyBits(D &d, const S &s)
{
    __builtin_memcpy(&d, &s, sizeof d < sizeof s ? sizeof d : sizeof s);
}
// A quadword (or word) from the low bytes of s, the rest zero; VuWiden<qword>(g).
template <class D, class S> inline D VuWiden(const S &s)
{
    D d;
    __builtin_memset(&d, 0, sizeof d);
    VuCopyBits(d, s);
    return d;
}
/* x = v.x^2 + v.y^2 (vmul.xy + vaddy.x; y keeps v.y^2) */
template <class QW> inline QW VuDotXY(QW v)
{
    VuLanes p = VuGet(v);
    return VuMake<QW>(p.x * p.x + p.y * p.y, p.y * p.y, 0.0f, 0.0f);
}
/* a * s.x + b * t.x, all four lanes (vmulax + vmaddx) */
template <class QW> inline QW VuScaleX2(QW a, QW s, QW b, QW t)
{
    VuLanes p = VuGet(a), q = VuGet(b);
    float k = VuGet(s).x, m = VuGet(t).x;
    return VuMake<QW>(p.x * k + q.x * m, p.y * k + q.y * m, p.z * k + q.z * m, p.w * k + q.w * m);
}
/* ((a + vf0) + b * s.x) + c * s.x, all four lanes (vadda with vf0, so w gains 1.0; vmaddax; vmaddx) */
template <class QW> inline QW VuAddScaled2(QW a, QW b, QW c, QW s)
{
    VuLanes p = VuGet(a), q = VuGet(b), r = VuGet(c);
    float k = VuGet(s).x;
    return VuMake<QW>((p.x + q.x * k) + r.x * k, (p.y + q.y * k) + r.y * k, (p.z + q.z * k) + r.z * k,
                      ((p.w + 1.0f) + q.w * k) + r.w * k);
}
/* r with old's w: an asm that writes only xyz of a "+j" operand leaves its w as it was */
template <class QW> inline QW VuKeepW(QW old, QW r)
{
    VuLanes p = VuGet(r);
    p.w = VuGet(old).w;
    return VuPut<QW>(p);
}
/* the MMI lane moves the decomp uses to read y and z: prot3w (lanes y, z, x, w) and pextuw with $zero (z, 0, w, 0) */
template <class QW> inline QW VuProt3w(QW v)
{
    VuLanes p = VuGet(v);
    VuLanes r = { p.y, p.z, p.x, p.w };
    return VuPut<QW>(r);
}
// pextuw rd, $zero, v: lanes (z, 0, w, 0).
template <class QW> inline QW VuPextuw0(QW v)
{
    VuLanes p = VuGet(v);
    VuLanes r = { p.z, 0.0f, p.w, 0.0f };
    return VuPut<QW>(r);
}
/* the VU's square root (vsqrt, and the EE FPU's sqrt.s): the root of |g| */
inline float VuSqrt(float g) { return __builtin_sqrtf(__builtin_fabsf(g)); }

template <class QW> inline QW VuSub(QW a, QW b)
{
    VuLanes p = VuGet(a), q = VuGet(b);
    return VuMake<QW>(p.x - q.x, p.y - q.y, p.z - q.z, p.w - q.w);
}
template <class QW> inline QW VuSubRef(const QW &a, const QW &b) { return VuSub(a, b); }
template <class QW> inline QW VuAdd(QW a, QW b)
{
    VuLanes p = VuGet(a), q = VuGet(b);
    return VuMake<QW>(p.x + q.x, p.y + q.y, p.z + q.z, p.w + q.w);
}
/* ACC = a + vf0 (so w gains 1.0), r = ACC + v * s.x */
template <class QW> inline QW VuAddScaled(QW a, QW v, QW s)
{
    VuLanes p = VuGet(a), q = VuGet(v);
    float k = VuGet(s).x;
    return VuMake<QW>(p.x + q.x * k, p.y + q.y * k, p.z + q.z * k, (p.w + 1.0f) + q.w * k);
}
/* x = (a.x b.x + a.y b.y) + a.z b.z; y, z keep the lane products */
template <class QW> inline QW VuDot(QW a, QW b)
{
    VuLanes p = VuGet(a), q = VuGet(b);
    float dx = p.x * q.x, dy = p.y * q.y, dz = p.z * q.z;
    return VuMake<QW>((dx + dy) + dz, dy, dz, 0.0f);
}
template <class QW, class G> inline QW VuFromFloat(G g)
{
    typedef char assert_g_is_a_word[sizeof(G) == 4 ? 1 : -1];
    float f;
    __builtin_memcpy(&f, &g, sizeof f);
    return VuMake<QW>(f, 0.0f, 0.0f, 0.0f);
}
template <class QW> inline float VuToFloat(QW v) { return VuGet(v).x; }
/* xyz = (r0 v.x + r1 v.y) + r2 v.z, accumulated in that order */
template <class QW> inline QW VuMulMatrix3(QW r0, QW r1, QW r2, QW v)
{
    VuLanes a = VuGet(r0), b = VuGet(r1), c = VuGet(r2), u = VuGet(v);
    return VuMake<QW>((a.x * u.x + b.x * u.y) + c.x * u.z, (a.y * u.x + b.y * u.y) + c.y * u.z,
                      (a.z * u.x + b.z * u.y) + c.z * u.z, 0.0f);
}
// VuMulMatrix3 plus the translation row r3 (v.w taken as 1).
template <class QW> inline QW VuMulMatrix4(QW r0, QW r1, QW r2, QW r3, QW v)
{
    VuLanes a = VuGet(r0), b = VuGet(r1), c = VuGet(r2), d = VuGet(r3), u = VuGet(v);
    return VuMake<QW>(((a.x * u.x + b.x * u.y) + c.x * u.z) + d.x, ((a.y * u.x + b.y * u.y) + c.y * u.z) + d.y,
                      ((a.z * u.x + b.z * u.y) + c.z * u.z) + d.z, 0.0f);
}
/* vopmula / vopmsub: r = a.yzx * b.zxy - b.yzx * a.zxy */
template <class QW> inline QW VuCross(QW a, QW b)
{
    VuLanes p = VuGet(a), q = VuGet(b);
    return VuMake<QW>(p.y * q.z - q.y * p.z, p.z * q.x - q.z * p.x, p.x * q.y - q.x * p.y, 0.0f);
}
template <class QW> inline QW VuScaleByX(QW v, QW k)
{
    VuLanes p = VuGet(v);
    float g = VuGet(k).x;
    return VuMake<QW>(p.x * g, p.y * g, p.z * g, p.w * g);
}
/* Q = 1 / d.x, then v * Q (a multiplication by the reciprocal, as the VU does it) */
template <class QW> inline QW VuDivByX(QW v, QW d)
{
    VuLanes p = VuGet(v);
    float q = 1.0f / VuGet(d).x;
    return VuMake<QW>(p.x * q, p.y * q, p.z * q, p.w * q);
}
template <class QW> inline QW VuMin(QW a, QW b)
{
    VuLanes p = VuGet(a), q = VuGet(b);
    return VuMake<QW>(p.x < q.x ? p.x : q.x, p.y < q.y ? p.y : q.y, p.z < q.z ? p.z : q.z, 0.0f);
}
template <class QW> inline QW VuMax(QW a, QW b)
{
    VuLanes p = VuGet(a), q = VuGet(b);
    return VuMake<QW>(p.x > q.x ? p.x : q.x, p.y > q.y ? p.y : q.y, p.z > q.z ? p.z : q.z, 0.0f);
}
template <class QW> inline QW VuZero() { return VuMake<QW>(0.0f, 0.0f, 0.0f, 0.0f); }
template <class QW> inline QW VuSqrtX(QW v) { return VuMake<QW>(VuSqrt(VuGet(v).x), 0.0f, 0.0f, 0.0f); }
template <class QW> inline QW VuLength(QW v) { return VuSqrtX(VuDot(v, v)); }
template <class QW> inline QW VuLengthXY(QW v)
{
    VuLanes p = VuGet(v);
    return VuMake<QW>(VuSqrt(p.x * p.x + p.y * p.y), 0.0f, 0.0f, 0.0f);
}
template <class QW> inline float QwordX(QW q) { return VuGet(q).x; }
template <class QW> inline float QwordY(QW q) { return VuGet(q).y; }
template <class QW> inline float QwordZ(QW q) { return VuGet(q).z; }
template <class QW> inline QW QwordFromFloats(float x, float y, float z) { return VuMake<QW>(x, y, z, 0.0f); }
inline float SqrtFloat(float g) { return VuSqrt(g); }
inline float FpuMax(float a, float b) { return a > b ? a : b; }
inline float FpuMin(float a, float b) { return a < b ? a : b; }
inline float FpuAbs(float g) { return __builtin_fabsf(g); }
#endif /* SLY2_EE_ASM */
#endif /* SLY2_VU0_H */
