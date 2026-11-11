// bbmark.cpp -- broad-phase collision: the world's sweep-and-prune over axis-aligned bounding boxes (SW +0x3CCC..
// +0x3D40). Each solid object in the world owns an AAOBR (its world box plus list links); the box's min/max
// endpoints sit in three per-axis arrays kept sorted by insertion sort, and whenever two boxes start or stop
// overlapping an object pair link (POX) is created or destroyed. Once per solver step RecalcSwXpAll rebuilds the
// contact points (XPs) of the pairs that changed. Entry points: AddObjectToBroadPhase / RemoveObjectFromBroadPhase (an object
// enters / leaves), UpdateSwAaox, RecalcSwXpAll, RefreshSwObjectCollisionFilter. One register pin, allocation-only asms.
// TU 0x1336F0..0x1344F0 (13 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_bbmark
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_bbmark
#include "sly2/slotheap.h"
#include "sly2/dli.h"
#include "sly2/sw.h"
#include "sly2/so.h"

// 0x134398 StaticInit_bbmark and GlobalConstructors_bbmark: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

struct X;
/* View of a solid object (an SO; V suffix next to sly2/so.h, decision 1.6): +0x14 its parent, +0xEC its AAOBR (the
   SO's physics tree record, seen here as X). Virtual calls go through the real SO. */
struct LOV { void *scriptFrame; void *scriptVariables; void *firstSubscriber; void *vptr; void *unknown10; void *parent; char pad18[0x1C - 0x18]; void *unknown1C; void *unknown20; void *unknown24; void *unknown28; char pad2C[0x34 - 0x2C]; void *firstRunCallback; char pad38[0x3C - 0x38]; void *proxyRecord; char pad40[0xEC - 0x40]; X *aaobr; };
typedef char assert_LOV_px[(unsigned int)&((LOV *)0)->aaobr == 0xEC ? 1 : -1];
/* A POX link: one overlapping pair, seen from one side (the other side's link is pother). */
struct L { void *contacts; L *next; L *other; LOV *object; int isForced; };
/* An object's AAOBR (world bounds and its list links). */
struct X
{
    float min[4];           /* +0x00 */
    float max[4];           /* +0x10 */
    char pad20[0x14];
    DL unknown34List;                /* +0x34 */
    char pad40[8];
    X *nextAaobr;          /* +0x48 link in SW::dlAaobr */
    char pad4C[0xC];
    X *nextXp;             /* +0x58 link in SW::dlXp */
    int unknown5C;
    LOV *object;                /* +0x60 the object */
    L *pairs;                  /* +0x64 its POX links */
    void *contactList68;             /* +0x68 XP list */
    void *contactList6C;             /* +0x6C XP list */
    unsigned long flags;      /* +0x70 1: in dlBounds, 2: in dlXp, 4/8/0x10: RecalcSwXpAll marks */
};
typedef char assert_X_grf[(unsigned int)&((X *)0)->flags == 0x70 ? 1 : -1];
/* AAOX (sly2/sw.h SW::aaaox[axis] arrays, caaox entries each): one endpoint of an AAOBR on one axis: 0 = min,
   1 = max; the AAOBR; the coordinate. */
struct AAOX { int isMax; X *aaobr; float *coordinate; };

struct C { int a[4]; };

extern "C" {
void InitDl__FP2DLi(DL *, int);
void AppendDlEntry__FP2DLPv(DL *, void *);
int IsEntryInDl__FP2DLPv(DL *, void *);
void RemoveDlEntry__FP2DLPv(DL *, void *);
L *AddSwObjectPair(SW *, X *, X *);
L *RemoveSwObjectPair(SW *, X *, X *);
void FreeSwXpList__FP2SWP2XP(SW *, void *);
void WakeSo(LOV *);
void *memmove(void *, const void *, unsigned int);
int ShouldSwPairObjects(SW *, X *, X *);
void UpdateSwPox(SW *, X *, X *, int);
int MaxInt(int a, int b);
void FlushSwBoundsUpdates(SW *p);
void UpdateSwAaox(SW *p);
C *STACKSCOPE_ctor(C *);
void STACKSCOPE_ctor_1AE4A0(C *, int) __asm__("_$_10STACKSCOPE");
void *AddSoXps(SW *, X *, X *);
}

