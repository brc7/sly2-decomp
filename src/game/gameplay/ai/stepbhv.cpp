// stepbhv.cpp -- more step behaviours: walk to a goal (BHVWALKGOAL, BHV168, a path behaviour steering along a
// jt-goal), walk-to with path planning (BHV193), follow an object (BHVFOLLOW, BHV194, with its distance-band state
// machine and path zones), three approach-and-interact behaviours of the same shape (BHV195/196/197: approach a
// target, then interact through SetPlayerInteractMode) and one of Murray's (BHV198).
// TU 0x144B28..0x1471C0 (43 functions, address order).
// Types: classes.h BHVWALKGOAL/BHV193/BHVFOLLOW/BHV195..198, guard.h STEPGUARD/TARGETREC, players.h MURRAY/BENTLEY,
// jt.h JT, stepplayer.h STEPPLAYER, so.h SO/SOPHYS/REGENT. Functions with .data or local VU0 helpers keep them in a
// namespace named after the function (as vec.cpp and mark.cpp do).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_stepbhv
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_stepbhv
#include "sly2/basic.h"
#include "sly2/so.h"
#include "sly2/guard.h"
#include "sly2/players.h"
#include "sly2/slotheap.h"
#include "sly2/scrap.h"
#include "sly2/classes.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/clock.h"

// 0x147068 StaticInit_stepbhv and GlobalConstructors_stepbhv: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* Class ids used here (docs/systems/object-model.md section 1). */
enum {
    CID_SO = 0x30,
    CID_PO = 0x32,
    CID_STEP = 0x33,
    CID_STEPPLAYER = 0x34,
    CID_JT = 0x35,          /* Sly */
    CID_MURRAY = 0x36,
    CID_BENTLEY = 0x37,
    CID_BHV168 = 0xA8,
    CID_BHVINTERACT = 0xC3,     /* BHV195 (Murray) */
    CID_BHVINTERACT2 = 0xC4,    /* BHV196 (Bentley) */
    CID_BHVINTERACT3 = 0xC5,    /* BHV197 (Sly) */
};
/* Guard flags (STEPGUARD +0x2C0, bit n = flag n; each behaviour raises its own: docs/systems/ai-guards.md
   section 2). Only the identified flags are named. */
enum {
    SGF_STRIKE = 0x3,
    SGF_KO = 0x7,
    SGF_JUGGLED = 0x17,
};

/* The behaviours are the classes.h ones (BHVWALKGOAL, BHV193, BHVFOLLOW, BHV195..198 on BHV/BHVPATH); their owners are
   partners (STEPPLAYER: MURRAY, BENTLEY, JT), read through those types. */
extern qword g_normalZ;   /* the up axis (0, 0, 1) */
extern qword D_002A2DA0;   /* the default direction for a zero vector */
extern "C" {
void InitBhv(BHV *behavior);
void StartBhv(BHV *behavior);
void CloseBhv(BHV *behavior);
void UpdateBhv(BHV *behavior);
void SetBhvBusy(BHV *behavior);
void ClearBhvBusy(BHV *behavior);
int IsBhvBusyAbove(BHV *behavior);
void ResetBhvSteering(BHV *behavior, POCTL *poctl);
void PopBhv(BHV **slot);
BHV *PushStepBehavior(STEP *step, int cid);
void InitBhvpath(BHVPATH *behavior);
void StartBhvpath(BHVPATH *behavior);
void UpdateBhvpath(BHVPATH *behavior);
int IsBhvpathAtEnd(BHVPATH *path);
void SetBhvpathShape(BHVPATH *path, SHAPE *shape, int n);
void SetJtgoal(JTGOAL *pjtgoal, qword pos);
void SetBhvWalkToGoal(BHV193 *behavior, qword goal);
void SetBhvfollowState(BHVFOLLOW *behavior, int state);
void UpdateBhvfollowPosition(BHVFOLLOW *behavior);
CID17 *GetBhvfollowLeaderGround(BHVFOLLOW *behavior);
int IsBhvfollowLeaderOnNewGround(BHVFOLLOW *behavior);
STEPGUARD *PickBhvfollowTarget(BHVFOLLOW *behavior);
int IsBhvInteractIdle(BHV195 *behavior);
int IsBhvInteract2Idle(BHV196 *behavior);
int IsBhvInteract3Idle(BHV197 *behavior);
void SetBhvInteractState(BHV195 *behavior, int state);
void SetBhvInteract2State(BHV196 *behavior, int state);
void SetBhvInteract3State(BHV197 *behavior, int state);
int IsLoInWorld(void *object);
int IsBasicDerivedFrom__FP5BASIC3CID(void *object, int cid);
int GetBasicCid(void *object);
CID17 *FindPathzoneAtPos(qword pos, int n);
int FindPathzonePathCurve(CID17 *pathZone, qword start, qword goal, SHAPE *path);
int IsPosFreeInPathzone(CID17 *pathZone, qword pos, int n);
void MeasureShape(SHAPE *shape);
qword GetLoWorldPos(void *object);
int SnapToGround(qword *pos, float above, float below, int c, int d);
void ReturnJtToPosition(STEP *step, qword pos);
unsigned long GetSgfMask(int n);
int StepguardHasSgf(STEPGUARD *guard, int sgf);
TARGETREC *FindTargetRecordById(void *object);
int IsWithinLimits__FP2LMf(LM *range, float g);
void SetAsegaSpeed__FP5ASEGAf(ASEGA *asega, float g);
float RandomFloatInRange__Fff(float a, float b);
float atan2f(float y, float x);
void UpdatePoctlPressed(POCTL *poctl);
void SetMurrayState(MURRAY *murray, int state);
void SetMurrayPowerSubstate(MURRAY *murray, int n);
}

// 0x144B28: Walk-goal behaviour (BHVWALKGOAL, BHV168) vtable slot 7 (OnPush): path behaviour base, speed scale 1.
void BHVWALKGOAL::OnPush()
{
    InitBhvpath(this);
    speedScale = 1.0f;
}

// 0x144B58: Walk-goal behaviour vtable slot 8 (Start): base, then sets the mover's jt-goal to the behaviour's goal
// (BHVPATH +0x80).
extern "C" void StartBhvwalkgoal(BHVWALKGOAL *behavior)
{
    StartBhvpath(behavior);
    SetJtgoal(&((STEPPLAYER *)behavior->owner)->jtgoal, behavior->startPosition);
}

// 0x144B90: Walk-goal behaviour vtable slot 10 (Update): path behaviour update, then refreshes the jt-goal.
extern "C" void UpdateBhvwalkgoal(BHVWALKGOAL *behavior)
{
    UpdateBhvpath(behavior);
    SetJtgoal(&((STEPPLAYER *)behavior->owner)->jtgoal, behavior->startPosition);
}

// Keeps its .data and VU0 helpers in a namespace of its own (see the file header).
namespace M_UpdateBhvwalkgoalPoctl {
float D_002BD2E0[3] __asm__("D_002BD2E0") = {0.0f, 0.01f, 0.0f};
extern float g_lmZeroOne[2] __asm__("g_lmZeroOne");
float D_002BD2F0 __asm__("D_002BD2F0") __attribute__((aligned(16))) = 500.0f;

// VU0 helper: horizontal (xy) length of a vector.
extern "C" { static __inline__ float LenXY(qword d)
{
    float g;
    qword t;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(t) : "j"(d));
    d = t;
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
} }
// VU0 helper: horizontal (xy) length of a vector (second instruction pattern for the match).
extern "C" { static __inline__ float LenXY2(qword d)
{
    float g;
    qword t;
    /* volatile: raises the priority of the preceding limit load so lui/lwc1 interleave with vsub (Claude agent) */
    __asm__ __volatile__("vmul.xy %0, %1, %1" : "=j"(t) : "j"(d));
    __asm__("vaddy.x %0, %0, %0y" : "+j"(t));
    d = t;
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
} }

