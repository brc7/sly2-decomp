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
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_so
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
#include "sly2/classes.h"
#include "sly2/light.h"
#include "sly2/vu0.h"
#include "sly2/act.h"
#include "sly2/dli.h"
#include "sly2/clock.h"
#include "sly2/vtables.h"

extern "C" void *func_002333B4(void *pv, int b, int cb) __asm__("memset");   /* memset */
extern qword g_normalZ;
/* 0x1F8C40 StaticInit_so (93 instructions) and its stub GlobalConstructors_so: g++'s initializer for the file-scope
   object below (built in a zeroed temporary and copied), then the six engine slot pools. D_003DD020 is the default
   query record that cnvo and guardbhv pass to FindShapeClosestPoint (role HYPOTHESIS; the name is a placeholder). */
struct CPQ
{
    qword axis;             /* g_normalZ (the up axis) */
    int unknown10;          /* 1 */
    int pad[3];
    // Zero the whole CPQ.
    CPQ() { func_002333B4(this, 0, sizeof(CPQ)); }
};
// A zeroed CPQ with axis g_normalZ and its unknown10 flag set.
static inline CPQ MakeCpq()
{
    CPQ cpq;
    cpq.axis = g_normalZ;
    cpq.unknown10 = 1;
    return cpq;
}
CPQ D_003DD020 = MakeCpq();
SLY2_INSTANTIATE_SLOT_POOLS()

// Types of the Lilly ports (merged copies)


void CloneGeom(GEOM *pgeomSrc, MATRIX4 *pdmat, GEOM *pgeomDst);

void ClearDle(DLE *pdle);


extern "C" void AddXfAct(SO *pso, XF *pxf, ACT *pact);

ACT *PactNewClone(ACT *pactBase, SO *pso, XF *pxf) __asm__("NewActFromParams");



void *PvAllocSwCopyImpl(struct SWALLOC *pswalloc, void *pvBase) __asm__("AllocCopyFromWorldHeap");


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

struct VU_VECTOR4
{
    qword qw;
};

/* SO::psobounds is sly2/light.h's SOBOUNDS. */

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
void RenderSoGlobset(GLOBSET *pglobset, MATRIX4 *pmatWorld, float uAlpha1, float uAlpha2, SOBOUNDS *psobounds);
void BuildSoLightPackets(SW *pg, MATRIX4 *pmatWorld, SOBOUNDS *pq);
qword ConvertXfVec(XF *pxfFrom, XF *pxfTo, qword qw);
void UnsettleXfSlot(SO *pso, XF *pxf);
void AddObjectToBroadPhase(SW *psw, SO *pso);
void SetXfRole(SO *pso, XF *pxf, int n);
void GetSoConstraintAxis(SO *pso, XF *pxf, CONSTR *pconstr, CONSTR *pconstrSo);
void RebuildSoPhysHook(SO *pso);
void SetSoBit35Hierarchy(SO *pso, int n);
void FreeSoContacts(SO *pso);
TARGETREC *FindTargetRecordById(LO *plo);
void *func_001AE4F8(int cb) __asm__("__builtin_new");
TARGETREC *InitTargetRecord(TARGETREC *ptargetrec, LO *plo);
REGENT *EnsureSoPhysRegionRecord(STEP *pstep);
void FramePtr_Assign(void *ppframe, CFrame *pframe) __asm__("__as__t7CRefPtr1Z6CFrameP6CFrame");
void AssignSidebagRef(void *ppsidebag, CSidebag *psidebag);
void UnsubscribeLoAll(CID148 *pxfh);
void FreeSlotheapSlot(SLOTHEAP *pslotheap, void *pv);
}

// 0x1F2108 InitSo
namespace M_InitSo {
extern float D_002C3B90 __asm__("D_002C3B90");
void InitLo(SO *) __asm__("InitLo");
void SetLoBusyKind(SO *, int) __asm__("SetLoBusyKind");
void InitDl__FP2DLi(void *, int) __asm__("InitDl__FP2DLi");
// Slot 7 (Init): InitLo, not busy, empty child list, hit damages 10/20, alpha 1, the default bounds scale, and the
// bit-35 flag inherited from the parent.
// Also known as: InitSo (upstream readable / Sly 1 name; InitSo__FP2SO).
extern "C" void InitSo(SO *so)
{
    InitLo(so);
    SetLoBusyKind(so, 0);
    InitDl__FP2DLi(&so->children, SLY2_OFFSETOF(LO, siblingLink));
    so->dhpHit = -10;
    so->dhpTouch = -20;
    so->opacity = 1.0f;
    so->boundsScale = D_002C3B90;
    so->isRenderable = (uchar)(so->parent ? so->parent->isRenderable : 1);
    so->renderStamp = -1;
}
} // namespace M_InitSo

// 0x1F21B8 SetSoParent
namespace M_SetSoParent {

void __ml__FRC7MATRIX4T0(MATRIX4 *, XF *, MATRIX4 *) SLY2_SRET_LABEL("__ml__FRC7MATRIX4T0", "MulMatrix4Into");
void LO_SetParent(SO *, SO *, XF *, int) __asm__("LO_SetParent");
void ReleaseSoBoundsHierarchy(SO *) __asm__("ReleaseSoBoundsHierarchy");
void EnsureSoBoundsHierarchy(SO *) __asm__("EnsureSoBoundsHierarchy");


// Slot 10 (SetParent): remembers the world matrix and velocities, re-parents the LO, re-points the XFs into the new
// parent, then rebinds contacts and bounds so the object keeps its world pose and motion.
// Also known as: SetAloParent (upstream readable / Sly 1 name; SetAloParent__FP3ALOT0P2XFi).
extern "C" void SetSoParent(SO *plo, SO *pa, XF *pb, int flags)
{
    MATRIX4 mat;
    qword q1, q2;
    XF *pv;
    int c, i;
    XF *aeBase;
    if (flags & 3) {
        bool f = plo->isInWorld; if (f) {
            mat = plo->rootTransform->worldMatrix;
        } else {
            XF *palo;
            func_002333B4(&mat, 0, 0x40);
            mat.mat[0][0] = mat.mat[1][1] = mat.mat[2][2] = mat.mat[3][3] = 1.0f;
            for (palo = plo->rootTransform; palo; palo = palo->parent) {
                MATRIX4 t;
                __ml__FRC7MATRIX4T0(&t, palo, &mat);
                mat = t;
            }
        }
    }
    if (flags & 4) q1 = plo->GetVelocity();
    if (flags & 8) q2 = plo->GetAngularVelocity();
    if (plo->parent) {
        c = plo->parent->numTransforms;
        aeBase = plo->parent->transforms;
    } else {
        c = 1;
        aeBase = 0;
    }
    if (pb) pv = pb; else if (pa) pv = pa->rootTransform; else pv = 0;
    for (i = 0; i < plo->numTransforms; i++) {
        XF *pe = &plo->transforms[i];
        int d = pe->parent - aeBase;
        if (d >= 0 && d < c) pe->parent = pv;
    }
    LO_SetParent(plo, pa, pb, flags);
    UpdateSoXfWorldMatrices(plo);
    if (flags & 1) plo->TranslateToPos(*(qword *)mat.mat[3]);
    if (flags & 2) plo->RotateToMat((MATRIX3 *)&mat);
    if (flags & 4) plo->SetVelocity(q1);
    if (flags & 8) plo->SetAngularVelocity(q2);
    ReleaseSoBoundsHierarchy(plo);
    EnsureSoBoundsHierarchy(plo);
    if (plo->parent && !plo->parent->isRenderable) SetSoBit35Hierarchy(plo, 0);
}
} // namespace M_SetSoParent

// 0x1F2468 ReleaseSoBoundsHierarchy
namespace M_ReleaseSoBoundsHierarchy {
struct Pool;
void FreeSlotheapSlot(struct Pool *, SOBOUNDS *) __asm__("FreeSlotheapSlot");
int IsBasicDerivedFrom__FP5BASIC3CID(LO *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
// Drops this object's (and its SO children's) reference to the shared bounds record (+0x134, refcounted pool).
extern "C" void ReleaseSoBoundsHierarchy(SO *so)
{
    LO *q;
    SOBOUNDS *pr = so->psobounds;
    if (pr) {
        if (--pr->refCount == 0) FreeSlotheapSlot(&(*(struct Pool *)&g_sw.slotheapSobounds), pr);
        so->psobounds = 0;
    }
    for (q = (LO *)so->children.head; q != so->ploRemovedFirst; q = (LO *)q->siblingLink.next) {
        if (IsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) ReleaseSoBoundsHierarchy((SO *)q);
    }
}
} // namespace M_ReleaseSoBoundsHierarchy

// 0x1F2500 EnsureSoBounds
namespace M_EnsureSoBounds {
typedef unsigned long ulong;
SOBOUNDS *AllocZeroedFromSlotheap(void *) __asm__("AllocZeroedFromSlotheap");
qword GetLoWorldPos(SO *) __asm__("GetLoWorldPos");
// Distance between a and b (xyz) as a float.
extern "C" { static inline float Distance(qword a,qword b) {
 qword d,one;
 float result;
 __asm__("vsub.xyzw %0,%1,%2":"=j"(d):"j"(a),"j"(b));
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"0"(d));
 __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(d));
 __asm__("mtc1 %1,%0":"=f"(result):"r"(d));
 return result;
} }
// Load the 64-bit bitfield unit at +0x148 (SO::soFlags: dhpHit, dhpTouch, unknownBits148A/B, ...) as one doubleword.
extern "C" { static inline ulong LoadState(SO *p)
{
    register ulong value __asm__("$2");
    __asm__("ld %0,%1":"=r"(value):"m"(p->soFlags));
    return value;
} }
// Attaches the object to its root's bounds record (allocating one if needed) and grows the record's radii to cover
// the object's radius plus its distance from the root's origin.
extern "C" void EnsureSoBounds(SO *so) {
 SO *top;
 SOBOUNDS *attached;
 if (!(so->model.grf50 & (3UL<<32))) return;
 top=so;
 attached=so->psobounds;
 if(!attached) {
  do {
   register SO *next __asm__("$4")=top->parent;
   register int one __asm__("$3")=1;
   if(!next || ((int)((unsigned int)(int)LoadState(top)<<8)>>28)==one) {
     register char*pool __asm__("$4");
     SOBOUNDS *s;
     __asm__("lui %0,0x3e":"=r"(pool));
     s=AllocZeroedFromSlotheap(pool+0x12D4);
     top->psobounds=s;
     so->psobounds=s;
     s->pposCenter=&top->rootTransform->worldMatq.r[3].q;
     ++so->psobounds->refCount;
     so->psobounds->scale=so->boundsScale;
     so->psobounds->pso=so;
     if(top==so)goto position;
     attached=so->psobounds;
     goto count;
   }
   top=next;
   attached=top->psobounds;
  } while(!attached);
  so->psobounds=attached;
 }
count:
 ++attached->refCount;
position:

 {
   qword position=GetLoWorldPos(so);
   float radius=so->model.boundsRadius+Distance(position,*so->psobounds->pposCenter);
   if(so->model.grf50&(1UL<<32)) {
     if(so->psobounds->strength<radius) so->psobounds->strength=radius;
   }
   if(so->model.grf50&(1UL<<33)) {
     if(so->psobounds->sRadius<radius) so->psobounds->sRadius=radius;
   }
 }
}

} // namespace M_EnsureSoBounds

// 0x1F2688 SetSoBoundsScale
/* Sets boundsScale, and the bounds record's copy when the record is this object's. */
// Stores the bounds scale (+0x13C, 400 by default) and mirrors it into the shared bounds record when this object
// owns it (inferred meaning).
extern "C" void SetSoBoundsScale(SO *so, float g)
{
    SOBOUNDS *psobounds;
    psobounds = so->psobounds;
    so->boundsScale = g;
    if ((psobounds != 0) && (psobounds->pso == so)) {
        psobounds->scale = g;
    }
}

