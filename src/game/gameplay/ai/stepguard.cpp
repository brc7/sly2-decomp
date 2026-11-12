// stepguard.cpp -- STEPGUARD (cid 0x38), the guard: the step object every guard and named enemy derives from. Load
// and post-load (detector, components, animation bindings, attacks), the per-frame update (special states, target
// detection, aim, pickpocket, behaviour stack, mover), damage and zap reactions, the guard flags (SGF: one bit per
// guard condition, mirrored into animation choice), animation bindings (a flag set -> an ASEG/SM to play), target
// choice and the detector's watched/ignored/forced class lists, stimulus perception, the mover (which behaviour
// drives the guard's motion), the shared per-target engagement records, attack objects (cid 0x78) and the guard
// manager (cid 0x7D).
// TU 0x205E70..0x20B840 (158 functions, address order).
// The per-function files keep their views in namespaces named after the function (docs/guides/tu-merging.md
// "Representation decisions"); the former m2c bodies are written at file scope on sly2/guard.h's STEPGUARD,
// DETECTOR, ANIMBINDING and BHV156 and the TU-private views below.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_stepguard
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_stepguard
#include "sly2/port.h"   /* SLY2_EE_LAYOUT */
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/step.h"
#include "sly2/guard.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/sw.h"
#include "sly2/cref.h"
#include "sly2/scrap.h"
#include "sly2/classes.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/act.h"
#include "sly2/clock.h"
#include "sly2/cm.h"
#include "sly2/game.h"
// 0x20B6E8 StaticInit_stepguard and GlobalConstructors_stepguard: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* Class ids used here (docs/systems/object-model.md section 1). */
enum {
    CID_PIPE = 0xB,
    CID_RAIL = 0xD,
    CID_CLASS19 = 0x13,
    CID_CLASS39 = 0x27,
    CID_STEPPLAYER = 0x34,
    CID_MURRAY = 0x36,
    CID_STEPGUARD = 0x38,
    CID_GUARD = 0x3A,
    CID_DIMITRI = 0x3B,
    CID_NEELA = 0x3C,
    CID_RAJAN = 0x3D,
    CID_JEANBISON = 0x3E,
    CID_CARMELITA = 0x3F,
    CID_CONTESSA = 0x40,
    CID_ARPEGGIO = 0x41,
    CID_BEAR = 0x42,
    CID_BEARCUB = 0x43,
    CID_BOMB = 0x4F,
    CID_BHVGUARD = 0x9C,
    CID_BHVANIM = 0x9D,
    CID_BHV161 = 0xA1,
    CID_BHV162 = 0xA2,
    CID_BHV169 = 0xA9,
    CID_BHV176 = 0xB0,
    CID_BHVJUGGLED = 0xB1,
};
/* Object ids used here. */
enum {
    OID_DEFAULT_ATTACK = 0x2CC,     /* the attack a guard without attack oids gets (GetStepguardDefaultAttack) */
};
/* Guard flags (STEPGUARD +0x2C0, bit n = flag n; each behaviour raises its own: docs/systems/ai-guards.md
   section 2). Only the identified flags are named. */
enum {
    SGF_KO = 0x7,
    SGF_MOVING = 0x9,
    SGF_LEAP = 0xF,
    SGF_JUGGLED = 0x17,
    SGF_COMBAT = 0x1D,
};
/* Special states (STEPGUARD +0xFBC, -1 none): the values are the ids of the gadgets that cause them
   (docs/systems/ai-guards.md section 6). */
enum {
    SPECIAL_NONE = -1,
    SPECIAL_SIZE_DESTABILIZER = 0x8,
    SPECIAL_GADGET_0B = 0xB,            /* not identified; bosses are immune */
    SPECIAL_REDUCTION_BOMB = 0xD,
    SPECIAL_TEMPORAL_LOCK = 0xE,
    SPECIAL_GUTTURAL_ROAR = 0x15,
    SPECIAL_INSANITY_STRIKE = 0x22,
    SPECIAL_RAGE_BOMB = 0x25,
};
/* Animation binding kinds (ANIMBINDING::kind): what the bound oid resolved to and how StartAnimBinding plays it.
   Word +0x4 holds the oid until resolved, then the resource or the kind's payload; +0x8 the running instance. */
enum {
    BINDING_NONE = -1,
    BINDING_ASEG = 0,       /* an animation segment, played as an ASEGA */
    BINDING_SM = 1,         /* a state machine, played as an SMA */
    BINDING_LEAP = 2,       /* a leap payload, played on the guard's LEAP (+0xDB0) */
    BINDING_DIRANIM = 3,    /* a four-animation payload, played on the guard's DIRANIM (+0xE20) */
};

#define PMOVER(guard) ((GUARDMOVER *)((char *)(guard) + 0xFD0))
/* The sub-behaviour of the guard's root behaviour in mode 0x10, as SetStepguardEscortPoint fills it. */
struct BHVARG : public BHV
{
    LO *escortObject;       /* +0x30 */
    int pad34[3];
    qword qw;               /* +0x40 */
};

extern ulong D_003DD480;        /* declared first: .bss is emitted in first-declaration order */
extern int D_003DD488;
extern LO *D_003DD48C;          /* the guard manager */
extern LO *D_002DD648;
extern float D_002A8600;
extern float D_002A8604;
extern float D_002A8610;
extern int D_002C3CF0;
LO *CreateLo(CID cid, SO *psoParent, OID oid, int n);
int IsBasicDerivedFrom(BASIC *pbasic, CID cid);
void AppendDlEntry(DL *pdl, void *pv);
void CopyAb(void *pvDst, void *pvSrc, unsigned int cb);
void ClearBhvstk(BHVSTK *stack);
extern "C" {
void CloneStep(STEPGUARD *guard, STEPGUARD *pstepguardBase);
void CloneLoV() __asm__("CloneLo");                      /* a0/a1 passed through */
void RemoveSoHierarchyV() __asm__("RemoveSoHierarchy");  /* a0 passed through */
void ReleaseAsega(ASEGA **ppasega);
void SetLoBusyKind(LO *plo, int busyk);
void ReleaseStepguardZapReactions(STEPGUARD *guard);
void InitStepguardMover(GUARDMOVER *mover, STEPGUARD *guard);
void EnsureStepguardManager(void);
int MaskHasSgf(unsigned long grfsg, int sgf);
void UpdateStepguardPickpocket(STEPGUARD *guard);
int TryAddStepguardDetectTarget(STEPGUARD *guard, LO *object, int n);
void *FindStepBehavior(STEPGUARD *guard, int cid);
void SetBhvThrownState(void *behavior, int n);
BHV *FindBhvInBhvstk(BHVSTK *stack, int cid);
int IsStepguardTargetValid(LO *object, LO *ploOther, int n);
int FindTargetRecordGuardIndex(TARGETREC *record, STEPGUARD *guard);
void SetTargetRecordState(TARGETREC *record, int n);
TARGETREC *EnsureTargetRecordById(LO *object);
void WakeSo(STEPGUARD *guard);
extern "C++" { void SetStepTargetHeading(STEPGUARD *guard) __asm__("SetStepTargetHeading"); }
unsigned long GetSgfMaskFromSpliceList(void *list);
ANIMBINDING *EnsureAnimBinding(STEPGUARD *guard, unsigned long sgfMask);
void ResolveAnimBinding(ANIMBINDING *binding, STEPGUARD *guard);
void ReleaseAnimBinding(ANIMBINDING *binding, STEPGUARD *guard);
void CloneAnimBinding(ANIMBINDING *binding);
void CloneZapReactions(STEPGUARD *guard, ANIMBINDING *table);
void FreeLeap(int h);
void FreeDirAnim(int h);
void AddStepguardAttackOid(STEPGUARD *guard);   /* the oid passes through in a1 */
void ResolveStepguardAttacks(STEPGUARD *guard);
void SetGuardAttackBindingKind2(GUARDATTACK *attack);
void SetStepguardAnimBinding(STEPGUARD *guard, void *sgfList, OID oid, int flags);
void ResolveZapReactions(STEPGUARD *guard, ANIMBINDING *table);
void ReleaseZapReactions(STEPGUARD *guard, ANIMBINDING *table);
void CloneStepguardAnimBindings(STEPGUARD *guard);
void SetStepguardTarget(STEPGUARD *guard, LO *ploTarget);
void ClearStepguardIgnoreClasses(STEPGUARD *guard);
void ClearStepguardForceClasses(STEPGUARD *guard);
void CloneStepguardMover(GUARDMOVER *mover, STEPGUARD *guard);
int StepguardHasSgf(STEPGUARD *guard, int sgf);
void RemoveTargetRecordGuard(TARGETREC *record, STEPGUARD *guard);
void AddTargetRecordGuard(TARGETREC *record, STEPGUARD *guard);
}

// 0x205E70 EnsureStepguardManager
// 0x205E70: Creates the world's guard manager (cid 0x7D, oid 0x656) once and stores it in D_003DD48C.
/* Creates the guard manager object (CID 0x7D, oid 0x656) once. */
extern "C" void EnsureStepguardManager(void)
{
    if (D_003DD48C == 0) {
        D_003DD48C = CreateLo((CID)0x7D, 0, (OID)0x656, -1);
    }
}

// 0x205EB8 InitStepguard
// 0x205EB8: STEPGUARD vtable slot 7 (Init): step Init, ensures the guard manager, creates the vision detector (cid
// 0x10) and sets the default vision ranges and angles, hit points, flag masks and the behaviour stack.
// (Claude agent, nonvu16, 2026-10-17) plain stores, no register pins; store order tuned (see notes/200000.md).
// Also known as: InitStepguard (upstream readable / Sly 1 name; InitStepguard__FP9STEPGUARD).
void STEPGUARD::Init()
{
    STEPGUARD *guard = this;
    sptr v;
    float a, b, limit, medium, high, extent;
    STEP::Init();
    EnsureStepguardManager();
    guard->pdetector = (DETECTOR *)CreateLo((CID)0x10, 0, (OID)0x288, -1);
    SetLoBusyKind(guard->pdetector, 2);
    v = (sptr)guard->pdetector->ploupd;
    if (!(v & 1))
        ((LOUPD *)v)->grf &= -3;
    else
        guard->pdetector->inlineFlags = v & -3;
    a = 0.785398245f;
    b = 1.30899704f;
    limit = D_002A8600;
    medium = 300.0f;
    high = 1200.0f;
    extent = 800.0f;
    guard->shootPan.minValue = -a;
    guard->shootPan.maxValue = a;
    guard->shootTilt.minValue = -b;
    guard->shootTilt.maxValue = b;
    guard->shootRange.minValue = medium;
    guard->shootRange.maxValue = high;
    guard->attackRange.minValue = extent;
    guard->unknownRange31C.minValue = medium;
    guard->walkTimes.minValue = D_002A8600;
    guard->attackRange.maxValue = high;
    guard->unknownRange31C.maxValue = 450.0f;
    guard->circleDistance.minValue = medium;
    guard->circleDistance.maxValue = 600.0f;
    guard->walkTimes.maxValue = limit;
    guard->closeAngleRange.minValue = -100.0f;
    guard->closeAngleRange.maxValue = 350.0f;
    guard->closeRange = 1000.0f;
    guard->interactAngleRange.minValue = -800.0f;
    guard->interactAngleRange.maxValue = extent;
    guard->interactRange = 3000.0f;
    guard->sleepTimeMin = 16.0f;
    guard->sleepTimeMax = 20.0f;
    guard->sleepDartCapacity = 50;
    guard->lookTimes.minValue = 5.5f;
    guard->lookTimes.maxValue = 6.5f;
    guard->knockdownZapMask = 0x29FF7E;
    guard->unknown364 = 1;
    guard->canInteract = 1;
    guard->sensesEnabled = 1;
    guard->unknownF40 = 1;
    guard->minStimulusLevel = -1;
    guard->searchMode = 0;
    guard->hurtZapMask = 0x20F800;
    guard->koZapMask = 0xF800;
    guard->pulseStartTime = -10.0f;
    guard->unknown4A0 = 1;
    guard->gravityRatio = 1.5f;
    guard->unknownBits148A = 2;
    guard->unknown3F0 = -200;
    guard->coinCountOverride = -1;
    guard->attachKind = -1;
    guard->pickpocketState = -1;
    guard->specialState = -1;
    guard->walkPhaseTime = D_002A8604;
    guard->removalMode = D_002C3CF0;
    ReleaseStepguardZapReactions(guard);
    InitStepguardMover(&guard->mover, guard);
}

// 0x206100 CloneStepguard
// 0x206100: STEPGUARD vtable slot 41 (Clone): step clone keeping its own detector, re-owning the mover, copying the
// detector state and deep-copying the animation bindings.
/* Clones the guard but keeps its own vision detector, which clones the other guard's detector (LO slot 41). */
// Also known as: CloneStepguard (upstream readable / Sly 1 name; CloneStepguard__FP9STEPGUARDT0).
extern "C" void CloneStepguard(STEPGUARD *guard, STEPGUARD *pstepguardBase)
{
    DETECTOR *detector;
    detector = guard->pdetector;
    CloneStep(guard, pstepguardBase);
    CloneStepguardMover(PMOVER(guard), guard);
    guard->pdetector = detector;
    detector->Clone(pstepguardBase->pdetector);
    CloneStepguardAnimBindings(guard);
}

// 0x206170 ResetStepguardFromScript
// 0x206170: Script action: reset to home (vtable slot 26), then send itself splice event 0xE.
extern "C" void ResetStepguardFromScript(STEPGUARD *guard)
{
    int n;
    void *apv[1];
    guard->OnProxyAcquire(0);
    n = 0;
    apv[0] = &n;
    guard->HandleSpliceEvent(0xE, 1, 1, apv, 0);
}

// 0x2061D8 PostStepguardLoad
/* an unaligned long constant (the original loads it with ldl/ldr) */
extern int D_002C3CF8[3];
extern const LONGU D_002A8608;
extern qword D_002A2DE0;
extern "C" {
void BindLoOidTable(STEPGUARD *guard, int n, void *pv);
void SetStepGravityRatio(STEPGUARD *guard, float f);
void SetXfRole(STEPGUARD *guard, XF *pxf, int n);
void ResolveStepguardAnimBindings(STEPGUARD *guard);
void *func_001AE4F8(int cb) __asm__("__builtin_new");
void InitStuckWatch(void *pv, STEPGUARD *guard);
void InitHazardWatch(void *pv, STEPGUARD *guard);
void PostSwCallback(void *g, STEPGUARD *guard, long k, int n);
void AddGuardScriptGoto(LO *po, int a, int b, float f);
void *FindSoXf(STEPGUARD *root, int oid, int b);
}
ACT *NewPositionAct(STEPGUARD *guard, void *x, int a, int b, int c, qword q) __asm__("NewPositionAct");

// 0x2061D8: STEPGUARD vtable slot 15 (PostLoad): step post-load, binds its oid table, gravity ratio, attaches
// children, resolves animation bindings and attack oids, and creates its stuck and hazard watches (+0x498/+0x49C).
// Also known as: PostStepguardLoad (upstream readable / Sly 1 name; PostStepguardLoad__FP9STEPGUARD).
void STEPGUARD::PostLoad()
{
    STEPGUARD *guard = this;
    void *x;
    STEP::PostLoad();
    BindLoOidTable(guard, 1, D_002C3CF8);
    SetStepGravityRatio(guard, guard->gravityRatio);
    SetXfRole(guard, guard->positionXf, 2);
    SetXfRole(guard, guard->rotationXf, 2);
    ResolveStepguardAnimBindings(guard);
    ResolveStepguardAttacks(guard);
    guard->stuckWatch = (STUCKWATCH *)func_001AE4F8(0x10);
    InitStuckWatch(guard->stuckWatch, guard);
    guard->hazardWatch = (HAZARDWATCH *)func_001AE4F8(0x14);
    InitHazardWatch(guard->hazardWatch, guard);
    if (!guard->proxyRecord->isCopy)
        PostSwCallback(&g_sw, guard, D_002A8608.l, 0);
    guard->altPatrolScript = (GUARDSCRIPT *)CreateLo((CID)0x71, 0, (OID)0x2CB, -1);
    AddGuardScriptGoto(guard->altPatrolScript, -1, -1, 40.0f);
    guard->homeRecord = (SSGE *)guard->altPatrolScript->commands.head;
    guard->altPatrolScript->Remove();
    if (guard->pickpocketIndicator0)
        guard->pickpocketIndicator0->Remove();
    if (guard->pickpocketIndicator1)
        guard->pickpocketIndicator1->Remove();
    x = FindSoXf(guard, guard->oid, 0xC);
    if (x)
        guard->scale = NewPositionAct(guard, x, 0, 7, 2, D_002A2DE0);
}

// 0x206378 GetStepguardDetector
// 0x206378: Returns the guard's vision detector.
extern "C" DETECTOR *GetStepguardDetector(STEPGUARD *guard)
{
    return guard->pdetector;
}

// 0x206380 ShouldStepguardIgnoreIntersection
extern "C" {
int ShouldSoIgnoreIntersection(STEPGUARD *guard, SO *psoOther);
int MaxInt(int ignore, int b);
}
// 0x206380: STEPGUARD vtable slot 61 (should an intersection with `other` be ignored): the step test, plus, while dead
// (flag 7), objects that are not fixed (slot 62), and, while juggled/carried (flag 0x17), objects of its holder's
// physics tree in juggled states 0/4/5.
// Also known as: OverlapKindGuard (upstream readable / Sly 1 name; OverlapKindGuard__FP3ALOT0).
int STEPGUARD::ShouldIgnoreIntersection(SO *other)
{
    STEPGUARD *guard = this;
    int ignore = ShouldSoIgnoreIntersection(guard, other);
    if (StepguardHasSgf(guard, SGF_KO)) {
        if (!other->IsFixed())
            ignore = MaxInt(ignore, 1);
    } else if (StepguardHasSgf(guard, SGF_JUGGLED)) {
        BHVJUGGLED *juggled = (BHVJUGGLED *)FindBhvInBhvstk(&guard->behaviors, CID_BHVJUGGLED);
        if (other->physicsTree == juggled->holder->physicsTree) {
            switch (juggled->juggleState) {
            case 0:
            case 4:
            case 5:
                ignore = MaxInt(ignore, 1);
                break;
            }
        }
    }
    return ignore;
}

// 0x206458 GetStepguardMaxHitPoints
// 0x206458: Splice getter: maximum hit points as the level sees them ((maxHitPoints - 1) / 10, at least 0).
/* The max-hp level: (maxHitPoints - 1) / 10, at least 0. */
extern "C" int GetStepguardMaxHitPoints(STEPGUARD *guard)
{
    int hp;
    hp = guard->maxHitPoints - 1;
    return ((hp <= -1) ? 0 : hp) / 10;
}

// 0x206488 SetStepguardMaxHitPoints
// 0x206488: Splice setter: maximum hit points (stored as (n + 1) * 10).
extern "C" void SetStepguardMaxHitPoints(STEPGUARD *guard, int nLevel)
{
    guard->maxHitPoints = (nLevel + 1) * 10;
}

// 0x2064A0 GetStepguardHpLevel
// 0x2064A0: Splice getter: the current hit points level (max(hp - 1, 0) + 1, inferred).
/* The hp level: hp - 1 (at least 0) plus 1. */
extern "C" int GetStepguardHpLevel(STEPGUARD *guard)
{
    int hp;
    hp = guard->hitPoints - 1;
    return ((hp <= -1) ? 0 : hp) + 1;
}

// 0x2064C0 SetStepguardHpLevel
// 0x2064C0: Splice setter: the current hit points (stored as (n + 1) * 10).
extern "C" void SetStepguardHpLevel(STEPGUARD *guard, int nLevel)
{
    guard->hitPoints = (nLevel + 1) * 10;
}

// 0x2064D8 RemoveStepguardHierarchy
// 0x2064D8: STEPGUARD vtable slot 12 (RemoveHierarchy): base, clears the target and the behaviour stack and
// retracts its held animation.
// Also known as: OnStepguardRemove (upstream readable / Sly 1 name; OnStepguardRemove__FP9STEPGUARD).
void STEPGUARD::RemoveHierarchy()
{
    STEPGUARD *guard = (STEPGUARD *)this;
    RemoveSoHierarchyV();
    SetStepguardTarget(guard, 0);
    ClearBhvstk(&guard->behaviors);
    ReleaseAsega(&guard->heldAnimation);
}

// 0x206518 ResetStepguardToHome
namespace M_ResetStepguardToHome {
extern LO *D_002E1E40 __asm__("D_002E1E40"), *D_002F7900 __asm__("D_002F7900"), *D_002DD5BC __asm__("D_002DD5BC");
void ClearStepguardMover(GUARDMOVER *mover) __asm__("ClearStepguardMover");
void SetStepguardSgfMask(STEPGUARD *guard, int n) __asm__("SetStepguardSgfMask");
void ResetStuckWatch(void *p) __asm__("ResetStuckWatch");
void ResetHazardWatch(void *p) __asm__("ResetHazardWatch");
void EnableStuckWatch(void *p) __asm__("EnableStuckWatch");
qword GetLoWorldPos(LO *plo) __asm__("GetLoWorldPos");
float GetStepHeading(STEPGUARD *guard) __asm__("GetStepHeading");
void SetStepTargetHeading(STEPGUARD *guard, float f) __asm__("SetStepTargetHeading");
unsigned long GetSgfMask(int n) __asm__("GetSgfMask");
void *FindBestAnimBinding(STEPGUARD *guard, unsigned long m) __asm__("FindBestAnimBinding");
void UpdateDetectorTrackedObject(DETECTOR *detector, STEPGUARD *guard, LO *plo) __asm__("UpdateDetectorTrackedObject");
void GenerateStepguardLoot(STEPGUARD *guard) __asm__("GenerateStepguardLoot");
void StageStepguardLoot(STEPGUARD *guard) __asm__("StageStepguardLoot");
BHV *PushStepBehavior(STEPGUARD *guard, int n) __asm__("PushStepBehavior");
void SetLoParent(DETECTOR *detector, STEPGUARD *guard, int n) __asm__("SetLoParent");
}

