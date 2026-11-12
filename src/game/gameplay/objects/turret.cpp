// turret.cpp -- TURRET, a player-controlled turret (a PO): aim (pitch/yaw from a target or the stick), zoom (from a
// target's bounds or the stick), firing through a launcher, and its rotation act. Entry points: the TURRET vtable
// (InitTurret, CloneTurret, PostLoad, OnTurretActive, UpdateTurret, FilterTurretProxySource). VU0 and EE 128-bit asm.
// TU 0x2259D8..0x226550 (12 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_turret
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_turret
#include "sly2/cref.h"
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/clock.h"
#include "sly2/sw.h"
#include "sly2/cm.h"
#include "sly2/classes.h"
#include "sly2/save.h"
// 0x2263F8 StaticInit_turret and GlobalConstructors_turret: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

union V {qword q;float f[4];};
struct M3 {qword r[3];};
struct M4 {qword r[4];};
struct SMPV {float a,b,c,d;};

/* The aim target. */
struct O {char pad[0x60];void *component;char pad64[0x88];SHAPE *shape;};
/* View of the turret (a PO). */
struct TURRETV
{
    void *scriptFrame; void *scriptVariables; void *firstSubscriber;
    VTE *vt;            /* +0x00C */
    void *parent; void *root; char pad18[0x1C - 0x18]; void *unknown1C; void *unknown20; void *unknown24; void *unknown28; char pad2C[0x34 - 0x2C]; void *firstRunCallback; char pad38[0x3C - 0x38]; void *proxyRecord; char pad40[0x54 - 0x40]; void *transforms; void *positionXf;
    int rotationXf;     /* +0x05C the rotation XF (an SO's pxf at +0x5C), handed to the rotation act */
    void *rootTransform; void *unknown64; void *unknown68; char pad6C[0x70 - 0x6C]; void *ploRemovedFirst; char pad74[0x80 - 0x74]; void *unknown80; char pad84[0xA8 - 0x84]; void *unknownA8; char padAC[0xB0 - 0xAC]; void *unknownB0; char padB4[0xBC - 0xB4]; void *unknownBC; void *unknownC0; void *unknownC4; void *unknownC8; char padCC[0xD8 - 0xCC]; void *unknownD8; void *unknownDC; char padE0[0xE4 - 0xE0]; void *parts; void *physics; void *physicsTree; char padF0[0x134 - 0xF0]; void *psobounds; void *pxa; char pad13C[0x150 - 0x13C];
    POCTL *control;   /* +0x150 */
    char pad154[0x168 - 0x154]; TURRET *unknown168; void *cameraTunnel; char pad170[0x174 - 0x170]; void *unknown174; char pad178[0x180 - 0x178];
    O *target;          /* +0x180 */
    int aim;            /* +0x184 aim at the target */
    int follow;         /* +0x188 zoom on the target */
    float pitch, yaw, vpitch, vyaw;   /* +0x18C */
    LM yawRange;        /* +0x19C */
    LM pitchRange;      /* +0x1A4 */
    LM range;           /* +0x1AC zoom range */
    float blend, velocity, strength;  /* +0x1B4 */
    LO *projectileProxy;   /* +0x1C0 what it fires (AcquireProxyObject) */
    void *launcher;     /* +0x1C4 */
    void *lastLaunched; /* +0x1C8 last object fired */
    int isActRunning;   /* +0x1CC the rotation act is running */
    void *self;         /* +0x1D0 */
    int isActive;       /* +0x1D4 */
    void *rotationAct;  /* +0x1D8 rotation act */
};
typedef char assert_TURRET_pact[(unsigned int)&((TURRETV *)0)->rotationAct == 0x1D8 ? 1 : -1];