// 0x1F26A8 EnsureSoBoundsHierarchy
namespace M_EnsureSoBoundsHierarchy {
int IsBasicDerivedFrom__FP5BASIC3CID(LO *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
// EnsureSoBounds on this object and recursively on its SO children.
extern "C" void EnsureSoBoundsHierarchy(SO *so)
{
    LO *q;
    EnsureSoBounds(so);
    for (q = (LO *)so->children.head; q != so->ploRemovedFirst; q = (LO *)q->siblingLink.next) {
        if (IsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) EnsureSoBoundsHierarchy((SO *)q);
    }
}
} // namespace M_EnsureSoBoundsHierarchy

// 0x1F2718 SetSoUnknownBits20
namespace M_func_001F2718 {
// Sets the 4-bit field at bits 20-23 of the flag word +0x148 (so.h unknownBits148B; meaning unknown, a splice option).
extern "C" void SetSoUnknownBits20(SO *p, int n) { p->unknownBits148B = n; }
} // namespace M_func_001F2718

// 0x1F2740 GetSoUnknownBits20
namespace M_func_001F2740 {
// Returns the 4-bit field at bits 20-23 of +0x148.
extern "C" int GetSoUnknownBits20(SO *p) { return p->unknownBits148B; }
} // namespace M_func_001F2740

// 0x1F2758 GetSoIgnoreLocked
namespace M_GetSoIgnoreLocked {
// Returns the ignore-locked flag (bit 34 of +0xD0).
extern "C" int GetSoIgnoreLocked(SO *p) { return (int)p->model.ignoreLocked & 1; }
} // namespace M_GetSoIgnoreLocked

// 0x1F2768 SetSoIgnoreLocked__FP2SOi
// SO+0xD0 bit 34 = GLOBSET+0x50 bit 34 (Lilly's port: globset.ignoreLocked).
// Sets the ignore-locked flag (bit 34 of +0xD0). Sly 1 name.
void SetSoIgnoreLocked(SO *pso, int f) { pso->model.ignoreLocked = f; }

// 0x1F2798 FindSoPhysRoot
namespace M_PsoFindPhysRoot {
// Walks up the parent chain to the first SO whose physics is not fixed (SOPHYS modes != 3) or that is marked as a
// tree root (+0x148 bit 34); returns the last object visited. That object owns the contact tree.
// Also known as: PaloPhysRoot (upstream readable / Sly 1 name; PaloPhysRoot__FP3ALO).
extern "C" SO *FindSoPhysRoot(SO *so)
{
    SO *r = 0;
    while (so) {
        r = so;
        if (so->physics && (so->physics->constrForce.kind != 3 || so->physics->constrTorque.kind != 3))
            break;
        if (so->isTreeRoot)
            break;
        so = so->parent;
    }
    return r;
}
} // namespace M_PsoFindPhysRoot

// 0x1F27F0 EnsureSoTree
/* Makes sure the object has a tree record: its physics root's (created by SO slot 53 when missing). */
// Gives the object a contact-tree record (+0xEC): shares its physics root's, creating it (slot 53) and registering
// it with the broad phase when needed.
// Also known as: PoxaEnsure (upstream readable / Sly 1 name; PoxaEnsure__FP3ALO).
extern "C" void EnsureSoTree(SO *so)
{
    SO *psoRoot;
    if (so->physicsTree == 0) {
        psoRoot = FindSoPhysRoot(so);
        if (psoRoot->physicsTree == 0) {
            psoRoot->physicsTree = psoRoot->NewTree();
            RefreshSwObjectCollisionFilter(&g_sw, psoRoot);
        }
        so->physicsTree = psoRoot->physicsTree;
    }
}

// 0x1F2870 AddSoPartToTree
namespace M_AddSoPartToTree {
int IsEntryInDl__FP2DLPv(void *, void *) __asm__("IsEntryInDl__FP2DLPv");
void AppendDlEntry__FP2DLPv(void *, void *) __asm__("AppendDlEntry__FP2DLPv");
void UpdateSoPartGeom(SOPART *) __asm__("UpdateSoPartGeom");
void WakeSo(SO *) __asm__("WakeSo");
// Adds one part to the tree's part list: copies the XF matrix and stamp, marks the tree dirty, transforms the
// part's geometry to world space and wakes the tree if the object is not fixed.
// Also known as: AddSoeToOxa (upstream readable / Sly 1 name; AddSoeToOxa__FP3ALOP3SOE).
extern "C" void AddSoPartToTree(SO *so, SOPART *pe)
{
    XF *po;
    EnsureSoTree(so);
    if (IsEntryInDl__FP2DLPv(&so->physicsTree->parts, pe)) return;
    AppendDlEntry__FP2DLPv(&so->physicsTree->parts, pe);
    po = pe->transform;
    pe->meshStamp = -1;
    pe->matWorld = po->worldMatrix;
    pe->worldStamp = po->worldStamp;
    InvalidateSoAaox(so);
    UpdateSoPartGeom(pe);
    if (!so->IsFixed()) WakeSo(so);
}
} // namespace M_AddSoPartToTree

// 0x1F2930 RemoveSoPartFromTree
namespace M_RemoveSoPartFromTree {
int IsEntryInDl__FP2DLPv(void *, void *) __asm__("IsEntryInDl__FP2DLPv");
void RemoveDlEntry__FP2DLPv(void *, void *) __asm__("RemoveDlEntry__FP2DLPv");
int IsDlEmpty__FP2DL(void *) __asm__("IsDlEmpty__FP2DL");
void RemoveSwPlacesOf(SW *, int, SOPART *) __asm__("RemoveSwPlacesOf");
void RemoveObjectFromBroadPhase(SW *, SOTREE *) __asm__("RemoveObjectFromBroadPhase");
void WakeSo(SO *) __asm__("WakeSo");
// Removes a part from the tree's part list and frees its contacts; drops the tree when it becomes empty, or detaches
// the object when none of its own parts remain.
// Also known as: RemoveSoeFromOxa (upstream readable / Sly 1 name; RemoveSoeFromOxa__FP3ALOP3SOE).
extern "C" void RemoveSoPartFromTree(SO *so, SOPART *pe)
{
    int i;
    if (!so->physicsTree) return;
    if (!IsEntryInDl__FP2DLPv(&so->physicsTree->parts, pe)) return;
    RemoveDlEntry__FP2DLPv(&so->physicsTree->parts, pe);
    if (pe->hasPlaces) RemoveSwPlacesOf(&g_sw, 0, pe);
    FreeSoContacts(so);
    if (IsDlEmpty__FP2DL(&so->physicsTree->parts)) {
        SOTREE *pq = so->physicsTree;
        pq->root->physicsTree = 0;
        so->physicsTree = 0;
        RemoveObjectFromBroadPhase(&g_sw, pq);
    } else if (so->physicsTree->root != so) {
        i = so->numParts;
        while (--i >= 0) {
            if (IsEntryInDl__FP2DLPv(&so->physicsTree->parts, (SOPART *)(i * (int)sizeof(SOPART) + (sptr)so->parts))) break;
        }
        if (i < 0) so->physicsTree = 0;
    }
done:
    if (!so->IsFixed()) WakeSo(so);
}
} // namespace M_RemoveSoPartFromTree

// 0x1F2A60 ShowSoPart
namespace M_ShowSoPart {
extern void AddSoPartToTree(SO *, SOPART *) __asm__("AddSoPartToTree");
// Un-hides a part and, when the object is in the world, adds it to the tree.
extern "C" void ShowSoPart(SO *pa, SOPART *pb)
{
    if (pb->isHidden) { pb->isHidden = 0;
    bool f = pa->isInWorld; if (f) AddSoPartToTree(pa, pb); }
}
} // namespace M_ShowSoPart

// 0x1F2AD0 HideSoPart
namespace M_HideSoPart {
void RemoveSoPartFromTree(SO *, SOPART *) __asm__("RemoveSoPartFromTree");
// Hides a part and, when the object is in the world, removes it from the tree.
// Also known as: SetSoeOff (upstream readable / Sly 1 name; SetSoeOff__FP3ALOP3SOE).
extern "C" void HideSoPart(SO *so, SOPART *pt)
{
    if (!pt->isHidden) {
        pt->isHidden = 1;
        if ((int)so->isInWorld & 1) RemoveSoPartFromTree(so, pt);
    }
}
} // namespace M_HideSoPart

// 0x1F2B20 AddSoParts
namespace M_AddSoParts {
void AddSoPartToTree(SO *, SOPART *) __asm__("AddSoPartToTree");
// Adds every visible part to the object's tree.
// Also known as: AddAloSoes (upstream readable / Sly 1 name; AddAloSoes__FP3ALO).
extern "C" void AddSoParts(SO *so)
{
    int i;
    for (i = 0; i < so->numParts; i++) {
        SOPART *pe = &so->parts[i];
        if (!pe->isHidden) AddSoPartToTree(so, pe);
    }
}
} // namespace M_AddSoParts

// 0x1F2BA8 RemoveSoParts
namespace M_RemoveSoParts {
void RemoveSoPartFromTree(SO *, SOPART *) __asm__("RemoveSoPartFromTree");
// Removes every visible part from the object's tree.
// Also known as: RemoveAloSoes (upstream readable / Sly 1 name; RemoveAloSoes__FP3ALO).
extern "C" void RemoveSoParts(SO *so)
{
    int i;
    for (i = 0; i < so->numParts; i++) {
        SOPART *pe = &so->parts[i];
        if (!pe->isHidden) RemoveSoPartFromTree(so, pe);
    }
}
} // namespace M_RemoveSoParts

// 0x1F2C30 AddSoHierarchy
namespace M_AddSoHierarchy {
struct S8; struct H; 
struct S8 { long l; } __attribute__((packed));
struct H { char pad[0x2C]; int numZones; };
extern const struct S8 D_002A8270 __asm__("D_002A8270");
void AddLoHierarchy(SO *) __asm__("AddLoHierarchy");
void AddSoParts(SO *) __asm__("AddSoParts");
qword GetLoWorldPos(SO *) __asm__("GetLoWorldPos");
void *FindPathzoneAtPos(qword, int) __asm__("FindPathzoneAtPos");
void SetSoPhysRegion(SO *, void *) __asm__("SetSoPhysRegion");
void PostSwCallback(SW *, SO *, long, int) __asm__("PostSwCallback");
// Slot 11 (AddHierarchy): AddLoHierarchy, adds the parts, sets flags 0x14, registers the physics region (if the
// object has a region record) and recurses into the children.
// Also known as: OnSoAdd (upstream readable / Sly 1 name; OnSoAdd__FP2SO).
extern "C" void AddSoHierarchy(SO *so)
{
    LO *q;
    void *pv;
    int unused[4];
    sptr v;
    AddLoHierarchy(so);
    UpdateSoXfWorldMatrices(so);
    AddSoParts(so);
    v = (sptr)so->ploupd;
    if ((v ^ 1) & 1) so->ploupd->grf |= 0x14;
    else so->inlineFlags = v | 0x14;
    pv = so->physics ? so->physics->pregent : 0;
    if (pv) {
        if (((struct H *)g_sw.pregtree) && ((struct H *)g_sw.pregtree)->numZones)
            SetSoPhysRegion(so, FindPathzoneAtPos(GetLoWorldPos(so), 1));
        else
            PostSwCallback(&g_sw, so, D_002A8270.l, 0);
    }
    for (q = (LO *)so->children.head; q != so->ploRemovedFirst; q = (LO *)q->siblingLink.next)
        q->AddHierarchy();
}
} // namespace M_AddSoHierarchy

// 0x1F2D48 RemoveSoHierarchy
namespace M_RemoveSoHierarchy {
struct K;
typedef unsigned long ulong;
struct K { long l; } __attribute__((packed));
extern const struct K D_002A8270 __asm__("D_002A8270");
void RemoveLoHierarchy(SO *) __asm__("RemoveLoHierarchy");
void RemoveSoParts(SO *) __asm__("RemoveSoParts");
void ClearSwCallbacks(SW *, int, SO *, long, int) __asm__("ClearSwCallbacks");
void SetSoPhysRegion(SO *, int) __asm__("SetSoPhysRegion");
// Slot 12 (RemoveHierarchy): RemoveLoHierarchy, removes the parts, resets the fade, unregisters the region callback
// and recurses into the children.
// Also known as: OnSoRemove (upstream readable / Sly 1 name; OnSoRemove__FP2SO).
extern "C" void RemoveSoHierarchy(SO *so)
{
    DLI dli;
    LO *po;
    REGENT *n;
    RemoveLoHierarchy(so);
    RemoveSoParts(so);
    so->opacitySpeed = 0;
    so->opacity = 1.0f;
    n = 0;
    if (so->physics) n = so->physics->pregent;
    if (n) {
        ClearSwCallbacks(&g_sw, 3, so, D_002A8270.l, 0);
        SetSoPhysRegion(so, 0);
    }
    InitDli(&dli, &so->children);
    while ((po = (__typeof__(po))PvNextDli(&dli)) != 0 && !po->isRemoved)
        po->RemoveHierarchy();
    g_pdliFirst = dli.m_pdliNext;
}
} // namespace M_RemoveSoHierarchy

// 0x1F2E78 UpdateSoLoroot
namespace M_UpdateSoLoroot {
void UpdateLoLoroot(SO *, LO *, SO *) __asm__("UpdateLoLoroot");
void RemoveSoParts(SO *) __asm__("RemoveSoParts");
void AddSoParts(SO *) __asm__("AddSoParts");
// Nearest ancestor of x (or x itself) with flag bit 56 set; null if none.
extern "C" { static inline LO *Find(LO *x)
{
    while (x && !x->isRemoved) x = x->parent;
    return x;
} }
// Slot 13: UpdateLoroot recursively; when the object has parts and its root changed, re-adds them to the new tree.
// Also known as: MoveAloPhys (upstream readable / Sly 1 name; MoveAloPhys__FP3ALON20).
extern "C" void UpdateSoLoroot(SO *so, LO *pNew, SO *pOld)
{
    LO *po;
    int unused[4];
    UpdateLoLoroot(so, pNew, pOld);
    for (po = (LO *)so->children.head; po; po = (LO *)po->siblingLink.next)
        po->UpdateLoroot(pNew, pOld);
    if (!so->numParts) return;
    for (po = so; ; po = po->parent) {
        if (po->isRemoved) return;
        if (po == pNew) break;
    }
    {
    LO *a = Find(pOld);
    LO *b = Find(so->parent);
    if (!a && !b) {
        RemoveSoParts(so);
        AddSoParts(so);
    }
    }
}
} // namespace M_UpdateSoLoroot

// 0x1F2FE0 SetSoGrf
namespace M_SetSoGrf {
// Slot 47: sets flags in the LO flag word (+0x40) on this object and all children.
// Also known as: MarkAloXfs (upstream readable / Sly 1 name; MarkAloXfs__FP3ALOUi).
extern "C" void SetSoGrf(SO *so, int f)
{
    LO *q;
    int unused[4];
    sptr v = (sptr)so->ploupd;
    if ((v ^ 1) & 1) so->ploupd->grf |= f;
    else so->inlineFlags = v | f;
    for (q = (LO *)so->children.head; q != so->ploRemovedFirst; q = (LO *)q->siblingLink.next)
        q->SetFlags(f);
}
} // namespace M_SetSoGrf

// 0x1F3078 ClearSoGrf
namespace M_ClearSoGrf {
// Slot 48: clears flags in the LO flag word on this object and all children.
extern "C" void ClearSoGrf(SO *so, int f)
{
    LO *q;
    int unused[4];
    sptr v = (sptr)so->ploupd;
    if ((v ^ 1) & 1) so->ploupd->grf &= ~f;
    else so->inlineFlags = v & ~f;
    for (q = (LO *)so->children.head; q != so->ploRemovedFirst; q = (LO *)q->siblingLink.next)
        q->ClearFlags(f);
}
} // namespace M_ClearSoGrf

// 0x1F3118 SetSoBusyDistance
namespace M_func_001F3118 {
extern void RemoveLoFromRenderTree(SO *) __asm__("RemoveLoFromRenderTree");
extern void AddLoToRenderTree(SO *) __asm__("AddLoToRenderTree");
extern void ResortSwBusyEntry(SW *, LOROOT *) __asm__("ResortSwBusyEntry");
// Slot 42: stores the busy distance (+0x38), refreshes the object's registration, re-sorts its root in the world
// and pushes the smaller value to the children (inferred).
extern "C" void SetSoBusyDistance(SO *so, float t)
{
    LO *q;
    int unused[4];
    so->busyRadius = t;
    if (so->renderStamp != -1) {
        RemoveLoFromRenderTree(so);
        AddLoToRenderTree(so);
    }
    if (!so->parent) {
        bool f = so->isInWorld;
        if (f) ResortSwBusyEntry(&g_sw, so->root);
    }
    for (q = (LO *)so->children.head; q; q = (LO *)q->siblingLink.next) {
        if (t < q->busyRadius) q->SetBusyDistance(t);
    }
}
} // namespace M_func_001F3118

// 0x1F31E0 LoadSoFromBrx
namespace M_LoadSoFromBrx {
typedef unsigned long ulong;
typedef unsigned int uint;
extern XF D_002C44F0 __asm__("D_002C44F0");   /* g_xfWorld (xf.cpp) */

void LoadSoXfsFromBrx(SO *,void *) __asm__("LoadSoXfsFromBrx");
int U32Read__18CBinaryInputStream(void *) __asm__("U32Read__18CBinaryInputStream");
int S16Read__18CBinaryInputStream(void *) __asm__("S16Read__18CBinaryInputStream");
int S32Read__18CBinaryInputStream(void *) __asm__("S32Read__18CBinaryInputStream");
int S8Read__18CBinaryInputStream(void *) __asm__("S8Read__18CBinaryInputStream");
float F32Read__18CBinaryInputStream(void *) __asm__("F32Read__18CBinaryInputStream");
void ReadGeom__18CBinaryInputStreamP4GEOM(void *,GEOM *) __asm__("ReadGeom__18CBinaryInputStreamP4GEOM");
void CBinaryInputStream_ReadBspc(void *,GEOM *,BSPC *) __asm__("CBinaryInputStream_ReadBspc");
void ReadMatrix__18CBinaryInputStreamP7MATRIX3(void *,MATRIX3 *) __asm__("ReadMatrix__18CBinaryInputStreamP7MATRIX3");
void LoadGlobsetFromBrx__FP7GLOBSETP2SOP18CBinaryInputStream(GLOBSET *,SO *,void *) __asm__("LoadGlobsetFromBrx__FP7GLOBSETP2SOP18CBinaryInputStream");
void *AllocClearedFromWorldHeap(SWALLOC *) __asm__("AllocClearedFromWorldHeap");
void AddSoParts(SO *) __asm__("AddSoParts");
void LoadOptionsFromBrx__FPvP18CBinaryInputStream(SO *,void *) __asm__("LoadOptionsFromBrx__FPvP18CBinaryInputStream");
void LoadSwObjectsFromBrx__FP2SOP18CBinaryInputStreamP14CLoadCollector(SO *,void *,void *) __asm__("LoadSwObjectsFromBrx__FP2SOP18CBinaryInputStreamP14CLoadCollector");
void SetLoVisMask(SO *,int) __asm__("SetLoVisMask");

/* LO::inlineFlags is a tagged word: bit 0 clear means it points at the flag word */
/* GetXfFromIndex (slot 51) called by hand: as a virtual call inside the part loop it keeps the vptr in a register */
static inline XF *Lookup(SO *p,int id){return p->GetXfFromIndex(id);}
// Also known as: LoadSoFromBrx (upstream readable / Sly 1 name; LoadSoFromBrx__FP2SOP18CBinaryInputStream).
static inline void SetActive(SO *p){uptr f=(uptr)p->ploupd;if((f^1)&1)p->ploupd->grf|=8;else p->inlineFlags=f|8;}
extern "C" void LoadSoFromBrx(SO *so,void *stream)
{
    int i=0;SOPART *e;
    LoadSoXfsFromBrx(so,stream);
    int metadata=U32Read__18CBinaryInputStream(stream);
    LoadGlobsetFromBrx__FP7GLOBSETP2SOP18CBinaryInputStream(&so->model,so,stream);
    int extra=so->GetExtraPartCount();
    so->numParts=S16Read__18CBinaryInputStream(stream);
    SWALLOC request;
    request.cb=(so->numParts+extra)*sizeof(SOPART);request.cbAlign=16;
    so->parts=(SOPART *)AllocClearedFromWorldHeap(&request);
    for(;i<so->numParts;i++){
        e=&so->parts[i];
        GEOM *local=&e->meshLocal;
        e->owner=so;
        e->transform=Lookup(so,S16Read__18CBinaryInputStream(stream));
        e->oid=(OID)S32Read__18CBinaryInputStream(stream);
        int f=S8Read__18CBinaryInputStream(stream);
        e->unknownFlag10A=f&1;
        f=S8Read__18CBinaryInputStream(stream);
        e->unknownFlag10B=f&1;
        e->radius=F32Read__18CBinaryInputStream(stream);
        ReadGeom__18CBinaryInputStreamP4GEOM(stream,local);
        if(!e->transform){e->transform=&D_002C44F0;e->meshWorld=e->meshLocal;}
        else CloneGeom(local,0,&e->meshWorld);
        CBinaryInputStream_ReadBspc(stream,&e->meshWorld,&e->bspc);
        e->zapKindFlags=U32Read__18CBinaryInputStream(stream);
        e->damage=S16Read__18CBinaryInputStream(stream);
        e->unknown1C=S16Read__18CBinaryInputStream(stream);
        f=S8Read__18CBinaryInputStream(stream);
        e->isHidden=f&1;
    }
    for(i=so->numParts;i<so->numParts+extra;i++){e=&so->parts[i];e->damage=-10;}
    AddSoParts(so);
    if(so->numParts>0){
        request.cb=sizeof(SOPHYS);request.cbAlign=16;
        SOPHYS *state=(SOPHYS *)AllocClearedFromWorldHeap(&request);
        so->physics=state;
        state->m=F32Read__18CBinaryInputStream(stream);
        ReadMatrix__18CBinaryInputStreamP7MATRIX3(stream,&state->inertiaLocal);
        ScaleSoInertia(so,1.0f);
        so->InitPhys();
    }
    LoadOptionsFromBrx__FPvP18CBinaryInputStream(so,stream);
    if(so->model.numPieces)SetActive(so);
    EnsureSoBounds(so);
    LoadSwObjectsFromBrx__FP2SOP18CBinaryInputStreamP14CLoadCollector(so,stream,0);
    SetLoVisMask(so,metadata);
}
} // namespace M_LoadSoFromBrx

// 0x1F3590 InitSoPhysics
/* Default physics: gravity -1750 down z, both constraints of kind 3. */
// Slot 64: physics defaults: gravity (0, 0, -1750) and both position and rotation XFs fixed (mode 3).
// Also known as: RestAloSost (upstream readable / Sly 1 name; RestAloSost__FP3ALO).
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
// Also known as: SetSoMass (upstream readable / Sly 1 name; SetSoMass__FP2SOf).
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
namespace M_AdjustSoMomint {
typedef union { qword q; float f; } U;
void func_0023320C(void *d, const void *s, unsigned int cb) __asm__("memcpy");
void *func_002333B4(void *d, int c, unsigned int cb) __asm__("memset");
int InvertMatrix3(MATRIX3 *pmat, MATRIX3 *pmatInverse) __asm__("InvertMatrix3");
// Scales the inertia matrix by r and recomputes its inverse (zero when singular).
// Also known as: AdjustSoMomint (upstream readable / Sly 1 name; AdjustSoMomint__FP2SOf).
extern "C" void ScaleSoInertia(SO *so, float s)
{
    SOPHYS *pt = so->physics;
    MATRIX3Q m;
    U u;
    MATRIX3Q r;
    qword uq;
    __asm__("qmtc2.ni %1, %0" : "=j"(uq) : "r"(s));
    u.q = uq;
    qword r0;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r0) : "j"(u.q), "j"(pt->inertiaq.r[0].q));
    r.r[0].q = r0;
    qword r1;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r1) : "j"(u.q), "j"(pt->inertiaq.r[1].q));
    r.r[1].q = r1;
    qword r2;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r2) : "j"(u.q), "j"(pt->inertiaq.r[2].q));
    r.r[2].q = r2;
    func_0023320C(&m, &r, sizeof(MATRIX3Q));
    pt->inertiaq = m;
    if (!InvertMatrix3(&so->physics->inertiaLocal, &so->physics->inverseInertiaLocal))
        func_002333B4(&so->physics->inverseInertiaLocal, 0, 0x30);
}
} // namespace M_AdjustSoMomint

// 0x1F36A8 PostSoLoad
// Slot 15 (PostLoad): PostLoLoad and the glob set's post-load.
extern "C" void PostSoLoad(SO *so)
{
    PostLoLoadV();
    UpdateSoXfWorldMatrices(so);
    BindGlobset(&so->model);
}

// 0x1F36E0 UpdateSo
namespace M_UpdateSo {
void UpdateLo(SO *) __asm__("UpdateLo");
qword GetLoWorldPos(SO *) __asm__("GetLoWorldPos");
void *FindPathzoneAtPos(qword, int) __asm__("FindPathzoneAtPos");
void SetSoPhysRegion(SO *, void *) __asm__("SetSoPhysRegion");
// Slot 16 (Update): UpdateLo, refreshes the physics region from the position and advances the fade (alpha +=
// rate * dt), removing the object when it fades to 0.
// Also known as: UpdateSo (upstream readable / Sly 1 name; UpdateSo__FP2SOf).
extern "C" void UpdateSo(SO *so)
{
    void *q;
    float d;
    UpdateLo(so);
    q = so->physics ? so->physics->pregent : 0;
    if (q) SetSoPhysRegion(so, FindPathzoneAtPos(GetLoWorldPos(so), 1));
    d = so->opacitySpeed;
    if (d != 0.0f) {
        float v = so->opacity + g_clock.deltaTime * d;
        so->opacity = v;
        if (v <= 0.0f) {
            so->opacity = 0.0f;
            so->opacitySpeed = 0.0f;
            so->Remove();
        } else if (1.0f <= v) {
            so->opacity = 1.0f;
            so->opacitySpeed = 0.0f;
        }
    }
}
} // namespace M_UpdateSo

// 0x1F37C8 SetSoPhysRegion
namespace M_func_001F37C8 {
int IsEntryInDl__FP2DLPv(void *, void *) __asm__("IsEntryInDl__FP2DLPv");
void RemoveDlEntry__FP2DLPv(void *, void *) __asm__("RemoveDlEntry__FP2DLPv");
void AppendDlEntry__FP2DLPv(void *, void *) __asm__("AppendDlEntry__FP2DLPv");
void InitObstacleQueryCursor(PATHZONE *, void *) __asm__("InitObstacleQueryCursor");
// Moves the object's region link from its current region's list to region q.
// Also known as: SetAloRegion (upstream readable / Sly 1 name; SetAloRegion__FP3ALOP9NAVREGION).
extern "C" void SetSoPhysRegion(SO *so, PATHZONE *q)
{
    REGENT *o = so->physics->pregent;
    PATHZONE *old = (PATHZONE *)o->pathzone;
    if (old == q) return;
    if (old) {
        if (IsEntryInDl__FP2DLPv(&old->dlRegent, o)) RemoveDlEntry__FP2DLPv(&old->dlRegent, o);
    }
    if (q) {
        if (!IsEntryInDl__FP2DLPv(&q->dlRegent, o)) {
            AppendDlEntry__FP2DLPv(&q->dlRegent, o);
            InitObstacleQueryCursor(q, o->unknown20);
        }
    }
    o->pathzone = (CID17 *)q;
}
} // namespace M_func_001F37C8

