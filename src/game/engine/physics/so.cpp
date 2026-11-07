// so.cpp -- SO (CID 0x30 : LO), the solid object: the base class of everything that has collision geometry and
// can take part in physics (props, platforms, doors, characters via PO/STEP, water).
// Role: init / load / clone; the transform hierarchy (XFs) and its velocities; parts (SOPART: a sphere or a BSP
// mesh with world-space geometry) grouped into a per-root contact tree (SOTREE) that the broad phase (bbmark.cpp)
// tracks; shared bounds records; physics records (SOPHYS: mass, inertia, gravity, constraints, region); the contact
// slots the solver calls (54-63); zap (damage) kinds and responses; fades; soft-mesh parts; splice-facing setters
// and the XF / part handle objects (CID 148/149/150) that scripts use.
// Entry points: InitSo, LoadSoFromBrx, PostSoLoad, UpdateSo (slot 16), UpdateSoXfWorld (slot 17), AddSoHierarchy /
// RemoveSoHierarchy, OnSoAdd / OnSoRemove, TranslateSoToPos / RotateSoToMat, Get/SetSoVelocity, WakeSo,
// UpdateSoTreeBounds, AccelSoToward*, DeliverZapToSo, FadeSoIn / FadeSoOut.
// TU 0x1F2108..0x1F8DB8 (174 functions, address order).
// The four Lilly ports (CloneSo = SO::Clone, GetSoAngularVelocity, SetSoIgnoreLocked, SetSoIceable) are at file
// scope (member definitions and C++ symbols) with their type copies merged; the other per-function files keep their
// views in namespaces named after the function (docs/guides/tu-merging.md "Representation decisions"). The former
// m2c bodies are written at file scope on sly2/so.h's SO/SOPHYS/SOTREE/SOPART/XF/CONSTR.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_so
// SLY2_ALIAS: _GLOBAL_$I$*=func_001F8DB8
#include "common.h"
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/cm.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/sw.h"
#include "sly2/scrap.h"
#include "sly2/vu0.h"

extern "C" void *func_002333B4(void *pv, int b, int cb) __asm__("memset");   /* memset */
extern qword D_002A2DC0;
/* 0x1F8C40 StaticInit_so (93 instructions) and its _GLOBAL_$I$ stub func_001F8DB8: g++'s initializer for the file-scope
   object below (built in a zeroed temporary and copied), then the six engine slot pools. D_003DD020 is the default
   query record that tu_1BC050 and guardbhv pass to FindShapeClosestPoint (role HYPOTHESIS; the name is a placeholder). */
struct CPQ
{
    qword axis;             /* D_002A2DC0 (the up axis) */
    int unknown10;          /* 1 */
    int pad[3];
    // Zero the whole CPQ.
    CPQ() { func_002333B4(this, 0, sizeof(CPQ)); }
};
// A zeroed CPQ with axis D_002A2DC0 and its unknown10 flag set.
static inline CPQ MakeCpq()
{
    CPQ cpq;
    cpq.axis = D_002A2DC0;
    cpq.unknown10 = 1;
    return cpq;
}
CPQ D_003DD020 = MakeCpq();
SLY2_INSTANTIATE_SLOT_POOLS()

// Types of the Lilly ports (merged copies)
struct EDGE
{
    ushort aipos[2];
    ushort aisurf[2];
    EDGE *pedgeNext;
    EDGE *pedgeOtherNext;
};

struct SURF
{
    VECTOR normal;
    float gDot;
    EDGE *pedge;
    EDGE *pedgeOther;
    uint grf;
    int ipos;
};

void CloneGeom(GEOM *pgeomSrc, MATRIX4 *pdmat, GEOM *pgeomDst);

void ClearDle(DLE *pdle);

struct BSP
{
    BSP *pbspNeg;
    BSP *pbspPos;
    SURF *psurf;
};

struct ACT
{
      int ack;
      SO *pso;
      XF *transform;
      int priority;
      ACT *pactNext;
      int role;
    // Uninitialised ACT.
    ACT() {}
    // ACT with the given ack value.
    ACT(int ack) : ack(ack) {}
};

extern "C" void AddXfAct(SO *pso, XF *pxf, ACT *pact);

ACT *PactNewClone(ACT *pactBase, SO *pso, XF *pxf) __asm__("NewActFromParams");

struct SUBGLOBX
{
      VECTOR posCenter;
      float sRadius;
      float gUnk10;
      uint grfUnk14;
};

struct XPLIST
{
      XP *pxpFirst;
      XPLIST *pxplistNext;
      XP **ppxpHead;
      SO *psoOther;
};

void *PvAllocSwCopyImpl(struct SWALLOC *pswalloc, void *pvBase) __asm__("AllocCopyFromWorldHeap");

struct SWALLOC
{
    int cb;
    int cbAlign;
};

void *PvAllocStackImpl(SWALLOC *pswalloc) __asm__("AllocFromStack");


void EnsureSoBounds(SO *pso) __asm__("EnsureSoBounds");

struct IKLIMB
{
      XF *pxfGoal;
      XF *pxfS;
      XF *pxfE;
      XF *pxfEnd;
      undefined1 unk_0x10[0xA0];
      qword qwEndLocal;
      float gLenUpper;
      float gLenLower;
      undefined4 unk_0xc8[6];
};

struct XFLINK
{
      XF *pxfFrom;
      XF *pxfTo;
      undefined4 unk_0x08[2];
};

struct CHAINLINK
{
      undefined1 unk_0x00[0x60];
      CHAIN *pchain;
      XF *pxf;
      undefined4 unk_0x68[2];
};

struct CHAIN
{
      int clink;
      CHAINLINK *alink;
      undefined4 unk_0x08[18];
};

struct VU_VECTOR4
{
    qword qw;
};

/* The bounds record of an SO (SO::psobounds): its owner and one float the owner keeps in step (SetSoBoundsScale). */
struct SOBOUNDS
{
    SO *pso;                /* +0x00 */
    int pad4[7];
    float scale;            /* +0x20 */
};
/* Objects only one function reads: CanSoTakeDamage's +0x44 word and ClearSoAnimIndex's +0xB4 word. */
struct SOWATCH { char pad0[0x44]; int unknown44; char pad48[0x6C]; int animIndex; };
/* The per-step record EnsureSoPhysRegionRecord finds (three floats at +4, +8, +0xC). */
struct STEPREC { int n0; float g4; float g8; float gC; };
/* A transform handle (a BASIC with a reference count at +0x10, the object at +0x20 and its XF at +0x24), freed into
   the slot heap D_003E15CC. */
struct XFH : public BASIC
{
    int cref;               /* +0x10 */
    int pad14[3];
    SO *pso;                /* +0x20 */
    XF *pxf;                /* +0x24 */
};
/* Hand-made virtual calls for the slots so.h does not declare with these signatures (g++ 2.95 {delta, pfn}). */
struct VTE { short d; short pad; void (*pfn)(); };
#define PVTE(p) (*(VTE **)((char *)(p) + 0xC))

extern SW D_003DD490;           /* g_sw */
int IsBasicDerivedFrom(BASIC *pbasic, CID cid);
void AppendDlEntry(DL *pdl, void *pv);
void RemoveDlEntry(DL *pdl, void *pv);
LO *FindSwObject(SW *psw, int grffso, OID oid, LO *ploContext);
LO *CreateLo(CID cid, SO *psoParent, OID oid, int n);
extern "C" {
SO *FindSoPhysRoot(SO *pso);
void RefreshSwObjectCollisionFilter(SW *psw, SO *pso);
void ScaleSoInertia(SO *pso, float r);
void PostLoLoadV() __asm__("PostLoLoad");      /* a0 passed through */
void UpdateSoXfWorldMatrices(SO *pso);
void BindGlobset(GLOBSET *pglobset);
void UpdateGeomWorld(MATRIX4 *pmat, GEOM *pgeomLocal, GEOM *pgeomWorld);
void InvalidateSoAaox(SO *pso);
void MarkSoTreeBoundsDirty(SO *pso);
void ClearSoTreeBoundsDirty(SO *pso);
void RefreshSoBoundsRecord(SO *pso);
void RenderSoGlobset(GLOBSET *pglobset, int n, float uAlpha1, float uAlpha2, SOBOUNDS *psobounds);
qword ConvertXfVec(XF *pxfFrom, XF *pxfTo, qword qw);
void UnsettleXfSlot(SO *pso, XF *pxf);
void AddObjectToBroadPhase(SW *psw, SO *pso);
void SetXfRole(SO *pso, XF *pxf, int n);
void GetSoConstraintAxis(SO *pso, XF *pxf, CONSTR *pconstr, CONSTR *pconstrSo);
void RebuildSoPhysHook(SO *pso);
void SetSoBit35Hierarchy(SO *pso, int n);
void FreeSoContacts(SO *pso);
int FindTargetRecordById(LO *plo);
void *func_001AE4F8(int cb) __asm__("__builtin_new");
void InitTargetRecord(void *ptargetrec, LO *plo);
STEPREC *EnsureSoPhysRegionRecord(STEP *pstep);
void FramePtr_Assign(void *ppframe, CFrame *pframe) __asm__("__as__t7CRefPtr1Z6CFrameP6CFrame");
void AssignSidebagRef(void *ppsidebag, CSidebag *psidebag);
void UnsubscribeLoAll(XFH *pxfh);
void FreeSlotheapSlot(SLOTHEAP *pslotheap, void *pv);
}

// 0x1F2108 InitSo
namespace M_InitSo { extern "C" {
struct F; struct P;
typedef unsigned long ulong;
struct F { ulong lo : 35; ulong bit35 : 1; ulong hi : 28; };
struct P { char pad[0x10]; struct P *parent; char pad1[0x12C - 0x14]; float alpha; char pad2[0x13C - 0x130]; float boundsScale; int unknown140; int unknown144; union { struct { signed char hitDamage0, hitDamage1; } c; struct F f; } u; };
extern float D_002C3B90;
void InitLo(struct P *);
void SetLoBusyKind(struct P *, int);
void InitDl__FP2DLi(void *, int);
// Slot 7 (Init): InitLo, not busy, empty child list, hit damages 10/20, alpha 1, the default bounds scale, and the
// bit-35 flag inherited from the parent.
void InitSo(struct P *so)
{
    InitLo(so);
    SetLoBusyKind(so, 0);
    InitDl__FP2DLi((char *)so + 0x64, 0x24);
    so->u.c.hitDamage0 = -10;
    so->u.c.hitDamage1 = -20;
    so->alpha = 1.0f;
    so->boundsScale = D_002C3B90;
    so->u.f.bit35 = (uchar)(so->parent ? so->parent->u.f.bit35 : 1);
    so->unknown140 = -1;
}
} } // namespace M_InitSo

// 0x1F21B8 SetSoParent
namespace M_SetSoParent {
struct VT; struct MAT; struct E; struct ALO; struct LO10; struct LO;
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct VT { short d; short pad; void *pfn; };
struct MAT { float m[4][4]; } __attribute__((aligned(16)));
struct E { char pad[0xA0]; struct E *parent; char pad2[0x100 - 0xA4]; };
struct LO;
struct ALO { char pad[0x40]; struct MAT mat; char pad2[0xA0 - 0x80]; struct ALO *parent; };
struct LO10 { char pad[0x50]; int numTransforms; struct E *transforms; char pad2[0x148 - 0x58]; ulong flags; };
struct LO {
    char pad[0xC]; struct VT *pvt; struct LO10 *parent; char pad2[0x28 - 0x14]; ulong bits0 : 60; ulong isInWorld : 1; ulong bits61 : 3;
    char pad3[0x50 - 0x30]; int numTransforms; struct E *transforms; char pad4[0x60 - 0x58]; struct ALO *rootTransform;
};
extern "C" {
void func_002333B4(void *, int, int) __asm__("memset");
void __ml__FRC7MATRIX4T0(struct MAT *, struct ALO *, struct MAT *);
void LO_SetParent(struct LO *, struct LO *, struct LO *, int);
void UpdateSoXfWorldMatrices(struct LO *);
void ReleaseSoBoundsHierarchy(struct LO *);
void EnsureSoBoundsHierarchy(struct LO *);
void SetSoBit35Hierarchy(struct LO *, int);
}

#define VCALL(po, slot, T) ((T)(po)->pvt[slot].pfn)((void *)((char *)(po) + (po)->pvt[slot].d)

// Slot 10 (SetParent): remembers the world matrix and velocities, re-parents the LO, re-points the XFs into the new
// parent, then rebinds contacts and bounds so the object keeps its world pose and motion.
extern "C" void SetSoParent(struct LO *plo, struct LO *pa, struct LO *pb, int flags)
{
    struct MAT mat;
    qword q1, q2;
    struct E *pv;
    int c, i;
    struct E *aeBase;
    if (flags & 3) {
        bool f = plo->isInWorld; if (f) {
            mat = plo->rootTransform->mat;
        } else {
            struct ALO *palo;
            func_002333B4(&mat, 0, 0x40);
            mat.m[0][0] = mat.m[1][1] = mat.m[2][2] = mat.m[3][3] = 1.0f;
            for (palo = plo->rootTransform; palo; palo = palo->parent) {
                struct MAT t;
                __ml__FRC7MATRIX4T0(&t, palo, &mat);
                mat = t;
            }
        }
    }
    if (flags & 4) q1 = VCALL(plo, 33, qword (*)(void *)));
    if (flags & 8) q2 = VCALL(plo, 34, qword (*)(void *)));
    if (plo->parent) {
        c = plo->parent->numTransforms;
        aeBase = plo->parent->transforms;
    } else {
        c = 1;
        aeBase = 0;
    }
    if (pb) pv = (struct E *)pb; else if (pa) pv = (struct E *)pa->rootTransform; else pv = 0;
    for (i = 0; i < plo->numTransforms; i++) {
        struct E *pe = &plo->transforms[i];
        int d = pe->parent - aeBase;
        if (d >= 0 && d < c) pe->parent = pv;
    }
    LO_SetParent(plo, pa, pb, flags);
    UpdateSoXfWorldMatrices(plo);
    if (flags & 1) VCALL(plo, 29, void (*)(void *, qword)), *(qword *)mat.m[3]);
    if (flags & 2) VCALL(plo, 30, void (*)(void *, struct MAT *)), &mat);
    if (flags & 4) VCALL(plo, 35, void (*)(void *, qword)), q1);
    if (flags & 8) VCALL(plo, 36, void (*)(void *, qword)), q2);
    ReleaseSoBoundsHierarchy(plo);
    EnsureSoBoundsHierarchy(plo);
    if (plo->parent && !(plo->parent->flags & (1L << 35))) SetSoBit35Hierarchy(plo, 0);
}
} // namespace M_SetSoParent

// 0x1F2468 ReleaseSoBoundsHierarchy
namespace M_ReleaseSoBoundsHierarchy { extern "C" {
struct R; struct L;
struct R { int pad[2]; int refCount; };
struct L { char pad[0x24]; struct L *next; char pad2[0x64 - 0x28]; struct L *firstChild; char pad3[0x70 - 0x68]; struct L *firstRemovedChild; char pad4[0x134 - 0x74]; struct R *bounds; };
struct Pool;
void FreeSlotheapSlot(struct Pool *, struct R *);
int IsBasicDerivedFrom__FP5BASIC3CID(struct L *, int);
// Drops this object's (and its SO children's) reference to the shared bounds record (+0x134, refcounted pool).
void ReleaseSoBoundsHierarchy(struct L *so)
{
    struct L *q;
    struct R *pr = so->bounds;
    if (pr) {
        if (--pr->refCount == 0) FreeSlotheapSlot(&(*(struct Pool *)&g_sw.slotheapSobounds), pr);
        so->bounds = 0;
    }
    for (q = so->firstChild; q != so->firstRemovedChild; q = q->next) {
        if (IsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) ReleaseSoBoundsHierarchy(q);
    }
}
} } // namespace M_ReleaseSoBoundsHierarchy

// 0x1F2500 EnsureSoBounds
namespace M_EnsureSoBounds { extern "C" {
struct Origin; struct Stats; struct Node;
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct Node;
struct Origin {char pad[0x70];qword pos;};
struct Stats {struct Node *owner;qword *origin;int count,unusedC;float low;int unused14,unused18;float high,scale;};
struct Node {
 char pad[0x10];struct Node *parent;
 char pad14[0x60-0x14];struct Origin *reference;
 char pad64[0xA0-0x64];float radius;
 char padA4[0xD0-0xA4];ulong flags;
 char padD8[0x134-0xD8];struct Stats *stats;
 int unused138;float scale;
 char pad140[8];ulong state;
};
struct Stats *AllocZeroedFromSlotheap(void *);
qword GetLoWorldPos(struct Node *);
// Distance between a and b (xyz) as a float.
static inline float Distance(qword a,qword b) {
 qword d,one;
 float result;
 __asm__("vsub.xyzw %0,%1,%2":"=j"(d):"j"(a),"j"(b));
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"0"(d));
 __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(d));
 __asm__("mtc1 %1,%0":"=f"(result):"r"(d));
 return result;
}
// Load the node's 64-bit state word.
static inline ulong LoadState(struct Node *p)
{
    register ulong value __asm__("$2");
    __asm__("ld %0,%1":"=r"(value):"m"(p->state));
    return value;
}
// Attaches the object to its root's bounds record (allocating one if needed) and grows the record's radii to cover
// the object's radius plus its distance from the root's origin.
void EnsureSoBounds(struct Node *so) {
 struct Node *top;
 struct Stats *attached;
 if (!(so->flags & (3UL<<32))) return;
 top=so;
 attached=so->stats;
 if(!attached) {
  do {
   register struct Node *next __asm__("$4")=top->parent;
   register int one __asm__("$3")=1;
   if(!next || ((int)((unsigned int)(int)LoadState(top)<<8)>>28)==one) {
     register char*pool __asm__("$4");
     struct Stats *s;
     __asm__("lui %0,0x3e":"=r"(pool));
     s=AllocZeroedFromSlotheap(pool+0x12D4);
     top->stats=s;
     so->stats=s;
     s->origin=&top->reference->pos;
     ++so->stats->count;
     so->stats->scale=so->scale;
     so->stats->owner=so;
     if(top==so)goto position;
     attached=so->stats;
     goto count;
   }
   top=next;
   attached=top->stats;
  } while(!attached);
  so->stats=attached;
 }
count:
 ++attached->count;
position:

 {
   qword position=GetLoWorldPos(so);
   float radius=so->radius+Distance(position,*so->stats->origin);
   if(so->flags&(1UL<<32)) {
     if(so->stats->low<radius) so->stats->low=radius;
   }
   if(so->flags&(1UL<<33)) {
     if(so->stats->high<radius) so->stats->high=radius;
   }
 }
}

#define OFF(T,F) ((unsigned long)&((T*)0)->F)
typedef char ancestor_layout[(sizeof(qword)==16 && __alignof__(qword)==16 && sizeof(void*)==4 && sizeof(float)==4 && sizeof(int)==4 && sizeof(unsigned)==4 && sizeof(ulong)==8 && sizeof(struct Origin)==0x80 && __alignof__(struct Origin)==16 && sizeof(struct Stats)==0x24 && __alignof__(struct Stats)==4 && sizeof(struct Node)==0x150 && __alignof__(struct Node)==8 && OFF(struct Origin,pos)==0x70 && OFF(struct Stats,owner)==0x0 && OFF(struct Stats,origin)==0x4 && OFF(struct Stats,count)==0x8 && OFF(struct Stats,low)==0x10 && OFF(struct Stats,high)==0x1c && OFF(struct Stats,scale)==0x20 && OFF(struct Node,parent)==0x10 && OFF(struct Node,reference)==0x60 && OFF(struct Node,radius)==0xa0 && OFF(struct Node,flags)==0xd0 && OFF(struct Node,stats)==0x134 && OFF(struct Node,scale)==0x13c && OFF(struct Node,state)==0x148)?1:-1];
} } // namespace M_EnsureSoBounds

// 0x1F2688 SetSoBoundsScale
/* Sets unknown13C, and the bounds record's copy when the record is this object's. */
// Stores the bounds scale (+0x13C, 400 by default) and mirrors it into the shared bounds record when this object
// owns it (inferred meaning).
extern "C" void SetSoBoundsScale(SO *so, float g)
{
    SOBOUNDS *psobounds;
    psobounds = so->psobounds;
    so->unknown13C = g;
    if ((psobounds != 0) && (psobounds->pso == so)) {
        psobounds->scale = g;
    }
}

// 0x1F26A8 EnsureSoBoundsHierarchy
namespace M_EnsureSoBoundsHierarchy { extern "C" {
struct L;
struct L { char pad[0x24]; struct L *next; char pad2[0x64 - 0x28]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; };
void EnsureSoBounds(struct L *);
int IsBasicDerivedFrom__FP5BASIC3CID(struct L *, int);
// EnsureSoBounds on this object and recursively on its SO children.
void EnsureSoBoundsHierarchy(struct L *so)
{
    struct L *q;
    EnsureSoBounds(so);
    for (q = so->first; q != so->end; q = q->next) {
        if (IsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) EnsureSoBoundsHierarchy(q);
    }
}
} } // namespace M_EnsureSoBoundsHierarchy

// 0x1F2718 SetSoUnknownBits20
namespace M_func_001F2718 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong a:20; int b:4; ulong c:40; };
// Sets the 4-bit field at bits 20-23 of the flag word +0x148 (so.h unknownBits148B; meaning unknown, a splice option).
void SetSoUnknownBits20(struct S *p, int n) { p->b = n; }
} } // namespace M_func_001F2718

// 0x1F2740 GetSoUnknownBits20
namespace M_func_001F2740 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong a:20; int b:4; ulong c:40; };
// Returns the 4-bit field at bits 20-23 of +0x148.
int GetSoUnknownBits20(struct S *p) { return p->b; }
} } // namespace M_func_001F2740

// 0x1F2758 GetSoIgnoreLocked
namespace M_GetSoIgnoreLocked { extern "C" {
struct S;
struct S { char pad[0xD0]; ulong globsetFlags; };
// Returns the ignore-locked flag (bit 34 of +0xD0).
int GetSoIgnoreLocked(struct S *p) { return (int)(p->globsetFlags >> 34) & 1; }
} } // namespace M_GetSoIgnoreLocked

// 0x1F2768 SetSoIgnoreLocked__FP2SOi
// SO+0xD0 bit 34 = GLOBSET+0x50 bit 34 (Lilly's port: globset.ignoreLocked).
// Sets the ignore-locked flag (bit 34 of +0xD0). Sly 1 name.
void SetSoIgnoreLocked(SO *pso, int f) { pso->model.ignoreLocked = f; }

// 0x1F2798 FindSoPhysRoot
namespace M_PsoFindPhysRoot { extern "C" {
struct Q; struct A;
typedef unsigned long ulong;
struct Q { char pad[0xA0]; int a0; char pad2[0x1C]; int c0; };
struct A { char pad[0x10]; struct A *parent; char pad2[0xD4]; struct Q *physics; char pad3[0x5C]; ulong flags; };
// Walks up the parent chain to the first SO whose physics is not fixed (SOPHYS modes != 3) or that is marked as a
// tree root (+0x148 bit 34); returns the last object visited. That object owns the contact tree.
struct A *FindSoPhysRoot(struct A *so)
{
    struct A *r = 0;
    while (so) {
        r = so;
        if (so->physics && (so->physics->c0 != 3 || so->physics->a0 != 3))
            break;
        if (so->flags & (1L << 34))
            break;
        so = so->parent;
    }
    return r;
}
} } // namespace M_PsoFindPhysRoot

// 0x1F27F0 EnsureSoTree
/* Makes sure the object has a tree record: its physics root's (created by SO slot 53 when missing). */
// Gives the object a contact-tree record (+0xEC): shares its physics root's, creating it (slot 53) and registering
// it with the broad phase when needed.
extern "C" void EnsureSoTree(SO *so)
{
    SO *psoRoot;
    if (so->physicsTree == 0) {
        psoRoot = FindSoPhysRoot(so);
        if (psoRoot->physicsTree == 0) {
            psoRoot->physicsTree = ((SOTREE *(*)(void *))PVTE(psoRoot)[53].pfn)((char *)psoRoot + PVTE(psoRoot)[53].d);
            RefreshSwObjectCollisionFilter(&D_003DD490, psoRoot);
        }
        so->physicsTree = psoRoot->physicsTree;
    }
}

// 0x1F2870 AddSoPartToTree
namespace M_AddSoPartToTree { extern "C" {
struct M; struct O; struct E; struct VTE; struct VT; struct Q; struct P;
typedef int qword __attribute__((mode(TI)));
struct M { qword a[4]; };
struct O { char pad[0x40]; struct M matWorld; char pad2[0xA4 - 0x80]; int stamp; };
struct E { struct O *transform; int pad[7]; struct M matWorld; char pad3[0xA8 - 0x60]; int geomStamp; int worldStamp; };
struct VTE { short d; short p; int (*pfn)(void *); };
struct VT { struct VTE a[63]; };
struct Q { char pad[0x34]; char parts[1]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0xEC - 0x10]; struct Q *tree; };
void EnsureSoTree(struct P *);
int IsEntryInDl__FP2DLPv(void *, void *);
void AppendDlEntry__FP2DLPv(void *, void *);
void InvalidateSoAaox(struct P *);
void UpdateSoPartGeom(struct E *);
void WakeSo(struct P *);
// Adds one part to the tree's part list: copies the XF matrix and stamp, marks the tree dirty, transforms the
// part's geometry to world space and wakes the tree if the object is not fixed.
void AddSoPartToTree(struct P *so, struct E *pe)
{
    struct O *po;
    EnsureSoTree(so);
    if (IsEntryInDl__FP2DLPv(so->tree->parts, pe)) return;
    AppendDlEntry__FP2DLPv(so->tree->parts, pe);
    po = pe->transform;
    pe->geomStamp = -1;
    pe->matWorld = po->matWorld;
    pe->worldStamp = po->stamp;
    InvalidateSoAaox(so);
    UpdateSoPartGeom(pe);
    if (!so->pvt->a[62].pfn((char *)so + so->pvt->a[62].d)) WakeSo(so);
}
} } // namespace M_AddSoPartToTree

