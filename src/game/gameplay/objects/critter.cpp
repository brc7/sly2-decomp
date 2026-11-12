// critter.cpp -- CID96 (0x60), the small critters (pigeons, rats): an SO that turns toward the nearest object,
// walks a curve and flees along another one, with a per-state animation table. Entry points: the CID96
// vtable (InitCritter, LoadCritterFromBrx, PostCritterLoad, UpdateCritter, HandleCritterMessage, TakeCritterDamage...)
// and the Splice setter SetCritterLink. TU 0x1B0B78..0x1B1DB8 (25 functions,
// address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_critter
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_critter
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/clock.h"
#include "sly2/classes.h"
#include "sly2/act.h"
#include "sly2/sw.h"
#include "sly2/emitter.h"   /* EXPL, EXPLSO: the hit listener */

// 0x1B1C60 StaticInit_critter and GlobalConstructors_critter: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

typedef int qwordV __attribute__((mode(TI)));
union U { qwordV q; float f[4]; int i[4]; };
union F { float value; int bits; };
struct M3 { U row[3]; };
struct M4 { U row[4]; };
struct V3 { int x, y, z; };
struct VECTOR3 { float x, y, z; };

/* Hand-made virtual calls: the vtable pointer of an object is at +0xC. */

struct SW;
struct ASEG;
struct ASEGA;
struct ALO;
struct STEP;
struct SHAPE;
struct EFFECT;
struct SOURCE;
struct CBinaryInputStream;
// The real classes behind the views below, for their virtual calls.
static inline ::LO *AsLo(void *plo) { return (::LO *)plo; }
static inline ::SO *AsSo(void *pso) { return (::SO *)pso; }
struct LOV { int pad0[3]; void *pvt; char pad10[0x74 - 0x10]; float duration; float masterSpeed; };   /* duration/speed: as an ASEG */

/* The XFs at SO+0x58/+0x5C as far as this TU reads them. */
struct XFV { char pad0[0x88]; void *timeSource; void *speedSource; void *firstActuator; char pad94[0x98 - 0x94]; short role; char pad9A[0xA0 - 0x9A]; void *parent; char padA4[0xA8 - 0xA4]; void *unknownA8; char padAC[0xF0 - 0xAC]; V3 unknownF0; };   /* unknownF0: a vector cleared as three words */

/* Per-state animation: the clip's OID until PostLoad resolves it to the ASEG, and its apply flags. */
struct E96 { OID oid; ASEG *paseg; int applyFlags; };

