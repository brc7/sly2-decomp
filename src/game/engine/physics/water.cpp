// water.cpp -- WATER (an SO): a body of water that pushes, drags and rights the objects floating in it.
// Role: keeps the list of objects inside the water (nodes at +0x150, each mirrored into the object's external-
// acceleration (XA) list at +0x138 so the solver asks the water for forces every step), emits the splash event
// when something falls in fast, measures how much of an object is submerged, evaluates the current at a point and
// applies per-object buoyancy, current drag, righting torque and wake (AddSoWaterAcceleration).
// Entry points: InitWater, UpdateWater (slot 16), AddWaterExternalAccelerations (slot 71), HandleWaterMessage,
// GetWaterSubmergedFraction, CalculateWaterCurrent; AddSoXa / RemoveSoXa / ApplySoExternalAccelerations are the
// SO-side XA list used by solve.cpp. VU0 macro-mode and EE asm; UpdateWater, GetWaterSubmergedFraction and
// AddSoWaterAcceleration are hand-tuned (register pins, allocation-only operands, a memory clobber).
// TU 0x228B48..0x229EF0 (22 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_water
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_water
#include "sly2/port.h"   /* sptr */
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/clock.h"
#include "sly2/classes.h"
#include "sly2/sw.h"
#include "sly2/emitter.h"   /* EXPL, EXPLSO */

// 0x229D98 StaticInit_water and GlobalConstructors_water: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

typedef int qwordV __attribute__((mode(TI)));
typedef unsigned long ulong;
union V { qwordV q; float f[4]; int n[4]; };
typedef struct { qwordV r[3]; } M3;
struct EVENT;
/* The message payload of messages 4 / 5 (entered / left the water): the water and the object. */
struct PAIR { WATER *owner; SO *object; };
/* The splash event: emitter.h's EXPLSO (kind = its grf flags, strength = its scale, normal = its vector, position =
   its origin) as UpdateWater's asm-addressed stack frame holds it. */
struct EVENT { int kind, pad4; float strength; int padC; qwordV normal; V position; char pad30[0x10]; };
typedef char assert_EVENT_size[sizeof(EVENT) == sizeof(EXPLSO) ? 1 : -1];
/* The water's own records are so.h's: its objects are XA records (source the water, other the object) on
   WATER::pxaFirst and on each object's SO::pxa list; the object's in-water marks are bits 31 / 32 of SO::soFlags
   (wasInWater / isInWater, tested and set by mask on the whole unit); the overlap candidates come from the water's
   physics tree (SOTREE::pxplistFirst, one XPLIST per other object); the colliders are its SOPARTs. A STEP is probed
   from its pivot (STEP::vecPivotOffset.z) and, standing on something, no lower than STEP::vecUnknown230.z; a PO's
   wake sound is PO::wakeSound. */
#define SO_WAS_IN_WATER 0x80000000L    /* SO::wasInWater */
#define SO_IS_IN_WATER 0x100000000L    /* SO::isInWater */
struct LSG { V first; qwordV pos; qwordV pad20; qwordV normal; float paramStart; float paramEnd; char pad48[0x18]; };
typedef char assert_LSG_size[sizeof(LSG) == 0x60 ? 1 : -1];
struct REQ { int size, alignment; };
struct MARK {};
/* CalculateWaterCurrent's matrix (constructed zeroed or not). */
extern "C" void *func_002333B4(void *, int, int) __asm__("memset");
enum Init { Uninitialized, Zero };
// 4x4 matrix, zeroed when constructed with Zero.
struct M4 { V r[4]; M4(Init mode) { if (mode == Zero) func_002333B4(this, 0, 0x40); } };
/* AddSoWaterAcceleration's types. */
struct M4B { M3 basis; qwordV position; };
struct COEF { float f[3]; };
/* sly2/basic.h's CLQ (Sly 1 util.h), same layout; basic.h cannot be included here (its qword is unsigned). */
/* The water current's spring gains (linear, angular): CLQ triples, defined here (only water reads them). */
CLQ D_002C4470 = {{{0.0f, 3.0f, 0.0f, 0.0f}}};
CLQ D_002C4480 = {{{0.0f, 0.75f, 0.0f, 0.0f}}};
union PHASE { V vectors[3]; M3 basis; };
union AXIS { COEF coefficients; V vector; };
struct WORK7F8 { V point, normal, velocity; PHASE phase; AXIS axis; V angularNormal, angularTangent, scalar; float angle; };
typedef char assert_WORK7F8_size[sizeof(WORK7F8) == 0xB0 ? 1 : -1];
/* UpdateWater's and GetWaterSubmergedFraction's stack layouts (their asm addresses these slots). */
union SCRATCH { REQ request; PAIR pair; V velocity; };
struct WORK { MARK mark; SCRATCH scratch; V start, end; LSG intervals[16]; V origin; EVENT event; int count; XA **head; REQ *request; };
struct WORK468 { V a, b; MARK mark __attribute__((aligned(16))); REQ msg __attribute__((aligned(16))); LSG alsg[0x10]; };

