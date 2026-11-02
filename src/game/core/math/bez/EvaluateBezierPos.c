/* (Claude agent) BODY IS EXACT: identical to the target except the 2 leading nops the build adds before the
   function-entry div.s. Those nops ARE in the original binary at 0x117120-0x117127, but config/junk.csv lists
   0x117120..0x117128 as a dead-code scrap and functions.csv starts this function at 0x117128. Fix: make the function
   start at 0x117120 (drop the junk.csv entry) and this file should match as-is (rename to EvaluateBezierPos). */
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
static inline qword VF(float g)
{
    qword q;
    __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(g));
    return q;
}
/** @sly2-annot addr=0x117120
 * Purpose: Cubic Hermite position/velocity/acceleration along a curve segment
 * Info: proposed name EvaluateBezierPos (T1); TU bez
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r1_116918+verdicts_A1
 * See: docs/engine/animation.md#animation-format-summary-anim_lab-v31,
 *   docs/engine/core-library.md#7-key-functions, docs/MATCHING_NOTES.md#junk-list-correction
 * @end-sly2-annot */
void EvaluateBezierPos(qword p0, qword v0, qword p1, qword v1, float dt, float t, float dtScale, qword *ppos, qword *pvel, qword *pacc)
{
    float v;
    U tmp;
    t = t / dt;
    v = 1.0f - t;
    if (ppos) {
        float uv2 = t * v * v;
        float u2 = t * t;
        float u2v = u2 * v;
        float u3c = u2 * t;
        qword h00 = VF(v * v * v + uv2 * 3.0f);
        qword h10 = VF(uv2 * dt);
        qword h11 = VF(-u2v * dt);
        qword h01 = VF(u3c + u2v * 3.0f);
        qword r;
        *(volatile qword *)&tmp.q = h00;
        *(volatile qword *)&tmp.q = h10;
        __asm__("vmulax.xyzw $ACC, %1, %5x\n\tvmaddax.xyzw $ACC, %2, %6x\n\tvmaddax.xyzw $ACC, %3, %7x\n\tvmaddx.xyzw %0, %4, %8x"
                : "=j"(r) : "j"(p0), "j"(v0), "j"(v1), "j"(p1), "j"(h00), "j"(h10), "j"(h11), "j"(h01));
        *(volatile qword *)&tmp.q = h11;
        *ppos = r;
        *(volatile qword *)&tmp.q = h01;
    }
    if (pvel) {
        qword dp, s, w0, w1, w2, r, r2;
        float v6 = 6.0f - t * 6.0f;
        float u3 = t * 3.0f;
        s = VF(dtScale);
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(dp) : "j"(p1), "j"(p0));
        *(volatile qword *)&tmp.q = s;
        w2 = VF(t * (u3 + -2.0f));
        w0 = VF(t * v6 / dt);
        *(volatile qword *)&tmp.q = w0;
        w1 = VF(1.0f - t * (4.0f - u3));
        __asm__("vmulax.xyzw $ACC, %4, %1x\n\tvmaddax.xyzw $ACC, %5, %2x\n\tvmaddx.xyzw %0, %6, %3x"
                : "=j"(r) : "j"(w0), "j"(w1), "j"(w2), "j"(dp), "j"(v0), "j"(v1));
        *(volatile qword *)&tmp.q = w1;
        __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r2) : "j"(s), "j"(r));
        *(volatile qword *)&tmp.q = w2;
        *pvel = r2;
    }
    if (pacc) {
        qword dp, s, w0, w1, w2, r, r2;
        float u6 = t * 6.0f;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(dp) : "j"(p1), "j"(p0));
        s = VF(dtScale * dtScale / dt);
        w1 = VF(u6 - 4.0f);
        *(volatile qword *)&tmp.q = s;
        w2 = VF(u6 - 2.0f);
        w0 = VF((6.0f - t * 12.0f) / dt);
        *(volatile qword *)&tmp.q = w0;
        __asm__("vmulax.xyzw $ACC, %4, %1x\n\tvmaddax.xyzw $ACC, %5, %2x\n\tvmaddx.xyzw %0, %6, %3x"
                : "=j"(r) : "j"(w0), "j"(w1), "j"(w2), "j"(dp), "j"(v0), "j"(v1));
        __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r2) : "j"(s), "j"(r));
        *(volatile qword *)&tmp.q = w1;
        *pacc = r2;
        *(volatile qword *)&tmp.q = w2;
    }
}