// 0x1F2930 RemoveSoPartFromTree
namespace M_RemoveSoPartFromTree { extern "C" {
struct VTE; struct E; struct Q; struct P; struct Q2;
typedef unsigned long ulong;
struct VTE { short d; short p; int (*pfn)(void *); };
struct E { char pad[0x10]; ulong flags; char pad2[0xB0 - 0x18]; };
struct P;
struct Q { char pad[0x34]; char parts[0x2C]; struct P *rootSo; };
struct P { char pad[0xC]; struct VTE *pvt; char pad2[0xE0 - 0x10]; int numParts; struct E *parts; int physics; struct Q *tree; };
struct Q2 { char pad[0xEC]; struct Q *tree; };
struct G;
extern struct G D_003DD490;
int IsEntryInDl__FP2DLPv(void *, void *);
void RemoveDlEntry__FP2DLPv(void *, void *);
int IsDlEmpty__FP2DL(void *);
void RemoveSwPlacesOf(struct G *, int, struct E *);
void FreeSoContacts(struct P *);
void RemoveObjectFromBroadPhase(struct G *, struct Q *);
void WakeSo(struct P *);
// Removes a part from the tree's part list and frees its contacts; drops the tree when it becomes empty, or detaches
// the object when none of its own parts remain.
void RemoveSoPartFromTree(struct P *so, struct E *pe)
{
    int i;
    if (!so->tree) return;
    if (!IsEntryInDl__FP2DLPv(so->tree->parts, pe)) return;
    RemoveDlEntry__FP2DLPv(so->tree->parts, pe);
    if (pe->flags & (1L << 51)) RemoveSwPlacesOf(&D_003DD490, 0, pe);
    FreeSoContacts(so);
    if (IsDlEmpty__FP2DL(so->tree->parts)) {
        struct Q *pq = so->tree;
        ((struct Q2 *)pq->rootSo)->tree = 0;
        so->tree = 0;
        RemoveObjectFromBroadPhase(&D_003DD490, pq);
    } else if (so->tree->rootSo != so) {
        i = so->numParts;
        while (--i >= 0) {
            if (IsEntryInDl__FP2DLPv(so->tree->parts, (struct E *)(i * 0xB0 + (int)so->parts))) break;
        }
        if (i < 0) so->tree = 0;
    }
done:
    if (!so->pvt[62].pfn((char *)so + so->pvt[62].d)) WakeSo(so);
}
} } // namespace M_RemoveSoPartFromTree

// 0x1F2A60 ShowSoPart
namespace M_ShowSoPart {
struct A; struct B;
typedef unsigned long ulong;
struct A { char pad[0x28]; ulong lo : 60; ulong isInWorld : 1; ulong hi : 3; };
struct B { char pad[0x10]; ulong lo : 50; ulong isHidden : 1; ulong hi : 13; };
extern "C" void AddSoPartToTree(A *, B *);
// Un-hides a part (clears flag bit 50) and, when the object is in the world, adds it to the tree.
extern "C" void ShowSoPart(A *pa, B *pb)
{
    if (pb->isHidden) { pb->isHidden = 0;
    bool f = pa->isInWorld; if (f) AddSoPartToTree(pa, pb); }
}
} // namespace M_ShowSoPart

// 0x1F2AD0 HideSoPart
namespace M_HideSoPart { extern "C" {
struct T; struct S;
struct T { char pad[0x10]; ulong flags; };
struct S { char pad[0x28]; ulong flags; };
void RemoveSoPartFromTree(struct S *, struct T *);
// Hides a part (sets flag bit 50) and, when the object is in the world, removes it from the tree.
void HideSoPart(struct S *so, struct T *pt)
{
    if (!(pt->flags & (1UL << 50))) {
        pt->flags |= 1UL << 50;
        if ((int)(so->flags >> 60) & 1) RemoveSoPartFromTree(so, pt);
    }
}
} } // namespace M_HideSoPart

// 0x1F2B20 AddSoParts
namespace M_AddSoParts { extern "C" {
struct E; struct P;
typedef unsigned long ulong;
struct E { int pad[4]; ulong flags; char pad2[0xB0 - 0x18]; };
struct P { char pad[0xE0]; int numParts; struct E *parts; };
void AddSoPartToTree(struct P *, struct E *);
// Adds every visible part to the object's tree.
void AddSoParts(struct P *so)
{
    int i;
    for (i = 0; i < so->numParts; i++) {
        struct E *pe = &so->parts[i];
        if (!(pe->flags & (1L << 50))) AddSoPartToTree(so, pe);
    }
}
} } // namespace M_AddSoParts

// 0x1F2BA8 RemoveSoParts
namespace M_RemoveSoParts { extern "C" {
struct E; struct P;
typedef unsigned long ulong;
struct E { int pad[4]; ulong flags; char pad2[0xB0 - 0x18]; };
struct P { char pad[0xE0]; int numParts; struct E *parts; };
void RemoveSoPartFromTree(struct P *, struct E *);
// Removes every visible part from the object's tree.
void RemoveSoParts(struct P *so)
{
    int i;
    for (i = 0; i < so->numParts; i++) {
        struct E *pe = &so->parts[i];
        if (!(pe->flags & (1L << 50))) RemoveSoPartFromTree(so, pe);
    }
}
} } // namespace M_RemoveSoParts

// 0x1F2C30 AddSoHierarchy
namespace M_AddSoHierarchy { extern "C" {
struct VTE; struct VT; struct S8; struct E; struct L; struct H; struct G;
typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[12]; };
struct S8 { long l; } __attribute__((packed));
struct E { char pad[0xDC]; void *region; };
struct L { char pad[0xC]; struct VT *pvt; char pad1[0x24 - 0x10]; struct L *next; char pad1b[0x40 - 0x28]; int flagWord; char pad2[0x64 - 0x44]; struct L *firstChild; char pad3[0x70 - 0x68]; struct L *firstRemovedChild; char pad4[0xE8 - 0x74]; struct E *physics; };
struct H { char pad[0x2C]; int numZones; };
struct G { char pad[0x45CC]; struct H *zoneBsp; };
extern struct G D_003DD490;
extern const struct S8 D_002A8270;
void AddLoHierarchy(struct L *);
void UpdateSoXfWorldMatrices(struct L *);
void AddSoParts(struct L *);
qword GetLoWorldPos(struct L *);
void *FindPathzoneAtPos(qword, int);
void SetSoPhysRegion(struct L *, void *);
void PostSwCallback(struct G *, struct L *, long, int);
// Slot 11 (AddHierarchy): AddLoHierarchy, adds the parts, sets flags 0x14, registers the physics region (if the
// object has a region record) and recurses into the children.
void AddSoHierarchy(struct L *so)
{
    struct L *q;
    void *pv;
    int unused[4];
    int v;
    AddLoHierarchy(so);
    UpdateSoXfWorldMatrices(so);
    AddSoParts(so);
    v = so->flagWord;
    if ((v ^ 1) & 1) *(int *)v |= 0x14;
    else so->flagWord = v | 0x14;
    pv = so->physics ? so->physics->region : 0;
    if (pv) {
        if (D_003DD490.zoneBsp && D_003DD490.zoneBsp->numZones)
            SetSoPhysRegion(so, FindPathzoneAtPos(GetLoWorldPos(so), 1));
        else
            PostSwCallback(&D_003DD490, so, D_002A8270.l, 0);
    }
    for (q = so->firstChild; q != so->firstRemovedChild; q = q->next)
        q->pvt->a[11].pfn((char *)q + q->pvt->a[11].d);
}
} } // namespace M_AddSoHierarchy

// 0x1F2D48 RemoveSoHierarchy
namespace M_RemoveSoHierarchy { extern "C" {
struct DL; struct DLI; struct VTE; struct O; struct E; struct P; struct K;
typedef unsigned long ulong;
typedef struct DL { void *head; void *tail; int linkOffset; } DL;
typedef struct DLI { DL *m_pdl; void **m_ppv; int m_ibDle; struct DLI *m_pdliNext; } DLI;
extern DLI *D_002DD2FC;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *pvt; char pad2[0x28 - 0x10]; ulong flags; };
struct E { char pad[0xDC]; int region; };
struct P { char pad[0x64]; DL children; char pad2[0xE8 - 0x70]; struct E *physics; char pad3[0x12C - 0xEC]; float alpha; int alphaRate; };
struct K { long l; } __attribute__((packed));
struct G;
extern struct G D_003DD490;
extern const struct K D_002A8270;
void RemoveLoHierarchy(struct P *);
void RemoveSoParts(struct P *);
void ClearSwCallbacks(struct G *, int, struct P *, long, int);
void SetSoPhysRegion(struct P *, int);
// Start iterating display list pdl with iterator pdli and push it on the global iterator stack (D_002DD2FC).
static inline void InitDli(DLI *pdli, DL *pdl)
{
    pdli->m_pdl = pdl;
    pdli->m_ppv = (void **)pdl;
    pdli->m_ibDle = pdl->linkOffset;
    pdli->m_pdliNext = D_002DD2FC;
    D_002DD2FC = pdli;
}
// Return the current element of the iterator and advance it along the element's link field.
static inline void *PvNextDli(DLI *pdli)
{
    void *pv = *pdli->m_ppv;
    pdli->m_ppv = (void **)((char *)pv + pdli->m_ibDle);
    return pv;
}
// Slot 12 (RemoveHierarchy): RemoveLoHierarchy, removes the parts, resets the fade, unregisters the region callback
// and recurses into the children.
void RemoveSoHierarchy(struct P *so)
{
    DLI dli;
    struct O *po;
    int n;
    RemoveLoHierarchy(so);
    RemoveSoParts(so);
    so->alphaRate = 0;
    so->alpha = 1.0f;
    n = 0;
    if (so->physics) n = so->physics->region;
    if (n) {
        ClearSwCallbacks(&D_003DD490, 3, so, D_002A8270.l, 0);
        SetSoPhysRegion(so, 0);
    }
    InitDli(&dli, &so->children);
    while ((po = (__typeof__(po))PvNextDli(&dli)) != 0 && !(po->flags & (1L << 56)))
        po->pvt[12].pfn((char *)po + po->pvt[12].d);
    D_002DD2FC = dli.m_pdliNext;
}
} } // namespace M_RemoveSoHierarchy

// 0x1F2E78 UpdateSoLoroot
namespace M_UpdateSoLoroot { extern "C" {
struct VTE; struct O;
typedef unsigned long ulong;
struct VTE { short d; short pad; void (*pfn)(void *, void *, void *); };
struct O { char pad[0xC]; struct VTE *pvt; struct O *parent; char pad2[0x24 - 0x14]; struct O *next; ulong flags; char pad3[0x64 - 0x30]; struct O *firstChild; char pad4[0xE0 - 0x68]; int numParts; };
void UpdateLoLoroot(struct O *, struct O *, struct O *);
void RemoveSoParts(struct O *);
void AddSoParts(struct O *);
// Nearest ancestor of x (or x itself) with flag bit 56 set; null if none.
static inline struct O *Find(struct O *x)
{
    while (x && !(x->flags & (1L << 56))) x = x->parent;
    return x;
}
// Slot 13: UpdateLoroot recursively; when the object has parts and its root changed, re-adds them to the new tree.
void UpdateSoLoroot(struct O *so, struct O *pNew, struct O *pOld)
{
    struct O *po;
    int unused[4];
    UpdateLoLoroot(so, pNew, pOld);
    for (po = so->firstChild; po; po = po->next)
        po->pvt[13].pfn((char *)po + po->pvt[13].d, pNew, pOld);
    if (!so->numParts) return;
    for (po = so; ; po = po->parent) {
        if (po->flags & (1L << 56)) return;
        if (po == pNew) break;
    }
    {
    struct O *a = Find(pOld);
    struct O *b = Find(so->parent);
    if (!a && !b) {
        RemoveSoParts(so);
        AddSoParts(so);
    }
    }
}
} } // namespace M_UpdateSoLoroot

// 0x1F2FE0 SetSoGrf
namespace M_SetSoGrf { extern "C" {
struct VTE; struct VT; struct L;
struct VTE { short d; short p; void (*pfn)(void *, int); };
struct VT { struct VTE a[48]; };
struct L { char pad[0xC]; struct VT *pvt; char pad1[0x24 - 0x10]; struct L *next; char pad1b[0x40 - 0x28]; int flagWord; char pad2[0x64 - 0x44]; struct L *firstChild; char pad3[0x70 - 0x68]; struct L *firstRemovedChild; };
// Slot 47: sets flags in the LO flag word (+0x40) on this object and all children.
void SetSoGrf(struct L *so, int f)
{
    struct L *q;
    int unused[4];
    int v = so->flagWord;
    if ((v ^ 1) & 1) *(int *)v |= f;
    else so->flagWord = v | f;
    for (q = so->firstChild; q != so->firstRemovedChild; q = q->next)
        q->pvt->a[47].pfn((char *)q + q->pvt->a[47].d, f);
}
} } // namespace M_SetSoGrf

// 0x1F3078 ClearSoGrf
namespace M_ClearSoGrf { extern "C" {
struct VTE; struct VT; struct L;
struct VTE { short d; short p; void (*pfn)(void *, int); };
struct VT { struct VTE a[49]; };
struct L { char pad[0xC]; struct VT *pvt; char pad1[0x24 - 0x10]; struct L *next; char pad1b[0x40 - 0x28]; int flagWord; char pad2[0x64 - 0x44]; struct L *firstChild; char pad3[0x70 - 0x68]; struct L *firstRemovedChild; };
// Slot 48: clears flags in the LO flag word on this object and all children.
void ClearSoGrf(struct L *so, int f)
{
    struct L *q;
    int unused[4];
    int v = so->flagWord;
    if ((v ^ 1) & 1) *(int *)v &= ~f;
    else so->flagWord = v & ~f;
    for (q = so->firstChild; q != so->firstRemovedChild; q = q->next)
        q->pvt->a[48].pfn((char *)q + q->pvt->a[48].d, f);
}
} } // namespace M_ClearSoGrf

// 0x1F3118 SetSoBusyDistance
namespace M_func_001F3118 {
struct VTE; struct VT; struct L;
typedef unsigned long ulong;
struct VTE { short d; short p; void (*pfn)(void *, float); };
struct VT { VTE a[43]; };
struct L { char pad[0xC]; VT *pvt; struct L *parent; int root; char pad1[0x24 - 0x18]; struct L *next; ulong lo : 60; ulong isInWorld : 1; ulong hi : 3; float unknown30, runList, busyDistance; char pad2[0x64 - 0x3C]; struct L *firstChild; char pad3[0x140 - 0x68]; int unknown140; };
struct G;
extern "C" { extern G D_003DD490; }
extern "C" void RemoveLoFromRenderTree(L *);
extern "C" void AddLoToRenderTree(L *);
extern "C" void ResortSwBusyEntry(G *, int);
// Slot 42: stores the busy distance (+0x38), refreshes the object's registration, re-sorts its root in the world
// and pushes the smaller value to the children (inferred).
extern "C" void SetSoBusyDistance(L *so, float t)
{
    L *q;
    int unused[4];
    so->busyDistance = t;
    if (so->unknown140 != -1) {
        RemoveLoFromRenderTree(so);
        AddLoToRenderTree(so);
    }
    if (!so->parent) {
        bool f = so->isInWorld;
        if (f) ResortSwBusyEntry(&D_003DD490, so->root);
    }
    for (q = so->firstChild; q; q = q->next) {
        if (t < q->busyDistance) q->pvt->a[42].pfn((char *)q + q->pvt->a[42].d, t);
    }
}
} // namespace M_func_001F3118

// 0x1F31E0 LoadSoFromBrx
namespace M_LoadSoFromBrx {
struct GEO; struct TREE; struct M4; struct VE; union FLAGS; struct E; struct STATE; struct DATA; struct P; struct MSG;
typedef unsigned long ulong;
typedef unsigned int uint;
typedef int qword __attribute__((mode(TI)));
struct GEO {int count;qword *points;int planeCount;void *planes;int edgeCount;void *edges;};
struct TREE {int count,a,b;void *nodes;};
struct M4 {qword rows[4];};
struct E;
struct P;
struct VE {short delta,pad;void (*fn)();};
union FLAGS {ulong raw;struct {void *mesh;short state;unsigned short high;} fields;};
struct E {void *object;P *owner;float radius;int tag;FLAGS flags;int id,pinned;M4 matrix;char pad60[8];TREE tree;GEO local,world;int state,stamp;};
struct STATE {qword rows[3];char pad30[0xA0];float mass;char padD4[0xC];};
struct DATA {char pad0[0x38];void *guard;char pad3C[0x24];};
struct P {char pad0[0xC];VE *vt;char pad10[0x30];uint flags;char pad44[0x3C];DATA data;int count;E *entries;STATE *state;};
struct MSG {int bytes,alignment;};
extern "C" { extern char D_002C44F0[]; }
extern "C" {
void LoadSoXfsFromBrx(P *,void *);
int U32Read__18CBinaryInputStream(void *);
int S16Read__18CBinaryInputStream(void *);
int S32Read__18CBinaryInputStream(void *);
int S8Read__18CBinaryInputStream(void *);
float F32Read__18CBinaryInputStream(void *);
void ReadGeom__18CBinaryInputStreamP4GEOM(void *,GEO *);
void CBinaryInputStream_ReadBspc(void *,GEO *,TREE *);
void ReadMatrix__18CBinaryInputStreamP7MATRIX3(void *,STATE *);
void LoadGlobsetFromBrx__FP7GLOBSETP2SOP18CBinaryInputStream(DATA *,P *,void *);
void *AllocClearedFromWorldHeap(MSG *);
void CloneGeom__FP4GEOMP7MATRIX4T0(GEO *,const M4 *,GEO *);
void AddSoParts(P *);
void ScaleSoInertia(P *,float);
void LoadOptionsFromBrx__FPvP18CBinaryInputStream(P *,void *);
void EnsureSoBounds(P *);
void LoadSwObjectsFromBrx__FP2SOP18CBinaryInputStreamP14CLoadCollector(P *,void *,void *);
void SetLoVisMask(P *,int);
}
// Call p's virtual at vtable slot 63 and return its int result.
static inline int Extra(P *p){VE *v=&p->vt[63];return ((int (*)(P *))v->fn)((P *)((char *)p+v->delta));}
// Call p's virtual at vtable slot 51 with id and return the pointer it gives.
static inline void *Lookup(P *p,int id){VE *v=&p->vt[51];return ((void *(*)(P *,int))v->fn)((P *)((char *)p+v->delta),id);}
// Call p's virtual at vtable slot 64.
static inline void Post(P *p){VE *v=&p->vt[64];((void (*)(P *))v->fn)((P *)((char *)p+v->delta));}
// Set flag 8: on p->flags when bit 0 is set, otherwise on the word p->flags points to (as the original does).
static inline void SetActive(P *p){uint f=p->flags;if((f^1)&1)*(uint *)f|=8;else p->flags=f|8;}
// Slot 14 (LoadFromBrx): the LO base, the model and glob set, the part array (XF id, flags, radius, local
// geometry, BSP...), the physics block (matrix, mass), options and bounds.
// Layout parse-verified: a parser following it reads every byte of 4 levels.
extern "C" void LoadSoFromBrx(P *so,void *stream)
{
    int i=0;E *e;
    LoadSoXfsFromBrx(so,stream);
    int metadata=U32Read__18CBinaryInputStream(stream);
    LoadGlobsetFromBrx__FP7GLOBSETP2SOP18CBinaryInputStream(&so->data,so,stream);
    int extra=Extra(so);
    so->count=S16Read__18CBinaryInputStream(stream);
    MSG request;
    request.bytes=(so->count+extra)*sizeof(E);request.alignment=16;
    so->entries=(E *)AllocClearedFromWorldHeap(&request);
    for(;i<so->count;i++){
        e=&so->entries[i];
        GEO *local=&e->local;
        e->owner=so;
        e->object=Lookup(so,S16Read__18CBinaryInputStream(stream));
        e->tag=S32Read__18CBinaryInputStream(stream);
        int f=S8Read__18CBinaryInputStream(stream);
        e->flags.raw=(e->flags.raw&~(1UL<<48))|((ulong)(f&1)<<48);
        f=S8Read__18CBinaryInputStream(stream);
        e->flags.raw=(e->flags.raw&~(1UL<<49))|((ulong)(f&1)<<49);
        e->radius=F32Read__18CBinaryInputStream(stream);
        ReadGeom__18CBinaryInputStreamP4GEOM(stream,local);
        if(!e->object){e->object=D_002C44F0;e->world=e->local;}
        else CloneGeom__FP4GEOMP7MATRIX4T0(local,0,&e->world);
        CBinaryInputStream_ReadBspc(stream,&e->world,&e->tree);
        e->id=U32Read__18CBinaryInputStream(stream);
        e->flags.fields.state=S16Read__18CBinaryInputStream(stream);
        e->pinned=S16Read__18CBinaryInputStream(stream);
        f=S8Read__18CBinaryInputStream(stream);
        e->flags.raw=(e->flags.raw&~(1UL<<50))|((ulong)(f&1)<<50);
    }
    for(i=so->count;i<so->count+extra;i++){e=&so->entries[i];e->flags.fields.state=-10;}
    AddSoParts(so);
    if(so->count>0){
        request.bytes=sizeof(STATE);request.alignment=16;
        STATE *state=(STATE *)AllocClearedFromWorldHeap(&request);
        so->state=state;
        state->mass=F32Read__18CBinaryInputStream(stream);
        ReadMatrix__18CBinaryInputStreamP7MATRIX3(stream,state);
        ScaleSoInertia(so,1.0f);
        Post(so);
    }
    LoadOptionsFromBrx__FPvP18CBinaryInputStream(so,stream);
    if(so->data.guard)SetActive(so);
    EnsureSoBounds(so);
    LoadSwObjectsFromBrx__FP2SOP18CBinaryInputStreamP14CLoadCollector(so,stream,0);
    SetLoVisMask(so,metadata);
}
} // namespace M_LoadSoFromBrx

// 0x1F3590 InitSoPhysics
/* Default physics: gravity -1750 down z, both constraints of kind 3. */
// Slot 64: physics defaults: gravity (0, 0, -1750) and both position and rotation XFs fixed (mode 3).
extern "C" void InitSoPhysics(SO *so)
{
    SOPHYS *psophys;
    VECTOR *pdv;
    psophys = so->physics;
    pdv = &psophys->gravity;
    psophys->gravity.x = 0.0f;
    pdv->z = -1750.0f;
    pdv->y = 0.0f;
    so->physics->constrForce.kind = 3;
    so->physics->constrTorque.kind = 3;
}

// 0x1F35C8 SetSoMass
// Sets the mass, scaling the inertia by the same ratio (ScaleSoInertia).
extern "C" void SetSoMass(SO *so, float m)
{
    SOPHYS *psophys;
    float mOld;
    float r;
    psophys = so->physics;
    mOld = psophys->m;
    r = m / mOld;
    psophys->m = mOld * r;
    ScaleSoInertia(so, r);
}

// 0x1F35F8 ScaleSoInertia
namespace M_AdjustSoMomint { extern "C" {
struct T; struct X;
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
typedef struct { qword r[3]; } M3;
struct T { M3 m; qword v[3]; };
struct X { char pad[0xE8]; struct T *physics; };
void func_0023320C(void *d, const void *s, unsigned int cb) __asm__("memcpy");
void *func_002333B4(void *d, int c, unsigned int cb) __asm__("memset");
int InvertMatrix3(struct T *pt, qword *pv);
// Scales the inertia matrix by r and recomputes its inverse (zero when singular).
void ScaleSoInertia(struct X *so, float s)
{
    struct T *pt = so->physics;
    M3 m;
    U u;
    M3 r;
    qword uq;
    __asm__("qmtc2.ni %1, %0" : "=j"(uq) : "r"(s));
    u.q = uq;
    qword r0;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r0) : "j"(u.q), "j"(pt->m.r[0]));
    r.r[0] = r0;
    qword r1;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r1) : "j"(u.q), "j"(pt->m.r[1]));
    r.r[1] = r1;
    qword r2;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r2) : "j"(u.q), "j"(pt->m.r[2]));
    r.r[2] = r2;
    func_0023320C(&m, &r, sizeof(M3));
    pt->m = m;
    if (!InvertMatrix3(so->physics, so->physics->v))
        func_002333B4(so->physics->v, 0, 0x30);
}
} } // namespace M_AdjustSoMomint

// 0x1F36A8 PostSoLoad
// Slot 15 (PostLoad): PostLoLoad and the glob set's post-load.
extern "C" void PostSoLoad(SO *so)
{
    PostLoLoadV();
    UpdateSoXfWorldMatrices(so);
    BindGlobset(&so->model);
}

// 0x1F36E0 UpdateSo
namespace M_UpdateSo { extern "C" {
struct VTE; struct VT; struct E; struct P; 
typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[10]; };
struct E { char pad[0xDC]; void *region; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0xE8 - 0x10]; struct E *physics; char pad3[0x12C - 0xEC]; float alpha; float alphaRate; };
extern struct CLOCK D_002DDED0;
void UpdateLo(struct P *);
qword GetLoWorldPos(struct P *);
void *FindPathzoneAtPos(qword, int);
void SetSoPhysRegion(struct P *, void *);
// Slot 16 (Update): UpdateLo, refreshes the physics region from the position and advances the fade (alpha +=
// rate * dt), removing the object when it fades to 0.
void UpdateSo(struct P *so)
{
    void *q;
    float d;
    UpdateLo(so);
    q = so->physics ? so->physics->region : 0;
    if (q) SetSoPhysRegion(so, FindPathzoneAtPos(GetLoWorldPos(so), 1));
    d = so->alphaRate;
    if (d != 0.0f) {
        float v = so->alpha + D_002DDED0.deltaTime * d;
        so->alpha = v;
        if (v <= 0.0f) {
            so->alpha = 0.0f;
            so->alphaRate = 0.0f;
            so->pvt->a[9].pfn((char *)so + so->pvt->a[9].d);
        } else if (1.0f <= v) {
            so->alpha = 1.0f;
            so->alphaRate = 0.0f;
        }
    }
}
} } // namespace M_UpdateSo

// 0x1F37C8 SetSoPhysRegion
namespace M_func_001F37C8 { extern "C" {
struct Q; struct O; struct E; struct P;
struct Q { char pad[0x118]; char dl[1]; };
struct O { char pad[0x10]; struct Q *pathzone; char pad2[0x20 - 0x14]; char unknown20[1]; };
struct E { char pad[0xDC]; struct O *region; };
struct P { char pad[0xE8]; struct E *physics; };
int IsEntryInDl__FP2DLPv(void *, void *);
void RemoveDlEntry__FP2DLPv(void *, void *);
void AppendDlEntry__FP2DLPv(void *, void *);
void InitObstacleQueryCursor(struct Q *, void *);
// Moves the object's region link from its current region's list to region q.
void SetSoPhysRegion(struct P *so, struct Q *q)
{
    struct O *o = so->physics->region;
    struct Q *old = o->pathzone;
    if (old == q) return;
    if (old) {
        if (IsEntryInDl__FP2DLPv(old->dl, o)) RemoveDlEntry__FP2DLPv(old->dl, o);
    }
    if (q) {
        if (!IsEntryInDl__FP2DLPv(q->dl, o)) {
            AppendDlEntry__FP2DLPv(q->dl, o);
            InitObstacleQueryCursor(q, o->unknown20);
        }
    }
    o->pathzone = q;
}
} } // namespace M_func_001F37C8

