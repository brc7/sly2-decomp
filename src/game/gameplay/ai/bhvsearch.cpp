// bhvsearch.cpp -- the guard search behaviour (BHV171, guard flag 0x1F). After losing its target the guard reacts,
// goes to the last known position, faces it, looks around, then visits up to four search points picked from nearby
// hiding places (several global object lists and three object classes) and gives up after a timeout. Entry points:
// the BHV slots (InitBhvSearch, Start, CloseBhvSearch, UpdateBhvSearch) and SetBhvSearchState. VU0 macro-mode asm.
// TU 0x171F58..0x173158 (16 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_bhvsearch
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_bhvsearch
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/clock.h"
#include "sly2/sw.h"
#include "sly2/classes.h"
// 0x173000 StaticInit_bhvsearch and GlobalConstructors_bhvsearch: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* Class ids used here (docs/systems/object-model.md section 1). */
enum {
    CID_CLASS19 = 0x13,
    CID_CLASS27 = 0x1B,
    CID_CLASS35 = 0x23,
    CID_BOOTY = 0x67,
    CID_BHVANIM = 0x9D,
    CID_BHVIDLE = 0x9E,
    CID_BHVGOTO = 0xBA,
};
/* Guard flags (STEPGUARD +0x2C0, bit n = flag n; each behaviour raises its own: docs/systems/ai-guards.md
   section 2). Only the identified flags are named. */
enum {
    SGF_SEARCH = 0x1F,
};

typedef union U { qword q; float f[4]; int i[4]; } U;

/* The search behaviour is classes.h's BHV171 (its guard: guard.h's STEPGUARD; the watched stimulus: NOISE, a counted
   object: BASIC slots 1/2). */

/* Candidate list built by ChooseBhvSearchPoints (TryAddSearchCandidate adds one). */
typedef struct ROW { U point; float angle; } ROW;
typedef struct QUERY { U point; float heading; unsigned count; ROW row[64]; U candidate, delta, scalar; } QUERY;
typedef char assert_QUERY_candidate[(unsigned int)&((QUERY *)0)->candidate == 0x820 ? 1 : -1];
/* An object of one of the global lists ChooseBhvSearchPoints searches (next links at several offsets). */
/* NODE: an object on one of the world lists ChooseBhvSearchPoints walks (opaque: one loop variable serves every list,
   each loop reads its own class's link and fields). */
typedef struct NODE NODE;
// The next object on a list whose class T links it through member dle (the list's own link: dleGrab, dleBreakable...).
#define NEXT_ON(T, dle, object) ((NODE *)((T *)(object))->dle.next)
typedef struct WORK{QUERY query;NODE*result256[256];NODE*result32[32];NODE*result128[128];}WORK;

struct GOAL { SHAPE *shape; LO *object; int unknown8[2]; qword pos; };   /* classes.h PATHLINKEND without its constructor */ typedef char assert_GOAL_pos[SLY2_OFFSETOF(GOAL, pos) == SLY2_OFFSETOF(PATHLINKEND, pos) && sizeof(GOAL) == sizeof(PATHLINKEND) ? 1 : -1];

