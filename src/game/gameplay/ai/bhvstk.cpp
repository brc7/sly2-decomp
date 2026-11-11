// bhvstk.cpp -- behaviours (BHV) and the per-object behaviour stack (BHVSTK). A guard (STEPGUARD) owns one stack of
// up to eight behaviours in preallocated 0xF0-byte slots; the top behaviour is the active one. Entry points:
// PushBhvstk (construct a behaviour of a given class in the next slot via ConstructBhv and start it), PopBhv /
// ClearBhvstk (close a behaviour and everything above it), UpdateBhvstk and HandleBhvstkMessage (per-frame update
// and message dispatch, top first), SetBhvBusy / ClearBhvBusy (busy state propagated to the behaviours below and
// the owning guard), and the empty BHV base methods that subclasses override.
// TU 0x1EEC60..0x1EF850 (25 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_bhvstk
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_bhvstk
#include "sly2/slotheap.h"
#include "sly2/classes.h"
#include "sly2/vtables.h"

// 0x1EF6F8 StaticInit_bhvstk and GlobalConstructors_bhvstk: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* The behaviours are step.h BHV (and the classes.h subclasses), the stack step.h BHVSTK, its owner a STEP (the stack
   at +0x26C); slot 13 (HandleMessage) is called through the vtable entry: step.h declares it without its arguments. */
/* ConstructBhv's view of a new behaviour under construction: its BASIC header words and the vtable at +0xC. */
struct BHVP { int tag, value, word8; void *vtable; };
struct AREQV { int cb; int nKind; };

/* Class ids: the guard step and the behaviour classes (docs/systems/object-model.md section 1). */
enum {
    CID_STEPGUARD = 0x38,
    CID_BHVGUARD = 0x9C,
    CID_BHVANIM = 0x9D,
    CID_BHVIDLE = 0x9E,
    CID_BHVSTRIKE = 0x9F,
    CID_BHV161 = 0xA1,
    CID_BHV162 = 0xA2,
    CID_BHVKO = 0xA3,
    CID_BHVATTACK = 0xA4,
    CID_BHVCOMBAT = 0xA5,
    CID_BHVWALKPATH = 0xA7,
    CID_BHV168 = 0xA8,
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
    CID_BHV181 = 0xB5,
    CID_BHV182 = 0xB6,
    CID_BHV183 = 0xB7,
    CID_BHVRAIL = 0xB8,
    CID_BHVPIPE = 0xB9,
    CID_BHVGOTO = 0xBA,
    CID_BHV187 = 0xBB,
    CID_BHV189 = 0xBD,
    CID_BHVPATHLINK = 0xBE,
    CID_BHV191 = 0xBF,
    CID_BHVPATROLPATH = 0xC0,
    CID_BHV193 = 0xC1,
    CID_BHV194 = 0xC2,
    CID_BHV195 = 0xC3,
    CID_BHV196 = 0xC4,
    CID_BHV197 = 0xC5,
    CID_BHV198 = 0xC6,
};
/* The behaviour vtables (the asm labels are the symbols). */
extern "C" {
int GetBhvSlotSize();
void *AllocFromWorldHeap(AREQV *request);
void *func_002333B4(void *, int, unsigned int) __asm__("memset");
void ClearPointerPair(PATHLINKEND *);
BHV *ConstructBhv(int kind, void *memory);
void PopBhv(BHV **slot);
void DecrementBhvstk(BHVSTK *stack);
int IsBasicDerivedFrom__FP5BASIC3CID(void *object, int cid);
void IncrementStepguardBusy(STEPGUARD *owner);
void DecrementStepguardBusy(STEPGUARD *owner);
}

// 0x1EEC60: Initializes a behaviour stack by binding it to its owner (the guard step that holds it at +0x26C).
// Runtime-confirmed layout {owner, count, slots[8]}.
extern "C" void InitBhvstk(BHVSTK *stack, STEP *owner)
{
    stack->owner = owner;
}

// 0x1EEC68: Clone fix-up: after an object copy, re-points the stack at its new owner.
extern "C" void CloneBhvstk(BHVSTK *stack, STEP *owner)
{
    stack->owner = owner;
}

// 0x1EEC70: Allocates the eight behaviour slots (GetBhvSlotSize() bytes each, 16-byte aligned) from the world heap
// after load.
extern "C" void PostBhvstkLoad(BHVSTK *stack)
{
    unsigned int i;
    AREQV request;
    for (i = 0; i < 8; i++) {
        request.cb = GetBhvSlotSize();
        request.nKind = 0x10;
        stack->behaviors[i] = (BHV *)AllocFromWorldHeap(&request);
    }
}