// 0x206518: STEPGUARD vtable slot 26 (reset to home, used by proxies and spawners): restores maximum values (hit
// points etc.), clears behaviours, mover and flags, re-caches the home position and heading, and derives walk/run
// capability bits from the animation bindings.
// Also known as: ResetGuard (upstream readable / Sly 1 name; ResetGuard__FP2LO).
void STEPGUARD::OnProxyAcquire(LO *plo)
{
    using namespace M_ResetStepguardToHome;
    STEPGUARD *guard = this;
    BHV *o;
    qword *position;
    SO::OnProxyAcquire(plo);
    position = &guard->homePosition;
    guard->hitPoints = guard->maxHitPoints;
    guard->sleepDartProgress = guard->sleepDartCapacity;
    guard->staggerProgress = guard->staggerCapacity;
    ClearBhvstk(&guard->behaviors);
    ClearStepguardMover(&guard->mover);
    SetStepguardSgfMask(guard, 0);
    ResetStuckWatch(guard->stuckWatch);
    ResetHazardWatch(guard->hazardWatch);
    EnableStuckWatch(guard->stuckWatch);
    guard->homePosition = GetLoWorldPos(guard);
    guard->homeHeading = GetStepHeading(guard);
    guard->homeRecord->ppos = position;
    guard->homeRecord->heading = guard->homeHeading;
    SetStepTargetHeading(guard, guard->homeHeading);
    guard->capabilityFlags = 0;
    if (FindBestAnimBinding(guard, GetSgfMask(SGF_MOVING)))
        guard->capabilityFlags |= 2;
    else
        guard->searchMode = 2;
    if (FindBestAnimBinding(guard, GetSgfMask(8)))
        guard->capabilityFlags |= 4;
    if (FindBestAnimBinding(guard, GetSgfMask(SGF_LEAP)))
        guard->capabilityFlags |= 1;
    if (FindBestAnimBinding(guard, GetSgfMask(0x11)))
        guard->capabilityFlags |= 8;
    UpdateDetectorTrackedObject(guard->pdetector, guard, D_002E1E40);
    UpdateDetectorTrackedObject(guard->pdetector, guard, D_002F7900);
    UpdateDetectorTrackedObject(guard->pdetector, guard, D_002DD5BC);
    UpdateDetectorTrackedObject(guard->pdetector, guard, D_002DD648);
    if (guard->lootPouch) {
        GenerateStepguardLoot(guard);
        StageStepguardLoot(guard);
    }
    if (guard->knockdownHelper)
        guard->knockdownHelper->Remove();
    o = PushStepBehavior(guard, CID_BHVGUARD);
    o->Start();
    SetLoParent(guard->pdetector, guard, 0xF);
    guard->pdetector->owner = guard;
    guard->pdetector->ownerPhysicsTree = guard->physicsTree;
}

// 0x206748 ClearStepguardState
namespace M_ClearStepguardState {
void DetachStepguard(STEPGUARD *guard) __asm__("DetachStepguard");
void *func_002333B4(void *p, int c, int n) __asm__("memset");
void ClearStepguardTargetLists(STEPGUARD *guard) __asm__("ClearStepguardTargetLists");
void SetStepguardPickpocketState(STEPGUARD *guard, int a) __asm__("SetStepguardPickpocketState");
void SetStepguardSpecial(STEPGUARD *guard, int n) __asm__("SetStepguardSpecial");
}
// 0x206748: STEPGUARD vtable slot 27 (clear state): base, detaches, clears counters, targets and target lists,
// special state and pickpocket state.
void STEPGUARD::OnProxyRelease(LO *plo)
{
    using namespace M_ClearStepguardState;
    STEPGUARD *guard = this;
    int one;
    SO::OnProxyRelease(plo);
    DetachStepguard(guard);
    guard->patrolScript = 0;
    func_002333B4(&guard->scriptResumePoint, 0, 0xC);
    guard->numAllowedZones = 0;
    guard->pdetector->isForced = 0;
    guard->pdetector->isDisabled = 0;
    guard->numLoot = 0;
    guard->firstLoot = 0;
    guard->isLootStaged = 0;
    guard->isLootFixed = 0;
    guard->designatedTargetCount = 0;
    guard->avoidObjectCount = 0;
    guard->forcedTarget = 0;
    SetStepguardTarget(guard, 0);
    ClearStepguardTargetLists(guard);
    one = 1;
    guard->isKnockedOut = 0;
    guard->escortObject = 0;
    guard->isHandsOff = 0;
    guard->handsOffLookAt = 0;
    guard->pulseStartTime = -10.0f;
    guard->unknown4A0 = one;
    guard->walkPhaseTime = D_002A8610;
    guard->removalMode = D_002C3CF0;
    SetStepguardPickpocketState(guard, -1);
    SetStepguardSpecial(guard, SPECIAL_NONE);
}

// 0x206820 RestartStepguard
namespace M_func_00206820 {
#if defined(SLY2_EE_LAYOUT)
struct STEPGUARDIsland; struct REQUEST;
/* island: the vtable pointer pinned to $3 across the first call (real virtual calls: 2/45 off, request store scheduled after the slot load) */
struct STEPGUARDIsland { char pad[0xC]; VTE *vt; };
#define VT(p, n, T) ((T)(p)->vt[n].pfn)
struct REQUEST { int value; int *pointer; };
// 0x206820: Restarts the guard: clear state (slot 27), message 0xF, reset to home (slot 26), message 0xE.
extern "C" void RestartStepguard(struct STEPGUARDIsland *guard)
{
    struct REQUEST request;
    register VTE *initialTable __asm__("$3");
    request.pointer=&request.value;
    initialTable=guard->vt;
    request.value=0;
    ((void (*)(void*,int))initialTable[27].pfn)((char*)guard+initialTable[27].d,0);
    VT(guard, 3, void (*)(void *, int, int, int, int **, int))((char *)guard + guard->vt[3].d, 0xF, 0, 1, &request.pointer, 0);
    VT(guard, 26, void (*)(void *, int))((char *)guard + guard->vt[26].d, 0);
    VT(guard, 3, void (*)(void *, int, int, int, int **, int))((char *)guard + guard->vt[3].d, 0xE, 0, 1, &request.pointer, 0);
}
#else
struct REQUEST { int value; int *pointer; };
// Restarts the guard: clear state (slot 27, OnProxyRelease), splice event 0xF, reset to home (slot 26,
// OnProxyAcquire), splice event 0xE (slot 3, HandleSpliceEvent, one argument: a pointer to a 0 word).
extern "C" void RestartStepguard(STEPGUARD *guard)
{
    struct REQUEST request;
    request.pointer = &request.value;
    request.value = 0;
    guard->OnProxyRelease(0);
    guard->HandleSpliceEvent(0xF, 0, 1, (void **)&request.pointer, 0);
    guard->OnProxyAcquire(0);
    guard->HandleSpliceEvent(0xE, 0, 1, (void **)&request.pointer, 0);
}
#endif
} // namespace M_func_00206820

// 0x2068D8 CompareGuardCandidateDistance
// 0x2068D8: qsort comparator for guard candidates on their leading distance (-1 if a < b, else 1).
/* qsort comparison of guard candidates by distance (their first word). */
// Also known as: FUN_00148910 (upstream readable / Sly 1 name; FUN_00148910__FPfT0).
extern "C" int CompareGuardCandidateDistance(float *pg1, float *pg2)
{
    int n;
    n = 1;
    if (*pg1 < *pg2) {
        n = -1;
    }
    return n;
}

// 0x206900 UpdateStepguardDetectTargets
namespace M_UpdateStepguardDetectTargets {
struct CANDIDATE;

typedef unsigned long ulong;

struct CANDIDATE {float distance;int *source;STEPGUARD *object;int mode;float stamp;int unused14;};
extern int D_003DD488 __asm__("D_003DD488");
extern ulong D_003DD480 __asm__("D_003DD480");
extern float D_002A8614 __asm__("D_002A8614");
extern float D_002C3D04 __asm__("D_002C3D04"),D_002C3D08 __asm__("D_002C3D08"),D_002C3D0C __asm__("D_002C3D0C");
ulong GetSgfMask(int) __asm__("GetSgfMask");
int FindSwObjectsByClass(SW *,int,int,int,int,STEPGUARD **) __asm__("FindSwObjectsByClass");
qword GetLoWorldPos(LO *) __asm__("GetLoWorldPos");
void qsort(void *,__SIZE_TYPE__,__SIZE_TYPE__,int (*)(const void *,const void *)) __asm__("qsort");
int CompareGuardCandidateDistance(const void *,const void *) __asm__("CompareGuardCandidateDistance");
int TryAddStepguardDetectTarget(STEPGUARD *,STEPGUARD *,int) __asm__("TryAddStepguardDetectTarget");
void UpdateDetectorTargets(DETECTOR *) __asm__("UpdateDetectorTargets");
DETECTORTARGET *FindDetectorTarget(DETECTOR *,LO *) __asm__("FindDetectorTarget");
// VU0 helper: vector difference.
// VU0 helper: horizontal (xy) length of a vector.
extern "C" { static __inline__ float LengthXY(qword a)
{
#ifdef SLY2_EE_ASM
    qword r;
    float f;
    __asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y" : "=j"(r) : "j"(a));
    __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "+j"(r));
    __asm__("mtc1 %1,%0" : "=f"(f) : "r"(r));
    return f;
#else
    VuLanes v = VuGet(a);
    return VuSqrt(v.x * v.x + v.y * v.y);
#endif
} }
// z component of a quadword vector.
// Absolute value.
// True if a difference vector is within the horizontal and vertical .data limits.
extern "C" { static __inline__ int Near(qword difference){return LengthXY(difference)<D_002C3D08 && Abs(QwordZ(difference))<D_002C3D0C;} }
/* Two empty-asm tricks for the nearby-object collection loop (T1592); neither emits an instruction:
   OPAQUE_COUNT: an empty asm that "sets" the candidate count. loop.c then sees no basic induction variable, so
     &candidates[n] stays a real `mult` by 24 instead of a strength-reduced pointer, and the increment is not PRE-copied.
   LOOP_SIZE_PAD: 24 empty volatile asms at the end of the loop body. They emit nothing, but loop.c's move_movables only
     hoists a constant out of a loop when threshold(95)*savings*lifetime >= insn count; the original loop body has ~96 RTL
     insns, so only the D_002C3D04 high part is hoisted (to $fp) and the `li 24` / `li -1` stay inside the loop. */
#ifdef SLY2_EE_ASM
#define OPAQUE_COUNT(n) __asm__("":"+r"(n))
#define PAD1 __asm__ volatile("");
#else
#define OPAQUE_COUNT(n)
#define PAD1
#endif
#define PAD4 PAD1 PAD1 PAD1 PAD1
#define LOOP_SIZE_PAD PAD4 PAD4 PAD4 PAD4 PAD4 PAD4
// 0x206900: Per-frame target detection: unless blind, collects objects of the preferred classes within 1500
// horizontally, sorts them by distance and re-adds them as detector targets (keeping their states), updates the
// detector, then the shared target records.
// Also known as: RefreshGuardTargets (upstream readable / Sly 1 name; RefreshGuardTargets__FP5GUARD).
extern "C" void UpdateStepguardDetectTargets(STEPGUARD *guard)
{
    struct CANDIDATE candidates[64];
    STEPGUARD *objects[256];
    qword anchorPosition, ownerPosition;
    if (!D_003DD488) {
        ulong a = GetSgfMask(5);
        ulong b = GetSgfMask(6);
        ulong c = GetSgfMask(SGF_JUGGLED);
        ulong d = GetSgfMask(SGF_KO);
        ulong e = GetSgfMask(16);
        ulong f = GetSgfMask(1);
        ulong g = GetSgfMask(22);
        ulong h = GetSgfMask(42);
        D_003DD488 = 1;
        D_003DD480 = a | b | c | d | e | f | g | h;
    }
    if (guard->grfsg & D_003DD480)
        return;
    if (guard->preferClassCount) {
        int candidateCount = 0;
        int kind;
        for (kind = 0; kind < guard->preferClassCount; kind++) {
            int *source = &guard->preferClasses[kind];
            int objectCount = FindSwObjectsByClass(&g_sw, 0x505, *source, 0, 256, objects);
            int i = 0;
            if (objectCount > 0) {
                float initialStamp = D_002A8614;
                do {
                    STEPGUARD *object = objects[i];
                    if (object != guard) {
                        qword otherPosition;
                        float distance;
                        ownerPosition = GetLoWorldPos(guard);
                        otherPosition = GetLoWorldPos(object);
                        distance = LengthXY(VuSub(ownerPosition, otherPosition));
                        if (distance < D_002C3D04 && (unsigned int)candidateCount < 64) {
                            struct CANDIDATE *record;
                            int old;
                            record = &candidates[candidateCount];
                            record->distance = distance;
                            record->mode = -1;
                            record->source = source;
                            record->object = object;
                            record->stamp = initialStamp;
                            candidateCount++;
                            for (old = guard->pdetector->ctarget - guard->generatedTargetCount; old < guard->pdetector->ctarget; old++) {
                                DETECTORTARGET *entry = &guard->pdetector->atarget[old];
                                if (entry->object == record->object) {
                                    record->mode = entry->state;
                                    record->stamp = entry->stateTime;
                                    break;
                                }
                            }
                        }
                        OPAQUE_COUNT(candidateCount);
                        LOOP_SIZE_PAD
                    }
                } while (++i < objectCount);
            }
        }
        qsort(candidates, candidateCount, sizeof(struct CANDIDATE), CompareGuardCandidateDistance);
        guard->pdetector->ctarget -= guard->generatedTargetCount;
        guard->generatedTargetCount = 0;
        if (candidateCount > 0) {
            int absentMode = -1;
            struct CANDIDATE *record = candidates;
            do {
                if (TryAddStepguardDetectTarget(guard, record->object, (int)record->source)) {
                    guard->generatedTargetCount++;
                    if (record->mode != absentMode) {
                        DETECTORTARGET *entry = &guard->pdetector->atarget[guard->pdetector->ctarget - 1];
                        entry->state = record->mode;
                        entry->stateTime = record->stamp;
                    }
                }
                record++;
            } while (--candidateCount);
        }
    }
    UpdateDetectorTargets(guard->pdetector);
    if (guard->pvEngage) {
        DETECTORTARGET *entry = FindDetectorTarget(guard->pdetector, guard->pvEngage->target);
        if (entry) {
            switch (entry->state) {
            case 1:
                SetTargetRecordState(guard->pvEngage, 1);
                break;
            case 2: {
                qword ownPosition, difference;
                anchorPosition = GetLoWorldPos(guard->pvEngage->target);
                ownPosition = GetLoWorldPos(guard);
                difference = VuSub(anchorPosition, ownPosition);
                if (Near(difference))
                    SetTargetRecordState(guard->pvEngage, 2);
            } break;
            }
        }
    }
}

} // namespace M_UpdateStepguardDetectTargets

// 0x206D48 IsStepguardRageImmune
namespace M_func_00206D48 {
extern int D_002C3D10[] __asm__("D_002C3D10");
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
// 0x206D48: True if the guard derives from one of the five boss classes (cids 0x3B, 0x40, 0x3D, 0x3E, 0x41) that
// rage does not affect.
extern "C" int IsStepguardRageImmune(void *p)
{
    unsigned int i;
    for (i = 0; i < 5; i++)
        if (IsBasicDerivedFrom__FP5BASIC3CID(p, D_002C3D10[i]))
            return 1;
    return 0;
}
} // namespace M_func_00206D48

// 0x206DB0 AddStepguardRageIgnoreClasses
namespace M_func_00206DB0 {
extern int D_002C3D10[] __asm__("D_002C3D10");
int FindSwObjectsByClass(void *p, int a, int b, int c, int d, int e) __asm__("FindSwObjectsByClass");
void AddStepguardIgnoreClass(void *p, int n) __asm__("AddStepguardIgnoreClass");
// 0x206DB0: Adds those five boss classes (when present in the world) to the ignored-class list.
extern "C" void AddStepguardRageIgnoreClasses(void *p)
{
    unsigned int i;
    for (i = 0; i < 5; i++) {
        int n = D_002C3D10[i];
        if (FindSwObjectsByClass(&g_sw, 0x605, n, 0, 0, 0))
            AddStepguardIgnoreClass(p, n);
    }
}
} // namespace M_func_00206DB0

