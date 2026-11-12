// pipe_jt.cpp -- JTPIPE, the player climbing a pipe (JT): init/load (blend speeds), the climbing frame and the push
// toward it, the climb range, the per-frame climb (stick to up/down speed, turning around at the pipe's slope, the
// blend weights) and the climb states (attach, idle, climb up/down, turn, release with their animations and sounds).
// VU0 and EE asm, register-pinned. TU 0x19DE10..0x19F0A8 (15 functions, address order).
// Entry points: InitJtPipe/PostJtLoadPipe (setup), SetJtPipeState (mount/leave), UpdateJtActivePipe and
// PresetJtAccelPipe (per frame), UpdateJtInternalXpsPipe (physics constraint), CanJtUseGadgetOnPipe.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_pipe_jt
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_pipe_jt
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/cm.h"
#include "sly2/aseg.h"
#include "sly2/act.h"
#include "player_input.h"
#include "sly2/stepplayer.h"
#include "sly2/jt.h"
#include "sly2/clock.h"
#include "sly2/classes.h"   /* PIPE */
// 0x19EF50 StaticInit_pipe_jt and GlobalConstructors_pipe_jt: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef struct MATRIX3 M3;
/* mat.h's VECTOR (16-byte aligned, the stack locals depend on it); sly2/basic.h's is 12 bytes. */
struct VECTORA { float x, y, z; } __attribute__((aligned(16)));
union U{qword q;float f[4];int i[4];};
struct COMPONENT;struct EFFECT;
/* The pipe the climb follows: the real PIPE (sly2/classes.h). */
typedef PIPE PATH;
struct SMPV{float maximum,minimum,duration;};
struct WORK{U ahead,behind;union{U input;SMPV smooth;}reuse;LM limit;};
struct WORKB{U ahead,behind;LM limit;U point,world,target;};
/* the clip channel FindAsegChannel returns: aseg.h CHN, its curve act.h AC */
struct PIPE;
extern "C"{
extern U D_002A2DD0,g_normalZ;SMPV D_002BF928={1500.0f,0.0f,0.5f};extern STEPPLAYER*D_002E1E40;
extern int g_grfjoyt;
CHN *FindAsegChannel(ASEG *o, STEPPLAYER *a, int b, int c);
void EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(STEPPLAYER *a, int b, int c, void *d, int e, int *f, int *g, void *h);
void GetPipeGoalMatrix(PATH *a, float g, int b, M3 *pm);
M3 *GetLoWorldMatrix(STEPPLAYER *o);
void EvaluateBezierMat(M3 *a, float g, float dt, float one, qword v, M3 *pm, qword z, M3 *pm2, qword *derivative);
M3 MulMatrix3Matrix3(M3 *a, M3 *b);
void DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(M3 *pm, float *pg, U *pv);
float GetShapeMinDistance(PATH *);
float GetShapeMaxDistance(PATH*);
qword PosSmooth(qword,qword,float,SMPV*,U*);
void SetJtAirJump(void*,int,void*,void*);int ConsumeButtonsPressedWithin__5POCTLUif(POCTL*,int,float);int GetButtonsPressedWithin__5POCTLUif(POCTL*,int,float);void ClearPoctlJoyPressed(POCTL*,int);
void StartJtMode(void*,int,void*,void*);int IsAsegaRetracted(ASEGA*);void SetJtPipeState(JTPIPE*,int);
float ClampShapeDistance(PATH*,float);void EvaluateShapeAtDistance(PATH*,float,U*,U*);qword GetPoctlLateralVector(POCTL*,qword);
void GetJtPipeDistanceRange(JTPIPE*,LM*);float SmoothValue__FfffP3SMPPf(float,float,float,SMPV*,float*);float ModPositive__Fff(float,float);void RecalculateAsegBlend(ASEG*);
float fabsf(float);
qword ConvertPosBetweenLos(void*,void*,qword);qword GetLoWorldPos(void*);
void AddJtPlaceOnCurve(void *pplaces, void *pso, float g);
void SubscribeLoObject(PATH*,STEPPLAYER *);void UnsubscribeLoObject(PATH*,STEPPLAYER *);
void LinkStepAttachment(STEPPLAYER *,PATH*,int);
float ClampToLimits__FP2LMf(LM*,float);
void SetStepplayerTnOverride(STEPPLAYER *,void*);
EFFECT*CreateSfxSound(int);void SetSndSourceLo(EFFECT*,void*);void SetSndSourcePos(EFFECT*,qword);
void SetJtCaneState__FP2JTi(STEPPLAYER *,int);void ReleaseSma(SMA**);
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(ASEG*,STEPPLAYER *,float,float,int,ASEGA**);
int IsCameraPolicyActive__FP5CPLCY(void *);
}

