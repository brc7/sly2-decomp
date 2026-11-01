/* func_001D4568 (Claude agent, wave-3 lane 9): hit reaction. After the base handler (func_001F7588) accepts the event, the
 * remaining count is updated (clamped at 0) and, for kind 0x100 (random hit) or 0x1000 (hit from a sender), a random linear
 * and angular impulse is built and added to the object's velocity through its virtual get/set pairs (vtable slots 33/35 and
 * 34/36).
 *
 * What decided the match (each step measured; see build/lane_notes/w3lane9.md):
 *  - The vectors are structs (VU_VECTOR4), not unions: a struct local gets its stack slot at purge_addressof time, after the
 *    expand-time VU_FLOAT temp, which gives the target frame (temp 0x00, linear 0x10, angular 0x20, direction 0x30, ...).
 *  - `linear = VecZero(); angular = linear;` (struct copy through a GPR: the target's qmfc2/sq zero init).
 *  - Named scalars (`first`, `second`) are built with the mfc1 constructor and live in memory; the VecCombo temporaries use the
 *    qmtc2 form (VuFloat), which also keeps the Random(D_002C21F8) argument load after the VecCombo.
 *  - VecCombo repeats its scalar operands (allocation only). The repeated f1 operand allows memory ("jm"): first/second are
 *    spilled pseudos, so it needs no reload. With "j" it was an extra reload that made vf5 a spill register and moved reload's
 *    round-robin, so the two final vadds got vf4/vf5 and vf1/vf2 instead of vf1/vf2 and vf3/vf4.
 */
typedef int qword __attribute__((mode(TI)));
struct VU_VECTOR4 { qword q; };
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
struct VU_FLOAT { qword q; VU_FLOAT() {} VU_FLOAT(float f) { __asm__("mfc1 %0, %1" : "=r"(q) : "f"(f)); } };
static inline VU_FLOAT VuFloat(float g) { VU_FLOAT v; __asm__("qmtc2.ni %1, %0" : "=j"(v.q) : "r"(g)); return v; }
struct VENTRY { short delta, pad; void *fn; };
struct P { char pad0[0xC]; VENTRY *vt; char pad10[0x550]; int remaining; };
struct EVENT { P *sender; char pad4[8]; int kind, change; };
extern qword D_002A2DA0, D_002A2DC0;
extern float D_002C21D0[2], D_002C21D8[2], D_002C21E0[2], D_002C21E8[2], D_002C21F0[2], D_002C21F8[2], D_002C2200[2];
extern "C" {
int func_001F7588(P *, EVENT *);
void UpdateRobottankHealthMeter(P *);
void SetRobottankState(P *, int);
float GRandInRange__Fff(float, float);
qword SetVectorCylind(float, float, float);
qword func_001A62E8(P *);
int func_001D4568(P *, EVENT *);
}
static inline VV Zero() { qword r; __asm__("vsub.xyzw %0, $vf0, $vf0" : "=j"(r)); return r; }
static inline VU_VECTOR4 VecZero() { VU_VECTOR4 v; v.q = Zero(); return v; }
static inline VV Add(VV a, VV b) { qword r; __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline VV Sub(VV a, VV b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline VV Sqrt(VV v) { qword r; __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(v)); return r; }
static inline VV Dot(VV a, VV b)
{
    qword r, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(r), "=j"(one) : "j"(a), "j"(b));
    return r;
}
static inline VV Length(VV v) { return Sqrt(Dot(v, v)); }
static inline float GFromQw(qword q) { float g; __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q)); return g; }
static inline VV DivideQ(VV v, VV l) { qword r; __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l), "j"(v)); return r; }
static inline VV NormalizeSafe(VV v, VV fallback)
{
    VV len = Length(v);
    if (GFromQw(len) < 0.0001f)
        return fallback;
    return DivideQ(v, len);
}
static inline VV Cross(VV a, VV b) { qword r; __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b)); return r; }
/* v * g with the scale (x) first */
static inline VV ScaleV(VV g, VV v) { qword r; __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(g), "j"(v)); return r; }
static inline VV operator*(VU_FLOAT g, VV v) { qword r; __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(g.q), "j"(v)); return r; }
/* f1 * v1 + f2 * v2 */
static inline VV VecCombo(VU_FLOAT f1, VV v1, VU_FLOAT f2, VV v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x"
            : "=j"(r) : "j"(f1.q), "j"(v1), "j"(f2.q), "j"(v2),
              "jm"(f1.q), "j"(f2.q)); /* last two: allocation only */
    return r;
}
static inline float Random(const float *range) { return GRandInRange__Fff(range[0], range[1]); }
/* object vector (virtual get) += delta (virtual set) */
static inline void AddVector(P *p, int get, int set, const VU_VECTOR4 &delta)
{
    VENTRY *vt = p->vt;
    VENTRY *pset = &vt[set];
    void *receiver = (char *)p + pset->delta;
    VV original = ((qword (*)(void *))vt[get].fn)((char *)p + vt[get].delta);
    ((void (*)(void *, qword))pset->fn)(receiver, Add(original, delta.q));
}

int func_001D4568(P *p, EVENT *event)
{
    if (!func_001F7588(p, event))
        return 0;
    int remaining = p->remaining + event->change;
    if (remaining < 0)
        remaining = 0;
    p->remaining = remaining;
    UpdateRobottankHealthMeter(p);
    if (!p->remaining)
        SetRobottankState(p, 3);
    VU_VECTOR4 linear, angular;
    linear = VecZero();
    angular = linear;
    switch (event->kind)
    {
    case 0x100:
    {
        float angle = GRandInRange__Fff(-3.14159274f, 3.14159274f);
        float horizontal = Random(D_002C21D0);
        float vertical = Random(D_002C21D8);
        linear.q = SetVectorCylind(angle, horizontal, vertical);
        angular.q = VuFloat(Random(D_002C21E0)) * NormalizeSafe(Cross(D_002A2DC0, linear.q), D_002A2DA0);
        break;
    }
    case 0x1000:
    {
        VV position = func_001A62E8(p);
        VV delta = Sub(position, func_001A62E8(event->sender));
        VU_VECTOR4 direction;
        direction.q = NormalizeSafe(Sub(delta, ScaleV(Dot(delta, D_002A2DC0), D_002A2DC0)), D_002A2DA0);
        VU_FLOAT first(Random(D_002C21E8));
        linear.q = VecCombo(first, direction.q, VuFloat(Random(D_002C21F0)), D_002A2DC0);
        VU_FLOAT second(Random(D_002C21F8));
        VU_VECTOR4 side;
        side.q = Cross(D_002A2DC0, direction.q);
        angular.q = VecCombo(second, side.q, VuFloat(Random(D_002C2200)), D_002A2DC0);
        break;
    }
    }
    AddVector(p, 33, 35, linear);
    AddVector(p, 34, 36, angular);
    return 1;
}
