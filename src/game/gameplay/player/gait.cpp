// gait.cpp -- the hero's movement helpers: GAIT (walk/run blend and footsteps, sly2/stepplayer.h), the curve
// markers of a blended clip, LEAP (a jump between two points: animation, ballistic or timed flight, landing), and the
// direction blend that picks the clip best aligned with a velocity. TU 0x20B840..0x20CE28 (31 functions, address
// order).
// Entry points: InitGait/PostJtLoadGait/UpdateGaitBlend/UpdateGaitFootsteps, StartLeap/UpdateLeap/SetLeapState,
// InitDirAnims/SelectDirAnim/StartDirAnim.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_gait
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_gait
#include "sly2/port.h"   /* SLY2_EE_LAYOUT */
#include "sly2/slotheap.h"
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/so.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
#include "sly2/sw.h"
#include "sly2/classes.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/act.h"
#include "sly2/clock.h"
#include "sly2/vtables.h"
#include "sly2/leap.h"

// 0x20CCD0 StaticInit_gait and GlobalConstructors_gait: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* Hand-made virtual calls: the vtable pointer of an object is at +0xC; entries are called with several shapes. */
union U { qword q; float f[4]; };

/* PostJtLoadGait: ported from Lilly's personal Sly 2 decomp (src/P2/jt.c), reduced to the declarations it needs. */
void FindAsegChannelTarget(ASEG *paseg, ALO *paloRoot, CHN *pchn, SO **ppso, XF **ppxf) __asm__("ResolveAsegChannelTarget");
CHN *PchnFindAseg(ASEG *paseg, ALO *palo, int xfk, int fStripped) __asm__("FindAsegChannel");
void EnsureAsegBlendDynamic(ALO *palo, int cbBl, int cbl, void *abl, int cmrsgc, int *mpimrsgcib, int *mpimrsgccmrsg, ASEGBL **ppasegbl);
void FillBlendSyncTables(int cbBl, int cbl, void *abl, int ibMrsg, int coid, OID *aoid);
void ReblendAsegbl(ASEGBL *pasegbl) __asm__("RecalculateAsegBlend");
struct VU_FLOAT
{
    qword qw;
};

/* A hero-like object as the leap reads it (an SO: XF at +0x58, flags word at +0x148, its control at +0x498; was ALO). */
struct LEAPSUBJECT : SO { char pad150[0x208 - 0x150];
    int n208; char pad20C[0x498 - 0x20C]; void *control; };
typedef char assert_ALO_size[sizeof(LEAPSUBJECT) == 0x4A0 ? 1 : -1];
#define POSITION(pxf) (*(qword *)(pxf)->worldMatrix.mat[3])

/* The leap (0x70 bytes, vtable 0x2DAA10 at +0xC); its tuning is the animation arc (sly2/stepplayer.h ASEGARC: the clip,
   its label times and the arc's acceleration; was the LEAPTUNE view). */
struct LEAPV
{
    int x0, x4, x8;
    void *pvt;              /* 0x0C */
    int state;              /* 0x10 */
    float stateTime;        /* 0x14 */
    char pad18[8];
    U previousVelocity;     /* 0x20 */
    ASEGARC *tuning;        /* 0x30 */
    LEAPSUBJECT *subject;           /* 0x34 */
    ASEGA *job;             /* 0x38 */
    int index;              /* 0x3C */
    U target;               /* 0x40 */
    float arrival, end;     /* 0x50 */
    LEAPSUBJECT *related;           /* 0x58 */
    int kind;               /* 0x5C */
    int ballistic, steep;   /* 0x60 */
    ACT *constraint;        /* 0x68 */
};
typedef char assert_LEAP_size[sizeof(LEAPV) == 0x70 ? 1 : -1];

/* The direction blend (four clips with their root velocities) and its 0x40-byte act. */
/* DIRE / DIRBL (the directional animation set, 0xD0): sly2/sw.h. */
struct DIRSEL { char pad[0x10]; qword direction; LEAPSUBJECT *root; ASEGA *active; DIRE *entries, *best; int flags, forward, backward; };
struct DIROWNER { char pad[0x30]; ASEG *paseg; };
struct DIRACT { int n0; int n4; int pad8; void *pvt; char pad10[0x10]; LEAPSUBJECT *palo; ASEGA *pasega; DIROWNER *powner; ASEG **ppaseg; int grfapl; char pad34[0xC]; };
typedef char assert_DIRACT_size[sizeof(DIRACT) == 0x40 ? 1 : -1];


