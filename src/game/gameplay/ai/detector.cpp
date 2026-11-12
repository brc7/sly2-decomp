// detector.cpp -- DETECTOR, a guard's vision detector (cid 0x10, an XFM). It tracks up to eight targets, each with
// a state (0 unseen, 1 seen, 2 lost) recomputed every frame from its near/far volumes, line of sight through the
// world's occluders and objects, and the target's own condition (hidden, disguised, invisible). State changes are
// sent to the detector (message 0x1C) and, when the hero becomes seen, broadcast as game event 0x13. Entry points:
// InitDetector, UpdateDetectorTargets (per frame), Add/RemoveDetectorTarget, SetDetectorTargetState(ById),
// GetDetectorTargetStateById, CalculateDetectorTargetState. VU0 asm.
// TU 0x153D58..0x154A60 (19 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_detector
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_detector
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/stackscope.h"
#include "sly2/sw.h"
#include "sly2/clock.h"
#include "sly2/classes.h"
#include "sly2/game.h"
#include "sly2/lsg.h"   /* LSG: the line clip's span records */

// 0x154908 StaticInit_detector and GlobalConstructors_detector: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

typedef union { qword q; float f[4]; } U;
/* Class ids tested here (docs/systems/object-model.md section 1). */
enum { CID_MS = 0x31, CID_PO = 0x32, CID_STEP = 0x33, CID_STEPPLAYER = 0x34, CID_STEPGUARD = 0x38, CID_SAFEHOUSE = 0x46,
       CID_COVER_C = 0xC, CID_COVER_22 = 0x22 };
/* DETECTOR::options bits. */
enum {
    DETECTOR_SKIP_CONCEALMENT = 1,   // ignore the target's concealment (cover/sneak) state
    DETECTOR_NEAR_IS_ENOUGH = 2,     // inside the near volume counts as seen, no line test
    DETECTOR_FILTER_MS = 4,          // the line test only counts MS objects (cid 0x31) as blockers
    DETECTOR_RETEST_LINE = 8         // re-test the line of sight every frame
};
/* Target detection states (DETECTORTARGET::state). */
enum { TARGET_UNSEEN = 0, TARGET_SEEN = 1, TARGET_LOST = 2 };
/* The detector is guard.h DETECTOR, its targets DETECTORTARGET. */
/* A target: any LO; the player-only members (STEP::pivotOffset, STEPPLAYER's jtgrab / isHidden) are read after the
   class tests. */
typedef STEPPLAYER SOV;
struct BUF { char a[4 * sizeof(LSG)]; };   /* four sly2/lsg.h LSGs */
/* Line-test callback arguments (FilterDetectorLineOfSightHit/FilterDetectorHitClass31). */
/* The hits are SOPARTs; their owners SOs with a physics tree (SOTREE). */
struct ARGS { DETECTORTARGET *entry; int *oldState; int *newState; };
// Allocation request: byte count and alignment.
struct AL { int cb; int cbAlign; AL(int c, int a) { cb = c; cbAlign = a; } };
// Vector with a trivial copy constructor.
struct VECTORQ { qword q; VECTORQ() {} VECTORQ(const VECTORQ &v) : q(v.q) {} };

extern qword g_normalZ;
extern "C" {
qword D_002BDBF0[1] = {0};   /* detector's .data: the zero position (16 bytes) */
void InitXfm(DETECTOR *object);
void SetLoBusyKind(DETECTOR *object, int n);
void UpdateLo(DETECTOR *object);
qword GetLoWorldPos(void *object);
int IsBasicDerivedFrom__FP5BASIC3CID(void *object, int state);
void BroadcastGameEvent(void *object, int n, void *q);
void memmove(void *dst, void *src, int cb);
SOV *GetHeroPo(void *object);
int IsLoInWorld(SOV *object);
int IsSwInputLocked(void *object);
qword ConvertPosBetweenLos(void *plo0, void *plo1, qword v);
int ClipSegmentBsp(void *object, uptr mask, qword a, qword b, int n, BUF *pbuf);
int IsPointInVol(void *object, qword v);
int ShouldKeepSoNotSelf(void *context, SOPART *hit);
int GetBasicCid(SO *po);
void *AllocFromStack(AL *request);
void IntersectSwBoundingBox(void *g, qword a, qword b, void *pfn, void *o, int *pcObject, SOPART ***papObject);
int FindFirstObjectHitByLine(int flags, qword a, qword b, int cObject, SOPART **apObject, void *phit);
VECTORQ *GetLoWorldMatrix(SOV *object);
void UpdateDetectorTargets(DETECTOR *object);
qword GetDetectorEyePosition(DETECTOR *object);
void AddDetectorTarget(DETECTOR *object, void *target);
void SetDetectorTargetState(DETECTOR *object, DETECTORTARGET *entry, int state);
DETECTORTARGET *FindDetectorTarget(DETECTOR *object, void *target);
int CalculateDetectorTargetState(DETECTOR *object, SOV *target, int state);
int FilterDetectorHitClass31(void *context, SOPART *hit);
int FilterDetectorLineOfSightHit(DETECTOR *object, SOPART *hit);
int IsDetectorLineBlocked(DETECTOR *object, SOV *target);
int IsDetectorTargetConcealed(DETECTOR *object, SOV *target);
}