// True if two points are farther apart (horizontally) than the .data limit.
extern "C" { static __inline__ int IsFarApart(qword a, qword b)
{
    qword d;
    float lim = D_002BD2F0;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    return lim < LenXY2(d);
} }
// VU0 helper: dot product.
extern "C" { static __inline__ qword Dot(qword a, qword b) { register qword r __asm__("vf2"); register qword t __asm__("vf4");
        __asm__("vmul.xyz %1, %2, %3\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z"
                : "=&j"(t), "=&j"(r) : "j"(a), "j"(b)); return r; } }
// VU0 helper: component of d perpendicular to an axis.
extern "C" { static __inline__ qword Perp(qword d, qword axis) { qword v, r;
        v = Dot(d, axis);
        __asm__("vmulx.xyzw %0, %1, %0x" : "+j"(v) : "j"(axis));
        __asm__("vsub.xyzw %0, %1, %0" : "+j"(v) : "j"(d)); return v; } }
// Evaluates a quadratic a0 + g (a1 + g a2).
extern "C" { static __inline__ float EvaluatePolynomial(float *a, float g) { return a[0] + g * (a[1] + g * a[2]); } }
// 0x144BC8: Walk-goal behaviour vtable slot 14 (steering): base fills the steering record, then while travelling
// (path states 0/1) sets the POCTL stick: speed from the horizontal distance to the goal (scaled) and heading
// toward it; holds button 8 when asked or when far from the mirrored PO, and after a delay copies that PO's masked
// buttons (an AI partner copying the player's presses, inferred).
// Also known as: Drive (upstream readable / Sly 1 name; Drive__14JOBACTORFOLLOWP6PADCTL).
extern "C" void UpdateBhvwalkgoalPoctl(BHVWALKGOAL *behavior, POCTL *poctl)
{
    ResetBhvSteering(behavior, poctl);
    if (behavior->state < 2) if (behavior->state >= 0) {
        qword pos;
        qword toGoal, v, t, up;
        float speed, clampedSpeed;
        __asm__("lqc2 %0, %1" : "=j"(pos) : "m"(behavior->startPosition));
        v = GetLoWorldPos(behavior->owner);
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(toGoal) : "j"(pos), "j"(v));
        up = g_normalZ;
        v = Perp(toGoal, up);   /* horizontal part of the offset to the goal */
        speed = EvaluatePolynomial(D_002BD2E0, LenXY(v));
        clampedSpeed = Clamp(speed, g_lmZeroOne[0], g_lmZeroOne[1]);
        if (0.0f < clampedSpeed) {
            poctl->leftStickDeflection = clampedSpeed * behavior->speedScale;
            poctl->leftStickAngle = atan2f(QwordY(v), QwordX(v));
        }
        if (behavior->forcesButton8)
            poctl->buttonsHeld |= 8;
        if (behavior->mirrorPoctl && behavior->mirrorDelay < g_clock.time - behavior->stateTime) {
            int mirror = 1;
            PO *leader = behavior->mirrorPoctl->controlled;
            qword own, other;
            __asm__("qmtc2.ni %1, %0" : "=j"(own) : "r"(GetLoWorldPos(behavior->owner)));
            __asm__("qmtc2.ni %1, %0" : "=j"(other) : "r"(GetLoWorldPos(leader)));
            if (IsFarApart(own, other))
                poctl->buttonsHeld |= 8;
            if (IsBasicDerivedFrom__FP5BASIC3CID(leader, CID_STEPPLAYER))
                mirror = ((STEPPLAYER *)leader)->GetPlayerState() != 0;
            if (mirror) {
                poctl->buttonsHeld |= behavior->mirrorPoctl->buttonsHeld & behavior->mirrorButtonMask;
                UpdatePoctlPressed(poctl);
            }
        }
    }
}
} // namespace M_UpdateBhvwalkgoalPoctl

// 0x144E20: Walk-to behaviour (BHV193) vtable slot 7 (OnPush): base, defaults (arrival radius 40, flags 1, speed
// scale 1).
// Also known as: Init (upstream readable / Sly 1 name).
void BHV193::OnPush()
{
    InitBhv(this);
    arrivalRadius = 40.0f;
    forcesButton8 = 1;
    speedScale = 1.0f;
}

// 0x144E68: Walk-to behaviour vtable slot 8 (Start): pushes a walk-goal child (cid 0xA8) with its parameters and
// goal and starts it.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvWalkTo(BHV193 *behavior)
{
    BHVWALKGOAL *child;
    StartBhv(behavior);
    child = (BHVWALKGOAL *)PushStepBehavior(behavior->owner, CID_BHV168);
    child->ignoresBlockers = behavior->usesStraightPath;
    child->arrivalRadius = behavior->arrivalRadius;
    child->forcesButton8 = behavior->forcesButton8;
    child->speedScale = behavior->speedScale;
    SetBhvWalkToGoal(behavior, behavior->goal);
    child->Start();
}

// 0x144EE8: True when the walk-to child has reached the end of its path or its path state is 2.
// Also known as: FAtGoal (upstream readable / Sly 1 name).
extern "C" int IsBhvWalkToDone(BHV193 *behavior)
{
    BHVPATH *child = (BHVPATH *)behavior->pbhvNext;
    int done = 0;
    if (IsBhvpathAtEnd(child) || child->state == 2)
        done = 1;
    return done;
}

// 0x144F40: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x10");

// 0x144F48: Stores the goal and builds the child's route to it: a straight line, or a path through the navigation
// graph (the guard's own path shape is rewritten either way).
// Also known as: SetGoal (upstream readable / Sly 1 name; SetGoal__12JOBACTORWALKG9VU_VECTOR).
extern "C" void SetBhvWalkToGoal(BHV193 *behavior, qword goal)
{
    behavior->goal = goal;
    if (!behavior->pbhvNext)
        return;
    STEPGUARD *guard = (STEPGUARD *)behavior->owner;
    SHAPE *path = guard->routeShape;
    REGENT *region = guard->physics ? guard->physics->pregent : 0;
    CID17 *pathZone = region ? region->pathzone : 0;
    qword start = GetLoWorldPos(guard);
    if (!pathZone || behavior->usesStraightPath) {
        path->numControlVertices = 2;
        *(qword *)&path->vertexPositions[0] = start;
        *(qword *)&path->vertexPositions[1] = behavior->goal;
        MeasureShape(path);
    } else {
        FindPathzonePathCurve(pathZone, start, behavior->goal, path);
    }
    SetBhvpathShape((BHVPATH *)behavior->pbhvNext, path, 0);
}

// 0x145010: Follow behaviour (BHVFOLLOW, BHV194) vtable slot 7 (OnPush): base, state -1, follow distances and speed
// factors (0.25/250/350/1.0/1.2).
// Also known as: Init (upstream readable / Sly 1 name).
void BHVFOLLOW::OnPush()
{
    InitBhv(this);
    state = -1;
    velocityLeadTime = 0.25f;
    distanceNear = 250.0f;
    distanceFar = 350.0f;
    speedNear = 1.0f;
    speedFar = 1.2f;
}

// 0x145080: Follow behaviour vtable slot 8 (Start): state 0.
void BHVFOLLOW::Start()
{
    StartBhv(this);
    SetBhvfollowState(this, 0);
}