// 0x1F3868 UpdateSoPhysRegionCallback
namespace M_func_001F3868 {
qword GetLoWorldPos(void *) __asm__("GetLoWorldPos");
int FindPathzoneAtPos(qword, int) __asm__("FindPathzoneAtPos");
void SetSoPhysRegion(void *, int) __asm__("SetSoPhysRegion");
// Deferred callback: SetSoPhysRegion with the region at the object's world position.
// Also known as: UpdateAloRegion (upstream readable / Sly 1 name; UpdateAloRegion__FP3ALO).
extern "C" void UpdateSoPhysRegionCallback(void *p) { SetSoPhysRegion(p, FindPathzoneAtPos(GetLoWorldPos(p), 1)); }
} // namespace M_func_001F3868

// 0x1F38A8 UpdateSoPartGeom
/* Recomputes the part's world geometry when its transform changed since the last update. */
// When a part's stamp differs from its XF's, transforms the part's local geometry to world space, copies the stamp
// and marks the tree dirty.
// Also known as: UpdateSoeWorld (upstream readable / Sly 1 name; UpdateSoeWorld__FP3SOE).
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
namespace M_UpdateSoPartMatrices {
// Copies each visible part's XF world matrix and stamp into the part.
// Also known as: CopyAloSoeMatrices (upstream readable / Sly 1 name; CopyAloSoeMatrices__FP3ALO).
extern "C" void UpdateSoPartMatrices(SO *so)
{
    int i;
    for (i = 0; i < so->numParts; i++) {
        SOPART *pe = &so->parts[i];
        if (!pe->isHidden) {
            XF *po = pe->transform;
            pe->matWorld = po->worldMatrix;
            pe->worldStamp = po->worldStamp;
        }
    }
}
} // namespace M_UpdateSoPartMatrices

// 0x1F3980 UpdateSoXfWorld
namespace M_func_001F3980 {
void UpdateSoPartMatrices(SO *) __asm__("UpdateSoPartMatrices");
void UpdateSoPartMesh(SOPART *, float) __asm__("UpdateSoPartMesh");
int IsXfRoleVelocity(int) __asm__("IsXfRoleVelocity");
void UpdateSoSlots(SO *) __asm__("UpdateSoSlots");
// Zero the first three ints of a.
extern "C" { static inline void Clear(int *a)
{
    a[2] = 0;
    a[1] = 0;
    a[0] = 0;
} }
// Slot 17: per-step transform update: part matrices, soft-mesh parts, XF positions into the physics record, the
// physics preset (slot 70) for dynamic XFs, slots and part geometry.
// Also known as: ProjectSoTransform (upstream readable / Sly 1 name; ProjectSoTransform__FP2SOfi).
extern "C" void UpdateSoXfWorld(SO *so)
{
    int i;
    UpdateSoPartMatrices(so);
    for (i = 0; i < so->numParts; i++) {
        SOPART *pe = &so->parts[i];
        if (pe->plvo) UpdateSoPartMesh(pe, g_clock.deltaTime);
    }
    if (so->physics) {
        if (so->positionXf) so->physics->velocity = *(qword *)&so->positionXf->velocity;
        if (so->rotationXf) so->physics->angularVelocity = *(qword *)&so->rotationXf->velocity;
        if ((unsigned short)(so->positionXf->role - 1) < 2 || so->rotationXf->role == 1 || so->rotationXf->role == 2) {
            if (IsXfRoleVelocity(so->positionXf->role)) {
                Clear((int *)&so->positionXf->acceleration);
            }
            if (IsXfRoleVelocity(so->rotationXf->role)) {
                Clear((int *)&so->rotationXf->acceleration);
            }
            so->PresetAccel();
        }
    }
    UpdateSoSlots(so);
    for (i = 0; i < so->numParts; i++) {
        SOPART *pe = &so->parts[i];
        if (!pe->isHidden) UpdateSoPartGeom(pe);
    }
}
} // namespace M_func_001F3980

// 0x1F3B28 SetSoGrfBit8
namespace M_func_001F3B28 {
int IsLoInWorld(SO *) __asm__("IsLoInWorld");
void AddLoToRenderTree(SO *) __asm__("AddLoToRenderTree");
void RemoveLoFromRenderTree(SO *) __asm__("RemoveLoFromRenderTree");

// Slot 21: sets or clears LO flag bit 8; enabling it registers objects marked with bit 35 that are in the world,
// disabling unregisters them (meaning inferred).
extern "C" void SetSoGrfBit8(SO *so, int enabled)
{
#ifdef SLY2_EE_ASM
    register int word __asm__("a2") = so->inlineFlags;
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
                    __asm__("sw %1,%0" : "=m"(so->inlineFlags) : "r"(value));
                }
            }
            if (so->isRenderable && IsLoInWorld(so))
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
                    __asm__("sw %1,%0" : "=m"(so->inlineFlags) : "r"(value));
                }
            }
            RemoveLoFromRenderTree(so);
        }
    }
#else
    /* the flags live in the tagged word itself (bit 0 set) or in the LOUPD entry it points to */
    /* (no int * to the tagged word: a native LO may keep it pointer-wide) */
    int isTagged = so->inlineFlags & 1;
    int flags = isTagged ? so->inlineFlags : so->ploupd->grf;
    if (enabled) {
        if (flags & 8)
            return;
        if (isTagged)
            so->inlineFlags |= 8;
        else
            so->ploupd->grf |= 8;
        if (so->isRenderable && IsLoInWorld(so))
            AddLoToRenderTree(so);
    } else {
        if (!(flags & 8))
            return;
        if (isTagged)
            so->inlineFlags &= ~8;
        else
            so->ploupd->grf &= ~8;
        RemoveLoFromRenderTree(so);
    }
#endif
}
} // namespace M_func_001F3B28

// 0x1F3C10 RefreshSoBoundsRecord
namespace M_func_001F3C10 {
// Recomputes the shared bounds record when its stamp is older than the world's current stamp. (inferred)
extern "C" void RefreshSoBoundsRecord(SO *so)
{
    SOBOUNDS *pq = so->psobounds;
    if (pq && pq->lightStamp != g_sw.frameCounter) {
        BuildSoLightPackets(&g_sw, 0, pq);
        so->psobounds->lightStamp = g_sw.frameCounter;
    }
}
} // namespace M_func_001F3C10

// 0x1F3C70 RenderSoSelf
// Slot 19 (RenderSelf): draws the glob set with the object's alpha when it is visible in the current camera mask.
// Also known as: RenderSoSelf (upstream readable / Sly 1 name; RenderSoSelf__FP2SOP2CMP2RO).
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
// Slot 20: draws the glob set with explicit parameters and recurses into the children.
void SO::RenderAll(MATRIX4 *pmatWorld, float gFirst, float gSecond, SOBOUNDS *psoboundsArg)
{
    SOBOUNDS b;
    int unused[4];
    LO *po;
    SOBOUNDS *pb = psoboundsArg;
    if (!pb && psobounds) {
        b = *psobounds;
        pb = &b;
        BuildSoLightPackets(&g_sw, pmatWorld, &b);
    }
    RenderSoGlobset(&model, pmatWorld, gFirst, gSecond, pb);
    for (po = (LO *)children.head; po != ploRemovedFirst; po = (LO *)po->siblingLink.next)
        po->RenderAll(pmatWorld, gFirst, gSecond, pb);
}

// 0x1F3E00 OnSoRemove
namespace M_OnSoRemove {
void OnLoRemove(SO *) __asm__("OnLoRemove");
void RemoveLoFromRenderTree(SO *) __asm__("RemoveLoFromRenderTree");
// Slot 22 (OnRemove): the object leaves the active world: base LO freeze, unregister, children, and if it owns its
// tree, a final bounds update and removal from g_sw.dlSoTree.
extern "C" void OnSoRemove(SO *so)
{
    LO *q;
    int unused[4];
    OnLoRemove(so);
    RemoveLoFromRenderTree(so);
    for (q = (LO *)so->children.head; q != so->ploRemovedFirst; q = (LO *)q->siblingLink.next) q->OnRemove();
    if (so->physicsTree && so->physicsTree->root == so) {
        so->UpdateBounds();
        M_func_001F37C8::RemoveDlEntry__FP2DLPv(&g_sw.dlSoTree, so->physicsTree);
    }
}
} // namespace M_OnSoRemove

// 0x1F3EA8 OnSoAdd
namespace M_OnSoAdd {
void OnLoAdd(SO *) __asm__("OnLoAdd");
void AddLoToRenderTree(SO *) __asm__("AddLoToRenderTree");
int IsEntryInDl__FP2DLPv(DL *, void *) __asm__("IsEntryInDl__FP2DLPv");
// Slot 23 (OnAdd): the object enters the active world: base LO unfreeze, re-register, children, and its tree joins
// g_sw.dlSoTree with fresh bounds.
extern "C" void OnSoAdd(SO *so)
{
    LO *q;
    int unused[4];
    OnLoAdd(so);
    if (so->isRenderable) {
        sptr v = (sptr)so->ploupd;
        if ((v ^ 1) & 1) v = so->ploupd->grf;
        if (v & 8) AddLoToRenderTree(so);
    }
    for (q = (LO *)so->children.head; q != so->ploRemovedFirst; q = (LO *)q->siblingLink.next) q->OnAdd();
    if (so->physicsTree && so->physicsTree->root == so) {
        if (!IsEntryInDl__FP2DLPv((DL *)&g_sw.dlSoTree, so->physicsTree)) M_func_001F37C8::AppendDlEntry__FP2DLPv(&g_sw.dlSoTree, so->physicsTree);
        so->UpdateBounds();
    }
}
} // namespace M_OnSoAdd

// 0x1F3FA0 GetSoWorldMatrix
// Slot 31: the world matrix of the root XF.
MATRIX4 * SO::GetWorldMatrix()
{
    SO *pso = (SO *)this;
    return &pso->rootTransform->worldMatrix;
}

// 0x1F3FB0 GetSoLocalXf
namespace M_func_001F3FB0 {
// Slot 32: the root XF, optionally writing an identity matrix to the output.
extern "C" XF *GetSoLocalXf(SO *so, MATRIX4 *pm)
{
    if (pm) {
        func_002333B4(pm, 0, 0x40);
        pm->mat[0][0] = pm->mat[1][1] = pm->mat[2][2] = pm->mat[3][3] = 1.0f;
    }
    return so->rootTransform;
}
} // namespace M_func_001F3FB0

// 0x1F4010 ApplySoProxy
namespace M_func_001F4010 {
MATRIX4 __ml__FRC7MATRIX4T0(MATRIX4 *, XF *) __asm__("__ml__FRC7MATRIX4T0");
void UpdateSoIkBoneLengths(SO *) __asm__("UpdateSoIkBoneLengths");
// Slot 37: applies a proxy's matrix to the first XF's local matrix, bumps the world stamp and updates the transform
// hierarchy.
// Also known as: SetAloMatrix (upstream readable / Sly 1 name; SetAloMatrix__FP3ALOP7MATRIX4).
extern "C" void ApplySoProxy(SO *so, MATRIX4 *pv)
{
    XF *pt = so->transforms;
    pt->localMatrix = __ml__FRC7MATRIX4T0(pv, pt);
    { int n = ++g_sw.transformStamp; so->transforms->worldStamp = n; }
    so->UpdateXfWorldHierarchy();
    UpdateSoIkBoneLengths(so);
}
} // namespace M_func_001F4010

// 0x1F40A0 TranslateSoToPos
namespace M_TranslateSoToPos {

typedef struct { qword r[3]; } M3;
typedef union { struct { M3 m3; qword r3; } s; float f[4][4]; qword r[4]; } __attribute__((aligned(16))) M4;
typedef union { qword q; float f[4]; } V4;
struct REGION;
qword *VECTOR_opAssign(qword *, qword) __asm__("VECTOR_opAssign");
M4 *InvertMatrix4Rigid(M4 *,const M4 *) __asm__("InvertMatrix4Rigid");
M4 *__ml__FRC7MATRIX4T0(M4 *, const M4 *, const M4 *) SLY2_SRET_LABEL("__ml__FRC7MATRIX4T0", "MulMatrix4Into");
void *func_002333B4(void *, int, int) __asm__("memset");
void UpdateSoPartMatrices(SO *) __asm__("UpdateSoPartMatrices");
qword GetLoWorldPos(SO *) __asm__("GetLoWorldPos");
struct REGION *FindPathzoneAtPos(qword, int) __asm__("FindPathzoneAtPos");
void SetSoPhysRegion(SO *, struct REGION *) __asm__("SetSoPhysRegion");
// Slot 29: moves the object to a world position (the position XF expressed relative to its parents), frees its
// contacts and refreshes XF modes, part matrices and region.
// Also known as: SetAloPos (upstream readable / Sly 1 name; SetAloPos__FP3ALOG9VU_VECTOR).
extern "C" void TranslateSoToPos(SO *so, qword v)
{
    M4 m0, matWorld, m80, mC0, m100, m140, m180, m1C0;
    XF *plo;
    const M3 *basis;
    register M4 *presult __asm__("$18");
    if (!so->positionXf) return;
    basis = (const M3 *)&so->rootTransform->worldMatq;
    VECTOR_opAssign(&m0.s.r3, v);
    m0.s.m3 = *basis;
    m0.f[1][3] = 0.0f;
    m0.f[2][3] = 0.0f;
    m0.f[0][3] = 0.0f;
    m0.f[3][3] = 1.0f;
    presult=&mC0;
    InvertMatrix4Rigid(&m100,(M4 *)&so->positionXf->worldMatq);
    __ml__FRC7MATRIX4T0(presult, &m100, (M4 *)&so->rootTransform->worldMatq);
    matWorld.r[1] = mC0.r[1];
    matWorld.r[0] = mC0.r[0];
    matWorld.r[2] = mC0.r[2];
    matWorld.r[3] = mC0.r[3];
    if (so->positionXf->parent) {
        m80 = *(M4 *)&so->positionXf->parent->worldMatq;
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
    VECTOR_opAssign(&plo->localMatq.r[3].q, mC0.r[3]);
    plo->localMatrix.mat[3][3] = 1.0f;
    { int stamp=++g_sw.transformStamp;so->positionXf->worldStamp=stamp; }
    so->UpdateXfWorldHierarchy();
    FreeSoContacts(so);
    UnsettleXfSlot(so, so->positionXf);
    UpdateSoPartMatrices(so);
    {
        REGENT *link = 0;
        if (so->physics) link = so->physics->pregent;
        if (link) SetSoPhysRegion(so, FindPathzoneAtPos(GetLoWorldPos(so), 1));
    }
}


} // namespace M_TranslateSoToPos

// 0x1F42E0 RotateSoToMat
namespace M_RotateSoToMat {
struct M3;
// 3x3 matrix of U rows with copy constructor and assignment.
struct M3 { QWROW r[3]; M3(){} M3(const M3 &m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];} M3 &operator=(const M3 &m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return *this;} };
extern M3 InvertMatrix3Cofactor(const M3 *) __asm__("InvertMatrix3Cofactor");
extern void NormalizeRotateMatrix3__FP7MATRIX3(M3 *) __asm__("NormalizeRotateMatrix3__FP7MATRIX3");
extern void *func_002333B4(void *,int,int) __asm__("memset");
extern void UpdateSoPartMatrices(SO *) __asm__("UpdateSoPartMatrices");
// Return the 3x3 matrix product of a and b.
static inline M3 Mul(const M3 &a,const M3 &b) return r
{
    qword a0=a.r[0].q,a1=a.r[1].q,a2=a.r[2].q;
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[0].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[0].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[1].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[1].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[2].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[2].q));
}
// Slot 30: sets the rotation from a world matrix (normalised against the parents), frees contacts and refreshes.
// Also known as: SetAloRotation (upstream readable / Sly 1 name; SetAloRotation__FP3ALOP7MATRIX3).
extern "C" void RotateSoToMat(SO *so,const M3 *basis)
{
    XF *object=so->rotationXf;
    if(!object)return;
    {
        M3 m;
        if(object->parent) {
            m=*(M3 *)&object->parent->worldMatq;
            NormalizeRotateMatrix3__FP7MATRIX3(&m);
        } else {
            func_002333B4(&m,0,sizeof(m));
            m.r[2].f[2]=1.0f;
            m.r[1].f[1]=1.0f;
            m.r[0].f[0]=1.0f;
        }
        M3 transformed=Mul(InvertMatrix3Cofactor(&m),*basis);
        M3 *dest=(M3 *)&so->rotationXf->localMatq;
        *dest=transformed;
        dest->r[0].i[3]=0;
        dest->r[1].i[3]=0;
        dest->r[2].i[3]=0;
        int stamp=++g_sw.transformStamp;
        so->rotationXf->worldStamp=stamp;
        so->UpdateXfWorldHierarchy();
        FreeSoContacts(so);
        UnsettleXfSlot(so,so->rotationXf);
        UpdateSoPartMatrices(so);
    }
}
} // namespace M_RotateSoToMat

// 0x1F4440 GetSoVelocity
namespace M_GetSoVelocity {
extern qword D_002A2DD0 __asm__("D_002A2DD0");
qword GetXfVelocity(XF *, int, qword) __asm__("GetXfVelocity");
// Slot 33: world-space velocity of the position XF (when it is dynamic), else zero.
// Also known as: VelAloPos (upstream readable / Sly 1 name; VelAloPos__FP3ALO).
extern "C" qword GetSoVelocity(SO *p) { XF *pt = p->positionXf; if (!pt || pt->kind != 1) return D_002A2DD0; return GetXfVelocity(pt, 0, *(qword *)pt->worldMatrix.mat[3]); }
} // namespace M_GetSoVelocity