struct CID96V
{
    void *scriptFrame; void *scriptVariables; void *firstSubscriber;
    void *pvt;              /* 0x0C */
    void *parent; void *root; char pad18[0x1C - 0x18]; void *unknown1C; void *unknown20; void *unknown24; void *unknown28; char pad2C[0x34 - 0x2C]; void *firstRunCallback; char pad38[0x3C - 0x38]; void *proxyRecord;
    unsigned int taggedFlags; /* 0x40 tagged word (pointer, or odd flags) */
    char pad44[0x54 - 0x44]; void *transforms;
    XFV *positionXf;        /* 0x58 */
    XFV *rotationXf;        /* 0x5C ("group" of the state controller) */
    void *rootTransform; void *unknown64; void *unknown68; char pad6C[0x70 - 0x6C]; void *ploRemovedFirst; char pad74[0x80 - 0x74]; void *unknown80; char pad84[0xA8 - 0x84]; void *unknownA8; char padAC[0xB0 - 0xAC]; void *unknownB0; char padB4[0xBC - 0xB4]; void *unknownBC; void *unknownC0; void *unknownC4; void *unknownC8; char padCC[0xD8 - 0xCC]; void *unknownD8; void *unknownDC; char padE0[0xE4 - 0xE0]; void *parts; void *physics; void *physicsTree; char padF0[0x134 - 0xF0]; void *psobounds; void *pxa; char pad13C[0x14C - 0x13C]; void *unknown14C;
    float nearDistance;     /* 0x150 a player this close startles it */
    float farDistance;      /* 0x154 ... or this close while moving faster than 150 */
    float idleWaitMin, idleWaitMax;     /* 0x158 random time spent idle */
    float walkWaitMin, walkWaitMax;     /* 0x160 random time spent walking */
    float fadeTime;         /* 0x168 fade-out time, also how early the flight ends */
    SHAPE *walkPath;        /* 0x16C */
    SHAPE *fleePath;        /* 0x170 */
    float pathU;            /* 0x174 parameter on the current path */
    float pathStep;         /* 0x178 search increment along it (sign: direction) */
    E96 stateAnims[6];      /* 0x17C animation per CRITTER_STATE */
    ASEGA *segmentAnim;     /* 0x1C4 */
    LOV *hitListener;       /* 0x1C8 told (vtable slot 51, message 6) when the critter is hit */
    float walkSpeed;        /* 0x1CC from the walk animation's root motion */
    F fleeSpeed;            /* 0x1D0 from the flee animation's root motion */
    int defaultRole;        /* 0x1D4 the rotation XF's role at load, restored when leaving idle/turn/startled */
    float heading;          /* 0x1D8 */
    int pad1DC;
    M3 headingBasis;        /* 0x1E0 rotation for heading, the spring target */
    VECTOR3 spinScale;      /* 0x210 spring parameters scaled by spinRatio */
    float spinRatio;        /* 0x21C */
    int state;              /* 0x220 CRITTER_STATE */
    float stateTime;        /* 0x224 */
    float nextStateTime;    /* 0x228 idle/walk end time */
    float fleeEndTime;      /* 0x22C when the fleeing critter fades out */
    int amount;             /* 0x230 passed to SprayDifficultyPrizes when hit (-1 by default) */
    char pad234[12];
    U damageOffset;         /* 0x240 local offset of the hit effect */
};
/* Critter states (inferred from the update and SetCritterState). */
enum CRITTER_STATE
{
    CRITTER_NONE = -1,
    CRITTER_IDLE = 0,
    CRITTER_WALK = 1,
    CRITTER_TURN = 2,       /* turning around at a path end */
    CRITTER_STARTLED = 3,   /* faces away from the player (spin doubled) */
    CRITTER_FLEE = 4,       /* runs along the flee path (or away) and fades out */
    CRITTER_HIT = 5
};
typedef char assert_CID96_size[sizeof(CID96V) == 0x250 ? 1 : -1];

extern "C" {
extern U D_002A2DD0, D_002A2DA0, g_normalZ;
extern char D_002DE578[];
int D_002C0230[6] = {8, 8, -1, 0, 8, -1};   /* critter's .data, address order */
extern const float D_002A70E0;   /* defined below where the ELF has it (.rodata order) */
VECTOR3 D_002C0248 = {25.0f, 0.0f, 0.25f};

void InitSo(CID96V *p);
void RemoveSoHierarchy(CID96V *p);
void LoadSoFromBrx(CID96V *p, CBinaryInputStream *pbis);
void PostSoLoad(CID96V *p);
void SO_ResetForProxy(CID96V *p);
void UpdateSo(CID96V *p);
void FadeSoOut(CID96V *p, float f);
void HandleBasicMessage(CID96V *p, int msg, ASEGA *pm);
void RenderSoSelf(CID96V *p);
void AccelSoTowardMatBounded(CID96V *p, M3 *pmat, VECTOR3 *pspin);
int CanSoTakeDamage(CID96V *p, struct A96 *pa);
void UpdateSoXfWorld(CID96V *p);
void SetXfRole(CID96V *, XFV *, int);
void WakeXfOwner(CID96V *, XFV *);
void ResetXfValueRef(CID96V *, XFV *);
float RandomFloatInRange__Fff(float, float);
void GetVectorSphere(qwordV, float *, float *, float *);
qwordV GetLoWorldPos(void *p);
void FindShapeClosestPoint(SHAPE *, qwordV, void *, U *, U *, float *, float *);
void FindShapeClosestPointFromParam(SHAPE *, qwordV, void *, qwordV *, qwordV *, float *, float *, float);
float GetShapeParamSearchStep(SHAPE *);
float GetShapeMaxDistance(SHAPE *);
float GetShapeMinParam(SHAPE *);
float GetShapeMaxParam(SHAPE *);
float WrapShapeParam(SHAPE *, float);
void EvaluateShapeAtParam(SHAPE *, float, qwordV *, qwordV *);
LOV *FindClosestSwPo(SW *, qwordV);
EFFECT *CreateSfxSound(int);
void SetSndSourceLo(EFFECT *, SOURCE *);
void SetSndSourcePos(EFFECT *, qwordV);
qwordV ConvertPosBetweenLos(CID96V *, CID96V *, qwordV);
void SprayDifficultyPrizes(void *, qwordV, int, int, CID96V *);
M3 BuildAxisAngleRotationMatrix(qwordV v, float g);
float atan2f(float y, float x);
CHN *FindAsegChannel(ASEG *paseg, CID96V *p, int a, int b);

void ResetCritter(CID96V *p);
void UpdateCritterPathFollow(CID96V *p);
void SetCritterState(CID96V *p, int next);
void SetCritterHeading(CID96V *p, float g);
int IsCritterFacingHeading(CID96V *p);
int ChooseCritterStateFromPlayer(CID96V *p);
void GetAsegSpeedForObject(CID96V *p, ASEG *paseg, float *pf);
int IsValidCritterLinkIndex(unsigned int n);
ASEG *FindCritterLinkObject(CID96V *p, OID oid);
void ResolveCritterLinks(CID96V *p);
}
void ReplaceCurrentAseg(ASEG *paseg, ALO *paloRoot, float t, float svt, int grfapl, ASEGA **ppasega);  /* ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA */
float NormalizeAngle(float r);                                                                  /* NormalizeAngle__Ff */
LOV *FindSwObject(SW *psw, int grf, OID oid, LOV *ploCtx) __asm__("FindSwObject__FP2SWi3OIDP2LO");                                   /* FindSwObject__FP2SWi3OIDP2LO */
void SetStepSpinRatio(STEP *pstep, float r);