extern qwordV D_002A2DA0;
extern "C" {
extern qwordV g_normalZ, D_002A2DD0;
extern M3 D_002A2D00;
void InitSo(SO *p);
void PostSoLoad(SO *p);
void UpdateSo(SO *);
void RemoveSoHierarchy(SO *p);
void HandleBasicMessage(SO *p, int k, SO *q);
void InitSoPhysics(SO *p);
void SetSoPartsBit48(SO *p, int a);
qwordV ConvertPosBetweenLos(int a, SO *p, qwordV q);
V *VECTOR_opAssign(V *pv, qwordV q);
void AccumulateXfChainVelocityStub(void *p, int a, M4 *pm1, M4 *pm2);
qwordV DecomposeRotationMatrixToVector(M4 *pm);
MARK *STACKSCOPE_ctor(MARK *);
void STACKSCOPE_ctor_1AE4A0(MARK *, int) __asm__("_$_10STACKSCOPE");
void *AllocFromStack(const REQ *);
int CanWaterInflictZap(void *, SOPART *);
int CollectSoParts(SO *, int (*)(void *, SOPART *), void *, SOPART **);
int GetSoTreeFlag2(SO *);
int IsXfRoleVelocity(int);
float GetWaterSubmergedFraction(WATER *, SO *, qwordV *, qwordV *);
XA *AllocSwXa(SW *);
int IsBasicDerivedFrom__FP5BASIC3CID(SO *, int);
int ClipEdgeToObjects(qwordV, qwordV, int, SOPART **, int, LSG *);
void *CreateSfxSound(int);
void SetSndSourceLo(void *, SO *);
void SetSndRange(void *, float, float);
void SetSndVolume(void *, float);
void UpdateSwXaList(WATER *, XA **);
qwordV GetLoWorldPos(void *p);
int AreFloatsNear__Ffff(float a, float b, float eps);
qwordV ApplyConstr(CONSTR *, XF *, qwordV);
void AddSoAcceleration(SO *, qwordV);
void CalculateWaterCurrent(WATER *, qwordV, qwordV *, qwordV *);
float SplitVectorAlongAxis(qwordV, qwordV, qwordV *, qwordV *);
void AccelSoTowardPosSpring(SO *, qwordV, const COEF *, qwordV, const COEF *);
void AccelSoTowardMatSpring(SO *, const M3 *, const COEF *, qwordV, const COEF *);
M4B *GetLoWorldMatrix(SO *);
void TiltMatUpright(const M3 *, M3 *, M3 *);
void DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(const M3 *, float *, qwordV *);
void AddSoAngularAcceleration(SO *, qwordV);
void WakeSoWater__FP2SOP5WATERfP6VECTORT3ff(SO *, WATER *, float, qwordV, qwordV, float, float);
void StopSnd(void *, float);
void RcPtr_Assign(void **, void *);
void RemoveSoXa(SO *p, XA *pxa);
void AddSoXa(SO *p, XA *pxa);
void FreeSwXaList(SW *pg, XA *pxa);
void UpdateSoTreeBounds(SO *p);
void WakeSo(SO *p, XA *pxa);
/* The same function called with the object only (AddSoXa: with pxa passed on, a1 is copied first; 8 of 11 off). */
void func_001F5078P(SO *p) __asm__("WakeSo");
int IsPosInSo(SO *p, qword pos);
}