// Initializes the player's pipe-climbing context (STEPPLAYER+0xF50): owner, state -1 (not on a pipe) and the default
// climb tuning (45 / 40 ...).
// Also known as: InitClimb (upstream readable / Sly 1 name; InitClimb__FP5ACTORi).
extern "C" void InitJtPipe(JTPIPE *pipe, STEPPLAYER *actor)
{
    U *poffset = (U *)&pipe->handPosition;
    pipe->player = actor;
    pipe->state = -1;
    U *poffset2 = (U *)&pipe->unknown40;
    poffset->f[0] = 45.0f;
    poffset->f[2] = 40.0f;
    poffset->i[1] = 0;
    poffset2->f[0] = 45.0f;
    poffset2->f[2] = -20.0f;
    poffset2->i[1] = 0;
}

// Post-load: measures the per-cycle climb speed of the two climb clips (root displacement over a cycle) and builds
// the dynamic blend between them.
// Also known as: BindSlopeBlend (upstream readable / Sly 1 name; BindSlopeBlend__FP10SLOPEBLEND).
extern "C" void PostJtLoadPipe(JTPIPE *pipe)
{
    int i;
    JTPIPEBL *e;
    VECTORA v0, v1;
    for (i = 0, e = pipe->abl; i < 2; i++, e++) {
        ASEG *o = e->animation;
        if (o) {
            CHN *r = FindAsegChannel(o, pipe->player, 1, 1);
            if (r) {
                r->pac->Evaluate(0.0f, 1.0f, &v0, 0);
                r->pac->Evaluate(o->duration, 1.0f, &v1, 0);
                e->speed = (v0.z - v1.z) / o->duration;
            }
        }
    }
    EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(pipe->player, sizeof(pipe->abl[0]), 2, pipe->abl, 0, 0, 0, &pipe->pasegbl);
}

// Return v scaled by g (broadcast pinned to vf2).
static inline qword Scale(qword v,float g){U u;register qword s __asm__("vf2");qword r;s = VuFromFloat<qword>(g);u.q=s;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(v),"j"(u.q));return r;}
// Physics preset while on a pipe: in the mount and climb states (0, 4) accelerates Sly toward the pipe's climbing
// frame (GetPipeGoalMatrix) at the current distance.
// Also known as: UpdateClimbMotion (upstream readable / Sly 1 name; UpdateClimbMotion__FP5ACTOR).
extern "C" void PresetJtAccelPipe(JTPIPE *pipe)
{
    M3 m, m2, m4;
    U vec;
    float f;
    M3 *pm;
    GetPipeGoalMatrix(((PATH *)pipe->pipeCurve), pipe->s, pipe->isReversed == 0, &m);
    if (pipe->state == 0 || pipe->state == 4) {
        STEPPLAYER *o = pipe->player;
        float dt = g_clock.deltaTime;
        float g = o->activeAnimation->animation->duration - o->activeAnimation->time;
        M3 *pa;
        g = g > dt ? g : dt;
        pa = GetLoWorldMatrix(o);
        EvaluateBezierMat(pa, g, dt, 1.0f,
                          pipe->player->GetAngularVelocity(),
                          &m, D_002A2DD0.q, &m2, 0);
    } else {
        m2 = m;
    }
    pm = &m2;
    m4 = MulMatrix3Matrix3(GetLoWorldMatrix(pipe->player), pm);
    DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(&m4, &f, &vec);
    {
        /* SetAngularVelocity (slot 36) through the vtable by hand: the original loads the slot before computing the
           argument (a plain virtual call loads it after) */
        STEPPLAYER *o = pipe->player;
#if defined(SLY2_EE_LAYOUT)
        VTE *vt = *(VTE **)((char *)o + 0xC);
        void (*call)(void *, qword) = (void (*)(void *, qword))vt[36].pfn;
        void *thisp = (char *)o + vt[36].d;
        call(thisp, Scale(vec.q, f / g_clock.deltaTime));
#else
        o->SetAngularVelocity(Scale(vec.q, f / g_clock.deltaTime));
#endif
    }
}