// 0x1450B0: Follow behaviour vtable slot 9 (Close): state -1.
extern "C" void CloseBhvfollow(BHVFOLLOW *behavior)
{
    CloseBhv(behavior);
    SetBhvfollowState(behavior, -1);
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A4E30 __asm__("D_002A4E30") = 3.40282347e+38f; }
// Keeps its .data and VU0 helpers in a namespace of its own (see the file header).
namespace M_func_001450E0 {
extern const float D_002A4E30 __asm__("D_002A4E30");   /* defined below where the ELF has it (.rodata order) */
CLQ D_002BD300 __asm__("D_002BD300") __attribute__((aligned(16))) = {{{-200.0f, 0.8f, 0.0f, 0.0f}}};
CLQ D_002BD310 __asm__("D_002BD310") = {{{200.0f, 1.0f, 0.0f, 0.0f}}};
CLQ D_002BD320 __asm__("D_002BD320") = {{{-400.0f, 1.0f, 0.0f, 0.0f}}};

// VU0 helper: distance between two points.
extern "C" { static __inline__ float Distance(qword a, qword b)
{
    qword d, t;
    float distance;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z" : "+j"(d), "=j"(t));
    __asm__("vsqrt Q, %0x\n"
        "vwaitq\n"
        "vaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(distance) : "j"(d));
    return distance;
} }
// Evaluates a CLQ quadratic at distance.
extern "C" { static __inline__ float EvaluateQuadratic(float distance, CLQ *clq) { return clq->g0 + distance * (clq->g1 + distance * clq->g2); } }

// 0x1450E0: Picks the follow behaviour's threat: the guard nearest the leader among those engaged with the follower
// (its engagement record, STEPPLAYER +0x1088) or (distance re-weighted) with the leader, skipping KO'd/juggled/thrown
// ones; the current threat and striking guards get their own distance weights (inferred: who the partner should fight).
// Also known as: FindEnemy (upstream readable / Sly 1 name).
extern "C" STEPGUARD *PickBhvfollowTarget(BHVFOLLOW *behavior)
{
    STEPGUARD *best = 0;
    float bestDistance = D_002A4E30;
    qword pos;
    unsigned long ignoreMask;
    TARGETREC *record;
    int i;
    GetLoWorldPos(behavior->owner);
    pos = GetLoWorldPos(behavior->leader);
    ignoreMask = GetSgfMask(SGF_KO) | GetSgfMask(0x10) | GetSgfMask(0x16) | GetSgfMask(SGF_JUGGLED) |
          GetSgfMask(0x2A) | GetSgfMask(0x2B);
    record = (TARGETREC *)((STEPPLAYER *)behavior->owner)->unknown1088;
    for (i = 0; i < record->guardCount; i++) {
        STEPGUARD *guard = record->guards[i];
        float distance;
        if (ignoreMask & guard->grfsg)
            continue;
        distance = Distance(pos, GetLoWorldPos(guard));
        if (guard == (STEPGUARD *)behavior->threat)
            distance = EvaluateQuadratic(distance, &D_002BD300);
        if (StepguardHasSgf(guard, SGF_STRIKE))
            distance = EvaluateQuadratic(distance, &D_002BD320);
        if (distance < bestDistance) {
            best = guard;
            bestDistance = distance;
        }
    }
    record = FindTargetRecordById(behavior->leader);
    if (record) {
        for (i = 0; i < record->guardCount; i++) {
            STEPGUARD *guard = record->guards[i];
            float distance;
            if (ignoreMask & guard->grfsg)
                continue;
            distance = Distance(pos, GetLoWorldPos(guard));
            distance = EvaluateQuadratic(distance, &D_002BD310);
            if (guard == (STEPGUARD *)behavior->threat)
                distance = EvaluateQuadratic(distance, &D_002BD300);
            if (StepguardHasSgf(guard, SGF_STRIKE))
                distance = EvaluateQuadratic(distance, &D_002BD320);
            if (distance < bestDistance) {
                best = guard;
                bestDistance = distance;
            }
        }
    }
    return best;
}
} // namespace M_func_001450E0

namespace RO_ORDER { extern const float r_D_002A4E34 __asm__("D_002A4E34") = 3.40282347e+38f; }
// Keeps its .data and VU0 helpers in a namespace of its own (see the file header).
namespace M_UpdateBhvfollow {
// UpdateBhvfollow: camera/AI state update. Keys: `f = IsBhvfollowLeaderOnNewGround(p) != 0; if (f)` (stops loop.c hoisting constant 1; now `onNewGround`), inline ClampInPlace.
extern const float D_002A4E34 __asm__("D_002A4E34");   /* defined below where the ELF has it (.rodata order) */
struct LM D_002BD330 __asm__("D_002BD330") = {0.0f, 600.0f};
struct LM D_002BD338 __asm__("D_002BD338") = {0.0f, 1200.0f};
float D_002BD340 __asm__("D_002BD340") = 200.0f;

// VU0 helper: horizontal (xy) length of a vector.
extern "C" { static __inline__ float LenXY(qword d)
{
    float g;
    qword t;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(t) : "j"(d));
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(t));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(t));
    return g;
} }
// VU0 helper: length of a vector.
extern "C" { static __inline__ float Len(qword d)
{
    qword t;
    float g;
    __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "+j"(d), "=j"(t));
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
} }
// Clamps *pg into [lo, hi].
extern "C" { static __inline__ void ClampInPlace(float *pg, float lo, float hi) { if (*pg < lo) *pg = lo; else if (hi < *pg) *pg = hi; } }
// True if two points are farther apart (horizontally) than the .data limit.
extern "C" { static __inline__ int IsFarApart(qword a, qword b) { float lim = D_002BD340; return lim < LenXY(VuSub(a, b)); } }

// 0x1453F0: Follow behaviour vtable slot 10 (Update): follows the leader (+0x30); distance bands switch between
// standing, walking and running, re-pathing when far. State 0 follows (re-targets the follow point when the leader moved, animation speed
// from the leader distance), 1 waits for the leader to land on new ground, 2 fights the threat until the partner's
// interact behaviour goes idle or the threat leaves the band.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvfollow(BHVFOLLOW *behavior)
{
    STEPPLAYER *follower;
    qword followerPos, leaderPos;
    float leaderDistanceXY, threatDistance;
    SO *threat;
    PO *leader;
    UpdateBhv(behavior);
    follower = (STEPPLAYER *)behavior->owner;
    leader = behavior->leader;
    followerPos = GetLoWorldPos(follower);
    leaderPos = GetLoWorldPos(leader);
    leaderDistanceXY = LenXY(VuSub(followerPos, leaderPos));
    threat = PickBhvfollowTarget(behavior);
    if (threat) {
        qword threatPos = GetLoWorldPos(threat);
        float toFollower = Len(VuSub(threatPos, followerPos));
        float toLeader = Len(VuSub(threatPos, leaderPos));
        threatDistance = toFollower < toLeader ? toFollower : toLeader;
    } else {
        threatDistance = D_002A4E34;
    }
    while (!IsBhvBusyAbove(behavior)) {
        int state = behavior->state;
        int onNewGround;
        switch (state) {
        case 0:
            onNewGround = IsBhvfollowLeaderOnNewGround(behavior) != 0;
            if (onNewGround)
                SetBhvfollowState(behavior, 1);
            else if (IsWithinLimits__FP2LMf(&D_002BD330, threatDistance)) {
                behavior->threat = threat;
                SetBhvfollowState(behavior, 2);
            }
            break;
        case 1:
            if (((STEPPLAYER *)behavior->owner)->jtair.state == -1)
                SetBhvfollowState(behavior, 0);
            break;
        case 2:
            onNewGround = IsBhvfollowLeaderOnNewGround(behavior) != 0;
            if (onNewGround)
                SetBhvfollowState(behavior, 1);
            else if (!IsWithinLimits__FP2LMf(&D_002BD338, threatDistance))
                SetBhvfollowState(behavior, 0);
            else {
                switch (GetBasicCid(behavior->owner)) {
                case CID_MURRAY:
                    if (IsBhvInteractIdle((BHV195 *)behavior->pbhvNext))
                        SetBhvfollowState(behavior, 0);
                    break;
                case CID_BENTLEY:
                    if (IsBhvInteract2Idle((BHV196 *)behavior->pbhvNext))
                        SetBhvfollowState(behavior, 0);
                    break;
                case CID_JT:
                    if (IsBhvInteract3Idle((BHV197 *)behavior->pbhvNext))
                        SetBhvfollowState(behavior, 0);
                    break;
                }
            }
            break;
        }
        if (behavior->state == state)
            break;
    }
    if (behavior->state == 0) {
        REGENT *region = behavior->owner->physics ? behavior->owner->physics->pregent : 0;
        CID17 *pathZone = region ? region->pathzone : 0;
        if (!pathZone)
            ((BHVPATH *)behavior->pbhvNext)->ignoresBlockers = 1;
        if (IsFarApart(behavior->followPoint, leaderPos))
            UpdateBhvfollowPosition(behavior);
        if (follower->activeAnimation) {
            float t = (leaderDistanceXY - behavior->distanceNear) / (behavior->distanceFar - behavior->distanceNear);
            ClampInPlace(&t, 0.0f, 1.0f);
            SetAsegaSpeed__FP5ASEGAf(follower->activeAnimation, (1.0f - t) * behavior->speedNear + t * behavior->speedFar);
        }
    }
}
} // namespace M_UpdateBhvfollow

