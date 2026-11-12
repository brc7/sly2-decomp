// hurt.cpp -- the player's hurt state (JTHURT, stepplayer.h): taking damage from a zap (TakeJtDamage), the hurt
// state machine (SetJtHurtState, its callbacks and per-state handlers), zap reactions (PresetJtAccelZap,
// UpdateJtActiveZap), knock-back and water-current handling, and the invulnerability tests.
// TU 0x1A0B18..0x1A2BD0 (28 functions, address order).
// Entry points: TakeJtDamage (damage in), SetJtHurtState (state machine), UpdateJtActiveZap/PresetJtAccelZap (per
// frame), IsJtInvulnerable / ShouldStepplayerIgnoreZap (zap filters), UpdateStepplayerWater (water hazard).
//
// Types: stepplayer.h's JTHURT / ZPR / STEPPLAYER, jt.h's JT, so.h's SO and the classes they point at. Each function
// keeps its local helpers (register pins, by-value helper shapes the match needs) in a namespace named after it (as
// vec.cpp and mark.cpp do), with a C fallback for each asm statement (#ifdef SLY2_EE_ASM). The hurt callbacks are
// passed as SLY2_HURTFN (stepplayer.h): a g++ 2.95 pointer to member is the HURTEND record on the EE.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_hurt
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_hurt
#include "sly2/jt.h"          /* JT; stepplayer.h JTHURT, ZPR, STEPPLAYER */
#include "sly2/so.h"
#include "sly2/emitter.h"     /* EXPL, EXPLSO (the water splash) */
#include "sly2/sw.h"
#include "sly2/cm.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/clock.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/game.h"
#include "sly2/xp.h"
#include "sly2/classes.h"     /* WATER */
#include "sly2/lsg.h"
#include "sly2/act.h"         /* CHAINLINK / CHAIN (the tail) */
#include "sly2/aseg.h"
#include "sly2/guard.h"       /* STEPGUARD (TakeJtDamage's attacker) */
// The embedded movement components of a player (JTAIR at +0x2C0, JTGRAB +0x4C0, ... JTTHROW +0x11B0) are taken from
// the real STEPPLAYER layout in sly2/stepplayer.h; the per-function views only declare the words they read.
static inline STEPPLAYER *AsStepplayer(void *player) { return (STEPPLAYER *)player; }
// The real classes behind the per-function object views below, for their virtual calls.
static inline ::LO *AsLo(void *plo) { return (::LO *)plo; }

// 0x1A2A78 StaticInit_hurt and GlobalConstructors_hurt: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* The two contact fields the hurt state sets (engine/physics/mark.cpp's XP: friction +0x94, elasticity +0x98). */

extern "C" {
extern float D_002BFB20;    /* hurt contact friction (defined below, in address order) */
extern float D_002BFB24;    /* hurt contact elasticity */
int IsAsegaRetracted(ASEGA *pasega);
void RunJtHurtCallback(JTHURT *pjthurt);
void ReturnJtToSafePlaceCallback(JTHURT *pjthurt);
void JtHurtEndCallback(JTHURT *hurt);
void SetJtHurtCallback(JTHURT *hurt, HURTEND k);
void JtHurtShockCallback(JTHURT *hurt);
void JtHurtFallCallback(JTHURT *hurt);
void JtHurtFallCallback2(JTHURT *hurt);
void ReturnJtToSafePlace(STEPPLAYER *pstepplayer, int n);
void SetJtHurtState(JTHURT *pjthurt, int nState);
void SetJtAirJump(JTAIR *pjtair, int nState, int n2, int n3);   /* HYPOTHESIS: sets the air state */
int IsJtGrabBusy(JTGRAB *pjtgrab, int zpk);
int IsStepplayerInvulnerable(STEPPLAYER *pstepplayer, int zpk);
void SetSoEyesClosed(STEPPLAYER *pstepplayer, float g);
void OnStepplayerHeroDied(STEPPLAYER *pstepplayer);
extern int D_002E1E00;
int IsSwHandsOff(SW *psw);
}

// Initializes the player's hurt/zap context (STEPPLAYER+0xDF0): owner, state -1, status -1, 2.0 s default and a
// count of 0x28.
// Also known as: InitDamage (upstream readable / Sly 1 name; InitDamage__FP5ACTORi).
extern "C" void InitJtHurt(JTHURT *hurt, STEPPLAYER *player)
{
    hurt->player = player;
    hurt->state = -1;
    hurt->postHitMode = -1;
    hurt->postHitDuration = 2.0f;
    hurt->maxHitPoints = 0x28;
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A6720 __asm__("D_002A6720") = 3.40282347e+38f; }
// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_SetJtHurtState {
// Zero the xyz words.
union U{float f[4];qword q;int bits[4];void Clear(){bits[0]=bits[1]=bits[2]=0;}};   // floats first: U globals are initialized
struct POLY{float a,b,c;};
struct LIMIT{float lo,hi;};

 extern U D_002A2DD0 __asm__("D_002A2DD0");
 // hurt's .data starts here (address order across the namespaces below). This namespace has no extern "C" block, so
 // the definitions carry it themselves (C symbols).
 extern U D_002BFA00 __asm__("D_002BFA00");   /* the tail's gravity while hurt */
extern U D_002BFA10 __asm__("D_002BFA10");    /* the tail's gravity in states 3 and 10 */
 extern POLY D_002BFA20 __asm__("D_002BFA20");   /* heading blend of state 8 by speed (quadratic) */
 extern LIMIT g_lmZeroOne __asm__("g_lmZeroOne");
