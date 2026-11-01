typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
/** @sly2-annot addr=0x11E9F8
 * Purpose: Scales v down to length gMax if longer
 * Info: proposed name LimitVectorLength (T1); TU vec
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#vec-0x11e198-0x11f440,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
qword func_0011E9F8(qword v, float gMax)
{
    qword d, one, l;
    float g;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(l));
    if (gMax < g) {
        U s;
        qword r;
        float q;
        __asm__ __volatile__("div.s %0, %1, %2" : "=f"(q) : "f"(gMax), "f"(g));
        __asm__("qmtc2.ni %1, %0" : "=j"(s.q) : "r"(q));
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(s.q));
        return r;
    }
    return v;
}