// 0x206E50 RemoveStepguardRageIgnoreClasses
namespace M_func_00206E50 {
extern int D_002C3D10[] __asm__("D_002C3D10");
void RemoveStepguardIgnoreClass(void *p, int n) __asm__("RemoveStepguardIgnoreClass");
// 0x206E50: Removes those five boss classes from the ignored-class list.
extern "C" void RemoveStepguardRageIgnoreClasses(void *p)
{
    unsigned int i;
    for (i = 0; i < 5; i++)
        RemoveStepguardIgnoreClass(p, D_002C3D10[i]);
}
} // namespace M_func_00206E50

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A8600 __asm__("D_002A8600") = 3.40282347e+38f; }
namespace RO_ORDER { extern const float r_D_002A8604 __asm__("D_002A8604") = -3.40282347e+38f; }
namespace RO_ORDER {   /* a non-virtual pointer-to-member constant {delta 0, index -1, pfn} */
void p_func_00206170() __asm__("ResetStepguardFromScript");
extern const unsigned int r_D_002A8608[2] __asm__("D_002A8608") = {0xFFFF0000, (unsigned int)p_func_00206170};
}
namespace RO_ORDER { extern const float r_D_002A8610 __asm__("D_002A8610") = -3.40282347e+38f; }
namespace RO_ORDER { extern const float r_D_002A8614 __asm__("D_002A8614") = -3.40282347e+38f; }
// 0x206EB0 CanSetStepguardSpecial
namespace M_func_00206EB0 {
int IsStepguardRageImmune(STEPGUARD *guard) __asm__("IsStepguardRageImmune");
int IsBasicDerivedFrom__FP5BASIC3CID(STEPGUARD *guard, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");

// 0x206EB0: True if special state `special` may be entered now (rules per state: never while dead or script-busy;
// some states not for bosses or rage-immune guards).
// Also known as: FGuardModeAllowed (upstream readable / Sly 1 name; FGuardModeAllowed__FP5GUARDi).
extern "C" int CanSetStepguardSpecial(STEPGUARD *guard, int special)
{
    BHV169 *bhv;
    if (special == -1)
        return 1;
    if (StepguardHasSgf(guard, SGF_KO))
        return 0;
    bhv = (BHV169 *)FindStepBehavior(guard, CID_BHV169);
    if (bhv && bhv->allowPartialRoute)
        return 0;
    switch (special) {
    case SPECIAL_SIZE_DESTABILIZER:
    case SPECIAL_REDUCTION_BOMB:
    case SPECIAL_TEMPORAL_LOCK:
        return 1;
    case SPECIAL_GUTTURAL_ROAR:
        return !(IsBasicDerivedFrom__FP5BASIC3CID(guard, CID_JEANBISON) || IsBasicDerivedFrom__FP5BASIC3CID(guard, CID_BEAR) || IsBasicDerivedFrom__FP5BASIC3CID(guard, CID_BEARCUB));
    case SPECIAL_INSANITY_STRIKE:
    case SPECIAL_RAGE_BOMB:
        return !IsStepguardRageImmune(guard);
    case SPECIAL_GADGET_0B:
        return !(IsBasicDerivedFrom__FP5BASIC3CID(guard, CID_DIMITRI) || IsBasicDerivedFrom__FP5BASIC3CID(guard, CID_RAJAN) || IsBasicDerivedFrom__FP5BASIC3CID(guard, CID_CONTESSA) ||
                 IsBasicDerivedFrom__FP5BASIC3CID(guard, CID_JEANBISON) || IsBasicDerivedFrom__FP5BASIC3CID(guard, CID_ARPEGGIO) || IsBasicDerivedFrom__FP5BASIC3CID(guard, CID_CARMELITA) ||
                 IsBasicDerivedFrom__FP5BASIC3CID(guard, CID_NEELA) || IsBasicDerivedFrom__FP5BASIC3CID(guard, CID_BEAR) || IsBasicDerivedFrom__FP5BASIC3CID(guard, CID_BEARCUB));
    }
    return 0;
}
} // namespace M_func_00206EB0

namespace RO_ORDER { extern const float r_D_002A8698 __asm__("D_002A8698") = 3.40282347e+38f; }
// 0x207030 SetStepguardSpecial
namespace M_SetStepguardSpecial {
extern qword D_002A2DE0 __asm__("D_002A2DE0");
extern qword D_002C3D30 __asm__("D_002C3D30");
extern float D_002A8698 __asm__("D_002A8698");
int CanSetStepguardSpecial(STEPGUARD *p, int k) __asm__("CanSetStepguardSpecial");
void SetActPositionGoal(void *p, qword q) __asm__("SetActPositionGoal");
void RemoveStepguardPreferClass(STEPGUARD *p, int n) __asm__("RemoveStepguardPreferClass");
void RemoveStepguardRageIgnoreClasses(STEPGUARD *p) __asm__("RemoveStepguardRageIgnoreClasses");
void AddStepguardPreferClass(STEPGUARD *p, int n) __asm__("AddStepguardPreferClass");
void AddStepguardRageIgnoreClasses(STEPGUARD *p) __asm__("AddStepguardRageIgnoreClasses");
ASEGA *GetAnimBindingInstance(void *p) __asm__("GetAnimBindingInstance");
void SetAsegaMasterSpeed(ASEGA *y, float f) __asm__("SetAsegaMasterSpeed");
LO *GetHeroPo(void *p) __asm__("GetHeroPo");
void ResumeBhvguard(BHV156 *bhv) __asm__("ResumeBhvguard");

// 0x207030: Sets the gadget/status special state (+0xFBC, time +0xFC0): 8/0xD shrink, 0xE time-frozen, 0x22/0x25
// rage (attack other guards), 0x15 and 0xB others; leaving a state undoes it.
// Also known as: SetGuardMode (upstream readable / Sly 1 name; SetGuardMode__FP5GUARDi).
extern "C" void SetStepguardSpecial(STEPGUARD *guard, int special)
{
    int old;
    if (special == guard->specialState)
        return;
    if (!CanSetStepguardSpecial(guard, special))
        return;
    old = guard->specialState;
    guard->specialState = special;
    guard->specialStartTime = g_clock.time;
    switch (old) {
    case SPECIAL_SIZE_DESTABILIZER:
    case SPECIAL_REDUCTION_BOMB:
        if (guard->scale)
            SetActPositionGoal(guard->scale, D_002A2DE0);
        break;
    case SPECIAL_INSANITY_STRIKE:
    case SPECIAL_RAGE_BOMB:
        RemoveStepguardPreferClass(guard, CID_STEPGUARD);
        RemoveStepguardRageIgnoreClasses(guard);
        break;
    case SPECIAL_TEMPORAL_LOCK: {
        BHVANIM *bhv = (__typeof__(bhv))FindStepBehavior(guard, CID_BHVANIM);
        if (bhv) {
            ASEGA *asega = GetAnimBindingInstance(bhv->binding);
            if (asega)
                SetAsegaMasterSpeed(asega, asega->animation->masterSpeed);
        }
        break;
    }
    case SPECIAL_GUTTURAL_ROAR: {
        BHV156 *bhv = (__typeof__(bhv))FindStepBehavior(guard, CID_BHVGUARD);
        if (bhv && bhv->mode == 0x12 && bhv->modeObject == GetHeroPo(&g_sw) && bhv->modeAmount == D_002A8698)
            ResumeBhvguard(bhv);
        break;
    }
    }
    switch (special) {
    case SPECIAL_SIZE_DESTABILIZER:
    case SPECIAL_REDUCTION_BOMB:
        if (guard->scale)
            SetActPositionGoal(guard->scale, D_002C3D30);
        break;
    case SPECIAL_INSANITY_STRIKE:
    case SPECIAL_RAGE_BOMB:
        AddStepguardPreferClass(guard, CID_STEPGUARD);
        AddStepguardRageIgnoreClasses(guard);
        break;
    case SPECIAL_TEMPORAL_LOCK: {
        BHVANIM *bhv = (__typeof__(bhv))FindStepBehavior(guard, CID_BHVANIM);
        if (bhv) {
            ASEGA *asega = GetAnimBindingInstance(bhv->binding);
            if (asega)
                SetAsegaMasterSpeed(asega, 0.0f);
        }
        {
            VECTOR *velocity = &guard->vecLocalVelocity;
            velocity->z = 0;
            velocity->y = 0;
            velocity->x = 0;
        }
        break;
    }
    case SPECIAL_GUTTURAL_ROAR: {
        BHV156 *bhv = (__typeof__(bhv))FindStepBehavior(guard, CID_BHVGUARD);
        if (bhv) {
            switch (bhv->mode) {
            case 1: case 2: case 3: case 4:
            case 0xC: case 0x10: case 0x11: case 0x12:
                ResumeBhvguard(bhv);
                break;
            }
        }
        break;
    }
    }
}
} // namespace M_SetStepguardSpecial

// 0x207288 UpdateStepguardSpecial
namespace M_UpdateStepguardSpecial {

extern float D_002C3D40[3] __asm__("D_002C3D40");    /* special-state durations: 10 s shrink, 30 s rage, 5 s roar */
void SetStepguardSpecial(STEPGUARD *p, int n) __asm__("SetStepguardSpecial");
// 0x207288: Ends the special state after its duration (10 s shrink, 30 s rage, 5 s state 0x15).
// Also known as: UpdateGuardTimedMode (upstream readable / Sly 1 name; UpdateGuardTimedMode__FP5GUARD).
extern "C" void UpdateStepguardSpecial(STEPGUARD *guard)
{
    switch (guard->specialState) {
    case SPECIAL_SIZE_DESTABILIZER:
    case SPECIAL_REDUCTION_BOMB:
        if (g_clock.time - guard->specialStartTime > D_002C3D40[0])
            SetStepguardSpecial(guard, SPECIAL_NONE);
        break;
    case SPECIAL_INSANITY_STRIKE:
    case SPECIAL_RAGE_BOMB:
        if (g_clock.time - guard->specialStartTime > D_002C3D40[1])
            SetStepguardSpecial(guard, SPECIAL_NONE);
        break;
    case SPECIAL_GUTTURAL_ROAR:
        if (g_clock.time - guard->specialStartTime > D_002C3D40[2])
            SetStepguardSpecial(guard, SPECIAL_NONE);
        break;
    }
}
} // namespace M_UpdateStepguardSpecial

// 0x207350 UpdateStepguard
namespace M_UpdateStepguard {
extern qword D_002A2DD0 __asm__("D_002A2DD0");
void UpdateStepguardSpecial(STEPGUARD *guard) __asm__("UpdateStepguardSpecial");
void UpdateStepguardDetectTargets(STEPGUARD *guard) __asm__("UpdateStepguardDetectTargets");
int IsAnimBindingDone(ANIMBINDING *binding) __asm__("IsAnimBindingDone");
void SetStepguardAttack(STEPGUARD *guard, GUARDATTACK *attack, int aimTarget, qword aimOffset) __asm__("SetStepguardAttack");
void UpdateStepguardAim(STEPGUARD *guard) __asm__("UpdateStepguardAim");
void UpdateStuckWatch(void *p) __asm__("UpdateStuckWatch");
void UpdateHazardWatch(void *p) __asm__("UpdateHazardWatch");
void UpdateBhvstk(BHVSTK *stack) __asm__("UpdateBhvstk");
void UpdateStepguardMover(GUARDMOVER *mover) __asm__("UpdateStepguardMover");
void StageStepguardLoot(STEPGUARD *guard) __asm__("StageStepguardLoot");
int IsAsegaRetracted(ASEGA *pasega) __asm__("IsAsegaRetracted");
void DropAsegaRef(ASEGA **ppasega) __asm__("DropAsegaRef");
}

// 0x207350: STEPGUARD vtable slot 16 (Update): step update; while in the world and not time-frozen: special-state
// timeouts, target detection, attack animation, aim, pickpocket, components, the behaviour stack, the mover and
// finished animations; guards that track a relocation point (named guards, inferred) record their last grounded
// position there for the relocate behaviour.
// Also known as: UpdateStepguard (upstream readable / Sly 1 name; UpdateStepguard__FP9STEPGUARDf).
void STEPGUARD::Update()
{
    using namespace M_UpdateStepguard;
    STEPGUARD *guard = this;
    STEP::Update();
    if (((int)guard->isInWorld & 1) == 0)
        return;
    if (guard->specialState == SPECIAL_TEMPORAL_LOCK)
        return;
    UpdateStepguardSpecial(guard);
    UpdateStepguardDetectTargets(guard);
    if (guard->currentAttack && IsAnimBindingDone(guard->currentAttack->activeBinding))
        SetStepguardAttack(guard, 0, 0, D_002A2DD0);
    if (guard->aimAct)
        UpdateStepguardAim(guard);
    if (guard->isLootStaged)
        StageStepguardLoot(guard);
    UpdateStuckWatch(guard->stuckWatch);
    UpdateHazardWatch(guard->hazardWatch);
    UpdateBhvstk(&guard->behaviors);
    UpdateStepguardMover(&guard->mover);
    if (guard->heldAnimation && IsAsegaRetracted(guard->heldAnimation))
        DropAsegaRef(&guard->heldAnimation);
    if (guard->tracksRelocatePoint && guard->floorContact)
        guard->relocatePoint = *guard->probeBottomPos;
}

// 0x207468 TakeStepguardDamage
namespace M_FTakeStepguardDamage {
int IsBasicDerivedFrom__FP5BASIC3CID(LO *object, int cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
int CanSoTakeDamage(STEPGUARD *guard, ZPR *zap) __asm__("CanSoTakeDamage");
void HandleBhvstkMessage(BHVSTK *stack, int message, ZPR *zap) __asm__("HandleBhvstkMessage");
}
// 0x207468: STEPGUARD vtable slot 75 (take damage): unless the zap is lethal, ignores it when invulnerable, from
// its own team's bomb (cid 0x4F) or rejected by the base; then forwards it to the behaviour stack (message 0xC).
// Also known as: FAcceptContactGuard (upstream readable / Sly 1 name; FAcceptContactGuard__FP3ALOP5CTMSG).
int STEPGUARD::TakeDamage(ZPR *zap)
{
    using namespace M_FTakeStepguardDamage;
    STEPGUARD *guard = this;
    if (!guard->IsZapLethal(zap)) {
        if (guard->IsInvulnerable((ZPK)zap->zpk))
            return 1;
        if (zap->ploSource && IsBasicDerivedFrom__FP5BASIC3CID(zap->ploSource, CID_BOMB) && ((BOMB *)zap->ploSource)->thrower->root == guard->root)
            return 1;
        if (!CanSoTakeDamage(guard, zap))
            return 0;
    }
    HandleBhvstkMessage(&guard->behaviors, 0xC, zap);
    return 1;
}

// 0x207530 IsStepguardZapLethal
// 0x207530: STEPGUARD vtable slot 86: true if the zap's damage reaches the maximum hit points and its kind is not
// one of the harmless kinds (0, 1, 0x80000, 0x100000).
// Also known as: FGuardHitMatters (upstream readable / Sly 1 name; FGuardHitMatters__FP5GUARDP5CTMSG).
int STEPGUARD::IsZapLethal(ZPR *zap)
{
    STEPGUARD *guard = this;
    if (-zap->dhp < guard->maxHitPoints)
        return 0;
    switch (zap->zpk) {
    case 0:
    case 1:
    case 0x80000:
    case 0x100000:
        return 0;
    }
    return 1;
}

// 0x207598 IsStepguardInvulnerable
namespace M_FInvulnerableStepguard {

// 0x207598: STEPGUARD vtable slot 76: true while invulnerable (+0x32C or +0x298 set, or flag 0x2A).
extern "C" int IsStepguardInvulnerable(STEPGUARD *guard)
{
    if (guard->isInvulnerable32C || guard->isInvulnerable)
        return 1;
    return StepguardHasSgf(guard, 0x2A) != 0;
}
} // namespace M_FInvulnerableStepguard

// 0x2075D0 HandleStepguardMessage
namespace M_HandleStepguardMessage {
struct OBJ; struct ATTACHMSG;
struct OBJ : ALO { char pad[0x54 - 0x50]; void *transforms; void *positionXf; void *rotationXf; void *rootTransform; void *unknown64; void *unknown68; char pad6C[0x70 - 0x6C]; void *ploRemovedFirst; char pad74[0x80 - 0x74]; void *unknown80; char pad84[0xA8 - 0x84]; void *unknownA8; char padAC[0xB0 - 0xAC]; void *unknownB0; char padB4[0xBC - 0xB4]; void *unknownBC; void *unknownC0; void *unknownC4; void *unknownC8; char padCC[0xD8 - 0xCC]; void *unknownD8; void *unknownDC; char padE0[0xE4 - 0xE0]; void *parts; void *physics; void *physicsTree; char padF0[0x134 - 0xF0]; void *psobounds; void *pxa; char pad13C[0x144 - 0x13C]; void *prender; char pad148[0x184 - 0x148]; int isHazard; /* (inferred) */ };   /* the attached object (an SO; the hazard volume's zap amount) */
struct ATTACHMSG { struct OBJ *object; void *attachedTo; };
void HandlePoMessage(STEPGUARD *p, int message, struct ATTACHMSG *arg) __asm__("HandlePoMessage");
void SetHazardWatchTarget(HAZARDWATCH *watch, struct OBJ *target) __asm__("SetHazardWatchTarget");
void HandleBhvstkMessage(void *stack, int message, struct ATTACHMSG *arg) __asm__("HandleBhvstkMessage");
// 0x2075D0: STEPGUARD vtable slot 5 (HandleMessage): PO handler; messages 4/5 (an object attached to / detached from the
// guard) point the hazard watch (+0x49C) at an attached hazard or clear it; every message is then offered to the behaviour stack.
// Also known as: HandleStepguardMessage (upstream readable / Sly 1 name; HandleStepguardMessage__FP9STEPGUARD5MSGIDPv).
extern "C" void HandleStepguardMessage(STEPGUARD *guard, int message, struct ATTACHMSG *arg)
{
    HandlePoMessage(guard, message, arg);
    switch (message) {
    case 4:
        if (arg->attachedTo == guard && arg->object->isHazard)
            SetHazardWatchTarget(guard->hazardWatch, arg->object);
        break;
    case 5:
        if (arg->attachedTo == guard && arg->object == (struct OBJ *)guard->hazardWatch->volume)
            SetHazardWatchTarget(guard->hazardWatch, 0);
        break;
    }
    HandleBhvstkMessage((&guard->behaviors), message, arg);
}
} // namespace M_HandleStepguardMessage

// 0x207690 RenderStepguardSelf
namespace M_RenderStepguardSelf {
 struct M4; struct PROFILE; 

// 4x4 matrix of U rows with copy constructor and assignment.
struct M4 {QWROW rows[4];M4(){}M4(const M4 &m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];rows[3]=m.rows[3];}M4 &operator=(const M4 &m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];rows[3]=m.rows[3];return *this;}};
/* The guard is read as STEPGUARD: LO::visibilityMask, SO::rootTransform (its world matrix rows), SO::model,
   SO::opacity / psobounds, STEP::vecPivotOffset, STEPGUARD::pulseStartTime; the camera policy is CM::tunnelPolicy. */
struct PROFILE {float a,b,c,unused,min,max;};
struct EFFECT;


extern float D_002C3D4C __asm__("D_002C3D4C");
extern float D_002C3D50[3] __asm__("D_002C3D50"),D_002C3D60[2] __asm__("D_002C3D60"),g_lmZeroOne[2] __asm__("g_lmZeroOne");

void RefreshSoBoundsRecord(STEPGUARD *) __asm__("RefreshSoBoundsRecord");
int IsCameraPolicyActive__FP5CPLCY(void *) __asm__("IsCameraPolicyActive__FP5CPLCY");
void RenderSoGlobset(EFFECT *,const M4 *,void *,float,float) __asm__("RenderSoGlobset");
void *func_002333B4(void *,int,unsigned int) __asm__("memset");
qword GetLoWorldPos(STEPGUARD *) __asm__("GetLoWorldPos");
float sinf(float) __asm__("sinf");
qword *VECTOR_opAssign(qword *,qword) __asm__("VECTOR_opAssign");
M4 *__ml__FRC7MATRIX4T0(M4 *,const M4 *,const M4 *) SLY2_SRET_LABEL("__ml__FRC7MATRIX4T0", "MulMatrix4Into");
M4 *InvertMatrix4Rigid(M4 *,const M4 *) SLY2_SRET_LABEL("InvertMatrix4Rigid", "InvertMatrix4RigidInto");

// VU0 helper: vector difference.
// VU0 helper: length of a vector.
static inline float Length(qword a)
{
#ifdef SLY2_EE_ASM
    qword one;
    float f;
    __asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "+j"(a), "=&j"(one));
    __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "+j"(a));
    __asm__("mtc1 %1,%0" : "=f"(f) : "r"(a));
    return f;
#else
    VuLanes p = VuGet(a);
    return VuSqrt((p.x * p.x + p.y * p.y) + p.z * p.z);
#endif
}
// 0x207690: STEPGUARD vtable slot 19 (render): draws the guard; while the camera policy is active, applies a 0.85 s
// squash/stretch pulse and a distance-based scale.
// Also known as: RenderStepguardSelf (upstream readable / Sly 1 name; RenderStepguardSelf__FP9STEPGUARDP2CMP2RO).
extern "C" void RenderStepguardSelf(STEPGUARD *guard)
{
    M4 local;
    QWROW offset;
    M4 basis,matrix,product,inverse;
    CM *camera=&g_cm;
    if(guard->visibilityMask&camera->grfvis){
        RefreshSoBoundsRecord(guard);
        if(!IsCameraPolicyActive__FP5CPLCY(&camera->tunnelPolicy)){
            RenderSoGlobset((EFFECT *)&guard->model,0,guard->psobounds,guard->opacity,guard->opacity);
        }else{
            func_002333B4(&local,0,sizeof(M4));
            local.rows[3].f[3]=1.0f;
            local.rows[2].f[2]=1.0f;
            local.rows[1].f[1]=1.0f;
            local.rows[0].f[0]=1.0f;
            float distance=Length(VuSub(GetLoWorldPos(guard),*(qword *)&camera->pos));
            PROFILE *profile=&(*(PROFILE *)&g_sw.clqGuardScale);
            float gain=profile->a+distance*(profile->b+distance*profile->c);
            float min=profile->min,max=profile->max;
            if(gain<min)gain=min;else if(max<gain)gain=max;
            float elapsed=g_clock.time-guard->pulseStartTime;
            float along=gain,across=gain;
            if(elapsed<0.85f){
                float wave=sinf(elapsed*D_002C3D4C);
                float u=elapsed*1.1764705f;
                float mix=(wave+1.0f)*0.5f;
                float magnitude=D_002C3D50[0]+u*(D_002C3D50[1]+u*D_002C3D50[2]);
                float blend=(1.0f-mix)*D_002C3D60[0]+mix*D_002C3D60[1];
                float min=g_lmZeroOne[0],max=g_lmZeroOne[1];
                if(magnitude<min)magnitude=min;else if(max<magnitude)magnitude=max;
                float strength=blend*magnitude;
                across=1.0f-strength;
                along=1.0f+strength;
            }
            local.rows[0].f[0]=across;
            local.rows[1].f[1]=across;
            local.rows[2].f[2]=along;
            offset.f[0]=(1.0f-across)*guard->vecPivotOffset.x;
            offset.f[2]=(1.0f-along)*guard->vecPivotOffset.z;
            offset.f[1]=(1.0f-across)*guard->vecPivotOffset.y;
            VECTOR_opAssign(&local.rows[3].q,offset.q);
            local.rows[3].f[3]=1.0f;
            M4 *productPointer=&product;
            M4 *basisPointer=&basis;
            M4 *matrixPointer=&matrix;
            basis.rows[0]=guard->rootTransform->worldMatq.r[0];
            basis.rows[1]=guard->rootTransform->worldMatq.r[1];
            basis.rows[2]=guard->rootTransform->worldMatq.r[2];
            basis.rows[3]=guard->rootTransform->worldMatq.r[3];
            __ml__FRC7MATRIX4T0(productPointer,basisPointer,&local);
            M4 *inversePointer=&inverse;
            InvertMatrix4Rigid(inversePointer,basisPointer);
            __ml__FRC7MATRIX4T0(matrixPointer,productPointer,inversePointer);
            RenderSoGlobset((EFFECT *)&guard->model,matrixPointer,guard->psobounds,guard->opacity,guard->opacity);
        }
    }
}
} // namespace M_RenderStepguardSelf

// 0x2079A0 OnStepguardRemove
namespace M_OnStepguardRemove {

void OnSoRemove(STEPGUARD *p) __asm__("OnSoRemove");
void RemoveDlEntry__FP2DLPv(struct DL *pdl, void *pv) __asm__("RemoveDlEntry__FP2DLPv");
// 0x2079A0: STEPGUARD vtable slot 22 (OnRemove): base, unlinks from the world's guard list and stamps the removal
// time (+0xFF8).
// Also known as: OnGuardUnregistered (upstream readable / Sly 1 name; OnGuardUnregistered__FP2LO).
extern "C" void OnStepguardRemove(STEPGUARD *guard)
{
    OnSoRemove(guard);
    RemoveDlEntry__FP2DLPv((DL *)&g_sw.dlStepguard, guard);
    guard->removalTime = g_clock.time;
}
} // namespace M_OnStepguardRemove

// 0x2079E0 OnStepguardAdd
namespace M_OnStepguardAdd {
struct DL; 
extern float D_002A8868 __asm__("D_002A8868");
struct DL { void *head, *tail; int count; };
void OnSoAdd(STEPGUARD *p) __asm__("OnSoAdd");
void AppendDlEntry__FP2DLPv(struct DL *pdl, void *pv) __asm__("AppendDlEntry__FP2DLPv");
// 0x2079E0: STEPGUARD vtable slot 23 (OnAdd): base, links on the world's guard list, removal time FLT_MAX.
// Also known as: OnGuardRegistered (upstream readable / Sly 1 name; OnGuardRegistered__FP2LO).
extern "C" void OnStepguardAdd(STEPGUARD *guard)
{
    OnSoAdd(guard);
    AppendDlEntry__FP2DLPv((struct DL *)&g_sw.dlStepguard, guard);
    guard->removalTime = D_002A8868;
}
} // namespace M_OnStepguardAdd

// 0x207A20 GetSgfMask
namespace M_GrfFromSgf {
// 0x207A20: Returns the bit mask of guard flag `sgf` (0 for -1).
extern "C" unsigned long GetSgfMask(int n) { if (n == -1) return 0; return 1L << n; }
} // namespace M_GrfFromSgf

// 0x207A40 GetSgfArrayMask
namespace M_GrfFromSgfArray {
unsigned long GetSgfMask(int n) __asm__("GetSgfMask");
// 0x207A40: Returns the OR of the masks of an array of guard flags.
extern "C" unsigned long GetSgfArrayMask(int count, int *sgfs)
{
    unsigned long mask = 0;
    int i;
    for (i = 0; i < count; i++)
        mask |= GetSgfMask(sgfs[i]);
    return mask;
}
} // namespace M_GrfFromSgfArray

// 0x207AA0 ListSgfsInMask
namespace M_CsgfFromGrf {
unsigned long GetSgfMask(int n) __asm__("GetSgfMask");
// 0x207AA0: Lists the guard flags set in a mask (from 46 down to 0) into an array; returns the count.
extern "C" int ListSgfsInMask(unsigned long mask, int *sgfs)
{
    int count = 0;
    int sgf;
    for (sgf = 0x2E; sgf >= 0; sgf--)
        if (mask & GetSgfMask(sgf)) {
            *sgfs++ = sgf;
            count++;
        }
    return count;
}
} // namespace M_CsgfFromGrf

// 0x207B10 GetSgfMaskFromSpliceList
namespace M_func_00207B10 {
void SetValuesFromSpliceList(CRefData *ref, void *list, int type, int capacity, int *count, int *values) SLY2_SRET_LABEL("SetValuesFromSpliceList", "SetValuesFromSpliceListInto");
unsigned long GetSgfArrayMask(int count, int *sgfs) __asm__("GetSgfArrayMask");
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
// 0x207B10: Converts a Splice list of guard-flag enums (type 0x1010) to a mask.
// Also known as: MaskOfList (upstream readable / Sly 1 name; MaskOfList__FPv).
unsigned long GetSgfMaskFromSpliceList(void *list) __asm__("GetSgfMaskFromSpliceList"); unsigned long GetSgfMaskFromSpliceList(void *list)
{
    int sgfs[16];
    CRefData ref;
    int count;
    unsigned long mask;
    SetValuesFromSpliceList(&ref, list, 0x1010, 0x10, &count, sgfs);
    mask = GetSgfArrayMask(count, sgfs);
    Decref__4CRef(&ref);
    return mask;
}
} // namespace M_func_00207B10

// 0x207B70 SetStepguardAnimBinding
// 0x207B70: Splice option: sets the animation (oid, argument) bound to a guard-flag set, creating the binding if
// needed.
// Also known as: SetGuardKinde (upstream readable / Sly 1 name; SetGuardKinde__FP5GUARD3OIDii).
extern "C" void SetStepguardAnimBinding(STEPGUARD *guard, void *sgfList, OID oid, int flags)
{
    ANIMBINDING *binding;
    binding = EnsureAnimBinding(guard, GetSgfMaskFromSpliceList(sgfList));
    binding->oid = oid;
    binding->flags = flags;
}

// 0x207BC8 SetAndResolveStepguardAnimBinding
// 0x207BC8: SetStepguardAnimBinding, then resolves the binding.
extern "C" void SetAndResolveStepguardAnimBinding(STEPGUARD *guard, void *sgfList, OID oid, int flags)
{
    ANIMBINDING *binding;
    binding = EnsureAnimBinding(guard, GetSgfMaskFromSpliceList(sgfList));
    SetStepguardAnimBinding(guard, sgfList, oid, flags);
    ResolveAnimBinding(binding, guard);
}

// 0x207C48 SetStepguardAnimBindingKind3
namespace M_func_00207C48 {
long GetSgfMaskFromSpliceList(void *list) __asm__("GetSgfMaskFromSpliceList");
DIRBL *AllocDirAnim(int cb) __asm__("AllocDirAnim");
void InvalidateDirAnims(DIRBL *payload) __asm__("InvalidateDirAnims");
// 0x207C48: Splice option: binds a flag set to a directional animation set (a new sly2/sw.h DIRBL: four animations by oid, which InitDirAnims resolves).
// Also known as: SetGuardKind3 (upstream readable / Sly 1 name; SetGuardKind3__FP5GUARDiiiii).
extern "C" void SetStepguardAnimBindingKind3(STEPGUARD *guard, void *sgfList, int anim0, int anim1, int anim2, int anim3)
{
    ANIMBINDING *binding = EnsureAnimBinding(guard, GetSgfMaskFromSpliceList(sgfList));
    binding->kind = BINDING_DIRANIM;
    binding->payload4 = AllocDirAnim(sizeof(DIRBL));
    InvalidateDirAnims((DIRBL *)binding->payload4);
    ((DIRBL *)binding->payload4)->ae[0].animation = (ASEG *)anim0;   /* an oid until InitDirAnims */
    ((DIRBL *)binding->payload4)->ae[1].animation = (ASEG *)anim1;   /* an oid until InitDirAnims */
    ((DIRBL *)binding->payload4)->ae[2].animation = (ASEG *)anim2;   /* an oid until InitDirAnims */
    ((DIRBL *)binding->payload4)->ae[3].animation = (ASEG *)anim3;   /* an oid until InitDirAnims */
    binding->flags = 8;
}
} // namespace M_func_00207C48

