// basic.cpp -- BASIC, the root of the object classes: class test (IsBasicDerivedFrom, GetBasicCid), the sidebag and its
// option setters (EnsureLoSidebag*), Splice events and the deferred Splice callbacks, LO subscriptions and messages.
// TU 0x132B00..0x1336F0 (28 functions, address order). The sidebag pool's GC members (PushFront 0x255C00, MakeRoot
// 0x258178) are CGcObj<CSidebag>'s (sly2/gc.h, emitted by splice/gc.cpp); the EMITTER class descriptor getter
// 0x258F30 is emitter.cpp's (sly2/emitter.h).
// Entry points: IsBasicDerivedFrom (class tests everywhere), HandleBasicSpliceEvent (vtable slot 3),
// SendLoMessage / SubscribeLoObject, ScheduleSwSpliceCallback, the EnsureLoSidebag* option setters.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_basic
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_basic
#include "sly2/slotheap.h"
#include "sly2/cref.h"
#include "sly2/clock.h"
#include "sly2/sw.h"
// 0x133598 StaticInit_basic and GlobalConstructors_basic: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* BASIC's member bodies are C functions over BASICV (an empty subclass of sly2/basic.h's BASIC): defining BASIC's
   members here would make this BASIC's key TU. CIDV is a signed class id (basic.h's CID has no enumerators, so it is
   unsigned and the derivation test would compare unsigned); the C++ definitions that name them carry their mangled
   names as asm labels. */
struct REFNODE;
struct CFrame;
/* A sidebag (pool 4): count, then the GC link word (index/page bit-fields, bit 15 a GC mark). */
struct SIDEBAG { int cref; int grfGc; };
   /* sly2/basic.h's BASIC; the C-named member bodies below take it */
struct LO : BASIC { };
/* A subscription: the subscriber and its count. */
struct REFNODE { LO *plo; int cref; REFNODE *pnext; };
enum CIDV { CIDV_Nil = -1 };
enum OPTID { OPTID_Nil = -1 };
struct CLQ;
struct LM;

/* A Splice value as the C callers pass it by hand (tag + payload; out-of-line constructors and Decref). */
/* A Splice pair node (pool 0) and the counted pointer to one. */
struct CPAIR;
struct SPAIR { CPAIR *p; };
struct CPAIR { int cref; int x4; CRefData ref; SPAIR sp; };

/* A deferred Splice callback (SW::slEvent list +0x39F0, SW::slotheapEvent heap +0x39FC, ids from SW::lastEventId). */
struct DLV { struct SWCB *head; struct SWCB *tail; int ibDle; };   /* SW::slEvent as the callback list */
struct SWCB { float t; BASIC *pbasic; CPAIR *pair; int id; SWCB *pnext; };
typedef char assert_SWCB_size[sizeof(SWCB) == sizeof(SWEVENT) ? 1 : -1];   /* the record sly2/sw.h SWEVENT, with this file's pair and BASIC views */

