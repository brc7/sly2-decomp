typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
struct LO { char pad[0x40]; M3 m; };
struct O { char pad[0xA0]; struct LO *plo; };
struct S { qword v; int k; };
static inline qword MulMV(M3 *pm, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z" : "=j"(r) : "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]), "j"(v));
    return r;
}
static inline qword Normalize(qword v)
{
    qword r, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q\n\tvdiv $Q, $vf0w, %0x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=&j"(r), "=&j"(one) : "j"(v));
    return r;
}
static inline qword Dot(qword a, qword b)
{
    qword d, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(d), "=&j"(one) : "j"(a), "j"(b));
    return d;
}
static inline qword Scale(qword n, qword d)
{
    qword r;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(n), "j"(d));
    return r;
}
static inline qword Sub(qword a, qword b)
{
    qword r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword Proj(qword a, qword n)
{
    qword r, one, d;
    __asm__("vmul.xyz %2, %3, %4\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %2, %2y\n\tvmaddz.x %2, %1, %2z\n\tvmulx.xyzw %0, %4, %2x" : "=j"(r), "=&j"(one), "=&j"(d) : "j"(a), "j"(n));
    return r;
}
static inline qword Rej(qword a, qword n)
{
    qword r, one, d;
    __asm__("vmul.xyz %1, %2, %3\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(one), "=&j"(d) : "j"(a), "j"(n));
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(d) : "j"(n), "j"(d));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(d));
    return r;
}
static inline void MulMVIn(M3 *pm, qword *pv)
{
    __asm__("vmulax.xyz $ACC, %1, %0x\n\tvmadday.xyz $ACC, %2, %0y\n\tvmaddz.xyz %0, %3, %0z" : "+j"(*pv) : "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]));
}
static inline void NormalizeIn(qword *pv)
{
    qword l, one;
    __asm__("vmul.xyz %1, %0, %0\n\tvaddw.x %2, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %2, %1z\n\tvsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %1, $vf0, $Q\n\tvdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %0, $Q" : "+j"(*pv), "=&j"(l), "=&j"(one));
}
/** @sly2-annot addr=0x1FA810
 * Purpose: Return a vector constrained by a CONSTR: kind 3 locked -> zero, 2 -> projected on
 *   (world-transformed) normal, 1 -> plane-projected
 * Info: proposed name ApplyConstr (T1); TU solve
 * Notes: verifier CORRECT: ApplySoConstraintWorld -> ApplyConstr | names_v2 v1 proposed
 *   ApplySoImpulse__FP2SOP6VECTORT1f (0.200) | CT switch exactly as Sly1 ApplyConstr (r7
 *   ApplySoConstraintWorld)
 * Confidence: high [T1 0.85] - evidence: Sly 1 / Lilly reference; static naming pass; sources:
 *   verify_r7r8+verdicts_tu_r7r8
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70
 * @end-sly2-annot */
qword ApplyConstr(struct S *ps, struct O *po, qword v)
{
    switch (ps->k) {
    case 3:
        __asm__("vsub.xyzw %0, $vf0, $vf0" : "=j"(v));
        break;
    case 2:
        if (po->plo) { qword n = ps->v; MulMVIn(&po->plo->m, &n); n = Normalize(n); v = Proj(v, n); }
        else v = Proj(v, ps->v);
        break;
    case 1:
        if (po->plo) { qword n = ps->v; MulMVIn(&po->plo->m, &n); n = Normalize(n); v = Rej(v, n); }
        else v = Rej(v, ps->v);
        break;
    }
    return v;
}