// 0x207CF8 SetAndResolveStepguardAnimBindingKind3
namespace M_func_00207CF8 {
long GetSgfMaskFromSpliceList(void *list) __asm__("GetSgfMaskFromSpliceList");
void SetStepguardAnimBindingKind3(STEPGUARD *guard, void *sgfList, int anim0, int anim1, int anim2, int anim3) __asm__("SetStepguardAnimBindingKind3");
// 0x207CF8: SetStepguardAnimBindingKind3, then resolves the binding.
extern "C" void SetAndResolveStepguardAnimBindingKind3(STEPGUARD *guard, void *sgfList, int anim0, int anim1, int anim2, int anim3)
{
    ANIMBINDING *binding = EnsureAnimBinding(guard, GetSgfMaskFromSpliceList(sgfList));
    SetStepguardAnimBindingKind3(guard, sgfList, anim0, anim1, anim2, anim3);
    ResolveAnimBinding(binding, guard);
}
} // namespace M_func_00207CF8

// 0x207D98 SetStepguardAnimBindingKind2
namespace M_func_00207D98 {
struct PAYLOAD2;
struct PAYLOAD2 { int parameter; };
unsigned long GetSgfMaskFromSpliceList(void *list) __asm__("GetSgfMaskFromSpliceList");
struct PAYLOAD2 *AllocLeap(int cb) __asm__("AllocLeap");
void InitAsegArc(struct PAYLOAD2 *payload) __asm__("InitAsegArc");
// 0x207D98: Splice option: binds a flag set to a LEAP payload (a new 0x40-byte object with one parameter).
extern "C" void SetStepguardAnimBindingKind2(STEPGUARD *guard, void *sgfList, int parameter)
{
    ANIMBINDING *binding = EnsureAnimBinding(guard, GetSgfMaskFromSpliceList(sgfList));
    binding->kind = BINDING_LEAP;
    binding->payload4 = AllocLeap(0x40);
    InitAsegArc((PAYLOAD2 *)binding->payload4);
    ((PAYLOAD2 *)binding->payload4)->parameter = parameter;
    binding->flags = 0;
}
} // namespace M_func_00207D98

// 0x207E08 SetAndResolveStepguardAnimBindingKind2
namespace M_func_00207E08 {
long GetSgfMaskFromSpliceList(void *list) __asm__("GetSgfMaskFromSpliceList");
void SetStepguardAnimBindingKind2(STEPGUARD *guard, void *sgfList, int b, float c, float d, float e, float f) __asm__("SetStepguardAnimBindingKind2");
// 0x207E08: SetStepguardAnimBindingKind2, then resolves the binding.
extern "C" void SetAndResolveStepguardAnimBindingKind2(STEPGUARD *guard, void *sgfList, int b, float c, float d, float e, float f)
{
    ANIMBINDING *binding = EnsureAnimBinding(guard, GetSgfMaskFromSpliceList(sgfList));
    SetStepguardAnimBindingKind2(guard, sgfList, b, c, d, e, f);
    ResolveAnimBinding(binding, guard);
}
} // namespace M_func_00207E08

// 0x207EB8 SetZapReactionBindings
namespace M_func_00207EB8 {
// 0x207EB8: Sets the binding (oid, flags) of every zap kind in `mask` (26 kinds) in a zap-reaction table.
// Also known as: SetArrayKindes (upstream readable / Sly 1 name; SetArrayKindes__FPvUi3OIDiT0).
extern "C" void SetZapReactionBindings(STEPGUARD *guard, int zapMask, int oid, int flags, ANIMBINDING *table)
{
    int i;
    int bit = 1;
    for (i = 25; i >= 0; i--) {
        if (zapMask & bit) {
            ReleaseAnimBinding(table, guard);
            table->oid = (OID)oid;
            table->flags = flags;
        }
        bit <<= 1;
        table++;
    }
}
} // namespace M_func_00207EB8

// 0x207F50 SetAndResolveZapReactionBindings
namespace M_func_00207F50 {
// 0x207F50: SetZapReactionBindings, then resolves each binding.
extern "C" void SetAndResolveZapReactionBindings(STEPGUARD *guard, int zapMask, int oid, int flags, ANIMBINDING *table)
{
    int i;
    int bit = 1;
    for (i = 25; i >= 0; i--) {
        if (zapMask & bit) {
            ReleaseAnimBinding(table, guard);
            table->oid = (OID)oid;
            table->flags = flags;
            ResolveAnimBinding(table, guard);
        }
        bit <<= 1;
        table++;
    }
}
} // namespace M_func_00207F50

// 0x207FF8 SetStepguardKoReactions
namespace M_func_00207FF8 {
void SetZapReactionBindings(STEPGUARD *guard, int zapMask, int oid, int flags, ANIMBINDING *table) __asm__("SetZapReactionBindings");
// 0x207FF8: Splice option: sets KO reactions (table +0xC10, used by the KO behaviour).
extern "C" void SetStepguardKoReactions(STEPGUARD *guard, int zapMask, int oid, int flags)
{
    SetZapReactionBindings(guard, zapMask, oid, flags, guard->koReactions);
}
} // namespace M_func_00207FF8

// 0x208018 SetAndResolveStepguardKoReactions
namespace M_func_00208018 {
void SetAndResolveZapReactionBindings(STEPGUARD *guard, int zapMask, int oid, int flags, ANIMBINDING *table) __asm__("SetAndResolveZapReactionBindings");
// 0x208018: Splice option: sets and resolves KO reactions.
extern "C" void SetAndResolveStepguardKoReactions(STEPGUARD *guard, int zapMask, int oid, int flags)
{
    SetAndResolveZapReactionBindings(guard, zapMask, oid, flags, guard->koReactions);
}
} // namespace M_func_00208018

// 0x208038 SetStepguardHurtReactions
namespace M_func_00208038 {
void SetZapReactionBindings(STEPGUARD *guard, int zapMask, int oid, int flags, ANIMBINDING *table) __asm__("SetZapReactionBindings");
// 0x208038: Splice option: sets hurt reactions (table +0xA70).
extern "C" void SetStepguardHurtReactions(STEPGUARD *guard, int zapMask, int oid, int flags)
{
    SetZapReactionBindings(guard, zapMask, oid, flags, guard->hurtReactions);
}
} // namespace M_func_00208038

// 0x208058 SetAndResolveStepguardHurtReactions
namespace M_func_00208058 {
void SetAndResolveZapReactionBindings(STEPGUARD *guard, int zapMask, int oid, int flags, ANIMBINDING *table) __asm__("SetAndResolveZapReactionBindings");
// 0x208058: Splice option: sets and resolves hurt reactions.
extern "C" void SetAndResolveStepguardHurtReactions(STEPGUARD *guard, int zapMask, int oid, int flags)
{
    SetAndResolveZapReactionBindings(guard, zapMask, oid, flags, guard->hurtReactions);
}
} // namespace M_func_00208058

// 0x208078 SetStepguardKnockdownReactions
namespace M_func_00208078 {
void SetZapReactionBindings(STEPGUARD *guard, int zapMask, int oid, int flags, ANIMBINDING *table) __asm__("SetZapReactionBindings");
// 0x208078: Splice option: sets knockdown reactions (table +0x8D0).
extern "C" void SetStepguardKnockdownReactions(STEPGUARD *guard, int zapMask, int oid, int flags)
{
    SetZapReactionBindings(guard, zapMask, oid, flags, guard->knockdownReactions);
}
} // namespace M_func_00208078

// 0x208098: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x10");

// 0x2080A0 ResolveZapReactions
// 0x2080A0: Resolves all 26 entries of a zap-reaction table.
extern "C" void ResolveZapReactions(STEPGUARD *guard, ANIMBINDING *table)
{
    ANIMBINDING *binding;
    int i;
    binding = table;
    i = 25;
    do {
        ResolveAnimBinding(binding, guard);
        i -= 1;
        binding++;
    } while (i >= 0);
}

// 0x2080F0 ResolveStepguardAnimBindings
// 0x2080F0: Resolves all of the guard's animation bindings and its three zap-reaction tables.
extern "C" void ResolveStepguardAnimBindings(STEPGUARD *guard)
{
    int i;
    for (i = 0; i < guard->cbindingrec; i++)
        ResolveAnimBinding(&guard->abindingrec[i].binding, guard);
    ResolveZapReactions(guard, guard->koReactions);
    ResolveZapReactions(guard, guard->knockdownReactions);
    ResolveZapReactions(guard, guard->hurtReactions);
}

// 0x208178 ReleaseZapReactions
// 0x208178: Releases all 26 entries of a zap-reaction table.
extern "C" void ReleaseZapReactions(STEPGUARD *guard, ANIMBINDING *table)
{
    ANIMBINDING *binding = table;
    int i;
    for (i = 25; i >= 0; i--) {   // 26 zap kinds
        ReleaseAnimBinding(binding, guard);
        binding++;
    }
}

// 0x2081C8 ReleaseStepguardZapReactions
// 0x2081C8: Releases the guard's three zap-reaction tables.
extern "C" void ReleaseStepguardZapReactions(STEPGUARD *guard)
{
    ReleaseZapReactions(guard, guard->koReactions);
    ReleaseZapReactions(guard, guard->knockdownReactions);
    ReleaseZapReactions(guard, guard->hurtReactions);
}

// 0x208208 CloneZapReactions
// 0x208208: Deep-copies all 26 entries of a zap-reaction table.
extern "C" void CloneZapReactions(STEPGUARD *guard, ANIMBINDING *table)
{
    int i;
    for (i = 25; i >= 0; i--) {
        CloneAnimBinding(table);
        table++;
    }
}

// 0x208250 CloneStepguardAnimBindings
// 0x208250: Deep-copies the animation bindings and zap-reaction tables after a clone.
extern "C" void CloneStepguardAnimBindings(STEPGUARD *guard)
{
    int i;
    for (i = 0; i < guard->cbindingrec; i++)
        CloneAnimBinding(&guard->abindingrec[i].binding);
    CloneZapReactions(guard, guard->koReactions);
    CloneZapReactions(guard, guard->knockdownReactions);
    CloneZapReactions(guard, guard->hurtReactions);
}