/* turret's .data (address order). */
LM D_002C4400 = {0.1f, 1.0f};
float D_002C4408[3] = {5.0f, 0.0f, 0.5f};          /* damping */
float D_002C4418[2] = {1.0f, 4.0f}, D_002C4420[3] = {2.0f, 0.0f, -1.0f};
extern LM g_lmZeroOne;
extern const SMPV D_002A97B0 = {0.2f, 0.0f, 0.5f, 1.0f};
extern "C" {
void InitPo(TURRETV *p);
void ClonePo(TURRETV *p, TURRETV *pSrc);
void PostSoLoad(TURRETV *p);
void OnPoActive(TURRETV *p, int b);
void SetCmPolicy(void *g, int a, void *h, void *p, int z);
void RevokeCmPolicy(void *g, int a, int b, void *h, void *p, int z);
void UpdateSo(TURRETV *p, float dt);
void UpdateTurretAim(TURRETV *p);
void UpdateTurretZoom(TURRETV *p);
void UpdateTurretFire(TURRETV *p);
void SetTurretActive(TURRETV *p, int k);
qword GetLoWorldPos(void *);
M3 *GetLoWorldMatrix(void *);
M3 LoadLookAtMatrix(qword) SLY2_SRET_LABEL("LoadLookAtMatrix", "LoadLookAtMatrixValue");
void DecomposeRotateMatrixPanTilt__FP7MATRIX3PfT1(const M3 *,float *,float *);
float ClampAbs__Fff(float,float);
float SmoothValue__FfffP3SMPPf(float,float,float,const float *,float *);
float ClampToLimits__FP2LMf(const LM *,float);
float SmoothValueWithAccel__FffffP4SMPAPf(float,float,float,float,SMPV *,float *);
void SetActPositionGoal(void *,qword);
CRef GetGlobalSaveVar(int,void *) SLY2_SRET_LABEL("GetGlobalSaveVar", "GetGlobalSaveVarValue");
void LoadIdentityMatrix4(void *,void *,M4 *,M4 *,float);
float atanf(float);
int POCTL_ConsumeAnyPressed(POCTL *x, int a);
void *AcquireProxyObject(LO *y, int a);
void LaunchSoFromLauncher(void *z, void *v);
void *ShouldSoIgnoreIntersection(TURRETV *p);
void *MaxInt(void *v, int a);
int NewPositionAct(TURRETV *p, int c, int a, int b, int d, qword q);
void ReleaseAct(void **ph);
}

// Packs three floats into a VU vector (x, y, z).

// TURRET vtable slot 7 (Init): PO defaults; pitchRange +-1.5 rad, yawRange +-0.7 rad (named from how UpdateTurretAim
// uses them; the act gets (0, -yaw, pitch)), zoom range and strength, active at load, and the self pointer.
extern "C" void InitTurret(TURRETV *turret)
{
    InitPo(turret);
    turret->range = D_002C4400;
    turret->isActRunning = 1;
    turret->strength = turret->range.maxValue;
    turret->pitchRange.minValue = -1.5f;
    turret->pitchRange.maxValue = 1.5f;
    turret->yawRange.minValue = -0.7f;
    turret->yawRange.maxValue = 0.7f;
    turret->isActive = 1;
    turret->self = turret;
}

// TURRET Clone: copies the source PO, then points the self pointer back at this turret.
extern "C" void *CloneTurret(TURRETV *turret, TURRETV *source)
{
    void *pv;
    ClonePo(turret, source);
    pv = turret->self;
    if (pv == source)
        turret->self = turret;
    return pv;
}

// TURRET vtable slot 15 (PostLoad): finishes the SO load and, if the level marked it active, activates it
// (SetTurretActive). (inferred)
// Also known as: PostLoadTurret (upstream readable / Sly 1 name; PostLoadTurret__FP2LO).
void TURRET::PostLoad()
{
    TURRETV *turret = (TURRETV *)this;
    PostSoLoad(turret);
    if (turret->isActRunning) { turret->isActRunning = 0; SetTurretActive(turret, 1); }
}