// 0x1EECD0: Pops every behaviour: popping the bottom slot closes everything stacked above it; then resets the depth
// to 0.
void ClearBhvstk(BHVSTK *p) __asm__("ClearBhvstk__FP6BHVSTK");
// Definition of ClearBhvstk__FP6BHVSTK (declared above): pop the bottom behaviour (which closes everything above it)
// and reset the depth.
// Also known as: ClearJobs (upstream readable / Sly 1 name; ClearJobs__FP5ACTOR).
void ClearBhvstk(BHVSTK *stack)
{
    struct { BHV *pbhv; int pad[3]; } bottom;
    if (stack->depth > 0) {
        bottom.pbhv = stack->behaviors[0];
        PopBhv(&bottom.pbhv);
    }
    stack->depth = 0;
}

// 0x1EED10: Per-frame update: calls each stacked behaviour's Update (vtable slot 10), top (most recently pushed)
// first.
// Also known as: UpdateJobs (upstream readable / Sly 1 name; UpdateJobs__FP5ACTOR).
extern "C" void UpdateBhvstk(BHVSTK *stack)
{
    int i;
    for (i = stack->depth; --i >= 0;) {
        BHV *behavior = stack->behaviors[i];
        behavior->Update();
    }
}

// 0x1EED78: Offers a step message (id, payload) to the stacked behaviours, top first, stopping at the first whose
// HandleBehaviorMessage (vtable slot 13) returns nonzero (consumed).
// Also known as: OfferJobMessage (upstream readable / Sly 1 name; OfferJobMessage__FP5GUARDii).
extern "C" void HandleBhvstkMessage(BHVSTK *stack, int messageId, int payload)
{
    int i = stack->depth;
    while (--i >= 0) {
        BHV *behavior = stack->behaviors[i];
        if (behavior->HandleBehaviorMessage(messageId, (void *)payload))
            break;
    }
}

// Matching helper: reads a count word with an explicit load (keeps the target's instruction order).
static inline int Count(const int *p)
{
    register int value __asm__("$2");
    __asm__("lw %0,%1" : "=r"(value) : "m"(*p));
    return value;
}
// 0x1EEE00: Pushes a new behaviour of class `cid`: takes the next free slot, clears it, constructs the behaviour
// there (ConstructBhv), links it to its owner and to the behaviour below, then calls its OnPush (vtable slot 7). Returns
// the new behaviour.
// Also known as: PjobPush (upstream readable / Sly 1 name; PjobPush__FP5GUARD3CID).
extern "C" BHV *PushBhvstk(BHVSTK *stack, int cid)
{
    BHV *below, *behavior;
    void *memory;
    if (stack->depth == 0)
        below = 0;
    else
        below = stack->behaviors[stack->depth - 1];
    {
        int index = Count(&stack->depth);
        memory = stack->behaviors[index];
        stack->depth = index + 1;
    }
    func_002333B4(memory, 0, GetBhvSlotSize());
    behavior = ConstructBhv(cid, memory);
    {
        register STEP *owner __asm__("$3") = stack->owner;
        behavior->owner = owner;
        behavior->pbhvPrev = below;
    }
    if (below)
        below->pbhvNext = behavior;
    behavior->OnPush();
    return behavior;
}

// 0x1EEED0: Decrements the stack depth (called by PopBhv once a behaviour has been closed).
extern "C" void DecrementBhvstk(BHVSTK *stack)
{
    stack->depth--;
}

// 0x1EEEE0: Returns the first stacked behaviour (searching bottom up) whose class derives from `cid`, or NULL.
// Also known as: PjobFind (upstream readable / Sly 1 name; PjobFind__FP5GUARDPv).
extern "C" BHV *FindBhvInBhvstk(BHVSTK *stack, int cid)
{
    int i;
    for (i = 0; i < stack->depth; i++) {
        BHV *behavior = stack->behaviors[i];
        if (IsBasicDerivedFrom__FP5BASIC3CID(behavior, cid)) return behavior;
    }
    return 0;
}

// 0x1EEF68: BHV vtable slot 7 (OnPush), base version: does nothing. Called by PushBhvstk right after construction.
// Also known as: Init (upstream readable / Sly 1 name).
void BHV::OnPush()
{
}