// CID96 vtable slot 7 (Init): SO init; startle distances 400/800, idle 2-6 s, walk 6-10 s, fade 1 s; no animations yet
// (apply flags from D_002C0230); speeds 300/1000; no state; LO flag 2.
// Also known as: InitBird (upstream readable / Sly 1 name; InitBird__FP2LO).
extern "C" void InitCritter(CID96V *critter)
{
    int i;
    E96 *pe;
    int *pi;
    InitSo(critter);
    critter->nearDistance = 400.0f;
    critter->farDistance = 800.0f;
    critter->idleWaitMin = 2.0f;
    critter->idleWaitMax = 6.0f;
    critter->walkWaitMin = 6.0f;
    critter->walkWaitMax = 10.0f;
    critter->fadeTime = 1.0f;
    pe = critter->stateAnims;
    pi = D_002C0230;
    for (i = 5; i >= 0; i--) {   /* counted down: the target's loop shape */
        pe->oid = (OID)-1;
        pe->applyFlags = *pi++;
        pe++;
    }
    critter->walkSpeed = 300.0f;
    critter->fleeSpeed.value = 1000.0f;
    critter->state = CRITTER_NONE;
    critter->amount = -1;
    if ((critter->taggedFlags ^ 1) & 1) *(int *)critter->taggedFlags |= 2;
    else critter->taggedFlags |= 2;
}

// CID96 vtable slot 12 (RemoveHierarchy): SO removal, animation stopped.
extern "C" void RemoveCritterHierarchy(CID96V *critter)
{
    RemoveSoHierarchy(critter);
    ReplaceCurrentAseg(0, 0, 0.0f, 0.0f, 0, &critter->segmentAnim);
}

// CID96 vtable slot 14 (LoadFromBrx): SO load; the position XF gets role 1 and a cleared vector; remembers the
// rotation XF's role; resolves the animation oids.
extern "C" void LoadCritterFromBrx(CID96V *critter, CBinaryInputStream *stream)
{
    XFV *xf;
    V3 *vector;
    LoadSoFromBrx(critter, stream);
    SetXfRole(critter, critter->positionXf, 1);
    xf = critter->positionXf;
    vector = &xf->unknownF0;
    vector->z = 0;
    vector->y = 0;
    xf->unknownF0.x = 0;
    critter->defaultRole = critter->rotationXf->role;
    ResolveCritterLinks(critter);
}

// CID96 vtable slot 15 (PostLoad): SO post-load, resolves the animations, takes the walk and flee speeds from their
// root motion, then resets.
// Also known as: StartBird (upstream readable / Sly 1 name; StartBird__FP2LO).
extern "C" void PostCritterLoad(CID96V *critter)
{
    PostSoLoad(critter);
    ResolveCritterLinks(critter);
    GetAsegSpeedForObject(critter, critter->stateAnims[CRITTER_WALK].paseg, &critter->walkSpeed);
    GetAsegSpeedForObject(critter, critter->stateAnims[CRITTER_FLEE].paseg, &critter->fleeSpeed.value);
    ResetCritter(critter);
}