// 0x2082D0 ChooseStepguardTarget
namespace M_PloChooseStepguardTarget {
union U;
// Choose an eligible actor using lock state, priority and weighted distance.
union U {qword q;float f[4];};
extern float D_002A886C __asm__("D_002A886C"),D_002C3D70[3] __asm__("D_002C3D70"),D_002C3D80[3] __asm__("D_002C3D80");
extern qword GetLoWorldPos(void *) __asm__("GetLoWorldPos");
extern int IsBasicDerivedFrom__FP5BASIC3CID(LO *,int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
extern int IsStepguardIgnoreClass(STEPGUARD *,LO *) __asm__("IsStepguardIgnoreClass");
extern int IsStepguardForceClass(STEPGUARD *,LO *) __asm__("IsStepguardForceClass");
extern int IsStepguardPreferClass(STEPGUARD *,LO *) __asm__("IsStepguardPreferClass");
// VU0 helper: vector difference.
static inline qword Sub(const qword &a, qword b)
{
    qword d, bv, av;
    __asm__("lqc2 %2,%4\n\tqmtc2.ni %3,%1\n\tvsub.xyzw %0,%2,%1" : "=&j"(d), "=&j"(bv), "=&j"(av) : "r"(b), "m"(a));
    return d;
}
// VU0 helper: length of a vector.
static inline float Length(qword d)
{
    qword one;
    float f;
    __asm__ __volatile__("vmul.xyz %0,%0,%0" : "+j"(d));
    __asm__ __volatile__("vaddw.x %0,$vf0,$vf0w" : "=j"(one));
    __asm__("vadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "+j"(d) : "j"(one));
    __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "+j"(d));
    __asm__("mtc1 %1,%0" : "=f"(f) : "r"(d));
    return f;
}
// Quadratic weight k0 + d (k1 + d k2).
static inline float Weight(float distance,const float *k){return k[0]+distance*(k[1]+distance*k[2]);}
// 0x2082D0: STEPGUARD vtable slot 88 (choose target): keeps the forced target while it is in the world; else picks
// among the detector's targets, forced classes first, skipping ignored classes, preferring preferred classes, with
// hysteresis for the current target.
} // namespace M_PloChooseStepguardTarget
// Also known as: PloGuardTarget (upstream readable / Sly 1 name; PloGuardTarget__FP5GUARD).
LO *STEPGUARD::ChooseTarget()
{
    using namespace M_PloChooseStepguardTarget;
    STEPGUARD *guard = this;
    LO *best = 0;
    if (guard->forcedTarget && ((int)((LO *)guard->forcedTarget)->isInWorld & 1)) {
        best = guard->forcedTarget;   // keep the forced target while it is in the world
        goto done;               // (single exit: a direct return does not match)
    }
    {
        float bestDistance = D_002A886C;
        U origin;
        origin.q = GetLoWorldPos(guard);
        for (int i = 0; i < guard->pdetector->ctarget; i++) {
            DETECTORTARGET *entry = &guard->pdetector->atarget[i];
            LO *candidate = entry->object;
            if (!candidate)
                continue;
            if (!entry->state
                && (!(guard->specialState == SPECIAL_INSANITY_STRIKE || guard->specialState == SPECIAL_RAGE_BOMB)
                    || !IsBasicDerivedFrom__FP5BASIC3CID(candidate, CID_STEPGUARD)))
                continue;
            if (IsStepguardIgnoreClass(guard, candidate))
                continue;
            if (IsStepguardForceClass(guard, candidate))
                return candidate;
            qword delta = Sub(origin.q, GetLoWorldPos(candidate));
            LO *currentTarget = guard->target;
            float distance = Length(delta);
            if (candidate == currentTarget)
                distance = Weight(distance, D_002C3D70);
            if ((guard->specialState == SPECIAL_INSANITY_STRIKE || guard->specialState == SPECIAL_RAGE_BOMB)
                && IsBasicDerivedFrom__FP5BASIC3CID(candidate, CID_STEPGUARD))
                distance = Weight(distance, D_002C3D80);
            if (IsStepguardPreferClass(guard, candidate)) {
                if (best && !IsStepguardPreferClass(guard, best)) {
                    // the first preferred candidate beats any non-preferred one, whatever the distance
                    bestDistance = distance;
                    best = candidate;
                    continue;
                }
            } else if (best && IsStepguardPreferClass(guard, best))
                continue;
            if (distance < bestDistance) {
                bestDistance = distance;
                best = candidate;
            }
        }
    }
done:
    return best;
}

// 0x208538 SetStepguardForcedTarget
// 0x208538: Splice setter: sets the forced target (+0x390).
extern "C" void SetStepguardForcedTarget(STEPGUARD *guard, LO *n)
{
    guard->forcedTarget = n;
}

// 0x208540 SetStepguardTarget
// 0x208540: Changes the current target: leaves the old target's shared engagement record and joins (creating if
// needed) the new one's.
/* Sets the guard's target, moving the guard from the old target's record (EnsureTargetRecordById) to the new one's. */
// Also known as: SetGuardPursued (upstream readable / Sly 1 name; SetGuardPursued__FP5GUARDP2LO).
extern "C" void SetStepguardTarget(STEPGUARD *guard, LO *ploTarget)
{
    LO *ploOld;
    TARGETREC *record;
    ploOld = guard->target;
    if (ploTarget == ploOld)
        return;
    if (ploOld != 0) {
        RemoveTargetRecordGuard(guard->pvEngage, guard);
        guard->pvEngage = 0;
    }
    guard->target = ploTarget;
    if (ploTarget != 0) {
        record = EnsureTargetRecordById(ploTarget);
        guard->pvEngage = record;
        AddTargetRecordGuard(record, guard);
    }
}

// 0x2085B0 AddStepguardDetectTargetBack
// 0x2085B0: Adds a detector target in the generated (back) part of the list.
extern "C" void AddStepguardDetectTargetBack(STEPGUARD *guard, LO *object)
{
    TryAddStepguardDetectTarget(guard, object, 0);
}

// 0x2085D0 TryAddStepguardDetectTarget
namespace M_FAddStepguardDetectTarget {
struct DETECTORTARGETIsland;
// Placement new.
} inline void *operator new(unsigned int,void *storage){return storage;} namespace M_FAddStepguardDetectTarget {  // global (ISO C++: no operator new in a namespace)

/* A detector target entry (DETECTOR's DTE) and the detector's target list. */
/* island: placement-new constructor for the pinned insert */
struct DETECTORTARGETIsland {
    STEPGUARD *object;
    int state;
    float stateTime;
    int pad;
    // Event record for source, starting in state 0 at the clock's current time.
    DETECTORTARGETIsland(STEPGUARD *source, const CLOCK &clock) : object(source), state(0), stateTime(clock.time) {}
};
/* island: entry addresses spelled as byte offsets in pinned registers (asm-pinned, see the comment below) */

extern struct DETECTORTARGETIsland *FindDetectorTarget(struct DETECTOR *detector, STEPGUARD *object) __asm__("FindDetectorTarget");
extern void *memmove(void *d, void *s, int n) __asm__("memmove");

// 0x2085D0: Inserts an object into the detector's target list (at most eight): `atFront` appends it as a designated
// target; otherwise it goes before the generated tail, evicting the oldest generated entry when full.
// (The entry addresses are spelled as byte offsets into the detector, 0x100 = targets, in pinned registers: that
// is what matches.)
int TryAddStepguardDetectTarget(STEPGUARD *guard, STEPGUARD *object, int atFront) __asm__("TryAddStepguardDetectTarget"); int TryAddStepguardDetectTarget(STEPGUARD *guard, STEPGUARD *object, int atFront)
{
    struct DETECTORTARGETIsland *entry;
    struct DETECTOR *detector;
    register unsigned int n __asm__("$16");
    if (!object || object == guard)
        return 0;
    entry = 0;
    if (!atFront)
        entry = FindDetectorTarget(((struct DETECTOR *)guard->pdetector), object);
    if (entry)
        return 0;   // already tracked
    detector = ((struct DETECTOR *)guard->pdetector);
    n = detector->ctarget;
    if (n >= 8) {
        int numGenerated;
        if (!atFront) {
            numGenerated = guard->generatedTargetCount;
            if (numGenerated > 0) {
                // Full: drop the oldest generated entry (the last) and insert in front of the generated tail.
                guard->generatedTargetCount = numGenerated - 1;
                n = 8;
                n -= numGenerated;
                n *= 16;
                n += 0x100;
                entry = (struct DETECTORTARGETIsland *)((char *)detector + n);
                memmove(entry + 1, entry, (numGenerated - 1) * 16);
                new(entry) DETECTORTARGETIsland(object, g_clock);
                ((struct DETECTOR *)guard->pdetector)->ctarget++;
                return 1;
            }
        }
    } else {
        if (atFront) {
            register unsigned int offset __asm__("$2") = n * 16 + 0x100;
            entry = (struct DETECTORTARGETIsland *)((char *)detector + offset);
        } else {
            int numGenerated = guard->generatedTargetCount;
            n -= numGenerated;
            n *= 16;
            n += 0x100;
            entry = (struct DETECTORTARGETIsland *)((char *)detector + n);
            memmove(entry + 1, entry, numGenerated * 16);
        }
        new(entry) DETECTORTARGETIsland(object, g_clock);
        ((struct DETECTOR *)guard->pdetector)->ctarget++;
        return 1;
    }
    return 0;
}
} // namespace M_FAddStepguardDetectTarget

// 0x208700 AddStepguardPreferClass
namespace M_func_00208700 {

// 0x208700: Adds a class to the preferred-class list (at most 3).
extern "C" void AddStepguardPreferClass(STEPGUARD *guard, int cid)
{
    unsigned int c = guard->preferClassCount;
    if (c < 3) {
        int *pn = &guard->preferClasses[c];
        guard->preferClassCount = c + 1;
        *pn = cid;
    }
}
} // namespace M_func_00208700

// 0x208728 RemoveStepguardPreferClass
namespace M_func_00208728 {

void *memmove(void *d, void *s, int n) __asm__("memmove");
// 0x208728: Removes a class from the preferred-class list.
// Also known as: RemoveTargetClass (upstream readable / Sly 1 name; RemoveTargetClass__FPvi).
extern "C" void RemoveStepguardPreferClass(STEPGUARD *guard, int cid)
{
    int i;
    for (i = 0; i < guard->preferClassCount; i++) {
        if (guard->preferClasses[i] == cid) {
            memmove(guard->preferClasses + i, guard->preferClasses + i + 1, (guard->preferClassCount - i) * 4 - 4);
            guard->preferClassCount--;
        }
    }
}
} // namespace M_func_00208728

// 0x2087C8 ClearStepguardGeneratedTargets
// 0x2087C8: Drops the generated detector targets and clears the preferred-class list.
/* Takes the guard's contribution back out of its detector's total. */
extern "C" void ClearStepguardGeneratedTargets(STEPGUARD *guard)
{
    DETECTOR *detector;
    detector = guard->pdetector;
    detector->ctarget = detector->ctarget - guard->generatedTargetCount;
    guard->preferClassCount = 0;
    guard->generatedTargetCount = 0;
}

// 0x2087E8 IsStepguardPreferClass
namespace M_func_002087E8 {

int IsBasicDerivedFrom__FP5BASIC3CID(void *o, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
// 0x2087E8: True if the object derives from a preferred class.
// Also known as: FTargetClass (upstream readable / Sly 1 name; FTargetClass__FPvP2LO).
extern "C" int IsStepguardPreferClass(STEPGUARD *guard, void *object)
{
    int i;
    for (i = 0; i < guard->preferClassCount; i++) {
        if (IsBasicDerivedFrom__FP5BASIC3CID(object, guard->preferClasses[i])) return 1;
    }
    return 0;
}
} // namespace M_func_002087E8

// 0x208860 RemoveStepguardDetectTarget
namespace M_func_00208860 {
DETECTORTARGET *FindDetectorTarget(DETECTOR *detector) __asm__("FindDetectorTarget");    /* the object passes through in a1 */
void CopyAb__FPvT0Ui(void *dst, void *src, int cb) __asm__("CopyAb__FPvT0Ui");
// 0x208860: Removes a designated (non-generated) detector target, compacting the list.
extern "C" void RemoveStepguardDetectTarget(STEPGUARD *guard)
{
    DETECTORTARGET *entry = FindDetectorTarget(guard->pdetector);
    DETECTOR *detector = guard->pdetector;
    int count = detector->ctarget;
    DETECTORTARGET *generatedStart = &detector->atarget[count - guard->generatedTargetCount];
    if (entry && entry < generatedStart) {
        int index;
        detector->ctarget = count - 1;
        index = entry - detector->atarget;
        CopyAb__FPvT0Ui(entry, entry + 1, (guard->pdetector->ctarget - index) * 16);
    }
}
} // namespace M_func_00208860

// 0x2088E0 ClearStepguardTargetLists
// 0x2088E0: Clears the detector targets and the preferred, generated, ignored and forced lists.
extern "C" void ClearStepguardTargetLists(STEPGUARD *guard)
{
    guard->pdetector->ctarget = 0;
    guard->preferClassCount = 0;
    guard->generatedTargetCount = 0;
    ClearStepguardIgnoreClasses(guard);
    ClearStepguardForceClasses(guard);
}

// 0x208920 AddStepguardIgnoreClass
// 0x208920: Adds a class to the ignored-class list (at most 5).
extern "C" void AddStepguardIgnoreClass(STEPGUARD *guard, int cid) { if ((unsigned int)guard->ignoreClassCount < 5) guard->ignoreClasses[guard->ignoreClassCount++] = cid; }

// 0x208948 RemoveStepguardIgnoreClass
namespace M_func_00208948 {

void *memmove(void *d, void *s, int n) __asm__("memmove");
// 0x208948: Removes a class from the ignored-class list.
extern "C" void RemoveStepguardIgnoreClass(STEPGUARD *guard, int cid)
{
    int i;
    for (i = 0; i < guard->ignoreClassCount; i++) {
        if (guard->ignoreClasses[i] == cid) {
            memmove(guard->ignoreClasses + i, guard->ignoreClasses + i + 1, (guard->ignoreClassCount - i) * 4 - 4);
            guard->ignoreClassCount--;
        }
    }
}
} // namespace M_func_00208948

// 0x2089E8 ClearStepguardIgnoreClasses
// 0x2089E8: Clears the ignored-class list.
extern "C" void ClearStepguardIgnoreClasses(STEPGUARD *guard)
{
    guard->ignoreClassCount = 0;
}

// 0x2089F0 IsStepguardIgnoreClass
namespace M_func_002089F0 {

int IsBasicDerivedFrom__FP5BASIC3CID(void *o, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
// 0x2089F0: True if the object derives from an ignored class.
extern "C" int IsStepguardIgnoreClass(STEPGUARD *guard, void *object)
{
    int i;
    for (i = 0; i < guard->ignoreClassCount; i++) {
        if (IsBasicDerivedFrom__FP5BASIC3CID(object, guard->ignoreClasses[i])) return 1;
    }
    return 0;
}
} // namespace M_func_002089F0

// 0x208A68 AddStepguardForceClass
// 0x208A68: Adds a class to the forced-class list (at most 2).
extern "C" void AddStepguardForceClass(STEPGUARD *guard, int cid) { if ((unsigned int)guard->forceClassCount < 2) guard->forceClasses[guard->forceClassCount++] = cid; }

// 0x208A90 RemoveStepguardForceClass
namespace M_func_00208A90 {

void *memmove(void *d, void *s, int n) __asm__("memmove");
// 0x208A90: Removes a class from the forced-class list.
extern "C" void RemoveStepguardForceClass(STEPGUARD *guard, int cid)
{
    int i;
    for (i = 0; i < guard->forceClassCount; i++) {
        if (guard->forceClasses[i] == cid) {
            memmove(guard->forceClasses + i, guard->forceClasses + i + 1, (guard->forceClassCount - i) * 4 - 4);
            guard->forceClassCount--;
        }
    }
}
} // namespace M_func_00208A90

// 0x208B30 ClearStepguardForceClasses
// 0x208B30: Clears the forced-class list.
extern "C" void ClearStepguardForceClasses(STEPGUARD *guard)
{
    guard->forceClassCount = 0;
}

// 0x208B38 IsStepguardForceClass
namespace M_func_00208B38 {

int IsBasicDerivedFrom__FP5BASIC3CID(void *o, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
// 0x208B38: True if the object derives from a forced class.
extern "C" int IsStepguardForceClass(STEPGUARD *guard, void *object)
{
    int i;
    for (i = 0; i < guard->forceClassCount; i++) {
        if (IsBasicDerivedFrom__FP5BASIC3CID(object, guard->forceClasses[i])) return 1;
    }
    return 0;
}
} // namespace M_func_00208B38

// 0x208BB0 GetRandomStepguardDelay
namespace M_func_00208BB0 {
float RandomFloatInRange__Fff(float a, float b) __asm__("RandomFloatInRange__Fff");
// 0x208BB0: Returns a random delay of 5..7 seconds.
extern "C" float GetRandomStepguardDelay(void) { return RandomFloatInRange__Fff(5.0f, 7.0f); }
} // namespace M_func_00208BB0

// 0x208BE0 CanStepguardSenseStimulus
namespace M_FStepguardSenseStimulus {

int AreStepguardSensesEnabled(STEPGUARD *guard) __asm__("AreStepguardSensesEnabled");
DETECTORTARGET *FindDetectorTarget(DETECTOR *detector, LO *object) __asm__("FindDetectorTarget");
qword GetLoWorldPos(LO *p) __asm__("GetLoWorldPos");
int IsPositionInNoise(NOISE *stimulus, qword pos) __asm__("IsPositionInNoise");
int IsPointInVol(VOL *volume, qword pos) __asm__("IsPointInVol");
void IntersectSwBoundingBox(void *g, qword a, qword b, void *p1, void *p2, int *pi, SOPART ***papso) __asm__("IntersectSwBoundingBox");
int FindFirstObjectHitByLine(int z, qword a, qword b, int i, SOPART **apso, void *phit) __asm__("FindFirstObjectHitByLine");
void ShouldKeepSoNotSelf() __asm__("ShouldKeepSoNotSelf");

}

// 0x208BE0: STEPGUARD vtable slot 89 (sense a stimulus): senses enabled, the stimulus loud enough (>= +0xE68) and
// not its own; kind 0 noises by range test, kind 1 inside the detector's near volume with a clear line, and so on
// per kind.
// Also known as: FGuardAcceptsNoise (upstream readable / Sly 1 name; FGuardAcceptsNoise__FP5GUARDP5NOISE).
int STEPGUARD::CanSenseStimulus(NOISE *stimulus)
{
    using namespace M_FStepguardSenseStimulus;
    STEPGUARD *guard = this;
    NOISETYPE *stimTemplate;
    qword guardPos, stimulusPos;
    if (!AreStepguardSensesEnabled(guard))
        return 0;
    if (stimulus->level < guard->minStimulusLevel)
        return 0;
    if (stimulus->source && stimulus->source == guard)
        return 0;
    if (stimulus->level == 4) {
        LO *subject = stimulus->subject;
        DETECTORTARGET *entry = FindDetectorTarget(guard->pdetector, subject);
        if (subject != guard && !entry)
            return 0;
    }
    stimTemplate = stimulus->type;
    guardPos = GetLoWorldPos(guard);
    stimulusPos = stimulus->position;
    switch (stimTemplate->kind) {
    case 0:
        return IsPositionInNoise(stimulus, GetLoWorldPos(guard));
    case 1: {
        VOL *nearVolume = guard->pdetector->pvolNear;
        if (!nearVolume || ((int)nearVolume->isInWorld & 1) == 0)
            return 0;
        if (!IsPointInVol(nearVolume, stimulusPos))
            return 0;
        {
            STACKSCOPE frame;
            int numObjects; SOPART **objectList;
            IntersectSwBoundingBox(&g_sw, guardPos, stimulusPos, (void *)ShouldKeepSoNotSelf, guard->physicsTree, &numObjects, &objectList);
            if (!FindFirstObjectHitByLine(0, guardPos, stimulusPos, numObjects, objectList, 0))
                return 1;
            return 0;
        }
    }
    }
    return 0;
}

// 0x208D70 AreStepguardSensesEnabled
namespace M_FStepguardSensesEnabled {

long GetSgfMask(int n) __asm__("GetSgfMask");
// 0x208D70: True if the guard's senses are on (+0xE64) and none of the flags that blind it (alerted, hurt, dead,
// stunned, thrown, ...) is set.
// Also known as: FGuardFreeN (upstream readable / Sly 1 name; FGuardFreeN__FP5GUARD).
extern "C" int AreStepguardSensesEnabled(STEPGUARD *guard)
{
    long m = GetSgfMask(0xB) | GetSgfMask(SGF_COMBAT) | GetSgfMask(SGF_KO) | GetSgfMask(5) | GetSgfMask(6)
           | GetSgfMask(0x10) | GetSgfMask(1) | GetSgfMask(0x16) | GetSgfMask(0x2A);
    if (m & guard->grfsg)
        return 0;
    return guard->sensesEnabled;
}
} // namespace M_FStepguardSensesEnabled

// 0x208E70 GuardEvalInteractMode
namespace M_GuardEvalInteractMode {
struct PLAYER;
struct PLAYER { char pad[0x10DC]; void *interactTarget; int pad2; int interactMode; char pad3[0x10F0 - 0x10E8]; int fightEnabled; /* (inferred) */ };
extern float D_002C3D90 __asm__("D_002C3D90");
extern float D_002C3D94 __asm__("D_002C3D94");
void GetHeroInteractDistance(struct PLAYER *player, STEPGUARD *guard, void *target, float *distance, float *angle) __asm__("GetHeroInteractDistance");
unsigned long GetSgfMask(int n) __asm__("GetSgfMask");
int IsBasicDerivedFrom__FP5BASIC3CID(void *po, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
qword GetLoWorldPos(PLAYER *p) __asm__("GetLoWorldPos");
int IsPointInVol(VOL *volume, qword pos) __asm__("IsPointInVol");
int IsWithinLimits__FP2LMf(LM *p, float f) __asm__("IsWithinLimits__FP2LMf");

// 0x208E70: Ranks how the hero can interact with this guard: 4 the guard is engaged with (attacking, hurt by,
// holding) the hero, 3 the hero is inside a pickpocket glint volume, 2 close (distance and angle window), 1 in
// interaction range, 0 none. Also returns the distance through `distanceOut`.
// Also known as: NRankGuard (upstream readable / Sly 1 name; NRankGuard__FP5GUARDP5ACTORPf).
extern "C" int GuardEvalInteractMode(STEPGUARD *guard, struct PLAYER *player, float *distanceOut)
{
    float distance, angle;
    void *opponent;
    VOL *indicator;
    LM *angleRange;
    GetHeroInteractDistance(player, guard, &player->interactTarget, &distance, &angle);
    if (distanceOut)
        *distanceOut = distance;
    if (!(guard->grfsg & GetSgfMask(1))) {
        if (guard->unknown364 && player->fightEnabled) {
            if (StepguardHasSgf(guard, SGF_COMBAT))
                opponent = guard->target;
            else if (StepguardHasSgf(guard, 5))
                opponent = ((BHVHIT *)FindStepBehavior(guard, CID_BHV161))->attacker;
            else if (StepguardHasSgf(guard, 6))
                opponent = ((BHVHIT *)FindStepBehavior(guard, CID_BHV162))->attacker;
            else if (StepguardHasSgf(guard, SGF_JUGGLED)) {
                opponent = ((BHVJUGGLED *)FindStepBehavior(guard, CID_BHVJUGGLED))->holder;
                if (opponent == player)
                    *distanceOut += D_002C3D90;
            } else
                opponent = 0;
            if (opponent) {
                if (opponent == player)
                    return 4;
                if (IsBasicDerivedFrom__FP5BASIC3CID(opponent, CID_STEPPLAYER))
                    return 4;
            }
        }
        if (guard == player->interactTarget && player->interactMode == 3)
            indicator = guard->pickpocketIndicator1;
        else
            indicator = guard->pickpocketIndicator0;
        if (indicator && !!((int)indicator->isInWorld & 1) /* (`!!` needed for the match) */ && IsPointInVol(indicator, GetLoWorldPos(player)))
            return 3;
    }
    angleRange = StepguardHasSgf(guard, SGF_JUGGLED) ? &guard->closeAngleRangeHeld : &guard->closeAngleRange;
    if (distance <= guard->closeRange * D_002C3D94 && IsWithinLimits__FP2LMf(angleRange, angle) && guard->unknown364)
        return 2;
    if (distance <= guard->interactRange && IsWithinLimits__FP2LMf(&guard->interactAngleRange, angle) && guard->canInteract)
        return 1;
    return 0;
}
} // namespace M_GuardEvalInteractMode

// 0x209098 IsStepguardHandsOff
namespace M_func_00209098 {

int IsSwInputLocked(SW *p) __asm__("IsSwInputLocked");
// 0x209098: True while the guard is hands-off (+0xFC8) or world input is locked.
extern "C" int IsStepguardHandsOff(STEPGUARD *guard)
{
    if (guard->isHandsOff != 0)
        return 1;
    return IsSwInputLocked(&g_sw);
}
} // namespace M_func_00209098

// 0x2090D0 ReleaseAnimBinding
// 0x2090D0: Releases an animation binding: frees its kind-2/3 payload and marks it empty (-1).
/* Releases the binding's handle by kind (2 or 3) and marks the binding empty. */
extern "C" void ReleaseAnimBinding(ANIMBINDING *binding, STEPGUARD *guard)
{
    int kind = binding->kind;
    if (kind != BINDING_LEAP) {   // (tested in this order for the match)
        if (kind == BINDING_DIRANIM) {
            FreeDirAnim(binding->h);
            binding->h = 0;
        }
    } else {
        FreeLeap(binding->h);
        binding->h = 0;
    }
    binding->h = -1;
    binding->kind = BINDING_NONE;
}

// 0x209130 ResolveAnimBinding
namespace M_ResolveAnimBinding {
struct LEAPPAYLOAD;
struct LEAPPAYLOAD { int id; void *aseg; };
struct DIRANIMPAYLOAD;
struct MARKER;
void PostAsegArcLoad(struct LEAPPAYLOAD *, void *) __asm__("PostAsegArcLoad");
void InitDirAnims(struct DIRANIMPAYLOAD *, void *) __asm__("InitDirAnims");
void *FindSwObject__FP2SWi3OIDP2LO(void *, int, int, void *) __asm__("FindSwObject__FP2SWi3OIDP2LO");
int GetBasicCid(void *) __asm__("GetBasicCid");
struct MARKER *FindAsegLabel(void *, int) __asm__("FindAsegLabel");
struct LEAPPAYLOAD *AllocLeap(int) __asm__("AllocLeap");
void InitAsegArc(struct LEAPPAYLOAD *) __asm__("InitAsegArc");

// 0x209130: Resolves a binding's oid: an ASEG (cid 0x6D; wrapped in a new leap payload if it has label
// 0x22) or an SM (cid 0x6F); else empty. Leap and DIRANIM payloads resolve their own oids.
// Also known as: ResolveKinde (upstream readable / Sly 1 name; ResolveKinde__FP5KINDEP5GUARD).
extern "C" void ResolveAnimBinding(ANIMBINDING *binding, STEPGUARD *context)
{
    void *resource;
    if (binding->kind != BINDING_LEAP) {
        if (binding->kind == BINDING_DIRANIM) {
            InitDirAnims((struct DIRANIMPAYLOAD *)binding->payload4, context);
            return;
        }
    } else {
        PostAsegArcLoad(((struct LEAPPAYLOAD *)binding->payload4), context);
        return;
    }
    resource = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, binding->oid, context);
    if (!resource) {
        binding->kind = BINDING_NONE;
        return;
    }
    switch (GetBasicCid(resource)) {
    case 0x6D:
        if (FindAsegLabel(resource, 0x22)) {
            binding->kind = BINDING_LEAP;
            binding->payload4 = AllocLeap(0x40);
            InitAsegArc(((struct LEAPPAYLOAD *)binding->payload4));
            ((struct LEAPPAYLOAD *)binding->payload4)->aseg = resource;
            PostAsegArcLoad(((struct LEAPPAYLOAD *)binding->payload4), context);
            if (binding->flags == -1)
                binding->flags = 0;
        } else {
            binding->aseg = (ASEG *)resource;
            binding->kind = BINDING_ASEG;
        }
        break;
    case 0x6F:
        binding->kind = BINDING_SM;
        binding->sm = (SM *)resource;
        if (binding->flags == -1)
            binding->flags = 0;
        break;
    default:
        binding->kind = BINDING_NONE;
        break;
    }
}
} // namespace M_ResolveAnimBinding

namespace RO_ORDER { extern const float r_D_002A8868 __asm__("D_002A8868") = 3.40282347e+38f; }
namespace RO_ORDER { extern const float r_D_002A886C __asm__("D_002A886C") = 3.40282347e+38f; }
// 0x209278 StartAnimBinding
namespace M_StartAnimBinding {
void ApplyAsegCore(ASEG *aseg, STEPGUARD *guard, int flags, ASEGA **instance, float start, float speed) __asm__("ApplyAsegCore");
void ApplySmGetSma(SM *sm, STEPGUARD *guard, int a, int flags, SMA **instance) __asm__("ApplySmGetSma");
void StartLeap(void *leap, STEPGUARD *guard, int flags, LEAP *instance) __asm__("StartLeap");
void StartDirAnim(void *diranim, STEPGUARD *guard, int flags, CID146 *instance) __asm__("StartDirAnim");
}
// 0x209278: Starts the bound animation: applies the ASEG, makes an SMA for an SM, or starts the leap/DIRANIM payload
// on the guard's own LEAP / DIRANIM; the instance is then referenced (slot 1).
// Also known as: PlayKinde (upstream readable / Sly 1 name; PlayKinde__FP5KINDEP5GUARDif).
extern "C" void StartAnimBinding(ANIMBINDING *binding, STEPGUARD *guard, int flags, float start)
{
    using namespace M_StartAnimBinding;
    switch (binding->kind) {
    case BINDING_NONE:
        break;
    case BINDING_ASEG:
        ApplyAsegCore(binding->aseg, guard, flags, &binding->asega, start, 1.0f);
        binding->pbasic->AddRef();
        break;
    case BINDING_SM:
        ApplySmGetSma(binding->sm, guard, -1, flags, (SMA **)&binding->payload);
        binding->pbasic->AddRef();
        break;
    case BINDING_LEAP:
        binding->pbasic = &guard->leap;
        StartLeap(binding->payload4, guard, flags, (LEAP *)binding->pbasic);
        binding->pbasic->AddRef();
        break;
    case BINDING_DIRANIM:
        binding->pbasic = &guard->cid146;
        StartDirAnim(binding->payload4, guard, flags, (CID146 *)binding->pbasic);
        binding->pbasic->AddRef();
        break;
    }
}

// 0x209360 StopAnimBinding
namespace M_StopAnimBinding {
void RetractAsega(BASIC *instance) __asm__("RetractAsega");
void RetractSma(BASIC *instance) __asm__("RetractSma");
void StopLeap(BASIC *instance) __asm__("StopLeap");
void StopDirAnim(BASIC *instance) __asm__("StopDirAnim");
}
// 0x209360: Stops the running animation instance per kind (retract the ASEGA or SMA, ...) and drops it (slot 2).
// Also known as: StopKinde (upstream readable / Sly 1 name; StopKinde__FP5KINDE).
extern "C" void StopAnimBinding(ANIMBINDING *binding)
{
    using namespace M_StopAnimBinding;
    switch (binding->kind) {
    case BINDING_NONE:
        break;
    case BINDING_ASEG:
        if (binding->pbasic) {
            RetractAsega(binding->pbasic);
            binding->pbasic->Release();
            binding->pbasic = 0;
        }
        break;
    case BINDING_SM:
        if (binding->pbasic) {
            RetractSma(binding->pbasic);
            binding->pbasic->Release();
            binding->pbasic = 0;
        }
        break;
    case BINDING_LEAP:
        if (binding->pbasic) {
            StopLeap(binding->pbasic);
            binding->pbasic->Release();
            binding->pbasic = 0;
        }
        break;
    case BINDING_DIRANIM:
        if (binding->pbasic) {
            StopDirAnim(binding->pbasic);
            binding->pbasic->Release();
            binding->pbasic = 0;
        }
        break;
    }
}

// 0x209430 IsAnimBindingDone
namespace M_FAnimBindingDone {
int IsAsegaRetracted(ASEGA *pasega) __asm__("IsAsegaRetracted");
int IsSmaRetracted(SMA *psma) __asm__("IsSmaRetracted");
int IsLeapDone(LEAP *pleap) __asm__("IsLeapDone");
int IsDirAnimStopped(CID146 *pdiranim) __asm__("IsDirAnimStopped");
// 0x209430: True if the binding's animation instance finished or was retracted (or none is running).
// (`done = 0; if (...) done = 1;` per case: the boolean-expression and early-return forms do not match.)
// Also known as: FKindeDone (upstream readable / Sly 1 name; FKindeDone__FP5KINDE).
extern "C" int IsAnimBindingDone(ANIMBINDING *binding)
{
    int done;
    switch (binding->kind) {
    case BINDING_ASEG:
        done = 0;
        if (!binding->pbasic || IsAsegaRetracted(binding->asega))
            done = 1;
        break;
    case BINDING_SM:
        done = 0;
        if (!binding->pbasic || IsSmaRetracted(binding->sma) || !binding->sma->pasegaCur)
            done = 1;
        break;
    case BINDING_LEAP:
        done = 0;
        if (!binding->pbasic || IsLeapDone((LEAP *)binding->pbasic))
            done = 1;
        break;
    case BINDING_DIRANIM:
        done = 0;
        if (!binding->pbasic || IsDirAnimStopped((CID146 *)binding->pbasic))
            done = 1;
        break;
    case BINDING_NONE:
    default:
        done = 1;
        break;
    }
    return done;
}
} // namespace M_FAnimBindingDone

// 0x209518 GetAnimBindingInstance
namespace M_func_00209518 {
// 0x209518: Returns the ASEGA that the binding's running instance plays (the instance itself for an ASEG).
// Also known as: PasegaOfKinde (upstream readable / Sly 1 name; PasegaOfKinde__FP5KINDE).
extern "C" ASEGA *GetAnimBindingInstance(ANIMBINDING *binding)
{
    switch (binding->kind) {
    case BINDING_ASEG: return binding->asega;
    case BINDING_SM: return binding->sma->pasegaCur;
    case BINDING_LEAP: return binding->leap->pasegaCur;
    case BINDING_DIRANIM: return binding->diranim->pasegaCur;
    }
    return 0;
}
} // namespace M_func_00209518

// 0x209590 CloneAnimBinding
namespace M_func_00209590 {
void *AllocLeap(int cb) __asm__("AllocLeap");
void *AllocDirAnim(int cb) __asm__("AllocDirAnim");
void CopyAb__FPvT0Ui(void *dst, void *src, int cb) __asm__("CopyAb__FPvT0Ui");
// 0x209590: Deep-copies a binding's leap (0x40 bytes) or DIRANIM (0xD0 bytes) payload, so a
// cloned guard does not share it.
extern "C" void CloneAnimBinding(ANIMBINDING *binding)
{
    void *copy;
    switch (binding->kind) {
    case BINDING_LEAP:
        copy = AllocLeap(0x40);
        CopyAb__FPvT0Ui(copy, binding->payload4, 0x40);
        binding->payload4 = copy;
        break;
    case BINDING_DIRANIM:
        copy = AllocDirAnim(sizeof(DIRBL));
        CopyAb__FPvT0Ui(copy, binding->payload4, sizeof(DIRBL));
        binding->payload4 = copy;
        break;
    }
}
} // namespace M_func_00209590

// 0x209618 DriveStepguardFromAnim
namespace M_func_00209618 {
 struct INSTANCE; struct LEAP;  struct STEPGUARDIsland;
typedef union { qword q; float f; } U;

struct INSTANCE { char pad[0x10]; qword velocity; /* DIRANIM */ char pad2[0x30 - 0x20]; struct ASEGA *asega; /* SMA */ };
struct LEAP { char pad[0x10]; int state; char pad2[0x38 - 0x14]; struct ASEGA *asega; };
/* island: register-pinned asm body */

/* island: register-pinned asm body; STEP +0x190/+0x1E0 as qwords (step.h has VECTOR/qword padding names) */
struct STEPGUARDIsland : PO { char pad[0x190 - 0x180]; qword desiredVelocity; char pad2[0x1E0 - 0x1A0]; qword desiredAngularVelocity; char pad3[0xFC4 - 0x1F0]; struct ACT *scaleAct; };
extern qword D_002A2DD0 __asm__("D_002A2DD0");
qword GetStepAsegaTranslation(struct STEPGUARDIsland *, struct ASEGA *, float) __asm__("GetStepAsegaTranslation");
qword GetStepAsegaRotation(struct STEPGUARDIsland *, struct ASEGA *, float) __asm__("GetStepAsegaRotation");
void UpdateLeap(struct LEAP *) __asm__("UpdateLeap");
// 0x209618: Drives the guard from its animation: sets the desired velocity and angular velocity from the binding'guard
// root motion (scaled by the special-state scale act when there is one); a leap drives
// it only outside its airborne states.
// Also known as: SteerByKinde (upstream readable / Sly 1 name; SteerByKinde__FP5KINDEP5GUARD).
#ifdef SLY2_EE_ASM
extern "C" void DriveStepguardFromAnim(struct ANIMBINDING *binding, struct STEPGUARDIsland *guard)
{
    U factor, value;
    register qword scalar __asm__("vf1");
    qword extra;
    register qword neg __asm__("vf2");
    {register qword *zeroAddress __asm__("v0")=&D_002A2DD0;
     register qword zero __asm__("v0");
     __asm__("lq %0,0(%1)":"=r"(zero):"r"(zeroAddress),"m"(*zeroAddress));
     __asm__("sq %1,%0":"=m"(value.q):"r"(zero));}
    extra=value.q;
    { register int kind __asm__("v1");
      __asm__("lw %0,%1":"=r"(kind):"m"(binding->kind));
    switch (kind) {
    case BINDING_ASEG:
    case BINDING_SM:
        {
            struct ASEGA *asega;
            if (kind == BINDING_ASEG) asega = (struct ASEGA *)binding->payload;
            else asega = ((struct INSTANCE *)binding->payload)->asega;
            if (asega) {
                value.q = GetStepAsegaTranslation(guard, asega, asega->time);
                extra = GetStepAsegaRotation(guard, asega, asega->time);
            }
        }
        break;
    case BINDING_LEAP:
        {
            struct LEAP *leap;
            UpdateLeap((struct LEAP *)binding->payload);
            leap = (struct LEAP *)binding->payload;
            if (leap->state > 0 && (leap->state < 3 || leap->state == 4)) break;
            value.q = GetStepAsegaTranslation(guard, leap->asega, leap->asega->time);
        }
        break;
    case BINDING_DIRANIM: {
        register struct INSTANCE *object __asm__("v0")=(struct INSTANCE *)binding->payload;
        register qword vector __asm__("v0");
        __asm__("lq %0,16(%1)":"=r"(vector):"r"(object),"m"(object->velocity));
        value.q=vector; }
        break;
    }
    }
    {
      register qword current __asm__("vf2")=value.q;
      if (guard->scaleAct) {
        register union FLOATWORD *wordAddress __asm__("v0")=(union FLOATWORD *)guard->scaleAct->transform;   /* the XF's first word: its scale */
        register int parameterWord __asm__("v0");
        __asm__("lw %0,0(%2)\n\tqmtc2.ni %0,%1":"=r"(parameterWord),"=j"(scalar):"r"(wordAddress),"m"(wordAddress->bits));
        factor.q=scalar;
        __asm__("vmulx.xyzw %0,%0,%1x":"+j"(current):"j"(factor.q));
        value.q=current;
      }
    }
    guard->desiredAngularVelocity = extra;
    __asm__ __volatile__("qmtc2.ni %1, %0" : "=j"(scalar) : "r"(-1.0f));
    factor.q=scalar;
    { register qword source __asm__("vf3")=value.q;
      __asm__("vmulx.xyzw %0,%1,%2x":"=j"(neg):"j"(source),"j"(factor.q)); }
    guard->desiredVelocity = neg;
}
#else
// C version of the EE body above (register-pinned asm over the views), on the header classes: the root-motion
// translation and rotation of the binding's instance (a leap only outside its airborne states, a directional
// animation its velocity), the translation scaled by the scale act's first transform word and negated.
extern "C" void DriveStepguardFromAnim(struct ANIMBINDING *binding, struct STEPGUARDIsland *guardView)
{
    STEPGUARD *guard = (STEPGUARD *)guardView;
    qword value = D_002A2DD0;
    qword extra = value;
    switch (binding->kind) {
    case BINDING_ASEG:
    case BINDING_SM: {
        ASEGA *asega = binding->kind == BINDING_ASEG ? binding->asega : binding->sma->pasegaCur;
        if (asega) {
            value = GetStepAsegaTranslation(guardView, asega, asega->time);
            extra = GetStepAsegaRotation(guardView, asega, asega->time);
        }
        break;
    }
    case BINDING_LEAP: {
        UpdateLeap((struct LEAP *)binding->leap);
        ::LEAP *leap = binding->leap;
        if (leap->state > 0 && (leap->state < 3 || leap->state == 4))
            break;
        value = GetStepAsegaTranslation(guardView, leap->pasegaCur, leap->pasegaCur->time);
        break;
    }
    case BINDING_DIRANIM:
        value = binding->diranim->velocity;
        break;
    }
    VuLanes v = VuGet(value);
    if (guard->scale) {
        float scale;
        __builtin_memcpy(&scale, ((ACT *)guard->scale)->transform, sizeof(scale));   /* the XF's first word */
        v.x *= scale; v.y *= scale; v.z *= scale; v.w *= scale;
    }
    guard->targetAngularVelocity = extra;
    v.x *= -1.0f; v.y *= -1.0f; v.z *= -1.0f; v.w *= -1.0f;
    guard->localVelocity = VuPut<qword>(v);
}
#endif
} // namespace M_func_00209618

// 0x209768 InitStepguardMover
// 0x209768: Initializes the guard's mover (+0xFD0): owner = guard.
extern "C" void InitStepguardMover(GUARDMOVER *mover, STEPGUARD *guard)
{
    mover->guard = guard;
}

// 0x209770 CloneStepguardMover
// 0x209770: Clone fix-up of the mover: owner = guard.
extern "C" void CloneStepguardMover(GUARDMOVER *mover, STEPGUARD *guard)
{
    mover->guard = guard;
}

// 0x209778 ClearStepguardMover
namespace M_func_00209778 {
// 0x209778: Clears the mover (driver, lock, third word).
extern "C" void ClearStepguardMover(GUARDMOVER *mover)
{
    mover->driver = 0;
    mover->isHeadingLocked = 0;
    mover->unknown8 = 0;
}
} // namespace M_func_00209778

// 0x209788 SetStepguardMoverDriver
// 0x209788: Sets the mover's driver: the behaviour whose motion update moves the guard.
extern "C" void SetStepguardMoverDriver(int *driverSlot, int driver)
{
    *driverSlot = driver;
}

// 0x209790 ResetStepguardMover
// 0x209790: Resets the mover: no driver, unlocked, zero desired velocity.
extern "C" void ResetStepguardMover(GUARDMOVER *mover)
{
    STEPGUARD *guard;
    mover->driver = 0;
    guard = mover->guard;
    mover->isHeadingLocked = 0;
    VECTOR *velocity = &guard->vecLocalVelocity;
    velocity->z = 0.0f;
    velocity->y = 0.0f;
    velocity->x = 0.0f;
    WakeSo(mover->guard);
}

// 0x2097C8 LockStepguardMover
// 0x2097C8: Locks the mover's heading.
extern "C" void LockStepguardMover(GUARDMOVER *mover)
{
    mover->isHeadingLocked = 1;
}

// 0x2097D8 UnlockStepguardMover
// 0x2097D8: Unlocks the mover's heading.
extern "C" void UnlockStepguardMover(GUARDMOVER *mover)
{
    mover->isHeadingLocked = 0;
}

// 0x2097E0 UpdateStepguardMover
// 0x2097E0: Per-frame mover update: calls the driver's motion update (vtable slot 11).
extern "C" void UpdateStepguardMover(GUARDMOVER *mover)
{
    BHV *driver = mover->driver;
    if (driver)
        driver->UpdateMotion();
}

// 0x209818 SetStepguardMoverRadTarget
// 0x209818: Unless the heading is locked, sets the guard's target heading.
// Also known as: SetMoveHeading (upstream readable / Sly 1 name; SetMoveHeading__FP7MOVEREF).
extern "C" void SetStepguardMoverRadTarget(GUARDMOVER *mover)
{
    if (mover->isHeadingLocked == 0) {
        SetStepTargetHeading(mover->guard);
    }
}

// 0x209840 SetStepguardMoverTargetMatrix
// 0x209840: Unless the heading is locked, sets the guard's target orientation matrix.
/* Sets the guard's target orientation unless the mover is locked. */
extern "C" void SetStepguardMoverTargetMatrix(GUARDMOVER *mover, MATRIX3 *matrix)
{
    STEPGUARD *guard;
    if (mover->isHeadingLocked == 0) {
        guard = mover->guard;
        guard->matTarget = *matrix;
    }
}

// 0x209870 SetStepguardSgfMask
// 0x209870: Stores the guard flag word and re-evaluates the pickpocket state.
// Also known as: SetGuardStateMask (upstream readable / Sly 1 name; SetGuardStateMask__FP5GUARDUl).
extern "C" void SetStepguardSgfMask(STEPGUARD *guard, unsigned long grfsg)
{
    guard->grfsg = grfsg;
    UpdateStepguardPickpocket(guard);
}

// 0x209890 AddStepguardSgf
namespace M_AddStepguardSgf {
struct SGFEVENT { int sgf; int gameEvent; };
extern struct SGFEVENT D_002C3D98[2] __asm__("D_002C3D98");
struct ARGS { int *sgf; int *state; };
long GetSgfMask(int n) __asm__("GetSgfMask");
void BroadcastGameEvent(void *p, int gameEvent, STEPGUARD *source) __asm__("BroadcastGameEvent");
}
// 0x209890: Raises guard flag `sgf`: sends itself splice event 7 (raised, -1) and broadcasts the game event tied to
// some flags (attacking 0x14, flag 0xC 0x16).
// Also known as: AddGuardState (upstream readable / Sly 1 name; AddGuardState__FP5GUARDi).
extern "C" void AddStepguardSgf(STEPGUARD *guard, int sgf)
{
    using namespace M_AddStepguardSgf;
    long mask = GetSgfMask(sgf);
    ARGS args;
    int state;
    unsigned int i;
    SetStepguardSgfMask(guard, guard->grfsg | mask);
    state = -1;
    args.sgf = &sgf;
    args.state = &state;
    guard->HandleSpliceEvent(7, 0, 2, (void **)&args, 0);
    for (i = 0; i < 2; i++) {
        if (MaskHasSgf(mask, D_002C3D98[i].sgf))
            BroadcastGameEvent(&g_game, D_002C3D98[i].gameEvent, guard);
    }
}

// 0x209970 RemoveStepguardSgf
namespace M_RemoveStepguardSgf {
extern unsigned long GetSgfMask(int n) __asm__("GetSgfMask");
}
// 0x209970: Lowers guard flag `sgf` and sends itself splice event 7 (-1, lowered).
// Also known as: RemoveGuardState (upstream readable / Sly 1 name; RemoveGuardState__FP5GUARDi).
extern "C" void RemoveStepguardSgf(STEPGUARD *guard, int sgf)
{
    using namespace M_RemoveStepguardSgf;
    int state;
    void *args[2];
    SetStepguardSgfMask(guard, guard->grfsg & ~GetSgfMask(sgf));
    state = -1;
    args[0] = &state;
    args[1] = &sgf;
    guard->HandleSpliceEvent(7, 0, 2, args, 0);
}

// 0x2099F0 MaskHasSgf
namespace M_FGrfHasSgf {
unsigned long GetSgfMask(int n) __asm__("GetSgfMask");
// 0x2099F0: True if flag `sgf` is set in a mask.
extern "C" int MaskHasSgf(unsigned long mask, int sgf) { return (mask & GetSgfMask(sgf)) != 0; }
} // namespace M_FGrfHasSgf

// 0x209A28 CountSgfMatch
namespace M_func_00209A28 {
// 0x209A28: Returns the number of flags in `subset` if it is a subset of `mask`, else 0.
extern "C" int CountSgfMatch(unsigned long subset, unsigned long mask)
{
    unsigned long common = subset & mask;
    int count;
    if (common != subset)
        return 0;
    for (count = 0; common != 0; count++)
        common &= common - 1;   // clear the lowest set bit
    return count;
}
} // namespace M_func_00209A28

// 0x209A68 StepguardHasSgf
// 0x209A68: True if the guard has flag `sgf` set.
extern "C" int StepguardHasSgf(STEPGUARD *guard, int sgf)
{
    return MaskHasSgf(guard->grfsg, sgf);
}

// 0x209A88 GetStepguardSgfRefList
namespace M_func_00209A88 {

void *ListSgfsInMask(long mask, char *sgfs) __asm__("ListSgfsInMask");
void BuildSpliceListFromUniformValues(CRefData *ret, void *count, int type, char *values) SLY2_SRET_LABEL("BuildSpliceListFromUniformValues", "BuildSpliceListFromUniformValuesInto");
// 0x209A88: Splice getter: the guard's set flags as a list.
extern "C" CRefData *GetStepguardSgfRefList(CRefData *ret, STEPGUARD *guard)
{
    char sgfs[0xC0];
    BuildSpliceListFromUniformValues(ret, ListSgfsInMask(guard->grfsg, sgfs), 0x1010, sgfs);
    return ret;
}
} // namespace M_func_00209A88

// 0x209AD0 FindBestAnimBinding
namespace M_PanimbindBest {
long GetSgfMask(int n) __asm__("GetSgfMask");
int CountSgfMatch(long subset, long mask) __asm__("CountSgfMatch");
// 0x209AD0: Finds the animation binding whose flag set is the largest subset of `mask` and contains its lowest
// flag.
// Also known as: PkindeBest (upstream readable / Sly 1 name; PkindeBest__FP5GUARDUl).
extern "C" ANIMBINDINGREC *FindBestAnimBinding(STEPGUARD *guard, long mask)
{
    int i;
    int sgf = 0;
    long lowestMask = 0;
    int bestCount = 0;
    ANIMBINDINGREC *best = 0;
    ANIMBINDINGREC *rec;
    for (; sgf < 0x30; sgf++) {
        lowestMask = GetSgfMask(sgf);
        if (mask & lowestMask)
            break;
    }
    for (i = 0; i < guard->cbindingrec; i++) {
        int count;
        rec = &guard->abindingrec[i];
        count = CountSgfMatch(rec->sgfMask, mask);
        if (count && (rec->sgfMask & lowestMask) && bestCount < count) {
            bestCount = count;
            best = rec;
        }
    }
    return best;
}
} // namespace M_PanimbindBest

// 0x209BC0 FindCurrentAnimBinding
namespace M_func_00209BC0 {

struct ENTRY;
struct ENTRY *FindBestAnimBinding(STEPGUARD *guard, long mask) __asm__("FindBestAnimBinding");
// 0x209BC0: Finds the best animation binding for the guard's current flags.
extern "C" struct ENTRY *FindCurrentAnimBinding(STEPGUARD *guard)
{
    return FindBestAnimBinding(guard, guard->grfsg);
}
} // namespace M_func_00209BC0

// 0x209BE0 FindAnimBinding
namespace M_func_00209BE0 {
// 0x209BE0: Finds the animation binding with exactly this flag set.
extern "C" ANIMBINDINGREC *FindAnimBinding(STEPGUARD *guard, long sgfMask)
{
    int i;
    for (i = 0; i < guard->cbindingrec; i++) {
        ANIMBINDINGREC *rec = &guard->abindingrec[i];
        if (rec->sgfMask == sgfMask)
            return rec;
    }
    return 0;
}
} // namespace M_func_00209BE0

// 0x209C18 EnsureAnimBinding
namespace M_func_00209C18 {
ANIMBINDINGREC *FindAnimBinding(STEPGUARD *guard, long sgfMask) __asm__("FindAnimBinding");
// 0x209C18: Finds or appends (released) the animation binding for a flag set.
// Also known as: PkindeOfMask (upstream readable / Sly 1 name; PkindeOfMask__FP5GUARDUl).
extern "C" ANIMBINDING *EnsureAnimBinding(STEPGUARD *guard, unsigned long sgfMask)
{
    ANIMBINDINGREC *rec = FindAnimBinding(guard, sgfMask);
    if (!rec) {
        ANIMBINDINGREC *added = &guard->abindingrec[guard->cbindingrec++];
        added->sgfMask = sgfMask;
        rec = added;
    }
    ReleaseAnimBinding(&rec->binding, guard);
    return &rec->binding;
}
} // namespace M_func_00209C18

// 0x209C88 AddStepguardAttachContacts
namespace M_func_00209C88 {
extern qword D_002A2DA0 __asm__("D_002A2DA0");
void IgnoreSoSlot59(STEPGUARD *p) __asm__("IgnoreSoSlot59");
void EvaluateShapeAtDistance(LO *shape, qword *pos, qword *tangent, float s) __asm__("EvaluateShapeAtDistance");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
qword *GetLoWorldMatrix(void *p) __asm__("GetLoWorldMatrix");

// 0x209C88: STEPGUARD vtable slot 59: while attached (rail, pipe, ...), adds constraint contacts toward the attach
// object.
}
// Also known as: AddGuardRailContacts (upstream readable / Sly 1 name; AddGuardRailContacts__FP3ALO).
void STEPGUARD::AddExtraXps()
{
    using namespace M_func_00209C88;
    STEPGUARD *guard = this;
    qword pos, tangent;
    IgnoreSoSlot59(guard);
    switch (guard->attachKind) {
    case 0:
        EvaluateShapeAtDistance(guard->attachObject, &pos, &tangent, guard->attachParam);
        guard->AddAxisXps(0x22, guard->attachObject, pos, tangent, *guard->probeBottomPos);
        break;
    case 1: {
        EvaluateShapeAtDistance(guard->attachObject, &pos, &tangent, guard->attachParam);
        guard->AddAxisXps(7, guard->attachObject, pos, tangent, GetLoWorldPos(guard->attachPoint));
        break;
    }
    case 2: {
        guard->AddAxisXps(7, guard->attachObject, GetLoWorldPos(guard->attachObject), *GetLoWorldMatrix(guard->attachObject), GetLoWorldPos(guard->attachPoint));
        break;
    }
    case 3: {
        guard->AddAxisXps(7, guard->attachObject, GetLoWorldPos(guard->attachObject), D_002A2DA0, *guard->probeBottomPos);
        break;
    }
    }
}
// (end of AddStepguardAttachContacts: M_func_00209C88)

// 0x209E28 SendStepguardRootBhvMode6
namespace M_func_00209E28 {
void *FindStepBehavior(void *p, int oid) __asm__("FindStepBehavior");
void *func_002333B4(void *pv, int c, int cb) __asm__("memset");
void SetBhvguardMode(void *bhv, BHVGUARDRECDATA *mode) __asm__("SetBhvguardMode");
// 0x209E28: Sends mode record 6 (thrown) with `value` as its object to the guard's root behaviour (BHVGUARD).
// Also known as: ReactGuardHeld (upstream readable / Sly 1 name; ReactGuardHeld__FP2LOi).
extern "C" void SendStepguardRootBhvMode6(void *guard, void *value)
{
    void *bhv = FindStepBehavior(guard, CID_BHVGUARD);
    BHVGUARDRECDATA mode;
    func_002333B4(&mode, 0, sizeof mode);
    mode.kind = 6;
    mode.arg.object = value;
    SetBhvguardMode(bhv, &mode);
}
} // namespace M_func_00209E28

// 0x209E88 StunStepguardThrownOp1
// 0x209E88: Thrown-behaviour operation 1 (when thrown or flag 0x12) (inferred).
// Also known as: CarryGuardHeld (upstream readable / Sly 1 name; CarryGuardHeld__FP2LO).
extern "C" void StunStepguardThrownOp1(STEPGUARD *guard)
{
    if ((StepguardHasSgf(guard, 0x16) != 0) || (StepguardHasSgf(guard, 0x12) != 0)) {
        SetBhvThrownState(FindStepBehavior(guard, CID_BHV176), 1);
    }
}

// 0x209EE0 StunStepguardThrownOp2
namespace M_func_00209EE0 {
extern float D_002C3DA8 __asm__("D_002C3DA8");
void SetBhvjuggledState(BHVJUGGLED *o, int n) __asm__("SetBhvjuggledState");
qword LimitVectorLength(qword v, float g) __asm__("LimitVectorLength");
}
// 0x209EE0: Thrown-behaviour operation 2 (flags 0x12/0x13), or juggled-behaviour operation 2 while stunned; then
// clamps the velocity's length (inferred).
// Also known as: ReleaseGuardHeld (upstream readable / Sly 1 name; ReleaseGuardHeld__FP2LO).
extern "C" void StunStepguardThrownOp2(STEPGUARD *guard)
{
    using namespace M_func_00209EE0;
    if (StepguardHasSgf(guard, 0x12) || StepguardHasSgf(guard, 0x13)) {
        SetBhvThrownState(FindStepBehavior(guard, CID_BHV176), 2);
    } else {
        BHVJUGGLED *juggled;
        if (!StepguardHasSgf(guard, SGF_JUGGLED))
            return;
        juggled = (BHVJUGGLED *)FindStepBehavior(guard, CID_BHVJUGGLED);
        if (juggled->juggleState != 3 && juggled->juggleState != 6)
            SetBhvjuggledState(juggled, 2);
    }
    {
        qword velocity = guard->GetVelocity();
        velocity = LimitVectorLength(velocity, D_002C3DA8);
        guard->SetVelocity(velocity);
    }
}

// 0x209FB8 StunStepguardThrownOp3
// 0x209FB8: Thrown-behaviour operation 3 (flags 0x16/0x13) (inferred).
// Also known as: ThrowGuardHeld (upstream readable / Sly 1 name; ThrowGuardHeld__FP2LO).
extern "C" void StunStepguardThrownOp3(STEPGUARD *guard)
{
    if ((StepguardHasSgf(guard, 0x16) != 0) || (StepguardHasSgf(guard, 0x13) != 0)) {
        SetBhvThrownState(FindStepBehavior(guard, CID_BHV176), 3);
    }
}

// 0x20A010 GetStepguardJuggledValue
// 0x20A010: Returns the juggled behaviour's hit count while stunned (flag 0x17), else 0 (inferred).
extern "C" int GetStepguardJuggledValue(STEPGUARD *guard)
{
    if (StepguardHasSgf(guard, SGF_JUGGLED))
        return ((BHVJUGGLED *)FindStepBehavior(guard, CID_BHVJUGGLED))->hitCount;
    return 0;
}

// 0x20A058 SetStepguardTargetLists
namespace M_func_0020A058 {

typedef struct { int tagk; int tag; } CRef;
void SetValuesFromSpliceList(CRef *ref, int list, int type, int capacity, void *count, void *values) SLY2_SRET_LABEL("SetValuesFromSpliceList", "SetValuesFromSpliceListInto");
void func_00129658(CRef *pd, CRef *ps) __asm__("__as__4CRefRC4CRef");
void Decref__4CRef(CRef *ref) __asm__("Decref__4CRef");
// 0x20A058: Splice option: sets the designated target list and the avoid-object list (two typed reference lists).
extern "C" void SetStepguardTargetLists(STEPGUARD *guard, int targetList, int avoidList)
{
    CRef result;
    CRef list;
    result.tagk = -1;
    SetValuesFromSpliceList(&list, targetList, 0x2002, 4, &guard->designatedTargetCount, &guard->designatedTargets);
    func_00129658(&result, &list);
    Decref__4CRef(&list);
    SetValuesFromSpliceList(&list, avoidList, 0x2002, 4, &guard->avoidObjectCount, &guard->avoidObjects);
    func_00129658(&result, &list);
    Decref__4CRef(&list);
    Decref__4CRef(&result);
}
} // namespace M_func_0020A058

// 0x20A100 SetStepguardEscortPoint
// 0x20A100: Stores an escort point/object pair and passes it to the root behaviour when it is in mode 0x10.
/* Stores an escort object and point; when the root behaviour (CID 0x9C) is in mode 0x10, also hands them to its sub-behaviour. */
extern "C" void SetStepguardEscortPoint(STEPGUARD *guard, LO *object, qword qw)
{
    BHV156 *behavior;
    LO *escort;
    BHVARG *pbhvSub;
    guard->escortObject = object;
    guard->escortPos = qw;
    behavior = (BHV156 *)FindBhvInBhvstk(&guard->behaviors, CID_BHVGUARD);
    if (behavior == 0)
        return;
    escort = guard->escortObject;
    if ((escort != 0) && (behavior->mode == 0x10)) {
        pbhvSub = (BHVARG *)behavior->pbhvNext;
        pbhvSub->escortObject = escort;
        pbhvSub->qw = guard->escortPos;
    }
}

// 0x20A168 CompareStepguardStimuli
namespace M_func_0020A168 {

qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");

// VU0 helper: squared distance from an object's position to a point.
extern "C" { static inline float DistSq(void *object, qword *point)
{
    qword a = GetLoWorldPos(object), d, r, t;
    float f;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(*point));
    __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(r), "=&j"(t) : "0"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(r));
    return f;
} }

// 0x20A168: Compares two stimuli for priority: template kind (lower first), level (higher first),
// time (later first), then distance (nearer first); finally address order.
// Also known as: NCompareNoises (upstream readable / Sly 1 name; NCompareNoises__FP5GUARDP5NOISET1).
extern "C" int CompareStepguardStimuli(void *guard, struct NOISE *a, struct NOISE *b)
{
    int diff;
    float distA, distB;
    diff = a->type->kind - b->type->kind;
    if (diff)
        return diff;
    diff = b->level - a->level;
    if (diff)
        return diff;
    if (a->startTime > b->startTime)
        return -1;
    if (a->startTime < b->startTime)
        return 1;
    distA = DistSq(guard, &a->position);
    distB = DistSq(guard, &b->position);
    if (distA < distB)
        return -1;
    if (distB > distA)
        return 1;
    return a - b;
}
} // namespace M_func_0020A168

// 0x20A298 IsStepguardTargetValid
namespace M_func_0020A298 {
struct ENTRY; 
extern float D_002C3DAC __asm__("D_002C3DAC");
struct ENTRY { void *object; int state; float stateTime; float lastSeenTime; /* (inferred) */ };
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
long GetSgfMask(int n) __asm__("GetSgfMask");
struct ENTRY *FindDetectorTarget(void *detector, void *object) __asm__("FindDetectorTarget");
// 0x20A298: True if a target is still valid for this guard (Murray special case, detector entry age and mask)
// (inferred).
// Also known as: FGuardSurprised (upstream readable / Sly 1 name; FGuardSurprised__FP5GUARDP2LOUl).
int IsStepguardTargetValid(STEPGUARD *guard, void *target, long mask) __asm__("IsStepguardTargetValid"); int IsStepguardTargetValid(STEPGUARD *guard, void *target, long mask)
{
    struct ENTRY *entry;
    // (Shared return labels: direct returns reorder the blocks and do not match.)
    if (guard->unknown3D0)
        goto ret0;
    if (IsBasicDerivedFrom__FP5BASIC3CID(target, CID_MURRAY) && !IsBasicDerivedFrom__FP5BASIC3CID(guard, CID_GUARD) && !(guard->grfsg & GetSgfMask(6)))
        goto ret1;
    entry = FindDetectorTarget(guard->pdetector, target);
    if (!entry)
        goto ret1;
    if (!entry->state || g_clock.time - entry->lastSeenTime < D_002C3DAC)
        goto ret1;
    if (!(guard->grfsg & mask))
        goto ret0;
ret1:
    return 1;
ret0:
    return 0;
}
} // namespace M_func_0020A298

// 0x20A378 IsStepguardTargetValidEx
// 0x20A378: IsStepguardTargetValid with extra cases for Murray and cids 0x3A/0x3D.
// Also known as: FGuardLaunchAllowed (upstream readable / Sly 1 name; FGuardLaunchAllowed__FP5GUARDP2LOi).
extern "C" int IsStepguardTargetValidEx(LO *object, LO *ploOther, int n)
{
    if ((IsBasicDerivedFrom((BASIC *)ploOther, (CID)0x36) != 0) && (IsBasicDerivedFrom((BASIC *)object, (CID)0x3A) == 0) &&
        (IsBasicDerivedFrom((BASIC *)object, (CID)0x3D) == 0) && (object != D_002DD648)) {
        return 1;
    }
    return IsStepguardTargetValid(object, ploOther, n);
}

// 0x20A408 ChooseStepguardStimulus
namespace M_func_0020A408 {

extern float D_002C3DB0 __asm__("D_002C3DB0");
int CompareStepguardStimuli(STEPGUARD *guard, NOISE *a, NOISE *b) __asm__("CompareStepguardStimuli");
DETECTORTARGET *FindDetectorTarget(DETECTOR *detector, LO *object) __asm__("FindDetectorTarget");
void SetDetectorTargetState(DETECTOR *detector, DETECTORTARGET *entry, int state) __asm__("SetDetectorTargetState");

// 0x20A408: Chooses the most important stimulus this guard can sense from the world's stimulus list; a stimulus
// already tracked re-marks the detector entry lost.
}
int AreStepguardSensesEnabled(STEPGUARD *guard) __asm__("AreStepguardSensesEnabled");
// Also known as: PloGuardInterest (upstream readable / Sly 1 name; PloGuardInterest__FP5GUARDP2LO).
extern "C" NOISE *ChooseStepguardStimulus(STEPGUARD *guard, NOISE *fallback)
{
    using namespace M_func_0020A408;
    NOISE *stimulus, *best;
    int unused[4];
    if (!AreStepguardSensesEnabled(guard))
        return fallback;
    best = fallback;
    for (stimulus = (NOISE *)g_sw.dlStimulus.head; stimulus; stimulus = stimulus->next) {
        if (guard->CanSenseStimulus(stimulus)) {   /* CanStepguardSenseStimulus */
            if (!best || CompareStepguardStimuli(guard, stimulus, best) < 0)
                best = stimulus;
        }
    }
    if (best && best->level == 4) {
        DETECTORTARGET *entry = FindDetectorTarget(guard->pdetector, best->subject);
        if (entry) {
            if (!entry->state) {
                SetDetectorTargetState(guard->pdetector, entry, 2);
                guard->nextAttackTime = g_clock.time + D_002C3DB0;
            }
            return fallback;
        }
    }
    return best;
}
// (end of ChooseStepguardStimulus: M_func_0020A408)

// 0x20A518 AddStepguardAllowedPathZone
// 0x20A518: Splice option: adds a path zone to the guard's allowed-zone list at +0xE94 (at most 8; ignored when
// full). CanUsePathlink (path.cpp) reads the same list: when it is non-empty, route planning only uses path links
// whose two end zones are on it (inferred: confines the guard to these zones).
extern "C" void AddStepguardAllowedPathZone(STEPGUARD *guard, int zone)
{
    if ((unsigned int)guard->numAllowedZones < 8)
        guard->allowedZones[guard->numAllowedZones++] = zone;
}

// 0x20A540 SetStepguardPatrolScript
namespace M_func_0020A540 {
void *func_002333B4(void *p, int c, int n) __asm__("memset");
void SetBhvguardMode(BHV156 *bhv, BHVGUARDRECDATA *mode) __asm__("SetBhvguardMode");
// 0x20A540: Sets the patrol script (+0x2E8) and restarts the root behaviour if it is patrolling.
// Also known as: SetGuardPatrol (upstream readable / Sly 1 name; SetGuardPatrol__FP5GUARDi).
extern "C" void SetStepguardPatrolScript(STEPGUARD *guard, GUARDSCRIPT *script)
{
    if (guard->patrolScript == script)
        return;
    BHV156 *bhv;
    guard->patrolScript = script;
    func_002333B4(&guard->scriptResumePoint, 0, 0xC);
    bhv = (BHV156 *)FindBhvInBhvstk(&guard->behaviors, CID_BHVGUARD);
    if (bhv) {
        int modeKind = bhv->mode;
        if (modeKind == 1) {   /* patrolling: leave the mode and re-enter it so the new script starts */
            BHVGUARDRECDATA mode;
            func_002333B4(&mode, 0, sizeof(mode));
            mode.kind = -1;
            SetBhvguardMode(bhv, &mode);
            func_002333B4(&guard->scriptResumePoint, 0, 0xC);
            mode.kind = modeKind;
            SetBhvguardMode(bhv, &mode);
        }
    }
}
} // namespace M_func_0020A540

// 0x20A5F8 InitTargetRecord
// 0x20A5F8: Initializes a shared per-target engagement record (0xAC bytes) and links it on the world's list.
/* Initialises a target record for plo and appends it to the record list g_sw.dlTargetrec. */
extern "C" TARGETREC *InitTargetRecord(TARGETREC *record, LO *object)
{
    record->target = object;
    record->unknown14 = 1.0f;
    record->facingTolerance = 2.0f;
    record->facingKind = -1;
    record->state = -1;
    AppendDlEntry(&g_sw.dlTargetrec, record);
    return record;
}

// 0x20A658 FindTargetRecordGuardIndex
// 0x20A658: Returns the guard's index in an engagement record's guard list (-1 if absent).
extern "C" int FindTargetRecordGuardIndex(TARGETREC *record, STEPGUARD *guard)
{
    int i;
    for (i = 0; i < record->guardCount; i++)
        if (guard == record->guards[i])
            return i;
    return -1;
}

// 0x20A690 GetTargetRecordFacing
namespace M_func_0020A690 {
struct CAMERA;
struct CAMERA { char pad[0x60]; float x; float y; };   /* +0x60: the camera's forward direction (inferred) */
float atan2f(float y, float x) __asm__("atan2f");
// 0x20A690: Heading reference of an engagement record: the camera's (kind 0) or the target's facing (kind 1).
// Also known as: RadTargetrec (upstream readable / Sly 1 name; RadTargetrec__FP9TARGETREC).
extern "C" float GetTargetRecordFacing(TARGETREC *record)
{
    float heading = 0.0f;
    switch (record->facingKind) {
    case 0:
        heading = atan2f(g_cm.mat.mat[0][1], g_cm.mat.mat[0][0]);   /* the camera's forward direction (row 0) */
        break;
    case 1: {
        MATRIX4 *forwardY = record->target->GetWorldMatrix();   /* the target's facing direction (slot 31) */
        MATRIX4 *forwardX = record->target->GetWorldMatrix();
        heading = atan2f(forwardY->mat[0][1], forwardX->mat[0][0]);
        break;
    }
    case -1:
        break;
    }
    return heading;
}
} // namespace M_func_0020A690

// 0x20A730 AddTargetRecordGuard
// 0x20A730: Adds a guard to an engagement record (the first one stamps the time and resets the state).
/* Adds a guard to a target record (stamping the time when it is the first). */
// Also known as: AddTargetrecGuard (upstream readable / Sly 1 name; AddTargetrecGuard__FP9TARGETRECP5GUARD).
extern "C" void AddTargetRecordGuard(TARGETREC *record, STEPGUARD *guard)
{
    int count;
    if (record->guardCount == 0) {
        record->firstEngageTime = g_clock.time;
        SetTargetRecordState(record, 0);
    }
    count = record->guardCount;
    record->guards[count] = guard;
    record->guardCount = count + 1;
}

// 0x20A790 RemoveTargetRecordGuard
// 0x20A790: Removes a guard from an engagement record.
/* Removes a guard from a target record (FindTargetRecordGuardIndex finds its index). */
// Also known as: RemoveTargetrecGuard (upstream readable / Sly 1 name; RemoveTargetrecGuard__FP9TARGETREC).
extern "C" void RemoveTargetRecordGuard(TARGETREC *record, STEPGUARD *guard)
{
    int index;
    int count;
    STEPGUARD **slot;
    index = FindTargetRecordGuardIndex(record, guard);
    count = record->guardCount - 1;
    slot = &record->guards[index];
    record->guardCount = count;
    CopyAb(slot, slot + 1, (count - index) * sizeof(STEPGUARD *));
}

// 0x20A7E0 IsTargetRecordStale
namespace M_func_0020A7E0 { extern float D_002C3DB4 __asm__("D_002C3DB4"); }
// 0x20A7E0: True if the engagement record has not been refreshed for longer than the timeout.
extern "C" int IsTargetRecordStale(TARGETREC *record) { return g_clock.time - record->stateTime > M_func_0020A7E0::D_002C3DB4; }

// 0x20A818 DoesTargetRecordAllowGuard
namespace M_func_0020A818 {
union U; struct Camera;
union U { qword q; float f[4]; };

qword GetLoWorldPos(LO *) __asm__("GetLoWorldPos");
int IsInCameraFrustum(CM *,qword,qword) __asm__("IsInCameraFrustum");
void GetVectorCylind(qword,float *,float *,float *) __asm__("GetVectorCylind");
float NormalizeAngle__Ff(float) __asm__("NormalizeAngle__Ff");

// Matching helper (register-pinned asm): prepares the plane-test operands (threshold bits, camera-relative point).
static inline void PlaneArguments(qword *threshold, qword *point, U *scalar, float f, const qword *camera,
                                  const qword *position)
{
    register qword view asm("vf1"), bits asm("$2"), local asm("vf2"), value asm("$6");
    __asm__("lqc2 %0,%3\n\tmfc1 %1,%4\n\tlqc2 %2,%5"
            : "=j"(view), "=&r"(bits), "=j"(local)
            : "m"(*camera), "f"(f), "m"(*position));
    scalar->q = bits;
    __asm__("por %0,$zero,%2\n\tvsub.xyzw %1,%1,%3" : "=r"(value), "+j"(local) : "r"(scalar->q), "j"(view));
    *threshold = value;
    *point = local;
}
// VU0 helper: loads a quadword into a VU register.
static inline qword Load(const qword *p){qword r;__asm__("lqc2 %0,%1":"=j"(r):"m"(*p));return r;}
// VU0 helper: vector difference.
static inline qword Sub(qword a,qword b){__asm__("vsub.xyzw %0,%0,%1":"=j"(a):"j"(b),"0"(a));return a;}
// 0x20A818: Engagement token: true if this guard may attack the record's target now (not disabled, no other engaged
// guard striking; facing kind 0 needs the guard ready, or in view of the camera and within the tolerance of
// the camera heading as seen from the target; kind 1 within the tolerance of the target's facing).
// (The failures share one `return 0` inside case 0 through `goto fail`: separate returns do not match.)
} // namespace M_func_0020A818
extern "C" float GetTargetRecordFacing(TARGETREC *record);
// Also known as: FTargetrecAllows (upstream readable / Sly 1 name; FTargetrecAllows__FP9TARGETRECP5GUARD).
extern "C" int DoesTargetRecordAllowGuard(TARGETREC *record, STEPGUARD *guard)
{
    using namespace M_func_0020A818;
    U scalar;
    float angle[2];
    U position0, position1;
    if (record->isDisabled)
        goto fail;
    if (!guard->ignoresBusyGuards) {
        int i, n = record->guardCount;
        for (i = 0; i < n; i++)
            if (record->guards[i]->isEngagedBusy)
                goto fail;
    }
    switch (record->facingKind) {
    case 0: {
        if (guard->ready)
            return 1;
        qword returned;
        returned = GetLoWorldPos(guard);
        register CM *camera __asm__("$4");
        __asm__("sq %2,%0\n\tla %1,%3" : "=m"(position0.q), "=r"(camera) : "r"(returned), "i"(&g_cm));
        qword threshold, point;
        PlaneArguments(&threshold, &point, &scalar, 0.0f, (qword *)&camera->pos, &position0.q);
        if (!IsInCameraFrustum(camera, point, threshold))
            goto fail;
        qword other;
        other = GetLoWorldPos(record->target);
        GetVectorCylind(Sub(position0.q, other), &angle[0], 0, 0);
        if (record->facingTolerance < __builtin_fabsf(NormalizeAngle__Ff(angle[0] - GetTargetRecordFacing(record)))) {
fail:
            return 0;
        }
        return 1;
    }
    case 1: {
        position1.q = GetLoWorldPos(guard);
        qword other;
        other = GetLoWorldPos(record->target);
        GetVectorCylind(Sub(position1.q, other), &angle[1], 0, 0);
        if (record->facingTolerance < __builtin_fabsf(NormalizeAngle__Ff(angle[1] - GetTargetRecordFacing(record))))
            return 0;
        return 1;
    }
    case -1:
        break;
    default:
        break;
    }
    return 1;
}

// 0x20A9A8 SetTargetRecordState
// 0x20A9A8: Sets an engagement record's state and its time.
// Also known as: SetTargetrecTimer (upstream readable / Sly 1 name; SetTargetrecTimer__FP9TARGETRECi).
extern "C" void SetTargetRecordState(TARGETREC *record, int state)
{
    record->stateTime = g_clock.time;
    record->state = state;
}

// 0x20A9C0 GetStepguardDefaultAttack
// Appends an attack (oid and resolved object) to the guard's two attack lists.
static __inline__ void PushStepguardAttack(STEPGUARD *guard, int oid, GUARDATTACK *attack)
{
    guard->attacks[guard->numAttacks++] = attack;
    guard->attackOids[guard->attackOidCount++] = oid;
}
// 0x20A9C0: Returns the guard's first attack object, creating the default one (cid 0x78, oid 0x2CC) if it has none.
extern "C" GUARDATTACK *GetStepguardDefaultAttack(STEPGUARD *guard)
{
    GUARDATTACK *attack;
    if (guard->attackOidCount == 0) {
        attack = (GUARDATTACK *)CreateLo((CID)0x78, guard, (OID)OID_DEFAULT_ATTACK, -1);
        PushStepguardAttack(guard, OID_DEFAULT_ATTACK, attack);
    } else {
        attack = guard->attacks[0];
    }
    return attack;
}

// 0x20AA40 AddStepguardAttackOid
// 0x20AA40: Splice option: adds an attack oid (at most 8).
extern "C" void AddStepguardAttackOidV(STEPGUARD *guard, int oid) __asm__("AddStepguardAttackOid");
// Also known as: AddGuardAction (upstream readable / Sly 1 name; AddGuardAction__FP2LOi).
extern "C" void AddStepguardAttackOidV(STEPGUARD *guard, int oid)
{
    if (oid != -1) {
        if ((unsigned int)guard->attackOidCount < 8)
            guard->attackOids[guard->attackOidCount++] = oid;
    }
}

// 0x20AA78 AddAndResolveStepguardAttackOid
// 0x20AA78: Adds an attack oid and resolves the attacks.
extern "C" void AddAndResolveStepguardAttackOid(STEPGUARD *guard)
{
    AddStepguardAttackOid(guard);
    ResolveStepguardAttacks(guard);
}

// 0x20AAA8 ResolveStepguardAttacks
namespace M_func_0020AAA8 {
LO *FindSwObject__FP2SWi3OIDP2LO(void *sw, int k, int oid, STEPGUARD *context) __asm__("FindSwObject__FP2SWi3OIDP2LO");
}
// 0x20AAA8: Resolves the attack oids into attack objects.
extern "C" void ResolveStepguardAttacks(STEPGUARD *guard)
{
    using namespace M_func_0020AAA8;
    int i;
    guard->numAttacks = 0;
    for (i = 0; i < guard->attackOidCount; i++) {
        LO *attack = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, guard->attackOids[i], guard);
        if (attack)
            guard->attacks[guard->numAttacks++] = (GUARDATTACK *)attack;
    }
}

// 0x20AB58 ClearStepguardAttacks
namespace M_func_0020AB58 {
// 0x20AB58: Clears both attack lists.
// Clear both attack lists (attack ids and attacks).
extern "C" void ClearStepguardAttacks(STEPGUARD *guard)
{
    guard->attackOidCount = 0;
    guard->numAttacks = 0;
}
} // namespace M_func_0020AB58

// 0x20AB68 SetStepguardAttack
namespace M_func_0020AB68 {
extern float D_002A88C4 __asm__("D_002A88C4");
float RandomFloatInRange__Fff(float a, float b) __asm__("RandomFloatInRange__Fff");
void SetStepguardAimAct(STEPGUARD *guard, GUARDATTACK *attack, int aimTarget, qword aimOffset) __asm__("SetStepguardAimAct");
}
// 0x20AB68: Switches the current attack: stops the old animation, sets the cooldown (now + random in the attack's
// range), starts the new animation and its aim.
// Also known as: SetGuardResponse (upstream readable / Sly 1 name; SetGuardResponse__FP5GUARDP9ACTIONOBJiG9VU_VECTOR).
extern "C" void SetStepguardAttack(STEPGUARD *guard, GUARDATTACK *attack, int aimTarget, qword aimOffset)
{
    using namespace M_func_0020AB68;
    if (guard->currentAttack) {
        float next;
        StopAnimBinding(guard->currentAttack->activeBinding);
        next = g_clock.time;
        next += RandomFloatInRange__Fff(guard->currentAttack->cooldownMin, guard->currentAttack->cooldownMax);
        guard->currentAttack = 0;
        guard->nextAttackTime = next;
    }
    guard->currentAttack = attack;
    if (attack) {
        StartAnimBinding(attack->activeBinding, guard, attack->activeBinding->flags | 1, 0.0f);
        guard->nextAttackTime = D_002A88C4;   /* no next attack while this one runs */
    }
    SetStepguardAimAct(guard, attack, aimTarget, aimOffset);
}

// 0x20AC28 SetStepguardAimAct
namespace M_func_0020AC28 {
extern qword D_002A2DD0 __asm__("D_002A2DD0");
void ReleaseAct(ACT **pact) __asm__("ReleaseAct");
}
// 0x20AC28: Releases and re-creates the aim act (the act that turns the attack's aim object, e.g. a gun
// arm, toward the target) of the current attack.
// Also known as: SetGuardActionAim (upstream readable / Sly 1 name;
//   SetGuardActionAim__FP5GUARDP9ACTIONOBJiG9VU_VECTOR).
extern "C" void SetStepguardAimAct(STEPGUARD *guard, GUARDATTACK *attack, int aimTarget, qword aimOffset)
{
    using namespace M_func_0020AC28;
    if (guard->aimAttack) {
        ReleaseAct(&guard->aimAct);
        guard->aimAct = 0;
        guard->aimAttack = 0;
    }
    guard->aimAttack = attack;
    if (attack && attack->aimObject) {
        guard->aimAct = NewPositionAct(guard, attack->aimObject, 5, 8, 3, D_002A2DD0);
        guard->aimTarget = aimTarget;
        guard->aimOffset = aimOffset;
    }
}

// 0x20ACC0 UpdateStepguardAim
namespace M_func_0020ACC0 {
 struct AIMOBJECT; struct CID120Island;

struct AIMOBJECT { char pad[0x30]; qword pos; char pad2[0x60]; int unknownA0; };
/* island: the aim object read as an XF with a qword at +0x30 (its matrix row) in an asm operand */
struct CID120Island { char pad[0x90]; qword strikeOffset; struct AIMOBJECT *aimObject; };

qword TransformXfPos(int target, int a, qword offset) __asm__("TransformXfPos");
void BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(MATRIX3 *matrix, qword from, qword to) __asm__("BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3");
qword DecomposeRotationMatrixToEuler(MATRIX3 *matrix) __asm__("DecomposeRotationMatrixToEuler");
// 0x20ACC0: Aims the attack: the rotation that turns the attack's precomputed strike offset toward the
// predicted target point, stored as the aim act's goal (Euler angles).
// Also known as: UpdateGuardActionAim (upstream readable / Sly 1 name; UpdateGuardActionAim__FP5GUARD).
extern "C" void UpdateStepguardAim(STEPGUARD *guard)
{
    MATRIX3 rotation;
    qword aimPoint, direction;
    aimPoint = TransformXfPos(guard->aimTarget, ((struct CID120Island *)guard->aimAttack)->aimObject->unknownA0, guard->aimOffset);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(direction) : "j"(aimPoint), "j"(((struct CID120Island *)guard->aimAttack)->aimObject->pos));
    BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(&rotation, ((struct CID120Island *)guard->aimAttack)->strikeOffset, direction);
    { struct ACT1 *act = ((struct ACT1 *)guard->aimAct); act->goalValue = DecomposeRotationMatrixToEuler(&rotation); }
}
} // namespace M_func_0020ACC0

