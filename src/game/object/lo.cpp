// lo.cpp -- the base object (LO : BASIC, lo.h): construction and init, the root record, parenting and the child
// list, load, deferred callbacks (LORUN), the default virtuals (render, translate, rotate, velocity...), world
// matrices and positions, visibility and busy state. TU 0x1A5270..0x1A7518 (74 functions, address order).
//
// Types: sly2/lo.h's LO (real virtual calls for GetWorldMatrix / TranslateToPos / RotateToMat), LOROOT and LORUN for the
// m2c bodies. The hand-written per-function files keep their views in namespaces named after each function (as
// jt.cpp does), their C++-named definitions as C-linkage definitions of their literal symbols there.
// Entry points: the LO virtuals (InitLo, AddLo, RemoveLo, LO_SetParent, UpdateLo, CloneLo...), the
// world-transform queries (GetLoWorldPos, GetLoWorldMatrix, ConvertAlo*), busy/visibility setters and deferred callbacks.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_lo
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_lo

#include "sly2/basic.h"
#include "sly2/port.h"
#include "sly2/lo.h"
#include "sly2/sw.h"
#include "sly2/cm.h"
#include "sly2/so.h"
#include "sly2/slotheap.h"
#include "sly2/scrap.h"
#include "sly2/vu0.h"
#include "sly2/dli.h"
#include "sly2/cref.h"
#include "sly2/clock.h"
#include "sly2/game.h"

/* The LO flag word at +0x28 (read as one 64-bit word by the m2c views; lo.h names the bits). */
#define LO_REMOVED_BIT    0x0100000000000000UL  /* bit 56 (isRemoved): taken out of the world */
#define LO_CLEAR_REFS_BIT 0x0800000000000000UL   /* bit 59: clear world references to it on removal */
#define LO_IN_WORLD_BIT   0x1000000000000000UL   /* bit 60 (isInWorld) */
#define LO_IN_WORLD_SHIFT 60
/* The 64-bit unit at +0x28 that holds lo.h's bit-fields, read whole (bits 0..31 overlap siblingLink.prev, so it is not
   a union member). InitLo copies the parent's isInWorld through it: the bit-field copy compiles to a same-position mask
   (and), where the original extracts and reinserts the bit with shifts (14/75 off). */
#define LO_FLAG_WORD(plo) (*(ulong *)((char *)(plo) + 0x28))

/* LO vtable slots the m2c bodies call through (index into the vtable; vt[n]). */
enum
{
    LO_SLOT_HANDLE_SPLICE_EVENT = 3,
    LO_SLOT_NOTIFY = 4,             /* message 0 added, 1 removed */
    LO_SLOT_ADD = 8,
    LO_SLOT_REMOVE = 9,
    LO_SLOT_SET_PARENT = 10,
    LO_SLOT_ADD_HIERARCHY = 11,
    LO_SLOT_REMOVE_HIERARCHY = 12,
    LO_SLOT_UPDATE_LOROOT = 13,
    LO_SLOT_ON_REMOVE = 22,
    LO_SLOT_ON_ADD = 23,
    LO_SLOT_GET_CHILDREN = 28,
    LO_SLOT_APPLY_TRANSFORM = 37,
};

// 0x1A73C0 StaticInit_lo and GlobalConstructors_lo: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

extern "C" {
extern qword D_002A2DD0;        /* (0, 0, 0) */
extern MATRIX4 D_002A2D30;      /* identity */
void *AllocZeroedFromSlotheap(SLOTHEAP *pslotheap);
void FreeLoroot(LO *plo);
void GetLoChildDl(SO *psoParent, DL **ppdl, LO **pploFirst);
LORUN *AddLoDeferredCallback(LO *plo);
void DeferLoRemoveCallback(LO *plo, void *pv);
void *AllocSwLoCallbackRecord(SW *world);
void RemoveLoFromSwUpdateList(SW *world, LO *plo);
void AddLoToSwUpdateList(SW *world, LO *plo);
void SetLoXfFromMat(LO *plo, MATRIX4 *matrix);
}
void RemoveDlEntry(DL *pdl, void *pv);
void LoadOptionsFromBrx(void *pv, CBinaryInputStream *stream);

namespace M_InitLo {
void EnsureLoroot(LO *s) __asm__("EnsureLoroot");
void GetLoChildDl(SO *p, void **a, void ***b) __asm__("GetLoChildDl");
void InsertDlEntryBefore__FP2DLPvT1(void *a, void *b, LO *s) __asm__("InsertDlEntryBefore__FP2DLPvT1");
// LO initialization (vtable slot 7): sets the busy defaults (busy kind 2 "never", busy distance 1e10), inherits the
// root record and the in-world bit from the parent (or makes its own root), sets the default flags and proxy-ref list,
// and links the object into its parent's child list. Every object class calls it from its own Init.
} // namespace M_InitLo
// LO::Init: base initialisation for every object (root record, in-world bit, default flags, proxy refs, parent link);
// see the comment above.
// Also known as: InitLo (upstream readable / Sly 1 name; InitLo__FP2LO).
void LO::Init()
{
    using namespace M_InitLo;
    void *a;
    void **b;
    proxyLoadKind = 0xF;
    busyRadius = 1e10f;
    busyKind = 2;
    visibilityMask = 0xFF;
    if (parent) {
        root = parent->root;
        isInWorld = (int)(LO_FLAG_WORD(parent) >> LO_IN_WORLD_SHIFT);   /* whole word (LO_FLAG_WORD) */
    } else {
        M_InitLo::EnsureLoroot(this);
        isInWorld = 1;
    }
    inlineFlags = 0x21;
    proxyRecord = parent ? parent->proxyRecord : g_sw.ppxrRoot;
    M_InitLo::GetLoChildDl(parent, &a, &b);
    InsertDlEntryBefore__FP2DLPvT1(a, *b, this);
}

// Gives the object a root record (LOROOT, from the world's pool) when it has none; the root tracks a top-level object
// in the world's busy and in-world lists.
extern "C" void EnsureLoroot(LO *plo)
{
    if (plo->root == 0) {
        LOROOT *ploroot = (LOROOT *)AllocZeroedFromSlotheap(&g_sw.slotheapLoroot);
        plo->root = ploroot;
        ploroot->topObject = plo;
    }
}

namespace M_FreeLoroot {
int IsEntryInDl__FP2DLPv(void *, void *) __asm__("IsEntryInDl__FP2DLPv");
void RemoveDlEntry__FP2DLPv(void *, void *) __asm__("RemoveDlEntry__FP2DLPv");
void RemoveFromSwBusyList(void *, void *) __asm__("RemoveFromSwBusyList");
void FreeSlotheapSlot(void *, void *) __asm__("FreeSlotheapSlot");
// Unlinks the object's root record from the world's root and busy lists, frees it and clears the object's pointer.
extern "C" void FreeLoroot(LO *p)
{
    DL *pdl = &g_sw.busyRoots;
    if (IsEntryInDl__FP2DLPv(pdl, p->root))
        RemoveDlEntry__FP2DLPv(pdl, p->root);
    RemoveFromSwBusyList(&g_sw, p->root);
    FreeSlotheapSlot(&g_sw.slotheapLoroot, p->root);
    p->root = 0;
}
} // namespace M_FreeLoroot

namespace M_GetLoChildDl {
// Returns the child list an object lives in and its "first removed child" slot: the parent's when there is one, else
// the world's top-level list.
void GetLoChildDl(SO *parent, void **childList, void **firstRemovedSlot) __asm__("GetLoChildDl"); void GetLoChildDl(SO *parent, void **childList, void **firstRemovedSlot)
{
    if (parent) { *childList = &parent->children; *firstRemovedSlot = &parent->ploRemovedFirst; }
    else { *childList = &g_sw.dlChild; *firstRemovedSlot = &g_sw.ploRemovedFirst; }
}
} // namespace M_GetLoChildDl