// Keeps its matrix view and VU0 helpers in a namespace of its own (see the file header).
namespace M_func_001457A8 {
// UpdateBhvfollowPosition MATCH (Claude agent for Lilly, T1647, 2026-10-25), from v6 (14).
// Branch 1: VU_FLOAT has a user copy ctor (so the temporary lives at 0x40) and its qmtc2 ctor writes a plain qword
// local first, then copies it into q (an asm output straight into the member goes through a second stack temp).
// Branch 2: the VU_FLOAT is loaded with an asm lw from "m"(f) (VU_FLOATM, as in SetBentleyHelperState), and AddScaled takes
// its operands in (a, s, b) order.
struct M3 { qword r[3]; };
struct M4 { qword r[4]; };

M3 *GetLoWorldMatrix(void *p) __asm__("GetLoWorldMatrix");
M3 BuildAxisAngleRotationMatrix(qword axis, float g) __asm__("BuildAxisAngleRotationMatrix");
void UpdateBhvfollowPosition(BHVFOLLOW *p) __asm__("UpdateBhvfollowPosition");

// Float broadcast into a VU0 register (copyable).
struct VU_FLOAT { qword q; VU_FLOAT(const VU_FLOAT &s) { q = s.q; } VU_FLOAT(float f) { qword t; __asm__("qmtc2.ni %1, %0" : "=j"(t) : "r"(f)); q = t; } };
// Wrap a qword as a VU_VECTOR.
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
// Load a float's bits into a GPR qword (lw).
struct VU_FLOATM { qword q; VU_FLOATM(const float &f) { __asm__("lw %0, %1" : "=r"(q) : "m"(f)); } };

// VU0 helper: a + b scaled by a VU float (multiply-add).
static inline qword AddScaled(VU_VECTOR a, VU_FLOATM s, VU_VECTOR b)
{
    qword r;
    __asm__(
        "vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %3, %2x"
        : "=j"(r)
        : "j"(a.q), "j"(s.q), "j"(b.q),
          "j"(s.q)); /* repeated "j"(s.q): allocation-only operand, keeps the lw value VF-class so it is spilled to 0x80 across the call */
    return r;
}

// VU0 helper: a + b scaled by a VU float (multiply-add).
static inline qword AddScaled(VU_VECTOR a, VU_FLOAT s, VU_VECTOR b)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %3, %2x" : "=j"(r) : "j"(a.q), "j"(s.q), "j"(b.q));
    return r;
}

// VU0 helper: 3x3 matrix times vector.
static inline qword MulM3(const M3 &m, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(r) : "j"(v), "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]));
    return r;
}

// 0x1457A8: Computes the follow position: the leader's position offset by its velocity (lead time
// +0x68) and by the follow offset (+0x40) in the leader's frame (its heading for a step, its orientation otherwise),
// then routes the walk-goal child there through the follower's path zone (or straight when it has none).
// Also known as: PlanFollow (upstream readable / Sly 1 name).
void UpdateBhvfollowPosition(BHVFOLLOW *behavior)
{
    union { M4 m4; struct { M3 m; qword pos; } s; } u;
    behavior->followPoint = GetLoWorldPos(behavior->leader);
    if (IsBasicDerivedFrom__FP5BASIC3CID(behavior->leader, CID_STEP)) {
        STEP *leader = (STEP *)behavior->leader;
        u.s.m = BuildAxisAngleRotationMatrix(g_normalZ, leader->targetHeading);
        u.s.pos = AddScaled(behavior->followPoint, VU_FLOAT(-behavior->velocityLeadTime), MulM3(u.s.m, leader->qwUnknown190));
    } else {
        PO *leader;
        u.s.m = *GetLoWorldMatrix(behavior->leader);
        leader = behavior->leader;
        u.s.pos = AddScaled(behavior->followPoint, VU_FLOATM(behavior->velocityLeadTime), leader->GetVelocity());
    }
    u.s.pos = VuAdd(u.s.pos, MulM3(u.s.m, behavior->followOffset));
    {
        STEPGUARD *guard = (STEPGUARD *)behavior->owner;
        SHAPE *path = guard->routeShape;
        REGENT *region = guard->physics ? guard->physics->pregent : 0;
        CID17 *pathZone = region ? region->pathzone : 0;
        qword start = GetLoWorldPos(guard);
        if (!pathZone) {
            path->numControlVertices = 2;
            *(qword *)&path->vertexPositions[0] = start;
            *(qword *)&path->vertexPositions[1] = u.s.pos;
            MeasureShape(path);
        } else if (GetBhvfollowLeaderGround(behavior) != pathZone) {
            FindPathzonePathCurve(pathZone, start, start, path);   /* leader in another zone: stay */
        } else if (!FindPathzonePathCurve(pathZone, start, u.s.pos, path)) {
            FindPathzonePathCurve(pathZone, start, start, path);
        }
        SetBhvpathShape((BHVPATH *)behavior->pbhvNext, path, 0);
    }
}
} // namespace M_func_001457A8

// 0x1459C0: True if the leader has landed on a different path zone than the follower's (not zone oid
// 0x2CA); stores the snapped landing point (+0x80) for state 1.
// Also known as: FNeedRecover (upstream readable / Sly 1 name).
extern "C" int IsBhvfollowLeaderOnNewGround(BHVFOLLOW *behavior)
{
    CID17 *ground = GetBhvfollowLeaderGround(behavior);
    REGENT *region;
    CID17 *pathZone;
    qword pos;
    if (!ground)
        return 0;
    region = behavior->owner->physics ? behavior->owner->physics->pregent : 0;
    pathZone = region ? region->pathzone : 0;
    if (ground == pathZone)
        return 0;
    if (((LO *)ground)->oid == 0x2CA)   /* CID17 has no header: read as the LO it derives from */
        return 0;
    if (IsBasicDerivedFrom__FP5BASIC3CID(behavior->leader, CID_STEPPLAYER) && ((STEPPLAYER *)behavior->leader)->jtair.state != -1)
        return 0;   /* still in the air */
    pos = GetLoWorldPos(behavior->leader);
    if (!IsPosFreeInPathzone(ground, pos, 0))
        return 0;
    if (!SnapToGround(&pos, 100.0f, -150.0f, 0, 0))
        return 0;
    behavior->landingPoint = pos;
    return 1;
}

