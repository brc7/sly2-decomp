/* func_001A4258 (Claude agent, wave-3 lane 9): does a sphere (pos, sRadius) touch a chain of spheres? First every sphere of the
 * chain is tested on its own, then the capsule-like hull between each adjacent pair: the common tangent line of the two circles
 * (normal n = u * s + perp * sqrt(1 - s*s) with u the unit axis, s = (r0 - r1) / |axis|) gives the tangent points A and B; the
 * point touches when its distance to the tangent is below sRadius and its projection onto AB falls inside the segment.
 *
 * What decided the match (each step measured; see build/lane_notes/w3lane9.md):
 *  - VV (qword wrapper with a converting ctor) as the helpers' return type: the struct copies survive until loop.c and make the
 *    pair loop big enough (218 RTL insns) that the 0.0f of the last test is not hoisted, while 1.0f still is.
 *  - ToVF(pos) inside each loop (loop.c hoists it into each preheader; pos itself stays in $a0).
 *  - The volatile sqrt.s is a scheduling barrier: with root computed as its own statement before the VecCombo, all scalar
 *    broadcasts (VU_FLOAT) come after it, which orders the FPU->VU0 transfers through $v0 like the target.
 *  - Length = Sqrt(Dot(v, v)) (the two-operand dot), the num/den/dist statement order, and ONE function-level sDist shared by both
 *    loops (a global pseudo: the radius sum gets $f0, the distance $f1).
 *  - Floats are taken out of VU0 two ways: GFromVf (qmfc2 asm, float in a GPR) and GFromQw (mtc1 asm, the qmfc2 is a reload
 *    on the VU0 unit, which is what delays the last dead VU_FLOAT store in the pair loop).
 */
typedef int qword __attribute__((mode(TI)));
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct SPHERE { qword pos; float sRadius; int pad[3]; };

static inline VV ToVF(qword v) { qword r; __asm__("qmtc2.ni %1, %0" : "=j"(r) : "r"(v)); return r; }
static inline VV Sub(VV a, VV b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline float GFromVf(VV v) { float g; __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(v)); return g; }
static inline float GFromQw(qword q) { float g; __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q)); return g; }
static inline VV Sqrt(VV v) { qword r; __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(v)); return r; }
static inline VV Dot(VV a, VV b)
{
    qword r, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(r), "=j"(one) : "j"(a), "j"(b));
    return r;
}
static inline VV Length(VV v) { return Sqrt(Dot(v, v)); }
static inline VV DivideQ(VV v, VV l) { qword r; __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l), "j"(v)); return r; }
static inline VV Divide(VV v, VU_FLOAT g) { qword r; __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(g.q), "j"(v)); return r; }
static inline VV Normalize(VV v) { return DivideQ(v, Length(v)); }
static inline VV Cross(VV a, VV b) { qword r; __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b)); return r; }
/* f1 * v1 + f2 * v2 */
static inline VV VecCombo(VU_FLOAT f1, VV v1, VU_FLOAT f2, VV v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1), "j"(f2.q), "j"(v2));
    return r;
}
/* base + v * g */
static inline VV AddScaled(VV base, VU_FLOAT g, VV v)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %3, %2x" : "=j"(r) : "j"(base), "j"(g.q), "j"(v));
    return r;
}
static inline float SqrtF(float g) { float r; __asm__ __volatile__("sqrt.s %0, %1" : "=&f"(r) : "f"(g)); return r; }

/** @sly2-annot addr=0x1A4258
 * Purpose: True if a sphere overlaps any of a list of spheres (or the capsules between consecutive ones)
 * Info: proposed name FSphereHitsSpheres (T1); TU light
 * Notes: names_v2 B proposed TwpsFindSwLights__FP2SWP6VECTORfiiPiT5PP5LIGHT (0.950)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r4b_rest+verdicts_followups
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
extern "C" int func_001A4258(qword pos, float sRadius, int csphere, SPHERE *asphere)
{
    int i;
    float sDist;
    for (i = 0; i < csphere; i++)
    {
        sDist = GFromQw(Length(Sub(ToVF(pos), asphere[i].pos)));
        if (sDist < sRadius + asphere[i].sRadius)
            return 1;
    }
    for (i = 1; i < csphere; i++)
    {
        SPHERE *ps0 = &asphere[i - 1];
        SPHERE *ps1 = &asphere[i];
        VV p = ToVF(pos);
        VV dpos = Sub(p, ps0->pos);
        VV axis = Sub(ps1->pos, ps0->pos);
        VV len = Length(axis);
        VV c1 = Cross(dpos, axis);
        float sLen = GFromVf(len);
        VV u = Divide(axis, VU_FLOAT(sLen));
        float s = (ps0->sRadius - ps1->sRadius) / sLen;
        VV perp = Normalize(Cross(u, c1));
        float root = SqrtF(1.0f - s * s);
        VV n = VecCombo(VU_FLOAT(s), u, VU_FLOAT(root), perp);
        VV posA = AddScaled(ps0->pos, VU_FLOAT(ps0->sRadius), n);
        VV pa = Sub(p, posA);
        VV posB = AddScaled(ps1->pos, VU_FLOAT(ps1->sRadius), n);
        VV ba = Sub(posB, posA);
        VV num = Dot(pa, ba);
        VV den = Dot(ba, ba);
        sDist = GFromQw(Dot(pa, n));
        float t = GFromQw(num) / GFromVf(den);
        if (sDist < sRadius && t >= 0.0f && t <= 1.0f)
            return 1;
    }
    return 0;
}