namespace M_AddLo {
void GetLoChildDl(SO *p, void **a, LO ***firstRemoved) __asm__("GetLoChildDl");
void RemoveDlEntry__FP2DLPv(void *a, LO *s) __asm__("RemoveDlEntry__FP2DLPv");
void InsertDlEntryBefore__FP2DLPvT1(void *a, LO *firstRemoved, LO *s) __asm__("InsertDlEntryBefore__FP2DLPvT1");
void ResortSwBusyEntry(void *g, LOROOT *n) __asm__("ResortSwBusyEntry");
int IsLoInWorld(LO *s) __asm__("IsLoInWorld");
void UpdateSoBusy(void *g, LO *s) __asm__("UpdateSoBusy");
// Adds the object to the world (vtable slot 8): clears the removed bit, moves it into the in-world part of its parent's
// child list, runs AddHierarchy when the parent is in the world, registers its root in the busy list and adds the
// children recursively, then calls OnAdd.
extern "C" void AddLo(LO *object)
{
    unsigned long isParentInWorld;
    void *childList;
    LO **firstRemoved;
    LO **list;
    int numChildren, i;
    if (!object->isRemoved)
        return;
    object->isRemoved = 0;
    GetLoChildDl(object->parent, &childList, &firstRemoved);
    if (object == *firstRemoved) {
        *firstRemoved = (LO *)object->siblingLink.next;
    } else {
        RemoveDlEntry__FP2DLPv(childList, object);
        InsertDlEntryBefore__FP2DLPvT1(childList, *firstRemoved, object);
    }
    if (!object->parent || (isParentInWorld = object->parent->isInWorld & 1) != 0) {
        object->AddHierarchy();
    }
    if (!object->parent && (int)object->busyKind != 2)
        ResortSwBusyEntry(&g_sw, object->root);
    numChildren = object->GetChildren(&list);
    for (i = 0; i < numChildren; i++) {
        LO *child = list[i];
        child->Add();
    }
    if (IsLoInWorld(object)) {
        object->OnAdd();
    } else if (!object->parent) {
        UpdateSoBusy(&g_sw, object);
    }
}
} // namespace M_AddLo

namespace M_RemoveLo {

int IsLoInWorld(LO *s) __asm__("IsLoInWorld");
void RemoveDlEntry__FP2DLPv(void *a, void *firstRemoved) __asm__("RemoveDlEntry__FP2DLPv");
void RemoveFromSwBusyList(void *a, LOROOT *b) __asm__("RemoveFromSwBusyList");
void GetLoChildDl(void *p, void **a, LO ***firstRemoved) __asm__("GetLoChildDl");
void AppendDlEntry__FP2DLPv(void *a, LO *s) __asm__("AppendDlEntry__FP2DLPv");
// Removes the object from the world (vtable slot 9): unregisters it, moves it to the removed tail of its parent's child
// list, sets the removed bit, removes the children recursively and runs RemoveHierarchy when it was in the world.
extern "C" void RemoveLo(LO *object)
{
    unsigned long wasInWorld;
    void *childList;
    LO **firstRemoved;
    LO **list;
    int numChildren, i;
    if (object->isRemoved)
        return;
    if (IsLoInWorld(object)) {
        object->OnRemove();
        if (!object->parent)
            RemoveDlEntry__FP2DLPv((*(char (*)[])&g_sw.busyRoots), object->root);
    }
    if (!object->parent)
        RemoveFromSwBusyList(&g_sw, object->root);
    wasInWorld = object->isInWorld & 1;
    GetLoChildDl(object->parent, &childList, &firstRemoved);
    RemoveDlEntry__FP2DLPv(childList, object);
    AppendDlEntry__FP2DLPv(childList, object);
    if (!*firstRemoved)
        *firstRemoved = object;
    object->isRemoved = 1;
    numChildren = object->GetChildren(&list);
    for (i = 0; i < numChildren; i++) {
        LO *child = list[i];
        child->Remove();
    }
    if (wasInWorld) {
        object->RemoveHierarchy();
    }
}
} // namespace M_RemoveLo

// Unlinks the object from its parent's child list without any add/remove callbacks.
extern "C" void SnipLo(LO *plo)
{
    DL *pdl;
    LO **pploFirst;
    GetLoChildDl(plo->parent, &pdl, (LO **)&pploFirst);
    if (*pploFirst == plo) {
        *pploFirst = (LO *)plo->siblingLink.next;
    }
    RemoveDlEntry(pdl, plo);
}

namespace M_AddLoHierarchy {
// vtable slot 11: sets the in-world bit and sends the "added" notification (SendMessage, message 0).
// Also known as: OnLoAdd (upstream readable / Sly 1 name; OnLoAdd__FP2LO).
extern "C" void AddLoHierarchy(LO *object)
{
    object->isInWorld = 1;
    object->SendMessage((MSGID)0, object);
}
} // namespace M_AddLoHierarchy

namespace M_RemoveLoHierarchy {
void RemoveSwPlacesOf(void *g, LO *s, int n) __asm__("RemoveSwPlacesOf");
// vtable slot 12: clears the in-world bit, clears references to the object when it is flagged for that, and sends the
// "removed" notification (SendMessage, message 1).
// Also known as: OnLoRemove (upstream readable / Sly 1 name; OnLoRemove__FP2LO).
extern "C" void RemoveLoHierarchy(LO *object)
{
    object->isInWorld = 0;
    if (object->clearsPlacesOnRemove)
        RemoveSwPlacesOf(&g_sw, object, 0);
    object->SendMessage((MSGID)1, object);
}
} // namespace M_RemoveLoHierarchy

namespace M_SetLoParent_1A58D0 {
// Reparents an object: calls its SetParent (vtable slot 10) with the new parent and flags.
// Also known as: ReparentBooty (upstream readable / Sly 1 name; ReparentBooty__FP3ALOii).
extern "C" void SetLoParent(LO *object, SO *a, int b) { object->SetParent(a, 0, b); }
} // namespace M_SetLoParent_1A58D0

namespace M_func_001A5900 {
/* (Claude agent) MATCH: parent bit via (int)(bit)&1, fNew via if(!removed) fNew=1; else fNew=0; (gives addiu/movn). */
void GetLoChildDl(SO *p, void **ppdl, LO ***pppalo) __asm__("GetLoChildDl");
void RemoveDlEntry__FP2DLPv(void *pdl, void *p) __asm__("RemoveDlEntry__FP2DLPv");
void AppendDlEntry__FP2DLPv(void *pdl, void *p) __asm__("AppendDlEntry__FP2DLPv");
void InsertDlEntryBefore__FP2DLPvT1(void *pdl, void *pBefore, void *p) __asm__("InsertDlEntryBefore__FP2DLPvT1");
int IsLoInWorld(LO *p) __asm__("IsLoInWorld");
void UpdateSoBusy(void *g, LO *p) __asm__("UpdateSoBusy");
// LO's SetParent (vtable slot 10): moves the object from the old to the new parent's child list keeping its in-world
// state, freezes it while in the world, sets the parent pointer, updates the root record (slot 13) and the children,
// and re-adds or notifies as needed.
extern "C" void LO_SetParent(LO *object, SO *newParent, XF *pxfParent, int grf)
{
    unsigned long wasInWorld = object->isInWorld & 1;
    unsigned long isInWorld = 0;
    void *oldList;
    LO **oldFirstRemoved;
    void *newList;
    LO **newFirstRemoved;
    SO *oldParent;
    LO **children;
    int numChildren, i;
    if (!newParent || ((int)newParent->isInWorld & 1)) {
        if (!object->isRemoved) isInWorld = 1; else isInWorld = 0;
    }
    GetLoChildDl(object->parent, &oldList, &oldFirstRemoved);
    GetLoChildDl(newParent, &newList, &newFirstRemoved);
    if (object->isRemoved) {
        if (object == *oldFirstRemoved)
            *oldFirstRemoved = (LO *)object->siblingLink.next;
        RemoveDlEntry__FP2DLPv(oldList, object);
        AppendDlEntry__FP2DLPv(newList, object);
        if (!*newFirstRemoved)
            *newFirstRemoved = object;
    } else {
        RemoveDlEntry__FP2DLPv(oldList, object);
        InsertDlEntryBefore__FP2DLPvT1(newList, *newFirstRemoved, object);
    }
    if (IsLoInWorld(object)) {
        object->OnRemove();
        if (object->root->topObject == object)
            RemoveDlEntry__FP2DLPv((*(char (*)[])&g_sw.busyRoots), object->root);
    }
    oldParent = object->parent;
    object->parent = newParent;
    object->UpdateLoroot(object, oldParent);
    numChildren = object->GetChildren(&children);
    for (i = 0; i < numChildren; i++) {
        LO *child = children[i];
        child->SetParent(newParent, pxfParent, grf);
    }
    if (isInWorld != wasInWorld) {
        if (isInWorld)
            object->AddHierarchy();
        else
            object->RemoveHierarchy();
    }
    if (IsLoInWorld(object))
        object->OnAdd();
    else if (!object->parent && isInWorld)
        UpdateSoBusy(&g_sw, object);
}
} // namespace M_func_001A5900