// 0x20AD38 InitGuardAttack
namespace M_func_0020AD38 { extern float D_002A88C8 __asm__("D_002A88C8"); }
// 0x20AD38: Guard attack (GUARDATTACK, cid 0x78) vtable slot 7 (Init): LO Init, range 0..300, height -5000..5000,
// cooldown 0.2..0.5 s, angle 10 degrees, no oids, empty binding.
void GUARDATTACK::Init()
{
    GUARDATTACK *attack = this;
    LO::Init();
    attack->distance.minValue = 0;
    attack->distance.maxValue = 300.0f;
    attack->height.minValue = -5000.0f;
    attack->height.maxValue = 5000.0f;
    attack->cooldownMin = 0.2f;
    attack->cooldownMax = 0.5f;
    attack->angle = 0.17453295f;   /* 10 degrees */
    attack->isEnabled = 1;
    attack->aimObjectOid = -1;
    attack->effectorOid = -1;
    attack->strikeLabel = -1;
    attack->minLineOfSight = M_func_0020AD38::D_002A88C8;
    ReleaseAnimBinding(&attack->binding, attack->guard);
}

// 0x20ADE8 CloneGuardAttack
// 0x20ADE8: Guard attack vtable slot 41 (Clone): LO clone plus a copy of its binding.
void GUARDATTACK::Clone(LO *ploBase)
{
    LO::Clone(ploBase);
    CloneAnimBinding(&binding);
}