struct SLOTHEAP;
extern "C" {
extern SW D_003DD490SW __asm__("g_sw");
extern qword D_002A2DD0;
/* gait's .data (address order): the gait's blend animation oids and blend parameters, and two tunables. */
OID D_002C3DC0[2] = {(OID)0x17D, (OID)0x17E};
OID D_002C3DC8[2] = {(OID)0x17F, (OID)0x180};
int D_002C3DD0[2] = {2, 2};
int D_002C3DD8[2] = {SLY2_OFFSETOF(BLGAIT, amrsgFoot0), SLY2_OFFSETOF(BLGAIT, amrsgFoot1)};   /* the gait entries' contact-window arrays (EnsureAsegBlendDynamic mpimrsgcib) */
float D_002C3DE0 = 5.0f;
float D_002C3DE4 = 1.1f;
extern const float D_002A8904;   /* defined below where the ELF has it (.rodata order) */

void RecalculateAsegBlend(ASEGBL *);
float ModPositive__Fff(float a, float b);
void *CreateSfxSound(int h);
void SetSndSourceLo(void *p, void *source);
qword GetLoWorldPos(void *p);
void SetSndSourcePos(void *p, qword q);
void SetSndRange(void *p, float a, float b);
void SetSndVolume(void *p, float t);
EA *FindAsegLabel(ASEG *paseg, int n);
CHN *FindAsegChannel(ASEG *paseg, LEAPSUBJECT *palo, int kind, int check);
void *memset(void *, int, unsigned int);
void SubscribeLoObject(ASEGA *, void *);
qword GetSoGravity(LEAPSUBJECT *);
void DetachStepguard(LEAPSUBJECT *);
void DisableStuckWatch(void *);
float ComputeMaxBallisticTime(qword,qword,float,float,float,float);
qword TransformXfPos(XF *,XF *,qword);
qword ConvertXfVec(XF *,XF *,qword);
ACT *NewBezierAct(LEAPSUBJECT *,XF *,int,qword,qword,float);
void RefreshSwObjectCollisionFilter(SW *,LEAPSUBJECT *);
void SetSoGravity(LEAPSUBJECT *,qword);
void SeekAsega(ASEGA *,int,float,float);
void AttachStepguard(LEAPSUBJECT *,LEAPSUBJECT *,int);
void EnableStuckWatch(void *);
int IsAsegaRetracted(ASEGA *);
int HandleBasicMessage(void *p, int msg, struct MSGL *pm);
void ReleaseAct(ACT **);
void SetXfRole(LEAPSUBJECT *o, XF *pxf, int m);

ASEGA *GetGaitBlendValue(GAIT *p);
qword GetGaitRootVelocity(GAIT *p);
void SetLeapState(LEAPV *p, int state);
LEAPV *ConstructLeap(LEAPV *p);
int IsLeapDone(LEAPV *p);
void ReleaseLeapConstraint(LEAPV *p);
int IsDirAnimStopped(DIRACT *p);
}
void ReplaceCurrentAseg(ASEG *paseg, ALO *paloRoot, float t, float svt, int grfapl, ASEGA **ppasega);  /* ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA */
int IsBasicDerivedFrom(BASIC *pbasic, CID cid);                                              /* IsBasicDerivedFrom__FP5BASIC3CID */
LO *FindSwObject(SW *psw, int grf, OID oid, LO *ploCtx);                                   /* FindSwObject__FP2SWi3OIDP2LO */

// Initializes a gait record (walk/run locomotion): owner, footstep sound id, walk volume 0.6, run volume 1.0.
// Also known as: InitGait (upstream readable / Sly 1 name; InitGait__FP4GAITii).
extern "C" void InitGait(GAIT *p, STEPPLAYER *pstepplayer, int sfxid) {
  p->player = pstepplayer;
  p->sfxidStep = sfxid;
  p->runVolume = 1.0f;
  p->walkVolume = 0.6f;
}