// vtable slot 13: with a parent, shares the parent's root record (freeing its own if it had one); without a parent and
// when makeRoot is set, makes its own root.
/* LO slot UpdateLoroot: share the parent's root, or make one of its own. */
extern "C" void UpdateLoLoroot(LO *plo, LO *ploRoot, int fRoot)
{
    if (plo->parent != 0) {
        if (ploRoot == plo && fRoot == 0) {
            FreeLoroot(plo);
        }
        plo->root = ((LO *)plo->parent)->root;
        return;
    }
    if (fRoot != 0) {
        plo->root = 0;
        EnsureLoroot(plo);
    }
}

// LO's child list query (vtable slot 28): a plain LO has no children; returns 0. (inferred: derived classes return
// the count and fill the array.)
extern "C" int LO_GetChildren()
{
    return 0;
}

namespace M_PsoFromLo {
int IsBasicDerivedFrom__FP5BASIC3CID(LO *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
// Returns the object itself when it is an SO (cid 0x30), else its parent SO.
// Also known as: PaloOrParent (upstream readable / Sly 1 name; PaloOrParent__FPv).
extern "C" SO *GetSoFromLo(LO *object) { if (IsBasicDerivedFrom__FP5BASIC3CID(object, 0x30)) return (SO *)object; return object->parent; }
} // namespace M_PsoFromLo

// Loads an LO from the BRX (vtable slot 14): reads its option records until the end marker.
// Layout parse-verified: a parser following it reads every byte of 4 levels.
// Also known as: LoadLoFromBrx (upstream readable / Sly 1 name; LoadLoFromBrx__FP2LOP18CBinaryInputStream).
extern "C" void LoadLoFromBrx(LO *plo, CBinaryInputStream *stream)
{
    LoadOptionsFromBrx(plo, stream);
}

namespace M_PostLoLoad {
extern float D_002A6A40 __asm__("D_002A6A40");
void RunLoDeferredCallbacks(LO *s) __asm__("RunLoDeferredCallbacks");
int FindBindingPtr__6CFrameUii(void *p, int a, int b) __asm__("FindBindingPtr__6CFrameUii");
void SetLoBusyKind(LO *s, int n) __asm__("SetLoBusyKind");
// After the level load (vtable slot 15): runs the deferred callbacks and sends Splice message 0; an object whose Splice
// frame binds an update handler becomes busy and wants Splice updates, others get the default next Splice time.
// Also known as: PostLoLoad (upstream readable / Sly 1 name; PostLoLoad__FP2LO).
extern "C" void PostLoLoad(LO *object)
{
    RunLoDeferredCallbacks(object);
    object->HandleSpliceEvent(0, 1, 0, 0, 0);
    if (object->scriptFrame.m_p && FindBindingPtr__6CFrameUii(object->scriptFrame.m_p, 1, 1)) {
        sptr v;
        if ((int)object->busyKind == 2)
            SetLoBusyKind(object, 0);
        v = (sptr)object->ploupd;
        if ((v ^ 1) & 1) object->ploupd->grf |= 2;
        else object->inlineFlags = v | 2;
    } else {
        object->nextSpliceTime = D_002A6A40;
    }
}
} // namespace M_PostLoLoad

namespace M_DeferLoRemoveCallback {
// The deferred-callback body that removes the object (its Remove, vtable slot 9).
void DeferLoRemoveCallback(LO *object) __asm__("DeferLoRemoveCallback"); void DeferLoRemoveCallback(LO *object) { object->Remove(); }
} // namespace M_DeferLoRemoveCallback

// Queues the object's removal to run with its deferred callbacks instead of immediately.
extern "C" void DeferLoRemove(LO *plo)
{
    AddLoDeferredCallback(plo)->pfn = DeferLoRemoveCallback;
}

// Allocates a deferred-callback record {function, argument, next} from the world's array and pushes it on the
// object's callback list; returns the record for the caller to fill.
/* A new deferred callback record on the object's list. */
extern "C" LORUN *AddLoDeferredCallback(LO *plo)
{
    LORUN *plorun = (LORUN *)AllocSwLoCallbackRecord(&g_sw);
    plorun->next = plo->firstRunCallback;
    plo->firstRunCallback = plorun;
    return plorun;
}

namespace M_RunLoDeferredCallbacks {
// Runs every deferred callback queued on the object, in the order they were added.
extern "C" void RunLoDeferredCallbacks(LO *object)
{
    LORUN *a[32];
    LORUN *e = object->firstRunCallback;
    int n;
    if (!e)
        return;
    n = 0;
    do { a[n] = e; n++; e = e->next; } while (e);
    for (n--; n >= 0; n--) { e = a[n]; e->pfn(object, e->argument); }
}
} // namespace M_RunLoDeferredCallbacks

namespace M_UpdateLo {
// LO's per-frame update (vtable slot 16): when the object's next Splice time has come, sends Splice message 1 (the
// update event).
// Also known as: UpdateLo (upstream readable / Sly 1 name; UpdateLo__FP2LO).
extern "C" void UpdateLo(LO *object)
{
    if (object->nextSpliceTime <= g_clock.time) {
        object->HandleSpliceEvent(1, 1, 0, 0, 0);
    }
}
} // namespace M_UpdateLo

namespace M_func_001A5E80 {
// Clear flags fl: on p->flags when bit 0 is set, otherwise on the word p->flags points to.
extern "C" { static inline void ClearFlag(LO *p, unsigned int fl)
{
    uptr v = (uptr)p->ploupd;
    if ((v ^ 1) & 1) p->ploupd->grf &= ~fl;
    else p->inlineFlags = v & ~fl;
} }
// vtable slot 17: clears flag 4 unless the parent has flag 4 set (the flag is inherited from the parent; its meaning
// is not known; the parent's flag word is read through its pointer form). (inferred)
extern "C" void InheritLoFlag4(LO *object)
{
    if (!object->parent || !(object->parent->ploupd->grf & 4)) ClearFlag(object, 4);
}
} // namespace M_func_001A5E80

// LO's RenderSelf (vtable slot 19): a plain LO draws nothing.
/* default (empty) virtual */
extern "C" void RenderLoSelf()
{
}

// LO's RenderAll (vtable slot 20): a plain LO draws nothing; SO overrides it with RenderSoAll. Sly 1: RenderAloAll.
/* default (empty) virtual */
extern "C" void RenderLoAll()
{
}

namespace M_func_001A5EE8 {
typedef struct { qword q[4]; } M4;
void InvertMatrix4Rigid(M4 *out, void *in) SLY2_SRET_LABEL("InvertMatrix4Rigid", "InvertMatrix4RigidInto");
void __ml__FRC7MATRIX4T0(M4 *out, void *a, M4 *b) SLY2_SRET_LABEL("__ml__FRC7MATRIX4T0", "MulMatrix4Into");
// Draws an object hierarchy at an explicit world matrix: combines the matrix with the inverse of the object's world
// matrix and calls RenderAll (slot 20) with it and two alpha factors.
// Also known as: AppendAloExtraDraws (upstream readable / Sly 1 name; AppendAloExtraDraws__FP5SCENEP3ALOff).
extern "C" void RenderLoAtMatrix(LO *object, void *m, float a, float b)
{
    M4 t;
    M4 r;
    M4 inv;
    M4 *pr = &r;
    M4 *pinv = &inv;
    InvertMatrix4Rigid(pinv, object->GetWorldMatrix());
    __ml__FRC7MATRIX4T0(pr, m, pinv);
    t = r;
    object->RenderAll((MATRIX4 *)&t, a, b, 0);
}
} // namespace M_func_001A5EE8

namespace M_func_001A5FB8 {
typedef union { qword q; float f[4]; int i[4]; } U;
struct M4 { U r[4]; };
qword *VECTOR_opAssign(qword *p, qword v) __asm__("VECTOR_opAssign");
void RenderLoAtMatrix(void *p, struct M4 *m, float a, float b) __asm__("RenderLoAtMatrix");
// Length of the xyz part of v (x lane).
extern "C" { static inline qword LenV(qword v) { qword l,t; __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(l),"=&j"(t) : "j"(v)); __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(l) : "0"(l));return l; } }
// Unit vector along v.
extern "C" { static inline qword Normalize(qword v) { qword l=LenV(v),r; __asm__("vdiv Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,Q" : "=j"(r) : "j"(l),"j"(v));return r; } }
// Draws an object stretched along the segment start->end, facing the camera (width scaling, 0.01 epsilon), through
// RenderLoAtMatrix. Used for beams and similar effects. (inferred)
// Also known as: AppendLaserLine (upstream readable / Sly 1 name; AppendLaserLine__FP5SCENEG9VU_VECTORT1ff).
extern "C" void RenderLoAlongSegment(void *object,qword a,qword b,float width,float scale)
{
 U x,y,z;
 struct M4 m; struct M4 *pm;
 U uk;
 qword d,n,t,k;
 float len;
 d=VuSub(b,a);
 n=VuCross(VuSub<qword>(a,g_cm.matq.pos),d);
 y.q=n;
 len=VuToFloat(LenV(n));
 if(0.01f<len) {
  len=width/len;
  pm=&m; t=Normalize(VuCross(n,d));
  x.q=t;
  k=VuFromFloat<qword>(len);
  uk.q=k;
  n=VuScaleByX(n,k);
  y.q=n;
  k=VuFromFloat<qword>(0.01f);
  uk.q=k;
  z.q=VuScaleByX(d,k);
  VECTOR_opAssign(&m.r[3].q,a);
  pm->r[0].q=x.q;
  pm->r[1].q=y.q;
  pm->r[2].q=z.q;
  m.r[0].i[3]=m.r[1].i[3]=m.r[2].i[3]=0;
  pm->r[3].f[3]=1.0f;
  RenderLoAtMatrix(object,pm,scale,scale);
 }
}
} // namespace M_func_001A5FB8

