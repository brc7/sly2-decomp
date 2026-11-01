// UpdateRobottankAi (Claude agent, wave-4 lane 7): MATCH. Mode-1 update of a vehicle with attached weapons (called from
// UpdateRobottank): pick a ready attachment (func_001D2240) or a target object (func_001D22D0), run the state machine
// (0 approach/orbit the target, 1 attack in place, 2 pursue the object, 3 back away) until the state is stable, steer
// for the current state, then let every ready attachment fire once its aim error is small enough.
// Callees: 0x2456F0 acosf, 0x2458F0 atan2f, 0x11D910 FCheckLm, 0x11CF60 RadNormalize, 0x11D448 GRandInRange,
// 0x11B500 rotation matrix about an axis.
// How it matches (fresh rewrite; notes in build/lane_notes/w4lane7.md):
// - position/target/delta/goal/offset are VU-class pseudos that live across calls, so reload spills them to
//   0x1C0..0x200 in pseudo-number order (goal is declared before offset for that reason).
// - Length2D lists its input twice (vmul.xy %0,%1,%2): reload then gives the two inputs separate registers, the
//   target's `vmul.xy vf1, vf1, vf3` with delta stored and reloaded. Its two extra "jm" operands are allocation-only:
//   they tip regclass so that delta (read four times through "r" by GetX/GetY) stays VU-class and is spilled, as in
//   the target, instead of getting $s1.
// - GetY returns the prot3w result as a float in a GPR (like func_001D2490): gcc's own mtc1 into $f12 is not an asm,
//   so reorg can still fill the atan2f delay slot with the goalHeading store.
// - The radius scalar goes through Lilly's VuFloatFromMemWord ("rV": reload makes the lw at the qmtc2), its VU_FLOAT
//   temporary takes slot 0x00, the case-2 matrix 0x10, the case-3 matrix the coalesced 0x00, and the HIT array
//   (declared after the tactic switch) 0x40.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
struct M3 { U rows[3]; };
struct LM { float min, max; };
struct CLOCK { float t, dt; };
struct BINDING { char pad0[0x28]; float pitch; };
struct P;
struct ATTACH {
    char pad0[0x20]; void *binding; char pad24[0xC]; void *mesh; BINDING *meshBinding; int pad38; P *anchor;
    char pad40[0x3C]; float radius; int tactic; void *constraint; LM enterRange, exitRange, fireInterval;
};
struct PATH { char pad0[0xE8]; int count; int padEC, padF0; U *points; };
struct HIT { U start, end, normal0, normal1; float startFraction, endFraction; char pad48[0x18]; };
struct P {
    char pad0[0x188]; int state; char pad18C[0x36C]; int attachCount; ATTACH *attachments[8]; float slip;
    char pad520[0x20]; float heading, goalHeading, steering, wheelAngle, drive; char pad554[0x50];
    P *target; void *navigator; PATH *path; float cursor, nextAttack; P *selectedObject; ATTACH *selectedAttach;
};
extern CLOCK D_002DDED0;
extern const qword D_002A2DA0, D_002A2DC0;
extern float D_002C20E8, D_002C20EC, D_002C20F0, D_002C20F4, D_002C20F8;
extern "C" {
qword func_001A62E8(P *);
ATTACH *func_001D2240(P *);
P *func_001D22D0(P *);
int func_0011D910(LM *, float);
void func_001D4B28(P *, int);
float func_002456F0(float);
M3 func_0011B500(qword, float);
int ClsgClipPathzone(void *, qword, qword, int, HIT *, int);
void MeasureShape(PATH *);
void func_001D2420(P *);
void func_001C02D0(void *, qword, qword, PATH *);
float func_002458F0(float, float);
M3 *func_001A6318(P *);
qword func_0022CA18(void *, void *);
float RadNormalize__Ff(float);
void SetSmaGoal(void *, int);
float GRandInRange__Fff(float, float);
void func_001D2490(P *);
int func_001D4E08(ATTACH *);
int FCheckVolPoint(void *, qword);
}
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
struct VU_FLOAT { qword q; };
static inline VV Sub(VV a, VV b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline VV Add(VV a, VV b) { qword r; __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline float GetX(VV q) { float f; __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q)); return f; }
static inline float GetY(VV q) { float f; __asm__("prot3w %0, %1" : "=r"(f) : "r"(q)); return f; }
static inline float GetZ(VV q) { float z; __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(q)); return z; }
static inline float Length2D(VV v)
{
    qword r;
    __asm__("vmul.xy %0, %1, %2\n\tvaddy.x %0, %0, %0y"
            : "=j"(r) : "j"(v), "j"(v), "jm"(v.q), "jm"(v.q)); /* last two: allocation only (keep delta VU-class) */
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "+j"(r));
    return GetX(r);
}
static inline float Dist2D(VV a, VV b) { return Length2D(Sub(a, b)); }
static inline VV Dot(VV a, VV b)
{
    qword r, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(r), "=j"(one) : "j"(a), "j"(b));
    return r;
}
static inline VV Sqrt(VV v) { qword r; __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(v)); return r; }
static inline VV DivideQ(VV v, VV l) { qword r; __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l), "j"(v)); return r; }
static inline VV NormalizeSafe(VV v, VV fallback)
{
    VV len = Sqrt(Dot(v, v));
    if (GetX(len) < 0.0001f)
        return fallback;
    return DivideQ(v, len);
}
static inline VU_FLOAT VuFloatFromMemWord(int n) { VU_FLOAT v; __asm__("qmtc2.ni %1, %0" : "=j"(v.q) : "rV"(n)); return v; }
static inline VV operator*(VU_FLOAT g, VV v) { qword r; __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(g.q), "j"(v)); return r; }
static inline VV Rotate(const M3 &m, VV v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r) : "j"(m.rows[0].q), "j"(m.rows[1].q), "j"(m.rows[2].q), "j"(v));
    return r;
}
static inline float Max(float a, float b) { return a < b ? b : a; }
static inline float Abs(float f) { return __builtin_fabsf(f); }