// 0x1F4488 SetSoVelocity
namespace M_SetSoVelocity {
int IsXfRoleMoving(int) __asm__("IsXfRoleMoving");
qword ConvertXfVec(int, XF *, qword) __asm__("ConvertXfVec");
void WakeSo(SO *) __asm__("WakeSo");
// Slot 35: sets the position XF's velocity so the world velocity becomes v, then refreshes and marks the physics.
// Also known as: SetAloVelocityToLoad (upstream readable / Sly 1 name; SetAloVelocityToLoad__FP3ALOG9VU_VECTOR).
extern "C" void SetSoVelocity(SO *so, qword q)
{
    XF *po = so->positionXf;
    qword cur, d;
    if (!po || po->kind != 1) return;
    if (!IsXfRoleMoving(po->role)) SetXfRole(so, po, 1);
    { float *pv = (float *)&po->velocity; pv[2] = 0.0f; pv[1] = 0.0f; pv[0] = 0.0f; }
    cur = so->GetVelocity();
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(q), "j"(cur));
    *(qword *)&po->velocity = ConvertXfVec(0, po->parent, d);
    UnsettleXfSlot(so, po);
    InvalidateSoAaox(so);
    WakeSo(so);
    SetSoBit35Hierarchy(so, 0);
}
} // namespace M_SetSoVelocity

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
// Also known as: AngVelAlo (upstream readable / Sly 1 name; AngVelAlo__Fv).
qword SO::GetAngularVelocity()
{
    XF *pxf = rotationXf;
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
struct M3;
typedef int qword __attribute__((mode(TI)));
// 3x3 matrix of U rows with copy constructor and assignment.
struct M3 { QWROW r[3]; M3(){} M3(const M3 &m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];} M3 &operator=(const M3 &m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return *this;} };
extern M3 InvertMatrix3Cofactor(const M3 *) __asm__("InvertMatrix3Cofactor");
extern void NormalizeRotateMatrix3__FP7MATRIX3(M3 *) __asm__("NormalizeRotateMatrix3__FP7MATRIX3");
extern void *func_002333B4(void *,int,int) __asm__("memset");
// Return the 3x3 matrix product of a and b.
static inline M3 Mul(const M3 &a,const M3 &b) return r
{
    qword a0=a.r[0].q,a1=a.r[1].q,a2=a.r[2].q;
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[0].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[0].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[1].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[1].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[2].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[2].q));
}
extern int IsXfRoleMoving(int) __asm__("IsXfRoleMoving");
extern qword ConvertXfVec(XF *,XF *,qword) __asm__("ConvertXfVec");
extern void WakeSo(SO *) __asm__("WakeSo");
extern void SetSoBit35Hierarchy(SO *,long) __asm__("SetSoBit35Hierarchy");
// Return a - *pb.
static inline qword Sub(qword a,const qword *pb) { qword r,b; __asm__("lqc2 %1,%3\n\tvsub.xyzw %0,%2,%1":"=j"(r),"=&j"(b):"j"(a),"m"(*pb));return r; }
// Rotate v by the 3x3 matrix m.
static inline qword Transform(const M3 &m,qword v) { qword r;
 __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r):"j"(m.r[0].q),"j"(m.r[1].q),"j"(m.r[2].q),"j"(v));return r;
}
// Zero the xyz words of v.
static inline void Clear(QWROW *v) { v->i[2]=0;v->i[1]=0;v->i[0]=0; }
// Slot 36: sets the rotation XF's angular velocity from a world-space w.
// Also known as: SetAloAngVelocityToLoad (upstream readable / Sly 1 name; SetAloAngVelocityToLoad__FP3ALOG9VU_VECTOR).
extern "C" void SetSoAngularVelocity(SO *so,qword position)
{
    XF *object=so->rotationXf;
    if(!object || object->kind!=6)return;
    if(!M_SetSoAngularVelocity::IsXfRoleMoving(object->role))SetXfRole(so,object,1);
    const M3 *rotation=(const M3 *)&object->worldMatq;
    Clear(&object->velq);
    M3 normalized;
    qword origin=so->GetAngularVelocity();
    normalized=*rotation;
    NormalizeRotateMatrix3__FP7MATRIX3(&normalized);
    M3 relative=Mul(*rotation,InvertMatrix3Cofactor(&normalized));
    object->velq.q=ConvertXfVec(0,object->parent,Transform(relative,Sub(position,&origin)));
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
    pxf = so->positionXf;
    ConvertXfVec(pxf->parent, so->rootTransform, *(qword *)&pxf->velocity);
}

// 0x1F4870 GetSoRotXfVelocityLocal
// The rotation XF's angular velocity converted between XF spaces.
extern "C" void GetSoRotXfVelocityLocal(SO *so)
{
    XF *pxf;
    pxf = so->rotationXf;
    ConvertXfVec(pxf->parent, so->rootTransform, *(qword *)&pxf->velocity);
}

// 0x1F4898 SetSoPosXfVelocityLocal
namespace M_func_001F4898 {
typedef int qword __attribute__((mode(TI)));
int IsXfRoleMoving(int) __asm__("IsXfRoleMoving");
void SetXfRole(void *, XF *, int) __asm__("SetXfRole");
qword ConvertXfVec(XF *, XF *, qword) __asm__("ConvertXfVec");
void UnsettleXfSlot(void *, XF *) __asm__("UnsettleXfSlot");
void InvalidateSoAaox(void *) __asm__("InvalidateSoAaox");
void WakeSo(void *) __asm__("WakeSo");
void SetSoBit35Hierarchy(void *, int) __asm__("SetSoBit35Hierarchy");
// Sets the position XF's velocity from a vector in the root XF's space, refreshes and marks the physics.
// Also known as: SetAloVelocity (upstream readable / Sly 1 name; SetAloVelocity__FP3ALOG9VU_VECTOR).
extern "C" void SetSoPosXfVelocityLocal(SO *so, qword v)
{
    XF *po;
    if (!IsXfRoleMoving(so->positionXf->role)) SetXfRole(so, so->positionXf, 1);
    po = so->positionXf;
    *(qword *)&po->velocity = ConvertXfVec(so->rootTransform, po->parent, v);
    UnsettleXfSlot(so, so->positionXf);
    InvalidateSoAaox(so);
    WakeSo(so);
    SetSoBit35Hierarchy(so, 0);
}
} // namespace M_func_001F4898

// 0x1F4930 SetSoRotXfVelocityLocal
namespace M_func_001F4930 {
typedef int qword __attribute__((mode(TI)));
int IsXfRoleMoving(int) __asm__("IsXfRoleMoving");
void SetXfRole(void *, XF *, int) __asm__("SetXfRole");
qword ConvertXfVec(XF *, XF *, qword) __asm__("ConvertXfVec");
void UnsettleXfSlot(void *, XF *) __asm__("UnsettleXfSlot");
void InvalidateSoAaox(void *) __asm__("InvalidateSoAaox");
void WakeSo(void *) __asm__("WakeSo");
void SetSoBit35Hierarchy(void *, int) __asm__("SetSoBit35Hierarchy");
// Sets the rotation XF's angular velocity from a vector in the root XF's space.
// Also known as: SetAloAngVelocity (upstream readable / Sly 1 name; SetAloAngVelocity__FP3ALOG9VU_VECTOR).
extern "C" void SetSoRotXfVelocityLocal(SO *so, qword v)
{
    XF *po;
    if (!IsXfRoleMoving(so->rotationXf->role)) SetXfRole(so, so->rotationXf, 1);
    po = so->rotationXf;
    *(qword *)&po->velocity = ConvertXfVec(so->rootTransform, po->parent, v);
    UnsettleXfSlot(so, so->rotationXf);
    InvalidateSoAaox(so);
    WakeSo(so);
    SetSoBit35Hierarchy(so, 0);
}
} // namespace M_func_001F4930

// 0x1F49C8 MatchSoXf
extern "C" void MoveLoToObject(LO *, LO *);
// Slot 38: matches another object's transform (MatchLoXf) and copies its velocities when the object has physics.
extern "C" void MatchSoXf(SO *so, LO *q)
{
    MoveLoToObject(so, q);
    if (so->physics) {
        so->SetVelocity(q->GetVelocity());
        so->SetAngularVelocity(q->GetAngularVelocity());
    }
}

// 0x1F4A80 SetSoXf
namespace M_func_001F4A80 {
struct M4; struct SOIsland;
// 4x4 matrix of U rows with copy constructor and assignment.
struct M4 {QWROW r[4];M4(){} M4(const M4&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];r[3]=m.r[3];} M4&operator=(const M4&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];r[3]=m.r[3];return *this;}};
#if defined(SLY2_EE_LAYOUT)
/* SO with its vptr named: the slot 35 entry is loaded by hand (scheduled asm) */
struct SOIsland { char pad[0xC]; VTE *pvt; char pad2[0xE8 - 0x10]; SOPHYS *physics; };
#else
struct SOIsland : SO {};
#endif
extern void SetLoTransformFromXf(struct SOIsland *p, XF *pa) __asm__("SetLoTransformFromXf");
extern M4 *GetXfAngularVelocity(M4 *,XF *,XF *) __asm__("GetXfAngularVelocity");
// The object's angular-velocity matrix (GetXfAngularVelocity).
static inline M4 BuildVelocity(XF *owner) return result { GetXfAngularVelocity(&result,owner,0); }
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
 asm("lw %0,%1":"=r"(function):"m"(((void (*)())entry.pfn)));
 return function;
}
#if defined(SLY2_EE_LAYOUT)
// Slot 39: sets the transform (SetXf) and derives linear and angular velocity from the XF rate matrix.
extern "C" void SetSoXf(struct SOIsland *so, XF *pa)
{
    M4 m;
    qword q;
    SetLoTransformFromXf(so, pa);
    if (!so->physics) return;
    { M4 t = BuildVelocity(pa); m = t; }
    VTE *vt=so->pvt;
    int delta=vt[35].d;
    q = Make(m.r[1].f[2],m.r[2].f[0],m.r[0].f[1]);
    SETVECTOR setpos=ReadSetter(vt[35]);
    {
        register qword r __asm__("vf6");
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w" : "+j"(r) : "j"(m.r[0].q), "j"(m.r[1].q), "j"(m.r[2].q), "j"(m.r[3].q), "j"(pa->worldMatq.r[3].q));
    QWROW v; v.q = q;
    qword rr;__asm__("qmfc2.ni %0,%1":"=r"(rr):"j"(r));
    setpos((char*)so+delta,rr);
    }
    {
        ((SO *)so)->SetAngularVelocity(q);
    }
}
#else
// Slot 39: sets the transform (SetXf) and derives linear and angular velocity from the XF rate matrix: the linear
// velocity is the rate matrix applied to the world position (slot 35, SetVelocity), the angular one its skew
// part (m[1][2], m[2][0], m[0][1]).
extern "C" void SetSoXf(struct SOIsland *so, XF *pa)
{
    M4 m;
    QWROW linear, angular;
    SetLoTransformFromXf(so, pa);
    if (!so->physics) return;
    { M4 t = BuildVelocity(pa); m = t; }
    const float *pos = pa->worldMatq.r[3].f;
    for (int i = 0; i < 3; i++)
        linear.f[i] = ((m.r[0].f[i] * pos[0] + m.r[1].f[i] * pos[1]) + m.r[2].f[i] * pos[2]) + m.r[3].f[i];
    linear.f[3] = 0;
    angular.f[0] = m.r[1].f[2];
    angular.f[1] = m.r[2].f[0];
    angular.f[2] = m.r[0].f[1];
    angular.f[3] = 0;
    so->SetVelocity(linear.q);
    so->SetAngularVelocity(angular.q);
}
#endif
} // namespace M_func_001F4A80

// 0x1F4B58 MarkSoTreeBoundsDirty
/* Queues the object's tree on list 1 (g_sw.dlSoTreeQueue1) once. */
// Marks the tree's bounds dirty (flag bit 0) and queues it on g_sw.dlSoTreeQueue1.
// Also known as: MarkOxaDirty1 (upstream readable / Sly 1 name; MarkOxaDirty1__FP3ALO).
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
namespace M_func_001F4BA8 {
// Uses the file-scope RemoveDlEntry(DL *, void *) (symbol RemoveDlEntry__FP2DLPv); a local `struct DL;` here would
// declare an unrelated M_func_001F4BA8::DL.
// Clears the tree's bounds-dirty flag and removes it from g_sw.dlSoTreeQueue1.
// Also known as: UnmarkOxaDirty1 (upstream readable / Sly 1 name; UnmarkOxaDirty1__FP3ALO).
extern "C" void ClearSoTreeBoundsDirty(SO *so)
{
    SOTREE *pt = so->physicsTree;
    if (!pt) return;
    if (pt->isInFirstQueue == 0) return;
    pt->isInFirstQueue = 0;
    RemoveDlEntry(&g_sw.dlSoTreeQueue1, so->physicsTree);
}
} // namespace M_func_001F4BA8

// 0x1F4BF8 InvalidateSoAaox
/* Queues the object's tree on lists 1 and 2 (g_sw.dlSoTreeQueue2) once.
   Per-object counterpart of Sly 1's InvalidateSwAaox. */
// After an object moved or changed its bounds: marks the tree bounds dirty and flags its broad-phase box once,
// queueing it for the next sweep.
// Also known as: MarkAloDirty (upstream readable / Sly 1 name; MarkAloDirty__FP3ALO).
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
namespace M_PxfFindSo {
// Finds the object's XF whose ids match the given XF's (Sly 1 PxfFindSo).
extern "C" XF *FindSoXf(SO *so, int oid, int kind)
{
    int i;
    for (i = 0; i < so->numTransforms; i++) { XF *pe = &so->transforms[i]; if (pe->oid == oid && pe->kind == kind) return pe; }
    return 0;
}
} // namespace M_PxfFindSo

// 0x1F4CA0 CloneSoXfRef
namespace M_func_001F4CA0 {
// Slot 52: re-points an XF pointer from the source object's XF block into the clone's.
extern "C" void CloneSoXfRef(SO *pnew, SO *pold, XF **pp)
{
    XF *t = *pp;
    XF *n;
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
    if (n->timeSource == &t->segmentStartTime) n->timeSource = &n->segmentStartTime;
    if (n->speedSource == &t->segmentEndTime) n->timeSource = &n->segmentEndTime;   /* sic: the game writes timeSource here too */
    *pp = n;
}
} // namespace M_func_001F4CA0

// 0x1F4D50 ClearSoTreeDirty
// Clears both tree-dirty flags and removes the tree from both queues.
// Also known as: UnmarkAloDirty (upstream readable / Sly 1 name; UnmarkAloDirty__FP3ALO).
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
namespace M_func_001F4DA8 {
struct N;
struct N { int pad[2]; void *so; char pad2[0xAC - 0xC]; struct N *next; };
void WakeSo(void *) __asm__("WakeSo");
void FreeSwXpList__FP2SWP2XP(SW *, struct N *) __asm__("FreeSwXpList__FP2SWP2XP");
// Unlinks and frees the contacts between two objects, then invalidates the first one's box.
extern "C" void RemoveSoContactsWith(void *pa, void *pb, struct N **ppn)
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
        FreeSwXpList__FP2SWP2XP(&g_sw, pfree);
    }
}
} // namespace M_func_001F4DA8

// 0x1F4E28 FreeSoContacts
namespace M_func_001F4E28 {
void FreeSwXpList__FP2SWP2XP(SW *, void *) __asm__("FreeSwXpList__FP2SWP2XP");
// Frees every contact of the tree, notifying both sides (slot 54) first.
// Also known as: DetachAloContacts (upstream readable / Sly 1 name; DetachAloContacts__FP3ALO).
extern "C" void FreeSoContacts(SO *so)
{
    XPLIST *pn;
    if (!so->physicsTree) return;
    InvalidateSoAaox(so);
    for (pn = so->physicsTree->pxplistFirst; pn; pn = pn->pxplistNext) {
        SO *q = so->physicsTree->root;
        q->OnContactFreed(pn->psoOther);
        pn->psoOther->OnContactFreed(so->physicsTree->root);
        FreeSwXpList__FP2SWP2XP(&g_sw, pn->pxpFirst);
        *pn->ppxpHead = 0;
        pn->pxpFirst = 0;
    }
    so->OnContactFreed(0);
    FreeSwXpList__FP2SWP2XP(&g_sw, so->physicsTree->pxpFirst);
    so->physicsTree->pxpFirst = 0;
}
} // namespace M_func_001F4E28

// 0x1F4F10 AdjustSoXps
namespace M_func_001F4F10 {
struct S8;
typedef unsigned long ulong;
struct S8 { long l; } __attribute__((packed));
extern const struct S8 D_002A8278 __asm__("D_002A8278");
void EnsureSwCallback(SW *, SO *, long, int) __asm__("EnsureSwCallback");
// Slot 58 (contact adjustment after marking): when the tree has touch flags, schedules the touch-zap callback
// (HandleSoTouchZaps). (inferred)
extern "C" void AdjustSoXps(SO *so)
{
    SOTREE *pe = so->physicsTree;
    XPLIST *pn;
    if (pe->firstZapContact) {
        EnsureSwCallback(&g_sw, so, D_002A8278.l, 0);
    } else if (so->zapMode) {
        for (pn = pe->pxplistFirst; pn; pn = pn->pxplistNext) {
            if (pn->pxpFirst) {
                EnsureSwCallback(&g_sw, so, D_002A8278.l, 0);
                break;
            }
        }
    }
}
} // namespace M_func_001F4F10

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
namespace M_FIgnoreSoIntersection {
// Slot 61: returns 1 when contacts between pa and pb are not needed: pb is fixed and pa is fixed too or ignores
// fixed objects (+0x148 bit 26). Sly 1 name: FIgnoreSoIntersection.
// Also known as: OverlapKindAlo (upstream readable / Sly 1 name; OverlapKindAlo__FP3ALOT0).
extern "C" int ShouldSoIgnoreIntersection(SO *pa, SO *pb)
{
    if ((pa->fIgnoreFixed || pa->IsFixed()) && pb->IsFixed()) return 1;
    return 0;
}
} // namespace M_FIgnoreSoIntersection

// 0x1F5040 IsSoFixed
/* Whether the object is fixed: no physics, or both constraints of kind 3. */
// Slot 62: returns 1 when the object has no physics record or both its XFs are fixed (mode 3).
// Also known as: FAloStill (upstream readable / Sly 1 name; FAloStill__FP3ALO).
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
// Also known as: WakeAlo (upstream readable / Sly 1 name; WakeAlo__FP3ALO).
extern "C" void WakeSo(SO *so)
{
    if (so->physicsTree != 0) {
        so->SetFlags(4);
        if (so->physics != 0) {
            UnsettleXfSlot(so, so->positionXf);
            UnsettleXfSlot(so, so->rotationXf);
        }
    }
}

// 0x1F50E0 UpdateSoTreeBounds
namespace M_UpdateSoTreeBounds {
struct VU_FLOAT;
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
struct VU_FLOAT {qword qw;};
extern qword D_002A2DE0 __asm__("D_002A2DE0"),D_002A2DF0 __asm__("D_002A2DF0"),D_002A2E00 __asm__("D_002A2E00");

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
// Also known as: UpdateSoBounds (upstream readable / Sly 1 name; UpdateSoBounds__FP2SO).
extern "C" void UpdateSoTreeBounds(SO *so)
{
    qword qwMin = D_002A2DF0;
    qword qwMax = D_002A2E00;
    SOPART *ppart;
    int i;
    for (ppart = (SOPART *)so->physicsTree->parts.head; ppart; ppart = (SOPART *)ppart->dle.next)
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
    for (ppart = (SOPART *)so->physicsTree->parts.head; ppart; ppart = (SOPART *)ppart->dle.next)
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
    g_sw.fAaoxUnsorted = 1;
    ClearSoTreeBoundsDirty(so);
}

typedef char layout_part_size[(sizeof(SOPART)==0xB0)?1:-1];
typedef char layout_part_matrix[((unsigned)&((SOPART*)0)->matWorld==0x20)?1:-1];
typedef char layout_part_dle[((unsigned)&((SOPART*)0)->dle==0x60)?1:-1];
typedef char layout_part_world[((unsigned)&((SOPART*)0)->meshWorld==0x90)?1:-1];
typedef char layout_part_stamp[((unsigned)&((SOPART*)0)->worldStamp==0xAC)?1:-1];
typedef char layout_tree_first[((unsigned)&((SOTREE*)0)->parts==0x34)?1:-1];
} // namespace M_UpdateSoTreeBounds

// 0x1F5480 ExtendSoTreeBounds
namespace M_ExtendSoTreeBounds {
// Grows the tree's box and radius to include a sphere (centre c, radius r).
extern "C" void ExtendSoTreeBounds(SO *so, qword c, float r)
{
    qword rr, lo, hi, d, t, one;
    float dist;
    __asm__("pextlw %0, %1, %1\n\tpextlw %0, %0, %0" : "=r"(rr) : "r"(r));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(lo) : "j"(c), "j"(rr));
    qword mn;
    __asm__("vmini.xyz %0, %1, %2" : "=j"(mn) : "j"(so->physicsTree->boundsMin), "j"(lo));
    so->physicsTree->boundsMin = mn;
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(hi) : "j"(c), "j"(rr));
    qword mx;
    __asm__("vmax.xyz %0, %1, %2" : "=j"(mx) : "j"(so->physicsTree->boundsMax), "j"(hi));
    so->physicsTree->boundsMax = mx;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(c) : "0"(c), "j"(so->physicsTree->boundsCenter));
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d), "=&j"(one) : "0"(c));
    __asm__("qmfc2.ni %0, %1" : "=r"(dist) : "j"(d));
    if (so->physicsTree->boundsRadius < dist) so->physicsTree->boundsRadius = dist;
}
} // namespace M_ExtendSoTreeBounds