// LO's TranslateToPos (vtable slot 29): a plain LO has no position to change.
/* default (empty) virtual */
extern "C" void TranslateLoToPos()
{
}

// LO's RotateToMat (vtable slot 30): a plain LO has no orientation to change.
/* default (empty) virtual */
extern "C" void RotateLoToMat()
{
}

// LO's GetVelocity (vtable slot 33): a plain LO does not move; returns the zero vector.
/* LO GetVelocity default: zero. */
extern "C" qword GetLoVelocity()
{
    return D_002A2DD0;
}

// LO's SetVelocity (vtable slot 35): no-op for a plain LO.
/* default (empty) virtual */
extern "C" void SetLoVelocity()
{
}

// LO's GetAngularVelocity (vtable slot 34): returns the zero vector.
extern "C" qword GetLoAngularVelocity()
{
    return D_002A2DD0;
}

// LO's SetAngularVelocity (vtable slot 36): no-op.
/* default (empty) virtual */
extern "C" void SetLoAngularVelocity()
{
}

// LO's OnRemove (vtable slot 22): unregisters the object from the world's per-object update table.
extern "C" void OnLoRemove(LO *plo)
{
    RemoveLoFromSwUpdateList(&g_sw, plo);
}

// LO's OnAdd (vtable slot 23): registers the object in the world's per-object update table.
extern "C" void OnLoAdd(LO *plo)
{
    AddLoToSwUpdateList(&g_sw, plo);
}

namespace M_func_001A61A0 {
int IsEntryInDl__FP2DLPv(struct DL *pdl, void *pv) __asm__("IsEntryInDl__FP2DLPv");
// True when the object's in-world bit is set and its root record is in the world's in-world list.
extern "C" int IsLoInWorld(LO *object)
{
    int r = 0;
    if ((int)object->isInWorld & 1) r = IsEntryInDl__FP2DLPv((DL *)&g_sw.busyRoots, object->root) != 0;
    return r;
}
} // namespace M_func_001A61A0

namespace M_HandleLoSpliceEvent {
void HandleBasicSpliceEvent(void *p, int n, int deferred, int argumentCount,
                   void *const *arguments, CRefData *output) __asm__("HandleBasicSpliceEvent");
void BroadcastGameEvent(void *p, int n, void *q) __asm__("BroadcastGameEvent");
#define FIELD_OFFSET(t,m) ((unsigned int)&(((struct t*)0x10000)->m)-0x10000)
typedef char Pointer_Size[sizeof(void*)==4?1:-1];
typedef char Int_Size[sizeof(int)==4?1:-1];
// LO's Splice event handler (vtable slot 3): the BASIC handling, then the trigger (5) and untrigger (6) events are also
// reported to the game progress object (as game events 0x1A/0x1B).
extern "C" void HandleLoSpliceEvent(void *object, int n, int deferred, int argumentCount,
                   void *const *arguments, CRefData *output)
{
    HandleBasicSpliceEvent(object, n, deferred, argumentCount, arguments, output);
    switch (n) {
    case 5:
        BroadcastGameEvent(&g_game, 0x1A, object);
        break;
    case 6:
        BroadcastGameEvent(&g_game, 0x1B, object);
        break;
    }
}
} // namespace M_HandleLoSpliceEvent
#undef FIELD_OFFSET

namespace M_OidProxyLo {
// Returns the oid of the proxy the object was cloned from (the last proxy record owned by it), or -1.
extern "C" int GetLoProxyOid(LO *object)
{
    int r = -1;
    PXR *node = object->proxyRecord;
    while (node) {
        if (node->object != object) break;
        r = node->proxyOid;
        node = node->parent;
    }
    return r;
}
} // namespace M_OidProxyLo

namespace M_FMatchesLoName {
// True when the object's oid, or the oid of one of its proxy records, equals oid.
extern "C" int HasLoOid(LO *object, int oid)
{
    PXR *node;
    if (object->oid == oid) return 1;
    for (node = object->proxyRecord; node && node->object == object; node = node->parent)
        if (node->proxyOid == oid) return 1;
    return 0;
}
} // namespace M_FMatchesLoName

// LO's world matrix (vtable slot 31): a plain LO returns the identity.
extern "C" MATRIX4 *LO_GetWorldMatrix()
{
    return &D_002A2D30;
}

// Returns the object's world position (the translation row of its world matrix, slot 31).
/* The object's world position (row 3 of its world matrix). */
// Also known as: PosOfLo (upstream readable / Sly 1 name; PosOfLo__FP2LO).
extern "C" qword GetLoWorldPos(LO *plo)
{
    return ((qword *)plo->GetWorldMatrix())[3];
}

// Returns the object's world matrix (vtable slot 31).
// Also known as: MatJtLo (upstream readable / Sly 1 name; MatJtLo__FP2LO).
extern "C" void GetLoWorldMatrix(LO *plo)
{
    plo->GetWorldMatrix();
}