// TURRET OnActive: when the player takes the turret, installs camera policy 9 (the turret view) on g_cm;
// revokes it when the player leaves.
// Also known as: SetTurretFocus (upstream readable / Sly 1 name; SetTurretFocus__FP2LOi).
extern "C" void OnTurretActive(TURRETV *turret, int b)
{
    OnPoActive(turret, b);
    if (b) SetCmPolicy(&g_cm, 9, &g_cm.turretPolicy, turret, 0);
    else RevokeCmPolicy(&g_cm, 0xF, 9, &g_cm.turretPolicy, turret, 0);
}

// TURRET vtable slot 16 (Update): SO update, then aim, zoom and fire.
// Also known as: UpdateTurret (upstream readable / Sly 1 name; UpdateTurret__FP6TURRETf).
extern "C" void UpdateTurret(TURRETV *turret, float dt)
{
    UpdateSo(turret, dt);
    UpdateTurretAim(turret);
    UpdateTurretZoom(turret);
    UpdateTurretFire(turret);
}

// Aims the turret: with a target and aiming enabled, turns pan/tilt toward it; otherwise integrates the stick into
// smoothed angular velocities. Both are clamped to the yaw/pitch ranges, and the rotation act gets the new goal.
// Also known as: UpdateTurretAim (upstream readable / Sly 1 name; UpdateTurretAim__FP6TURRET).
extern "C" void UpdateTurretAim(TURRETV *turret)
{
    if (turret->target && turret->aim) {
        M3 m;
        float pitch, yaw;
        qword target;
        m = LoadLookAtMatrix((target = GetLoWorldPos(turret->target), VuSub(target, GetLoWorldPos(turret))));
        DecomposeRotateMatrixPanTilt__FP7MATRIX3PfT1(&m, &pitch, &yaw);
        float dp = pitch - turret->pitch;
        float dy = yaw - turret->yaw;
        turret->pitch = pitch;
        turret->yaw = yaw;
        turret->vpitch = dp;
        turret->vyaw = dy;
    } else {
        if (turret->control) {
            /* stick input, cubed for fine aim, scaled by the zoom strength */
            float x = ClampAbs__Fff(turret->control->dxLeft + turret->control->dxRight, 1.0f);
            float y = ClampAbs__Fff(turret->control->dyLeft, 1.0f);
            float strength = turret->strength;
            float gain = (1.0f - strength) * 0.5f + strength;
            float cubic = -x * x;
            cubic = cubic * x;
            float accel = gain * 10.0f;
            float ax = cubic * accel;
            float limit = strength * 1.3f;
            if (ax * turret->vpitch <= 0.0f) turret->vpitch = 0.0f;   /* reversing stops at once */
            float cubeY = y * y;
            cubeY = cubeY * y;
            float ay = cubeY * accel;
            if (!GetGlobalSaveVar((*(int *)&g_sw.psvpReverseAim), &g_gs).u.n) ay = -ay;   /* "reverse aim" option */
            if (ay * turret->vyaw <= 0.0f) turret->vyaw = 0.0f;
            CLOCK *clock = &g_clock;
            const float *damping = D_002C4408;
            turret->vpitch = SmoothValue__FfffP3SMPPf(turret->vpitch, 0.0f, clock->deltaTime, damping, 0);
            turret->vyaw = SmoothValue__FfffP3SMPPf(turret->vyaw, 0.0f, clock->deltaTime, damping, 0);
            turret->vpitch = ClampAbs__Fff(turret->vpitch + ax * clock->deltaTime, limit * 0.9f);
            turret->vyaw = ClampAbs__Fff(turret->vyaw + ay * clock->deltaTime, limit * 0.8f);
            turret->pitch += turret->vpitch * clock->deltaTime;
            turret->yaw += turret->vyaw * clock->deltaTime;
        }
    }
    turret->pitch = ClampToLimits__FP2LMf(&turret->pitchRange, turret->pitch);
    turret->yaw = ClampToLimits__FP2LMf(&turret->yawRange, turret->yaw);
    if (turret->isActRunning) {
        V v;
        v.q = QwordFromFloats<qword>(0.0f, -turret->yaw, turret->pitch);
        SetActPositionGoal(turret->rotationAct, v.q);
    }
}