// 0x20AE18 PostGuardAttackLoad
namespace M_func_0020AE18 {
struct EVAL;
struct EVAL { char pad[0x30]; qword pos; };
int IsBasicDerivedFrom__FP5BASIC3CID(LO *object, int cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
float *FindAsegLabel(ASEG *aseg, int label) __asm__("FindAsegLabel");
void PredictAsegTransform(ASEG *aseg, STEPGUARD *root, XF *so, XF *relativeTo, struct EVAL *result, int c, float t, float g) __asm__("PredictAsegTransform");
// 0x20AE18: Guard attack vtable slot 15 (PostLoad): LO post-load, finds its owning guard up the parent chain,
// resolves its binding and, when it has an aim object, precomputes the strike offset: where the
// effector is, relative to the aim object, at the strike label of the attack animation (inferred).
}
// Also known as: PostLoadGuardAction (upstream readable / Sly 1 name; PostLoadGuardAction__FP2LO).
void GUARDATTACK::PostLoad()
{
    using namespace M_func_0020AE18;
    GUARDATTACK *attack = this;
    LO *object;
    LO::PostLoad();
    for (object = attack->parent; object && !IsBasicDerivedFrom__FP5BASIC3CID(object, CID_STEPGUARD); object = object->parent)
        ;
    attack->guard = (STEPGUARD *)object;
    ResolveAnimBinding(&attack->binding, (STEPGUARD *)object);
    if (attack->oid != (OID)OID_DEFAULT_ATTACK)
        attack->activeBinding = &attack->binding;
    if (attack->aimObjectOid != -1) {
        struct EVAL eval;
        ASEG *aseg;
        float *labelTime;
        attack->aimObject = (XF *)FindSoXf(attack->guard, attack->aimObjectOid, 6);
        attack->effector = (XF *)FindSoXf(attack->guard, attack->effectorOid, 6);
        aseg = attack->binding.aseg;
        labelTime = FindAsegLabel(aseg, attack->strikeLabel);
        PredictAsegTransform(aseg, attack->guard, attack->effector, attack->aimObject->parent, &eval, 0, *labelTime, 1.0f);
        attack->strikeOffset = eval.pos;
    }
}
// (end of PostGuardAttackLoad: M_func_0020AE18)

// 0x20AF08 SetGuardAttackAngleDegrees
// 0x20AF08: Splice option: sets the attack angle (given in degrees, stored in radians).
extern "C" void SetGuardAttackAngleDegrees(GUARDATTACK *attack, float degrees) { attack->angle = degrees * 0.01745329424738884f; }

// 0x20AF28 SetGuardAttackOids
// 0x20AF28: Splice option: sets the attack's aim object, effector and strike label (inferred).
extern "C" void SetGuardAttackOids(GUARDATTACK *attack, int aimObjectOid, int effectorOid, int strikeLabel)
{
    attack->aimObjectOid = aimObjectOid;
    attack->effectorOid = effectorOid;
    attack->strikeLabel = strikeLabel;
}

// 0x20AF38 SetGuardAttackBinding
// 0x20AF38: Splice option: sets the attack's animation binding.
extern "C" void SetGuardAttackBinding(GUARDATTACK *attack, OID oid, int flags)
{
    ReleaseAnimBinding(&attack->binding, attack->guard);
    attack->binding.oid = oid;
    attack->binding.flags = flags;
}

// 0x20AF88 SetAndResolveGuardAttackBinding
// 0x20AF88: Sets and resolves the attack's animation binding.
extern "C" void SetAndResolveGuardAttackBinding(GUARDATTACK *attack, OID oid, int flags)
{
    ANIMBINDING *binding;
    binding = &attack->binding;
    ReleaseAnimBinding(binding, attack->guard);
    attack->binding.oid = oid;
    attack->binding.flags = flags;
    ResolveAnimBinding(binding, attack->guard);
}

// 0x20AFF0 SetGuardAttackBindingKind2
namespace M_func_0020AFF0 {
struct PAYLOAD2;
struct PAYLOAD2 { int parameter; };
struct PAYLOAD2 *AllocLeap(int cb) __asm__("AllocLeap");
void InitAsegArc(struct PAYLOAD2 *payload) __asm__("InitAsegArc");
// 0x20AFF0: Splice option: sets the attack's binding to a leap payload.
void SetGuardAttackBindingKind2(GUARDATTACK *attack, int parameter) __asm__("SetGuardAttackBindingKind2"); void SetGuardAttackBindingKind2(GUARDATTACK *attack, int parameter)
{
    ANIMBINDING *binding = &attack->binding;
    ReleaseAnimBinding(binding, attack->guard);
    attack->binding.kind = BINDING_LEAP;
    binding->payload4 = AllocLeap(0x40);
    InitAsegArc((PAYLOAD2 *)binding->payload4);
    ((PAYLOAD2 *)binding->payload4)->parameter = parameter;
    binding->flags = 0;
}
} // namespace M_func_0020AFF0

// 0x20B060 SetAndResolveGuardAttackBindingKind2
// 0x20B060: Sets and resolves the attack's kind-2 binding.
extern "C" void SetAndResolveGuardAttackBindingKind2(GUARDATTACK *attack)
{
    SetGuardAttackBindingKind2(attack);
    ResolveAnimBinding(&attack->binding, attack->guard);
}

// 0x20B090 GetStepguardGroundProbeFlags
namespace M_func_0020B090 {
long GetSgfMask(int n) __asm__("GetSgfMask");
int GetStepGroundProbeFlags(STEP *p) __asm__("GetStepGroundProbeFlags");
// 0x20B090: STEPGUARD vtable slot 85 (ground-probe flags): 1 with flag 0x14, 0 while thrown (flags 0x12/0x13/0x16),
// else the mover driver's or the step's.
// Also known as: GrfGuardXp (upstream readable / Sly 1 name; GrfGuardXp__FP3ALO).
extern "C" int GetStepguardGroundProbeFlags(STEPGUARD *guard)
{
    long thrownMask;
    if (guard->grfsg & GetSgfMask(0x14))
        return 1;
    thrownMask = GetSgfMask(0x12) | GetSgfMask(0x13) | GetSgfMask(0x16);
    if (guard->grfsg & thrownMask)
        return 0;
    if (guard->mover.driver)
        return guard->mover.driver->GetGroundProbeFlags();
    return GetStepGroundProbeFlags(guard);
}
} // namespace M_func_0020B090

// 0x20B140 EmitStepguardStimulus
namespace M_func_0020B140 {
extern float D_002C3DB8 __asm__("D_002C3DB8");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
void CreateNoise(struct NOISETYPE *noiseType, qword pos, STEPGUARD *source, NOISE **result) __asm__("CreateNoise");
NOISE *NewNoise(NOISETYPE *pstimk, qword pos, STEPGUARD *source) __asm__("NewNoise");
// 0x20B140: Emits a stimulus at the guard aimed at `target` (kind 4) from its own noise type or the world default,
// and remembers when (+0xF44).
// Also known as: RaiseGuardAlarm (upstream readable / Sly 1 name; RaiseGuardAlarm__FP5GUARDP2LO).
extern "C" void EmitStepguardStimulus(STEPGUARD *guard, void *target)
{
    NOISE *stimulus;
    qword targetPos = GetLoWorldPos(target);
    stimulus = 0;
    if (guard->noiseType)
        CreateNoise(guard->noiseType, GetLoWorldPos(guard), guard, &stimulus);
    else
        stimulus = NewNoise(g_sw.apstimk[4], GetLoWorldPos(guard), guard);
    stimulus->targetPosition = targetPos;
    stimulus->subject = (LO *)target;
    stimulus->level = 4;
    guard->alertCooldownEnd = g_clock.time + D_002C3DB8;
}
} // namespace M_func_0020B140

// 0x20B210 DetachStepguard
namespace M_func_0020B210 {

void LinkStepAttachment(STEPGUARD *guard, LO *object, int attach) __asm__("LinkStepAttachment");
// 0x20B210: Detaches the guard from what it is attached to (no attach kind or object).
// (One case per attach kind, each with its own call: merging the identical cases changes the jump table.)
// Also known as: DetachGuardMove (upstream readable / Sly 1 name; DetachGuardMove__FP5GUARD).
extern "C" void DetachStepguard(STEPGUARD *guard)
{
    switch (guard->attachKind) {
    case 0:
        LinkStepAttachment(guard, guard->attachObject, 0);
        break;
    case 1:
        LinkStepAttachment(guard, guard->attachObject, 0);
        break;
    case 2:
        LinkStepAttachment(guard, guard->attachObject, 0);
        break;
    case 3:
        LinkStepAttachment(guard, guard->attachObject, 0);
        break;
    }
    guard->attachObject = 0;
    guard->attachKind = -1;
}
} // namespace M_func_0020B210

// 0x20B2A8 AttachStepguard
namespace M_func_0020B2A8 {
struct ATTACHOBJ;
struct ATTACHOBJ { char pad[0x124]; int unknown124; };   /* the cid 0x13 class's mode word (not a guard type) */
int IsBasicDerivedFrom__FP5BASIC3CID(LO *object, int cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
void FindShapeClosestPoint(LO *shape, qword pos, void *a, qword *b, qword *c, float *d, float *param) __asm__("FindShapeClosestPoint");
void LinkStepAttachment(STEPGUARD *guard, LO *object, int attach) __asm__("LinkStepAttachment");

// 0x20B2A8: Attaches the guard to an object, choosing the attach kind by class: pipe (cid 0xB), rail (cid 0xD), cid
// 0x13, cid 0x27 (inferred nouns).
// Also known as: AttachGuardMove (upstream readable / Sly 1 name; AttachGuardMove__FP5GUARDP2LOi).
extern "C" void AttachStepguard(STEPGUARD *guard, LO *object, int arg)
{
    guard->attachObject = object;
    guard->isMovingForward = arg;
    if (IsBasicDerivedFrom__FP5BASIC3CID(object, CID_PIPE)) {
        guard->attachKind = 1;
        FindShapeClosestPoint(guard->attachObject, GetLoWorldPos(guard->attachPoint), 0, 0, 0, 0, &guard->attachParam);
        LinkStepAttachment(guard, guard->attachObject, 1);
    } else if (IsBasicDerivedFrom__FP5BASIC3CID(guard->attachObject, CID_RAIL)) {
        guard->attachKind = 0;
        FindShapeClosestPoint(guard->attachObject, GetLoWorldPos(guard), 0, 0, 0, 0, &guard->attachParam);
        LinkStepAttachment(guard, guard->attachObject, 1);
    } else if (IsBasicDerivedFrom__FP5BASIC3CID(guard->attachObject, CID_CLASS19) && ((struct ATTACHOBJ *)guard->attachObject)->unknown124 == 3) {
        guard->attachKind = 2;
        LinkStepAttachment(guard, guard->attachObject, 1);
    } else if (IsBasicDerivedFrom__FP5BASIC3CID(guard->attachObject, CID_CLASS39)) {
        guard->attachKind = 3;
        LinkStepAttachment(guard, guard->attachObject, 1);
    } else {
        guard->attachObject = 0;
    }
}
} // namespace M_func_0020B2A8

// 0x20B3D0 GetStepguardHealthState
namespace M_JthsCurrentStepguard {

unsigned long GetSgfMask(int n) __asm__("GetSgfMask");
// 0x20B3D0: STEPGUARD vtable slot 77 (health state): 2 dead (flag 7), 1 hurt (flags 5/6), else 0.
// Also known as: NGuardStatus (upstream readable / Sly 1 name; NGuardStatus__FP5GUARD).
extern "C" int GetStepguardHealthState(STEPGUARD *guard)
{
    if (guard->grfsg & GetSgfMask(SGF_KO))
        return 2;
    if (guard->grfsg & (GetSgfMask(5) | GetSgfMask(6)))
        return 1;
    return 0;
}
} // namespace M_JthsCurrentStepguard

// 0x20B438 IncrementStepguardBusy
// 0x20B438: Increments the guard's busy count; the first one marks it busy (busy level 1).
/* Busy count up; the first marks the guard busy. */
// Also known as: LockGuardActivation (upstream readable / Sly 1 name; LockGuardActivation__FP5GUARDi).
extern "C" int IncrementStepguardBusy(STEPGUARD *guard)
{
    int count;
    if (guard->busyCount == 0) {
        SetLoBusyKind(guard, 1);
    }
    count = guard->busyCount + 1;
    guard->busyCount = count;
    return count;
}

// 0x20B478 DecrementStepguardBusy
// 0x20B478: Decrements the busy count; at 0 it is no longer busy.
// Also known as: UnlockGuardActivation (upstream readable / Sly 1 name; UnlockGuardActivation__FP5GUARDi).
extern "C" void DecrementStepguardBusy(STEPGUARD *guard)
{
    int count;
    count = guard->busyCount - 1;
    guard->busyCount = count;
    if (count == 0) {
        SetLoBusyKind(guard, 0);
    }
}

// 0x20B4A8 InitGuardManager
// 0x20B4A8: Guard manager (GUARDMGR, cid 0x7D) vtable slot 7 (Init): LO Init, flag 2, registered for updates.
void GUARDMGR::Init()
{
    GUARDMGR *manager = this;
    sptr flags;
    LO::Init();
    flags = (sptr)manager->ploupd;
    if (!(flags & 1))
        ((LOUPD *)flags)->grf |= 2;   // (bit 0 clear: the word points at the real flags)
    else
        manager->inlineFlags = flags | 2;
    SetLoBusyKind(manager, 1);
}

// 0x20B500 PostGuardManagerLoad
namespace M_func_0020B500 {
typedef struct ALLOCREQ { int cb; int nKind; } ALLOCREQ;
int FindSwObjectsByClass(void *sw, int a, int cid, int c, int maxCount, STEPGUARD **objects) __asm__("FindSwObjectsByClass");
void *AllocFromWorldHeap(ALLOCREQ *request) __asm__("AllocFromWorldHeap");
}
// 0x20B500: Guard manager vtable slot 15 (PostLoad): lists every guard (cid 0x38) in the world.
// Also known as: PostLoadGuardManager (upstream readable / Sly 1 name; PostLoadGuardManager__FP2LO).
void GUARDMGR::PostLoad()
{
    using namespace M_func_0020B500;
    GUARDMGR *manager = this;
    ALLOCREQ request;
    LO::PostLoad();
    manager->guardCount = FindSwObjectsByClass(&g_sw, 0x605, CID_STEPGUARD, 0, 0, 0);
    request.cb = manager->guardCount * sizeof(STEPGUARD *);
    request.nKind = 0x10;
    manager->guards = (STEPGUARD **)AllocFromWorldHeap(&request);
    FindSwObjectsByClass(&g_sw, 0x405, CID_STEPGUARD, 0, manager->guardCount, manager->guards);
}

// 0x20B590 UpdateGuardManager
namespace M_func_0020B590 {
extern float D_002C3DBC __asm__("D_002C3DBC");
extern const float D_002A88CC __asm__("D_002A88CC");
void *func_002333B4(void *p, int c, int n) __asm__("memset");
void SetBhvguardMode(void *bhv, BHVGUARDRECDATA *mode) __asm__("SetBhvguardMode");
void ResetDetectorTargets(DETECTOR *detector) __asm__("ResetDetectorTargets");
}
// 0x20B590: Guard manager vtable slot 16 (Update): for guards removed from the world for more than 0.25 s: mode 1
// removes them, mode 2 restarts their root behaviour and resets their detector.
// Also known as: UpdateGuardManager (upstream readable / Sly 1 name; UpdateGuardManager__FP2LO).
void GUARDMGR::Update()
{
    using namespace M_func_0020B590;
    GUARDMGR *manager = this;
    int i, count;
    STEPGUARD **guards;
    LO::Update();
    count = manager->guardCount;
    guards = manager->guards;
    for (i = 0; i < count; i++) {
        STEPGUARD *guard = guards[i];
        if (((int)guard->isInWorld & 1) && g_clock.time - guard->removalTime > D_002C3DBC) {
            switch (guard->removalMode) {
            case 0:
                break;
            case 1:
                guard->Remove();
                break;
            case 2: {
                void *bhv = FindStepBehavior(guard, CID_BHVGUARD);
                if (bhv) {
                    BHVGUARDRECDATA mode;
                    func_002333B4(&mode, 0, sizeof(mode));
                    mode.kind = 1;
                    SetBhvguardMode(bhv, &mode);
                }
                if (guard->pdetector)
                    ResetDetectorTargets(guard->pdetector);
                break;
            }
            }
            guard->removalTime = D_002A88CC;
        }
    }
}


// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
extern const float D_002A88C4 = 3.40282347e+38f;
extern const float D_002A88C8 = 3.40282347e+38f;
extern const float D_002A88CC = 3.40282347e+38f;