// CID96 vtable slot 26 (proxy reset): the base, then ResetCritter.
extern "C" void ResetCritterFromProxy(CID96V *critter)
{
    SO_ResetForProxy(critter);
    ResetCritter(critter);
}

// Spin ratio 1; a critter with a walk path starts walking from the path's start (vtable slot 29 moves it there),
// others idle.
// Also known as: PlaceBird (upstream readable / Sly 1 name; PlaceBird__FP2LO).
void ResetCritter(CID96V *critter)
{
    qwordV v;
    SetStepSpinRatio((STEP *)critter, 1.0f);
    if (critter->walkPath) {
        SetCritterState(critter, CRITTER_WALK);
        critter->pathU = GetShapeMinParam(critter->walkPath);
        critter->pathStep = GetShapeParamSearchStep(critter->walkPath);
        EvaluateShapeAtParam(critter->walkPath, critter->pathU, &v, 0);
        AsSo(critter)->TranslateToPos(v);
    } else {
        SetCritterState(critter, CRITTER_IDLE);
    }
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A70E0 __asm__("D_002A70E0") = 3.40282347e+38f; }
// CID96 vtable slot 16 (Update): SO update and the state machine: idle, walking and turning critters are startled by a
// nearby player, otherwise idle and walk alternate on their timers and a turn ends when the critter faces its
// heading; a startled critter flees once it faces away and its animation has finished; a fleeing one fades out at
// fleeEndTime. Then the path following.
// Also known as: UpdateBird (upstream readable / Sly 1 name; UpdateBird__FP2LO).
extern "C" void UpdateCritter(CID96V *critter)
{
    int newState;
    UpdateSo(critter);
    newState = critter->state;
    switch (newState) {
    case CRITTER_IDLE:
        newState = ChooseCritterStateFromPlayer(critter);
        if (newState != CRITTER_STARTLED && critter->nextStateTime <= g_clock.time) {
            if (critter->walkPath) newState = CRITTER_WALK;
        }
        break;
    case CRITTER_WALK:
        newState = ChooseCritterStateFromPlayer(critter);
        if (newState != CRITTER_STARTLED && critter->nextStateTime <= g_clock.time) newState = CRITTER_IDLE;
        break;
    case CRITTER_TURN:
        newState = ChooseCritterStateFromPlayer(critter);
        if (newState != CRITTER_STARTLED && IsCritterFacingHeading(critter)) newState = CRITTER_WALK;
        break;
    case CRITTER_STARTLED:
        if (!critter->segmentAnim && IsCritterFacingHeading(critter)) newState = CRITTER_FLEE;
        break;
    case CRITTER_FLEE:
        if (critter->fleeEndTime <= g_clock.time) {
            FadeSoOut(critter, critter->fadeTime);
            critter->fleeEndTime = D_002A70E0;   /* FLT_MAX: once */
        }
        break;
    }
    SetCritterState(critter, newState);
    UpdateCritterPathFollow(critter);
}

struct M96 { int kind; int pad[3]; qwordV vectorA; qwordV vectorB; qwordV vectorC; };
struct A96 { char pad[0x20]; qwordV unknown20; qwordV unknown30; int pad1; int flags; };   /* the hit (a WKR) */
// CID96 vtable slot 75 (absorb a hit): after the base absorb, a hit with flag 8 kills the critter (CRITTER_HIT) and
// tells its hit listener (vtable slot 51, message 6 with the hit's two vectors). Returns the base result.
// Also known as: FAcceptBirdContact (upstream readable / Sly 1 name; FAcceptBirdContact__FP3ALOP5CTMSG).
extern "C" int TakeCritterDamage(CID96V *critter, A96 *hit)
{
    int result = CanSoTakeDamage(critter, hit);
    if (result && (hit->flags & 8) && critter->state != CRITTER_HIT) {
        SetCritterState(critter, CRITTER_HIT);
        if (critter->hitListener) {
            M96 m;
            m.kind = 6;
            m.vectorB = hit->unknown20;
            m.vectorA = hit->unknown30;
            ((EXPL *)critter->hitListener)->ExplodeExplso((EXPLSO *)&m);   /* M96: EXPLSO's grf, vector and origin */
        }
    }
    return result;
}