// VU helpers of UpdateTurretZoom: load/move into a VU register, difference, length (as float or as raw bits),
// transpose-rotate by a 3x3 matrix, and the position of a value inside an LM range (Fraction) and back (Blend).
static inline qword LoadVF(const qword &value){qword r;__asm__("lqc2 %0,%1":"=j"(r):"m"(value));return r;}
// Move a 128-bit GPR value into a VU0 register.
static inline qword ToVF(qword v){qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(v));return r;}
// Return b - a.
static inline qword Difference(qword b,qword a){qword r;__asm__("vsub.xyzw %0,%2,%1":"=j"(r):"j"(b),"j"(a));return r;}
// Length of d as a float.
static inline float Length(qword d){qword r;float f;
    r = VuDot(d, d);
    __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(r));
    f = VuToFloat(r);return f;}
// Length of d, read through $3.
static inline float OffsetLength(qword d){qword r;register int bits __asm__("$3");
    r = VuDot(d, d);
    __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(r));
    __asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(r));union {int i;float f;} value;value.i=(int)bits;return value.f;}
// Rotate v by the transpose of m (three dot products).
static inline qword InverseRotate(const M3 &m,qword v){qword r=m.r[0],one,a,b,c;
    __asm__("vmul.xyzw %2,%0,%7\n\tvmaxw.xyzw %1,$vf0,$vf0w\n\tvmul.xyzw %3,%5,%7\n\tvmul.xyzw %4,%6,%7\n\tvadday.x ACC,%2,%2y\n\tvmaddz.x %0,%1,%2z\n\tvaddax.y ACC,%3,%3x\n\tvmaddz.y %0,%1,%3z\n\tvaddax.z ACC,%4,%4x\n\tvmaddy.z %0,%1,%4y":"+j"(r),"=&j"(one),"=&j"(a),"=&j"(b),"=&j"(c):"j"(m.r[1]),"j"(m.r[2]),"j"(v));return r;}
// Fraction of f within the range [lo, hi]; 0 when the range is empty.
static inline float Fraction(LM *range,float f){if(range->minValue==range->maxValue)return 0.0f;return (f-range->minValue)/(range->maxValue-range->minValue);}
// Interpolate within the range at u.
static inline float Blend(LM *range,float u){return (1.0f-u)*range->minValue+u*range->maxValue;}
// Zoom: framed on the target's bounds when following one, else driven by the controller's zoom input.
// Also known as: UpdateTurretZoom (upstream readable / Sly 1 name; UpdateTurretZoom__FP3ALO).
extern "C" void UpdateTurretZoom(TURRETV *turret)
{
    if (turret->target && turret->follow) {
        M4 m;
        V future, offset, local, swapped;
        SMPV spring;
        V saved;
        V scratch;
        /* the field of view that holds the target: from its radius at its distance, and from where it will be next
           frame (LoadIdentityMatrix4 predicts its matrix) as seen in the turret's frame */
        float radius = turret->target->shape ? turret->target->shape->nextSpliceTime : 0.0f;
        scratch.q = ToVF(GetLoWorldPos(turret));
        float angle = atanf(radius / (Length(VuSub(scratch.q, GetLoWorldPos(turret->target))) * 0.5f));
        angle = angle + angle;
        CLOCK *clock = &g_clock;
        angle = Max(angle, turret->range.minValue);
        LoadIdentityMatrix4(turret->target->component, 0, &m, 0, clock->deltaTime);
        scratch.q = LoadVF(m.r[3]);
        future.q = scratch.q;
        offset.q = VuSub(scratch.q, GetLoWorldPos(turret->target));
        float extent = radius + OffsetLength(offset.q);
        saved.q = future.q;
        local.q = Difference(GetLoWorldPos(turret), saved.q);
        local.q = InverseRotate(*GetLoWorldMatrix(turret), local.q);
        swapped.f[0] = -local.f[1];
        swapped.f[1] = local.f[2];
        swapped.f[2] = local.f[0];
        float desired;
        if (local.f[0] > 0.0001f) {
            float horizontal = atanf((Abs(swapped.f[0]) + extent) / (swapped.f[2] * g_cm.aspectRatio));
            horizontal = horizontal + horizontal;
            float vertical = atanf((Abs(swapped.f[1]) + extent) / swapped.f[2]);
            vertical = vertical + vertical;
            desired = Min(Max(horizontal, vertical), turret->range.maxValue);
        } else desired = turret->range.maxValue;
        desired = Max(desired, angle);
        /* spring the blend toward it, faster for bigger changes (D_002C4418) */
        float target = ClampToLimits__FP2LMf(&g_lmZeroOne, 1.0f - Fraction(&turret->range, desired));
        float delta = Abs(target - turret->blend);
        spring = D_002A97B0;
        float rate = (1.0f - delta) * D_002C4418[0] + delta * D_002C4418[1];
        spring.d *= rate;
        spring.a *= rate;
        spring.b *= rate;
        turret->blend = SmoothValueWithAccel__FffffP4SMPAPf(turret->blend, turret->velocity, target, g_clock.deltaTime, &spring, &turret->velocity);
    } else {
        if (turret->control) {
            float blend = turret->blend, zoom = turret->control->dyRight;
            turret->velocity = (D_002C4420[0] + blend * (D_002C4420[1] + blend * D_002C4420[2])) * zoom * Abs(zoom);
            turret->blend = ClampToLimits__FP2LMf(&g_lmZeroOne, blend + turret->velocity * g_clock.deltaTime);
        }
    }
    turret->strength = Blend(&turret->range, 1.0f - turret->blend);
}