// 0x1F5510 GetSoConstraintAxis
namespace M_func_001F5510 {
struct OUT;
typedef struct { qword row[3]; } M3;
struct OUT { qword axis; int mode; };
extern int D_002C3C20[] __asm__("D_002C3C20");
extern qword D_002C3BA0[] __asm__("D_002C3BA0");
extern qword D_002A2DD0 __asm__("D_002A2DD0");
// Rotate value by the 3x3 matrix (register-pinned rows).
extern "C" { static inline qword Transform(M3 *matrix,qword value) {
    register qword x __asm__("vf2")=matrix->row[0];
    register qword y __asm__("vf1")=matrix->row[1];
    register qword z __asm__("vf3")=matrix->row[2];
    __asm__("vmulax.xyz $ACC,%1,%3x\n\tvmadday.xyz $ACC,%2,%3y\n\tvmaddz.xyz %0,%0,%3z" : "+j"(z) : "j"(x),"j"(y),"j"(value));
    return z;
} }
// Unit vector along value, or fallback when its length is too small.
extern "C" { static inline qword NormalizeOr(qword value,qword fallback) {
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
} }
// Looks up constraint i in the constraint tables: its mode, and its axis transformed by a matrix and normalised.
// (inferred)
// Also known as: SetCnstrAxis (upstream readable / Sly 1 name; SetCnstrAxis__FP2XFP6CNSTRSiP3OUT).
void GetSoConstraintAxis(void *unused,M3 *matrix,int index,struct OUT *out) __asm__("GetSoConstraintAxis"); void GetSoConstraintAxis(void *unused,M3 *matrix,int index,struct OUT *out) {
    out->mode=D_002C3C20[index];
    out->axis=NormalizeOr(Transform(matrix,D_002C3BA0[index]),D_002A2DD0);
}
} // namespace M_func_001F5510

// 0x1F55C8 RebuildSoPhysHook
namespace M_func_001F55C8 {
int FindSwObjectsByClass(SW *, int, int, SO *, int, SO **) __asm__("FindSwObjectsByClass");
void RemoveSoParts(SO *) __asm__("RemoveSoParts");
void AddSoParts(SO *) __asm__("AddSoParts");
// After a physics-root change, regroups the contact trees of the objects whose physics root changed and registers
// them again. Sly 1 name.
// Also known as: ReRootAlo (upstream readable / Sly 1 name; ReRootAlo__FP3ALO).
extern "C" void RebuildSoPhysHook(SO *so)
{
    SO *ap[64];
    int i;
    int n;
    if ((so->isInWorld & 1) == 0) return;
    if (FindSoPhysRoot(so) != so->physicsTree->root) {
        n = FindSwObjectsByClass(&g_sw, 1, 0x30, so, 0x40, ap);
        for (i = 0; i < n; i++) {
            {
                SO *po = ap[i];
                if (po->physicsTree) {
                    SO *q = FindSoPhysRoot(po);
                    if (q != po->physicsTree->root) {
                        RemoveSoParts(po);
                        if (!q->physicsTree) {
                            q->physicsTree = q->NewTree();
                            RefreshSwObjectCollisionFilter(&g_sw, q);
                        }
                        po->physicsTree = q->physicsTree;
                        AddSoParts(po);
                    }
                }
            }
        }
    }
    RefreshSwObjectCollisionFilter(&g_sw, so);
    WakeSo(so);
}
} // namespace M_func_001F55C8

// 0x1F56F8 NewSoTree
// Slot 53: creates the object's tree record in the broad phase (AddObjectToBroadPhase). (inferred)
extern "C" void NewSoTree(SO *so)
{
    AddObjectToBroadPhase(&g_sw, so);
}

// 0x1F5720 SetSoCnstrForce
// Sets the translation constraint from a CNSTR: makes the position XF dynamic if needed, stores axis and mode in the
// physics record and rebuilds the physics hook.
// Also known as: SetAloCnstrLin (upstream readable / Sly 1 name; SetAloCnstrLin__FP3ALOi).
extern "C" void SetSoCnstrForce(SO *so, CONSTR *pconstr)
{
    XF *pxf;
    if (so->physics != 0) {
        pxf = so->positionXf;
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
// Also known as: SetAloCnstrAng (upstream readable / Sly 1 name; SetAloCnstrAng__FP3ALOi).
extern "C" void SetSoCnstrTorque(SO *so, CONSTR *pconstr)
{
    XF *pxf;
    if (so->physics != 0) {
        pxf = so->rotationXf;
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
namespace M_GetSoDvGravity {
extern qword D_002A2DD0 __asm__("D_002A2DD0");
// The object's gravity vector, or zero without a physics record.
extern "C" qword GetSoGravity(SO *p) { if (!p->physics) return D_002A2DD0; return *(qword *)&p->physics->gravity; }   /* gravity read as a quadword */
} // namespace M_GetSoDvGravity

// 0x1F5880 SetSoGravity
namespace M_SetSoDvGravity {
// Sets the object's gravity vector (making the position XF dynamic if needed) and invalidates its box.
// Also known as: SetAloGravity (upstream readable / Sly 1 name; SetAloGravity__FP3ALOG9VU_VECTOR).
extern "C" void SetSoGravity(SO *so, qword q)
{
    if (so->physics) {
        XF *po = so->positionXf;
        if (po->firstActuator == 0 && po->role != 2) SetXfRole(so, po, 1);
        *(qword *)&so->physics->gravity = q;   /* gravity written as a quadword */
        WakeSo(so);
    }
}
} // namespace M_SetSoDvGravity

// 0x1F58F0 GetSoCameraDefaults
namespace M_GetSoCpdefi {
qword GetXfVelocity(XF *, int, qword) __asm__("GetXfVelocity");
void TiltMatUpright(void *, int, MATRIX3 *) __asm__("TiltMatUpright");
float atan2f(float, float) __asm__("atan2f");
// Slot 50: the camera's default info for this object: base position and velocity, home heading from the upright
// matrix, adjustment 100, no flags, last-move time now.
// Also known as: GetSoCpdefi (upstream readable / Sly 1 name; GetSoCpdefi__FP2SOP6CPDEFI).
extern "C" void GetSoCameraDefaults(SO *so, CPDEFI *po)
{
    XF *o = so->rootTransform;
    MATRIX3 v;
    void *ps;
    qword q = o->worldMatq.r[3].q;
    *(qword *)&po->basePosition = q;
    ps = &o->worldMatrix;
    *(qword *)&po->vBase = GetXfVelocity(o, 0, q);
    TiltMatUpright(ps, 0, &v);
    po->homeHeading = atan2f(-v.mat[0][1], -v.mat[0][0]); po->adjustDistance = 100.0f; po->grfcpd = 0; po->lastMoveTime = g_clock.time; po->turnAxis = 0;
}
} // namespace M_GetSoCpdefi

// 0x1F5988 OnSoContactFreed
// Slot 54: a contact of the object is being freed; empty default (STEP overrides it).
// Also known as: SetAloOverlapNothing (upstream readable / Sly 1 name; SetAloOverlapNothing__FP3ALOT0).
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
union U; struct M3;
// AccelSoTowardMatBounded: angular braking and bounded angular-velocity correction.
// coef a = top angular speed, (a-b)/c = angular acceleration, a*a/(2*acceleration) = stopping angle.
// Caps an overspeed, turns the owner toward `orientation` (expressed in its parent space) with a desired
// speed taken from the predicted remaining angle, and adds a force change bounded by acceleration*dt.
// Matched from the codex/lilly drafts (T1623). Notes: notes/1F0000.md.
union U {qword q;float f[4];};
// 3x3 matrix of U rows with copy constructor and assignment.
struct M3 {U rows[3];M3(){} M3(const M3&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];} M3&operator=(const M3&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];return *this;}};
extern M3 *ConvertXfMat(M3 *,XF *,XF *,const M3 *) __asm__("ConvertXfMat");
// The orientation converted from parent space to world space (ConvertXfMat).
static inline M3 BuildFrame(XF*parent,const M3*orientation) return result {ConvertXfMat(&result,0,parent,orientation);}
extern void NormalizeRotateMatrix3__FP7MATRIX3(M3 *) __asm__("NormalizeRotateMatrix3__FP7MATRIX3");
extern void MulMatrix3Matrix3(M3 *,const M3 *,const M3 *) __asm__("MulMatrix3Matrix3");
extern void DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(const M3 *,float *,qword *) __asm__("DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR");
extern float ClampAbs__Fff(float,float) __asm__("ClampAbs__Fff");
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
// Also known as: BrakeAloRotation (upstream readable / Sly 1 name; BrakeAloRotation__FP3ALOP7MATRIX3Pf).
extern "C" void AccelSoTowardMatBounded(SO *so,const M3 *orientation,const CLQ *c)
{
    M3 frame,relative;
    U axis,wanted,change,predicted;
    float angle;
    float maximum=c->g0;
    float acceleration=(maximum-c->g1)/c->g2;
    float square=maximum*maximum;
    float stopAngle;
    XF *owner=so->rotationXf;
    // one variable for the speed and, later, the speed along the turn axis (target: f4 for both)
    float speed=Length(owner->velq.q);
    stopAngle=square/(acceleration+acceleration);
    float desired,remaining,length,gain;
    CLOCK *clock;
    if(maximum<speed){
        U scalar;float correction=((maximum-speed)*2.5f)/speed;
        change.q=ScaleFactor(owner->velq.q,Scalar(correction,&scalar));
        owner->acceleration=Add(owner->acceleration,change.q);
    }
    frame=BuildFrame(owner->parent,orientation);
    M_func_001F59A0::NormalizeRotateMatrix3__FP7MATRIX3(&frame);
    {M3 out;MulMatrix3Matrix3(&out,(M3 *)&owner->localMatq,&frame);relative=out;}
    DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(&relative,&angle,&axis.q);
    desired=ClampAbs__Fff((angle/stopAngle)*maximum,maximum);
    speed=QwordX(DotQ(owner->velq.q,axis.q));
    clock=&g_clock;
    float half=clock->deltaTime*0.5f;
    remaining=angle-(speed+desired)*half;
    if(remaining<stopAngle)desired=ClampAbs__Fff((remaining/stopAngle)*maximum,maximum);
    U scalar;
    qword w;
    {qword k=Scalar(desired,&scalar);w=Scale(axis.q,k);StoreQ(w,&wanted);}
    {qword v=owner->velq.q;qword k=Scalar(clock->deltaTime,&scalar);predicted.q=AddScaled(v,owner->acceleration,k);}
    qword cv=VuSub(w,predicted.q);
    change.q=cv;
    length=Length(cv);
    if(length<acceleration*clock->deltaTime)gain=1.0f/clock->deltaTime;
    else gain=acceleration/length;
    StoreQ(ScaleNew(cv,Scalar(gain,&scalar)),&change);
    owner->acceleration=Add(owner->acceleration,change.q);
}

typedef char check_matrix_size[(sizeof(M3)==0x30)?1:-1];
} // namespace M_func_001F59A0

// 0x1F5C30 AccelSoTowardMatSpring
namespace M_AccelSoTowardMatSpring {
union U; struct M3;
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
// 3x3 matrix of U rows with copy constructor and assignment.
struct M3 {U rows[3];M3(){} M3(const M3&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];} M3&operator=(const M3&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];return *this;}};
extern M3 ConvertXfMat(XF *,XF *,const M3 *) __asm__("ConvertXfMat");
extern void NormalizeRotateMatrix3__FP7MATRIX3(M3 *) __asm__("NormalizeRotateMatrix3__FP7MATRIX3");
extern M3 MulMatrix3Matrix3(const M3 *,const M3 *) __asm__("MulMatrix3Matrix3");
extern void DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(const M3 *,float *,qword *) __asm__("DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR");
extern qword ConvertXfVec(XF *,XF *,qword) __asm__("ConvertXfVec");
// Length of the xyz part of a as a float.
static inline float Length(qword a){qword d,one;float f;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a));
__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(d):"0"(d));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(d));return f;}
// Return old + v * s.x.
static inline qword AddScaled(qword old,qword v,qword s){__asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x":"+j"(old):"j"(v),"j"(s));return old;}
// Spring torque toward a target orientation (quadratic CLQ on the angle) and toward a target angular velocity
// (clamped to 1/dt).
// Also known as: AccelAloTowardRotation (upstream readable / Sly 1 name;
//   AccelAloTowardRotation__FP3ALOP7MATRIX3P3CLQG9VU_VECTORT2).
extern "C" void AccelSoTowardMatSpring(SO*so,const M3*orientation,const CLQ*c,qword velocity,const CLQ*d)
{
    XF *owner=so->rotationXf;
    if(c){
        M3 frame,relative;
        U axis;
        float angle;
        frame=ConvertXfMat(0,owner->parent,orientation);
        NormalizeRotateMatrix3__FP7MATRIX3(&frame);
        relative=MulMatrix3Matrix3((M3 *)&owner->localMatq,&frame);
        DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(&relative,&angle,&axis.q);
        if(0.0001f<angle){
            float gain=c->g2*angle*angle+c->g1*angle+c->g0;
            U scalar;
            qword k=VuFromFloat<qword>(gain);
            {qword result=AddScaled(owner->acceleration,axis.q,k);scalar.q=k;owner->acceleration=result;}
        }
    }
    if(d){
        qword transformed=ConvertXfVec(0,owner->parent,velocity);
        qword direction=VuSub(transformed,(qword)owner->velq.q);
        float length=Length(direction);
        if(0.0001f<length){
            float gain=(d->g2*length+d->g1)+d->g0/length;
            float limit=1.0f/g_clock.deltaTime;
            U scalar;
            qword k;
            if(limit<gain)gain=limit;
            k=VuFromFloat<qword>(gain);
            {qword result=AddScaled(owner->acceleration,direction,k);scalar.q=k;owner->acceleration=result;}
        }
    }
}
} // namespace M_AccelSoTowardMatSpring