// 0x145AA8: Returns the object the leader is standing on, if any.
/* The path zone of the followed object: a CID 0x30 object's region (SO::physics->pregent->pathzone), else the zone
   at its position. */
extern "C" CID17 *GetBhvfollowLeaderGround(BHVFOLLOW *behavior)
{
    if (IsBasicDerivedFrom__FP5BASIC3CID(behavior->leader, CID_SO)) {
        SOPHYS *physics = behavior->leader->physics;
        REGENT *region = physics ? physics->pregent : 0;
        return region ? region->pathzone : 0;
    }
    return FindPathzoneAtPos(GetLoWorldPos(behavior->leader), 1);
}

// Matching helper: pushes the interact behaviour of class `cid` (BHV195/196/197, all with the target at +0x38) on
// the owner, hands it the threat (+0x90) as its target and starts it (vtable slot 8).
static __inline__ void Activate(BHVFOLLOW *behavior, int cid)
{
    BHV195 *child = (BHV195 *)PushStepBehavior(behavior->owner, cid);
    child->target = (TARGET *)behavior->threat;
    child->Start();
}
// 0x145B18: Follow behaviour state change; state 0 pushes a walk-goal child toward the follow position
// (mirroring the leader's button 8), 1 moves the follower to the leader's landing point, 2 pushes the partner's
// interact behaviour (by class) against the threat.
// Also known as: SetState (upstream readable / Sly 1 name; SetState__12JOBCOMPANIONi).
extern "C" void SetBhvfollowState(BHVFOLLOW *behavior, int state)
{
    if (state == behavior->state) return;
    PopBhv(&behavior->pbhvNext);
    behavior->state = state;
    behavior->stateTime = g_clock.time;
    switch (state) {
    case 0: {
        BHVWALKGOAL *child = (BHVWALKGOAL *)PushStepBehavior(behavior->owner, CID_BHV168);
        if (IsBasicDerivedFrom__FP5BASIC3CID(behavior->leader, CID_PO)) {
            child->mirrorPoctl = behavior->leader->controller;   /* copy the leader's button 8 after 0.1..0.3 s */
            child->mirrorButtonMask = 8;
            child->mirrorDelay = RandomFloatInRange__Fff(0.1f, 0.3f);
        }
        UpdateBhvfollowPosition(behavior);
        child->Start();
        break;
    }
    case 1:
        ReturnJtToPosition(behavior->owner, behavior->landingPoint);
        break;
    case 2:
        switch (GetBasicCid(behavior->owner)) {
        case CID_MURRAY: Activate(behavior, CID_BHVINTERACT); break;
        case CID_BENTLEY: Activate(behavior, CID_BHVINTERACT2); break;
        case CID_JT: Activate(behavior, CID_BHVINTERACT3); break;
        }
        break;
    }
}

// 0x145CD0: Interact behaviour (BHV195) vtable slot 7 (OnPush): base, state -1.
extern "C" void InitBhvInteract(BHV195 *behavior)
{
    InitBhv(behavior);
    behavior->state = -1;
}

// 0x145D00: Interact behaviour vtable slot 8 (Start): state 0, starts interact mode 4 with the target
// (SetPlayerInteractMode, slot 95) and sets the owner's +0x10E0 bit 1.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhvInteract(BHV195 *behavior)
{
    STEPPLAYER *owner;
    StartBhv(behavior);
    SetBhvInteractState(behavior, 0);
    owner = (STEPPLAYER *)behavior->owner;
    owner->SetPlayerInteractMode(4, behavior->target, 4, behavior->target);
    ((STEPPLAYER *)behavior->owner)->interactBlockFlags |= 1;
}

// 0x145D68: Interact behaviour vtable slot 9 (Close): clears the owner's +0x10E0 bit 1.
void BHV195::Close()
{
    CloseBhv(this);
    ((STEPPLAYER *)owner)->interactBlockFlags &= ~1;
}

// 0x145DA0: True if the interact behaviour is idle (state -1).
extern "C" int IsBhvInteractIdle(BHV195 *behavior)
{
    return behavior->state == -1;
}

// Keeps its .data and VU0 code in a namespace of its own (see the file header).
namespace M_func_00145DB0 {
float D_002BD344 __asm__("D_002BD344") = 300.0f;
extern "C" { struct { float near; float far; } D_002BD348 = {250.0f, 350.0f}; }
qword *GetLoWorldMatrix(void *) __asm__("GetLoWorldMatrix");

// 0x145DB0: Interact behaviour vtable slot 10 (Update): approaches the target (+0x38, a guard) and interacts; aborts
// when Murray is busy (hurt state +0xDF4) or the target is gone or knocked out. State 0 walks up and interacts
// (state 2) once close and facing it, 1 runs back into the distance band, 2 interacts until the attack ends.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvInteract(BHV195 *behavior)
{
    QWROW toTarget;
    qword targetPos, ownerPos, delta, length, one, normalized;
    float distance;
    UpdateBhv(behavior);
    if (behavior->state == -1)
        return;
    if (((MURRAY *)behavior->owner)->jthurt.state != -1 || !IsLoInWorld(behavior->target) || ((STEPGUARD *)behavior->target)->isKnockedOut) {
        SetBhvInteractState(behavior, -1);
        return;
    }
    targetPos = GetLoWorldPos(behavior->target);
    ownerPos = GetLoWorldPos(behavior->owner);
    { qword *fallback = &D_002A2DA0;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(delta) : "j"(targetPos), "j"(ownerPos));
    toTarget.q = delta;
    toTarget.f[2] = 0.0f;   /* horizontal direction */
    {
        register qword l1 __asm__("vf1");
        register qword r2 __asm__("vf2");
        float gl;
        __asm__("vmul.xyz %0, %1, %1" : "=j"(l1) : "j"(toTarget.q));
        __asm__("vaddw.x %0, $vf0, $vf0w\n\tvadday.x ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(r2), "+j"(l1));
        __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(r2) : "j"(l1));
        length = r2;
        __asm__("qmfc2.ni %0, %1" : "=r"(gl) : "j"(length));
        if (gl < 1e-4f) {
            toTarget.q = *fallback;
        } else {
            __asm__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(normalized) : "j"(toTarget.q), "j"(length));
            toTarget.q = normalized;
        }
    }
    }
    __asm__("mtc1 %1, %0" : "=f"(distance) : "r"(length)); /* gcc emits the qmfc2 and schedules it */
    for (;;) {
        int state = behavior->state;
        switch (state) {
        case 0:
            if (distance < D_002BD344) {
                qword *forward = GetLoWorldMatrix(behavior->owner);   /* first row of the owner's orientation */
                register qword t __asm__("vf2");
                qword one2;
                float dot;
                t = *forward;
                __asm__("vmul.xyz %0, %0, %1" : "+j"(t) : "j"(toTarget.q));
                __asm__("vaddw.x %0, $vf0, $vf0w\n\tvadday.x ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(one2), "+j"(t));
                __asm__("qmfc2.ni %0, %1" : "=r"(dot) : "j"(t));
                if (0.8f < dot)
                    SetBhvInteractState(behavior, 2);
            } else if (D_002BD348.far < distance) {
                SetBhvInteractState(behavior, 1);
            }
            break;
        case 1:
            if (distance < D_002BD348.near)
                SetBhvInteractState(behavior, 0);
            break;
        case 2:
            if (((MURRAY *)behavior->owner)->attackSubstate == -1 && 0.1f < g_clock.time - behavior->stateTime)
                SetBhvInteractState(behavior, -1);
            break;
        }
        if (behavior->state == state)
            break;
    }
}
} // namespace M_func_00145DB0