// Fires on any newly pressed button: takes a projectile from the proxy pool, launches it from the launcher and fires
// the turret's Splice event 5. (inferred)
// Also known as: UpdateTurretFire (upstream readable / Sly 1 name; UpdateTurretFire__FP3ALO).
extern "C" void UpdateTurretFire(TURRETV *turret)
{
    if (turret->control && POCTL_ConsumeAnyPressed(turret->control, 0xFF)) {
        if (turret->projectileProxy) {
            void *v = AcquireProxyObject(turret->projectileProxy, 0);
            if (!v) return;
            turret->lastLaunched = v;
            if (turret->launcher) LaunchSoFromLauncher(turret->launcher, v);
            ((::LO *)turret)->HandleSpliceEvent(5, 0, 0, 0, 0);
        } else
            ((::LO *)turret)->HandleSpliceEvent(5, 0, 0, 0, 0);
    }
}

// Intersection filter (slot 61): the SO default, plus ignoring objects made from the same proxy as the turret's
// projectile (so the turret ignores its own shots). (inferred)
// Also known as: OverlapKindTurret (upstream readable / Sly 1 name; OverlapKindTurret__FP3ALOT0).
extern "C" void *FilterTurretProxySource(TURRETV *turret, LO *other)
{
    void *v = ShouldSoIgnoreIntersection(turret);
    if (turret->projectileProxy && other->proxyRecord == turret->projectileProxy->proxyRecord) v = MaxInt(v, 1);
    return v;
}

// Turns the turret on (creates the rotation act toward the current pan/tilt) or off (releases the act). (inferred)
// Also known as: SetTurretRotate (upstream readable / Sly 1 name; SetTurretRotate__FP3ALOi).
extern "C" void SetTurretActive(TURRETV *turret, int k)
{
    if (turret->isActRunning != k) {
        turret->isActRunning = k;
        if (k) {
            V v;
            v.q = QwordFromFloats<qword>(0.0f, -turret->yaw, turret->pitch);
            turret->rotationAct = (void *)NewPositionAct(turret, turret->rotationXf, 1, 5, 3, v.q);
        } else {
            ReleaseAct(&turret->rotationAct);
        }
    }
}