// 0x1F5E10 AccelSoTowardPosSpring
namespace M_AccelSoTowardPosSpring {
qword GetLoWorldPos(SO*) __asm__("GetLoWorldPos");
qword ConvertXfVec(void*,void*,qword) __asm__("ConvertXfVec");
// Return x - y (vf1/vf2/vf3).
extern "C" { static inline qword Sub(qword x,qword y){register qword a __asm__("vf1")=x,b __asm__("vf2")=y,r __asm__("vf3");__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;} }
// Length of the xyz part of value as a float.
extern "C" { static inline float Length(qword value){register qword a __asm__("vf3")=value,r __asm__("vf1"),one __asm__("vf2");float f;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(r),"=&j"(one):"j"(a));
__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(r):"0"(r));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(r));return f;} }
// Load a 128-bit vector into a GPR ($2) with lq.
extern "C" { static inline qword LoadVector(const QWROW *value){register qword result asm("v0");__asm__("lq %0,%1":"=r"(result):"m"(value->q));return result;} }
// Move a float's bits into a GPR qword ($2).
extern "C" { static inline qword Scalar(float f){register qword r __asm__("$2");__asm__("mfc1 %0,%1":"=r"(r):"f"(f));return r;} }
// Return old + direction * factor.x.
extern "C" { static inline qword AddScaled(qword old,qword direction,qword factor){register qword a __asm__("vf2")=old,b __asm__("vf4")=direction,scale __asm__("vf3")=factor,r __asm__("vf1");
__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=&j"(r):"j"(a),"j"(b),"j"(scale));return r;} }
// Spring force toward a target position (CLQ on the distance) and toward a target velocity (clamped to 1/dt).
// Also known as: AccelAloTowardPosition (upstream readable / Sly 1 name;
//   AccelAloTowardPosition__FP3ALOG9VU_VECTORP3CLQT1T2).
extern "C" void AccelSoTowardPosSpring(SO*input,qword position,CLQ*positionCoefficients,qword velocity,CLQ*velocityCoefficients)
{
 QWROW argument,point,old1,scale1,speed,old2,scale2;
 register SO *p __asm__("$17")=input;
 register CLQ *c __asm__("$16")=positionCoefficients,*d __asm__("$18")=velocityCoefficients;
 register XF *owner __asm__("$19");
 point.q=position;owner=p->positionXf;speed.q=velocity;
 if(c){
   qword difference=Sub(point.q,GetLoWorldPos(p));
   float distance=Length(difference);
   if(1.0e-4f<distance){
     float inverse=c->g0/distance;
     float value=c->g2*distance+c->g1;
     register void *parent __asm__("$5");
     old1.q=owner->acceleration;parent=owner->parent;value+=inverse;
     argument.q=Scalar(value);scale1=argument;
     owner->acceleration=AddScaled(old1.q,ConvertXfVec(0,parent,difference),scale1.q);
   }
 }
 if(d){
   qword difference=Sub(speed.q,p->GetVelocity());
   float distance=Length(difference);
   if(1.0e-4f<distance){
     register void *parent asm("a1");
     float value=(d->g2*distance+d->g1)+d->g0/distance;
     float limit=1.0f/g_clock.deltaTime;
     if(limit<value)value=limit;
     scale2.q=Scalar(value);
     {register qword oldValue asm("v0")=owner->acceleration;old2.q=oldValue;}parent=owner->parent;argument.q=LoadVector(&scale2);
     owner->acceleration=AddScaled(old2.q,ConvertXfVec(0,parent,difference),scale2.q);
   }
 }
}
} // namespace M_AccelSoTowardPosSpring

// 0x1F5FF8 FindSoGlob
namespace M_func_001F5FF8 {
// Finds a glob in the object's glob set by oid.
extern "C" GLOB *FindSoGlob(SO *so, int oid)
{
    int i;
    for (i = 0; i < so->model.numPieces; i++) { GLOB *pe = &so->model.pieces[i]; if (pe->oid == oid) return pe; }
    return 0;
}
} // namespace M_func_001F5FF8

// 0x1F6030 FindSoPart
namespace M_PsopartFindSo {
// Finds a part by oid (Sly 1 PsopartFindSo).
extern "C" SOPART *FindSoPart(SO *so, int oid)
{
    int i;
    for (i = 0; i < so->numParts; i++) { SOPART *pe = &so->parts[i]; if (pe->oid == oid) return pe; }
    return 0;
}
} // namespace M_PsopartFindSo

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
// Also known as: CloneSo (upstream readable / Sly 1 name; CloneSo__FP2SOT0).
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
            CHAINLINK *plink = pxf->plink;
            CHAIN *pchain = plink->chain;
            if (plink == pchain->links)
            {
                pchain = (CHAIN *)PvAllocSwCopy(sizeof(CHAIN), pchain);
                pchain->links = (CHAINLINK *)PvAllocSwCopy(pchain->linkCount * sizeof(CHAINLINK), pchain->links);
                for (int ilink = 0; ilink < pchain->linkCount; ilink++)
                {
                    CHAINLINK *plinkNew = &pchain->links[ilink];
                    plinkNew->chain = pchain;
                    CloneXfRef(ploBase, &plinkNew->pxf);
                    plinkNew->pxf->plink = plinkNew;
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
    CloneXfRef(ploBase, &positionXf);
    CloneXfRef(ploBase, &rotationXf);
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
namespace M_CloneSoHierarchy {
void CloneLoWithParent__FP2LOP2SO(LO *, SO *) __asm__("CloneLoWithParent__FP2LOP2SO");
// Slot 40: clones the object (slot 41) and then every child that was not removed.
extern "C" void CloneSoHierarchy(SO *pa, SO *so)
{
    LO *q;
    pa->Clone(so);
    for (q = (LO *)so->children.head; q != so->ploRemovedFirst; q = (LO *)q->siblingLink.next)
        if (!q->isSkippedByClone) CloneLoWithParent__FP2LOP2SO(q, pa);
}
} // namespace M_CloneSoHierarchy

// 0x1F6788 FindAsegaByOid
namespace M_func_001F6788 {
// Searches the world's ASEGA list for the application on root object palo (of the segment with oid oidAseg unless
// oidAseg is -1). sensor.cpp's declaration gives the parameter types.
extern "C" ASEGA *FindAsegaByOid(ALO *palo, OID oidAseg)
{
    ASEGA *p;
    int unused[4];
    for (p = (*(ASEGA * *)&g_sw.dlAsega); p; p = (ASEGA *)p->dleSw.next) {
        if (p->rootObject == palo) {
            if (oidAseg == -1 || p->animation->oid == oidAseg)
                return p;
        }
    }
    return 0;
}
} // namespace M_func_001F6788

// 0x1F67E0 FindLiveSma
namespace M_func_001F67E0 {
// The same search over the world's list of live state-machine applications (SMA): the one on root object palo, of
// the machine with oid oidSm unless oidSm is -1. relay.cpp's PsmaFindOnAlo declaration gives the parameter types.
// Also known as: PsmaFindAloSm (upstream readable / Sly 1 name; PsmaFindAloSm__Fi3OID).
extern "C" SMA *FindLiveSma(ALO *palo, OID oidSm)
{
    SMA *p;
    int unused[4];
    for (p = (*(SMA * *)&g_sw.dlSma); p; p = (SMA *)p->dleSw.next) {
        if (p->paloRoot == palo) {
            if (oidSm == -1 || p->stateMachine->oid == oidSm)
                return p;
        }
    }
    return 0;
}
} // namespace M_func_001F67E0

// 0x1F6838 GetSoTreeFlag2
namespace M_func_001F6838 {
// Bit 2 of the tree flags; 1 when the object has no tree. (inferred)
// Also known as: FAloOutOfSweep (upstream readable / Sly 1 name; FAloOutOfSweep__FP3ALO).
extern "C" int GetSoTreeFlag2(SO *p) { if (!p->physicsTree) return 1; return p->physicsTree->skipsUpdate; }
} // namespace M_func_001F6838

// 0x1F6860 SetSoTreeFlag2
// Sets bit 2 of the tree flags and frees the tree's contacts. (inferred)
// Also known as: SetAloContactsOff (upstream readable / Sly 1 name; SetAloContactsOff__FP3ALOi).
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
namespace M_func_001F68A8 {
// Sets (f = 1) bit 48 in the flag word of every part. (inferred)
// Also known as: SetAloSensors (upstream readable / Sly 1 name; SetAloSensors__FP3ALOi).
extern "C" void SetSoPartsBit48(SO *so, int f)
{
    int i;
    for (i = 0; i < so->numParts; i++) so->parts[i].unknownFlag10A = f;
}
} // namespace M_func_001F68A8

// 0x1F6918 SetSoIgnoreFixed
namespace M_func_001F6918 {
// Sets the ignore-fixed flag (+0x148 bit 26, so.h fIgnoreFixed) and refreshes the broad-phase pairs.
extern "C" void SetSoIgnoreFixed(SO *p, int f) { p->fIgnoreFixed = f; RefreshSwObjectCollisionFilter(&g_sw, p); }
} // namespace M_func_001F6918

// 0x1F6960 GetSoIgnoreFixed
namespace M_func_001F6960 {
// Returns the ignore-fixed flag.
extern "C" int GetSoIgnoreFixed(SO *p) { return p->fIgnoreFixed; }
} // namespace M_func_001F6960

// 0x1F6978 ShowSoGlobsById
namespace M_ShowSoGlobsById {
// Shows every glob with id k and the part linked to it.
// Also known as: ShowAloGlobs (upstream readable / Sly 1 name; ShowAloGlobs__FP3ALOi).
extern "C" void ShowSoGlobsById(SO *so, int k)
{
    int i;
    for (i = 0; i < so->model.numPieces; i++) {
        GLOB *pe = &so->model.pieces[i];
        if (pe->oid == k) {
            pe->isHidden = 0;
            if (pe->ppose) pe->ppose->isHidden = 0;
        }
    }
}
} // namespace M_ShowSoGlobsById

// 0x1F69F8 HideSoGlobsById
namespace M_HideSoGlobsById {
// Hides every glob with id k and the part linked to it.
extern "C" void HideSoGlobsById(SO *so, int k)
{
    int i;
    for (i = 0; i < so->model.numPieces; i++) {
        GLOB *pe = &so->model.pieces[i];
        if (pe->oid == k) {
            pe->isHidden = 1;
            if (pe->ppose) pe->ppose->isHidden = 1;
        }
    }
}
} // namespace M_HideSoGlobsById

// 0x1F6A60: dead-code scrap, a nop unit (sly2/scrap.h)
SLY2_SCRAP("nop");

// 0x1F6A68 HideSoAllGlobs
namespace M_func_001F6A68 {
// Hides every glob and its linked part.
extern "C" void HideSoAllGlobs(SO *so)
{
    int i;
    for (i = 0; i < so->model.numPieces; i++) {
        GLOB *pe = &so->model.pieces[i];
        pe->isHidden = 1;
        if (pe->ppose)
            pe->ppose->isHidden = 1;
    }
}
} // namespace M_func_001F6A68

// 0x1F6AC8 ShowSoPartsById
namespace M_ShowSoPartsById {
void ShowSoPart(SO *, SOPART *) __asm__("ShowSoPart");
// Shows the parts whose id is k.
// Also known as: SetAloSoesByTag (upstream readable / Sly 1 name; SetAloSoesByTag__FP3ALOi).
extern "C" void ShowSoPartsById(SO *so, int k)
{
    int i;
    for (i = 0; i < so->numParts; i++) {
        SOPART *pe = &so->parts[i]; if (pe->oid == k) ShowSoPart(so, pe);
    }
}
} // namespace M_ShowSoPartsById

// 0x1F6B48 HideSoPartsById
namespace M_func_001F6B48 {
void HideSoPart(SO *, SOPART *) __asm__("HideSoPart");
// Hides the parts whose id is k.
extern "C" void HideSoPartsById(SO *so, int k)
{
    int i;
    for (i = 0; i < so->numParts; i++) {
        SOPART *pe = &so->parts[i]; if (pe->oid == k) HideSoPart(so, pe);
    }
}
} // namespace M_func_001F6B48

// 0x1F6BC8 SetSoPartsZapKindsById
namespace M_func_001F6BC8 {
// Sets the zap kinds (+0x18) of the parts whose id is n.
extern "C" void SetSoPartsZapKindsById(SO *p, int n, int x) { int i; for (i = 0; i < p->numParts; i++) { SOPART *pe = &p->parts[i]; if (pe->oid == n) pe->zapKindFlags = x; } }
} // namespace M_func_001F6BC8

// 0x1F6C08 EnsureSoPartMesh
namespace M_func_001F6C08 {
struct FRAME;
// EnsureSoPartMesh (0x1F6C08): build the scratch mesh of P once (p->mesh at +0x10).
// Allocates the MESH, per-point velocity/acceleration arrays and per-triangle vertex triples inside a
// temporary allocator FRAME, collects each triangle's three vertices from the edge list (edge face ids
// -> unique edge vertices, at most 3 per triangle), then orients every triple so that
// dot(cross(v1-v0, v2-v0), plane normal) >= 0 (swaps vertex 1/2 otherwise).
// MATCH (T1582): ONE function-scope triangle index (`itri`, used by the edge loop and as the second loop's
// counter) and ONE function-scope `ptri` shared by both loops; per-loop locals gave register rotation only.
struct FRAME;
extern FRAME *STACKSCOPE_ctor(FRAME *) __asm__("STACKSCOPE_ctor");
extern void STACKSCOPE_ctor_1AE4A0(FRAME *,int) __asm__("_$_10STACKSCOPE");
// Empty scope object (constructor/destructor only shape the generated code).
struct FRAME {FRAME(){}~FRAME(){}};
extern void *AllocFromWorldHeap(SWALLOC *) __asm__("AllocFromWorldHeap");
extern void *AllocClearedFromWorldHeap(SWALLOC *) __asm__("AllocClearedFromWorldHeap");
extern void *AllocClearedFromStack(SWALLOC *) __asm__("AllocClearedFromStack");
// Return a - b.
static inline qword Sub(qword a,qword b) {qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"0"(a),"j"(b));return r;}
// Cross product a x b.
static inline qword Cross(qword a,qword b){qword r;__asm__("vopmula.xyz $ACC,%1,%2\n\tvopmsub.xyz %0,%2,%1":"=j"(r):"0"(a),"j"(b));return r;}
// Dot product of the xyz parts of a and b as a float.
static inline float Dot(qword a,qword b){qword r,one;float f;
__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(r),"=&j"(one):"0"(a),"j"(b));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(r));return f;}
// Builds a part's soft-mesh record once: per-vertex velocities and accelerations and the oriented triangles.
// Also known as: EnsureSoeMesh (upstream readable / Sly 1 name; EnsureSoeMesh__FP3SOE).
extern "C" void EnsureSoPartMesh(SOPART *so)
{
    if(so->plvo)return;
    QWROW first,second,cross;
    FRAME storage;
    SWALLOC request;
    int triBytes=sizeof(LVO),alignment=16;
    STACKSCOPE_ctor(&storage);
    SWALLOC *allocation=&request;
    request.cb=triBytes;allocation->cbAlign=alignment;
    so->plvo=(LVO *)AllocFromWorldHeap(allocation);
    request.cb=so->meshWorld.numPositions*16;allocation->cbAlign=alignment;
    so->plvo->mpiposvLocal=(VECTOR *)AllocClearedFromWorldHeap(allocation);
    request.cb=so->meshWorld.numPositions*16;allocation->cbAlign=alignment;
    so->plvo->mpiposdvLocal=(VECTOR *)AllocClearedFromWorldHeap(allocation);
    request.cb=so->meshWorld.numSurfaces*triBytes;allocation->cbAlign=alignment;
    so->plvo->mpisurfsurfx=(SURFX *)AllocClearedFromWorldHeap(allocation);
    request.cb=so->meshWorld.numSurfaces*4;allocation->cbAlign=alignment;
    int *counts=(int *)AllocClearedFromStack(allocation);
    int itri;
    SURFX *ptri;
    for(int i=0;i<so->meshWorld.numEdges;i++) {
        EDGE *edge=&so->meshWorld.edges[i];
        for(int side=0;side<2;side++) {
            itri=edge->aisurf[side];
            ptri=&so->plvo->mpisurfsurfx[itri];
            for(int endpoint=0;endpoint<2;endpoint++) {
                int vertex=edge->aipos[endpoint];
                int j;
                for(j=0;j<counts[itri];j++)if(vertex==ptri->aipos[j])break;
                if(j<counts[itri])continue;
                if(j<3){ptri->aipos[counts[itri]]=vertex;counts[itri]++;}
            }
        }
    }
    QWROW *positions=(QWROW *)so->meshLocal.positions;
    for(itri=0;itri<so->meshWorld.numSurfaces;itri++) {
        ptri=&so->plvo->mpisurfsurfx[itri];
        SURF *plane=&so->meshLocal.surfaces[itri];
        first.q=Sub(positions[ptri->aipos[1]].q,positions[ptri->aipos[0]].q);
        second.q=Sub(positions[ptri->aipos[2]].q,positions[ptri->aipos[0]].q);
        cross.q=Cross(first.q,second.q);
        if(Dot(cross.q,*(qword *)&plane->normal)<0.0f){int t=ptri->aipos[1];ptri->aipos[1]=ptri->aipos[2];ptri->aipos[2]=t;}
    }
    STACKSCOPE_ctor_1AE4A0(&storage,2);
}
} // namespace M_func_001F6C08

// 0x1F6EF8 UpdateSoPartMesh
namespace M_func_001F6EF8 {
typedef union {qword q;float f;} V;
void InitPlaneFromPositions(void*,qword,qword,qword) __asm__("InitPlaneFromPositions");
// Return a + b * s.x (vf5).
extern "C" { static inline qword Step(qword a,qword b,qword s){register qword r __asm__("vf5");
__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=&j"(r):"j"(a),"j"(b),"j"(s));return r;} }
// Store the half-step scalar and return p + old*s + next*s (velocity-Verlet style position step) (inferred).
extern "C" { static inline qword Position(qword p,qword old,qword next,qword half,V *scalar){register qword r __asm__("vf3");
__asm__("sqc2 %5,%1\n\tvadda.xyzw $ACC,%2,$vf0\n\tvmaddax.xyzw $ACC,%3,%5x\n\tvmaddx.xyzw %0,%4,%5x":"=&j"(r),"=m"(scalar->q):"j"(p),"j"(old),"j"(next),"j"(half));return r;} }
// Squared length of the xyz part of q as a float.
extern "C" { static inline float LengthSquared(qword q){qword r,one;float f;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(r),"=&j"(one):"j"(q));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(r));return f;} }
// Copy the velocity array pointer through pinned registers.
extern "C" { static inline qword *VelocityCursor(qword *p){register qword *initial __asm__("$2")=p;register qword *r __asm__("$4");__asm__("daddu %0,%1,$zero":"=r"(r):"r"(initial));return r;} }
// a < b (slt) in $2.
extern "C" { static inline int PlaneLess(int a,int b){register int r __asm__("$2");__asm__("slt %0,%1,%2":"=r"(r):"r"(a),"r"(b));return r;} }
// Integrates a soft-mesh part's vertices by dt (semi-implicit), tracks the largest radius and invalidates the part
// after large moves.
// Also known as: IntegrateSoeMesh (upstream readable / Sly 1 name; IntegrateSoeMesh__FP3SOEf).
extern "C" void UpdateSoPartMesh(SOPART *so,float dt)
{
 V scalar;
 register int vertexIndex __asm__("a2")=0;
 qword *velocity=(qword *)so->plvo->mpiposvLocal;
 register int initialCount __asm__("$3");
 register qword *acceleration __asm__("$5")=({register qword *r __asm__("$5");__asm__("lw %0,%2\n\tlw %1,%3":"=&r"(initialCount),"=r"(r):"m"(so->meshLocal.numPositions),"m"(so->plvo->mpiposdvLocal));r;});
 register qword *positions __asm__("s4");
 float maximum=0.0f;
 positions=(qword *)so->meshLocal.positions;
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
     __asm__("sqc2 %4,%2\n\tlqc2 %0,%3\n\tlw %1,%5":"=j"(force),"=r"(state),"=m"(scalar.q):"m"(*acceleration),"j"(time),"m"(so->meshStamp));
     next=Step(old,force,time);
     newpoint=Position(point,old,next,half,&scalar);
     if(state!=invalid && 1.0f<LengthSquared(VuSub(point,newpoint)))so->meshStamp=invalid;
     square=LengthSquared(newpoint);
     *position=newpoint;
     *velocityCursor=next;
     if(maximum<square)maximum=square;
     limit=so->meshLocal.numPositions;
   }while(++velocityCursor,++position,++acceleration,++vertexIndex<limit);
 }
 so->radius=SqrtFloat(maximum);
 {
   SURFX *firstTri=so->plvo->mpisurfsurfx;
   char *firstPlane=(char*)so->meshLocal.surfaces;
   register int planeIndex __asm__("s3")=0;
   if(so->meshLocal.numSurfaces>0){SURFX *tri=firstTri;char*plane=firstPlane;do{
     qword a=positions[tri->aipos[0]],b=positions[tri->aipos[1]],c=positions[tri->aipos[2]];
     __asm__("addiu %0,%0,12":"+r"(tri));
     InitPlaneFromPositions(plane,a,b,c);
     plane+=0x20;
   }while(PlaneLess(++planeIndex,so->meshLocal.numSurfaces));}
 }
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_int[(sizeof(int)==4)?1:-1];
typedef char layout_float[(sizeof(float)==4)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_V[(sizeof(V)==16)?1:-1];
typedef char layout_V_alignment[(__alignof__(V)==16)?1:-1];
} // namespace M_func_001F6EF8

// 0x1F70C0 CloneSoPartGeomInserting
namespace M_func_001F70C0 {
struct M4; struct FRAME;
// Insert zeroed vertices, rebuild transformed geometry, and rebase its tree.
// 4x4 matrix of U rows with copy constructor and assignment.
struct M4 {QWROW rows[4];M4(){}M4(const M4&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];rows[3]=m.rows[3];}M4&operator=(const M4&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];rows[3]=m.rows[3];return *this;}};
struct FRAME {int pad[4];};
extern FRAME *STACKSCOPE_ctor(FRAME *) __asm__("STACKSCOPE_ctor");
extern void STACKSCOPE_ctor_1AE4A0(FRAME *,int) __asm__("_$_10STACKSCOPE");
extern void *AllocClearedFromStack(SWALLOC *) __asm__("AllocClearedFromStack");
extern void CopyAb__FPvT0Ui(void *,const void *,unsigned int) __asm__("CopyAb__FPvT0Ui");
extern M4 *InvertMatrix4Rigid(M4 *,const M4 *) __asm__("InvertMatrix4Rigid");
extern M4 *__ml__FRC7MATRIX4T0(M4 *,const M4 *,const M4 *) SLY2_SRET_LABEL("__ml__FRC7MATRIX4T0", "MulMatrix4Into");

// Squared length of the xyz part of a as a float.
static inline float SquaredLength(qword a){qword one;float f;__asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(a),"=&j"(one));__asm__("mtc1 %1,%0":"=f"(f):"r"(a));return f;}
/* volatile: the game's sqrt.s is a barrier for the scheduler (the radius store must precede the next constant). */
static inline float Sqrt(float f){float r;__asm__ volatile("sqrt.s %0,%1":"=f"(r):"f"(f));return r;}
// Copies a part's geometry into another space, inserting zeroed vertices, and rebuilds the world geometry and BSP.
// Also known as: RebuildSoe (upstream readable / Sly 1 name; RebuildSoe__FP3SOEP2XFiT0).
extern "C" void CloneSoPartGeomInserting(SOPART *source,XF *space,int inserted,SOPART *dest)
{
    M4 transform;
    GEOM temporary;
    FRAME frame;
    STACKSCOPE_ctor(&frame); /* explicit begin/end calls (POD frame), not a ctor/dtor class: that reorders the argument copies. */
    SWALLOC request;SWALLOC *allocation=&request;
    float maximum=0.0f;
    dest->transform=space;dest->owner=source->owner;dest->oid=source->oid;dest->unknown1C=source->unknown1C;
    {for(int i=0;i<source->meshLocal.numPositions;i++){float length=SquaredLength(*(qword *)&source->meshLocal.positions[i]);if(maximum<length)maximum=length;}}
    dest->radius=Sqrt(maximum);
    temporary=source->meshLocal;
    request.cb=(temporary.numPositions+inserted)*16;allocation->cbAlign=16;
    qword *points=(qword *)AllocClearedFromStack(allocation);
    int pinned=source->unknown1C;
    CopyAb__FPvT0Ui(points,temporary.positions,pinned*16);
    CopyAb__FPvT0Ui(points+(pinned+inserted),temporary.positions+pinned,(temporary.numPositions-pinned)*16);
    temporary.numPositions+=inserted;temporary.positions=(VECTOR4 *)points;
    {M4 product,inverse;__ml__FRC7MATRIX4T0(&product,(InvertMatrix4Rigid(&inverse,(M4 *)&space->worldMatrix),&inverse),(M4 *)&source->transform->worldMatrix);transform=product;}
    CloneGeom(&temporary,(MATRIX4 *)&transform,&dest->meshLocal);
    CloneGeom(&dest->meshLocal,&space->worldMatrix,&dest->meshWorld);
    CloneBspc(&source->bspc,&source->meshWorld,&dest->bspc,&dest->meshWorld);
    STACKSCOPE_ctor_1AE4A0(&frame,2);
}
} // namespace M_func_001F70C0