extern const float D_002A6720 __asm__("D_002A6720");   /* defined below where the ELF has it (.rodata order) */
 int IsBasicDerivedFrom__FP5BASIC3CID(STEPPLAYER*,int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
 float atan2f(float,float) __asm__("atan2f");
 float NormalizeAngle__Ff(float) __asm__("NormalizeAngle__Ff");
 void SetStepTargetHeading(STEPPLAYER*,float) __asm__("SetStepTargetHeading");
 void FixStepAngularVelocity(STEPPLAYER*) __asm__("FixStepAngularVelocity");
 void SetStepTargetHeadingFromVector(STEPPLAYER*,qword) __asm__("SetStepTargetHeadingFromVector");
 qword GetLoWorldPos(STEPPLAYER*) __asm__("GetLoWorldPos");
 float GetStepHeading(STEPPLAYER*) __asm__("GetStepHeading");
 void InvalidateSoAaox(STEPPLAYER*) __asm__("InvalidateSoAaox");
 void ReleaseAsega(ASEGA**) __asm__("ReleaseAsega");
 void ReleaseSma(SMA**) __asm__("ReleaseSma");
 void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(ASEG*,STEPPLAYER*,float,float,int,ASEGA**) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");
 void ReplaceSma(void*,STEPPLAYER*,int,void*,SMA**) __asm__("ReplaceSma");

// The tail chains of Sly (CID 0x35): JT::tailXfs[i] -> XF::plink -> CHAINLINK::chain -> CHAIN::gravity. Sly's model has
// no tail transforms, so tailXfs are null and the EE stores through null (whatever low memory holds at +0xA8); the
// native build skips a missing tail.
static inline CHAIN *TailChain(STEPPLAYER *player, int i) { return ((JT *)player)->tailXfs[i]->plink->chain; }
#if defined(SLY2_EE_LAYOUT)
#define SLY2_HAS_TAIL(player, i) 1
#else
#define SLY2_HAS_TAIL(player, i) (((JT *)(player))->tailXfs[i] && ((JT *)(player))->tailXfs[i]->plink)
#endif
static inline void SetTailGravity(STEPPLAYER *player, int i, const U &g) { if (SLY2_HAS_TAIL(player, i)) TailChain(player, i)->gravity = g.q; }
static inline void ClearTailGravity(STEPPLAYER *player, int i)
{
    if (SLY2_HAS_TAIL(player, i)) {
        VECTOR &g = TailChain(player, i)->vecGravity;
        g.x = g.y = g.z = 0;
    }
}
// The clip of a hurt state that has none of its own: stateClips[0] below state 8, stateClips[8] from state 8 on (the
// byte offset is the original's form).
static inline ASEG *DefaultClip(JTHURT *hurt, int next) { ASEG **base = hurt->stateClips; int offset = next < 8 ? 0 : 8 * (int)sizeof(ASEG *); return *(ASEG **)((char *)base + offset); }

#ifdef SLY2_EE_ASM
// Load the main camera's position into a GPR.
static inline qword Position()
{
    register CM *manager __asm__("$2") = &g_cm;
    register qword value __asm__("$2");
    __asm__("lq %0,%1" : "=r"(value) : "m"(*(qword *)&manager->pos));
    return value;
}
// Move a 128-bit GPR value into a VU0 register.
static inline qword From(qword a){qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(a));return r;}
// x lane of a as a float.
static inline float X(qword a){int word;float f;__asm__("qmfc2.ni %0,%1":"=r"(word):"j"(a));__asm__("mtc1 %1,%0":"=f"(f):"r"(word));return f;}
// *a - b, fetching the hurt context's player (register-pinned).
static inline qword Difference(U *a, qword b, JTHURT *c, STEPPLAYER *&player)
{
    register qword x __asm__("vf1"), y __asm__("vf2");
    register STEPPLAYER *owner __asm__("$4");
    __asm__("lqc2 %0,%3\n\tqmtc2.ni %4,%1\n\tlw %2,%5\n\tvsub.xyzw %0,%0,%1"
            : "=&j"(x), "=&j"(y), "=r"(owner)
            : "m"(a->q), "r"(b), "m"(c->player));
    player = owner;
    return x;
}
// *a - b, fetching the hurt context's player (second register variant).
static inline qword Difference1(U *a, qword b, JTHURT *c, STEPPLAYER *&player)
{
    register qword x __asm__("vf4"), y __asm__("vf1");
    register STEPPLAYER *owner __asm__("$4");
    __asm__("qmtc2.ni %4,%1\n\tlw %2,%5\n\tlqc2 %0,%3\n\tvsub.xyzw %0,%0,%1"
            : "=&j"(x), "=&j"(y), "=r"(owner)
            : "m"(a->q), "r"(b), "m"(c->player));
    player = owner;
    return x;
}
#else
static inline qword Position() { return g_cm.pos; }
static inline qword Difference(U *a, qword b, JTHURT *c, STEPPLAYER *&player) { player = c->player; return VuSub(a->q, b); }
static inline qword Difference1(U *a, qword b, JTHURT *c, STEPPLAYER *&player) { player = c->player; return VuSub(a->q, b); }
#endif
// Clamp value to [lo, range->hi].
static inline float ClampKnownLower(float value, float lo, LIMIT *range)
{
#ifdef SLY2_EE_ASM
    register float hi __asm__("$f1") = range->hi;
#else
    float hi = range->hi;
#endif
    if (value < lo)
        return lo;
    if (hi < value)
        return hi;
    return value;
}
// Hurt sub-state switch (leave/enter): Sly-specific settings for CID 0x35, facing and launch from the hit direction,
// retracting/applying hit clips, and the death states 8..12.
// Also known as: SetDamageState (upstream readable / Sly 1 name; SetDamageState__FP5ACTORi).
extern "C" void SetJtHurtState(JTHURT *hurt, int next)
{
    U velocity, camera0, camera1;
    if (next == hurt->state)
        return;
    switch (hurt->state) {
    case -1:
        if (IsBasicDerivedFrom__FP5BASIC3CID(hurt->player, 0x35)) {
            STEPPLAYER *player = hurt->player;
            SetTailGravity(player, 0, D_002BFA00);
            SetTailGravity(player, 1, D_002BFA00);
        }
        hurt->player->EndMeleeAttack();
        ((U *)&hurt->player->localVelocity)->Clear();   /* x, y, z (the form the match needs) */
        hurt->player->canCollect = 0;
        hurt->recoveredTime = D_002A6720;
        break;
    case 3:
    case 10:
        if (IsBasicDerivedFrom__FP5BASIC3CID(hurt->player, 0x35))
            ClearTailGravity(hurt->player, 0);
        break;
    }
    ASEG *clip = 0;
    int flags = 1;
    switch (next) {
    case -1:
        if (IsBasicDerivedFrom__FP5BASIC3CID(hurt->player, 0x35)) {
            STEPPLAYER *player = hurt->player;
            ClearTailGravity(player, 0);
            ClearTailGravity(player, 1);
        }
        hurt->player->canCollect = 1;
        hurt->recoveredTime = g_clock.time;
        break;
    case 8: {
        qword moving = hurt->player->GetVelocity();
        STEPPLAYER *player = hurt->player;
#ifdef SLY2_EE_ASM
        register qword mv __asm__("vf4") = From(moving), axis __asm__("vf1"), dot __asm__("vf2"), one __asm__("vf3");
        float threshold;
        __asm__("li.s %0,%F2\n\tlqc2 %1,%3" : "=&f"(threshold), "=j"(axis) : "F"(-250.0f), "m"(player->aqwTarget[0]));
        __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
                : "=&j"(dot), "=&j"(one)
                : "j"(mv), "j"(axis));
        if (X(dot) > threshold)
#else
        /* the velocity along the facing row: moving forward faster than -250 falls forward (state 9) */
        if (VuToFloat(VuDot(moving, player->aqwTarget[0])) > -250.0f)
#endif
            next = 9;
        else {
            float fraction, lower;
            LIMIT *range;
#ifdef SLY2_EE_ASM
            register qword v __asm__("vf2") = From(player->GetVelocity());
            register LIMIT *rangeReg __asm__("$2");
            __asm__("lui $3,%%hi(%5)\n\tvmul.xy $vf1,%4,%4\n\tvaddy.x $vf1,$vf1,$vf1y\n\taddiu %2,$3,%%lo(%5)\n\tvsqrt "
                    "$Q,$vf1x\n\tvwaitq\n\tvaddq.x $vf1,$vf0,$Q\n\tlwc1 $f1,8(%2)\n\tqmfc2.ni $4,$vf1\n\tlwc1 "
                    "$f2,4(%2)\n\tmtc1 $4,$f0\n\tlwc1 %0,%%lo(%5)($3)\n\tmul.s $f1,$f0,$f1\n\tlui %2,%%hi(%6)\n\tlwc1 "
                    "%1,%%lo(%6)(%2)\n\tsqc2 %4,%3\n\taddiu %2,%2,%%lo(%6)\n\tadd.s $f2,$f2,$f1\n\tmul.s "
                    "$f0,$f0,$f2\n\tadd.s %0,%0,$f0"
                    : "=&f"(fraction), "=&f"(lower), "=&r"(rangeReg), "=m"(velocity.q)
                    : "j"(v), "i"(&D_002BFA20), "i"(&g_lmZeroOne)
                    : "$3", "$4", "$f0", "$f1", "$f2", "vf1", "memory");
            range = rangeReg;
#else
            /* the planar speed s: fraction = a + s * (b + s * c), clamped to [0, 1] */
            velocity.q = player->GetVelocity();
            float speed = VuSqrt(velocity.f[0] * velocity.f[0] + velocity.f[1] * velocity.f[1]);
            fraction = D_002BFA20.a + speed * (D_002BFA20.b + speed * D_002BFA20.c);
            lower = g_lmZeroOne.lo;
            range = &g_lmZeroOne;
#endif
            float blend = ClampKnownLower(fraction, lower, range);
            float angle = atan2f(velocity.f[1], velocity.f[0]) + 3.1415927f;
            angle = NormalizeAngle__Ff(angle - hurt->player->targetHeading);
            SetStepTargetHeading(hurt->player, hurt->player->targetHeading + angle * blend);
            FixStepAngularVelocity(hurt->player);
        }
    }
    case 9:
        clip = hurt->stateClips[next];
        flags = 0x21;
        break;
    case 10:
        camera0.q = Position();
        {
            STEPPLAYER *owner;
            qword direction = Difference(&camera0, GetLoWorldPos(hurt->player), hurt, owner);
            SetStepTargetHeadingFromVector(owner, direction);
        }
        flags = 0x21;
    case 3:
        clip = hurt->stateClips[next];
        if (IsBasicDerivedFrom__FP5BASIC3CID(hurt->player, 0x35))
            SetTailGravity(hurt->player, 0, D_002BFA10);
        break;
    case 5:
    case 11:
        camera1.q = Position();
        {
            STEPPLAYER *owner;
            qword direction = Difference1(&camera1, GetLoWorldPos(hurt->player), hurt, owner);
            SetStepTargetHeadingFromVector(owner, direction);
        }
        if (next == 11)
            flags = 0x21;
        clip = hurt->stateClips[next];
        break;
    case 0:
    case 6:
    case 7: clip = hurt->stateClips[next]; break;
    case 4:
    case 12:
        hurt->player->SetAngularVelocity(D_002A2DD0.q);
        SetStepTargetHeading(hurt->player, GetStepHeading(hurt->player));
        hurt->player->controller->leftStickAngle = hurt->player->targetHeading;
        if (next == 12)
            flags = 0x21;
        clip = hurt->stateClips[next];
        break;
    case 1:
    case 2:
        flags = 0;
        SetStepTargetHeading(hurt->player, GetStepHeading(hurt->player));
        hurt->player->controller->leftStickAngle = hurt->player->targetHeading;
        clip = hurt->stateClips[next];
        break;
    }
    InvalidateSoAaox(hurt->player);
    hurt->state = next;
    hurt->stateTime = g_clock.time;
    if (next == -1)
        return;
    if (!clip)
        clip = DefaultClip(hurt, next);
    ReleaseAsega(&hurt->player->activeAnimation);
    ReleaseSma(&hurt->player->activeStateMachine);
    ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(clip, hurt->player, 0.0f, 1.0f, flags, &hurt->player->activeAnimation);
    ReplaceSma(0, hurt->player, -1, 0, &hurt->player->activeStateMachine);
    hurt->player->ApplyStateStepParams();
}
} // namespace M_SetJtHurtState

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001A1098 {
struct D { int a; int b; };
extern HURTZ D_002BFA30[7] __asm__("D_002BFA30");
HURTZ *AllocCopyFromWorldHeap(struct D *d, void *p) __asm__("AllocCopyFromWorldHeap");
XF *FindSoXf(STEPPLAYER *a, int b, int c) __asm__("FindSoXf");
// Post-load: allocates the 7-entry hurt table and looks up the hurt-related child transforms.
extern "C" void PostLoadJtHurt(JTHURT *s)
{
    struct D d;
    int i;
    s->churtz = 7;
    d.a = 0x54;
    d.b = 0x10;
    s->ahurtz = AllocCopyFromWorldHeap(&d, D_002BFA30);
    for (i = 0; i < s->churtz; i++) {
        HURTZ *e = &s->ahurtz[i];
        e->pxf = FindSoXf(s->player, e->id, 6);
    }
}
} // namespace M_func_001A1098

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_PresetJtAccelZap {
struct X { char pad[0x80]; qword v80; };
struct O : LO { char pad1[0xE8-0x50]; struct X *xE8; char pad2[0x198-0xEC]; float f198; void Set(qword v) __asm__("AddSoAcceleration"); void Negate(QWROW &u); };
struct P { struct O *o; int state; };
extern qword D_002A2DD0 __asm__("D_002A2DD0");
extern float D_002BFA90[3] __asm__("D_002BFA90");   /* the zap spring */

extern void AccelSoTowardPosSpring(struct O *p,qword a,int n,qword b,float *c) __asm__("AccelSoTowardPosSpring");
// Broadcast g into a VU0 register and store it to u.
static inline qword BroadcastTo(float g,QWROW *u) { qword q=VuFromFloat<qword>(g); u->q=q;return q; }
// Set the vector to -xE8->v80 (keeping the -1 broadcast in u).
inline void O::Negate(QWROW &u) { qword v=xE8->v80; qword k=VuFromFloat<qword>(-1.0f); u.q=k; Set(VuScaleByX(v,k)); }
// Physics preset in the hurt state: in knockback states adds velocity or springs toward the target velocity.
// Also known as: UpdateDamageMotion (upstream readable / Sly 1 name; UpdateDamageMotion__FP5ACTOR).
extern "C" void PresetJtAccelZap(struct P *p)
{
 QWROW u,v,u2;
 qword k;
 float zero;
 switch(p->state) {
 case 1:case 2:
  p->o->Negate(u);
  AccelSoTowardPosSpring(p->o,D_002A2DD0,0,D_002A2DD0,D_002BFA90);
  break;
 case 5:case 11:
  p->o->Negate(u2);
  v.i[2]=0;v.i[1]=0;v.i[0]=0;
  zero=v.f[2];
  if(p->o->f198!=zero)
   v.f[2]=QwordZ(p->o->GetVelocity());
  AccelSoTowardPosSpring(p->o,D_002A2DD0,0,v.q,D_002BFA90);
  if(p->o->f198!=zero) {
   u.f[2]=zero;u.f[1]=zero;u.f[0]=zero;
   u.f[2]=(-p->o->f198-v.f[2])/g_clock.deltaTime;
   p->o->Set(u.q);
  }
  break;
 }
}
} // namespace M_PresetJtAccelZap