// 0x1F3868 UpdateSoPhysRegionCallback
namespace M_func_001F3868 { extern "C" {
typedef int qword __attribute__((mode(TI)));
qword GetLoWorldPos(void *);
int FindPathzoneAtPos(qword, int);
void SetSoPhysRegion(void *, int);
// Deferred callback: SetSoPhysRegion with the region at the object's world position.
void UpdateSoPhysRegionCallback(void *p) { SetSoPhysRegion(p, FindPathzoneAtPos(GetLoWorldPos(p), 1)); }
} } // namespace M_func_001F3868

// 0x1F38A8 UpdateSoPartGeom
/* Recomputes the part's world geometry when its transform changed since the last update. */
// When a part's stamp differs from its XF's, transforms the part's local geometry to world space, copies the stamp
// and marks the tree dirty.
extern "C" void UpdateSoPartGeom(SOPART *part)
{
    XF *pxf;
    pxf = part->transform;
    if (part->meshStamp != pxf->worldStamp) {
        UpdateGeomWorld(&pxf->worldMatrix, &part->meshLocal, &part->meshWorld);
        part->meshStamp = part->transform->worldStamp;
        InvalidateSoAaox(part->owner);
    }
}

// 0x1F3900 UpdateSoPartMatrices
namespace M_UpdateSoPartMatrices { extern "C" {
struct M; struct O; struct E; struct P;
typedef unsigned long ulong;
typedef int qword __attribute__((mode(TI)));
struct M { qword a[4]; };
struct O { char pad[0x40]; struct M matWorld; char pad2[0xA4 - 0x80]; int stamp; };
struct E { struct O *transform; int pad[3]; ulong flags; int pad2[2]; struct M matWorld; char pad3[0xAC - 0x60]; int worldStamp; };
struct P { char pad[0xE0]; int numParts; struct E *parts; };
// Copies each visible part's XF world matrix and stamp into the part.
void UpdateSoPartMatrices(struct P *so)
{
    int i;
    for (i = 0; i < so->numParts; i++) {
        struct E *pe = &so->parts[i];
        if (!(pe->flags & (1L << 50))) {
            struct O *po = pe->transform;
            pe->matWorld = po->matWorld;
            pe->worldStamp = po->stamp;
        }
    }
}
} } // namespace M_UpdateSoPartMatrices

// 0x1F3980 UpdateSoXfWorld
namespace M_func_001F3980 { extern "C" {
struct VTE; struct E; struct Q; struct R; struct P; 
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct VTE { short d; short p; void (*pfn)(void *); };
struct E { char pad[0x10]; ulong flags; char pad2[0xB0 - 0x18]; };
struct Q { char pad[0x98]; short role; char pad2[0xB0 - 0x9A]; qword velocity; char padC0[0xF0 - 0xC0]; int unknownF0[3]; };
struct R { char pad[0x60]; qword velocity; qword angularVelocity; };
struct P { char pad[0xC]; struct VTE *pvt; char pad2[0x58 - 0x10]; struct Q *positionXf; struct Q *rotationXf; char pad3[0xE0 - 0x60]; int numParts; struct E *parts; struct R *physics; };
extern struct CLOCK D_002DDED0;
void UpdateSoPartMatrices(struct P *);
void UpdateSoPartMesh(struct E *, float);
int IsXfRoleVelocity(int);
void UpdateSoSlots(struct P *);
void UpdateSoPartGeom(struct E *);
// Zero the first three ints of a.
static inline void Clear(int *a)
{
    a[2] = 0;
    a[1] = 0;
    a[0] = 0;
}
// Slot 17: per-step transform update: part matrices, soft-mesh parts, XF positions into the physics record, the
// physics preset (slot 70) for dynamic XFs, slots and part geometry.
void UpdateSoXfWorld(struct P *so)
{
    int i;
    UpdateSoPartMatrices(so);
    for (i = 0; i < so->numParts; i++) {
        struct E *pe = &so->parts[i];
        if (*(int *)&pe->flags) UpdateSoPartMesh(pe, D_002DDED0.deltaTime);
    }
    if (so->physics) {
        if (so->positionXf) so->physics->velocity = so->positionXf->velocity;
        if (so->rotationXf) so->physics->angularVelocity = so->rotationXf->velocity;
        if ((unsigned short)(so->positionXf->role - 1) < 2 || so->rotationXf->role == 1 || so->rotationXf->role == 2) {
            if (IsXfRoleVelocity(so->positionXf->role)) {
                Clear(so->positionXf->unknownF0);
            }
            if (IsXfRoleVelocity(so->rotationXf->role)) {
                Clear(so->rotationXf->unknownF0);
            }
            so->pvt[70].pfn((char *)so + so->pvt[70].d);
        }
    }
    UpdateSoSlots(so);
    for (i = 0; i < so->numParts; i++) {
        struct E *pe = &so->parts[i];
        if (!(pe->flags & (1L << 50))) UpdateSoPartGeom(pe);
    }
}
} } // namespace M_func_001F3980

// 0x1F3B28 SetSoGrfBit8
namespace M_func_001F3B28 { extern "C" {
struct P;
typedef unsigned long ulong;

struct P {
    char pad[0x40];
    int flagWord;
    char pad2[0x148 - 0x44];
    ulong flags;
};

int IsLoInWorld(struct P *);
void AddLoToRenderTree(struct P *);
void RemoveLoFromRenderTree(struct P *);

// Slot 21: sets or clears LO flag bit 8; enabling it registers objects marked with bit 35 that are in the world,
// disabling unregisters them (meaning inferred).
void SetSoGrfBit8(struct P *so, int enabled)
{
    register int word __asm__("a2") = so->flagWord;
    register int flags __asm__("v1") = word;
    register int firstTest __asm__("v0");

    __asm__("xori %0,%1,1\n\tandi %0,%0,1"
            : "=r"(firstTest) : "r"(word));
    if (firstTest)
        __asm__("lw %0,0(%0)" : "+r"(flags) : "m"(*(int *)flags));
    {
        register int has8 __asm__("v0") = flags & 8;
        if (enabled) {
            if (has8)
                return;
            {
                register int tagged __asm__("v1") = word;
                register int indirect __asm__("v0");
                __asm__("xori %0,%1,1\n\tandi %0,%0,1"
                        : "=r"(indirect) : "r"(tagged));
                if (indirect)
                    *(int *)tagged |= 8;
                else {
                    register int value __asm__("v0") = word | 8;
                    __asm__("sw %1,%0" : "=m"(so->flagWord) : "r"(value));
                }
            }
            if ((so->flags & (1L << 35)) && IsLoInWorld(so))
                AddLoToRenderTree(so);
        } else {
            if (!has8)
                return;
            {
                register int *destination __asm__("a0") = (int *)word;
                register int mask __asm__("v1") = ~8;
                if ((word ^ 1) & 1) {
                    register int value __asm__("v0");
                    __asm__("lw %0,0(%1)" : "=r"(value)
                            : "r"(destination), "m"(*destination));
                    *destination = value & mask;
                } else {
                    register int value __asm__("v0") = word & ~8;
                    __asm__("sw %1,%0" : "=m"(so->flagWord) : "r"(value));
                }
            }
            RemoveLoFromRenderTree(so);
        }
    }
}
} } // namespace M_func_001F3B28

// 0x1F3C10 RefreshSoBoundsRecord
namespace M_func_001F3C10 { extern "C" {
struct G; struct Q; struct A;
struct G { char pad[0x3D60]; int nStampLights; };
extern struct G D_003DD490;
struct Q { char pad[0xC]; int stamp; };
struct A { char pad[0x134]; struct Q *bounds; };
void BuildSoLightPackets(struct G *pg, int n, struct Q *pq);
// Recomputes the shared bounds record when its stamp is older than the world's current stamp. (inferred)
void RefreshSoBoundsRecord(struct A *so)
{
    struct Q *pq = so->bounds;
    if (pq && pq->stamp != D_003DD490.nStampLights) {
        BuildSoLightPackets(&D_003DD490, 0, pq);
        so->bounds->stamp = D_003DD490.nStampLights;
    }
}
} } // namespace M_func_001F3C10

// 0x1F3C70 RenderSoSelf
// Slot 19 (RenderSelf): draws the glob set with the object's alpha when it is visible in the current camera mask.
extern "C" void RenderSoSelf(SO *so)
{
    float uAlpha;
    if (so->visibilityMask & (*(int *)&g_cm.grfvis)) {
        RefreshSoBoundsRecord(so);
        uAlpha = so->opacity;
        RenderSoGlobset(&so->model, 0, uAlpha, uAlpha, so->psobounds);
    }
}

// 0x1F3CC8 RenderSoAll
namespace M_func_001F3CC8 { extern "C" {
struct B; struct VTE; struct O; struct P;
struct B { char b[0x30]; };
struct VTE { short d; short pad; void (*pfn)(void *, int, float, float, struct B *); };
struct O { char pad[0xC]; struct VTE *pvt; char pad2[0x24 - 0x10]; struct O *next; };
struct P { char pad[0x64]; struct O *firstChild; char pad2[0x70 - 0x68]; struct O *firstRemovedChild; char pad3[0x80 - 0x74]; char globset[0x134 - 0x80]; struct B *bounds; };
struct G;
extern struct G D_003DD490;
void BuildSoLightPackets(struct G *, int, struct B *);
void RenderSoGlobset(void *, int, float, float, struct B *);
// Slot 20: draws the glob set with explicit parameters and recurses into the children.
void RenderSoAll(struct P *so, int a, struct B *pb, float f1, float f2)
{
    struct B b;
    int unused[4];
    struct O *po;
    if (!pb && so->bounds) {
        b = *so->bounds;
        pb = &b;
        BuildSoLightPackets(&D_003DD490, a, &b);
    }
    RenderSoGlobset(so->globset, a, f1, f2, pb);
    for (po = so->firstChild; po != so->firstRemovedChild; po = po->next)
        po->pvt[20].pfn((char *)po + po->pvt[20].d, a, f1, f2, pb);
}
} } // namespace M_func_001F3CC8

// 0x1F3E00 OnSoRemove
namespace M_OnSoRemove { extern "C" {
struct VTE; struct VT; struct L; struct E;
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[50]; };
struct E;
struct L { char pad[0xC]; struct VT *pvt; char pad1[0x24 - 0x10]; struct L *next; char pad2[0x64 - 0x28]; struct L *firstChild; char pad3[0x70 - 0x68]; struct L *firstRemovedChild; char pad4[0xEC - 0x74]; struct E *tree; };
struct E { char pad[0x60]; struct L *rootSo; };
struct DL;
void OnLoRemove(struct L *);
void RemoveLoFromRenderTree(struct L *);
void RemoveDlEntry__FP2DLPv(struct DL *, void *);
#define VF(q, n) (q)->pvt->a[n].pfn((char *)(q) + (q)->pvt->a[n].d)
// Slot 22 (OnRemove): the object leaves the active world: base LO freeze, unregister, children, and if it owns its
// tree, a final bounds update and removal from g_sw.dlSoTree.
void OnSoRemove(struct L *so)
{
    struct L *q;
    int unused[4];
    OnLoRemove(so);
    RemoveLoFromRenderTree(so);
    for (q = so->firstChild; q != so->firstRemovedChild; q = q->next) VF(q, 22);
    if (so->tree && so->tree->rootSo == so) {
        VF(so, 49);
        RemoveDlEntry__FP2DLPv(&g_sw.dlSoTree, so->tree);
    }
}
} } // namespace M_OnSoRemove

// 0x1F3EA8 OnSoAdd
namespace M_OnSoAdd { extern "C" {
struct VTE; struct VT; struct L; struct E;
typedef unsigned long ulong;
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[50]; };
struct E;
struct L { char pad[0xC]; struct VT *pvt; char pad1[0x24 - 0x10]; struct L *next; char pad1b[0x40 - 0x28]; int flagWord; char pad2[0x64 - 0x44]; struct L *firstChild; char pad3[0x70 - 0x68]; struct L *firstRemovedChild; char pad4[0xEC - 0x74]; struct E *tree; char pad5[0x148 - 0xF0]; ulong flags; };
struct E { char pad[0x60]; struct L *rootSo; };
struct DL;
void OnLoAdd(struct L *);
void AddLoToRenderTree(struct L *);
int IsEntryInDl__FP2DLPv(struct DL *, void *);
void AppendDlEntry__FP2DLPv(struct DL *, void *);
#define VF(q, n) (q)->pvt->a[n].pfn((char *)(q) + (q)->pvt->a[n].d)
// Slot 23 (OnAdd): the object enters the active world: base LO unfreeze, re-register, children, and its tree joins
// g_sw.dlSoTree with fresh bounds.
void OnSoAdd(struct L *so)
{
    struct L *q;
    int unused[4];
    OnLoAdd(so);
    if (so->flags & (1L << 35)) {
        int v = so->flagWord;
        if ((v ^ 1) & 1) v = *(int *)v;
        if (v & 8) AddLoToRenderTree(so);
    }
    for (q = so->firstChild; q != so->firstRemovedChild; q = q->next) VF(q, 23);
    if (so->tree && so->tree->rootSo == so) {
        if (!IsEntryInDl__FP2DLPv((DL *)&g_sw.dlSoTree, so->tree)) AppendDlEntry__FP2DLPv(&g_sw.dlSoTree, so->tree);
        VF(so, 49);
    }
}
} } // namespace M_OnSoAdd

// 0x1F3FA0 GetSoWorldMatrix
// Slot 31: the world matrix of the root XF.
MATRIX4 * SO::GetWorldMatrix()
{
    SO *pso = (SO *)this;
    return &pso->rootTransform->worldMatrix;
}

// 0x1F3FB0 GetSoLocalXf
namespace M_func_001F3FB0 { extern "C" {
struct A;
struct A { char pad[0x60]; int rootTransform; };
void func_002333B4(void *p, int c, int cb) __asm__("memset");
// Slot 32: the root XF, optionally writing an identity matrix to the output.
int GetSoLocalXf(struct A *so, float *pm)
{
    if (pm) {
        func_002333B4(pm, 0, 0x40);
        pm[0] = pm[5] = pm[10] = pm[15] = 1.0f;
    }
    return so->rootTransform;
}
} } // namespace M_func_001F3FB0

// 0x1F4010 ApplySoProxy
namespace M_func_001F4010 { extern "C" {
struct M4; struct T; struct VTE; struct VT; struct P; struct G;
typedef int qword __attribute__((mode(TI)));
struct M4 { qword a[4]; };
struct T { struct M4 matWorld; char pad[0xA4 - 0x40]; int stamp; };
struct VTE { short d; short p; int (*pfn)(void *); };
struct VT { struct VTE a[19]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x54 - 0x10]; struct T *rootTransform; };
struct G { char pad[0x3D58]; int nStampXf; };
extern struct G D_003DD490;
struct M4 __ml__FRC7MATRIX4T0(void *, struct T *);
void UpdateSoIkBoneLengths(struct P *);
// Slot 37: applies a proxy's matrix to the root XF, bumps the world stamp and updates the transform hierarchy.
void ApplySoProxy(struct P *so, void *pv)
{
    struct T *pt = so->rootTransform;
    pt->matWorld = __ml__FRC7MATRIX4T0(pv, pt);
    { int n = ++D_003DD490.nStampXf; so->rootTransform->stamp = n; }
    so->pvt->a[18].pfn((char *)so + so->pvt->a[18].d);
    UpdateSoIkBoneLengths(so);
}
} } // namespace M_func_001F4010

// 0x1F40A0 TranslateSoToPos
namespace M_TranslateSoToPos {
struct LO; struct E; struct VTE; struct P; struct SW;
extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
typedef union { struct { M3 m3; qword r3; } s; float f[4][4]; qword r[4]; } __attribute__((aligned(16))) M4;
typedef union { qword q; float f[4]; } V4;
struct LO { char pad[0x30]; V4 position; M4 matWorld; char pad2[0xA0 - 0x80]; struct LO *parent; int stamp; };
struct REGION;
struct REGION_LINK;
struct E { char pad[0xDC]; struct REGION_LINK *regionLink; };
struct VTE { short d; short p; void (*pfn)(void *); };
struct P { char pad[0xC]; struct VTE *pvt; char pad2[0x58 - 0x10]; struct LO *positionXf; char pad3[0x60 - 0x5C]; struct LO *rootTransform; char pad4[0xE8 - 0x64]; struct E *physics; };
struct SW { char pad[0x3D58]; int transformStamp; };
extern struct SW D_003DD490;
qword *VECTOR_opAssign(qword *, qword);
M4 *InvertMatrix4Rigid(M4 *,const M4 *);
M4 *__ml__FRC7MATRIX4T0(M4 *, const M4 *, const M4 *);
void *func_002333B4(void *, int, int) __asm__("memset");
void FreeSoContacts(struct P *);
void UnsettleXfSlot(struct P *, struct LO *);
void UpdateSoPartMatrices(struct P *);
qword GetLoWorldPos(struct P *);
struct REGION *FindPathzoneAtPos(qword, int);
void SetSoPhysRegion(struct P *, struct REGION *);
// Slot 29: moves the object to a world position (the position XF expressed relative to its parents), frees its
// contacts and refreshes XF modes, part matrices and region.
void TranslateSoToPos(struct P *so, qword v)
{
    M4 m0, matWorld, m80, mC0, m100, m140, m180, m1C0;
    struct LO *plo;
    const M3 *basis;
    register M4 *presult __asm__("$18");
    if (!so->positionXf) return;
    basis = &so->rootTransform->matWorld.s.m3;
    VECTOR_opAssign(&m0.s.r3, v);
    m0.s.m3 = *basis;
    m0.f[1][3] = 0.0f;
    m0.f[2][3] = 0.0f;
    m0.f[0][3] = 0.0f;
    m0.f[3][3] = 1.0f;
    presult=&mC0;
    InvertMatrix4Rigid(&m100,&so->positionXf->matWorld);
    __ml__FRC7MATRIX4T0(presult, &m100, &so->rootTransform->matWorld);
    matWorld.r[1] = mC0.r[1];
    matWorld.r[0] = mC0.r[0];
    matWorld.r[2] = mC0.r[2];
    matWorld.r[3] = mC0.r[3];
    if (so->positionXf->parent) {
        m80 = so->positionXf->parent->matWorld;
    } else {
        M4 *identity = &m80;
        func_002333B4(identity, 0, 0x40);
        identity->f[3][3] = 1.0f;
        identity->f[2][2] = 1.0f;
        identity->f[1][1] = 1.0f;
        m80.f[0][0] = 1.0f;
    }
    M4 *world=&m140;
    InvertMatrix4Rigid(&m180,&m80);
    __ml__FRC7MATRIX4T0(world, &m180, &m0);
    InvertMatrix4Rigid(&m1C0,&matWorld);
    __ml__FRC7MATRIX4T0(&m100, world, &m1C0);
    plo = so->positionXf;
    mC0 = m100;
    VECTOR_opAssign(&plo->position.q, mC0.r[3]);
    plo->position.f[3] = 1.0f;
    { int stamp=++D_003DD490.transformStamp;so->positionXf->stamp=stamp; }
    so->pvt[18].pfn((char *)so + so->pvt[18].d);
    FreeSoContacts(so);
    UnsettleXfSlot(so, so->positionXf);
    UpdateSoPartMatrices(so);
    {
        struct REGION_LINK *link = 0;
        if (so->physics) link = so->physics->regionLink;
        if (link) SetSoPhysRegion(so, FindPathzoneAtPos(GetLoWorldPos(so), 1));
    }
}

}
} // namespace M_TranslateSoToPos

// 0x1F42E0 RotateSoToMat
namespace M_RotateSoToMat {
union U; struct M3; struct LO; struct VTE; struct P; struct G;
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; int i[4]; };
// 3x3 matrix of U rows with copy constructor and assignment.
struct M3 { U r[3]; M3(){} M3(const M3 &m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];} M3 &operator=(const M3 &m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return *this;} };
struct LO { M3 matrix; U position; M3 rotation; char pad[0x30]; LO *parent; int stamp; };
struct VTE { short delta,pad; void (*fn)(void *); };
struct P { char pad[0xC]; VTE *vt; char pad10[0x4C]; LO *object; };
struct G { char pad[0x3D58]; int stamp; };
extern "C" { extern G D_003DD490; }
extern "C" M3 InvertMatrix3Cofactor(const M3 *);
extern "C" void NormalizeRotateMatrix3__FP7MATRIX3(M3 *);
extern "C" void *func_002333B4(void *,int,int) __asm__("memset");
extern "C" void FreeSoContacts(P *);
extern "C" void UnsettleXfSlot(P *,LO *);
extern "C" void UpdateSoPartMatrices(P *);
// Return the 3x3 matrix product of a and b.
static inline M3 Mul(const M3 &a,const M3 &b) return r
{
    qword a0=a.r[0].q,a1=a.r[1].q,a2=a.r[2].q;
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[0].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[0].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[1].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[1].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[2].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[2].q));
}
// Slot 30: sets the rotation from a world matrix (normalised against the parents), frees contacts and refreshes.
extern "C" void RotateSoToMat(P *so,const M3 *basis)
{
    LO *object=so->object;
    if(!object)return;
    {
        M3 m;
        if(object->parent) {
            m=object->parent->rotation;
            NormalizeRotateMatrix3__FP7MATRIX3(&m);
        } else {
            func_002333B4(&m,0,sizeof(m));
            m.r[2].f[2]=1.0f;
            m.r[1].f[1]=1.0f;
            m.r[0].f[0]=1.0f;
        }
        M3 transformed=Mul(InvertMatrix3Cofactor(&m),*basis);
        M3 *dest=&so->object->matrix;
        *dest=transformed;
        dest->r[0].i[3]=0;
        dest->r[1].i[3]=0;
        dest->r[2].i[3]=0;
        int stamp=++D_003DD490.stamp;
        so->object->stamp=stamp;
        so->vt[18].fn((char *)so+so->vt[18].delta);
        FreeSoContacts(so);
        UnsettleXfSlot(so,so->object);
        UpdateSoPartMatrices(so);
    }
}
} // namespace M_RotateSoToMat

// 0x1F4440 GetSoVelocity
namespace M_GetSoVelocity { extern "C" {
struct T; struct S;
typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct T { char pad[0x70]; qword position; char pad2[0x14]; int kind; };
struct S { char pad[0x58]; struct T *positionXf; };
qword GetXfVelocity(struct T *, int, qword);
// Slot 33: world-space velocity of the position XF (when it is dynamic), else zero.
qword GetSoVelocity(struct S *p) { struct T *pt = p->positionXf; if (!pt || pt->kind != 1) return D_002A2DD0; return GetXfVelocity(pt, 0, pt->position); }
} } // namespace M_GetSoVelocity

// 0x1F4488 SetSoVelocity
namespace M_SetSoVelocity { extern "C" {
struct VTE; struct VT; struct O; struct P;
typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short p; void *pfn; };
struct VT { struct VTE a[34]; };
struct O { char pad[0x94]; int kind; short role; char pad2[0xA0 - 0x9A]; int parent; char pad3[0xB0 - 0xA4]; union { qword q; int a[4]; } velocity; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x58 - 0x10]; struct O *positionXf; };
int IsXfRoleMoving(int);
void SetXfRole(struct P *, struct O *, int);
qword ConvertXfVec(int, int, qword);
void UnsettleXfSlot(struct P *, struct O *);
void InvalidateSoAaox(struct P *);
void WakeSo(struct P *);
void SetSoBit35Hierarchy(struct P *, int);
// Slot 35: sets the position XF's velocity so the world velocity becomes v, then refreshes and marks the physics.
void SetSoVelocity(struct P *so, qword q)
{
    struct O *po = so->positionXf;
    qword cur, d;
    if (!po || po->kind != 1) return;
    if (!IsXfRoleMoving(po->role)) SetXfRole(so, po, 1);
    { float *pv = (float *)&po->velocity; pv[2] = 0.0f; pv[1] = 0.0f; pv[0] = 0.0f; }
    cur = ((qword (*)(void *))so->pvt->a[33].pfn)((char *)so + so->pvt->a[33].d);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(q), "j"(cur));
    po->velocity.q = ConvertXfVec(0, po->parent, d);
    UnsettleXfSlot(so, po);
    InvalidateSoAaox(so);
    WakeSo(so);
    SetSoBit35Hierarchy(so, 0);
}
} } // namespace M_SetSoVelocity

// 0x1F4558 GetSoAngularVelocity
// GetSoAngularVelocity: ported from Lilly's personal Sly 2 decomp (src/P2/so_motion.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A2DD0 (g_qwZero)
// 0x001F4558 GetAngularVelocity__2SO  (MATCHES Sly 2, T0906)
// Virtual 33: the angular velocity of the rotation XF (xfk 6) in world space, zero without one. The chain's rate matrix (MatDmatOfXfChain,
// MATCH) is turned by N * inverse(worldMatrix) (N = worldMatrix with its rotation orthonormalised: the scale is taken out) and the skew-symmetric
// part (m12, m20, m01) of the product is the angular velocity, packed into a quadword with the MMI instructions pextlw / pcpyld.
// HOW IT IS WRITTEN: (1) the result of MatDmatOfXfChain is used through `&MatDmatOfXfChain(pxf, NULL)` (taking the address of the temporary:
// the compiler warns and keeps the pointer in a register across the call, s0; that is what makes the copy of the temporary read
// `lq 0x60($sp)` / `lq 0x10($s0)` / `lq 0x20($s0)`, rows 1 and 2 through the pointer); (2) matNorm and matRate are declared before the call
// (frame: matNorm 0x00, matRate 0x30, the temporary 0x60) and the product is an in-place assignment; pmatWorld is a pointer variable (s1);
// (3) THE PACKING IS ONE asm STATEMENT that contains the three mfc1 as well (the floats come in as "f" operands, two scratch GPRs):
// with the mfc1 outside (the compiler's own, two asm statements) the scheduler sees gZ's path to the end one instruction shorter than
// gX's and gY's, gives the row-0 chain of the product a lower priority and computes the rows in the order 1, 2, 0 (51.6%); with ONE
// statement the three chains tie and the rows come out 0, 1, 2 as in the game. (Same recipe as the fused qmfc2 + mtc1 of vu0.h.)
extern "C" MATRIX3 InvertMatrix3Cofactor(MATRIX3 *pmat);

// 3x3 matrix product matL * matR.
inline MATRIX3 operator*(const MATRIX3 &matL, const MATRIX3 &matR) return mat
{
    qword qwL0 = *(qword *)matL.mat[0];
    qword qwL1 = *(qword *)matL.mat[1];
    qword qwL2 = *(qword *)matL.mat[2];
    VU_VECTOR4 v0;
    VU_VECTOR4 v1;
    VU_VECTOR4 v2;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v0.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(((VU_VECTOR4 *)matR.mat[0])->qw));
    *(VU_VECTOR4 *)mat.mat[0] = v0;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v1.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(((VU_VECTOR4 *)matR.mat[1])->qw));
    *(VU_VECTOR4 *)mat.mat[1] = v1;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v2.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(((VU_VECTOR4 *)matR.mat[2])->qw));
    *(VU_VECTOR4 *)mat.mat[2] = v2;
}