namespace M_func_001A6340 {
extern void *GetLoWorldMatrix(void) __asm__("GetLoWorldMatrix");
extern qword DecomposeRotationMatrixToEuler(void *p) __asm__("DecomposeRotationMatrixToEuler");
// Returns the object's world orientation as Euler angles in degrees.
extern "C" qword GetLoEulerDegrees(void)
{
    union { qword q; float f; } r;
    qword v = DecomposeRotationMatrixToEuler(GetLoWorldMatrix());
    r.q = QwordFromFloats<qword>(QwordX(v) * 57.2957763671875f, QwordY(v) * 57.2957763671875f, QwordZ(v) * 57.2957763671875f);
    return r.q;
}
} // namespace M_func_001A6340

namespace M_func_001A63B0 {
// Pack x, y, z into a 128-bit GPR value.
extern "C" { static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2
	mfc1 %1,%3
	pextlw %0,%1,%0
	mfc1 %1,%4
	pcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
} }
void LoadRotateMatrixEuler(MATRIX3Q *pm, qword v) __asm__("LoadRotateMatrixEuler");
// Sets the object's orientation from Euler angles in degrees (through RotateToMat, slot 30).
extern "C" void SetLoEulerDegrees(struct LO *object, qword v)
{
    MATRIX3Q c;
    union { qword q; float f; } r;
    MATRIX3Q m;
    r.q = Make(QwordX(v) * 0.017453294f, QwordY(v) * 0.017453294f, QwordZ(v) * 0.017453294f);
    LoadRotateMatrixEuler(&m, r.q);
    c = m;
    object->RotateToMat((MATRIX3 *)&c);   /* a 3x3 rotation (M3) */
}
} // namespace M_func_001A63B0

namespace M_func_001A6450 {
// LO's ApplyTransform (vtable slot 37: premultiply by a matrix; XFM_ApplyTransform is the XFM version): a plain LO
// has no transform of its own, so it forwards the matrix to every child returned by slot 28. (inferred)
extern "C" void LO_ApplyTransform(LO *object, MATRIX4 *arg)
{
    LO **list;
    int n, i;
    n = object->GetChildren(&list);
    for (i = 0; i < n; i++) {
        LO *p = list[i];
        p->ApplyProxy(arg);
    }
}
} // namespace M_func_001A6450

namespace M_BindLoOidTable {
struct E {int flags,id,fieldOffset;};   /* one reference: how to find it, its id, where to store it */
enum { BIND_PART = 0x10, BIND_SO_PART = 0x20, BIND_SO_GLOBS = 0x40, BIND_HIDE_OR_REMOVE = 0x200 };
void*FindSoPart(SO*,int) __asm__("FindSoPart");
void HideSoPart(SO*, void*) __asm__("HideSoPart");
void*FindSoGlob(SO*,int) __asm__("FindSoGlob");
void HideSoGlobsById(SO*,int) __asm__("HideSoGlobsById");
struct LO*FindSwObject__FP2SWi3OIDP2LO(void*,int,int,SO*) __asm__("FindSwObject__FP2SWi3OIDP2LO");
// Resolves a table of {flags, id, field offset} object references into pointer fields of the object: transforms (XF) by oid,
// SO sub-objects, or objects found by oid in the world; flag 0x200 removes the object found.
// Also known as: BindLoParts (upstream readable / Sly 1 name; BindLoParts__FP3ALOiPv).
extern "C" void BindLoOidTable(SO *object, int numEntries, struct E *entries)
{
    int i = 0;
    while (i < numEntries) {
        struct E *entry = &entries[i];
        int flags = entry->flags;
        if (flags & BIND_PART) {
            /* The count and byte offset share the native search registers. */
            register int j __asm__("$7") = object->numTransforms;
            register int offset __asm__("$6") = j * (int)sizeof(XF);
            XF *part;
            i++;
            /* search the parts from the last one down (this loop shape is what matches) */
        nextPart:
            j--;
            offset -= (int)sizeof(XF);
            if (j < 0)
                goto notFound;
            part = (XF *)(offset + (sptr)object->transforms);
            if (part->oid != entry->id)
                goto nextPart;
            *(void **)((char *)object + entry->fieldOffset) = part;
        notFound:;
        } else if (flags & BIND_SO_PART) {
            void *found = FindSoPart(object, entry->id);
            if (found) {
                i++;
                *(void **)((char *)object + entry->fieldOffset) = found;
                if (entry->flags & BIND_HIDE_OR_REMOVE)
                    HideSoPart(object, found);
            } else
                i++;
        } else if (flags & BIND_SO_GLOBS) {
            void *found = FindSoGlob(object, entry->id);
            if (found) {
                i++;
                *(void **)((char *)object + entry->fieldOffset) = found;
                if (entry->flags & BIND_HIDE_OR_REMOVE)
                    HideSoGlobsById(object, entry->id);
            } else
                i++;
        } else {
            int findKind;
            struct LO *found;
            if (flags & 2)
                findKind = 4;
            else
                findKind = (flags & 1) ? 5 : 1;
            found = FindSwObject__FP2SWi3OIDP2LO(&g_sw, (entry->flags & 0x400) ? findKind | 0x400 : findKind, entry->id, object);
            if (!found) {
                i++;
                continue;
            }
            if (entry->flags & BIND_HIDE_OR_REMOVE)
                found->Remove();
            i++;
            *(void **)((char *)object + entry->fieldOffset) = found;
        }
    }
}

#define OFFSET(t,m) ((unsigned int)&(((t *)0x10000)->m)-0x10000)
typedef char layout_entry_flags[OFFSET(struct E,flags)==0?1:-1];
typedef char layout_entry_id[OFFSET(struct E,id)==4?1:-1];
typedef char layout_entry_offset[OFFSET(struct E,fieldOffset)==8?1:-1];
typedef char layout_entry_size[sizeof(struct E)==12?1:-1];
typedef char layout_word_size[sizeof(int)==4?1:-1];
typedef char layout_pointer_size[sizeof(void*)==4?1:-1];
} // namespace M_BindLoOidTable
#undef OFFSET

namespace M_func_001A66C8 {
// vtable slot 47: sets bits in the object's flag word.
extern "C" void SetLoFlags(LO *object, unsigned int fl)
{
    uptr v = (uptr)object->ploupd;
    if ((v ^ 1) & 1) object->ploupd->grf |= fl;
    else object->inlineFlags = v | fl;
}
} // namespace M_func_001A66C8

namespace M_func_001A66F8 {
// vtable slot 48: clears bits in the object's flag word.
extern "C" void ClearLoFlags(LO *object, unsigned int fl)
{
    uptr v = (uptr)object->ploupd;
    if ((v ^ 1) & 1) object->ploupd->grf &= ~fl;
    else object->inlineFlags = v & ~fl;
}
} // namespace M_func_001A66F8