extern const float D_002A5398;   /* defined below where the ELF has it (.rodata order) */
float D_002BE5A0 = 2.0f;   /* bhvsearch's .data starts here (address order) */
extern const float D_002A53D4;   /* defined below where the ELF has it (.rodata order) */
extern const LM D_002A53C0, D_002A53C8;   /* distance, +-pi/4; defined below where the ELF has them (.rodata order) */
extern qword D_002A2DA0,g_normalZ;
extern const float D_002A53D0;   /* defined below where the ELF has it (.rodata order) */
LM D_002BE5A8 = {-0.7f, -0.5f}, D_002BE5B0 = {300.0f, 400.0f};
extern "C" {
void InitBhv(BHV171 *behavior);
void StartBhv(BHV171 *behavior);
void CloseBhv(BHV171 *behavior);
void UpdateBhv(BHV171 *behavior);
int IsBhvBusyAbove(BHV171 *behavior);
void PopBhv(BHV **ppbhv);
void AddStepguardSgf(STEPGUARD *guard, int sgf);
void RemoveStepguardSgf(STEPGUARD *guard, int sgf);
int IsBhvanimDone(void *behavior);
int IsStepguardHandsOff(STEPGUARD *guard);
BHV *PushStepBehavior(STEP *pstep, int cid);
qword GetLoWorldPos(void *object);
void ClearPointerPair(GOAL *goal);
void SetBhvgotoGoal(void *behavior, GOAL *goal);
int HasBhvgotoArrived(void *behavior);
int IsBhvgotoPathFinished(void *behavior);
float atan2f(float y, float x);
void SetStepguardMoverRadTarget(GUARDMOVER *pmover, float rad);
void SetBhvSearchState(BHV171 *behavior, int state);
void SetBhvSearchLookTimers(BHV171 *behavior);
void SetBhvSearchWatched(BHV171 *behavior, NOISE *pwatch);
int HasBhvSearchReachedTarget(BHV171 *behavior);
qword GetBhvSearchLastKnownPosition(BHV171 *behavior);
int TryAddSearchCandidate(QUERY *pquery);
int CompareSearchCandidates(ROW *a, ROW *b);
void ChooseBhvSearchPoints(BHV171 *behavior);
void StepBhvSearchPoint(BHV171 *behavior);
void GetVectorSphere(qword v, float *pa, float *pb, float *pc);
float GetStepHeading(void *object);
void FindShapeClosestPoint(NODE*,qword,void*,qword*,qword*,float*,float*);
int FindSwObjectsByClass(void*,int,int,NODE*,int,NODE**);
void qsort(void*,__SIZE_TYPE__,__SIZE_TYPE__,int(*)(ROW*,ROW*));
qword SetVectorCylind(float,float,float);
}
float RandomFloatInRange(float gMin, float gMax);
int IsWithinLimits(LM *plm, float g) __asm__("IsWithinLimits__FP2LMf");   /* LMV: this file's view of LM */
float NormalizeAngle(float rad);
int IsBasicDerivedFrom(NODE *pbasic, int cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");

// VU0 helper: x component of a vector.
// VU0 helper: y component of a vector.

// 0x171F58: Search behaviour (BHV171) vtable slot 7 (OnPush): base, raises guard flag 0x1F, state -1, both look
// timers off (FLT_MAX).
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvSearch(BHV171 *behavior)
{
    InitBhv(behavior);
    AddStepguardSgf(((STEPGUARD *)behavior->owner), SGF_SEARCH);
    behavior->state = -1;
    behavior->unknown40 = behavior->lookEndTime = 3.40282346638528859812e+38f;
}

// 0x171FA0: Search behaviour vtable slot 8 (Start): base, then state 0 (react).
void BHV171::Start()
{
    BHV171 *behavior = (BHV171 *)this;
    StartBhv(behavior);
    SetBhvSearchState(behavior, 0);
}

// 0x171FD0: Search behaviour vtable slot 9 (Close): base, state -1, lowers flag 0x1F and drops the watched
// stimulus.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvSearch(BHV171 *behavior)
{
    CloseBhv(behavior);
    SetBhvSearchState(behavior, -1);
    RemoveStepguardSgf(((STEPGUARD *)behavior->owner), SGF_SEARCH);
    SetBhvSearchWatched(behavior, 0);
}

// 0x172018: Search behaviour vtable slot 10 (Update): state machine 0 react, 1 go to the last known position, 2
// face it and look, 3 visit the search points, 4 give up, 5 hold while the guard is hands-off.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvSearch(BHV171 *behavior) {
    int s;
    UpdateBhv(behavior);
    while (!IsBhvBusyAbove(behavior)) {
        s = behavior->state;
        switch (s) {
        case 0:
            if (IsBhvanimDone(behavior->pbhvNext)) SetBhvSearchState(behavior, 1);
            break;
        case 1:
            if (IsStepguardHandsOff(((STEPGUARD *)behavior->owner))) {
                SetBhvSearchState(behavior, 5);
            } else if (behavior->needsReplan) {
                if (HasBhvSearchReachedTarget(behavior)) {
                    SetBhvSearchState(behavior, 2);
                } else {
                    qword v = GetBhvSearchLastKnownPosition(behavior);
                    GOAL goal;
                    ClearPointerPair(&goal);
                    goal.pos = v;
                    SetBhvgotoGoal(behavior->pbhvNext, &goal);
                }
                behavior->needsReplan = 0;
            } else if (HasBhvSearchReachedTarget(behavior) || HasBhvgotoArrived(behavior->pbhvNext) || IsBhvgotoPathFinished(behavior->pbhvNext)) {
                SetBhvSearchState(behavior, 2);
            }
            break;
        case 2:
            if (IsStepguardHandsOff(((STEPGUARD *)behavior->owner))) {
                SetBhvSearchState(behavior, 5);
            } else if (behavior->needsReplan) {
                if (HasBhvSearchReachedTarget(behavior)) {
                    SetBhvSearchLookTimers(behavior);
                } else {
                    SetBhvSearchState(behavior, 1);
                }
                behavior->needsReplan = 0;
            } else if (behavior->lookEndTime < g_clock.time || IsBhvanimDone(behavior->pbhvNext)) {
                SetBhvSearchState(behavior, 4);
            } else if (g_clock.time > behavior->nextTime) {
                SetBhvSearchState(behavior, 3);
            } else {
                qword a = GetBhvSearchLastKnownPosition(behavior);
                qword b = GetLoWorldPos(((STEPGUARD *)behavior->owner));
                qword d;
                d = VuSub(a, b);
                SetStepguardMoverRadTarget(&((STEPGUARD *)behavior->owner)->mover, atan2f(QwordY(d), QwordX(d)));
            }
            break;
        case 3:
            if (IsStepguardHandsOff(((STEPGUARD *)behavior->owner))) {
                SetBhvSearchState(behavior, 5);
            } else if (behavior->needsReplan) {
                if (HasBhvSearchReachedTarget(behavior)) {
                    SetBhvSearchState(behavior, 2);
                } else {
                    SetBhvSearchState(behavior, 1);
                }
                behavior->needsReplan = 0;
            } else if (behavior->lookEndTime < g_clock.time) {
                SetBhvSearchState(behavior, 4);
            } else {
                StepBhvSearchPoint(behavior);
            }
            break;
        case 5:
            if (!IsStepguardHandsOff(((STEPGUARD *)behavior->owner))) SetBhvSearchState(behavior, 1);
            break;
        case 4:
            if (behavior->needsReplan) {
                if (HasBhvSearchReachedTarget(behavior)) {
                    SetBhvSearchState(behavior, 2);
                } else {
                    SetBhvSearchState(behavior, 1);
                }
                behavior->needsReplan = 0;
            } else if (IsBhvanimDone(behavior->pbhvNext)) {
                SetBhvSearchState(behavior, -1);
            }
            break;
        }
        if (behavior->state != s) continue;
        break;
    }
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A5398 __asm__("D_002A5398") = 3.40282347e+38f; }
// 0x172320: Enters a search state, pushing its child behaviour (an animation for react/look/give-up, a goto for
// travel, a look-at) and arming its timers.
// Also known as: SetInvestigateState (upstream readable / Sly 1 name; SetInvestigateState__14JOBINVESTIGATEi).
extern "C" void SetBhvSearchState(BHV171 *behavior, int n) {
    if (n == behavior->state) return;
    PopBhv(&behavior->pbhvNext);
    behavior->state = n;
    behavior->stateTime = g_clock.time;
    switch (n) {
    case 0:
        {
            BHVANIM *child = (BHVANIM *)PushStepBehavior(behavior->owner, CID_BHVANIM);
            child->sgf = 0xC;
            child->Start();
        }
        break;
    case 1:
        {
            BHVGOTO *child = (BHVGOTO *)PushStepBehavior(behavior->owner, CID_BHVGOTO);
            GOAL goal;
            qword v;
            child->allowPartialRoute = 1;
            v = GetBhvSearchLastKnownPosition(behavior);
            ClearPointerPair(&goal);
            goal.pos = v;
            SetBhvgotoGoal(child, &goal);
            child->Start();
            behavior->lookEndTime = D_002A5398;
        }
        break;
    case 2:
        {
            qword a = GetBhvSearchLastKnownPosition(behavior);
            qword b = GetLoWorldPos(((STEPGUARD *)behavior->owner));
            qword d;
            d = VuSub(a, b);
            SetStepguardMoverRadTarget(&((STEPGUARD *)behavior->owner)->mover, atan2f(QwordY(d), QwordX(d)));
            {
                BHV *child = PushStepBehavior(behavior->owner, CID_BHVIDLE);
                child->Start();
            }
            SetBhvSearchLookTimers(behavior);
        }
        break;
    case 3:
        {
            BHVGOTO *child = (BHVGOTO *)PushStepBehavior(behavior->owner, CID_BHVGOTO);
            child->allowPartialRoute = 1;
            ChooseBhvSearchPoints(behavior);
            StepBhvSearchPoint(behavior);
            child->Start();
        }
        break;
    case 4:
        {
            BHVANIM *child = (BHVANIM *)PushStepBehavior(behavior->owner, CID_BHVANIM);
            child->sgf = 0xE;
            child->Start();
        }
        SetBhvSearchWatched(behavior, 0);
        behavior->needsReplan = 0;
        break;
    case 5:
        {
            BHVIDLE *child = (BHVIDLE *)PushStepBehavior(behavior->owner, CID_BHVIDLE);
            child->sgf = 1;
            child->Start();
        }
        break;
    }
}