// Clears a three-word vector.
static __inline__ void ClearV(V3 *pv) { pv->z = 0; pv->y = 0; pv->x = 0; }
// CID96 vtable slot 17: while turning or startled, clears both XFs' vectors and springs toward the heading (vtable
// slot 70, PresetCritterAccel); then the SO step.
// Also known as: UpdateBirdPhys (upstream readable / Sly 1 name; UpdateBirdPhys__FP3ALO).
extern "C" void UpdateCritterXfWorld(CID96V *critter)
{
    int state = critter->state;
    if (state < CRITTER_FLEE) if (state >= CRITTER_TURN) {   /* nested: a range test becomes one unsigned compare */
        ClearV(&critter->positionXf->unknownF0);
        ClearV(&critter->rotationXf->unknownF0);
        AsSo(critter)->PresetAccel();
    }
    UpdateSoXfWorld(critter);
}

// CID96 vtable slot 70: spring acceleration toward the heading basis with the spin parameters (AccelSoTowardMatBounded). (inferred)
// Also known as: TurnBird (upstream readable / Sly 1 name; TurnBird__FP3ALOii).
extern "C" void PresetCritterAccel(CID96V *critter)
{
    AccelSoTowardMatBounded(critter, &critter->headingBasis, &critter->spinScale);
}

// CID96 vtable slot 5 (HandleMessage): the basic handler; when its startled animation ends (message 2 from its
// ASEGA playing that clip) the critter flees.
// Also known as: OnBirdMessage (upstream readable / Sly 1 name; OnBirdMessage__FP2LO5MSGIDPv).
extern "C" void HandleCritterMessage(CID96V *critter, int kind, ASEGA *anim)
{
    HandleBasicMessage(critter, kind, anim);
    if (kind == 2 && anim == critter->segmentAnim && anim->animation == critter->stateAnims[CRITTER_STARTLED].paseg)
        SetCritterState(critter, CRITTER_FLEE);
}

// CID96 vtable slot 19 (RenderSelf): the SO render.
void CID96::RenderSelf()
{
    CID96V *critter = (CID96V *)this;
    RenderSoSelf(critter);
}

typedef union { qwordV q; float f[4]; } VU;
// Length of a vector, in lane x.
static __inline__ qwordV VLen(qwordV c)
{
    qwordV l, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(c));
    {
    qwordV r;
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(r) : "j"(l));
    return r;
    }
}
// Path following while walking or fleeing: from the closest point on the path, aims one search step ahead and sets
// the velocity toward it at the state's speed; a walking critter past either end turns around.
// Also known as: MoveBirdAlongPath (upstream readable / Sly 1 name; MoveBirdAlongPath__FP2LO).
void UpdateCritterPathFollow(CID96V *critter)
{
    SHAPE *path;
    float speed, t;
    VU upos;
    switch (critter->state) {
    case CRITTER_WALK:
        path = critter->walkPath;
        speed = critter->walkSpeed;
        break;
    case CRITTER_FLEE:
        path = critter->fleePath;
        speed = critter->fleeSpeed.value;
        break;
    default:
        path = 0;
        break;
    }
    if (!path)
        return;
    upos.q = GetLoWorldPos(critter);
    FindShapeClosestPointFromParam(path, upos.q, 0, 0, 0, &critter->pathU, 0, critter->pathU);
    t = WrapShapeParam(path, critter->pathU + critter->pathStep);
    if (t < GetShapeMinParam(path) || GetShapeMaxParam(path) < t) {
        if (critter->state == CRITTER_WALK)
            SetCritterState(critter, CRITTER_TURN);
    } else {
        VU v, uc, ur, ub;
        qwordV d, c, l, n, b, r;
        register qwordV k __asm__("$2");
        float gl;
        register int lengthBits __asm__("$3");
        EvaluateShapeAtParam(path, t, &v.q, 0);
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(v.q), "j"(upos.q));
        {
            register const qwordV *address __asm__("$2") = &D_002A2DA0.q;
            k = *address;
        }
        __asm__("vmove.xyzw %0, %1" : "=j"(c) : "j"(d));
        uc.q = c;
        l = VLen(uc.q);
        __asm__("qmfc2.ni %0,%2\n\tmtc1 %0,%1" : "=&r"(lengthBits), "=f"(gl) : "j"(l));
        if (gl < 1e-4f) {
            n = k;
        } else {
            __asm__ __volatile__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(n) : "j"(d), "j"(l));
        }
        __asm__("qmtc2.ni %1, %0" : "=j"(b) : "r"(speed));
        ub.q = b;
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(n), "j"(ub.q));
        ur.q = r;
        AsSo(critter)->SetVelocity(ur.q);
    }
}