extern qword D_002A2DD0;
void NormalizeRotateMatrix3(MATRIX3 *pmat);
MATRIX4 MatDmatOfXfChain(XF *pxfFrom, XF *pxfTo) __asm__("GetXfAngularVelocity");
// Slot 34: world-space angular velocity of the rotation XF (when it is dynamic), else zero (Lilly port).
qword SO::GetAngularVelocity()
{
    XF *pxf = unknown5C;
    if (!pxf || pxf->kind != 6)
    {
        return D_002A2DD0;
    }
    MATRIX3 matNorm;
    MATRIX3 matRate;
    MATRIX4 *pmatDmat = &MatDmatOfXfChain(pxf, 0);
    matRate = *(MATRIX3 *)pmatDmat;
    MATRIX3 *pmatWorld = (MATRIX3 *)&pxf->worldMatrix;
    matNorm = *pmatWorld;
    NormalizeRotateMatrix3(&matNorm);
    matRate = matNorm * InvertMatrix3Cofactor(pmatWorld) * matRate;
    float gX = matRate.mat[1][2];
    float gY = matRate.mat[2][0];
    float gZ = matRate.mat[0][1];
    qword qwXYZ;
    int iTmp;
    __asm__("mfc1 %0, %2\n\tmfc1 %1, %3\n\tpextlw %0, %1, %0\n\tmfc1 %1, %4\n\tpcpyld %0, %1, %0"
            : "=&r"(qwXYZ), "=&r"(iTmp) : "f"(gX), "f"(gY), "f"(gZ));
    VECTOR4 vecW;
    *(qword *)&vecW = qwXYZ;
    return *(qword *)&vecW;
}

// 0x1F46D8 SetSoAngularVelocity
namespace M_SetSoAngularVelocity {
union U; struct M3; struct LO; struct VTE; struct P;
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; int i[4]; };
// 3x3 matrix of U rows with copy constructor and assignment.
struct M3 { U r[3]; M3(){} M3(const M3 &m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];} M3 &operator=(const M3 &m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return *this;} };
struct LO { M3 matrix; U position; M3 rotation; char pad[0x24]; int kind; short state; char pad9A[6]; LO *parent; int stamp; char padA8[8]; U velocity; };
struct VTE { short delta,pad; qword (*fn)(void *); };
struct P { char pad[0xC]; VTE *vt; char pad10[0x4C]; LO *object; };
extern "C" M3 InvertMatrix3Cofactor(const M3 *);
extern "C" void NormalizeRotateMatrix3__FP7MATRIX3(M3 *);
extern "C" void *func_002333B4(void *,int,int) __asm__("memset");
extern "C" void UnsettleXfSlot(P *,LO *);
// Return the 3x3 matrix product of a and b.
static inline M3 Mul(const M3 &a,const M3 &b) return r
{
    qword a0=a.r[0].q,a1=a.r[1].q,a2=a.r[2].q;
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[0].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[0].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[1].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[1].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[2].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[2].q));
}
extern "C" int IsXfRoleMoving(int);
extern "C" void SetXfRole(P *,LO *,int);
extern "C" qword ConvertXfVec(LO *,LO *,qword);
extern "C" void InvalidateSoAaox(P *);
extern "C" void WakeSo(P *);
extern "C" void SetSoBit35Hierarchy(P *,long);
// Return a - *pb.
static inline qword Sub(qword a,const qword *pb) { qword r,b; __asm__("lqc2 %1,%3\n\tvsub.xyzw %0,%2,%1":"=j"(r),"=&j"(b):"j"(a),"m"(*pb));return r; }
// Rotate v by the 3x3 matrix m.
static inline qword Transform(const M3 &m,qword v) { qword r;
 __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r):"j"(m.r[0].q),"j"(m.r[1].q),"j"(m.r[2].q),"j"(v));return r;
}
// Zero the xyz words of v.
static inline void Clear(U *v) { v->i[2]=0;v->i[1]=0;v->i[0]=0; }
// Slot 36: sets the rotation XF's angular velocity from a world-space w.
extern "C" void SetSoAngularVelocity(P *so,qword position)
{
    LO *object=so->object;
    if(!object || object->kind!=6)return;
    if(!IsXfRoleMoving(object->state))SetXfRole(so,object,1);
    const M3 *rotation=&object->rotation;
    Clear(&object->velocity);
    M3 normalized;
    qword origin=so->vt[34].fn((char *)so+so->vt[34].delta);
    normalized=*rotation;
    NormalizeRotateMatrix3__FP7MATRIX3(&normalized);
    M3 relative=Mul(*rotation,InvertMatrix3Cofactor(&normalized));
    object->velocity.q=ConvertXfVec(0,object->parent,Transform(relative,Sub(position,&origin)));
    UnsettleXfSlot(so,object);
    InvalidateSoAaox(so);
    WakeSo(so);
    SetSoBit35Hierarchy(so,0);
}
} // namespace M_SetSoAngularVelocity

// 0x1F4848 GetSoPosXfVelocityLocal
// The position XF's velocity converted between XF spaces.
extern "C" void GetSoPosXfVelocityLocal(SO *so)
{
    XF *pxf;
    pxf = so->unknown58;
    ConvertXfVec(pxf->parent, so->rootTransform, *(qword *)&pxf->velocity);
}

// 0x1F4870 GetSoRotXfVelocityLocal
// The rotation XF's angular velocity converted between XF spaces.
extern "C" void GetSoRotXfVelocityLocal(SO *so)
{
    XF *pxf;
    pxf = so->unknown5C;
    ConvertXfVec(pxf->parent, so->rootTransform, *(qword *)&pxf->velocity);
}

// 0x1F4898 SetSoPosXfVelocityLocal
namespace M_func_001F4898 { extern "C" {
struct O; struct P; struct Q;
typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x98]; short role; char pad2[0xA0 - 0x9A]; int parent; char pad3[0xB0 - 0xA4]; qword velocity; };
struct P { char pad[0x58]; struct O *positionXf; };
struct Q { char pad[0x60]; int rootTransform; };
int IsXfRoleMoving(int);
void SetXfRole(void *, struct O *, int);
qword ConvertXfVec(int, int, qword);
void UnsettleXfSlot(void *, struct O *);
void InvalidateSoAaox(void *);
void WakeSo(void *);
void SetSoBit35Hierarchy(void *, int);
// Sets the position XF's velocity from a vector in the root XF's space, refreshes and marks the physics.
void SetSoPosXfVelocityLocal(struct P *so, qword v)
{
    struct O *po;
    if (!IsXfRoleMoving(so->positionXf->role)) SetXfRole(so, so->positionXf, 1);
    po = so->positionXf;
    po->velocity = ConvertXfVec(((struct Q *)so)->rootTransform, po->parent, v);
    UnsettleXfSlot(so, so->positionXf);
    InvalidateSoAaox(so);
    WakeSo(so);
    SetSoBit35Hierarchy(so, 0);
}
} } // namespace M_func_001F4898

// 0x1F4930 SetSoRotXfVelocityLocal
namespace M_func_001F4930 { extern "C" {
struct O; struct P; struct Q;
typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x98]; short role; char pad2[0xA0 - 0x9A]; int parent; char pad3[0xB0 - 0xA4]; qword velocity; };
struct P { char pad[0x5C]; struct O *rotationXf; };
struct Q { char pad[0x60]; int rootTransform; };
int IsXfRoleMoving(int);
void SetXfRole(void *, struct O *, int);
qword ConvertXfVec(int, int, qword);
void UnsettleXfSlot(void *, struct O *);
void InvalidateSoAaox(void *);
void WakeSo(void *);
void SetSoBit35Hierarchy(void *, int);
// Sets the rotation XF's angular velocity from a vector in the root XF's space.
void SetSoRotXfVelocityLocal(struct P *so, qword v)
{
    struct O *po;
    if (!IsXfRoleMoving(so->rotationXf->role)) SetXfRole(so, so->rotationXf, 1);
    po = so->rotationXf;
    po->velocity = ConvertXfVec(((struct Q *)so)->rootTransform, po->parent, v);
    UnsettleXfSlot(so, so->rotationXf);
    InvalidateSoAaox(so);
    WakeSo(so);
    SetSoBit35Hierarchy(so, 0);
}
} } // namespace M_func_001F4930

// 0x1F49C8 MatchSoXf
namespace M_func_001F49C8 {
class B; class X;
typedef int qword __attribute__((mode(TI)));
class B {
public:
    int a, b, c;
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual qword Get33();
    virtual qword Get34();
    virtual void Set35(qword);
    virtual void Set36(qword);
};
class X : public B {
public:
    char pad[0xE8 - 0x10];
    void *physics;
};
extern "C" void MoveLoToObject(X *, X *);
// Slot 38: matches another object's transform (MatchLoXf) and copies its velocities when the object has physics.
extern "C" void MatchSoXf(X *so, X *q)
{
    MoveLoToObject(so, q);
    if (so->physics) {
        so->Set35(q->Get33());
        so->Set36(q->Get34());
    }
}
} // namespace M_func_001F49C8

// 0x1F4A80 SetSoXf
namespace M_func_001F4A80 {
union U; struct M4; struct VTE; struct A; struct P;
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
// 4x4 matrix of U rows with copy constructor and assignment.
struct M4 {U r[4];M4(){} M4(const M4&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];r[3]=m.r[3];} M4&operator=(const M4&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];r[3]=m.r[3];return *this;}};
struct VTE { short d; short pad; void (*pfn)(); };
struct A { char pad[0x70]; qword pos; };
struct P { char pad[0xC]; struct VTE *pvt; char pad2[0xE8 - 0x10]; void *pe; };
extern "C" void SetLoTransformFromXf(struct P *p, struct A *pa);
extern "C" M4 *GetXfAngularVelocity(M4 *,A *,A *);
// The object's angular-velocity matrix (GetXfAngularVelocity).
static inline M4 BuildVelocity(A *owner) return result { GetXfAngularVelocity(&result,owner,0); }
// Pack x, y, z into a 128-bit GPR value.
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__ __volatile__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
typedef void (*SETVECTOR)(void *,qword);
// Load a vtable entry's function pointer.
static inline SETVECTOR ReadSetter(const VTE &entry){
 SETVECTOR function;
 asm("lw %0,%1":"=r"(function):"m"(entry.pfn));
 return function;
}
// Slot 39: sets the transform (SetXf) and derives linear and angular velocity from the XF rate matrix.
extern "C" void SetSoXf(struct P *so, struct A *pa)
{
    M4 m;
    qword q;
    SetLoTransformFromXf(so, pa);
    if (!so->pe) return;
    { M4 t = BuildVelocity(pa); m = t; }
    VTE *vt=so->pvt;
    int delta=vt[35].d;
    q = Make(m.r[1].f[2],m.r[2].f[0],m.r[0].f[1]);
    SETVECTOR setpos=ReadSetter(vt[35]);
    {
        register qword r __asm__("vf6");
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w" : "+j"(r) : "j"(m.r[0].q), "j"(m.r[1].q), "j"(m.r[2].q), "j"(m.r[3].q), "j"(pa->pos));
    U v; v.q = q;
    qword rr;__asm__("qmfc2.ni %0,%1":"=r"(rr):"j"(r));
    setpos((char*)so+delta,rr);
    }
    {
        ((void (*)(void *, qword))so->pvt[36].pfn)((char *)so + so->pvt[36].d, q);
    }
}
} // namespace M_func_001F4A80

// 0x1F4B58 MarkSoTreeBoundsDirty
/* Queues the object's tree on list 1 (g_sw.dlSoTreeQueue1) once. */
// Marks the tree's bounds dirty (flag bit 0) and queues it on g_sw.dlSoTreeQueue1.
extern "C" void MarkSoTreeBoundsDirty(SO *so)
{
    SOTREE *ptree;
    ptree = so->physicsTree;
    if (ptree != 0) {
        if ((uchar)ptree->isInFirstQueue == 0) {
            ptree->isInFirstQueue = 1;
            AppendDlEntry(&g_sw.dlSoTreeQueue1, so->physicsTree);
        }
    }
}

// 0x1F4BA8 ClearSoTreeBoundsDirty
namespace M_func_001F4BA8 { extern "C" {
struct T; struct S;
struct T { char pad[0x70]; ulong flags; };
struct S { char pad[0xEC]; struct T *tree; };
struct DL;
void RemoveDlEntry__FP2DLPv(struct DL *, void *);
// Clears the tree's bounds-dirty flag and removes it from g_sw.dlSoTreeQueue1.
void ClearSoTreeBoundsDirty(struct S *so)
{
    struct T *pt = so->tree;
    if (!pt) return;
    if ((pt->flags & 1) == 0) return;
    pt->flags &= ~1;
    RemoveDlEntry__FP2DLPv(&g_sw.dlSoTreeQueue1, so->tree);
}
} } // namespace M_func_001F4BA8

// 0x1F4BF8 InvalidateSoAaox
/* Queues the object's tree on lists 1 and 2 (g_sw.dlSoTreeQueue2) once.
   Per-object counterpart of Sly 1's InvalidateSwAaox. */
// After an object moved or changed its bounds: marks the tree bounds dirty and flags its broad-phase box once,
// queueing it for the next sweep.
extern "C" void InvalidateSoAaox(SO *so)
{
    SOTREE *ptree;
    MarkSoTreeBoundsDirty(so);
    ptree = so->physicsTree;
    if (ptree != 0) {
        if (ptree->isInSecondQueue == 0) {
            ptree->isInSecondQueue = 1;
            AppendDlEntry(&g_sw.dlSoTreeQueue2, so->physicsTree);
        }
    }
}

// 0x1F4C58 FindSoXf
namespace M_PxfFindSo { extern "C" {
struct E; struct S;
struct E { char pad[0x94]; int kind; int pad2; int oid; char pad4[0x60]; };
struct S { char pad[0x50]; int numTransforms; struct E *transforms; };
// Finds the object's XF whose ids match the given XF's (Sly 1 PxfFindSo).
struct E *FindSoXf(struct S *so, int oid, int kind)
{
    int i;
    for (i = 0; i < so->numTransforms; i++) { struct E *pe = &so->transforms[i]; if (pe->oid == oid && pe->kind == kind) return pe; }
    return 0;
}
} } // namespace M_PxfFindSo

// 0x1F4CA0 CloneSoXfRef
namespace M_func_001F4CA0 { extern "C" {
struct T; struct S;
struct T { char pad[0x80]; int unknown80; int unknown84; void *source88; void *source8C; char pad2[0x100 - 0x90]; };
struct S { char pad[0x10]; struct S *parent; char pad2[0x50 - 0x14]; int numTransforms; struct T *transforms; };
// Slot 52: re-points an XF pointer from the source object's XF block into the clone's.
void CloneSoXfRef(struct S *pnew, struct S *pold, struct T **pp)
{
    struct T *t = *pp;
    struct T *n;
    int i;
    if (!t) return;
    n = 0;
    i = t - pold->transforms;
    if (i >= 0 && i < pnew->numTransforms) {
        n = pnew->transforms + i;
    } else {
        pold = pold->parent;
        i = t - pold->transforms;
        if (i >= 0 && i < pold->numTransforms) n = pnew->parent->transforms + i;
    }
    if (n->source88 == &t->unknown80) n->source88 = &n->unknown80;
    if (n->source8C == &t->unknown84) n->source88 = &n->unknown84;   /* sic: the game writes source88 here too */
    *pp = n;
}
} } // namespace M_func_001F4CA0

// 0x1F4D50 ClearSoTreeDirty
// Clears both tree-dirty flags and removes the tree from both queues.
extern "C" void ClearSoTreeDirty(SO *so)
{
    SOTREE *ptree;
    ClearSoTreeBoundsDirty(so);
    ptree = so->physicsTree;
    if (ptree != 0) {
        if (ptree->isInSecondQueue) {
            ptree->isInSecondQueue = 0;
            RemoveDlEntry(&g_sw.dlSoTreeQueue2, so->physicsTree);
        }
    }
}

// 0x1F4DA8 RemoveSoContactsWith
namespace M_func_001F4DA8 { extern "C" {
struct N;
struct N { int pad[2]; void *so; char pad2[0xAC - 0xC]; struct N *next; };
struct G;
extern struct G D_003DD490;
void WakeSo(void *);
void FreeSwXpList__FP2SWP2XP(struct G *, struct N *);
// Unlinks and frees the contacts between two objects, then invalidates the first one's box.
void RemoveSoContactsWith(void *pa, void *pb, struct N **ppn)
{
    struct N *pfree = 0;
    struct N *pn;
loop:
    pn = *ppn;
    if (pn) {
        int i = pn->so == pa;
        if (((struct N *)((char *)pn + (i << 6)))->so == pb) {
            *ppn = pn->next;
            pn->next = pfree;
            pfree = pn;
        } else {
            ppn = &pn->next;
        }
        goto loop;
    }
    if (pfree) {
        WakeSo(pa);
        FreeSwXpList__FP2SWP2XP(&D_003DD490, pfree);
    }
}
} } // namespace M_func_001F4DA8

// 0x1F4E28 FreeSoContacts
namespace M_func_001F4E28 { extern "C" {
struct VTE; struct VT; struct X; struct N; struct E; struct P;
struct VTE { short d; short p; void (*pfn)(void *, void *); };
struct VT { struct VTE a[55]; };
struct X { char pad[0xC]; struct VT *pvt; };
struct N { void *contacts; struct N *next; int *head; struct X *other; };
struct E { char pad[0x60]; struct X *rootSo; struct N *contactLists; void *unknownContacts68; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0xEC - 0x10]; struct E *tree; };
struct G;
extern struct G D_003DD490;
void InvalidateSoAaox(struct P *);
void FreeSwXpList__FP2SWP2XP(struct G *, void *);
#define VF(q, n, a) (q)->pvt->a[n].pfn((char *)(q) + (q)->pvt->a[n].d, a)
// Frees every contact of the tree, notifying both sides (slot 54) first.
void FreeSoContacts(struct P *so)
{
    struct N *pn;
    if (!so->tree) return;
    InvalidateSoAaox(so);
    for (pn = so->tree->contactLists; pn; pn = pn->next) {
        struct X *q = so->tree->rootSo;
        q->pvt->a[54].pfn((char *)q + q->pvt->a[54].d, pn->other);
        pn->other->pvt->a[54].pfn((char *)pn->other + pn->other->pvt->a[54].d, so->tree->rootSo);
        FreeSwXpList__FP2SWP2XP(&D_003DD490, pn->contacts);
        *pn->head = 0;
        pn->contacts = 0;
    }
    so->pvt->a[54].pfn((char *)so + so->pvt->a[54].d, 0);
    FreeSwXpList__FP2SWP2XP(&D_003DD490, so->tree->unknownContacts68);
    so->tree->unknownContacts68 = 0;
}
} } // namespace M_func_001F4E28

// 0x1F4F10 AdjustSoXps
namespace M_func_001F4F10 { extern "C" {
struct S8; struct N; struct E; struct P;
typedef unsigned long ulong;
struct S8 { long l; } __attribute__((packed));
struct N { int contacts; struct N *next; };
struct E { char pad[0x64]; struct N *contactLists; int pad2; int zapContacts; };
struct P { char pad[0xEC]; struct E *tree; char pad2[0x148 - 0xF0]; ulong flags; };
struct G;
extern struct G D_003DD490;
extern const struct S8 D_002A8278;
void EnsureSwCallback(struct G *, struct P *, long, int);
// Slot 58 (contact adjustment after marking): when the tree has touch flags, schedules the touch-zap callback
// (HandleSoTouchZaps). (inferred)
void AdjustSoXps(struct P *so)
{
    struct E *pe = so->tree;
    struct N *pn;
    if (pe->zapContacts) {
        EnsureSwCallback(&D_003DD490, so, D_002A8278.l, 0);
    } else if (so->flags & 0x3000000) {
        for (pn = pe->contactLists; pn; pn = pn->next) {
            if (pn->contacts) {
                EnsureSwCallback(&D_003DD490, so, D_002A8278.l, 0);
                break;
            }
        }
    }
}
} } // namespace M_func_001F4F10

// 0x1F4FA8 IgnoreSoSlot59
// Slot 59: empty default.
extern "C" void IgnoreSoSlot59(void)
{
}

// 0x1F4FB0 AdjustSoNewXp
// Slot 55: new-contact setup hook; empty default (STEP overrides it).
extern "C" void AdjustSoNewXp(void)
{
}

// 0x1F4FB8 IgnoreSoSlot56
// Slot 56: empty default.
extern "C" void IgnoreSoSlot56(void)
{
}

// 0x1F4FC0 AdjustSoXpVelocity
// Slot 57: contact-velocity hook; empty default (STEP overrides it).
extern "C" void AdjustSoXpVelocity(void)
{
}

// 0x1F4FC8 ShouldSoIgnoreIntersection
namespace M_FIgnoreSoIntersection { extern "C" {
struct VTE; struct VT; struct X;
typedef unsigned long ulong;
struct VTE { short d; short p; int (*pfn)(void *); };
struct VT { struct VTE a[63]; };
struct X { char pad[0xC]; struct VT *pvt; char pad2[0x148 - 0x10]; ulong fl; };
#define VF(p) (p)->pvt->a[62].pfn((char *)(p) + (p)->pvt->a[62].d)
// Slot 61: returns 1 when contacts between pa and pb are not needed: pb is fixed and pa is fixed too or ignores
// fixed objects (+0x148 bit 26). Sly 1 name: FIgnoreSoIntersection.
int ShouldSoIgnoreIntersection(struct X *pa, struct X *pb)
{
    if (((pa->fl & 0x4000000) || VF(pa)) && VF(pb)) return 1;
    return 0;
}
} } // namespace M_FIgnoreSoIntersection

// 0x1F5040 IsSoFixed
/* Whether the object is fixed: no physics, or both constraints of kind 3. */
// Slot 62: returns 1 when the object has no physics record or both its XFs are fixed (mode 3).
extern "C" int IsSoFixed(SO *so)
{
    SOPHYS *psophys;
    int nKind;
    int f;
    psophys = so->physics;
    f = 0;
    if ((psophys == 0) || ((nKind = psophys->constrForce.kind, nKind == 3) && (psophys->constrTorque.kind == nKind))) {
        f = 1;
    }
    return f;
}

// 0x1F5078 WakeSo
/* Wakes the object (LO slot 47 with 4) and unsettles its two physics transforms. */
// Wakes an object in the broad phase: sets the awake flag (4) and un-settles its position and rotation slots.
// (inferred)
extern "C" void WakeSo(SO *so)
{
    if (so->physicsTree != 0) {
        ((void (*)(void *, int))PVTE(so)[47].pfn)((char *)so + PVTE(so)[47].d, 4);
        if (so->physics != 0) {
            UnsettleXfSlot(so, so->unknown58);
            UnsettleXfSlot(so, so->unknown5C);
        }
    }
}

// 0x1F50E0 UpdateSoTreeBounds
namespace M_UpdateSoTreeBounds {
struct MATRIX4; struct VECTOR; struct XF; struct GEOM; struct DLE; struct SOPART; struct SOTREE; struct SO; struct SW; struct VU_FLOAT;
// UpdateSoTreeBounds (0x1F50E0): SO virtual 48 - rebuild the bounds of the SO's tree record (SO+0xEC).
// Pass 1, the box of every visible collision part: UpdateSoPartGeom (0x1F38A8) first, then a sphere part adds its
// XF position -/+ radius * D_002A2DE0, a mesh part its world points, and a part whose geometry is stale
// (meshStamp != worldStamp) also its local points from unknown1C on, moved by the part's matrix.
// Pass 2, the sphere: centre = (min + max) * 0.5, radius = the farthest sphere edge / point from it.
// Then the world's +0x3CCC flag is set and 0x1F4BA8 is called.
// MATCH (T1583), no register pins. What it took (details in notes/1F0000.md):
// - ONE function-scope `int i` for all four point loops, each condition re-reading geom.cpos from memory;
// - the box loops go through ExtendBox(qword &, qword &, const qword &): the address-taken min/max make cse1's
//   cse_around_loop give up (stores to them), so the count copies come from gcse PRE as in the game, and loop.c
//   then leaves both flag masks in the part loop (ori/dsll32 at every test);
// - the radius code takes the centre as `const qword &` (gives the game's vmove copies of the centre), the sphere
//   position is read as `*(qword *)mat[3]` (aliases the VU_FLOAT home store, so it is read twice);
// - the transformed local points use GDistanceFused (vsub inside the dot-product asm, the 1.0 output first).
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct MATRIX4 {float mat[4][4];};
struct VECTOR {float x,y,z,w;};
struct XF {char pad[0x40];MATRIX4 worldMatrix;};
struct GEOM {int numPositions;VECTOR *positions;int numSurfaces;void *surfaces;int numEdges;void *edges;};
struct SOPART;
struct DLE {SOPART *next,*prev;};
struct SOPART
{
    /* 0x00 */ XF *transform;
    /* 0x04 */ void *owner;
    /* 0x08 */ float radius;          // sphere radius (unknownFlag10B parts)
    /* 0x0c */ int oid;
    /* 0x10 */ void *plvo;
    /* 0x14 */ short damage;
    /* 0x16 */ ulong unknownFlag10A:1;
               ulong unknownFlag10B:1; // the part is a sphere around its XF's position
               ulong isHidden:1;
               ulong unknownBits10A:13;
    /* 0x18 */ unsigned zapKindFlags;
    /* 0x1c */ int unknown1C;
    /* 0x20 */ MATRIX4 matWorld;
    /* 0x60 */ DLE dle;
    /* 0x68 */ char bspc[0x10];
    /* 0x78 */ GEOM meshLocal;
    /* 0x90 */ GEOM meshWorld;
    /* 0xa8 */ int meshStamp;
    /* 0xac */ int worldStamp;
};
struct SOTREE {qword boundsMin,boundsMax,boundsCenter;float boundsRadius;SOPART *ppartFirst;};
struct SO {char pad[0xEC];SOTREE *physicsTree;};
struct SW {char pad[0x3CCC];int fUnk3CCC;};
struct VU_FLOAT {qword qw;};
extern "C" { extern qword D_002A2DE0,D_002A2DF0,D_002A2E00; }
extern "C" { extern SW D_003DD490; }
extern "C" void UpdateSoPartGeom(SOPART *);
extern "C" void ClearSoTreeBoundsDirty(SO *);

// Broadcast a float into a VU0 register.
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "r"(g));
    return v;
}
// qwBase + qwVec * g
static inline qword QwAddScaled(qword qwBase, qword qwVec, VU_FLOAT g)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(qwBase), "j"(qwVec), "j"(g.qw));
    return qw;
}
// Return qwBase + qwVec * g.
static inline qword QwAddScaledF(qword qwBase, qword qwVec, float g)
{
    return QwAddScaled(qwBase, qwVec, VuFloat(g));
}
// Transform point qwPos by the 4x4 matrix pmat.
static inline qword QwTransformPos(MATRIX4 *pmat, qword qwPos)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qwResult)
            : "j"(*(qword *)pmat->mat[0]), "j"(*(qword *)pmat->mat[1]), "j"(*(qword *)pmat->mat[2]), "j"(*(qword *)pmat->mat[3]), "j"(qwPos));
    return qwResult;
}
// |v| (x, y, z)
static inline float GLength(qword v)
{
    qword qw;
    qword vfOne;
    float g;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(qw), "=j"(vfOne) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    __asm__("qmfc2 %0, %1" : "=r"(g) : "j"(qw));
    return g;
}
// Distance between qwA and qwB.
static inline float GDistance(const qword &qwA, const qword &qwB)
{
    return GLength(VuSub(qwA, qwB));
}
// |a - b| with the subtraction inside the dot-product asm (the transformed local points)
static inline float GDistanceFused(qword qwA, qword qwB)
{
    qword qw;
    qword vfOne;
    float g;
    __asm__("vsub.xyzw %1, %2, %3\n\tvmul.xyz %1, %1, %1\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z"
            : "=j"(vfOne), "=j"(qw) : "j"(qwA), "j"(qwB));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    __asm__("qmfc2 %0, %1" : "=r"(g) : "j"(qw));
    return g;
}
// Grow the box [qwMin, qwMax] to contain qwPos.
static inline void ExtendBox(qword &qwMin, qword &qwMax, const qword &qwPos)
{
    qwMin = VuMin(qwMin, qwPos);
    qwMax = VuMax(qwMax, qwPos);
}