// 0x0020B868 PostJtLoadGait__FP4GAIT  (our name)
// After the level load (JT::PostLoad, for the gaits at JT+0x1320 and +0x1C30): the foot markers of both clips (FillBlendSyncTables), each clip's root movement per second
// ((position at the end - position at the start) / length), the blend (EnsureAsegBlendDynamic with the two marker arrays). Then every blended channel is told
// which marker array keeps it in step: XFs with oid 0x10 / 0x12 / 0x14 the second foot's (amrsgc[1]), 0x11 / 0x13 / 0x15 the first's (the default), any other none.
static inline qword DivVec(qword qw, VU_FLOAT g)
{
    qword qwRet;
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(qwRet) : "j"(g.qw), "j"(qw));
    return qwRet;
}
void PostJtLoadGait(GAIT *pgait) __asm__("PostJtLoadGait");
// Gait post-load: foot markers, root velocity per clip and the dynamic walk/run blend.
// Also known as: BindGait (upstream readable / Sly 1 name; BindGait__FP4GAIT).
void PostJtLoadGait(GAIT *gait)
{
    if (gait->abl[0].animation == 0)
        return;
    FillBlendSyncTables(0x40, 2, gait->abl, 0x20, 2, D_002C3DC0);
    FillBlendSyncTables(0x40, 2, gait->abl, 0x30, 2, D_002C3DC8);
    for (uint ibl = 0; ibl < 2; ibl++)
    {
        BLGAIT *pblgait = &gait->abl[ibl];
        ASEG *aseg = pblgait->animation;
        if (aseg != 0)
        {
            CHN *pchn = PchnFindAseg(aseg, gait->player, 1, 1);
            VECTOR4 vStart, vEnd;
            pchn->pac->Evaluate(0.0f, 1.0f, (qword *)&vStart, 0);
            pchn->pac->Evaluate(aseg->duration, 1.0f, (qword *)&vEnd, 0);
            qword qwDiff = VuSub(*(qword *)&vEnd, *(qword *)&vStart);
            VU_FLOAT vuT;
            __asm__("qmtc2 %1, %0" : "=j"( vuT.qw ) : "rV"(  *(int *)&aseg->duration )) ;
            pblgait->vRoot = DivVec(qwDiff, vuT);
        }
    }
    EnsureAsegBlendDynamic(gait->player, sizeof(gait->abl[0]), 2, gait->abl, 2, D_002C3DD8, D_002C3DD0, &gait->blendedAnimation);
    for (int ichn = 0; ichn < gait->blendedAnimation->cchn; ichn++)
    {
        CHN *pchn = &gait->blendedAnimation->achn[ichn];
        AC5 *pvkblend;
        ACG2 *pskblend;
        if (pchn->u.isScalar)
        {
            pvkblend = 0;
            pskblend = (ACG2 *)pchn->pac;
        }
        else
        {
            pvkblend = (AC5 *)pchn->pac;
            pskblend = 0;
        }
        SO *so;
        XF *xf;
        FindAsegChannelTarget(gait->blendedAnimation, gait->player, pchn, &so, &xf);
        switch (xf->oid)
        {
            case 0x10:
            case 0x12:
            case 0x14:
                if (pvkblend != 0)
                    pvkblend->pmrsgc = &gait->blendedAnimation->amrsgc[1];
                if (pskblend != 0)
                    pskblend->pmrsgc = &gait->blendedAnimation->amrsgc[1];
                break;
            case 0x11:
            case 0x13:
            case 0x15:
                break;
            default:
                if (pvkblend != 0)
                    pvkblend->pmrsgc = 0;
                if (pskblend != 0)
                    pskblend->pmrsgc = 0;
                break;
        }
    }
    ReblendAsegbl(gait->blendedAnimation);
}

// The weighted root velocity of the two gait clips.
// Also known as: VecGaitVelocity (upstream readable / Sly 1 name; VecGaitVelocity__FP4GAIT).
qword GetGaitRootVelocity(GAIT *gait)
{
    MRSGC *pb = gait->blendedAnimation->amrsgc;
    float denominator=pb->amrsg->dt;
    qword acc = D_002A2DD0;
    BLGAIT *pe = gait->abl;
    unsigned int i;
    for (i = 0; i < 2; i++) {
        U u;
        qword u_q;
        __asm__("qmtc2.ni %1, %0" : "=j"(u_q) : "r"(pe->weight * (pe->amrsgFoot0[0].dt / denominator)));
        u.q = u_q;
        __asm__("vadda.xyzw $ACC, %0, $vf0\n\tvmaddx.xyzw %0, %1, %2x" : "+j"(acc) : "j"(pe->vRoot), "j"(u.q));
        pe++;
    }
    return acc;
}

// Returns the value at +0x50 of the gait's blend (+4). (inferred)
ASEGA *GetGaitBlendValue(GAIT *gait) {
    return (ASEGA *)gait->blendedAnimation->dlAsega.head;
}

// Broadcast the int bits into vf2 and store them to *value.
static inline void InitScalar(U *value,int bits) {
    register int word __asm__("$2")=bits;
    register qword r __asm__("vf2");
    __asm__("qmtc2.ni %1,%0" : "=j"(r) : "r"(word));
    value->q=r;
}
// Return -v (scaled by the broadcast in *value), register-pinned.
static inline qword Negate(qword v,U *value) {
    register qword scalar __asm__("vf1");
    register qword result __asm__("vf3");
    __asm__("qmtc2.ni %1,%0" : "=j"(scalar) : "r"(-1.0f));
    value->q=scalar;
    __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(result) : "j"(v),"j"(value->q));
    return result;
}
// Sets the walk/run weights, reblends and writes the resulting root velocity.
// Also known as: SetGaitMix (upstream readable / Sly 1 name; SetGaitMix__FP4GAITf).
extern "C" void UpdateGaitBlend(GAIT *gait, float g)
{
    STEPPLAYER *pt;
    U b, a;
    qword q, r1, r2;
    if (gait->abl[1].animation == 0) {
        gait->abl[1].weight = 0.0f;
        gait->abl[0].weight = 1.0f;
    } else {
        gait->abl[1].weight = g;
        gait->abl[0].weight = 1.0f - g;
    }
    RecalculateAsegBlend(gait->blendedAnimation);
    pt = gait->player;
    InitScalar(&a,*(int *)&GetGaitBlendValue(gait)->speed);
    b = a;
    q = GetGaitRootVelocity(gait);
    r1=Negate(q,&b);
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r2) : "j"(a.q), "j"(r1));
    *(qword *)&pt->unknown190 = r2;
}