namespace M_CloneLo {
#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
typedef unsigned int uint;
typedef unsigned long ulong;
struct BINDING{int refs,pad4,pad8;int cref[2];int next;};
struct FIELD{int refs,pad4;BINDING*binding;};
struct ROOT{int refs,pad4,count;void*items[4];BINDING*binding;};
struct RF;
struct RR;
struct REQUEST{int bytes,alignment;};
struct META{char pad[0xC];uint bytes;};
typedef char BINDING_Size[sizeof(BINDING)==24?1:-1];
typedef char FIELD_Size[sizeof(FIELD)==12?1:-1];
typedef char ROOT_Size[sizeof(ROOT)==32?1:-1];
typedef char REQUEST_Size[sizeof(REQUEST)==8?1:-1];
typedef char META_Size[sizeof(META)==16?1:-1];
typedef char BINDING_refs_Offset[FIELD_OFFSET(BINDING,refs)==0?1:-1];
typedef char BINDING_cref_Offset[FIELD_OFFSET(BINDING,cref)==12?1:-1];
typedef char BINDING_next_Offset[FIELD_OFFSET(BINDING,next)==20?1:-1];
typedef char FIELD_refs_Offset[FIELD_OFFSET(FIELD,refs)==0?1:-1];
typedef char FIELD_binding_Offset[FIELD_OFFSET(FIELD,binding)==8?1:-1];
typedef char ROOT_refs_Offset[FIELD_OFFSET(ROOT,refs)==0?1:-1];
typedef char ROOT_items_Offset[FIELD_OFFSET(ROOT,items)==12?1:-1];
typedef char ROOT_binding_Offset[FIELD_OFFSET(ROOT,binding)==28?1:-1];
typedef char META_bytes_Offset[FIELD_OFFSET(META,bytes)==12?1:-1];

extern char g_gc[] __asm__("g_gc"),D_004520D0[] __asm__("_t9CSlotPool2i24i2048$s_pool"),D_00452148[] __asm__("_t9CSlotPool2i12i200$s_pool");
void CGc_UpdateRecyclable(void*) __asm__("CGc_UpdateRecyclable");
void func_00257250(void*,int) __asm__("_$_t7CRefPtr1Z5CVarb");
void Decref__4CRef(void*) __asm__("Decref__4CRef");
void Unlink__t6CGcObj1Z5CVarb(void*) __asm__("Unlink__t6CGcObj1Z5CVarb");
void Unlink__t6CGcObj1Z8CSidebag(void*) __asm__("Unlink__t6CGcObj1Z8CSidebag");
void FreeSlotheapSlot(void*,void*) __asm__("FreeSlotheapSlot");
void func_00257078(ROOT*,int) __asm__("_$_6CFrame");
void AssignSidebagRef(FIELD**,FIELD*) __asm__("AssignSidebagRef");
void FramePtr_Assign(ROOT**,ROOT*) __asm__("__as__t7CRefPtr1Z6CFrameP6CFrame");
int MakeRoot__t6CGcObj1Z8CSidebag(FIELD*) __asm__("MakeRoot__t6CGcObj1Z8CSidebag");
int MakeRoot__t6CGcObj1Z6CFrame(ROOT*) __asm__("MakeRoot__t6CGcObj1Z6CFrame");
RF CSidebag_CloneTo(FIELD*,LO*) __asm__("CSidebag_CloneTo");
RR CFrame_CloneTo(ROOT*,LO*) __asm__("CFrame_CloneTo");
void*AllocCopyFromWorldHeap(REQUEST*,void*) __asm__("AllocCopyFromWorldHeap");
void ClearSle(SLE*) __asm__("ClearSle");
void ClearSl(SL*) __asm__("ClearSl");
void AppendSlEntry(SL*,PXR*) __asm__("AppendSlEntry");
SL*GetSwProxyRefBucket(SW*,int) __asm__("GetSwProxyRefBucket");
void CopyAb__FPvT0Ui(void*,const void*,uint) __asm__("CopyAb__FPvT0Ui");

struct RF{FIELD*p;RF(){}RF(const RF&r):p(r.p){if(p)++p->refs;}~RF(){FIELD*f=p;if(f&&!--f->refs){BINDING*b=f->binding;if(b&&!--b->refs){func_00257250(&b->next,2);Decref__4CRef(b->cref);Unlink__t6CGcObj1Z5CVarb(b);FreeSlotheapSlot(D_004520D0,b);}Unlink__t6CGcObj1Z8CSidebag(f);FreeSlotheapSlot(D_00452148,f);}}};
struct RR{ROOT*p;RR(){}RR(const RR&r):p(r.p){if(p)++p->refs;}~RR(){ROOT*r=p;if(r&&!--r->refs)func_00257078(r,3);}};
typedef char FieldRefSize[sizeof(RF)==4?1:-1];
typedef char RootRefSize[sizeof(RR)==4?1:-1];
#if defined(SLY2_EE_LAYOUT)
/* LO's flag bits as the original copies them: int / uint bit-fields in the 64-bit unit at +0x28 (lo.h declares them
   long / ulong, and g++ folds a copy between same-position long fields into and/or masks where the original extracts
   and reinserts each field; two rounds of casts did not change that). EE layout only: the #else copies lo.h's fields. */
struct LOFLAGS{int siblingPrev;char cpaloFindSwObjects;unsigned char visibilityMask;int proxyLoadKind:4,busyKind:4;uint isRemoved:1,isSkippedByClone:1,keepsOwnVisMask:1;} __attribute__((aligned(8)));
#define LOFLAGS_OF(p) ((LOFLAGS*)((char*)(p)+0x28))
#endif
static inline PXR*CopyPxr(int bytes,REQUEST*r,PXR*n){r->bytes=bytes;r->alignment=16;return(PXR*)AllocCopyFromWorldHeap(r,n);}
/* LO's tagged update word (lo.h inlineFlags / ploupd): the flags inline when bit 0 is set, else LOUPD::grf. Get reads
   the flags; Set stores v (bit 0 cleared) and keeps the target's bit 0. The word is read pointer-wide (uptr). */
static inline int GetTaggedFlags(const LO*p){
#ifdef SLY2_EE_ASM
    register uptr v __asm__("$4")=(uptr)p->ploupd;
#else
    uptr v=(uptr)p->ploupd;
#endif
    if(!(v&1))v=((LOUPD*)v)->grf;return v;}
static inline void SetTaggedFlags(LO*p,int v){v&=~1;uptr d=(uptr)p->ploupd;if(!(d&1))((LOUPD*)d)->grf=v|(((LOUPD*)d)->grf&1);else p->inlineFlags=v|(d&1);}
/* Where LO's own data ends (0x50 on the EE, sizeof(LO)): the class body copied past it starts there. On the Itanium ABI a
   derived class may start in LO's tail padding, so this is the end of the last member, not sizeof. */
#define LO_DATA_END (SLY2_OFFSETOF(LO,unknown44)+(int)sizeof(((LO*)0)->unknown44))
// LO's Clone (vtable slot 41): copies the Splice frame and sidebag references, scale, flag bits and tag word from the
// source, clones the proxy nodes during proxy instantiation and copies the class body past the LO header (size from
// the class info).
// Also known as: CloneLo (upstream readable / Sly 1 name; CloneLo__FP2LOT0).
extern "C" void CloneLo(LO*object,const LO*source){
    CGc_UpdateRecyclable(g_gc);
    if(source->scriptVariables.m_p){
        {RF r=CSidebag_CloneTo((FIELD*)source->scriptVariables.m_p,object);AssignSidebagRef((FIELD**)&object->scriptVariables.m_p,r.p);}
        (void)MakeRoot__t6CGcObj1Z8CSidebag((FIELD*)object->scriptVariables.m_p);
    }
    if(source->scriptFrame.m_p){
        {RR r=CFrame_CloneTo((ROOT*)source->scriptFrame.m_p,object);FramePtr_Assign((ROOT**)&object->scriptFrame.m_p,r.p);}
        (void)MakeRoot__t6CGcObj1Z6CFrame((ROOT*)object->scriptFrame.m_p);
    }
    object->busyRadius=source->busyRadius;
#if defined(SLY2_EE_LAYOUT)
    LOFLAGS_OF(object)->busyKind=LOFLAGS_OF(source)->busyKind;
    LOFLAGS_OF(object)->isSkippedByClone=LOFLAGS_OF(source)->isSkippedByClone;
    LOFLAGS_OF(object)->proxyLoadKind=LOFLAGS_OF(source)->proxyLoadKind;
    LOFLAGS_OF(object)->keepsOwnVisMask=LOFLAGS_OF(source)->keepsOwnVisMask;
#else
    object->busyKind=source->busyKind;
    object->isSkippedByClone=source->isSkippedByClone;
    object->proxyLoadKind=source->proxyLoadKind;
    object->keepsOwnVisMask=source->keepsOwnVisMask;
#endif
    SetTaggedFlags(object,GetTaggedFlags(source));
    object->firstRunCallback=source->firstRunCallback;
    char*dst;const char*src;
    if(g_sw.isrcLoading!=-2){src=(const char*)source+LO_DATA_END;dst=(char*)object+LO_DATA_END;object->proxyRecord=source->proxyRecord;}
    else{
        src=(const char*)source+LO_DATA_END;dst=(char*)object+LO_DATA_END;REQUEST request;
        for(PXR*n=source->proxyRecord;n&&n->object==source;n=n->parent){
            PXR*copy=CopyPxr(sizeof(PXR),&request,n);
            copy->object=object;
            ClearSle(&copy->oidHashLink);
            ClearSle(&copy->siblingLink);
            ClearSl(&copy->children);
            AppendSlEntry(&copy->parent->children,copy);
            SL*list=GetSwProxyRefBucket(&g_sw,copy->proxyOid);
            AppendSlEntry(list,copy);
        }
    }
    CopyAb__FPvT0Ui(dst,src,object->GetClassInfo()->instanceSize-LO_DATA_END);
}
} // namespace M_CloneLo
#undef FIELD_OFFSET
#undef LO_DATA_END
#undef LOFLAGS_OF