// Adds an object to the broad phase: allocates its AAOBR from the world pool, links it into the world lists and
// inserts its min/max endpoints into all three axis arrays (two more endpoints per axis). Returns the AAOBR.
// Also known as: PoxaAddSw (upstream readable / Sly 1 name; PoxaAddSw__FP2SWP3ALO).
extern "C" X *AddObjectToBroadPhase(SW *world, LOV *object) {
    int axis;
    X *aaobr = (X *)AllocZeroedFromSlotheap(&world->slotheapSotree);
    InitDl__FP2DLi(&aaobr->unknown34List, 0x60);
    aaobr->object = object;
    AppendDlEntry__FP2DLPv(&world->dlSoTreeAll, aaobr);
    if (IsEntryInDl__FP2DLPv(&world->busyRoots, object->parent)) AppendDlEntry__FP2DLPv(&world->dlSoTree, aaobr);
    for (axis = 0; axis < 3; axis++) {
        AAOX *endpoint = &world->aaaox[axis][world->caaox];
        endpoint->isMax = 0;
        endpoint->aaobr = aaobr;
        endpoint->coordinate = &aaobr->min[axis];
        endpoint++;
        endpoint->isMax = 1;
        endpoint->aaobr = aaobr;
        endpoint->coordinate = &aaobr->max[axis];
    }
    world->caaox += 2;
    return aaobr;
}

/* MATCH (Claude agent laneB14, 2026-10-23), from v2 (3). One register pin (k3 in s5) plus commented empty asms:
 * - an empty asm after `pe = ae` keeps the loop-made i+1 (addiu s4) last in its block, so reorg puts it in the b slot;
 * - that asm lengthens ae's live range by one insn, which drops ae's global-alloc priority below p's (s2/s3 swap).
 *   Five empty asms after the first loop add the same length to p's range (they stand in for ~5 insns the original
 *   had there), restoring the target's p/ae priority order;
 * - paae is set (before k3) so the aae address is computed ahead of the k3 constant, as in the target. */
/* v2 (Claude agent laneB12, 2026-10-23): aligned 7 -> 3. The /12 pointer-diff divisions written out as (bytes * k3) >> 2 with k3 = 0xAAAAAAAB in ONE commented register pin ($21 = s5, as in the target): fixes the s4/s5 swap. Left: ori s5 before addiu fp, and i+1 (addiu s4) scheduled right after the sll instead of in the b delay slot (j = i + 1 / i = j step, split init: same 3). */
// Removes an object's AAOBR from the broad phase: for every overlap pair both objects are notified (slot 54) and
// the pair's contact lists freed, then the box leaves the world lists and its endpoints are deleted from the axes.
// Also known as: RemoveSwOxa (upstream readable / Sly 1 name; RemoveSwOxa__FP2SWP3OXA).
extern "C" void RemoveObjectFromBroadPhase(SW *world, X *aaobr)
{
    L *pair, *nextPair;
    int axis;
    for (pair = aaobr->pairs; pair; pair = nextPair) {
        L *otherPair;
        ((SO *)aaobr->object)->OnContactFreed((SO *)pair->object);
        ((SO *)pair->object)->OnContactFreed((SO *)aaobr->object);
        otherPair = RemoveSwObjectPair(world, pair->object->aaobr, aaobr);
        if (pair->contacts) {
            if (!((SO *)pair->object)->IsFixed())
                WakeSo(pair->object);
            FreeSwXpList__FP2SWP2XP(world, pair->contacts);
        }
        nextPair = pair->next;
        FreeSlotheapSlot(&world->slotheapXplist, pair);
        FreeSlotheapSlot(&world->slotheapXplist, otherPair);
    }
    /* five empty asms: lengthen world's live range (global-alloc priority), emulating the original insn count here */
    __asm__ __volatile__("");
    __asm__ __volatile__("");
    __asm__ __volatile__("");
    __asm__ __volatile__("");
    __asm__ __volatile__("");
    FreeSwXpList__FP2SWP2XP(world, aaobr->contactList6C);
    RemoveDlEntry__FP2DLPv(&world->dlSoTreeAll, aaobr);
    if (IsEntryInDl__FP2DLPv(&world->dlSoTree, aaobr))
        RemoveDlEntry__FP2DLPv(&world->dlSoTree, aaobr);
    if ((aaobr->flags & 1) == 1)
        RemoveDlEntry__FP2DLPv(&world->dlSoTreeQueue1, aaobr);
    if (aaobr->flags & 2)
        RemoveDlEntry__FP2DLPv(&world->dlSoTreeQueue2, aaobr);
    {
    AAOX **axisArrays;
    register unsigned int k3 __asm__("$21");
    axis = 0;
    axisArrays = world->aaaox;
    k3 = 0xAAAAAAAB;  /* register pin: the exact-/12 multiplier (target s5) */
    for (; axis < 3; axis++) {
        AAOX *endpoints = ((SW *)((char *)axisArrays - 0x3CD4))->aaaox[axis];  /* = world->aaaox[axis]; this form keeps the addu base-first */
        AAOX *endpoint = endpoints;
        AAOX *minEndpoint;
        __asm__ __volatile__("");  /* keeps i+1 after pe = ae, so reorg puts it in the b delay slot */
        while (1) {
            while (endpoint->aaobr != aaobr)
                endpoint++;
            if (endpoint->isMax)
                break;
            minEndpoint = endpoint++;
        }
        memmove(minEndpoint, minEndpoint + 1, ((int)(((char *)endpoint - (char *)minEndpoint) * k3) >> 2) * sizeof(AAOX) - sizeof(AAOX));
        memmove(endpoint - 1, endpoint + 1, (world->caaox - ((int)(((char *)endpoint - (char *)endpoints) * k3) >> 2)) * sizeof(AAOX) - sizeof(AAOX));
    }
    }
    world->caaox -= 2;
    FreeSlotheapSlot(&world->slotheapSotree, aaobr);
}