// Plays the footstep sound when a foot goes down, volume interpolated between walk and run, audible range 500/2000.
// Also known as: UpdateGaitFeet (upstream readable / Sly 1 name; UpdateGaitFeet__FP4GAIT).
extern "C" void UpdateGaitFootsteps(GAIT *gait)
{
    ASEGA *x = (ASEGA *)gait->blendedAnimation->dlAsega.head;
    float g = x->time;
    int i;
    for (i = 0; i < 2; i++) {
        ASEGBL *y = gait->blendedAnimation;
        MRSG *z = y->amrsgc[i].amrsg;
        int f = z->dt <= ModPositive__Fff(g - z->t, y->duration);
        if (f == gait->afFootDown[i])
            continue;
        if (!f) {
            float t = gait->abl[1].weight;
            void *h;
            {
                float u = 1.0f - t;
                t *= gait->runVolume;
                t += u * gait->walkVolume;
            }
            h = CreateSfxSound(gait->sfxidStep);
            SetSndSourceLo(h, 0);
            SetSndSourcePos(h, GetLoWorldPos(x->rootObject));
            SetSndRange(h, 500.0f, 2000.0f);
            SetSndVolume(h, t);
        }
        gait->afFootDown[i] = f;
    }
}

// Clears both foot-down flags.
// Also known as: ResetGaitFeet (upstream readable / Sly 1 name; ResetGaitFeet__FP4GAIT).
extern "C" void ResetGaitFeet(GAIT *gait) { gait->afFootDown[1] = 0; gait->afFootDown[0] = 0; }

// Initializes an animation arc record (first word = -1).
extern "C" void InitAsegArc(OID *p) {
    *p = (OID)-1;
}

// Initialize curve timing, planar distance and curvature.
// Arc timing from the animation labels 0x22/0x23/0x24: planar distance and curvature of the leap.
extern "C" void PostAsegArcLoad(ASEGARC *p, LEAPSUBJECT *arg)
{
    EA *pa, *pb, *pc;
    CHN *c;
    U v0, v1, v2;
    qword d;
    int n;
    register AC *thirdCurve __asm__("$7");
    register float start __asm__("$f12");
    register qword *derivative __asm__("$6");
    float h;
    if (p->fInit)
        return;
    if (!p->paseg) {
        p->paseg = (ASEG *)FindSwObject(&D_003DD490SW, 4, p->oid, (LO *)arg);
        if (!p->paseg)
            return;
    }
    pa = FindAsegLabel(p->paseg, 0x22);
    if (!pa)
        return;
    pb = FindAsegLabel(p->paseg, 0x23);
    if (!pb)
        return;
    pc = FindAsegLabel(p->paseg, 0x24);
    if (!pc)
        return;
    p->tLabel22 = pa->time;
    p->tLabel23 = pb->time;
    p->tLabel24 = pc->time;
    p->minimumTime = p->tLabel24 - p->tLabel23;
    p->flightTime = p->tLabel24 - p->tLabel22;
    c = FindAsegChannel(p->paseg, arg, 1, 1);
    if (c) {
        c->pac->Evaluate(p->tLabel22, 1.0f, &v0.q, 0);
        c->pac->Evaluate(p->tLabel24, 1.0f, &v1.q, 0);
        __asm__("vsub.xyzw %0,%2,%3\n\tlwc1 %1,%4" : "=j"(d), "=f"(start) : "j"(v0.q), "j"(v1.q), "m"(p->tLabel22));
        h = p->tLabel24;
        __asm__("vmul.xy %0, %0, %0\n\tvaddy.x %0, %0, %0y\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q\n\tdaddu "
                "%1,%z3,$zero\n\tsub.s %2,%2,%4"
                : "+j"(d), "=r"(derivative), "+f"(h)
                : "rJ"((qword *)0), "f"(start));
        h *= 0.5f;
        __asm__("qmfc2.ni %0,%3\n\tsw %0,%1\n\tlw %2,%4"
                : "=&r"(n), "=m"(p->sLeap), "=r"(thirdCurve)
                : "j"(d), "m"(c->pac));
        thirdCurve->Evaluate(start + h, 1.0f, &v2.q, derivative);
        {
            VECTOR *pv = &p->vecAcceleration;
            pv->x = 0;
            pv->y = 0;
            pv->z = (-v2.f[2] + v0.f[2]) / (h * h * 0.5f);
        }
    }
    p->fInit = 1;
}

// Constructs a leap object (CID 0x91) in place and enters state 0.
// Also known as: StartK2 (upstream readable / Sly 1 name; StartK2__FP5KIND2P3ALOiP6K2PLAY).
extern "C" void StartLeap(ASEGARC *ptune, LEAPSUBJECT *psubject, int index, LEAPV *p) {
    if (ptune->paseg == 0)
        return;
    memset(p, 0, sizeof(LEAP));
    memset(p, 0, sizeof(LEAP));
    ConstructLeap(p);
    p->tuning = ptune;
    p->subject = psubject;
    p->index = index;
    SetLeapState(p, 0);
}

// Allocates a leap object from the world's leap slot heap.
extern "C" LEAPV *AllocLeap(void) {
    return (LEAPV *)AllocZeroedFromSlotheap(&g_sw.slotheapLeap);
}