// 0x1F72F8 IsPosInSo
namespace M_func_001F72F8 {
int FindBspLeafContainingPointFast(void *, uptr, qword) __asm__("FindBspLeafContainingPointFast");
// Slot 73: returns 1 when pos is inside any visible sphere part or part BSP of the object.
// Also known as: FAloContainsPoint (upstream readable / Sly 1 name; FAloContainsPoint__FP3ALOG9VU_VECTOR).
extern "C" int IsPosInSo(SO *so, qword pos)
{
    int i;
    for (i = 0; i < so->numParts; i++) {
        SOPART *pe = &so->parts[i];
        if (pe->isHidden) continue;
        if (pe->unknownFlag10B) {
            qword d, one;
            float r2, d2;
            int t;
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pos), "j"(*(qword *)pe->transform->worldMatrix.mat[3]));
            r2 = pe->radius * pe->radius;
            __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "0"(d));
            __asm__("qmfc2.ni %0, %1" : "=r"(t) : "j"(d));
            __asm__("mtc1 %1, %0" : "=f"(d2) : "r"(t));
            { int f = d2 < r2; if (f) return 1; }
        }
        if (pe->bspc.nodes && FindBspLeafContainingPointFast(pe->bspc.nodes, SLY2_BSP_NO_LIMIT, pos)) return 1;
    }
    return 0;
}
} // namespace M_func_001F72F8

// 0x1F7410 InflictSoContactZaps
namespace M_func_001F7410 {
struct H; struct N;
/* XP as seen from one side (two of them, 0x40 each) and the contact chain. */
struct H { SOPART *part; int pad; void *so; void *object; qword pos; qword posLocal; qword velocity; };
struct N { struct H h[2]; char pad[0xAC - 0x80]; struct N *next; };
LO *FindZapResponder(void *, int) __asm__("FindZapResponder");
int DeliverZapToSo(ZPR *) __asm__("DeliverZapToSo");
void FreeSwXpList__FP2SWP2XP(SW *, void *) __asm__("FreeSwXpList__FP2SWP2XP");
// Slot 74: for each contact whose other part has zap kinds, builds a zap record and delivers it; frees the list.
// (inferred)
// Also known as: DispatchAloSensorContacts (upstream readable / Sly 1 name; DispatchAloSensorContacts__FP3ALO).
extern "C" void InflictSoContactZaps(SO *so)
{
    struct N *pn;
    for (pn = (struct N *)so->physicsTree->firstZapContact; pn; pn = pn->next) {
        int i = pn->h[0].so != so;
        struct H *ph = (struct H *)((char *)pn + (i << 6));
        if (ph->part->zapKindFlags) {
            struct H *pot = (struct H *)((char *)pn + ((1 - i) << 6));
            LO *r = FindZapResponder(pot->object, ph->part->zapKindFlags);
            if (r) {
                ZPR m;
                func_002333B4(&m, 0, 0x60);
                m.ploSource = so;
                m.part = ph->part;
                m.unknown08 = r;
                m.zpk = ph->part->zapKindFlags;
                m.dhp = ph->part->damage;
                m.position = pot->pos;
                m.velocity = ph->velocity;
                DeliverZapToSo(&m);
                if (!so->physicsTree) break;
            }
        }
    }
    if (so->physicsTree) {
        FreeSwXpList__FP2SWP2XP(&g_sw, so->physicsTree->firstZapContact);
        so->physicsTree->firstZapContact = 0;
    }
}
} // namespace M_func_001F7410

// 0x1F7530 FindZapResponder
namespace M_func_001F7530 {
int GetSoZapResponseFlags(void *p, void *q) __asm__("GetSoZapResponseFlags");
// Returns the first LO up the parent chain whose zap response to the given kinds is nonzero.
// Also known as: PaloXpFilter (upstream readable / Sly 1 name; PaloXpFilter__FP3ALOUi).
extern "C" LO *FindZapResponder(LO *lo, void *q)
{
    for (; lo; lo = lo->parent) {
        if (GetSoZapResponseFlags(lo, q))
            return lo;
    }
    return 0;
}
} // namespace M_func_001F7530

// 0x1F7588 CanSoTakeDamage
// Slot 75 (so.h TakeDamage): returns 1 when the zap record has any flag bits (grfzpr, +0x44), i.e. the zap
// hurts (inferred).
// Also known as: FAbsorbSoWkr (upstream readable / Sly 1 name; FAbsorbSoWkr__FP2SOP3WKR).
extern "C" int CanSoTakeDamage(SO *so, ZPR *pzpr)
{
    return pzpr->grfzpr != 0;
}

// 0x1F7598 DeliverZapToSo
namespace M_func_001F7598 {
struct M; struct M2; struct M3;
/* The zap message (MSGID 0xB) payload. */
struct M { SO *po; LO *ploSource; ZPR *zpr; };
struct M2 { SO **pso; int *zapKinds; };
struct M3 { ZPR *zpr; };
SO *GetSoFromLo(void *) __asm__("GetSoFromLo");
int GetSoZapResponseFlags(SO *, int) __asm__("GetSoZapResponseFlags");
ZPR *AllocZeroedFromSlotheap(void *) __asm__("AllocZeroedFromSlotheap");
void ClearDle__FP3DLE(void *) __asm__("ClearDle__FP3DLE");
void AppendDlEntry__FP2DLPv(void *, void *) __asm__("AppendDlEntry__FP2DLPv");

// Delivers a zap (damage) record to the first object in the target chain that accepts it: skips zaps already in the
// recent-zap list, computes the response and calls the object's zap handler.
// Sly 1 zap.cpp vocabulary (InflictZpdZap, FInflictSoZap).
// Also known as: PaloAcceptContact (upstream readable / Sly 1 name; PaloAcceptContact__FP5CTMSG).
extern "C" SO *DeliverZapToSo(ZPR *pk)
{
    int unused[4];
    struct M m;
    struct M2 m2;
    SO *po;
    for (po = GetSoFromLo(pk->unknown08); po; po = po->parent) {
        ZPR *pn;
        for (pn = *(ZPR **)&g_sw.dlZprHit; pn; pn = (ZPR *)pn->dle.next) {
            if (pn->ploSource == pk->ploSource && pn->part == pk->part && pn->zpk == pk->zpk && pn->psoHit == po) return 0;
        }
        pk->grfzpr = GetSoZapResponseFlags(po, pk->zpk);
        if (po->TakeDamage(pk)) {
            ZPR *pkNew;
            pk->psoHit = po;
            pkNew = AllocZeroedFromSlotheap((*(char (*)[])&g_sw.slotheapZprHit));
            *pkNew = *pk;
            pkNew->hitTime = g_clock.time;
            ClearDle__FP3DLE(&pkNew->dle);
            AppendDlEntry__FP2DLPv(&g_sw.dlZprHit, pkNew);
            if (pk->grfzpr & 0xF8) {
                {
                    m.po = po; m.ploSource = pk->ploSource; m.zpr = pk;
                    po->SendMessage((MSGID)0xB, &m);
                    pk->ploSource->SendMessage((MSGID)0xB, &m);
                }
                {
                    m2.pso = &po; m2.zapKinds = &pk->zpk;
                    pk->ploSource->HandleSpliceEvent(0xA, 1, 2, (void **)&m2, 0);
                }
                {
                    m2.pso = (SO **)pk;
                    po->HandleSpliceEvent(0xB, 1, 2, (void **)&m2, 0);
                }
            }
            return po;
        }
    }
    return 0;
}
} // namespace M_func_001F7598

// 0x1F77C0 ExpireSwZapRecords
namespace M_ExpireSwZapRecords {
// Drops zap-history entries older than 0.25 s from the world's list.
// Also known as: AgeSwContacts (upstream readable / Sly 1 name; AgeSwContacts__FPv).
extern "C" void ExpireSwZapRecords(SW *so)
{
    struct DLI dli;
    ZPR *pe;
    InitDli(&dli, &so->dlZprHit);
    while ((pe = (__typeof__(pe))PvNextDli(&dli)) != 0) {
        if (g_clock.time < pe->hitTime + 0.25f) continue;
        M_func_001F37C8::RemoveDlEntry__FP2DLPv(&so->dlZprHit, pe);
        FreeSlotheapSlot(&so->slotheapZprHit, pe);
    }
    g_pdliFirst = dli.m_pdliNext;
}
} // namespace M_ExpireSwZapRecords

// 0x1F78A0 GetZapKindIndex
namespace M_func_001F78A0 {
// Index of the lowest set bit among the 26 zap kinds.
// Also known as: IbitLow (upstream readable / Sly 1 name; IbitLow__FUi).
extern "C" int GetZapKindIndex(int x)
{
    int i; int m;
    for (i = 0, m = 1; i < 26; i++, m <<= 1) if (x & m) break;
    return i;
}
} // namespace M_func_001F78A0

// 0x1F78D8 GetSoZapResponseFlags
namespace M_GrfzpdFromSoGrfzpk {
// OR of the object's per-kind response bytes (+0x110[26]) for the kinds in grfzpk (Sly 1 GrfzpdFromSoGrfzpk).
// Also known as: GrfAloXpBytes (upstream readable / Sly 1 name; GrfAloXpBytes__FP3ALOUi).
extern "C" int GetSoZapResponseFlags(SO *so, int f)
{
    int r = 0; int i; int m;
    for (i = 0, m = 1; i < 26; i++) { if (m & f) r |= so->zapKindFlags[i]; m <<= 1; }
    return r;
}
} // namespace M_GrfzpdFromSoGrfzpk

// 0x1F7918 SetSoZapResponse
namespace M_func_001F7918 {
// Sets the object's response byte to b for each zap kind in grfzpk.
// Also known as: SetAloXpBytes (upstream readable / Sly 1 name; SetAloXpBytes__FP3ALOUiUc).
extern "C" void SetSoZapResponse(SO *so, int f, int b)
{
    int i; int m;
    for (i = 0, m = 1; i < 26; i++) { if (m & f) so->zapKindFlags[i] = b; m <<= 1; }
}
} // namespace M_func_001F7918

// 0x1F7950 HandleSoTouchZaps
namespace M_func_001F7950 {
struct G2;
typedef unsigned long ulong;
struct G2 { LO *p; };
extern struct G2 D_002F7900 __asm__("D_002F7900");
void SetSoZapMode(SO *, int) __asm__("SetSoZapMode");
qword GetLoWorldPos(SO *) __asm__("GetLoWorldPos");
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void DeliverZapToSo(ZPR *) __asm__("DeliverZapToSo");
// Deferred touch callback: processes the contacts' zaps (slot 74), then emits the object's own zap according to
// its zap mode. (inferred)
// Also known as: EmitAloContact (upstream readable / Sly 1 name; EmitAloContact__FP3ALO).
extern "C" void HandleSoTouchZaps(SO *so)
{
    int k;
    ZPR m;
    if (!so->physicsTree) return;
    if (so->physicsTree->firstZapContact) so->InflictContactZaps();
    if (!so->physicsTree) return;
    k = so->zapMode;
    if (!k) return;
    SetSoZapMode(so, 0);
    func_002333B4(&m, 0, 0x60);
    m.unknown08 = so;
    m.position = GetLoWorldPos(so);
    m.velocity = so->GetVelocity();
    if (k == 3 && IsBasicDerivedFrom__FP5BASIC3CID(so, 0x38)) {
        m.zpk = 0x40000; m.dhp = -50; m.ploSource = D_002F7900.p;
    } else {
        m.ploSource = so; m.zpk = 0x20000; m.dhp = so->dhpTouch;
    }
    DeliverZapToSo(&m);
}
} // namespace M_func_001F7950

// 0x1F7A68 SetSoZapMode
namespace M_func_001F7A68 {
// Sets the zap mode (+0x148 bits 24-25, 0..3); mode 2 makes the parts with zap kind 0 damaging (0x10000) with the
// object's hit damage, the other modes make them harmless.
// Also known as: SetAloEntryMode (upstream readable / Sly 1 name; SetAloEntryMode__FP3ALOi).
extern "C" void SetSoZapMode(SO *so, int k)
{
    int from, to, s;
    int i;
    if (k == (int)so->zapMode) return;
    switch (k) {
    case 0: case 1: case 3:
        from = 0x10000; to = 0; s = -10;
        break;
    case 2:
        s = so->dhpHit; from = 0; to = 0x10000;
        break;
    }
    for (i = 0; i < so->numParts; i++) {
        SOPART *pe = &so->parts[i];
        if (pe->zapKindFlags == from) { pe->zapKindFlags = to; pe->damage = s; }
    }
    so->zapMode = (unsigned int)k;
}
} // namespace M_func_001F7A68

// 0x1F7B40 GetSoZapMode
namespace M_func_001F7B40 {
// Returns the zap mode.
extern "C" int GetSoZapMode(SO *p) { return p->zapMode & 3; }
} // namespace M_func_001F7B40

// 0x1F7B58 SetSoHitDamage
// Sets the hit damage (stored negated in the byte at +0x148; default 10). (inferred)
extern "C" void SetSoHitDamage(SO *so, int n)
{
    so->dhpHit = -n;
}

// 0x1F7B68 GetSoHitDamage
// Returns the hit damage. (inferred)
extern "C" int GetSoHitDamage(SO *so)
{
    return -so->dhpHit;
}

// 0x1F7B78 SetSoHitDamage2
// Sets the second hit damage (byte +0x149, negated; default 20). (inferred)
extern "C" void SetSoHitDamage2(SO *so, int n)
{
    so->dhpTouch = -n;
}

// 0x1F7B88 GetSoHitDamage2
// Returns the second hit damage. (inferred)
extern "C" int GetSoHitDamage2(SO *so)
{
    return -so->dhpTouch;
}

