/* include/sly2/vu0.h -- VU0 macro-mode vector helpers shared by the game TUs (one definition per idiom).
 *
 * Each helper is one inline asm idiom that the per-function decompilation had pasted into every TU under a private
 * name (Sub/VSub/QwSub, Add/QwAdd/VAdd, AddScaled/QwAddScaledX...). See docs/guides/tu-merging.md "Shared inline
 * helpers" for which spellings were folded in, and which ones must stay private (pinned registers, allocation-only
 * operands, "+j" forms, scheduling nudges).
 *
 * The helpers are templates over the quadword type only because the TUs spell qword in two ways that are distinct
 * C++ types (`typedef int qword __attribute__((mode(TI)))` in most TUs, `unsigned long long` in sly2/basic.h); the
 * template keeps every TU's own qword. Names are descriptive placeholders (HYPOTHESIS: the originals were operators of
 * a VU_VECTOR class, as in Sly 1; no Sly 1 symbol names these idioms). */
#ifndef SLY2_VU0_H
#define SLY2_VU0_H

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

#endif /* SLY2_VU0_H */