// Mover/follower state controller: switch from the current state to `next`.
// Leaving states -1/0/2/3 restores the group's default action; entering a state sets up its velocity, mode, timers or
// effect; afterwards the new state, its time stamp and (except for state 2) the state's animation entry are recorded.
struct WORK{U first,second,direction,velocity,scalar;float angle,parameter;};
// Stop the critter (zero velocity).
static inline void SetZeroVelocity(CID96V*critter){AsSo(critter)->SetVelocity(D_002A2DD0.q);}
// broadcast a float into a VU register and keep the scratch store the original left in `home`
static inline qwordV SplatInto(float f,U*home){qwordV q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));home->q=q;return q;}
// a scaled by lane x of s.
static inline qwordV ScaleX(qwordV a,qwordV s){qwordV r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(a),"j"(s));return r;}
// length of a vector, in two steps (sum of squares, then sqrt) so the stores/loads of the callers can sit between them
static inline qwordV LengthV(qwordV v){qwordV d,one;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(v));__asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(d));return d;}
// Lane x as a float.
// v * (1/n.x); the "memory" clobbers (here and on LEN1M/LEN2M) keep the surrounding loads/stores in source order
static inline qwordV DivBy(qwordV v,qwordV n){qwordV r;__asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(r):"j"(n),"j"(v):"memory");return r;}
// A float's bit pattern into a VU register.
static inline qwordV SplatBits(int bits){qwordV q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(bits));return q;}
#ifdef SLY2_EE_ASM
#define LEN1M(d,one,v) __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(v):"memory")
#define LEN2M(d) __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(d): :"memory")
#else
#define LEN1M(d, one, v) ((d) = VuDot((v), (v)))   /* |v|^2 in x */
#define LEN2M(d) ((d) = VuSqrtX(d))
#endif
// State change. Leaving idle/turn/startled restores the rotation XF's role. Idle: stops, picks the idle time. Walk:
// walking mode, picks the walk time. Turn: reverses the path direction and heads the other way. Startled: stops and
// doubles the spin ratio. Flee: runs along the flee path from its closest point (fading out fadeTime before the end)
// or, without one, away from the closest player for 2 s, at the flee speed. Hit: a sound and SprayDifficultyPrizes's effect
// at damageOffset (amount, -100), then removed (vtable slot 9). Every state but turn plays its animation.
// Also known as: SetBirdState (upstream readable / Sly 1 name; SetBirdState__FP2LOi).
void SetCritterState(CID96V *critter, int next)
{
    int old = critter->state;
    if (old == next) return;
    WORK w;
    switch (old) {
    case CRITTER_NONE:
    case CRITTER_IDLE:
    case CRITTER_TURN:
    case CRITTER_STARTLED:
        SetXfRole(critter, critter->rotationXf, critter->defaultRole);
        WakeXfOwner(critter, critter->rotationXf);
        break;
    }
    switch (next) {
    case CRITTER_IDLE:
        AsSo(critter)->SetVelocity(D_002A2DD0.q);
        ResetXfValueRef(critter, critter->rotationXf);
        {
            float now = g_clock.time;
            critter->nextStateTime = now + RandomFloatInRange__Fff(critter->idleWaitMin, critter->idleWaitMax);
        }
        break;
    case CRITTER_WALK:
        AsSo(critter)->SetFlags(4);
        {
            float now = g_clock.time;
            critter->nextStateTime = now + RandomFloatInRange__Fff(critter->walkWaitMin, critter->walkWaitMax);
        }
        break;
    case CRITTER_TURN: {
        critter->pathStep = -critter->pathStep;
        float negative = -1.0f;
        M4 *m = (M4 *)AsSo(critter)->GetWorldMatrix();
        qwordV row = m->row[0].q;
        qwordV factor = SplatInto(negative, &w.first);
        qwordV scaled = ScaleX(row, factor);
        GetVectorSphere(scaled, &w.angle, 0, 0);
        SetCritterHeading(critter, w.angle);
        SetZeroVelocity(critter);
        SetXfRole(critter, critter->rotationXf, 1);
        WakeXfOwner(critter, critter->rotationXf);
        break;
    }
    case CRITTER_STARTLED:
        SetZeroVelocity(critter);
        SetStepSpinRatio((STEP *)critter, critter->spinRatio + critter->spinRatio);
        SetXfRole(critter, critter->rotationXf, 1);
        WakeXfOwner(critter, critter->rotationXf);
        break;
    case CRITTER_FLEE: {
        w.first.q = GetLoWorldPos(critter);
        if (critter->fleePath) {
            FindShapeClosestPoint(critter->fleePath, w.first.q, 0, &w.second, 0, &critter->pathU, &w.parameter);
            float phase = GetShapeParamSearchStep(critter->fleePath);
            qwordV delta = VuSub(w.first.q, w.second.q);
            float speed = critter->fleeSpeed.value;
            float travel = VuToFloat(LengthV(delta));
            critter->pathStep = phase;
            travel /= speed;
            float end = GetShapeMaxDistance(critter->fleePath);
            critter->fleeEndTime = g_clock.time + travel + (end - w.parameter) / critter->fleeSpeed.value - critter->fadeTime;
        } else {
            LOV *near = FindClosestSwPo(&g_sw, w.first.q);
            w.second = w.first;
            w.first.q = GetLoWorldPos(near);
            critter->fleeEndTime = g_clock.time + 2.0f;
        }
        qwordV ff = w.first.q;
        qwordV ss = w.second.q;
        qwordV gap = VuSub(ss, ff);
        w.direction.q = gap;
        w.direction.i[2] = 0;   /* horizontal */
        qwordV point = w.direction.q;
        // register pin: local-alloc would otherwise give vf1 to the unused `one` scratch of LEN1M (the original has the length in vf1)
        register qwordV norm __asm__("vf1");
        qwordV one;
        LEN1M(norm, one, point);
        qwordV fallback = D_002A2DA0.q;
        LEN2M(norm);
        float length = VuToFloat(norm);
        qwordV direction;
        if (length < 0.0001f) direction = fallback;
        else direction = DivBy(point, norm);
        qwordV factor = SplatBits(critter->fleeSpeed.bits);
        w.scalar.q = factor;
        qwordV velocity = ScaleX(direction, factor);
        w.velocity.q = velocity;
        AsSo(critter)->SetVelocity(velocity);
        AsSo(critter)->SetFlags(4);
        break;
    }
    case CRITTER_HIT: {
        EFFECT *sound = CreateSfxSound(0x14);
        SetSndSourceLo(sound, 0);
        SetSndSourcePos(sound, GetLoWorldPos(critter));
        SprayDifficultyPrizes(D_002DE578, ConvertPosBetweenLos(critter, 0, critter->damageOffset.q), critter->amount, -100, critter);
        AsSo(critter)->Remove();
        break;
    }
    }
    critter->state = next;
    critter->stateTime = g_clock.time;
    if (next != CRITTER_TURN) {
        ReplaceCurrentAseg(critter->stateAnims[next].paseg, (ALO *)critter, 0.0f, 1.0f, critter->stateAnims[next].applyFlags | 1, &critter->segmentAnim);
    }
}