// Creates or destroys the pair link between two AAOBRs. k = 1 forces the pair on, 2 forces it off, 0 decides from
// ShouldSwPairObjects; removing a pair wakes both objects and frees its contacts.
// Also known as: UpdateSwPox (upstream readable / Sly 1 name; UpdateSwPox__FP2SWP3OXAT1Uc).
extern "C" void UpdateSwPox(SW *world, X *aaobrA, X *aaobrB, int mode)
{
    L *pair;
    L *existingPair;
    int isForced;
    for (existingPair = aaobrA->pairs; existingPair; existingPair = existingPair->next) {
        if (existingPair->object == aaobrB->object)
            break;
    }
    switch (mode) {
    case 1:
        isForced = 1;
        break;
    case 2:
        isForced = 0;
        break;
    case 0:
        isForced = 0;
        if (existingPair)
            isForced = existingPair->isForced;
        break;
    }
    pair = existingPair;
    if (isForced || ShouldSwPairObjects(world, aaobrA, aaobrB)) {
        if (!pair) {
            L *pairB;
            existingPair = AddSwObjectPair(world, aaobrA, aaobrB);
            pairB = AddSwObjectPair(world, aaobrB, aaobrA);
            existingPair->other = pairB;
            pairB->other = existingPair;
            pair = existingPair;
        }
    } else if (pair) {
        L *pairA, *pairB;
        ((SO *)aaobrA->object)->OnContactFreed((SO *)aaobrB->object);
        ((SO *)aaobrB->object)->OnContactFreed((SO *)aaobrA->object);
        pairA = RemoveSwObjectPair(world, aaobrA, aaobrB);
        pairB = RemoveSwObjectPair(world, aaobrB, aaobrA);
        if (pairA->contacts) {
            FreeSwXpList__FP2SWP2XP(world, pairA->contacts);
            if (!((SO *)pairA->object)->IsFixed())
                WakeSo(pairA->object);
            if (!((SO *)pairB->object)->IsFixed())
                WakeSo(pairB->object);
        }
        FreeSlotheapSlot(&world->slotheapXplist, pairA);
        FreeSlotheapSlot(&world->slotheapXplist, pairB);
        pair = 0;
    }
    if (pair) {
        pair->other->isForced = isForced;
        pair->isForced = isForced;
    }
}