// 0x172550: True if the search behaviour is idle (state -1).
extern "C" int IsBhvSearchIdle(BHV171 *behavior)
{
    return ~behavior->state == 0;
}

// 0x172560: Arms the look timers: a random look duration from the guard's range (+0xE6C..+0xE70); the second
// (give-up) timer only for guards with +0xE78 == 1.
// Also known as: SetDeadlines (upstream readable / Sly 1 name).
extern "C" void SetBhvSearchLookTimers(BHV171 *behavior) {
    float t = g_clock.time;
    behavior->lookEndTime = t + RandomFloatInRange(((STEPGUARD *)behavior->owner)->lookTimes.minValue, ((STEPGUARD *)behavior->owner)->lookTimes.maxValue);
    if (((STEPGUARD *)behavior->owner)->searchMode == 1) behavior->nextTime = g_clock.time + D_002BE5A0;
    else behavior->nextTime = 3.40282346638528859812e+38f;
}

// 0x1725E0: Replaces the watched stimulus (a refcounted object at +0x44, via its add/drop reference slots) and
// flags a re-plan.
// Also known as: SetNoise (upstream readable / Sly 1 name; SetNoise__14JOBINVESTIGATEP5NOISE).
extern "C" void SetBhvSearchWatched(BHV171 *behavior, NOISE *watched) {
    if (watched == behavior->watched) return;
    if (behavior->watched) ((BASIC *)behavior->watched)->Release();
    behavior->watched = watched;
    if (watched) ((BASIC *)watched)->AddRef();
    behavior->needsReplan = 1;
}