/* SetCid96SpinRatio (docs/engine.md section 9): the symbol still says STEP. */
// Spin spring parameters: D_002C0248 {25, 0, 0.25} scaled by (ratio, ratio, 1/sqrt(ratio)); a larger ratio turns faster.
void SetStepSpinRatio(STEP *pstep, float ratio)
{
    CID96V *critter = (CID96V *)pstep;
    critter->spinScale = D_002C0248;
    critter->spinScale.x *= ratio;
    critter->spinScale.y *= ratio;
    critter->spinScale.z *= 1.0f / SqrtFloat(ratio);
    critter->spinRatio = ratio;
}

// Sets the heading (radians about the up axis) and its rotation.
// Also known as: SetBirdHeading (upstream readable / Sly 1 name; SetBirdHeading__FP4BIRDf).
void SetCritterHeading(CID96V *critter, float heading)
{
    critter->heading = heading;
    critter->headingBasis = BuildAxisAngleRotationMatrix(g_normalZ.q, heading);
}

// True when the critter's x axis (world matrix, vtable slot 31, read twice) is within 1 degree of the heading.
// Also known as: FBirdFacing (upstream readable / Sly 1 name; FBirdFacing__FP2LO).
int IsCritterFacingHeading(CID96V *critter)
{
    float *a = (float *)AsSo(critter)->GetWorldMatrix();
    float *b = (float *)AsSo(critter)->GetWorldMatrix();
    float error = NormalizeAngle(critter->heading - atan2f(a[1], b[0]));
    if (__builtin_fabsf(error) < 0.017453294f)   /* 1 degree */
        return 1;
    return 0;
}

