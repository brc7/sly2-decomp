// barrier.cpp -- BARRIER, a solid object that filters what may pass through it: it names up to four objects and four
// classes, and either lets exactly those through (fThrough) or stops only those. When something touches it and the
// level script has bound the impact event (5), it reports each touching object to the script once per frame.
// Entry points: the BARRIER vtable (Init, PostBarrierLoad, ShouldBarrierIgnoreIntersection, NotifyBarrierImpact) and the
// Splice setters AddBarrierObject / AddBarrierClass (via RefThunk_* in class_registry.cpp).
// TU 0x1320C8..0x132570 (9 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_barrier
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_barrier
#include "sly2/slotheap.h"
#include "sly2/classes.h"
#include "sly2/sw.h"

// 0x132418 StaticInit_barrier and GlobalConstructors_barrier: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()


/* Contact graph of a solid object, as SendBarrierImpactMessages walks it (inferred: SO contact lists). */
struct CONTACTEND { int unknown0; int unknown4; SO *object; char pad[0x34]; };
struct CONTACT { CONTACTEND ends[2]; char pad[0xAC - 0x80]; CONTACT *next; };
struct CONTACTL { CONTACT *firstContact; CONTACTL *next; };

/* EnsureSwCallback takes an 8-byte struct by value in one register. The C original loaded it with ldl/ldr straight
   into a2; g++ 2.95 copies a by-value class argument through a stack temporary (pilot blocker B4), so the C++ call
   passes the 8 bytes as a long read through a packed (alignment 1) view, which gives the same ldl/ldr. */
struct CALLBACKK { int a; int b; };

extern CALLBACKK D_002A4790;

