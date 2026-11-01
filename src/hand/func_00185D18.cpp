// func_00185D18 (Claude agent, lane G16, 2026-10-24): MATCH, no register pins, VU helper asms only.
// From laneG15v1 (6). Keys: (1) D_002BE918 is `extern const float` read straight into the qmtc2's "rV" operand
// (VuFloatFromMem(float)): reload makes the lw, and the unchanging MEM keeps the *ppos/*pvel stores free of
// anti-dependences on it (3). (2) the last AddScaled2 lists a dead "$2" (v0) clobber: the extra dependence on the lw
// wins its sched2 tie with the acc reload and keeps `li v0,1` after the asm (0). The "r"(ptr), "r"(pvel) operands
// are allocation-only (local-alloc order ptr s0, pvel s1, ppos s2).
typedef int qword __attribute__((mode(TI)));
struct TRIREF { int index; float a, b; };
extern "C" {
void func_00185310(void *p, qword v, int n, TRIREF *ptr, qword *ppos, qword *pnormal);
extern const float D_002BE918;
}
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(*(int *)&f)); } VU_FLOAT() {} };
static inline VU_FLOAT VuFloatFromMem(float f) { VU_FLOAT v; __asm__("qmtc2.ni %1, %0" : "=j"(v.q) : "rV"(f)); return v; }
static inline qword AddScaled(qword a, qword b, VU_FLOAT s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a), "j"(b), "j"(s.q));
    return r;
}
static inline qword AddScaled2(qword a, qword b, VU_FLOAT sb, qword c, VU_FLOAT sc)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %2, %3x\n\tvmaddx.xyzw %0, %4, %5x"
        : "=j"(r) : "j"(a), "j"(b), "j"(sb.q), "j"(c), "j"(sc.q));
    return r;
}
/* Same as AddScaled2; the extra "r"(hint) operands are allocation-only (emit nothing); v0 is dead here (clobber only orders sched2). */
static inline qword AddScaled2H(qword a, qword b, VU_FLOAT sb, qword c, VU_FLOAT sc, void *hint, void *hint2)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %2, %3x\n\tvmaddx.xyzw %0, %4, %5x"
        : "=j"(r) : "j"(a), "j"(b), "j"(sb.q), "j"(c), "j"(sc.q), "r"(hint), "r"(hint2) : "$2");
    return r;
}
/** @sly2-annot addr=0x185D18
 * Purpose: Project a ballistic point (pos+v t+a t^2/2) onto the mesh; out landing pos (offset along normal)
 *   and velocity
 * Info: proposed name PredictMeshContact (T2); TU tu_185130
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#tu_185130-0x185130-0x186610,
 *   docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
extern "C" int func_00185D18(void *p, qword pos, qword vel, qword acc, TRIREF *ptr, qword *ppos, qword *pvel, float offset, float t)
{
    TRIREF tr;
    union { qword q; } closest, normal;
    func_00185310(p, AddScaled2(pos, vel, VU_FLOAT(t), acc, VU_FLOAT((t * 0.5f) * t)), 0, &tr, &closest.q, &normal.q);
    *ptr = tr;
    qword rp = AddScaled(closest.q, normal.q, VU_FLOAT(offset));
    qword rv = AddScaled2H(vel, acc, VU_FLOAT(t), normal.q, VuFloatFromMem(D_002BE918), ptr, pvel);
    *ppos = rp;
    *pvel = rv;
    return 1;
}
