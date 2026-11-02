// func_0014C748 MATCH (Claude agent, lane K2, 2026-10-24), no register pins, VU helper asms only. Fresh rewrite (old v12 draft 10).
// Keys: (1) the three dots as mtc1-from-VF (GXOfVf with an allocation-only "j") instead of a qmfc2 "=r"(float) asm: gcc emits the
// qmfc2 itself, which gives the target's prologue schedule and VF numbering; (2) `float a = Dot(dir, dir);` declared BEFORE b
// (no function-top a/b): a's pseudo is then lower than b's, so global-alloc gives a f4 / b f5 (equal priority, lower number first);
// (3) by-value SubQ for the prologue subtracts and const-ref Sub in the loop (0x20/0x30 homes); (4) D_002A2DA0 read into a local
// before the Normalize test (lq + qmtc2 in the b slot); (5) the loop's Sub before the apos store.
typedef int qword __attribute__((mode(TI)));
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct LSG { qword apos[2]; qword anormal[2]; float au[2]; int lsgk; };
extern "C" {
extern qword D_002A2DA0;
void InitPlaneFromNormalPos(qword *pnormal, qword normal, qword pos);
int func_0014C748(qword posCenter, float sRadius, qword pos0, qword pos1, int clsgMax, LSG *alsg);
}

static inline float SqrtF(float g) { float r; __asm__ __volatile__("sqrt.s %0, %1" : "=&f"(r) : "f"(g)); return r; }
static inline float GMin(float a, float b) { return b < a ? b : a; }
static inline float GMax(float a, float b) { return a > b ? a : b; }
static inline qword SubQ(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Sub(const qword &a, const qword &b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Combo(const VU_FLOAT &f1, const qword &v1, const VU_FLOAT &f2, const qword &v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1), "j"(f2.q), "j"(v2));
    return r;
}
static inline qword QwDot(const qword &a, const qword &b)
{
    qword d, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    return d;
}
static inline qword QwSqrt(const qword &a) { qword r; __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(a)); return r; }
static inline qword QwDivide(const qword &v, const qword &l) { qword r; __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(r) : "j"(v), "j"(l)); return r; }
static inline float GXOfQw(const qword &q) { float x; __asm__("mtc1 %1, %0" : "=f"(x) : "r"(q)); return x; }
static inline float GXOfVf(const qword &q) { float x; __asm__("mtc1 %1, %0" : "=f"(x) : "r"(q), "j"(q)); return x; } /* allocation-only "j": the value stays in a VF register */
static inline float Dot(const qword &a, const qword &b) { return GXOfVf(QwDot(a, b)); }
static inline qword Normalize(const qword &v)
{
    qword l = QwSqrt(QwDot(v, v));
    qword k = D_002A2DA0;
    qword r;
    if (GXOfQw(l) < 1e-4f)
        r = k;
    else
        r = QwDivide(v, l);
    return r;
}

/** @sly2-annot addr=0x14C748
 * Purpose: Segment/sphere intersection: quadratic in u, clamp to [0,1], emit up to 2 LSG entries
 * Info: proposed name ClsgClipEdgeToSphere (T1); TU clip
 * Notes: names_v2 C proposed ClsgClipLineToSphere__FP6VECTORfT0T0iP3LSG (0.638)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r2b_rest
 * See: docs/engine/physics-collision.md#4-geometry-and-queries,
 *   docs/reference/functions/physics-collision.md#clip-0x14c6a0-0x14cfe0
 * @end-sly2-annot */
int func_0014C748(qword posCenter, float sRadius, qword pos0, qword pos1, int clsgMax, LSG *alsg)
{
    float au[2];
    if (clsgMax <= 0)
        return 0;
    qword dpos = SubQ(pos0, posCenter);
    float c = Dot(dpos, dpos) - sRadius * sRadius;
    qword dir = SubQ(pos1, pos0);
    float a = Dot(dir, dir);
    float b = 2.0f * Dot(dir, dpos);
    float disc = b * b - 4.0f * a * c;
    if (disc < 1e-4f)
        return 0;
    disc = SqrtF(disc);
    {
        float u0 = (-b - disc) / (2.0f * a);
        float u1 = (-b + disc) / (2.0f * a);
        au[0] = u0;
        au[1] = u1;
        if (u0 < 0.0f && u1 < 0.0f)
            return 0;
        if (u0 > 1.0f && u1 > 1.0f)
            return 0;
    }
    {
        float t0 = au[0];
        float t1 = au[1];
        t1 = GMin(1.0f, t1);
        t0 = GMax(t0, 0.0f);
        au[1] = t1;
        au[0] = t0;
    }
    for (int i = 0; i < 2; i++) {
        float u = au[i];
        alsg->au[i] = u;
        qword pos = Combo(VU_FLOAT(u), pos1, VU_FLOAT(1.0f - u), pos0);
        qword d = Sub(pos, posCenter);
        alsg->apos[i] = pos;
        InitPlaneFromNormalPos(&alsg->anormal[i], Normalize(d), pos);
    }
    alsg->lsgk = 1;
    return 1;
}