extern "C" {
void InitSo(BARRIER *pso);
void PostSoLoad(BARRIER *pso);
void RefreshSwObjectCollisionFilter(void *psw, BARRIER *pso);
int HasLoOid(void *plo, int oid);
int ShouldSoIgnoreIntersection(BARRIER *pso, void *psoOther);
void AdjustSoNewXp(BARRIER *pso);
void EnsureSwCallback(void *psw, void *pv, long k, int n);
}
int IsBasicDerivedFrom(struct BASIC *pbasic, CID cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
extern "C" void *FindBindingPtr__6CFrameUii(void *pframe, int a, int b);

// BARRIER vtable slot 7 (Init): initializes the SO part, then sets SO flag bit 34 (fTreeRoot in sly2/so.h), so the
// barrier is the root of its own physics tree.
// Also known as: InitBarrier (upstream readable / Sly 1 name; InitBarrier__FP7BARRIER).
void BARRIER::Init()
{
    BARRIER *barrier = this;
    InitSo(barrier);
    barrier->isTreeRoot = 1;
}

// BARRIER vtable slot 15 (PostLoad): finishes the SO load, then, if the barrier has physics, recomputes the world's
// collision-pair filter for it so the pass/stop rules of ShouldBarrierIgnoreIntersection take effect. (inferred)
// Also known as: PostBarrierLoad (upstream readable / Sly 1 name; PostBarrierLoad__FP7BARRIER).
extern "C" void PostBarrierLoad(BARRIER *barrier)
{
    PostSoLoad(barrier);
    if (barrier->physics)
        RefreshSwObjectCollisionFilter(&g_sw, barrier);
}

// BARRIER vtable slot 61 (ShouldIgnoreIntersection): decides how the barrier collides with another object. The other
// object "is named" when it matches one of the barrier's object names or derives from one of its classes. With
// letsNamedThrough set, named objects get 3 (pass) and others the SO default; otherwise named objects get 2 and others 1.
// Return codes follow SO::ShouldIgnoreIntersection (0/1 = collide, higher = ignore in some way). (inferred)
// Also known as: OverlapKindBarrier (upstream readable / Sly 1 name; OverlapKindBarrier__FP3ALOT0).
extern "C" int ShouldBarrierIgnoreIntersection(BARRIER *barrier, void *other)
{
    int i, j;
    int isNamed = 0;
    for (i = 0; i < barrier->numObjects; i++) {
        if (HasLoOid(other, barrier->objects[i])) isNamed = 1;
    }
    for (j = 0; j < barrier->numClasses; j++) {
        if (IsBasicDerivedFrom((struct BASIC *)other, barrier->classes[j])) isNamed = 1;
    }
    {
        int result;
        if (barrier->letsNamedThrough) {
            if (isNamed) result = 3;
            else result = ShouldSoIgnoreIntersection(barrier, other);
        } else result = isNamed ? 2 : 1;
        return result;
    }
}

// BARRIER vtable slot 55 (AdjustNewXp, called when a new contact point is created): runs the SO default, then, if the
// level script binds the barrier's impact event (5), schedules SendBarrierImpactMessages as a once-per-frame world
// callback (several contacts in one frame produce one report).
// Also known as: OnBarrierXpNew (upstream readable / Sly 1 name; OnBarrierXpNew__FP3ALO).
extern "C" void NotifyBarrierImpact(BARRIER *barrier)
{
    AdjustSoNewXp(barrier);
    if (barrier->scriptFrame.m_p && FindBindingPtr__6CFrameUii(barrier->scriptFrame.m_p, 5, 1))
        EnsureSwCallback(&g_sw, barrier, ((LONGU *)&D_002A4790)->l, 0);
}

// Which end (0 or 1) of a contact is the given object.
static inline int GetContactEndIndex(CONTACT *contact, SO *object) { return contact->ends[0].object == object ? 0 : 1; }

// Deferred world callback scheduled by NotifyBarrierImpact: collects up to 16 distinct objects touching the barrier
// and, for each, stores it in impactObject and fires the Splice impact event (5) through vtable slot 3
// (HandleSpliceEvent), so the script can ask which object hit the barrier.
// Also known as: TellBarrierContacts (upstream readable / Sly 1 name; TellBarrierContacts__FPv).
extern "C" void SendBarrierImpactMessages(BARRIER *barrier)
{
    SO *touching[16];
    int numTouching = 0;
    CONTACTL *contactList;
    int i;
    for (contactList = (CONTACTL *)barrier->physicsTree->pxplistFirst; contactList; contactList = contactList->next) {
        CONTACT *contact;
        for (contact = contactList->firstContact; contact; contact = contact->next) {
            SO *other = contact->ends[1 - GetContactEndIndex(contact, barrier)].object;
            for (i = 0; i < numTouching; i++)
                if (other == touching[i])
                    break;
            if (i >= numTouching) {
                touching[numTouching++] = other;
                if (numTouching >= sizeof(touching) / sizeof(touching[0]))
                    goto done;   /* a helper returning the count here changes the registers */
            }
        }
    }
done:
    for (i = 0; i < numTouching; i++) {
        barrier->impactObject = touching[i];
        barrier->HandleSpliceEvent(5, 0, 0, 0, 0);
    }
}

/* The count is signed in ShouldBarrierIgnoreIntersection's loops (slt/blez) and compared unsigned here (sltiu). */
// Splice setter: adds an object name to the barrier's list (at most four; extra names are ignored).
void AddBarrierObject(BARRIER *barrier, OID oid) __asm__("AddBarrierObject__FP7BARRIER3OID");
// Add an object name to the barrier's list (at most four).
void AddBarrierObject(BARRIER *barrier, OID oid)
{
    if ((unsigned int)barrier->numObjects < 4) barrier->objects[barrier->numObjects++] = oid;
}

// Splice setter: adds a class to the barrier's list (at most four; extra classes are ignored).
void AddBarrierClass(BARRIER *barrier, CID cid) __asm__("AddBarrierClass__FP7BARRIER3CID");
// Add a class to the barrier's list (at most four).
void AddBarrierClass(BARRIER *barrier, CID cid)
{
    if ((unsigned int)barrier->numClasses < 4) barrier->classes[barrier->numClasses++] = cid;
}

// ---- Pointer constants and records, by address name (docs/guides/tu-merging.md "Data"): words as the ELF has
// them, pointers through asm-labelled declarations. Non-virtual pointer-to-member constants are 8 bytes
// {delta 0, index -1, pfn}; the definitions sit in their own namespace so no declaration above has to agree.
namespace ANPTR_barrier {
void p_SendBarrierImpactMessages() __asm__("SendBarrierImpactMessages");

extern const unsigned int D_002A4790[2] __asm__("D_002A4790") = {0xFFFF0000, (unsigned int)p_SendBarrierImpactMessages};

} // namespace ANPTR_barrier