// The distance range the player may climb on the current pipe: unbounded on a closed pipe, else from the bottom
// (+35, or +100 depending on the pipe) to 100 below the top.
// Also known as: LmClimb (upstream readable / Sly 1 name; LmClimb__FP5ACTORPf).
extern "C" void GetJtPipeDistanceRange(JTPIPE *pipe, LM *ag) {
    if (((PATH *)pipe->pipeCurve)->isClosed) {
        ag->minValue = -3.40282346638528859812e+38f;
        ag->maxValue = 3.40282346638528859812e+38f;
    } else if (pipe->isReversed) {
        ag->minValue = GetShapeMinDistance(((PATH *)pipe->pipeCurve)) + 35.0f;
        ag->maxValue = GetShapeMaxDistance(((PATH *)pipe->pipeCurve)) - 100.0f;
    } else {
        ag->minValue = GetShapeMinDistance(((PATH *)pipe->pipeCurve)) + 100.0f;
        ag->maxValue = GetShapeMaxDistance(((PATH *)pipe->pipeCurve)) - 35.0f;
    }
}

// The path's eligibility flag (bit 60).
static inline ulong Eligible(PATH*p){return p->isInWorld;}
// Ask the JT whether action kind is allowed (virtual slot 99).
static inline int CanAct(STEPPLAYER*jt,int kind){return jt->CanUseGadget(kind);}
// Direction from *a to *b and the axis pointer (register-pinned).
static inline qword Direction(U *a, U *b, U *&axis)
{
    register qword direction __asm__("vf3");
    register U *address __asm__("$2");
    __asm__("lqc2 $vf2,%2\n\tlui %0,%%hi(%4)\n\tlqc2 $vf1,%3\n\taddiu %0,%0,%%lo(%4)\n\tvsub.xyzw %1,$vf2,$vf1"
            : "=&r"(address), "=j"(direction)
            : "m"(a->q), "m"(b->q), "i"(&g_normalZ)
            : "vf1", "vf2");
    axis = address;
    return direction;
}
// Unit vector along v (register-pinned).
static inline qword Normal(qword v, U *axis)
{
    register qword d __asm__("vf1"), r __asm__("vf1");
    float length;
    register int bits __asm__("$3");
    __asm__("vmul.xyz %0,%1,%1\n\tvaddw.x $vf2,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,$vf2,%0z"
            : "=j"(d)
            : "j"(v)
            : "vf2");
    register qword fallback __asm__("$2") = axis->q;
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "+j"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(bits) : "j"(d));
    __asm__("mtc1 %1,%0" : "=f"(length) : "r"(bits));
    if (length < 0.0001f) {
        r = fallback;
    } else {
        __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q" : "=j"(r) : "j"(d), "j"(v));
    }
    qword q;
    __asm__("qmfc2.ni %0,%1" : "=r"(q) : "j"(r));
    return q;
}
// Per-frame pipe climbing: smooths the hand offset, X jumps off (SetJtAirJump kind 2), the mode button enters
// binocucom/recon mode, and stick input climbs up/down or spins around the pipe within GetJtPipeDistanceRange,
// with climb sounds.
// Also known as: UpdateClimbTransitions (upstream readable / Sly 1 name; UpdateClimbTransitions__FP5ACTOR).
extern "C" void UpdateJtActivePipe(JTPIPE *pipe)
{
    WORK w;
    float lowSpeed, highSpeed;
    STEPPLAYER *actor = pipe->player;
    actor->smoothedPosOffset = PosSmooth(actor->smoothedPosOffset, D_002A2DD0.q, g_clock.deltaTime, &D_002BF928, 0);
    register float amount __asm__("$f4"), dot __asm__("$f3"), zero __asm__("$f2");
    register int previous __asm__("$5");
    {
        register POCTL *input __asm__("$18") = pipe->player->controller;
        if (!Eligible(((PATH *)pipe->pipeCurve))) {
            SetJtAirJump(&pipe->player->jtair, 0, 0, 0);
            return;
        }
        if (pipe->state && ConsumeButtonsPressedWithin__5POCTLUif(input, BUTTON_JUMP, 0.25f)) {
            SetJtAirJump(&pipe->player->jtair, 2, 0, 0);
            return;
        }
        if (GetButtonsPressedWithin__5POCTLUif(input, BUTTON_LOOK, 0.15f) && CanAct(pipe->player, 3)) {
            ClearPoctlJoyPressed(input, BUTTON_LOOK);
            StartJtMode(&pipe->player->jtmode, 0, 0, 0);
        }
        int old;
        do {
            old = pipe->state;
            if ((old == 0 || old == 4) && IsAsegaRetracted(pipe->player->activeAnimation))
                SetJtPipeState(pipe, 1);
        } while (pipe->state != old);
        if (pipe->state == 0 || pipe->state == 4)
            return;
        EvaluateShapeAtDistance(((PATH *)pipe->pipeCurve), ClampShapeDistance(((PATH *)pipe->pipeCurve), pipe->s + 50.0f), &w.ahead, 0);
        EvaluateShapeAtDistance(((PATH *)pipe->pipeCurve), ClampShapeDistance(((PATH *)pipe->pipeCurve), pipe->s - 50.0f), &w.behind, 0);
        U *axis;
        qword direction = Direction(&w.ahead, &w.behind, axis);
        qword normalized = Normal(direction, axis);
        float z = QwordZ(normalized);
        if (fabsf(z) > 0.5f) {
            if (z > 0.0f) {
                if (!pipe->isReversed)
                    goto checkMemory;
                goto turn;
            }
            if (!pipe->isReversed)
                goto turn;
        }
    checkMemory:
        if (pipe->velocityAlong > 0.4f && pipe->isReversed && QwordZ(normalized) > -0.3f)
            goto turn;
        if (pipe->velocityAlong < -0.4f && !pipe->isReversed && QwordZ(normalized) < 0.3f)
            goto turn;
        goto proceed;
    turn:
        SetJtPipeState(pipe, 4);
        return;
    proceed:;
        register qword tangent __asm__("$2") = GetPoctlLateralVector(input, VuSub(w.ahead.q, w.behind.q));
        __asm__("lwc1 $f1,0x10(%6)\n\tqmtc2.ni %5,$vf4\n\tlwc1 $f0,0x14(%6)\n\tmtc1 $zero,%2\n\tmfc1 $3,$f1\n\tmfc1 "
                "$4,$f0\n\tpextlw $3,$4,$3\n\tmfc1 $4,%2\n\tpcpyld $3,$4,$3\n\tlwc1 %0,0x1c(%6)\n\tqmtc2.ni "
                "$3,$vf3\n\tlw %1,0x20(%7)\n\tvmul.xyz $vf1,$vf4,$vf3\n\tvaddw.x $vf2,$vf0,$vf0w\n\tvadday.x "
                "ACC,$vf1,$vf1y\n\tvmaddz.x $vf1,$vf2,$vf1z\n\tsq $3,%3\n\tqmfc2.ni $2,$vf1\n\tmtc1 $2,%4"
                : "=f"(amount), "=r"(previous), "=f"(zero), "=m"(w.reuse.input.q), "=f"(dot)
                : "r"(tangent), "r"(input), "r"(pipe)
                : "$2", "$3", "$4", "$f0", "$f1", "vf1", "vf2", "vf3", "vf4", "memory");
    }
    switch (previous) {
    case 0:
        if (amount < 0.25f)
            pipe->stickDirection = -1;
        else if (dot < -0.7f)
            pipe->stickDirection = 1;
        break;
    case 1:
        if (amount < 0.25f)
            pipe->stickDirection = -1;
        else if (dot > 0.7f)
            pipe->stickDirection = 0;
        break;
    case -1:
        if (amount > 0.35f) {
            if (zero <= dot)
                pipe->stickDirection = 0;
            else
                pipe->stickDirection = 1;
        }
        break;
    }
    int mode = -1;
    register float blend __asm__("$f3") = dot;
    register int selected __asm__("$3") = pipe->stickDirection;
    switch (selected) {
    case 0:
        mode = pipe->isReversed ? 3 : 2;
        blend = amount;
        break;
    case 1:
        mode = pipe->isReversed ? 2 : 3;
        blend = amount;
        break;
    case -1:
        mode = 1;
        blend = 0.0f;
        break;
    }

    float speed;
    register float maximumInput __asm__("$f1");
    if (blend > 0.01f) {
        register float initialPart __asm__("$f0") = 1.0f;
        maximumInput = pipe->abl[1].speed;
        initialPart -= blend;
        register float low __asm__("$f2") = pipe->abl[0].speed;
        blend *= maximumInput;
        initialPart *= low;
        speed = initialPart + blend;
        if (mode == 3)
            speed = -speed;
    } else {
        speed = 0.0f;
        mode = 1;
        maximumInput = pipe->abl[1].speed;
    }
    if (pipe->isReversed)
        speed = -speed;
    register CLOCK *clock __asm__("$19") = &g_clock;
    register float nextMemory __asm__("$f0");
    register PATH *path __asm__("$3");
    __asm__("abs.s $f5,%2\n\tlwc1 $f2,4(%7)\n\t.set noat\n\tlui $1,0x3f80\n\tmtc1 $1,%2\n\t.set at\n\tneg.s "
            "%0,$f2\n\tlwc1 $f4,0x1c(%8)\n\tsub.s %2,%2,$f2\n\t.set noat\n\tlui $1,0x3e4c\n\tori $1,$1,0xcccd\n\tmtc1 "
            "$1,$f3\n\t.set at\n\tswc1 $f5,%3\n\tmul.s %0,%0,%6\n\tswc1 $f3,%4\n\tmul.s %2,%2,$f4\n\tsw "
            "$zero,%5\n\tdiv.s %0,%0,$f5\n\tlw %1,0xc(%8)\n\tadd.s %0,%0,%2"
            : "=f"(nextMemory), "=r"(path), "+f"(maximumInput), "=m"(w.reuse.smooth.maximum),
              "=m"(w.reuse.smooth.duration), "=m"(w.reuse.smooth.minimum)
            : "f"(speed), "r"(clock), "r"(pipe)
            : "$1", "$f2", "$f3", "$f4", "$f5", "memory");
    pipe->velocityAlong = nextMemory;
    if (!path->isClosed) {
        GetJtPipeDistanceRange(pipe, &w.limit);
        (void)SmoothValue__FfffP3SMPPf(pipe->s, w.limit.minValue, clock->deltaTime, &w.reuse.smooth, &lowSpeed);
        (void)SmoothValue__FfffP3SMPPf(pipe->s, w.limit.maxValue, clock->deltaTime, &w.reuse.smooth, &highSpeed);
        float low = -highSpeed, high = -lowSpeed;
        if (speed < low)
            speed = low;
        else if (high < speed)
            speed = high;
    }
    register float stopped __asm__("$f21") = 0.0f;
    pipe->climbSpeed = -speed;
    if (speed != stopped) {
        pipe->s += pipe->climbSpeed * g_clock.deltaTime;
        InvalidateSoAaox(pipe->player);
    }
    if (((PATH *)pipe->pipeCurve)->isClosed)
        pipe->s = ModPositive__Fff(pipe->s, GetShapeMaxDistance(((PATH *)pipe->pipeCurve)));
    register float magnitude __asm__("$f2") = fabsf(speed);
    if (magnitude < 0.01f)
        mode = 1;
    else {
        register float initial __asm__("$f0") = pipe->abl[0].speed, one __asm__("$f4"), weight __asm__("$f3");
        __asm__("lwc1 $f1,%4\n\tabs.s %0,%0\n\t.set noat\n\tlui $1,0x3f80\n\tmtc1 $1,%1\n\t.set at\n\tabs.s "
                "$f1,$f1\n\tsub.s %2,%2,%0\n\tsub.s $f1,$f1,%0\n\tdiv.s %3,%2,$f1"
                : "+f"(initial), "=f"(one), "+f"(magnitude), "=f"(weight)
                : "m"(pipe->abl[1].speed)
                : "$1", "$f1");
        if (weight < stopped)
            weight = stopped;
        else if (one < weight)
            weight = one;
        register ASEG *animation __asm__("$4");
        register float complement __asm__("$f0");
        __asm__(".set noat\n\tlui $1,0x3f80\n\tmtc1 $1,%0\n\t.set at\n\tlw %1,0x70(%3)\n\tsub.s %0,%0,%4\n\tswc1 %4,%2"
                : "=f"(complement), "=r"(animation), "=m"(pipe->abl[1].weight)
                : "r"(pipe), "f"(weight)
                : "$1", "memory");
        pipe->abl[0].weight = complement;
        RecalculateAsegBlend(animation);
    }
    SetJtPipeState(pipe, mode);
}

