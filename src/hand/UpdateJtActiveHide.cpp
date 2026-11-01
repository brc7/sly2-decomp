// UpdateJtActiveHide: per-frame update of a hero's movement/action context (the struct at actor+0x4C0; called from the
// actor's mode handler, see UpdateJtPre / UpdateBentley). Dispatches on the context's action, then commits the
// animation / axis weights for the resulting step.
//
// Matched from w5lane7's near-miss (3/953). Notes on the forms that matter (each verified against the target):
// - GLength takes a VU_VECTOR by value: the inlined parameter copy is a store to an ADDRESSOF slot during cse1
//   (it may alias ctx->actor), and a plain register copy after purge_addressof, so only cse2 sees the case-0 lean
//   test's ctx->actor reload as the earlier one. That leaves the target's `daddu v0,v1` copy in the vy test.
// - QwDiv: vdiv as its own volatile no-output asm, so jump.c cannot hoist the Normalize fallback copy.
// - GFromVf: one mtc1 asm from a TI qword (gcc emits the qmfc2 itself and schedules it).
// - SqrtF: the usual volatile "=&f" sqrt.s helper (local-alloc must not tie it to its input).
// - case 2 falls through into case 4; slow-path step choice is a ternary (blocks thread_jumps).
typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;

union U { qword q; float f[4]; int i[4]; };
struct M3 { U r[3]; };
struct VTE { short delta, pad; void *fn; };
struct JOY { char pad0[0x10]; float x, y, heading, amount; };
struct ANIM { char pad0[0x14]; void *table; char pad18[4]; float phase; char pad20[0x20]; float rate; };
struct COMPONENT { VTE *vt; };
struct BINDING { int key; COMPONENT *component; };
struct SMP { float svFast, svSlow, dtFast; };
struct HEIGHT { float min, max; };
struct ENTRY { float weight; int active; char pad8[8]; U direction; };
struct PROGRAM { void *owner; void *clip; char pad8[0x18]; float x20; char pad24[0x3C]; float x60; char pad64[0x40]; };
struct FOCUS { char pad0[0xE4]; int cyclic; char padE8[0x18C - 0xE8]; int enabled; char pad190[0x1BC - 0x190]; uint flags; char pad1C0[8]; FOCUS *other; };
struct ACTOR {
    char pad0[0xC]; VTE *vt; char pad10[0x150 - 0x10]; JOY *pjoy; char pad154[0x190 - 0x154]; U velocity;
    char pad1A0[8]; float heading; char pad1AC[4]; M3 local; char pad1E0[0x218 - 0x1E0]; qword *ppos;
    char pad21C[0x2C0 - 0x21C]; char control[0x1024 - 0x2C0]; void *x1024; char pad1028[0x1030 - 0x1028]; ANIM *anim;
    char pad1034[0x1040 - 0x1034]; int role; char pad1044[0x1520 - 0x1044]; char x1520[0x100]; char x1620[0x19D4 - 0x1620];
    void *x19D4, *x19D8, *x19DC, *x19E0; char pad19E4[0x19F0 - 0x19E4]; void *x19F0, *x19F4;
};
struct CONTEXT {
    ACTOR *actor; float stamp; int action; int step; float tHold; FOCUS *focus; void *setting; float parameter;
    float value; int reversed; int index; int x2C; char pad30[0x50 - 0x30]; float maximum; char pad54[0x68 - 0x54];
    void *x68; char pad6C[4]; ENTRY axes[8]; void *request; char pad174[0x180 - 0x174]; PROGRAM program180;
    char pad224[0x240 - 0x224]; PROGRAM program240; char pad2E4[0x330 - 0x2E4]; ENTRY axes2[8]; void *request2;
    char pad434[0x440 - 0x434]; char sub[0x10];
};
struct CLOCK { float t, dt; };
struct VU_FLOAT { qword q; };