// 0x153D58: DETECTOR vtable slot 7 (Init): XFM Init, registered for updates (busy level 1), then sets object flag
// 2.
extern "C" void InitDetector(DETECTOR *detector)
{
    uptr flags;
    InitXfm(detector);
    SetLoBusyKind(detector, 1);
    flags = (uptr)detector->ploupd;
    if (!(flags & 1)) ((LOUPD *)flags)->grf |= 2;   // (bit 0 clear: the word points at the real flags)
    else detector->inlineFlags = flags | 2;
}

// 0x153DB0: DETECTOR vtable slot 15 (Update): LO update, then re-evaluates the targets.
// Also known as: UpdateDetectorObject (upstream readable / Sly 1 name; UpdateDetectorObject__FP2LOf).
void DETECTOR::Update()
{
    DETECTOR *detector = this;
    UpdateLo(detector);
    UpdateDetectorTargets(detector);
}

// 0x153DE0: Re-evaluates every tracked target: its new state comes from CalculateDetectorTargetState(target, old
// state) and is applied through SetDetectorTargetState (which notifies on change).
// Also known as: UpdateDetector (upstream readable / Sly 1 name; UpdateDetector__FP8DETECTOR).
extern "C" void UpdateDetectorTargets(DETECTOR *detector)
{
    int i;
    for (i = 0; i < detector->ctarget; i++) {
        DETECTORTARGET *entry = &detector->atarget[i];
        SetDetectorTargetState(detector, entry, CalculateDetectorTargetState(detector, (SOV *)entry->object, entry->state));
    }
}

// 0x153E60: Returns the detector's eye position: its own position when it has no eye object, else the eye object's
// position, raised 80 units when that object is a guard (STEPGUARD family, cid 0x38).
// Also known as: PosDetectorEye (upstream readable / Sly 1 name; PosDetectorEye__FP8DETECTOR).
extern "C" qword GetDetectorEyePosition(DETECTOR *detector)
{
    if (!detector->owner) return GetLoWorldPos(detector);
    if (IsBasicDerivedFrom__FP5BASIC3CID(detector->owner, CID_STEPGUARD)) {
        U height;
        qword heightVector;   /* the asm writes a scalar, stored into the union after (see "asm output" in the merge guide) */
        qword result;
        qword eyeObjectPos = GetLoWorldPos(detector->owner);
        float raise = 80.0f;
        heightVector = VuFromFloat<qword>(raise);
        height.q = heightVector;
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(result) : "j"(eyeObjectPos), "j"(g_normalZ), "j"(height.q));
        return result;
    }
    return GetLoWorldPos(detector->owner);
}

// 0x153EF8: When the tracked object changes (`oldObject` != `newObject`), adds the new one as a target (inferred
// use: target hand-over).
extern "C" void UpdateDetectorTrackedObject(DETECTOR *detector, void *oldObject, void *newObject)
{
    if (oldObject != newObject) {
        AddDetectorTarget(detector, newObject);
    }
}

// 0x153F18: Adds an object to the target list (at most eight) in state 0, unless it is already tracked.
// Also known as: AddDetectorTarget (upstream readable / Sly 1 name; AddDetectorTarget__FP8DETECTORP2LO).
extern "C" void AddDetectorTarget(DETECTOR *detector, void *object)
{
    if (FindDetectorTarget(detector, object) != 0)
        return;
    unsigned int count = detector->ctarget;
    if (count < 8U) {   // at most eight targets
        DETECTORTARGET *entry;
        detector->ctarget = count + 1;
        entry = &detector->atarget[count];
        entry->object = (LO *)object;
        SetDetectorTargetState(detector, entry, 0);
    }
}

