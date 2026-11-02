// EvaluateBezierMat (Claude agent, lane A13, 2026-10-23): MATCH, from build/hand_wip/EvaluateBezierMat.fresh15.cpp (4).
// Rotation interpolation between two orientations (pmat0/w0 and pmat1/w1); writes the blended matrix and/or the
// blended angular velocity.  Matching notes:
//  - k is a plain qword (no volatile copy): it gets no hard register and is reloaded from its spill slot (0x120).
//  - In the final combine asm k is operand 1 and w operand 2 (template %2,%1), so reload gives k vf5 / w vf6.
//  - The trailing "j"(k) is an allocation-only operand (see AGENT_GUIDE "Allocation-only asm operands").
//  - kb's dead VU-float store is written after the result store; ka keeps it inside VF().
//  - ScaleT's asm is volatile (commented below) to emulate the toolchain's scheduling of the trailing stores.
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
typedef struct { qword r[3]; } M3;
struct VU_FLOAT {
    qword q;
    VU_FLOAT(float f) { __asm__("mfc1 %0, %1" : "=r"(q) : "f"(f)); }
};
extern "C" {
void func_0011B570(M3 *pm, qword v);
M3 *MulMatrix3Matrix3(M3 *pr, M3 *pa, M3 *pb);
qword func_0011BFB8(M3 *pm);
}
static inline M3 Rot(qword v) return m
{
    func_0011B570(&m, v);
}
static inline qword VF(float g)
{
    U s;
    qword q;
    __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(g));
    s.q = q;
    return q;
}
static inline qword VF0(float g)
{
    qword q;
    __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(g));
    return q;
}
static inline qword ScaleT(qword v, qword k)
{
    qword r;
    /* volatile: emulates the original's schedule (the dead stores stay after it) */
    __asm__ volatile("vmulx.xyzw %0, %1, %0x" : "=j"(r) : "j"(v), "0"(k));
    return r;
}
static inline qword Scale(qword v, qword k)
{
    qword r;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(k));
    return r;
}
static inline void MulP(M3 *pm, const M3 &a, const M3 *pb)
{
    M3 t;
    qword r0, r1, r2;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r0) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(pb->r[0]));
    t.r[0] = r0; pm->r[0] = r0;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r1) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(pb->r[1]));
    t.r[1] = r1; pm->r[1] = r1;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r2) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(pb->r[2]));
    t.r[2] = r2; pm->r[2] = r2;
}
extern "C" void EvaluateBezierMat(float dt, float t, float gScale, M3 *pmat0, qword w0, M3 *pmat1, qword w1, M3 *pmatOut, qword *pwOut);
/** @sly2-annot addr=0x117308
 * Purpose: Rotation interpolation between (mat0,w0) and (mat1,w1): blended orientation matrix and/or angular
 *   velocity
 * Info: proposed name EvaluateBezierMat (T1); TU bez
 * Notes: names_v2 C proposed LimitBezierMulti__Fie (0.356)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/core-library.md#bez-0x117030-0x1177a8
 * @end-sly2-annot */
void EvaluateBezierMat(float dt, float t, float gScale, M3 *pmat0, qword w0, M3 *pmat1, qword w1, M3 *pmatOut, qword *pwOut)
{
    float u = t / dt;
    float v = 1.0f - u;
    if (pmatOut) {
        float u2 = u * u;
        float h2 = u2 * v;
        float u3 = u2 * u;
        M3 a, b, rel, rot;
        qword w;
        func_0011B570(&rot, Scale(w0, VF(u * v * v * dt)));
        MulP(&rel, rot, pmat0);
        a = rel;
        func_0011B570(&rot, Scale(w1, VF(-h2 * dt)));
        MulP(&rel, rot, pmat1);
        b = rel;
        MulMatrix3Matrix3(&rel, &a, &b);
        w = func_0011BFB8(&rel);
        func_0011B570(&rot, Scale(w, VF(h2 * (3.0f - dt) + u3)));
        MulP(&rel, rot, &a);
        *pmatOut = rel;
    }
    if (pwOut) {
        qword k = VU_FLOAT(u * (6.0f - u * 6.0f) / dt).q;
        qword w;
        {
            M3 rel;
            MulMatrix3Matrix3(&rel, pmat0, pmat1);
            w = func_0011BFB8(&rel);
        }
        float h = u * 3.0f;
        M3 us;
        register qword sc __asm__("vf2"); /* pin */
        __asm__("qmtc2.ni %1, %0" : "=j"(sc) : "r"(gScale));
        us.r[0] = sc;
        qword kb = VF0(u * (h + -2.0f));
        qword ka = VF(1.0f - u * (4.0f - h));
        qword r;
        __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddax.xyzw $ACC, %3, %4x\n\tvmaddx.xyzw %0, %5, %6x"
                : "=j"(r) : "j"(k), "j"(w), "j"(w0), "j"(ka), "j"(w1), "j"(kb), "j"(k)); /* last "j"(k): allocation-only operand */
        *pwOut = ScaleT(r, sc);
        { U s2; s2.q = kb; }
    }
}