// VU0 helper: distance between two points.
static inline float Dist(qword a, qword b) {
    qword d;
    float f;
    d = VuSub(a, b);
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    f = VuToFloat(d);
    return f;
}
// 0x172658: True if the guard cannot search any more or its last known position is nearer than the target object +
// 50.
// Also known as: FNear (upstream readable / Sly 1 name).
extern "C" int HasBhvSearchReachedTarget(BHV171 *behavior) {
    STEPGUARD *guard = ((STEPGUARD *)behavior->owner);
    LO *t;
    float d;
    if (!(guard->capabilityFlags & 2) || guard->searchMode == 2) return 1;
    t = guard->searchTarget;
    if (!t) return 0;
    d = Dist(GetLoWorldPos(t), GetLoWorldPos(((STEPGUARD *)behavior->owner)));
    if (Dist(GetBhvSearchLastKnownPosition(behavior), GetLoWorldPos(((STEPGUARD *)behavior->owner))) < d + 50.0f) return 1;
    return 0;
}

// 0x172760: Returns the last known target position from the shared target record (its first position when this
// guard is in mode 4 for it, else its second).
// Also known as: PosNoise (upstream readable / Sly 1 name).
extern "C" qword GetBhvSearchLastKnownPosition(BHV171 *behavior)
{
    NOISE *t = behavior->watched;
    if (t->level == 4 && t->subject == ((STEPGUARD *)behavior->owner))
        return t->position;
    return behavior->watched->targetPosition;
}