// 0x146068: Interact behaviour vtable slot 14 (steering): drives toward the target (slowly in state 0).
// Also known as: Drive (upstream readable / Sly 1 name; Drive__15JOBMURRAYATTACKP6PADCTL).
extern "C" void SteerBhvInteract(BHV195 *behavior, POCTL *poctl)
{
    ResetBhvSteering(behavior, poctl);
    if (behavior->state < 3) if (behavior->state >= 0) {   // (two ifs, upper bound first: `&&` reorders the tests)
        qword targetPos, ownerPos, toTarget;
        float speed;
        targetPos = GetLoWorldPos(behavior->target);
        ownerPos = GetLoWorldPos(behavior->owner);
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(toTarget) : "j"(targetPos), "j"(ownerPos));
        speed = 1.0f;
        if (behavior->state == 0)
            speed = 0.1f;
        poctl->leftStickDeflection = speed;
        poctl->leftStickAngle = atan2f(QwordY(toTarget), QwordX(toTarget));
        if (behavior->state == 2 && g_clock.time - behavior->stateTime < 0.1f) {
            poctl->buttonsHeld |= 0x80; /* press the attack button for the first 0.1 s of state 2 */
            UpdatePoctlPressed(poctl);
        }
    }
}

// 0x146158: Interact behaviour state change; state 2 is busy.
/* Sets the behaviour's state (stamping the time); state 2 is the busy one. */
// Also known as: SetState (upstream readable / Sly 1 name; SetState__15JOBMURRAYATTACKi).
extern "C" void SetBhvInteractState(BHV195 *behavior, int state)
{
    int previous;
    previous = behavior->state;
    if (state == previous)
        return;
    if (previous == 2) {
        ClearBhvBusy(behavior);
    }
    behavior->state = state;
    behavior->stateTime = g_clock.time;
    if (state == 2) {
        SetBhvBusy(behavior);
    }
}

// 0x1461C8: Second interact behaviour (BHV196, the same shape as BHV195) vtable slot 7 (OnPush).
extern "C" void InitBhvInteract2(BHV196 *behavior)
{
    InitBhv(behavior);
    behavior->state = -1;
}

// 0x1461F8: Second interact behaviour vtable slot 8 (Start).
extern "C" void StartBhvInteract2(BHV196 *behavior)
{
    STEPPLAYER *owner;
    StartBhv(behavior);
    SetBhvInteract2State(behavior, 0);
    owner = (STEPPLAYER *)behavior->owner;
    owner->SetPlayerInteractMode(4, behavior->target, 4, behavior->target);
    ((STEPPLAYER *)behavior->owner)->interactBlockFlags |= 1;
}

// 0x146260: Second interact behaviour vtable slot 9 (Close).
void BHV196::Close()
{
    CloseBhv(this);
    ((STEPPLAYER *)owner)->interactBlockFlags &= ~1;
}

// 0x146298: True if the second interact behaviour is idle (state -1).
extern "C" int IsBhvInteract2Idle(BHV196 *behavior)
{
    return behavior->state == -1;
}

// Keeps its .data and VU0 code in a namespace of its own (see the file header).
namespace M_func_001462A8 {
float D_002BD350 __asm__("D_002BD350") = 350.0f;
extern "C" { struct { float near; float far; } D_002BD358 = {250.0f, 350.0f}; }
qword *GetLoWorldMatrix(void *) __asm__("GetLoWorldMatrix");

// 0x1462A8: Second interact behaviour vtable slot 10 (Update): the BHV195 approach with its own conditions (Bentley:
// the attack is his crosshair, BTBLENDS state +0x1714).
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvInteract2(BHV196 *behavior)
{
    QWROW toTarget;
    qword targetPos, ownerPos, delta, length, one, normalized;
    float distance;
    UpdateBhv(behavior);
    if (behavior->state == -1)
        return;
    if (((BENTLEY *)behavior->owner)->jthurt.state != -1 || !IsLoInWorld(behavior->target) || ((STEPGUARD *)behavior->target)->isKnockedOut) {
        SetBhvInteract2State(behavior, -1);
        return;
    }
    targetPos = GetLoWorldPos(behavior->target);
    ownerPos = GetLoWorldPos(behavior->owner);
    { qword *fallback = &D_002A2DA0;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(delta) : "j"(targetPos), "j"(ownerPos));
    toTarget.q = delta;
    toTarget.f[2] = 0.0f;   /* horizontal direction */
    {
        register qword l1 __asm__("vf1");
        register qword r2 __asm__("vf2");
        float gl;
        __asm__("vmul.xyz %0, %1, %1" : "=j"(l1) : "j"(toTarget.q));
        __asm__("vaddw.x %0, $vf0, $vf0w\n\tvadday.x ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(r2), "+j"(l1));
        __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(r2) : "j"(l1));
        length = r2;
        __asm__("qmfc2.ni %0, %1" : "=r"(gl) : "j"(length));
        if (gl < 1e-4f) {
            toTarget.q = *fallback;
        } else {
            __asm__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(normalized) : "j"(toTarget.q), "j"(length));
            toTarget.q = normalized;
        }
    }
    }
    __asm__("mtc1 %1, %0" : "=f"(distance) : "r"(length)); /* gcc emits the qmfc2 and schedules it */
    for (;;) {
        int state = behavior->state;
        switch (state) {
        case 0:
            if (distance < D_002BD350) {
                qword *forward = GetLoWorldMatrix(behavior->owner);   /* first row of the owner's orientation */
                register qword t __asm__("vf2");
                qword one2;
                float dot;
                t = *forward;
                __asm__("vmul.xyz %0, %0, %1" : "+j"(t) : "j"(toTarget.q));
                __asm__("vaddw.x %0, $vf0, $vf0w\n\tvadday.x ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(one2), "+j"(t));
                __asm__("qmfc2.ni %0, %1" : "=r"(dot) : "j"(t));
                if (0.8f < dot)
                    SetBhvInteract2State(behavior, 2);
            } else if (D_002BD358.far < distance) {
                SetBhvInteract2State(behavior, 1);
            }
            break;
        case 1:
            if (distance < D_002BD358.near)
                SetBhvInteract2State(behavior, 0);
            break;
        case 2:
            if (((BENTLEY *)behavior->owner)->btblends.state == -1 && 0.1f < g_clock.time - behavior->stateTime)
                SetBhvInteract2State(behavior, -1);
            break;
        }
        if (behavior->state == state)
            break;
    }
}
} // namespace M_func_001462A8

// 0x146560: Second interact behaviour vtable slot 14 (steering).
// Also known as: Drive (upstream readable / Sly 1 name; Drive__16JOBBENTLEYATTACKP6PADCTL).
extern "C" void SteerBhvInteract2(BHV196 *behavior, POCTL *poctl)
{
    ResetBhvSteering(behavior, poctl);
    if (behavior->state < 3) if (behavior->state >= 0) {   // (two ifs, upper bound first: `&&` reorders the tests)
        qword targetPos, ownerPos, toTarget;
        float speed;
        targetPos = GetLoWorldPos(behavior->target);
        ownerPos = GetLoWorldPos(behavior->owner);
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(toTarget) : "j"(targetPos), "j"(ownerPos));
        speed = 1.0f;
        if (behavior->state == 0)
            speed = 0.1f;
        poctl->leftStickDeflection = speed;
        poctl->leftStickAngle = atan2f(QwordY(toTarget), QwordX(toTarget));
        if (behavior->state == 2 && g_clock.time - behavior->stateTime < 0.1f) {
            poctl->buttonsHeld |= 0x80; /* press the attack button for the first 0.1 s of state 2 */
            UpdatePoctlPressed(poctl);
        }
    }
}