// 0x1EEF70: BHV vtable slot 8 (Start), base version: does nothing. Starts the behaviour with default parameters;
// callers that push without a specific setter call it.
// Also known as: Enter (upstream readable / Sly 1 name).
extern "C" void StartBhv(BHV *behavior)
{
}

// 0x1EEF78: BHV vtable slot 9 (Close), base version: pops whatever is stacked above this behaviour.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhv(BHV *behavior)
{
    PopBhv(&behavior->pbhvNext);
}

// 0x1EEF98: BHV vtable slot 10 (Update), base version: empty per-frame update.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhv(BHV *behavior)
{
}

// 0x1EEFA0: BHV vtable slot 12, base version: returns the owner step's ground-probe flag mask, 0 when the step's
// +0x204 override is set, else 7 (bit 0 snap to ground, bit 1 alternate probe point, bit 2 count in solver
// contacts) (inferred).
// Also known as: State (upstream readable / Sly 1 name).
extern "C" int GetBhvGroundProbeFlags(BHV *behavior)
{
    return (behavior->owner->unknown204 != 0) ? 0 : 7;   /* the step's ground-probe override */
}

// 0x1EEFB8: BHV vtable slot 11 (UpdateMotion), base version: empty. Per-frame motion update for the behaviour that
// owns the step's motion controller (STEPGUARD +0xFD0).
// Also known as: Steer (upstream readable / Sly 1 name).
extern "C" void UpdateBhvMotion(BHV *behavior)
{
}

// 0x1EEFC0: Marks a behaviour busy (once): if the owner is a STEPGUARD (cid 0x38) its busy count is incremented,
// and every behaviour below gets its busy-above count incremented. Runtime-confirmed busy protocol.
// Also known as: LockJob (upstream readable / Sly 1 name; LockJob__FP5AIJOB).
extern "C" void SetBhvBusy(BHV *behavior)
{
    if (behavior->isBusy)
        return;
    BHV *lower;
    behavior->isBusy = 1;
    if (IsBasicDerivedFrom__FP5BASIC3CID(behavior->owner, CID_STEPGUARD)) {
        IncrementStepguardBusy((STEPGUARD *)behavior->owner);
    }
    for (lower = behavior->pbhvPrev; lower; lower = lower->pbhvPrev)
        lower->numBusyAbove++;
}

// 0x1EF038: Inverse of SetBhvBusy: clears the busy flag, decrements the owning STEPGUARD's busy count and the
// busy-above count of every behaviour below.
// Also known as: UnlockJob (upstream readable / Sly 1 name; UnlockJob__FP5AIJOB).
extern "C" void ClearBhvBusy(BHV *behavior)
{
    if (!behavior->isBusy)
        return;
    BHV *lower;
    behavior->isBusy = 0;
    if (IsBasicDerivedFrom__FP5BASIC3CID(behavior->owner, CID_STEPGUARD)) {
        DecrementStepguardBusy((STEPGUARD *)behavior->owner);
    }
    for (lower = behavior->pbhvPrev; lower; lower = lower->pbhvPrev)
        lower->numBusyAbove--;
}

// 0x1EF0A8: True if a behaviour higher on the stack is busy.
extern "C" int IsBhvBusyAbove(BHV *behavior)
{
    return behavior->numBusyAbove > 0;
}

// 0x1EF0B8: BHV vtable slot 13 (HandleMessage), base version: returns 0 (message not consumed).
// Also known as: Message (upstream readable / Sly 1 name; Message__5AIJOBiPv).
extern "C" int HandleBhvMessage(BHV *behavior, int messageId, int payload)
{
    return 0;
}

// 0x1EF0C0: BHV vtable slot 14, base version: resets a steering record (clears its +0x1C word and sets its +0x18
// heading to the owner step's target heading, STEP::targetHeading at +0x1A8) (inferred).
// Also known as: Drive (upstream readable / Sly 1 name; Drive__5AIJOBP6PADCTL).
extern "C" void ResetBhvSteering(BHV *behavior, POCTL *steering)
{
    *(int *)&steering->leftStickDeflection = 0;   /* (an integer zero store, as the target) */
    steering->leftStickAngle = behavior->owner->targetHeading;
}