// Length of a vector as a float.
static inline float Length(qwordV v)
{
    qwordV d, one;
    float f;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
// The closest player startles the critter when within nearDistance, or within farDistance while moving at 150 or
// more (velocity: vtable slot 33); the critter then heads the player's way (the startled turn) and the result is
// CRITTER_STARTLED. Otherwise the current state.
// Also known as: StBirdSees (upstream readable / Sly 1 name; StBirdSees__FP2LO).
int ChooseCritterStateFromPlayer(CID96V *critter)
{
    int result = critter->state;
    qwordV pos = GetLoWorldPos(critter);
    LOV *player = FindClosestSwPo(&g_sw, pos);
    U d;
    float distance;
    if (!player)
        return result;
    qwordV d_q;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d_q) : "j"(GetLoWorldPos(player)), "j"(pos));   /* via a scalar (C++) */
    d.q = d_q;
    distance = Length(d.q);
    if (critter->farDistance < distance)
        return result;
    if (critter->nearDistance < distance) {
        qwordV v = AsLo(player)->GetVelocity();
        if (Length(v) < 150.0f)
            return result;
    }
    SetCritterHeading(critter, atan2f(d.f[1], d.f[0]));
    return CRITTER_STARTLED;
}

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) V;
// Root-motion speed of a clip on this critter: |x at the end - x at the start| / duration * master speed (the clip's
// channel for the critter, FindAsegChannel, its curve evaluated with AC::Evaluate). Leaves *speed alone without one.
// Also known as: FStepLength (upstream readable / Sly 1 name; FStepLength__FP4ASEGP3ALOPf).
void GetAsegSpeedForObject(CID96V *critter, ASEG *clip, float *speed)
{
    V start, end;
    CHN *channel;
    if (!clip) return;
    channel = FindAsegChannel(clip, critter, 1, 1);
    if (!channel) return;
    channel->pac->Evaluate(0.0f, 1.0f, &start, 0);
    channel->pac->Evaluate(clip->duration, 1.0f, &end, 0);
    *speed = __builtin_fabsf(end.x - start.x) / clip->duration * clip->masterSpeed;
}

// Is this a state index (0..5)?
int IsValidCritterLinkIndex(unsigned int n)
{
    return n < 6U;
}

// Splice: sets the animation of a state (not turn or hit) and, unless -1, its apply flags.
// Also known as: SetStepSeg (upstream readable / Sly 1 name; SetStepSeg__FP8STEPSEGSi3OIDi).
extern "C" void SetCritterLink(CID96V *critter, int state, OID oid, int applyFlags)
{
    if (IsValidCritterLinkIndex(state) && state != CRITTER_TURN && state != CRITTER_HIT) {
        E96 *anim = &critter->stateAnims[state];
        anim->oid = oid;
        if (applyFlags != -1)
            anim->applyFlags = applyFlags;
    }
}

// Finds an animation by oid (from the critter) and removes it from the world (vtable slot 9).
ASEG *FindCritterLinkObject(CID96V *critter, OID oid)
{
    LOV *object;
    object = FindSwObject(&g_sw, 1, oid, (LOV *)critter);
    if (object != 0) {
        AsLo(object)->Remove();
    }
    return (ASEG *)object;
}

// Resolves the pending animation oids (each only once).
// Also known as: BindStepSegs (upstream readable / Sly 1 name; BindStepSegs__FP8STEPSEGS).
void ResolveCritterLinks(CID96V *critter)
{
    int i;
    E96 *anim;
    for (i = 0; i < 6; i++) {
        anim = &critter->stateAnims[i];
        if (anim->oid != -1) {
            anim->paseg = FindCritterLinkObject(critter, anim->oid);
            anim->oid = (OID)-1;
        }
    }
}