extern "C" void UpdateRobottankAi(P *p)
{
    VV position = func_001A62E8(p);
    VV target = func_001A62E8(p->target);
    VV delta = Sub(target, position);
    float distance = Length2D(delta);
    p->selectedAttach = func_001D2240(p);
    if (!p->selectedAttach)
        p->selectedObject = func_001D22D0(p);
    else
        p->selectedObject = 0;
    int state;
    do {
        state = p->state;
        switch (state) {
        case 0:
            if (p->selectedAttach) {
                if (func_0011D910(&p->selectedAttach->enterRange, distance))
                    func_001D4B28(p, 1);
            } else if (p->selectedObject) {
                func_001D4B28(p, 2);
            } else {
                func_001D4B28(p, 3);
            }
            break;
        case 1:
            if (!p->selectedAttach || !func_0011D910(&p->selectedAttach->exitRange, distance))
                func_001D4B28(p, 0);
            break;
        case 2:
            if (!p->selectedObject)
                func_001D4B28(p, 0);
            break;
        case 3:
            if (p->selectedAttach || p->selectedObject)
                func_001D4B28(p, 0);
            break;
        }
    } while (state != p->state);

    switch (p->state) {
    case 0: {
        // approach: a point at the attachment's radius from the target, rotated around it by the tactic
        float radius = p->selectedAttach->radius;
        float angle = func_002456F0(radius / Max(radius * D_002C20E8, distance));
        VV goal;
        VV offset = VuFloatFromMemWord(*(int *)&p->selectedAttach->radius) * NormalizeSafe(Sub(position, target), D_002A2DA0);
        switch (p->selectedAttach->tactic) {
        case 0:
        case 1:
            goal = target;
            break;
        case 2: {
            M3 m = func_0011B500(D_002A2DC0, angle);
            goal = Add(target, Rotate(m, offset));
            break;
        }
        case 3: {
            M3 m = func_0011B500(D_002A2DC0, -angle);
            goal = Add(target, Rotate(m, offset));
            break;
        }
        case 4:
            break;
        }
        HIT hits[4];
        if (ClsgClipPathzone(p->navigator, position, goal, 4, hits, 2) == 1 &&
            hits[0].startFraction == 0.0f && hits[0].endFraction == 1.0f) {
            // straight line to the goal
            p->path->points[0].q = position;
            p->path->points[1].q = goal;
            p->path->count = 2;
            MeasureShape(p->path);
            func_001D2420(p);
        } else if (p->path->count < 2 || 50.0f < Dist2D(p->path->points[p->path->count - 1].q, target)) {
            func_001C02D0(p->navigator, position, target, p->path);
            func_001D2420(p);
        }
        func_001D2490(p);
        p->wheelAngle = p->heading;
        break;
    }
    case 1: {
        // attack: stop, turn the wheels towards the target, fire when the selected attachment is aimed
        p->drive = 0.0f;
        p->goalHeading = p->heading;
        p->wheelAngle = func_002458F0(GetY(delta), GetX(delta));
        if (p->nextAttack < D_002DDED0.t) {
            ATTACH *attach = func_001D2240(p);
            if (attach) {
                if (attach->mesh) {
                    M3 m = *func_001A6318(attach->anchor);
                    float aim = func_002458F0(m.rows[0].f[1], m.rows[0].f[0]);
                    qword packed = func_0022CA18(attach->mesh, attach->mesh);
                    float error = Abs(RadNormalize__Ff(aim - p->wheelAngle));
                    float pitch = GetZ(packed) - attach->meshBinding->pitch;
                    if (error < D_002C20EC && Abs(pitch) < D_002C20F0)
                        goto fire;
                    break;
                }
            fire:
                SetSmaGoal(attach->binding, 0x68);
                float t = D_002DDED0.t;
                p->nextAttack = t + GRandInRange__Fff(1.0f, 1.5f);
            }
        }
        break;
    }
    case 2: {
        VV object = func_001A62E8(p->selectedObject);
        if (p->path->count < 2 || 50.0f < Dist2D(p->path->points[p->path->count - 1].q, object)) {
            func_001C02D0(p->navigator, position, object, p->path);
            func_001D2420(p);
        }
        func_001D2490(p);
        p->wheelAngle = p->heading;
        break;
    }
    case 3:
        p->goalHeading = func_002458F0(-GetY(delta), -GetX(delta));
        p->wheelAngle = p->heading;
        p->drive = p->slip * 0.85f;
        break;
    }

    if (D_002DDED0.t > p->nextAttack) {
        for (int i = 0; i < p->attachCount; i++) {
            ATTACH *attach = p->attachments[i];
            if (!func_001D4E08(attach))
                continue;
            if (attach->constraint && !FCheckVolPoint(attach->constraint, target))
                continue;
            if (0.0f < attach->enterRange.max && p->state != 1)
                continue;
            if (attach->mesh) {
                M3 m = *func_001A6318(attach->anchor);
                float aim = func_002458F0(m.rows[0].f[1], m.rows[0].f[0]);
                qword packed = func_0022CA18(attach->mesh, attach->mesh);
                float error = Abs(RadNormalize__Ff(aim - p->wheelAngle));
                float pitch = GetZ(packed) - attach->meshBinding->pitch;
                if (D_002C20F4 < error || D_002C20F8 < Abs(pitch))
                    continue;
            }
            SetSmaGoal(attach->binding, 0x68);
            float t = D_002DDED0.t;
            p->nextAttack = t + GRandInRange__Fff(attach->fireInterval.min, attach->fireInterval.max);
        }
    }
}