// 0x1F7B98 EnsureSoInSwList187
/* Gives the object a CID 0x12 child with oid 0x187 unless it has one. */
// Adds the object to world list 0x187 if it is not in it yet (meaning of the list unknown). (inferred)
extern "C" void EnsureSoInSwList187(SO *so)
{
    if (FindSwObject(&g_sw, 2, (OID)0x187, so) == 0) {
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
namespace M_FadeSoOut {
void SetSoFadeRate(void *, float) __asm__("SetSoFadeRate");
void SetSoAlpha(void *, float) __asm__("SetSoAlpha");
// Fades the object out over dur seconds (alpha rate -1/dur); dur <= 0 removes it at once.
// Also known as: FadeAloOut (upstream readable / Sly 1 name; FadeAloOut__FP3ALOf).
extern "C" void FadeSoOut(void *so, float f)
{
    if (f <= 0.0f) {
        SetSoFadeRate(so, 0.0f);
        SetSoAlpha(so, 0.0f);
    } else {
        SetSoFadeRate(so, -1.0f / f);
    }
}
} // namespace M_FadeSoOut

// 0x1F7C68 FadeSoIn
namespace M_FadeSoIn {
void SetSoFadeRate(SO *, float) __asm__("SetSoFadeRate");
void SetSoAlpha(SO *, float) __asm__("SetSoAlpha");
// Fades the object in over dur seconds, re-adding it if it was removed; dur <= 0 shows it at once.
// Also known as: FadeAloIn (upstream readable / Sly 1 name; FadeAloIn__FP3ALOf).
extern "C" void FadeSoIn(SO *so, float f)
{
    if (f <= 0.0f) {
        SetSoFadeRate(so, 0.0f);
        SetSoAlpha(so, 1.0f);
    } else {
        if (so->isRemoved) {
            so->Add();
            so->opacity = 0.0f;
        }
        SetSoFadeRate(so, 1.0f / f);
    }
}
} // namespace M_FadeSoIn

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
namespace M_SetSoFadeRate {
// Sets the fade rate; a nonzero rate adds the object and marks it for updates.
// Also known as: SetFade (upstream readable / Sly 1 name; SetFade__FP3ALOf).
extern "C" void SetSoFadeRate(SO *so, float g)
{
    if (g != 0.0f) {
        sptr v;
        so->Add();
        v = (sptr)so->ploupd;
        if ((v ^ 1) & 1) so->ploupd->grf |= 2;
        else so->inlineFlags = v | 2;
    }
    so->opacitySpeed = g;
}
} // namespace M_SetSoFadeRate

// 0x1F7E08 CollectSoParts
namespace M_func_001F7E08 {
// Collects the parts that pass an optional predicate into an array; returns the count.
// Also known as: CsoeCollect (upstream readable / Sly 1 name; CsoeCollect__FP3ALOPvN21).
extern "C" int CollectSoParts(SO *so, int (*pfn)(void *, SOPART *), void *pv, SOPART **ape)
{
    int n = 0;
    int i;
    for (i = 0; i < so->numParts; i++) {
        SOPART *pe = &so->parts[i];
        if (!pfn || pfn(pv, pe)) {
            ape[n++] = pe;
        }
    }
    return n;
}
} // namespace M_func_001F7E08

// 0x1F7EC8 SetSoUnknownBits16
namespace M_func_001F7EC8 {
// Sets the 4-bit field at bits 16-19 of +0x148 (meaning unknown, a splice option).
extern "C" void SetSoUnknownBits16(SO *p, int n) { p->unknownBits148A = n; }
} // namespace M_func_001F7EC8

// 0x1F7EF0 GetSoUnknownBits16
namespace M_func_001F7EF0 {
// Returns the 4-bit field at bits 16-19 of +0x148.
extern "C" int GetSoUnknownBits16(SO *p) { return p->unknownBits148A; }
} // namespace M_func_001F7EF0

// 0x1F7F08 GetSoNoJtPlace
namespace M_func_001F7F08 {
// Returns the no-JT-place flag (+0x148 bit 29, so.h isNotLandable).
extern "C" int GetSoNoJtPlace(SO *p) { return p->isNotLandable; }
} // namespace M_func_001F7F08

// 0x1F7F20 SetSoNoJtPlace
namespace M_func_001F7F20 {
// Sets the no-JT-place flag.
extern "C" void SetSoNoJtPlace(SO *p, int n) { p->isNotLandable = n; }
} // namespace M_func_001F7F20

// 0x1F7F48 SetSoTreeRoot
namespace M_func_001F7F48 {
// Sets the tree-root flag (+0x148 bit 34, so.h isTreeRoot) and rebuilds the physics hook when it changed. (inferred)
// Also known as: SetAloPhysRootFlag (upstream readable / Sly 1 name; SetAloPhysRootFlag__FP3ALOl).
extern "C" void SetSoTreeRoot(SO *so, long f)
{
    if (f != so->isTreeRoot) {
        so->isTreeRoot = f;
        RebuildSoPhysHook(so);
    }
}
} // namespace M_func_001F7F48

// 0x1F7FA0 SetSoScrollerSpeeds
namespace M_func_001F7FA0 {
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void SetScrollerMasterSpeeds__FP8SCROLLERff(void *, float, float) __asm__("SetScrollerMasterSpeeds__FP8SCROLLERff");
// Sets the master speeds of every scroller (CID 0x8F) in the object's animation list.
extern "C" void SetSoScrollerSpeeds(SO *so, float f, float g)
{
    int i;
    for (i = 0; i < so->model.cpsaa; i++) {
        void *q = so->model.apsaa[i];
        if (IsBasicDerivedFrom__FP5BASIC3CID(q, 0x8F)) SetScrollerMasterSpeeds__FP8SCROLLERff(q, f, g);
    }
}
} // namespace M_func_001F7FA0

// 0x1F8038 SetSoSaaParams
namespace M_func_001F8038 {
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void SetSaiDuDv(void *, float, float) __asm__("SetSaiDuDv");
// Passes two parameters to every CID 0x8F entry of the object's animation list (inferred).
extern "C" void SetSoSaaParams(SO *so, float f, float g)
{
    int i;
    for (i = 0; i < so->model.cpsaa; i++) {
        void *q = so->model.apsaa[i];
        if (IsBasicDerivedFrom__FP5BASIC3CID(q, 0x8F)) SetSaiDuDv(q, f, g);
    }
}
} // namespace M_func_001F8038

// 0x1F80D0 SetSoEyesClosed
namespace M_func_001F80D0 {
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void SetEyesClosed(void *, float) __asm__("SetEyesClosed");
// Opens or closes the eyes (CID 0x8E) of the object and its SO children.
// Also known as: SetAloEyesClosed (upstream readable / Sly 1 name; SetAloEyesClosed__FP3ALOf).
extern "C" void SetSoEyesClosed(SO *so, float f)
{
    int i;
    SO *q;
    for (i = 0; i < so->model.cpsaa; i++) {
        SAA *pv = so->model.apsaa[i];
        if (IsBasicDerivedFrom__FP5BASIC3CID(pv, 0x8E)) SetEyesClosed(pv, f);
    }
    for (q = (SO *)so->children.head; q != (SO *)so->ploRemovedFirst; q = (SO *)q->siblingLink.next)
        if (IsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) SetSoEyesClosed(q, f);
}
} // namespace M_func_001F80D0

// 0x1F8198 FindSoSaa
namespace M_PsaaFindSoOid {

   /* an SAA: its oid (+0x18) */
// Finds an entry of the object's animation list by oid (Sly 1 PsaaFindSoOid).
extern "C" struct SAA *FindSoSaa(SO *so, int n)
{
    int i;
    for (i = 0; i < so->model.cpsaa; i++) { struct SAA *pt = (struct SAA *)so->model.apsaa[i]; if (pt->unknown18 == n) return pt; }
    return 0;
}
} // namespace M_PsaaFindSoOid

// 0x1F81E0 SetSoAnimIndex
namespace M_func_001F81E0 {
// Selects one of the model's shader oids: selectedShader = -(index in the list + 3), or 0 when not found. (inferred)
extern "C" void SetSoAnimIndex(SO *so, int n)
{
    int i;
    if (n == -1) {
        so->model.selectedShader = 0;
        return;
    }
    for (i = 0; i < so->model.numShaderOids; i++) {
        if (n == so->model.shaderOids[i]) {
            so->model.selectedShader = -(i + 3);
            return;
        }
    }
    so->model.selectedShader = 0;
}
} // namespace M_func_001F81E0

// 0x1F8238 ClearSoAnimIndex
// Clears the selected shader (+0xB4 = 0). (inferred)
extern "C" void ClearSoAnimIndex(SO *so)
{
    so->model.selectedShader = 0;
}

// 0x1F8240 FindTargetRecordById
namespace M_func_001F8240 {
// Finds the target record of an object in the world's list (g_sw.dlTargetrec), or 0.
TARGETREC *FindTargetRecordById(LO *plo) __asm__("FindTargetRecordById"); TARGETREC *FindTargetRecordById(LO *plo) { TARGETREC *pt; for (pt = (TARGETREC *)g_sw.dlTargetrec.head; pt; pt = (TARGETREC *)pt->dleTargetrec.next) if (pt->target == plo) return pt; return 0; }
} // namespace M_func_001F8240

// 0x1F8278 EnsureTargetRecordById
/* Makes a target record (room for 32 guards, InitTargetRecord) for plo unless it has one. */
// Returns the object's target record, creating one when missing.
// Also known as: PtargetrecOf (upstream readable / Sly 1 name; PtargetrecOf__FP2LO).
extern "C" TARGETREC *EnsureTargetRecordById(LO *plo)
{
    TARGETREC *ptargetrec = FindTargetRecordById(plo);
    if (ptargetrec == 0) {
        ptargetrec = InitTargetRecord((TARGETREC *)func_001AE4F8(SLY2_OFFSETOF(TARGETREC, guards) + 32 * sizeof(STEPGUARD *)), plo);
    }
    return ptargetrec;
}

// 0x1F82B8 SetSoTargetRecordParams
namespace M_func_001F82B8 {
/* The two words after TARGETREC::facingKind: unknown14 and facingTolerance (copied as one 8-byte record). */
struct K { float unknown14; float facingTolerance; };
// Ensures the object's target record and sets its facing kind and, when given, its two facing parameters.
// (inferred)
extern "C" void SetSoTargetRecordParams(LO *p, int x, struct K *pk) { TARGETREC *pt = EnsureTargetRecordById(p); pt->facingKind = x; if (pk) *(struct K *)&pt->unknown14 = *pk; }
} // namespace M_func_001F82B8

// 0x1F8300 EnsureSoPhysRegionRecord
namespace M_func_001F8300 {
struct CB;
struct CB { int cb; int n; };
void *AllocClearedFromWorldHeap(struct CB *) __asm__("AllocClearedFromWorldHeap");
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
// Fill the request and allocate cb zeroed bytes (kind n) from the world heap.
extern "C" { static inline void *PvAllocMsg(struct CB *pcb, int cb, int n) { pcb->cb = cb; pcb->n = n; return AllocClearedFromWorldHeap(pcb); } }
// Ensures the physics region record {object, three tuning floats with defaults 75 (150 for class 0x34) and 1}.
// (inferred)
// Also known as: PregentEnsure (upstream readable / Sly 1 name; PregentEnsure__FP3ALO).
REGENT *EnsureSoPhysRegionRecord(SO *so) __asm__("EnsureSoPhysRegionRecord"); REGENT *EnsureSoPhysRegionRecord(SO *so)
{
    if (!so->physics) return 0;
    if (!so->physics->pregent) {
        struct CB cb;
        REGENT *q = (__typeof__(q))PvAllocMsg(&cb, sizeof(REGENT), 0x10);
        q->pso = so;
        if (IsBasicDerivedFrom__FP5BASIC3CID(so, 0x34)) q->radius = 150.0f;
        else q->radius = 75.0f;
        q->mass = 1.0f;
        so->physics->pregent = q;
    }
    return so->physics->pregent;
}
} // namespace M_func_001F8300

// 0x1F83B0 SetSoRegionRadius
// Sets the object's REGENT radius (the path follower's avoidance radius): level option 197.
extern "C" void SetSoRegionRadius(STEP *step, float g)
{
    REGENT *prec;
    prec = EnsureSoPhysRegionRecord(step);
    if (prec != 0) {
        prec->radius = g;
    }
}

// 0x1F83E0 GetSoRegionRadius

// Returns the object's REGENT radius (0 without a record): level option 197.
extern "C" float GetSoRegionRadius(SO *p)
{
    REGENT *pu = 0;
    if (p->physics) pu = p->physics->pregent;
    if (pu) return pu->radius;
    return 0.0f;
}


// 0x1F8410 SetSoRegionMass
// Sets the object's REGENT mass (the path follower's avoidance weight): level option 198.
extern "C" void SetSoRegionMass(STEP *step, float g)
{
    REGENT *prec;
    prec = EnsureSoPhysRegionRecord(step);
    if (prec != 0) {
        prec->mass = g;
    }
}

// 0x1F8440 GetSoRegionMass

// Returns the object's REGENT mass (0 without a record): level option 198.
extern "C" float GetSoRegionMass(SO *p)
{
    REGENT *pu = 0;
    if (p->physics) pu = p->physics->pregent;
    if (pu) return pu->mass;
    return 0.0f;
}


// 0x1F8470 SetSoRegionHeightOffset
// Sets the object's REGENT height offset (its centre above its position): level option 199.
extern "C" void SetSoRegionHeightOffset(STEP *step, float g)
{
    REGENT *prec;
    prec = EnsureSoPhysRegionRecord(step);
    if (prec != 0) {
        prec->heightOffset = g;
    }
}

// 0x1F84A0 GetSoRegionHeightOffset

// Returns the object's REGENT height offset (0 without a record): level option 199.
extern "C" float GetSoRegionHeightOffset(SO *p)
{
    REGENT *pu = 0;
    if (p->physics) pu = p->physics->pregent;
    if (pu) return pu->heightOffset;
    return 0.0f;
}


// 0x1F84D0 SetSoIceable__FP2SOi
// SO+0x148 bit 33 (64-bit unit at 0x148). Lilly's port calls this bit hurtRespawnsHero; the function name says "iceable".
// Sets the iceable flag (+0x148 bit 33). Sly 1 name.
void SetSoIceable(SO *pso, int f) { pso->hurtRespawnsHero = f; }

// 0x1F8500 GetSoIceable
namespace M_GetSoIceable {
// Returns the iceable flag.
extern "C" int GetSoIceable(SO *p) { return (int)p->hurtRespawnsHero & 1; }   /* bit 33 (so.h hurtRespawnsHero) */
} // namespace M_GetSoIceable

// 0x1F8510 NewSoXfHandle
namespace M_func_001F8510 {
struct Pool;
CID149 *AllocZeroedFromSlotheap(struct Pool *) __asm__("AllocZeroedFromSlotheap");
// Creates an XF handle object (CID 149) for the object's XF matching (oid, kind), for scripts. Built by hand (cleared,
// vptr stored, the counted pointers' words zeroed): a constructor call would count references.
extern "C" CID149 *NewSoXfHandle(SO *so, int k, int m)
{
    int i;
    for (i = 0; i < so->numTransforms; i++) {
        XF *pe = &so->transforms[i];
        if (pe->kind == m && (k == -1 || pe->oid == k)) {
            CID149 *q = AllocZeroedFromSlotheap(&(*(struct Pool *)&g_sw.slotheapXfh));
            func_002333B4(q, 0, sizeof(CID149));
            SLY2_SET_VPTR(q, CID149);
            q->pso = so;
            q->pxf = pe;
            q->scriptFrame.m_p = 0;
            q->scriptVariables.m_p = 0;
            return q;
        }
    }
    return 0;
}
} // namespace M_func_001F8510

// 0x1F85E0 NewSoPartHandle
namespace M_func_001F85E0 {
struct Pool;
SOPART *FindSoPart(SO *pso, int oid) __asm__("FindSoPart");
CID150 *AllocZeroedFromSlotheap(struct Pool *) __asm__("AllocZeroedFromSlotheap");
// Creates a part handle object (CID 150) for the part with the given oid; built by hand like NewSoXfHandle's.
extern "C" CID150 *NewSoPartHandle(SO *pa, int oid)
{
    SOPART *r = FindSoPart(pa, oid);
    CID150 *q;
    if (!r) return 0;
    q = AllocZeroedFromSlotheap(&(*(struct Pool *)&g_sw.slotheapXfh));
    func_002333B4(q, 0, sizeof(CID150));
    q->pso = pa;
    q->psopart = r;
    SLY2_SET_VPTR(q, CID150);
    q->scriptFrame.m_p = 0;
    q->scriptVariables.m_p = 0;
    return q;
}
} // namespace M_func_001F85E0

// 0x1F8668 SetSoBit35Hierarchy
namespace M_func_001F8668 {
void AddLoToRenderTree(SO *) __asm__("AddLoToRenderTree");
void RemoveLoFromRenderTree(SO *) __asm__("RemoveLoFromRenderTree");
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
// Sets bit 35 of +0x148 on the object and its children, registering them when the world requires it. (inferred)
// Also known as: SetAloIced (upstream readable / Sly 1 name; SetAloIced__FP3ALOl).
void SetSoBit35Hierarchy(SO *so, long f) __asm__("SetSoBit35Hierarchy"); void SetSoBit35Hierarchy(SO *so, long f)
{
    SO *q;
    if (f != so->isRenderable) {
        so->isRenderable = f;
        if (g_sw.pblockRender) {
            if (f) AddLoToRenderTree(so);
            else RemoveLoFromRenderTree(so);
        }
        for (q = (SO *)so->children.head; q; q = (SO *)q->siblingLink.next)
            if (IsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) SetSoBit35Hierarchy(q, f);
    }
}
} // namespace M_func_001F8668

// 0x1F8730 IncrefXfHandle
// Handle slot 0: increments the reference count.
void CID148::AddRef()   /* key function: CID148's vtable comes out here */
{
    cref = cref + 1;
}

// 0x1F8740 DecrefXfHandle
/* Drops a reference; the last one clears the splice frame and sidebag, unsubscribes and frees the handle. */
// Decrements the handle's reference count; at 0 destroys it and returns it to the pool.
extern "C" void DecrefXfHandle(CID148 *pxfh)
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
namespace M_func_001F87A0 {
// Returns the XF's 16-bit field +0xAC in thousandths (meaning unknown).
extern "C" float GetXfHandleParamAC(CID149 *p) { return (unsigned short)p->pxf->stiffness * 0.001f; }
} // namespace M_func_001F87A0

// 0x1F87C8 SetXfHandleParamAC
namespace M_func_001F87C8 {
// Sets the XF's field +0xAC (value * 1000) and refreshes its mode.
extern "C" void SetXfHandleParamAC(CID149 *p, float g) { p->pxf->stiffness = (int)(g * 1000.0f); UnsettleXfSlot(p->pso, p->pxf); }
} // namespace M_func_001F87C8

// 0x1F8808 GetXfHandleParamAE
namespace M_func_001F8808 {
// Returns the XF's 16-bit field +0xAE in thousandths.
extern "C" float GetXfHandleParamAE(CID149 *p) { return (unsigned short)p->pxf->damping * 0.001f; }
} // namespace M_func_001F8808

// 0x1F8830 SetXfHandleParamAE
namespace M_func_001F8830 {
// Sets the XF's field +0xAE (value * 1000) and refreshes its mode.
extern "C" void SetXfHandleParamAE(CID149 *p, float g) { p->pxf->damping = (int)(g * 1000.0f); UnsettleXfSlot(p->pso, p->pxf); }
} // namespace M_func_001F8830

// 0x1F8870 GetXfHandleParamAC2
namespace M_func_001F8870 {
// Same as GetXfHandleParamAC for the other handle class.
extern "C" float GetXfHandleParamAC2(CID149 *p) { return (unsigned short)p->pxf->stiffness * 0.001f; }
} // namespace M_func_001F8870

// 0x1F8898 SetXfHandleParamAC2
namespace M_func_001F8898 {
// Same as SetXfHandleParamAC for the other handle class.
extern "C" void SetXfHandleParamAC2(CID149 *p, float g) { p->pxf->stiffness = (int)(g * 1000.0f); UnsettleXfSlot(p->pso, p->pxf); }
} // namespace M_func_001F8898

// 0x1F88D8 GetXfFromHandle
// Returns the handle's XF, refreshing its mode first when asked.
extern "C" XF *GetXfFromHandle(CID149 *pxfh, int n)
{
    if (n == 1) {
        UnsettleXfSlot(pxfh->pso, pxfh->pxf);
    }
    return pxfh->pxf;
}

// 0x1F8910 FindXfHandleActuator
namespace M_func_001F8910 {
void RemoveXfAct(SO *, XF *, ACT *) __asm__("RemoveXfAct");
void FreeAct(ACT *) __asm__("FreeAct");
void SortXfActs(SO *, XF *) __asm__("SortXfActs");
// Finds the XF's actuator of kind k; n = -1 removes it, otherwise it is updated and re-prioritised.
// Also known as: PctrlFindXfh (upstream readable / Sly 1 name; PctrlFindXfh__FP7XFBASICiii).
extern "C" ACT *FindXfHandleActuator(CID149 *handle, int k, int n, int m)
{
    ACT *pe;
    for (pe = handle->pxf->firstActuator; pe; pe = pe->pactNext) {
        if (pe->ack != k) continue;
        if (n == -1) {
            RemoveXfAct(handle->pso, handle->pxf, pe);
            FreeAct(pe);
            return 0;
        }
        if (m != pe->priority) {
            pe->priority = m;
            SortXfActs(handle->pso, handle->pxf);
        }
        if (pe == handle->pxf->firstActuator) {
            SetXfRole(handle->pso, handle->pxf, n);
            UnsettleXfSlot(handle->pso, handle->pxf);
        }
        return pe;
    }
    return 0;
}
} // namespace M_func_001F8910

// 0x1F89E0 SetXfHandleActuator1
namespace M_func_001F89E0 {
ACT *FindXfHandleActuator(CID149 *, int, int, int) __asm__("FindXfHandleActuator");
void SetActPositionGoal(ACT *, qword) __asm__("SetActPositionGoal");
void NewPositionAct(SO *, XF *, int, int, int, qword) __asm__("NewPositionAct");
// Updates or creates the XF's kind-1 actuator (a target vector) with its parameters.
// Also known as: SetXfhPos (upstream readable / Sly 1 name; SetXfhPos__FP7XFBASICiiG9VU_VECTOR).
extern "C" void SetXfHandleActuator1(CID149 *handle, int n, int m, qword q)
{
    ACT *r = FindXfHandleActuator(handle, 1, n, m);
    if (r) {
        r->role = n;
        SetActPositionGoal(r, q);
    } else if (n != -1) {
        NewPositionAct(handle->pso, handle->pxf, m, n, 2, q);
    }
}
} // namespace M_func_001F89E0

// 0x1F8A78 SetXfHandleActuator1World
namespace M_func_001F8A78 {
qword TransformXfPos(XF *pxfFrom, XF *pxfTo, qword q) __asm__("TransformXfPos");
void SetXfHandleActuator1(CID149 *p, int a, int b, qword q) __asm__("SetXfHandleActuator1");
// SetXfHandleActuator1 with a vector converted from the XF's parent space.
extern "C" void SetXfHandleActuator1World(CID149 *handle, int a, int b, qword q)
{
    SetXfHandleActuator1(handle, a, b, TransformXfPos(0, handle->pxf->parent, q));
}
} // namespace M_func_001F8A78

// 0x1F8AD8 SetXfHandleActuator1Mat
namespace M_func_001F8AD8 {
struct M3;
struct M3 { qword a[3]; };
void ConvertXfMat(struct M3 *, XF *, XF *) __asm__("ConvertXfMat");
qword DecomposeRotationMatrixToEuler(struct M3 *) __asm__("DecomposeRotationMatrixToEuler");
void SetXfHandleActuator1(CID149 *, int, int, qword) __asm__("SetXfHandleActuator1");
// SetXfHandleActuator1 with the value taken from a matrix.
extern "C" void SetXfHandleActuator1Mat(CID149 *handle, int a, int b)
{
    struct M3 c, t;
    ConvertXfMat(&t, 0, handle->pxf->parent);
    c = t;
    SetXfHandleActuator1(handle, a, b, DecomposeRotationMatrixToEuler(&c));
}
} // namespace M_func_001F8AD8

// 0x1F8B58 SetXfHandleActuator4
namespace M_func_001F8B58 {
ACT *FindXfHandleActuator(CID149 *, int, int, int) __asm__("FindXfHandleActuator");
void NewPathAct(SO *, XF *, int, int, float, float) __asm__("NewPathAct");
// Updates or creates the XF's kind-4 actuator with two floats.
extern "C" void SetXfHandleActuator4(CID149 *handle, int n, int m, float f, float g)
{
    ACT4 *r = (ACT4 *)FindXfHandleActuator(handle, 4, n, m);
    if (r) {
        r->role = n;
        r->lookAheadWeight = f;
        r->lookAheadTime = g;
    } else if (n != -1) {
        NewPathAct(handle->pso, handle->pxf, m, n, f, g);
    }
}
} // namespace M_func_001F8B58

// 0x1F8BF0 SetXfHandleKind16Params
namespace M_func_001F8BF0 {
struct U; struct V;
struct U { int a, b; int n; float x, y, z, r; int pad; qword q; };
struct V { char pad[0x60]; struct U *pu; };
// For a kind-16 XF: sets its count, vector, angle (degrees to radians) and rate.
extern "C" void SetXfHandleKind16Params(CID149 *handle, int n, qword q, float x, float y, float z, float deg)
{ XF *pt = handle->pxf; if (pt->role == 16) { struct U *pu = ((struct V *)pt->reference)->pu; pu->n = n; pu->x = x; pu->y = y; pu->z = z; pu->r = deg * 0.01745329424738884f; pu->q = q; } }
} // namespace M_func_001F8BF0




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

extern const unsigned int D_002A8270[2] __asm__("D_002A8270") = {0xFFFF0000, (unsigned int)p_func_001F3868};
extern const unsigned int D_002A8278[2] __asm__("D_002A8278") = {0xFFFF0000, (unsigned int)p_func_001F7950};

} // namespace ANPTR_so