// Allocates a pair link from the world pool for px1 with px2 as the other side and pushes it on px1's pair list.
// Also known as: PoxAddSw (upstream readable / Sly 1 name; PoxAddSw__FP2SWP3OXAT1).
extern "C" L *AddSwObjectPair(SW *world, X *aaobr, X *otherAaobr)
{
    L *pair = (L *)AllocZeroedFromSlotheap(&world->slotheapXplist);
    pair->object = otherAaobr->object;
    pair->next = aaobr->pairs;
    aaobr->pairs = pair;
    return pair;
}

// Unlinks and returns px1's pair link whose other side is px2 (0 if none).
// Also known as: PoxRemoveSw (upstream readable / Sly 1 name; PoxRemoveSw__FP2SWP3OXAT1).
extern "C" L *RemoveSwObjectPair(SW *unused, X *aaobr, X *otherAaobr) {
    LOV *otherObject = otherAaobr->object;
    L *pair = aaobr->pairs;
    if (pair) {
        L **link = &pair->next;
        if (pair->object == otherObject) {
            aaobr->pairs = pair->next;
        } else {
            for (;;) {
                pair = pair->next;
                if (!pair) break;
                if (pair->object == otherObject) { *link = pair->next; break; }
                link = &pair->next;
            }
        }
    }
    return pair;
}

// Returns 1 when two AAOBRs overlap on all three axes and neither object asks to ignore the other: both objects'
// ignore-intersection answers (slot 61) are combined with MaxInt and the pair is made unless the result is 1 or 3.
// Also known as: FOverlapOxa (upstream readable / Sly 1 name; FOverlapOxa__FP3OXAT0Pv).
extern "C" int ShouldSwPairObjects(SW *unused, X *aaobrA, X *aaobrB)
{
    int ignoreA, ignoreB, ignore;
    if (aaobrA->min[0] >= aaobrB->max[0] || aaobrA->min[1] >= aaobrB->max[1] || aaobrA->min[2] >= aaobrB->max[2] ||
        aaobrB->min[0] >= aaobrA->max[0] || aaobrB->min[1] >= aaobrA->max[1] || aaobrB->min[2] >= aaobrA->max[2])
        return 0;
    ignoreA = ((SO *)aaobrA->object)->ShouldIgnoreIntersection((SO *)aaobrB->object);
    ignoreB = ((SO *)aaobrB->object)->ShouldIgnoreIntersection((SO *)aaobrA->object);
    ignore = MaxInt(ignoreA, ignoreB);
    return ignore != 1 && ignore != 3;
}

// Drains the pending-bounds list: for each queued object calls its bounds update (slot 49, UpdateSoTreeBounds)
// until the list is empty.
// Also known as: FlushSwOxaDirty1 (upstream readable / Sly 1 name; FlushSwOxaDirty1__FP2SW).
extern "C" void FlushSwBoundsUpdates(SW *world) {
    X *aaobr;
    while ((aaobr = (X *)world->dlSoTreeQueue1.head) != 0) {
        ((SO *)aaobr->object)->UpdateBounds();
    }
}

// If the endpoint arrays are dirty, insertion-sorts each axis; whenever a min and a max endpoint swap places the
// pair is re-evaluated with UpdateSwPox(.., 0). This is the incremental sweep-and-prune step.
// Also known as: UpdateSwAaox (upstream readable / Sly 1 name; UpdateSwAaox__FP2SW).
extern "C" void UpdateSwAaox(SW *world)
{
    int axis;
    if (!world->fAaoxUnsorted)
        return;
    world->fAaoxUnsorted = 0;
    if (!world->caaox)
        return;
    for (axis = 0; axis < 3; axis++) {
        AAOX *endpoint = world->aaaox[axis];
        AAOX *endpoints = endpoint;
        float maxCoordinate = *endpoint->coordinate;
        int remaining;
        for (remaining = world->caaox, endpoint++; --remaining > 0; endpoint++) {
            float coordinate = *endpoint->coordinate;
            if (maxCoordinate <= coordinate) {
                maxCoordinate = coordinate;
            } else {
                AAOX *insertAt;
                AAOX moved;
                for (insertAt = endpoint - 1; insertAt >= endpoints; insertAt--) {
                    if (*insertAt->coordinate <= coordinate)
                        break;
                    if (endpoint->isMax != insertAt->isMax)
                        UpdateSwPox(world, endpoint->aaobr, insertAt->aaobr, 0);
                }
                insertAt++;
                moved = *endpoint;
                memmove(insertAt + 1, insertAt, (endpoint - insertAt) * sizeof(AAOX));
                *insertAt = moved;
            }
        }
    }
}