// Returns a leap object to the world's leap slot heap.
extern "C" void FreeLeap(LEAPV *leap) {
    FreeSlotheapSlot(&g_sw.slotheapLeap, leap);
}

// Leap (CID 0x91) constructor: sets its vtable.
LEAPV *ConstructLeap(LEAPV *leap)
{
    SLY2_SET_VPTR(leap, LEAP);
    leap->state = -1;
    /* BASIC's two counted pointers, through BASIC itself: natively the vptr is the first word, and LEAPV's x0 / x4
       (the EE offsets of these members) would overwrite it after SLY2_SET_VPTR stored it */
    ((BASIC *)leap)->scriptFrame.m_p = 0;
    ((BASIC *)leap)->scriptVariables.m_p = 0;
    return leap;
}

// If the leap is not finished: state 4 and clears its animation job.
// Also known as: StopK2 (upstream readable / Sly 1 name; StopK2__FP6K2PLAY).
extern "C" void StopLeap(LEAPV *leap) { if (!IsLeapDone(leap)) { SetLeapState(leap, 4); ReplaceCurrentAseg(0, 0, 0.0f, 1.0f, 0, &leap->job); } }

// Whether the leap is in state 4 (done).
int IsLeapDone(LEAPV *leap) {
    return leap->state == 4;
}

// Leap state machine for states 1..3.
// Also known as: UpdateK2 (upstream readable / Sly 1 name; UpdateK2__FP6K2PLAY).
extern "C" void UpdateLeap(LEAPV *leap)
{
    int k;
    do {
        k = leap->state;
        switch (k) {
        case 1:
            if (leap->subject->n208 && leap->stateTime + 0.15f < g_clock.time)
                SetLeapState(leap, 3);
            else if (leap->end <= g_clock.time)
                SetLeapState(leap, 2);
            break;
        case 2:
            if (leap->subject->n208 || g_clock.time - leap->arrival > D_002C3DE0)
                SetLeapState(leap, 3);
            else if (leap->arrival <= g_clock.time) {
                if (leap->ballistic)
                    SetLeapState(leap, 3);
                else if (IsBasicDerivedFrom((BASIC *)leap->subject, (CID)0x38))
                    EnableStuckWatch(leap->subject->control);
            }
            break;
        case 3:
            if (IsAsegaRetracted(leap->job))
                SetLeapState(leap, 4);
            break;
        }
    } while (k != leap->state);
}

struct MSGL { int a, b, n8; };
// Leap message handler (slot 5): animation label 0x22 launches the leap (state 1).
void LEAP::HandleMessage(MSGID msg_m, void *pm_m)   /* key function: LEAP's vtable comes out here */
{ LEAPV *p = (LEAPV *)this; int msg = (int)msg_m; MSGL *pm = (MSGL *)pm_m; if (msg == 9 && pm->n8 == 0x22) SetLeapState(p, 1); HandleBasicMessage(p, msg, pm); }

