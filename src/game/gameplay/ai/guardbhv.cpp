// guardbhv.cpp -- the guard behaviours: BHV subclasses that run on a STEPGUARD's behaviour stack (see bhvstk.cpp).
// The root BHVGUARD picks a mode (patrol, search, alert, combat, script, thrown, held, sleep, ...) and pushes the
// behaviour for it; those push smaller ones: BHVANIM plays the animation bound to a guard-flag set and its
// subclasses add a purpose (idle, strike, hit/hurt/knockdown/KO, leap, sleep, shoot); BHVPATH follows a path shape
// and BHVWALKPATH walks it with the walk animation; BHVGOTO plans a route through path zones and path links;
// BHVATTACK/BHVCOMBAT choose and run attacks; plus patrol, rail, pipe, juggled (held/carried), targets, circle,
// escort, stalk, relocate and patrol-path behaviours. Each behaviour has the BHV slots OnPush (7), Start (8), Close
// (9), Update (10), UpdateMotion (11), ground-probe flags (12), HandleMessage (13) and usually a SetState switch
// that pushes its child for the new state.
// TU 0x174898..0x17F090 (189 functions, address order), then the inline ClearPointerPair (linkonce copy 0x2562A8).
// The per-function files keep their views in namespaces named after the function (docs/guides/tu-merging.md
// "Representation decisions"); the former m2c bodies (mostly the Init/Start/Close/Done members of each behaviour)
// are written at file scope on step.h's BHV, guard.h's STEPGUARD and the TU-private derived behaviour views below.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_guardbhv
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_guardbhv
#include "sly2/basic.h"
#include "sly2/port.h"
#include "sly2/cref.h"
#include "sly2/scrap.h"
#include "sly2/lo.h"
#include "sly2/step.h"
#include "sly2/guard.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/sw.h"
#include "sly2/classes.h"
#include "sly2/jt.h"
#include "sly2/players.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/clock.h"
#include "sly2/game.h"
#include "sly2/joy.h"
#include "sly2/emitter.h"   /* EXPL, EXPLSO */
#include "sly2/act.h"   /* ACT8 */
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/lsg.h"
#include "sly2/leap.h"
// 0x17EF38 StaticInit_guardbhv and GlobalConstructors_guardbhv: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

// Starts a freshly pushed child behaviour: its virtual Start (slot 8). Takes any pointer to the child (a few
// namespaces still type it with a private view).
template <class T> static inline void StartPushedBhv(T *child) { ((BHV *)child)->Start(); }

/* Zap (damage) kinds of a zap message (docs/systems/physics-and-collision.md section 6); only the
   kinds tested by name here. */
enum { ZAP_WATER = 0x2000, ZAP_STUCK_WATCH = 0x4000, ZAP_SLEEP_DART = 0x80000, ZAP_JUGGLE = 0x200000 };
/* Zap reaction bits of a zap message's flags (HandleBhvguardMessage; inferred from how each bit is resolved):
   flinch (stagger meter), hurt, KO and knockdown. */
enum {
    REACT_FLINCH = 0x10,
    REACT_HURT = 0x20,
    REACT_KO = 0x40,
    REACT_KNOCKDOWN = 0x80,
    REACT_ANY = 0xF0
};
/* BHVGUARD awareness modes (the root behaviour's mode record kind, +0x30; docs/systems/ai-guards.md section 6). Past
   the documented ones, a mode is named after the behaviour it pushes (SetBhvguardMode) and what ends it
   (UpdateBhvguard); GUARD_MODE_WATCH is inferred. */
enum {
    GUARD_MODE_NONE = -1,
    GUARD_MODE_START = 0,
    GUARD_MODE_CALM = 1,
    GUARD_MODE_SUSPICIOUS = 2,
    GUARD_MODE_ALERT_ANIM = 3,
    GUARD_MODE_ALERT = 4,
    GUARD_MODE_SCRIPT = 5,
    GUARD_MODE_THROWN = 6,
    GUARD_MODE_JUGGLED = 7,
    GUARD_MODE_SLEEP = 8,
    GUARD_MODE_HURT = 9,
    GUARD_MODE_KNOCKDOWN = 10,
    GUARD_MODE_KO = 11,
    GUARD_MODE_TARGET_LIST = 12,
    GUARD_MODE_FLEE = 13,
    GUARD_MODE_BHV182 = 14,
    GUARD_MODE_NEELA_CHASE = 15,
    GUARD_MODE_ESCORT = 16,
    GUARD_MODE_WATCH = 17,
    GUARD_MODE_STALK = 18,
    GUARD_MODE_RELOCATE = 19,
};

/* Class ids used here (docs/systems/object-model.md section 1). */
enum {
    CID_SHAPE = 0xA,
    CID_PIPE = 0xB,
    CID_RAIL = 0xD,
    CID_CLASS19 = 0x13,
    CID_CLASS39 = 0x27,
    CID_PO = 0x32,
    CID_STEP = 0x33,
    CID_STEPPLAYER = 0x34,
    CID_JT = 0x35,
    CID_STEPGUARD = 0x38,
    CID_GUARD = 0x3A,
    CID_DIMITRI = 0x3B,
    CID_BOMB = 0x4F,
    CID_CLASS98 = 0x62,
    CID_CLASS101 = 0x65,
    CID_BHVANIM = 0x9D,
    CID_BHVIDLE = 0x9E,
    CID_BHVSTRIKE = 0x9F,
    CID_BHVHIT = 0xA0,
    CID_BHV161 = 0xA1,
    CID_BHV162 = 0xA2,
    CID_BHVKO = 0xA3,
    CID_BHVATTACK = 0xA4,
    CID_BHVCOMBAT = 0xA5,
    CID_BHVWALKPATH = 0xA7,
    CID_BHV169 = 0xA9,
    CID_BHVPATROL = 0xAA,
    CID_BHV171 = 0xAB,
    CID_BHVLEAP = 0xAC,
    CID_BHV173 = 0xAD,
    CID_BHV174 = 0xAE,
    CID_BHV175 = 0xAF,
    CID_BHV176 = 0xB0,
    CID_BHVJUGGLED = 0xB1,
    CID_BHV178 = 0xB2,
    CID_BHV179 = 0xB3,
    CID_BHV180 = 0xB4,
    CID_BHV182 = 0xB6,
    CID_BHV183 = 0xB7,
    CID_BHVGOTO = 0xBA,
    CID_BHV189 = 0xBD,
    CID_BHVPATHLINK = 0xBE,
    CID_BHV191 = 0xBF,
};
/* Guard flags (STEPGUARD +0x2C0, bit n = flag n; each behaviour raises its own: docs/systems/ai-guards.md
   section 2). Only the identified flags are named. */
enum {
    SGF_MOVING = 0x9,
    SGF_JUGGLED = 0x17,
    SGF_ATTACK = 0x18,
    SGF_ON_RAIL = 0x1B,   // raised by BHVRAIL and by BHVLEAP onto a rail
    SGF_COMBAT = 0x1D,
    SGF_PATROL = 0x1E,
    SGF_ON_PIPE = 0x27,   // BHVLEAP onto a pipe
    SGF_ON_CLASS19 = 0x28,   // BHVLEAP onto a cid 0x13 kind-3 object (inferred)
    SGF_ON_CLASS39 = 0x29,   // BHVLEAP onto a cid 0x27 object (inferred)
    SGF_PATHLINK = 0x2C,
    SGF_PATROLPATH = 0x2E,
};

/* The leap record a kind-2 animation binding drives (ANIMBINDING::payload): BHVSTRIKE arms its two flags at +0x60/+0x64
   (LEAP's fields are not named in sly2/leap.h). */
struct STRIKE { char pad0[0x60]; int unknown60; int unknown64; };
/* A message carrying an object (HandleBhvattackMessage, message 0xB). */
struct MSGLO { int n0; LO *plo; };

extern float D_002BE610;        /* attack retry delay */
int IsBasicDerivedFrom(BASIC *pbasic, CID cid);
extern "C" {
void InitBhv(BHV *behavior);
void StartBhv(BHV *behavior);
void CloseBhv(BHV *behavior);
void UpdateBhv(BHV *behavior);
int HandleBhvMessage(BHV *behavior, int msgid, void *pv);
void SetBhvBusy(BHV *behavior);
void ClearBhvBusy(BHV *behavior);
int IsBhvBusyAbove(BHV *behavior);
void AddStepguardSgf(STEPGUARD *guard, int sgf);
void RemoveStepguardSgf(STEPGUARD *guard, int sgf);
void SetStepguardTarget(STEPGUARD *guard, LO *object);
STEPGUARD *GetStepguardFromLo(LO *object);
void RefreshSwObjectCollisionFilter(SW *psw, STEP *pstep);
void InitBhvanim(BHVANIM *behavior);
void StartBhvanim(BHVANIM *behavior);
void CloseBhvanim(BHVANIM *behavior);
void SetBhvanimState(BHVANIM *behavior, int nState);
int IsBhvanimDone(BHVANIM *behavior);
int HandleBhvanimMessage(BHVANIM *behavior, int msg, ASEGEVENTMSG *pm);
ASEGA *GetAnimBindingInstance(ANIMBINDING *binding);
void SetBhvattackState(BHVATTACK *behavior, int nState);
void SetBhvcombatState(BHVCOMBAT *behavior, int nState);
CID120 *ChooseStepguardAttack(STEPGUARD *guard, LO *ploTarget, void *pvEngage);
void CloseBhvhit(BHV *behavior);
void SetBhvpatrolState(BHV *behavior, int nState);
void SetBhvThrownState(BHV176 *behavior, int nState);
void SetBhvjuggledState(BHVJUGGLED *behavior, int nState);
void SetBhvshootState(BHVSHOOT *behavior, int nState);
void SetBhvTargetsState(BHV178 *behavior, int nState);
void SetBhvCircleState(BHV179 *behavior, int nState);
void SetBhvEscortState(BHV183 *behavior, int nState);
void SetBhvrailState(BHVRAIL *behavior, int nState);
void SetBhvpipeState(BHVPIPE *behavior, int nState);
void SetBhvgotoState(BHVGOTO *behavior, int nState);
void PlanBhvgoto(BHVGOTO *behavior);
int IsBhvpathAtEnd(BHVPATH *behavior);
int HasBhvgotoArrived(BHVGOTO *behavior);
void SetBhvStalkState(BHV189 *behavior, int nState);
void SetBhvpathlinkState(BHVPATHLINK *behavior, int nState);
void AdvanceBhvpathlink(BHVPATHLINK *behavior);
void MoveBhvRelocateToPoint(BHV *behavior);
void AdvanceBhvpatrolpathGoal(BHV *behavior, int n);
extern qword D_002A2DD0;        /* the zero vector */
qword GetLoWorldPos(LO *plo);
void PopBhv(BHV **ppbhv);
void UpdateBhvMotion(BHV *behavior);
void SubscribeLoObject(LO *plo, LO *ploSubscriber);
void UnsubscribeLoObject(LO *plo, LO *ploSubscriber);
}

// 0x174898 GetStepguardFromLo
// 0x174898: Returns the guard that owns an object: the object's hierarchy top if it is a STEPGUARD, else (for bombs,
// cid 0x4F) the hierarchy top of the bomb's thrower; NULL otherwise.
extern "C" STEPGUARD *GetStepguardFromLo(LO *p) {
    if (!p) return 0;
    if (IsBasicDerivedFrom(p->root->topObject, (CID)CID_STEPGUARD)) return (STEPGUARD *)p->root->topObject;
    if (IsBasicDerivedFrom(p, (CID)CID_BOMB) && IsBasicDerivedFrom(((BOMB *)p)->thrower->root->topObject, (CID)CID_STEPGUARD))
        return (STEPGUARD *)((BOMB *)p)->thrower->root->topObject;
    return 0;
}

// 0x174920 InitBhvanim
namespace M_InitBhvanim {

void InitBhv(BHVANIM *p) __asm__("InitBhv");
// 0x174920: BHVANIM vtable slot 7 (OnPush): base, state -1, no guard flag (shared by BHV160).
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvanim(BHVANIM *behavior)
{
    InitBhv(behavior);
    behavior->sgf = 0;
    behavior->state = -1;
}
} // namespace M_InitBhvanim

// 0x174950 StartBhvanim
namespace M_StartBhvanim {

void StartBhv(BHVANIM *p) __asm__("StartBhv");
void AddStepguardSgf(void *p, int n) __asm__("AddStepguardSgf");
ANIMBINDING *FindCurrentAnimBinding(void *p) __asm__("FindCurrentAnimBinding");
// 0x174950: BHVANIM vtable slot 8 (Start): raises the behaviour's guard flag, picks the animation binding for the
// guard's current flags and enters state 0 (playing), or state 1 (done) when there is none.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvanim(BHVANIM *behavior) {
    StartBhv(behavior);
    AddStepguardSgf(behavior->owner, behavior->sgf);
    if (!behavior->binding) {
        behavior->binding = FindCurrentAnimBinding(behavior->owner);
        if (!behavior->binding) { SetBhvanimState(behavior, 1); return; }
    }
    SetBhvanimState(behavior, 0);
}
} // namespace M_StartBhvanim

// 0x1749C0 SetBhvanimState
namespace M_SetBhvanimState {
void SetStepguardMoverDriver(void *p, BHVANIM *ps) __asm__("SetStepguardMoverDriver");
void ResetStepguardMover(void *p, BHVANIM *ps) __asm__("ResetStepguardMover");
int StepguardHasSgf(STEPGUARD *o, int n) __asm__("StepguardHasSgf");
void StartAnimBinding(ANIMBINDING *e, STEPGUARD *o, int n, float g) __asm__("StartAnimBinding");
void StopAnimBinding(ANIMBINDING *e) __asm__("StopAnimBinding");
void ClearBhvBusy(BHVANIM *p) __asm__("ClearBhvBusy");
// 0x1749C0: BHVANIM state switch: 0 becomes the mover's driver and starts the binding (continuing the walk cycle
// phase for flag 9); 1 clears busy, saves the walk phase, stops the binding and releases the mover.
// Also known as: SetMoveState (upstream readable / Sly 1 name; SetMoveState__7JOBMOVEi).
extern "C" void SetBhvanimState(BHVANIM *behavior, int n) {
    if (behavior->state == n) return;
    behavior->state = n;
    behavior->stateTime = g_clock.time;
    switch (n) {
    case 0: {
        float f;
        SetStepguardMoverDriver((&((STEPGUARD *)behavior->owner)->mover), behavior);
        if (behavior->binding->kind == 0 && StepguardHasSgf(((STEPGUARD *)behavior->owner), SGF_MOVING) && g_clock.time == ((STEPGUARD *)behavior->owner)->walkPhaseTime)
            f = behavior->binding->aseg->duration * ((STEPGUARD *)behavior->owner)->walkPhase;
        else
            f = 0.0f;
        StartAnimBinding(behavior->binding, ((STEPGUARD *)behavior->owner), behavior->binding->flags | 1, f);
        break;
    }
    case 1:
        ClearBhvBusy(behavior);
        if (behavior->binding) {
            if (behavior->binding->kind == 0 && StepguardHasSgf(((STEPGUARD *)behavior->owner), SGF_MOVING)) {
                ((STEPGUARD *)behavior->owner)->walkPhase = behavior->binding->asega->time / behavior->binding->aseg->duration;
                ((STEPGUARD *)behavior->owner)->walkPhaseTime = g_clock.time;
            }
            StopAnimBinding(behavior->binding);
            behavior->binding = 0;
            ResetStepguardMover((&((STEPGUARD *)behavior->owner)->mover), behavior);
        }
        ((STEPGUARD *)behavior->owner)->steeringMode = 0;
        break;
    }
}
} // namespace M_SetBhvanimState

// 0x174B28 CloseBhvanim
// 0x174B28: BHVANIM vtable slot 9 (Close): base, state 1 (stop the animation), lowers its guard flag.
// Also known as: Exit (upstream readable / Sly 1 name).
void BHVANIM::Close()
{
    BHVANIM *behavior = this;
    CloseBhv(behavior);
    SetBhvanimState(behavior, 1);
    RemoveStepguardSgf((STEPGUARD *)behavior->owner, behavior->sgf);
}

// 0x174B68 UpdateBhvanim
namespace M_UpdateBhvanim {

void UpdateBhv(BHVANIM *p) __asm__("UpdateBhv");
int IsAnimBindingDone(void *p) __asm__("IsAnimBindingDone");
// 0x174B68: BHVANIM vtable slot 10 (Update): finishes (state 1) when the animation is done.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvanim(BHVANIM *behavior) {
    int s;
    UpdateBhv(behavior);
    do {
        s = behavior->state;
        if (s == 0 && IsAnimBindingDone(behavior->binding)) SetBhvanimState(behavior, 1);
    } while (s != behavior->state);
}
} // namespace M_UpdateBhvanim

// 0x174BD0 HandleBhvanimMessage
namespace M_BHVANIM_FHandleMessage {
typedef union { qword q; float f; } U;
extern LO *D_002F7900 __asm__("D_002F7900");          /* one of the three heroes (screen.cpp) */
qword GetStepAsegaTranslation(STEP *pstep, ASEGA *asega, float f) __asm__("GetStepAsegaTranslation");
void LockStepguardMover(GUARDMOVER *pmover, BHVANIM *s) __asm__("LockStepguardMover");
void UnlockStepguardMover(GUARDMOVER *pmover, BHVANIM *s) __asm__("UnlockStepguardMover");
BHV *FindStepBehavior(STEP *pstep, int cid) __asm__("FindStepBehavior");
LO *GetHeroPo(SW *psw) __asm__("GetHeroPo");
void TriggerJoyRumbleRumk__FP3JOY4RUMKf(void *p, int n, float f) __asm__("TriggerJoyRumbleRumk__FP3JOY4RUMKf");
int IsLoSelfOrDescendantOf(LO *plo, LO *ploAncestor) __asm__("IsLoSelfOrDescendantOf");
void JoltCamera(void *p, int n) __asm__("JoltCamera");
void SetSoZapMode(SO *pso, int n) __asm__("SetSoZapMode");
} // namespace M_BHVANIM_FHandleMessage
// 0x174BD0: BHVANIM vtable slot 13 (HandleMessage): message 9 carries an ASEG event of the playing animation: 0x15E
// root-motion translation, 0x25/0x26 lock/unlock the mover, 0x27/0x28 set/clear busy, 0x29/0x2A ground flag, rumble
// and camera events, and so on; other messages go to the base.
// Also known as: Message (upstream readable / Sly 1 name; Message__7JOBMOVEiPv).
extern "C" int HandleBhvanimMessage(BHVANIM *behavior, int msg, ASEGEVENTMSG *pm) {
    using namespace M_BHVANIM_FHandleMessage;
    ASEGA *animation;
    if (msg != 9) return 0;
    if (!behavior->binding) return 0;
    animation = GetAnimBindingInstance(behavior->binding);
    if (animation != pm->asega) return 0;
    switch (pm->event) {
    case 0x15E:
        {
            qword v = GetStepAsegaTranslation(((STEPGUARD *)behavior->owner), animation, pm->time);
            qword n, r;
            U u;
            STEPGUARD *a;
            { qword u_q; __asm__("qmtc2.ni %1, %0" : "=j"(u_q) : "r"(-1.0f)); u.q = u_q; }
            __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(n) : "j"(v), "j"(u.q));
            ((STEPGUARD *)behavior->owner)->qwUnknown190 = n;
            a = ((STEPGUARD *)behavior->owner);
            {
                qword *m = a->aqwTarget;
                __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
                        : "=j"(r) : "j"(m[0]), "j"(m[1]), "j"(m[2]), "j"(v));
            }
            a->SetVelocity(r);
        }
        return 1;
    case 0x25:
        LockStepguardMover(&((STEPGUARD *)behavior->owner)->mover, behavior);
        return 1;
    case 0x26:
        UnlockStepguardMover(&((STEPGUARD *)behavior->owner)->mover, behavior);
        return 1;
    case 0x29:
        ((STEPGUARD *)behavior->owner)->steeringMode = 1;
        return 1;
    case 0x2A:
        ((STEPGUARD *)behavior->owner)->steeringMode = 0;
        return 1;
    case 0x27:
        SetBhvBusy(behavior);
        return 1;
    case 0x28:
        ClearBhvBusy(behavior);
        return 1;
    case 0x2F:
        {
            BHVCOMBAT *o = (BHVCOMBAT *)FindStepBehavior(behavior->owner, CID_BHVCOMBAT);
            if (o && o->target == GetHeroPo(&g_sw)) TriggerJoyRumbleRumk__FP3JOY4RUMKf(&g_joy, 8, 1.0f);
        }
        return 0;
    case 0x34:
    case 0x35:
        {
            LO *g = GetHeroPo(&g_sw);
            BHVHIT *o = (BHVHIT *)FindStepBehavior(behavior->owner, CID_BHVHIT);
            if (o && IsLoSelfOrDescendantOf(o->attacker, g) && g == D_002F7900) {
                switch (pm->event) {
                case 0x35: JoltCamera(&g_cm, 0); break;
                case 0x34: TriggerJoyRumbleRumk__FP3JOY4RUMKf(&g_joy, 3, 1.50000005960464477539e-01f); break;
                }
            }
        }
        return 0;
    case 0x36:
        SetSoZapMode(behavior->owner, 2);
        return 0;
    }
    return 0;
}

// 0x174EA0 GetBhvanimGroundProbeFlags
namespace M_BHVANIM_GrfGroundProbe {
struct U; 
struct U { char pad[0x10]; int mode; };
int GetBhvGroundProbeFlags(BHVANIM *p) __asm__("GetBhvGroundProbeFlags");
// 0x174EA0: BHVANIM vtable slot 12 (ground-probe flags): when the binding drives a kind-2 payload, from its mode (7
// for modes 0/3/4, else 1); else the base value.
// Also known as: State (upstream readable / Sly 1 name).
extern "C" int GetBhvanimGroundProbeFlags(BHVANIM *behavior) {
    if (behavior->binding->kind == 2) {
        switch (((struct U *)behavior->binding->asega)->mode) { case 0: case 3: case 4: return 7; default: return 1; }
    }
    return GetBhvGroundProbeFlags(behavior);
}
} // namespace M_BHVANIM_GrfGroundProbe

// 0x174F08 UpdateBhvanimMotion
namespace M_UpdateBhvanimMotion {
void DriveStepguardFromAnim(ANIMBINDING *binding, STEPGUARD *guard) __asm__("DriveStepguardFromAnim");
float atan2f(float y, float x) __asm__("atan2f");
void SetStepguardMoverRadTarget(GUARDMOVER *pmover, float f) __asm__("SetStepguardMoverRadTarget");
} // namespace M_UpdateBhvanimMotion
// 0x174F08: BHVANIM vtable slot 11 (UpdateMotion): while playing, drives the guard's velocity from the animation's
// root motion (else zero) and faces the look-at object.
// Also known as: Steer (upstream readable / Sly 1 name).
extern "C" void UpdateBhvanimMotion(BHVANIM *behavior)
{
    using namespace M_UpdateBhvanimMotion;
    UpdateBhvMotion(behavior);
    if (behavior->state == 0)
        DriveStepguardFromAnim(behavior->binding, ((STEPGUARD *)behavior->owner));
    else
        ((STEPGUARD *)behavior->owner)->qwUnknown190 = D_002A2DD0;
    if (behavior->lookAt) {
        qword a = GetLoWorldPos(behavior->lookAt);
        qword b = GetLoWorldPos(((STEPGUARD *)behavior->owner));
        qword d;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
        SetStepguardMoverRadTarget(&((STEPGUARD *)behavior->owner)->mover, atan2f(QwordY(d), QwordX(d)));
    }
}

// 0x174FB8 IsBhvanimDone
// 0x174FB8: True if the animation behaviour has finished (state 1).
extern "C" int IsBhvanimDone(BHVANIM *behavior)
{
    return behavior->state == 1;
}

// 0x174FC8 InitBhvidle
// 0x174FC8: BHVIDLE vtable slot 7 (OnPush): BHVANIM OnPush with the stand/idle flag (2).
// Also known as: Init (upstream readable / Sly 1 name).
void BHVIDLE::OnPush()
{
    InitBhvanim(this);
    sgf = 2;
}

// 0x174FF8 StartBhvidle
namespace M_StartBhvidle {

extern qword D_002A2DD0 __asm__("D_002A2DD0");
void StartBhvanim(BHVIDLE *p) __asm__("StartBhvanim");
int StepguardHasSgf(STEPGUARD *t, int n) __asm__("StepguardHasSgf");
void SetStepguardAttack(STEPGUARD *t, int a, int b, qword q) __asm__("SetStepguardAttack");
void ResetDetectorTargets(void *p) __asm__("ResetDetectorTargets");
void SetXfRole(STEPGUARD *t, XF *a, int b) __asm__("SetXfRole");
// 0x174FF8: BHVIDLE vtable slot 8 (Start): BHVANIM Start; with the hands-off flag (1) also cancels the attack and
// resets the detector targets; sets the XF roles.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvidle(BHVIDLE *behavior) {
    StartBhvanim(behavior);
    if (StepguardHasSgf(((STEPGUARD *)behavior->owner), 1)) {
        SetStepguardAttack(((STEPGUARD *)behavior->owner), 0, 0, D_002A2DD0);
        ResetDetectorTargets(((STEPGUARD *)behavior->owner)->pdetector);
        if (((STEPGUARD *)behavior->owner)->handsOffLookAt) behavior->lookAt = ((STEPGUARD *)behavior->owner)->handsOffLookAt;
    }
    SetXfRole(((STEPGUARD *)behavior->owner), ((STEPGUARD *)behavior->owner)->positionXf, 1);
    if (!behavior->lookAt) SetXfRole(((STEPGUARD *)behavior->owner), ((STEPGUARD *)behavior->owner)->rotationXf, 1);
}
} // namespace M_StartBhvidle

// 0x175090 CloseBhvidle
namespace M_CloseBhvidle {
void SetXfRole(STEPGUARD *t, XF *a, int b) __asm__("SetXfRole");
} // namespace M_CloseBhvidle
// 0x175090: BHVIDLE vtable slot 9 (Close): BHVANIM Close and restores the XF roles.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvidle(BHVIDLE *behavior)
{
    using namespace M_CloseBhvidle;
    CloseBhvanim(behavior);
    SetXfRole(((STEPGUARD *)behavior->owner), ((STEPGUARD *)behavior->owner)->positionXf, 2);
    if (!behavior->lookAt)
        SetXfRole(((STEPGUARD *)behavior->owner), ((STEPGUARD *)behavior->owner)->rotationXf, 2);
}

// 0x1750E0 HandleBhvidleMessage
// 0x1750E0: BHVIDLE vtable slot 13 (HandleMessage): ASEG events 0x29/0x2A toggle the ground flag and the XF role;
// else the BHVANIM handler.
// Also known as: Message (upstream readable / Sly 1 name; Message__8JOBSTANDiPv).
extern "C" int HandleBhvidleMessage(BHVIDLE *behavior, int msg, ASEGEVENTMSG *pm)
{
    if (msg == 9 && behavior->binding && GetAnimBindingInstance(behavior->binding) == pm->asega) {
        switch (pm->event) {
        case 0x29:
            ((STEPGUARD *)behavior->owner)->steeringMode = 1;
            M_CloseBhvidle::SetXfRole(((STEPGUARD *)behavior->owner), ((STEPGUARD *)behavior->owner)->rotationXf, 2);
            return 1;
        case 0x2A:
            ((STEPGUARD *)behavior->owner)->steeringMode = 0;
            if (!behavior->lookAt)
                M_CloseBhvidle::SetXfRole(((STEPGUARD *)behavior->owner), ((STEPGUARD *)behavior->owner)->rotationXf, 1);
            return 1;
        }
    }
    return HandleBhvanimMessage(behavior, msg, pm);
}

// 0x1751C0 InitBhvstrike
namespace M_InitBhvstrike {

void InitBhvanim(BHVSTRIKE *p) __asm__("InitBhvanim");
// 0x1751C0: BHVSTRIKE vtable slot 7 (OnPush): BHVANIM OnPush with the attacking flag (3, which broadcasts game
// event 0x14).
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvstrike(BHVSTRIKE *behavior)
{
    InitBhvanim(behavior);
    behavior->sgf = 3;
}
} // namespace M_InitBhvstrike

// 0x1751F0 StartBhvstrike
// 0x1751F0: BHVSTRIKE vtable slot 8 (Start): marks the guard engaged-busy (+0xF3C), BHVANIM Start, and enables the
// leap payload when the binding is kind 2.
// Also known as: Enter (upstream readable / Sly 1 name).
void BHVSTRIKE::Start()
{
    BHVSTRIKE *behavior = this;
    ANIMBINDING *panim;
    ((STEPGUARD *)behavior->owner)->isEngagedBusy = 1;
    StartBhvanim(behavior);
    panim = behavior->binding;
    if (panim->kind == 2) {
        ((STRIKE *)panim->payload)->unknown60 = 1;
        ((STRIKE *)behavior->binding->payload)->unknown64 = 1;
        behavior->isLeapEnabled = 1;
    }
}

// 0x175258 UpdateBhvstrike
namespace M_UpdateBhvstrike {

void UpdateBhvanim(BHVSTRIKE *p) __asm__("UpdateBhvanim");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
void SetLeapTarget(void *p, qword q) __asm__("SetLeapTarget");
// 0x175258: BHVSTRIKE vtable slot 10 (Update): BHVANIM update; while the leap is enabled keeps its target on the
// victim.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvstrike(BHVSTRIKE *behavior)
{
    UpdateBhvanim(behavior);
    if (!behavior->binding || !behavior->binding->asega)
        behavior->isLeapEnabled = 0;
    if (behavior->isLeapEnabled) {
        ANIMBINDING *t = behavior->binding;
        SetLeapTarget(t->asega, GetLoWorldPos(behavior->lookAt));
    }
}
} // namespace M_UpdateBhvstrike

// 0x1752C0 CloseBhvstrike
namespace M_CloseBhvstrike {

void CloseBhvanim(BHVSTRIKE *p) __asm__("CloseBhvanim");
// 0x1752C0: BHVSTRIKE vtable slot 9 (Close): BHVANIM Close and clears engaged-busy.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvstrike(BHVSTRIKE *behavior)
{
    CloseBhvanim(behavior);
    ((STEPGUARD *)behavior->owner)->isEngagedBusy = 0;
}
} // namespace M_CloseBhvstrike

// 0x1752F0 HandleBhvstrikeMessage
namespace M_BHVSTRIKE_FHandleMessage {
void ReleaseLeapConstraint(void *pleap) __asm__("ReleaseLeapConstraint");
} // namespace M_BHVSTRIKE_FHandleMessage
// 0x1752F0: BHVSTRIKE vtable slot 13 (HandleMessage): ASEG events 0x2D/0x2E open/close the hit window, 0x30
// releases the leap; else the BHVANIM handler.
// Also known as: Message (upstream readable / Sly 1 name; Message__9JOBSTRIKEiPv).
extern "C" int HandleBhvstrikeMessage(BHVSTRIKE *behavior, int msg, ASEGEVENTMSG *pm) {
    if (msg == 9 && behavior->binding && GetAnimBindingInstance(behavior->binding) == pm->asega) {
        switch (pm->event) {
        case 0x2D: ((STEPGUARD *)behavior->owner)->isEngagedBusy = 1; return 1;
        case 0x2E: ((STEPGUARD *)behavior->owner)->isEngagedBusy = 0; return 1;
        case 0x30: behavior->isLeapEnabled = 0; M_BHVSTRIKE_FHandleMessage::ReleaseLeapConstraint(behavior->binding->payload); return 1;
        }
    }
    return HandleBhvanimMessage(behavior, msg, pm);
}

// 0x1753D8 InitBhvattack
namespace M_InitBhvattack {

void InitBhv(BHVATTACK *p) __asm__("InitBhv");
void AddStepguardSgf(void *p, int n) __asm__("AddStepguardSgf");
// 0x1753D8: BHVATTACK vtable slot 7 (OnPush): raises flag 0x18 (attack in progress), state -1.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvattack(BHVATTACK *behavior)
{
    InitBhv(behavior);
    AddStepguardSgf(behavior->owner, SGF_ATTACK);
    behavior->state = -1;
}
} // namespace M_InitBhvattack

// 0x175410 StartBhvattack
// 0x175410: BHVATTACK vtable slot 8 (Start): state 0 (strike), busy.
// Also known as: Enter (upstream readable / Sly 1 name).
void BHVATTACK::Start()
{
    BHVATTACK *behavior = this;
    StartBhv(behavior);
    SetBhvattackState(behavior, 0);
    SetBhvBusy(behavior);
}

// 0x175448 CloseBhvattack
// 0x175448: BHVATTACK vtable slot 9 (Close): ends the strike, lowers flag 0x18.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvattack(BHVATTACK *behavior)
{
    CloseBhv(behavior);
    SetBhvattackState(behavior, -1);
    RemoveStepguardSgf((STEPGUARD *)behavior->owner, SGF_ATTACK);
}

// 0x175488 SetBhvattackState
namespace M_SetBhvattackState {
void SetStepguardAimAct(STEPGUARD *guard, CID120 *attack, XF *pxf, qword v) __asm__("SetStepguardAimAct");
BHV *PushStepBehavior(void *pstep, int cid) __asm__("PushStepBehavior");
} // namespace M_SetBhvattackState
// 0x175488: BHVATTACK state switch: 0 subscribes to the victim and pushes a BHVSTRIKE with the chosen attack and
// aim; 1 pushes the follow-through animation (flag 4) after a confirmed hit; -1 clears busy.
// Also known as: SetActState (upstream readable / Sly 1 name; SetActState__6JOBACTi).
extern "C" void SetBhvattackState(BHVATTACK *behavior, int n) {
    using namespace M_SetBhvattackState;
    if (n == behavior->state) return;
    if (behavior->state == 0) {
        UnsubscribeLoObject(behavior->victim, behavior->owner);
        behavior->victim = 0;
        SetStepguardAimAct((STEPGUARD *)behavior->owner, 0, 0, D_002A2DD0);
    }
    PopBhv(&behavior->pbhvNext);
    behavior->state = n;
    behavior->stateTime = g_clock.time;
    switch (n) {
    case 0:
        {
            BHVSTRIKE *o;
            SubscribeLoObject(behavior->victim, behavior->owner);
            o = (BHVSTRIKE *)PushStepBehavior(behavior->owner, CID_BHVSTRIKE);
            o->lookAt = behavior->victim;
            o->binding = behavior->attack->activeBinding;
            if (behavior->attack->aimObject) {
                MATRIX4 b;
                SetStepguardAimAct((STEPGUARD *)behavior->owner, behavior->attack, ((XFM *)behavior->victim)->GetLocalTransform(&b),
                                   *(qword *)b.mat[3]);
            }
            o->Start();
        }
        break;
    case 1:
        {
            BHVANIM *o = (BHVANIM *)PushStepBehavior(behavior->owner, CID_BHVANIM);
            o->sgf = 4;
            o->lookAt = behavior->victim;
            o->Start();
        }
        break;
    case -1:
        ClearBhvBusy(behavior);
        break;
    }
}

// 0x175600 UpdateBhvattack
namespace M_UpdateBhvattack {

void UpdateBhv(BHVATTACK *p) __asm__("UpdateBhv");
int IsBhvBusyAbove(BHVATTACK *p) __asm__("IsBhvBusyAbove");
int IsBhvanimDone(void *p) __asm__("IsBhvanimDone");
// 0x175600: BHVATTACK vtable slot 10 (Update): after the strike, goes to the follow-through if the victim confirmed
// the hit, else finishes.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvattack(BHVATTACK *behavior) {
    int s;
    UpdateBhv(behavior);
    do {
        if (IsBhvBusyAbove(behavior)) return;
        s = behavior->state;
        switch (s) {
        case 0:
            if (behavior->isHitConfirmed) SetBhvattackState(behavior, 1);
            else if (IsBhvanimDone(behavior->pbhvNext)) SetBhvattackState(behavior, -1);
            break;
        case 1:
            if (IsBhvanimDone(behavior->pbhvNext)) SetBhvattackState(behavior, -1);
            break;
        }
    } while (s != behavior->state);
}
} // namespace M_UpdateBhvattack

// 0x1756C0 HandleBhvattackMessage
// 0x1756C0: BHVATTACK vtable slot 13 (HandleMessage): message 0xB from the victim naming this guard means the hit
// landed.
// Also known as: Message (upstream readable / Sly 1 name; Message__6JOBACTiPv).
extern "C" int HandleBhvattackMessage(BHVATTACK *behavior, int msgid, MSGLO *pmsg)
{
    LO *object;
    if (msgid == 0xB) {
        object = pmsg->plo;
        if (object == behavior->victim) {
            if (GetStepguardFromLo(object) == (STEPGUARD *)behavior->owner) {
                behavior->isHitConfirmed = 1;
                return 1;
            }
        }
    }
    return HandleBhvMessage(behavior, msgid, pmsg);
}

// 0x175748 IsBhvattackDone
// 0x175748: True if the attack behaviour has finished (state -1).
extern "C" int IsBhvattackDone(BHVATTACK *behavior)
{
    return behavior->state == -1;
}

// 0x175758 InitBhvcombat
namespace M_InitBhvcombat {

void InitBhv(BHVCOMBAT *p) __asm__("InitBhv");
void AddStepguardSgf(void *p, int n) __asm__("AddStepguardSgf");
// 0x175758: BHVCOMBAT vtable slot 7 (OnPush): raises flag 0x1D (engaged), state -1.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvcombat(BHVCOMBAT *behavior)
{
    InitBhv(behavior);
    AddStepguardSgf(behavior->owner, SGF_COMBAT);
    behavior->state = -1;
}
} // namespace M_InitBhvcombat

// 0x175790 StartBhvcombat
// 0x175790: BHVCOMBAT vtable slot 8 (Start): state 3 (pursue) and sets the guard's target.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvcombat(BHVCOMBAT *behavior)
{
    StartBhv(behavior);
    SetBhvcombatState(behavior, 3);
    SetStepguardTarget((STEPGUARD *)behavior->owner, behavior->target);
}

// 0x1757D0 CloseBhvcombat
// 0x1757D0: BHVCOMBAT vtable slot 9 (Close): state -1, lowers flag 0x1D.
// Also known as: Exit (upstream readable / Sly 1 name).
void BHVCOMBAT::Close()
{
    BHVCOMBAT *behavior = this;
    CloseBhv(behavior);
    SetBhvcombatState(behavior, -1);
    RemoveStepguardSgf((STEPGUARD *)behavior->owner, SGF_COMBAT);
}

// 0x175810 IsBhvcombatDone
// 0x175810: True if combat has ended (state -1: target lost or given up).
extern "C" int IsBhvcombatDone(BHVCOMBAT *behavior)
{
    return behavior->state == -1;
}

// 0x175820 FilterAttackLineOfSightHit
namespace M_func_00175820 {
int IsBasicDerivedFrom__FP5BASIC3CID(SO *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
// 0x175820: Line-test filter for attack checks: ignores non-blocking hits, cid 0x62 objects (when the attack asks)
// and any step.
extern "C" int FilterAttackLineOfSightHit(CID120 *x, SOPART *p) {
    if (p->unknownFlag10A) return 0;
    if (p->owner->physicsTree->grf & 4) return 0;
    if (x->ignoresClass98 && IsBasicDerivedFrom__FP5BASIC3CID(p->owner, CID_CLASS98)) return 0;
    return !IsBasicDerivedFrom__FP5BASIC3CID(p->owner, CID_STEP);
}
} // namespace M_func_00175820

// 0x1758A0 ChooseStepguardAttack
namespace M_PentryChooseStepguardAttack {
union U; struct SCALE; struct LSG; struct LOS; struct POLY; struct BASIC;
// ChooseStepguardAttack: pick the first enabled response ENTRY of p that accepts target (range, height, angle, optional
// line of sight), ask p's HandleSpliceEvent(0x18) about it with a CRef(true), then run the response.
// (Claude agent, T1672) MATCH from lanef10v1 with real C++ locals instead of the SLOT union and explicit calls:
// FRAME with out-of-line ctor/dtor (STACKSCOPE_ctor / STACKSCOPE_ctor_1AE4A0(this, 2)), CRef(bool) with inline dtor
// (Decref), and `void *argument = (char *)entry + 0x18;` declared after the CRef.
union U { qword q; float f[4]; };
struct SCALE { char pad[0x20]; float value; };   /* STEPGUARD::scale's record */
struct LSG { U point; char rest[0x50]; };
struct LOS { U start, end; LSG hit; };
extern void Decref__4CRef(void *) __asm__("Decref__4CRef");
struct POLY { float a, b, c; };
struct BASIC {
    char pad0[0xC];
    virtual void AddRef();
    virtual void Release();
    virtual void HandleSpliceEvent(unsigned symidEvent, int fPost, int cargs, void **ppvargs, CRef *pref);
};

extern POLY D_002BE600 __asm__("D_002BE600");
extern LM g_lmZeroOne __asm__("g_lmZeroOne");
int DoesTargetRecordAllowGuard(void *, STEPGUARD *) __asm__("DoesTargetRecordAllowGuard");
unsigned GetSourceIndexFromProxyId__Fi(unsigned) __asm__("GetSourceIndexFromProxyId__Fi");
int IsWithinLimits__FP2LMf(LM *, float) __asm__("IsWithinLimits__FP2LMf");
qword ConvertPosBetweenLos(LO *, LO *, qword) __asm__("ConvertPosBetweenLos");
float atan2f(float, float) __asm__("atan2f");
int FilterAttackLineOfSightHit(void *, void *) __asm__("FilterAttackLineOfSightHit");
void IntersectSwBoundingBox(void *, qword, qword, int (*)(void *, void *), void *, int *, void ***) __asm__("IntersectSwBoundingBox");
int FindFirstObjectHitByLine(int, qword, qword, int, void **, LSG *) __asm__("FindFirstObjectHitByLine");
void SetStepguardAttack(STEPGUARD *, CID120 *, XF *, qword) __asm__("SetStepguardAttack");
void SetTargetRecordState(void *, int) __asm__("SetTargetRecordState");

// VU0 helper: vector difference.
/* the second "j"(v) is allocation-only (same helper as GetHeroInteractDistance): it fixes the from/to lqc2 reloads */
static inline qword VuLengthXY(qword v)
{
    qword qw;
    __asm__("vmul.xy %0, %1, %2\n\tvaddy.x %0, %0, %0y" : "=j"(qw) : "j"(v), "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
// VU0 helper: length of a vector (in x).
// VU0 helper: the x component of a VU register as a float.
// VU0 helper: horizontal (xy) length of a vector.
static inline float VectorLengthXY(qword v)
{
    return VuToFloat(VuLengthXY(v));
}
// VU0 helper: length of a vector.
static inline float VectorLength(qword v)
{
    return VuToFloat(VuLength(v));
}
// VU0 helper: x component of a quadword vector.
// VU0 helper: y component of a quadword vector.
// VU0 helper: z component of a quadword vector.

// True if the line test hit something nearer than `limit` to the line start.
static inline int Blocked(LOS *line, float limit)
{
    return VectorLength(VuSub(line->start.q, line->hit.point.q)) < limit;
}

// 0x1758A0: Picks the first enabled attack whose range, height, angle and line of sight accept the target, confirms
// it with Splice event 0x18, starts it and marks the target record as attacked. Returns the attack, or NULL.
// Also known as: PactionSelect (upstream readable / Sly 1 name; PactionSelect__FP5GUARDP2LOP9TARGETREC).
extern "C" CID120 *ChooseStepguardAttack(STEPGUARD *guard, LO *target, void *gate)
{
    qword from, to;
    float height, distance;
    int i;

    if (guard->currentAttack)
        return 0;
    if (!target)
        return 0;
    {
        int enabled = ((int)((LO *)(target))->isInWorld & 1);
        if (!enabled)
            return 0;
    }
    if (g_clock.time < guard->nextAttackTime)
        return 0;
    if (IsBasicDerivedFrom(target, (CID)CID_PO)) {
        if (((PO *)target)->GetHealthState())
            return 0;
    }
    if (gate && !DoesTargetRecordAllowGuard(gate, guard))
        return 0;
    from = GetLoWorldPos(guard);
    to = GetLoWorldPos(target);
    {
        qword d = VuSub(to, from);
        distance = VectorLengthXY(d);
        height = QwordZ(d);
    }
    if (GetSourceIndexFromProxyId__Fi(target->proxyRecord->proxyId) == GetSourceIndexFromProxyId__Fi(guard->proxyRecord->proxyId))
        distance += ((unsigned)target < (unsigned)guard) ? 75.0f : -75.0f;
    if ((guard->specialState == 8 || guard->specialState == 13) && guard->scale) {
        float s = ((SCALE *)guard->scale)->value;
        distance /= s;
        height /= s;
    }
    for (i = 0; i < guard->numAttacks; i++) {
        CID120 *entry = guard->attacks[i];
        float angle, gain;
        if (!entry->isEnabled)
            continue;
        if (!IsWithinLimits__FP2LMf(&entry->distance, distance))
            continue;
        if (!IsWithinLimits__FP2LMf(&entry->height, height))
            continue;
        {
            qword local = ConvertPosBetweenLos(0, guard, to);
            angle = __builtin_fabsf(atan2f(QwordY(local), QwordX(local)));
        }
        gain = D_002BE600.a + distance * (D_002BE600.b + distance * D_002BE600.c);
        {
            float lo = g_lmZeroOne.minValue, hi = g_lmZeroOne.maxValue;
            if (gain < lo)
                gain = lo;
            else if (hi < gain)
                gain = hi;
        }
        if (entry->angle < gain * angle)
            continue;
        if (entry->needsLineOfSight) {
            STACKSCOPE frame;
            LOS line;
            int count;
            void **objects;
            line.start.q = from;
            line.end.q = to;
            IntersectSwBoundingBox(&g_sw, from, to, FilterAttackLineOfSightHit, entry, &count, &objects);
            if (FindFirstObjectHitByLine(0, line.start.q, line.end.q, count, objects, &line.hit) &&
                Blocked(&line, entry->minLineOfSight))
                continue;
        }
        {
            CRef ref(true);
            void *argument = (char *)entry + 0x18;
            ((BASIC *)guard)->HandleSpliceEvent(0x18, 0, 1, &argument, &ref);
            if (ref.m_tagk != 0x17 || !ref.u.n)
                continue;
            if (entry->aimsAtTarget) {
                MATRIX4 frame;
                XF *owner = ((XFM *)target)->GetLocalTransform(&frame);
                SetStepguardAttack(guard, entry, owner, *(qword *)frame.mat[3]);
                entry = 0;
                if (gate)
                    SetTargetRecordState(gate, 3);
            }
            return entry;
        }
    }
    return 0;
}
} // namespace M_PentryChooseStepguardAttack

// 0x175D00 CanBhvcombatShoot
namespace M_FBhvcombatCanShoot {
struct X;
struct X { int a[4]; };          /* the STACKSCOPE (constructed and destroyed by hand: the out-of-line pair) */
float GetStepHeading(STEP *pstep) __asm__("GetStepHeading");
void GetVectorSphere(qword v, float *a, float *b, float *c) __asm__("GetVectorSphere");
int IsWithinLimits__FP2LMf(LM *plm, float f) __asm__("IsWithinLimits__FP2LMf");
float NormalizeAngle__Ff(float f) __asm__("NormalizeAngle__Ff");
qword *GetLoWorldMatrix(LO *plo) __asm__("GetLoWorldMatrix");
void CalculateVectorPanTilt(qword v, int n, float *pf) __asm__("CalculateVectorPanTilt");
void STACKSCOPE_ctor(struct X *x) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(struct X *x, int n) __asm__("_$_10STACKSCOPE");
int ShouldKeepSoNotSelf(void) __asm__("ShouldKeepSoNotSelf");
void IntersectSwBoundingBox(SW *psw, qword a, qword b, int (*fn)(void), SOTREE *ptree, int *pa, SOPART ***pb) __asm__("IntersectSwBoundingBox");
SOPART *FindFirstObjectHitByLine(int n, qword a, qword b, int c, SOPART **d, LSG *y) __asm__("FindFirstObjectHitByLine");
} // namespace M_FBhvcombatCanShoot
// 0x175D00: True if the guard should use its ranged attack now: it can shoot (capability bit 3), no attack is
// running, the target is inside its range/pan/tilt limits and visible; immediately when the target is out of melee
// reach (hanging, on a ledge, ...).
// Also known as: FCanShoot (upstream readable / Sly 1 name).
extern "C" int CanBhvcombatShoot(BHVCOMBAT *behavior) {
    using namespace M_FBhvcombatCanShoot;
    struct X x;
    LSG y;   /* the line test's hit record */
    float f[4];
    int na; SOPART **nb;
    qword pa, pb;
    qword d;
    float g;
    int mustShootNow;
    JT *t;
    STEPGUARD *a = ((STEPGUARD *)behavior->owner);
    if ((a->capabilityFlags & 8) == 0 || a->currentAttack || behavior->attack || !behavior->target || (((int)((LO *)behavior->target)->isInWorld & 1) == 0)) return 0;
    if (IsBasicDerivedFrom(behavior->target, (CID)CID_PO)) {
        if (((PO *)behavior->target)->GetHealthState() == 2) return 0;
    }
    mustShootNow = 0;
    if (IsBasicDerivedFrom(behavior->target, (CID)CID_STEPPLAYER)) {
        t = ((JT *)behavior->target);
        if (t->jtgrab.kind != -1) {
            if (t->jtgrab.state < 5) mustShootNow = t->jtgrab.state >= 3;
        }
        if (t->jtair.state != -1) {
            if (t->jtair.state == 0x11) mustShootNow = 1;
        }
        if (t->jtpipe.state != -1) mustShootNow = 1;
        if (IsBasicDerivedFrom(t, (CID)CID_JT)) {
            switch (t->state) {
            case 3: case 0x15: case 0x16:
                mustShootNow = 1;
                break;
            }
        }
    }
    pa = GetLoWorldPos(((STEPGUARD *)behavior->owner));
    pb = GetLoWorldPos(behavior->target);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pb), "j"(pa));
    g = GetStepHeading(behavior->owner);
    GetVectorSphere(d, &f[0], &f[1], &f[2]);
    if (!IsWithinLimits__FP2LMf(&((STEPGUARD *)behavior->owner)->shootRange, f[2])) return 0;
    {
        float h = NormalizeAngle__Ff(f[0] - g);
        if (!IsWithinLimits__FP2LMf(&((STEPGUARD *)behavior->owner)->shootPan, h)) return 0;
    }
    CalculateVectorPanTilt(*GetLoWorldMatrix(((STEPGUARD *)behavior->owner)), 0, &f[3]);
    {
        float h = NormalizeAngle__Ff(f[1] - f[3]);
        if (!IsWithinLimits__FP2LMf(&((STEPGUARD *)behavior->owner)->shootTilt, h)) return 0;
    }
    if (!mustShootNow) {
        if (behavior->state == 0) {
            if (g_clock.time - behavior->stateTime < 3.0f) return 0;
        } else if (behavior->state != 2) {
            return 0;
        }
    }
    {
        SOPART *r;
        STACKSCOPE_ctor(&x);
        IntersectSwBoundingBox(&g_sw, pa, pb, ShouldKeepSoNotSelf, ((STEPGUARD *)behavior->owner)->physicsTree, &na, &nb);
        r = FindFirstObjectHitByLine(0, pa, pb, na, nb, &y);
        if (r && r->owner->root->topObject != behavior->target) {
            STACKSCOPE_ctor_1AE4A0(&x, 2);
            return 0;
        }
        STACKSCOPE_ctor_1AE4A0(&x, 2);
        return 1;
    }
}

// 0x175FF0 TryBhvcombatChooseAttack
// 0x175FF0: Tries to choose an attack on the target; on failure pushes the attack cooldown forward. Returns whether
// one was chosen.
// Also known as: FTryAction (upstream readable / Sly 1 name).
extern "C" int TryBhvcombatChooseAttack(BHVCOMBAT *behavior)
{
    STEPGUARD *guard;
    CID120 *pentry;
    guard = (STEPGUARD *)behavior->owner;
    pentry = ChooseStepguardAttack(guard, behavior->target, guard->pvEngage);
    behavior->attack = pentry;
    if (pentry != 0) {
        return 1;
    }
    guard = (STEPGUARD *)behavior->owner;
    if (guard->nextAttackTime < g_clock.time) {
        guard->nextAttackTime = g_clock.time + D_002BE610;
    }
    return 0;
}

// 0x176060 HasBhvcombatLostTarget
namespace M_FBhvcombatLostTarget {

int IsTargetRecordStale(void *p) __asm__("IsTargetRecordStale");
// 0x176060: True if the target is lost: no hit confirmed and the shared target record is stale.
// Also known as: FLost (upstream readable / Sly 1 name).
extern "C" int HasBhvcombatLostTarget(BHVCOMBAT *behavior)
{
    int f;
    if (behavior->isHitConfirmed)
        f = 0;
    else if (((STEPGUARD *)behavior->owner)->unknown3D4)
        f = 0;
    else
        f = IsTargetRecordStale(((STEPGUARD *)behavior->owner)->pvEngage);
    return f;
}
} // namespace M_FBhvcombatLostTarget

// 0x176098 UpdateBhvcombat
namespace M_UpdateBhvcombat {
union U; struct SCALE; struct X; struct WORK;
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef union U{qword q;float f[4];}U;
typedef struct SCALE{char pad[0x20];float value;}SCALE;   /* STEPGUARD::scale's record */
/* a PATHLINKEND without its clearing constructor (ClearPointerPair is called by hand, after the distance tests) */
typedef struct X{void*a,*b;char pad8[8];qword point;}X;
typedef struct WORK{X frame;QWROW point;}WORK;
extern float D_002BE614 __asm__("D_002BE614");

int IsStepguardHandsOff(STEPGUARD*) __asm__("IsStepguardHandsOff");
int HasBhvcombatLostTarget(BHVCOMBAT*) __asm__("HasBhvcombatLostTarget");
struct CID17*FindPathzoneAtPos(qword,int) __asm__("FindPathzoneAtPos");
void ClearPointerPair(X*) __asm__("ClearPointerPair");
void SetBhvgotoGoal(void*,X*) __asm__("SetBhvgotoGoal");
int IsBhvattackDone(void*) __asm__("IsBhvattackDone");
int IsBhvshootDone(void*) __asm__("IsBhvshootDone");
int IsBhvanimDone(void*) __asm__("IsBhvanimDone");
void EmitStepguardStimulus(STEPGUARD*,LO*) __asm__("EmitStepguardStimulus");
// Stores a quadword (keeps the target's instruction choice).
extern "C" { static inline void StorePoint(QWROW*home,qword a){__asm__("sq %1,%0":"=m"(home->q):"r"(a):"memory");} }
// VU0 helper: x of a VU register as a float (register-pinned).
extern "C" { static inline float XFloat(qword q){register float f __asm__("$3");__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(q));return f;} }
// VU0 helper: square root of x.
extern "C" { static inline qword Root(qword q){__asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(q));return q;} }
// Matching helper (register-pinned asm): horizontal distance from `pos` to a point; also returns the actor and its mode.
extern "C" { static inline float DistanceXY(BHVCOMBAT *p, QWROW *point, qword pos, STEPGUARD **actor, int *mode)
{
    register qword a __asm__("vf2"), b __asm__("vf3"), d __asm__("vf1");
    STEPGUARD *o;
    register int n __asm__("$5");
    float f;
    b = pos;
    o = (STEPGUARD *)p->owner;
    a = point->q;
    *mode = o->specialState;
    __asm__("vsub.xyzw %0,%1,%2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0,%0,%0" : "+j"(d));
    __asm__("vaddy.x %0,%0,%0y" : "+j"(d));
    d = Root(d);
    __asm__("qmfc2.ni %0,%1" : "=r"(n) : "j"(d));
    __asm__("mtc1 %1,%0" : "=f"(f) : "r"(n));
    *actor = o;
    return f;
} }
// Returns the region object of the actor's tuning owner (NULL if none).
extern "C" { static inline struct CID17*ActorRegion(STEPGUARD*actor){REGENT*owner=actor->physics?actor->physics->pregent:0;return owner?owner->pathzone:0;} }
// Matching helper (register-pinned asm): true if the actor moved farther than a .data threshold from `point`.
extern "C" { static inline int Moved(BHVCOMBAT *p, QWROW *point)
{
#ifdef SLY2_EE_ASM
    register qword a __asm__("vf2"), d __asm__("vf1");
    qword one;
    register char *high __asm__("$2");
    register float threshold __asm__("$f1");
    float distance;
    register int result __asm__("$2");
    a = point->q;
    __asm__("lui %0,%1" : "=r"(high) : "i"(0x2C));
    d = p->cachedPosition;
    __asm__("vsub.xyzw %0,%1,%0" : "+j"(d) : "j"(a));
    __asm__(
        "lwc1 %2,-6636(%3)\n\tvmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
        : "+j"(d), "=&j"(one), "=f"(threshold)
        : "r"(high), "m"(*(float *)(high - 6636)));
    __asm__("addiu %1,$zero,1\n\tvsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "+j"(d), "=r"(result));
    distance = XFloat(d);
    result = threshold < distance ? result : 0;
    return result;
#else
    VuLanes d = VuGet(VuSub(point->q, p->cachedPosition));
    float distance = VuSqrt((d.x * d.x + d.y * d.y) + d.z * d.z);
    return D_002BE614 < distance ? 1 : 0;
#endif
} }
typedef char U_Size[sizeof(QWROW)==16?1:-1];
typedef char X_Size[sizeof(X)==32?1:-1];
typedef char WORK_Size[sizeof(WORK)==48?1:-1];
typedef char SCALE_value_Offset[FIELD_OFFSET(SCALE,value)==32?1:-1];
typedef char X_a_Offset[FIELD_OFFSET(X,a)==0?1:-1];
typedef char X_b_Offset[FIELD_OFFSET(X,b)==4?1:-1];
typedef char X_point_Offset[FIELD_OFFSET(X,point)==16?1:-1];
typedef char WORK_frame_Offset[FIELD_OFFSET(WORK,frame)==0?1:-1];
typedef char WORK_point_Offset[FIELD_OFFSET(WORK,point)==32?1:-1];
// 0x176098: BHVCOMBAT vtable slot 10 (Update): state machine 0 close in / 3 pursue -> 1 melee attack, 2 ranged
// attack, 4 give up, 5 hands-off; re-goals the pursuit when the target moves far or changes path zone; emits a
// stimulus to alert others.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvcombat(BHVCOMBAT *behavior)
{
    WORK work;
    int old;
    UpdateBhv(behavior);
    do {
        old = behavior->state;
        switch (old) {
        case 0:
        case 3: {
            float distance;
            struct CID17 *region;
            qword current;
            STEPGUARD *actor;
            int mode;
            if (IsBhvBusyAbove(behavior))
                break;
            if (IsStepguardHandsOff(((STEPGUARD *)behavior->owner))) {
                SetBhvcombatState(behavior, 5);
                break;
            }
            if (HasBhvcombatLostTarget(behavior)) {
                SetBhvcombatState(behavior, 4);
                break;
            }
            if (TryBhvcombatChooseAttack(behavior)) {
                SetBhvcombatState(behavior, 1);
                break;
            }
            if (CanBhvcombatShoot(behavior)) {
                SetBhvcombatState(behavior, 2);
                break;
            }
            StorePoint(&work.point, GetLoWorldPos(behavior->target));
            current = GetLoWorldPos(((STEPGUARD *)behavior->owner));
            distance = DistanceXY(behavior, &work.point, current, &actor, &mode);
            if (mode == 8 || mode == 13) {
                SCALE *scale = (SCALE *)actor->scale;
                if (scale)
                    distance /= scale->value;
            }
            region = FindPathzoneAtPos(work.point.q, 1);
            if (behavior->state != 0) {
                if (behavior->state != 3)
                    break;
                if (distance < ((STEPGUARD *)behavior->owner)->attackRange.minValue && region == ActorRegion(((STEPGUARD *)behavior->owner))) {
                    SetBhvcombatState(behavior, 0);
                    break;
                }
                if (behavior->pathzone == region && !Moved(behavior, &work.point))
                    break;
                behavior->pathzone = region;
                behavior->cachedPosition = work.point.q;
                ClearPointerPair(&work.frame);
                {
                    register qword framePoint __asm__("$3") = work.point.q;
                    work.frame.point = framePoint;
                }
                SetBhvgotoGoal(behavior->pbhvNext, &work.frame);
            } else {
                if (((STEPGUARD *)behavior->owner)->attackRange.maxValue < distance || region != ActorRegion(((STEPGUARD *)behavior->owner)))
                    SetBhvcombatState(behavior, 3);
            }
            break;
        }
        case 1:
            if (IsStepguardHandsOff(((STEPGUARD *)behavior->owner))) {
                SetBhvcombatState(behavior, 5);
                break;
            }
            if (!IsBhvBusyAbove(behavior) && IsBhvattackDone(behavior->pbhvNext))
                SetBhvcombatState(behavior, 0);
            break;
        case 2:
            if (IsStepguardHandsOff(((STEPGUARD *)behavior->owner))) {
                SetBhvcombatState(behavior, 5);
                break;
            }
            if (!IsBhvBusyAbove(behavior) && IsBhvshootDone(behavior->pbhvNext))
                SetBhvcombatState(behavior, 0);
            break;
        case 4:
            if (!IsBhvBusyAbove(behavior) && IsBhvanimDone(behavior->pbhvNext))
                SetBhvcombatState(behavior, -1);
            break;
        case 5:
            if (!IsBhvBusyAbove(behavior) && !IsStepguardHandsOff(((STEPGUARD *)behavior->owner)))
                SetBhvcombatState(behavior, 3);
            break;
        }
    } while (old != behavior->state);
    if (((STEPGUARD *)behavior->owner)->unknownF40 && old < 4 && old >= 0 && ((STEPGUARD *)behavior->owner)->alertCooldownEnd <= g_clock.time
        && g_sw.guardAlertResumeTime <= g_clock.time)
        EmitStepguardStimulus(((STEPGUARD *)behavior->owner), behavior->target);
}

typedef char Pointer_Size[sizeof(void*)==4?1:-1];
typedef char Integer_Size[sizeof(int)==4?1:-1];
typedef char Qword_Size[sizeof(qword)==16?1:-1];
} // namespace M_UpdateBhvcombat

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A5408 __asm__("D_002A5408") = 3.40282347e+38f; }
// 0x176458 SetBhvcombatState
namespace M_SetBhvcombatState {
extern float D_002A5408 __asm__("D_002A5408");
void PopBhv(void *p) __asm__("PopBhv");
float RandomFloatInRange__Fff(float a, float b) __asm__("RandomFloatInRange__Fff");
BHV *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
void SetTargetRecordState(void *p, int n) __asm__("SetTargetRecordState");
void SetBhvgotoGoal(BHVGOTO *p, PATHLINKEND *x) __asm__("SetBhvgotoGoal");
struct CID17 *FindPathzoneAtPos(qword v, int n) __asm__("FindPathzoneAtPos");
void SetDetectorTargetStateById(void *p, LO *a, int b) __asm__("SetDetectorTargetStateById");
// 0x176458: BHVCOMBAT state switch: 0 circle (BHVCIRCLE), 1 attack (BHVATTACK), 2 shoot (BHVSHOOT), 3 run toward
// the target (BHVGOTO with flag 0x1C), 4 lost-him animation (flag 0xD) and drop the target, 5 hands-off idle.
// Also known as: SetPursueState (upstream readable / Sly 1 name; SetPursueState__9JOBPURSUEi).
extern "C" void SetBhvcombatState(BHVCOMBAT *behavior, int n) {
    if (n == behavior->state) return;
    PopBhv((&behavior->pbhvNext));
    if (behavior->state == 1) {
        float t = g_clock.time;
        t += RandomFloatInRange__Fff(behavior->attack->cooldownMin, behavior->attack->cooldownMax);
        ((STEPGUARD *)behavior->owner)->nextAttackTime = t;
        behavior->attack = 0;
    }
    behavior->state = n;
    behavior->stateTime = g_clock.time;
    switch (n) {
    case 0:
        {
            BHV *o = PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHV179);
            o->Start();
        }
        break;
    case 1:
        {
            BHVATTACK *o = (BHVATTACK *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHVATTACK);
            o->victim = behavior->target;
            o->attack = behavior->attack;
            ((STEPGUARD *)behavior->owner)->nextAttackTime = D_002A5408;
            o->Start();
        }
        SetTargetRecordState(((STEPGUARD *)behavior->owner)->pvEngage, 3);
        break;
    case 2:
        {
            BHVSHOOT *o = (BHVSHOOT *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHV173);
            o->target = behavior->target;
            o->Start();
        }
        SetTargetRecordState(((STEPGUARD *)behavior->owner)->pvEngage, 4);
        break;
    case 3:
        {
            BHVGOTO *o = (BHVGOTO *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHVGOTO);
            qword v1, v2;
            o->allowPartialRoute = 1;
            o->sgf = 0x1C;
            v1 = GetLoWorldPos(behavior->target);
            v2 = GetLoWorldPos(behavior->target);
            PATHLINKEND x;
            x.pos = v2;
            SetBhvgotoGoal(o, &x);
            o->lookAtTarget = behavior->target;
            o->unknownAC = 1;
            o->Start();
            behavior->cachedPosition = v1;
            behavior->pathzone = FindPathzoneAtPos(v1, 1);
        }
        break;
    case 4:
        {
            BHVANIM *o = (BHVANIM *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHVANIM);
            o->sgf = 0xD;
            o->Start();
        }
        SetDetectorTargetStateById(((STEPGUARD *)behavior->owner)->pdetector, behavior->target, 0);
        SetStepguardTarget(((STEPGUARD *)behavior->owner), 0);
        break;
    case 5:
        {
            BHVANIM *o = (BHVANIM *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHVIDLE);
            o->sgf = 1;
            o->Start();
        }
        break;
    }
}
} // namespace M_SetBhvcombatState

// 0x1766D8 InitBhvguard
// 0x1766D8: BHVGUARD vtable slot 7 (OnPush): no mode (-1).
void BHVGUARD::OnPush()
{
    InitBhv(this);
    mode.kind = -1;
}

// 0x176708 StartBhvguard
namespace M_StartBhvguard {
void func_002333B4(void *p, int c, int n) __asm__("memset");
void SetBhvguardMode(BHV156 *behavior, BHVGUARDRECDATA *m) __asm__("SetBhvguardMode");
} // namespace M_StartBhvguard
// 0x176708: BHVGUARD vtable slot 8 (Start): mode 0 (which immediately becomes patrol).
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvguard(BHV156 *p)
{
    using namespace M_StartBhvguard;
    BHVGUARDRECDATA f;
    StartBhv(p);
    func_002333B4(&f, 0, sizeof(f));
    f.kind = GUARD_MODE_START;
    SetBhvguardMode(p, &f);
}

// 0x176750 SetBhvguardMode
namespace M_SetBhvguardMode {
extern float D_002BE618[2] __asm__("D_002BE618");
int IsBhvguardModeSame(BHVGUARDRECDATA *a, BHVGUARDRECDATA *b) __asm__("IsBhvguardModeSame");
void PopBhv(void *p) __asm__("PopBhv");
BHV *PushStepBehavior(STEPGUARD *p, int n) __asm__("PushStepBehavior");
void SetBhvScriptScript(BHV169 *o, void *a, int b, int c) __asm__("SetBhvScriptScript");
void SetBhvSearchWatched(BHV171 *o, void *n) __asm__("SetBhvSearchWatched");
float RandomFloatInRange__Fff(float a, float b) __asm__("RandomFloatInRange__Fff");
void SetStepguardTarget(STEPGUARD *p, int n) __asm__("SetStepguardTarget");
}
// 0x176750: BHVGUARD mode switch (record {kind, object, ...}): pops the child and pushes the behaviour for the new
// mode: 1 patrol, 2 search a stimulus, 3 alert animation, 4 combat, 5 script, 6 thrown, 7 held, 8 sleep, and the
// special modes.
// Also known as: React (upstream readable / Sly 1 name; React__8JOBREACTP5AIMSG).
extern "C" void SetBhvguardMode(BHV156 *behavior, BHVGUARDRECDATA *m) {
    using namespace M_SetBhvguardMode;
    int clearsTarget;
    if (IsBhvguardModeSame(m, &behavior->modeRec)) return;
    PopBhv(&behavior->pbhvNext);
    behavior->modeRec = *m;
    behavior->modeTime = g_clock.time;
    clearsTarget = 0;
    switch (m->kind) {
    case GUARD_MODE_SCRIPT:
        {
            BHV169 *o;
            clearsTarget = 1;
            o = (BHV169 *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHV169);
            SetBhvScriptScript(o, m->arg.object, 1, 0);
            o->Start();
        }
        break;
    case GUARD_MODE_CALM:
        {
            BHV *o;
            clearsTarget = 1;
            o = (BHV *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHVPATROL);
            o->Start();
        }
        break;
    case GUARD_MODE_ALERT_ANIM:
        {
            BHVANIM *o = (BHVANIM *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHVANIM);
            o->sgf = 0xB;
            o->lookAt = (LO *)m->arg.object;
            o->Start();
        }
        break;
    case GUARD_MODE_ALERT:
        {
            BHVCOMBAT *o = (BHVCOMBAT *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHVCOMBAT);
            o->target = (LO *)m->arg.object;
            o->Start();
        }
        break;
    case GUARD_MODE_SUSPICIOUS:
        {
            BHV171 *o;
            clearsTarget = 1;
            o = (BHV171 *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHV171);
            SetBhvSearchWatched(o, m->arg.object);
            o->Start();
        }
        break;
    case GUARD_MODE_HURT:
        {
            BHVHIT *o = (BHVHIT *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHV161);
            o->zpr = m->zap;
            o->Start();
        }
        break;
    case GUARD_MODE_KNOCKDOWN:
        {
            BHVHIT *o = (BHVHIT *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHV162);
            o->zpr = m->zap;
            o->Start();
        }
        break;
    case GUARD_MODE_KO:
        {
            BHVHIT *o = (BHVHIT *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHVKO);
            o->zpr = m->zap;
            clearsTarget = 1;
            o->Start();
        }
        break;
    case GUARD_MODE_SLEEP:
        {
            BHV *o = (BHV *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHV175);
            o->Start();
        }
        break;
    case GUARD_MODE_THROWN:
        {
            BHV176 *o = (BHV176 *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHV176);
            o->thrower = (sptr)behavior->modeRec.arg.object;
            o->Start();
        }
        break;
    case GUARD_MODE_JUGGLED:
        {
            BHVJUGGLED *o = (BHVJUGGLED *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHVJUGGLED);
            o->holder = (SO *)m->arg.object;
            o->Start();
        }
        break;
    case GUARD_MODE_TARGET_LIST:
        {
            BHV *o = (BHV *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHV178);
            o->Start();
        }
        break;
    case GUARD_MODE_FLEE:
        {
            BHV180 *o = (BHV180 *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHV180);
            o->target = (LO *)m->arg.object;
            o->Start();
        }
        break;
    case GUARD_MODE_BHV182:
        {
            BHV182 *o = (BHV182 *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHV182);
            o->route = (CID123 *)m->arg.object;
            o->Start();
        }
        break;
    case GUARD_MODE_NEELA_CHASE:
        {
            BHV174 *o = (BHV174 *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHV174);
            o->chaseScript = ((NEELA *)behavior->owner)->chaseScript;
            o->Start();
        }
        break;
    case GUARD_MODE_ESCORT:
        {
            BHV183 *o;
            clearsTarget = 1;
            o = (BHV183 *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHV183);
            o->escorted = ((STEPGUARD *)behavior->owner)->escortObject;
            o->localOffset = ((STEPGUARD *)behavior->owner)->escortPos;
            o->Start();
        }
        break;
    case GUARD_MODE_WATCH:
        {
            BHVIDLE *o = (BHVIDLE *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHVIDLE);
            o->sgf = 0x1A;
            o->lookAt = (LO *)m->arg.object;
            o->Start();
        }
        { float t = g_clock.time; behavior->watchEndTime = t + RandomFloatInRange__Fff(D_002BE618[0], D_002BE618[1]); }
        break;
    case GUARD_MODE_STALK:
        {
            BHV189 *o = (BHV189 *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHV189);
            o->target = (LO *)m->arg.object;
            o->forcedDelay = m->arg.amount;
            o->Start();
        }
        break;
    case GUARD_MODE_RELOCATE:
        {
            BHV191 *o = (BHV191 *)PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHV191);
            o->end.pos = ((STEPGUARD *)behavior->owner)->relocatePoint;
            o->Start();
        }
        break;
    }
    if (clearsTarget) SetStepguardTarget(((STEPGUARD *)behavior->owner), 0);
}
// (end of SetBhvguardMode: M_SetBhvguardMode)

// 0x176B80 InflictBhvguardZap
namespace M_InflictBhvguardZap {
void *func_002333B4(void *p, int c, int n) __asm__("memset");
void DeliverZapToSo(ZPR *pzpr) __asm__("DeliverZapToSo");
} // namespace M_InflictBhvguardZap
// 0x176B80: Inflicts a zap of kind 0x40000 and `amount` on the guard from `source`.
// Also known as: SendHit (upstream readable / Sly 1 name; SendHit__8JOBREACTii).
extern "C" void InflictBhvguardZap(BHVGUARD *behavior, int amount, SO *source)
{
    using namespace M_InflictBhvguardZap;
    ZPR f;
    func_002333B4(&f, 0, sizeof(f));
    f.ploSource = source;
    f.dhp = amount;
    f.zpk = 0x40000;
    f.unknown08 = behavior->owner;
    DeliverZapToSo(&f);
}

// 0x176BE8 HandleBhvguardMessage
namespace M_BHVGUARD_FHandleMessage {
union U; struct WORK; struct OFFSETINPUTS;
// Handle movement-state response events.
union U{qword q;float f[4];};
struct WORK{BHVGUARDRECDATA next;U scalar,own,other,copy,delta;};
struct OFFSETINPUTS{qword own,other,copy;};

extern MURRAY*D_002F7900 __asm__("D_002F7900");
extern float D_002BE620 __asm__("D_002BE620");
void*func_002333B4(void*,int,unsigned) __asm__("memset");
MATRIX4*GetLoWorldMatrix(LO*) __asm__("GetLoWorldMatrix");
qword ConvertVecBetweenLos(LO*,LO*,qword) __asm__("ConvertVecBetweenLos");
void SpawnMurrayHitEffectUnchecked(MURRAY*,qword) __asm__("SpawnMurrayHitEffectUnchecked");
void SpawnMurrayHitEffect(MURRAY*,qword) __asm__("SpawnMurrayHitEffect");
LO*NewNoise(void*,qword,STEPGUARD*) __asm__("NewNoise");
void SetStepguardSpecial(STEPGUARD*,int) __asm__("SetStepguardSpecial");
void SetSoZapMode(SO*,int) __asm__("SetSoZapMode");
unsigned GetSoZapResponseFlags(SO*,unsigned) __asm__("GetSoZapResponseFlags");
int GetZapKindIndex(unsigned) __asm__("GetZapKindIndex");
void SetTargetRecordState(TARGETREC*,int) __asm__("SetTargetRecordState");
void SetBhvSleepState(BHV175*,int) __asm__("SetBhvSleepState");
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(ASEG*,STEPGUARD*,float,float,int,ASEGA**) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");
unsigned long GetSgfMask(int) __asm__("GetSgfMask");
int IsStepguardTargetValidEx(STEPGUARD*,LO*,unsigned long) __asm__("IsStepguardTargetValidEx");
int IsStepguardTargetValid(STEPGUARD*,LO*,unsigned long) __asm__("IsStepguardTargetValid");
void*FindBestAnimBinding(STEPGUARD*,unsigned long) __asm__("FindBestAnimBinding");

// Clears a mode record.
static __inline__ void Clear(BHVGUARDRECDATA*s){(void)func_002333B4(s,0,sizeof(*s));}
// The guard's zap-reaction binding tables (STEPGUARD::knockdownReactions and hurtReactions), indexed by zap kind.
enum { KNOCKDOWN_REACTIONS = SLY2_OFFSETOF(STEPGUARD, knockdownReactions), HURT_REACTIONS = SLY2_OFFSETOF(STEPGUARD, hurtReactions) };
// Returns the per-kind slot of an object's table.
static __inline__ ANIMBINDING *Slot(STEPGUARD *p, unsigned kind, int offset)
{
    int index = GetZapKindIndex(kind);
    return (ANIMBINDING *)((char *)p + offset + index * sizeof(ANIMBINDING));
}
// The larger of two values.
static __inline__ float Max(float a,float b){register float right __asm__("$f1")=b;__asm__("max.s %0,%0,%1":"+f"(a):"f"(right));return a;}
// Moves a float's bits to a CPU register (register-pinned for the match).
static __inline__ qword Raw(float value){register qword bits __asm__("$10");__asm__("mfc1 %0,%1":"=r"(bits):"f"(value));return bits;}
// VU0 helper: a float broadcast into a VU register.
static __inline__ qword Scalar(float value)
{
    int bits;
    qword result;
    __asm__("mfc1 %0,%1" : "=r"(bits) : "f"(value));
    __asm__("qmtc2.ni %1,%0" : "=j"(result) : "r"(bits));
    return result;
}
// VU0 helper: normalized vector.
static __inline__ qword Normalize(qword input)
{
    qword norm, one, result;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z\n\tvsqrt "
            "$Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q"
            : "=&j"(norm), "=&j"(one)
            : "j"(input));
    __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q" : "=j"(result) : "j"(norm), "j"(input));
    return result;
}
// VU0 helper: s = a * s.x.
static __inline__ qword ScaleInPlace(qword a,qword s){__asm__("vmulx.xyzw %0,%1,%0x":"+j"(s):"j"(a));return s;}
// a < b (kept as a helper for the match).
static __inline__ int Below(int a,int b){return a<b;}
enum KINDENUM{KIND_NONE=-1,KIND_MAX=0x7fffffff};
// 0x176BE8: BHVGUARD vtable slot 13 (HandleMessage): message 12 (zap) picks hurt, knockdown or KO from the zap
// kind, the reaction tables and the hit points; special zaps put it to sleep (0x80000), grab it (0x200000, held),
// throw it, and so on; other messages switch modes (alert, script, ...).
// Also known as: Message (upstream readable / Sly 1 name; Message__8JOBREACTiPv).
extern "C" int HandleBhvguardMessage(BHV156 *behavior, int event, ZPR *msg)
{
    WORK w;
    int flags;
    int originalFlags;
    if (event == 12) {
        ZPR *selected = msg;
        if (((STEPGUARD *)behavior->owner)->IsZapLethal(msg)) {
            if (D_002F7900 && msg->ploSource == D_002F7900 && msg->zpk == 0x40000
                && ((MURRAY *)msg->ploSource)->fTossGadget)
                SpawnMurrayHitEffectUnchecked(D_002F7900, GetLoWorldPos(((STEPGUARD *)behavior->owner)));
            if (msg->zpk == 0x40000) {
                void *context = (*(void **)&g_sw.apstimk[3]);
                qword position = GetLoWorldPos(((STEPGUARD *)behavior->owner));
                (void)NewNoise(context, position, ((STEPGUARD *)behavior->owner));
            }
            Clear(&w.next);
            w.next.kind = GUARD_MODE_KO;
            w.next.zap = *msg;
            w.next.zap.grfzpr = REACT_KO;
            SetBhvguardMode(behavior, &w.next);
            return 1;
        }
        switch (((STEPGUARD *)behavior->owner)->specialState) {
        case 14:
            SetStepguardSpecial(((STEPGUARD *)behavior->owner), -1);
            break;
        case 8:
        case 13:
            msg->dhp <<= 2;
            break;
        }
        if (((STEPGUARD *)behavior->owner)->unknown29C)
            msg->dhp = 0;
        if (((STEPGUARD *)behavior->owner)->tracksRelocatePoint && behavior->modeRec.kind != GUARD_MODE_KO && behavior->modeRec.kind != GUARD_MODE_SCRIPT
            && (msg->zpk == ZAP_STUCK_WATCH || msg->zpk == ZAP_WATER)) {
            SetSoZapMode(((STEPGUARD *)behavior->owner), 0);
            Clear(&w.next);
            w.next.kind = GUARD_MODE_RELOCATE;
            SetBhvguardMode(behavior, &w.next);
            return 1;
        }
        switch ((KINDENUM)behavior->modeRec.kind) {
        case GUARD_MODE_HURT:
            if (!(GetSoZapResponseFlags(((STEPGUARD *)behavior->owner), msg->zpk) & 0x40)) {
                if (!(msg->zpk & ((STEPGUARD *)behavior->owner)->hurtZapMask) || (((BHVHIT *)behavior->pbhvNext)->zapKind & ((STEPGUARD *)behavior->owner)->hurtZapMask))
                    return 1;
            }
            break;
        case GUARD_MODE_KNOCKDOWN:
            if (!(msg->zpk & ((STEPGUARD *)behavior->owner)->knockdownZapMask))
                return 1;
            break;
        case GUARD_MODE_KO:
            if (!(msg->zpk & ((STEPGUARD *)behavior->owner)->koZapMask))
                return 1;
            if (((BHVHIT *)behavior->pbhvNext)->zapKind & ((STEPGUARD *)behavior->owner)->koZapMask)
                return 1;
            break;
        }
        if (((STEPGUARD *)behavior->owner)->target == msg->ploSource)
            SetTargetRecordState(((STEPGUARD *)behavior->owner)->pvEngage, 5);
        if (msg->zpk == ZAP_SLEEP_DART && (msg->grfzpr & 8)) {
            if (behavior->modeRec.kind == GUARD_MODE_SLEEP) {
                BHV175 *h = (BHV175 *)behavior->pbhvNext;
                SetBhvSleepState(h, 0);
                h->wakeTime = Max(g_clock.time + D_002BE620, h->wakeTime);
            } else {
                ((STEPGUARD *)behavior->owner)->sleepDartProgress += msg->dhp;
                if (((STEPGUARD *)behavior->owner)->sleepDartProgress <= 0) {
                    ((STEPGUARD *)behavior->owner)->sleepDartProgress = ((STEPGUARD *)behavior->owner)->sleepDartCapacity;
                    Clear(&w.next);
                    w.next.kind = GUARD_MODE_SLEEP;
                    SetBhvguardMode(behavior, &w.next);
                } else
                    ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(((STEPGUARD *)behavior->owner)->sleepDartAseg, ((STEPGUARD *)behavior->owner), 0.0f, 1.0f, 1,
                                                          &((STEPGUARD *)behavior->owner)->sleepDartAsega);
            }
        } else if (msg->zpk == ZAP_JUGGLE && (msg->grfzpr & 8)
                   && IsStepguardTargetValidEx(((STEPGUARD *)behavior->owner), msg->ploSource, GetSgfMask(6))) {
            {
                {
                    int reserve = ((STEPGUARD *)behavior->owner)->hitPoints + msg->dhp;
                    int limited = 4 < reserve ? reserve : 5;
                    ((STEPGUARD *)behavior->owner)->hitPoints = limited;
                }
                (void)NewNoise(g_sw.apstimk[2], GetLoWorldPos(((STEPGUARD *)behavior->owner)), ((STEPGUARD *)behavior->owner));
                Clear(&w.next);
                w.next.kind = GUARD_MODE_JUGGLED;
                w.next.arg.object = msg->ploSource;
                SetBhvguardMode(behavior, &w.next);
            }
        } else if (msg->zpk == 0x1000000 && (msg->grfzpr & 8)) {
            Clear(&w.next);
            if (IsStepguardTargetValid(((STEPGUARD *)behavior->owner), msg->ploSource, GetSgfMask(SGF_JUGGLED)))
                w.next.kind = GUARD_MODE_KNOCKDOWN;
            else
                w.next.kind = GUARD_MODE_HURT;
            w.next.zap = *msg;
            behavior->modeRec.kind = GUARD_MODE_NONE;
            SetBhvguardMode(behavior, &w.next);
            SetStepguardSpecial(((STEPGUARD *)behavior->owner), 8);
        } else if (msg->zpk == 0x2000000 && (msg->grfzpr & 8)) {
            Clear(&w.next);
            if (IsStepguardTargetValid(((STEPGUARD *)behavior->owner), msg->ploSource, GetSgfMask(SGF_JUGGLED)))
                w.next.kind = GUARD_MODE_KNOCKDOWN;
            else
                w.next.kind = GUARD_MODE_HURT;
            w.next.zap = *msg;
            behavior->modeRec.kind = GUARD_MODE_NONE;
            SetBhvguardMode(behavior, &w.next);
            SetStepguardSpecial(((STEPGUARD *)behavior->owner), 0x22);
        } else if ((originalFlags = msg->grfzpr) & REACT_ANY) {
            flags = originalFlags;
            Clear(&w.next);
            {
                void *context = msg->zpk == 0x40000 ? (*(void **)&g_sw.apstimk[3]) : (*(void **)&g_sw.apstimk[0]);
                qword position = GetLoWorldPos(((STEPGUARD *)behavior->owner));
                (void)NewNoise(context, position, ((STEPGUARD *)behavior->owner));
            }
            if (Below(behavior->modeRec.kind, GUARD_MODE_KO) && !Below(behavior->modeRec.kind, GUARD_MODE_SLEEP)
                && IsBasicDerivedFrom(msg->ploSource, (CID)CID_CLASS101)
                && (originalFlags & (REACT_FLINCH | REACT_HURT | REACT_KNOCKDOWN)))
                flags = originalFlags | REACT_KO;
            if (IsStepguardTargetValid(((STEPGUARD *)behavior->owner), msg->ploSource, GetSgfMask(SGF_JUGGLED)))
                flags |= REACT_KNOCKDOWN;
            if (flags & REACT_KO)
                flags &= ~(REACT_FLINCH | REACT_HURT | REACT_KNOCKDOWN);
            if (flags & (REACT_HURT | REACT_KNOCKDOWN))
                flags &= ~REACT_FLINCH;
            if (flags & REACT_KNOCKDOWN) {
                flags &= ~REACT_HURT;
                int type = Slot(((STEPGUARD *)behavior->owner), msg->zpk, KNOCKDOWN_REACTIONS)->kind;
                if (type == -1 && Slot(((STEPGUARD *)behavior->owner), msg->zpk, HURT_REACTIONS)->kind != type) {
                    flags &= ~REACT_KNOCKDOWN;
                    flags |= REACT_HURT;
                }
                if (Slot(((STEPGUARD *)behavior->owner), msg->zpk, KNOCKDOWN_REACTIONS)->kind == -1
                    && !FindBestAnimBinding(((STEPGUARD *)behavior->owner), GetSgfMask(6))) {
                    flags &= ~REACT_KNOCKDOWN;
                    flags |= REACT_HURT;
                }
            }
            if (flags & REACT_FLINCH) {
                if (((STEPGUARD *)behavior->owner)->staggerCapacity > 0) {
                    ((STEPGUARD *)behavior->owner)->staggerProgress += msg->dhp;
                    if (((STEPGUARD *)behavior->owner)->staggerProgress > 0) {
                        ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(((STEPGUARD *)behavior->owner)->staggerAseg, ((STEPGUARD *)behavior->owner), 0.0f, 1.0f, 1,
                                                              &((STEPGUARD *)behavior->owner)->heldAnimation);
                        ((STEPGUARD *)behavior->owner)->pulseStartTime = g_clock.time;
                    } else
                        flags |= REACT_HURT;
                } else
                    flags |= REACT_HURT;
            }
            if (flags & (REACT_HURT | REACT_KNOCKDOWN)) {
                ((STEPGUARD *)behavior->owner)->staggerProgress = ((STEPGUARD *)behavior->owner)->staggerCapacity;
                int delta = ((STEPGUARD *)behavior->owner)->staggerCapacity > 0 ? -((STEPGUARD *)behavior->owner)->staggerCapacity : msg->dhp;
                int reserve = ((STEPGUARD *)behavior->owner)->hitPoints + delta;
                ((STEPGUARD *)behavior->owner)->hitPoints = reserve < 0 ? 0 : reserve;
                if (((STEPGUARD *)behavior->owner)->hitPoints > 0) {
                    w.next.kind = (flags & REACT_KNOCKDOWN) ? GUARD_MODE_KNOCKDOWN : GUARD_MODE_HURT;
                    w.next.zap = *msg;
                    selected = &w.next.zap;
                } else
                    flags |= REACT_KO;
            }
            if (flags & REACT_KO) {
                w.next.kind = GUARD_MODE_KO;
                w.next.zap = *selected;
                selected = &w.next.zap;
            }
            if (flags & (REACT_HURT | REACT_KO | REACT_KNOCKDOWN)) {
                if (IsBasicDerivedFrom(selected->ploSource, (CID)CID_STEPPLAYER)) {
                    qword own = GetLoWorldPos(((STEPGUARD *)behavior->owner));
                    LO *otherObject = selected->ploSource;
                    w.own.q = own;
                    w.other.q = GetLoWorldPos(otherObject);
                    qword scalar = Raw(-150.0f);
                    w.copy.q = scalar;
                    LO *frameObject = selected->ploSource;
                    w.scalar.q = scalar;
                    MATRIX4 *frame = GetLoWorldMatrix(frameObject);
                    int special;
                    {
                        register qword axis __asm__("vf1"), origin __asm__("vf2"), delta __asm__("vf3");
                        register LO *receiver __asm__("$4");
                        register int queryKind __asm__("$5");
                        const int targetKind = 0x36;
                        const OFFSETINPUTS &inputs = *(const OFFSETINPUTS *)&w.own;
#ifdef SLY2_EE_ASM
                        __asm__("lqc2 %0,%5\n\taddiu %4,$zero,%8\n\tlqc2 %1,16+%6\n\tlqc2 %2,32+%6\n\tvadda.xyzw "
                                "$ACC,%1,$vf0\n\tvmaddx.xyzw %0,%0,%2x\n\tlw %3,%7\n\tlqc2 %1,%6\n\tvsub.xyzw %2,%1,%0"
                                : "=&j"(axis), "=&j"(origin), "=&j"(delta), "=&r"(receiver), "=&r"(queryKind)
                                : "m"(*(qword *)frame->mat[0]), "m"(inputs), "m"(selected->ploSource), "i"(targetKind));
#else
                        {   /* axis = (inputs[1] + vf0) + row 0 * inputs[2].x; delta = inputs[0] - axis */
                            const qword *in = (const qword *)&inputs;
                            axis = VuAddScaled<qword>(in[1], *(qword *)frame->mat[0], in[2]);
                            origin = in[0];
                            delta = VuSub(origin, axis);
                            receiver = selected->ploSource;
                            queryKind = targetKind;
                        }
#endif
                        w.delta.q = delta;
                        special = IsBasicDerivedFrom(receiver, (CID)queryKind);
                    }
                    qword direction = w.delta.q;
                    if (special && ((MURRAY *)selected->ploSource)->state == 12) {
                        LO *object = selected->ploSource;
                        *(qword *)&w.scalar = ConvertVecBetweenLos(0, object, direction);
                        w.scalar.f[0] *= 0.25f;
                        direction = ConvertVecBetweenLos(object, 0, w.scalar.q);
                    }
                    qword normalized = Normalize(direction);
                    *(qword *)&w.scalar = Scalar(500.0f);
                    selected->velocity = ScaleInPlace(normalized, w.scalar.q);
                } else if (((STEPGUARD *)behavior->owner)->specialState == -1 && IsBasicDerivedFrom(selected->ploSource, (CID)CID_STEPGUARD)
                           && ((STEPGUARD *)selected->ploSource)->specialState == 0x22)
                    SetStepguardSpecial(((STEPGUARD *)behavior->owner), 0x22);
                behavior->modeRec.kind = GUARD_MODE_NONE;
                SetBhvguardMode(behavior, &w.next);
            }
        }
        // every zap ends here
        if (D_002F7900 && selected->zpk == 0x20000)
            SpawnMurrayHitEffect(D_002F7900, GetLoWorldPos(((STEPGUARD *)behavior->owner)));
        return 1;
    }
    return HandleBhvMessage(behavior, event, msg);
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_mask[(sizeof(unsigned long)==8)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_U[(sizeof(QWROW)==16)?1:-1];
typedef char layout_U_q[((unsigned long)&((QWROW*)0)->q==0x0)?1:-1];
typedef char layout_U_f[((unsigned long)&((QWROW*)0)->f==0x0)?1:-1];
typedef char layout_WORK_next[((unsigned long)&((WORK*)0)->next==0x0)?1:-1];
typedef char layout_WORK_scalar[((unsigned long)&((WORK*)0)->scalar==0x70)?1:-1];

typedef char layout_WORK_size[(sizeof(WORK)==0xC0)?1:-1];

typedef char layout_OFFSETINPUTS_size[(sizeof(OFFSETINPUTS)==0x30)?1:-1];
typedef char layout_OFFSETINPUTS_own[((unsigned long)&((OFFSETINPUTS*)0)->own==0x0)?1:-1];
typedef char layout_OFFSETINPUTS_other[((unsigned long)&((OFFSETINPUTS*)0)->other==0x10)?1:-1];
typedef char layout_OFFSETINPUTS_copy[((unsigned long)&((OFFSETINPUTS*)0)->copy==0x20)?1:-1];
typedef char layout_WORK_own[((unsigned long)&((WORK*)0)->own==0x80)?1:-1];
typedef char layout_WORK_other[((unsigned long)&((WORK*)0)->other==0x90)?1:-1];
typedef char layout_WORK_copy[((unsigned long)&((WORK*)0)->copy==0xA0)?1:-1];
typedef char layout_WORK_delta[((unsigned long)&((WORK*)0)->delta==0xB0)?1:-1];
} // namespace M_BHVGUARD_FHandleMessage

// 0x177618 UpdateBhvguard
namespace M_UpdateBhvguard {

 int IsBhvguardEscortActive(BHV156*) __asm__("IsBhvguardEscortActive");
 int HasBhvguardTargetList(BHV*) __asm__("HasBhvguardTargetList");
 int IsBhvguardModeSame(const BHVGUARDRECDATA*,const BHVGUARDRECDATA*) __asm__("IsBhvguardModeSame");
 void SetBhvguardMode(BHV156*,const BHVGUARDRECDATA*) __asm__("SetBhvguardMode");
 void*func_002333B4(void*,int,unsigned int) __asm__("memset");
 int IsStepguardForceClass(STEPGUARD*,LO*) __asm__("IsStepguardForceClass");
 float GetRandomStepguardDelay(STEPGUARD*) __asm__("GetRandomStepguardDelay");
 CID147*ChooseStepguardStimulus(STEPGUARD*,CID147*) __asm__("ChooseStepguardStimulus");
 void SetBhvSearchWatched(BHV171*,CID147*) __asm__("SetBhvSearchWatched");
 int IsBhvSearchIdle(BHV171*) __asm__("IsBhvSearchIdle");
 int IsBhvScriptDone(BHV*) __asm__("IsBhvScriptDone");
 LO*EmitSwNoise(SW*,int,qword,STEPGUARD*) __asm__("EmitSwNoise");
 void RemoveNoise(LO*) __asm__("RemoveNoise");
 void InflictBhvguardZap(BHV156*,int,SO*) __asm__("InflictBhvguardZap");
 int IsBhvThrownDone(BHV*) __asm__("IsBhvThrownDone");
 int IsBhvRelocateDone(BHV*) __asm__("IsBhvRelocateDone");
 int IsBhvStalkDone(BHV*) __asm__("IsBhvStalkDone");
 void ResumeBhvguard(BHV156*) __asm__("ResumeBhvguard");

// Clears a mode record.
static inline void Clear(BHVGUARDRECDATA*s){func_002333B4(s,0,sizeof(*s));}
// Fills a mode record to engage `object`: a timed forced-class attack (kind 18, random delay) for forced classes, else plain combat (kind 3).
static inline void Attack(BHV156 *p, BHVGUARDRECDATA *s, LO *object)
{
    if (IsStepguardForceClass((STEPGUARD *)p->owner, object)) {
        s->kind = 18;
        s->arg.object = object;
        s->arg.amount = GetRandomStepguardDelay((STEPGUARD *)p->owner);
    } else {
        s->kind = 3;
        s->arg.object = object;
    }
}
typedef char qword_Size[sizeof(qword)==16?1:-1];
// 0x177618: BHVGUARD vtable slot 10 (Update): the top-level guard AI: patrol -> (stimulus) search -> (target seen)
// alert animation -> combat; returns to patrol when the child finishes; also the script, escort and target-list
// modes.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvguard(BHV156 *behavior)
{
    UpdateBhv(behavior);
    while (!IsBhvBusyAbove(behavior)) {
        BHVGUARDRECDATA previous = behavior->modeRec;
        BHVGUARDRECDATA *current = &behavior->modeRec;
        switch (behavior->modeRec.kind) {
        case GUARD_MODE_CALM:
        case GUARD_MODE_ESCORT:
        case GUARD_MODE_WATCH: {
            BHVGUARDRECDATA next;
            Clear(&next);
            if (!HasBhvguardTargetList(behavior)) {
                LO *object = ((STEPGUARD *)behavior->owner)->ChooseTarget();
                if (object)
                    Attack(behavior, &next, object);
                else {
                    CID147 *selected = ChooseStepguardStimulus((STEPGUARD *)behavior->owner, 0);
                    if (selected) {
                        next.kind = GUARD_MODE_SUSPICIOUS;
                        next.arg.object = selected;
                    } else if (behavior->modeRec.kind == GUARD_MODE_WATCH && g_clock.time < behavior->watchEndTime)
                        next.kind = GUARD_MODE_WATCH;
                    else {
                        if (IsBhvguardEscortActive(behavior))
                            next.kind = GUARD_MODE_ESCORT;
                        else
                            next.kind = GUARD_MODE_CALM;
                    }
                }
            } else
                next.kind = GUARD_MODE_TARGET_LIST;
            SetBhvguardMode(behavior, &next);
            break;
        }
        case GUARD_MODE_SUSPICIOUS: {
            if (HasBhvguardTargetList(behavior)) {
                BHVGUARDRECDATA next;
                Clear(&next);
                next.kind = GUARD_MODE_TARGET_LIST;
                SetBhvguardMode(behavior, &next);
            } else {
                LO *object = ((STEPGUARD *)behavior->owner)->ChooseTarget();
                if (object) {
                    BHVGUARDRECDATA next;
                    Clear(&next);
                    Attack(behavior, &next, object);
                    SetBhvguardMode(behavior, &next);
                } else {
                    BHV171 *h = (BHV171 *)behavior->pbhvNext;
                    CID147 *old = h->watched;
                    CID147 *selected = ChooseStepguardStimulus((STEPGUARD *)behavior->owner, old);
                    if (selected != old)
                        SetBhvSearchWatched(h, selected);
                    else if (IsBhvSearchIdle(h)) {
                        BHVGUARDRECDATA next;
                        Clear(&next);
                        next.kind = GUARD_MODE_CALM;
                        SetBhvguardMode(behavior, &next);
                    }
                }
            }
            break;
        }
        case GUARD_MODE_ALERT_ANIM: {
            if (HasBhvguardTargetList(behavior)) {
                BHVGUARDRECDATA next;
                Clear(&next);
                next.kind = GUARD_MODE_TARGET_LIST;
                SetBhvguardMode(behavior, &next);
            } else if (IsBhvanimDone((BHVANIM *)behavior->pbhvNext)) {
                BHVGUARDRECDATA next;
                Clear(&next);
                if (IsBasicDerivedFrom(behavior->owner, (CID)CID_DIMITRI)) {
                    next.kind = GUARD_MODE_FLEE;
                    next.arg.object = behavior->modeRec.arg.object;
                } else {
                    next.kind = GUARD_MODE_ALERT;
                    next.arg.object = behavior->modeRec.arg.object;
                }
                SetBhvguardMode(behavior, &next);
            }
            break;
        }
        case GUARD_MODE_SCRIPT:
            if (IsBhvScriptDone(behavior->pbhvNext))
                ResumeBhvguard(behavior);
            break;
        case GUARD_MODE_ALERT: {
            BHVCOMBAT *h = (BHVCOMBAT *)behavior->pbhvNext;
            if (HasBhvguardTargetList(behavior)) {
                BHVGUARDRECDATA next;
                Clear(&next);
                next.kind = GUARD_MODE_TARGET_LIST;
                SetBhvguardMode(behavior, &next);
            } else if (IsBhvcombatDone(h)) {
                BHVGUARDRECDATA next;
                Clear(&next);
                next.kind = GUARD_MODE_CALM;
                SetBhvguardMode(behavior, &next);
            } else {
                LO *object = ((STEPGUARD *)behavior->owner)->ChooseTarget();
                if (object != ((STEPGUARD *)behavior->owner)->target) {
                    BHVGUARDRECDATA next;
                    Clear(&next);
                    if (object)
                        Attack(behavior, &next, object);
                    else {
                        next.kind = GUARD_MODE_CALM;
                        LO *other = h->target;
                        if (other && IsBasicDerivedFrom(other, (CID)CID_STEPPLAYER) && ((STEPPLAYER *)other)->GetHealthState() == 2) {
                            STEPGUARD *owner = GetStepguardFromLo(((STEPPLAYER *)other)->jthurt.zpr.ploSource);
                            if (owner && owner != (STEPGUARD *)behavior->owner) {
                                next.arg.object = owner;
                                next.kind = GUARD_MODE_WATCH;
                            }
                        }
                    }
                    SetBhvguardMode(behavior, &next);
                }
            }
            break;
        }
        case GUARD_MODE_TARGET_LIST: {
            if (!HasBhvguardTargetList(behavior)) {
                BHVGUARDRECDATA next;
                Clear(&next);
                next.kind = GUARD_MODE_CALM;
                SetBhvguardMode(behavior, &next);
            }
            break;
        }
        case GUARD_MODE_START: {
            BHVGUARDRECDATA next;
            Clear(&next);
            next.kind = GUARD_MODE_CALM;
            SetBhvguardMode(behavior, &next);
            break;
        }
        case GUARD_MODE_SLEEP: {
            if (IsBhvanimDone((BHVANIM *)behavior->pbhvNext)) {
                LO *object = EmitSwNoise(&g_sw, 0, GetLoWorldPos(behavior->owner), (STEPGUARD *)behavior->owner);
                BHVGUARDRECDATA next;
                Clear(&next);
                next.kind = GUARD_MODE_SUSPICIOUS;
                next.arg.object = object;
                SetBhvguardMode(behavior, &next);
                RemoveNoise(object);
            }
            break;
        }
        case GUARD_MODE_JUGGLED: {
            BHVJUGGLED *h = (BHVJUGGLED *)behavior->pbhvNext;
            if (IsBhvanimDone(h)) {
                if (h->juggleState != -1) {
                    if (h->juggleState == 3)
                        ResumeBhvguard(behavior);
                } else {
                    InflictBhvguardZap(behavior, -50, h->holder);
                    if (behavior->modeRec.kind == GUARD_MODE_JUGGLED)
                        ResumeBhvguard(behavior);
                }
            }
            break;
        }
        case GUARD_MODE_THROWN:
            if (IsBhvThrownDone(behavior->pbhvNext))
                ResumeBhvguard(behavior);
            break;
        case GUARD_MODE_HURT:
            if (IsBhvanimDone((BHVANIM *)behavior->pbhvNext))
                ResumeBhvguard(behavior);
            break;
        case GUARD_MODE_KNOCKDOWN:
            if (IsBhvanimDone((BHVANIM *)behavior->pbhvNext))
                ResumeBhvguard(behavior);
            break;
        case GUARD_MODE_RELOCATE:
            if (IsBhvRelocateDone(behavior->pbhvNext))
                ResumeBhvguard(behavior);
            break;
        case GUARD_MODE_STALK:
            if (IsBhvStalkDone(behavior->pbhvNext))
                ResumeBhvguard(behavior);
            break;
        }
        if (IsBhvguardModeSame(current, &previous))
            break;
    }
}
} // namespace M_UpdateBhvguard

// 0x177BA0 ResumeBhvguard
namespace M_ResumeBhvguard {
extern float D_002A54D0 __asm__("D_002A54D0");
int IsBasicDerivedFrom__FP5BASIC3CID(STEPGUARD *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void *GetHeroPo(SW *g) __asm__("GetHeroPo");
int HasBhvguardTargetList(BHVGUARD *p) __asm__("HasBhvguardTargetList");
int ShouldBhvguardUseNeelaBehavior(BHV *p) __asm__("ShouldBhvguardUseNeelaBehavior");
int IsStepguardForceClass(STEPGUARD *p, void *q) __asm__("IsStepguardForceClass");
float GetRandomStepguardDelay(STEPGUARD *p) __asm__("GetRandomStepguardDelay");
qword GetLoWorldPos(STEPGUARD *p) __asm__("GetLoWorldPos");
void *EmitSwNoise(SW *g, int n, qword v, STEPGUARD *p) __asm__("EmitSwNoise");
void SetBhvguardMode(BHVGUARD *p, BHVGUARDREC *f) __asm__("SetBhvguardMode");
// 0x177BA0: Picks the mode to resume after an interrupting mode ends: flee (Dimitri), stalk the hero (special
// 0x15), the target list, Neela's chase, combat or stalk on the current target, investigate its own position after
// being hit, else patrol.
// Also known as: NextReaction (upstream readable / Sly 1 name).
extern "C" void ResumeBhvguard(BHVGUARD *behavior) {
    BHVGUARDREC f;   /* cleared by its constructor (memset) */
    if (IsBasicDerivedFrom__FP5BASIC3CID((STEPGUARD *)behavior->owner, CID_DIMITRI) && ((DIMITRI *)behavior->owner)->routeCount > 0) {
        f.kind = GUARD_MODE_FLEE;
        f.arg.object = GetHeroPo(&g_sw);
    } else if (((STEPGUARD *)behavior->owner)->specialState == 0x15) {
        f.kind = GUARD_MODE_STALK;
        f.arg.object = GetHeroPo(&g_sw);
        f.arg.amount = D_002A54D0;
    } else if (HasBhvguardTargetList(behavior)) {
        f.kind = GUARD_MODE_TARGET_LIST;
    } else if (ShouldBhvguardUseNeelaBehavior(behavior)) {
        f.kind = GUARD_MODE_NEELA_CHASE;
    } else {
        void *q;
        q = ((STEPGUARD *)behavior->owner)->ChooseTarget();
        if (q) {
            if (IsStepguardForceClass(((STEPGUARD *)behavior->owner), q)) {
                f.kind = GUARD_MODE_STALK;
                f.arg.object = q;
                f.arg.amount = GetRandomStepguardDelay(((STEPGUARD *)behavior->owner));
            } else {
                f.kind = GUARD_MODE_ALERT;
                f.arg.object = q;
            }
        } else {
            int n = behavior->mode.kind;
            if (n < 11) {
                if (n >= 6) {
                    f.kind = GUARD_MODE_SUSPICIOUS;
                    f.arg.object = EmitSwNoise(&g_sw, 0, GetLoWorldPos(((STEPGUARD *)behavior->owner)), ((STEPGUARD *)behavior->owner));
                } else {
                    f.kind = GUARD_MODE_CALM;
                }
            } else {
                f.kind = GUARD_MODE_CALM;
            }
        }
    }
    SetBhvguardMode(behavior, &f);
}
} // namespace M_ResumeBhvguard

// 0x177D20 HasBhvguardTargetList
// 0x177D20: True if the guard has designated targets or avoid objects.
extern "C" int HasBhvguardTargetList(BHV *behavior)
{
    STEPGUARD *guard;
    int f;
    guard = (STEPGUARD *)behavior->owner;
    f = 0;
    if ((guard->avoidObjectCount != 0) || (guard->designatedTargetCount != 0)) {
        f = 1;
    }
    return f;
}

// 0x177D48 IsBhvguardEscortActive
// 0x177D48: True if the guard's escort object exists and is in the world.
extern "C" int IsBhvguardEscortActive(BHVGUARD *behavior)
{
    LO *t = ((STEPGUARD *)behavior->owner)->escortObject;
    int f = 0;
    if (t)
        f = ((int)((LO *)(t))->isInWorld & 1);
    return f;
}

// 0x177D70 ShouldBhvguardUseNeelaBehavior
// 0x177D70: True if the guard is Neela (cid 0x3C) and her +0x108C option is set (inferred: use her chase
// behaviour).
extern "C" int ShouldBhvguardUseNeelaBehavior(BHV *behavior)
{
    int f;
    f = 0;
    if (IsBasicDerivedFrom(behavior->owner, (CID)0x3C) != 0) {
        f = ((NEELA *)behavior->owner)->chaseScript != 0;
    }
    return f;
}

namespace RO_ORDER { extern const float r_D_002A54D0 __asm__("D_002A54D0") = 3.40282347e+38f; }
// 0x177DC0 IsBhvguardModeSame
// 0x177DC0: True if two mode records are the same (kind plus the kind's key field: the zap kind of the zap modes,
// else the mode's object).
// Also known as: FSameMsg (upstream readable / Sly 1 name; FSameMsg__FP5AIMSGT0).
extern "C" int IsBhvguardModeSame(BHVGUARDRECDATA *a, BHVGUARDRECDATA *b) {
    if (a->kind != b->kind) return 0;
    switch (a->kind) {
    case 9: case 10: case 11: return a->zap.zpk == b->zap.zpk;
    case 4: return a->arg.object == b->arg.object;
    case 5: return a->arg.object == b->arg.object;
    case 3: return a->arg.object == b->arg.object;
    case 13: return a->arg.object == b->arg.object;
    case 2: return a->arg.object == b->arg.object;
    case 14: return a->arg.object == b->arg.object;
    case 18: return a->arg.object == b->arg.object;
    case 6: return a->arg.object == b->arg.object;
    }
    return 1;
}

// 0x177E80 InitBhvpath
namespace M_InitBhvpath {
extern float D_002BE624 __asm__("D_002BE624"), D_002BE628 __asm__("D_002BE628"), D_002BE62C __asm__("D_002BE62C");
} // namespace M_InitBhvpath
// 0x177E80: BHVPATH vtable slot 7 (OnPush): no path, default speeds and radii, state -1.
void BHVPATH::OnPush()
{
    using namespace M_InitBhvpath;
    BHVPATH *p = this;
    InitBhv(p);
    p->pathParam = p->startParamHint = -1.0f;
    p->arrivalRadius = D_002BE62C;
    p->nearRadius = D_002BE624;
    p->farRadius = D_002BE628;
    p->state = -1;
    p->isBlocked = 0;
    p->isInitialized = 1;
}

// 0x177EF0 StartBhvpath
namespace M_StartBhvpath {

extern float D_002BE624 __asm__("D_002BE624"), D_002BE628 __asm__("D_002BE628"), D_002BE62C __asm__("D_002BE62C");
void StartBhv(BHVPATH *p) __asm__("StartBhv");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
// The larger of two values.
// The smaller of two values.
// 0x177EF0: BHVPATH vtable slot 8 (Start): remembers the start position, clamps the radii from the speed, state 0.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvpath(BHVPATH *behavior) {
    float g;
    StartBhv(behavior);
    behavior->startPosition = GetLoWorldPos(behavior->owner);
    g = Max(behavior->arrivalRadius, D_002BE62C);
    behavior->arrivalRadius = g;
    behavior->nearRadius = Min(g * 0.25f, D_002BE624);
    behavior->farRadius = Min(g, D_002BE628);
    behavior->SetPathState(0);
}
} // namespace M_StartBhvpath

// 0x177F78 StepBhvpathAlongShape
namespace M_StepBhvpathAlongShape {
union VEC; struct CLQ; struct LM; struct VU_FLOAT; struct VUF; struct VV; struct VU_VECTOR;
// StepBhvpathAlongShape (Claude agent, T1690, 2026-10-25): MATCH. Curve follower step: closest point on the curve(s), steering
// velocity toward the next point, avoidance of other bodies (loop), integration and collision clamp.
// Built on lilly15 (aligned 11, T1686). What finished it (each measured, see notes/170000.md):
//  - accel = VecCombo(VU_FLOAT(&D_002BE658), vGoal - p->v, VU_FLOAT(&D_002BE65C), next - p->pos): the game's own
//    VecCombo (0x258480, matched as src/game/gameplay/ai/tu_1553F0/VecCombo__FG8VU_FLOATG9VU_VECTORT0T1.cpp) inlined with by-value params and
//    the weight evaluated first. No "memory" clobber is needed; the old QwBlendF(dv, w1, dp, w2) order could not give
//    the target lqc2 p->v / lui / lw / vsub / qmtc2 / lqc2 p->pos / sqc2 / lui schedule.
//  - dpos = VuSubtract(posActor, posOther) (plain, not the VSub VV wrapper): the posActor/pctx spill slots 0x90/0xA0.
//  - dirNow length through VuLengthAlt (the vmul.xyz reads its input as two "j" operands, allocation only): the extra
//    refs give the VLength copy a higher global.c priority than the projected dpos, so the copy takes vf5 and dpos vf7.
// Earlier keys (lilly15):
//  - FindShapeClosestPointFromDistance takes the float right after the position (pcrv, pos, s, pv, ...): arg-move order f12 before a2.
//  - Top and dirNow normalizes as `NormalizeSafeN(VV v, VV fallback)` / `DivSafe(v, len, fallback)` (return style,
//    by-value VV fallback): the fallback lq/qmtc2 lands in the branch delay slots with the target registers.
//  - "rV" on the VU_FLOAT(const float *) qmtc2 input: the lw reuses the dying lui register (reload-made load).
//  - The two `1.0f - x` sites share ONE float g (the target keeps both results in f5); site 1 is written
//    `gFar = 1.0f; gFar -= x; g = gFar;` so loop.c does not combine and hoist the two 1.0f loads.
//  - Loop helpers take/return the VV class by value (VSub, VAddScaled, VProjectOut, VCross, VDot, VLength): the extra
//    16-byte copies raise the loop to ~339 RTL insns, so loop.c no longer hoists hi(D_002BE664); that drops a0 from the
//    reload spill set and the later reload registers (a2/v0/v1) match.
//  - plain-C MaxFloat (`a < b ? b : a`) instead of a max.s asm; qmfc2 asm for the GPR copy of vGoal (fInit store).
// One register pin: qwActor (s2) - the target copies the spilled posActor into s2 with a plain lq in a delay slot,
// which an asm copy cannot give (reorg stops at asms) and a plain copy is propagated away (without it: 285 differ).
union VEC
{
    qword qw;
    float f[4];
};
struct CLQ
{
    float g0, g1, g2;
};
struct LM
{
    float minValue, maxValue;
};

extern char D_003DD020[] __asm__("D_003DD020");
extern qword D_002A2DA0 __asm__("D_002A2DA0"), g_normalZ __asm__("g_normalZ"), D_002A2DD0 __asm__("D_002A2DD0");
extern float D_002BE630 __asm__("D_002BE630"), D_002BE634 __asm__("D_002BE634"), D_002BE658 __asm__("D_002BE658"), D_002BE65C __asm__("D_002BE65C"), D_002BE660 __asm__("D_002BE660"), D_002BE664 __asm__("D_002BE664"), D_002BE668 __asm__("D_002BE668"), D_002BE66C __asm__("D_002BE66C"), D_002BE670 __asm__("D_002BE670"), D_002BE674 __asm__("D_002BE674"), D_002BE690 __asm__("D_002BE690");
extern CLQ D_002BE640 __asm__("D_002BE640"), D_002BE680 __asm__("D_002BE680");
extern LM D_002BE650 __asm__("D_002BE650"), g_lmZeroOne __asm__("g_lmZeroOne");
qword GetLoWorldPos(void *pobj) __asm__("GetLoWorldPos");
float WrapShapeDistance(SHAPE *pcrv, float s) __asm__("WrapShapeDistance");
float GetShapeMaxDistance(SHAPE *pcrv) __asm__("GetShapeMaxDistance");
void FindShapeClosestPointFromDistance(SHAPE *pcrv, qword pos, float s, void *pv, VEC *ppos, VEC *pdir, float *pg, float *ps) __asm__("FindShapeClosestPointFromDistance");
void EvaluateShapeAtDistance(SHAPE *pcrv, float s, VEC *ppos, VEC *pdir) __asm__("EvaluateShapeAtDistance");

void FindClosestPointOnLineSegment(qword a, qword b, qword c, VEC *pv, float *pg0, float *pg1) __asm__("FindClosestPointOnLineSegment");
int FindPathzoneClosestFreePoint(PATHZONE *pctx, qword pos, VEC *pv) __asm__("FindPathzoneClosestFreePoint");

struct VU_FLOAT
{
    qword qw;
    // Broadcast a float into a VU0 register.
    VU_FLOAT(float g) { __asm__("qmtc2.ni %1, %0" : "=j"(qw) : "r"(g)); }
    // Broadcast the float at pg (read from memory) into a VU0 register.
    VU_FLOAT(const float *pg) { __asm__("qmtc2.ni %1, %0" : "=j"(qw) : "rV"(*(const int *)pg)); }

};
// VU0 helper: x component of a quadword vector.
// VU0 helper: x component of a quadword vector (VU register operand variant for the match).
static inline float VuGetXKeepInVf(qword qw)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(qw), "j"(qw)); /* allocation-only "j"(qw): the vector stays in a VF register */
    return x;
}
// VU0 helper: x component of a quadword vector (operand order variant for the match).
static inline float VuGetXClobberA0A1(qword qw)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(qw), "j"(qw) : "$4", "$5"); /* allocation-only "j"(qw); clobbers of the dead a0/a1 */
    return x;
}
struct VUF
{
    qword qw;
    // Broadcast the float at pg (read from memory) into a VU0 register.
    VUF(const float *pg) { __asm__("qmtc2.ni %1, %0" : "=j"(qw) : "rV"(*(const int *)pg)); }
};
// VU0 helper: vector difference.
// VU0 helper: dot product (in x).
static inline qword VuDotProduct(const qword &a, const qword &b)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(a), "j"(b));
    return qw;
}
// VU0 helper: v minus its projection on n.
static inline qword VuProjectOut(qword v, qword n)
{
    qword d = VuDotProduct(v, n);
    qword al, ac;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(al) : "j"(d), "j"(n));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(ac) : "j"(v), "j"(al));
    return ac;
}
// VU0 helper: length of a vector (in x).
static inline qword VuLength(qword v)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(v));
    qword r;
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(qw));
    return r;
}
/* length of v; copy receives v itself (allocation-only tied output, never written); the "r"(v) operands load v through a GPR */
#ifdef SLY2_EE_ASM
#define QW_LENGTH_COPY(len, copy, v) \
    { \
        qword qwSq_, vfOne_; \
        __asm__("vmul.xyz %0, %3, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" \
                : "=j"(qwSq_), "=j"(vfOne_), "=j"(copy) : "2"(v), "r"(v), "r"(v)); \
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(len) : "j"(qwSq_)); \
    }
#else
#define QW_LENGTH_COPY(len, copy, v) { (copy) = (v); (len) = ::VuLength(copy); }
#endif
// VU0 helper: horizontal (xy) length of a vector.
// VU0 helper: v divided by a given length.
// Same idiom as sly2/vu0.h VuDivByX; kept local because its callers pass mixed operand types (the template wants one).
static inline qword VuDivByX(qword v, qword qwLen)
{
    qword qw;
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(qw) : "j"(qwLen), "j"(v));
    return qw;
}
struct VV
{
    qword q;
    // Uninitialised vector.
    VV() {}
    // Wrap a qword.
    VV(const qword &v) { q = v; }
    // The wrapped qword.
    operator qword() const { return q; }
};
// VU0 helper: normalized v, or `fallback` for a zero vector.
static inline VV NormalizeSafeN(VV v, VV fallback)
{
    qword len = VuLength(v);
    if (QwordX(len) < 0.0001f)
        return fallback;
    return VuDivByX(v, len);
}
// VU0 helper: v / len, or `fallback` when the length is zero.
static inline VV DivSafe(VV v, VV len, VV fallback)
{
    if (QwordX(len) < 0.0001f)
        return fallback;
    return VuDivByX(v, len);
}
// VU0 helper: vector scaled by the x of a VU float.
static inline qword VuScaleByX(qword v, const VU_FLOAT &g)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(v), "j"(g.qw));
    return qw;
}
// VU0 helper: base + vector * scale.
static inline qword VuMulAdd(qword a, qword b, const VU_FLOAT &g)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(a), "j"(b), "j"(g.qw));
    return qw;
}
// VU0 helper: one integration step pos + v * g1 + a * g2.
static inline qword VuIntegrateStep(qword pos, qword v, const VU_FLOAT &g1, qword a, const VU_FLOAT &g2)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %2, %3x\n\tvmaddx.xyzw %0, %4, %5x"
            : "=j"(qw) : "j"(pos), "j"(v), "j"(g1.qw), "j"(a), "j"(g2.qw));
    return qw;
}
// VU0 helper: cross product.
// VU0 helper: z component of a quadword vector.
// Evaluates a CLQ curve at x clamped to an LM range.
static inline float EvaluateClampedQuadratic(CLQ *pclq, LM *plm, float x)
{
    float g = pclq->g0 + x * (pclq->g1 + x * pclq->g2);
    float gMin = plm->minValue;
    float gMax = plm->maxValue;
    if (g < gMin)
        g = gMin;
    else if (gMax < g)
        g = gMax;
    return g;
}
// Clamps g to [gMin, gMax].
// The larger of two floats.
// Returns an object's velocity through its vtable.
static inline qword GetObjectVelocity(void *pv)
{
    LO *pobj = (LO *)pv;
    return pobj->GetVelocity();
}

// VU0 helper: vector difference.
static inline VV VSub(VV a, VV b) { return VuSub(a, b); }
// VU0 helper: a + b * s.
static inline VV VAddScaled(VV a, VV b, const VU_FLOAT &g) { return VuMulAdd(a, b, g); }
// VU0 helper: v minus its projection on n.
static inline VV VProjectOut(VV v, VV n) { return VuProjectOut(v, n); }
// VU0 helper: cross product.
static inline VV VCross(VV a, VV b) { return VuCross(a, b); }
// VU0 helper: dot product.
static inline VV VDot(VV a, VV b) { return VuDotProduct(a.q, b.q); }
// VU0 helper: length of a vector.
static inline VV VLength(VV v) { return VuLength(v); }
// VU0 helper: length of a vector (second instruction pattern for the match).
static inline qword VuLengthAlt(qword v)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(v), "j"(v));
    qword r;
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(qw));
    return r;
}
// VU0 helper: length of a vector (alternate pattern).
static inline VV VLength2(VV v) { return VuLengthAlt(v); }
struct VU_VECTOR
{
    qword q;
    // Wrap a qword as a VU_VECTOR.
    VU_VECTOR(const qword &v) { q = v; }
};
// VU0 helper: weighted sum v1 * f1 + v2 * f2.
static inline qword VecCombo(VUF f1, VU_VECTOR v1, VUF f2, VU_VECTOR v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.qw), "j"(v1.q), "j"(f2.qw), "j"(v2.q));
    return r;
}
// 0x177F78: Path follower step: finds the closest point on the path shape, steers toward the next point, avoids
// other bodies, integrates and clamps against collision.
extern "C" void StepBhvpathAlongShape(BHVPATH *behavior)
{
    VEC point, dir, next, nextDir;
    const qword *pqwZero;
    qword posActor = GetLoWorldPos(behavior->owner);
    register qword qwActor asm("$18") = posActor; /* the one register pin: the target reloads the spilled posActor into s2 (lq in a delay slot) */

    REGENT *pbody = 0;
    if (behavior->owner->physics)
        pbody = behavior->owner->physics->pregent;
    PATHZONE *pctx = 0;
    if (pbody)
        pctx = (PATHZONE *)pbody->pathzone;
    int fReverse = behavior->isReversed;
    float s;
    if (fReverse)
        s = behavior->pathParam - D_002BE634;
    else
        s = behavior->pathParam + D_002BE634;
    float sCurve = WrapShapeDistance(behavior->shape, s);
    FindShapeClosestPointFromDistance(behavior->shape, qwActor, sCurve, D_003DD020, &point, &dir, 0, &behavior->pathParam);
    if (behavior->state == 1 || behavior->state == 4)
    {
        fReverse = behavior->isBlocked != 2;
        if (fReverse)
            s = behavior->blockerParam - D_002BE634;
        else
            s = behavior->blockerParam + D_002BE634;
        float sCurve = WrapShapeDistance(behavior->blocker, s);
        FindShapeClosestPointFromDistance(behavior->blocker, qwActor, sCurve, D_003DD020, &point, &dir, 0, &behavior->blockerParam);
    }
    dir.qw = NormalizeSafeN(VuProjectOut(dir.qw, g_normalZ), D_002A2DA0);
    behavior->separation = QwordX(VuLengthXY(VuSub(point.qw, posActor)));
    float ds = EvaluateClampedQuadratic(&D_002BE640, &D_002BE650, behavior->separation);
    if (fReverse)
        ds = -ds;
    float gSpeed;
    if (IsBhvpathAtEnd(behavior))
    {
        gSpeed = 0.0f;
        nextDir.qw = D_002A2DA0;
        next.qw = posActor;
    }
    else
    {
        gSpeed = D_002BE630;
        switch (behavior->state)
        {
        case 0:
        case 2:
        case 3:
            if (behavior->shape == behavior->owner->routeShape)
            {
                float sMin = -behavior->pathParam;
                float sMax = GetShapeMaxDistance(behavior->shape) - behavior->pathParam;
                float dsClamp = Clamp(ds, sMin, sMax);
                if (ds != dsClamp)
                    gSpeed = 0.0f;
                next.qw = VuMulAdd(point.qw, dir.qw, VU_FLOAT(dsClamp));
                nextDir.qw = dir.qw;
            }
            else
            {
                float sNext;
                if (behavior->shape->isClosed)
                {
                    sNext = WrapShapeDistance(behavior->shape, behavior->pathParam + ds);
                }
                else
                {
                    float sMin = -behavior->pathParam;
                    float sMax = GetShapeMaxDistance(behavior->shape) - behavior->pathParam;
                    float dsClamp = Clamp(ds, sMin, sMax);
                    if (ds != dsClamp)
                        gSpeed = 0.0f;
                    sNext = behavior->pathParam + dsClamp;
                }
                EvaluateShapeAtDistance(behavior->shape, sNext, &next, &nextDir);
            }
            break;
        case 1:
        case 4:
            next.qw = VuMulAdd(point.qw, dir.qw, VU_FLOAT(ds));
            nextDir.qw = dir.qw;
            break;
        }
    }
    if (fReverse)
        nextDir.qw = VuScaleByX(nextDir.qw, VU_FLOAT(-1.0f));
    qword vGoal = VuScaleByX(nextDir.qw, VU_FLOAT(gSpeed));
    qword vGoalCopy;
    __asm__("qmfc2.ni %0, %1" : "=r"(vGoalCopy) : "j"(vGoal)); /* GPR copy of vGoal */

    if (behavior->isInitialized)
    {
        behavior->velocity = vGoalCopy;
        behavior->startPosition = next.qw;
        behavior->isInitialized = 0;
    }
    qword accel = VecCombo(VUF(&D_002BE658), VuSub(vGoal, behavior->velocity), VUF(&D_002BE65C), VuSub(next.qw, behavior->startPosition));
    if (!behavior->ignoresBlockers)
    {
        REGENT *pbodySelf = behavior->owner->physics->pregent;
        for (REGENT *pbodyOther = (REGENT *)pctx->dlRegent.head; pbodyOther; pbodyOther = (REGENT *)pbodyOther->dlePathzone.next)
        {
            if (pbodyOther->radius == 0.0f)
                continue;
            if (pbodyOther->mass == 0.0f)
                continue;
            void *pobj = pbodyOther->pso;
            if (pobj == (void *)behavior->owner)
                continue;
            VEC posOther;
            VEC vecUnused;
            posOther.qw = GetLoWorldPos(pobj);
            posOther.f[2] += pbodyOther->heightOffset;
            qword dpos = VuSub(posActor, posOther.qw);
            if (__builtin_fabsf(QwordZ(dpos)) > D_002BE660)
                continue;
            qword vOther = GetObjectVelocity(pobj);
            qword posPredict = VAddScaled(posOther.qw, vOther, VU_FLOAT(&D_002BE664));
            dpos = VProjectOut(dpos, g_normalZ);

            qword dposLater = VProjectOut(VSub(behavior->startPosition, posPredict), g_normalZ);

            VEC normal;
            FindClosestPointOnLineSegment(D_002A2DD0, dpos, dposLater, &normal, 0, 0);
            const qword *pqwZeroHere = &D_002A2DA0;
            qword normalCopy;
            qword qwLen;
            QW_LENGTH_COPY(qwLen, normalCopy, normal.qw);
            if (QwordX(qwLen) < 0.0001f)
                normal.qw = *pqwZeroHere;
            else
                normal.qw = VuDivByX(normalCopy, qwLen);
            float sGap = VuGetXKeepInVf(qwLen);
            sGap = StdMax(0.0f, sGap - pbodyOther->radius - pbodySelf->radius);
            float gMass = pbodyOther->mass / (pbodyOther->mass + pbodySelf->mass);
            float g;
            if (sGap < D_002BE668)
            {
                float gFar = 1.0f;
                gFar -= sGap / D_002BE668;
                g = gFar;

                accel = VAddScaled(accel, normal.qw, VU_FLOAT(gMass * g * D_002BE66C));
            }
            if (sGap < D_002BE670)
            {
                g = 1.0f - sGap / D_002BE670;

                pqwZero = &D_002A2DA0;
                qword dirNow = DivSafe(dpos, VLength2(dpos), *pqwZero);

                g *= EvaluateClampedQuadratic(&D_002BE680, &g_lmZeroOne, QwordX(VDot(dirNow, behavior->owner->aqwTarget[0])));
                if (g == 0.0f)
                    continue;
                VEC side;
                side.qw = VCross(g_normalZ, nextDir.qw);
                if (behavior->shape != behavior->owner->routeShape && QwordX(VDot(normal.qw, side.qw)) < 0.0f)
                    g = -g;
                accel = VAddScaled(accel, VCross(normal.qw, g_normalZ), VU_FLOAT(gMass * g * D_002BE674));
            }
        }
    }
    float half = 0.5f;
    float dt = g_clock.deltaTime;
    accel = VuProjectOut(accel, g_normalZ);
    qword posNext = VuIntegrateStep(behavior->startPosition, behavior->velocity, VU_FLOAT(dt), accel, VU_FLOAT(dt * (dt * half)));
    qword vNew = VuMulAdd(behavior->velocity, accel, VU_FLOAT(dt));
    qword vNext = vNew;
    if (!behavior->ignoresBlockers)
    {
        VEC posHit;
        VEC vecUnused2;
        if (FindPathzoneClosestFreePoint(pctx, posNext, &posHit))
        {
            VEC normal;
            pqwZero = &D_002A2DA0;
            normal.qw = VuProjectOut(VuSub(posHit.qw, posNext), g_normalZ);
            qword qwLen = VuLength(normal.qw);
            if (QwordX(qwLen) < 0.0001f)
                normal.qw = *pqwZero;
            else
                normal.qw = VuDivByX(normal.qw, qwLen);
            if (D_002BE690 < VuGetXClobberA0A1(qwLen))
            {
                float gClose = VuGetXKeepInVf(VuDotProduct(vNew, normal.qw));
                posNext = posHit.qw;
                if (gClose < 0.0f)
                    vNext = VuMulAdd(vNew, normal.qw, VU_FLOAT(-gClose));
            }
        }
    }
    behavior->startPosition = posNext;
    behavior->velocity = vNext;
}
} // namespace M_StepBhvpathAlongShape

// 0x1789F8 UpdateBhvpath
namespace M_UpdateBhvpath {

struct G;
extern struct G D_003DD020 __asm__("D_003DD020");


void StepBhvpathAlongShape(BHVPATH *p) __asm__("StepBhvpathAlongShape");
int FindShapeFreePoint(qword q, void *a1, void *a2, void *obj, int dir, CID14 **pp, float *pf, float g) __asm__("FindShapeFreePoint");
void FindShapeClosestPoint(CID14 *po, qword q, struct G *pg, void *pv, qword *b, float *c, float *pf) __asm__("FindShapeClosestPoint");
float GetShapeDistanceBetween(CID14 *p, float a, float b) __asm__("GetShapeDistanceBetween");
// VU0 helper: distance between two points.
extern "C" { static inline float Dist(qword a, qword b) {
    qword d;
    float f;
    __asm__("vsub.xyzw %0, %2, %1" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
} }
// 0x1789F8: BHVPATH vtable slot 10 (Update): follows the path; detects blockers on the shape and switches between
// move, wait and end (3) states.
extern "C" void UpdateBhvpath(BHVPATH *behavior) {
    float g;
    qword pos;
    int s;
    UpdateBhv(behavior);
    StepBhvpathAlongShape(behavior);
    pos = GetLoWorldPos(behavior->owner);
    do {
        s = behavior->state;
        switch (s) {
        case 0:
        case 2:
        case 3:
            if (!behavior->ignoresBlockers) {
                SOPHYS *x = behavior->owner->physics;
                REGENT *y = x ? x->pregent : 0;
                int n = FindShapeFreePoint(pos, y ? y->pathzone : 0, x->pregent->unknown20, behavior->shape, behavior->isReversed == 0, &behavior->blocker, &behavior->blockerRange, behavior->pathParam);
                behavior->isBlocked = n;
                switch (n) {
                case 0:
                    break;
                case 3:
                case 4:
                    behavior->SetPathState(3);
                    break;
                case 1:
                case 2: {
                    FindShapeClosestPoint(behavior->blocker, pos, &D_003DD020, 0, 0, 0, &g);
                    if (80.0f < __builtin_fabsf(GetShapeDistanceBetween(behavior->blocker, g, behavior->blockerRange))) {
                        behavior->blockerParam = g;
                        behavior->SetPathState(1);
                    } else {
                        behavior->isBlocked = 0;
                    }
                    break;
                }
                }
            }
            if (behavior->isBlocked == 0) {
                float r = behavior->state == 0 ? behavior->nearRadius : behavior->farRadius;
                int f = r < Dist(behavior->startPosition, pos);
                if (f) behavior->SetPathState(0);
                else behavior->SetPathState(2);
            }
            break;
        case 1:
        case 4:
            if (((uchar)behavior->blocker->isInWorld == 0) || behavior->blocker->owner) {
                behavior->SetPathState(0);
            } else if (__builtin_fabsf(GetShapeDistanceBetween(behavior->blocker, behavior->blockerParam, behavior->blockerRange)) < 80.0f) {
                behavior->SetPathState(0);
            } else {
                float r = behavior->state == 1 ? behavior->nearRadius : behavior->farRadius;
                int f = r < Dist(behavior->startPosition, pos);
                if (f) behavior->SetPathState(1);
                else behavior->SetPathState(4);
            }
            break;
        }
    } while (behavior->state != s);
}
} // namespace M_UpdateBhvpath

// 0x178CD8 SetBhvpathState
// 0x178CD8: BHVPATH state change: stores the state and its time.
extern "C" void SetBhvpathState(BHVPATH *behavior, int nState)
{
    if (nState != behavior->state) {
        behavior->state = nState;
        behavior->stateTime = g_clock.time;
    }
}

// 0x178CF8: dead-code scrap, a nop unit (sly2/scrap.h)
SLY2_SCRAP("nop");

// 0x178D00 IsBhvpathAtEnd
namespace M_FBhvpathAtEnd {

qword GetLoWorldPos(void *) __asm__("GetLoWorldPos");
// VU0 helper: distance between two points.
extern "C" { static inline float Dist(qword a, qword b) {
    qword d;
    float f;
    __asm__("vsub.xyzw %0, %2, %1" : "=&j"(d) : "j"(b), "j"(a));
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
} }
// 0x178D00: True if the path end is defined and the guard is within the arrival radius of the end point.
// Also known as: FArrived (upstream readable / Sly 1 name).
extern "C" int IsBhvpathAtEnd(BHVPATH *behavior)
{
    if (!behavior->hasEnd)
        return 0;
    return Dist(GetLoWorldPos(behavior->owner), behavior->endPoint) <= behavior->arrivalRadius;
}
} // namespace M_FBhvpathAtEnd

// 0x178D80 IsBhvpathFinished
// 0x178D80: True if the path behaviour reached the end (state 3).
extern "C" int IsBhvpathFinished(BHVPATH *behavior)
{
    return behavior->state == 3;
}

// 0x178D90 SetBhvpathShape
namespace M_SetBhvpathShape {
struct G;
extern struct G D_003DD020 __asm__("D_003DD020");
void FindShapeClosestPointFromDistance(SHAPE *shape, float g, qword q, struct G *pg, void *pv, qword *b, float *c, float *pf) __asm__("FindShapeClosestPointFromDistance");
void FindShapeClosestPoint(SHAPE *shape, qword q, struct G *pg, void *pv, qword *b, float *c, float *pf) __asm__("FindShapeClosestPoint");
float GetShapeMinDistance(SHAPE *shape) __asm__("GetShapeMinDistance");
float GetShapeMaxDistance(SHAPE *shape) __asm__("GetShapeMaxDistance");
void EvaluateShapeAtDistance(SHAPE *shape, float g, qword *pv, int n) __asm__("EvaluateShapeAtDistance");
} // namespace M_SetBhvpathShape
// 0x178D90: Sets the shape to follow and the direction: finds the start parameter (closest point) and, for open
// shapes, the end point.
// Also known as: Follow (upstream readable / Sly 1 name; Follow__9JOBFOLLOWP3CRVi).
extern "C" void SetBhvpathShape(BHVPATH *behavior, SHAPE *shape, int n) {
    using namespace M_SetBhvpathShape;
    behavior->isReversed = n;
    behavior->shape = shape;
    if (behavior->startParamHint >= 0.0f) {
        FindShapeClosestPointFromDistance(shape, behavior->startParamHint, GetLoWorldPos(behavior->owner), &D_003DD020, 0, 0, 0, &behavior->pathParam);
        behavior->startParamHint = -1.0f;
    } else {
        FindShapeClosestPoint(shape, GetLoWorldPos(behavior->owner), &D_003DD020, 0, 0, 0, &behavior->pathParam);
    }
    if (shape->isClosed) {
        behavior->hasEnd = 0;
    } else {
        behavior->hasEnd = 1;
        if (behavior->isReversed) behavior->endParam = GetShapeMinDistance(shape);
        else behavior->endParam = GetShapeMaxDistance(shape);
        EvaluateShapeAtDistance(shape, behavior->endParam, &behavior->endPoint, 0);
    }
}

// 0x178E98 InitBhvwalkpath
namespace M_InitBhvwalkpath {
extern float D_002BE694 __asm__("D_002BE694");
void InitBhvpath(BHVPATH *p) __asm__("InitBhvpath");
} // namespace M_InitBhvwalkpath
// 0x178E98: BHVWALKPATH vtable slot 7 (OnPush): BHVPATH OnPush, walk animation flag 9, rate 1.
// Also known as: Init (upstream readable / Sly 1 name).
void BHVWALKPATH::OnPush()
{
    using namespace M_InitBhvwalkpath;
    BHVWALKPATH *p = this;
    InitBhvpath(p);
    p->stuckTimer = D_002BE694;
    p->rate = 1.0f;
    p->animSgf = 9;
}

// 0x178EE0 UpdateBhvwalkpath
namespace M_UpdateBhvwalkpath {
 struct CONTROL; struct VU_FLOAT;
// UpdateBhvwalkpath (Claude agent, lane G13, 2026-10-24): laneG10v2 (no pins) + VU_FLOAT(float) qmtc2 constructor building the speed in place, and the 1e-4 length test read with VuGetX (gcc emits the qmfc2, asm mtc1).
/* the kind-3 binding's handle and instance (the DIRANIM pair; leap.h pads CID146) */

struct CONTROL { char pad[0x24]; void *animation; };
// Float broadcast into a VU0 register on construction.
struct VU_FLOAT { qword qw; VU_FLOAT() {} VU_FLOAT(float g) { __asm__("qmtc2 %1, %0" : "=j"(qw) : "r"(g)); } };

extern CLQ D_002BE6A0 __asm__("D_002BE6A0");
extern LM g_lmZeroOne __asm__("g_lmZeroOne");
extern float D_002BE694 __asm__("D_002BE694"), D_002BE6B0 __asm__("D_002BE6B0");
extern qword g_normalZ __asm__("g_normalZ");
void UpdateBhvpath(BHVPATH *) __asm__("UpdateBhvpath");
MATRIX3 *GetLoWorldMatrix(LO *) __asm__("GetLoWorldMatrix");
void SetAsegaSpeed__FP5ASEGAf(void *, float) __asm__("SetAsegaSpeed__FP5ASEGAf");
float atan2f(float, float) __asm__("atan2f");
void SetStepguardMoverRadTarget(GUARDMOVER *, float) __asm__("SetStepguardMoverRadTarget");
void SelectDirAnim(CONTROL *, qword) __asm__("SelectDirAnim");

// VU0 helper: a float in a VU register.
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "r"(g));
    return v;
}
// VU0 helper: the x component of a VU register as a float.
// VU0 helper: x component of a quadword vector.
// VU0 helper: y component of a quadword vector.
// VU0 helper: vector difference.
// VU0 helper: dot product (in x).
// VU0 helper: v minus its projection on n.
static inline qword VuProjectOut(qword v, qword n)
{
    qword d = VuDot(v, n);
    qword al, ac;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(al) : "j"(d), "j"(n));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(ac) : "j"(v), "j"(al));
    return ac;
}
// VU0 helper: length of a vector (in x).
// VU0 helper: horizontal (xy) length of a vector.
// VU0 helper: length of a vector (operands not tied, for the match).
static inline qword VuLengthUntied(qword v)
{
    qword qwSq;
    qword vfOne;
    qword qwLen;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qwSq), "=j"(vfOne) : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qwLen) : "j"(qwSq));
    return qwLen;
}
// VU0 helper: length via a dot product.
static inline qword VuLengthFromDot(qword v)
{
    qword qw = VuDot(v, v);
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
// VU0 helper: length of a vector.
static inline float VectorLength(qword v) { return VuToFloat(VuLength(v)); }
// VU0 helper: horizontal (xy) length of a vector.
static inline float VectorLengthXY(qword v) { return VuToFloat(VuLengthXY(v)); }
// VU0 helper: vector scaled by the x of a VU float.
static inline qword VuScaleByX(qword qwVec, VU_FLOAT g)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwVec), "j"(g.qw));
    return qw;
}
// VU0 helper: dot products of a vector with the rows of a 3x3 matrix.
static inline qword VuDotMatrixRows(MATRIX3 *pmat, qword qwV)
{
    qword qwLocal;
    qword qwTmp0, qwTmp1, qwTmp2, qwOnes;
    __asm__("vmul.xyzw %2, %5, %8\n\tvmaxw.xyzw %1, $vf0, $vf0w\n\tvmul.xyzw %3, %6, %8\n\tvmul.xyzw %4, %7, %8\n\tvadday.x $ACC, %2, %2y\n\tvmaddz.x %0, %1, %2z\n\tvaddax.y $ACC, %3, %3x\n\tvmaddz.y %0, %1, %3z\n\tvaddax.z $ACC, %4, %4x\n\tvmaddy.z %0, %1, %4y"
            : "=j"(qwLocal), "=&j"(qwOnes), "=&j"(qwTmp0), "=&j"(qwTmp1), "=&j"(qwTmp2)
            : "j"(*(qword *)pmat->mat[0]), "j"(*(qword *)pmat->mat[1]), "j"(*(qword *)pmat->mat[2]), "j"(qwV));
    return qwLocal;
}
// Evaluates a CLQ curve at x clamped to an LM range.
static inline float EvaluateClampedQuadratic(CLQ *pclq, LM *plm, float x)
{
    float g = pclq->g0 + x * (pclq->g1 + x * pclq->g2);
    float gMin = plm->minValue;
    float gMax = plm->maxValue;
    if (g < gMin)
        return gMin;
    if (gMax < g)
        return gMax;
    return g;
}
// True if a position moved measurably since the last one.
static inline int HasMoved(qword qwPos, qword qwPosLast)
{
    return D_002BE6B0 < VectorLength(VuSub(qwPos, qwPosLast));
}

// 0x178EE0: BHVWALKPATH vtable slot 10 (Update): BHVPATH update, then sets the walk animation speed and the mover
// heading toward the target point; runs the stuck timer while not moving.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvwalkpath(BHVWALKPATH *behavior)
{
    UpdateBhvpath(behavior);
    if ((unsigned int)behavior->state < 2)
    {
        qword pos = GetLoWorldPos(behavior->owner);
        qword dpos = VuProjectOut(VuSub(behavior->startPosition, pos), g_normalZ);
        float g = EvaluateClampedQuadratic(&D_002BE6A0, &g_lmZeroOne, QwordX(VuLengthXY(dpos)));
        ANIMBINDING *pchoice = ((BHVANIM *)behavior->pbhvNext)->binding;
        switch (pchoice->kind)
        {
        case 0:
            SetAsegaSpeed__FP5ASEGAf(pchoice->asega, behavior->rate * g);
            SetStepguardMoverRadTarget(&((STEPGUARD *)behavior->owner)->mover, atan2f(QwordY(dpos), QwordX(dpos)));
            break;
        case 3:
        {
            qword qwLocal = VuDotMatrixRows(GetLoWorldMatrix(behavior->owner), dpos);
            qword qwFallback = D_002A2DD0;
            float gSpeed = behavior->rate * g;
            gSpeed *= QwordX(VuLengthXY(((CID146 *)pchoice->payload4)->velocity));
            qword qwLen = VuLengthUntied(qwLocal);
            VU_FLOAT vuSpeed(gSpeed);
            qword qwDir;
            if (QwordX(qwLen) < 0.0001f)
            {
                qwDir = qwFallback;
            }
            else
            {
                __asm__("vdiv $Q, $vf0w, %0x\n\tvwaitq" : : "j"(qwLen));
                __asm__("vmulq.xyzw %0, %1, $Q" : "=j"(qwDir) : "j"(qwLocal));
            }
            SelectDirAnim((CONTROL *)pchoice->payload, VuScaleByX(qwDir, vuSpeed));
            SetAsegaSpeed__FP5ASEGAf(((CONTROL *)pchoice->payload)->animation, g);
            break;
        }
        }
        if (HasMoved(pos, behavior->stuckCheckPos))
        {
            behavior->stuckCheckPos = pos;
            behavior->stuckTimer = D_002BE694;
        }
        else
        {
            behavior->stuckTimer -= g_clock.deltaTime;
        }
    }
}
} // namespace M_UpdateBhvwalkpath

// 0x1791C8 SetBhvwalkpathState
namespace M_SetBhvwalkpathState {
extern float D_002BE694 __asm__("D_002BE694");
void SetBhvpathState(BHVWALKPATH *p, int n) __asm__("SetBhvpathState");
void PopBhv(void *p) __asm__("PopBhv");
BHV *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
// 0x1791C8: BHVWALKPATH state change: moving states push the walk animation (its flag, 8 when carrying an object),
// stopped states push BHVIDLE.
extern "C" void SetBhvwalkpathState(BHVWALKPATH *behavior, int n) {
    int old = behavior->state;
    if (n == old) return;
    SetBhvpathState(behavior, n);
    switch (behavior->state) {
    case 0: case 1:
        if ((unsigned)old < 2) return;
        PopBhv((&behavior->pbhvNext));
        {
            BHVANIM *o = (BHVANIM *)PushStepBehavior(behavior->owner, CID_BHVANIM);
            o->sgf = behavior->animSgf;
            if (behavior->animSgf == 8) o->lookAt = behavior->lookAtTarget;
            o->Start();
        }
        behavior->stuckCheckPos = GetLoWorldPos(behavior->owner);
        behavior->stuckTimer = D_002BE694;
        break;
    case 2: case 3: case 4:
        if (old == 2 || old == 4 || old == 3) return;
        PopBhv((&behavior->pbhvNext));
        {
            BHVANIM *o = (BHVANIM *)PushStepBehavior(behavior->owner, CID_BHVIDLE);
            o->lookAt = behavior->lookAtTarget;
            o->Start();
        }
        break;
    }
}
} // namespace M_SetBhvwalkpathState

// 0x1792E8 IsBhvwalkpathStuck
namespace M_FBhvwalkpathStuck {

// 0x1792E8: True if the guard is walking and the stuck timer ran out.
// Also known as: FStuck (upstream readable / Sly 1 name).
extern "C" int IsBhvwalkpathStuck(BHVWALKPATH *behavior)
{
    int f = 0;
    if (behavior->state < 2) {
        if (behavior->state >= 0)
            f = behavior->stuckTimer < 0.0f;
    }
    return f;
}
} // namespace M_FBhvwalkpathStuck

// 0x179328 StartBhvhit
namespace M_StartBhvhit {
struct E4;
extern float D_002A5584 __asm__("D_002A5584");
extern LO *D_002F7900 __asm__("D_002F7900");
/* the kind-2 (leap) binding's handle record (leap.h pads LEAP) */
struct E4 { char pad[0x1C]; float knockbackDistance; };
float atan2f(float y, float x) __asm__("atan2f");
float *GetLoWorldMatrix(LO *plo) __asm__("GetLoWorldMatrix");
void SetStepguardMoverRadTarget(GUARDMOVER *pmover, float f) __asm__("SetStepguardMoverRadTarget");
float NormalizeAngle__Ff(float f) __asm__("NormalizeAngle__Ff");
qword SetVectorCylind(float a, float b, float c) __asm__("SetVectorCylind");
void SetLeapTarget(void *p, qword v) __asm__("SetLeapTarget");
void SetStepguardAttack(STEPGUARD *guard, int b, int c, qword v) __asm__("SetStepguardAttack");
void ReplaceHeldSwNoise(SW *psw, int n, qword v, STEPGUARD *guard, CID147 **ppnoise) __asm__("ReplaceHeldSwNoise");
LO *GetHeroPo(SW *psw) __asm__("GetHeroPo");
int IsLoSelfOrDescendantOf(LO *plo, LO *ploAncestor) __asm__("IsLoSelfOrDescendantOf");
void TriggerJoyRumbleRumk__FP3JOY4RUMKf(void *p, int n, float f) __asm__("TriggerJoyRumbleRumk__FP3JOY4RUMKf");
} // namespace M_StartBhvhit
// 0x179328: BHVHIT vtable slot 8 (Start): BHVANIM Start, faces away from the hit, knock-back leap, cancels the
// attack, holds a pain noise, arms the rumble and zap-mode timers.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvhit(BHVHIT *behavior) {
    using namespace M_StartBhvhit;
    int f;
    float a;
    ANIMBINDING *e;
    StartBhvanim(behavior);
    {
        qword d, one, l;
        float g;
        __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(behavior->zpr.velocity));
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
        __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
        f = 0;
        if (g > 1.0e-4f) f = behavior->zapKind != 0x40000;
    }
    if (f) {
        a = atan2f(-((float *)&behavior->zpr.velocity)[1], -((float *)&behavior->zpr.velocity)[0]);
    } else {
        float *m = GetLoWorldMatrix(((STEPGUARD *)behavior->owner));
        a = atan2f(m[1], GetLoWorldMatrix(((STEPGUARD *)behavior->owner))[0]);
    }
    SetStepguardMoverRadTarget(&((STEPGUARD *)behavior->owner)->mover, a);
    e = behavior->binding;
    if (e && e->kind == 2) {
        qword r = SetVectorCylind(NormalizeAngle__Ff(((STEPGUARD *)behavior->owner)->targetHeading + 3.14159274101257324219e+00f), ((E4 *)behavior->binding->payload4)->knockbackDistance, 0.0f);
        qword pos = GetLoWorldPos(((STEPGUARD *)behavior->owner));
        qword s;
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(s) : "j"(pos), "j"(r));
        SetLeapTarget(behavior->binding->payload, s);
    }
    SetStepguardAttack(((STEPGUARD *)behavior->owner), 0, 0, D_002A2DD0);
    if (behavior->state == 0) SetBhvBusy(behavior);
    ReplaceHeldSwNoise(&g_sw, 1, GetLoWorldPos(((STEPGUARD *)behavior->owner)), ((STEPGUARD *)behavior->owner), &((STEPGUARD *)behavior->owner)->heldNoise);
    behavior->cameraJoltTime = behavior->zapModeTime = D_002A5584;
    ((STEPGUARD *)behavior->owner)->nextAttackTime = 0;
    if (((STEPGUARD *)behavior->owner)->unknown334) {
        LO *o = GetHeroPo(&g_sw);
        if (o && IsLoSelfOrDescendantOf(behavior->attacker, o) && (o == D_002F7900 || (behavior->zapKind & 0x40000))) {
            TriggerJoyRumbleRumk__FP3JOY4RUMKf(&g_joy, 3, 1.50000005960464477539e-01f);
            behavior->cameraJoltTime = g_clock.time + 5.00000044703483581543e-02f;
        }
        behavior->zapModeTime = g_clock.time + 1.33333340287208557129e-01f;
    }
}

// 0x179598 CloseBhvhit
// 0x179598: BHVHIT vtable slot 9 (Close): BHVANIM Close, releases the held noise.
// Also known as: Exit (upstream readable / Sly 1 name).
void BHVHIT::Close()
{
    CloseBhvanim(this);
    M_StartBhvhit::ReplaceHeldSwNoise(&g_sw, -1, D_002A2DD0, 0, &((STEPGUARD *)owner)->heldNoise);
}

namespace RO_ORDER { extern const float r_D_002A5584 __asm__("D_002A5584") = 3.40282347e+38f; }
// 0x1795E8 UpdateBhvhit
namespace M_UpdateBhvhit {
void UpdateBhvanim(BHVHIT *p) __asm__("UpdateBhvanim");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
void JoltCamera(void *p, int n) __asm__("JoltCamera");
void SetSoZapMode(STEPGUARD *t, int n) __asm__("SetSoZapMode");
// 0x1795E8: BHVHIT vtable slot 10 (Update): BHVANIM update, keeps the held noise at the guard, fires the delayed
// camera jolt and zap-mode change.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvhit(BHVHIT *behavior) {
    float t;
    UpdateBhvanim(behavior);
    {
        STEPGUARD *pt = ((STEPGUARD *)behavior->owner);
        CID147 *v = pt->heldNoise;
        v->position = GetLoWorldPos(pt);
    }
    t = g_clock.time + 0.0001f;
    if (behavior->cameraJoltTime < t) { behavior->cameraJoltTime = 3.40282346638528859812e+38f; JoltCamera(&g_cm, 0); }
    if (behavior->zapModeTime < t) { behavior->zapModeTime = 3.40282346638528859812e+38f; SetSoZapMode(((STEPGUARD *)behavior->owner), 2); }
}
} // namespace M_UpdateBhvhit

// 0x1796A0 InitBhvHurt
// 0x1796A0: Hurt behaviour (BHV161) vtable slot 7 (OnPush): BHVHIT OnPush with the hurt flag (5).
void BHV161::OnPush()
{
    InitBhvanim(this);
    sgf = 5;
}

// 0x1796D0 StartBhvHurt
namespace M_func_001796D0 {
int GetZapKindIndex(int p) __asm__("GetZapKindIndex");
void StartBhvhit(BHV161 *p) __asm__("StartBhvhit");
void BroadcastGameEvent(void *g, int event, void *p) __asm__("BroadcastGameEvent");
void SetDetectorTargetStateById(void *p, LO *a, int b) __asm__("SetDetectorTargetStateById");
// 0x1796D0: Hurt behaviour vtable slot 8 (Start): picks the reaction from the hurt table by zap kind, BHVHIT Start,
// broadcasts game event 0x15 and marks the attacker on the detector.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvHurt(BHV161 *behavior) {
    behavior->binding = &((STEPGUARD *)behavior->owner)->hurtReactions[GetZapKindIndex(behavior->zapKind)];
    if (behavior->binding->kind == -1) behavior->binding = 0;
    StartBhvhit(behavior);
    BroadcastGameEvent(&g_game, 0x15, ((STEPGUARD *)behavior->owner));
    SetDetectorTargetStateById(((STEPGUARD *)behavior->owner)->pdetector, behavior->attacker, 2);
}
} // namespace M_func_001796D0

// 0x179750 InitBhvKnockdown
// 0x179750: Knockdown behaviour (BHV162) vtable slot 7 (OnPush): BHVHIT OnPush with the knockdown flag (6).
void BHV162::OnPush()
{
    InitBhvanim(this);
    sgf = 6;
}

// 0x179780 StartBhvKnockdown
namespace M_func_00179780 {
int GetZapKindIndex(int p) __asm__("GetZapKindIndex");
void StartBhvhit(BHV162 *p) __asm__("StartBhvhit");
void BroadcastGameEvent(void *g, int event, void *p) __asm__("BroadcastGameEvent");
void SetDetectorTargetStateById(void *p, LO *a, int b) __asm__("SetDetectorTargetStateById");
// 0x179780: Knockdown behaviour vtable slot 8 (Start): reaction from the knockdown table, BHVHIT Start, game event
// 0x15, starts the helper component (+0xFEC).
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvKnockdown(BHV162 *behavior) {
    behavior->binding = &((STEPGUARD *)behavior->owner)->knockdownReactions[GetZapKindIndex(behavior->zapKind)];
    if (behavior->binding->kind == -1) behavior->binding = 0;
    StartBhvhit(behavior);
    BroadcastGameEvent(&g_game, 0x15, ((STEPGUARD *)behavior->owner));
    SetDetectorTargetStateById(((STEPGUARD *)behavior->owner)->pdetector, behavior->attacker, 2);
    { LO *o = ((STEPGUARD *)behavior->owner)->knockdownHelper; if (o) o->Add(); }
}
} // namespace M_func_00179780

// 0x179820 CloseBhvKnockdown
namespace M_func_00179820 {
void CloseBhvhit(BHV162 *p) __asm__("CloseBhvhit");
// 0x179820: Knockdown behaviour vtable slot 9 (Close): BHVHIT Close and takes the helper object out of the world.
extern "C" void CloseBhvKnockdown(BHV162 *behavior)
{
    LO *o;
    CloseBhvhit(behavior);
    o = ((STEPGUARD *)behavior->owner)->knockdownHelper;
    if (o) o->Remove();
}
} // namespace M_func_00179820

// 0x179868 InitBhvko
namespace M_InitBhvko {

void InitBhvanim(BHVKO *p) __asm__("InitBhvanim");
// 0x179868: BHVKO vtable slot 7 (OnPush): BHVHIT OnPush with the KO flag (7).
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvko(BHVKO *behavior)
{
    InitBhvanim(behavior);
    behavior->sgf = 7;
}
} // namespace M_InitBhvko

// 0x179898 StartBhvko
namespace M_StartBhvko {
/* StartBhvko (Claude agent lane G2, 2026-10-24). Count() is an inline whose else arm starts with the
   for-init `n = 0` (keeps the target's bltz/then-first layout). The coinDropPos vector is read into q before the call. */
extern char D_002DE578[] __asm__("D_002DE578");
int GetZapKindIndex(int mask) __asm__("GetZapKindIndex");
LO *GetHeroPo(SW *psw) __asm__("GetHeroPo");
void BroadcastGameEvent(void *a, int n, STEPGUARD *p) __asm__("BroadcastGameEvent");
qword ConvertPosBetweenLos(LO *source, LO *target, qword v) __asm__("ConvertPosBetweenLos");
void SprayDifficultyPrizes(void *a, qword v, int n, int m, STEPGUARD *p) __asm__("SprayDifficultyPrizes");
void ResetDetectorTargets(DETECTOR *p) __asm__("ResetDetectorTargets");
// Matching helper: reads a count word with an explicit load (keeps the target's instruction order).
extern "C" { static inline int Count(STEPGUARD *a) {
    int n, i;
    if (a->coinCountOverride >= 0) {
        n = a->coinCountOverride;
    } else {
        for (i = a->firstLoot, n = 0; i < a->numLoot; i++) {
            GUARDLOOT *e = &a->loot[i];
            if (e->oid == 0x2B3) n += e->count;
        }
        n = (n + 1) / 2;
    }
    return n;
} }
// 0x179898: BHVKO vtable slot 8 (Start): reaction from the KO table, BHVHIT Start; the first time: marks the guard
// KO, broadcasts 0x18 (by the hero) or 0x17, drops coins from its loot and resets the detector.
extern "C" void StartBhvko(BHVKO *behavior) {
    int n;
    STEPGUARD *a;
    ANIMBINDING *pe = &((STEPGUARD *)behavior->owner)->koReactions[GetZapKindIndex(behavior->zapKind)];
    behavior->binding = pe;
    if (pe->kind == -1) behavior->binding = 0;
    StartBhvhit(behavior);
    a = ((STEPGUARD *)behavior->owner);
    if (a->isKnockedOut != 0)
        return;
    a->isKnockedOut = 1;
    if (behavior->attacker == GetHeroPo(&g_sw))
        BroadcastGameEvent(&g_game, 0x18, ((STEPGUARD *)behavior->owner));
    else
        BroadcastGameEvent(&g_game, 0x17, ((STEPGUARD *)behavior->owner));
    n = Count(((STEPGUARD *)behavior->owner));
    {
        qword q = ((STEPGUARD *)behavior->owner)->coinDropPos;
        qword v;
        /* Empty barrier: the target re-reads p->p10 for arg0 after loading q instead of reusing the register. */
        __asm__ __volatile__("" : : : "memory");
        v = ConvertPosBetweenLos(((STEPGUARD *)behavior->owner), 0, q);
        SprayDifficultyPrizes(D_002DE578, v, n, ((STEPGUARD *)behavior->owner)->unknown3F0, ((STEPGUARD *)behavior->owner));
    }
    RefreshSwObjectCollisionFilter(&g_sw, ((STEPGUARD *)behavior->owner));
    ResetDetectorTargets(((STEPGUARD *)behavior->owner)->pdetector);
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_int[(sizeof(int)==4)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
} // namespace M_StartBhvko

// 0x179A20 CloseBhvko
// 0x179A20: BHVKO vtable slot 9 (Close): BHVHIT Close and refreshes the world filter.
void BHVKO::Close()
{
    BHV *behavior = (BHV *)this;
    CloseBhvhit(behavior);
    RefreshSwObjectCollisionFilter(&g_sw, behavior->owner);
}

// 0x179A58 InitBhvpatrol
namespace M_InitBhvpatrol {

void InitBhv(BHVPATROL *p) __asm__("InitBhv");
void AddStepguardSgf(void *p, int n) __asm__("AddStepguardSgf");
// 0x179A58: BHVPATROL vtable slot 7 (OnPush): raises flag 0x1E (patrolling), timers off.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvpatrol(BHVPATROL *behavior)
{
    InitBhv(behavior);
    behavior->state = -1;
    behavior->walkEndTime = behavior->pauseEndTime = 3.40282346638528859812e+38f;
    AddStepguardSgf(behavior->owner, SGF_PATROL);
}
} // namespace M_InitBhvpatrol

// 0x179AA0 StartBhvpatrol
namespace M_StartBhvpatrol {

void StartBhv(BHVPATROL *p) __asm__("StartBhv");
void SetBhvpatrolState(BHVPATROL *p, int n) __asm__("SetBhvpatrolState");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
void ChooseGuardScriptNearestCommand(CID113 *script, qword q, SSGRESUME *presume) __asm__("ChooseGuardScriptNearestCommand");
int IsStepguardHandsOff(STEPGUARD *t) __asm__("IsStepguardHandsOff");
// 0x179AA0: BHVPATROL vtable slot 8 (Start): takes the patrol script (or the alternate one with capability 2),
// resumes at the nearest script point, then walks (0), stands (2) or is hands-off (3).
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvpatrol(BHVPATROL *behavior) {
    STEPGUARD *t;
    StartBhv(behavior);
    t = ((STEPGUARD *)behavior->owner);
    if (t->patrolScript) behavior->script = t->patrolScript;
    else if (t->capabilityFlags & 2) behavior->script = t->altPatrolScript;
    else { behavior->script = 0; SetBhvpatrolState(behavior, 2); return; }
    if (((STEPGUARD *)behavior->owner)->scriptResumePoint.command) ChooseGuardScriptNearestCommand(behavior->script, GetLoWorldPos(((STEPGUARD *)behavior->owner)), &((STEPGUARD *)behavior->owner)->scriptResumePoint);
    if (IsStepguardHandsOff(((STEPGUARD *)behavior->owner))) SetBhvpatrolState(behavior, 3); else SetBhvpatrolState(behavior, 0);
}
} // namespace M_StartBhvpatrol

// 0x179B60 CloseBhvpatrol
// 0x179B60: BHVPATROL vtable slot 9 (Close): stops, lowers flag 0x1E.
// Also known as: Exit (upstream readable / Sly 1 name).
void BHVPATROL::Close()
{
    BHV *behavior = (BHV *)this;
    SetBhvpatrolState(behavior, -1);
    CloseBhv(behavior);
    RemoveStepguardSgf((STEPGUARD *)behavior->owner, SGF_PATROL);
}

// 0x179BA0 UpdateBhvpatrol
namespace M_UpdateBhvpatrol {
void UpdateBhv(BHVPATROL *p) __asm__("UpdateBhv");
int IsStepguardHandsOff(void *p) __asm__("IsStepguardHandsOff");
int IsBhvBusyAbove(BHVPATROL *p) __asm__("IsBhvBusyAbove");
void SetBhvpatrolState(BHVPATROL *p, int n) __asm__("SetBhvpatrolState");
int IsBhvScriptDone(void *p) __asm__("IsBhvScriptDone");
// 0x179BA0: BHVPATROL vtable slot 10 (Update): alternates walking the script for a random time and standing idle
// for a random time; stands forever when the script ends; pauses while hands-off.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvpatrol(BHVPATROL *behavior) {
    int f, s;
    UpdateBhv(behavior);
    f = IsStepguardHandsOff(behavior->owner);
    while (!IsBhvBusyAbove(behavior)) {
        s = behavior->state;
        switch (s) {
        case 1:
            if (f) SetBhvpatrolState(behavior, 3);
            else if (g_clock.time > behavior->pauseEndTime) SetBhvpatrolState(behavior, 0);
            break;
        case 2:
            if (f) SetBhvpatrolState(behavior, 3);
            break;
        case 0:
            if (f) SetBhvpatrolState(behavior, 3);
            else if (g_clock.time > behavior->walkEndTime) SetBhvpatrolState(behavior, 1);
            else if (IsBhvScriptDone(behavior->pbhvNext)) { behavior->script = 0; SetBhvpatrolState(behavior, 2); }
            break;
        case 3:
            if (!f) {
                if (behavior->script) SetBhvpatrolState(behavior, 1);
                else SetBhvpatrolState(behavior, 2);
            }
            break;
        }
        if (behavior->state != s) continue;
        break;
    }
}
} // namespace M_UpdateBhvpatrol

// 0x179D30 SetBhvpatrolState
namespace M_SetBhvpatrolState {
BHV *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
void SaveBhvScriptResumeState(BHV *p, SSGRESUME *presume) __asm__("SaveBhvScriptResumeState");
void SetBhvScriptScript(BHV169 *o, CID113 *script, int a, SSGRESUME *presume) __asm__("SetBhvScriptScript");
void ArmBhvpatrolPauseTimer(BHVPATROL *p) __asm__("ArmBhvpatrolPauseTimer");
void ArmBhvpatrolWalkTimer(BHVPATROL *p) __asm__("ArmBhvpatrolWalkTimer");
// 0x179D30: BHVPATROL state switch: 0 runs the patrol script (BHV169, saving the resume point), 1/2 idle, 3
// hands-off idle.
// Also known as: SetWaitState (upstream readable / Sly 1 name; SetWaitState__7JOBWAITi).
void SetBhvpatrolState(BHVPATROL *behavior, int n) __asm__("SetBhvpatrolState"); void SetBhvpatrolState(BHVPATROL *behavior, int n) {
    if (n == behavior->state) return;
    if (behavior->state == 0) SaveBhvScriptResumeState(behavior->pbhvNext, &((STEPGUARD *)behavior->owner)->scriptResumePoint);
    PopBhv(&behavior->pbhvNext);
    behavior->state = n;
    behavior->stateTime = g_clock.time;
    switch (n) {
    case 1: case 2:
        {
            BHV *o = PushStepBehavior(behavior->owner, CID_BHVIDLE);
            o->Start();
        }
        ArmBhvpatrolPauseTimer(behavior);
        behavior->walkEndTime = 3.40282346638528859812e+38f;
        break;
    case 0:
        {
            BHV169 *o = (BHV169 *)PushStepBehavior(behavior->owner, CID_BHV169);
            SetBhvScriptScript(o, behavior->script, 0, &((STEPGUARD *)behavior->owner)->scriptResumePoint);
            o->Start();
        }
        ArmBhvpatrolWalkTimer(behavior);
        behavior->pauseEndTime = 3.40282346638528859812e+38f;
        break;
    case 3:
        {
            BHVIDLE *o = (BHVIDLE *)PushStepBehavior(behavior->owner, CID_BHVIDLE);
            o->sgf = 1;
            o->Start();
        }
        break;
    }
}
} // namespace M_SetBhvpatrolState

// 0x179E80 ArmBhvpatrolWalkTimer
namespace M_ArmBhvpatrolWalkTimer {
float RandomFloatInRange__Fff(float a, float b) __asm__("RandomFloatInRange__Fff");
// 0x179E80: Arms the patrol walk timer from the guard's walk-time range (FLT_MAX = forever).
extern "C" void ArmBhvpatrolWalkTimer(BHVPATROL *behavior)
{
    if (behavior->walkEndTime < 3.40282346638528859812e+38f)
        return;
    if (((STEPGUARD *)behavior->owner)->walkTimes.minValue == 3.40282346638528859812e+38f)
        behavior->walkEndTime = 3.40282346638528859812e+38f;
    else {
        float t = g_clock.time;
        behavior->walkEndTime = t + RandomFloatInRange__Fff(((STEPGUARD *)behavior->owner)->walkTimes.minValue, ((STEPGUARD *)behavior->owner)->walkTimes.maxValue);
    }
}
} // namespace M_ArmBhvpatrolWalkTimer

// 0x179EF8 ArmBhvpatrolPauseTimer
namespace M_ArmBhvpatrolPauseTimer {
float RandomFloatInRange__Fff(float a, float b) __asm__("RandomFloatInRange__Fff");
// 0x179EF8: Arms the patrol pause timer from the guard's pause-time range.
extern "C" void ArmBhvpatrolPauseTimer(BHVPATROL *behavior)
{
    if (behavior->pauseEndTime < 3.40282346638528859812e+38f)
        return;
    if (((STEPGUARD *)behavior->owner)->pauseTimes.minValue == 3.40282346638528859812e+38f)
        behavior->pauseEndTime = 3.40282346638528859812e+38f;
    else {
        float t = g_clock.time;
        behavior->pauseEndTime = t + RandomFloatInRange__Fff(((STEPGUARD *)behavior->owner)->pauseTimes.minValue, ((STEPGUARD *)behavior->owner)->pauseTimes.maxValue);
    }
}
} // namespace M_ArmBhvpatrolPauseTimer

// 0x179F70 InitBhvleap
namespace M_InitBhvleap {

void InitBhvanim(BHVLEAP *p) __asm__("InitBhvanim");
// 0x179F70: BHVLEAP vtable slot 7 (OnPush): BHVANIM OnPush with the leaping flag (0xF).
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvleap(BHVLEAP *behavior)
{
    InitBhvanim(behavior);
    behavior->sgf = 0xF;
}
} // namespace M_InitBhvleap

// 0x179FA0 StartBhvleap
namespace M_StartBhvleap {
union U;   union TEMP; struct WORK; struct LIMIT;
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
union U{qword q;float f[4];};
   /* the CID19 hang handle (no header yet): its kind */
union TEMP{struct{U direction;MATRIX3Q rotation;}first;MATRIX3Q basis;};
struct WORK{TEMP temp;float curve;};
struct LIMIT{float low,high;};

 extern unsigned long D_002DFB70 __asm__("D_002DFB70");
 extern int D_002DFB78 __asm__("D_002DFB78");
 extern qword g_normalZ __asm__("g_normalZ");
 extern LIMIT D_002BE6B8 __asm__("D_002BE6B8");
 unsigned long GetSgfMask(int) __asm__("GetSgfMask");
 int IsBasicDerivedFrom__FP5BASIC3CID(void*,int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
 qword GetLoWorldPos(void*) __asm__("GetLoWorldPos");
 MATRIX3Q*GetLoWorldMatrix(void*) __asm__("GetLoWorldMatrix");
 void FindShapeClosestPoint(LO*, qword, void*, qword*, qword*, float*, float*) __asm__("FindShapeClosestPoint");
 float atan2f(float,float) __asm__("atan2f");
 float NormalizeAngle__Ff(float) __asm__("NormalizeAngle__Ff");
 void BuildAxisAngleRotationMatrix(MATRIX3Q*,float,qword) __asm__("BuildAxisAngleRotationMatrix");
 MATRIX3Q*BuildOrthonormalMatrixZ(MATRIX3Q*,qword,qword) __asm__("BuildOrthonormalMatrixZ");
 void GetPipeGoalMatrix(LO*,int,MATRIX3Q*,float) __asm__("GetPipeGoalMatrix");
 void AdjustBhvleapAttachPoint(BHVLEAP*) __asm__("AdjustBhvleapAttachPoint");
 int SnapToGround(qword*,float,float,int,int(*)(void)) __asm__("SnapToGround");
 void StartBhvanim(BHVLEAP*) __asm__("StartBhvanim");
 void SetStepguardMoverRadTarget(GUARDMOVER*,float) __asm__("SetStepguardMoverRadTarget");
 void SetStepguardMoverTargetMatrix(GUARDMOVER*,MATRIX3Q*) __asm__("SetStepguardMoverTargetMatrix");
 void SetLeapTarget(LEAP*,qword) __asm__("SetLeapTarget");
 void SetBhvBusy(BHVLEAP*) __asm__("SetBhvBusy");

// Heading (atan2) of a vector's xy components.
static inline float Angle(qword delta)
{
    qword x;
    int y;
    float fx, fy;
    __asm__("qmfc2.ni %0,%1" : "=r"(x) : "j"(delta));
    __asm__("prot3w %0,%1" : "=r"(y) : "r"(x));
    __asm__("mtc1 %1,%0" : "=f"(fx) : "r"(x));
    __asm__("mtc1 %1,%0" : "=f"(fy) : "r"(y));
    return atan2f(fy, fx);
}
// Matching helper (register-pinned asm): returns current - home and the actor, in the operand order the target uses.
static inline qword HeadingInputs(BHVLEAP *p, U *home, qword current, STEPGUARD **actor)
{
    register qword a __asm__("vf1"), b __asm__("vf2");
    STEPGUARD *o;
    __asm__("qmtc2.ni %3,%1\n\tlw %2,%4\n\tlqc2 %0,%5"
            : "=j"(a), "=j"(b), "=&r"(o)
            : "r"(current), "m"(p->owner), "m"(home->q));
    __asm__("vsub.xyzw %0,%0,%1" : "+j"(a) : "j"(b));
    *actor = o;
    return a;
}
// Matching helper (register-pinned asm): returns home - current and the actor, in the operand order the target uses.
static inline qword BasisInputs(BHVLEAP *p, U *home, qword current, STEPGUARD **actor)
{
    register qword a __asm__("vf1"), b __asm__("vf2");
    STEPGUARD *o;
    __asm__("lqc2 %0,%5\n\tqmtc2.ni %3,%1\n\tlw %2,%4"
            : "=j"(a), "=j"(b), "=&r"(o)
            : "r"(current), "m"(p->owner), "m"(home->q));
    __asm__("vsub.xyzw %0,%1,%0" : "+j"(b) : "j"(a));
    *actor = o;
    return b;
}
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char M3_Size[sizeof(MATRIX3Q)==48?1:-1];
typedef char TEMP_Size[sizeof(TEMP)==64?1:-1];
typedef char WORK_Size[sizeof(WORK)==80?1:-1];
typedef char WORK_temp_Offset[FIELD_OFFSET(WORK,temp)==0?1:-1];
typedef char WORK_curve_Offset[FIELD_OFFSET(WORK,curve)==64?1:-1];
typedef char TEMP_first_direction_Offset[FIELD_OFFSET(TEMP,first.direction)==0?1:-1];
typedef char TEMP_first_rotation_Offset[FIELD_OFFSET(TEMP,first.rotation)==16?1:-1];
typedef char TEMP_basis_Offset[FIELD_OFFSET(TEMP,basis)==0?1:-1];
typedef char LIMIT_low_Offset[FIELD_OFFSET(LIMIT,low)==0?1:-1];
typedef char LIMIT_high_Offset[FIELD_OFFSET(LIMIT,high)==4?1:-1];
// 0x179FA0: BHVLEAP vtable slot 8 (Start): aims at the target point/object; landing on a rail, pipe or attachable
// object raises the matching attach flag (for the animation choice) and builds the attach basis; starts the leap
// animation and sets the leap target.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvleap(BHVLEAP *behavior)
{
    WORK work;
    U point, otherPoint, direction;
    if (!D_002DFB78) {
        unsigned long a = GetSgfMask(SGF_ON_RAIL), b = GetSgfMask(SGF_ON_PIPE), c = GetSgfMask(SGF_ON_CLASS19), d = GetSgfMask(SGF_ON_CLASS39);
        D_002DFB78 = 1;
        D_002DFB70 = a | b | c | d;
    }
    unsigned long mask = D_002DFB70;
    unsigned long old = ((STEPGUARD *)behavior->owner)->grfsg & mask;
    ((STEPGUARD *)behavior->owner)->grfsg &= ~mask;
    unsigned long active = 0;
    if (behavior->target) {
        float heading;
        if (IsBasicDerivedFrom__FP5BASIC3CID(behavior->target, CID_SHAPE)) {
            LO *target = behavior->target;
            qword position = GetLoWorldPos(behavior->owner);
            FindShapeClosestPoint(target, position, 0, &behavior->landingPoint, &work.temp.first.direction.q, 0,
                                  &work.curve);
            heading = atan2f(work.temp.first.direction.f[1], work.temp.first.direction.f[0]);
        } else {
            behavior->landingPoint = GetLoWorldPos(behavior->target);
            MATRIX3Q *a = GetLoWorldMatrix(behavior->target);
            MATRIX3Q *b = GetLoWorldMatrix(behavior->target);
            heading = atan2f(((U *)a->r)->f[1], ((U *)b->r)->f[0]);
        }
        if (IsBasicDerivedFrom__FP5BASIC3CID(behavior->target, CID_RAIL)) {
            active |= GetSgfMask(SGF_ON_RAIL);
            if (!behavior->isReversed)
                heading = NormalizeAngle__Ff(heading + 3.1415927410125732421875f);
            behavior->hasAttachBasis = 1;
            BuildAxisAngleRotationMatrix(&work.temp.first.rotation, heading, g_normalZ);
            *(MATRIX3Q *)&behavior->attachBasis = work.temp.first.rotation;
        } else if (IsBasicDerivedFrom__FP5BASIC3CID(behavior->target, CID_CLASS39)) {
            active |= GetSgfMask(SGF_ON_CLASS39);
        } else if (IsBasicDerivedFrom__FP5BASIC3CID(behavior->target, CID_PIPE)) {
            active |= GetSgfMask(SGF_ON_PIPE);
            behavior->hasAttachBasis = 1;
            GetPipeGoalMatrix(behavior->target, behavior->isReversed, (MATRIX3Q *)&behavior->attachBasis, work.curve);
            AdjustBhvleapAttachPoint(behavior);
        } else if (IsBasicDerivedFrom__FP5BASIC3CID(behavior->target, CID_CLASS19) && ((HND *)behavior->target)->nHangKind == 3) {
            active |= GetSgfMask(SGF_ON_CLASS19);
            behavior->hasAttachBasis = 1;
            MATRIX3Q *m = GetLoWorldMatrix(behavior->target);
            (void)BuildOrthonormalMatrixZ(&work.temp.basis, m->r[0].q, g_normalZ);
            *(MATRIX3Q *)&behavior->attachBasis = work.temp.basis;
            AdjustBhvleapAttachPoint(behavior);
        } else {
            (void)SnapToGround(&behavior->landingPoint, D_002BE6B8.high, D_002BE6B8.low, 1, 0);
            behavior->target = 0;
        }
    }
    ((STEPGUARD *)behavior->owner)->grfsg |= active;
    StartBhvanim(behavior);
    ((STEPGUARD *)behavior->owner)->grfsg &= ~active;
    ((STEPGUARD *)behavior->owner)->grfsg |= old;
    if (behavior->binding) {
        switch (((STEPGUARD *)behavior->owner)->attachKind) {
        case -1:
        case 0:
        case 3: {
            point.q = behavior->landingPoint;
            qword current = GetLoWorldPos(behavior->owner);
            STEPGUARD *actor;
            qword delta = HeadingInputs(behavior, &point, current, &actor);
            GUARDMOVER *control = &actor->mover;
            float angle = Angle(delta);
            SetStepguardMoverRadTarget(control, angle);
            break;
        }
        case 2: {
            otherPoint.q = behavior->landingPoint;
            qword current = GetLoWorldPos(behavior->owner);
            STEPGUARD *actor;
            direction.q = BasisInputs(behavior, &otherPoint, current, &actor);
            MATRIX3Q *m = GetLoWorldMatrix(actor->attachObject);
            (void)BuildOrthonormalMatrixZ(&work.temp.basis, m->r[0].q, direction.q);
            SetStepguardMoverTargetMatrix(&((STEPGUARD *)behavior->owner)->mover, &work.temp.basis);
            break;
        }
        }
        ((LEAP *)behavior->binding->payload)->extra = behavior->extra;
        ((LEAP *)behavior->binding->payload)->target = behavior->target;
        ((LEAP *)behavior->binding->payload)->isReversed = behavior->isReversed;
        SetLeapTarget((LEAP *)behavior->binding->payload, behavior->landingPoint);
    }
    SetBhvBusy(behavior);
}
} // namespace M_StartBhvleap

// 0x17A3D0 CloseBhvleap
// 0x17A3D0: BHVLEAP vtable slot 9 (Close): BHVANIM Close, clears busy.
// Also known as: Exit (upstream readable / Sly 1 name).
void BHVLEAP::Close()
{
    BHVLEAP *behavior = this;
    CloseBhvanim(behavior);
    ClearBhvBusy(behavior);
}

// 0x17A400 UpdateBhvleap
namespace M_UpdateBhvleap {
struct U; struct T; 
struct U { char pad[0x10]; int mode; };
struct T { int a, b; struct U *payload; };
void UpdateBhvanim(BHVLEAP *p) __asm__("UpdateBhvanim");
void SetStepguardMoverTargetMatrix(void *p, void *q) __asm__("SetStepguardMoverTargetMatrix");
// 0x17A400: BHVLEAP vtable slot 10 (Update): BHVANIM update; after the jump phase locks the heading to the attach
// basis.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvleap(BHVLEAP *behavior)
{
    UpdateBhvanim(behavior);
    if (behavior->hasAttachBasis && !behavior->state && ((struct T *)behavior->binding)->payload->mode >= 2)
        SetStepguardMoverTargetMatrix((&((STEPGUARD *)behavior->owner)->mover), &behavior->attachBasis);
}
} // namespace M_UpdateBhvleap

// 0x17A468 AdjustBhvleapAttachPoint
namespace M_func_0017A468 {
struct B; struct C;
struct B { char pad[0x90]; qword v; };   /* the guard's attach point (STEPGUARD +0xF90, untyped) */
struct C { char pad[0x30]; qword v; };   /* the root XF */
// 0x17A468: Offsets the leap landing point by the guard's attach-point offset in the attach basis.
// Also known as: OffsetUseByHands (upstream readable / Sly 1 name; OffsetUseByHands__FP6JOBUSE).
extern "C" void AdjustBhvleapAttachPoint(BHVLEAP *behavior)
{
    STEPGUARD *a = (STEPGUARD *)behavior->owner;
    qword *attachBasis = (qword *)&behavior->attachBasis;
    qword d, t;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(behavior->landingPoint), "j"(((struct C *)a->rootTransform)->v));
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(t) : "j"(((struct B *)a->attachPoint)->v), "j"(attachBasis[0]), "j"(attachBasis[1]), "j"(attachBasis[2]));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(d), "j"(t));
    behavior->landingPoint = d;
}
} // namespace M_func_0017A468

// 0x17A4B0 InitBhvSleep
// 0x17A4B0: Sleep behaviour (BHV175) vtable slot 7 (OnPush): BHVANIM OnPush with the sleeping flag (0x10),
// sub-state -1.
// Also known as: Init (upstream readable / Sly 1 name).
void BHV175::OnPush()
{
    BHV175 *behavior = this;
    InitBhvanim(behavior);
    behavior->sgf = 0x10;
    behavior->sleepState = -1;
}

// 0x17A4E8 StartBhvSleep
namespace M_func_0017A4E8 {
void StartBhvanim(BHV175 *p) __asm__("StartBhvanim");
void SetBhvSleepState(BHV175 *p, int n) __asm__("SetBhvSleepState");
void ResetDetectorTargets(void *p) __asm__("ResetDetectorTargets");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
void ReplaceHeldSwNoise(SW *g, int a, qword q, void *b, void *pv) __asm__("ReplaceHeldSwNoise");
// 0x17A4E8: Sleep behaviour vtable slot 8 (Start): BHVANIM Start, sub-state 0 (asleep, random duration), resets the
// detector, holds a snoring noise.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvSleep(BHV175 *behavior)
{
    StartBhvanim(behavior);
    SetBhvSleepState(behavior, 0);
    ResetDetectorTargets(((STEPGUARD *)behavior->owner)->pdetector);
    ReplaceHeldSwNoise(&g_sw, 1, GetLoWorldPos(((STEPGUARD *)behavior->owner)), ((STEPGUARD *)behavior->owner),
                       &((STEPGUARD *)behavior->owner)->heldNoise);
}
} // namespace M_func_0017A4E8

// 0x17A558 CloseBhvSleep
namespace M_func_0017A558 {
extern qword D_002A2DD0 __asm__("D_002A2DD0");
void CloseBhvanim(BHV175 *p) __asm__("CloseBhvanim");
void SetBhvSleepState(BHV175 *p, int n) __asm__("SetBhvSleepState");
void ReplaceHeldSwNoise(SW *g, int a, qword q, int b, void *pv) __asm__("ReplaceHeldSwNoise");
// 0x17A558: Sleep behaviour vtable slot 9 (Close): BHVANIM Close, releases the held noise.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvSleep(BHV175 *behavior)
{
    CloseBhvanim(behavior);
    SetBhvSleepState(behavior, -1);
    ReplaceHeldSwNoise(&g_sw, -1, D_002A2DD0, 0, &((STEPGUARD *)behavior->owner)->heldNoise);
}
} // namespace M_func_0017A558

// 0x17A5B0 SetBhvSleepState
namespace M_func_0017A5B0 {
void SetSmaGoal(void *p, int n) __asm__("SetSmaGoal");
float RandomFloatInRange__Fff(float a, float b) __asm__("RandomFloatInRange__Fff");
// 0x17A5B0: Sleep behaviour sub-state: 0 asleep (state machine goal 0x44, arms the wake timer), 1 waking up (goal
// 0x43).
// Also known as: SetSleepState (upstream readable / Sly 1 name; SetSleepState__8JOBSLEEPi).
extern "C" void SetBhvSleepState(BHV175 *behavior, int n) {
    if (n == behavior->sleepState) return;
    behavior->sleepState = n;
    behavior->sleepStateF = n;
    switch (n) {
    case 0:
        if (behavior->binding->kind == 1) SetSmaGoal(behavior->binding->payload, 0x44);
        { float t = g_clock.time; behavior->wakeTime = t + RandomFloatInRange__Fff(((STEPGUARD *)behavior->owner)->sleepTimeMin, ((STEPGUARD *)behavior->owner)->sleepTimeMax); }
        break;
    case 1:
        if (behavior->binding->kind == n) SetSmaGoal(behavior->binding->payload, 0x43);
        break;
    }
}
} // namespace M_func_0017A5B0

// 0x17A670 UpdateBhvSleep
namespace M_func_0017A670 {
void UpdateBhvanim(BHV175 *p) __asm__("UpdateBhvanim");
void SetBhvSleepState(BHV175 *p, int n) __asm__("SetBhvSleepState");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
// 0x17A670: Sleep behaviour vtable slot 10 (Update): wakes up when the timer expires; keeps the held noise at the
// guard.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvSleep(BHV175 *behavior) {
    int s;
    UpdateBhvanim(behavior);
    do {
        s = behavior->sleepState;
        if (s == 0 && g_clock.time > behavior->wakeTime) SetBhvSleepState(behavior, 1);
    } while (behavior->sleepState != s);
    {
        STEPGUARD *t = (STEPGUARD *)behavior->owner;
        CID147 *v = t->heldNoise;
        v->position = GetLoWorldPos(t);
    }
}
} // namespace M_func_0017A670

// 0x17A6F8 InitBhvThrown
namespace M_func_0017A6F8 {

void InitBhv(BHV176 *p) __asm__("InitBhv");
void AddStepguardSgf(void *p, int n) __asm__("AddStepguardSgf");
// 0x17A6F8: Thrown behaviour (BHV176) vtable slot 7 (OnPush): raises flag 0x16 (thrown), state -1.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvThrown(BHV176 *behavior)
{
    InitBhv(behavior);
    AddStepguardSgf(behavior->owner, 0x16);
    behavior->state = -1;
}
} // namespace M_func_0017A6F8

// 0x17A730 StartBhvThrown
namespace M_func_0017A730 {

extern qword D_002A2DD0 __asm__("D_002A2DD0");
void StartBhv(BHV176 *p) __asm__("StartBhv");
void DetachStepguard(STEPGUARD *t) __asm__("DetachStepguard");
void SetStepguardAttack(STEPGUARD *t, int a, int b, qword q) __asm__("SetStepguardAttack");
void FreeSoContacts(STEPGUARD *t) __asm__("FreeSoContacts");
void SetBhvBusy(BHV176 *p) __asm__("SetBhvBusy");
void ResetDetectorTargets(void *p) __asm__("ResetDetectorTargets");
void SetDetectorTargetStateById(void *p, int a, int b) __asm__("SetDetectorTargetStateById");
// 0x17A730: Thrown behaviour vtable slot 8 (Start): state 0; detaches, cancels the attack, frees contacts, busy,
// resets the detector and marks the thrower.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvThrown(BHV176 *behavior)
{
    StartBhv(behavior);
    SetBhvThrownState(behavior, 0);
    DetachStepguard(((STEPGUARD *)behavior->owner));
    SetStepguardAttack(((STEPGUARD *)behavior->owner), 0, 0, D_002A2DD0);
    FreeSoContacts(((STEPGUARD *)behavior->owner));
    SetBhvBusy(behavior);
    ResetDetectorTargets(((STEPGUARD *)behavior->owner)->pdetector);
    if (behavior->thrower)
        SetDetectorTargetStateById(((STEPGUARD *)behavior->owner)->pdetector, behavior->thrower, 2);
}
} // namespace M_func_0017A730

// 0x17A7B8 CloseBhvThrown
// 0x17A7B8: Thrown behaviour vtable slot 9 (Close): state -1, lowers flag 0x16.
// Also known as: Exit (upstream readable / Sly 1 name).
void BHV176::Close()
{
    BHV176 *behavior = this;
    CloseBhv(behavior);
    SetBhvThrownState(behavior, -1);
    RemoveStepguardSgf((STEPGUARD *)behavior->owner, 0x16);
}

// 0x17A7F8 UpdateBhvThrown
namespace M_func_0017A7F8 {
extern float D_002BE6C0 __asm__("D_002BE6C0");
void UpdateBhv(BHV176 *p) __asm__("UpdateBhv");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
CID147 *NewNoise(void *p, qword q, STEPGUARD *t) __asm__("NewNoise");
// 0x17A7F8: Thrown behaviour vtable slot 10 (Update): ends after landing (states 2/3); emits a yelling noise
// periodically while airborne.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvThrown(BHV176 *behavior) {
    int s;
    UpdateBhv(behavior);
    do {
        s = behavior->state;
        if (s < 4) { if (s >= 2 && ((STEPGUARD *)behavior->owner)->floorContact) SetBhvThrownState(behavior, -1); }
    } while (s != behavior->state);
    if (s == 1 || s == 4) {
        if (g_clock.time - behavior->lastYellTime > D_002BE6C0) {
            SW *g = &g_sw;
            behavior->lastYellTime = g_clock.time;
            NewNoise(g->apstimk[0], GetLoWorldPos(((STEPGUARD *)behavior->owner)), ((STEPGUARD *)behavior->owner))->level = 3;
        }
    }
}
} // namespace M_func_0017A7F8

// 0x17A8C8 SetBhvThrownState
namespace M_func_0017A8C8 {
void PopBhv(void *p) __asm__("PopBhv");
void ClearBhvBusy(BHV176 *p) __asm__("ClearBhvBusy");
BHVANIM *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
// 0x17A8C8: Thrown behaviour state switch: pushes the animation for the state (flags 18/19/20/21/19 for states 0-4)
// and sets the airborne flag for states 0-1; -1 clears busy.
// Also known as: SetState (upstream readable / Sly 1 name; SetState__5JOBB0i).
extern "C" void SetBhvThrownState(BHV176 *behavior, int n) {
    if (behavior->state == n) return;
    PopBhv(&behavior->pbhvNext);
    behavior->state = n;
    behavior->stateTime = g_clock.time;
    switch (n) {
    case 0: case 1: case 2: case 3: case 4:
        {
            BHVANIM *o = PushStepBehavior(behavior->owner, CID_BHVANIM);
            int a[5] = { 18, 19, 20, 21, 19 };
            o->sgf = a[behavior->state];
            o->Start();
        }
        ((STEPGUARD *)behavior->owner)->isInvulnerable32C = (unsigned)behavior->state < 2;
        break;
    case -1:
        ClearBhvBusy(behavior);
        ((STEPGUARD *)behavior->owner)->isInvulnerable32C = 0;
        break;
    }
}
} // namespace M_func_0017A8C8

// 0x17A9C0 IsBhvThrownDone
// 0x17A9C0: True if the thrown behaviour has finished (state -1).
extern "C" int IsBhvThrownDone(BHV176 *behavior)
{
    return behavior->state == -1;
}

// 0x17A9D0 InitBhvjuggled
// 0x17A9D0: BHVJUGGLED (held/carried) vtable slot 7 (OnPush): BHVANIM OnPush with the held flag (0x17).
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvjuggled(BHVJUGGLED *behavior)
{
    InitBhvanim(behavior);
    behavior->sgf = 0x17;
    behavior->juggleState = -1;
}

// 0x17AA08 StartBhvjuggled
namespace M_StartBhvjuggled {

extern qword D_002A2DD0 __asm__("D_002A2DD0");
void StartBhvanim(BHVJUGGLED *p) __asm__("StartBhvanim");
void SetStepguardAttack(STEPGUARD *t, int a, int b, qword q) __asm__("SetStepguardAttack");
void DetachStepguard(STEPGUARD *t) __asm__("DetachStepguard");
void SetBhvBusy(BHVJUGGLED *p) __asm__("SetBhvBusy");
void SetDetectorTargetStateById(void *p, LO *a, int b) __asm__("SetDetectorTargetStateById");
// 0x17AA08: BHVJUGGLED vtable slot 8 (Start): BHVANIM Start, cancels the attack, detaches, state 0 (attached to the
// holder's hold point), busy.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvjuggled(BHVJUGGLED *behavior)
{
    StartBhvanim(behavior);
    SetStepguardAttack(((STEPGUARD *)behavior->owner), 0, 0, D_002A2DD0);
    DetachStepguard(((STEPGUARD *)behavior->owner));
    SetBhvjuggledState(behavior, 0);
    SetBhvBusy(behavior);
    SetDetectorTargetStateById(((STEPGUARD *)behavior->owner)->pdetector, behavior->holder, 2);
}
} // namespace M_StartBhvjuggled

// 0x17AA78 CalculateBhvjuggledHoldPoint
namespace M_func_0017AA78 {
typedef union {qword q;float g[4];} U;
extern qword g_normalZ __asm__("g_normalZ"),D_002A2DA0 __asm__("D_002A2DA0");
extern float D_002BE6C4 __asm__("D_002BE6C4");
qword GetLoWorldPos(void*) __asm__("GetLoWorldPos");
// VU0 helper: moves a CPU quadword into a VU register.
extern "C" { static inline qword ToVU(qword word){qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(word));return r;} }
// VU0 helper: vector difference.
// VU0 helper: a minus its projection on n.
extern "C" { static inline qword Reject(qword a, qword n)
{
    qword d, one;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "=&j"(d), "=&j"(one)
            : "j"(a), "j"(n));
    __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(d) : "j"(n), "j"(d));
    return VuSub(a, d);
} }
// VU0 helper: normalized a, or `fallback` for a zero vector.
extern "C" { static inline qword Normal(qword a, qword fallback)
{
    register qword d __asm__("vf1");
    qword one;
    float len;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "=&j"(d), "=&j"(one)
            : "j"(a));
    __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(len) : "j"(d));
    if (len < 0.0001f) {
        d = fallback;
    } else {
        __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q" : "=j"(d) : "j"(d), "j"(a));
    }
    return d;
} }

// VU0 helper: offset point along a direction and up axis (register-pinned asm).
extern "C" { static inline qword NativePoint(qword point, qword direction, qword along, qword up, qword height, unsigned int *high)
{
    register unsigned int h __asm__("$2");
    __asm__("vadda.xyzw ACC,%0,$vf0\n\tvmaddax.xyzw ACC,%2,%3x\n\tvmaddx.xyzw %0,%4,%5x\n\tlui %1,%%hi(%6)"
            : "+j"(point), "=r"(h)
            : "j"(direction), "j"(along), "j"(up), "j"(height), "i"(&D_002BE6C4));
    *high = h;
    return point;
} }
// VU0 helper: heading point: point + up * height (register-pinned asm).
extern "C" { static inline qword NativeHeading(qword point, qword up, qword height)
{
    __asm__("vadda.xyzw ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x" : "+j"(point) : "j"(up), "j"(height));
    return point;
} }
// Stores a point and a scalar (register-pinned asm for the match).
extern "C" { static inline void PublishPoint(U *home, qword scalar, qword *out, qword point)
{
    __asm__ volatile("sqc2 %2,%0\n\tsqc2 %3,%1" : "=m"(home->q), "=m"(*out) : "j"(scalar), "j"(point));
} }
// 0x17AA78: Computes the hold point and direction from the holder's request (its vtable slot 96) relative to the
// guard.
// Also known as: GetTarget (upstream readable / Sly 1 name; GetTarget__5JOBB1P12LAUNCHTARGETP6VECTORT2).
extern "C" void CalculateBhvjuggledHoldPoint(BHVJUGGLED *behavior, STUNHOLD *request, qword *outPoint, qword *outDirection)
{
    qword first = ToVU(GetLoWorldPos(behavior->owner));
    qword direction = Normal(Reject(VuSub(first, GetLoWorldPos(behavior->holder)), g_normalZ), D_002A2DA0);
    {
        U scalar;
        register unsigned int addressWord __asm__("$2") = (uptr)&g_normalZ;
        register qword along __asm__("vf4"), up __asm__("vf5"), point __asm__("vf2"), height __asm__("vf6");
        __asm__("lw $3,%5\n\tlqc2 %2,0(%0)\n\tqmtc2.ni $3,%1\n\tlw %0,%6\n\tlqc2 %3,%7\n\tqmtc2.ni %0,%4"
                : "+&r"(addressWord), "=j"(along), "=j"(up), "=j"(point), "=j"(height)
                : "m"(request->gSide), "m"(request->gUp), "m"(request->qwPos)
                : "$3", "memory");
        {
            unsigned int high;
            qword result = NativePoint(point, direction, along, up, height, &high);
            register unsigned int constantWord __asm__("$2") = high;
            register qword heading __asm__("vf1"), constant __asm__("vf3");
            __asm__("lqc2 %1,%3\n\tlw %0,%%lo(%4)(%0)\n\tqmtc2.ni %0,%2"
                    : "+r"(constantWord), "=j"(heading), "=j"(constant)
                    : "m"(request->qwVel), "i"(&D_002BE6C4)
                    : "memory");
            PublishPoint(&scalar, along, outPoint, result);
            heading = NativeHeading(heading, up, constant);
            PublishPoint(&scalar, height, outDirection, heading);
            scalar.q = constant;
        }
    }
}

typedef char qword_Size[(sizeof(qword)==16)?1:-1];
typedef char U_Size[(sizeof(U)==16)?1:-1];

typedef char pointer_Width[sizeof(void*)==4?1:-1];
typedef char word_Width[sizeof(unsigned int)==4?1:-1];
typedef char integer_Width[sizeof(int)==4?1:-1];
typedef char float_Width[sizeof(float)==4?1:-1];
typedef char qword_Alignment[__alignof__(qword)==16?1:-1];
typedef char U_Alignment[__alignof__(U)==16?1:-1];
typedef char U_q_Offset[(unsigned)&((U*)0)->q==0?1:-1];
typedef char U_scalar_Offset[(unsigned)&((U*)0)->g==0?1:-1];
} // namespace M_func_0017AA78

// 0x17ABC8 UpdateBhvjuggledHoldAct
namespace M_func_0017ABC8 {
void CalculateBhvjuggledHoldPoint(BHVJUGGLED *p, STUNHOLD *r, qword *a, qword *b) __asm__("CalculateBhvjuggledHoldPoint");
qword TransformXfPos(XF *a, XF *b, qword q) __asm__("TransformXfPos");
qword ConvertXfVec(XF *a, XF *b, qword q) __asm__("ConvertXfVec");
void SetRestActPose(ACT8 *pact, qword a, qword b) __asm__("SetRestActPose");
// 0x17ABC8: Re-queries the holder and updates the attach act's target.
// Also known as: UpdateTarget (upstream readable / Sly 1 name).
extern "C" void UpdateBhvjuggledHoldAct(BHVJUGGLED *behavior) {
    STUNHOLD r;
    qword holdPoint, holdDirection;
    XF *w;
    qword a;
    if (!behavior->unknown60) return;
    ((STEPPLAYER *)behavior->holder)->GetStunHold(&r, ((ACT8 *)behavior->holdAct)->arrivalTime - g_clock.time);
    if (!r.fValid) { behavior->unknown60 = 0; return; }
    w = ((STEPGUARD *)behavior->owner)->positionXf;
    CalculateBhvjuggledHoldPoint(behavior, &r, &holdPoint, &holdDirection);
    a = TransformXfPos(0, w->parent, holdPoint);
    SetRestActPose((ACT8 *)behavior->holdAct, a, ConvertXfVec(0, w->parent, holdDirection));
}
} // namespace M_func_0017ABC8

// 0x17AC90 UpdateBhvjuggled
namespace M_UpdateBhvjuggled {
struct Y; struct X;
// Update the movement-state controller.
extern float D_002BE6C8 __asm__("D_002BE6C8"), D_002BE6CC __asm__("D_002BE6CC");
struct Y { char pad[0x60]; void *frame; };
struct X { char pad[0x19A4]; struct Y *unknown19A4; };
void UpdateBhvanim(BHVJUGGLED *p) __asm__("UpdateBhvanim");
void UpdateBhvjuggledHoldAct(BHVJUGGLED *p) __asm__("UpdateBhvjuggledHoldAct");
qword GetLoWorldPos(void *a) __asm__("GetLoWorldPos");
void LoadIdentityMatrix4(void *a, void *b, MATRIX4Q *pm, MATRIX4Q *other, float g) __asm__("LoadIdentityMatrix4");
// Returns the frame object of the actor's +0x19A4 record.
extern "C" { static inline void*Frame(struct X*x){register struct Y*y __asm__("$2")=x->unknown19A4;return y->frame;} }
// VU0 helper: z component of a vector.
// 0x17AC90: BHVJUGGLED vtable slot 10 (Update): held/carried states 0-1, released/falling states 2-3 (until
// landed), prediction states 4-5.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvjuggled(BHVJUGGLED *behavior) {
    qword v;
    int s;
    UpdateBhvanim(behavior);
    v = behavior->owner->GetVelocity();
    do {
        s = behavior->juggleState;
        switch (s) {
        case 0:
        case 1:
            if (behavior->juggleState == 1 && behavior->owner->floorContact) {
                SetBhvjuggledState(behavior, 3);
            } else if (behavior->owner->positionXf->firstActuator != (ACT *)behavior->holdAct || ((ACT8 *)behavior->owner->positionXf->firstActuator)->arrivalTime < g_clock.time) {
                SetBhvjuggledState(behavior, 2);
            } else if (QwordZ(v) < 0.0f && behavior->juggleState == 0) {
                SetBhvjuggledState(behavior, 1);
            } else {
                UpdateBhvjuggledHoldAct(behavior);
            }
            break;
        case 2:
            if (behavior->owner->floorContact) SetBhvjuggledState(behavior, 3);
            break;
        case 3:
            break;
        case 4:
        case 5: {
            MATRIX4Q out;
            QWROW a, b, t, vel, ud;
            qword tq, c;
            float dt;
            register void*nullSource __asm__("$5");
            register void*nullOutput __asm__("$7");
            register struct X*frameLink __asm__("$3");
            CLOCK *clock=&g_clock;
            a.q = GetLoWorldPos(behavior->owner);
            {
                qword velocity = behavior->owner->GetVelocity();
                dt=clock->deltaTime;
                /* Preserve velocity and prepare the prediction inputs. */
#ifdef SLY2_EE_ASM
                __asm__("sq %4,%0\n\tdaddu %1,%z5,$zero\n\tdaddu %2,%z5,$zero\n\tlw %3,%6" : "=m"(b.q),"=&r"(nullOutput),"=&r"(nullSource),"=&r"(frameLink) : "r"(velocity),"J"(0),"m"(((struct X *)behavior->holder)));
#else
                b.q = velocity; nullOutput = 0; nullSource = 0; frameLink = (struct X *)behavior->holder;
#endif
            }
            LoadIdentityMatrix4(Frame(frameLink), nullSource, &out, (MATRIX4Q *)nullOutput, dt);
            t.q = out.r[3].q;
            t.f[2] += D_002BE6C8;
            tq = t.q;
            c = GetLoWorldPos(behavior->owner);
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(c) : "j"(tq), "j"(c));
            clock=(CLOCK*)*(int*)&clock->deltaTime;
            { qword ud_q; __asm__("qmtc2.ni %1, %0" : "=j"(ud_q) : "r"(clock)); ud.q = ud_q; }
            { qword vel_q; __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(vel_q) : "j"(ud.q), "j"(c)); vel.q = vel_q; }
            if (behavior->juggleState == 4) {
                if (a.f[2] < t.f[2] || D_002BE6CC < g_clock.time - behavior->stateTime) {
                    SetBhvjuggledState(behavior, 5);
                    break;
                }
                vel.f[2] = b.f[2];
            }
            behavior->owner->SetVelocity(vel.q);
            break;
        }
        case 6:
            if (behavior->owner->floorContact) SetBhvjuggledState(behavior, -1);
            break;
        }
    } while (behavior->juggleState != s);
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_U[(sizeof(QWROW)==16)?1:-1];
typedef char layout_X[(sizeof(struct X)==0x19A8)?1:-1];
typedef char layout_Y[(sizeof(struct Y)==0x64)?1:-1];
typedef char layout_U_q[((unsigned)&((QWROW*)0)->q==0)?1:-1];
typedef char layout_U_f[((unsigned)&((QWROW*)0)->f==0)?1:-1];
typedef char layout_X_y[((unsigned)&((struct X*)0)->unknown19A4==0x19A4)?1:-1];
typedef char layout_Y_frame[((unsigned)&((struct Y*)0)->frame==0x60)?1:-1];
} // namespace M_UpdateBhvjuggled

// 0x17AF30 HandleBhvjuggledMessage
namespace M_BHVJUGGLED_FHandleMessage {
 struct X;
extern void *D_002BE6D0[] __asm__("D_002BE6D0");
extern int D_002BE6E8[] __asm__("D_002BE6E8");
   /* an EXPL */
struct X { int a; int pad[7]; qword v; qword pad2; };
int IsAnimBindingDone(ANIMBINDING *c) __asm__("IsAnimBindingDone");
void StopAnimBinding(ANIMBINDING *c) __asm__("StopAnimBinding");
void StartAnimBinding(ANIMBINDING *c, STEPGUARD *a, int fl, float g) __asm__("StartAnimBinding");
float *FindAsegLabel(void *p, void *q) __asm__("FindAsegLabel");
void SeekAsega(void *p, float a, float b, int n) __asm__("SeekAsega");
struct EXPL *FindSwObject__FP2SWi3OIDP2LO(SW *g, int a, int b, LO *c) __asm__("FindSwObject__FP2SWi3OIDP2LO");
void *func_002333B4(void *p, int c, int n) __asm__("memset");
qword GetLoWorldPos(STEPGUARD *a) __asm__("GetLoWorldPos");
void NewNoise(void *p, qword v, STEPGUARD *a) __asm__("NewNoise");
// 0x17AF30: BHVJUGGLED vtable slot 13 (HandleMessage): BHVANIM events; ASEG event 0x3D resets the hit count; a
// 0x200000 zap while held is an extra hit (animation seek, effect, hit value, noise). The payload is the event
// (message 9) or the zap (message 0xC).
// Also known as: Message (upstream readable / Sly 1 name; Message__5JOBB1iPv).
extern "C" int HandleBhvjuggledMessage(BHVJUGGLED *behavior, int msg, ASEGEVENTMSG *pm) {
    if (HandleBhvanimMessage(behavior, msg, pm)) return 1;
    switch (msg) {
    case 9:
        if (behavior->binding == 0) return 0;
        if (GetAnimBindingInstance(behavior->binding) != pm->asega) return 0;
        if (pm->event == 0x3D) behavior->hitCount = 0;
        return 0;
    case 0xC:
        {
            int i, n;
            float g;
            SW *pg;
            struct EXPL *o;
            STEPGUARD *a;
            if (((ZPR *)pm)->zpk != 0x200000) return 0;
            if (!(((ZPR *)pm)->grfzpr & 8)) return 0;
            if (!behavior->juggleState) return 0;
            if (IsAnimBindingDone(behavior->binding)) {
                StopAnimBinding(behavior->binding);
                StartAnimBinding(behavior->binding, ((STEPGUARD *)behavior->owner), behavior->binding->flags | 1, 0.0f);
            }
            behavior->hitCount++;
            g = 0.0f;
            i = 4; if (behavior->hitCount <= 3) i = behavior->hitCount;
            while (i > 0) {
                float *r = FindAsegLabel(behavior->binding->aseg, D_002BE6D0[i]);
                if (r) { g = *r; break; }
                i--;
            }
            SeekAsega(behavior->binding->asega, g, 1.0f, 0);
            SetBhvjuggledState(behavior, 0);
            o = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 5, D_002BE6E8[i], 0);
            if (o) {
                struct X x;
                func_002333B4(&x, 0, sizeof x);
                x.a = 5;
                x.v = GetLoWorldPos(((STEPGUARD *)behavior->owner));
                ((EXPL *)o)->ExplodeExplso((EXPLSO *)&x);   /* X: EXPLSO grf 5 and origin */
            }
            a = ((STEPGUARD *)behavior->owner);
            {
                int m = a->hitPoints + ((ZPR *)pm)->dhp;
                a->hitPoints = m < 5 ? 5 : m;
            }
            NewNoise(g_sw.apstimk[2], GetLoWorldPos(((STEPGUARD *)behavior->owner)), ((STEPGUARD *)behavior->owner));
            return 1;
        }
    }
    return 0;
}
} // namespace M_BHVJUGGLED_FHandleMessage

// 0x17B178 CloseBhvjuggled
// 0x17B178: BHVJUGGLED vtable slot 9 (Close): state -1 (restores the mass), BHVANIM Close.
// Also known as: Exit (upstream readable / Sly 1 name).
void BHVJUGGLED::Close()
{
    BHVJUGGLED *behavior = this;
    SetBhvjuggledState(behavior, -1);
    CloseBhvanim(behavior);
}

// 0x17B1A8 SetBhvjuggledState
namespace M_SetBhvjuggledState {
union U; union F; struct VOBJ; struct GROUP; struct ENTRY;
// Movement-state transitions and actor response.
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
// Copy the 128-bit value.
union U{qword q;float f[4];int bits[4];void Assign(const U&v){q=v.q;}};
union F{float value;int bits;};
struct GROUP{char pad[0xA0];void*space;};
struct VOBJ{void *scriptFrame; void *scriptVariables; void *firstSubscriber;VTE*vt;};   /* the guard's vtable as BlendSet's asm reads it */
struct ENTRY{float value;int kind,key,pad;};
struct DELTA;

 extern float D_002BE6FC __asm__("D_002BE6FC"),D_002BE700 __asm__("D_002BE700"),D_002BE704 __asm__("D_002BE704");
 extern F D_002BE708 __asm__("D_002BE708");
 extern U g_normalZ __asm__("g_normalZ");
 void SetSoMass(STEP*,float) __asm__("SetSoMass");
 void SeekAsega(ASEGA*,int,float,float) __asm__("SeekAsega");
 void ReleaseAct(DELTA**) __asm__("ReleaseAct");
 void SetXfRole(STEP*,GROUP*,int) __asm__("SetXfRole");
 void RefreshSwObjectCollisionFilter(void*,STEP*) __asm__("RefreshSwObjectCollisionFilter");
 void FreeSoContacts(STEP*) __asm__("FreeSoContacts");
 void SetBhvanimState(BHVJUGGLED*,int) __asm__("SetBhvanimState");
 void CalculateBhvjuggledHoldPoint(BHVJUGGLED*,STUNHOLD*,U*,U*) __asm__("CalculateBhvjuggledHoldPoint");
 qword GetLoWorldPos(void*) __asm__("GetLoWorldPos");
 qword TransformXfPos(void*,void*,qword) __asm__("TransformXfPos");
 qword ConvertXfVec(void*,void*,qword) __asm__("ConvertXfVec");
 DELTA*NewBezierAct(STEP*,GROUP*,void*,float,qword,qword) __asm__("NewBezierAct");
 LO*GetHeroPo(void*) __asm__("GetHeroPo");
 void TriggerJoyRumbleRumk__FP3JOY4RUMKf(void*,int,float) __asm__("TriggerJoyRumbleRumk__FP3JOY4RUMKf");
 ENTRY*FindAsegLabel(void*,int) __asm__("FindAsegLabel");

// Matching helper (register-pinned asm): blends the guard toward a desired vector with a gain from .data; kept as asm for the match.
static inline void BlendSet(BHVJUGGLED *p, qword current, U *desired, U *scale, U *scratch)
{
    register VOBJ *actor __asm__("$7");
    register VTE *vt __asm__("$6");
    register int word __asm__("$4");
    register qword a __asm__("vf1"), b __asm__("vf2"), d __asm__("vf3"), lift __asm__("vf2"), axis __asm__("vf1"),
        k __asm__("vf4");
    const unsigned gainAddress = 0x2BE708;
    register char *gainBase __asm__("$4");
    __asm__("lw %0,%4\n\tqmtc2.ni %5,%1\n\tlqc2 %2,%6\n\tlui %3,%7"
            : "=&r"(actor), "=j"(b), "=j"(a), "=r"(gainBase)
            : "m"(p->owner), "r"(current), "m"(desired->q), "i"((gainAddress + 0x8000) >> 16));
    const unsigned axisLocation = 0x2A2DC0;
    register char *axisBase __asm__("$3");
    __asm__("lw %0,%4\n\tvsub.xyzw %1,%5,%6\n\tlw %2,%7\n\tlui %3,%8"
            : "=&r"(vt), "=j"(d), "=r"(word), "=r"(axisBase)
            : "m"(actor->vt), "j"(a), "j"(b), "m"(*(int *)(gainBase + (short)(gainAddress & 0xffff))),
              "i"((axisLocation + 0x8000) >> 16));
    register U *axisAddress __asm__("$3");
    __asm__("qmtc2.ni %2,%0\n\taddiu %1,%3,%4"
            : "=j"(lift), "=r"(axisAddress)
            : "r"(word), "r"(axisBase), "i"((short)(axisLocation & 0xffff)));
    register int adjustment __asm__("$4");
    register void (*setter)() __asm__("$3");
    __asm__("lqc2 %0,%4\n\tlh %1,%5\n\tlqc2 %2,%6\n\tlw %3,%7"
            : "=j"(axis), "=r"(adjustment), "=j"(k), "=r"(setter)
            : "m"(axisAddress->q), "m"(vt[35].d), "m"(scale->q), "m"(((void (*)())vt[35].pfn)));
    register qword packed __asm__("$5");
    register VOBJ *adjusted __asm__("$4");
    __asm__("vmulax.xyzw $ACC,%3,%0x\n\tvmaddx.xyzw %0,%4,%5x\n\tqmfc2.ni %1,%0\n\taddu %2,%6,%7"
            : "+j"(k), "=r"(packed), "=r"(adjusted)
            : "j"(d), "j"(axis), "j"(lift), "r"(actor), "r"(adjustment));
    scratch->q = lift;
    ((void (*)(VOBJ *, qword))setter)(adjusted, packed);
}
typedef char Pointer_Size[sizeof(void*)==4?1:-1];
typedef char Int_Size[sizeof(int)==4?1:-1];
typedef char Float_Size[sizeof(float)==4?1:-1];
typedef char Short_Size[sizeof(short)==2?1:-1];
typedef char U_Alignment[__alignof__(U)==16?1:-1];
typedef char GROUP_Size[sizeof(GROUP)==0xA4?1:-1];
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char F_Size[sizeof(F)==4?1:-1];
typedef char ENTRY_Size[sizeof(ENTRY)==16?1:-1];
typedef char GROUP_space_Offset[FIELD_OFFSET(GROUP,space)==160?1:-1];
typedef char ENTRY_value_Offset[FIELD_OFFSET(ENTRY,value)==0?1:-1];
typedef char ENTRY_kind_Offset[FIELD_OFFSET(ENTRY,kind)==4?1:-1];
typedef char ENTRY_key_Offset[FIELD_OFFSET(ENTRY,key)==8?1:-1];
// True for the motionless modes (0, 4, 5).
static inline int Still(int m){int f=0;if(m==4||m==5||m==0)f=1;return f;}
// 0x17B1A8: BHVJUGGLED state switch: attaches an act to the holder, releases acts and XF roles, changes the mass
// and seeks animation labels (0x3C/0x173).
// Also known as: SetState (upstream readable / Sly 1 name; SetState__5JOBB1i).
extern "C" void SetBhvjuggledState(BHVJUGGLED *behavior, int next)
{
    STUNHOLD request;
    U point, direction, scratch, scale, cached;
    if (next == behavior->juggleState)
        return;
    switch (behavior->juggleState) {
    case -1:
        SetSoMass(behavior->owner, D_002BE6FC * behavior->owner->physics->m);
        break;
    case 3:
        if (next != -1)
            SeekAsega(behavior->binding->asega, 0, 0.0f, 1.0f);
        break;
    }
    if ((unsigned)behavior->juggleState < 2 && (unsigned)next >= 2) {
        ReleaseAct((DELTA **)&behavior->holdAct);
        SetXfRole(behavior->owner, (GROUP *)behavior->owner->positionXf, 2);
        SetXfRole(behavior->owner, (GROUP *)behavior->owner->rotationXf, 2);
    }
    int was = Still(behavior->juggleState), now = Still(next);
    behavior->juggleState = next;
    if (was != now) {
        const unsigned gameAddress = 0x3DD490;
        register char *gameBase __asm__("$4");
        register STEP *callbackActor __asm__("$5");
        __asm__("lui %0,%2\n\tlw %1,%3"
                : "=&r"(gameBase), "=r"(callbackActor)
                : "i"((gameAddress + 0x8000) >> 16), "m"(behavior->owner));
        RefreshSwObjectCollisionFilter(gameBase + (short)(gameAddress & 0xffff), callbackActor);
        FreeSoContacts(behavior->owner);
        behavior->juggleState = next;
    }
    const unsigned clockAddress = 0x2DDED0;
    char *clockHigh;
    register char *clockCarrier __asm__("$19");
    register float newTime __asm__("$f0");
    __asm__("lui %0,%1" : "=r"(clockHigh) : "i"((clockAddress + 0x8000) >> 16));
    __asm__("daddu %0,%2,$0\n\tlwc1 %1,%3"
            : "=&r"(clockCarrier), "=f"(newTime)
            : "r"(clockHigh), "m"(*(float *)(clockHigh + (short)(clockAddress & 0xffff))));
    behavior->stateTime = newTime;
    switch (next) {
    case -1:
        SetSoMass(behavior->owner, behavior->owner->physics->m / D_002BE6FC);
        SetBhvanimState(behavior, 1);
        break;
    case 0: {
        ((STEPPLAYER *)behavior->holder)->GetStunHold(&request, D_002BE700);
        CalculateBhvjuggledHoldPoint(behavior, &request, &point, &direction);
        register qword scalar __asm__("$2");
        register STEP *beforeActor __asm__("$4");
        register float duration __asm__("$f1") = D_002BE700;
        register float factor __asm__("$f0") = D_002BE704;
        __asm__("lq %0,%4\n\tlw %1,%5\n\tdiv.s %2,%2,%6\n\tsq %0,%3\n\tmfc1 %0,%2"
                : "=&r"(scalar), "=&r"(beforeActor), "+f"(factor), "=m"(cached.q)
                : "m"(point.q), "m"(behavior->owner), "f"(duration));
        scratch.q = scalar;
        scale = scratch;
        qword current = GetLoWorldPos(beforeActor);
        BlendSet(behavior, current, &cached, &scale, &scratch);
        GROUP *first = (GROUP *)behavior->owner->positionXf;
        if (!((DELTA *)behavior->holdAct)) {
            float end = *(float *)(clockCarrier + (short)(clockAddress & 0xffff)) + D_002BE700;
            qword localPoint = TransformXfPos(0, first->space, point.q);
            qword localDirection = ConvertXfVec(0, first->space, direction.q);
            *(DELTA **)&behavior->holdAct = NewBezierAct(behavior->owner, first, 0, end, localPoint, localDirection);
        }
        int owned = 0;
        if (request.fValid)
            owned = ((STEPPLAYER *)behavior->holder)->interactTarget == behavior->owner;
        behavior->unknown60 = owned;
        if (((STEPGUARD *)behavior->owner)->unknown334) {
            LO *focus = GetHeroPo(&g_sw);
            if (focus && behavior->holder == focus)
                TriggerJoyRumbleRumk__FP3JOY4RUMKf(&g_joy, 3, 0.15f);
        }
        break;
    }
    case 3: {
        ENTRY *entry = FindAsegLabel(behavior->binding->aseg, 0x3C);
        if (entry)
            SeekAsega(behavior->binding->asega, 0, entry->value, 1.0f);
        break;
    }
    case 4:
    case 5:
        if (behavior->juggleState == 4) {
            ENTRY *entry = FindAsegLabel(behavior->binding->aseg, 0x173);
            if (entry)
                SeekAsega(behavior->binding->asega, 0, entry->value, 1.0f);
        }
        break;
    case 1:
    case 2:
        break;
    }
}
} // namespace M_SetBhvjuggledState

// 0x17B578 GetBhvjuggledGroundProbeFlags
namespace M_BHVJUGGLED_GrfGroundProbe {

// 0x17B578: BHVJUGGLED vtable slot 12 (ground-probe flags): 7 once landed (state 3), else 1.
extern "C" int GetBhvjuggledGroundProbeFlags(BHVJUGGLED *behavior) { return behavior->juggleState != 3 ? 1 : 7; }
} // namespace M_BHVJUGGLED_GrfGroundProbe

// 0x17B590 InitBhvshoot
namespace M_InitBhvshoot {

void InitBhv(BHVSHOOT *p) __asm__("InitBhv");
// 0x17B590: BHVSHOOT vtable slot 7 (OnPush): state -1.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvshoot(BHVSHOOT *behavior)
{
    InitBhv(behavior);
    behavior->state = -1;
}
} // namespace M_InitBhvshoot

// 0x17B5C0 StartBhvshoot
// 0x17B5C0: BHVSHOOT vtable slot 8 (Start): state 0.
// Also known as: Enter (upstream readable / Sly 1 name).
void BHVSHOOT::Start()
{
    BHVSHOOT *behavior = this;
    StartBhv(behavior);
    SetBhvshootState(behavior, 0);
}

// 0x17B5F0 CloseBhvshoot
// 0x17B5F0: BHVSHOOT vtable slot 9 (Close): state -1.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvshoot(BHVSHOOT *behavior)
{
    CloseBhv(behavior);
    SetBhvshootState(behavior, -1);
}

// 0x17B620 UpdateBhvshoot
// 0x17B620: BHVSHOOT vtable slot 10 (Update): finishes when the animation child is done.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvshoot(BHVSHOOT *behavior)
{
    int nState;
    UpdateBhv(behavior);
    while (IsBhvBusyAbove(behavior) == 0) {
        nState = behavior->state;
        if ((nState == 0) && (IsBhvanimDone((BHVANIM *)behavior->pbhvNext) != 0)) {
            SetBhvshootState(behavior, -1);
        }
        if (nState == behavior->state) {
            break;
        }
    }
}

// 0x17B690 HandleBhvshootMessage
namespace M_BHVSHOOT_FHandleMessage {
// 0x17B690: BHVSHOOT vtable slot 13 (HandleMessage): ASEG events 0x31/0x32 open/close the firing window (+0xF60).
// Also known as: Message (upstream readable / Sly 1 name; Message__8JOBSHOOTiPv).
extern "C" int HandleBhvshootMessage(BHVSHOOT *behavior, int msg, ASEGEVENTMSG *pm) {
    if (msg == 9 && !behavior->state && ((BHVANIM *)behavior->pbhvNext)->binding && GetAnimBindingInstance(((BHVANIM *)behavior->pbhvNext)->binding) == pm->asega) {
        switch (pm->event) {
        case 0x31: ((STEPGUARD *)behavior->owner)->firingWindow = 1; return 1;
        case 0x32: ((STEPGUARD *)behavior->owner)->firingWindow = 0; return 1;
        }
    }
    return HandleBhvMessage(behavior, msg, pm);
}
} // namespace M_BHVSHOOT_FHandleMessage

// 0x17B750 SetBhvshootState
namespace M_SetBhvshootState {

void PopBhv(void *p) __asm__("PopBhv");
BHVANIM *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
// 0x17B750: BHVSHOOT state change: 0 pushes the shooting animation (flag 0x11) aimed at the target and opens the
// firing state.
// Also known as: SetShootState (upstream readable / Sly 1 name; SetShootState__8JOBSHOOTi).
extern "C" void SetBhvshootState(BHVSHOOT *behavior, int n) {
    if (n == behavior->state) return;
    PopBhv((&behavior->pbhvNext));
    if (!behavior->state) ((STEPGUARD *)behavior->owner)->firingWindow = 0;
    behavior->state = n;
    behavior->stateTime = g_clock.time;
    if (n)
        return;
    BHVANIM *o = PushStepBehavior(((STEPGUARD *)behavior->owner), CID_BHVANIM);
    o->sgf = 0x11;
    o->lookAt = behavior->target;
    o->Start();
    ((STEPGUARD *)behavior->owner)->firingWindow = 1;
}
} // namespace M_SetBhvshootState

// 0x17B7F8 IsBhvshootDone
// 0x17B7F8: True if the shoot behaviour has finished (state -1).
extern "C" int IsBhvshootDone(BHVSHOOT *behavior)
{
    return behavior->state == -1;
}

// 0x17B808 InitBhvTargets
namespace M_func_0017B808 {

void InitBhv(BHV178 *p) __asm__("InitBhv");
void AddStepguardSgf(void *p, int n) __asm__("AddStepguardSgf");
// 0x17B808: Targets behaviour (BHV178) vtable slot 7 (OnPush): raises flag 0x20, state -1.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvTargets(BHV178 *behavior)
{
    InitBhv(behavior);
    AddStepguardSgf(behavior->owner, 0x20);
    behavior->state = -1;
}
} // namespace M_func_0017B808

// 0x17B840 StartBhvTargets
// 0x17B840: Targets behaviour vtable slot 8 (Start): state 0.
// Also known as: Enter (upstream readable / Sly 1 name).
void BHV178::Start()
{
    BHV178 *behavior = this;
    StartBhv(behavior);
    SetBhvTargetsState(behavior, 0);
}

// 0x17B870 CloseBhvTargets
// 0x17B870: Targets behaviour vtable slot 9 (Close): state -1, lowers flag 0x20.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvTargets(BHV178 *behavior)
{
    CloseBhv(behavior);
    SetBhvTargetsState(behavior, -1);
    RemoveStepguardSgf((STEPGUARD *)behavior->owner, 0x20);
}

// 0x17B8B0 SetBhvTargetsState
namespace M_func_0017B8B0 {
void PopBhv(void *p) __asm__("PopBhv");
BHV *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
void SetStepguardTarget(void *p, int n) __asm__("SetStepguardTarget");
// 0x17B8B0: Targets behaviour state change: 0 idle without a target, 1 combat (BHVCOMBAT) on the chosen target, 2
// hands-off idle.
// Also known as: SetB2State (upstream readable / Sly 1 name; SetB2State__5JOBB2i).
extern "C" void SetBhvTargetsState(BHV178 *behavior, int n)
{
    if (n == behavior->state)
        return;
    PopBhv(&behavior->pbhvNext);
    behavior->state = n;
    behavior->stateTime = g_clock.time;
    switch (n) {
    case 0: {
        BHVIDLE *o = (BHVIDLE *)PushStepBehavior(behavior->owner, CID_BHVIDLE);
        StartPushedBhv(o);
    }
        SetStepguardTarget(behavior->owner, 0);
        break;
    case 1: {
        BHVCOMBAT *o = (BHVCOMBAT *)PushStepBehavior(behavior->owner, CID_BHVCOMBAT);
        o->target = behavior->chosenTarget;
        o->isHitConfirmed = 1;
        StartPushedBhv(o);
    } break;
    case 2: {
        BHVIDLE *o = (BHVIDLE *)PushStepBehavior(behavior->owner, CID_BHVIDLE);
        o->sgf = 1;
        StartPushedBhv(o);
    } break;
    }
}
} // namespace M_func_0017B8B0

// 0x17B9C0 ChooseBhvTargetsTarget
namespace M_func_0017B9C0 {
union U; struct RANGE; struct POLY; struct WORK;
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
struct RANGE{float lo,hi;};
struct POLY{float a,b,c;};
struct WORK{float t,d,t2,d2;QWROW origin,bestPoint;};

 extern RANGE D_002BE740 __asm__("D_002BE740");
 extern POLY D_002BE720 __asm__("D_002BE720"),D_002BE730 __asm__("D_002BE730");
 float RandomFloatInRange__Fff(float,float) __asm__("RandomFloatInRange__Fff");
 qword GetLoWorldPos(void*) __asm__("GetLoWorldPos");
 void FindClosestPointOnLine(qword,qword,qword,qword*,float*,float*) __asm__("FindClosestPointOnLine");

// VU0 helper: moves a CPU quadword into a VU register.
static inline qword From(qword a){qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(a));return r;}
// VU0 helper: loads a quadword into a VU register.
static inline qword Load(QWROW*u){qword r;__asm__("lqc2 %0,%1":"=j"(r):"m"(u->q));return r;}
// VU0 helper: x of a VU register as a float.
// VU0 helper: length of a vector.
static inline float Length(qword v)
{
    register qword one __asm__("vf2");
    qword d = v;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "+j"(d), "=&j"(one)
            : "j"(v));
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "+j"(d));
    return VuToFloat(d);
}
// VU0 helper: distance from an origin to a point.
static inline float Distance(QWROW *origin, qword point)
{
    register qword a __asm__("vf2") = origin->q;
    qword b = point, d;
    __asm__("vsub.xyzw %0,%1,%2" : "=j"(d) : "j"(a), "j"(b));
    return Length(d);
}
// VU0 helper: distance from a point to an origin.
static inline float DistanceFrom(qword point, QWROW *origin)
{
    qword a = point, b = Load(origin), d;
    __asm__("vsub.xyzw %0,%1,%2" : "=&j"(d) : "j"(a), "j"(b));
    return Length(d);
}
// Identity (keeps a value in a register for the match).
static inline qword To(qword a){return a;}
// VU0 helper: stores a VU register to memory.
static inline void Store(QWROW*u,qword a){__asm__("sqc2 %1,%0":"=m"(u->q):"j"(a));}
// Clamps *x into a range.
static inline void ClampIn(float*x,RANGE*p){float lo=p->lo,hi=p->hi;if(*x<lo)*x=lo;else if(hi<*x)*x=hi;}
typedef char U_Size[sizeof(QWROW)==16?1:-1];
typedef char POLY_Size[sizeof(POLY)==12?1:-1];
typedef char RANGE_Size[sizeof(RANGE)==8?1:-1];
typedef char WORK_Size[sizeof(WORK)==48?1:-1];
typedef char WORK_t_Offset[FIELD_OFFSET(WORK,t)==0?1:-1];
typedef char WORK_d_Offset[FIELD_OFFSET(WORK,d)==4?1:-1];
typedef char WORK_t2_Offset[FIELD_OFFSET(WORK,t2)==8?1:-1];
typedef char WORK_d2_Offset[FIELD_OFFSET(WORK,d2)==12?1:-1];
typedef char WORK_origin_Offset[FIELD_OFFSET(WORK,origin)==16?1:-1];
typedef char WORK_bestPoint_Offset[FIELD_OFFSET(WORK,bestPoint)==32?1:-1];
typedef char POLY_a_Offset[FIELD_OFFSET(POLY,a)==0?1:-1];
typedef char POLY_b_Offset[FIELD_OFFSET(POLY,b)==4?1:-1];
typedef char POLY_c_Offset[FIELD_OFFSET(POLY,c)==8?1:-1];
typedef char RANGE_lo_Offset[FIELD_OFFSET(RANGE,lo)==0?1:-1];
typedef char RANGE_hi_Offset[FIELD_OFFSET(RANGE,hi)==4?1:-1];
// 0x17B9C0: Every random interval, picks the cheapest designated target (cost: distance, a bonus for preferred
// ones, a penalty near avoid objects) or a nearer avoid object within 800.
// Also known as: Choose (upstream readable / Sly 1 name).
extern "C" void ChooseBhvTargetsTarget(BHV178 *behavior)
{
    WORK work;
    LO *best;
    float lower;
    qword zero;
    float now = g_clock.time;
    if (now < behavior->nextChooseTime)
        return;
    // D_002BE710 holds the random-delay range.
    register unsigned int randomAddress __asm__("$2");
#ifdef SLY2_EE_ASM
    __asm__("vsub.xyzw %0,$vf0,$vf0\n\tlui %1,0x2c" : "=j"(zero), "=r"(randomAddress));
    __asm__("sqc2 %4,%0\n\tdaddu %2,%z5,$zero\n\tlwc1 %1,-0x18f0(%3)\n\taddiu %3,%3,-0x18f0"
            : "=m"(work.bestPoint.q), "=f"(lower), "=&r"(best), "+r"(randomAddress)
            : "j"(zero), "J"(0), "m"(*(float *)(randomAddress - 0x18f0)));
    behavior->nextChooseTime = now + RandomFloatInRange__Fff(lower, ((RANGE *)randomAddress)->hi);
#else
    extern RANGE delayRange __asm__("D_002BE710");   /* the random-delay range */
    zero = VuZero<qword>();
    work.bestPoint.q = zero;
    best = 0;
    lower = delayRange.lo;
    behavior->nextChooseTime = now + RandomFloatInRange__Fff(lower, delayRange.hi);
#endif
    work.origin.q = GetLoWorldPos(behavior->owner);
    // D_002A55FC supplies the initial selection cost.
    int i;
    STEPGUARD *actor;
    unsigned int limitHigh;
    {
        register int firstIndex __asm__("$5");
        register STEPGUARD *firstActor __asm__("$4");
        register unsigned int high __asm__("$3");
#ifdef SLY2_EE_ASM
        __asm__("lui %0,0x2a\n\tdaddu %1,$zero,$zero\n\tlw %2,%3"
                : "=&r"(high), "=&r"(firstIndex), "=r"(firstActor)
                : "m"(behavior->owner));
#else
        high = 0;   /* unused: bestCost reads D_002A55FC by name */
        firstIndex = 0;
        firstActor = (STEPGUARD *)behavior->owner;
#endif
        limitHigh = high;
        i = firstIndex;
        actor = firstActor;
    }
#ifdef SLY2_EE_ASM
    float bestCost = *(float *)(limitHigh + 0x55fc);
#else
    extern const float initialCost __asm__("D_002A55FC");   /* FLT_MAX */
    float bestCost = initialCost;
#endif
    if (actor->designatedTargetCount > 0)
        do {
            LO *choice = actor->designatedTargets[i];
            if ((int)choice->isInWorld & 1) {
                qword candidate = From(GetLoWorldPos(choice));
                float cost = Distance(&work.origin, candidate);
                if (choice == behavior->preferredTarget)
                    cost = D_002BE720.a + cost * (D_002BE720.b + cost * D_002BE720.c);
                for (int j = 0; j < ((STEPGUARD *)behavior->owner)->avoidObjectCount; j++) {
                    qword position = GetLoWorldPos(((STEPGUARD *)behavior->owner)->avoidObjects[j]);
                    FindClosestPointOnLine(position, work.origin.q, To(candidate), 0, &work.t, &work.d);
                    if (work.t < 0.0f)
                        continue;
                    if (work.t > 1.0f)
                        continue;
                    float penalty = D_002BE730.a + work.d * (D_002BE730.b + work.d * D_002BE730.c);
                    ClampIn(&penalty, &D_002BE740);
                    cost += penalty;
                }
                if (cost < bestCost) {
                    Store(&work.bestPoint, candidate);
                    bestCost = cost;
                    best = choice;
                }
            }
            ++i;
            actor = (STEPGUARD *)behavior->owner;
        } while (i < actor->designatedTargetCount);
    behavior->preferredTarget = best;
    LO *nearest = 0;
    float distance = 800.0f;
    for (int k = 0; k < ((STEPGUARD *)behavior->owner)->avoidObjectCount; k++) {
        LO *choice = ((STEPGUARD *)behavior->owner)->avoidObjects[k];
        qword position = From(GetLoWorldPos(choice));
        float f = DistanceFrom(position, &work.origin);
        if (f > distance)
            continue;
        if (best) {
            FindClosestPointOnLine(To(position), work.origin.q, work.bestPoint.q, 0, &work.t2, &work.d2);
            if (work.t2 < 0.0f)
                continue;
            if (work.t2 > 1.0f)
                continue;
            if (work.d2 > 500.0f)
                continue;
        }
        nearest = choice;
        distance = f;
    }
    behavior->nearAvoidObject = nearest;
    behavior->chosenTarget = nearest ? nearest : behavior->preferredTarget;
}

typedef char Pointer_Size[sizeof(void*)==4?1:-1];
typedef char Int_Size[sizeof(int)==4?1:-1];
typedef char UInt_Size[sizeof(unsigned int)==4?1:-1];
typedef char Float_Size[sizeof(float)==4?1:-1];
typedef char Long_Size[sizeof(unsigned long)==8?1:-1];
typedef char Qword_Size[sizeof(qword)==16?1:-1];
typedef char U_Alignment[__alignof__(QWROW)==16?1:-1];
typedef char WORK_Alignment[__alignof__(WORK)==16?1:-1];
} // namespace M_func_0017B9C0

// 0x17BD78 UpdateBhvTargets
namespace M_func_0017BD78 {
void UpdateBhv(BHV178 *p) __asm__("UpdateBhv");
int IsStepguardHandsOff(void *p) __asm__("IsStepguardHandsOff");
void ChooseBhvTargetsTarget(BHV178 *p) __asm__("ChooseBhvTargetsTarget");
int IsBhvBusyAbove(BHV178 *p) __asm__("IsBhvBusyAbove");
// 0x17BD78: Targets behaviour vtable slot 10 (Update): re-chooses and (re)engages the designated targets; pauses
// while hands-off.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvTargets(BHV178 *behavior) {
    int f, s;
    UpdateBhv(behavior);
    f = IsStepguardHandsOff(behavior->owner);
    ChooseBhvTargetsTarget(behavior);
    while (!IsBhvBusyAbove(behavior)) {
        s = behavior->state;
        switch (s) {
        case 0:
            if (f) SetBhvTargetsState(behavior, 2);
            else if (behavior->chosenTarget) SetBhvTargetsState(behavior, 1);
            break;
        case 1:
            if (f) SetBhvTargetsState(behavior, 2);
            else if (!behavior->chosenTarget) SetBhvTargetsState(behavior, 0);
            else if (behavior->chosenTarget != ((BHVCOMBAT *)behavior->pbhvNext)->target) { SetBhvTargetsState(behavior, -1); SetBhvTargetsState(behavior, 1); }
            else if (IsBhvcombatDone((BHVCOMBAT *)behavior->pbhvNext)) SetBhvTargetsState(behavior, 0);
            break;
        case 2:
            if (!f) SetBhvTargetsState(behavior, 0);
            break;
        }
        if (s != behavior->state) continue;
        break;
    }
}
} // namespace M_func_0017BD78

// 0x17BED0 InitBhvCircle
namespace M_func_0017BED0 {

void InitBhv(BHV179 *p) __asm__("InitBhv");
void AddStepguardSgf(void *p, int n) __asm__("AddStepguardSgf");
// 0x17BED0: Circle behaviour (BHV179) vtable slot 7 (OnPush): raises flag 0x19, state -1.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvCircle(BHV179 *behavior)
{
    InitBhv(behavior);
    AddStepguardSgf(behavior->owner, 0x19);
    behavior->state = -1;
}
} // namespace M_func_0017BED0

// 0x17BF08 StartBhvCircle
namespace M_func_0017BF08 {
void StartBhv(BHV179 *p) __asm__("StartBhv");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
void FindPathzonePathCurve(void *p, qword a, qword b, void *path) __asm__("FindPathzonePathCurve");
// 0x17BF08: Circle behaviour vtable slot 8 (Start): with the path-zone capability (bit 2) walks to a spacing point,
// else stands facing the target.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvCircle(BHV179 *behavior) {
    StartBhv(behavior);
    if (((STEPGUARD *)behavior->owner)->capabilityFlags & 4) {
        REGENT *c = behavior->owner->physics ? behavior->owner->physics->pregent : 0;
        void *pv = c ? c->pathzone : 0;
        qword q = GetLoWorldPos(behavior->owner);
        FindPathzonePathCurve(pv, q, GetLoWorldPos(behavior->owner), behavior->owner->routeShape);
        SetBhvCircleState(behavior, 1);
    } else {
        SetBhvCircleState(behavior, 0);
    }
}
} // namespace M_func_0017BF08

// 0x17BFB8 CloseBhvCircle
// 0x17BFB8: Circle behaviour vtable slot 9 (Close): state -1, lowers flag 0x19.
// Also known as: Exit (upstream readable / Sly 1 name).
void BHV179::Close()
{
    BHV179 *behavior = this;
    CloseBhv(behavior);
    SetBhvCircleState(behavior, -1);
    RemoveStepguardSgf((STEPGUARD *)behavior->owner, 0x19);
}

// 0x17BFF8 ChooseBhvCirclePosition
namespace M_func_0017BFF8 {
union U; struct SCALE; struct FLOW; struct SEGMENT; struct VU_FLOAT;
// ChooseBhvCirclePosition MATCH (Claude agent lane H14), no pins: laneH13v2 with the inner loop as `int k=0;int i=hits;do{SEGMENT*base=segments;Closest(target,&base[k],...);...;k++;}while(--i);`. base is its own pseudo inside the do-body (cse block after the loop label, so not shared with the 1C0A40 call arg, which stays `addiu t0,sp,0x50`); loop.c hoists it to the inner preheader, the &base[k] giv is then initialised as a copy of it, and pass 1 hoists base once more into the index-loop preheader (s4), like the target.
// ChooseBhvCirclePosition laneH13v2 (Claude agent lane H13): inner loop `SEGMENT*segment=base;int i=hits;do{...segment++;}while(--i);` with a function-scope `SEGMENT*base=segments;` so cse cannot share &segments with the 1C0A40 call arg (rematerialized like the target), 22 -> 17. Left: base is set before the mode loop (s6) instead of the index-loop preheader (target s4), which shifts s-reg roles.
// ChooseBhvCirclePosition laneH12v1 (Claude agent lane H12): laneH11v2 + `U point` at function scope between selected and segments, and the two VU scales passed as a qword (VuFromMemWord) into VuMulAddViaScratch(base,v,const qword&g,U*scratch) that stores g into point (target sqc2 0x40): frame and the whole VU block exact (82 -> 72 differing, aligned 26).
// ChooseBhvCirclePosition laneH11v2 (Claude agent lane H11): laneH6v1 + 11EE08 call via inline Closest(qword,const SEGMENT*,U&,float&) (55->33) + plain VuMulAdd with `U point` declared mid-block before `if(hits>0)` (frame 0x460 exact, 33->26).
// ChooseBhvCirclePosition laneH6v1 (Claude agent lane H6): fresh Lilly-style rewrite, no pins, VU helper asms only. aligned 55 (old .c 81, which used lw/lqc2 instruction asms).
// Left: frame 0x480 vs 0x460 (VU_FLOAT temps get their own slot 0x370; target stores them into point at 0x40), region spilled instead of fp, &point/&distance hoisted by loop.c into s-regs (target rematerializes them).
union U{qword q;float f[4];int i[4];};
struct SCALE{char pad[0x20];float value;};   /* STEPGUARD::scale's record (untyped) */
struct FLOW{float polarAngle,polarLength,candidateAngle,candidateLength,distance;};
struct SEGMENT{qword a,b;char data[0x40];};

extern float D_002A5600 __asm__("D_002A5600");
qword GetLoWorldPos(void*) __asm__("GetLoWorldPos");
void GetVectorCylind(qword,float*,float*,float*) __asm__("GetVectorCylind");
float NormalizeAngle__Ff(float) __asm__("NormalizeAngle__Ff");float ClampAbs__Fff(float,float) __asm__("ClampAbs__Fff");float ClampToLimits__FP2LMf(LM*,float) __asm__("ClampToLimits__FP2LMf");
qword SetVectorCylind(float,float,float) __asm__("SetVectorCylind");int IsWithinLimits__FP2LMf(LM*,float) __asm__("IsWithinLimits__FP2LMf");
int FindPathzoneClosestFreePoint(void*,qword,qword*) __asm__("FindPathzoneClosestFreePoint");
int FindPathzoneSegmentBlocker(void*,qword,qword,int,SEGMENT*) __asm__("FindPathzoneSegmentBlocker");
void FindClosestPointOnLineSegment(qword,qword,qword,qword*,float*,float*) __asm__("FindClosestPointOnLineSegment");

struct VU_FLOAT{qword qw;};
// VU0 helper: loads a float constant from a .data word into a VU register.
static inline VU_FLOAT VuFloatFromMemWord(int n){VU_FLOAT v;__asm__("qmtc2.ni %1,%0":"=j"(v.qw):"rV"(n));return v;}
// VU0 helper: vector sum.
// VU0 helper: vector difference.
// VU0 helper: base + v * g, using a scratch slot (for the match).
static inline qword VuMulAddViaScratch(qword base, qword v, const qword &g, U *scratch)
{
    scratch->q = g;
    qword r;
    __asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x" : "=j"(r) : "j"(base), "j"(v), "j"(g));
    return r;
}
// VU0 helper: loads a float constant from a .data word into a VU register.
static inline qword VuFromMemWord(int n){qword v;__asm__("qmtc2.ni %1,%0":"=j"(v):"rV"(n));return v;}
// VU0 helper: base + vector * scale.
static inline qword VuMulAdd(qword base, qword v, VU_FLOAT g)
{
    qword r;
    __asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x" : "=j"(r) : "j"(base), "j"(v), "j"(g.qw));
    return r;
}

// Closest point on a segment to t, and its distance.
static inline void Closest(qword t,const SEGMENT*s,U&pt,float&d){FindClosestPointOnLineSegment(t,s->a,s->b,&pt.q,0,&d);}
// 0x17BFF8: Chooses a free path-zone point near the target within the guard's close/far distance bands and angle
// limits (narrow, then wide), sampling 10/10/15 directions.
// Also known as: PosSurroundPlace (upstream readable / Sly 1 name; PosSurroundPlace__FP5GUARDP9NAVREGIONG9VU_VECTORf).
extern "C" qword ChooseBhvCirclePosition(BHV179 *behavior, void *region, qword origin, float heading)
{
    U closest;
    LM first, second, selected;
    U point;
    SEGMENT segments[8];
    FLOW flow;
    TARGETREC *tuning;
    first = ((STEPGUARD *)behavior->owner)->unknownRange31C;
    second = ((STEPGUARD *)behavior->owner)->circleDistance;
    if (((STEPGUARD *)behavior->owner)->specialState == 8 || ((STEPGUARD *)behavior->owner)->specialState == 13) {
        SCALE *scale = (SCALE *)((STEPGUARD *)behavior->owner)->scale;
        if (scale) {
            float factor = scale->value;
            first.minValue *= factor;
            first.maxValue *= factor;
            second.minValue *= factor;
            second.maxValue *= factor;
        }
    }
    tuning = ((STEPGUARD *)behavior->owner)->pvEngage;
    GetVectorCylind(VuSub(GetLoWorldPos(behavior->owner), origin), &flow.polarAngle, &flow.polarLength, 0);
    float angle = heading + ClampAbs__Fff(NormalizeAngle__Ff(flow.polarAngle - heading), tuning->unknown14);
    float length = ClampToLimits__FP2LMf(&first, flow.polarLength);
    if (FindPathzoneClosestFreePoint(region, VuAdd(origin, SetVectorCylind(angle, length, 0.0f)), &closest.q)) {
        GetVectorCylind(VuSub(closest.q, origin), &flow.candidateAngle, &flow.candidateLength, 0);
        if (__builtin_fabsf(NormalizeAngle__Ff(flow.candidateAngle - heading)) < tuning->facingTolerance
            && IsWithinLimits__FP2LMf(&second, flow.candidateLength))
            return closest.q;
    }
    qword target, best;
    int mode;
    for (mode = 0; mode < 3; mode++) {
        int count;
        float limit;
        switch (mode) {
        case 0:
            selected = first;
            count = 10;
            limit = tuning->unknown14;
            break;
        case 1:
            selected = second;
            count = 10;
            limit = tuning->facingTolerance;
            break;
        case 2:
            selected.minValue = 0.0f;
            selected.maxValue = second.maxValue;
            count = 15;
            limit = 3.14159265f;
            break;
        }
        if (tuning->facingKind == -1)
            limit = 3.14159265f;
        float bestDistance = D_002A5600;
        int index;
        for (index = 0; index < count; index++) {
            qword direction = SetVectorCylind(heading + (-1.0f + index / ((count - 1) * 0.5f)) * limit, 1.0f, 0.0f);
            int hits =
                FindPathzoneSegmentBlocker(region, VuMulAddViaScratch(origin, direction, VuFromMemWord(*(int *)&selected.minValue), &point),
                              VuMulAddViaScratch(origin, direction, VuFromMemWord(*(int *)&selected.maxValue), &point), 8, segments);
            if (hits > 0) {
                int k = 0;
                int i = hits;
                do {
                    SEGMENT *base = segments;
                    Closest(target, &base[k], point, flow.distance);
                    if (flow.distance < bestDistance) {
                        best = point.q;
                        bestDistance = flow.distance;
                    }
                    k++;
                } while (--i);
            }
        }
        if (bestDistance != D_002A5600)
            return best;
    }
    if (FindPathzoneClosestFreePoint(region, origin, &closest.q))
        return closest.q;
    return origin;
}
} // namespace M_func_0017BFF8

// 0x17C3E8 UpdateBhvCircle
namespace M_func_0017C3E8 {
extern float D_002BE748 __asm__("D_002BE748");
void UpdateBhv(BHV179 *p) __asm__("UpdateBhv");
float GetTargetRecordFacing(TARGETREC *engagement) __asm__("GetTargetRecordFacing");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
void GetVectorCylind(qword v, float *a, float *b, int n) __asm__("GetVectorCylind");
float NormalizeAngle__Ff(float f) __asm__("NormalizeAngle__Ff");
int IsWithinLimits__FP2LMf(void *p, float f) __asm__("IsWithinLimits__FP2LMf");
qword ChooseBhvCirclePosition(BHV179 *p, void *pathzone, float f, qword v) __asm__("ChooseBhvCirclePosition");
void FindPathzonePathCurve(void *pathzone, qword a, qword b, SHAPE *c) __asm__("FindPathzonePathCurve");
// 0x17C3E8: Circle behaviour vtable slot 10 (Update): when out of the distance band or off-heading relative to the
// target, re-paths to a new spacing point.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvCircle(BHV179 *behavior) {
    STEPGUARD *a;
    SOPHYS *c;
    REGENT *d;
    void *pathzone;
    SHAPE *b;
    TARGETREC *engagement;
    void *targetObject;
    float targetFacing;
    BHVWALKPATH *walk;
    float cylinder[2];
    qword pos;
    UpdateBhv(behavior);
    d = 0;
    if (behavior->state == 0)
        return;
    a = (STEPGUARD *)behavior->owner;
    c = a->physics;
    b = a->routeShape;
    if (c) d = c->pregent;
    pathzone = d ? d->pathzone : 0;
    engagement = a->pvEngage;
    targetObject = engagement->target;
    targetFacing = GetTargetRecordFacing(engagement);
    pos = GetLoWorldPos(targetObject);
    walk = (BHVWALKPATH *)behavior->pbhvNext;
    if (walk->separation > D_002BE748) goto repath;   // (a float test inverted into an if-block compiles as c.le/bc1f: no match)
    {
        qword dv;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(dv) : "j"(*(qword *)&b->vertexPositions[b->numControlVertices - 1]), "j"(pos));
        GetVectorCylind(dv, &cylinder[0], &cylinder[1], 0);
    }
    if (__builtin_fabsf(NormalizeAngle__Ff(cylinder[0] - targetFacing)) > engagement->facingTolerance) goto repath;
    if (IsWithinLimits__FP2LMf(&((STEPGUARD *)behavior->owner)->circleDistance, cylinder[1])) return;
repath:
    {
        qword v = GetLoWorldPos(behavior->owner);
        FindPathzonePathCurve(pathzone, v, ChooseBhvCirclePosition(behavior, pathzone, targetFacing, pos), b);
    }
    SetBhvpathShape(walk, b, 0);
}
} // namespace M_func_0017C3E8

// 0x17C550 SetBhvCircleState
namespace M_func_0017C550 {
void PopBhv(void *p) __asm__("PopBhv");
BHV *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
// 0x17C550: Circle behaviour state change: 0 idle facing the target, 1 walk the guard's path shape (BHVWALKPATH).
// Also known as: SetSurroundState (upstream readable / Sly 1 name; SetSurroundState__11JOBSURROUNDi).
extern "C" void SetBhvCircleState(BHV179 *behavior, int n)
{
    if (n == behavior->state)
        return;
    PopBhv(&behavior->pbhvNext);
    behavior->state = n;
    switch (n) {
    case 0: {
        BHVIDLE *o = (BHVIDLE *)PushStepBehavior(behavior->owner, CID_BHVIDLE);
        o->lookAt = ((STEPGUARD *)behavior->owner)->pvEngage->target;
        o->Start();
    } break;
    case 1: {
        BHVWALKPATH *o = (BHVWALKPATH *)PushStepBehavior(behavior->owner, CID_BHVWALKPATH);
        SetBhvpathShape(o, behavior->owner->routeShape, 0);
        o->animSgf = 8;
        o->lookAtTarget = ((STEPGUARD *)behavior->owner)->pvEngage->target;
        o->Start();
    } break;
    }
}
} // namespace M_func_0017C550

// 0x17C638 InitBhvEscort
// 0x17C638: Escort behaviour (BHV183) vtable slot 7 (OnPush): raises flag 0x26, state -1.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvEscort(BHV183 *behavior)
{
    InitBhv(behavior);
    behavior->state = -1;
    AddStepguardSgf((STEPGUARD *)behavior->owner, 0x26);
}

// 0x17C670 CloseBhvEscort
// 0x17C670: Escort behaviour vtable slot 9 (Close): lowers flag 0x26.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvEscort(BHV *behavior)
{
    CloseBhv(behavior);
    RemoveStepguardSgf((STEPGUARD *)behavior->owner, 0x26);
}

// 0x17C6A0 StartBhvEscort
// 0x17C6A0: Escort behaviour vtable slot 8 (Start): state 0.
// Also known as: Enter (upstream readable / Sly 1 name).
void BHV183::Start()
{
    BHV183 *behavior = this;
    StartBhv(behavior);
    SetBhvEscortState(behavior, 0);
}

// 0x17C6D0 UpdateBhvEscort
namespace M_func_0017C6D0 {
/* UpdateBhvEscort (Claude agent): state-machine controller. pos is a plain qword (spilled pseudo) so p->v60=pos goes qmfc2+sq; the goto loop + explicit 250.0f local stops loop.c hoisting the case-1 constant. */
typedef union {qword q;float f[4];} U;
void UpdateBhv(BHV183*) __asm__("UpdateBhv");
int IsBhvBusyAbove(BHV183*) __asm__("IsBhvBusyAbove");
qword GetLoWorldPos(void*) __asm__("GetLoWorldPos");
qword ConvertPosBetweenLos(void*,void*,qword) __asm__("ConvertPosBetweenLos");
int IsStepguardHandsOff(void*) __asm__("IsStepguardHandsOff");
void SetBhvgotoGoal(BHVGOTO*,PATHLINKEND*) __asm__("SetBhvgotoGoal");
// VU0 helper: vector difference.
extern "C" { static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=&j"(r):"j"(a),"j"(b));return r;} }
// VU0 helper: a -= b.
extern "C" { static inline qword SubIn(qword a,qword b){__asm__("vsub.xyzw %0,%0,%1":"+j"(a):"j"(b));return a;} }
// VU0 helper: horizontal (xy) length of a vector.
extern "C" { static inline float LenXY(qword v)
{
    float r;
    __asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y" : "=j"(v) : "0"(v));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(v) : "0"(v));
    __asm__("qmfc2.ni %0,%1" : "=r"(r) : "j"(v));
    return r;
} }
// 0x17C6D0: Escort behaviour vtable slot 10 (Update): follows the escorted object at a local offset: walks when
// within 700, runs (state 1) beyond 1000, re-goals after the object moves 250.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvEscort(BHV183 *behavior)
{
    qword pos;
    float dist;
    int state;
    float lim = 250.0f;
    UpdateBhv(behavior);
    pos = GetLoWorldPos(behavior->escorted);
    dist = LenXY(Sub(pos, GetLoWorldPos(behavior->owner)));
    goto test;
top: {
    state = behavior->state;
    switch (state) {
    case 0:
        if (IsStepguardHandsOff(behavior->owner))
            SetBhvEscortState(behavior, 2);
        else if (1000.0f < dist)
            SetBhvEscortState(behavior, 1);
        else if (lim < LenXY(SubIn(behavior->lastGoalPosition, pos))) {
            qword v;
            behavior->lastGoalPosition = pos;
            v = ConvertPosBetweenLos(behavior->escorted, 0, behavior->localOffset);
            PATHLINKEND x;
            x.pos = v;
            SetBhvgotoGoal((BHVGOTO *)behavior->pbhvNext, &x);
        }
        break;
    case 1:
        if (IsStepguardHandsOff(behavior->owner))
            SetBhvEscortState(behavior, 2);
        else if (dist < 700.0f)
            SetBhvEscortState(behavior, 0);
        else if (lim < LenXY(SubIn(behavior->lastGoalPosition, pos))) {
            behavior->lastGoalPosition = pos;
            PATHLINKEND x;
            x.pos = pos;
            SetBhvgotoGoal((BHVGOTO *)behavior->pbhvNext, &x);
        }
        break;
    case 2:
        if (!IsStepguardHandsOff(behavior->owner))
            SetBhvEscortState(behavior, 0);
        break;
    }
    if (state == behavior->state)
        return;
}
test:
    if (!IsBhvBusyAbove(behavior))
        goto top;
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_U[(sizeof(QWROW)==16)?1:-1];
typedef char layout_float[(sizeof(float)==4)?1:-1];
} // namespace M_func_0017C6D0

// 0x17C928 SetBhvEscortState
namespace M_func_0017C928 {
void PopBhv(void *p) __asm__("PopBhv");
BHV *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
qword ConvertPosBetweenLos(void *p, int n, qword v) __asm__("ConvertPosBetweenLos");
/* PATHLINKEND cleared by an explicit ClearPointerPair call at function scope: its constructor (a scoped PATHLINKEND)
   swaps two instructions in case 0 (scheduling). */
struct PATHLINKENDIsland { qword a; qword v; };
void ClearPointerPair(PATHLINKENDIsland *x) __asm__("ClearPointerPair");
void SetBhvgotoGoal(BHVGOTO *p, PATHLINKENDIsland *x) __asm__("SetBhvgotoGoal");
// 0x17C928: Escort behaviour state change: 0 go to the offset point, 1 run to the object (flag 0x1C), 2 hands-off
// idle.
// Also known as: SetState (upstream readable / Sly 1 name; SetState__10JOBSTATIONi).
extern "C" void SetBhvEscortState(BHV183 *behavior, int n) {
    PATHLINKENDIsland x;
    if (n == behavior->state) return;
    PopBhv(&behavior->pbhvNext);
    behavior->state = n;
    behavior->stateTime = g_clock.time;
    switch (n) {
    case 1:
        {
            BHVGOTO *o = (BHVGOTO *)PushStepBehavior(behavior->owner, CID_BHVGOTO);
            qword v = GetLoWorldPos(behavior->escorted);
            behavior->lastGoalPosition = v;
            o->allowPartialRoute = 1;
            o->lookAtTarget = behavior->escorted;
            o->unknownAC = 1;
            ClearPointerPair(&x);
            x.v = v;
            SetBhvgotoGoal(o, &x);
            o->sgf = 0x1C;
            o->Start();
        }
        break;
    case 2:
        {
            BHVIDLE *o = (BHVIDLE *)PushStepBehavior(behavior->owner, CID_BHVIDLE);
            o->lookAt = behavior->escorted;
            o->sgf = 1;
            o->Start();
        }
        break;
    case 0:
        {
            BHVGOTO *o = (BHVGOTO *)PushStepBehavior(behavior->owner, CID_BHVGOTO);
            qword v;
            behavior->lastGoalPosition = GetLoWorldPos(behavior->escorted);
            v = ConvertPosBetweenLos(behavior->escorted, 0, behavior->localOffset);
            o->allowPartialRoute = 1;
            ClearPointerPair(&x);
            x.v = v;
            SetBhvgotoGoal(o, &x);
            o->lookAtTarget = behavior->escorted;
            o->Start();
        }
        break;
    }
}
} // namespace M_func_0017C928

// 0x17CAC8 InitBhvrail
namespace M_InitBhvrail {

void InitBhv(BHVRAIL *p) __asm__("InitBhv");
void AddStepguardSgf(void *p, int n) __asm__("AddStepguardSgf");
// 0x17CAC8: BHVRAIL vtable slot 7 (OnPush): raises flag 0x1B (on a rail), state -1.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvrail(BHVRAIL *behavior)
{
    InitBhv(behavior);
    behavior->state = -1;
    behavior->animSpeed = 1.0f;
    AddStepguardSgf(behavior->owner, SGF_ON_RAIL);
}
} // namespace M_InitBhvrail

// 0x17CB10 StartBhvrail
// 0x17CB10: BHVRAIL vtable slot 8 (Start): state 0.
void BHVRAIL::Start()
{
    BHVRAIL *behavior = this;
    StartBhv(behavior);
    SetBhvrailState(behavior, 0);
}

// 0x17CB40 CloseBhvrail
// 0x17CB40: BHVRAIL vtable slot 9 (Close): state -1, lowers flag 0x1B.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvrail(BHVRAIL *behavior)
{
    CloseBhv(behavior);
    SetBhvrailState(behavior, -1);
    RemoveStepguardSgf((STEPGUARD *)behavior->owner, SGF_ON_RAIL);
}

// 0x17CB80 UpdateBhvrail
namespace M_UpdateBhvrail {
typedef union { qword q; float f; } U;
extern float D_002BE74C __asm__("D_002BE74C");
union V;
union V { qword q; float f[4]; };
void UpdateBhv(BHVRAIL *p) __asm__("UpdateBhv");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
float WrapShapeDistance(void *p, float f) __asm__("WrapShapeDistance");
void FindShapeClosestPointFromDistance(void *p, float f, qword v, void *a, qword *b, union V *pv, float *c, float *pt) __asm__("FindShapeClosestPointFromDistance");
int HasBhvrailArrived(BHVRAIL *p) __asm__("HasBhvrailArrived");
float atan2f(float y, float x) __asm__("atan2f");
void SetStepguardMoverRadTarget(void *p, float f) __asm__("SetStepguardMoverRadTarget");
void SetAsegaSpeed__FP5ASEGAf(void *p, float f) __asm__("SetAsegaSpeed__FP5ASEGAf");
// 0x17CB80: BHVRAIL vtable slot 10 (Update): walks along the attached rail shape toward the goal parameter; stands
// on arrival.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvrail(BHVRAIL *behavior) {
    int s;
    UpdateBhv(behavior);
    do {
        s = behavior->state;
        if (s == 0) {
            STEPGUARD *a = ((STEPGUARD *)behavior->owner);
            void *q = a->attachObject;
            float t = a->attachParam;
            qword pos;
            union V v;
            a->isMovingForward = t < behavior->goalParam;
            pos = GetLoWorldPos(((STEPGUARD *)behavior->owner));
            FindShapeClosestPointFromDistance(q, WrapShapeDistance(q, ((STEPGUARD *)behavior->owner)->isMovingForward ? t + D_002BE74C : t - D_002BE74C), pos, 0, 0, &v, 0, &t);
            ((STEPGUARD *)behavior->owner)->attachParam = t;
            if (HasBhvrailArrived(behavior)) {
                SetBhvrailState(behavior, 1);
            } else {
                if (((STEPGUARD *)behavior->owner)->isMovingForward == 0) {
                    qword w = v.q;
                    U u;
                    { qword u_q; __asm__("qmtc2.ni %1, %0" : "=j"(u_q) : "r"(-1.0f)); u.q = u_q; }
                    { qword v_q; __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(v_q) : "j"(w), "j"(u.q)); v.q = v_q; }
                }
                SetStepguardMoverRadTarget((&((STEPGUARD *)behavior->owner)->mover), atan2f(v.f[1], v.f[0]));
                SetAsegaSpeed__FP5ASEGAf(((BHVANIM *)behavior->pbhvNext)->binding->asega, behavior->animSpeed);
            }
        }
    } while (behavior->state != s);
}
} // namespace M_UpdateBhvrail

// 0x17CD00 SetBhvrailState
namespace M_SetBhvrailState {

void PopBhv(void *p) __asm__("PopBhv");
BHVANIM *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
// 0x17CD00: BHVRAIL state change: 0 walk animation (flag 9), 1 stand (flag 2).
// Also known as: SetRailState (upstream readable / Sly 1 name; SetRailState__7JOBRAILi).
extern "C" void SetBhvrailState(BHVRAIL *behavior, int n) {
    if (n == behavior->state) return;
    PopBhv((&behavior->pbhvNext));
    behavior->state = n;
    behavior->stateTime = g_clock.time;
    switch (n) {
    case 1:
        {
            BHVANIM *o = PushStepBehavior(behavior->owner, CID_BHVANIM);
            o->sgf = 2;
            o->Start();
        }
        break;
    case 0:
        {
            BHVANIM *o = PushStepBehavior(behavior->owner, CID_BHVANIM);
            o->sgf = 9;
            o->Start();
        }
        break;
    }
}
} // namespace M_SetBhvrailState

// 0x17CDB8 SetBhvrailGoal
namespace M_SetBhvrailGoal {

float GetShapeMaxDistance(void *p) __asm__("GetShapeMaxDistance");
float GetShapeMinDistance(void *p) __asm__("GetShapeMinDistance");
// 0x17CDB8: Sets the rail goal to the rail's maximum or minimum parameter, the direction, and starts walking.
// Also known as: SetDirection (upstream readable / Sly 1 name; SetDirection__7JOBRAILi).
extern "C" void SetBhvrailGoal(BHVRAIL *behavior, int f)
{
    void *pv = ((STEPGUARD *)behavior->owner)->attachObject;
    if (f)
        behavior->goalParam = GetShapeMaxDistance(pv);
    else
        behavior->goalParam = GetShapeMinDistance(pv);
    ((STEPGUARD *)behavior->owner)->isMovingForward = ((STEPGUARD *)behavior->owner)->attachParam < behavior->goalParam;
    SetBhvrailState(behavior, 0);
}
} // namespace M_SetBhvrailGoal

// 0x17CE30 HasBhvrailArrived
namespace M_FBhvrailArrived {

// 0x17CE30: True if the rail parameter is within 50 of the goal.
extern "C" int HasBhvrailArrived(BHVRAIL *behavior) { return __builtin_fabsf(((STEPGUARD *)behavior->owner)->attachParam - behavior->goalParam) < 50.0f; }
} // namespace M_FBhvrailArrived

// 0x17CE68 InitBhvpipe
// 0x17CE68: BHVPIPE vtable slot 7 (OnPush): raises flag 0x27 (on a pipe), state -1.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvpipe(BHVPIPE *behavior)
{
    InitBhv(behavior);
    behavior->state = -1;
    AddStepguardSgf((STEPGUARD *)behavior->owner, SGF_ON_PIPE);
}

// 0x17CEA0 StartBhvpipe
// 0x17CEA0: BHVPIPE vtable slot 8 (Start): state 0.
// Also known as: Enter (upstream readable / Sly 1 name).
void BHVPIPE::Start()
{
    BHVPIPE *behavior = this;
    StartBhv(behavior);
    SetBhvpipeState(behavior, 0);
}

// 0x17CED0 CloseBhvpipe
// 0x17CED0: BHVPIPE vtable slot 9 (Close): state -1, lowers flag 0x27.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvpipe(BHVPIPE *behavior)
{
    CloseBhv(behavior);
    SetBhvpipeState(behavior, -1);
    RemoveStepguardSgf((STEPGUARD *)behavior->owner, SGF_ON_PIPE);
}

// 0x17CF10 UpdateBhvpipe
namespace M_UpdateBhvpipe {
struct O; struct L;
extern float D_002DFB7C __asm__("D_002DFB7C");
struct O : PO { char pad[0x198 - 0x180]; float moveSpeed; char pad2[0x14]; char orientation[0xF84 - 0x1B0]; int attachObject; float attachParam; int isMovingForward; };
struct L { float a, b, c; };
void UpdateBhv(BHVPIPE *p) __asm__("UpdateBhv");
int HasBhvpipeArrived(BHVPIPE *p) __asm__("HasBhvpipeArrived");
float SmoothValue__FfffP3SMPPf(float a, float b, float dt, struct L *pl, float *n) __asm__("SmoothValue__FfffP3SMPPf");
float WrapShapeDistance(int n, float g) __asm__("WrapShapeDistance");
void GetPipeGoalMatrix(int a, float g, int b, void *pv) __asm__("GetPipeGoalMatrix");
// 0x17CF10: BHVPIPE vtable slot 10 (Update): shimmies along the pipe toward the goal with a smoothed speed; keeps
// the pipe goal matrix.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvpipe(BHVPIPE *behavior) {
    int s;
    UpdateBhv(behavior);
    do {
        s = behavior->state;
        if (s == 0) {
            if (HasBhvpipeArrived(behavior)) {
                SetBhvpipeState(behavior, 1);
            } else {
                struct L l;
                float g;
                ((struct O *)behavior->owner)->isMovingForward = ((struct O *)behavior->owner)->attachParam < behavior->goalParam;
                if (((struct O *)behavior->owner)->isMovingForward) D_002DFB7C = -((struct O *)behavior->owner)->moveSpeed;
                else D_002DFB7C = ((struct O *)behavior->owner)->moveSpeed;
                {
                    float d = D_002DFB7C;
                    l.b = 0.0f;
                    l.c = 0.1f;
                    l.a = __builtin_fabsf(d);
                    g = SmoothValue__FfffP3SMPPf(behavior->smoothedSpeed, d, g_clock.deltaTime, &l, 0);
                }
                behavior->smoothedSpeed = g;
                ((struct O *)behavior->owner)->attachParam = WrapShapeDistance(((struct O *)behavior->owner)->attachObject, ((struct O *)behavior->owner)->attachParam + g * g_clock.deltaTime);
            }
        }
    } while (behavior->state != s);
    {
        struct O *o = ((struct O *)behavior->owner);
        GetPipeGoalMatrix(o->attachObject, o->attachParam, o->isMovingForward, o->orientation);
    }
}
} // namespace M_UpdateBhvpipe

// 0x17D058 SetBhvpipeState
namespace M_SetBhvpipeState {

void PopBhv(void *p) __asm__("PopBhv");
BHVANIM *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
// 0x17D058: BHVPIPE state change: 0 move animation (flag 9), 1 stand (flag 2).
// Also known as: SetLadderState (upstream readable / Sly 1 name; SetLadderState__9JOBLADDERi).
extern "C" void SetBhvpipeState(BHVPIPE *behavior, int n) {
    if (n == behavior->state) return;
    PopBhv((&behavior->pbhvNext));
    behavior->state = n;
    behavior->stateTime = g_clock.time;
    switch (n) {
    case 1:
        { BHVANIM *o = PushStepBehavior(behavior->owner, CID_BHVANIM); o->sgf = 2; o->Start(); } behavior->smoothedSpeed = 0;
        break;
    case 0:
        {
            BHVANIM *o = PushStepBehavior(behavior->owner, CID_BHVANIM);
            o->sgf = 9;
            o->Start();
        }
        break;
    }
}
} // namespace M_SetBhvpipeState

// 0x17D110 SetBhvpipeGoal
namespace M_SetBhvpipeGoal {

float GetShapeMaxDistance(void *p) __asm__("GetShapeMaxDistance");
float GetShapeMinDistance(void *p) __asm__("GetShapeMinDistance");
// 0x17D110: Sets the pipe goal to the pipe's maximum or minimum parameter.
// Also known as: SetDirection (upstream readable / Sly 1 name; SetDirection__9JOBLADDERi).
extern "C" void SetBhvpipeGoal(BHVPIPE *behavior, int f)
{
    void *pv = ((STEPGUARD *)behavior->owner)->attachObject;
    if (f)
        behavior->goalParam = GetShapeMaxDistance(pv);
    else
        behavior->goalParam = GetShapeMinDistance(pv);
    ((STEPGUARD *)behavior->owner)->isMovingForward = ((STEPGUARD *)behavior->owner)->attachParam < behavior->goalParam;
    SetBhvpipeState(behavior, 0);
}
} // namespace M_SetBhvpipeGoal

// 0x17D188 HasBhvpipeArrived
namespace M_FBhvpipeArrived {

// 0x17D188: True if the pipe parameter is within 50 of the goal.
extern "C" int HasBhvpipeArrived(BHVPIPE *behavior) { return __builtin_fabsf(((STEPGUARD *)behavior->owner)->attachParam - behavior->goalParam) < 50.0f; }
} // namespace M_FBhvpipeArrived

// 0x17D1C0 InitBhvAttachCid19
// 0x17D1C0: Attach behaviour for cid 0x13 objects (BHV187) vtable slot 7 (OnPush): raises flag 0x28.
// Also known as: Init (upstream readable / Sly 1 name).
void BHV187::OnPush()
{
    BHV *behavior = (BHV *)this;
    InitBhv(behavior);
    AddStepguardSgf((STEPGUARD *)behavior->owner, SGF_ON_CLASS19);
}

// 0x17D1F0 StartBhvAttachCid19
namespace M_func_0017D1F0 {
struct M3;
struct M3 { qword a, b, c; };
extern qword g_normalZ __asm__("g_normalZ");
void StartBhv(BHV187 *p) __asm__("StartBhv");
BHVANIM *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
qword *GetLoWorldMatrix(void *p) __asm__("GetLoWorldMatrix");
struct M3 BuildOrthonormalMatrixZ(qword a, qword b) __asm__("BuildOrthonormalMatrixZ");
// 0x17D1F0: Attach behaviour (cid 0x13) vtable slot 8 (Start): pushes the stand animation and aligns to the attach
// object's frame.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvAttachCid19(BHV187 *behavior)
{
    BHVANIM *o;
    STEPGUARD *t;
    StartBhv(behavior);
    o = PushStepBehavior(behavior->owner, CID_BHVANIM);
    o->sgf = 2;
    o->Start();
    t = (STEPGUARD *)behavior->owner;
    *(M3 *)t->aqwTarget = BuildOrthonormalMatrixZ(*GetLoWorldMatrix(t->attachObject), g_normalZ);
}
} // namespace M_func_0017D1F0

// 0x17D280 CloseBhvAttachCid19
// 0x17D280: Attach behaviour (cid 0x13) vtable slot 9 (Close): lowers flag 0x28.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvAttachCid19(BHV *behavior)
{
    CloseBhv(behavior);
    RemoveStepguardSgf((STEPGUARD *)behavior->owner, SGF_ON_CLASS19);
}

// 0x17D2B0 InitBhvAttachCid39
// 0x17D2B0: Attach behaviour for cid 0x27 objects (BHV188) vtable slot 7 (OnPush): raises flag 0x29.
void BHV188::OnPush()
{
    BHV *behavior = (BHV *)this;
    InitBhv(behavior);
    AddStepguardSgf((STEPGUARD *)behavior->owner, SGF_ON_CLASS39);
}

// 0x17D2E0 StartBhvAttachCid39
namespace M_func_0017D2E0 {
void StartBhv(BHV188 *p) __asm__("StartBhv");
BHVANIM *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
// 0x17D2E0: Attach behaviour (cid 0x27) vtable slot 8 (Start): pushes the stand animation.
extern "C" void StartBhvAttachCid39(BHV188 *behavior)
{
    BHVANIM *o;
    StartBhv(behavior);
    o = PushStepBehavior(behavior->owner, CID_BHVANIM);
    o->sgf = 2;
    o->Start();
}
} // namespace M_func_0017D2E0

// 0x17D330 CloseBhvAttachCid39
// 0x17D330: Attach behaviour (cid 0x27) vtable slot 9 (Close): lowers flag 0x29.
extern "C" void CloseBhvAttachCid39(BHV *behavior)
{
    CloseBhv(behavior);
    RemoveStepguardSgf((STEPGUARD *)behavior->owner, SGF_ON_CLASS39);
}

namespace RO_ORDER { extern const float r_D_002A55FC __asm__("D_002A55FC") = 3.40282347e+38f; }
namespace RO_ORDER { extern const float r_D_002A5600 __asm__("D_002A5600") = 3.40282347e+38f; }
// 0x17D360 InitBhvgoto
namespace M_InitBhvgoto {
struct T;
struct T { char pad[0x28]; int version; };
void InitBhv(BHVGOTO *p) __asm__("InitBhv");
// 0x17D360: BHVGOTO vtable slot 7 (OnPush): no flag, state -1, stamps the path-graph version.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvgoto(BHVGOTO *behavior)
{
    InitBhv(behavior);
    behavior->state = -1;
    behavior->sgf = -1;
    behavior->planTime = -3.40282346638528859812e+38f;
    behavior->pathGraphVersion = ((struct T *)g_sw.pregtree)->version;
}
} // namespace M_InitBhvgoto

// 0x17D3B0 StartBhvgoto
// 0x17D3B0: BHVGOTO vtable slot 8 (Start): raises the caller-chosen flag (e.g. 0x1C run), stands and plans the
// route.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvgoto(BHVGOTO *behavior)
{
    int sgf;
    sgf = behavior->sgf;
    if (sgf != -1) {
        AddStepguardSgf((STEPGUARD *)behavior->owner, sgf);
    }
    StartBhv(behavior);
    SetBhvgotoState(behavior, 4);
    PlanBhvgoto(behavior);
}

// 0x17D408 CloseBhvgoto
// 0x17D408: BHVGOTO vtable slot 9 (Close): state -1, lowers its flag.
// Also known as: Exit (upstream readable / Sly 1 name).
void BHVGOTO::Close()
{
    BHVGOTO *behavior = this;
    int sgf;
    CloseBhv(behavior);
    SetBhvgotoState(behavior, -1);
    sgf = behavior->sgf;
    if (sgf != -1) {
        RemoveStepguardSgf((STEPGUARD *)behavior->owner, sgf);
    }
}

// 0x17D450 SetBhvgotoState
namespace M_SetBhvgotoState {
void PopBhv(void *p) __asm__("PopBhv");
BHV *PushStepBehavior(STEPGUARD *guard, int n) __asm__("PushStepBehavior");
void SetBhvpathShape(BHVPATH *o, void *shape, int b) __asm__("SetBhvpathShape");
}
// 0x17D450: BHVGOTO state change: 4 stand (BHVIDLE), 1/2 walk the path (BHVWALKPATH, the run animation when
// allowed), 3 traverse a path link (BHVPATHLINK), 5 relocate (BHV191 on the relocate goal).
// Also known as: SetState (upstream readable / Sly 1 name; SetState__7JOBGOTOi).
extern "C" void SetBhvgotoState(BHVGOTO *behavior, int n) {
    using namespace M_SetBhvgotoState;
    if (n == behavior->state) return;
    if (!((unsigned)n < 3 && behavior->state >= 0 && behavior->state < 3)) PopBhv(&behavior->pbhvNext);
    behavior->state = n;
    behavior->stateTime = g_clock.time;
    switch (n) {
    case 4:
        {
            BHVIDLE *o = (BHVIDLE *)PushStepBehavior((STEPGUARD *)behavior->owner, CID_BHVIDLE);
            o->lookAt = behavior->lookAtTarget;
            o->Start();
        }
        break;
    case 3:
        {
            BHVPATHLINK *o = (BHVPATHLINK *)PushStepBehavior((STEPGUARD *)behavior->owner, CID_BHVPATHLINK);
            o->pathlink = behavior->firstLink;
            o->Start();
        }
        break;
    case 1:
    case 2:
        {
            BHVWALKPATH *o = (BHVWALKPATH *)behavior->pbhvNext;
            if (o == 0) {
                o = (BHVWALKPATH *)PushStepBehavior((STEPGUARD *)behavior->owner, CID_BHVWALKPATH);
                o->lookAtTarget = behavior->lookAtTarget;
                o->ignoresBlockers = behavior->isDirect;
                if (!behavior->lookAtTarget || behavior->unknownAC || !(((STEPGUARD *)behavior->owner)->capabilityFlags & 4)) o->animSgf = 9; else o->animSgf = 8;
                SetBhvpathShape(o, behavior->owner->routeShape, 0);
                o->Start();
            } else {
                SetBhvpathShape(o, behavior->owner->routeShape, 0);
            }
        }
        break;
    case -1:
    case 0:
        break;
    case 5:
        {
            BHV191 *o = (BHV191 *)PushStepBehavior((STEPGUARD *)behavior->owner, CID_BHV191);
            o->end = behavior->relocateGoal;
            o->Start();
        }
        break;
    }
}

// 0x17D608 SnapPositionToSurfaceAbove
namespace M_FSnapPosToSurfaceAbove {
   struct VU_FLOAT;
// SnapPositionToSurfaceAbove: ported from Lilly's personal Sly 2 decomp (src/P2/guardbhv_goto.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BE750 (D_002BE750), D_002BE754 (D_002BE754), g_normalZ
// 0x0017D608 FSnapPosToSurfaceAbove__FPUx  (our name; was FUN_0017D608)
// A line from the position straight up (5000 long) against the world's objects: when it hits one, the position is moved to the end of the
// hit piece and 1 is returned. MoveBhvRelocateToPoint (0x17E9B0) tries it when SnapToGround finds no floor under the point.

struct VU_FLOAT
{
    qword qw;
};
// VU0 helper: loads a float constant from a .data word into a VU register.
static inline VU_FLOAT VuFloatFromMemWord(int n)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "rV"(n));
    return v;
}
extern float D_002BE750 __asm__("D_002BE750");
extern float D_002BE754 __asm__("D_002BE754");
extern qword g_normalZ __asm__("g_normalZ");

void CollectVisibleMsGlobs(int *pcpso, SOPART ***papso) __asm__("CollectVisibleMsGlobs");


SOPART *HitTestLineObjects(int grfhtl, qword pos0, qword pos1, int cpso, SOPART **apso, LSG *plsgBest) __asm__("FindFirstObjectHitByLine");

// VU0 helper: base + vector * scale.
static inline qword VuMulAdd(qword qwBase, qword qwVec, VU_FLOAT gScale)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(qwBase), "j"(qwVec), "j"(gScale.qw));
    return qw;
}
int SnapPositionToSurfaceAbove(qword *ppos) __asm__("SnapPositionToSurfaceAbove");
// 0x17D608: Casts a line 5000 units up from the position; on a hit moves the position there and returns true.
// Also known as: FSnapPosToSurfaceAbove (upstream readable / Sly 1 name; FSnapPosToSurfaceAbove__FP6VECTOR).
int SnapPositionToSurfaceAbove(qword *ppos)
{
    STACKSCOPE scope;
    int cpso;
    SOPART **apso;
    CollectVisibleMsGlobs(&cpso, &apso);
    qword posBottom = VuMulAdd(*ppos, g_normalZ, VuFloatFromMemWord(*(int *)&D_002BE750));
    qword posTop = VuMulAdd(*ppos, g_normalZ, VuFloatFromMemWord(*(int *)&D_002BE754));
    LSG lsg;
    if (HitTestLineObjects(0, posBottom, posTop, cpso, apso, &lsg))
    {
        *ppos = lsg.apos[1];
        return 1;
    }
    return 0;
}
} // namespace M_FSnapPosToSurfaceAbove

// 0x17D6C8 UpdateBhvgoto
namespace M_UpdateBhvgoto {
extern float D_002BE758 __asm__("D_002BE758"), D_002BE75C __asm__("D_002BE75C");
struct W { int pad[0x28 / 4]; int version; };
int IsBhvwalkpathStuck(BHVPATH *t) __asm__("IsBhvwalkpathStuck");
float GetShapeMaxDistance(SHAPE *p) __asm__("GetShapeMaxDistance");
void EvaluateShapeAtDistance(SHAPE *p, float f, qword *pv, int n) __asm__("EvaluateShapeAtDistance");
int IsBhvpathlinkDone(BHV *t) __asm__("IsBhvpathlinkDone");
int IsBhvRelocateDone(BHV *t) __asm__("IsBhvRelocateDone");
}
// 0x17D6C8: BHVGOTO vtable slot 10 (Update): re-plans when stale or the path graph changed; at a path link's start
// traverses it; relocates when stuck.
// Also known as: Update (upstream readable / Sly 1 name).
void BHVGOTO::Update() {
    using namespace M_UpdateBhvgoto;
    BHVGOTO *behavior = this;
    int s;
    UpdateBhv(behavior);
    if (!(((STEPGUARD *)behavior->owner)->capabilityFlags & 2)) return;
    while (!IsBhvBusyAbove(behavior)) {
        s = behavior->state;
        switch (s) {
        case 1:
        case 2:
            {
                BHVPATH *t = (BHVPATH *)behavior->pbhvNext;
                if (s == 2 && IsBhvpathAtEnd(t)) {
                    SetBhvgotoState(behavior, 3);
                } else if (g_clock.time - behavior->planTime > D_002BE758 || behavior->pathGraphVersion != ((struct W *)g_sw.pregtree)->version) {
                    PlanBhvgoto(behavior);
                } else if (IsBhvwalkpathStuck(t)) {
                    float f = t->pathParam + D_002BE75C;
                    float maxParam = GetShapeMaxDistance(t->shape);
                    behavior->relocateGoal.shape = 0;
                    behavior->relocateGoal.object = 0;
                    EvaluateShapeAtDistance(t->shape, f < maxParam ? f : maxParam, &behavior->relocateGoal.pos, 0);
                    ((float *)&behavior->relocateGoal.pos)[2] = ((float *)((STEPGUARD *)behavior->owner)->unknown218)[2];
                    SetBhvgotoState(behavior, 5);
                }
            }
            break;
        case 3:
            if (IsBhvpathlinkDone(behavior->pbhvNext)) PlanBhvgoto(behavior);
            break;
        case 5:
            if (IsBhvRelocateDone(behavior->pbhvNext)) PlanBhvgoto(behavior);
            break;
        }
        if (s != behavior->state) continue;
        break;
    }
}

// 0x17D8A8 FindBhvgotoRoute
namespace M_FindBhvgotoRoute {
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
qword GetPathlinkEndpointPos(PATHLINKEND *p, qword goalPos) __asm__("GetPathlinkEndpointPos");
struct CID17 *FindPathzoneAtPos(qword goalPos, int n) __asm__("FindPathzoneAtPos");
int FindPathlinkRoute(void *a, STEPGUARD *b, qword goalPos, int c, int d, PATHLINK **pp) __asm__("FindPathlinkRoute");
void FindPathzoneClosestPoint(void *a, qword goalPos, qword *pv, int n) __asm__("FindPathzoneClosestPoint");
// 0x17D8A8: Routes the goal through the path zones: directly within one zone, else through path links (optionally
// allowing partial routes), else stops at the closest reachable point.
extern "C" void FindBhvgotoRoute(BHVGOTO *behavior) {
    behavior->numRouteLinks = 0;
    behavior->waypoint = behavior->goal;
    if (behavior->isDirect != 0)
        return;
    qword pos = GetLoWorldPos(behavior->owner);
    qword goalPos = GetPathlinkEndpointPos(&behavior->goal, pos);
    SOPHYS *c = behavior->owner->physics;
    REGENT *d = c ? c->pregent : 0;
    void *currentZone = d ? d->pathzone : 0;
    void *goalZone;
    int failed;
    goalZone = FindPathzoneAtPos(goalPos, 0);
    failed = 0;
    if (goalZone == 0) {
        failed = 1;
    } else if (goalZone != currentZone) {
        SW *g = &g_sw;
        int n;
        if ((behavior->numRouteLinks = FindPathlinkRoute(g->pregtree, (STEPGUARD *)behavior->owner, goalPos, 1, 1, behavior->route)) <= 0) {
            if (behavior->allowPartialRoute == 0) {
                failed = 1;
            } else {
                n = FindPathlinkRoute(g->pregtree, (STEPGUARD *)behavior->owner, goalPos, 0, 1, behavior->route);
                behavior->numRouteLinks = n;
                if (n == 0) {
                    failed = 1;
                } else {
                    void *a;
                    if (behavior->route[0] == 0) {
                        a = currentZone;
                        behavior->numRouteLinks = 0;
                    } else {
                        a = behavior->route[n - 1]->endZone;
                    }
                    behavior->waypoint.shape = 0;
                    behavior->waypoint.object = 0;
                    FindPathzoneClosestPoint(a, goalPos, &behavior->waypoint.pos, 1);
                }
            }
        }
    }
    if (failed) {
        behavior->waypoint.shape = 0;
        behavior->waypoint.object = 0;
        behavior->waypoint.pos = pos;
    }
}
} // namespace M_FindBhvgotoRoute

// 0x17DA28 PlanBhvgoto
namespace M_PlanBhvgoto {
struct W;
struct W { int pad[0x28 / 4]; int version; };   /* the region tree (REGTREE has no layout yet): its path-graph version */
void FindBhvgotoRoute(BHVGOTO *p) __asm__("FindBhvgotoRoute");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
/* PATHLINKEND cleared by an explicit ClearPointerPair call: its constructor at that point changes the register of
   the numRouteLinks load (v0/v1). */
struct PATHLINKENDIsland { SHAPE *shape; LO *object; int unknown8[2]; qword pos; };
void ClearPointerPair(PATHLINKENDIsland *x) __asm__("ClearPointerPair");
qword GetPathlinkEndpointPos(PATHLINKENDIsland *s, qword v) __asm__("GetPathlinkEndpointPos");
void FindClosestPointOnLineSegment(qword p, qword a, qword b, qword *pc, float *pt, int n) __asm__("FindClosestPointOnLineSegment");
void MeasureShape(SHAPE *b) __asm__("MeasureShape");
int FindPathzonePathCurve(void *q, qword a, qword b, SHAPE *c) __asm__("FindPathzonePathCurve");
// VU0 helper: z component of a vector.
// 0x17DA28: Plans the move: routes, builds the path curve to the next waypoint or path link (or a straight
// two-point shape) and picks the state.
// Also known as: Search (upstream readable / Sly 1 name).
extern "C" void PlanBhvgoto(BHVGOTO *behavior) {
    PATHLINKENDIsland nextGoal;
    int nextState;
    if (behavior->state == -1) return;
    if (!(((STEPGUARD *)behavior->owner)->capabilityFlags & 2)) return;
    if (IsBhvBusyAbove(behavior)) return;
    SetBhvgotoState(behavior, 0);
    FindBhvgotoRoute(behavior);
    ClearPointerPair(&nextGoal);
    if (behavior->numRouteLinks == 0) {
        nextGoal = *(PATHLINKENDIsland *)&behavior->waypoint;
        nextState = 1;
    } else {
        PATHLINK *q = behavior->firstLink;
        nextGoal = *(PATHLINKENDIsland *)&q->aend[0];
        nextState = 2;
        if (q->kind == 0) {
            union { qword q; float f[4]; } c;
            float segmentParam;
            qword pos = GetLoWorldPos(behavior->owner);
            qword a = GetPathlinkEndpointPos((PATHLINKENDIsland *)&q->aend[0], pos);
            qword b = GetPathlinkEndpointPos((PATHLINKENDIsland *)&q->aend[1], pos);
            int isNearLink;
            FindClosestPointOnLineSegment(pos, a, b, &c.q, &segmentParam, 0);
            isNearLink = 0;
            if (segmentParam > 0.0f && segmentParam < 1.0f) {
                qword d, l;
                float distance;
                __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(c.q), "j"(pos));
                __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(d));
                __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
                __asm__("qmfc2.ni %0, %1" : "=r"(distance) : "j"(l));
                if (distance < 50.0f) {
                    if (__builtin_fabsf(c.f[2] - QwordZ(pos)) < 300.0f) isNearLink = 1;
                }
            }
            if (isNearLink) nextState = 3;
        }
    }
    if (nextState > 0) if (nextState < 3) {   // (two ifs: "&&" lays the blocks out differently)
        SHAPE *b = behavior->owner->routeShape;
        qword pos = GetLoWorldPos(behavior->owner);
        qword q = GetPathlinkEndpointPos(&nextGoal, pos);
        if (behavior->isDirect) {
            b->numControlVertices = 2;
            *(qword *)&b->vertexPositions[0] = pos;
            *(qword *)&b->vertexPositions[1] = q;
            MeasureShape(b);
        } else {
            SOPHYS *c = behavior->owner->physics;
            REGENT *d = c ? c->pregent : 0;
            void *e = d ? d->pathzone : 0;
            if (!FindPathzonePathCurve(e, pos, q, b)) {
                *(PATHLINKENDIsland *)&behavior->relocateGoal = nextGoal;
                nextState = 5;
            }
        }
    }
    SetBhvgotoState(behavior, nextState);
    behavior->planTime = g_clock.time;
    behavior->pathGraphVersion = ((struct W *)g_sw.pregtree)->version;
}
} // namespace M_PlanBhvgoto

// 0x17DCB8 HasBhvgotoArrived
// 0x17DCB8: True if walking directly (state 1) and at the path end.
// Also known as: NArrived (upstream readable / Sly 1 name).
extern "C" int HasBhvgotoArrived(BHVGOTO *behavior)
{
    int nState;
    int f;
    nState = behavior->state;
    f = 0;
    if (nState == 1) {
        f = (IsBhvpathAtEnd((BHVPATH *)behavior->pbhvNext) != 0) ? nState : 0;
    }
    return f;
}

// 0x17DCF8 IsBhvgotoPathFinished
namespace M_FBhvgotoPathFinished {

int IsBhvpathFinished(void *p) __asm__("IsBhvpathFinished");
// 0x17DCF8: True if walking (states 1-2) and the walk path has finished.
// Also known as: FStuck (upstream readable / Sly 1 name).
extern "C" int IsBhvgotoPathFinished(BHVGOTO *behavior)
{
    if (behavior->state >= 3)
        return 0;
    if (behavior->state <= 0)
        return 0;
    return IsBhvpathFinished(behavior->pbhvNext);
}
} // namespace M_FBhvgotoPathFinished

// 0x17DD38 SetBhvgotoGoal
// 0x17DD38: Sets the goal and re-plans.
// Also known as: SetGoal (upstream readable / Sly 1 name; SetGoal__7JOBGOTOP4GOAL).
extern "C" void SetBhvgotoGoal(BHVGOTO *behavior, PATHLINKEND *pq)
{
    behavior->goal = *pq;
    behavior->planTime = -3.40282346638528859812e+38f;
    PlanBhvgoto(behavior);
}

// 0x17DD70 InitBhvStalk
namespace M_func_0017DD70 {

void InitBhv(BHV189 *p) __asm__("InitBhv");
void AddStepguardSgf(void *p, int n) __asm__("AddStepguardSgf");
// 0x17DD70: Stalk behaviour (BHV189) vtable slot 7 (OnPush): raises flag 0x2B, state -1.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvStalk(BHV189 *behavior)
{
    InitBhv(behavior);
    AddStepguardSgf(behavior->owner, 0x2B);
    behavior->state = -1;
}
} // namespace M_func_0017DD70

// 0x17DDA8 StartBhvStalk
// 0x17DDA8: Stalk behaviour vtable slot 8 (Start): state 0 (alert animation at the target) and sets the target.
// Also known as: Enter (upstream readable / Sly 1 name).
void BHV189::Start()
{
    BHV189 *behavior = this;
    StartBhv(behavior);
    SetBhvStalkState(behavior, 0);
    SetStepguardTarget((STEPGUARD *)behavior->owner, behavior->target);
}

// 0x17DDE8 CloseBhvStalk
namespace M_func_0017DDE8 {
void CloseBhv(BHV189 *p) __asm__("CloseBhv");
void ResetDetectorTargets(void *p) __asm__("ResetDetectorTargets");
void SetStepguardTarget(STEPGUARD *t, int n) __asm__("SetStepguardTarget");
// 0x17DDE8: Stalk behaviour vtable slot 9 (Close): state -1, resets the detector, clears the target, lowers flag
// 0x2B.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvStalk(BHV189 *behavior)
{
    CloseBhv(behavior);
    SetBhvStalkState(behavior, -1);
    ResetDetectorTargets(((STEPGUARD *)behavior->owner)->pdetector);
    SetStepguardTarget((STEPGUARD *)behavior->owner, 0);
    RemoveStepguardSgf((STEPGUARD *)behavior->owner, 0x2B);
}
} // namespace M_func_0017DDE8

// 0x17DE40 IsBhvStalkDone
// 0x17DE40: True if the stalk behaviour has finished (state -1).
extern "C" int IsBhvStalkDone(BHV189 *behavior)
{
    return behavior->state == -1;
}

// 0x17DE50 UpdateBhvStalk
namespace M_func_0017DE50 {
extern float D_002BE760 __asm__("D_002BE760"), D_002BE764 __asm__("D_002BE764"), D_002BE768 __asm__("D_002BE768");
void UpdateBhv(BHV189 *p) __asm__("UpdateBhv");
int IsBhvBusyAbove(BHV189 *p) __asm__("IsBhvBusyAbove");
int IsBhvanimDone(void *p) __asm__("IsBhvanimDone");
int IsStepguardHandsOff(void *p) __asm__("IsStepguardHandsOff");
int IsStepguardForceClass(STEPGUARD *a, void *o) __asm__("IsStepguardForceClass");
float GetRandomStepguardDelay(STEPGUARD *a) __asm__("GetRandomStepguardDelay");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
qword GetPathlinkEndpointPos(PATHLINKEND *s, qword v) __asm__("GetPathlinkEndpointPos");
void RegoalBhvStalk(BHV189 *p) __asm__("RegoalBhvStalk");
// VU0 helper: distance between two points.
extern "C" { static __inline__ float Dist(qword a, qword b) {
    qword d;
    float f;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
} }
// 0x17DE50: Stalk behaviour vtable slot 10 (Update): after the alert animation, runs near the forced-class target
// and keeps re-positioning; leaves when the delay expires and the target is no longer forced.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvStalk(BHV189 *behavior) {
    int s;
    UpdateBhv(behavior);
    while (s = behavior->state, !IsBhvBusyAbove(behavior)) {
        switch (behavior->state) {
        case 0:
            if (IsBhvanimDone(behavior->pbhvNext)) SetBhvStalkState(behavior, 1);
            break;
        case 1:
            if (IsStepguardHandsOff(((STEPGUARD *)behavior->owner))) {
                SetBhvStalkState(behavior, 2);
            } else {
                { float dt = g_clock.deltaTime; behavior->forcedDelay -= dt; }
                if (behavior->forcedDelay <= 0.0f) {
                    void *o;
                    o = ((STEPGUARD *)behavior->owner)->ChooseTarget();
                    if (o == behavior->target && IsStepguardForceClass(((STEPGUARD *)behavior->owner), o)) {
                        behavior->forcedDelay = GetRandomStepguardDelay(((STEPGUARD *)behavior->owner));
                    } else {
                        SetBhvStalkState(behavior, -1);
                    }
                }
            }
            break;
        case 2:
            if (!IsStepguardHandsOff(((STEPGUARD *)behavior->owner))) SetBhvStalkState(behavior, 1);
            break;
        }
        if (behavior->state != s) continue;
        break;
    }
    if (behavior->state == 1) {
        BHVGOTO *t = (BHVGOTO *)behavior->pbhvNext;
        if (g_clock.time - behavior->lastRegoalTime > D_002BE760) {
        PATHLINKEND *x = &t->goal;
        qword pos = GetLoWorldPos(((STEPGUARD *)behavior->owner));
        int f = 0;
        qword q = GetPathlinkEndpointPos(x, pos);
        if (Dist(GetLoWorldPos(((STEPGUARD *)behavior->owner)), q) < D_002BE764) {
            f = 1;
        } else if (Dist(GetLoWorldPos(((STEPGUARD *)behavior->owner)), GetLoWorldPos(behavior->target)) < D_002BE768) {
            f = 1;
        }
        if (f) RegoalBhvStalk(behavior);
        }
    }
}
} // namespace M_func_0017DE50

// 0x17E0E0 RegoalBhvStalk
namespace M_func_0017E0E0 {

extern float D_002BE76C __asm__("D_002BE76C");
extern float D_002BE770[16] __asm__("D_002BE770");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
float atan2f(float y, float x) __asm__("atan2f");
qword GetPathlinkEndpointPos(PATHLINKEND *s, qword v) __asm__("GetPathlinkEndpointPos");
qword SetVectorCylind(float a, float b, float c) __asm__("SetVectorCylind");
int FindPathzoneClosestFreePoint(void *q, qword v, qword *out) __asm__("FindPathzoneClosestFreePoint");
struct PATHLINKENDIsland;
void ClearPointerPair(PATHLINKENDIsland *x) __asm__("ClearPointerPair");

/* PATHLINKEND with an inline constructor around ClearPointerPair: PATHLINKEND's own (out-of-line) constructor keeps &x
   in a saved register instead of rematerializing it (1 instruction). */
struct PATHLINKENDIsland { qword a; qword v; PATHLINKENDIsland() { ClearPointerPair(this); } };
// VU0 helper: x component of a vector.
// VU0 helper: y component of a vector.
// 0x17E0E0: Picks the farthest free point among 16 directions around the target and sends the goto child there.
// Also known as: ChooseAway (upstream readable / Sly 1 name).
extern "C" void RegoalBhvStalk(BHV189 *behavior) {
    union { qword q; } hit;
    qword a, b, d, r;
    BHVGOTO *t;
    SOPHYS *c;
    REGENT *dd;
    void *q;
    float ang, best;
    unsigned int i;
    behavior->lastRegoalTime = g_clock.time;
    t = (BHVGOTO *)behavior->pbhvNext;
    a = GetLoWorldPos(behavior->target);
    b = GetLoWorldPos(behavior->owner);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(b), "j"(a));
    c = behavior->owner->physics;
    dd = c ? c->pregent : 0;
    q = dd ? dd->pathzone : 0;
    best = 0.0f;
    ang = atan2f(QwordY(d), QwordX(d));
    r = GetPathlinkEndpointPos(&t->goal, b);
    for (i = 0; i < 16; i++) {
        qword o = SetVectorCylind(ang + D_002BE770[i], D_002BE76C, 0.0f);
        qword s;
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(s) : "j"(b), "j"(o));
        if (FindPathzoneClosestFreePoint(q, s, &hit.q)) {
            qword e, l;
            float g;
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(e) : "j"(hit.q), "j"(a));
            __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(e) : "j"(e));
            __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(e));
            __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
            if (g > best) {
                best = g;
                r = hit.q;
            }
        }
    }
    PATHLINKENDIsland x;
    x.v = r;
    SetBhvgotoGoal(t, (PATHLINKEND *)&x);
}
} // namespace M_func_0017E0E0

// 0x17E278 SetBhvStalkState
namespace M_func_0017E278 {
void PopBhv(void *p) __asm__("PopBhv");
BHV *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
void RegoalBhvStalk(BHV189 *p) __asm__("RegoalBhvStalk");
// 0x17E278: Stalk behaviour state change: 0 alert animation (flag 0xB) at the target, 1 run (BHVGOTO with flag
// 0x1C), 2 hands-off idle.
// Also known as: SetState (upstream readable / Sly 1 name; SetState__5JOBBDi).
extern "C" void SetBhvStalkState(BHV189 *behavior, int n)
{
    if (n == behavior->state)
        return;
    PopBhv(&behavior->pbhvNext);
    behavior->state = n;
    behavior->stateTime = g_clock.time;
    switch (n) {
    case 0: {
        BHVANIM *o = (BHVANIM *)PushStepBehavior(behavior->owner, CID_BHVANIM);
        o->sgf = 0xB;
        o->lookAt = behavior->target;
        o->Start();
    } break;
    case 1: {
        BHVGOTO *o = (BHVGOTO *)PushStepBehavior(behavior->owner, CID_BHVGOTO);
        o->allowPartialRoute = 1;
        o->sgf = 0x1C;
        RegoalBhvStalk(behavior);
        o->Start();
    } break;
    case 2: {
        BHVIDLE *o = (BHVIDLE *)PushStepBehavior(behavior->owner, CID_BHVIDLE);
        o->sgf = 1;
        o->Start();
    } break;
    }
}
} // namespace M_func_0017E278

// 0x17E3A0 InitBhvpathlink
// 0x17E3A0: BHVPATHLINK vtable slot 7 (OnPush): raises flag 0x2C, state -1.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvpathlink(BHVPATHLINK *behavior)
{
    InitBhv(behavior);
    behavior->state = -1;
    AddStepguardSgf((STEPGUARD *)behavior->owner, SGF_PATHLINK);
}

// 0x17E3D8 StartBhvpathlink
// 0x17E3D8: BHVPATHLINK vtable slot 8 (Start): busy, begins the first link segment.
// Also known as: Enter (upstream readable / Sly 1 name).
void BHVPATHLINK::Start()
{
    BHVPATHLINK *behavior = this;
    StartBhv(behavior);
    SetBhvBusy(behavior);
    AdvanceBhvpathlink(behavior);
}

// 0x17E410 CloseBhvpathlink
// 0x17E410: BHVPATHLINK vtable slot 9 (Close): stops, clears busy, lowers flag 0x2C.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvpathlink(BHVPATHLINK *behavior)
{
    CloseBhv(behavior);
    SetBhvpathlinkState(behavior, -1);
    ClearBhvBusy(behavior);
    RemoveStepguardSgf((STEPGUARD *)behavior->owner, SGF_PATHLINK);
}

// 0x17E458 UpdateBhvpathlink
namespace M_UpdateBhvpathlink {
void UpdateBhv(BHVPATHLINK *p) __asm__("UpdateBhv");
int IsBhvpathlinkDone(BHVPATHLINK *p) __asm__("IsBhvpathlinkDone");
int IsBhvRelocateDone(void *p) __asm__("IsBhvRelocateDone");
int IsBhvanimDone(void *p) __asm__("IsBhvanimDone");
int IsBhvpathlinkAtSegmentEnd(BHVPATHLINK *p) __asm__("IsBhvpathlinkAtSegmentEnd");
int IsBhvpathAtEnd(void *p) __asm__("IsBhvpathAtEnd");
// 0x17E458: BHVPATHLINK vtable slot 10 (Update): per segment, kind 0 walks (with a timeout) and kind 1 leaps;
// advances at the segment end; a final relocation step.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvpathlink(BHVPATHLINK *behavior) {
    UpdateBhv(behavior);
    if (IsBhvpathlinkDone(behavior)) return;
    switch (behavior->state) {
    case 1:
        if (IsBhvRelocateDone(behavior->pbhvNext)) AdvanceBhvpathlink(behavior);
        break;
    case 0: {
        PATHLINK *t = behavior->pathlink;
        switch (t->kind) {
        case 1:
            if (IsBhvanimDone(behavior->pbhvNext)) {
                if (IsBhvpathlinkAtSegmentEnd(behavior)) AdvanceBhvpathlink(behavior);
                else SetBhvpathlinkState(behavior, 1);
            }
            break;
        case 0:
            if (behavior->segmentTimeout < g_clock.time) {
                SetBhvpathlinkState(behavior, 1);
            } else {
                SOPHYS *c = behavior->owner->physics;
                REGENT *d = c ? c->pregent : 0;
                CID17 *n = d ? d->pathzone : 0;
                if (n == t->endZone && IsBhvpathAtEnd(behavior->pbhvNext)) AdvanceBhvpathlink(behavior);
            }
            break;
        }
        break;
    }
    }
}
} // namespace M_UpdateBhvpathlink

// 0x17E570 IsBhvpathlinkDone
// 0x17E570: True if the path-link traversal has finished (state -1).
extern "C" int IsBhvpathlinkDone(BHVPATHLINK *behavior)
{
    return behavior->state == -1;
}

// 0x17E580 AdvanceBhvpathlink
// 0x17E580: Advances to the next segment of the path link, or finishes and clears busy.
// Also known as: NextRecord (upstream readable / Sly 1 name).
extern "C" void AdvanceBhvpathlink(BHVPATHLINK *behavior)
{
    int i;
    i = behavior->segmentIndex + 1;
    behavior->segmentIndex = i;
    if (i < behavior->pathlink->numEnds) {
        SetBhvpathlinkState(behavior, -1);
        SetBhvpathlinkState(behavior, 0);
        return;
    }
    SetBhvpathlinkState(behavior, -1);
    ClearBhvBusy(behavior);
}

// 0x17E5F0 IsBhvpathlinkAtSegmentEnd
namespace M_FBhvpathlinkAtSegmentEnd {
int IsNearPathlinkEndpoint(void *pe, void *p, float g) __asm__("IsNearPathlinkEndpoint");
// 0x17E5F0: True if the guard is within 25 of the current segment's end point.
extern "C" int IsBhvpathlinkAtSegmentEnd(BHVPATHLINK *behavior)
{
    return IsNearPathlinkEndpoint(&behavior->pathlink->aend[behavior->segmentIndex], behavior->owner, 25.0f);
}
} // namespace M_FBhvpathlinkAtSegmentEnd

// 0x17E628 PushBhvpathlinkLeap
namespace M_PushBhvpathlinkLeap {

BHVLEAP *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
qword GetPathlinkEndpointPos(void *pe, qword q) __asm__("GetPathlinkEndpointPos");
// 0x17E628: Pushes a leap (BHVLEAP) to the current segment's end point.
extern "C" void PushBhvpathlinkLeap(BHVPATHLINK *behavior) {
    BHVLEAP *o = PushStepBehavior(behavior->owner, CID_BHVLEAP);
    o->extra = 1;
    o->landingPoint = GetPathlinkEndpointPos(&behavior->pathlink->aend[behavior->segmentIndex], GetLoWorldPos(behavior->owner));
    o->Start();
}
} // namespace M_PushBhvpathlinkLeap

// 0x17E6B0 SetBhvpathlinkState
namespace M_SetBhvpathlinkState {
struct SEG;
extern float D_002BE7B0 __asm__("D_002BE7B0"), D_002BE7B4 __asm__("D_002BE7B4");
struct SEG { qword a; qword b; };   /* a PATHLINKEND as two quadwords (the relocate goal is copied so) */
void PopBhv(void *p) __asm__("PopBhv");
BHV *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
qword GetPathlinkEndpointPos(PATHLINKEND *s, qword v) __asm__("GetPathlinkEndpointPos");
void MeasureShape(SHAPE *b) __asm__("MeasureShape");
void PushBhvpathlinkLeap(BHVPATHLINK *p) __asm__("PushBhvpathlinkLeap");
// 0x17E6B0: BHVPATHLINK state change: 0 the segment (a two-point walk with a timeout, or a leap), 1 relocate.
// Also known as: SetState (upstream readable / Sly 1 name; SetState__7JOBLINKi).
extern "C" void SetBhvpathlinkState(BHVPATHLINK *behavior, int n) {
    if (n == behavior->state) return;
    PopBhv((&behavior->pbhvNext));
    behavior->state = n;
    behavior->stateTime = g_clock.time;
    switch (n) {
    case 0:
        switch (behavior->pathlink->kind) {
        case 0:
            {
                BHVWALKPATH *o = (BHVWALKPATH *)PushStepBehavior(behavior->owner, CID_BHVWALKPATH);
                SHAPE *b = behavior->owner->routeShape;
                qword pos = GetLoWorldPos(((STEPGUARD *)behavior->owner));
                qword q = GetPathlinkEndpointPos(&behavior->pathlink->aend[behavior->segmentIndex], pos);
                qword d;
                float len;
                float t = g_clock.time;
                __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pos), "j"(q));
                __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(d));
                __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
                __asm__("qmfc2.ni %0, %1" : "=r"(len) : "j"(d));
                {
                    float g = len / D_002BE7B0;
                    behavior->segmentTimeout = t + (g < D_002BE7B4 ? D_002BE7B4 : g);
                }
                b->numControlVertices = 2;
                *(qword *)&b->vertexPositions[0] = pos;
                *(qword *)&b->vertexPositions[1] = q;
                MeasureShape(b);
                o->ignoresBlockers = 1;
                o->arrivalRadius = 10.0f;
                SetBhvpathShape(o, b, 0);
                StartPushedBhv(o);
            }
            break;
        case 1:
            PushBhvpathlinkLeap(behavior);
            break;
        }
        break;
    case 1:
        {
            BHV191 *o = (BHV191 *)PushStepBehavior(behavior->owner, CID_BHV191);
            /* aend[segmentIndex] (sg[3] == +0x60); the int arithmetic keeps the target's address computation */
            struct SEG *sg = (struct SEG *)(behavior->segmentIndex * 32 + (int)behavior->pathlink);
            *(qword *)&o->end = sg[3].a;
            o->end.pos = sg[3].b;
            StartPushedBhv(o);
        }
        break;
    }
}
} // namespace M_SetBhvpathlinkState

// 0x17E888 InitBhvRelocate
// 0x17E888: Relocate behaviour (BHV191) vtable slot 7 (OnPush): raises flag 0x2D.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvRelocate(BHV *behavior)
{
    InitBhv(behavior);
    AddStepguardSgf((STEPGUARD *)behavior->owner, 0x2D);
}

// 0x17E8B8 StartBhvRelocate
// 0x17E8B8: Relocate behaviour vtable slot 8 (Start): moves to the point.
// Also known as: Enter (upstream readable / Sly 1 name).
void BHV191::Start()
{
    BHV *behavior = (BHV *)this;
    StartBhv(behavior);
    MoveBhvRelocateToPoint(behavior);
}

// 0x17E8E8 CloseBhvRelocate
// 0x17E8E8: Relocate behaviour vtable slot 9 (Close): lowers flag 0x2D.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvRelocate(BHV *behavior)
{
    CloseBhv(behavior);
    RemoveStepguardSgf((STEPGUARD *)behavior->owner, 0x2D);
}

// 0x17E918 UpdateBhvRelocate
namespace M_func_0017E918 {
void UpdateBhv(BHV191 *p) __asm__("UpdateBhv");
int IsBhvanimDone(void *p) __asm__("IsBhvanimDone");
int IsNearPathlinkEndpoint(void *pe, void *p, float g) __asm__("IsNearPathlinkEndpoint");
void MoveBhvRelocateToPoint(BHV191 *p) __asm__("MoveBhvRelocateToPoint");
// 0x17E918: Relocate behaviour vtable slot 10 (Update): retries until within 25 of the point.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvRelocate(BHV191 *behavior) {
    UpdateBhv(behavior);
    if (((STEPGUARD *)behavior->owner)->capabilityFlags & 1) {
        if (IsBhvanimDone(behavior->pbhvNext) && !IsNearPathlinkEndpoint(&behavior->end, behavior->owner, 25.0f)) MoveBhvRelocateToPoint(behavior);
    } else if (!IsNearPathlinkEndpoint(&behavior->end, behavior->owner, 25.0f)) MoveBhvRelocateToPoint(behavior);
}
} // namespace M_func_0017E918

// 0x17E9B0 MoveBhvRelocateToPoint
namespace M_func_0017E9B0 {
extern qword D_002A2DD0 __asm__("D_002A2DD0");
void PopBhv(void *p) __asm__("PopBhv");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
qword GetPathlinkEndpointPos(void *p, qword q) __asm__("GetPathlinkEndpointPos");
int SnapToGround(qword *pq, float a, float b, int n, void *pfn) __asm__("SnapToGround");
void SnapPositionToSurfaceAbove(qword *pq) __asm__("SnapPositionToSurfaceAbove");
BHV *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
void DetachStepguard(STEPGUARD *p) __asm__("DetachStepguard");
// 0x17E9B0: Snaps the relocation point to the ground (or the surface above), then leaps there (capability bit 0) or
// teleports (detach, set the position, zero the velocities).
// Also known as: Start (upstream readable / Sly 1 name).
void MoveBhvRelocateToPoint(BHV191 *behavior) __asm__("MoveBhvRelocateToPoint"); void MoveBhvRelocateToPoint(BHV191 *behavior) {
    union { qword q; float a[4]; } u;
    PopBhv(&behavior->pbhvNext);
    u.q = GetPathlinkEndpointPos(&behavior->end, GetLoWorldPos(behavior->owner));
    if (!SnapToGround(&u.q, 150.0f, -250.0f, 1, 0)) SnapPositionToSurfaceAbove(&u.q);
    if (((STEPGUARD *)behavior->owner)->capabilityFlags & 1) {
        BHVLEAP *o = (BHVLEAP *)PushStepBehavior(behavior->owner, CID_BHVLEAP);
        o->landingPoint = u.q;
        o->extra = 1;
        StartPushedBhv(o);
    } else {
        DetachStepguard((STEPGUARD *)behavior->owner);
        {
            STEP *o = behavior->owner;
            { qword u_q; __asm__("vsub.xyzw %0, %1, %2" : "=j"(u_q) : "j"(u.q), "j"(o->pivotOffset)); u.q = u_q; }
            o->TranslateToPos(u.q);
        }
        behavior->owner->SetVelocity(D_002A2DD0);
        behavior->owner->SetAngularVelocity(D_002A2DD0);
    }
}
} // namespace M_func_0017E9B0

// 0x17EAE8 IsBhvRelocateDone
namespace M_func_0017EAE8 {
int IsBhvanimDone(void *p) __asm__("IsBhvanimDone");
int IsNearPathlinkEndpoint(void *pe, void *p, float g) __asm__("IsNearPathlinkEndpoint");
// 0x17EAE8: True if the relocation is complete: the leap (if any) finished and the guard is within 25 of the point.
// Also known as: FDone (upstream readable / Sly 1 name).
extern "C" int IsBhvRelocateDone(BHV191 *behavior)
{
    if (((STEPGUARD *)behavior->owner)->capabilityFlags & 1) {
        if (!IsBhvanimDone(behavior->pbhvNext))
            return 0;
    }
    return IsNearPathlinkEndpoint(&behavior->end, behavior->owner, 25.0f);
}
} // namespace M_func_0017EAE8

// 0x17EB48 InitBhvpatrolpath
// 0x17EB48: BHVPATROLPATH vtable slot 7 (OnPush): raises flag 0x2E, no parameter.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvpatrolpath(BHVPATROLPATH *behavior)
{
    InitBhv(behavior);
    behavior->pathParam = -1.0f;
    AddStepguardSgf((STEPGUARD *)behavior->owner, SGF_PATROLPATH);
}

// 0x17EB88 StartBhvpatrolpath
namespace M_StartBhvpatrolpath {
struct G;
extern struct G D_003DD020 __asm__("D_003DD020");
void StartBhv(BHVPATROLPATH *p) __asm__("StartBhv");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
void FindShapeClosestPointFromDistance(SHAPE *a, float g, qword q, struct G *pg, void *pv, qword *b, float *c, float *pf) __asm__("FindShapeClosestPointFromDistance");
void FindShapeClosestPoint(SHAPE *a, qword q, struct G *pg, void *pv, qword *b, float *c, float *pf) __asm__("FindShapeClosestPoint");
BHV *PushStepBehavior(void *p, int n) __asm__("PushStepBehavior");
void AdvanceBhvpatrolpathGoal(BHVPATROLPATH *p, int n) __asm__("AdvanceBhvpatrolpathGoal");
// 0x17EB88: BHVPATROLPATH vtable slot 8 (Start): starts at the closest parameter on the path shape, pushes the goto
// child and its first goal.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvpatrolpath(BHVPATROLPATH *behavior) {
    BHVGOTO *o;
    StartBhv(behavior);
    if (behavior->pathParam >= 0.0f)
        FindShapeClosestPointFromDistance(behavior->shape, behavior->pathParam, GetLoWorldPos(behavior->owner), &D_003DD020, &behavior->goal, 0, 0, &behavior->pathParam);
    else
        FindShapeClosestPoint(behavior->shape, GetLoWorldPos(behavior->owner), &D_003DD020, &behavior->goal, 0, 0, &behavior->pathParam);
    o = (BHVGOTO *)PushStepBehavior(behavior->owner, CID_BHVGOTO);
    o->lookAtTarget = behavior->lookAtTarget;
    o->unknownAC = behavior->unknown54;
    o->isDirect = behavior->isDirect;
    AdvanceBhvpatrolpathGoal(behavior, 1);
    o->Start();
}
} // namespace M_StartBhvpatrolpath

// 0x17EC80 CloseBhvpatrolpath
// 0x17EC80: BHVPATROLPATH vtable slot 9 (Close): lowers flag 0x2E.
// Also known as: Exit (upstream readable / Sly 1 name).
void BHVPATROLPATH::Close()
{
    BHV *behavior = (BHV *)this;
    CloseBhv(behavior);
    RemoveStepguardSgf((STEPGUARD *)behavior->owner, SGF_PATROLPATH);
}

// 0x17ECB0 UpdateBhvpatrolpath
// 0x17ECB0: BHVPATROLPATH vtable slot 10 (Update): on arrival, advances the goal along the shape.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvpatrolpath(BHV *behavior)
{
    UpdateBhv(behavior);
    if (HasBhvgotoArrived((BHVGOTO *)behavior->pbhvNext) != 0) {
        AdvanceBhvpatrolpathGoal(behavior, 0);
    }
}

// 0x17ECF0 AdvanceBhvpatrolpathGoal
namespace M_AdvanceBhvpatrolpathGoal {
struct R;
extern float D_002BE7BC __asm__("D_002BE7BC"), D_002BE7C0 __asm__("D_002BE7C0"), D_002BE7C4 __asm__("D_002BE7C4");
struct R { float start; float end; };
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
float GetShapeMinDistance(SHAPE *b) __asm__("GetShapeMinDistance");
float GetShapeMaxDistance(SHAPE *b) __asm__("GetShapeMaxDistance");
float WrapShapeDistance(SHAPE *b, float f) __asm__("WrapShapeDistance");
void BuildShapeValidRanges(void *p) __asm__("BuildShapeValidRanges");
struct R *FindRangeContaining(void *p, float f) __asm__("FindRangeContaining");
void EvaluateShapeAtDistance(SHAPE *b, float f, qword *pv, int n) __asm__("EvaluateShapeAtDistance");
/* PATHLINKEND cleared by an explicit ClearPointerPair call (block scope): its constructor at that point changes the
   schedule (2 instructions). */
struct PATHLINKENDIsland { qword a; qword v; };
void ClearPointerPair(PATHLINKENDIsland *x) __asm__("ClearPointerPair");
// 0x17ECF0: Steps the shape parameter by a class-dependent distance in the travel direction (clamping or wrapping,
// skipping invalid ranges) and sends it to the goto child.
// Also known as: Step (upstream readable / Sly 1 name; Step__11JOBPATHSTEPi).
void AdvanceBhvpatrolpathGoal(BHVPATROLPATH *behavior, int force) __asm__("AdvanceBhvpatrolpathGoal"); void AdvanceBhvpatrolpathGoal(BHVPATROLPATH *behavior, int force) {
    float d;
    float t;
    if (IsBasicDerivedFrom__FP5BASIC3CID(behavior->owner, CID_GUARD)) d = D_002BE7BC; else d = D_002BE7C0;
    if (behavior->isForward == 0) d = -d;
    t = behavior->pathParam + d;
    if (behavior->shape->isClosed == 0) {
        float lo = GetShapeMinDistance(behavior->shape);
        float hi = GetShapeMaxDistance(behavior->shape);
        if (t < lo) t = lo;
        else if (hi < t) t = hi;
    } else {
        t = WrapShapeDistance(behavior->shape, t);
    }
    if (behavior->isDirect == 0) {
        struct R *r;
        BuildShapeValidRanges(behavior->shape->pranges);
        r = FindRangeContaining(behavior->shape->pranges, t);
        if (r) t = WrapShapeDistance(behavior->shape, behavior->isForward != 0 ? r->end : r->start);
    }
    if (force == 0 && __builtin_fabsf(behavior->pathParam - t) < D_002BE7C4) return;
    {
        BHVGOTO *q;
        PATHLINKENDIsland x;
        behavior->pathParam = t;
        EvaluateShapeAtDistance(behavior->shape, t, &behavior->goal, 0);
        q = (BHVGOTO *)behavior->pbhvNext;
        ClearPointerPair(&x);
        x.v = behavior->goal;
        SetBhvgotoGoal(q, (PATHLINKEND *)&x);
    }
}
} // namespace M_AdvanceBhvpatrolpathGoal

// 0x17EE78 IsBhvpatrolpathDone
namespace M_FBhvpatrolpathDone {

int IsBhvpatrolpathAtEnd(BHVPATROLPATH *p) __asm__("IsBhvpatrolpathAtEnd");
int HasBhvgotoArrived(void *p) __asm__("HasBhvgotoArrived");
// 0x17EE78: True if the patrol path reached the end of an open shape and arrived.
// Also known as: FDone (upstream readable / Sly 1 name).
extern "C" int IsBhvpatrolpathDone(BHVPATROLPATH *behavior)
{
    if (!IsBhvpatrolpathAtEnd(behavior)) return 0;
    return HasBhvgotoArrived(behavior->pbhvNext);
}
} // namespace M_FBhvpatrolpathDone

// 0x17EEB8 IsBhvpatrolpathAtEnd
namespace M_FBhvpatrolpathAtEnd {
extern float D_002BE7B8 __asm__("D_002BE7B8");
float GetShapeMaxDistance(SHAPE *p) __asm__("GetShapeMaxDistance");
float GetShapeMinDistance(SHAPE *p) __asm__("GetShapeMinDistance");
// 0x17EEB8: True if the shape is open and the parameter is within tolerance of its end.
// Also known as: FAtEnd (upstream readable / Sly 1 name).
extern "C" int IsBhvpatrolpathAtEnd(BHVPATROLPATH *behavior) {
    float g;
    if (behavior->shape->isClosed) return 0;
    if (behavior->isForward) g = GetShapeMaxDistance(behavior->shape); else g = GetShapeMinDistance(behavior->shape);
    if (D_002BE7B8 < __builtin_fabsf(behavior->pathParam - g)) return 0;
    return 1;
}
} // namespace M_FBhvpatrolpathAtEnd

// 0x2562A8 ClearPointerPair
namespace M_func_002562A8 {
typedef struct { int a; int b; } P;
// 0x2562A8: Inline default constructor of a two-pointer record (both cleared); linkonce copy emitted here.
extern "C" { inline void ClearPointerPair(P *p) { p->a = 0; p->b = 0; } }   /* inline, after its callers: they call it and g++ emits the linkonce copy */
} // namespace M_func_002562A8


// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
VECTOR4 D_002BE600 = {0.0f, 0.005f, 0.0f, 0.0f};
float D_002BE610 = 0.25f;
float D_002BE614 = 500.0f;
float D_002BE618[2] = {3.0f, 5.0f};
float D_002BE620 = 5.0f;
float D_002BE624 = 25.0f;
float D_002BE628 = 60.0f;
float D_002BE62C = 100.0f;
float D_002BE630 = 500.0f;
float D_002BE634 = 100.0f;
VECTOR4 D_002BE640 = {200.0f, -0.5f, 0.0f, 0.0f};
LM D_002BE650 = {0.0f, 150.0f};
float D_002BE658 = 7.5f;
float D_002BE65C = 30.0f;
float D_002BE660 = 250.0f;
float D_002BE664 = 0.5f;
float D_002BE668 = 200.0f;
float D_002BE66C = 10000.0f;
float D_002BE670 = 350.0f;
float D_002BE674 = 6000.0f;
VECTOR4 D_002BE680 = {0.5f, -1.0f, 0.0f, 0.0f};
float D_002BE690 = 1.0f;
float D_002BE694 = 1.5f;
VECTOR4 D_002BE6A0 = {0.5f, 0.0033333334f, 0.0f, 0.0f};
float D_002BE6B0 = 25.0f;
LM D_002BE6B8 = {-150.0f, 100.0f};
float D_002BE6C0 = 1.0f;
float D_002BE6C4 = -1500.0f;
float D_002BE6C8 = 35.0f;
float D_002BE6CC = 0.25f;
int D_002BE6D0[5] = {55, 56, 57, 58, 59};
int D_002BE6E8[5] = {62, 63, 64, 65, 66};
float D_002BE6FC = 0.1f;
float D_002BE700 = 0.6f;
float D_002BE704 = 1.0f;
LM D_002BE708 = {1750.0f, 0.0f};
VECTOR4 D_002BE710 = {1.0f, 2.0f, 0.0f, 0.0f};
VECTOR4 D_002BE720 = {-300.0f, 0.9f, 0.0f, 0.0f};
VECTOR4 D_002BE730 = {600.0f, -2.0f, 0.0f, 0.0f};
LM D_002BE740 = {0.0f, 600.0f};
float D_002BE748 = 200.0f;
float D_002BE74C = 150.0f;
float D_002BE750 = 0.0f;
float D_002BE754 = 5000.0f;
float D_002BE758 = 5.0f;
float D_002BE75C = 50.0f;
float D_002BE760 = 0.5f;
float D_002BE764 = 150.0f;
float D_002BE768 = 300.0f;
float D_002BE76C = 500.0f;
float D_002BE770[16] = {0.0f, 0.15f, -0.15f, 0.3f, -0.3f, 0.6f, -0.6f, 1.0f, -1.0f, 1.5f, -1.5f, 2.0f, -2.0f, 2.5f, -2.5f, 3.14f};
float D_002BE7B0 = 75.0f;
float D_002BE7B4 = 2.0f;
float D_002BE7B8 = 10.0f;
float D_002BE7BC = 100.0f;
float D_002BE7C0 = 250.0f;
float D_002BE7C4 = 5.0f;
unsigned long D_002DFB70;
int D_002DFB78;
float D_002DFB7C;