// Adds the pipe contact constraint for the physics step: the curve point and tangent at the current distance,
// passed to the owner's constraint slot (vt82, kind 7).
// Also known as: AddClimbContacts (upstream readable / Sly 1 name; AddClimbContacts__FP5ACTOR).
extern "C" void UpdateJtInternalXpsPipe(JTPIPE *pipe) {
    U a, b, u;
    EvaluateShapeAtDistance(((PATH *)pipe->pipeCurve), pipe->s, &a, &b);
    u.q = ConvertPosBetweenLos(pipe->player, 0, pipe->handPosition);
    { STEPPLAYER *o = pipe->player; o->AddAxisXps(7, (LO *)((PATH *)pipe->pipeCurve), a.q, b.q, u.q); }
}

// Clears the two adjust values (+0x94, +0x98) of the given contact adjust record while on a pipe. (inferred)
// Also known as: AdjustClimbXp (upstream readable / Sly 1 name; AdjustClimbXp__FP2XPPv).
extern "C" void ResetJtPipeAdjust(void *p, int *s) { s[0x94/4] = 0; s[0x98/4] = 0; }

// Writes the pipe curve's point at the player's current distance.
extern "C" void GetJtPipePosition(JTPIPE *s, U *posOut) { EvaluateShapeAtDistance(((PATH *)s->pipeCurve), s->s, posOut, 0); }