// Slot 7 (Init): InitSo, then the water's defaults (spring 1, gravity-like -10 and the other tuning values).
extern "C" void InitWater(WATER *water)
{
    sptr flags;
    InitSo(water);
    flags = (sptr)water->ploupd;   /* LO's tagged word (lo.h) */
    water->hasWakeSound = 1;
    water->unknown184 = -10;
    water->zapKind = 0x2000;
    if (!(flags & 1)) ((LOUPD *)flags)->grf |= 2;
    else water->unknown40 = flags | 2;
}

// Slot 64: resets the SO physics defaults (InitSoPhysics), sets bit 48 on every part (SetSoPartsBit48) and gives
// the water friction and spring factors of 1. (inferred: physics init override)
// Also known as: PostLoadWater (upstream readable / Sly 1 name; PostLoadWater__FP3ALO).
extern "C" void InitWaterPhysics(WATER *water)
{
    InitSoPhysics(water);
    SetSoPartsBit48(water, 1);
    { SOPHYS *physics = water->physics;
    physics->waterDrag = 1.0f;
    physics->buoyancy = 1.0f; }
}

// Slot 15 (PostLoad): only the SO post-load.
void WATER::PostLoad()
{
    PostSoLoad(this);
}

// Converts a world position into the water's local space and evaluates the current there, returning the linear
// velocity in *ppos and the angular velocity in *prot (both world space).
extern "C" void CalculateWaterCurrent(WATER *water, qwordV pos, qwordV *velocityOut, qwordV *angularVelocityOut)
{
    V localPos;
    M4 localMatrix(Uninitialized);
    M3 *identity;
    M4 *localMatrixPtr;
    localPos.q = ConvertPosBetweenLos(0, water, pos);
    localPos.f[2] = 0.0f;
    identity = &D_002A2D00;
    localMatrixPtr = &localMatrix;
    VECTOR_opAssign(&localMatrix.r[3], localPos.q);
    localMatrixPtr->r[0].q = identity->r[0];
    localMatrixPtr->r[1].q = identity->r[1];
    localMatrixPtr->r[2].q = identity->r[2];
    localMatrix.r[2].n[3] = 0;
    localMatrix.r[1].n[3] = 0;
    localMatrix.r[0].n[3] = 0;
    localMatrixPtr->r[3].f[3] = 1.0f;
    M4 currentMatrix(Zero);
    {
        M4 *currentMatrixPtr = &currentMatrix;
        currentMatrix.r[3].f[0] = water->posCurrent.x;
        currentMatrix.r[3].f[1] = water->posCurrent.y;
        currentMatrix.r[3].f[2] = water->posCurrent.z;
        AccumulateXfChainVelocityStub(water->rootTransform, 0, localMatrixPtr, currentMatrixPtr);
        if (velocityOut) *velocityOut = currentMatrix.r[3].q;
        if (angularVelocityOut) *angularVelocityOut = DecomposeRotationMatrixToVector(currentMatrixPtr);
    }
}

// Walks an XA node list and syncs each node with its object's in-water flags: nodes whose object entered get added
// to the object's XA list, those that left are removed; nodes whose object is no longer inside are unlinked and
// returned to the world's XA pool. The pending flags are cleared. Sly 1 name.
// Also known as: SettleWaterNodes (upstream readable / Sly 1 name; SettleWaterNodes__FP3ALOPv).
extern "C" void UpdateSwXaList(WATER *unused, XA **link)
{
    XA *node = *link;
    XA *freeList = 0;
    while (node) {
        SO *object = node->other;
        XA *nextNode = node->nextInSource;
        long flags = object->soFlags;
        if (((int)(flags >> 31) & 1) != ((flags >> 32) & 1)) {   /* wasInWater != isInWater */
            if (flags & SO_WAS_IN_WATER)
                RemoveSoXa(object, node);
            else
                AddSoXa(object, node);
        }
        if (node->other->soFlags & SO_IS_IN_WATER) {
            link = &node->nextInSource;
        } else {
            *link = nextNode;
            node->nextInSource = freeList;
            freeList = node;
        }
        {
            long cleared = node->other->soFlags & ~SO_IS_IN_WATER;
            node->other->soFlags = cleared & ~SO_WAS_IN_WATER;
        }
        node = nextNode;
    }
    FreeSwXaList(&g_sw, freeList);
}

