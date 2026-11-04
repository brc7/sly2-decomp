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

template <class QW> inline QW VuMulMatrix4(QW r0, QW r1, QW r2, QW r3, QW v)
{
    QW r;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(r) : "j"(r0), "j"(r1), "j"(r2), "j"(r3), "j"(v));
    return r;
}

#endif /* SLY2_VU0_H */