// Broadcast a float into a VU0 register.
static inline qword Scalar(float f){qword r;__asm__("qmtc2 %1,%0":"=j"(r):"r"(f));return r;}
// Return a*x + b*y.
static inline qword Combine(qword a,qword x,qword b,qword y){qword r;__asm__("vmulax.xyzw ACC,%1,%2x\n\tvmaddx.xyzw %0,%3,%4x":"=j"(r):"j"(a),"j"(x),"j"(b),"j"(y));return r;}
// Store a VU0 register to home.
static inline void Store(U *home,qword value){__asm__("sqc2 %1,%0":"=m"(home->q):"j"(value));}
// Leap states: 0 start animation; 1 launch (ballistic time or the given flight time) with a constraint; 2/3 seek the
// target; 4 land.
// Also known as: SetK2State (upstream readable / Sly 1 name; SetK2State__FP6K2PLAYi).
void SetLeapState(LEAPV *leap, int state)
{
    U scalar, delta, acceleration, velocity, finalVelocity;
    if (state == leap->state)
        return;
    leap->state = state;
    leap->stateTime = g_clock.time;
    switch (state) {
    case 0:
        ReplaceCurrentAseg(leap->tuning->paseg, (ALO *)leap->subject, 0.0f, 1.0f, leap->index, &leap->job);
        SubscribeLoObject(leap->job, leap);
        leap->previousVelocity.q = GetSoGravity(leap->subject);
        break;
    case 1: {
        if (IsBasicDerivedFrom((BASIC *)leap->subject, (CID)0x38)) {
            LEAPSUBJECT *subject = leap->subject;
            DetachStepguard(subject);
            DisableStuckWatch(subject->control);
        }
        float playbackRate = leap->job->speed;
        register LEAPSUBJECT *motionSubject __asm__("$3") = leap->subject;
        register XF *motionSpace __asm__("$4");
        register ASEGARC *motionTuning __asm__("$3");
        register float square __asm__("$f0");
        __asm__("lw %0,%3\n\tmul.s %1,%4,%4\n\tlw %2,%5"
                : "=&r"(motionSpace), "=f"(square), "=r"(motionTuning)
                : "m"(motionSubject->positionXf), "f"(playbackRate), "m"(leap->tuning));
        register qword rateSquared __asm__("vf3");
        register int ballistic __asm__("$2");
        __asm__("lqc2 $vf4,%4\n\tlqc2 $vf1,%5\n\tmfc1 %0,%8\n\tqmtc2.ni %0,%1\n\tvmulx.xyzw $vf2,$vf1,%1x\n\tsqc2 "
                "$vf2,%2\n\tlqc2 $vf2,%6\n\tvsub.xyzw $vf1,$vf2,$vf4\n\tsqc2 $vf1,%3\n\tlw %0,%7"
                : "=&r"(ballistic), "=&j"(rateSquared), "=m"(acceleration.q), "=m"(delta.q)
                : "m"(POSITION(motionSpace)), "m"(motionTuning->acceleration), "m"(leap->target.q),
                  "m"(leap->ballistic), "f"(square)
                : "vf1", "vf2", "vf4");
        scalar.q = rateSquared;
        float time;
        if (ballistic) {
            float low, high, slopeLow, slopeHigh;
            if (leap->related && (IsBasicDerivedFrom((BASIC *)leap->related, (CID)0x13) ||
                                  IsBasicDerivedFrom((BASIC *)leap->related, (CID)0xB))) {
                low = 1250.0f;
                slopeLow = 1.1f;
                high = 0.0f;
                slopeHigh = 50.0f;
            } else if (leap->steep) {
                low = 1500.0f;
                slopeLow = 1.33f;
                high = -1500.0f;
                slopeHigh = -1.33f;
            } else {
                low = 1250.0f;
                slopeLow = 1.1f;
                high = -1250.0f;
                slopeHigh = -1.1f;
            }
            time = ComputeMaxBallisticTime(delta.q, acceleration.q, low, high, slopeLow, slopeHigh);
        } else
            time = motionTuning->flightTime / playbackRate;
        time = StdMax(leap->tuning->minimumTime / playbackRate, time);
        LEAPSUBJECT *subject = leap->subject;
#if defined(SLY2_EE_LAYOUT)
        /* SO::SetVelocity (vtable slot 35), the entry read by hand before the velocity is built: a plain virtual
           call loads it after, which reorders the function. */
        VTE &entry = (*(VTE **)((char *)subject + 0xC))[35];
        short adjustment = entry.d;
        void (*applyVelocity)(void *, qword) = (void (*)(void *, qword))entry.pfn;
#endif
        scalar.q = Scalar(1.0f / time);
        qword inverseTime = scalar.q;
        scalar.q = Scalar(-0.5f * time);
        qword initialVelocity = Combine(delta.q, inverseTime, acceleration.q, scalar.q);
        Store(&velocity, initialVelocity);
#if defined(SLY2_EE_LAYOUT)
        applyVelocity((char *)subject + adjustment, velocity.q);
#else
        subject->SetVelocity(velocity.q);
#endif
        leap->arrival = g_clock.time + time;
        leap->end = leap->arrival - leap->tuning->minimumTime / playbackRate;
        if (leap->ballistic) {
            register qword timeScalar __asm__("vf1"), motionVelocity __asm__("vf2"), force __asm__("vf3");
            register LEAPSUBJECT *launchSubject __asm__("$3");
            register qword target __asm__("$6");
            __asm__("qmtc2.ni %4,%0\n\tlqc2 %1,%5\n\tlqc2 %2,%6\n\tlw %3,%7"
                    : "=j"(timeScalar), "=j"(motionVelocity), "=j"(force), "=r"(launchSubject)
                    : "r"(time), "m"(velocity.q), "m"(acceleration.q), "m"(leap->subject));
            __asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%2,%3x\n\tlq %1,%4"
                    : "+j"(motionVelocity), "=r"(target)
                    : "j"(force), "j"(timeScalar), "m"(leap->target.q));
            register XF *parentSpace __asm__("$2");
            __asm__("sqc2 %4,%0\n\tlw %1,%5\n\tsqc2 %3,%2"
                    : "=m"(finalVelocity.q), "=r"(parentSpace), "=m"(scalar.q)
                    : "j"(timeScalar), "j"(motionVelocity), "m"(launchSubject->positionXf));
            qword position = TransformXfPos(0, parentSpace->parent, target);
            qword speed = ConvertXfVec(0, leap->subject->positionXf->parent, finalVelocity.q);
            leap->constraint = NewBezierAct(leap->subject, leap->subject->positionXf, 5, position, speed, leap->arrival);
            leap->subject->fIgnoreFixed = 1;
            RefreshSwObjectCollisionFilter(&g_sw, leap->subject);
        }
        SetSoGravity(leap->subject, acceleration.q);
        break;
    }
    case 2: SeekAsega(leap->job, 0, leap->tuning->tLabel23, 1.0f); break;
    case 3:
        SeekAsega(leap->job, 0, leap->tuning->tLabel24, 1.0f);
        if (leap->related)
            AttachStepguard(leap->subject, leap->related, leap->kind);
        break;
    case 4:
        if (leap->ballistic)
            ReleaseLeapConstraint(leap);
        SetSoGravity(leap->subject, leap->previousVelocity.q);
        if (IsBasicDerivedFrom((BASIC *)leap->subject, (CID)0x38))
            EnableStuckWatch(leap->subject->control);
        break;
    }
}