struct Q { char pad[0x20]; qword v20; char pad2[4]; int n34; };
// Writes the player's velocity along the pipe: the curve tangent at the current distance scaled by the climb speed.
extern "C" void GetJtPipeVelocity(JTPIPE *pipe, Q *q)
{
    U v;
    U s;
    qword r;
    EvaluateShapeAtDistance(((PATH *)pipe->pipeCurve), pipe->s, 0, &v);
    pipe = (JTPIPE *)*(int *)&pipe->climbSpeed;
    qword sq;
    sq = VuFromFloat<qword>(pipe);   /* C++: asm output into a scalar, then stored (see the mismatch table) */
    s.q = sq;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v.q), "j"(s.q));
    q->v20 = r;
    q->n34 = 0;
}

// If the pipe allows it, records the current pipe and distance in the player's safe-place table (AddJtPlaceOnCurve)
// so a fall can return the player here.
// Also known as: UpdateSafePipe (upstream readable / Sly 1 name; UpdateSafePipe__FP5ACTOR).
extern "C" void RecordJtPipePlace(JTPIPE *pipe)
{
    PATH *ppath = ((PATH *)pipe->pipeCurve);
    if (ppath->noSafePlace == 0)
        AddJtPlaceOnCurve(&pipe->player->jtplaces, (GRABSPOT *)ppath, pipe->s);   /* its GRABSPOT base (null stays null) */
}