// 0x146650: Second interact behaviour state change.
/* Sets the behaviour's state (stamping the time); state 2 is the busy one. */
// Also known as: SetState (upstream readable / Sly 1 name; SetState__16JOBBENTLEYATTACKi).
extern "C" void SetBhvInteract2State(BHV196 *behavior, int state)
{
    int previous;
    previous = behavior->state;
    if (state == previous)
        return;
    if (previous == 2) {
        ClearBhvBusy(behavior);
    }
    behavior->state = state;
    behavior->stateTime = g_clock.time;
    if (state == 2) {
        SetBhvBusy(behavior);
    }
}

// 0x1466C0: Third interact behaviour (BHV197) vtable slot 7 (OnPush).
extern "C" void InitBhvInteract3(BHV197 *behavior)
{
    InitBhv(behavior);
    behavior->state = -1;
}

// 0x1466F0: Third interact behaviour vtable slot 8 (Start).
extern "C" void StartBhvInteract3(BHV197 *behavior)
{
    STEPPLAYER *owner;
    StartBhv(behavior);
    SetBhvInteract3State(behavior, 0);
    owner = (STEPPLAYER *)behavior->owner;
    owner->SetPlayerInteractMode(4, behavior->target, 4, behavior->target);
    ((STEPPLAYER *)behavior->owner)->interactBlockFlags |= 1;
}

// 0x146758: Third interact behaviour vtable slot 9 (Close).
void BHV197::Close()
{
    CloseBhv(this);
    ((STEPPLAYER *)owner)->interactBlockFlags &= ~1;
}

// 0x146790: True if the third interact behaviour is idle (state -1).
extern "C" int IsBhvInteract3Idle(BHV197 *behavior)
{
    return behavior->state == -1;
}

// Keeps its .data, matrix-row view and VU0 helpers in a namespace of its own (see the file header).
namespace M_func_001467A0 {
struct M3 { qword r[3]; };
float D_002BD360 __asm__("D_002BD360") = 450.0f;                                  /* attack distance */
float D_002BD368 __asm__("D_002BD368") __attribute__((aligned(8))) = 400.0f;    /* band: near */
float D_002BD36C __asm__("D_002BD36C") = 550.0f;                                  /* band: far */
float D_002BD370 __asm__("D_002BD370") = 0.2f;                                    /* air time before the air attack */
struct M3 *GetLoWorldMatrix(void *p) __asm__("GetLoWorldMatrix");

// VU0 helper: dot product.
extern "C" { static __inline__ float Dot(qword a, qword b)
{
    qword r, t;
    float g;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(r), "=j"(t) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(r));
    return g;
} }

// VU0 helper: normalizes *pv in place (or uses the default for a zero vector) and returns the length.
extern "C" { static __inline__ float SafeNormalize(qword *pv, qword *pvDefault)
{
    register qword v __asm__("vf3") = *pv;
    qword t, l;
    float g;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(t), "=j"(l) : "j"(v));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(t), "0"(l));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
    if (g < 0.0001f)
        *pv = *pvDefault;
    else
    {
        qword r;
        __asm__("vdiv Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, Q" : "=&j"(r) : "j"(l), "j"(v));
        *pv = r;
    }
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
    return g;
} }

// 0x1467A0: Third interact behaviour vtable slot 10 (Update): the approach with two extra states: when close and facing
// the target, 30% of the time it jumps (3) and attacks in the air after 0.2 s (4) instead of the ground attack (2).
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvInteract3(BHV197 *behavior)
{
    union { qword q; float f[4]; } toTarget;
    float distance;
    int state;
    UpdateBhv(behavior);
    if (behavior->state == -1)
        return;
    if (((JT *)behavior->owner)->jthurt.state != -1 || !IsLoInWorld(behavior->target) || ((STEPGUARD *)behavior->target)->isKnockedOut) {
        SetBhvInteract3State(behavior, -1);
        return;
    }
    {
        qword targetPos = GetLoWorldPos(behavior->target);
        qword ownerPos = GetLoWorldPos(behavior->owner);
        qword dq;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(dq) : "j"(targetPos), "j"(ownerPos));
        toTarget.q = dq;
        toTarget.f[2] = 0.0f;
        distance = SafeNormalize(&toTarget.q, &D_002A2DA0);
    }
    do {
        state = behavior->state;
        switch (state) {
        case 0:
            if (distance < D_002BD360) {
                struct M3 *orientation = GetLoWorldMatrix(behavior->owner);
                if (0.8f < Dot(orientation->r[0], toTarget.q)) {   /* facing the target */
                    if (RandomFloatInRange__Fff(0.0f, 1.0f) < 0.3f)
                        SetBhvInteract3State(behavior, 3);
                    else
                        SetBhvInteract3State(behavior, 2);
                }
            } else if (D_002BD36C < distance) {
                SetBhvInteract3State(behavior, 1);
            }
            break;
        case 1:
            if (distance < D_002BD368)
                SetBhvInteract3State(behavior, 0);
            break;
        case 2:
            if (((JT *)behavior->owner)->animState == -1 && 0.1f < g_clock.time - behavior->stateTime)
                SetBhvInteract3State(behavior, -1);
            break;
        case 3: {
            float t = g_clock.time;
            if (0.1f < t - behavior->stateTime) {
                JT *owner = (JT *)behavior->owner;
                if (owner->jtair.state != -1) {
                    if (owner->jtair.state == 1 && D_002BD370 < t - owner->jtair.stateStartTime)
                        SetBhvInteract3State(behavior, 4);
                } else
                    SetBhvInteract3State(behavior, -1);
            }
            break;
        }
        case 4:
            if (0.1f < g_clock.time - behavior->stateTime && ((JT *)behavior->owner)->state != 13)
                SetBhvInteract3State(behavior, -1);
            break;
        }
    } while (behavior->state != state);
}
} // namespace M_func_001467A0

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00146B18 {
// MATCH (Claude agent for Lilly, T1657 2026-10-25). Case 3 builds CLQ clq = {-0.25, 0.0015625} and LM lm = {0.1, 1.0} on the
// stack: poctl->leftStickDeflection = ClampToLimits(&lm, EvaluateQuadratic(|d.xy|, &clq)), poctl->leftStickAngle = atan2(d.y, d.x) with d = pos(target) - pos(owner).
// What finished it (the old 1-off was the por/li.s order of the clq init): see notes/140000.md (T1657).
struct CLQ { float g0, g1, g2, gPad; } __attribute__((aligned(16)));   /* basic.h CLQ with the 16-byte stack alignment the frame needs */
// VU0 helper: square root through the POCTL register.
extern "C" { static __inline__ float SqrtQ(qword t)
{
    float g;
    __asm__ __volatile__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(t));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(t));
    return g;
} }
// Evaluates a CLQ quadratic at g.
extern "C" { static __inline__ float EvaluateQuadratic(float g, struct CLQ *pclq) { return pclq->g0 + g * (pclq->g1 + g * pclq->g2); } }
// Clamps g to an LM range.
extern "C" { static __inline__ float ClampToLimits(struct LM *plm, float g)
{
    float gMin = plm->minValue, gMax = plm->maxValue;
    if (g < gMin)
        return gMin;
    if (gMax < g)
        return gMax;
    return g;
} }
/* b: allocation-only "j" input (Claude agent). It keeps b live past the clamp, so b gets its own early qmtc2 instead of
   staying in v0 until the vsub (then the clq clear can use v0 like the target). */