// 0x153F80: Removes an object from the target list: its state is forced to 0 first (so listeners see the change),
// then the array is compacted.
// Also known as: RemoveDetectorTarget (upstream readable / Sly 1 name; RemoveDetectorTarget__FP8DETECTORi).
extern "C" void RemoveDetectorTarget(DETECTOR *detector, void *object)
{
    DETECTORTARGET *entry = FindDetectorTarget(detector, object);
    if (entry) {
        SetDetectorTargetState(detector, entry, 0);
        detector->ctarget--;
        memmove(entry, entry + 1, (&detector->atarget[detector->ctarget] - entry) * sizeof(DETECTORTARGET));
    }
}

// 0x153FF8: Forgets every target's detection: all states become 0, stamped with the current time (no
// notifications).
extern "C" void ResetDetectorTargets(DETECTOR *detector)
{
    int i;
    for (i = 0; i < detector->ctarget; i++) {
        detector->atarget[i].state = 0;
        detector->atarget[i].stateTime = g_clock.time;
    }
}

// 0x154038: Looks up a target by object and sets its detection state.
extern "C" void SetDetectorTargetStateById(DETECTOR *detector, void *object, int state)
{
    DETECTORTARGET *entry = FindDetectorTarget(detector, object);
    if (entry != 0) {
        SetDetectorTargetState(detector, entry, state);
    }
}

// 0x154080: Changes a target's state: records when it left state 0 and when it changed, notifies the detector
// through vtable slot 3 (HandleSpliceEvent, message 0x1C with {entry, &old, &new}) and, when the detector's parent
// is a STEP (cid 0x33: a player or a guard) and the new state is TARGET_SEEN, broadcasts game event 0x13.
// Also known as: SetDetentryState (upstream readable / Sly 1 name; SetDetentryState__FP8DETECTORP8DETENTRYi).
extern "C" void SetDetectorTargetState(DETECTOR *detector, DETECTORTARGET *entry, int newState)
{
    ARGS args;
    if (!entry || newState == entry->state)
        return;
    if (entry->state == TARGET_UNSEEN)
        entry->detectTime = g_clock.time;
    args.entry = entry;
    args.oldState = &entry->state;
    args.newState = &newState;
    detector->HandleSpliceEvent(0x1C, 0, 3, (void **)&args, 0);
    entry->state = newState;
    entry->stateTime = g_clock.time;
    if (newState == TARGET_SEEN) {
        void *parent = detector->parent;
        if (parent && IsBasicDerivedFrom__FP5BASIC3CID(parent, CID_STEP))
            BroadcastGameEvent(&g_game, 0x13, parent);
    }
}

// 0x154158: Returns the detection state of an object's target entry, or -1 if it is not tracked.
// Also known as: X509v3_data_type_by_NID (upstream readable / Sly 1 name).
extern "C" int GetDetectorTargetStateById(DETECTOR *detector, void *object)
{
    DETECTORTARGET *entry = FindDetectorTarget(detector, object);
    if (entry)
        return entry->state;
    return -1;
}

// 0x154180: Finds the target entry for an object (NULL for a null object or an untracked one).
// Also known as: PdetentryFind (upstream readable / Sly 1 name; PdetentryFind__FP8DETECTORi).
extern "C" DETECTORTARGET *FindDetectorTarget(DETECTOR *detector, void *object)
{
    int i;
    if (!object)
        return 0;
    for (i = 0; i < detector->ctarget; i++) {
        DETECTORTARGET *entry = &detector->atarget[i];
        if (entry->object == object)
            return entry;
    }
    return 0;
}