// 0x1EF0D8: Pops the behaviour in a slot: if non-null, calls its Close (vtable slot 9, which pops everything above
// it), decrements the owner's stack depth and clears the slot.
// Also known as: EndChildJob (upstream readable / Sly 1 name; EndChildJob__FPP5AIJOB).
extern "C" void PopBhv(BHV **slot)
{
    BHV *behavior = *slot;
    if (behavior) {
        BHVSTK *stack = &behavior->owner->behaviors;
        behavior->Close();
        DecrementBhvstk(stack);
        *slot = 0;
    }
}

// Clears a behaviour slot (`size` bytes, the class's size) and its BASIC header: the start of every behaviour
// class's inlined constructor (the caller then installs the vtable).
static inline BHVP *ClearBhv(void *memory, int size)
{
    BHVP *header = (BHVP *)memory;
    func_002333B4(memory, 0, size);
    header->tag = 0;
    header->value = 0;
    return header;
}
// 0x1EF138: Placement factory: constructs the behaviour of class `cid` (0x9C..0xC6) in `memory` by clearing the
// class's size and installing its BASIC header and vtable; some classes also initialize embedded reference fields.
// Also known as: PjobNew (upstream readable / Sly 1 name; PjobNew__F3CIDPv).
extern "C" BHV *ConstructBhv(int cid, void *memory)
{
    BHV *behavior;
    switch (cid) {
    case CID_BHVGUARD:
        SLY2_SET_VPTR(ClearBhv(memory, 0xB0), BHVGUARD);
        func_002333B4((char *)memory + 0x30, 0, 0x70);
        behavior = (BHV *)memory;
        break;
    case CID_BHVANIM:
        SLY2_SET_VPTR(ClearBhv(memory, 0x50), BHVANIM);
        behavior = (BHV *)memory;
        break;
    case CID_BHVIDLE:
        SLY2_SET_VPTR(ClearBhv(memory, 0x50), BHVIDLE);
        behavior = (BHV *)memory;
        break;
    case CID_BHVSTRIKE:
        SLY2_SET_VPTR(ClearBhv(memory, 0x60), BHVSTRIKE);
        behavior = (BHV *)memory;
        break;
    case CID_BHV161:
        SLY2_SET_VPTR(ClearBhv(memory, 0xC0), BHV161);
        behavior = (BHV *)memory;
        break;
    case CID_BHV162:
        SLY2_SET_VPTR(ClearBhv(memory, 0xC0), BHV162);
        behavior = (BHV *)memory;
        break;
    case CID_BHVKO:
        SLY2_SET_VPTR(ClearBhv(memory, 0xC0), BHVKO);
        behavior = (BHV *)memory;
        break;
    case CID_BHVATTACK:
        SLY2_SET_VPTR(ClearBhv(memory, 0x50), BHVATTACK);
        behavior = (BHV *)memory;
        break;
    case CID_BHVCOMBAT:
        SLY2_SET_VPTR(ClearBhv(memory, 0x60), BHVCOMBAT);
        behavior = (BHV *)memory;
        break;
    case CID_BHVWALKPATH:
        SLY2_SET_VPTR(ClearBhv(memory, 0xF0), BHVWALKPATH);
        behavior = (BHV *)memory;
        break;
    case CID_BHV169:
        SLY2_SET_VPTR(ClearBhv(memory, 0x60), BHV169);
        behavior = (BHV *)memory;
        break;
    case CID_BHVPATROL:
        SLY2_SET_VPTR(ClearBhv(memory, 0x50), BHVPATROL);
        behavior = (BHV *)memory;
        break;
    case CID_BHV171:
        SLY2_SET_VPTR(ClearBhv(memory, 0xE0), BHV171);
        behavior = (BHV *)memory;
        break;
    case CID_BHV174:
        SLY2_SET_VPTR(ClearBhv(memory, 0x40), BHV174);
        behavior = (BHV *)memory;
        break;
    case CID_BHVLEAP:
        SLY2_SET_VPTR(ClearBhv(memory, 0xA0), BHVLEAP);
        behavior = (BHV *)memory;
        break;
    case CID_BHV175:
        SLY2_SET_VPTR(ClearBhv(memory, 0x60), BHV175);
        behavior = (BHV *)memory;
        break;
    case CID_BHV176:
        SLY2_SET_VPTR(ClearBhv(memory, 0x40), BHV176);
        behavior = (BHV *)memory;
        break;
    case CID_BHVJUGGLED:
        SLY2_SET_VPTR(ClearBhv(memory, 0x70), BHVJUGGLED);
        behavior = (BHV *)memory;
        break;
    case CID_BHV173:
        SLY2_SET_VPTR(ClearBhv(memory, 0x40), BHVSHOOT);
        behavior = (BHV *)memory;
        break;
    case CID_BHV178:
        SLY2_SET_VPTR(ClearBhv(memory, 0x50), BHV178);
        behavior = (BHV *)memory;
        break;
    case CID_BHV179:
        SLY2_SET_VPTR(ClearBhv(memory, 0x40), BHV179);
        behavior = (BHV *)memory;
        break;
    case CID_BHV180:
        SLY2_SET_VPTR(ClearBhv(memory, 0x40), BHV180);
        behavior = (BHV *)memory;
        break;
    case CID_BHV181:
        SLY2_SET_VPTR(ClearBhv(memory, 0x60), BHV181);
        behavior = (BHV *)memory;
        break;
    case CID_BHV168:
        SLY2_SET_VPTR(ClearBhv(memory, 0xE0), BHVWALKGOAL);
        behavior = (BHV *)memory;
        break;
    case CID_BHV193:
        SLY2_SET_VPTR(ClearBhv(memory, 0x50), BHV193);
        behavior = (BHV *)memory;
        break;
    case CID_BHV194:
        SLY2_SET_VPTR(ClearBhv(memory, 0xA0), BHVFOLLOW);
        behavior = (BHV *)memory;
        break;
    case CID_BHV195:
        SLY2_SET_VPTR(ClearBhv(memory, 0x40), BHV195);
        behavior = (BHV *)memory;
        break;
    case CID_BHV196:
        SLY2_SET_VPTR(ClearBhv(memory, 0x40), BHV196);
        behavior = (BHV *)memory;
        break;
    case CID_BHV197:
        SLY2_SET_VPTR(ClearBhv(memory, 0x40), BHV197);
        behavior = (BHV *)memory;
        break;
    case CID_BHV182:
        SLY2_SET_VPTR(ClearBhv(memory, 0x50), BHV182);
        behavior = (BHV *)memory;
        break;
    case CID_BHV198:
        SLY2_SET_VPTR(ClearBhv(memory, 0x40), BHV198);
        behavior = (BHV *)memory;
        break;
    case CID_BHV183:
        SLY2_SET_VPTR(ClearBhv(memory, 0x70), BHV183);
        behavior = (BHV *)memory;
        break;
    case CID_BHVRAIL:
        SLY2_SET_VPTR(ClearBhv(memory, 0x40), BHVRAIL);
        behavior = (BHV *)memory;
        break;
    case CID_BHVPIPE:
        SLY2_SET_VPTR(ClearBhv(memory, 0x40), BHVPIPE);
        behavior = (BHV *)memory;
        break;
    case CID_BHV187:
        SLY2_SET_VPTR(ClearBhv(memory, 0x30), BHV187);
        behavior = (BHV *)memory;
        break;
    case CID_BHVGOTO:
        SLY2_SET_VPTR(ClearBhv(memory, 0xD0), BHVGOTO);
        ClearPointerPair((PATHLINKEND *)((char *)memory + 0x40));
        ClearPointerPair((PATHLINKEND *)((char *)memory + 0x60));
        ClearPointerPair((PATHLINKEND *)((char *)memory + 0x80));
        behavior = (BHV *)memory;
        break;
    case CID_BHV189:
        SLY2_SET_VPTR(ClearBhv(memory, 0x50), BHV189);
        behavior = (BHV *)memory;
        break;
    case CID_BHVPATHLINK:
        SLY2_SET_VPTR(ClearBhv(memory, 0x50), BHVPATHLINK);
        behavior = (BHV *)memory;
        break;
    case CID_BHV191:
        SLY2_SET_VPTR(ClearBhv(memory, 0x50), BHV191);
        ClearPointerPair((PATHLINKEND *)((char *)memory + 0x30));
        behavior = (BHV *)memory;
        break;
    case CID_BHVPATROLPATH:
        SLY2_SET_VPTR(ClearBhv(memory, 0x60), BHVPATROLPATH);
        behavior = (BHV *)memory;
        break;
    default:
        break;
    }
    return behavior;
}

// 0x1EF6F0: Returns the slot size every behaviour must fit in: 0xF0 bytes (the largest subclass, the walk-path
// behaviour 0xA7).
extern "C" int GetBhvSlotSize()
{
    return 0xF0;
}