extern "C" { static __inline__ float AngleXYKeepAlive(qword d, qword b)
{
    register qword r __asm__("v1"); /* register pin (Claude agent) */
    float x;
    __asm__("qmfc2.ni %0, %1" : "=r"(r) : "j"(d), "j"(b));
    x = QwordX(r);
    {
        float y;
        __asm__("prot3w %0,%1" : "=r"(y) : "0"(r));
        return atan2f(y, x);
    }
} }
/* y via an int + mtc1 (as in CalculateVectorPanTilt): no GPR output reload here, which keeps reload's round-robin on the target's v0 for case 3 */
extern "C" { static __inline__ float AngleXY(qword d)
{
    int y;
    float fy;
    __asm__("prot3w %0,%1" : "=r"(y) : "r"(d));
    __asm__("mtc1 %1,%0" : "=f"(fy) : "r"(y));
    return atan2f(fy, QwordX(d));
} }
// 0x146B18: Third interact behaviour vtable slot 14 (steering): speed limited from the horizontal distance, heading
// toward the target; states 2 and 4 press the attack button (0x80) and state 3 jumps (0x40) for their first 0.1 s.
// Also known as: Drive (upstream readable / Sly 1 name; Drive__12JOBSLYATTACKP6PADCTL).
extern "C" void SteerBhvInteract3(BHV197 *behavior, POCTL *poctl)
{
    ResetBhvSteering(behavior, poctl);
    switch (behavior->state) {
    case 1: {
        qword d = VuSub(GetLoWorldPos(behavior->target), GetLoWorldPos(behavior->owner));
        poctl->leftStickDeflection = 1.0f;
        poctl->leftStickAngle = AngleXY(d);
        break;
    }
    case 2: {
        qword d = VuSub(GetLoWorldPos(behavior->target), GetLoWorldPos(behavior->owner));
        poctl->leftStickDeflection = 1.0f;
        poctl->leftStickAngle = AngleXY(d);
        if (g_clock.time - behavior->stateTime < 0.1f) {
            poctl->buttonsHeld |= 0x80;
            UpdatePoctlPressed(poctl);
        }
        break;
    }
    case 3: {
        qword a = GetLoWorldPos(behavior->target);
        qword b = GetLoWorldPos(behavior->owner);
        struct CLQ clq = {-0.25f, 0.00156249990686774f};
        qword d;
        float s;
        {
        qword t;
        /* "m"(clq.g0): allocation-only input (Claude agent); the vsub waits for the g0 store, like the target */
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b), "m"(clq.g0));
        __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(t) : "j"(d));
        {
        struct LM lm = {0.1f, 1.0f};
        s = ClampToLimits(&lm, EvaluateQuadratic(SqrtQ(t), &clq));
        }
        }
        poctl->leftStickDeflection = s;
        poctl->leftStickAngle = AngleXYKeepAlive(d, b);
        if (g_clock.time - behavior->stateTime < 0.1f && ((JT *)behavior->owner)->jtair.state == -1) {
            poctl->buttonsHeld |= 0x40; /* jump (state 3: the jump attack) */
            UpdatePoctlPressed(poctl);
        }
        break;
    }
    case 4: {
        qword d = VuSub(GetLoWorldPos(behavior->target), GetLoWorldPos(behavior->owner));
        poctl->leftStickDeflection = 1.0f;
        poctl->leftStickAngle = AngleXY(d);
        if (g_clock.time - behavior->stateTime < 0.1f) {
            poctl->buttonsHeld |= 0x80;
            UpdatePoctlPressed(poctl);
        }
        break;
    }
    }
}
} // namespace M_func_00146B18

// 0x146E18: Third interact behaviour state change (busy in states 2..4).
// Also known as: SetState (upstream readable / Sly 1 name; SetState__12JOBSLYATTACKi).
extern "C" void SetBhvInteract3State(BHV197 *behavior, int state)
{
    if (state == behavior->state) return;
    switch (behavior->state) {
    case 2: case 3: case 4:
        ClearBhvBusy(behavior);
    }
    behavior->state = state;
    behavior->stateTime = g_clock.time;
    switch (state) {
    case 2: case 3: case 4:
        SetBhvBusy(behavior);
    }
}

// 0x146E98: Murray behaviour (BHV198) vtable slot 8 (Start): sets MURRAY +0x1AC0, Murray state 0xC.
// Also known as: Enter (upstream readable / Sly 1 name).
void BHV198::Start()
{
    StartBhv(this);
    ((MURRAY *)owner)->isBehaviorActive = 1;
    SetMurrayState((MURRAY *)owner, 0xC);
    SetMurrayPowerSubstate((MURRAY *)owner, 0);
}

// 0x146EE0: Murray behaviour vtable slot 9 (Close): clears +0x1AC0, Murray state 0.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvMurray(BHV198 *behavior)
{
    CloseBhv(behavior);
    ((MURRAY *)behavior->owner)->isBehaviorActive = 0;
    SetMurrayState(((MURRAY *)behavior->owner), 0);
}

// Keeps its .data in a namespace of its own (see the file header).
namespace M_func_00146F18 {
extern void *D_002E1E40 __asm__("D_002E1E40");
float D_002BD374 __asm__("D_002BD374") = 0.5f, D_002BD378 __asm__("D_002BD378") = 15.0f;   /* heading window (rad), turn rate (rad/s) */
void SetStepTargetHeadingFromVector(MURRAY *step, qword direction) __asm__("SetStepTargetHeadingFromVector");
float NormalizeAngle__Ff(float g) __asm__("NormalizeAngle__Ff");
float GetStepHeading(MURRAY *step) __asm__("GetStepHeading");
float ClampAbs__Fff(float g, float dg) __asm__("ClampAbs__Fff");
void SetStepTargetHeading(MURRAY *step, float heading) __asm__("SetStepTargetHeading");
// VU0 helper: x component of a vector.
// VU0 helper: y component of a vector.
// 0x146F18: Murray behaviour vtable slot 10 (Update): Murray's sub-state machine (+0x1840): in 0 he faces D_002E1E40
// (the current player, inferred) and remembers that heading; in 1 he keeps turning toward it (at most 15 rad/s) while
// it stays within 0.5 rad of the remembered heading.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvMurray(BHV198 *behavior)
{
    UpdateBhv(behavior);
    switch (((MURRAY *)behavior->owner)->subState) {
    case 0:
        if (D_002E1E40) {
            qword playerPos, murrayPos, toPlayer;
            playerPos = GetLoWorldPos(D_002E1E40);
            murrayPos = GetLoWorldPos(((MURRAY *)behavior->owner));
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(toPlayer) : "j"(playerPos), "j"(murrayPos));
            SetStepTargetHeadingFromVector(((MURRAY *)behavior->owner), toPlayer);
        }
        behavior->startHeading = ((MURRAY *)behavior->owner)->targetHeading;
        break;
    case 1:
        if (D_002E1E40) {
            qword playerPos, murrayPos, toPlayer;
            float headingToPlayer, heading;
            playerPos = GetLoWorldPos(D_002E1E40);
            murrayPos = GetLoWorldPos(((MURRAY *)behavior->owner));
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(toPlayer) : "j"(playerPos), "j"(murrayPos));
            headingToPlayer = atan2f(QwordY(toPlayer), QwordX(toPlayer));
            if (__builtin_fabsf(NormalizeAngle__Ff(headingToPlayer - behavior->startHeading)) < D_002BD374) {
                heading = GetStepHeading(((MURRAY *)behavior->owner));
                {
                    float turn = NormalizeAngle__Ff(headingToPlayer - heading);
                    float dt = g_clock.deltaTime;
                    float turnRate = D_002BD378;
                    SetStepTargetHeading(((MURRAY *)behavior->owner), heading + ClampAbs__Fff(turn, turnRate * dt));
                }
            }
        }
        break;
    }
}
} // namespace M_func_00146F18