extern "C" {
extern CLASSDESC D_002BAAE8[];   /* the class descriptor table (sly2/basic.h) */
extern int D_0027A1D8[];   /* s_aiEvtParm (class_registry_options.inc) */
extern int D_0027A108[];   /* s_aotypEvtParm, read from its second word */

SIDEBAG *PtNew__t6CGcObj1Z8CSidebag(void);
void AssignSidebagRef(SIDEBAG **pp, SIDEBAG *p);
void MakeRoot__t6CGcObj1Z8CSidebag(SIDEBAG *p);
void EnsureBasicOption__FP5BASICiP4CRef(CRefData *pref, LO *plo, int optid, CRefData *prefValue);
void Decref__4CRef(CRefData *pref);
void CRef_ctor_CLQ(CRefData *pref, CLQ *pclq);
void CRef_ctor_LM(CRefData *pref, LM *plm);
void CRef_ctor_BASIC(CRefData *pref, BASIC *pbasic);
void CRef_ctor(CRefData *pref, qword v);
CPAIR *PtNew__t6CGcObj1Z5CPair(void);
void Attach__t7CRefPtr1Z5CPairP5CPair(SPAIR *psp, CPAIR *pn);
void PairPtr_Assign(SPAIR *psp, CPAIR *pn) __asm__("__as__t7CRefPtr1Z5CPairP5CPair");
void PairPtr_Release(SPAIR *psp, int n) __asm__("_$_t7CRefPtr1Z5CPair");
CRefData *CRef_SetPair(CRefData *pref, CPAIR *pn);
void CRef_SetSymid(CRefData *pref, int n);
void CRef_SetBasic(CRefData *pref, BASIC *pbasic);
void Unlink__t6CGcObj1Z5CPair(void *pv);
void EvalExpression__FP5CPairP6CFrame(CRefData *pref, CPAIR *pair, CFrame *frame);
void CRefAssign(CRefData *pref, CRefData *prefFrom) __asm__("__as__4CRefRC4CRef");   /* CRef::operator= */
void InsertSlEntryAfter(DLV *, SWCB *, SWCB *);
void RemoveSlEntry(DLV *, SWCB *);
int FindBindingPtr__6CFrameUii(CFrame *p, int k, int n);
CRefData *BuildSpliceListFromValues(CRefData *ret, int x, int *pn, void **y);

void EvalSpliceEventCallback(BASIC *p, CPAIR *pair, CFrame *frame, CRefData *pref);
SWCB *ScheduleSwSpliceCallback(SW *world, BASIC *p, CPAIR *pair, float t);
SPAIR *BuildSpliceEventCall(SPAIR *ret, BASIC *pbasic, int k, CRefData *pref);
}
int IsBasicDerivedFrom(BASIC *pbasic, CIDV cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void EnsureBasicSidebag(BASIC *p) __asm__("EnsureBasicSidebag__FP5BASIC");

// BASIC's default for vtable slot 1 (empty); inherited by about 190 classes. Its role is not known.
void BASIC::AddRef() {
}

// BASIC's default for vtable slot 2 (empty). Its role is not known.
extern "C" void BASIC_Slot2Default(void) {
}

// True when the object's class is cid or derives from it: the class ids of a subtree are contiguous, so this is a
// range test of the object's cid against [cid, cidMax of cid) in the class-descriptor table.
// Also known as: FIsBasicDerivedFrom (upstream readable / Sly 1 name).
int IsBasicDerivedFrom(BASIC *object, CIDV n) {
    CLASSINFO *pr = ((::BASIC *)object)->GetClassInfo();
    int x = pr->cid;
    int f = 0;
    if (x >= n) { CLASSDESC *pt = &D_002BAAE8[n]; f = x < (int)pt->lastDescendantCid; }   /* exclusive bound */
    return f;
}

// Gives the object a sidebag (its Splice variable bindings) when it has none.
void EnsureBasicSidebag(BASIC *object) {
    SIDEBAG **pf = (SIDEBAG **)&object->scriptVariables.m_p;
    if (*pf == 0) { AssignSidebagRef(pf, PtNew__t6CGcObj1Z8CSidebag()); MakeRoot__t6CGcObj1Z8CSidebag(*pf); }
}

/* EnsureLoSidebag* (Sly 1 names); the C++ original inlined the CRef constructor/setter, ~CRef -> Decref. */
// Sets a boolean sidebag option on the object unless it already has a value (EnsureBasicOption).
void EnsureLoSidebagBool(LO *plo, OPTID optid, int v)
{
    CRefData ref;
    CRefData refT;
    ref.m_tagk = 0x17;
    ref.u.n = v != 0;
    EnsureBasicOption__FP5BASICiP4CRef(&refT, plo, optid, &ref);
    Decref__4CRef(&refT);
    Decref__4CRef(&ref);
}

// Sets an integer sidebag option unless already set.
void EnsureLoSidebagInt(LO *plo, OPTID optid, int v)
{
    CRefData ref;
    CRefData refT;
    ref.m_tagk = 0x15;
    ref.u.n = v;
    EnsureBasicOption__FP5BASICiP4CRef(&refT, plo, optid, &ref);
    Decref__4CRef(&refT);
    Decref__4CRef(&ref);
}

// Sets a float sidebag option unless already set.
void EnsureLoSidebagFloat(LO *plo, OPTID optid, float g)
{
    CRefData ref;
    CRefData refT;
    ref.m_tagk = 0x16;
    ref.u.g = g;
    EnsureBasicOption__FP5BASICiP4CRef(&refT, plo, optid, &ref);
    Decref__4CRef(&refT);
    Decref__4CRef(&ref);
}

/* The payload constructors need a frame slot apart from the result (the m2c frames: value at 0, result at 0x10). */
struct CREFFR { CRefData ref; char pad8[8]; CRefData refT; };
// Sets a CLQ (four-float curve) sidebag option unless already set.
void EnsureLoSidebagClq(LO *plo, OPTID optid, CLQ *pclq)
{
    CREFFR fr;
    CRef_ctor_CLQ(&fr.ref, pclq);
    EnsureBasicOption__FP5BASICiP4CRef(&fr.refT, plo, optid, &fr.ref);
    Decref__4CRef(&fr.refT);
    Decref__4CRef(&fr.ref);
}

// Sets an LM (min/max range) sidebag option unless already set.
void EnsureLoSidebagLm(LO *plo, OPTID optid, LM *plm)
{
    CREFFR fr;
    CRef_ctor_LM(&fr.ref, plm);
    EnsureBasicOption__FP5BASICiP4CRef(&fr.refT, plo, optid, &fr.ref);
    Decref__4CRef(&fr.refT);
    Decref__4CRef(&fr.ref);
}

// Sets an object-id sidebag option (stored as an integer) unless already set.
void EnsureLoSidebagOid(LO *plo, OPTID optid, OID v)
{
    CRefData ref;
    CRefData refT;
    ref.m_tagk = 0x15;
    ref.u.n = v;
    EnsureBasicOption__FP5BASICiP4CRef(&refT, plo, optid, &ref);
    Decref__4CRef(&refT);
    Decref__4CRef(&ref);
}

// Sets a vector sidebag option unless already set (vector passed by value).
extern "C" void EnsureLoSidebagVector(LO *plo, int optid, qword v)
{
    CRefData ref;
    CRefData refT;
    CRef_ctor(&ref, v);
    EnsureBasicOption__FP5BASICiP4CRef(&refT, plo, optid, &ref);
    Decref__4CRef(&refT);
    Decref__4CRef(&ref);
}

// Sets an object-reference sidebag option to null unless already set.
extern "C" void EnsureLoSidebagBasic(LO *plo, int optid)
{
    CREFFR fr;
    CRef_ctor_BASIC(&fr.ref, 0);
    EnsureBasicOption__FP5BASICiP4CRef(&fr.refT, plo, optid, &fr.ref);
    Decref__4CRef(&fr.refT);
    Decref__4CRef(&fr.ref);
}

// Returns the object's class id (from its class info, vtable slot 6).
extern "C" int GetBasicCid(BASIC *object)
{
    return object->GetClassInfo()->cid;
}

// Getter of BASIC option 2 (object type 0x2000): returns the object itself, so Splice can ask an object for "self".
// (inferred)
extern "C" int GetBasicSelf(int n)
{
    return n;
}

// Builds the Splice call expression for an event: (handler-symbol self args...), where args is the event's
// argument list. Returns an owning pointer to the list.
SPAIR *BuildSpliceEventCall(SPAIR *ret, BASIC *a1, int a2, CRefData *px)
{
    SPAIR sp;
    CPAIR *p;
    Attach__t7CRefPtr1Z5CPairP5CPair(&sp, PtNew__t6CGcObj1Z5CPair());
    {
        CRefData *pref = &sp.p->ref;
        CRef_SetPair(pref, PtNew__t6CGcObj1Z5CPair());
    }
    p = (CPAIR *)sp.p->ref.u.pv;
    CRef_SetSymid(&p->ref, a2);
    {
        CPAIR *pn = PtNew__t6CGcObj1Z5CPair();
        PairPtr_Assign(&p->sp, pn);
    }
    p = p->sp.p;
    CRef_SetBasic(&p->ref, a1);
    if (px && px->m_tagk)
        PairPtr_Assign(&p->sp, (CPAIR *)px->u.pv);
    Attach__t7CRefPtr1Z5CPairP5CPair(ret, sp.p);
    {
        CPAIR *pt = sp.p;
        if (pt && --pt->cref == 0) {
            PairPtr_Release(&pt->sp, 2);
            Decref__4CRef(&pt->ref);
            Unlink__t6CGcObj1Z5CPair(pt);
            FreeSlotheapSlot(&CSlotPool<0x14, 0x2000>::s_pool.slotheap, pt);
        }
    }
    return ret;
}

// Evaluates a Splice event call in the object's frame and copies the result to *pref when given.
void EvalSpliceEventCallback(BASIC *p, CPAIR *pair, CFrame *frame, CRefData *pref)
{
    CRefData ref;
    EvalExpression__FP5CPairP6CFrame(&ref, pair, frame);
    if (pref != 0) {
        CRefAssign(pref, &ref);
    }
    Decref__4CRef(&ref);
}

// Schedules a Splice callback: allocates a node from the world's pool with an id (a new one when anonymous) and the
// time t, and inserts it into the world's time-sorted callback list. Returns the node.
SWCB *ScheduleSwSpliceCallback(SW *world, BASIC *a, CPAIR *pc, float t) {
    SWCB *pn;
    SWCB *pnPrev;
    SWCB *pnCur;
    pc->cref++;
    pn = (SWCB *)AllocZeroedFromSlotheap(&world->slotheapEvent);
    pn->pbasic = a;
    if (!a) pn->id = ++g_sw.lastEventId;
    else pn->id = -1;
    pn->pair = pc;
    pn->t = t;
    pn->pnext = 0;
    pnPrev = 0;
    for (pnCur = ((DLV *)&g_sw.slEvent)->head; pnCur; pnCur = pnCur->pnext) {
        if (pnCur->t > pn->t) break;
        pnPrev = pnCur;
    }
    InsertSlEntryAfter((DLV *)&g_sw.slEvent, pnPrev, pn);
    return pn;
}

// Cancels the scheduled Splice callback with the given id and frees its node.
extern "C" void CancelSwSpliceCallback(SW *world, int id) {
    SWCB *pn;
    if (id == -1) return;
    for (pn = ((DLV *)&world->slEvent)->head; pn; pn = pn->pnext) {
        if (pn->id == id) {
            CPAIR *pr;
            RemoveSlEntry((DLV *)&world->slEvent, pn);
            pr = pn->pair;
            if (--pr->cref == 0 && pr) {
                PairPtr_Release(&pr->sp, 2);
                Decref__4CRef(&pr->ref);
                Unlink__t6CGcObj1Z5CPair(pr);
                FreeSlotheapSlot(&CSlotPool<0x14, 0x2000>::s_pool.slotheap, pr);
            }
            FreeSlotheapSlot(&world->slotheapEvent, pn);
            return;
        }
    }
}

// BASIC's Splice event handler (vtable slot 3): when the object's frame binds a handler for the event, builds the
// arguments (types from the event parameter tables) and either schedules the call (posted) or evaluates it now.
// Sly 1: HandleLoSpliceEvent.
extern "C" void HandleBasicSpliceEvent(BASIC *object, int k, int f, int x, void **y, CRefData *z)
{
    if (object->scriptFrame.m_p && FindBindingPtr__6CFrameUii(object->scriptFrame.m_p, k, 1)) {
        CRefData ref;
        SPAIR sp;
        BuildSpliceListFromValues(&ref, x, &D_0027A108[1 + D_0027A1D8[k]], y);
        BuildSpliceEventCall(&sp, object, k, &ref);
        if (f)
            ScheduleSwSpliceCallback(&g_sw, object, sp.p, g_clock.time);
        else
            EvalSpliceEventCallback(object, sp.p, object->scriptFrame.m_p, z);
        {
            CPAIR *pt = sp.p;
            if (pt && --pt->cref == 0) {
                PairPtr_Release(&pt->sp, 2);
                Decref__4CRef(&pt->ref);
                Unlink__t6CGcObj1Z5CPair(pt);
                FreeSlotheapSlot(&CSlotPool<0x14, 0x2000>::s_pool.slotheap, pt);
            }
        }
        Decref__4CRef(&ref);
    }
}

// Sends the Splice trigger event (5) to the object (vtable slot 3, not posted). BASIC option 12. Buttons, exits, warps
// and camera volumes send event 5 when triggered and 6 when untriggered.
extern "C" void SendTriggerEvent(BASIC *p) { ((::BASIC *)p)->HandleSpliceEvent(5, 0, 0, 0, 0); }

// Sends the Splice untrigger event (6) to the object (vtable slot 3, not posted). BASIC option 13.
extern "C" void SendUntriggerEvent(BASIC *p) { ((::BASIC *)p)->HandleSpliceEvent(6, 0, 0, 0, 0); }

// Subscribes target to the object's messages: adds it to the object's subscriber list (counted, so repeated
// subscriptions need as many unsubscribes).
extern "C" void SubscribeLoObject(LO *object, LO *id) {
    REFNODE *pe;
    for (pe = object->firstSubscriber; pe; pe = pe->pnext) {
        if (pe->plo == id) { pe->cref++; return; }
    }
    {
        register REFNODE *peNew __asm__("$3") = (REFNODE *)AllocZeroedFromSlotheap(&g_sw.slotheapRefnode);
        __asm__("" : "+r"(peNew));
        peNew->plo = id;
        { register int one __asm__("$2") = 1; __asm__("" : "+r"(one)); peNew->cref = one; }
        peNew->pnext = object->firstSubscriber;
        object->firstSubscriber = peNew;
    }
}

// Removes one subscription of target from the object's subscriber list.
extern "C" void UnsubscribeLoObject(LO *object, LO *id) {
    REFNODE **ppe;
    REFNODE *pe;
    for (ppe = &object->firstSubscriber; (pe = *ppe) != 0; ppe = &pe->pnext) { if (pe->plo == id) { if (--pe->cref <= 0) { *ppe = pe->pnext; FreeSlotheapSlot(&g_sw.slotheapRefnode, pe); } break; } }
}

// Frees the object's whole subscriber list.
extern "C" void UnsubscribeLoAll(LO *object) {
    REFNODE *pe = object->firstSubscriber;
    while (pe) { REFNODE *pNext = pe->pnext; FreeSlotheapSlot(&g_sw.slotheapRefnode, pe); pe = pNext; }
    object->firstSubscriber = 0;
}

// BASIC's SendMessage (vtable slot 4): the object handles the message itself (slot 5), then every subscriber does.
extern "C" void SendLoMessage(LO *object, int a, void *b) {
    REFNODE *pn;
    ((::BASIC *)object)->HandleMessage((MSGID)a, b);
    for (pn = object->firstSubscriber; pn; ) {
        LO *po = pn->plo;
        pn = pn->pnext;
        ((::BASIC *)po)->HandleMessage((MSGID)a, b);
    }
}

// BASIC's HandleMessage (vtable slot 5): ignores the message; derived classes override it and call it as the base.
extern "C" void HandleBasicMessage(BASIC *object, int msg, void *pv) {
}