// Track collider membership and emit contact and impact events.
static inline qwordV Add(qwordV a,qwordV b){qwordV r;__asm__("vadd.xyzw %0,%1,%2":"=j"(r):"0"(a),"j"(b));return r;}
// Copy the request pointer through $3.
static inline REQ*Fields(REQ*p){register REQ*copy __asm__("$3");__asm__("daddu %0,%1,$zero":"=r"(copy):"r"(p));return copy;}
// Call the object's velocity virtual (vtable slot 33).
static inline qwordV Velocity(SO *o){return ((SO *)o)->GetVelocity();}
// Explode the splash effect (EXPL slot 51, ExplodeExplso) with the event e.
static inline void Impact(EXPL *o,EVENT *e){o->ExplodeExplso((EXPLSO *)e);}
// Call the object's signal virtual (vtable slot 4).
static inline void Signal(SO *o,int event,PAIR *pair){o->SendMessage((MSGID)event,pair);}
// Slot 16 (Update): UpdateSo, then rebuilds the set of objects overlapping the water, sends contact messages for
// newcomers and an impact (splash) event for objects that hit the surface fast, and syncs the XA nodes.
// Also known as: UpdateWater (upstream readable / Sly 1 name; UpdateWater__FP5WATER).
extern "C" void UpdateWater(WATER *water){
 WORK work;XA *node;SOPART **objects;XPLIST *link;
 UpdateSo(water);
 if(!water->numParts)return;
 work.request=&work.scratch.request;work.head=&water->pxaFirst;
 for(node=*work.head;node;node=node->nextInSource)node->other->soFlags|=SO_WAS_IN_WATER;
 STACKSCOPE_ctor(&work.mark);
 {register int size __asm__("$3")=water->numParts;register int alignment __asm__("$2")=16;register REQ*req __asm__("$4")=work.request;size*=4;work.scratch.request.size=size;Fields(req)->alignment=alignment;
 objects=(SOPART **)AllocFromStack(req);}
 work.count=CollectSoParts(water,CanWaterInflictZap,0,objects);
 for(link=water->physicsTree->pxplistFirst;link;link=link->pxplistNext){
  SO *object=link->psoOther;
  if(!object->physics||object->physics->buoyancy<1e-4f||GetSoTreeFlag2(object)||!IsXfRoleVelocity(object->positionXf->role))continue;
  if(GetWaterSubmergedFraction(water,object,0,0)==0.0f)continue;
  object->soFlags|=SO_IS_IN_WATER;
  if(object->soFlags&SO_WAS_IN_WATER)continue;
  {register SW*world __asm__("$4")=&g_sw;node=AllocSwXa(world);}
  node->source=water;node->other=object;node->nextInSource=water->pxaFirst;water->pxaFirst=node;
  if(!water->recipient)continue;
  work.scratch.velocity.q=Velocity(object);
  if(work.scratch.velocity.f[2]<-100.0f){
  {int i;for(i=0;i<object->numParts;i++){
   SOPART *collider=&object->parts[i];float dz;
   if(collider->unknownFlag10A||collider->isHidden)continue;
   work.origin.q=D_002A2DD0;
   work.start.q=VuSub((qwordV)collider->matqWorld.r[3].q,work.origin.q);
   work.end.q=VuSub((qwordV)collider->transform->worldMatq.r[3].q,work.origin.q);
   if(IsBasicDerivedFrom__FP5BASIC3CID(object,0x33))dz=((STEP *)object)->vecPivotOffset.z;
   else dz=object->physicsTree->vecBoundsMin.z-collider->transform->worldMatq.r[3].f[2];
   work.start.f[2]+=dz;work.end.f[2]+=dz;
   if(ClipEdgeToObjects(work.start.q,work.end.q,work.count,objects,16,work.intervals)>0){
    qwordV normal;
    float fall=StdMin(-work.scratch.velocity.f[2],600.0f)*(1.0f/600.0f);
    float radius=((object->physicsTree->vecBoundsMax.x-object->physicsTree->vecBoundsMin.x)+(object->physicsTree->vecBoundsMax.y-object->physicsTree->vecBoundsMin.y))*0.5f;
    void *effect=CreateSfxSound(0x13);
    SetSndSourceLo(effect,object);
    SetSndRange(effect,100.0f,1000.0f);
    SetSndVolume(effect,fall);
    func_002333B4(&work.event,0,sizeof(work.event));
    normal=g_normalZ;
    work.event.position.q=Add(work.intervals[0].first.q,work.origin.q);
    work.event.kind=14;work.event.strength=radius*fall;work.event.normal=normal;
    Impact(water->recipient,&work.event);
   }
  }
  }
  }
 }
 for(node=water->pxaFirst;node;node=node->nextInSource){
  SO *object=node->other;
  long flags=object->soFlags;
  if(((int)(flags>>31)&1)!=((flags>>32)&1)){
   work.scratch.pair.owner=water;work.scratch.pair.object=object;
   Signal(object,object->soFlags&SO_WAS_IN_WATER?5:4,(PAIR *)work.request);
  }
 }
 UpdateSwXaList(water,work.head);STACKSCOPE_ctor_1AE4A0(&work.mark,2);
}

