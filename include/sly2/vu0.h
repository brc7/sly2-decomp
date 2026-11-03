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

#endif /* SLY2_VU0_H */