namespace M_CloneLoHierarchy {
// vtable slot 40: clones the hierarchy by forwarding to Clone (slot 41).
extern "C" void CloneLoHierarchy(LO *object, LO *a) { object->Clone(a); }
} // namespace M_CloneLoHierarchy

namespace M_FFindLoParent {
// True when other is the object itself or one of its ancestors.
// Also known as: FLoInParentChain (upstream readable / Sly 1 name; FLoInParentChain__FP2LOT0).
extern "C" int IsLoSelfOrDescendantOf(LO *a, LO *b)
{
    while (a) {
        if (a == b) break;
        a = a->parent;
    }
    return a != 0;
}
} // namespace M_FFindLoParent

namespace M_func_001A6B30 {
extern XF D_002C44F0 __asm__("D_002C44F0");   /* a static identity transform */
void func_002333B4(void *p, int c, int n) __asm__("memset");
// LO's local transform (vtable slot 32): writes the identity into *matrix when given and returns a static identity
// transform.
extern "C" XF *LO_GetLocalTransform(LO *unused, MATRIX4 *pm)
{
    if (pm) {
        func_002333B4(pm, 0, 0x40);
        pm->mat[0][0] = pm->mat[1][1] = pm->mat[2][2] = pm->mat[3][3] = 1.0f;
    }
    return &D_002C44F0;
}
} // namespace M_func_001A6B30

// vtable slot 38: moves the object onto another object's world transform (warps and relays use it to place objects).
// Also known as: TeleportLo (upstream readable / Sly 1 name; TeleportLo__FP2LOT0).
extern "C" void MoveLoToObject(LO *plo, LO *ploOther)
{
    SetLoXfFromMat(plo, ploOther->GetWorldMatrix());
}

// vtable slot 39: moves the object to the transform held in an XF record.
extern "C" void SetLoTransformFromXf(LO *plo, XF *pxf)
{
    SetLoXfFromMat(plo, &pxf->worldMatrix);
}

// Moves the object to a world matrix: TranslateToPos (slot 29) to its position, then RotateToMat (slot 30).
// Also known as: PlaceAloAt (upstream readable / Sly 1 name; PlaceAloAt__FP3ALOP7MATRIX4).
extern "C" void SetLoXfFromMat(LO *plo, MATRIX4 *matrix)
{
    plo->TranslateToPos(((qword *)matrix)[3]);
    plo->RotateToMat((MATRIX3 *)matrix);   /* its rotation rows */
}

namespace M_func_001A6C48 {
void ResortSwBusyEntry(SW *g, void *p) __asm__("ResortSwBusyEntry");
// vtable slot 42: sets the busy distance (default 1e10) and re-sorts the root in the world's busy list.
extern "C" void SetLoBusyDistance(LO *object, float g)
{
    object->busyRadius = g;
    if (!object->parent && ((int)object->isInWorld & 1)) ResortSwBusyEntry(&g_sw, object->root);
}
} // namespace M_func_001A6C48

// Returns the busy kind (signed 4-bit field: 0 busy near the camera, 1 always, 2 never, 3 busy near the hero).
extern "C" int GetLoBusyKind(LO *plo)
{
    return plo->busyKind;
}

namespace M_SetLoBusyk {
void ResortSwBusyEntry(void *, void *) __asm__("ResortSwBusyEntry");
// Sets the busy kind (0 busy within the busy distance of the camera, 1 always busy, 2 never busy, 3 busy within the
// busy distance of the hero) and re-sorts the root in the world's busy list when it is in the world.
extern "C" void SetLoBusyKind(LO *object, int busyk)
{
    if (busyk != object->busyKind) {
        object->busyKind = busyk;
        if (!object->parent && (unsigned char)object->isInWorld != 0)
            ResortSwBusyEntry(&g_sw, object->root);
    }
}
} // namespace M_SetLoBusyk

namespace M_func_001A6D28 {
void ResortSwBusyEntry(void *, void *) __asm__("ResortSwBusyEntry");
// Sets the visibility mask: re-sorts the root, sets the mask through slot 46 and ORs it into every ancestor.
extern "C" void SetLoVisMask(LO *object, int b)
{
    LO *ancestor;
    if ((unsigned char)object->root->topObject->isInWorld)
        ResortSwBusyEntry(&g_sw, object->root);
    object->SetVisMask(b);
    for (ancestor = object->parent; ancestor; ancestor = ancestor->parent)
        ancestor->visibilityMask |= b;
}
} // namespace M_func_001A6D28

// LO's SetVisMask (vtable slot 46): stores the visibility mask byte.
extern "C" void LO_SetVisMask(LO *plo, uchar grfvis)
{
    plo->visibilityMask = grfvis;
}

namespace M_SetSoVisMask {
void SetLoVisMask(LO *, int) __asm__("SetLoVisMask");
// SO's SetVisMask (vtable slot 46): stores the mask and propagates it to the children that do not keep their own.
extern "C" void SetSoVisMask(SO *object, int b)
{
    LO *q;
    object->visibilityMask = b;
    for (q = (LO *)object->children.head; q; q = (LO *)q->siblingLink.next) {
        if (!q->keepsOwnVisMask)
            SetLoVisMask(q, b);
    }
}
} // namespace M_SetSoVisMask

namespace M_func_001A6E38 {
// Sets or clears the "keeps its own visibility mask" bit (a parent's SetVisMask then skips it).
extern "C" void SetLoOwnVisMask(LO *object, int f) { object->keepsOwnVisMask = f; }
} // namespace M_func_001A6E38

namespace M_func_001A6E70 {
// True when the object keeps its own visibility mask.
extern "C" int HasLoOwnVisMask(LO *object) { return (int)object->keepsOwnVisMask & 1; }
} // namespace M_func_001A6E70

namespace M_func_001A6E80 {
extern "C" { static inline void SetFlag(LO *p, unsigned int fl)
{
    uptr v = (uptr)p->ploupd;
    if ((v ^ 1) & 1) p->ploupd->grf |= fl;
    else p->inlineFlags = v | fl;
} }
extern "C" { static inline void ClearFlag(LO *p, unsigned int fl)
{
    uptr v = (uptr)p->ploupd;
    if ((v ^ 1) & 1) p->ploupd->grf &= ~fl;
    else p->inlineFlags = v & ~fl;
} }
// vtable slot 21: sets or clears flag 8 of the flag word (meaning not known). (inferred)
extern "C" void SetLoFlag8(LO *object, int f)
{
    if (f) SetFlag(object, 8);
    else ClearFlag(object, 8);
}
} // namespace M_func_001A6E80

// LO's default for vtable slot 43: returns 0 (the slot's meaning is not known).
extern "C" int LO_Slot43Default()
{
    return 0;
}

// LO's default for vtable slot 44: no-op (meaning not known).
/* default (empty) virtual */
extern "C" void LO_Slot44Default()
{
}

// LO's default for vtable slot 45: no-op (meaning not known).
/* default (empty) virtual */
extern "C" void LO_Slot45Default()
{
}