// Slot 49: rebuilds the tree's axis-aligned box and bounding sphere from its visible parts, marks the broad phase
// dirty and clears the tree's dirty flag.
extern "C" void UpdateSoTreeBounds(SO *so)
{
    qword qwMin = D_002A2DF0;
    qword qwMax = D_002A2E00;
    SOPART *ppart;
    int i;
    for (ppart = so->physicsTree->ppartFirst; ppart; ppart = ppart->dle.next)
    {
        if (ppart->isHidden)
            continue;
        UpdateSoPartGeom(ppart);
        if (ppart->unknownFlag10B)
        {
            float gRadius = ppart->radius;
            qword qwLo = QwAddScaledF(*(qword *)ppart->transform->worldMatrix.mat[3], D_002A2DE0, -gRadius);
            qwMin = VuMin(qwMin, qwLo);
            qword qwHi = QwAddScaledF(*(qword *)ppart->transform->worldMatrix.mat[3], D_002A2DE0, gRadius);
            qwMax = VuMax(qwMax, qwHi);
        }
        else
        {
            for (i = 0; i < ppart->meshWorld.numPositions; i++)
            {
                qword qwPos = *(qword *)&ppart->meshWorld.positions[i];
                ExtendBox(qwMin, qwMax, qwPos);
            }
        }
        if (ppart->meshStamp != ppart->worldStamp)
        {
            for (i = ppart->unknown1C; i < ppart->meshWorld.numPositions; i++)
            {
                qword qwPos = QwTransformPos(&ppart->matWorld, *(qword *)&ppart->meshLocal.positions[i]);
                ExtendBox(qwMin, qwMax, qwPos);
            }
        }
    }

    so->physicsTree->boundsMin = qwMin;
    so->physicsTree->boundsMax = qwMax;
    VU_FLOAT gHalf = VuFloat(0.5f);
    float sRadius = 0.0f;
    qword qwCenter;
    __asm__("vmulax.xyzw $ACC, %1, %3x\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qwCenter) : "j"(qwMin), "j"(qwMax), "j"(gHalf.qw));
    for (ppart = so->physicsTree->ppartFirst; ppart; ppart = ppart->dle.next)
    {
        if (ppart->unknownFlag10B)
        {
            float gDist = GDistance(qwCenter, *(qword *)ppart->transform->worldMatrix.mat[3]) + ppart->radius;
            if (sRadius < gDist)
                sRadius = gDist;
        }
        else
        {
            for (i = 0; i < ppart->meshWorld.numPositions; i++)
            {
                float gDist = GDistance(qwCenter, *(qword *)&ppart->meshWorld.positions[i]);
                if (sRadius < gDist)
                    sRadius = gDist;
            }
        }
        if (ppart->meshStamp != ppart->worldStamp)
        {
            for (i = ppart->unknown1C; i < ppart->meshWorld.numPositions; i++)
            {
                qword qwPos = QwTransformPos(&ppart->matWorld, *(qword *)&ppart->meshLocal.positions[i]);
                float gDist = GDistanceFused(qwCenter, qwPos);
                if (sRadius < gDist)
                    sRadius = gDist;
            }
        }
    }
    so->physicsTree->boundsCenter = qwCenter;
    so->physicsTree->boundsRadius = sRadius;
    D_003DD490.fUnk3CCC = 1;
    ClearSoTreeBoundsDirty(so);
}

typedef char layout_part_size[(sizeof(SOPART)==0xB0)?1:-1];
typedef char layout_part_matrix[((unsigned)&((SOPART*)0)->matWorld==0x20)?1:-1];
typedef char layout_part_dle[((unsigned)&((SOPART*)0)->dle==0x60)?1:-1];
typedef char layout_part_world[((unsigned)&((SOPART*)0)->meshWorld==0x90)?1:-1];
typedef char layout_part_stamp[((unsigned)&((SOPART*)0)->worldStamp==0xAC)?1:-1];
typedef char layout_tree_first[((unsigned)&((SOTREE*)0)->ppartFirst==0x34)?1:-1];
} // namespace M_UpdateSoTreeBounds

// 0x1F5480 ExtendSoTreeBounds
namespace M_ExtendSoTreeBounds { extern "C" {
struct B; struct P;
typedef int qword __attribute__((mode(TI)));
struct B { qword min; qword max; qword center; float radius; };
struct P { char pad[0xEC]; struct B *tree; };
// Grows the tree's box and radius to include a sphere (centre c, radius r).
void ExtendSoTreeBounds(struct P *so, qword c, float r)
{
    qword rr, lo, hi, d, t, one;
    float dist;
    __asm__("pextlw %0, %1, %1\n\tpextlw %0, %0, %0" : "=r"(rr) : "r"(r));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(lo) : "j"(c), "j"(rr));
    qword mn;
    __asm__("vmini.xyz %0, %1, %2" : "=j"(mn) : "j"(so->tree->min), "j"(lo));
    so->tree->min = mn;
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(hi) : "j"(c), "j"(rr));
    qword mx;
    __asm__("vmax.xyz %0, %1, %2" : "=j"(mx) : "j"(so->tree->max), "j"(hi));
    so->tree->max = mx;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(c) : "0"(c), "j"(so->tree->center));
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d), "=&j"(one) : "0"(c));
    __asm__("qmfc2.ni %0, %1" : "=r"(dist) : "j"(d));
    if (so->tree->radius < dist) so->tree->radius = dist;
}
} } // namespace M_ExtendSoTreeBounds

// 0x1F5510 GetSoConstraintAxis
namespace M_func_001F5510 { extern "C" {
struct OUT;
typedef int qword __attribute__((mode(TI)));
typedef struct { qword row[3]; } M3;
struct OUT { qword axis; int mode; };
extern int D_002C3C20[];
extern qword D_002C3BA0[];
extern qword D_002A2DD0;
// Rotate value by the 3x3 matrix (register-pinned rows).
static inline qword Transform(M3 *matrix,qword value) {
    register qword x __asm__("vf2")=matrix->row[0];
    register qword y __asm__("vf1")=matrix->row[1];
    register qword z __asm__("vf3")=matrix->row[2];
    __asm__("vmulax.xyz $ACC,%1,%3x\n\tvmadday.xyz $ACC,%2,%3y\n\tvmaddz.xyz %0,%0,%3z" : "+j"(z) : "j"(x),"j"(y),"j"(value));
    return z;
}
// Unit vector along value, or fallback when its length is too small.
static inline qword NormalizeOr(qword value,qword fallback) {
    register qword length __asm__("vf1");
    register qword one __asm__("vf2");
    register qword result __asm__("vf1");
    float scalar;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(length),"=&j"(one) : "j"(value));
    __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "=j"(length) : "j"(length));
    __asm__("mtc1 %1,%0" : "=f"(scalar) : "r"(length));
    if(scalar<0.0001f) result=fallback;
    else __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q" : "=j"(result) : "j"(length),"j"(value));
    return result;
}
// Looks up constraint i in the constraint tables: its mode, and its axis transformed by a matrix and normalised.
// (inferred)
void GetSoConstraintAxis(void *unused,M3 *matrix,int index,struct OUT *out) {
    out->mode=D_002C3C20[index];
    out->axis=NormalizeOr(Transform(matrix,D_002C3BA0[index]),D_002A2DD0);
}
} } // namespace M_func_001F5510

// 0x1F55C8 RebuildSoPhysHook
namespace M_func_001F55C8 { extern "C" {
struct VTE; struct Q; struct O;
typedef unsigned long ulong;
struct VTE { short d; short pad; void *(*pfn)(void *); };
struct O;
struct Q { char pad[0x60]; struct O *rootSo; };
struct O { char pad[0xC]; struct VTE *pvt; char pad2[0x28 - 0x10]; ulong flags; char pad3[0xEC - 0x30]; struct Q *tree; };
struct G;
extern struct G D_003DD490;
struct O *FindSoPhysRoot(struct O *);
int FindSwObjectsByClass(struct G *, int, int, struct O *, int, struct O **);
void RemoveSoParts(struct O *);
void RefreshSwObjectCollisionFilter(struct G *, struct O *);
void AddSoParts(struct O *);
void WakeSo(struct O *);
// After a physics-root change, regroups the contact trees of the objects whose physics root changed and registers
// them again. Sly 1 name.
void RebuildSoPhysHook(struct O *so)
{
    struct O *ap[64];
    int i;
    int n;
    if (((so->flags >> 60) & 1) == 0) return;
    if (FindSoPhysRoot(so) != so->tree->rootSo) {
        n = FindSwObjectsByClass(&D_003DD490, 1, 0x30, so, 0x40, ap);
        for (i = 0; i < n; i++) {
            {
                struct O *po = ap[i];
                if (po->tree) {
                    struct O *q = FindSoPhysRoot(po);
                    if (q != po->tree->rootSo) {
                        RemoveSoParts(po);
                        if (!q->tree) {
                            q->tree = (__typeof__(q->tree))q->pvt[53].pfn((char *)q + q->pvt[53].d);
                            RefreshSwObjectCollisionFilter(&D_003DD490, q);
                        }
                        po->tree = q->tree;
                        AddSoParts(po);
                    }
                }
            }
        }
    }
    RefreshSwObjectCollisionFilter(&D_003DD490, so);
    WakeSo(so);
}
} } // namespace M_func_001F55C8

// 0x1F56F8 NewSoTree
// Slot 53: creates the object's tree record in the broad phase (AddObjectToBroadPhase). (inferred)
extern "C" void NewSoTree(SO *so)
{
    AddObjectToBroadPhase(&D_003DD490, so);
}

// 0x1F5720 SetSoCnstrForce
// Sets the translation constraint from a CNSTR: makes the position XF dynamic if needed, stores axis and mode in the
// physics record and rebuilds the physics hook.
extern "C" void SetSoCnstrForce(SO *so, CONSTR *pconstr)
{
    XF *pxf;
    if (so->physics != 0) {
        pxf = so->unknown58;
        if ((pxf->firstActuator == 0) && (pxf->role != 2)) {
            SetXfRole(so, pxf, 1);
        }
        GetSoConstraintAxis(so, pxf, pconstr, &so->physics->constrForce);
        RebuildSoPhysHook(so);
        if (so->physics->constrForce.kind != 3) {
            SetSoBit35Hierarchy(so, 0);
        }
    }
}

// 0x1F57C0 SetSoCnstrTorque
// Sets the rotation constraint from a CNSTR (the rotational twin of SetSoCnstrForce).
extern "C" void SetSoCnstrTorque(SO *so, CONSTR *pconstr)
{
    XF *pxf;
    if (so->physics != 0) {
        pxf = so->unknown5C;
        if ((pxf->firstActuator == 0) && (pxf->role != 2)) {
            SetXfRole(so, pxf, 1);
        }
        GetSoConstraintAxis(so, pxf, pconstr, &so->physics->constrTorque);
        RebuildSoPhysHook(so);
        if (so->physics->constrTorque.kind != 3) {
            SetSoBit35Hierarchy(so, 0);
        }
    }
}

// 0x1F5860 GetSoGravity
namespace M_GetSoDvGravity { extern "C" {
struct T; struct S;
typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct T { char pad[0x80]; qword gravity; };
struct S { char pad[0xE8]; struct T *physics; };
// The object's gravity vector, or zero without a physics record.
qword GetSoGravity(struct S *p) { if (!p->physics) return D_002A2DD0; return p->physics->gravity; }
} } // namespace M_GetSoDvGravity

// 0x1F5880 SetSoGravity
namespace M_SetSoDvGravity { extern "C" {
struct O; struct E; struct P;
typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x90]; int firstAct; char pad2[4]; short role; };
struct E { char pad[0x80]; qword gravity; };
struct P { char pad[0x58]; struct O *positionXf; char pad2[0xE8 - 0x5C]; struct E *physics; };
void SetXfRole(struct P *, struct O *, int);
void WakeSo(struct P *);
// Sets the object's gravity vector (making the position XF dynamic if needed) and invalidates its box.
void SetSoGravity(struct P *so, qword q)
{
    if (so->physics) {
        struct O *po = so->positionXf;
        if (po->firstAct == 0 && po->role != 2) SetXfRole(so, po, 1);
        so->physics->gravity = q;
        WakeSo(so);
    }
}
} } // namespace M_SetSoDvGravity

// 0x1F58F0 GetSoCameraDefaults
namespace M_GetSoCpdefi { extern "C" {
struct V; struct O; struct P; struct Out; 
typedef int qword __attribute__((mode(TI)));
struct V { float x, y, z, w; float pad[8]; };
struct O { char pad[0x40]; char rotation[0x30]; qword position; };
struct P { char pad[0x60]; struct O *rootTransform; };
struct Out { int flags; int pad[3]; qword position; qword velocity; float heading; float adjustment; float lastMoveTime; int unknown3C; };
extern struct CLOCK D_002DDED0;
qword GetXfVelocity(struct O *, int, qword);
void TiltMatUpright(void *, int, struct V *);
float atan2f(float, float);
// Slot 50: the camera's default info for this object: base position and velocity, home heading from the upright
// matrix, adjustment 100, no flags, last-move time now.
void GetSoCameraDefaults(struct P *so, struct Out *po)
{
    struct O *o = so->rootTransform;
    struct V v;
    void *ps;
    qword q = o->position;
    po->position = q;
    ps = o->rotation;
    po->velocity = GetXfVelocity(o, 0, q);
    TiltMatUpright(ps, 0, &v);
    po->heading = atan2f(-v.y, -v.x); po->adjustment = 100.0f; po->flags = 0; po->lastMoveTime = D_002DDED0.time; po->unknown3C = 0;
}
} } // namespace M_GetSoCpdefi

// 0x1F5988 OnSoContactFreed
// Slot 54: a contact of the object is being freed; empty default (STEP overrides it).
extern "C" void OnSoContactFreed(void)
{
}

// 0x1F5990 AddSoCustomXps
// Slot 60: adds object-specific contacts; empty default (STEP's ground probe overrides it).
extern "C" void AddSoCustomXps(void)
{
}

// 0x1F5998 GetSoExtraPartCount
// Slot 63 (so.h GetExtraPartCount): the number of extra parts the class adds; 0 for a plain SO (STEP adds its phys
// part and answers 1) (inferred from the slot name).
extern "C" int GetSoExtraPartCount(void)
{
    return 0;
}

// 0x1F59A0 AccelSoTowardMatBounded
namespace M_func_001F59A0 {
union U; struct M3; struct OWNER; struct P; struct COEF; 
// AccelSoTowardMatBounded: angular braking and bounded angular-velocity correction.
// coef a = top angular speed, (a-b)/c = angular acceleration, a*a/(2*acceleration) = stopping angle.
// Caps an overspeed, turns the owner toward `orientation` (expressed in its parent space) with a desired
// speed taken from the predicted remaining angle, and adds a force change bounded by acceleration*dt.
// Matched from the codex/lilly drafts (T1623). Notes: notes/1F0000.md.
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
// 3x3 matrix of U rows with copy constructor and assignment.
struct M3 {U rows[3];M3(){} M3(const M3&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];} M3&operator=(const M3&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];return *this;}};
struct SPACE;
struct OWNER {M3 matrix;char pad30[0x70];SPACE *parent;char padA4[0xC];qword velocity;char padC0[0x30];qword force;};
struct P {char pad5C[0x5C];OWNER *owner;};
struct COEF {float a,b,c;};
struct CLOCK {float time,deltaTime;};
extern "C" { extern CLOCK D_002DDED0; }
extern "C" M3 *ConvertXfMat(M3 *,SPACE *,SPACE *,const M3 *);
// The orientation converted from parent space to world space (ConvertXfMat).
static inline M3 BuildFrame(SPACE*parent,const M3*orientation) return result {ConvertXfMat(&result,0,parent,orientation);}
extern "C" void NormalizeRotateMatrix3__FP7MATRIX3(M3 *);
extern "C" void MulMatrix3Matrix3(M3 *,const M3 *,const M3 *);
extern "C" void DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(const M3 *,float *,qword *);
extern "C" float ClampAbs__Fff(float,float);
// Return a + b.
static inline qword Add(qword a,qword b){__asm__("vadd.xyzw %0,%0,%1":"+j"(a):"j"(b));return a;}
// volatile store: the game's later loads (dt, velocity, the epilogue) wait for these sqc2s
static inline void StoreQ(qword v,U *out){__asm__ __volatile__("sqc2 %1,%0":"=m"(out->q):"j"(v));}
// Broadcast f into a VU0 register and store it to *out.
static inline qword Scalar(float f,U *out){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));__asm__("sqc2 %1,%0":"=m"(out->q):"j"(q));return q;}
// Return source scaled by k.x.
static inline qword Scale(qword source,qword k){qword v=source;__asm__("vmulx.xyzw %0,%0,%1x":"+j"(v):"j"(k));return v;}
// Return v scaled by k.x, computed in k's register.
static inline qword ScaleFactor(qword v,qword k){__asm__("vmulx.xyzw %0,%1,%0x":"+j"(k):"j"(v));return k;}
// Return v scaled by k.x into a fresh register.
static inline qword ScaleNew(qword v,qword k){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=&j"(r):"j"(v),"j"(k));return r;}
// Return a + b * k.x.
static inline qword AddScaled(qword a,qword b,qword k){__asm__("vadda.xyzw ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x":"+j"(a):"j"(b),"j"(k));return a;}
// Dot product of the xyz parts of value and normal (x lane).
static inline qword DotQ(qword value,qword normal){qword d=value,b=normal,one;__asm__("vmul.xyz %0,%0,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(d),"=&j"(one):"j"(b));return d;}
// x of a qword: the compiler emits the qmfc2, the asm the mtc1 (an asm qmfc2 issues it before the 0.5 load)
static inline float Length(qword a){qword d,one;float f;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(a));__asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=j"(d):"0"(d));__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(d));return f;}

// Accelerates the rotation toward a target orientation with angular braking and a bounded correction (top speed
// and acceleration from coef). Used for steering (STEP mode 0).
extern "C" void AccelSoTowardMatBounded(P *so,const M3 *orientation,const COEF *c)
{
    M3 frame,relative;
    U axis,wanted,change,predicted;
    float angle;
    float maximum=c->a;
    float acceleration=(maximum-c->b)/c->c;
    float square=maximum*maximum;
    float stopAngle;
    OWNER *owner=so->owner;
    // one variable for the speed and, later, the speed along the turn axis (target: f4 for both)
    float speed=Length(owner->velocity);
    stopAngle=square/(acceleration+acceleration);
    float desired,remaining,length,gain;
    CLOCK *clock;
    if(maximum<speed){
        U scalar;float correction=((maximum-speed)*2.5f)/speed;
        change.q=ScaleFactor(owner->velocity,Scalar(correction,&scalar));
        owner->force=Add(owner->force,change.q);
    }
    frame=BuildFrame(owner->parent,orientation);
    NormalizeRotateMatrix3__FP7MATRIX3(&frame);
    {M3 out;MulMatrix3Matrix3(&out,&owner->matrix,&frame);relative=out;}
    DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(&relative,&angle,&axis.q);
    desired=ClampAbs__Fff((angle/stopAngle)*maximum,maximum);
    speed=QwordX(DotQ(owner->velocity,axis.q));
    clock=&D_002DDED0;
    float half=clock->deltaTime*0.5f;
    remaining=angle-(speed+desired)*half;
    if(remaining<stopAngle)desired=ClampAbs__Fff((remaining/stopAngle)*maximum,maximum);
    U scalar;
    qword w;
    {qword k=Scalar(desired,&scalar);w=Scale(axis.q,k);StoreQ(w,&wanted);}
    {qword v=owner->velocity;qword k=Scalar(clock->deltaTime,&scalar);predicted.q=AddScaled(v,owner->force,k);}
    qword cv=VuSub(w,predicted.q);
    change.q=cv;
    length=Length(cv);
    if(length<acceleration*clock->deltaTime)gain=1.0f/clock->deltaTime;
    else gain=acceleration/length;
    StoreQ(ScaleNew(cv,Scalar(gain,&scalar)),&change);
    owner->force=Add(owner->force,change.q);
}

typedef char check_matrix_size[(sizeof(M3)==0x30)?1:-1];
typedef char check_owner_parent[((unsigned)&((OWNER*)0)->parent==0xA0)?1:-1];
typedef char check_owner_velocity[((unsigned)&((OWNER*)0)->velocity==0xB0)?1:-1];
typedef char check_owner_force[((unsigned)&((OWNER*)0)->force==0xF0)?1:-1];
typedef char check_object_owner[((unsigned)&((P*)0)->owner==0x5C)?1:-1];
typedef char check_clock_dt[((unsigned)&((CLOCK*)0)->deltaTime==4)?1:-1];
} // namespace M_func_001F59A0

// 0x1F5C30 AccelSoTowardMatSpring
namespace M_AccelSoTowardMatSpring {
union U; struct M3; struct OWNER; struct P; struct COEF; 
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
// 3x3 matrix of U rows with copy constructor and assignment.
struct M3 {U rows[3];M3(){} M3(const M3&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];} M3&operator=(const M3&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];return *this;}};
struct OWNER {M3 matrix;char pad30[0x70];OWNER *parent;char padA4[0xC];qword velocity;char padC0[0x30];qword force;};
struct P {char pad5C[0x5C];OWNER *owner;};
struct COEF {float a,b,c;};
struct CLOCK {float time,deltaTime;};
extern "C" { extern CLOCK D_002DDED0; }
extern "C" M3 ConvertXfMat(OWNER *,OWNER *,const M3 *);
extern "C" void NormalizeRotateMatrix3__FP7MATRIX3(M3 *);
extern "C" M3 MulMatrix3Matrix3(const M3 *,const M3 *);
extern "C" void DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(const M3 *,float *,qword *);
extern "C" qword ConvertXfVec(OWNER *,OWNER *,qword);
// Length of the xyz part of a as a float.
static inline float Length(qword a){qword d,one;float f;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a));
__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(d):"0"(d));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(d));return f;}
// Return old + v * s.x.
static inline qword AddScaled(qword old,qword v,qword s){__asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x":"+j"(old):"j"(v),"j"(s));return old;}
// Spring torque toward a target orientation (quadratic CLQ on the angle) and toward a target angular velocity
// (clamped to 1/dt).
extern "C" void AccelSoTowardMatSpring(P*so,const M3*orientation,const COEF*c,qword velocity,const COEF*d)
{
    OWNER *owner=so->owner;
    if(c){
        M3 frame,relative;
        U axis;
        float angle;
        frame=ConvertXfMat(0,owner->parent,orientation);
        NormalizeRotateMatrix3__FP7MATRIX3(&frame);
        relative=MulMatrix3Matrix3(&owner->matrix,&frame);
        DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(&relative,&angle,&axis.q);
        if(0.0001f<angle){
            float gain=c->c*angle*angle+c->b*angle+c->a;
            U scalar;
            qword k=VuFromFloat<qword>(gain);
            {qword result=AddScaled(owner->force,axis.q,k);scalar.q=k;owner->force=result;}
        }
    }
    if(d){
        qword transformed=ConvertXfVec(0,owner->parent,velocity);
        qword direction=VuSub(transformed,owner->velocity);
        float length=Length(direction);
        if(0.0001f<length){
            float gain=(d->c*length+d->b)+d->a/length;
            float limit=1.0f/D_002DDED0.deltaTime;
            U scalar;
            qword k;
            if(limit<gain)gain=limit;
            k=VuFromFloat<qword>(gain);
            {qword result=AddScaled(owner->force,direction,k);scalar.q=k;owner->force=result;}
        }
    }
}
} // namespace M_AccelSoTowardMatSpring