// 0x172798: Adds a candidate point to a 64-entry list if its pitch and distance from the guard are within limits,
// recording its heading relative to the guard.
// Also known as: AddSearchPlace (upstream readable / Sly 1 name; AddSearchPlace__FP8WAYQUERY).
extern "C" int TryAddSearchCandidate(QUERY *query) {
    float a, b, c;
    qword d;
    ROW *row;
    float f;
    d = VuSub(query->candidate.q, query->point.q);
    GetVectorSphere(d, &a, &b, &c);
    if (!IsWithinLimits((LM *)&D_002A53C0, c)) return 0;
    if (!IsWithinLimits((LM *)&D_002A53C8, b)) return 0;
    if (query->count >= 0x40) return 0;
    row = &query->row[query->count++];
    row->point.q = query->candidate.q;
    f = NormalizeAngle(a - query->heading);
    row->angle = f;
    if (f < 0.0f) row->angle = 6.28318530718f - f;
    return 1;
}

// 0x172888: Sort predicate for search candidates: by relative heading.
extern "C" int CompareSearchCandidates(ROW *a, ROW *b) { return a->angle < b->angle; }   /* 0 or 1, never negative: not a valid qsort comparator (the result depends on the qsort; the port must use the EE qsort, or a stable sort with a < b, to reproduce the order) */

struct VU_FLOAT{qword qw;};
// VU0 helper: vector difference.
// VU0 helper: vector sum.
// VU0 helper: length of a vector (in x).
static inline qword VuLength(qword v)
{
    qword qw, qwLen;
    qw = VuDot(v, v);
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qwLen) : "j"(qw));
    return qwLen;
}
// allocation-only "0"(vfOne): the length reuses the vfOne register, as in the target
static inline qword VuLengthReuseOne(qword v)
{
    qword qw, vfOne, qwLen;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne)
            : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qwLen) : "j"(qw), "0"(vfOne));
    return qwLen;
}
// VU0 helper: horizontal (xy) length of a vector.
// VU0 helper: x of a VU register as a float.
// VU0 helper: x component of a quadword vector.
// allocation-only second "j"(b): the extra reference raises b's local-alloc priority, so b gets vf1 and the dot vf2 as in the target
static inline qword VuDotProduct(qword a, qword b)
{
    qword qw, vfOne;
    __asm__("vmul.xyz %1, %2, %3\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z"
            : "=j"(vfOne), "=j"(qw)
            : "j"(a), "j"(b), "j"(b));
    return qw;
}
// VU0 helper: vector scaled by the x of g.
static inline qword VuScaleByX(qword v,qword g){qword qw;__asm__("vmulx.xyzw %0, %1, %2x":"=j"(qw):"j"(v),"0"(g));return qw;}
// VU0 helper: v divided by g.
// VU0 helper: base + vector * scale.
static inline qword VuMulAdd(qword base, qword vec, qword g)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(base), "j"(vec), "j"(g));
    return qw;
}
// VU0 helper: loads a float constant from a .data word into a VU register.
static inline qword VuFloatFromMemWord(int n){qword v;__asm__("qmtc2 %1, %0":"=j"(v):"rV"(n));return v;}
// VU0 helper: broadcasts a constant into a VU register (volatile so it is not hoisted, as in the target).
static inline qword VuFloatVolatile(float g)
{
    qword v;
    // volatile: the target re-broadcasts the constant inside the loop (loop.c must not hoist it)
    __asm__ __volatile__("qmtc2 %1, %0" : "=j"(v) : "r"(g));
    return v;
}
// vsub r = t - b. Allocation-only constraints: "=j"(t)/"1"(t) marks the copy t as consumed by the vsub, and "0"(b) puts r in b's register (as in the target)
#ifdef SLY2_EE_ASM
#define QW_SUB_T(r,t,b) __asm__("vsub.xyzw %0, %1, %2":"=j"(r),"=j"(t):"0"(b),"1"(t))
#else
#define QW_SUB_T(r, t, b) ((r) = VuSub((t), (b)))   /* r = t - b (t unchanged) */
#endif

