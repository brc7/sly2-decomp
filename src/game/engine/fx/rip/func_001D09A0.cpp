/* func_001D09A0 (Claude agent, wave-3 lane 6): particle turning + axis-dependent damping (P: basis rows at 0x40, velocity at 0x90, drag at 0xC4).
 * Turn the basis and velocity about basis row 1 by a clamped quadratic of vel.row0, split the velocity into the part along the old row 0 and the
 * part across it, normalise the along part (fallback D_002A2DA0 when shorter than 1e-4), blend its length toward a target and damp the
 * across part, update with drag cleared, then restore drag.
 *
 * What made it match:
 *  - one float `g` holds the dot product at the start and the second length readout at the end (gcc gives it one pseudo whose conflicts in
 *    both places put it in $f5);
 *  - the length is a TI qword left in a VF register; both readouts are `mtc1` asms with an "r" operand so the compiler emits the qmfc2.ni
 *    (reload register, $v1 / $a3 like the target);
 *  - NormalizeSafe gets the fallback as a pointer (computed before the length, kept in $v0);
 *  - the vector to normalise is read into $v0 and copied into a VF register by a plain C copy, so the qmtc2.ni is a real move with the
 *    load latency in both schedulers (an asm qmtc2 is "free" to the scheduler and lands one slot early); the volatile read stops combine
 *    from folding the load and the copy back into an lqc2.
 */
typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
struct P { char pad0[0x40]; M3 matrix; char pad70[0x20]; qword vel; char padA0[0x24]; float drag; };
struct CLOCK { float time, dt; };
extern "C" {
extern CLOCK D_002DDED0;
extern float D_002C2010[3], D_002C2020[2], D_002C2028, D_002C202C;
extern qword D_002A2DA0;
M3 func_0011B500(qword, float);
float func_0011E290(qword, qword, qword *, qword *);
void func_001CF0A0(P *);
}
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
/* f1*v1 + f2*v2 */
static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1.q), "j"(f2.q), "j"(v2.q));
    return r;
}
static inline float GClamp(float g, const float *lim)
{
    float gMin = lim[0], gMax = lim[1];
    if (g < gMin) return gMin;
    if (gMax < g) return gMax;
    return g;
}
static inline float Dot(qword a, qword b)
{
    qword d, one;
    float g;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(d));
    return g;
}
/* m * v for a 3x3 matrix stored as rows */
static inline qword Rotate(const M3 &m, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(r) : "j"(v), "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]));
    return r;
}
/* a*b for 3x3 matrices; g++ named return value (its two dead stack copies are in the target) */
static inline M3 Multiply(const M3 &a, const M3 &b) return r
{
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z" : "=j"(r.r[0]) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(b.r[0]));
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z" : "=j"(r.r[1]) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(b.r[1]));
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z" : "=j"(r.r[2]) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(b.r[2]));
}
/* length of the xyz part, left in a VF register (x) */
static inline qword Length(qword v)
{
    qword d, one, l;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(d));
    return l;
}
static inline qword Divide(qword v, qword l)
{
    qword r;
    __asm__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(r) : "j"(v), "j"(l), "j"(l)); /* allocation-only second "j"(l): keeps the length in a VF register */
    return r;
}
static inline float GetX(qword q) { float g; __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q)); return g; }
/* *pv /= |*pv|, or *pv = *pfallback when the length is below 1e-4; returns the length */
static inline qword NormalizeSafe(qword *pv, const qword *pfallback)
{
    register qword g asm("$2") = *(volatile qword *)pv; /* the one register pin: the target reads the vector into $v0, then qmtc2 */
    qword v = g;
    qword len = Length(v);
    if (GetX(len) < 0.0001f) *pv = *pfallback;
    else *pv = Divide(v, len);
    return len;
}
/** @sly2-annot addr=0x1D09A0
 * Purpose: LEAF Project: flutter + base ProjectRipTransform
 * Info: proposed name ProjectLeafTransform (T1); TU rip
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
extern "C" void func_001D09A0(P *p)
{
    qword row0 = p->matrix.r[0];
    qword axis = p->matrix.r[1];
    float g = Dot(p->vel, row0);
    float t = GClamp(D_002C2010[0] + g * (D_002C2010[1] + g * D_002C2010[2]), D_002C2020);
    M3 rot = func_0011B500(axis, t * D_002DDED0.dt);
    p->vel = Rotate(rot, p->vel);
    p->matrix = Multiply(rot, p->matrix);
    float drag = p->drag;
    p->drag = 0.0f;
    struct { qword along, across; } w;
    func_0011E290(p->vel, row0, &w.along, &w.across);
    qword len = NormalizeSafe(&w.along, &D_002A2DA0);
    float dt = D_002DDED0.dt;
    g = GetX(len);
    float gAlong = D_002C2028 + (D_002C202C * dt + 1.0f) * (g - D_002C2028);
    float gAcross = drag * dt + 1.0f;
    p->vel = VecCombo(VU_FLOAT(gAlong), VU_VECTOR(w.along), VU_FLOAT(gAcross), VU_VECTOR(w.across));
    func_001CF0A0(p);
    p->drag = drag;
}