// Once per solver step: flushes bounds updates, re-sorts the axes, then walks the objects queued for contact
// recalculation and rebuilds the XP lists of their pairs (AddSoXps), using a stack array of up to 0x300 objects.
// Also known as: UpdateSwXps (upstream readable / Sly 1 name; UpdateSwXps__FP2SW).
extern "C" void RecalcSwXpAll(SW *world)
{
    C scope;
    X *touched[0x300];
    int numTouched = 0;
    X *aaobr;
    int i;
    FlushSwBoundsUpdates(world);
    UpdateSwAaox(world);
    STACKSCOPE_ctor(&scope);
    for (aaobr = (X *)world->dlSoTreeQueue2.head; aaobr; aaobr = aaobr->nextXp) {
        unsigned long flags = aaobr->flags;
        L *pair;
        if (flags & 4)
            continue;
        flags |= 0x10;
        aaobr->flags = flags;
        if (!(flags & 8)) {
            aaobr->flags = flags | 8;
            touched[numTouched++] = aaobr;
        }
        for (pair = aaobr->pairs; pair; pair = pair->next) {
            X *otherAaobr = pair->object->aaobr;
            unsigned long otherFlags = otherAaobr->flags;
            void *oldContacts;
            if (otherFlags & 0x10)
                continue;
            if (otherFlags & 4)
                continue;
            if (!(otherFlags & 8)) {
                otherAaobr->flags = otherFlags | 8;
                touched[numTouched++] = otherAaobr;
            }
            ((SO *)aaobr->object)->OnContactFreed((SO *)otherAaobr->object);
            ((SO *)otherAaobr->object)->OnContactFreed((SO *)aaobr->object);
            oldContacts = pair->contacts;
            FreeSwXpList__FP2SWP2XP(world, oldContacts);
            { void *newContacts = AddSoXps(world, aaobr, otherAaobr); pair->contacts = newContacts; pair->other->contacts = newContacts; }
            if (oldContacts || pair->contacts) {
                if (!((SO *)aaobr->object)->IsFixed())
                    WakeSo(aaobr->object);
                if (!((SO *)otherAaobr->object)->IsFixed())
                    WakeSo(otherAaobr->object);
            }
        }
    }
    for (aaobr = (X *)world->dlSoTreeQueue2.head; aaobr; aaobr = (X *)world->dlSoTreeQueue2.head) {
        aaobr->flags &= ~0x10L;
        ((SO *)aaobr->object)->OnContactFreed((SO *)0);
        FreeSwXpList__FP2SWP2XP(world, aaobr->contactList68);
        aaobr->contactList68 = 0;
        ((SO *)aaobr->object)->AddExtraXps();
        aaobr->flags &= ~2L;
        RemoveDlEntry__FP2DLPv(&world->dlSoTreeQueue2, aaobr);
    }
    for (i = 0; i < numTouched; i++) {
        aaobr = touched[i];
        aaobr->flags &= ~8L;
        ((SO *)aaobr->object)->AdjustXps();
    }
    STACKSCOPE_ctor_1AE4A0(&scope, 2);
}

// Returns the larger of two ints; kept out of line by the compiler. Used to merge ignore-intersection answers.
extern "C" int MaxInt(int a, int b)
{
    return (b >= a) ? b : a;
}

// When an object's collision filter changed: if it has an AAOBR (+0xEC), invalidates its box and re-evaluates its
// pair with every other AAOBR in the world.
// Also known as: RecalcSwOxfFilterForObject (upstream readable / Sly 1 name; RecalcSwOxfFilterForObject__FP2SWP2SO).
extern "C" void RefreshSwObjectCollisionFilter(SW *world, LOV *object) {
    X *other;
    if (object->aaobr) {
        InvalidateSoAaox((SO *)object);
        for (other = (X *)world->dlSoTreeAll.head; other; other = other->nextAaobr)
            if (other != object->aaobr) UpdateSwPox(world, object->aaobr, other, 0);
    }
}