// Slot 71: called by the solver for each object in the water; forwards to the object's slot 72
// (AddSoWaterAcceleration by default) with the water as argument. Sly 1 name.
// Also known as: NotifyWaterObject (upstream readable / Sly 1 name; NotifyWaterObject__FP3ALOP5WNODE).
extern "C" void AddWaterExternalAccelerations(WATER *water, XA *pxa)
{
    SO *object = pxa->other;
    object->AddWaterAcceleration(water);
}

// Slot 12 (RemoveHierarchy): SO base, then detaches every XA node from its object and frees the whole list.
extern "C" void RemoveWaterHierarchy(WATER *water)
{
    XA *node;
    RemoveSoHierarchy(water);
    for (node = water->pxaFirst; node; node = node->nextInSource) RemoveSoXa(node->other, node);
    FreeSwXaList(&g_sw, water->pxaFirst);
    water->pxaFirst = 0;
}

// Slot 5 (HandleMessage): on message 1 from another object, drops that object's XA node and sends it message 5
// (left the water); then the base message handler.
extern "C" void HandleWaterMessage(WATER *water, int message, SO *sender)
{
    if (message == 1 && sender != water) {
        PAIR pair;
        XA **link = &water->pxaFirst;
        XA *node = *link;
        if (node) {
            if (node->other == sender) {
                *link = node->nextInSource;
            } else {
                for (;;) {
                    XA *previous = *link;
                    node = previous->nextInSource;
                    link = &previous->nextInSource;
                    if (!node) break;
                    if (node->other == sender) {
                        previous->nextInSource = node->nextInSource;
                        break;
                    }
                }
            }
        }
        pair.owner = water;
        pair.object = node->other;
        if (node) {
            node->nextInSource = 0;
            RemoveSoXa(node->other, node);
            FreeSwXaList(&g_sw, node);
        }
        pair.object->SendMessage((MSGID)5, &pair);
    }
    HandleBasicMessage(water, message, sender);
}

