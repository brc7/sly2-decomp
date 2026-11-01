/* func_0011E4B0 (Claude agent): angle between vectors + optional normalized axis. Length's float comes from an mtc1 asm fed by gcc's own qmfc2 ("r"(qword)), and the axis length vector (pinned vf1) is passed as a GPR qword to the qmtc2, so the move is shared like the original. */
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
extern qword D_002A2DA0;
qword func_0011E390(qword v);
float func_002456F0(float g);
static inline float Length(qword v)
{
    qword d, one;
    float f;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "+j"(d));
    __asm__("mtc1 %1, %0" : "=f"(f) : "r"(d));
    return f;
}
static inline float Dot(qword a, qword b)
{
    qword d, one;
    float f;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
static inline qword Cross(qword a, qword b)
{
    qword r;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword LenV(qword v)
{
    register qword d __asm__("vf1");
    qword one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "+j"(d));
    return d;
}
static inline qword Scale(qword v, qword g)
{
    U u;
    qword r;
    __asm__("qmtc2.ni %1, %0" : "=j"(u.q) : "r"(g));
    __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(r) : "j"(v), "j"(u.q));
    return r;
}
/** @sly2-annot addr=0x11E4B0
 * Purpose: Angle between two vectors (acosf) and optional normalised rotation axis
 * Info: proposed name RadBetweenVectors (T1); TU vec
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#vec-0x11e198-0x11f440,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
float func_0011E4B0(qword a, qword b, qword *paxis)
{
    float la = Length(a);
    float lb = Length(b);
    if (la < 1e-4f || lb < 1e-4f) {
        if (paxis)
            *paxis = D_002A2DA0;
        return 0.0f;
    }
    if (paxis) {
        qword axis = Cross(a, b);
        qword dl = LenV(axis);
        float l;
        __asm__("mtc1 %1, %0" : "=f"(l) : "r"(dl));
        if (l > 1e-8f)
            axis = Scale(axis, dl);
        else
            axis = func_0011E390(a);
        *paxis = axis;
    }
    return func_002456F0(Dot(a, b) / (la * lb));
}