namespace RO_ORDER { extern const LM r_D_002A53C0 __asm__("D_002A53C0") = {200.0f, 1200.0f}; }
namespace RO_ORDER { extern const LM r_D_002A53C8 __asm__("D_002A53C8") = {-0.785398185f, 0.785398185f}; }
namespace RO_ORDER { extern const float r_D_002A53D0 __asm__("D_002A53D0") = -3.40282347e+38f; }
// 0x1728B0: Chooses up to four search points: gathers candidate hiding places (several global object lists, cids
// 0x23/0x67/0x1B), sorts them by heading, keeps up to four well-spread points and fills the rest randomly behind
// the guard.
// Also known as: FindSearchPlaces (upstream readable / Sly 1 name).
extern "C" void ChooseBhvSearchPoints(BHV171 *behavior)
{
    WORK w;
    NODE *object;
    int i;
    unsigned accepted;
    float lastAngle;
    w.query.point.q = GetLoWorldPos(((STEPGUARD *)behavior->owner));
    w.query.heading = GetStepHeading(((STEPGUARD *)behavior->owner));
    w.query.count = 0;
    for (object = (*(NODE **)&g_sw.dlCid34); object; object = NEXT_ON(POLESPOT, dleGrab, object)) {
        qword posObj = GetLoWorldPos(object);
        qword qwFallback = D_002A2DA0;
        w.query.delta.q = VuSub(w.query.point.q, posObj);
        w.query.delta.i[2] = 0;
        qword len = VuLengthReuseOne(w.query.delta.q);
        qword g = VuFloatFromMemWord(*(int *)&((POLESPOT *)object)->sRadius);
        qword n;
        if (QwordX(len) < 0.0001f) {
            w.query.scalar.q = g;
            n = qwFallback;
        } else {
            w.query.scalar.q = g;
            n = VuDivByX(w.query.delta.q, len);
        }
        w.query.candidate.q = VuMulAdd(posObj, n, g);
        TryAddSearchCandidate(&w.query);
    }
    for (object = (*(NODE **)&g_sw.dlCid12); object; object = NEXT_ON(WALLSPOT, dleGrab, object)) {
        FindShapeClosestPoint(object, w.query.point.q, 0, &w.query.candidate.q, 0, 0, 0);
        TryAddSearchCandidate(&w.query);
    }
    for (object = (*(NODE **)&g_sw.dlCid38); object; object = NEXT_ON(CID38, dleGrab, object)) {
        w.query.candidate.q = GetLoWorldPos(object);
        TryAddSearchCandidate(&w.query);
    }
    for (object = (*(NODE **)&g_sw.dlCid88); object; object = NEXT_ON(BARREL, dleGrab, object)) {
        w.query.candidate.q = GetLoWorldPos(object);
        TryAddSearchCandidate(&w.query);
    }
    for (object = (*(NODE **)&g_sw.dlCid11); object; object = NEXT_ON(PIPE, dleGrab, object)) {
        FindShapeClosestPoint(object, w.query.point.q, 0, &w.query.candidate.q, 0, 0, 0);
        TryAddSearchCandidate(&w.query);
    }
    for (object = (*(NODE **)&g_sw.dlCid39); object; object = NEXT_ON(CID39, dleGrab, object)) {
        w.query.candidate.q = GetLoWorldPos(object);
        TryAddSearchCandidate(&w.query);
    }
    for (object = (*(NODE **)&g_sw.dlCid13); object; object = NEXT_ON(RAIL, dleGrab, object)) {
        FindShapeClosestPoint(object, w.query.point.q, 0, &w.query.candidate.q, 0, 0, 0);
        TryAddSearchCandidate(&w.query);
    }
    for (object = (*(NODE **)&g_sw.dlTarget); object; object = NEXT_ON(::TARGET, targetListLink, object)) {
        if ((((::TARGET *)object)->targetKindFlags & 1) && IsBasicDerivedFrom(object, CID_CLASS19)) {
            w.query.candidate.q = GetLoWorldPos(object);
            TryAddSearchCandidate(&w.query);
        }
    }
    {
        int count = FindSwObjectsByClass(&g_sw, 0x505, CID_CLASS35, 0, 0x100, w.result256);
        for (int i = 0; i < count; i++) {
            w.query.candidate.q = GetLoWorldPos(w.result256[i]);
            TryAddSearchCandidate(&w.query);
        }
    }
    {
        int count = FindSwObjectsByClass(&g_sw, 0x505, CID_BOOTY, 0, 0x20, w.result32);
        for (int i = 0; i < count; i++) {
            w.query.candidate.q = GetLoWorldPos(w.result32[i]);
            TryAddSearchCandidate(&w.query);
        }
    }
    {
        int count = FindSwObjectsByClass(&g_sw, 0x505, CID_CLASS27, 0, 0x80, w.result128);
        for (int i = 0; i < count; i++) {
            w.query.candidate.q = GetLoWorldPos(w.result128[i]);
            TryAddSearchCandidate(&w.query);
        }
    }
    for (object = (*(NODE **)&g_sw.dlCid98); object; object = NEXT_ON(CID98, dleBreakable, object)) {
        w.query.candidate.q = GetLoWorldPos(object);
        TryAddSearchCandidate(&w.query);
    }
    lastAngle = 0.0f;
    qsort(w.query.row, w.query.count, sizeof(ROW), CompareSearchCandidates);
    accepted = 0;
    {
        BHV171POINT *previous = 0;
        for (i = 0; i < (int)w.query.count && accepted < 4; i++) {
            ROW *row = &w.query.row[i];
            qword original = row->point.q;
            qword up = g_normalZ;
            qword delta = VuSub(original, w.query.point.q);
            qword qwFallback = D_002A2DA0;
            qword planar = VuSub(delta, VuScaleByX(up, VuDotProduct(delta, up)));
            qword len = VuLength(planar);
            if (QwordX(len) < 0.0001f)
                planar = qwFallback;
            else
                planar = VuDivByX(planar, len);
            qword s = VuFloatVolatile(200.0f);
            qword target = VuMulAdd(w.query.point.q, planar, s);
            w.query.delta.q = s;
            // the one register pin: the vsub consumes a copy of target in vf2, made before the previous test (target: vmove in the beqz slot)
            register qword probe __asm__("vf2") = target;
            if (previous) {
                qword d;
                QW_SUB_T(d, probe, previous->candidate);
                if (QwordX(VuLengthXY(d)) < 150.0f)
                    continue;
            }
            previous = &behavior->searchPoints[accepted++];
            lastAngle = row->angle;
            previous->original = original;
            previous->candidate = target;
        }
    }
    qword origin = w.query.point.q;
    qword anchor = VuAdd(origin, SetVectorCylind(w.query.heading + 3.1415927f, -350.0f, 0.0f));
    while (accepted < 4) {
        float angle, length;
        qword point;
        lastAngle += RandomFloatInRange(D_002BE5A8.minValue, D_002BE5A8.maxValue);
        angle = lastAngle + w.query.heading;
        length = RandomFloatInRange(D_002BE5B0.minValue, D_002BE5B0.maxValue);
        point = VuAdd(anchor, SetVectorCylind(angle, length, 0.0f));
        {
            BHV171POINT *pair = &behavior->searchPoints[accepted];
            pair->candidate = point;
            pair->original = point;
        }
        accepted++;
    }
    behavior->searchPointIndex = 0;
    behavior->nextTime = D_002A53D0;
}

namespace RO_ORDER { extern const float r_D_002A53D4 __asm__("D_002A53D4") = 3.40282347e+38f; }
// 0x172F10: About once a second after arriving, sends the goto child to the next of the four search points.
// Also known as: NextSearchPlace (upstream readable / Sly 1 name).
extern "C" void StepBhvSearchPoint(BHV171 *behavior) {
    void *pbhvCur;
    BHV171POINT *pe;
    GOAL goal;
    if (behavior->searchPointIndex >= 4) return;
    pbhvCur = behavior->pbhvNext;
    if (behavior->nextTime == D_002A53D4) {
        if (HasBhvgotoArrived(pbhvCur)) behavior->nextTime = g_clock.time + 1.0f;
    }
    if (g_clock.time < behavior->nextTime) return;
    pe = &behavior->searchPoints[behavior->searchPointIndex++];
    if (behavior->searchPointIndex >= 4) behavior->searchPointIndex = 0;
    ClearPointerPair(&goal);
    goal.pos = pe->candidate;
    SetBhvgotoGoal(pbhvCur, &goal);
    behavior->nextTime = D_002A53D4;
}