  /* 3.59e-43f 2.123e-42f 5.218e-42f */



  /* cids */









  /* 4e-45f 2.8e-44f 1.7e-44f 3.1e-44f */






ulong D_003DD480;
int D_003DD488;
LO *D_003DD48C;

// ---- stepguard's .data in address order (tu-merging.md "Phase 6: one link mode"): one block after the functions (definition order is emission order); the declarations above are what the functions see.
int D_002C3CF0 = 0;
int D_002C3CF8[3] = {256, 1515, SLY2_OFFSETOF(STEPGUARD, sleepDartAseg)};
float D_002C3D04 = 1500.0f;
float D_002C3D08 = 1000.0f;
float D_002C3D0C = 250.0f;
int D_002C3D10[5] = {59, 64, 61, 62, 65};
VECTOR4 D_002C3D30 = {0.5f, 0.5f, 0.5f, 0.0f};
float D_002C3D40[3] = {10.0f, 30.0f, 5.0f};
float D_002C3D4C = 80.0f;
float D_002C3D50[3] = {1.0f, -2.0f, 1.0f};
float D_002C3D60[2] = {-0.08f, 0.05f};
float D_002C3D70[3] __attribute__((aligned(16))) = {-400.0f, 0.8f, 0.0f};
float D_002C3D80[3] = {-500.0f, 0.8f, 0.0f};
float D_002C3D90 __attribute__((aligned(16))) = -500.0f;
float D_002C3D94 = 0.7f;
int D_002C3D98[4] = {3, 20, 12, 22};
float D_002C3DA8 = 800.0f;
float D_002C3DAC = 0.2f;
float D_002C3DB0 = 2.0f;
float D_002C3DB4 = 5.0f;
float D_002C3DB8 = 1.0f;
float D_002C3DBC = 0.25f;

#include "sly2/port.h"
#if !defined(SLY2_EE_LAYOUT)
/* Native adapters for SLY2_SRET_LABEL (sly2/port.h; docs/guides/portability.md "Struct results and arm64"). The EE
   passes a struct result's address as a hidden first argument, so the declarations elsewhere that spell this file's
   functions with the other form (a by-value result for an explicit out-pointer, or the reverse) call the same code
   there; natively they bind to these entries, which have their spelling. Plain records with the definitions' sizes. */
/* g_aeopid calls the getter as CRef (BASIC::*)() (otyp 0x9010; spliceobj.cpp GetOptionRecordValue). */
extern "C" CRef GetStepguardSgfRefListValue(STEPGUARD *guard)
{ CRef r; M_func_00209A88::GetStepguardSgfRefList(&r, guard); return r; }
#endif