// Query the vertical hit span and optional point and normal outputs.
static inline float Epsilon(){register float f __asm__("$f14");__asm__ __volatile__("li.s %0,%1":"=f"(f):"F"(1e-4f));return f;}
// y lane of q as raw int bits.
static inline int WordY(qwordV q){int r;__asm__("prot3w %0,%1":"=r"(r):"r"(q));return r;}
// z lane of q as raw int bits.
static inline int WordZ(qwordV q){int r;__asm__("pextuw %0,$0,%1":"=r"(r):"r"(q));return r;}
// Reinterpret int bits as a float.
static inline float FloatWord(int word){float f;__asm__("mtc1 %1,%0":"=f"(f):"r"(word));return f;}
// True when the x lanes of d and n are within e.
static inline int NearX(qwordV d,qwordV n,float e){return AreFloatsNear__Ffff(QwordX(d),QwordX(n),e);}
// True when the y lanes of d and n are within the epsilon.
static inline int NearY(qwordV d,qwordV n){int y=WordY(d);float e=Epsilon();return AreFloatsNear__Ffff(FloatWord(y),QwordY(n),e);}
// True when the z lanes of d and n are within the epsilon.
static inline int NearZ(qwordV d,qwordV n){int z=WordZ(d);float e=Epsilon();return AreFloatsNear__Ffff(FloatWord(z),QwordZ(n),e);}
// Returns the fraction (0..1) of an object's vertical extent that is under the water surface, found by casting a
// vertical line through the object's position against the water; optionally returns the surface point and normal.
// Objects derived from class 0x33 are probed at their own position. Sly 1 name: UGetWaterSubmerged.
// Also known as: GWaterDepth (upstream readable / Sly 1 name; GWaterDepth__FP3ALOT0P6VECTORT2).
extern "C" float GetWaterSubmergedFraction(WATER *water, SO *object, qwordV *surfacePoint, qwordV *surfaceNormal)
{
    WORK468 work;
    void **parts;
    int numParts;
    float fraction;
    work.a.q = work.b.q = GetLoWorldPos(object);
    if (IsBasicDerivedFrom__FP5BASIC3CID(object, 0x33)) {
        STEP *step = (STEP *)object;
        work.a.f[2] = work.a.f[2] + step->vecPivotOffset.z;
        if (step->floorContact)
            work.a.f[2] = work.a.f[2] > step->vecUnknown230.z ? work.a.f[2] : step->vecUnknown230.z;
    } else {
        work.a.f[2] = object->physicsTree->vecBoundsMin.z;
    }
    work.b.f[2] = object->physicsTree->vecBoundsMax.z;
    STACKSCOPE_ctor(&work.mark);
    {
        REQ *request = &work.msg;
        request->size = water->numParts * 4;
        request->alignment = 0x10;
        parts = (void **)AllocFromStack(request);
    }
    numParts = CollectSoParts(water, CanWaterInflictZap, 0, (SOPART **)parts);
    if (!ClipEdgeToObjects(work.a.q, work.b.q, numParts, (SOPART **)parts, 0x10, work.alsg)) {
        if (surfaceNormal)
            *surfaceNormal = g_normalZ;
        STACKSCOPE_ctor_1AE4A0(&work.mark, 2);
        return 0.0f;
    }
    if (surfacePoint)
        *surfacePoint = work.alsg[0].pos;
    qwordV normal = work.alsg[0].normal;
    if (surfaceNormal) {
        qwordV reference;
        qwordV *comparison=&D_002A2DD0;
        float epsilon=Epsilon();
        int isDefault;
        *surfaceNormal = normal;
        reference = *comparison;
        isDefault = NearX(reference,normal,epsilon) && NearY(reference,normal) && NearZ(reference,normal);
        if (isDefault)
            *surfaceNormal = g_normalZ;
    }
    fraction = work.alsg[0].paramEnd - work.alsg[0].paramStart;
    STACKSCOPE_ctor_1AE4A0(&work.mark, 2);
    return fraction;
}

// Slot 49: recomputes the water's tree bounds (UpdateSoTreeBounds). Sly 1 name.
extern "C" void UpdateWaterBounds(WATER *water)
{
    UpdateSoTreeBounds(water);
}

// Allocates a cleared XA node from the world's pool (Sly 1 PxaAllocSw).
extern "C" XA *AllocSwXa(SW *world)
{
    return (XA *)AllocZeroedFromSlotheap(&world->slotheapXa);
}

// Returns every node of an XA list to the world's pool. Sly 1 name.
extern "C" void FreeSwXaList(SW *world, XA *node)
{
    while (node) {
        XA *next = node->nextInSource;
        FreeSlotheapSlot(&world->slotheapXa, node);
        node = next;
    }
}

// Pushes an XA node on the object's external-acceleration list and wakes the object.
// Also known as: LinkObjNode (upstream readable / Sly 1 name; LinkObjNode__FP5WNODEi).
extern "C" void AddSoXa(SO *object, XA *node)
{
    node->nextInOther = object->pxa;
    object->pxa = node;
    func_001F5078P(object);
}