struct ACTPOS { char pad[0x20]; qword v; };
// Updates the leap's target and constraint target.
// Also known as: SetK2Target (upstream readable / Sly 1 name; SetK2Target__FP6K2PLAYG9VU_VECTOR).
extern "C" void SetLeapTarget(LEAPV *leap, qword v)
{
    switch (leap->state) {
    case 0: leap->target.q = v; break;
    case 1:
    case 2:
        if (leap->ballistic) {
            ACTPOS *pd = (ACTPOS *)leap->constraint;
            pd->v = TransformXfPos(0, leap->subject->positionXf->parent, v);
        }
        break;
    }
}

// Drops the leap constraint and clears LO flag bit 26.
// Also known as: ReleaseK2Ballistic (upstream readable / Sly 1 name; ReleaseK2Ballistic__FP6K2PLAY).
void ReleaseLeapConstraint(LEAPV *leap)
{
    if (!leap->ballistic)
        return;
    leap->ballistic = 0;
    if (!leap->constraint)
        return;
    ReleaseAct(&leap->constraint);
    SetXfRole(leap->subject, leap->subject->positionXf, 2);
    leap->subject->fIgnoreFixed = 0;
    RefreshSwObjectCollisionFilter(&g_sw, leap->subject);
}

// Marks the directional animations uninitialized.
extern "C" void InvalidateDirAnims(DIRBL *dirAnims) {
    dirAnims->fInit = 0;
}

// Measures the root velocity and speed of the four directional animations.
// Also known as: ResolveK3 (upstream readable / Sly 1 name; ResolveK3__FP5KIND3P3ALO).
extern "C" void InitDirAnims(DIRBL *dirAnims, LEAPSUBJECT *arg)
{
    DIRE *e;
    unsigned int i;
    if (dirAnims->fInit)
        return;
    for (i = 0; i < 4; i++) {
        ASEG *o;
        CHN *c;
        U v0, v1;
        U u;
        qword d, vel, r, s;
        register qword tmp __asm__("vf4");
        register qword start __asm__("vf2");
        register qword end __asm__("vf3");
        register qword scalar __asm__("vf1");
        int f;
        e = &dirAnims->ae[i];
        o = (ASEG *)FindSwObject(&D_003DD490SW, 4, (OID)(int)e->animation, (LO *)arg);
        e->animation = o;
        if (!o)
            continue;
        c = FindAsegChannel(o, arg, 1, 1);
        c->pac->Evaluate(0.0f, 1.0f, &v0.q, 0);
        c->pac->Evaluate(o->duration, 1.0f, &v1.q, 0);
        {
            register int bits __asm__("$17") = *(int *)&o->duration;
            __asm__("qmtc2.ni %3,%0\n\tlqc2 %1,%4\n\tlqc2 %2,%5"
                    : "=j"(scalar), "=j"(start), "=j"(end)
                    : "r"(bits), "m"(v0.q), "m"(v1.q));
        }
        u.q = scalar;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "0"(end), "j"(start));
        __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(vel) : "0"(u.q), "j"(d));
        __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
                : "=&j"(r), "=&j"(tmp)
                : "j"(vel));
        e->velocity = vel;
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(s) : "0"(r));
        __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(s));
        e->speed.bits = f;
    }
    dirAnims->fInit = 1;
}

// Initializes a directional-animation object (CID 0x92) and applies its animation.
// Also known as: StartK3 (upstream readable / Sly 1 name; StartK3__FP5KIND3iiP6K3PLAY).
extern "C" void StartDirAnim(DIROWNER *dirAnims, LEAPSUBJECT *a, int b, DIRACT *object)
{
    memset(object, 0, sizeof(*object));
    memset(object, 0, sizeof(*object));
    object->powner = dirAnims;
    object->palo = a;
    SLY2_SET_VPTR(object, CID146);
    object->grfapl = b;
    object->n0 = 0;
    object->n4 = 0;
    ReplaceCurrentAseg(dirAnims->paseg, (ALO *)a, 0.0f, 1.0f, b, &object->pasega);
    SubscribeLoObject(object->pasega, object);
    object->ppaseg = &dirAnims->paseg;
}

// Allocates a directional-animation object from the world's slot heap.
extern "C" DIRACT *AllocDirAnim(void) {
    return (DIRACT *)AllocZeroedFromSlotheap(&g_sw.slotheapDiract);
}

// Returns a directional-animation object to the world's slot heap.
extern "C" void FreeDirAnim(DIRACT *dirAnims) {
    FreeSlotheapSlot(&g_sw.slotheapDiract, dirAnims);
}