// VU0 helper: vector sum.
// 0x1541D8: Computes the new detection state (0 unseen, 1 seen, 2 lost) of one target from its current one: unseen
// if the target is gone, dead, disguised against this hero, globally invisible, disabled or flagged hidden; seen if
// the detector is forced; else blocked by any world occluder BSP on the eye line -> unseen; then by state: 0 ->
// seen if inside the near volume (and far volume), not concealed and the line is clear; 1 -> lost when leaving the
// near volume; 2 -> seen again when back in the near volume with a clear line. Flag 8 re-tests the line every
// frame.
// Also known as: NDetectState (upstream readable / Sly 1 name; NDetectState__FP8DETECTORP2LOi).
extern "C" int CalculateDetectorTargetState(DETECTOR *detector, SOV *target, int state)
{
    BUF hitBuffer;
    qword targetPos;
    CID23 *occluder;   /* the occluder volumes, g_sw.dlCid23 */
    // (Single exit through `done`: returning directly keeps the result in $v0 instead of a saved register and
    // no longer matches.)
    SOV *hero = GetHeroPo(&g_sw);
    if (!target) { state = TARGET_UNSEEN; goto done; }
    if (!IsLoInWorld(target)) { state = TARGET_UNSEEN; goto done; }
    if (IsBasicDerivedFrom__FP5BASIC3CID(target, CID_PO) && ((PO *)target)->GetHealthState() == 2) { state = TARGET_UNSEEN; goto done; }
    if (IsBasicDerivedFrom__FP5BASIC3CID(target, CID_STEPPLAYER) && hero && IsBasicDerivedFrom__FP5BASIC3CID(hero, CID_SAFEHOUSE)) { state = TARGET_UNSEEN; goto done; }
    if (IsSwInputLocked(&g_sw)) { state = TARGET_UNSEEN; goto done; }
    if (detector->isDisabled) { state = TARGET_UNSEEN; goto done; }
    if (IsBasicDerivedFrom__FP5BASIC3CID(target, CID_STEPPLAYER) && target->isHidden) { state = TARGET_UNSEEN; goto done; }
    if (detector->isForced) { state = TARGET_SEEN; goto done; }
    if (IsBasicDerivedFrom__FP5BASIC3CID(target, CID_STEP))
        targetPos = VuAdd(GetLoWorldPos(target), target->pivotOffset);
    else
        targetPos = GetLoWorldPos(target);
    for (occluder = *(CID23 **)&g_sw.dlCid23; occluder; occluder = (CID23 *)occluder->dleList.next) {
        qword eyeLocal = ConvertPosBetweenLos(0, occluder, GetDetectorEyePosition(detector));
        qword targetLocal = ConvertPosBetweenLos(0, occluder, targetPos);
        if (ClipSegmentBsp(occluder->bspc.nodes, SLY2_BSP_NO_LIMIT, eyeLocal, targetLocal, 4, &hitBuffer) > 0) { state = TARGET_UNSEEN; goto done; }
    }
    switch (state) {
    case TARGET_UNSEEN:
        if (detector->ownerPhysicsTree) {
            XPLIST *child;   /* the owner's contact lists: a target it touches is seen */
            for (child = detector->ownerPhysicsTree->pxplistFirst; child; child = child->pxplistNext) {
                if (child->psoOther == (SO *)target) {
                    if (child->pxpFirst) { state = TARGET_SEEN; goto done; }
                    break;
                }
            }
        }
        if (!IsPointInVol(detector->pvolNear, targetPos)) { state = TARGET_UNSEEN; goto done; }
        if (detector->pvolFar && !IsPointInVol(detector->pvolFar, targetPos)) { state = TARGET_UNSEEN; goto done; }
        if ((detector->options & DETECTOR_SKIP_CONCEALMENT) == 0 && IsDetectorTargetConcealed(detector, target)) { state = TARGET_UNSEEN; goto done; }
        if (detector->options & DETECTOR_NEAR_IS_ENOUGH) { state = TARGET_SEEN; goto done; }
        { state = !IsDetectorLineBlocked(detector, target); goto done; }
    case TARGET_SEEN:
        if (detector->pvolFar && !IsPointInVol(detector->pvolFar, targetPos)) { state = TARGET_UNSEEN; goto done; }
        if (!IsPointInVol(detector->pvolNear, targetPos)) { state = TARGET_LOST; goto done; }
        break;
    case TARGET_LOST:
        if (detector->pvolFar && !IsPointInVol(detector->pvolFar, targetPos)) { state = TARGET_UNSEEN; goto done; }
        if (IsPointInVol(detector->pvolNear, targetPos)) {
            if (detector->options & DETECTOR_NEAR_IS_ENOUGH) { state = TARGET_SEEN; goto done; }
            if (IsDetectorLineBlocked(detector, target)) return state;
            state = TARGET_SEEN;
            goto done;
        }
        break;
    default:
        goto done;
    }
    if ((detector->options & DETECTOR_RETEST_LINE) && IsDetectorLineBlocked(detector, target)) state = TARGET_UNSEEN;
done:
    return state;
}

// 0x154528: Line-test object filter (detector flag 4): accepts only objects passing the default filter whose class
// is cid 0x31.
extern "C" int FilterDetectorHitClass31(void *context, SOPART *hit)
{
    if (ShouldKeepSoNotSelf(context, hit) == 0) {
        return 0;
    }
    return GetBasicCid(hit->owner) == CID_MS;
}

// 0x154568: Line-test object filter (default): ignores hits flagged as non-blocking, objects whose collision entry
// has bit 2, objects in the detector's own hierarchy, and anything below a STEP (cid 0x33: player or guard) ancestor.
extern "C" int FilterDetectorLineOfSightHit(DETECTOR *detector, SOPART *hit)
{
    SO *object;
    if (hit->unknownFlag10A)
        return 0;
    object = hit->owner;
    if (object->physicsTree->skipsUpdate)
        return 0;
    if (object->root == detector->root)
        return 0;
    {
        SO *ancestor = object;
        while (ancestor && !IsBasicDerivedFrom__FP5BASIC3CID(ancestor, CID_STEP))
            ancestor = ancestor->parent;
        return ancestor == 0;
    }
}