// 0x1F5E10 AccelSoTowardPosSpring
namespace M_AccelSoTowardPosSpring { extern "C" {
struct COEF; struct VE; struct OWNER; struct S; 
typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];} V;
struct COEF {float a,b,c;};
struct VE {short d,pad;qword(*fn)(void*);};
struct OWNER {char pad[0xA0];void *parent;char padA4[0x4C];qword force;};
struct S {char pad[0xC];struct VE *vt;char pad10[0x48];struct OWNER *owner;};
struct CLOCK {float time,deltaTime;};
extern struct CLOCK D_002DDED0;
qword GetLoWorldPos(struct S*);
qword ConvertXfVec(void*,void*,qword);
// Return x - y (vf1/vf2/vf3).
static inline qword Sub(qword x,qword y){register qword a __asm__("vf1")=x,b __asm__("vf2")=y,r __asm__("vf3");__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
// Length of the xyz part of value as a float.
static inline float Length(qword value){register qword a __asm__("vf3")=value,r __asm__("vf1"),one __asm__("vf2");float f;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(r),"=&j"(one):"j"(a));
__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(r):"0"(r));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(r));return f;}
// Load a 128-bit vector into a GPR ($2) with lq.
static inline qword LoadVector(const V *value){register qword result asm("v0");__asm__("lq %0,%1":"=r"(result):"m"(value->q));return result;}
// Move a float's bits into a GPR qword ($2).
static inline qword Scalar(float f){register qword r __asm__("$2");__asm__("mfc1 %0,%1":"=r"(r):"f"(f));return r;}
// Return old + direction * factor.x.
static inline qword AddScaled(qword old,qword direction,qword factor){register qword a __asm__("vf2")=old,b __asm__("vf4")=direction,scale __asm__("vf3")=factor,r __asm__("vf1");
__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=&j"(r):"j"(a),"j"(b),"j"(scale));return r;}
// Spring force toward a target position (CLQ on the distance) and toward a target velocity (clamped to 1/dt).
void AccelSoTowardPosSpring(struct S*input,qword position,struct COEF*positionCoefficients,qword velocity,struct COEF*velocityCoefficients)
{
 V argument,point,old1,scale1,speed,old2,scale2;
 register struct S *p __asm__("$17")=input;
 register struct COEF *c __asm__("$16")=positionCoefficients,*d __asm__("$18")=velocityCoefficients;
 register struct OWNER *owner __asm__("$19");
 point.q=position;owner=p->owner;speed.q=velocity;
 if(c){
   qword difference=Sub(point.q,GetLoWorldPos(p));
   float distance=Length(difference);
   if(1.0e-4f<distance){
     float inverse=c->a/distance;
     float value=c->c*distance+c->b;
     register void *parent __asm__("$5");
     old1.q=owner->force;parent=owner->parent;value+=inverse;
     argument.q=Scalar(value);scale1=argument;
     owner->force=AddScaled(old1.q,ConvertXfVec(0,parent,difference),scale1.q);
   }
 }
 if(d){
   qword difference=Sub(speed.q,p->vt[33].fn((char*)p+p->vt[33].d));
   float distance=Length(difference);
   if(1.0e-4f<distance){
     register void *parent asm("a1");
     float value=(d->c*distance+d->b)+d->a/distance;
     float limit=1.0f/D_002DDED0.deltaTime;
     if(limit<value)value=limit;
     scale2.q=Scalar(value);
     {register qword oldValue asm("v0")=owner->force;old2.q=oldValue;}parent=owner->parent;argument.q=LoadVector(&scale2);
     owner->force=AddScaled(old2.q,ConvertXfVec(0,parent,difference),scale2.q);
   }
 }
}
} } // namespace M_AccelSoTowardPosSpring

// 0x1F5FF8 FindSoGlob
namespace M_func_001F5FF8 { extern "C" {
struct E; struct S;
struct E { int oid; char pad[0x24]; };
struct S { char pad[0xB8]; int numGlobs; int pad2; struct E *globs; };
// Finds a glob in the object's glob set by oid.
struct E *FindSoGlob(struct S *so, int oid)
{
    int i;
    for (i = 0; i < so->numGlobs; i++) { struct E *pe = &so->globs[i]; if (pe->oid == oid) return pe; }
    return 0;
}
} } // namespace M_func_001F5FF8

// 0x1F6030 FindSoPart
namespace M_PsopartFindSo { extern "C" {
struct E; struct S;
struct E { int a[3]; int oid; char pad[0xA0]; };
struct S { char pad[0xE0]; int numParts; struct E *parts; };
// Finds a part by oid (Sly 1 PsopartFindSo).
struct E *FindSoPart(struct S *so, int oid)
{
    int i;
    for (i = 0; i < so->numParts; i++) { struct E *pe = &so->parts[i]; if (pe->oid == oid) return pe; }
    return 0;
}
} } // namespace M_PsopartFindSo

// 0x1F6068 CloneSo
// CloneSo: ported from Lilly's personal Sly 2 decomp (src/P2/so.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002C44F0 (g_xfWorld)
// 0x001F6068 Clone__2SOP2LO  (MATCHES Sly 2, T1075)
// Virtual 40. LO's clone (the byte copy) took the base's child list, tree record and removed-children mark along: the clone's own
// are put back. Then everything the SO owns is copied for the clone and every pointer into the base's XFs is re-pointed at the
// clone's (CloneXfRef): the XF block (each XF's parent, the record its slot holds in roles 4 / 13 / 15 / 16, its actuators - cloned
// in their order of priority by PactNewClone + AddXfAct), the three XFs of the object (position, rotation, root), the
// model (CloneGlobset), the collision parts (geometry, BSP and the per-geometry arrays) and the physics record with its region
// entry. A clone in the world joins the contact list, and it gets a bounds record of its own (EnsureSoBounds).


// True when the object is in the world.
inline int FIsLoInWorld(LO *plo)
{
    return plo->isInWorld != 0;
}










extern XF D_002C44F0;
void AddSoParts(SO *pso) __asm__("AddSoParts");
void CloneGlobset(GLOBSET *pglobset, SO *pso, LO *ploBase) __asm__("CloneGlobset");
void CloneBspc(BSPC *pbspcBase, GEOM *pgeomBase, BSPC *pbspc, GEOM *pgeom) __asm__("CloneBspc");
// Allocate cb bytes from the world heap and copy pvBase into them.
inline void *PvAllocSwCopy(int cb, void *pvBase)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = 0x10;
    return PvAllocSwCopyImpl(pswalloc, pvBase);
}
// Allocate cb bytes from the frame stack.
inline void *PvAllocStack(int cb)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = 0x10;
    return PvAllocStackImpl(pswalloc);
}
// Slot 41 (Clone): copies the object's state from another SO (Lilly port of CloneSo).
void SO::Clone(LO *ploBase)
{
    SO *psoBase = (SO *)ploBase;
    SOTREE *ptreeSave = physicsTree;
    LO *ploRemovedFirstSave = ploRemovedFirst;
    DL dlChildSave = children;
    int ixf = 0;
    LO::Clone(ploBase);
    physicsTree = ptreeSave;
    children = dlChildSave;
    ploRemovedFirst = ploRemovedFirstSave;
    psobounds = 0;
    transforms = (XF *)PvAllocSwCopy(numTransforms * sizeof(XF), transforms);
    for (ixf = 0; ixf < numTransforms; ixf++)
    {
        XF *pxf = &transforms[ixf];
        CloneXfRef(ploBase, &pxf->parent);
        switch (pxf->role)
        {
        case 4:
            CloneXfRef(ploBase, &pxf->reference);
            break;
        case 13:
        {
            IKLIMB *plimb = (IKLIMB *)PvAllocSwCopy(sizeof(IKLIMB), pxf->reference);
            CloneXfRef(ploBase, &plimb->pxfGoal);
            CloneXfRef(ploBase, &plimb->pxfS);
            CloneXfRef(ploBase, &plimb->pxfE);
            CloneXfRef(ploBase, &plimb->pxfEnd);
            plimb->pxfS->reference = (XF *)plimb;
            plimb->pxfE->reference = (XF *)plimb;
            break;
        }
        case 15:
        {
            XFLINK *pxflink = (XFLINK *)PvAllocSwCopy(sizeof(XFLINK), pxf->reference);
            pxf->reference = (XF *)pxflink;
            if (pxflink->pxfFrom != &D_002C44F0)
            {
                CloneXfRef(ploBase, &pxflink->pxfFrom);
            }
            CloneXfRef(ploBase, &pxflink->pxfTo);
            break;
        }
        case 16:
        {
            CHAINLINK *plink = (CHAINLINK *)pxf->reference;
            CHAIN *pchain = plink->pchain;
            if (plink == pchain->alink)
            {
                pchain = (CHAIN *)PvAllocSwCopy(sizeof(CHAIN), pchain);
                pchain->alink = (CHAINLINK *)PvAllocSwCopy(pchain->clink * sizeof(CHAINLINK), pchain->alink);
                for (int ilink = 0; ilink < pchain->clink; ilink++)
                {
                    CHAINLINK *plinkNew = &pchain->alink[ilink];
                    plinkNew->pchain = pchain;
                    CloneXfRef(ploBase, &plinkNew->pxf);
                    plinkNew->pxf->reference = (XF *)plinkNew;
                }
            }
            break;
        }
        }
        if (pxf->firstActuator)
        {
            int cact = 0;
            ACT *pact = pxf->firstActuator;
            do
            {
                pact = pact->pactNext;
                cact++;
            } while (pact);
            STACKSCOPE scope;
            ACT **apact = (ACT **)PvAllocStack(cact * sizeof(ACT *));
            int iact = 0;
            for (pact = pxf->firstActuator; pact; pact = pact->pactNext)
            {
                apact[iact++] = pact;
            }
            pxf->firstActuator = 0;
            for (iact = cact - 1; iact >= 0; iact--)
            {
                ACT *pactNew = PactNewClone(apact[iact], this, pxf);
                AddXfAct(this, pxf, pactNew);
            }
        }
    }
    CloneXfRef(ploBase, &unknown58);
    CloneXfRef(ploBase, &unknown5C);
    CloneXfRef(ploBase, &rootTransform);
    CloneGlobset(&model, this, ploBase);
    int cpartExtra = GetExtraPartCount();
    parts = (SOPART *)PvAllocSwCopy((numParts + cpartExtra) * sizeof(SOPART), parts);
    for (int ipart = 0; ipart < numParts; ipart++)
    {
        SOPART *ppart = &parts[ipart];
        CloneXfRef(ploBase, &ppart->transform);
        ppart->owner = this;
        ClearDle(&ppart->dle);
        GEOM geom;
        CloneGeom(&ppart->meshWorld, 0, &geom);
        ppart->meshWorld = geom;
        SOPART *ppartBase = &psoBase->parts[ipart];
        CloneBspc(&ppartBase->bspc, &ppartBase->meshWorld, &ppart->bspc, &ppart->meshWorld);
        if (ppart->plvo)
        {
            int cb = sizeof(LVO);
            ppart->plvo = (LVO *)PvAllocSwCopy(cb, ppart->plvo);
            ppart->plvo->mpiposvLocal = (VECTOR *)PvAllocSwCopy(ppart->meshLocal.numPositions * 0x10, ppart->plvo->mpiposvLocal);
            ppart->plvo->mpiposdvLocal = (VECTOR *)PvAllocSwCopy(ppart->meshLocal.numPositions * 0x10, ppart->plvo->mpiposdvLocal);
            ppart->plvo->mpisurfsurfx = (SURFX *)PvAllocSwCopy(ppart->meshLocal.numSurfaces * 0xC, ppart->plvo->mpisurfsurfx);
        }
    }
    if (physics)
    {
        physics = (SOPHYS *)PvAllocSwCopy(sizeof(SOPHYS), physics);
        if (physics->pregent)
        {
            physics->pregent = (REGENT *)PvAllocSwCopy(sizeof(REGENT), physics->pregent);
            physics->pregent->pso = this;
        }
    }
    if (FIsLoInWorld(this))
    {
        AddSoParts(this);
    }
    EnsureSoBounds(this);
}

// 0x1F66F0 CloneSoHierarchy
namespace M_CloneSoHierarchy { extern "C" {
struct VTE; struct VT; struct A; struct L;
typedef unsigned long ulong;
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[42]; };
struct A { char pad[0xC]; struct VT *pvt; };
struct L { char pad[0x24]; struct L *next; ulong flags; char pad2[0x64 - 0x30]; struct L *firstChild; char pad3[0x70 - 0x68]; struct L *firstRemovedChild; };
void CloneLoWithParent__FP2LOP2SO(struct L *, struct A *);
// Slot 40: clones the object (slot 41) and then every child that was not removed.
void CloneSoHierarchy(struct A *pa, struct L *so)
{
    struct L *q;
    pa->pvt->a[41].pfn((char *)pa + pa->pvt->a[41].d);
    for (q = so->firstChild; q != so->firstRemovedChild; q = q->next)
        if (!(q->flags & (1L << 57))) CloneLoWithParent__FP2LOP2SO(q, pa);
}
} } // namespace M_CloneSoHierarchy

// 0x1F6788 FindAsegaByOid
namespace M_func_001F6788 { extern "C" {
struct B; struct N;
struct B { char pad[0x18]; int oid; };
struct N { char pad[0x14]; struct B *owner; int oid; char pad2[8]; struct N *next; };
// Searches the world's ASEGA list for an entry with the given oid (and owner oid m unless m is -1). (inferred)
struct N *FindAsegaByOid(int n, int m)
{
    struct N *p;
    int unused[4];
    for (p = (*(struct N * *)&g_sw.dlAsega); p; p = p->next) {
        if (p->oid == n) {
            if (m == -1 || p->owner->oid == m)
                return p;
        }
    }
    return 0;
}
} } // namespace M_func_001F6788

// 0x1F67E0 FindLiveSma
namespace M_func_001F67E0 { extern "C" {
struct B; struct N;
struct B { char pad[0x18]; int oid; };
struct N { char pad[0x1C]; struct N *next; int pad2; struct B *owner; int oid; };
// The same search over the world's list of live state-machine instances (SMA). (inferred)
struct N *FindLiveSma(int n, int m)
{
    struct N *p;
    int unused[4];
    for (p = (*(struct N * *)&g_sw.dlSma); p; p = p->next) {
        if (p->oid == n) {
            if (m == -1 || p->owner->oid == m)
                return p;
        }
    }
    return 0;
}
} } // namespace M_func_001F67E0

// 0x1F6838 GetSoTreeFlag2
namespace M_func_001F6838 { extern "C" {
struct T; struct S;
struct T { char pad[0x70]; ulong flags; };
struct S { char pad[0xEC]; struct T *tree; };
// Bit 2 of the tree flags; 1 when the object has no tree. (inferred)
int GetSoTreeFlag2(struct S *p) { if (!p->tree) return 1; return (int)((long)(p->tree->flags << 30) >> 32) & 1; }
} } // namespace M_func_001F6838

// 0x1F6860 SetSoTreeFlag2
// Sets bit 2 of the tree flags and frees the tree's contacts. (inferred)
extern "C" void SetSoTreeFlag2(SO *so, int f)
{
    SOTREE *ptree;
    ptree = so->physicsTree;
    if (ptree != 0) {
        ptree->skipsUpdate = f;
        FreeSoContacts(so);
    }
}

// 0x1F68A8 SetSoPartsBit48
namespace M_func_001F68A8 { extern "C" {
struct E; struct P;
typedef unsigned long ulong;
struct E { char pad[0x10]; ulong lo : 48; ulong bit48 : 1; ulong hi : 15; char pad2[0xB0 - 0x18]; };
struct P { char pad[0xE0]; int numParts; struct E *parts; };
// Sets (f = 1) bit 48 in the flag word of every part. (inferred)
void SetSoPartsBit48(struct P *so, int f)
{
    int i;
    for (i = 0; i < so->numParts; i++) so->parts[i].bit48 = f;
}
} } // namespace M_func_001F68A8

// 0x1F6918 SetSoIgnoreFixed
namespace M_func_001F6918 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong a:26; int b:1; ulong c:37; };
struct G;
extern struct G D_003DD490;
void RefreshSwObjectCollisionFilter(struct G *, struct S *);
// Sets the ignore-fixed flag (+0x148 bit 26, so.h fIgnoreFixed) and refreshes the broad-phase pairs.
void SetSoIgnoreFixed(struct S *p, int f) { p->b = f; RefreshSwObjectCollisionFilter(&D_003DD490, p); }
} } // namespace M_func_001F6918

// 0x1F6960 GetSoIgnoreFixed
namespace M_func_001F6960 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong flags; };
// Returns the ignore-fixed flag.
int GetSoIgnoreFixed(struct S *p) { return (int)((long)(p->flags << 6) >> 32) & 1; }
} } // namespace M_func_001F6960

// 0x1F6978 ShowSoGlobsById
namespace M_ShowSoGlobsById { extern "C" {
struct Q; struct E; struct P;
struct Q { char pad[0x10]; ulong flags; };
struct E { int oid; int pad[2]; int flags; int pad2[5]; struct Q *part; };
struct P { char pad[0xB8]; int numGlobs; int pad2; struct E *globs; };
// Shows every glob with id k and the part linked to it.
void ShowSoGlobsById(struct P *so, int k)
{
    int i;
    for (i = 0; i < so->numGlobs; i++) {
        struct E *pe = &so->globs[i];
        if (pe->oid == k) {
            pe->flags &= ~0x800;
            if (pe->part) pe->part->flags &= ~(1L << 50);
        }
    }
}
} } // namespace M_ShowSoGlobsById

// 0x1F69F8 HideSoGlobsById
namespace M_HideSoGlobsById { extern "C" {
struct Q; struct E; struct P;
struct Q { char pad[0x10]; ulong flags; };
struct E { int oid; int pad[2]; int flags; int pad2[5]; struct Q *part; };
struct P { char pad[0xB8]; int numGlobs; int pad2; struct E *globs; };
// Hides every glob with id k and the part linked to it.
void HideSoGlobsById(struct P *so, int k)
{
    int i;
    for (i = 0; i < so->numGlobs; i++) {
        struct E *pe = &so->globs[i];
        if (pe->oid == k) {
            pe->flags |= 0x800;
            if (pe->part) pe->part->flags |= (1L << 50);
        }
    }
}
} } // namespace M_HideSoGlobsById

// 0x1F6A60: dead-code scrap, a nop unit (sly2/scrap.h)
SLY2_SCRAP("nop");

// 0x1F6A68 HideSoAllGlobs
namespace M_func_001F6A68 { extern "C" {
struct C; struct E; struct A;
typedef unsigned long ulong;
struct C { char pad[0x10]; ulong flags; };
struct E { char pad[0xC]; int flags; char pad2[0x14]; struct C *part; };
struct A { char pad[0xB8]; int numGlobs; int pad2; struct E *globs; };
// Hides every glob and its linked part.
void HideSoAllGlobs(struct A *so)
{
    int i;
    for (i = 0; i < so->numGlobs; i++) {
        struct E *pe = &so->globs[i];
        pe->flags |= 0x800;
        if (pe->part)
            pe->part->flags |= 1L << 50;
    }
}
} } // namespace M_func_001F6A68

// 0x1F6AC8 ShowSoPartsById
namespace M_ShowSoPartsById { extern "C" {
struct E; struct P;
struct E { int pad[3]; int oid; char pad2[0xB0 - 0x10]; };
struct P { char pad[0xE0]; int numParts; struct E *parts; };
void ShowSoPart(struct P *, struct E *);
// Shows the parts whose id is k.
void ShowSoPartsById(struct P *so, int k)
{
    int i;
    for (i = 0; i < so->numParts; i++) {
        struct E *pe = &so->parts[i]; if (pe->oid == k) ShowSoPart(so, pe);
    }
}
} } // namespace M_ShowSoPartsById

// 0x1F6B48 HideSoPartsById
namespace M_func_001F6B48 { extern "C" {
struct E; struct P;
struct E { int pad[3]; int oid; char pad2[0xB0 - 0x10]; };
struct P { char pad[0xE0]; int numParts; struct E *parts; };
void HideSoPart(struct P *, struct E *);
// Hides the parts whose id is k.
void HideSoPartsById(struct P *so, int k)
{
    int i;
    for (i = 0; i < so->numParts; i++) {
        struct E *pe = &so->parts[i]; if (pe->oid == k) HideSoPart(so, pe);
    }
}
} } // namespace M_func_001F6B48

// 0x1F6BC8 SetSoPartsZapKindsById
namespace M_func_001F6BC8 { extern "C" {
struct E; struct S;
struct E { int a[3]; int oid; int b[2]; int zapKinds; char pad[0x94]; };
struct S { char pad[0xE0]; int numParts; struct E *parts; };
// Sets the zap kinds (+0x18) of the parts whose id is n.
void SetSoPartsZapKindsById(struct S *p, int n, int x) { int i; for (i = 0; i < p->numParts; i++) { struct E *pe = &p->parts[i]; if (pe->oid == n) pe->zapKinds = x; } }
} } // namespace M_func_001F6BC8

// 0x1F6C08 EnsureSoPartMesh
namespace M_func_001F6C08 {
union U; struct TRI; struct MESH; struct EDGE; struct PLANE; struct P; struct FRAME; struct MSG;
// EnsureSoPartMesh (0x1F6C08): build the scratch mesh of P once (p->mesh at +0x10).
// Allocates the MESH, per-point velocity/acceleration arrays and per-triangle vertex triples inside a
// temporary allocator FRAME, collects each triangle's three vertices from the edge list (edge face ids
// -> unique edge vertices, at most 3 per triangle), then orients every triple so that
// dot(cross(v1-v0, v2-v0), plane normal) >= 0 (swaps vertex 1/2 otherwise).
// MATCH (T1582): ONE function-scope triangle index (`itri`, used by the edge loop and as the second loop's
// counter) and ONE function-scope `ptri` shared by both loops; per-loop locals gave register rotation only.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
struct TRI {int vertex[3];};
struct MESH {qword *velocity,*acceleration;TRI *triangles;};
struct EDGE {unsigned short vertex[2],face[2];int extra[2];};
struct PLANE {U normal;U extra;};
struct P {char pad0[0x10];MESH *mesh;char pad14[0x68];U *positions;int planeCount;PLANE *planes;char pad88[8];int pointCount;U *source;int triCount;void *triSource;int edgeCount;EDGE *edges;};
struct FRAME;
extern "C" FRAME *STACKSCOPE_ctor(FRAME *);
extern "C" void STACKSCOPE_ctor_1AE4A0(FRAME *,int) __asm__("_$_10STACKSCOPE");
// Empty scope object (constructor/destructor only shape the generated code).
struct FRAME {FRAME(){}~FRAME(){}};
struct MSG {int bytes,alignment;};
extern "C" void *AllocFromWorldHeap(MSG *);
extern "C" void *AllocClearedFromWorldHeap(MSG *);
extern "C" void *AllocClearedFromStack(MSG *);
// Return a - b.
static inline qword Sub(qword a,qword b) {qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"0"(a),"j"(b));return r;}
// Cross product a x b.
static inline qword Cross(qword a,qword b){qword r;__asm__("vopmula.xyz $ACC,%1,%2\n\tvopmsub.xyz %0,%2,%1":"=j"(r):"0"(a),"j"(b));return r;}
// Dot product of the xyz parts of a and b as a float.
static inline float Dot(qword a,qword b){qword r,one;float f;
__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(r),"=&j"(one):"0"(a),"j"(b));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(r));return f;}
// Builds a part's soft-mesh record once: per-vertex velocities and accelerations and the oriented triangles.
extern "C" void EnsureSoPartMesh(P *so)
{
    if(so->mesh)return;
    U first,second,cross;
    FRAME storage;
    MSG request;
    int triBytes=sizeof(TRI),alignment=16;
    STACKSCOPE_ctor(&storage);
    MSG *allocation=&request;
    request.bytes=triBytes;allocation->alignment=alignment;
    so->mesh=(MESH *)AllocFromWorldHeap(allocation);
    request.bytes=so->pointCount*16;allocation->alignment=alignment;
    so->mesh->velocity=(qword *)AllocClearedFromWorldHeap(allocation);
    request.bytes=so->pointCount*16;allocation->alignment=alignment;
    so->mesh->acceleration=(qword *)AllocClearedFromWorldHeap(allocation);
    request.bytes=so->triCount*triBytes;allocation->alignment=alignment;
    so->mesh->triangles=(TRI *)AllocClearedFromWorldHeap(allocation);
    request.bytes=so->triCount*4;allocation->alignment=alignment;
    int *counts=(int *)AllocClearedFromStack(allocation);
    int itri;
    TRI *ptri;
    for(int i=0;i<so->edgeCount;i++) {
        EDGE *edge=&so->edges[i];
        for(int side=0;side<2;side++) {
            itri=edge->face[side];
            ptri=&so->mesh->triangles[itri];
            for(int endpoint=0;endpoint<2;endpoint++) {
                int vertex=edge->vertex[endpoint];
                int j;
                for(j=0;j<counts[itri];j++)if(vertex==ptri->vertex[j])break;
                if(j<counts[itri])continue;
                if(j<3){ptri->vertex[counts[itri]]=vertex;counts[itri]++;}
            }
        }
    }
    U *positions=so->positions;
    for(itri=0;itri<so->triCount;itri++) {
        ptri=&so->mesh->triangles[itri];
        PLANE *plane=&so->planes[itri];
        first.q=Sub(positions[ptri->vertex[1]].q,positions[ptri->vertex[0]].q);
        second.q=Sub(positions[ptri->vertex[2]].q,positions[ptri->vertex[0]].q);
        cross.q=Cross(first.q,second.q);
        if(Dot(cross.q,plane->normal.q)<0.0f){int t=ptri->vertex[1];ptri->vertex[1]=ptri->vertex[2];ptri->vertex[2]=t;}
    }
    STACKSCOPE_ctor_1AE4A0(&storage,2);
}
} // namespace M_func_001F6C08