namespace M_ConvertAloPos {
void InvertMatrix4Rigid(MATRIX4Q *pm, MATRIX4Q *pn) SLY2_SRET_LABEL("InvertMatrix4Rigid", "InvertMatrix4RigidInto");
// Converts a point from one object's space to another's (through world space; a null object means world space).
// Also known as: ConvertPos (upstream readable / Sly 1 name; ConvertPos__FP2LOT0G9VU_VECTOR).
extern "C" qword ConvertPosBetweenLos(struct LO *a, struct LO *b, qword v)
{
    if (a) {
        MATRIX4Q *pm = (MATRIX4Q *)a->GetWorldMatrix();
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddaz.xyz $ACC, %4, %1z\n\tvmaddw.xyz %0, %5, $vf0w"
                : "=j"(v) : "j"(v), "j"(pm->r[0].q), "j"(pm->r[1].q), "j"(pm->r[2].q), "j"(pm->r[3].q));
    }
    if (b) {
        MATRIX4Q m;
        InvertMatrix4Rigid(&m, (MATRIX4Q *)b->GetWorldMatrix());
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddaz.xyz $ACC, %4, %1z\n\tvmaddw.xyz %0, %5, $vf0w"
                : "=j"(v) : "j"(v), "j"(m.r[0].q), "j"(m.r[1].q), "j"(m.r[2].q), "j"(m.r[3].q));
    }
    return v;
}
} // namespace M_ConvertAloPos

namespace M_ConvertAloVec {
MATRIX3Q *GetLoWorldMatrix(void *p) __asm__("GetLoWorldMatrix");
void InvertMatrix3Cofactor(MATRIX3Q *pm, MATRIX3Q *pn) SLY2_SRET_LABEL("InvertMatrix3Cofactor", "InvertMatrix3CofactorInto");
// Converts a direction vector from one object's space to another's (rotation only).
// Also known as: ConvertVec (upstream readable / Sly 1 name; ConvertVec__FP2LOT0G9VU_VECTOR).
extern "C" qword ConvertVecBetweenLos(void *a, void *b, qword v)
{
    if (a) {
        MATRIX3Q *pm = GetLoWorldMatrix(a);
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
                : "=j"(v) : "j"(v), "j"(pm->r[0].q), "j"(pm->r[1].q), "j"(pm->r[2].q));
    }
    if (b) {
        MATRIX3Q m;
        InvertMatrix3Cofactor(&m, GetLoWorldMatrix(b));
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
                : "=j"(v) : "j"(v), "j"(m.r[0].q), "j"(m.r[1].q), "j"(m.r[2].q));
    }
    return v;
}
} // namespace M_ConvertAloVec

namespace M_ConvertAloMat {
extern MATRIX3Q *GetLoWorldMatrix(void *p) __asm__("GetLoWorldMatrix");
extern MATRIX3Q InvertMatrix3Cofactor(const MATRIX3Q *b) __asm__("InvertMatrix3Cofactor");
static inline MATRIX3Q Mul(const MATRIX3Q &a, const MATRIX3Q &b) return r
{
    qword a0 = a.r[0].q, a1 = a.r[1].q, a2 = a.r[2].q;
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
        : "=j"(r.r[0].q) : "j"(a0), "j"(a1), "j"(a2), "j"(b.r[0].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
        : "=j"(r.r[1].q) : "j"(a0), "j"(a1), "j"(a2), "j"(b.r[1].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
        : "=j"(r.r[2].q) : "j"(a0), "j"(a1), "j"(a2), "j"(b.r[2].q));
}
// Converts a 3x3 matrix from one object's space to another's.
extern "C" MATRIX3Q ConvertMatBetweenLos(void *a, void *b, const MATRIX3Q *m)
{
    MATRIX3Q result;
    if (a) result = Mul(*GetLoWorldMatrix(a), *m);
    else result = *m;
    if (b) {
        result = Mul(InvertMatrix3Cofactor(GetLoWorldMatrix(b)), result);
    }
    return result;
}
} // namespace M_ConvertAloMat

namespace M_ConvertAloMat4 {
typedef struct { qword q[4]; } M4;
void InvertMatrix4Rigid(M4 *out, void *in) SLY2_SRET_LABEL("InvertMatrix4Rigid", "InvertMatrix4RigidInto");
void __ml__FRC7MATRIX4T0(M4 *out, void *a, M4 *b) SLY2_SRET_LABEL("__ml__FRC7MATRIX4T0", "MulMatrix4Into");
// Converts a 4x4 matrix from one object's space to another's.
extern "C" M4 ConvertMat4BetweenLos(LO *o1, LO *o2, M4 *m)
{
    M4 t;
    M4 r;
    M4 inv;
    if (o1) {
        __ml__FRC7MATRIX4T0(&r, o1->GetWorldMatrix(), m);
        t = r;
    } else {
        t = *m;
    }
    if (o2) {
        M4 *pinv = &inv;
        M4 *pr = &r;
        InvertMatrix4Rigid(pinv, o2->GetWorldMatrix());
        __ml__FRC7MATRIX4T0(pr, pinv, &t);
        t = r;
    }
    return t;
}
} // namespace M_ConvertAloMat4

namespace M_func_001A72D0 {
void InvertMatrix4Rigid(MATRIX4Q *pm) __asm__("InvertMatrix4Rigid");
// Converts a point through an explicit matrix, or through its inverse when inverse is set. (inferred)
extern "C" qword ConvertMatPos(MATRIX4Q *pm, int f, qword v)
{
    if (pm) {
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddaz.xyz $ACC, %4, %1z\n\tvmaddw.xyz %0, %5, $vf0w"
                : "=j"(v) : "j"(v), "j"(pm->r[0].q), "j"(pm->r[1].q), "j"(pm->r[2].q), "j"(pm->r[3].q));
    }
    if (f) {
        MATRIX4Q m;
        InvertMatrix4Rigid(&m);
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddaz.xyz $ACC, %4, %1z\n\tvmaddw.xyz %0, %5, $vf0w"
                : "=j"(v) : "j"(v), "j"(m.r[0].q), "j"(m.r[1].q), "j"(m.r[2].q), "j"(m.r[3].q));
    }
    return v;
}
} // namespace M_func_001A72D0

namespace M_func_001A7348 {
void InvertMatrix3Cofactor(MATRIX3Q *pm) __asm__("InvertMatrix3Cofactor");
// Converts a direction vector through an explicit 3x3 matrix or its inverse. (inferred)
extern "C" qword ConvertMatVec(MATRIX3Q *pm, int f, qword v)
{
    if (pm) {
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
                : "=j"(v) : "j"(v), "j"(pm->r[0].q), "j"(pm->r[1].q), "j"(pm->r[2].q));
    }
    if (f) {
        MATRIX3Q m;
        InvertMatrix3Cofactor(&m);
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
                : "=j"(v) : "j"(v), "j"(m.r[0].q), "j"(m.r[1].q), "j"(m.r[2].q));
    }
    return v;
}
} // namespace M_func_001A7348

// 0x1A73B0..0x1A73BC: 2 dead-code scraps (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0xe0");
SLY2_SCRAP("addiu $sp, $sp, 0x100");

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
extern const float D_002A6A40 = 3.40282347e+38f;

#include "sly2/port.h"
#if !defined(SLY2_EE_LAYOUT)
/* Native adapters for SLY2_SRET_LABEL (sly2/port.h; docs/guides/portability.md "Struct results and arm64"). The EE
   passes a struct result's address as a hidden first argument, so the declarations elsewhere that spell this file's
   functions with the other form (a by-value result for an explicit out-pointer, or the reverse) call the same code
   there; natively they bind to these entries, which have their spelling. Plain records with the definitions' sizes. */
namespace SRET_lo {
struct M3R { qword r[3]; };
struct M4R { qword r[4]; };
extern "C" M4R ConvertMat4BetweenLos_value(LO *, LO *, M4R *) __asm__("ConvertMat4BetweenLos");
extern "C" M4R *ConvertMat4BetweenLosInto(M4R *out, LO *from, LO *to, M4R *matrix)
{ *out = ConvertMat4BetweenLos_value(from, to, matrix); return out; }
extern "C" M3R ConvertMatBetweenLos_value(LO *, LO *, const M3R *) __asm__("ConvertMatBetweenLos");
extern "C" M3R *ConvertMatBetweenLosInto(M3R *out, LO *from, LO *to, const M3R *matrix)
{ *out = ConvertMatBetweenLos_value(from, to, matrix); return out; }
} // namespace SRET_lo
#endif