// 0x1545F0: True if the line from the detector's eye to the target is blocked by a world object
// (FindFirstObjectHitByLine over the objects in the line's bounding box, using one of the filters above).
// Also known as: FSightBlocked (upstream readable / Sly 1 name; FSightBlocked__FP8DETECTORP2LO).
extern "C" int IsDetectorLineBlocked(DETECTOR *detector, SOV *target)
{
    STACKSCOPE stackScope;
    int i;
    qword *localPoints = D_002BDBF0;
    qword eyePos = GetDetectorEyePosition(detector);
    AL request(0x10, 0x10);
    qword *points = (qword *)AllocFromStack(&request);
    int objectCount;
    SOPART **objects;   /* the parts in the box */
    qword boxMax = eyePos;
    qword boxMin = eyePos;
    for (i = 0; i < 1; i++) {
        qword point = ConvertPosBetweenLos(target, 0, localPoints[i]);
        points[i] = point;
        __asm__("vmini.xyz %0, %0, %1" : "+j"(boxMin) : "j"(point));
        __asm__("vmax.xyz %0, %0, %1" : "+j"(boxMax) : "j"(point));
    }
    void *filter = (void *)FilterDetectorLineOfSightHit;
    if (detector->options & DETECTOR_FILTER_MS) filter = (void *)FilterDetectorHitClass31;
    IntersectSwBoundingBox(&g_sw, boxMin, boxMax, filter, detector, &objectCount, &objects);
    bool isClear;
    for (i = 0; (isClear = i < 1); i++)
        if (!FindFirstObjectHitByLine(0, eyePos, points[i], objectCount, objects, 0)) break;
    return !isClear;
}

// VU0 helper: dot product.
static inline float Dot(qword a, const VECTORQ &b)
{
    float g;
    a = VuDot(a, b.q);
    g = VuToFloat(a);
    return g;
}
// 0x154770: True if a player target (cid 0x34) is concealed by its current state: hiding in or behind cover objects
// (cids 0xC and 0x22, facing away from the detector), and certain sneak/hang states (inferred: the state numbers
// are JT states).
// Also known as: FTargetHidden (upstream readable / Sly 1 name; FTargetHidden__FP8DETECTORP2LO).
extern "C" int IsDetectorTargetConcealed(DETECTOR *detector, SOV *target)
{
    if (!IsBasicDerivedFrom__FP5BASIC3CID(target, CID_STEPPLAYER)) return 0;
    if (target->jtgrab.kind == -1) return 0;
    VECTORQ offset; offset.q = VuSub(GetLoWorldPos(target), GetLoWorldPos(detector));
    switch (target->jtgrab.state) {
    case 0:
    case 13:
        if (IsBasicDerivedFrom__FP5BASIC3CID(target->jtgrab.grabbedObject, CID_COVER_C)) {
            if (((GRABSPOT *)SLY2_GRABSPOT_BASE(WALLSPOT, target->jtgrab.grabbedObject))->unknown04) return 0;
        } else if (IsBasicDerivedFrom__FP5BASIC3CID(target->jtgrab.grabbedObject, CID_COVER_22)) {
            if (((GRABSPOT *)SLY2_GRABSPOT_BASE(POLESPOT, target->jtgrab.grabbedObject))->unknown04) return 0;
        } else {
#if !defined(__R5900__)
            /* Nothing grabbed: the EE reads a word of low memory here (kernel bytes, nonzero), so not concealed. */
            if (!target->jtgrab.grabbedObject) return 0;
#endif
            return !((GRABSPOT *)SLY2_GRABSPOT_BASE(POLESPOT, target->jtgrab.grabbedObject))->unknown04;
        }
        {
            bool isFacingAway = Dot(offset.q, *GetLoWorldMatrix(target)) <= 0.0f;
            bool expectFacingAway = false;
            if (target->jtgrab.state == 0) expectFacingAway = !target->isGrabReversed;
            if (isFacingAway == expectFacingAway) break;
            return 1;
        }
    case 1:
        if (target->jtgrab.kind) break;
        return 1;
    case 2:
        return 1;
    case 8:
        if (target->jtgrab.kind == 9 || target->jtgrab.kind == 10) return 1;
        break;
    }
    return 0;
}