// Unlinks an XA node from the object's external-acceleration list and wakes the object.
extern "C" void RemoveSoXa(SO *object, XA *node)
{
    XA **link;
    for (link = &object->pxa; *link; link = &(*link)->nextInOther) {
        if (*link == node) { *link = node->nextInOther; node->nextInOther = 0; break; }
    }
    WakeSo(object, node);
}

// Asks each owner in the object's XA list for its accelerations (owner slot 71); called by the solver before
// integration. (inferred)
// Also known as: ApplyAloWater (upstream readable / Sly 1 name; ApplyAloWater__FP3ALO).
extern "C" void ApplySoExternalAccelerations(SO *object)
{
    XA *node;
    for (node = object->pxa; node; node = node->nextInOther) {
        SO *owner = node->source;
        owner->AddExternalAccelerations(node);
    }
}

// Default slot 71 for an SO: no external accelerations.
extern "C" void AddSoExternalAccelerations()
{
}

// Resolve one contact between two physics objects: spring push, friction impulses (linear and
// angular), restoring torque, and the water-splash / emitter-clear effects.
static inline float Y(qwordV q){qwordV v;float f;__asm__("prot3w %0,%1":"=r"(v):"r"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(v));return f;}
// z lane of q as a float.
static inline float Z(qwordV q){qwordV v;float f;__asm__("pextuw %0,$zero,%1":"=r"(v):"r"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(v));return f;}
// True when a and b are equal within epsilon on x, y and z.
static inline bool Near(qwordV a,qwordV b,float epsilon){return AreFloatsNear__Ffff(QwordX(a),QwordX(b),epsilon)&&AreFloatsNear__Ffff(Y(a),Y(b),epsilon)&&AreFloatsNear__Ffff(Z(a),Z(b),epsilon);}
// Return v scaled by k.x.
static inline qwordV Scale(qwordV v,qwordV k){qwordV r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(v),"j"(k));return r;}
// Return motion + projection (normal component).
static inline qwordV AddNormal(qwordV motion,const V &projection){qwordV r;asm("vadd.xyzw %0,%1,%2":"=j"(r):"0"(motion),"j"(projection.q));return r;}
// Return motion + projection (tangent component).
static inline qwordV AddTangent(qwordV motion,const V &projection){qwordV r;asm("vadd.xyzw %0,%1,%2":"=j"(r):"0"(motion),"j"(projection.q));return r;}
// Broadcast sign into scalar and return angular scaled by it.
static inline qwordV NegativeAngular(qwordV angular,float sign,qwordV &scalar){qwordV factor;factor = VuFromFloat<qwordV>(sign);scalar=factor;return Scale(angular,factor);}
// The coefficient tables are indexed directly (not through a pointer) so each loop forms its own address.
#define GAIN2(G,amount,friction) {for(i=0;i<3;i++)coefficients->f[i]=G.coefficients[i]*amount*friction;}
#define GAIN1(G,friction) {for(i=0;i<3;i++)coefficients->f[i]=G.coefficients[i]*friction;}
// Set the xyz floats of v to zero.
static inline void ZeroXyz(V &v,float zero){v.f[0]=v.f[1]=v.f[2]=zero;}
// Stop and release the emitter's sound, if any.
static inline void Clear(void **sound){if(sound&&*sound){StopSnd(*sound,0.0f);RcPtr_Assign(sound,0);}}
// Default slot 72: the water's effect on one object (a). Buoyancy pushes along the object's direction in
// proportion to the submerged fraction; linear and angular velocities are pulled toward the water's current with
// spring gains scaled by friction; a righting torque turns the object upright; a wake is spawned while it floats
// on a near-horizontal surface, and class 0x32 objects lose their emitter sound otherwise. Sly 1 name.
// Also known as: ApplyWaterToAlo (upstream readable / Sly 1 name; ApplyWaterToAlo__FP3ALOT0).
extern "C" void AddSoWaterAcceleration(SO *object,WATER *water){
 WORK7F8 work;
 int i;
 float span=GetWaterSubmergedFraction(water,object,&work.point.q,&work.normal.q);
 float epsilon=0.0001f;
 if(span<epsilon)return;
 float spring=Near(D_002A2DD0,*(qwordV *)&object->physics->gravity,epsilon)?0.0f:object->physics->buoyancy*water->physics->buoyancy;
 float friction=object->physics->waterDrag*water->physics->waterDrag;
 float factor;
 if(spring!=0.0f){
  asm volatile("": : :"memory"); // the original re-reads a->physics here instead of reusing the pointer loaded above
  qwordV k=VuFromFloat<qwordV>(-spring*span);work.velocity.q=k;
  AddSoAcceleration(object,ApplyConstr(&object->physics->constrForce,object->rootTransform,Scale(*(qwordV *)&object->physics->gravity,k)));
 }
 if(friction!=0.0f){
  CalculateWaterCurrent(water,GetLoWorldPos(object),&work.velocity.q,&work.phase.vectors[0].q);
  qwordV linear=object->GetVelocity();qwordV relative=VuSub(work.velocity.q,linear);
  float dot=SplitVectorAlongAxis(relative,work.normal.q,&work.phase.vectors[1].q,&work.phase.vectors[2].q);
  if(span==1.0f||0.0f<dot)factor=1.0f;else factor=span*0.5f+0.5f;
  if(IsXfRoleVelocity(object->positionXf->role)){
   COEF *coefficients=&work.axis.coefficients;
   GAIN2(D_002C4470,factor,friction);
   AccelSoTowardPosSpring(object,D_002A2DD0,0,AddNormal(linear,work.phase.vectors[1]),coefficients);
   GAIN2(D_002C4470,span,friction);
   AccelSoTowardPosSpring(object,D_002A2DD0,0,AddTangent(linear,work.phase.vectors[2]),coefficients);
  }
  if(IsXfRoleVelocity(object->rotationXf->role)){
   qwordV angular=object->GetAngularVelocity();
   qwordV k;
   qwordV negative=NegativeAngular(angular,-1.0f,k);
   work.scalar.q=k;SplitVectorAlongAxis(negative,work.normal.q,&work.angularNormal.q,&work.angularTangent.q);
   COEF *coefficients=&work.axis.coefficients;
   GAIN2(D_002C4480,span,friction);
   AccelSoTowardMatSpring(object,&D_002A2D00,0,AddNormal(angular,work.angularNormal),coefficients);
   GAIN1(D_002C4480,friction);
   AccelSoTowardMatSpring(object,&D_002A2D00,0,AddTangent(angular,work.angularTangent),coefficients);
  }
 }
 else ZeroXyz(work.velocity,0.0f);
 if(spring!=0.0f){
  TiltMatUpright(&GetLoWorldMatrix(object)->basis,&work.phase.basis,0);
  DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(&work.phase.basis,&work.angle,&work.axis.vector.q);
  float gain=spring*3.0f;qwordV k=VuFromFloat<qwordV>(work.angle*gain*span);
  qwordV rotation=Scale(work.axis.vector.q,k);work.angularNormal.q=k;
  AddSoAngularAcceleration(object,rotation);
 }
 if(0.0f<span&&span<1.0f&&0.9f<work.normal.f[2]){
  float radius=((object->physicsTree->vecBoundsMax.x-object->physicsTree->vecBoundsMin.x)+(object->physicsTree->vecBoundsMax.y-object->physicsTree->vecBoundsMin.y))*0.25f;
  WakeSoWater__FP2SOP5WATERfP6VECTORT3ff(object,water,g_clock.deltaTime,work.velocity.q,work.point.q,0.1f,radius);
 }
 else if(IsBasicDerivedFrom__FP5BASIC3CID(object,0x32))Clear(&((PO *)object)->wakeSound);
}

// Returns 1 unless the contact's flag bit 50 is set: whether the water may inflict its zap through this contact
// (inferred from Sly 1 FInflictWaterZap).
extern "C" int CanWaterInflictZap(void *water, SOPART *collider)
{
    return !collider->isHidden;
}

// Slot 73: point-in-object test, forwarded to IsPosInSo with the same position argument. (inferred)
extern "C" int IsPosInWater(WATER *water, qword pos)
{
    return IsPosInSo(water, pos);
}