/* Runs the hurt callback each time the player's current animation (activeAnimation) has retracted, until the state
   stops changing (or is -1). */
// When the owner's current animation has retracted, runs the pending post-hurt callback (RunJtHurtCallback),
// repeating while the state keeps changing. (inferred)
// Also known as: UpdateDamageTransitions (upstream readable / Sly 1 name; UpdateDamageTransitions__FP5ACTOR).
extern "C" void UpdateJtHurtInput(JTHURT *hurt)
{
    int state;
    do {
        state = hurt->state;
        if (IsAsegaRetracted(hurt->player->activeAnimation))
            RunJtHurtCallback(hurt);
    } while (hurt->state != -1 && hurt->state != state);
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_UpdateJtActiveZap {
struct LO;
struct LO { int x0, x4, x8; void *vptr; char pad10[0xD0 - 0x10]; float xD0; char padD4[0x1A8 - 0xD4]; float x1A8; char pad1AC[0x208 - 0x1AC]; int x208; };
struct S { struct LO *po; int x4; float x8; char pad[0x90 - 0xC]; int x90; float x94, x98, x9C; };
extern float D_002BFAA0[3] __asm__("D_002BFAA0");
extern float D_002BFAB0[3] __asm__("D_002BFAB0");
extern float D_002BFAC0[3] __asm__("D_002BFAC0");
extern float g_lmZeroOne[2] __asm__("g_lmZeroOne");
void InvalidateSoAaox(struct LO *po) __asm__("InvalidateSoAaox");
float cosf(float g) __asm__("cosf");
float atan2f(float y, float x) __asm__("atan2f");
float SmoothAngle__FfffP3SMPPf(void *p, int n, float a, float b, float c) __asm__("SmoothAngle__FfffP3SMPPf");
void SetStepTargetHeading(struct LO *po, float g) __asm__("SetStepTargetHeading");
int IsJtStuck(struct LO *po, int n, float g) __asm__("IsJtStuck");
void RunJtHurtCallback(struct S *s) __asm__("RunJtHurtCallback");
// Per-frame update of the hurt state (jts 4): smooths the heading, advances timers, runs the pending callback and
// escapes when stuck (IsJtStuck).
// Also known as: UpdateDamage (upstream readable / Sly 1 name; UpdateDamage__FP5ACTOR).
extern "C" void UpdateJtActiveZap(struct S *s)
{
    if (s->x90 != -1) {
        float u;
        float w;
        float a;
        if (s->x4 == -1) {
            float dt = g_clock.time - s->x98;
            if (s->x9C < dt) {
                s->x90 = -1;
                s->po->xD0 = 0.0f;
                InvalidateSoAaox(s->po);
                goto done;
            }
            u = dt / s->x9C;
        } else {
            u = 0.0f;
        }
        w = cosf((g_clock.time - s->x94) * 10.0f);
        a = Clamp(D_002BFAA0[0] + u * (D_002BFAA0[1] + u * D_002BFAA0[2]), g_lmZeroOne[0], g_lmZeroOne[1]);
        s->po->xD0 = a * Clamp(D_002BFAB0[0] + w * (D_002BFAB0[1] + w * D_002BFAB0[2]), g_lmZeroOne[0], g_lmZeroOne[1]);
    }
done:
    switch (s->x4) {
    case -1:
        break;
    case 7: {
        union { qword q; float f[4]; } v;
        float g;
        v.q = AsLo(s->po)->GetVelocity();
        g = atan2f(v.f[1], v.f[0]) + 3.1415927f;
        SetStepTargetHeading(s->po, SmoothAngle__FfffP3SMPPf(D_002BFAC0, 0, s->po->x1A8, g, g_clock.deltaTime));
        if (0.25f < g_clock.time - s->x8 && s->po->x208)
            RunJtHurtCallback(s);
        else if (IsJtStuck(s->po, 0, s->x8))
            RunJtHurtCallback(s);
        break;
    }
    }
}
} // namespace M_UpdateJtActiveZap

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001A15B0 {
// Stores a pointer-to-member callback to run when the hurt animation ends.
extern "C" void SetJtHurtCallback(JTHURT *hurt, HURTEND k) { hurt->hurtend = k; }
} // namespace M_func_001A15B0

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_RunJtHurtCallback {
void *func_002333B4(void *p, int c, unsigned int n) __asm__("memset");
// Takes and clears the pending callback and calls it; with none, the death states return to a safe place and the
// others start a fall (SetJtAirJump).
// Also known as: EndDamageReaction (upstream readable / Sly 1 name; EndDamageReaction__FP5ACTOR).
extern "C" void RunJtHurtCallback(JTHURT *s)
{
    HURTEND pmf = s->hurtend;
    func_002333B4(&s->hurtend, 0, sizeof(s->hurtend));
    if (pmf.index) {
        pmf.pfn((char *)s + pmf.delta);
    } else if ((unsigned int)(s->state - 8) < 5) {
        s->player->OnHeroDied();
    } else if (s->zpr.ploSource && ((SO *)s->zpr.ploSource)->hurtRespawnsHero) {
        ReturnJtToSafePlaceCallback(s);
    } else {
        SetJtAirJump(&s->player->jtair, 11, 0, 0);
    }
}
} // namespace M_RunJtHurtCallback

// Callback thunk: ReturnJtToSafePlace(owner, null).
extern "C" void ReturnJtToSafePlaceCallback(JTHURT *hurt)
{
    ReturnJtToSafePlace(hurt->player, 0);
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001A16C8 {
// Select and blend the movement vector for the active mode.
typedef union U{float f[4];qword q;int bits[4];}U;   // floats first: U globals are initialized
typedef union F{float value;int bits;}F;
struct M3{U row[3];};
struct P{LO*self;char pad4[0x30-4];LO*target;char pad34[8];int mode;char pad40[0x20];U velocity;};
extern F D_002BFACC __asm__("D_002BFACC");
extern F D_002BFAD0 __asm__("D_002BFAD0");
extern F D_002BFAD4 __asm__("D_002BFAD4");
extern U D_002BFAE0 __asm__("D_002BFAE0");
extern U D_002BFAF0 __asm__("D_002BFAF0");
extern F D_002BFB00 __asm__("D_002BFB00");
extern F D_002BFB04 __asm__("D_002BFB04");
extern U D_002A2DA0 __asm__("D_002A2DA0"),g_normalZ __asm__("g_normalZ");
int IsBasicDerivedFrom__FP5BASIC3CID(LO*,int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
qword LimitVectorLength(qword,float) __asm__("LimitVectorLength");
qword GetLoWorldPos(LO*) __asm__("GetLoWorldPos");
struct M3*GetLoWorldMatrix(LO*) __asm__("GetLoWorldMatrix");
qword ConvertVecBetweenLos(LO*,LO*,qword) __asm__("ConvertVecBetweenLos");
void SetJtHurtState(struct P*,int) __asm__("SetJtHurtState");
// Broadcast a float into a VU0 register (via a GPR).
extern "C" { static inline qword Scalar(float f){qword q;int bits;__asm__("mfc1 %0,%1":"=r"(bits):"f"(f));__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(bits));return q;} }
// Move a float's bits into a GPR qword.
extern "C" { static inline qword Word(float f){qword q;__asm__("mfc1 %0,%1":"=r"(q):"f"(f));return q;} }
// Return v scaled by k.x.
extern "C" { static inline qword Scale(qword v,qword k){__asm__("vmulx.xyzw %0,%0,%1x":"+j"(v):"j"(k));return v;} }
// y lane of q as a float.
extern "C" { static inline float Z(qword q){register int bits __asm__("$3");float f;__asm__("prot3w %0,%1":"=r"(bits):"r"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(bits));return f;} }
// Unit vector along *u, or fallback when its length is too small (register-pinned).
extern "C" { static inline qword Normal(U*u,qword fallback){
    register qword v __asm__("vf3")=u->q;
    register qword d __asm__("vf1"),one __asm__("vf2"),r __asm__("vf4");
    register int bits __asm__("$3");float length;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=j"(one):"j"(v));
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(d));
    __asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(d));
    __asm__("mtc1 %1,%0":"=f"(length):"r"(bits));
    if(length<0.0001f)return fallback;
    __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(r):"j"(d),"j"(v));
    return r;
} }
// Return a*direction.x + b*z.x (operands as in the original).
extern "C" { static inline qword Blend(qword direction,qword z,qword a,qword b){__asm__("vmulax.xyzw $ACC,%1,%2x\n\tvmaddx.xyzw %0,%0,%3x":"+j"(z):"j"(direction),"j"(a),"j"(b));return z;} }
// self - (target + basis.row0 * negative).
extern "C" { static inline qword Intercept(U*self,U*target,struct M3*basis,U*negative){
    register qword position __asm__("vf2")=target->q;
    register qword axis __asm__("vf1")=basis->row[0].q;
    register qword scale __asm__("vf3")=negative->q;
    __asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%0,%2x":"+j"(axis):"j"(position),"j"(scale));
    position=self->q;
    __asm__("vsub.xyzw %0,%0,%1":"+j"(position):"j"(axis));
    return position;
} }
// *first - target (register-pinned).
extern "C" { static inline qword Difference(U*first,qword target){
    register qword a __asm__("vf2");
    register qword b __asm__("vf3");
    register qword r __asm__("vf1");
    __asm__("lqc2 %0,%2\n\tqmtc2.ni %3,%1":"=j"(a),"=j"(b):"m"(first->q),"r"(target));
    __asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;
} }
// Selects and blends the knockback velocity from the damage kind flags (0x200, 0x8000, ...) and the source
// direction, limits its length, then enters the hurt state.
// Also known as: ApplyDamageVelocity (upstream readable / Sly 1 name; ApplyDamageVelocity__FP5ACTOR).
extern "C" void StartJtKnockback(struct P*p){
    U output=p->velocity;
    U scratch,scalar;
    U selfPosition,targetPosition,negative,otherPosition;
    switch(p->mode){
    case 0x200:
    case 0x8000:
        if(IsBasicDerivedFrom__FP5BASIC3CID(p->target,0x33)){
            qword k=Scalar(D_002BFACC.value+1.0f);
            qword v=Scale(p->velocity.q,k);
            scratch.q=k;
            output.q=v;
            output.q=LimitVectorLength(v,D_002BFAD4.value);
            output.f[2]=Max(output.f[2],D_002BFAD0.value);
        }
        break;
    case 0x40:
        if(IsBasicDerivedFrom__FP5BASIC3CID(p->target,0x36)){
            LO*target=p->target;
            struct M3*basis;
            qword delta,local;
            selfPosition.q=GetLoWorldPos(p->self);
            targetPosition.q=GetLoWorldPos(target);
            {qword k=Word(-150.0f);scratch.q=k;negative.q=k;}
            basis=GetLoWorldMatrix(target);
            delta=Intercept(&selfPosition,&targetPosition,basis,&negative);
            local=ConvertVecBetweenLos(0,target,delta);
            if(Z(local)>0.0f)output.q=ConvertVecBetweenLos(target,0,D_002BFAE0.q);
            else output.q=ConvertVecBetweenLos(target,0,D_002BFAF0.q);
        }
        break;
    case 0x100:
        if(IsBasicDerivedFrom__FP5BASIC3CID(p->target,0x4F)){
            LO*target=p->target;
            qword n,a,b,mixed,z;
            if(IsBasicDerivedFrom__FP5BASIC3CID(target,0x51)){
                struct M3*basis=(struct M3*)target->GetWorldMatrix();   /* its rows as quadwords */
                scratch=basis->row[0];
            }else{
                qword targetPoint;
                otherPosition.q=GetLoWorldPos(p->self);
                targetPoint=GetLoWorldPos(target);
                scratch.q=Difference(&otherPosition,targetPoint);
            }
            scratch.bits[2]=0;
            n=Normal(&scratch,D_002A2DA0.q);
            {
                register int first __asm__("$4"),second __asm__("$3");
                register U*up __asm__("$2");
                register qword weight __asm__("vf2"),lift __asm__("vf3"),axis __asm__("vf1");
                // Read blend data and initialize the scalar temporary.
                __asm__("lui %0,%%hi(%7)\n\tlui %1,%%hi(%8)\n\tlui %2,%%hi(%9)\n\tlw %0,%%lo(%7)(%0)\n\taddiu %2,%2,%%lo(%9)\n\tlw %1,%%lo(%8)(%1)\n\tlqc2 %5,0(%2)\n\tqmtc2.ni %0,%3\n\tqmtc2.ni %1,%4\n\tsqc2 %3,%6"
                    :"=&r"(first),"=&r"(second),"=&r"(up),"=j"(weight),"=j"(lift),"=j"(axis),"=m"(scalar.q)
                    :"i"(&D_002BFB00.bits),"i"(&D_002BFB04.bits),"i"(&g_normalZ):"memory");
                a=weight;b=lift;z=axis;
            }

            mixed=Blend(n,z,a,b);
            scratch.q=n;
            output.q=mixed;
            // The C file stored a compound literal (U){b} here; as C++ that is an extra temporary. The empty asm keeps the
            // store last, where the original has it. This form exists to reproduce the original codegen.
            __asm__("":"+j"(b));scalar.q=b;
        }
        break;
    }
    {
        p->self->SetVelocity(output.q);
    }
    SetJtHurtState(p,7);
}
} // namespace M_func_001A16C8

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_FTakeJtDamage {
 extern char D_002DE578[] __asm__("D_002DE578");
 extern qword D_002A2DD0 __asm__("D_002A2DD0");
 union VQ{float f[4];qword q;};
 extern VQ D_002BFB10 __asm__("D_002BFB10");   // C symbol: this namespace has no extern "C" block
 int ShouldJtVetoDamage(JTHURT*,ZPR*) __asm__("ShouldJtVetoDamage");
 int ScaleByDifficulty(void*,int) __asm__("ScaleByDifficulty");
 void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(ASEG*,STEPPLAYER*,float,float,int,ASEGA**) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");
 int IsBasicDerivedFrom__FP5BASIC3CID(LO*,int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
 void DropStepplayerBooty(STEPPLAYER*) __asm__("DropStepplayerBooty");
 void BroadcastGameEvent(void*,int,LO*) __asm__("BroadcastGameEvent");
 void SetJtHurtCallback(JTHURT*,HURTFN) __asm__("SetJtHurtCallback");
 void StartJtKnockback(JTHURT*) __asm__("StartJtKnockback");
 int GetBasicCid(STEPPLAYER*) __asm__("GetBasicCid");
 void*CreateSfxSound(int) __asm__("CreateSfxSound");
 void SetSndSourceLo(void*,STEPPLAYER*) __asm__("SetSndSourceLo");
 qword GetLoWorldPos(STEPPLAYER*) __asm__("GetLoWorldPos");
 void*NewNoise(NOISETYPE*,qword,STEPPLAYER*) __asm__("NewNoise");

// a + b with unsigned wrap.
static inline int Add32(int a,int b){return (int)((unsigned)a+(unsigned)b);}
// z lane of value as a float.
#ifdef SLY2_EE_ASM
static inline float Z(qword value){register int word __asm__("$9");float result;__asm__("pextuw %0,$zero,%1":"=r"(word):"r"(value));__asm__("mtc1 %1,%0":"=f"(result):"r"(word));return result;}
#else
static inline float Z(qword value){return QwordZ(value);}
#endif
// Applies a damage record to the player: the owner may veto it (vt86), flag filters, damage scaled by difficulty,
// health counter, game event broadcast, hit clip, sound, knockback and callback. Returns nonzero if taken.
// Also known as: FAcceptDamage (upstream readable / Sly 1 name; FAcceptDamage__FP5ACTORP5CTMSG).
extern "C" int TakeJtDamage(JTHURT *p, ZPR *input)
{
    ZPR accepted;
    int delta;
    if (ShouldJtVetoDamage(p, input)) {
        if (p->state >= 8 && p->state <= 12)

            return 0;
        delta = -32768;
        goto notify;
    }
    if (p->player->IsInvulnerable((ZPK)input->zpk))
        return 0;
    if (p->player->jtgrab.state == 1)   /* grabbing: the grabbed object takes it */
        return ((STEPPLAYER *)p->player->jtgrab.grabbedObject)->TakeDamage(input);
    if (p->player->takesNoDamage)
        delta = 0;
    else if (p->postHitMode == 1)
        delta = 0;
    else
        delta = ScaleByDifficulty(D_002DE578, input->dhp);
    if (input->zpk == 0x100000) {
        if (Add32(*p->hitPoints, delta) > 0) {   /* a non-fatal 0x100000 zap: the flinch clip, no hurt state */
            if (p->postHitMode != 1) {
                p->postHitMode = 0;
                p->hitTime = p->recoveredTime = g_clock.time;
            }
            *p->hitPoints = Add32(*p->hitPoints, delta);
            if (p->flinchClip)
                ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(p->flinchClip, p->player, 0.0f, 1.0f, 1, &p->flinchAsega);
            goto sound;
        }
        /* a fatal one becomes a 0x200 knockback without velocity */
#ifdef SLY2_EE_ASM
        register ZPR *destination __asm__("$2");
        __asm__("daddu %0,%1,$zero" : "=r"(destination) : "r"(&accepted));
        *destination = *input;
#else
        accepted = *input;
#endif
        input = &accepted;
        accepted.zpk = 0x200;
#ifdef SLY2_EE_ASM
        __asm__("sw $zero,4(%3)\n\tsw $zero,8(%3)\n\tsw $zero,%2"
                : "=m"(accepted.vecVelocity.y), "=m"(accepted.vecVelocity.z), "=m"(accepted.vecVelocity.x)
                : "r"(&accepted.vecVelocity.x));
#else
        accepted.vecVelocity.x = accepted.vecVelocity.y = accepted.vecVelocity.z = 0;
#endif
    }
    if (input->ploSource && IsBasicDerivedFrom__FP5BASIC3CID(input->ploSource, 0x38)) {
        STEPGUARD *guard = (STEPGUARD *)input->ploSource;
        if (guard->specialState == 8 || guard->specialState == 13)
            delta /= 2;
    }
notify: {
    p->player->SetPlayerState(3);
    BOOTY *booty = (BOOTY *)p->player->pbooty;
    if (booty && (booty->pbootyeCur->dropsOnHit || Add32(*p->hitPoints, delta) <= 0))
        DropStepplayerBooty(p->player);
    BroadcastGameEvent(&g_game, 0xF, input->ploSource);
    p->zpr = *input;
    p->zpr.dhp = delta;
    SetJtHurtCallback(p, SLY2_HURTFN_NONE);
    int n = Add32(*p->hitPoints, p->zpr.dhp);
    *p->hitPoints = n >= 0 ? n : 0;
    if (*p->hitPoints > 0) {
        p->postHitMode = 1;
        p->hitTime = g_clock.time;
    }
    switch (input->zpk) {
    case 0x100:
    case 0x200: {
        StartJtKnockback(p);
        if (Z(p->player->GetVelocity()) > 400.0f) {
            int effect = -1;
            switch (GetBasicCid(p->player)) {
            case 0x35: effect = 0x69; break;
            case 0x36: effect = 0x84; break;
            case 0x37: effect = 0x83; break;
            }
            STEPPLAYER *player = p->player;
            void *created = CreateSfxSound(effect);
            SetSndSourceLo(created, player);
        }
        SetJtHurtCallback(p, SLY2_HURTFN(EndCallback, JtHurtEndCallback));
        break;
    }
    case 0x8000:
        SetJtHurtState(p, 7);
        SetJtHurtCallback(p, SLY2_HURTFN(ShockCallback, JtHurtShockCallback));
        break;
    case 0x800:
    case 0x1000:
        p->player->SetVelocity(D_002A2DD0);
        p->player->SetAngularVelocity(D_002A2DD0);
        SetJtHurtState(p, input->zpk == 0x800 ? 1 : 2);
        SetJtHurtCallback(p, SLY2_HURTFN(FallCallback, JtHurtFallCallback));
        break;
    case 0x2000:
        if (*p->hitPoints > 0) {
            SetJtHurtState(p, 3);
            SetJtHurtCallback(p, SLY2_HURTFN(SafePlaceCallback, ReturnJtToSafePlaceCallback));
        } else
            SetJtHurtState(p, 10);
        break;
    case 0x400:
        p->player->SetVelocity(D_002BFB10.q);
        if (*p->hitPoints > 0)
            SetJtHurtState(p, 4);
        else
            SetJtHurtState(p, 12);
        break;
    case 0x4000:
        if (*p->hitPoints > 0) {
            SetJtHurtState(p, 5);
            SetJtHurtCallback(p, SLY2_HURTFN(SafePlaceCallback, ReturnJtToSafePlaceCallback));
        } else
            SetJtHurtState(p, 11);
        break;
    case 0x40:
        StartJtKnockback(p);
        SetJtHurtCallback(p, SLY2_HURTFN(EndCallback, JtHurtEndCallback));
        break;
    }
}
sound:
#ifdef SLY2_EE_ASM
    register SW *game __asm__("$16") = &g_sw;
#else
    SW *game = &g_sw;
#endif
    qword point = GetLoWorldPos(p->player);
    NOISETYPE *channel = game->apstimk[6];
    (void)NewNoise(channel, point, p->player);
    return 1;
}
} // namespace M_FTakeJtDamage

// Callback: back to hurt state 0 if health remains, else state 8 (dead).
extern "C" void JtHurtEndCallback(JTHURT *hurt)
{
    if (*hurt->hitPoints > 0) {
        SetJtHurtState(hurt, 0);
        return;
    }
    SetJtHurtState(hurt, 8);
}

/* With hit points left: back to a safe place when the source respawns the hurt player (SO::hurtRespawnsHero), else air
   state 11; without: hurt state 8. */
// Callback after a hazard fall: if alive, returns to a safe place when the hazard object has flag bit 33, else starts
// a fall (SetJtAirJump kind 11); dead -> death state.
// Also known as: NextStunned (upstream readable / Sly 1 name; NextStunned__FP5ACTOR).
extern "C" void JtHurtFallCallback(JTHURT *hurt)
{
    if (*hurt->hitPoints > 0) {
        if (hurt->zpr.ploSource && ((SO *)hurt->zpr.ploSource)->hurtRespawnsHero)
            ReturnJtToSafePlaceCallback(hurt);
        else
            SetJtAirJump(&hurt->player->jtair, 11, 0, 0);
    } else {
        SetJtHurtState(hurt, 8);
    }
}

extern "C" {
extern const HURTEND D_002A6790;   /* JtHurtFallCallback2 as a HURTEND record (defined below, in .rodata order) */
void *CreateSfxSound(int n);
void SetSndSourceLo(void *psnd, int n);
#if defined(SLY2_EE_LAYOUT)
void func_001A15B0L(JTHURT *pjthurt, long cb) __asm__("SetJtHurtCallback");   /* SetJtHurtCallback taking the record as a long */
#endif
}
// Callback: plays sound 0x6A, enters hurt state 6 and chains the next callback (D_002A6790).
// Also known as: NextSpecial (upstream readable / Sly 1 name; NextSpecial__FP5ACTOR).
extern "C" void JtHurtShockCallback(JTHURT *hurt)
{
    SetSndSourceLo(CreateSfxSound(0x6A), 0);
    SetJtHurtState(hurt, 6);
#if defined(SLY2_EE_LAYOUT)
    func_001A15B0L(hurt, ((const LONGU *)&D_002A6790)->l);   /* the 8-byte record in one register (ldl / ldr), as the EE passes it */
#else
    SetJtHurtCallback(hurt, D_002A6790);
#endif
}

/* With hit points left: back to a safe place when the source respawns the hurt player (SO::hurtRespawnsHero), else air
   state 11; without: hurt state 8. */
// Same body as JtHurtFallCallback, for another hazard kind.
extern "C" void JtHurtFallCallback2(JTHURT *hurt)
{
    if (*hurt->hitPoints > 0) {
        if (hurt->zpr.ploSource && ((SO *)hurt->zpr.ploSource)->hurtRespawnsHero)
            ReturnJtToSafePlaceCallback(hurt);
        else
            SetJtAirJump(&hurt->player->jtair, 11, 0, 0);
    } else {
        SetJtHurtState(hurt, 8);
    }
}

/* The friction and elasticity of a contact by hurt state (D_002BFB20/D_002BFB24; no bounce in states 4 and 12, the
   contact's own values in 0, 6 and 7). */
namespace RO_ORDER {   /* pointers through asm-labelled declarations */
// Contact adjust hook in the hurt state: states 4/12 set the contact friction/elasticity to D_002BFB20 and 0,
// other states other values.
extern const HURTEND r_D_002A6790 __asm__("D_002A6790") = {0, 0xFFFF, (void (*)(void *))JtHurtFallCallback2};   /* delta 0, index -1 */
}
// Contact adjust hook in the hurt state: states 4/12 set the adjust values to D_002BFB20 and 0, other states other
// values. (inferred)
// Also known as: AdjustDamageXp (upstream readable / Sly 1 name; AdjustDamageXp__FP5ACTORP2XP).
extern "C" void AdjustJtHurtXp(JTHURT *hurt, XP *xp)
{
    switch (hurt->state) {
    case 4: case 12:
        xp->gFriction = D_002BFB20;
        xp->gElasticity = 0;
        break;
    case 0: case 6: case 7:
        break;
    default:
        xp->gFriction = D_002BFB20;
        xp->gElasticity = D_002BFB24;
        break;
    }
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001A2160 {
struct Q { unsigned int x0; char pad[0x30]; int x34; };
struct S { int x0; int x4; };
// Camera-definition hook: hurt states 5 and 8..12 clear +0x34; always sets bit 0 of the flags. (inferred)
extern "C" void AddJtHurtCpdefiFlags(struct S *hurt, struct Q *q)
{
    switch (hurt->x4) {
    case 5: case 8: case 9: case 10: case 11: case 12: q->x34 = 0; break; case 4: break;
    }
    q->x0 |= 1;
}
} // namespace M_func_001A2160

/* Invulnerable to zap kind zpk in some player states (3, 8, 21, 22: kind 0x4000; 7: whatever the grab allows), else
   as any step player. */
// JT vtable slot 76 (zap kind): hang/pipe/rope states ignore kind 0x4000, the grab state asks IsJtGrabBusy, else the
// STEPPLAYER base (ShouldStepplayerIgnoreZap).
// Also known as: FSlyImmune (upstream readable / Sly 1 name; FSlyImmune__FP5ACTORi).
extern "C" int IsJtInvulnerable(JT *jt, int zpk)
{
    switch (jt->state) {
    case 3:
    case 8:
    case 21:
    case 22:
        if (zpk == 0x4000)
            return 1;
        break;
    case 7:
        if (IsJtGrabBusy(&jt->jtgrab, zpk))
            return 1;
        break;
    }
    return IsStepplayerInvulnerable(jt, zpk);
}

/* Whether the hurt player ignores zap kind zpk: always while unknown298 is set; kinds 0x2000/0x4000 by the hurt, grab
   and air states; others while hurt, in post-hit mode 1, with D_002E1E00 bit 0 set or with the world's hands off. */
// Base invulnerability check: the object flag at x298, hands-off mode, or for zap kinds 0x2000/0x4000 depending on the
// hurt/grab/air sub-states.
// Also known as: FDamageImmune (upstream readable / Sly 1 name; FDamageImmune__FP5ACTORi).
extern "C" int ShouldStepplayerIgnoreZap(JTHURT *hurt, int zpk)
{
    if (hurt->player->isInvulnerable)
        return 1;
    if (zpk == 0x2000 || zpk == 0x4000) {
        switch (hurt->player->jthurt.state) {
        case -1:
            switch (hurt->player->jtgrab.state) {
            case -1:
                switch (hurt->player->jtair.state) {
                case -1: goto ret0;
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                case 17: {
                    register int k __asm__("$3") = 0x4000;
                    register int result __asm__("$2") = 1;
                    if (zpk == k)
                        return result;
                    result = 0;
                    return result;
                }
                }
                return 0;
            case 1:
            case 2:
            case 5:
            case 6:
            case 9: goto ret0;
            }
            goto ret1;
        case 3:
        case 5:
        case 10:
        case 11: goto ret1;
        }
        goto ret0;
    }
    if (hurt->state != -1)
        return 1;
    if (hurt->postHitMode == 1)
        return 1;
    if (D_002E1E00 & 1)
        return 1;
    if (!IsSwHandsOff(&g_sw))
        goto ret0;
ret1:
    return 1;
ret0:
    return 0;
}

/* STEPPLAYER slot 85 on the hurt player. */
// Returns the owner's damage veto (vtable slot 86) for the record.
extern "C" int ShouldJtVetoDamage(JTHURT *hurt, ZPR *damage)
{
    return hurt->player->IsZapLethal(damage);
}

// JT vtable slot 93 (hero died): closes Sly's eyes, then the STEPPLAYER base.
extern "C" void OnJtDeath(STEPPLAYER *player)
{
    SetSoEyesClosed(player, 1.0f);
    OnStepplayerHeroDied(player);
}

/* 0 when not hurt (1 in post-hit mode 1); hurt: 1 with hit points left, else 2. */
// 0 none, 1 hurt, 2 dead (health <= 0) while in a hurt state; otherwise whether the status is 1.
// Also known as: NDamageStatus (upstream readable / Sly 1 name; NDamageStatus__FP5ACTOR).
extern "C" int GetJtHurtStatus(JTHURT *hurt)
{
    if (hurt->state != -1) return *hurt->hitPoints <= 0 ? 2 : 1;
    return hurt->postHitMode == 1;
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001A23F8 {
extern float D_002BFB20 __asm__("D_002BFB20");
extern float D_002BFB24 __asm__("D_002BFB24");   /* hurt's contact friction and elasticity */
extern int D_002BFB28[14] __asm__("D_002BFB28");   /* parent state, -1: none */
// The adjust record for the current hurt state, falling back through the parent-state table D_002BFB28.
// Also known as: IsoeDamageMesh (upstream readable / Sly 1 name; IsoeDamageMesh__FP5ACTORPi).
extern "C" SOPART *GetJtHurtAdjustRecord(JTHURT *hurt, int *pout)
{
    int i; SOPART *r = 0; for (i = hurt->state; i != -1 && (r = hurt->aprecHurt[i]) == 0; i = D_002BFB28[i]) ; *pout = 0; return r;
}
} // namespace M_func_001A23F8

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001A2460 {
struct M4 {QWROW r[4];};
#if defined(SLY2_EE_LAYOUT)
/* The splash message: EXPLSO's grf and origin only. The EE passes this 0x30-byte record, so ExplodeExplso reads the
   rest of the EXPLSO (ploTouch, pxf, count) from whatever follows it in the frame. */
struct MSG {int type;char pad4[0x1C];QWROW position;};
#endif
extern qword GetLoWorldPos(LO*) __asm__("GetLoWorldPos");
extern void*func_002333B4(void*,int,int) __asm__("memset");
extern void*DeliverZapToSo(ZPR*) __asm__("DeliverZapToSo");
extern int ProbeStepplayerSurface(STEPPLAYER*,SO*,qword*) __asm__("ProbeStepplayerSurface");
extern void LoadIdentityMatrix4(void*,void*,M4*,M4*,float) __asm__("LoadIdentityMatrix4");
extern M4 __ml__FRC7MATRIX4T0(const M4*,const M4*) __asm__("__ml__FRC7MATRIX4T0");
// From UpdateStepplayer: if the player's height-adjusted point is inside the linked volume, inflicts the zap/drown
// damage and probes for the surface (ProbeStepplayerSurface).
// Also known as: UpdateActorWater (upstream readable / Sly 1 name; UpdateActorWater__FP5ACTOR).
extern "C" void UpdateStepplayerWater(STEPPLAYER *player)
{
    QWROW point;
    int valid;
    if (player->pwater) {
        point.q = GetLoWorldPos(player);
        point.f[2] += player->waterProbe[2];
        if (player->jthurt.state == 3 || player->jthurt.state == 10)
            point.f[2] -= 10.0f;
        WATER *link = player->pwater;
        valid = link->IsPosInside(point.q);
    } else
        valid = 0;
    if (!valid)
        return;
    if (player->pwater->unknown184) {
        int notify = 0;
        if (QwordZ(player->GetVelocity()) < 0.0f || (player->jtair.state == -1 && player->jthurt.state != 7))
            notify = 1;
        if (notify) {
            ZPR hit;
            func_002333B4(&hit, 0, sizeof(hit));
            hit.ploSource = (LO *)player->pwater;
            hit.unknown08 = player;
            hit.zpk = player->pwater->zapKind;
            hit.dhp = player->pwater->unknown184;
            hit.position = GetLoWorldPos(player);
            hit.velocity = player->GetVelocity();
            (void)DeliverZapToSo(&hit);
        }
    }
    if (!player->pwater->emitter)
        return;
    if (!ProbeStepplayerSurface(player, player->pwater, &point.q))
        return;
    int i = 0;
    if (!(player->clorefSplash > 0))
        return;
    QWROW position;
    M4 predicted, base;
    M4 *pbase = &base;
    M4 *ppred = &predicted;
    // Preserve the saved register for the predicted position.
    register QWROW *predpos __asm__("$20") = &predicted.r[3];
    for (; i < player->clorefSplash; i++) {
        LOREF *entry = &player->alorefSplash[i];
        if (g_clock.time - entry->tLast < 0.05f)
            continue;
        SO *object = (SO *)entry->plo;
        position.q = GetLoWorldPos(object);
        void *frame = object->GetLocalTransform((MATRIX4 *)pbase);
        LoadIdentityMatrix4(frame, 0, ppred, 0, g_clock.deltaTime);
        predicted = __ml__FRC7MATRIX4T0(ppred, pbase);
        int crossed = 0;
        if (point.f[2] <= position.f[2]) {
            if (predpos->f[2] <= point.f[2])
                crossed = 1;
        } else if (point.f[2] <= predpos->f[2])
            crossed = 1;
        if (crossed) {
#if defined(SLY2_EE_LAYOUT)
            MSG msg;
            msg.position = position;
            msg.position.f[2] = point.f[2];
            msg.type = 4;
            EXPL *effect = player->pwater->emitter;
            effect->ExplodeExplso((EXPLSO *)&msg);
#else
            EXPLSO msg;
            func_002333B4(&msg, 0, sizeof(msg));
            QWROW origin = position;
            origin.f[2] = point.f[2];
            msg.origin = origin.q;
            msg.grf = 4;
            player->pwater->emitter->ExplodeExplso(&msg);
#endif
            entry->tLast = g_clock.time;
        }
    }
}
} // namespace M_func_001A2460

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001A2788 {

extern qword D_002A2DD0 __asm__("D_002A2DD0");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
void *AllocFromStack(SWALLOC *pmsg) __asm__("AllocFromStack");
int CollectSoParts(SO *pl, void *pfn, int n, void *pv) __asm__("CollectSoParts");
int ClipEdgeToObjects(qword a, qword b, int n, void *pv, int c, LSG *phit) __asm__("ClipEdgeToObjects");
void CanWaterInflictZap(void) __asm__("CanWaterInflictZap");
int ProbeStepplayerSurface(STEPPLAYER *po, SO *pl, qword *pv) __asm__("ProbeStepplayerSurface");

// Line test from the player down to the linked volume's surface; writes the hit position.
// Also known as: FWaterSurfaceUnder (upstream readable / Sly 1 name; FWaterSurfaceUnder__FP5ACTORP3ALOP6VECTOR).
int ProbeStepplayerSurface(STEPPLAYER *object, SO *pl, qword *pv)
{
    QWROW off, b, a;
    LSG hit[16];
    off.q = D_002A2DD0;
    {
        qword p = GetLoWorldPos(object);
        qword d;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(p), "j"(off.q));
        a.q = d;
        b.q = d;
    }
    a.f[2] += object->waterProbe[1];
    b.f[2] += object->waterProbe[0];
    STACKSCOPE frame;
    SWALLOC msg;
    SWALLOC *message = &msg;
    message->cb = pl->numParts * sizeof(SOPART *);
    message->cbAlign = 0x10;
    void *pbuf = AllocFromStack(message);
    int n = CollectSoParts(pl, (void *)CanWaterInflictZap, 0, pbuf);
    if (!ClipEdgeToObjects(a.q, b.q, n, pbuf, 16, hit))
        return 0;
    qword r;
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(hit[0].apos[0]), "j"(off.q));
    *pv = r;
    return 1;
}
} // namespace M_func_001A2788

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001A28B0 {
int ProbeStepplayerSurface(STEPPLAYER *p,void *o,qword *pos) __asm__("ProbeStepplayerSurface");
void CalculateWaterCurrent(void *o,qword pos,qword *dir,qword *v) __asm__("CalculateWaterCurrent");
void AddSoWaterAcceleration(STEPPLAYER *p,void *o) __asm__("AddSoWaterAcceleration");
qword GetLoWorldPos(STEPPLAYER *p) __asm__("GetLoWorldPos");
float SmoothValueWithAccel__FffffP4SMPAPf(float pos,float vel,float target,float dt,SMPA *s,float *result) __asm__("SmoothValueWithAccel__FffffP4SMPAPf");
void AddSoAcceleration(STEPPLAYER *p,qword v) __asm__("AddSoAcceleration");
// Rotate *v by the 3x3 matrix m.
extern "C" { static inline qword Mul(const qword *m,const qword *v) { qword r; __asm__("vmulax.xyz ACC,%2,%1x\n\tvmadday.xyz ACC,%3,%1y\n\tvmaddz.xyz %0,%4,%1z" : "=j"(r) : "j"(*v),"j"(m[0]),"j"(m[1]),"j"(m[2]));return r; } }
// Broadcast int bits into a VU0 register.
extern "C" { static inline qword Broadcast(int g) { qword r; __asm__("qmtc2.ni %1,%0" : "=j"(r) : "r"(g));return r; } }
// STEPPLAYER vtable slot 72 (shared by Sly and Murray): in water/vertical states applies the water current and
// buoyancy; otherwise the SO base.
// Also known as: ApplyWaterToActor (upstream readable / Sly 1 name; ApplyWaterToActor__FP3ALOT0).
extern "C" void ResolveStepplayerWaterContact(STEPPLAYER *player,void *o)
{
 QWROW origin,vel,pos,local,dir,v50,accel,bcast;
 float z,g;
 qword v,k;
 struct CLOCK *clock;
 if(player->jtair.state==5) goto vertical;
 switch(player->jthurt.state) {
 case 7:
 vertical:
  origin.q=player->GetVelocity();
  if(origin.f[2]<0.0f) AddSoWaterAcceleration(player,o);
  break;
 case 3:case 10:
  if(!ProbeStepplayerSurface(player,o,&pos.q)) break;
  local.q=Mul(player->aqwTarget,&player->localVelocity);
  CalculateWaterCurrent(o,pos.q,&dir.q,&v50.q);
  v=player->GetVelocity();
  vel.q=v;
  v=VuAdd(v,local.q);
  v=VuSub(v,dir.q);
  k=Broadcast(*(int *)&player->waterProbe[3]);
  bcast.q=k;
  v=VuScaleByX(v,k);
  accel.q=v;
  origin.q=GetLoWorldPos(player);
  clock=&g_clock;
  SmoothValueWithAccel__FffffP4SMPAPf(origin.f[2]+player->waterProbe[2],vel.f[2]-dir.f[2],pos.f[2],clock->deltaTime,&player->waterHeightSmoothing,&z);
  g=(z-vel.f[2])/clock->deltaTime-player->physics->gravity.z;
  accel.f[2]=accel.f[2]>g?accel.f[2]:g;
  AddSoAcceleration(player,accel.q);
  break;
 default:
  AddSoWaterAcceleration(player,o);
  break;
 }
}
} // namespace M_func_001A28B0


// ---- Pointer constants and records, by address name (docs/guides/tu-merging.md "Data"): words as the ELF has
// them, pointers through asm-labelled declarations. Non-virtual pointer-to-member constants are 8 bytes
// {delta 0, index -1, pfn}; the definitions sit in their own namespace so no declaration above has to agree.
namespace ANPTR_hurt {
void p_func_001A2080() __asm__("JtHurtFallCallback2");


} // namespace ANPTR_hurt

// ---- hurt's .data in address order (tu-merging.md "Phase 6: one link mode"): one block after the functions (definition order is emission order); the declarations above are what the functions see.
namespace M_SetJtHurtState {
extern U D_002BFA00 __asm__("D_002BFA00")={{0.0f,0.0f,-750.0f,0.0f}};
extern U D_002BFA10 __asm__("D_002BFA10")={{0.0f,0.0f,500.0f,0.0f}};
extern POLY D_002BFA20 __asm__("D_002BFA20")={-1.0f,0.004f,0.0f};
} // namespace M_SetJtHurtState extern "C"
namespace M_func_001A1098 {
HURTZ D_002BFA30[7] __asm__("D_002BFA30") = {{0xD, 30.0f, 0}, {0xC, 30.0f, 0}, {0x13, 30.0f, 0}, {0x12, 30.0f, 0}, {0x16, 60.0f, 0}, {0xF7, 30.0f, 0},
    {0x13D, 30.0f, 0}};
} // namespace M_func_001A1098 extern "C"
namespace M_PresetJtAccelZap {
extern float D_002BFA90[3] __asm__("D_002BFA90") __attribute__((aligned(16))) = {0.0f, 10.0f, 0.01f};
} // namespace M_PresetJtAccelZap
namespace M_UpdateJtActiveZap {
float D_002BFAA0[3] __asm__("D_002BFAA0") = {1.0f, 0.0f, -0.75f};
float D_002BFAB0[3] __asm__("D_002BFAB0") = {0.7f, 0.3f, 0.0f};
float D_002BFAC0[3] __asm__("D_002BFAC0") = {3.5f, 0.0f, 0.5f};
} // namespace M_UpdateJtActiveZap extern "C"
namespace M_func_001A16C8 {
/* The F words are plain floats (4-aligned in the ELF; the one-float union would be 8-aligned), defined under
 * their asm labels so the declarations above stay as the functions see them. */
namespace HURT_DATA { float r_D_002BFACC __asm__("D_002BFACC") = 0.25f; }
namespace HURT_DATA { float r_D_002BFAD0 __asm__("D_002BFAD0") = 700.0f; }
namespace HURT_DATA { float r_D_002BFAD4 __asm__("D_002BFAD4") = 1200.0f; }
U D_002BFAE0 __asm__("D_002BFAE0")={{1000.0f,900.0f,950.0f,0.0f}};
U D_002BFAF0 __asm__("D_002BFAF0")={{1000.0f,-900.0f,950.0f,0.0f}};
namespace HURT_DATA { float r_D_002BFB00 __asm__("D_002BFB00") = 600.0f; }
namespace HURT_DATA { float r_D_002BFB04 __asm__("D_002BFB04") = 800.0f; }
} // namespace M_func_001A16C8 extern "C"
namespace M_FTakeJtDamage {
extern VQ D_002BFB10 __asm__("D_002BFB10")={{0.0f,0.0f,-1000.0f,0.0f}};
} // namespace M_FTakeJtDamage extern "C"
namespace M_func_001A23F8 {
float D_002BFB20 __asm__("D_002BFB20") = 1.0f;
float D_002BFB24 __asm__("D_002BFB24") = 0.3f;
int D_002BFB28[14] __asm__("D_002BFB28") = {-1, 0, 0, 0, -1, 0, 0, 0, 0, -1, 8, 8, 5, 4};
}