// Stores two vectors in the pipe context (meaning not identified). (inferred)
extern "C" void SetJtPipeVectors(JTPIPE *s, qword a, qword b) { s->handPosition = a; s->unknown40 = b; }

// Set the path's state (virtual slot 3).
static inline void PathState(PATH*p,int state){p->HandleSpliceEvent(state,0,0,0,0);}
// Move a 128-bit GPR value into a VU0 register.
static inline qword Vector(qword q){qword v;__asm__("qmtc2.ni %1,%0":"=j"(v):"r"(q));return v;}
// Direction from *a to *b and the axis pointer (second register variant).
static inline qword DirectionB(U *a, U *b, U *&axis)
{
    register qword first __asm__("vf2"), second __asm__("vf1"), direction __asm__("vf3");
    register U *address __asm__("$2");
    __asm__("lqc2 %0,%4\n\tlui %2,%%hi(%6)\n\tlqc2 %1,%5\n\taddiu %2,%2,%%lo(%6)\n\tvsub.xyzw %3,%0,%1"
            : "=j"(first), "=j"(second), "=&r"(address), "=j"(direction)
            : "m"(a->q), "m"(b->q), "i"(&g_normalZ));
    axis = address;
    return direction;
}
// Unit vector along v (second register variant).
static inline qword NormalB(qword v, U *axis)
{
    register qword d __asm__("vf1"), r __asm__("vf2");
    float length;
    register int bits __asm__("$3");
    d = VuDot(v, v);
    register qword fallback __asm__("$2") = axis->q;
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "+j"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(bits) : "j"(d));
    __asm__("mtc1 %1,%0" : "=f"(length) : "r"(bits));
    if (length < 0.0001f)
        return fallback;
    __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q" : "=j"(r) : "j"(d), "j"(v));
    qword q;
    __asm__("qmfc2.ni %0,%1" : "=r"(q) : "j"(r));
    return q;
}
// target - position and the focus pointer (register-pinned).
static inline qword GoalInputs(U *target, qword position, void *&focus)
{
    register qword a __asm__("vf2"), b __asm__("vf3"), r __asm__("vf1");
    register void *nullFocus __asm__("$5");
    __asm__("lqc2 %0,%3\n\tdaddu %2,$zero,$zero\n\tqmtc2.ni %4,%1"
            : "=j"(a), "=j"(b), "=&r"(nullFocus)
            : "m"(target->q), "r"(position));
    focus = nullFocus;
    r = VuSub(a, b);
    return r;
}
// World-space point for the pipe and its owning JT (register-pinned).
static inline qword WorldInputs(JTPIPE *pipe, U *point, U *world, qword offset, STEPPLAYER *&owner)
{
    register STEPPLAYER *actor __asm__("$2");
    register qword value __asm__("vf1");
    __asm__("lw %0,%3\n\tlqc2 %1,%4\n\tsqc2 %5,%2"
            : "=&r"(actor), "=&j"(value), "=m"(world->q)
            : "m"(pipe->player), "m"(point->q), "j"(offset));
    owner = actor;
    return value;
}
// Zero the xyz words of v.
static inline void Clear(U*v){v->i[0]=v->i[1]=v->i[2]=0;}
// Pipe sub-state switch: from -1 mounts the pipe (subscribe, register, find the curve distance within +-50 of the
// grab point); each state picks its clips and sounds; reaching uses the cane state (SetJtCaneState).
// Also known as: SetClimbState (upstream readable / Sly 1 name; SetClimbState__FP5ACTORi).
extern "C" void SetJtPipeState(JTPIPE *pipe, int next)
{
    int old = pipe->state;
    if (next == old)
        return;
    WORKB w;
    switch (old) {
    case -1: {
        PathState(((PATH *)pipe->pipeCurve), 5);
        SubscribeLoObject(((PATH *)pipe->pipeCurve), pipe->player);
        LinkStepAttachment(pipe->player, ((PATH *)pipe->pipeCurve), 1);
        pipe->stickDirection = old;
        EvaluateShapeAtDistance(((PATH *)pipe->pipeCurve), ClampShapeDistance(((PATH *)pipe->pipeCurve), pipe->s + 50.0f), &w.ahead, 0);
        EvaluateShapeAtDistance(((PATH *)pipe->pipeCurve), ClampShapeDistance(((PATH *)pipe->pipeCurve), pipe->s - 50.0f), &w.behind, 0);
        U *axis;
        qword direction = DirectionB(&w.ahead, &w.behind, axis);
        pipe->isReversed = QwordZ(NormalB(direction, axis)) < -0.5f;
        GetJtPipeDistanceRange(pipe, &w.limit);
        pipe->s = ClampToLimits__FP2LMf(&w.limit, pipe->s);
        EvaluateShapeAtDistance(((PATH *)pipe->pipeCurve), pipe->s, &w.point, 0);
        qword offset = Vector(ConvertPosBetweenLos(pipe->player, 0, pipe->handPosition));
        STEPPLAYER *owner;
        qword point = WorldInputs(pipe, &w.point, &w.world, offset, owner);
        owner->smoothedPosOffset = VuSub(point, offset);
        pipe->player->goalDelta = VuSub(pipe->player->goalPos, w.point.q);
        ((U *)&pipe->player->goalDelta)->i[2] = 0;
        SetStepplayerTnOverride(pipe->player, ((PATH *)pipe->pipeCurve)->ptn);
        if (((PATH *)pipe->pipeCurve)->attachEffectId) {
            EFFECT *e = CreateSfxSound(((PATH *)pipe->pipeCurve)->attachEffectId);
            SetSndSourceLo(e, 0);
            SetSndSourcePos(e, w.point.q);
        }
        break;
    }
    case 0:
        if (pipe->player == D_002E1E40)
            SetJtCaneState__FP2JTi(pipe->player, ((JT *)pipe->player)->unknown14D4 ? 10 : 2);
        break;
    }
    pipe->state = next;
    pipe->stateStartTime = g_clock.time;
    ASEG *clip = 0;
    float position = 0.0f, speed = 1.0f;
    int flags = 1;
    switch (next) {
    case -1: {
        PathState(((PATH *)pipe->pipeCurve), 6);
        UnsubscribeLoObject(((PATH *)pipe->pipeCurve), pipe->player);
        LinkStepAttachment(pipe->player, ((PATH *)pipe->pipeCurve), 0);
        STEPPLAYER *actor = pipe->player;
        w.target.q = actor->goalPos;
        void *focus;
        actor->goalDelta = GoalInputs(&w.target, GetLoWorldPos(actor), focus);
        ((U *)&pipe->player->goalDelta)->i[2] = 0;
        SetStepplayerTnOverride(pipe->player, focus);
        if (((PATH *)pipe->pipeCurve)->releaseEffectId) {
            EFFECT *e = CreateSfxSound(((PATH *)pipe->pipeCurve)->releaseEffectId);
            SetSndSourceLo(e, 0);
            SetSndSourcePos(e, GetLoWorldPos(pipe->player));
        }
        *(PATH **)&pipe->pipeCurve = 0;
        return;
    }
    case 0:
        clip = pipe->pasegMount;
        if (!clip) {
            clip = pipe->pasegTurn;
            register float half __asm__("$f0") = 0.5f;
            position = clip->duration * half;
            speed = half;
        }
        pipe->velocityAlong = 0;
        break;
    case 1:
        clip = pipe->pasegHold;
        flags = 9;
        break;
    case 2:
        clip = pipe->pasegbl;
        flags = 9;
        break;
    case 3:
        clip = pipe->pasegbl;
        speed = -1.0f;
        flags = 9;
        break;
    case 4:
        pipe->isReversed = !pipe->isReversed;
        clip = pipe->pasegTurn;
        break;
    }
    ReleaseSma(&pipe->player->activeStateMachine);
    ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(clip, pipe->player, position, speed, flags, &pipe->player->activeAnimation);
    Clear((U *)&pipe->player->localVelocity);
}

// Gadget check while on a pipe: only gadget kind 3 is considered, allowed when the owner's gadget flag (x12F0) and the
// global flag are set and no camera policy is active.
extern "C" int CanJtUseGadgetOnPipe(JTPIPE *pipe, int k) {
    if (k == 3) {
        int r = 0;
        if (pipe->player->canFreeLook && (g_grfjoyt & 2)) r = !IsCameraPolicyActive__FP5CPLCY(&g_cm.part3a0);
        return r;
    }
    return 0;
}
#undef FIELD_OFFSET