// 0x1F6EF8 UpdateSoPartMesh
namespace M_func_001F6EF8 { extern "C" {
struct TRI; struct MESH; struct P;
typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f;} V;
struct TRI {int index[3];};
struct MESH {qword *velocity,*acceleration;struct TRI *triangles;};
struct P {void *x0;int x4;float maximum;int xC;struct MESH*mesh;char pad14[0x64];int count;qword *positions;int triCount;void *planes;char pad88[0x20];int state;};
void InitPlaneFromPositions(void*,qword,qword,qword);
// Return a + b * s.x (vf5).
static inline qword Step(qword a,qword b,qword s){register qword r __asm__("vf5");
__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=&j"(r):"j"(a),"j"(b),"j"(s));return r;}
// Store the half-step scalar and return p + old*s + next*s (velocity-Verlet style position step) (inferred).
static inline qword Position(qword p,qword old,qword next,qword half,V *scalar){register qword r __asm__("vf3");
__asm__("sqc2 %5,%1\n\tvadda.xyzw $ACC,%2,$vf0\n\tvmaddax.xyzw $ACC,%3,%5x\n\tvmaddx.xyzw %0,%4,%5x":"=&j"(r),"=m"(scalar->q):"j"(p),"j"(old),"j"(next),"j"(half));return r;}
// Squared length of the xyz part of q as a float.
static inline float LengthSquared(qword q){qword r,one;float f;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(r),"=&j"(one):"j"(q));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(r));return f;}
// Copy the velocity array pointer through pinned registers.
static inline qword *VelocityCursor(qword *p){register qword *initial __asm__("$2")=p;register qword *r __asm__("$4");__asm__("daddu %0,%1,$zero":"=r"(r):"r"(initial));return r;}
// a < b (slt) in $2.
static inline int PlaneLess(int a,int b){register int r __asm__("$2");__asm__("slt %0,%1,%2":"=r"(r):"r"(a),"r"(b));return r;}
// Integrates a soft-mesh part's vertices by dt (semi-implicit), tracks the largest radius and invalidates the part
// after large moves.
void UpdateSoPartMesh(struct P*so,float dt)
{
 V scalar;
 register int vertexIndex __asm__("a2")=0;
 qword *velocity=so->mesh->velocity;
 register int initialCount __asm__("$3");
 register qword *acceleration __asm__("$5")=({register qword *r __asm__("$5");__asm__("lw %0,%2\n\tlw %1,%3":"=&r"(initialCount),"=r"(r):"m"(so->count),"m"(so->mesh->acceleration));r;});
 register qword *positions __asm__("s4");
 float maximum=0.0f;
 positions=so->positions;
 if(initialCount>0){
   register qword time __asm__("vf7")=VuFromFloat<qword>(dt);
   register int invalid __asm__("$7")=-1;
   register qword half __asm__("vf6")=VuFromFloat<qword>(dt*0.5f);
   register qword *position __asm__("$3")=positions;
   register qword *velocityCursor __asm__("$4")=VelocityCursor(velocity);
   register int limit __asm__("$2");
   do{
     register qword old __asm__("vf1")=*velocityCursor,force __asm__("vf2"),point __asm__("vf4")=*position;
     int state;
     qword next,newpoint;
     float square;
     __asm__("sqc2 %4,%2\n\tlqc2 %0,%3\n\tlw %1,%5":"=j"(force),"=r"(state),"=m"(scalar.q):"m"(*acceleration),"j"(time),"m"(so->state));
     next=Step(old,force,time);
     newpoint=Position(point,old,next,half,&scalar);
     if(state!=invalid && 1.0f<LengthSquared(VuSub(point,newpoint)))so->state=invalid;
     square=LengthSquared(newpoint);
     *position=newpoint;
     *velocityCursor=next;
     if(maximum<square)maximum=square;
     limit=so->count;
   }while(++velocityCursor,++position,++acceleration,++vertexIndex<limit);
 }
 so->maximum=SqrtFloat(maximum);
 {
   struct TRI*firstTri=so->mesh->triangles;
   char *firstPlane=(char*)so->planes;
   register int planeIndex __asm__("s3")=0;
   if(so->triCount>0){struct TRI*tri=firstTri;char*plane=firstPlane;do{
     qword a=positions[tri->index[0]],b=positions[tri->index[1]],c=positions[tri->index[2]];
     __asm__("addiu %0,%0,12":"+r"(tri));
     InitPlaneFromPositions(plane,a,b,c);
     plane+=0x20;
   }while(PlaneLess(++planeIndex,so->triCount));}
 }
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_int[(sizeof(int)==4)?1:-1];
typedef char layout_float[(sizeof(float)==4)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_V[(sizeof(V)==16)?1:-1];
typedef char layout_V_alignment[(__alignof__(V)==16)?1:-1];
typedef char layout_TRI[(sizeof(struct TRI)==12)?1:-1];
typedef char layout_MESH[(sizeof(struct MESH)==12)?1:-1];
typedef char layout_P[(sizeof(struct P)==0xAC)?1:-1];
typedef char layout_P_maximum[((unsigned)&((struct P*)0)->maximum==8)?1:-1];
typedef char layout_P_mesh[((unsigned)&((struct P*)0)->mesh==0x10)?1:-1];
typedef char layout_P_count[((unsigned)&((struct P*)0)->count==0x78)?1:-1];
typedef char layout_P_positions[((unsigned)&((struct P*)0)->positions==0x7C)?1:-1];
typedef char layout_P_triCount[((unsigned)&((struct P*)0)->triCount==0x80)?1:-1];
typedef char layout_P_planes[((unsigned)&((struct P*)0)->planes==0x84)?1:-1];
typedef char layout_P_state[((unsigned)&((struct P*)0)->state==0xA8)?1:-1];
typedef char layout_MESH_velocity[((unsigned)&((struct MESH*)0)->velocity==0)?1:-1];
typedef char layout_MESH_acceleration[((unsigned)&((struct MESH*)0)->acceleration==4)?1:-1];
typedef char layout_MESH_triangles[((unsigned)&((struct MESH*)0)->triangles==8)?1:-1];
} } // namespace M_func_001F6EF8

// 0x1F70C0 CloneSoPartGeomInserting
namespace M_func_001F70C0 {
union U; struct M4; struct LO; struct GEO; struct TREE; struct P; struct FRAME; struct MSG;
// Insert zeroed vertices, rebuild transformed geometry, and rebase its tree.
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
// 4x4 matrix of U rows with copy constructor and assignment.
struct M4 {U rows[4];M4(){}M4(const M4&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];rows[3]=m.rows[3];}M4&operator=(const M4&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];rows[3]=m.rows[3];return *this;}};
struct LO {char pad40[0x40];M4 matrix;};
struct GEO {int count;qword *points;int planeCount;void *planes;int unknown10,unknown14;};
struct TREE {int count,flag,unused;void *nodes;};
struct P {LO *owner;int flags;float radius;int material;char pad10[0xC];int pinnedCount;char pad20[0x48];TREE tree;GEO local,world;};
struct FRAME {int pad[4];};
extern "C" FRAME *STACKSCOPE_ctor(FRAME *);
extern "C" void STACKSCOPE_ctor_1AE4A0(FRAME *,int) __asm__("_$_10STACKSCOPE");
struct MSG {int bytes,alignment;};
extern "C" void *AllocClearedFromStack(MSG *);
extern "C" void CopyAb__FPvT0Ui(void *,const void *,unsigned int);
extern "C" M4 *InvertMatrix4Rigid(M4 *,const M4 *);
extern "C" M4 *__ml__FRC7MATRIX4T0(M4 *,const M4 *,const M4 *);
extern "C" void CloneGeom__FP4GEOMP7MATRIX4T0(GEO *,const M4 *,GEO *);
extern "C" void CloneBspc(TREE *,GEO *,TREE *,GEO *);

// Squared length of the xyz part of a as a float.
static inline float SquaredLength(qword a){qword one;float f;__asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(a),"=&j"(one));__asm__("mtc1 %1,%0":"=f"(f):"r"(a));return f;}
/* volatile: the game's sqrt.s is a barrier for the scheduler (the radius store must precede the next constant). */
static inline float Sqrt(float f){float r;__asm__ volatile("sqrt.s %0,%1":"=f"(r):"f"(f));return r;}
// Copies a part's geometry into another space, inserting zeroed vertices, and rebuilds the world geometry and BSP.
extern "C" void CloneSoPartGeomInserting(P *source,LO *space,int inserted,P *dest)
{
    M4 transform;
    GEO temporary;
    FRAME frame;
    STACKSCOPE_ctor(&frame); /* explicit begin/end calls (POD frame), not a ctor/dtor class: that reorders the argument copies. */
    MSG request;MSG *allocation=&request;
    float maximum=0.0f;
    dest->owner=space;dest->flags=source->flags;dest->material=source->material;dest->pinnedCount=source->pinnedCount;
    {for(int i=0;i<source->local.count;i++){float length=SquaredLength(source->local.points[i]);if(maximum<length)maximum=length;}}
    dest->radius=Sqrt(maximum);
    temporary=source->local;
    request.bytes=(temporary.count+inserted)*16;allocation->alignment=16;
    qword *points=(qword *)AllocClearedFromStack(allocation);
    int pinned=source->pinnedCount;
    CopyAb__FPvT0Ui(points,temporary.points,pinned*16);
    CopyAb__FPvT0Ui(points+(pinned+inserted),temporary.points+pinned,(temporary.count-pinned)*16);
    temporary.count+=inserted;temporary.points=points;
    {M4 product,inverse;__ml__FRC7MATRIX4T0(&product,(InvertMatrix4Rigid(&inverse,&space->matrix),&inverse),&source->owner->matrix);transform=product;}
    CloneGeom__FP4GEOMP7MATRIX4T0(&temporary,&transform,&dest->local);
    CloneGeom__FP4GEOMP7MATRIX4T0(&dest->local,&space->matrix,&dest->world);
    CloneBspc(&source->tree,&source->world,&dest->tree,&dest->world);
    STACKSCOPE_ctor_1AE4A0(&frame,2);
}
} // namespace M_func_001F70C0

// 0x1F72F8 IsPosInSo
namespace M_func_001F72F8 { extern "C" {
struct O; struct E; struct P;
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct O { char pad[0x70]; qword pos; };
struct E { struct O *transform; int pad; float radius; int pad2; ulong flags; char pad3[0x74 - 0x18]; void *bsp; char pad4[0xB0 - 0x78]; };
struct P { char pad[0xE0]; int numParts; struct E *parts; };
int FindBspLeafContainingPointFast(void *, int, qword);
// Slot 73: returns 1 when pos is inside any visible sphere part or part BSP of the object.
int IsPosInSo(struct P *so, qword pos)
{
    int i;
    for (i = 0; i < so->numParts; i++) {
        struct E *pe = &so->parts[i];
        if (pe->flags & (1L << 50)) continue;
        if (pe->flags & (1L << 49)) {
            qword d, one;
            float r2, d2;
            int t;
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pos), "j"(pe->transform->pos));
            r2 = pe->radius * pe->radius;
            __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "0"(d));
            __asm__("qmfc2.ni %0, %1" : "=r"(t) : "j"(d));
            __asm__("mtc1 %1, %0" : "=f"(d2) : "r"(t));
            { int f = d2 < r2; if (f) return 1; }
        }
        if (pe->bsp && FindBspLeafContainingPointFast(pe->bsp, 0x7FFFFFFF, pos)) return 1;
    }
    return 0;
}
} } // namespace M_func_001F72F8

// 0x1F7410 InflictSoContactZaps
namespace M_func_001F7410 { extern "C" {
struct O; struct H; struct N; struct E; struct P; struct M;
typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x14]; short damage; short pad2; int zapKinds; };
struct H { struct O *part; int pad; void *so; void *object; qword pos; qword posLocal; qword velocity; };
struct N { struct H h[2]; char pad[0xAC - 0x80]; struct N *next; };
struct E { char pad[0x6C]; struct N *zapContacts; };
struct P { char pad[0xEC]; struct E *tree; };
struct M { void *source; void *part; void *so; int zapKinds; int damage; int pad[3]; qword posLocal; qword velocity; char pad2[0x60 - 0x40]; };
struct G;
extern struct G D_003DD490;
void *FindZapResponder(void *, int);
void func_002333B4(void *, int, int) __asm__("memset");
int DeliverZapToSo(struct M *);
void FreeSwXpList__FP2SWP2XP(struct G *, void *);
// Slot 74: for each contact whose other part has zap kinds, builds a zap record and delivers it; frees the list.
// (inferred)
void InflictSoContactZaps(struct P *so)
{
    struct N *pn;
    for (pn = so->tree->zapContacts; pn; pn = pn->next) {
        int i = pn->h[0].so != so;
        struct H *ph = (struct H *)((char *)pn + (i << 6));
        if (ph->part->zapKinds) {
            struct H *pot = (struct H *)((char *)pn + ((1 - i) << 6));
            void *r = FindZapResponder(pot->object, ph->part->zapKinds);
            if (r) {
                struct M m;
                func_002333B4(&m, 0, 0x60);
                m.source = so;
                m.part = ph->part;
                m.so = r;
                m.zapKinds = ph->part->zapKinds;
                m.damage = ph->part->damage;
                m.posLocal = pot->pos;
                m.velocity = ph->velocity;
                DeliverZapToSo(&m);
                if (!so->tree) break;
            }
        }
    }
    if (so->tree) {
        FreeSwXpList__FP2SWP2XP(&D_003DD490, so->tree->zapContacts);
        so->tree->zapContacts = 0;
    }
}
} } // namespace M_func_001F7410

// 0x1F7530 FindZapResponder
namespace M_func_001F7530 { extern "C" {
struct N;
int GetSoZapResponseFlags(void *p, void *q);
struct N { char pad[0x10]; struct N *next; };
// Returns the first LO in a chain whose zap response to the given kinds is nonzero.
struct N *FindZapResponder(struct N *lo, void *q)
{
    for (; lo; lo = lo->next) {
        if (GetSoZapResponseFlags(lo, q))
            return lo;
    }
    return 0;
}
} } // namespace M_func_001F7530

// 0x1F7588 CanSoTakeDamage
// Slot 75 (so.h TakeDamage): returns 1 when the zap record's damage field (+0x44) is nonzero, i.e. the zap
// hurts (inferred).
extern "C" int CanSoTakeDamage(SO *so, SOWATCH *pwatch)
{
    return pwatch->unknown44 != 0;
}

// 0x1F7598 DeliverZapToSo
namespace M_func_001F7598 { extern "C" {
struct VT; struct O; struct K; struct N; struct M; struct M2; struct M3;
typedef int qword __attribute__((mode(TI)));
struct O;
struct VT {
    short d; short pad;
    void *pfn;
};
struct O { char pad[0xC]; struct VT *pvt; struct O *parent; };
struct K {
    struct O *source; int part; void *target; int zapKinds;
    char pad[0x44 - 0x10];
    int responseFlags; struct O *responder; float time; char dle[0x10];
} __attribute__((aligned(16)));
struct N { int source; int part; int pad8; int zapKinds; char pad[0x48 - 0x10]; struct O *responder; char pad2[0x50 - 0x4C]; struct N *next; };
struct M { struct O *po; int n; struct K *zpr; };
struct M2 { struct O **pso; int *zapKinds; };
struct M3 { struct K *zpr; };
extern float D_002DDED0;
struct O *GetSoFromLo(void *);
int GetSoZapResponseFlags(struct O *, int);
struct K *AllocZeroedFromSlotheap(void *);
void ClearDle__FP3DLE(void *);
void AppendDlEntry__FP2DLPv(void *, void *);

#define VCALL(po, slot, T) ((T)(po)->pvt[slot].pfn)((void *)((char *)(po) + (po)->pvt[slot].d)

// Delivers a zap (damage) record to the first object in the target chain that accepts it: skips zaps already in the
// recent-zap list, computes the response and calls the object's zap handler.
// Sly 1 zap.cpp vocabulary (InflictZpdZap, FInflictSoZap).
struct O *DeliverZapToSo(struct K *pk)
{
    int unused[4];
    struct M m;
    struct M2 m2;
    struct O *po;
    for (po = GetSoFromLo(pk->target); po; po = po->parent) {
        struct N *pn;
        for (pn = (*(struct N * *)&g_sw.dlZprHit); pn; pn = pn->next) {
            if (pn->source == (int)pk->source && pn->part == pk->part && pn->zapKinds == pk->zapKinds && pn->responder == po) return 0;
        }
        pk->responseFlags = GetSoZapResponseFlags(po, pk->zapKinds);
        if (VCALL(po, 75, int (*)(void *, struct K *)), pk)) {
            struct K *pkNew;
            pk->responder = po;
            pkNew = AllocZeroedFromSlotheap((*(char (*)[])&g_sw.slotheapZprHit));
            *pkNew = *pk;
            pkNew->time = D_002DDED0;
            ClearDle__FP3DLE(pkNew->dle);
            AppendDlEntry__FP2DLPv(&(*(struct N * *)&g_sw.dlZprHit), pkNew);
            if (pk->responseFlags & 0xF8) {
                {
                    m.po = po; m.n = (int)pk->source; m.zpr = pk;
                    VCALL(po, 4, void (*)(void *, int, struct M *)), 0xB, &m);
                    VCALL(pk->source, 4, void (*)(void *, int, struct M *)), 0xB, &m);
                }
                {
                    m2.pso = &po; m2.zapKinds = &pk->zapKinds;
                    VCALL(pk->source, 3, void (*)(void *, int, int, int, void *, int)), 0xA, 1, 2, &m2, 0);
                }
                {
                    m2.pso = (struct O **)pk;
                    VCALL(po, 3, void (*)(void *, int, int, int, void *, int)), 0xB, 1, 2, &m2, 0);
                }
            }
            return po;
        }
    }
    return 0;
}
} } // namespace M_func_001F7598

// 0x1F77C0 ExpireSwZapRecords
namespace M_ExpireSwZapRecords { extern "C" {
struct DL; struct DLI; struct E; struct P;
struct DL { void *pvFirst; void *pvLast; int linkOffset; };
struct DLI { struct DL *pdl; void **ppv; int ibDle; struct DLI *pdliNext; };
extern struct DLI *D_002DD2FC;
extern struct CLOCK D_002DDED0;
struct E { char pad[0x4C]; float t; };
struct P { char pad[0x4118]; char pool[0x18]; struct DL dl; };
void RemoveDlEntry__FP2DLPv(struct DL *, void *);
void FreeSlotheapSlot(void *, void *);
// Start iterating display list pdl with iterator pdli and push it on the global iterator stack (D_002DD2FC).
static inline void InitDli(struct DLI *pdli, struct DL *pdl)
{
    pdli->pdl = pdl;
    pdli->ppv = (void **)pdl;
    pdli->ibDle = pdl->linkOffset;
    pdli->pdliNext = D_002DD2FC;
    D_002DD2FC = pdli;
}
// Return the current element of the iterator and advance it along the element's link field.
static inline void *PvNextDli(struct DLI *pdli)
{
    void *pv = *pdli->ppv;
    pdli->ppv = (void **)((char *)pv + pdli->ibDle);
    return pv;
}
// Drops zap-history entries older than 0.25 s from the world's list.
void ExpireSwZapRecords(struct P *so)
{
    struct DLI dli;
    struct E *pe;
    InitDli(&dli, &so->dl);
    while ((pe = (__typeof__(pe))PvNextDli(&dli)) != 0) {
        if (D_002DDED0.time < pe->t + 0.25f) continue;
        RemoveDlEntry__FP2DLPv(&so->dl, pe);
        FreeSlotheapSlot(so->pool, pe);
    }
    D_002DD2FC = dli.pdliNext;
}
} } // namespace M_ExpireSwZapRecords

// 0x1F78A0 GetZapKindIndex
namespace M_func_001F78A0 { extern "C" {
// Index of the lowest set bit among the 26 zap kinds.
int GetZapKindIndex(int x)
{
    int i; int m;
    for (i = 0, m = 1; i < 26; i++, m <<= 1) if (x & m) break;
    return i;
}
} } // namespace M_func_001F78A0

// 0x1F78D8 GetSoZapResponseFlags
namespace M_GrfzpdFromSoGrfzpk { extern "C" {
struct S;
struct S { char pad[0x110]; unsigned char ab[26]; };
// OR of the object's per-kind response bytes (+0x110[26]) for the kinds in grfzpk (Sly 1 GrfzpdFromSoGrfzpk).
int GetSoZapResponseFlags(struct S *so, int f)
{
    int r = 0; int i; int m;
    for (i = 0, m = 1; i < 26; i++) { if (m & f) r |= so->ab[i]; m <<= 1; }
    return r;
}
} } // namespace M_GrfzpdFromSoGrfzpk

// 0x1F7918 SetSoZapResponse
namespace M_func_001F7918 { extern "C" {
struct S;
struct S { char pad[0x110]; unsigned char ab[26]; };
// Sets the object's response byte to b for each zap kind in grfzpk.
void SetSoZapResponse(struct S *so, int f, int b)
{
    int i; int m;
    for (i = 0, m = 1; i < 26; i++) { if (m & f) so->ab[i] = b; m <<= 1; }
}
} } // namespace M_func_001F7918

// 0x1F7950 HandleSoTouchZaps
namespace M_func_001F7950 { extern "C" {
struct VTE; struct VT; struct E; struct P; struct M; struct G2;
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct VTE { short d; short p; void *pfn; };
struct VT { struct VTE a[75]; };
struct E { char pad[0x6C]; int zapContacts; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0xEC - 0x10]; struct E *tree; char pad3[0x148 - 0xF0]; union { struct { signed char hitDamage0, hitDamage1; } c; ulong flags; } u; };
struct M { void *source; int n4; void *target; int zapKinds; int damage; int pad[3]; qword pos; qword velocity; char pad2[0x60 - 0x40]; };
struct G2 { void *p; };
extern struct G2 D_002F7900;
void SetSoZapMode(struct P *, int);
void func_002333B4(void *, int, int) __asm__("memset");
qword GetLoWorldPos(struct P *);
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void DeliverZapToSo(struct M *);
// Deferred touch callback: processes the contacts' zaps (slot 74), then emits the object's own zap according to
// its zap mode. (inferred)
void HandleSoTouchZaps(struct P *so)
{
    int k;
    struct M m;
    if (!so->tree) return;
    if (so->tree->zapContacts) ((void (*)(void *))so->pvt->a[74].pfn)((char *)so + so->pvt->a[74].d);
    if (!so->tree) return;
    k = (int)(so->u.flags >> 24) & 3;
    if (!k) return;
    SetSoZapMode(so, 0);
    func_002333B4(&m, 0, 0x60);
    m.target = so;
    m.pos = GetLoWorldPos(so);
    m.velocity = ((qword (*)(void *))so->pvt->a[33].pfn)((char *)so + so->pvt->a[33].d);
    if (k == 3 && IsBasicDerivedFrom__FP5BASIC3CID(so, 0x38)) {
        m.zapKinds = 0x40000; m.damage = -50; m.source = D_002F7900.p;
    } else {
        m.source = so; m.zapKinds = 0x20000; m.damage = so->u.c.hitDamage1;
    }
    DeliverZapToSo(&m);
}
} } // namespace M_func_001F7950

// 0x1F7A68 SetSoZapMode
namespace M_func_001F7A68 { extern "C" {
struct E; struct P;
typedef unsigned long ulong;
struct E { char pad[0x14]; short damage; short pad2; int zapKinds; char pad3[0xB0 - 0x1C]; };
struct P { char pad[0xE0]; int numParts; struct E *parts; char pad2[0x148 - 0xE8]; union { signed char hitDamage0; struct { ulong lo : 24; ulong zapMode : 2; ulong hi : 38; } f; ulong flags; } u; };
// Sets the zap mode (+0x148 bits 24-25, 0..3); mode 2 makes the parts with zap kind 0 damaging (0x10000) with the
// object's hit damage, the other modes make them harmless.
void SetSoZapMode(struct P *so, int k)
{
    int from, to, s;
    int i;
    if (k == ((int)(so->u.flags >> 24) & 3)) return;
    switch (k) {
    case 0: case 1: case 3:
        from = 0x10000; to = 0; s = -10;
        break;
    case 2:
        s = so->u.hitDamage0; from = 0; to = 0x10000;
        break;
    }
    for (i = 0; i < so->numParts; i++) {
        struct E *pe = &so->parts[i];
        if (pe->zapKinds == from) { pe->zapKinds = to; pe->damage = s; }
    }
    so->u.f.zapMode = (unsigned int)k;
}
} } // namespace M_func_001F7A68

// 0x1F7B40 GetSoZapMode
namespace M_func_001F7B40 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong flags; };
// Returns the zap mode.
int GetSoZapMode(struct S *p) { return (int)((long)(p->flags << 8) >> 32) & 3; }
} } // namespace M_func_001F7B40

// 0x1F7B58 SetSoHitDamage
// Sets the hit damage (stored negated in the byte at +0x148; default 10). (inferred)
extern "C" void SetSoHitDamage(SO *so, int n)
{
    so->unknown148 = -n;
}

// 0x1F7B68 GetSoHitDamage
// Returns the hit damage. (inferred)
extern "C" int GetSoHitDamage(SO *so)
{
    return -so->unknown148;
}

// 0x1F7B78 SetSoHitDamage2
// Sets the second hit damage (byte +0x149, negated; default 20). (inferred)
extern "C" void SetSoHitDamage2(SO *so, int n)
{
    so->unknown149 = -n;
}

// 0x1F7B88 GetSoHitDamage2
// Returns the second hit damage. (inferred)
extern "C" int GetSoHitDamage2(SO *so)
{
    return -so->unknown149;
}

// 0x1F7B98 EnsureSoInSwList187
/* Gives the object a CID 0x12 child with oid 0x187 unless it has one. */
// Adds the object to world list 0x187 if it is not in it yet (meaning of the list unknown). (inferred)
extern "C" void EnsureSoInSwList187(SO *so)
{
    if (FindSwObject(&D_003DD490, 2, (OID)0x187, so) == 0) {
        CreateLo((CID)0x12, so, (OID)0x187, -1);
    }
}

// 0x1F7BF0 GetSoPhys
// The object's physics record (+0xE8).
extern "C" SOPHYS *GetSoPhys(SO *so)
{
    return so->physics;
}

// 0x1F7BF8 FadeSoOut
namespace M_FadeSoOut { extern "C" {
void SetSoFadeRate(void *, float);
void SetSoAlpha(void *, float);
// Fades the object out over dur seconds (alpha rate -1/dur); dur <= 0 removes it at once.
void FadeSoOut(void *so, float f)
{
    if (f <= 0.0f) {
        SetSoFadeRate(so, 0.0f);
        SetSoAlpha(so, 0.0f);
    } else {
        SetSoFadeRate(so, -1.0f / f);
    }
}
} } // namespace M_FadeSoOut

// 0x1F7C68 FadeSoIn
namespace M_FadeSoIn { extern "C" {
struct VTE; struct VT; struct P;
typedef unsigned long ulong;
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[9]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x28 - 0x10]; ulong flags; char pad3[0x12C - 0x30]; float alpha; };
void SetSoFadeRate(struct P *, float);
void SetSoAlpha(struct P *, float);
// Fades the object in over dur seconds, re-adding it if it was removed; dur <= 0 shows it at once.
void FadeSoIn(struct P *so, float f)
{
    if (f <= 0.0f) {
        SetSoFadeRate(so, 0.0f);
        SetSoAlpha(so, 1.0f);
    } else {
        if (so->flags & (1L << 56)) {
            so->pvt->a[8].pfn((char *)so + so->pvt->a[8].d);
            so->alpha = 0.0f;
        }
        SetSoFadeRate(so, 1.0f / f);
    }
}
} } // namespace M_FadeSoIn

// 0x1F7D10 SetSoAlpha
/* Sets the alpha; at 0 the object is removed (LO Remove, unless it is a CID 0x69), above 0 added (LO Add). */
// Sets the alpha; at 0 or below the object is removed (except class 0x69), otherwise added.
extern "C" void SetSoAlpha(SO *so, float uAlpha)
{
    if (uAlpha <= 0.0f) {
        if (IsBasicDerivedFrom(so, (CID)0x69) == 0) {
            so->Remove();
        }
    } else {
        so->Add();
    }
    so->opacity = uAlpha;
}

// 0x1F7D90 SetSoFadeRate
namespace M_SetSoFadeRate { extern "C" {
struct VTE; struct VT; struct P;
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[9]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x40 - 0x10]; int flagWord; char pad3[0x130 - 0x44]; float alphaRate; };
// Sets the fade rate; a nonzero rate adds the object and marks it for updates.
void SetSoFadeRate(struct P *so, float g)
{
    if (g != 0.0f) {
        int v;
        so->pvt->a[8].pfn((char *)so + so->pvt->a[8].d);
        v = so->flagWord;
        if ((v ^ 1) & 1) *(int *)v |= 2;
        else so->flagWord = v | 2;
    }
    so->alphaRate = g;
}
} } // namespace M_SetSoFadeRate