// Clears the directional animation's job if active.
// Also known as: StopK3 (upstream readable / Sly 1 name; StopK3__FP6K3PLAY).
extern "C" void StopDirAnim(DIRACT *dirAnims) { if (!IsDirAnimStopped(dirAnims)) ReplaceCurrentAseg(0, 0, 0.0f, 1.0f, 0, &dirAnims->pasega); }

// Whether the directional animation's job is cleared.
int IsDirAnimStopped(DIRACT *dirAnims) {
    return dirAnims->pasega == 0;
}

// Directional-animation message handler (slot 5): labels 0x17D/0x17E set forward, 0x17F/0x180 backward.
// Also known as: OnK3Event (upstream readable / Sly 1 name; OnK3Event__FP6K3PLAYiP8ASEGMSG9).
void CID146::HandleMessage(MSGID msg_m, void *pm_m)
{
    DIRSEL *p = (DIRSEL *)this;
    int msg = (int)msg_m;
    MSGL *pm = (MSGL *)pm_m;
    if (msg == 9) {
        switch (pm->n8) {
        case 0x17D: p->forward = 1; break;
        case 0x17E: p->forward = 0; break;
        case 0x17F: p->backward = 1; break;
        case 0x180: p->backward = 0; break;
        }
    }
    HandleBasicMessage(p, msg, pm);
}

// Select the best animation direction and update its velocity.
static inline qword Dot(qword a, qword b)
{
    qword r, one;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "=&j"(r), "=&j"(one)
            : "j"(a), "j"(b));
    return r;
}
// Dot product of a and b (x lane), reusing a's register.
static inline qword DotReuse(qword a, qword b)
{
    qword one;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "=j"(b), "=&j"(one)
            : "j"(a), "0"(b));
    return b;
}
// Square root of the x lane of q.
static inline qword Sqrt(qword q){__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(q):"0"(q));return q;}
// Store the unit vector along v into dst.
static inline void Normalize(U *dst, qword v)
{
    qword length = Sqrt(Dot(v, v));
    register qword result __asm__("vf3");
    __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q" : "=&j"(result) : "j"(length), "j"(v));
    dst->q = result;
}
// Unit vector along v.
static inline qword NormalizeEntry(qword v)
{
    qword length = Sqrt(Dot(v, v));
    __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q" : "=j"(length) : "0"(length), "j"(v));
    return length;
}
// Broadcast the float word p->bits into vf1.
static inline qword ScalarFW(const FLOATWORD*p){register qword v __asm__("vf1");register int w __asm__("$2")=p->bits;__asm__("qmtc2.ni %1,%0":"=j"(v):"r"(w));return v;}
// Return v scaled by s.x (vf2).
static inline qword Scale(qword v,qword s){register qword r __asm__("vf2");__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(v),"j"(s));return r;}
// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A8904 __asm__("D_002A8904") = -3.40282347e+38f; }
// Chooses the best of the four directional animations for the desired direction.
// Also known as: SelectK3 (upstream readable / Sly 1 name; SelectK3__FP6K3PLAYG9VU_VECTOR).
extern "C" void SelectDirAnim(DIRSEL *dirAnims, qword v)
{
    U scalar, direction;
    register qword finalDirection __asm__("vf3");
    DIRE *best = 0;
    register DIRE *current __asm__("v0");
    Normalize(&direction, v);
    float maximum = D_002A8904;
    if (dirAnims->forward || dirAnims->backward) {
        DIRE *entry;
        unsigned int i;
        float bias;
        register qword scoringDirection __asm__("vf4");
        __asm__("lqc2 %0,%4\n\tlw %1,%5\n\tdaddu %2,%z6,$zero\n\tlwc1 %3,%7"
                : "=j"(scoringDirection), "=&r"(entry), "=&r"(i), "=f"(bias)
                : "m"(direction.q), "m"(dirAnims->entries), "rJ"(0), "m"(D_002C3DE4));
        do {
            ASEG *animation = entry->animation;
            if (animation) {
                float score = VuToFloat(DotReuse(scoringDirection, NormalizeEntry(entry->velocity)));
                if (animation == dirAnims->active->animation)
                    score *= bias;
                if (maximum < score) {
                    maximum = score;
                    best = entry;
                }
            }
        } while (++entry, ++i < 4);
        current = dirAnims->best;
        finalDirection = direction.q;
        if (best != current) {
            float time = 0.0f;
            EA *tag;
            ASEG *animation;
            int id;
            if (dirAnims->forward) {
                id = 0x17D;
                animation = best->animation;
            } else {
                id = 0x17F;
                animation = best->animation;
            }
            tag = FindAsegLabel(animation, id);
            if (tag)
                time = tag->time;
            ReplaceCurrentAseg(best->animation, (ALO *)dirAnims->root, time, 1.0f, dirAnims->flags, &dirAnims->active);
            SubscribeLoObject(dirAnims->active, dirAnims);
            dirAnims->best = best;
        }
    }
    current = dirAnims->best;
    finalDirection = direction.q;
    scalar.q = ScalarFW(&current->speed);
    dirAnims->direction = Scale(finalDirection, scalar.q);
}