extern "C" {
extern CLOCK D_002DDED0;
extern ACTOR *D_002E1E40;
extern qword D_002A2DA0;                    // (1, 0, 0, 0)
extern float D_002BF5BC, D_002BF5C0, D_002BF5C4, D_002BF5C8, D_002BF5CC;
extern HEIGHT D_002BF630;
extern U D_002BF5D0[3], D_002BF600[3];
qword func_001A62E8(ACTOR *);
void func_00182CD8(void *, ACTOR *, float *, U *, float *);
qword func_0011F088(qword, qword, float, SMP *, U *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
qword SetVectorCylind(float, float, float);
float SMinShape(FOCUS *);
float SMaxShape(FOCUS *);
float UMinShape(FOCUS *);
float UMaxShape(FOCUS *);
float GSmooth__FfffP3SMPPf(float, float, float, SMP *, float *);
void func_001940B0(CONTEXT *, int);
void func_00204660(ACTOR *, float);
float RadStepHeading(ACTOR *);
float RadNormalize__Ff(float);
void SetJtGrabKind(CONTEXT *, int);
void UpdateGaitBlend(PROGRAM *, float);
void UpdateGaitFootsteps(PROGRAM *);
void DrainStepplayerMp(ACTOR *, float);
void FindShapeClosestPointFromU(FOCUS *, qword, void *, U *, U *, float *, float *, float);
void func_00195A00(void *, int, int, int);
float func_002458F0(float, float);
void FindShapeClosestPoint(FOCUS *, qword, void *, void *, U *, void *, void *);
void UpdateJtHands(void *);
void func_001A0450(ACTOR *, ENTRY *, void *);
void func_00205910(ACTOR *, HEIGHT *, int, U *);
BINDING *func_0012FF10(void *, ACTOR *, int, int);
void FaceStepToward(ACTOR *, FOCUS *);
void FixStepAngularVelocity(ACTOR *);
void RebuildJtXmg(ACTOR *, void *, void *, void *, float, void *);
void func_00193C98(ACTOR *, void *, void *);
}

static inline qword GetVelocity(ACTOR *p) { VTE &v = p->vt[33]; return ((qword (*)(void *))v.fn)((char *)p + v.delta); }
static inline void ReadBinding(BINDING *b, ANIM *a, U *out)
{
    COMPONENT *c = b->component;
    VTE &v = c->vt[3];
    ((void (*)(void *, int, U *, float, float))v.fn)((char *)c + v.delta, 0, out, a->phase, a->rate);
}

static inline VU_FLOAT VuFloat(float g) { VU_FLOAT v; __asm__("qmtc2.ni %1,%0" : "=j"(v.q) : "r"(g)); return v; }
static inline qword QwZero() { qword r; __asm__("vsub.xyzw %0,$vf0,$vf0" : "=j"(r)); return r; }
static inline qword QwAdd(qword a, qword b) { qword r; __asm__("vadd.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword QwScale(qword a, VU_FLOAT g) { qword r; __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(r) : "j"(a), "j"(g.q)); return r; }
static inline qword QwScaleX(qword a, qword g) { qword r; __asm__("vmulx.xyzw %0,%2,%1x" : "=j"(r) : "j"(g), "j"(a)); return r; }
static inline float GFromVf(qword v) { float g; __asm__("mtc1 %1,%0" : "=f"(g) : "r"(v)); return g; }
static inline qword QwDot(qword a, qword b)
{
    qword d, one;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    return d;
}
static inline qword QwSqrt(qword d) { qword r; __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "=j"(r) : "j"(d)); return r; }
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
static inline float GLength(VU_VECTOR v) { return GFromVf(QwSqrt(QwDot(v.q, v.q))); }
static inline float GLengthXY(qword v)
{
    qword l;
    __asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y" : "=j"(l) : "j"(v));
    return GFromVf(QwSqrt(l));
}
static inline qword QwDiv(qword v, qword l)
{
    qword r;
    __asm__ __volatile__("vdiv $Q,$vf0w,%0x\n\tvwaitq" : : "j"(l));
    __asm__("vmulq.xyzw %0,%1,$Q" : "=j"(r) : "j"(v));
    return r;
}
static inline qword QwNormalize(qword v, qword fallback)
{
    qword l = QwSqrt(QwDot(v, v));
    if (GFromVf(l) < 0.0001f)
        return fallback;
    return QwDiv(v, l);
}
// M * v (rows of M dotted with v); w comes from row 0
static inline qword QwMulM3(M3 *pm, qword v)
{
    qword r = pm->r[0].q;
    qword t0, t1, t2, one;
    __asm__("vmul.xyzw %2,%0,%5\n\tvmaxw.xyzw %1,$vf0,$vf0w\n\tvmul.xyzw %3,%6,%5\n\tvmul.xyzw %4,%7,%5\n\tvadday.x $ACC,%2,%2y\n\tvmaddz.x %0,%1,%2z\n\tvaddax.y $ACC,%3,%3x\n\tvmaddz.y %0,%1,%3z\n\tvaddax.z $ACC,%4,%4x\n\tvmaddy.z %0,%1,%4y"
            : "+j"(r), "=&j"(one), "=&j"(t0), "=&j"(t1), "=&j"(t2) : "j"(v), "j"(pm->r[1].q), "j"(pm->r[2].q));
    return r;
}
static inline void SetXYZ(U *p, float x, float y, float z) { p->f[0] = x; p->f[1] = y; p->f[2] = z; }
static inline float SqrtF(float x) { float r; __asm__ __volatile__("sqrt.s %0,%1" : "=&f"(r) : "f"(x)); return r; }
static inline float GClamp(float g, float lo, float hi) { if (g < lo) return lo; else if (hi < g) return hi; else return g; }
static inline float GMin(float a, float b) { return b < a ? b : a; }
static inline float GMax(float a, float b) { return a < b ? b : a; }

extern "C" void UpdateJtActiveHide(CONTEXT *ctx)
{
    U pos;
    U velOld;
    pos.q = func_001A62E8(ctx->actor);
    velOld.q = GetVelocity(ctx->actor);
    JOY *pjoy = ctx->actor->pjoy;

    switch (ctx->action) {
    case 0:
    case 13: {
        U goal;
        float heading;
        func_00182CD8(ctx->setting, ctx->actor, &ctx->value, &goal, &heading);
        float speedMax;
        if (ctx->actor->role == 1)
            speedMax = GLength(ctx->axes[2].direction.q) * ctx->actor->anim->rate;
        else
            speedMax = GLength(ctx->axes[4].direction.q) * ctx->actor->anim->rate;
        goal.f[2] = pos.f[2];
        SMP smp;
        smp.svFast = speedMax;
        smp.svSlow = 0.0f;
        smp.dtFast = 0.3f;
        U vel;
        func_0011F088(pos.q, goal.q, D_002DDED0.dt, &smp, &vel);
        float lateral;
        qword offset;
        if (FIsBasicDerivedFrom__FP5BASIC3CID(ctx->focus, 38)) {
            offset = QwZero();
            lateral = 0.0f;
        } else {
            U side;
            float angle;
            if (ctx->actor->role == 1)
                angle = heading - 1.5707964f;
            else
                angle = heading + 1.5707964f;
            side.q = SetVectorCylind(angle, 1.0f, 0.0f);
            U input;
            input.q = SetVectorCylind(pjoy->heading, 1.0f, 0.0f);
            float g = GClamp(D_002BF5BC * GFromVf(QwDot(side.q, input.q)), -1.0f, 1.0f);
            g *= pjoy->amount;
            if (__builtin_fabsf(g) < 0.2f)
                g = 0.0f;
            lateral = g * speedMax;
            lateral = GMin(SqrtF(speedMax * speedMax - GFromVf(QwDot(vel.q, vel.q))), lateral);
            float s;
            FOCUS *focus;
            if (FIsBasicDerivedFrom__FP5BASIC3CID(ctx->focus, 12) && !(focus = ctx->focus)->cyclic) {
                float lo, hi;
                GSmooth__FfffP3SMPPf(ctx->value, SMinShape(focus) + 30.0f, D_002DDED0.dt, &smp, &lo);
                GSmooth__FfffP3SMPPf(ctx->value, SMaxShape(focus) - 30.0f, D_002DDED0.dt, &smp, &hi);
                s = GClamp(lateral, lo, hi);
            } else {
                s = lateral;
            }
            offset = QwScale(side.q, VuFloat(s));
        }
        ACTOR *actor = ctx->actor;
        actor->velocity.q = QwScale(QwMulM3(&actor->local, QwAdd(vel.q, offset)), VuFloat(-1.0f));
        int step;
        if (25.0f < GLength(ctx->actor->velocity.q)) {
            step = 1;
            if (ctx->action == 0) {
                if (45.0f < GLength(vel.q)) {
                    func_001940B0(ctx, 0);
                } else if (20.0f < ctx->actor->velocity.f[1]) {
                    if (ctx->actor->role == 1)
                        func_001940B0(ctx, 1);
                    else
                        func_001940B0(ctx, 2);
                } else if (ctx->actor->velocity.f[1] < -20.0f) {
                    if (ctx->actor->role == 1)
                        func_001940B0(ctx, 2);
                    else
                        func_001940B0(ctx, 1);
                }
            }
        } else {
            step = 0;
            if (FIsBasicDerivedFrom__FP5BASIC3CID(ctx->focus, 12)) {
                FOCUS *focus = ctx->focus;
                if (lateral < -25.0f && (focus->flags & 1)) {
                    step = ctx->action == 0 && ctx->actor->role == 0 ? 3 : 2;
                } else if (25.0f < lateral && (focus->flags & 2)) {
                    step = ctx->action == 0 && ctx->actor->role == 0 ? 2 : 3;
                }
            }
            if (ctx->action == 0)
                func_001940B0(ctx, 3);
        }
        func_00204660(ctx->actor, heading);
        if (ctx->step == 5 && 0.25f < __builtin_fabsf(RadNormalize__Ff(RadStepHeading(ctx->actor) - heading)))
            step = 5;
        SetJtGrabKind(ctx, step);
        break;
    }
    case 1:
        if (ctx->focus->enabled && 0.0f < pjoy->amount && ctx->program180.clip) {
            SetJtGrabKind(ctx, 4);
            UpdateGaitBlend(&ctx->program180, pjoy->amount);
            func_00204660(ctx->actor, ctx->actor->pjoy->heading);
            UpdateGaitFootsteps(&ctx->program180);
        } else {
            SetJtGrabKind(ctx, 0);
        }
        break;
    case 2:
        DrainStepplayerMp(ctx->actor, D_002BF5C0);
        if (ctx->step == 0 ? 0.1f < pjoy->amount : 0.0f < pjoy->amount) {
            SetJtGrabKind(ctx, 4);
            UpdateGaitBlend((PROGRAM *)((char *)ctx->actor + 0x1C30), pjoy->amount * D_002BF5C4);
        } else {
            SetJtGrabKind(ctx, 0);
            break;
        }
        // fall through: face the stick
    case 4:
        func_00204660(ctx->actor, pjoy->heading);
        break;
    case 3: {
        FOCUS *focus = ctx->focus;
        U point, tangent;
        FindShapeClosestPointFromU(focus, *ctx->actor->ppos, 0, &point, &tangent, &ctx->parameter, &ctx->value, ctx->parameter);
        if (focus->flags && !focus->cyclic &&
            (ctx->parameter < UMinShape(focus) + 0.01f || UMaxShape(focus) - 0.01f < ctx->parameter)) {
            func_00195A00(ctx->actor->control, 0, 0, 0);
            break;
        }
        tangent.f[2] = 0.0f;
        tangent.q = QwNormalize(tangent.q, D_002A2DA0);
        float heading = func_002458F0(tangent.f[1], tangent.f[0]);
        U dir;
        U input;
        input.q = SetVectorCylind(pjoy->heading, 1.0f, 0.0f);
        if (!focus->other)
            dir = tangent;
        else
            FindShapeClosestPoint(focus->other, *ctx->actor->ppos, 0, 0, &dir, 0, 0);
        dir.f[2] = 0.0f;
        dir.q = QwNormalize(dir.q, D_002A2DA0);
        float g = GFromVf(QwDot(input.q, dir.q));
        switch (ctx->index) {
        case 0:
            if (pjoy->amount < 0.25f)
                ctx->index = -1;
            else if (g < -0.7f)
                ctx->index = 1;
            break;
        case 1:
            if (pjoy->amount < 0.25f)
                ctx->index = -1;
            else if (0.7f < g)
                ctx->index = 0;
            break;
        case -1:
            if (0.35f < pjoy->amount) {
                if (0.0f <= g)
                    ctx->index = 0;
                else
                    ctx->index = 1;
            }
            break;
        }
        switch (ctx->index) {
        case 0:
            ctx->reversed = 0;
            break;
        case 1:
            ctx->reversed = 1;
            break;
        }
        if (focus->flags) {
            g = 0.0f;
        } else {
            SMP smp;
            float limit;
            smp.svSlow = ctx->program240.x20;
            smp.svFast = ctx->program240.x60;
            smp.dtFast = D_002BF5C8;
            if (focus->cyclic) {
                limit = smp.svFast;
            } else if (ctx->reversed) {
                GSmooth__FfffP3SMPPf(ctx->value, 50.0f, D_002DDED0.dt, &smp, &limit);
                limit = -limit;
            } else {
                GSmooth__FfffP3SMPPf(ctx->value, SMaxShape(focus) - 50.0f, D_002DDED0.dt, &smp, &limit);
            }
            float ratio = (limit - smp.svSlow) / (smp.svFast - smp.svSlow);
            float amount = pjoy->amount;
            if (amount < 0.0f)
                g = 0.0f;
            else if (ratio < amount)
                g = ratio;
            else
                g = amount;
        }
        if (ctx->reversed)
            heading = RadNormalize__Ff(heading + 3.1415927f);
        if (0.0f < g) {
            SetJtGrabKind(ctx, 4);
            UpdateGaitBlend(&ctx->program240, g);
            func_00204660(ctx->actor, heading);
            UpdateGaitFootsteps(&ctx->program240);
        } else {
            SetJtGrabKind(ctx, 0);
            func_00204660(ctx->actor, heading);
        }
        break;
    }
    case 5:
    case 9:
        UpdateJtHands(ctx->sub);
        break;
    case 8:
        switch (ctx->step) {
        case 7:
        case 8:
            break;
        default:
            if (0.0f < pjoy->amount) {
                SetJtGrabKind(ctx, 10);
                ENTRY *e = &ctx->axes2[3];
                if (!e->active)
                    e = &ctx->axes2[2];
                SetXYZ(&ctx->actor->velocity, -pjoy->y * GLengthXY(e->direction.q), pjoy->x * GLengthXY(ctx->axes2[1].direction.q), 0.0f);
                func_001A0450(ctx->actor, ctx->axes2, ctx->request2);
                ACTOR *actor = ctx->actor;
                float turn = -actor->velocity.f[1] / D_002BF5CC;
                actor->velocity.q = QwScaleX(D_002A2DA0, QwDot(actor->velocity.q, D_002A2DA0));
                func_00204660(ctx->actor, ctx->actor->heading + turn * D_002DDED0.dt);
            } else {
                SetJtGrabKind(ctx, 9);
            }
            if (ctx->actor == D_002E1E40)
                func_00205910(ctx->actor, &D_002BF630, 3, D_002BF5D0);
            else
                func_00205910(ctx->actor, &D_002BF630, 3, D_002BF600);
            break;
        }
        break;
    case 10: {
        U v;
        v.f[2] = 0.0f;
        v.f[1] = 0.0f;
        v.f[0] = 0.0f;
        ACTOR *actor = ctx->actor;
        ANIM *anim = actor->anim;
        if (anim) {
            BINDING *b = func_0012FF10(anim->table, actor, 1, 1);
            if (b) {
                ReadBinding(b, anim, &v);
                v.q = QwScale(v.q, VuFloat(-1.0f));
            }
        }
        ctx->actor->velocity = v;
        break;
    }
    case 11:
        if (ctx->focus) {
            FaceStepToward(ctx->actor, ctx->focus);
            FixStepAngularVelocity(ctx->actor);
        }
        break;
    }

    switch (ctx->step) {
    case 1:
        func_001A0450(ctx->actor, ctx->axes, ctx->request);
        if (ctx->actor->x1024 == ctx->x68) {
            switch (ctx->x2C) {
            case 1:
            case 2:
                if (ctx->x2C == 1)
                    ctx->maximum = GMax(ctx->maximum, ctx->axes[1].weight);
                else
                    ctx->maximum = GMax(ctx->maximum, ctx->axes[0].weight);
                break;
            }
        }
        break;
    case 0:
        if (ctx->action == 3) {
            ACTOR *actor = ctx->actor;
            RebuildJtXmg(actor, actor->x19D8, actor->x19E0, actor->x19F0, 5.0f, actor->x1520);
            RebuildJtXmg(actor, actor->x19D4, actor->x19DC, actor->x19F4, 5.0f, actor->x1620);
            func_00193C98(actor, actor->x1520, actor->x19F0);
            func_00193C98(actor, actor->x1620, actor->x19F4);
        }
        break;
    }
}