// 0x1F7E08 CollectSoParts
namespace M_func_001F7E08 { extern "C" {
struct E; struct P;
struct E { char pad[0xB0]; };
struct P { char pad[0xE0]; int numParts; struct E *parts; };
// Collects the parts that pass an optional predicate into an array; returns the count.
int CollectSoParts(struct P *so, int (*pfn)(void *, struct E *), void *pv, struct E **ape)
{
    int n = 0;
    int i;
    for (i = 0; i < so->numParts; i++) {
        struct E *pe = &so->parts[i];
        if (!pfn || pfn(pv, pe)) {
            ape[n++] = pe;
        }
    }
    return n;
}
} } // namespace M_func_001F7E08

// 0x1F7EC8 SetSoUnknownBits16
namespace M_func_001F7EC8 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong a:16; int b:4; ulong c:44; };
// Sets the 4-bit field at bits 16-19 of +0x148 (meaning unknown, a splice option).
void SetSoUnknownBits16(struct S *p, int n) { p->b = n; }
} } // namespace M_func_001F7EC8

// 0x1F7EF0 GetSoUnknownBits16
namespace M_func_001F7EF0 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong a:16; int b:4; ulong c:44; };
// Returns the 4-bit field at bits 16-19 of +0x148.
int GetSoUnknownBits16(struct S *p) { return p->b; }
} } // namespace M_func_001F7EF0

// 0x1F7F08 GetSoNoJtPlace
namespace M_func_001F7F08 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong flags; };
// Returns the no-JT-place flag (+0x148 bit 29, so.h isNotLandable).
int GetSoNoJtPlace(struct S *p) { return (int)((long)(p->flags << 3) >> 32) & 1; }
} } // namespace M_func_001F7F08

// 0x1F7F20 SetSoNoJtPlace
namespace M_func_001F7F20 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong a:29; int b:1; ulong c:34; };
// Sets the no-JT-place flag.
void SetSoNoJtPlace(struct S *p, int n) { p->b = n; }
} } // namespace M_func_001F7F20

// 0x1F7F48 SetSoTreeRoot
namespace M_func_001F7F48 { extern "C" {
struct A;
typedef unsigned long ulong;
struct A { char pad[0x148]; ulong a:34; ulong f:1; ulong b:1; };
void RebuildSoPhysHook(struct A *p);
// Sets the tree-root flag (+0x148 bit 34, so.h isTreeRoot) and rebuilds the physics hook when it changed. (inferred)
void SetSoTreeRoot(struct A *so, long f)
{
    if (f != so->f) {
        so->f = f;
        RebuildSoPhysHook(so);
    }
}
} } // namespace M_func_001F7F48

// 0x1F7FA0 SetSoScrollerSpeeds
namespace M_func_001F7FA0 { extern "C" {
struct P;
struct P { char pad[0xA4]; int numSaas; void **saas; };
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void SetScrollerMasterSpeeds__FP8SCROLLERff(void *, float, float);
// Sets the master speeds of every scroller (CID 0x8F) in the object's animation list.
void SetSoScrollerSpeeds(struct P *so, float f, float g)
{
    int i;
    for (i = 0; i < so->numSaas; i++) {
        void *q = so->saas[i];
        if (IsBasicDerivedFrom__FP5BASIC3CID(q, 0x8F)) SetScrollerMasterSpeeds__FP8SCROLLERff(q, f, g);
    }
}
} } // namespace M_func_001F7FA0

// 0x1F8038 SetSoSaaParams
namespace M_func_001F8038 { extern "C" {
struct P;
struct P { char pad[0xA4]; int numSaas; void **saas; };
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void SetSaiDuDv(void *, float, float);
// Passes two parameters to every CID 0x8F entry of the object's animation list (inferred).
void SetSoSaaParams(struct P *so, float f, float g)
{
    int i;
    for (i = 0; i < so->numSaas; i++) {
        void *q = so->saas[i];
        if (IsBasicDerivedFrom__FP5BASIC3CID(q, 0x8F)) SetSaiDuDv(q, f, g);
    }
}
} } // namespace M_func_001F8038

// 0x1F80D0 SetSoEyesClosed
namespace M_func_001F80D0 { extern "C" {
struct L;
struct L { char pad[0x24]; struct L *next; char pad2[0x64 - 0x28]; struct L *firstChild; char pad3[0x70 - 0x68]; struct L *firstRemovedChild; char pad4[0xA4 - 0x74]; int numSaas; void **saas; };
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void SetEyesClosed(void *, float);
// Opens or closes the eyes (CID 0x8E) of the object and its SO children.
void SetSoEyesClosed(struct L *so, float f)
{
    int i;
    struct L *q;
    for (i = 0; i < so->numSaas; i++) {
        void *pv = so->saas[i];
        if (IsBasicDerivedFrom__FP5BASIC3CID(pv, 0x8E)) SetEyesClosed(pv, f);
    }
    for (q = so->firstChild; q != so->firstRemovedChild; q = q->next)
        if (IsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) SetSoEyesClosed(q, f);
}
} } // namespace M_func_001F80D0

// 0x1F8198 FindSoSaa
namespace M_PsaaFindSoOid { extern "C" {
struct T; struct S;
struct T { char pad[0x18]; int oid; };
struct S { char pad[0xA4]; int numSaas; struct T **saas; };
// Finds an entry of the object's animation list by oid (Sly 1 PsaaFindSoOid).
struct T *FindSoSaa(struct S *so, int n)
{
    int i;
    for (i = 0; i < so->numSaas; i++) { struct T *pt = so->saas[i]; if (pt->oid == n) return pt; }
    return 0;
}
} } // namespace M_PsaaFindSoOid

// 0x1F81E0 SetSoAnimIndex
namespace M_func_001F81E0 { extern "C" {
struct A;
struct A { char pad[0xAC]; int numAnimIds; int *animIds; int animIndex; };
// Selects an animation by oid: +0xB4 = -(index in the list + 3), or 0 when not found. (inferred)
void SetSoAnimIndex(struct A *so, int n)
{
    int i;
    if (n == -1) {
        so->animIndex = 0;
        return;
    }
    for (i = 0; i < so->numAnimIds; i++) {
        if (n == so->animIds[i]) {
            so->animIndex = -(i + 3);
            return;
        }
    }
    so->animIndex = 0;
}
} } // namespace M_func_001F81E0

// 0x1F8238 ClearSoAnimIndex
// Clears the selected animation (+0xB4 = 0). (inferred)
extern "C" void ClearSoAnimIndex(SOWATCH *pwatch)
{
    pwatch->animIndex = 0;
}

// 0x1F8240 FindTargetRecordById
namespace M_func_001F8240 { extern "C" {
struct T;
struct T { int a[3]; int id; char pad[0x10]; struct T *next; };
// Finds a target record in the world's list by id. (inferred)
struct T *FindTargetRecordById(int n) { struct T *pt; for (pt = (*(struct T * *)&g_sw.dlTargetrec); pt; pt = pt->next) if (pt->id == n) return pt; return 0; }
} } // namespace M_func_001F8240

// 0x1F8278 EnsureTargetRecordById
/* Makes a target record (0xAC bytes, InitTargetRecord) for plo unless it has one. */
// Finds a target record by id, creating one when missing. (inferred)
extern "C" void EnsureTargetRecordById(LO *plo)
{
    if (FindTargetRecordById(plo) == 0) {
        InitTargetRecord(func_001AE4F8(0xAC), plo);
    }
}

// 0x1F82B8 SetSoTargetRecordParams
namespace M_func_001F82B8 { extern "C" {
struct K; struct T;
struct K { int a, b; };
struct T { char pad[0x10]; int value; struct K key; };
struct T *EnsureTargetRecordById(void *);
// Ensures the target record and sets its value and optional 8-byte key. (inferred)
// The record lives in list D_003E145C and is keyed by the object pointer.
void SetSoTargetRecordParams(void *p, int x, struct K *pk) { struct T *pt = EnsureTargetRecordById(p); pt->value = x; if (pk) pt->key = *pk; }
} } // namespace M_func_001F82B8

// 0x1F8300 EnsureSoPhysRegionRecord
namespace M_func_001F8300 { extern "C" {
struct Q; struct E; struct P; struct CB;
struct Q { void *so; int pad; float unknown8; float unknownC; };
struct E { char pad[0xDC]; struct Q *region; };
struct P { char pad[0xE8]; struct E *physics; };
struct CB { int cb; int n; };
void *AllocClearedFromWorldHeap(struct CB *);
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int);
// Fill the request and allocate cb zeroed bytes (kind n) from the world heap.
static inline void *PvAllocMsg(struct CB *pcb, int cb, int n) { pcb->cb = cb; pcb->n = n; return AllocClearedFromWorldHeap(pcb); }
// Ensures the physics region record {object, three tuning floats with defaults 75 (150 for class 0x34) and 1}.
// (inferred)
struct Q *EnsureSoPhysRegionRecord(struct P *so)
{
    if (!so->physics) return 0;
    if (!so->physics->region) {
        struct CB cb;
        struct Q *q = (__typeof__(q))PvAllocMsg(&cb, 0x70, 0x10);
        q->so = so;
        if (IsBasicDerivedFrom__FP5BASIC3CID(so, 0x34)) q->unknown8 = 150.0f;
        else q->unknown8 = 75.0f;
        q->unknownC = 1.0f;
        so->physics->region = q;
    }
    return so->physics->region;
}
} } // namespace M_func_001F8300

// 0x1F83B0 SetSoPhysRecordParam8
// Sets the region record's second float. (inferred)
extern "C" void SetSoPhysRecordParam8(STEP *step, float g)
{
    STEPREC *prec;
    prec = EnsureSoPhysRegionRecord(step);
    if (prec != 0) {
        prec->g8 = g;
    }
}

// 0x1F83E0 GetSoPhysRecordParam8
namespace M_func_001F83E0 { extern "C" {
struct U; struct T; struct S;
struct U { char pad[0x8]; float value; };
struct T { char pad[0xDC]; struct U *region; };
struct S { char pad[0xE8]; struct T *physics; };
// Returns the region record's second float (0 without a record). (inferred)
float GetSoPhysRecordParam8(struct S *p) { struct U *pu = 0; if (p->physics) pu = p->physics->region; if (pu) return pu->value; return 0.0f; }
} } // namespace M_func_001F83E0

// 0x1F8410 SetSoPhysRecordParamC
// Sets the region record's third float. (inferred)
extern "C" void SetSoPhysRecordParamC(STEP *step, float g)
{
    STEPREC *prec;
    prec = EnsureSoPhysRegionRecord(step);
    if (prec != 0) {
        prec->gC = g;
    }
}

// 0x1F8440 GetSoPhysRecordParamC
namespace M_func_001F8440 { extern "C" {
struct U; struct T; struct S;
struct U { char pad[0xC]; float value; };
struct T { char pad[0xDC]; struct U *region; };
struct S { char pad[0xE8]; struct T *physics; };
// Returns the region record's third float (0 without a record). (inferred)
float GetSoPhysRecordParamC(struct S *p) { struct U *pu = 0; if (p->physics) pu = p->physics->region; if (pu) return pu->value; return 0.0f; }
} } // namespace M_func_001F8440

// 0x1F8470 SetSoPhysRecordParam4
// Sets the region record's first float. (inferred)
extern "C" void SetSoPhysRecordParam4(STEP *step, float g)
{
    STEPREC *prec;
    prec = EnsureSoPhysRegionRecord(step);
    if (prec != 0) {
        prec->g4 = g;
    }
}

// 0x1F84A0 GetSoPhysRecordParam4
namespace M_func_001F84A0 { extern "C" {
struct U; struct T; struct S;
struct U { char pad[0x4]; float value; };
struct T { char pad[0xDC]; struct U *region; };
struct S { char pad[0xE8]; struct T *physics; };
// Returns the region record's first float (0 without a record). (inferred)
float GetSoPhysRecordParam4(struct S *p) { struct U *pu = 0; if (p->physics) pu = p->physics->region; if (pu) return pu->value; return 0.0f; }
} } // namespace M_func_001F84A0

// 0x1F84D0 SetSoIceable__FP2SOi
// SO+0x148 bit 33 (64-bit unit at 0x148). Lilly's port calls this bit hurtRespawnsHero; the function name says "iceable".
// Sets the iceable flag (+0x148 bit 33). Sly 1 name.
void SetSoIceable(SO *pso, int f) { pso->hurtRespawnsHero = f; }

// 0x1F8500 GetSoIceable
namespace M_GetSoIceable { extern "C" {
struct S;
struct S { char pad[0x148]; ulong fl; };
// Returns the iceable flag.
int GetSoIceable(struct S *p) { return (int)(p->fl >> 33) & 1; }
} } // namespace M_GetSoIceable

// 0x1F8510 NewSoXfHandle
namespace M_func_001F8510 { extern "C" {
struct Q; struct E; struct P;
struct Q { int a, b, numTransforms; void *pvt; int pad[4]; void *so; void *transform; int pad2[2]; };
struct E { char pad[0x94]; int kind; int pad2; int oid; char pad3[0x100 - 0xA0]; };
struct P { char pad[0x50]; int numTransforms; struct E *transforms; };
extern char D_002CA2C0[] __asm__("_vt$6CID149");
struct Pool;
struct Q *AllocZeroedFromSlotheap(struct Pool *);
void func_002333B4(void *, int, int) __asm__("memset");
// Creates an XF handle object (CID 149) for the object's XF matching (oid, kind), for scripts.
struct Q *NewSoXfHandle(struct P *so, int k, int m)
{
    int i;
    for (i = 0; i < so->numTransforms; i++) {
        struct E *pe = &so->transforms[i];
        if (pe->kind == m && (k == -1 || pe->oid == k)) {
            struct Q *q = AllocZeroedFromSlotheap(&(*(struct Pool *)&g_sw.slotheapXfh));
            func_002333B4(q, 0, 0x30);
            q->pvt = D_002CA2C0;
            q->so = so;
            q->transform = pe;
            q->a = 0;
            q->b = 0;
            return q;
        }
    }
    return 0;
}
} } // namespace M_func_001F8510

// 0x1F85E0 NewSoPartHandle
namespace M_func_001F85E0 { extern "C" {
struct Q;
struct Q { int a, b, c; void *pvt; int pad[4]; void *object; void *part; int pad2[2]; };
extern char D_002CA280[] __asm__("_vt$6CID150");
struct Pool;
void *FindSoPart(void *);
struct Q *AllocZeroedFromSlotheap(struct Pool *);
void *func_002333B4(void *, int, int) __asm__("memset");
// Creates a part handle object (CID 150) for the part with the given oid.
struct Q *NewSoPartHandle(void *pa)
{
    void *r = FindSoPart(pa);
    struct Q *q;
    if (!r) return 0;
    q = AllocZeroedFromSlotheap(&(*(struct Pool *)&g_sw.slotheapXfh));
    func_002333B4(q, 0, 0x30);
    q->object = pa;
    q->part = r;
    q->pvt = D_002CA280;
    q->a = 0;
    q->b = 0;
    return q;
}
} } // namespace M_func_001F85E0

// 0x1F8668 SetSoBit35Hierarchy
namespace M_func_001F8668 { extern "C" {
struct L; struct G;
typedef unsigned long ulong;
struct L { char pad[0x24]; struct L *next; char pad2[0x64 - 0x28]; struct L *firstChild; char pad3[0x148 - 0x68]; ulong lo : 35; ulong bit35 : 1; ulong hi : 28; };
struct G { char pad[0x3E5C]; int unknown3E5C; };
extern struct G D_003DD490;
void AddLoToRenderTree(struct L *);
void RemoveLoFromRenderTree(struct L *);
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int);
// Sets bit 35 of +0x148 on the object and its children, registering them when the world requires it. (inferred)
void SetSoBit35Hierarchy(struct L *so, long f)
{
    struct L *q;
    if (f != so->bit35) {
        so->bit35 = f;
        if (D_003DD490.unknown3E5C) {
            if (f) AddLoToRenderTree(so);
            else RemoveLoFromRenderTree(so);
        }
        for (q = so->firstChild; q; q = q->next)
            if (IsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) SetSoBit35Hierarchy(q, f);
    }
}
} } // namespace M_func_001F8668

// 0x1F8730 IncrefXfHandle
// Handle slot 0: increments the reference count.
void CID148::AddRef()   /* key function: CID148's vtable comes out here */
{
    XFH *pxfh = (XFH *)this;
    pxfh->cref = pxfh->cref + 1;
}

// 0x1F8740 DecrefXfHandle
/* Drops a reference; the last one clears the splice frame and sidebag, unsubscribes and frees the handle. */
// Decrements the handle's reference count; at 0 destroys it and returns it to the pool.
extern "C" void DecrefXfHandle(XFH *pxfh)
{
    int cref;
    cref = pxfh->cref - 1;
    pxfh->cref = cref;
    if (cref <= 0) {
        FramePtr_Assign(&pxfh->scriptFrame, 0);
        AssignSidebagRef(&pxfh->scriptVariables, 0);
        UnsubscribeLoAll(pxfh);
        FreeSlotheapSlot(&g_sw.slotheapXfh, pxfh);
    }
}

// 0x1F87A0 GetXfHandleParamAC
namespace M_func_001F87A0 { extern "C" {
struct T; struct S;
struct T { char pad[0xAC]; unsigned short unknownAC; };
struct S { char pad[0x24]; struct T *transform; };
// Returns the XF's 16-bit field +0xAC in thousandths (meaning unknown).
float GetXfHandleParamAC(struct S *p) { return p->transform->unknownAC * 0.001f; }
} } // namespace M_func_001F87A0

// 0x1F87C8 SetXfHandleParamAC
namespace M_func_001F87C8 { extern "C" {
struct T; struct S;
struct T { char pad[0xAC]; short unknownAC; };
struct S { char pad[0x20]; void *so; struct T *transform; };
void UnsettleXfSlot(void *, struct T *);
// Sets the XF's field +0xAC (value * 1000) and refreshes its mode.
void SetXfHandleParamAC(struct S *p, float g) { p->transform->unknownAC = (int)(g * 1000.0f); UnsettleXfSlot(p->so, p->transform); }
} } // namespace M_func_001F87C8

// 0x1F8808 GetXfHandleParamAE
namespace M_func_001F8808 { extern "C" {
struct T; struct S;
struct T { char pad[0xAE]; unsigned short unknownAE; };
struct S { char pad[0x24]; struct T *transform; };
// Returns the XF's 16-bit field +0xAE in thousandths.
float GetXfHandleParamAE(struct S *p) { return p->transform->unknownAE * 0.001f; }
} } // namespace M_func_001F8808

// 0x1F8830 SetXfHandleParamAE
namespace M_func_001F8830 { extern "C" {
struct T; struct S;
struct T { char pad[0xAE]; short unknownAE; };
struct S { char pad[0x20]; void *so; struct T *transform; };
void UnsettleXfSlot(void *, struct T *);
// Sets the XF's field +0xAE (value * 1000) and refreshes its mode.
void SetXfHandleParamAE(struct S *p, float g) { p->transform->unknownAE = (int)(g * 1000.0f); UnsettleXfSlot(p->so, p->transform); }
} } // namespace M_func_001F8830

// 0x1F8870 GetXfHandleParamAC2
namespace M_func_001F8870 { extern "C" {
struct T; struct S;
struct T { char pad[0xAC]; unsigned short unknownAC; };
struct S { char pad[0x24]; struct T *transform; };
// Same as GetXfHandleParamAC for the other handle class.
float GetXfHandleParamAC2(struct S *p) { return p->transform->unknownAC * 0.001f; }
} } // namespace M_func_001F8870

// 0x1F8898 SetXfHandleParamAC2
namespace M_func_001F8898 { extern "C" {
struct T; struct S;
struct T { char pad[0xAC]; short unknownAC; };
struct S { char pad[0x20]; void *so; struct T *transform; };
void UnsettleXfSlot(void *, struct T *);
// Same as SetXfHandleParamAC for the other handle class.
void SetXfHandleParamAC2(struct S *p, float g) { p->transform->unknownAC = (int)(g * 1000.0f); UnsettleXfSlot(p->so, p->transform); }
} } // namespace M_func_001F8898

// 0x1F88D8 GetXfFromHandle
// Returns the handle's XF, refreshing its mode first when asked.
extern "C" XF *GetXfFromHandle(XFH *pxfh, int n)
{
    if (n == 1) {
        UnsettleXfSlot(pxfh->pso, pxfh->pxf);
    }
    return pxfh->pxf;
}

// 0x1F8910 FindXfHandleActuator
namespace M_func_001F8910 { extern "C" {
struct E; struct Q; struct P;
struct E { int kind; int pad[2]; int priority; struct E *next; };
struct Q { char pad[0x90]; struct E *firstAct; };
struct P { char pad[0x20]; void *so; struct Q *transform; };
void RemoveXfAct(void *, struct Q *, struct E *);
void FreeAct(struct E *);
void SortXfActs(void *, struct Q *);
void SetXfRole(void *, struct Q *, int);
void UnsettleXfSlot(void *, struct Q *);
// Finds the XF's actuator of kind k; n = -1 removes it, otherwise it is updated and re-prioritised.
struct E *FindXfHandleActuator(struct P *handle, int k, int n, int m)
{
    struct E *pe;
    for (pe = handle->transform->firstAct; pe; pe = pe->next) {
        if (pe->kind != k) continue;
        if (n == -1) {
            RemoveXfAct(handle->so, handle->transform, pe);
            FreeAct(pe);
            return 0;
        }
        if (m != pe->priority) {
            pe->priority = m;
            SortXfActs(handle->so, handle->transform);
        }
        if (pe == handle->transform->firstAct) {
            SetXfRole(handle->so, handle->transform, n);
            UnsettleXfSlot(handle->so, handle->transform);
        }
        return pe;
    }
    return 0;
}
} } // namespace M_func_001F8910

// 0x1F89E0 SetXfHandleActuator1
namespace M_func_001F89E0 { extern "C" {
struct R; struct P;
typedef int qword __attribute__((mode(TI)));
struct R { char pad[0x14]; int role; };
struct P { char pad[0x20]; void *so; void *transform; };
struct R *FindXfHandleActuator(struct P *, int, int, int);
void SetActPositionGoal(struct R *, qword);
void NewPositionAct(void *, void *, int, int, int, qword);
// Updates or creates the XF's kind-1 actuator (a target vector) with its parameters.
void SetXfHandleActuator1(struct P *handle, int n, int m, qword q)
{
    struct R *r = FindXfHandleActuator(handle, 1, n, m);
    if (r) {
        r->role = n;
        SetActPositionGoal(r, q);
    } else if (n != -1) {
        NewPositionAct(handle->so, handle->transform, m, n, 2, q);
    }
}
} } // namespace M_func_001F89E0

// 0x1F8A78 SetXfHandleActuator1World
namespace M_func_001F8A78 { extern "C" {
struct B; struct A;
typedef int qword __attribute__((mode(TI)));
struct B { char pad[0xA0]; int n; };
struct A { char pad[0x24]; struct B *pb; };
qword TransformXfPos(int a, int n, qword q);
void SetXfHandleActuator1(struct A *p, int a, int b, qword q);
// SetXfHandleActuator1 with a vector converted from the XF's parent space.
void SetXfHandleActuator1World(struct A *handle, int a, int b, qword q)
{
    SetXfHandleActuator1(handle, a, b, TransformXfPos(0, handle->pb->n, q));
}
} } // namespace M_func_001F8A78

// 0x1F8AD8 SetXfHandleActuator1Mat
namespace M_func_001F8AD8 { extern "C" {
struct M3; struct O; struct P;
typedef int qword __attribute__((mode(TI)));
struct M3 { qword a[3]; };
struct O { char pad[0xA0]; int parent; };
struct P { char pad[0x24]; struct O *transform; };
void ConvertXfMat(struct M3 *, int, int);
qword DecomposeRotationMatrixToEuler(struct M3 *);
void SetXfHandleActuator1(struct P *, int, int, qword);
// SetXfHandleActuator1 with the value taken from a matrix.
void SetXfHandleActuator1Mat(struct P *handle, int a, int b)
{
    struct M3 c, t;
    ConvertXfMat(&t, 0, handle->transform->parent);
    c = t;
    SetXfHandleActuator1(handle, a, b, DecomposeRotationMatrixToEuler(&c));
}
} } // namespace M_func_001F8AD8

// 0x1F8B58 SetXfHandleActuator4
namespace M_func_001F8B58 { extern "C" {
struct R; struct P;
struct R { char pad[0x14]; int role; int pad2; float unknown1C; float unknown20; };
struct P { char pad[0x20]; void *so; void *transform; };
struct R *FindXfHandleActuator(struct P *, int, int, int);
void NewPathAct(void *, void *, int, int, float, float);
// Updates or creates the XF's kind-4 actuator with two floats.
void SetXfHandleActuator4(struct P *handle, int n, int m, float f, float g)
{
    struct R *r = FindXfHandleActuator(handle, 4, n, m);
    if (r) {
        r->role = n;
        r->unknown1C = f;
        r->unknown20 = g;
    } else if (n != -1) {
        NewPathAct(handle->so, handle->transform, m, n, f, g);
    }
}
} } // namespace M_func_001F8B58

// 0x1F8BF0 SetXfHandleKind16Params
namespace M_func_001F8BF0 { extern "C" {
struct U; struct V; struct T; struct S;
typedef int qword __attribute__((mode(TI)));
struct U { int a, b; int n; float x, y, z, r; int pad; qword q; };
struct V { char pad[0x60]; struct U *pu; };
struct T { char pad[0x98]; short k; char pad2[0xE]; struct V *pv; };
struct S { char pad[0x24]; struct T *pt; };
// For a kind-16 XF: sets its count, vector, angle (degrees to radians) and rate.
void SetXfHandleKind16Params(struct S *handle, int n, qword q, float x, float y, float z, float deg)
{ struct T *pt = handle->pt; if (pt->k == 16) { struct U *pu = pt->pv->pu; pu->n = n; pu->x = x; pu->y = y; pu->z = z; pu->r = deg * 0.01745329424738884f; pu->q = q; } }
} } // namespace M_func_001F8BF0




// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
float D_002C3B90 = 400.0f;
VECTOR4 D_002C3BA0[8] = {{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f, 0.0f}};
int D_002C3C20[8] = {0, 3, 2, 2, 2, 1, 1, 1};

// ---- Pointer constants and records, by address name (docs/guides/tu-merging.md "Data"): words as the ELF has
// them, pointers through asm-labelled declarations. Non-virtual pointer-to-member constants are 8 bytes
// {delta 0, index -1, pfn}; the definitions sit in their own namespace so no declaration above has to agree.
namespace ANPTR_so {
void p_func_001F3868() __asm__("UpdateSoPhysRegionCallback");
void p_func_001F7950() __asm__("HandleSoTouchZaps");
extern "C" {
extern const unsigned int D_002A8270[2] = {0xFFFF0000, (unsigned int)p_func_001F3868};
extern const unsigned int D_002A8278[2] = {0xFFFF0000, (unsigned int)p_func_001F7950};
}
} // namespace ANPTR_so
