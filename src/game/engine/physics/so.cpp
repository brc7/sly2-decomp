// so.cpp -- SO (CID 0x30 : LO), the solid object: init/load/clone, hierarchy, physics records, constraints, tree
// records and contact queues, parts, bounds, rendering. TU 0x1F2108..0x1F8DB8 (174 functions, address order).
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

extern "C" void *func_002333B4(void *pv, int b, int cb) __asm__("memset");   /* memset */
extern qword D_002A2DC0;
/* 0x1F8C40 StaticInit_so (93 instructions) and its _GLOBAL_$I$ stub func_001F8DB8: g++'s initializer for the file-scope
   object below (built in a zeroed temporary and copied), then the six engine slot pools. D_003DD020 is the default
   query record that tu_1BC050 and guardbhv pass to FindShapeClosestPoint (role HYPOTHESIS; the name is a placeholder). */
struct CPQ
{
    qword q0;               /* D_002A2DC0 */
    int n10;                /* 1 */
    int pad[3];
    CPQ() { func_002333B4(this, 0, sizeof(CPQ)); }
};
static inline CPQ MakeCpq()
{
    CPQ cpq;
    cpq.q0 = D_002A2DC0;
    cpq.n10 = 1;
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
      XF *pxf;
      int nPriority;
      ACT *pactNext;
      int nRole;
    ACT() {}
    ACT(int ack) : ack(ack) {}
};

extern "C" void AddXfAct(SO *pso, XF *pxf, ACT *pact);

ACT *PactNewClone(ACT *pactBase, SO *pso, XF *pxf) __asm__("func_0012E3F0");

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

void *PvAllocSwCopyImpl(struct SWALLOC *pswalloc, void *pvBase) __asm__("func_001AE238");

struct SWALLOC
{
    int cb;
    int cbAlign;
};

void *PvAllocStackImpl(SWALLOC *pswalloc) __asm__("PvAllocSlot__FP8ALLOCREQ");


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

/* The bounds record of an SO (SO::psobounds): its owner and one float the owner keeps in step (func_001F2688). */
struct SOBOUNDS
{
    SO *pso;                /* +0x00 */
    int pad4[7];
    float g20;              /* +0x20 */
};
/* Objects only one function reads: func_001F7588's +0x44 word and func_001F8238's +0xB4 word. */
struct SOWATCH { char pad0[0x44]; int n44; char pad48[0x6C]; int nB4; };
/* The per-step record func_001F8300 finds (three floats at +4, +8, +0xC). */
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
int FIsBasicDerivedFrom(BASIC *pbasic, CID cid);
void AppendDlEntry(DL *pdl, void *pv);
void RemoveDlEntry(DL *pdl, void *pv);
LO *PloFindSwObject(SW *psw, int grffso, OID oid, LO *ploContext);
LO *PloNew(CID cid, SO *psoParent, OID oid, int n);
extern "C" {
SO *PsoFindPhysRoot(SO *pso);
void RecalcSwOxfFilterForObject(SW *psw, SO *pso);
void AdjustSoMomint(SO *pso, float r);
void PostLoLoadV() __asm__("PostLoLoad");      /* a0 passed through */
void UpdateSoXfWorldMatrices(SO *pso);
void BindGlobset(GLOBSET *pglobset);
void UpdateGeomWorld(MATRIX4 *pmat, GEOM *pgeomLocal, GEOM *pgeomWorld);
void func_001F4BF8(SO *pso);
void func_001F4B58(SO *pso);
void func_001F4BA8(SO *pso);
void func_001F3C10(SO *pso);
void RenderSoGlobset(GLOBSET *pglobset, int n, float uAlpha1, float uAlpha2, SOBOUNDS *psobounds);
qword ConvertXfVec(XF *pxfFrom, XF *pxfTo, qword qw);
void SlotUnsettle(SO *pso, XF *pxf);
void AddSwAaobrObject(SW *psw, SO *pso);
void func_002303A0(SO *pso, XF *pxf, int n);
void func_001F5510(SO *pso, XF *pxf, CONSTR *pconstr, CONSTR *pconstrSo);
void func_001F55C8(SO *pso);
void func_001F8668(SO *pso, int n);
void func_001F4E28(SO *pso);
int func_001F8240(LO *plo);
void *func_001AE4F8(int cb);
void func_0020A5F8(void *ptargetrec, LO *plo);
STEPREC *func_001F8300(STEP *pstep);
void FramePtr_Assign(void *ppframe, CFrame *pframe);
void func_002580A8(void *ppsidebag, CSidebag *psidebag);
void UnsubscribeLoAll(XFH *pxfh);
void func_0011CD98(SLOTHEAP *pslotheap, void *pv);
}

// 0x1F2108 InitSo
namespace M_InitSo { extern "C" {
struct F; struct P;
typedef unsigned long ulong;
struct F { ulong lo : 35; ulong b35 : 1; ulong hi : 28; };
struct P { char pad[0x10]; struct P *parent; char pad1[0x12C - 0x14]; float g12C; char pad2[0x13C - 0x130]; float g13C; int n140; int pad3; union { struct { signed char a, b; } c; struct F f; } u; };
extern float D_002C3B90;
void InitLo(struct P *);
void SetLoBusyk(struct P *, int);
void InitDl__FP2DLi(void *, int);
/** @sly2-annot addr=0x1F2108
 * Purpose: SO vt7 Init: InitLo, SetLoBusyk(0), clear DL +0x64, dhp bytes +0x148/+0x149 = -10/-20,
 *   gAlpha(+0x12C)=1, +0x13C=D_002C3B90, inherit +0x148 bit35 from parent, +0x140=-1
 * Info: proposed name InitSo (T1); class SO slot7; TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/guards-ai.md#8-source-files, docs/engine/physics-collision.md#6-source-files,
 *   docs/engine/physics-collision.md#7-key-functions
 * @end-sly2-annot */
void InitSo(struct P *p)
{
    InitLo(p);
    SetLoBusyk(p, 0);
    InitDl__FP2DLi((char *)p + 0x64, 0x24);
    p->u.c.a = -10;
    p->u.c.b = -20;
    p->g12C = 1.0f;
    p->g13C = D_002C3B90;
    p->u.f.b35 = (uchar)(p->parent ? p->parent->u.f.b35 : 1);
    p->n140 = -1;
}
} } // namespace M_InitSo

// 0x1F21B8 SetSoParent
namespace M_SetSoParent {
struct VT; struct MAT; struct E; struct ALO; struct LO10; struct LO;
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct VT { short d; short pad; void *pfn; };
struct MAT { float m[4][4]; } __attribute__((aligned(16)));
struct E { char pad[0xA0]; struct E *pe; char pad2[0x100 - 0xA4]; };
struct LO;
struct ALO { char pad[0x40]; struct MAT mat; char pad2[0xA0 - 0x80]; struct ALO *pNext; };
struct LO10 { char pad[0x50]; int c; struct E *ae; char pad2[0x148 - 0x58]; ulong fl; };
struct LO {
    char pad[0xC]; struct VT *pvt; struct LO10 *p10; char pad2[0x28 - 0x14]; ulong pad28 : 60; ulong b60 : 1; ulong pad61 : 3;
    char pad3[0x50 - 0x30]; int c; struct E *ae; char pad4[0x60 - 0x58]; struct ALO *palo;
};
extern "C" {
void func_002333B4(void *, int, int) __asm__("memset");
void __ml__FRC7MATRIX4T0(struct MAT *, struct ALO *, struct MAT *);
void func_001A5900(struct LO *, struct LO *, struct LO *, int);
void UpdateSoXfWorldMatrices(struct LO *);
void ReleaseSoBoundsHierarchy(struct LO *);
void EnsureSoBoundsHierarchy(struct LO *);
void func_001F8668(struct LO *, int);
}

#define VCALL(po, slot, T) ((T)(po)->pvt[slot].pfn)((void *)((char *)(po) + (po)->pvt[slot].d)

/** @sly2-annot addr=0x1F21B8
 * Purpose: SO vt10: compute old world matrix / velocities (vt33/vt34), LO::SetParent, re-point XF parents
 *   into new parent, rebind contacts and bounds (0x1F2468/0x1F26A8), refresh phys flag
 * Info: proposed name SetSoParent (T1); class SO slot10; TU so
 * Notes: names_v2 C proposed PivotSo__FP2SOi (0.196)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/formats/meshes-scene.md#4-scene-assembly-and-placement,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void SetSoParent(struct LO *plo, struct LO *pa, struct LO *pb, int grf)
{
    struct MAT mat;
    qword q1, q2;
    struct E *pv;
    int c, i;
    struct E *aeBase;
    if (grf & 3) {
        bool f = plo->b60; if (f) {
            mat = plo->palo->mat;
        } else {
            struct ALO *palo;
            func_002333B4(&mat, 0, 0x40);
            mat.m[0][0] = mat.m[1][1] = mat.m[2][2] = mat.m[3][3] = 1.0f;
            for (palo = plo->palo; palo; palo = palo->pNext) {
                struct MAT t;
                __ml__FRC7MATRIX4T0(&t, palo, &mat);
                mat = t;
            }
        }
    }
    if (grf & 4) q1 = VCALL(plo, 33, qword (*)(void *)));
    if (grf & 8) q2 = VCALL(plo, 34, qword (*)(void *)));
    if (plo->p10) {
        c = plo->p10->c;
        aeBase = plo->p10->ae;
    } else {
        c = 1;
        aeBase = 0;
    }
    if (pb) pv = (struct E *)pb; else if (pa) pv = (struct E *)pa->palo; else pv = 0;
    for (i = 0; i < plo->c; i++) {
        struct E *pe = &plo->ae[i];
        int d = pe->pe - aeBase;
        if (d >= 0 && d < c) pe->pe = pv;
    }
    func_001A5900(plo, pa, pb, grf);
    UpdateSoXfWorldMatrices(plo);
    if (grf & 1) VCALL(plo, 29, void (*)(void *, qword)), *(qword *)mat.m[3]);
    if (grf & 2) VCALL(plo, 30, void (*)(void *, struct MAT *)), &mat);
    if (grf & 4) VCALL(plo, 35, void (*)(void *, qword)), q1);
    if (grf & 8) VCALL(plo, 36, void (*)(void *, qword)), q2);
    ReleaseSoBoundsHierarchy(plo);
    EnsureSoBoundsHierarchy(plo);
    if (plo->p10 && !(plo->p10->fl & (1L << 35))) func_001F8668(plo, 0);
}
} // namespace M_SetSoParent

// 0x1F2468 ReleaseSoBoundsHierarchy
namespace M_ReleaseSoBoundsHierarchy { extern "C" {
struct R; struct L;
struct R { int pad[2]; int c; };
struct L { char pad[0x24]; struct L *next; char pad2[0x64 - 0x28]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; char pad4[0x134 - 0x74]; struct R *pr; };
struct Pool;
void func_0011CD98(struct Pool *, struct R *);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct L *, int);
/** @sly2-annot addr=0x1F2468
 * Purpose: Drop the shared bounds record (+0x134, refcounted, pool (*(struct Pool *)&g_sw.slotheapSobounds)) of this SO and all SO children
 * Info: proposed name ReleaseSoBoundsHierarchy (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void ReleaseSoBoundsHierarchy(struct L *p)
{
    struct L *q;
    struct R *pr = p->pr;
    if (pr) {
        if (--pr->c == 0) func_0011CD98(&(*(struct Pool *)&g_sw.slotheapSobounds), pr);
        p->pr = 0;
    }
    for (q = p->first; q != p->end; q = q->next) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) ReleaseSoBoundsHierarchy(q);
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
struct Stats *PvAllocSlotheapClear(void *);
qword func_001A62E8(struct Node *);
static inline float Distance(qword a,qword b) {
 qword d,one;
 float result;
 __asm__("vsub.xyzw %0,%1,%2":"=j"(d):"j"(a),"j"(b));
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"0"(d));
 __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(d));
 __asm__("mtc1 %1,%0":"=f"(result):"r"(d));
 return result;
}
static inline ulong LoadState(struct Node *p)
{
    register ulong value __asm__("$2");
    __asm__("ld %0,%1":"=r"(value):"m"(p->state));
    return value;
}
/** @sly2-annot addr=0x1F2500
 * Purpose: Attach SO to the bounds record (+0x134) of its root (alloc from pool if none), then grow record
 *   low/high radius by radius+distance from root origin per flag bits 32/33 of +0xD0
 * Info: proposed name EnsureSoBounds (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void EnsureSoBounds(struct Node *p) {
 struct Node *top;
 struct Stats *attached;
 if (!(p->flags & (3UL<<32))) return;
 top=p;
 attached=p->stats;
 if(!attached) {
  do {
   register struct Node *next __asm__("$4")=top->parent;
   register int one __asm__("$3")=1;
   if(!next || ((int)((unsigned int)(int)LoadState(top)<<8)>>28)==one) {
     register char*pool __asm__("$4");
     struct Stats *s;
     __asm__("lui %0,0x3e":"=r"(pool));
     s=PvAllocSlotheapClear(pool+0x12D4);
     top->stats=s;
     p->stats=s;
     s->origin=&top->reference->pos;
     ++p->stats->count;
     p->stats->scale=p->scale;
     p->stats->owner=p;
     if(top==p)goto position;
     attached=p->stats;
     goto count;
   }
   top=next;
   attached=top->stats;
  } while(!attached);
  p->stats=attached;
 }
count:
 ++attached->count;
position:

 {
   qword position=func_001A62E8(p);
   float radius=p->radius+Distance(position,*p->stats->origin);
   if(p->flags&(1UL<<32)) {
     if(p->stats->low<radius) p->stats->low=radius;
   }
   if(p->flags&(1UL<<33)) {
     if(p->stats->high<radius) p->stats->high=radius;
   }
 }
}

#define OFF(T,F) ((unsigned long)&((T*)0)->F)
typedef char ancestor_layout[(sizeof(qword)==16 && __alignof__(qword)==16 && sizeof(void*)==4 && sizeof(float)==4 && sizeof(int)==4 && sizeof(unsigned)==4 && sizeof(ulong)==8 && sizeof(struct Origin)==0x80 && __alignof__(struct Origin)==16 && sizeof(struct Stats)==0x24 && __alignof__(struct Stats)==4 && sizeof(struct Node)==0x150 && __alignof__(struct Node)==8 && OFF(struct Origin,pos)==0x70 && OFF(struct Stats,owner)==0x0 && OFF(struct Stats,origin)==0x4 && OFF(struct Stats,count)==0x8 && OFF(struct Stats,low)==0x10 && OFF(struct Stats,high)==0x1c && OFF(struct Stats,scale)==0x20 && OFF(struct Node,parent)==0x10 && OFF(struct Node,reference)==0x60 && OFF(struct Node,radius)==0xa0 && OFF(struct Node,flags)==0xd0 && OFF(struct Node,stats)==0x134 && OFF(struct Node,scale)==0x13c && OFF(struct Node,state)==0x148)?1:-1];
} } // namespace M_EnsureSoBounds

// 0x1F2688 func_001F2688
/** @sly2-annot addr=0x1F2688
 * Purpose: Store float at +0x13C and mirror it into the shared bounds record if this SO owns it
 * Info: proposed name SetSoBoundsScale (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
/* Sets gUnk13c, and the bounds record's copy when the record is this object's. */
extern "C" void func_001F2688(SO *pso, float g)
{
    SOBOUNDS *psobounds;
    psobounds = pso->psobounds;
    pso->gUnk13c = g;
    if ((psobounds != 0) && (psobounds->pso == pso)) {
        psobounds->g20 = g;
    }
}

// 0x1F26A8 EnsureSoBoundsHierarchy
namespace M_EnsureSoBoundsHierarchy { extern "C" {
struct L;
struct L { char pad[0x24]; struct L *next; char pad2[0x64 - 0x28]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; };
void EnsureSoBounds(struct L *);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct L *, int);
/** @sly2-annot addr=0x1F26A8
 * Purpose: EnsureSoBounds on this SO and recursively on SO children
 * Info: proposed name EnsureSoBoundsHierarchy (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void EnsureSoBoundsHierarchy(struct L *p)
{
    struct L *q;
    EnsureSoBounds(p);
    for (q = p->first; q != p->end; q = q->next) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) EnsureSoBoundsHierarchy(q);
    }
}
} } // namespace M_EnsureSoBoundsHierarchy

// 0x1F2718 func_001F2718
namespace M_func_001F2718 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong a:20; int b:4; ulong c:40; };
/** @sly2-annot addr=0x1F2718
 * Purpose: Set 4-bit field +0x148 bits 20-23
 * Info: proposed name SetSoField148_20 (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F2718(struct S *p, int n) { p->b = n; }
} } // namespace M_func_001F2718

// 0x1F2740 func_001F2740
namespace M_func_001F2740 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong a:20; int b:4; ulong c:40; };
/** @sly2-annot addr=0x1F2740
 * Purpose: Get 4-bit field +0x148 bits 20-23
 * Info: proposed name GetSoField148_20 (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F2740(struct S *p) { return p->b; }
} } // namespace M_func_001F2740

// 0x1F2758 GetSoIgnoreLocked
namespace M_GetSoIgnoreLocked { extern "C" {
struct S;
struct S { char pad[0xD0]; ulong fl; };
/** @sly2-annot addr=0x1F2758
 * Purpose: Return bit 34 of +0xD0 (fIgnoreLocked)
 * Info: proposed name GetSoIgnoreLocked (T1); TU so
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int GetSoIgnoreLocked(struct S *p) { return (int)(p->fl >> 34) & 1; }
} } // namespace M_GetSoIgnoreLocked

// 0x1F2768 SetSoIgnoreLocked__FP2SOi
// SO+0xD0 bit 34 = GLOBSET+0x50 bit 34 (Lilly's port: globset.fUnk34).
/** @sly2-annot addr=0x1F2768
 * Purpose: Set +0xD0 bit34 (fIgnoreLocked)
 * Info: proposed name SetSoIgnoreLocked__FP2SOi (EXISTING); TU so
 * Confidence: high [EXISTING 0.85] - evidence: pre-existing symbol in symbol_addrs.txt; sources:
 *   r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions, docs/engine/physics-collision.md#open-questions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void SetSoIgnoreLocked(SO *pso, int f) { pso->globset.fUnk34 = f; }

// 0x1F2798 PsoFindPhysRoot
namespace M_PsoFindPhysRoot { extern "C" {
struct Q; struct A;
typedef unsigned long ulong;
struct Q { char pad[0xA0]; int a0; char pad2[0x1C]; int c0; };
struct A { char pad[0x10]; struct A *next; char pad2[0xD4]; struct Q *pq; char pad3[0x5C]; ulong fl; };
/** @sly2-annot addr=0x1F2798
 * Purpose: Walk up the parent chain to the first SO that has non-fixed physics (SOPHYS modes != 3) or flag
 *   +0x148 bit34; returns last visited
 * Info: proposed name PsoFindPhysRoot (T1); TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct A *PsoFindPhysRoot(struct A *p)
{
    struct A *r = 0;
    while (p) {
        r = p;
        if (p->pq && (p->pq->c0 != 3 || p->pq->a0 != 3))
            break;
        if (p->fl & (1L << 34))
            break;
        p = p->next;
    }
    return r;
}
} } // namespace M_PsoFindPhysRoot

// 0x1F27F0 func_001F27F0
/** @sly2-annot addr=0x1F27F0
 * Purpose: Ensure the SO has a contact-tree record (+0xEC): take the phys root's, creating it via vt53 and
 *   registering with g_sw (0x134320)
 * Info: proposed name EnsureSoTree (T2); TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
/* Makes sure the object has a tree record: its physics root's (created by SO slot 53 when missing). */
extern "C" void func_001F27F0(SO *pso)
{
    SO *psoRoot;
    if (pso->ptree == 0) {
        psoRoot = PsoFindPhysRoot(pso);
        if (psoRoot->ptree == 0) {
            psoRoot->ptree = ((SOTREE *(*)(void *))PVTE(psoRoot)[53].pfn)((char *)psoRoot + PVTE(psoRoot)[53].d);
            RecalcSwOxfFilterForObject(&D_003DD490, psoRoot);
        }
        pso->ptree = psoRoot->ptree;
    }
}

// 0x1F2870 AddSoPartToTree
namespace M_AddSoPartToTree { extern "C" {
struct M; struct O; struct E; struct VTE; struct VT; struct Q; struct P;
typedef int qword __attribute__((mode(TI)));
struct M { qword a[4]; };
struct O { char pad[0x40]; struct M m; char pad2[0xA4 - 0x80]; int nA4; };
struct E { struct O *po; int pad[7]; struct M m; char pad3[0xA8 - 0x60]; int nA8; int nAC; };
struct VTE { short d; short p; int (*pfn)(void *); };
struct VT { struct VTE a[63]; };
struct Q { char pad[0x34]; char dl[1]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0xEC - 0x10]; struct Q *pq; };
void func_001F27F0(struct P *);
int FFindDlEntry__FP2DLPv(void *, void *);
void AppendDlEntry__FP2DLPv(void *, void *);
void func_001F4BF8(struct P *);
void UpdateSoPartGeom(struct E *);
void func_001F5078(struct P *);
/** @sly2-annot addr=0x1F2870
 * Purpose: Add one SOPART to the tree record's part DL (+0x34): copy XF matrix/stamp, mark tree dirty
 *   (0x1F4BF8), UpdateSoPartGeom, invalidate if not fixed
 * Info: proposed name AddSoPartToTree (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void AddSoPartToTree(struct P *p, struct E *pe)
{
    struct O *po;
    func_001F27F0(p);
    if (FFindDlEntry__FP2DLPv(p->pq->dl, pe)) return;
    AppendDlEntry__FP2DLPv(p->pq->dl, pe);
    po = pe->po;
    pe->nA8 = -1;
    pe->m = po->m;
    pe->nAC = po->nA4;
    func_001F4BF8(p);
    UpdateSoPartGeom(pe);
    if (!p->pvt->a[62].pfn((char *)p + p->pvt->a[62].d)) func_001F5078(p);
}
} } // namespace M_AddSoPartToTree

// 0x1F2930 RemoveSoPartFromTree
namespace M_RemoveSoPartFromTree { extern "C" {
struct VTE; struct E; struct Q; struct P; struct Q2;
typedef unsigned long ulong;
struct VTE { short d; short p; int (*pfn)(void *); };
struct E { char pad[0x10]; ulong fl; char pad2[0xB0 - 0x18]; };
struct P;
struct Q { char pad[0x34]; char dl[0x2C]; struct P *p60; };
struct P { char pad[0xC]; struct VTE *pvt; char pad2[0xE0 - 0x10]; int nE0; struct E *aE4; int nE8; struct Q *pq; };
struct Q2 { char pad[0xEC]; struct Q *pq; };
struct G;
extern struct G D_003DD490;
int FFindDlEntry__FP2DLPv(void *, void *);
void RemoveDlEntry__FP2DLPv(void *, void *);
int FIsDlEmpty__FP2DL(void *);
void RemoveSwPlacesOf(struct G *, int, struct E *);
void func_001F4E28(struct P *);
void RemoveSwAaobrObject(struct G *, struct Q *);
void func_001F5078(struct P *);
/** @sly2-annot addr=0x1F2930
 * Purpose: Remove a SOPART from the tree DL; free its XP list; drop tree when empty (0x1337D0) or detach if
 *   no own parts remain
 * Info: proposed name RemoveSoPartFromTree (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void RemoveSoPartFromTree(struct P *p, struct E *pe)
{
    int i;
    if (!p->pq) return;
    if (!FFindDlEntry__FP2DLPv(p->pq->dl, pe)) return;
    RemoveDlEntry__FP2DLPv(p->pq->dl, pe);
    if (pe->fl & (1L << 51)) RemoveSwPlacesOf(&D_003DD490, 0, pe);
    func_001F4E28(p);
    if (FIsDlEmpty__FP2DL(p->pq->dl)) {
        struct Q *pq = p->pq;
        ((struct Q2 *)pq->p60)->pq = 0;
        p->pq = 0;
        RemoveSwAaobrObject(&D_003DD490, pq);
    } else if (p->pq->p60 != p) {
        i = p->nE0;
        while (--i >= 0) {
            if (FFindDlEntry__FP2DLPv(p->pq->dl, (struct E *)(i * 0xB0 + (int)p->aE4))) break;
        }
        if (i < 0) p->pq = 0;
    }
done:
    if (!p->pvt[62].pfn((char *)p + p->pvt[62].d)) func_001F5078(p);
}
} } // namespace M_RemoveSoPartFromTree

// 0x1F2A60 ShowSoPart
namespace M_ShowSoPart {
struct A; struct B;
typedef unsigned long ulong;
struct A { char pad[0x28]; ulong lo : 60; ulong b60 : 1; ulong hi : 3; };
struct B { char pad[0x10]; ulong lo : 50; ulong b50 : 1; ulong hi : 13; };
extern "C" void AddSoPartToTree(A *, B *);
/** @sly2-annot addr=0x1F2A60
 * Purpose: If part hidden (fl bit50): clear it and, when SO in world, AddSoPartToTree
 * Info: proposed name ShowSoPart (T1); TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void ShowSoPart(A *pa, B *pb)
{
    if (pb->b50) { pb->b50 = 0;
    bool f = pa->b60; if (f) AddSoPartToTree(pa, pb); }
}
} // namespace M_ShowSoPart

// 0x1F2AD0 HideSoPart
namespace M_HideSoPart { extern "C" {
struct T; struct S;
struct T { char pad[0x10]; ulong fl; };
struct S { char pad[0x28]; ulong fl; };
void RemoveSoPartFromTree(struct S *, struct T *);
/** @sly2-annot addr=0x1F2AD0
 * Purpose: Set part hidden bit50 and, when in world, RemoveSoPartFromTree
 * Info: proposed name HideSoPart (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void HideSoPart(struct S *p, struct T *pt)
{
    if (!(pt->fl & (1UL << 50))) {
        pt->fl |= 1UL << 50;
        if ((int)(p->fl >> 60) & 1) RemoveSoPartFromTree(p, pt);
    }
}
} } // namespace M_HideSoPart

// 0x1F2B20 AddSoParts
namespace M_AddSoParts { extern "C" {
struct E; struct P;
typedef unsigned long ulong;
struct E { int pad[4]; ulong fl; char pad2[0xB0 - 0x18]; };
struct P { char pad[0xE0]; int c; struct E *ae; };
void AddSoPartToTree(struct P *, struct E *);
/** @sly2-annot addr=0x1F2B20
 * Purpose: AddSoPartToTree for every visible SOPART (+0xE0 count / +0xE4 array, 0xB0 stride)
 * Info: proposed name AddSoParts (T1); TU so
 * Notes: names_v2 C proposed GetSoCpdefi__FP2SOfP6CPDEFI (0.310)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void AddSoParts(struct P *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (!(pe->fl & (1L << 50))) AddSoPartToTree(p, pe);
    }
}
} } // namespace M_AddSoParts

// 0x1F2BA8 RemoveSoParts
namespace M_RemoveSoParts { extern "C" {
struct E; struct P;
typedef unsigned long ulong;
struct E { int pad[4]; ulong fl; char pad2[0xB0 - 0x18]; };
struct P { char pad[0xE0]; int c; struct E *ae; };
void RemoveSoPartFromTree(struct P *, struct E *);
/** @sly2-annot addr=0x1F2BA8
 * Purpose: RemoveSoPartFromTree for every visible SOPART
 * Info: proposed name RemoveSoParts (T1); TU so
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void RemoveSoParts(struct P *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (!(pe->fl & (1L << 50))) RemoveSoPartFromTree(p, pe);
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
struct E { char pad[0xDC]; void *pDC; };
struct L { char pad[0xC]; struct VT *pvt; char pad1[0x24 - 0x10]; struct L *next; char pad1b[0x40 - 0x28]; int v40; char pad2[0x64 - 0x44]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; char pad4[0xE8 - 0x74]; struct E *pe; };
struct H { char pad[0x2C]; int n2C; };
struct G { char pad[0x45CC]; struct H *ph; };
extern struct G D_003DD490;
extern const struct S8 D_002A8270;
void AddLoHierarchy(struct L *);
void UpdateSoXfWorldMatrices(struct L *);
void AddSoParts(struct L *);
qword func_001A62E8(struct L *);
void *PpathzoneFromPos(qword, int);
void func_001F37C8(struct L *, void *);
void PostSwCallback(struct G *, struct L *, long, int);
/** @sly2-annot addr=0x1F2C30
 * Purpose: SO vt11: AddLoHierarchy, 0x22FAA0, AddSoParts, set grf 0x14, register region (if SOPHYS region
 *   record), recurse vt11 on children
 * Info: proposed name AddSoHierarchy (T1); class SO slot11; TU so
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/particles-effects.md#trail-0x21bb88-0x21c820,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void AddSoHierarchy(struct L *p)
{
    struct L *q;
    void *pv;
    int unused[4];
    int v;
    AddLoHierarchy(p);
    UpdateSoXfWorldMatrices(p);
    AddSoParts(p);
    v = p->v40;
    if ((v ^ 1) & 1) *(int *)v |= 0x14;
    else p->v40 = v | 0x14;
    pv = p->pe ? p->pe->pDC : 0;
    if (pv) {
        if (D_003DD490.ph && D_003DD490.ph->n2C)
            func_001F37C8(p, PpathzoneFromPos(func_001A62E8(p), 1));
        else
            PostSwCallback(&D_003DD490, p, D_002A8270.l, 0);
    }
    for (q = p->first; q != p->end; q = q->next)
        q->pvt->a[11].pfn((char *)q + q->pvt->a[11].d);
}
} } // namespace M_AddSoHierarchy

// 0x1F2D48 RemoveSoHierarchy
namespace M_RemoveSoHierarchy { extern "C" {
struct DL; struct DLI; struct VTE; struct O; struct E; struct P; struct K;
typedef unsigned long ulong;
typedef struct DL { void *head; void *tail; int ibDle; } DL;
typedef struct DLI { DL *m_pdl; void **m_ppv; int m_ibDle; struct DLI *m_pdliNext; } DLI;
extern DLI *D_002DD2FC;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *pvt; char pad2[0x28 - 0x10]; ulong fl; };
struct E { char pad[0xDC]; int nDC; };
struct P { char pad[0x64]; DL dl; char pad2[0xE8 - 0x70]; struct E *pe; char pad3[0x12C - 0xEC]; float f12C; int n130; };
struct K { long l; } __attribute__((packed));
struct G;
extern struct G D_003DD490;
extern const struct K D_002A8270;
void RemoveLoHierarchy(struct P *);
void RemoveSoParts(struct P *);
void ClearSwCallbacks(struct G *, int, struct P *, long, int);
void func_001F37C8(struct P *, int);
static inline void InitDli(DLI *pdli, DL *pdl)
{
    pdli->m_pdl = pdl;
    pdli->m_ppv = (void **)pdl;
    pdli->m_ibDle = pdl->ibDle;
    pdli->m_pdliNext = D_002DD2FC;
    D_002DD2FC = pdli;
}
static inline void *PvNextDli(DLI *pdli)
{
    void *pv = *pdli->m_ppv;
    pdli->m_ppv = (void **)((char *)pv + pdli->m_ibDle);
    return pv;
}
/** @sly2-annot addr=0x1F2D48
 * Purpose: SO vt12: RemoveLoHierarchy, RemoveSoParts, reset fade (+0x130=0, +0x12C=1), unregister region
 *   callback, recurse vt12 on children
 * Info: proposed name RemoveSoHierarchy (T1); class SO slot12; TU so
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8,
 *   docs/reference/functions/gameplay-objects.md#missile-0x1b1db8-0x1b2820
 * @end-sly2-annot */
void RemoveSoHierarchy(struct P *p)
{
    DLI dli;
    struct O *po;
    int n;
    RemoveLoHierarchy(p);
    RemoveSoParts(p);
    p->n130 = 0;
    p->f12C = 1.0f;
    n = 0;
    if (p->pe) n = p->pe->nDC;
    if (n) {
        ClearSwCallbacks(&D_003DD490, 3, p, D_002A8270.l, 0);
        func_001F37C8(p, 0);
    }
    InitDli(&dli, &p->dl);
    while ((po = (__typeof__(po))PvNextDli(&dli)) != 0 && !(po->fl & (1L << 56)))
        po->pvt[12].pfn((char *)po + po->pvt[12].d);
    D_002DD2FC = dli.m_pdliNext;
}
} } // namespace M_RemoveSoHierarchy

// 0x1F2E78 UpdateSoLoroot
namespace M_UpdateSoLoroot { extern "C" {
struct VTE; struct O;
typedef unsigned long ulong;
struct VTE { short d; short pad; void (*pfn)(void *, void *, void *); };
struct O { char pad[0xC]; struct VTE *pvt; struct O *p10; char pad2[0x24 - 0x14]; struct O *pNext; ulong fl; char pad3[0x64 - 0x30]; struct O *pFirst; char pad4[0xE0 - 0x68]; int nE0; };
void func_001A5B80(struct O *, struct O *, struct O *);
void RemoveSoParts(struct O *);
void AddSoParts(struct O *);
static inline struct O *Find(struct O *x)
{
    while (x && !(x->fl & (1L << 56))) x = x->p10;
    return x;
}
/** @sly2-annot addr=0x1F2E78
 * Purpose: SO vt13: UpdateLoroot, recurse to children; if the SO has parts and its root changed, re-add parts
 *   to the new tree
 * Info: proposed name UpdateSoLoroot (T1); class SO slot13; TU so
 * Notes: names_v2 C proposed ApplySoConstraintWorld__FP2SOP6CONSTRP6VECTORN22 (0.438)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void UpdateSoLoroot(struct O *p, struct O *pNew, struct O *pOld)
{
    struct O *po;
    int unused[4];
    func_001A5B80(p, pNew, pOld);
    for (po = p->pFirst; po; po = po->pNext)
        po->pvt[13].pfn((char *)po + po->pvt[13].d, pNew, pOld);
    if (!p->nE0) return;
    for (po = p; ; po = po->p10) {
        if (po->fl & (1L << 56)) return;
        if (po == pNew) break;
    }
    {
    struct O *a = Find(pOld);
    struct O *b = Find(p->p10);
    if (!a && !b) {
        RemoveSoParts(p);
        AddSoParts(p);
    }
    }
}
} } // namespace M_UpdateSoLoroot

// 0x1F2FE0 SetSoGrf
namespace M_SetSoGrf { extern "C" {
struct VTE; struct VT; struct L;
struct VTE { short d; short p; void (*pfn)(void *, int); };
struct VT { struct VTE a[48]; };
struct L { char pad[0xC]; struct VT *pvt; char pad1[0x24 - 0x10]; struct L *next; char pad1b[0x40 - 0x28]; int v40; char pad2[0x64 - 0x44]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; };
/** @sly2-annot addr=0x1F2FE0
 * Purpose: SO vt47: grf(+0x40 tagged word) |= f, then vt47 on each child
 * Info: proposed name SetSoGrf (T1); class SO slot47; TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void SetSoGrf(struct L *p, int f)
{
    struct L *q;
    int unused[4];
    int v = p->v40;
    if ((v ^ 1) & 1) *(int *)v |= f;
    else p->v40 = v | f;
    for (q = p->first; q != p->end; q = q->next)
        q->pvt->a[47].pfn((char *)q + q->pvt->a[47].d, f);
}
} } // namespace M_SetSoGrf

// 0x1F3078 ClearSoGrf
namespace M_ClearSoGrf { extern "C" {
struct VTE; struct VT; struct L;
struct VTE { short d; short p; void (*pfn)(void *, int); };
struct VT { struct VTE a[49]; };
struct L { char pad[0xC]; struct VT *pvt; char pad1[0x24 - 0x10]; struct L *next; char pad1b[0x40 - 0x28]; int v40; char pad2[0x64 - 0x44]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; };
/** @sly2-annot addr=0x1F3078
 * Purpose: SO vt48 (new): grf &= ~f, then vt48 on each child
 * Info: proposed name ClearSoGrf (T1); class SO slot48; TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void ClearSoGrf(struct L *p, int f)
{
    struct L *q;
    int unused[4];
    int v = p->v40;
    if ((v ^ 1) & 1) *(int *)v &= ~f;
    else p->v40 = v & ~f;
    for (q = p->first; q != p->end; q = q->next)
        q->pvt->a[48].pfn((char *)q + q->pvt->a[48].d, f);
}
} } // namespace M_ClearSoGrf

// 0x1F3118 func_001F3118
namespace M_func_001F3118 {
struct VTE; struct VT; struct L;
typedef unsigned long ulong;
struct VTE { short d; short p; void (*pfn)(void *, float); };
struct VT { VTE a[43]; };
struct L { char pad[0xC]; VT *pvt; struct L *parent; int n14; char pad1[0x24 - 0x18]; struct L *next; ulong lo : 60; ulong b60 : 1; ulong hi : 3; float g30, g34, g38; char pad2[0x64 - 0x3C]; struct L *first; char pad3[0x140 - 0x68]; int n140; };
struct G;
extern "C" { extern G D_003DD490; }
extern "C" void func_001D6310(L *);
extern "C" void func_001D6378(L *);
extern "C" void func_00147E88(G *, int);
/** @sly2-annot addr=0x1F3118
 * Purpose: SO vt42: store busy distance +0x38, refresh +0x140 registration (0x1D6310/0x1D6378), re-sort root
 *   in g_sw (0x147E88), push smaller value to children
 * Info: proposed name SetSoSBusy (T2); class SO slot42; TU so
 * Notes: names_v2 C proposed ApplyConstr__FP6CONSTRP6VECTORN21 (0.438)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F3118(L *p, float t)
{
    L *q;
    int unused[4];
    p->g38 = t;
    if (p->n140 != -1) {
        func_001D6310(p);
        func_001D6378(p);
    }
    if (!p->parent) {
        bool f = p->b60;
        if (f) func_00147E88(&D_003DD490, p->n14);
    }
    for (q = p->first; q; q = q->next) {
        if (t < q->g38) q->pvt->a[42].pfn((char *)q + q->pvt->a[42].d, t);
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
void *func_001AE2A0(MSG *);
void CloneGeom__FP4GEOMP7MATRIX4T0(GEO *,const M4 *,GEO *);
void AddSoParts(P *);
void AdjustSoMomint(P *,float);
void LoadOptionsFromBrx__FPvP18CBinaryInputStream(P *,void *);
void EnsureSoBounds(P *);
void LoadSwObjectsFromBrx__FP2SOP18CBinaryInputStreamP14CLoadCollector(P *,void *,void *);
void func_001A6D28(P *,int);
}
static inline int Extra(P *p){VE *v=&p->vt[63];return ((int (*)(P *))v->fn)((P *)((char *)p+v->delta));}
static inline void *Lookup(P *p,int id){VE *v=&p->vt[51];return ((void *(*)(P *,int))v->fn)((P *)((char *)p+v->delta),id);}
static inline void Post(P *p){VE *v=&p->vt[64];((void (*)(P *))v->fn)((P *)((char *)p+v->delta));}
static inline void SetActive(P *p){uint f=p->flags;if((f^1)&1)*(uint *)f|=8;else p->flags=f|8;}
/** @sly2-annot addr=0x1F31E0
 * Purpose: SO vt14: LoadLo base, read model/globset, SOPART array (XF id, flags, radius, local geom, BSP...),
 *   physics block (matrix, mass), options, bounds
 * Info: proposed name LoadSoFromBrx (T1); class SO slot14; TU so
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6+parse-verified
 * See: docs/engine/gameplay-objects.md#key-functions, docs/engine/physics-collision.md#7-key-functions,
 *   docs/engine/player.md#11-key-functions
 * @end-sly2-annot */
extern "C" void LoadSoFromBrx(P *p,void *stream)
{
    int i=0;E *e;
    LoadSoXfsFromBrx(p,stream);
    int metadata=U32Read__18CBinaryInputStream(stream);
    LoadGlobsetFromBrx__FP7GLOBSETP2SOP18CBinaryInputStream(&p->data,p,stream);
    int extra=Extra(p);
    p->count=S16Read__18CBinaryInputStream(stream);
    MSG request;
    request.bytes=(p->count+extra)*sizeof(E);request.alignment=16;
    p->entries=(E *)func_001AE2A0(&request);
    for(;i<p->count;i++){
        e=&p->entries[i];
        GEO *local=&e->local;
        e->owner=p;
        e->object=Lookup(p,S16Read__18CBinaryInputStream(stream));
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
    for(i=p->count;i<p->count+extra;i++){e=&p->entries[i];e->flags.fields.state=-10;}
    AddSoParts(p);
    if(p->count>0){
        request.bytes=sizeof(STATE);request.alignment=16;
        STATE *state=(STATE *)func_001AE2A0(&request);
        p->state=state;
        state->mass=F32Read__18CBinaryInputStream(stream);
        ReadMatrix__18CBinaryInputStreamP7MATRIX3(stream,state);
        AdjustSoMomint(p,1.0f);
        Post(p);
    }
    LoadOptionsFromBrx__FPvP18CBinaryInputStream(p,stream);
    if(p->data.guard)SetActive(p);
    EnsureSoBounds(p);
    LoadSwObjectsFromBrx__FP2SOP18CBinaryInputStreamP14CLoadCollector(p,stream,0);
    func_001A6D28(p,metadata);
}
} // namespace M_LoadSoFromBrx

// 0x1F3590 func_001F3590
/** @sly2-annot addr=0x1F3590
 * Purpose: SO vt64: SOPHYS gravity vector (+0x80) = (0,0,-1750), both XF modes (+0xA0/+0xC0) = 3 (fixed)
 * Info: proposed name ResetSoPhys (T2); class SO slot64; TU so
 * Notes: names_v2 C proposed ConstrFromCnstr__F5CNSTRP2CTP6VECTOR (0.438)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
/* Default physics: gravity -1750 down z, both constraints of kind 3. */
extern "C" void func_001F3590(SO *pso)
{
    SOPHYS *psophys;
    VECTOR *pdv;
    psophys = pso->psophys;
    pdv = &psophys->dvGravity;
    psophys->dvGravity.x = 0.0f;
    pdv->z = -1750.0f;
    pdv->y = 0.0f;
    pso->psophys->constrForce.nKind = 3;
    pso->psophys->constrTorque.nKind = 3;
}

// 0x1F35C8 SetSoMass
/** @sly2-annot addr=0x1F35C8
 * Purpose: Scale SOPHYS mass (+0xD0) to new value and scale inertia via 0x1F35F8
 * Info: proposed name SetSoMass (T1); TU so
 * Notes: names_v2 C proposed SetSoCameraStyle__FP2SO3CMK (0.167)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void SetSoMass(SO *pso, float m)
{
    SOPHYS *psophys;
    float mOld;
    float r;
    psophys = pso->psophys;
    mOld = psophys->m;
    r = m / mOld;
    psophys->m = mOld * r;
    AdjustSoMomint(pso, r);
}

// 0x1F35F8 AdjustSoMomint
namespace M_AdjustSoMomint { extern "C" {
struct T; struct X;
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
typedef struct { qword r[3]; } M3;
struct T { M3 m; qword v[3]; };
struct X { char pad[0xE8]; struct T *pt; };
void func_0023320C(void *d, const void *s, unsigned int cb) __asm__("memcpy");
void *func_002333B4(void *d, int c, unsigned int cb) __asm__("memset");
int FInvertMatrix3(struct T *pt, qword *pv);
/** @sly2-annot addr=0x1F35F8
 * Purpose: Scale SOPHYS inertia matrix by r and recompute its inverse (zero if singular)
 * Info: proposed name AdjustSoMomint (T1); TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void AdjustSoMomint(struct X *p, float s)
{
    struct T *pt = p->pt;
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
    if (!FInvertMatrix3(p->pt, p->pt->v))
        func_002333B4(p->pt->v, 0, 0x30);
}
} } // namespace M_AdjustSoMomint

// 0x1F36A8 PostSoLoad
/** @sly2-annot addr=0x1F36A8
 * Purpose: SO vt15: PostLoLoad, 0x22FAA0, globset post-load (0x167B68 on +0x80)
 * Info: proposed name PostSoLoad (T1); class SO slot15; TU so
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/physics-collision.md#water-0x228b48-0x229ef0
 * @end-sly2-annot */
extern "C" void PostSoLoad(SO *pso)
{
    PostLoLoadV();
    UpdateSoXfWorldMatrices(pso);
    BindGlobset(&pso->globset);
}

// 0x1F36E0 UpdateSo
namespace M_UpdateSo { extern "C" {
struct VTE; struct VT; struct E; struct P; 
typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[10]; };
struct E { char pad[0xDC]; void *pDC; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0xE8 - 0x10]; struct E *pe; char pad3[0x12C - 0xEC]; float g12C; float g130; };
extern struct CLOCK D_002DDED0;
void UpdateLo(struct P *);
qword func_001A62E8(struct P *);
void *PpathzoneFromPos(qword, int);
void func_001F37C8(struct P *, void *);
/** @sly2-annot addr=0x1F36E0
 * Purpose: SO vt16: UpdateLo; refresh region from position; advance fade alpha +0x12C by rate +0x130*dt,
 *   removing (vt9) at 0
 * Info: proposed name UpdateSo (T1); class SO slot16; TU so
 * Notes: names_v2 C proposed PxpFindSoGround__FP2SOT0Pi (0.200)
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void UpdateSo(struct P *p)
{
    void *q;
    float d;
    UpdateLo(p);
    q = p->pe ? p->pe->pDC : 0;
    if (q) func_001F37C8(p, PpathzoneFromPos(func_001A62E8(p), 1));
    d = p->g130;
    if (d != 0.0f) {
        float v = p->g12C + D_002DDED0.dt * d;
        p->g12C = v;
        if (v <= 0.0f) {
            p->g12C = 0.0f;
            p->g130 = 0.0f;
            p->pvt->a[9].pfn((char *)p + p->pvt->a[9].d);
        } else if (1.0f <= v) {
            p->g12C = 1.0f;
            p->g130 = 0.0f;
        }
    }
}
} } // namespace M_UpdateSo

// 0x1F37C8 func_001F37C8
namespace M_func_001F37C8 { extern "C" {
struct Q; struct O; struct E; struct P;
struct Q { char pad[0x118]; char dl[1]; };
struct O { char pad[0x10]; struct Q *pq; char pad2[0x20 - 0x14]; char sub[1]; };
struct E { char pad[0xDC]; struct O *po; };
struct P { char pad[0xE8]; struct E *pe; };
int FFindDlEntry__FP2DLPv(void *, void *);
void RemoveDlEntry__FP2DLPv(void *, void *);
void AppendDlEntry__FP2DLPv(void *, void *);
void func_001C1440(struct Q *, void *);
/** @sly2-annot addr=0x1F37C8
 * Purpose: Move the SOPHYS region link (+0xDC) from its current region DL (+0x118) to region q
 * Info: proposed name SetSoPhysRegion (T2); TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F37C8(struct P *p, struct Q *q)
{
    struct O *o = p->pe->po;
    struct Q *old = o->pq;
    if (old == q) return;
    if (old) {
        if (FFindDlEntry__FP2DLPv(old->dl, o)) RemoveDlEntry__FP2DLPv(old->dl, o);
    }
    if (q) {
        if (!FFindDlEntry__FP2DLPv(q->dl, o)) {
            AppendDlEntry__FP2DLPv(q->dl, o);
            func_001C1440(q, o->sub);
        }
    }
    o->pq = q;
}
} } // namespace M_func_001F37C8

// 0x1F3868 func_001F3868
namespace M_func_001F3868 { extern "C" {
typedef int qword __attribute__((mode(TI)));
qword func_001A62E8(void *);
int PpathzoneFromPos(qword, int);
void func_001F37C8(void *, int);
/** @sly2-annot addr=0x1F3868
 * Purpose: Deferred callback: SetSoPhysRegion(so, region at PosLoWorld)
 * Info: proposed name UpdateSoPhysRegionCallback (T2); TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F3868(void *p) { func_001F37C8(p, PpathzoneFromPos(func_001A62E8(p), 1)); }
} } // namespace M_func_001F3868

// 0x1F38A8 UpdateSoPartGeom
/** @sly2-annot addr=0x1F38A8
 * Purpose: If part stamp != XF stamp: transform local geom to world (0x1664A0), copy stamp, mark tree dirty
 * Info: proposed name UpdateSoPartGeom (T1); TU so
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
/* Recomputes the part's world geometry when its transform changed since the last update. */
extern "C" void UpdateSoPartGeom(SOPART *ppart)
{
    XF *pxf;
    pxf = ppart->pxf;
    if (ppart->nStampGeom != pxf->nStampWorld) {
        UpdateGeomWorld(&pxf->matWorld, &ppart->geomLocal, &ppart->geomWorld);
        ppart->nStampGeom = ppart->pxf->nStampWorld;
        func_001F4BF8(ppart->pso);
    }
}

// 0x1F3900 UpdateSoPartMatrices
namespace M_UpdateSoPartMatrices { extern "C" {
struct M; struct O; struct E; struct P;
typedef unsigned long ulong;
typedef int qword __attribute__((mode(TI)));
struct M { qword a[4]; };
struct O { char pad[0x40]; struct M m; char pad2[0xA4 - 0x80]; int nA4; };
struct E { struct O *po; int pad[3]; ulong fl; int pad2[2]; struct M m; char pad3[0xAC - 0x60]; int nAC; };
struct P { char pad[0xE0]; int c; struct E *ae; };
/** @sly2-annot addr=0x1F3900
 * Purpose: Copy each visible part's XF world matrix (+0x40) and stamp (+0xA4) into the part
 * Info: proposed name UpdateSoPartMatrices (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void UpdateSoPartMatrices(struct P *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (!(pe->fl & (1L << 50))) {
            struct O *po = pe->po;
            pe->m = po->m;
            pe->nAC = po->nA4;
        }
    }
}
} } // namespace M_UpdateSoPartMatrices

// 0x1F3980 func_001F3980
namespace M_func_001F3980 { extern "C" {
struct VTE; struct E; struct Q; struct R; struct P; 
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct VTE { short d; short p; void (*pfn)(void *); };
struct E { char pad[0x10]; ulong fl; char pad2[0xB0 - 0x18]; };
struct Q { char pad[0x98]; short h98; char pad2[0xB0 - 0x9A]; qword vB0; char padC0[0xF0 - 0xC0]; int aF0[3]; };
struct R { char pad[0x60]; qword v60; qword v70; };
struct P { char pad[0xC]; struct VTE *pvt; char pad2[0x58 - 0x10]; struct Q *p58; struct Q *p5C; char pad3[0xE0 - 0x60]; int nE0; struct E *aE4; struct R *pE8; };
extern struct CLOCK D_002DDED0;
void UpdateSoPartMatrices(struct P *);
void func_001F6EF8(struct E *, float);
int func_00230380(int);
void UpdateSoSlots(struct P *);
void UpdateSoPartGeom(struct E *);
static inline void Clear(int *a)
{
    a[2] = 0;
    a[1] = 0;
    a[0] = 0;
}
/** @sly2-annot addr=0x1F3980
 * Purpose: SO vt17: refresh part matrices, step soft-mesh parts (0x1F6EF8), sync XF positions into SOPHYS,
 *   vt70 if XF kinds 1/2, UpdateSoSlots, UpdateSoPartGeom
 * Info: proposed name UpdateSoXfWorld (T2); class SO slot17; TU so
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/particles-effects.md#trail-0x21bb88-0x21c820,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_001F3980(struct P *p)
{
    int i;
    UpdateSoPartMatrices(p);
    for (i = 0; i < p->nE0; i++) {
        struct E *pe = &p->aE4[i];
        if (*(int *)&pe->fl) func_001F6EF8(pe, D_002DDED0.dt);
    }
    if (p->pE8) {
        if (p->p58) p->pE8->v60 = p->p58->vB0;
        if (p->p5C) p->pE8->v70 = p->p5C->vB0;
        if ((unsigned short)(p->p58->h98 - 1) < 2 || p->p5C->h98 == 1 || p->p5C->h98 == 2) {
            if (func_00230380(p->p58->h98)) {
                Clear(p->p58->aF0);
            }
            if (func_00230380(p->p5C->h98)) {
                Clear(p->p5C->aF0);
            }
            p->pvt[70].pfn((char *)p + p->pvt[70].d);
        }
    }
    UpdateSoSlots(p);
    for (i = 0; i < p->nE0; i++) {
        struct E *pe = &p->aE4[i];
        if (!(pe->fl & (1L << 50))) UpdateSoPartGeom(pe);
    }
}
} } // namespace M_func_001F3980

// 0x1F3B28 func_001F3B28
namespace M_func_001F3B28 { extern "C" {
struct P;
typedef unsigned long ulong;

struct P {
    char pad[0x40];
    int v40;
    char pad2[0x148 - 0x44];
    ulong fl;
};

int func_001A61A0(struct P *);
void func_001D6378(struct P *);
void func_001D6310(struct P *);

/** @sly2-annot addr=0x1F3B28
 * Purpose: SO vt21: set/clear grf bit 8; when enabling and +0x148 bit35 and in world -> register (0x1D6378);
 *   disabling -> unregister
 * Info: proposed name SetSoFlag8 (T2); class SO slot21; TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F3B28(struct P *p, int enabled)
{
    register int word __asm__("a2") = p->v40;
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
                    __asm__("sw %1,%0" : "=m"(p->v40) : "r"(value));
                }
            }
            if ((p->fl & (1L << 35)) && func_001A61A0(p))
                func_001D6378(p);
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
                    __asm__("sw %1,%0" : "=m"(p->v40) : "r"(value));
                }
            }
            func_001D6310(p);
        }
    }
}
} } // namespace M_func_001F3B28

// 0x1F3C10 func_001F3C10
namespace M_func_001F3C10 { extern "C" {
struct G; struct Q; struct A;
struct G { char pad[0x3D60]; int n; };
extern struct G D_003DD490;
struct Q { char pad[0xC]; int n; };
struct A { char pad[0x134]; struct Q *pq; };
void func_001A4478(struct G *pg, int n, struct Q *pq);
/** @sly2-annot addr=0x1F3C10
 * Purpose: If bounds record (+0x134) stamp != g_sw+0x3D60, recompute via 0x1A4478 and store stamp
 * Info: proposed name RefreshSoBoundsLighting (T2); TU so
 * Notes: names_v2 B proposed SetSoNoXpsSelf__FP2SOi (0.856)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_001F3C10(struct A *p)
{
    struct Q *pq = p->pq;
    if (pq && pq->n != D_003DD490.n) {
        func_001A4478(&D_003DD490, 0, pq);
        p->pq->n = D_003DD490.n;
    }
}
} } // namespace M_func_001F3C10

// 0x1F3C70 RenderSoSelf
/** @sly2-annot addr=0x1F3C70
 * Purpose: SO vt19: if visible in current vis mask (+0x2D & (*(int *)&g_cm.grfvis)) draw globset (+0x80) with alpha
 *   +0x12C
 * Info: proposed name RenderSoSelf (T1); class SO slot19; TU so
 * Notes: names_v2 C proposed SetSoNoXpsCenter__FP2SOi (0.438)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions, docs/engine/rendering.md#1-frame-structure,
 *   docs/reference/functions/particles-effects.md#trail-0x21bb88-0x21c820
 * @end-sly2-annot */
extern "C" void RenderSoSelf(SO *pso)
{
    float uAlpha;
    if (pso->grfvis & (*(int *)&g_cm.grfvis)) {
        func_001F3C10(pso);
        uAlpha = pso->uAlpha;
        RenderSoGlobset(&pso->globset, 0, uAlpha, uAlpha, pso->psobounds);
    }
}

// 0x1F3CC8 func_001F3CC8
namespace M_func_001F3CC8 { extern "C" {
struct B; struct VTE; struct O; struct P;
struct B { char b[0x30]; };
struct VTE { short d; short pad; void (*pfn)(void *, int, float, float, struct B *); };
struct O { char pad[0xC]; struct VTE *pvt; char pad2[0x24 - 0x10]; struct O *pNext; };
struct P { char pad[0x64]; struct O *pFirst; char pad2[0x70 - 0x68]; struct O *pEnd; char pad3[0x80 - 0x74]; char x80[0x134 - 0x80]; struct B *pb; };
struct G;
extern struct G D_003DD490;
void func_001A4478(struct G *, int, struct B *);
void RenderSoGlobset(void *, int, float, float, struct B *);
/** @sly2-annot addr=0x1F3CC8
 * Purpose: SO vt20: draw globset with explicit params and recurse vt20 to children
 * Info: proposed name RenderSoAll (T2); class SO slot20; TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F3CC8(struct P *p, int a, struct B *pb, float f1, float f2)
{
    struct B b;
    int unused[4];
    struct O *po;
    if (!pb && p->pb) {
        b = *p->pb;
        pb = &b;
        func_001A4478(&D_003DD490, a, &b);
    }
    RenderSoGlobset(p->x80, a, f1, f2, pb);
    for (po = p->pFirst; po != p->pEnd; po = po->pNext)
        po->pvt[20].pfn((char *)po + po->pvt[20].d, a, f1, f2, pb);
}
} } // namespace M_func_001F3CC8

// 0x1F3E00 OnSoRemove
namespace M_OnSoRemove { extern "C" {
struct VTE; struct VT; struct L; struct E;
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[50]; };
struct E;
struct L { char pad[0xC]; struct VT *pvt; char pad1[0x24 - 0x10]; struct L *next; char pad2[0x64 - 0x28]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; char pad4[0xEC - 0x74]; struct E *pe; };
struct E { char pad[0x60]; struct L *p60; };
struct DL;
void OnLoRemove(struct L *);
void func_001D6310(struct L *);
void RemoveDlEntry__FP2DLPv(struct DL *, void *);
#define VF(q, n) (q)->pvt->a[n].pfn((char *)(q) + (q)->pvt->a[n].d)
/** @sly2-annot addr=0x1F3E00
 * Purpose: SO vt22: LO::Freeze, unregister (0x1D6310), vt22 children, if tree owner: vt49 + remove from
 *   g_sw.dlSoTree
 * Info: proposed name OnSoRemove (T1); class SO slot22; TU so
 * Notes: names_v2 C proposed RebuildSoPhysHook__FP2SO (0.638) | slot 22 OnRemove resolved by arbitration
 *   (OnRemove/OnAdd = object leaves/enters the active world, incl. proximity activation) (was FreezeSo)
 * Confidence: high [T1 0.85] - evidence: arbitration; static naming pass; sources:
 *   arbitration_slots22_23+verdicts_tu_r4r6
 * See: docs/engine/guards-ai.md#9-key-functions, docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void OnSoRemove(struct L *p)
{
    struct L *q;
    int unused[4];
    OnLoRemove(p);
    func_001D6310(p);
    for (q = p->first; q != p->end; q = q->next) VF(q, 22);
    if (p->pe && p->pe->p60 == p) {
        VF(p, 49);
        RemoveDlEntry__FP2DLPv(&g_sw.dlSoTree, p->pe);
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
struct L { char pad[0xC]; struct VT *pvt; char pad1[0x24 - 0x10]; struct L *next; char pad1b[0x40 - 0x28]; int v40; char pad2[0x64 - 0x44]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; char pad4[0xEC - 0x74]; struct E *pe; char pad5[0x148 - 0xF0]; ulong fl; };
struct E { char pad[0x60]; struct L *p60; };
struct DL;
void OnLoAdd(struct L *);
void func_001D6378(struct L *);
int FFindDlEntry__FP2DLPv(struct DL *, void *);
void AppendDlEntry__FP2DLPv(struct DL *, void *);
#define VF(q, n) (q)->pvt->a[n].pfn((char *)(q) + (q)->pvt->a[n].d)
/** @sly2-annot addr=0x1F3EA8
 * Purpose: SO vt23: LO::Unfreeze, re-register, vt23 children, add tree to g_sw.dlSoTree and vt49
 * Info: proposed name OnSoAdd (T1); class SO slot23; TU so
 * Notes: names_v2 C proposed FGetSoContactList__FP2SOPv (0.167) | slot 23 OnAdd resolved by arbitration
 *   (OnRemove/OnAdd = object leaves/enters the active world, incl. proximity activation) (was UnfreezeSo)
 * Confidence: high [T1 0.85] - evidence: arbitration; static naming pass; sources: arbitration_slots22_23
 * See: docs/engine/guards-ai.md#9-key-functions, docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void OnSoAdd(struct L *p)
{
    struct L *q;
    int unused[4];
    OnLoAdd(p);
    if (p->fl & (1L << 35)) {
        int v = p->v40;
        if ((v ^ 1) & 1) v = *(int *)v;
        if (v & 8) func_001D6378(p);
    }
    for (q = p->first; q != p->end; q = q->next) VF(q, 23);
    if (p->pe && p->pe->p60 == p) {
        if (!FFindDlEntry__FP2DLPv((DL *)&g_sw.dlSoTree, p->pe)) AppendDlEntry__FP2DLPv(&g_sw.dlSoTree, p->pe);
        VF(p, 49);
    }
}
} } // namespace M_OnSoAdd

// 0x1F3FA0 SO_PmatWorld
/** @sly2-annot addr=0x1F3FA0
 * Purpose: SO vt31: &pxfRoot(+0x60)->matWorld (+0x40)
 * Info: proposed name SO_PmatWorld (T1); class SO slot31; TU so
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6_1D5150
 * See: docs/formats/meshes-scene.md#3-transforms-cstatic,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" MATRIX4 *SO_PmatWorld(SO *pso)
{
    return &pso->pxfRoot->matWorld;
}

// 0x1F3FB0 func_001F3FB0
namespace M_func_001F3FB0 { extern "C" {
struct A;
struct A { char pad[0x60]; int n; };
void func_002333B4(void *p, int c, int cb) __asm__("memset");
/** @sly2-annot addr=0x1F3FB0
 * Purpose: SO vt32: optional identity into M4 out, return +0x60
 * Info: proposed name SO_PxfLocal (T2); class SO slot32; TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F3FB0(struct A *p, float *pm)
{
    if (pm) {
        func_002333B4(pm, 0, 0x40);
        pm[0] = pm[5] = pm[10] = pm[15] = 1.0f;
    }
    return p->n;
}
} } // namespace M_func_001F3FB0

// 0x1F4010 func_001F4010
namespace M_func_001F4010 { extern "C" {
struct M4; struct T; struct VTE; struct VT; struct P; struct G;
typedef int qword __attribute__((mode(TI)));
struct M4 { qword a[4]; };
struct T { struct M4 m; char pad[0xA4 - 0x40]; int nA4; };
struct VTE { short d; short p; int (*pfn)(void *); };
struct VT { struct VTE a[19]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x54 - 0x10]; struct T *pt; };
struct G { char pad[0x3D58]; int n; };
extern struct G D_003DD490;
struct M4 __ml__FRC7MATRIX4T0(void *, struct T *);
void func_00186820(struct P *);
/** @sly2-annot addr=0x1F4010
 * Purpose: SO vt37: root XF matrix = proxy matrix * own, bump g_sw stamp, vt18, 0x186820
 * Info: proposed name ApplySoProxy (T2); class SO slot37; TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/formats/meshes-scene.md#4-scene-assembly-and-placement,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4010(struct P *p, void *pv)
{
    struct T *pt = p->pt;
    pt->m = __ml__FRC7MATRIX4T0(pv, pt);
    { int n = ++D_003DD490.n; p->pt->nA4 = n; }
    p->pvt->a[18].pfn((char *)p + p->pvt->a[18].d);
    func_00186820(p);
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
struct LO { char pad[0x30]; V4 v30; M4 m40; char pad2[0xA0 - 0x80]; struct LO *pA0; int nA4; };
struct REGION;
struct REGION_LINK;
struct E { char pad[0xDC]; struct REGION_LINK *regionLink; };
struct VTE { short d; short p; void (*pfn)(void *); };
struct P { char pad[0xC]; struct VTE *pvt; char pad2[0x58 - 0x10]; struct LO *p58; char pad3[0x60 - 0x5C]; struct LO *p60; char pad4[0xE8 - 0x64]; struct E *pe; };
struct SW { char pad[0x3D58]; int n3D58; };
extern struct SW D_003DD490;
qword *VECTOR_opAssign(qword *, qword);
M4 *InvertMatrix4Rigid(M4 *,const M4 *);
M4 *__ml__FRC7MATRIX4T0(M4 *, const M4 *, const M4 *);
void *func_002333B4(void *, int, int) __asm__("memset");
void func_001F4E28(struct P *);
void SlotUnsettle(struct P *, struct LO *);
void UpdateSoPartMatrices(struct P *);
qword func_001A62E8(struct P *);
struct REGION *PpathzoneFromPos(qword, int);
void func_001F37C8(struct P *, struct REGION *);
/** @sly2-annot addr=0x1F40A0
 * Purpose: SO vt29: compute new position XF matrix relative to parents, free contacts, refresh XF kinds, part
 *   matrices, region
 * Info: proposed name TranslateSoToPos (T1); class SO slot29; TU so
 * Notes: names_v2 B proposed EnsureSoLvo__FP2SO (0.856)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void TranslateSoToPos(struct P *p, qword v)
{
    M4 m0, m40, m80, mC0, m100, m140, m180, m1C0;
    struct LO *plo;
    const M3 *basis;
    register M4 *presult __asm__("$18");
    if (!p->p58) return;
    basis = &p->p60->m40.s.m3;
    VECTOR_opAssign(&m0.s.r3, v);
    m0.s.m3 = *basis;
    m0.f[1][3] = 0.0f;
    m0.f[2][3] = 0.0f;
    m0.f[0][3] = 0.0f;
    m0.f[3][3] = 1.0f;
    presult=&mC0;
    InvertMatrix4Rigid(&m100,&p->p58->m40);
    __ml__FRC7MATRIX4T0(presult, &m100, &p->p60->m40);
    m40.r[1] = mC0.r[1];
    m40.r[0] = mC0.r[0];
    m40.r[2] = mC0.r[2];
    m40.r[3] = mC0.r[3];
    if (p->p58->pA0) {
        m80 = p->p58->pA0->m40;
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
    InvertMatrix4Rigid(&m1C0,&m40);
    __ml__FRC7MATRIX4T0(&m100, world, &m1C0);
    plo = p->p58;
    mC0 = m100;
    VECTOR_opAssign(&plo->v30.q, mC0.r[3]);
    plo->v30.f[3] = 1.0f;
    { int stamp=++D_003DD490.n3D58;p->p58->nA4=stamp; }
    p->pvt[18].pfn((char *)p + p->pvt[18].d);
    func_001F4E28(p);
    SlotUnsettle(p, p->p58);
    UpdateSoPartMatrices(p);
    {
        struct REGION_LINK *link = 0;
        if (p->pe) link = p->pe->regionLink;
        if (link) func_001F37C8(p, PpathzoneFromPos(func_001A62E8(p), 1));
    }
}

}
} // namespace M_TranslateSoToPos

// 0x1F42E0 RotateSoToMat
namespace M_RotateSoToMat {
union U; struct M3; struct LO; struct VTE; struct P; struct G;
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; int i[4]; };
struct M3 { U r[3]; M3(){} M3(const M3 &m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];} M3 &operator=(const M3 &m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return *this;} };
struct LO { M3 matrix; U position; M3 rotation; char pad[0x30]; LO *parent; int stamp; };
struct VTE { short delta,pad; void (*fn)(void *); };
struct P { char pad[0xC]; VTE *vt; char pad10[0x4C]; LO *object; };
struct G { char pad[0x3D58]; int stamp; };
extern "C" { extern G D_003DD490; }
extern "C" M3 InvertMatrix3Cofactor(const M3 *);
extern "C" void NormalizeRotateMatrix3__FP7MATRIX3(M3 *);
extern "C" void *func_002333B4(void *,int,int) __asm__("memset");
extern "C" void func_001F4E28(P *);
extern "C" void SlotUnsettle(P *,LO *);
extern "C" void UpdateSoPartMatrices(P *);
static inline M3 Mul(const M3 &a,const M3 &b) return r
{
    qword a0=a.r[0].q,a1=a.r[1].q,a2=a.r[2].q;
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[0].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[0].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[1].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[1].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[2].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[2].q));
}
/** @sly2-annot addr=0x1F42E0
 * Purpose: SO vt30: set rotation XF matrix from world matrix (parent-normalized), bump stamp, vt18, free
 *   contacts, refresh
 * Info: proposed name RotateSoToMat (T1); class SO slot30; TU so
 * Notes: names_v2 B proposed ApplySoImpulse__FP2SOP6VECTORT1f (0.792)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void RotateSoToMat(P *p,const M3 *basis)
{
    LO *object=p->object;
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
        M3 *dest=&p->object->matrix;
        *dest=transformed;
        dest->r[0].i[3]=0;
        dest->r[1].i[3]=0;
        dest->r[2].i[3]=0;
        int stamp=++D_003DD490.stamp;
        p->object->stamp=stamp;
        p->vt[18].fn((char *)p+p->vt[18].delta);
        func_001F4E28(p);
        SlotUnsettle(p,p->object);
        UpdateSoPartMatrices(p);
    }
}
} // namespace M_RotateSoToMat

// 0x1F4440 GetSoVelocity
namespace M_GetSoVelocity { extern "C" {
struct T; struct S;
typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct T { char pad[0x70]; qword v; char pad2[0x14]; int n; };
struct S { char pad[0x58]; struct T *pt; };
qword GetXfVelocity(struct T *, int, qword);
/** @sly2-annot addr=0x1F4440
 * Purpose: SO vt33: velocity of position XF (kind 1) in world space, else zero
 * Info: proposed name GetSoVelocity (T1); class SO slot33; TU so
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
qword GetSoVelocity(struct S *p) { struct T *pt = p->pt; if (!pt || pt->n != 1) return D_002A2DD0; return GetXfVelocity(pt, 0, pt->v); }
} } // namespace M_GetSoVelocity

// 0x1F4488 SetSoVelocity
namespace M_SetSoVelocity { extern "C" {
struct VTE; struct VT; struct O; struct P;
typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short p; void *pfn; };
struct VT { struct VTE a[34]; };
struct O { char pad[0x94]; int n94; short k; char pad2[0xA0 - 0x9A]; int nA0; char pad3[0xB0 - 0xA4]; union { qword q; int a[4]; } v; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x58 - 0x10]; struct O *po; };
int func_00230310(int);
void func_002303A0(struct P *, struct O *, int);
qword ConvertXfVec(int, int, qword);
void SlotUnsettle(struct P *, struct O *);
void func_001F4BF8(struct P *);
void func_001F5078(struct P *);
void func_001F8668(struct P *, int);
/** @sly2-annot addr=0x1F4488
 * Purpose: SO vt35: set position-XF velocity so world velocity = v (minus current), refresh, mark phys
 * Info: proposed name SetSoVelocity (T1); class SO slot35; TU so
 * Notes: names_v2 C proposed CalculateSoTrajectoryApex__FP2SOP6VECTORfT1 (0.638)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void SetSoVelocity(struct P *p, qword q)
{
    struct O *po = p->po;
    qword cur, d;
    if (!po || po->n94 != 1) return;
    if (!func_00230310(po->k)) func_002303A0(p, po, 1);
    { float *pv = (float *)&po->v; pv[2] = 0.0f; pv[1] = 0.0f; pv[0] = 0.0f; }
    cur = ((qword (*)(void *))p->pvt->a[33].pfn)((char *)p + p->pvt->a[33].d);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(q), "j"(cur));
    po->v.q = ConvertXfVec(0, po->nA0, d);
    SlotUnsettle(p, po);
    func_001F4BF8(p);
    func_001F5078(p);
    func_001F8668(p, 0);
}
} } // namespace M_SetSoVelocity

// 0x1F4558 GetSoAngularVelocity
// GetSoAngularVelocity: ported from Lilly's personal Sly 2 decomp (src/P2/so_motion.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A2DD0 (g_qwZero)
// 0x001F4558 GetAngularVelocity__2SO  (MATCHES Sly 2, T0906)
// Virtual 33: the angular velocity of the rotation XF (xfk 6) in world space, zero without one. The chain's rate matrix (MatDmatOfXfChain,
// MATCH) is turned by N * inverse(matWorld) (N = matWorld with its rotation orthonormalised: the scale is taken out) and the skew-symmetric
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
MATRIX4 MatDmatOfXfChain(XF *pxfFrom, XF *pxfTo) __asm__("func_0022D148");
qword SO::GetAngularVelocity()
{
    XF *pxf = pxfUnk5c;
    if (!pxf || pxf->xfk != 6)
    {
        return D_002A2DD0;
    }
    MATRIX3 matNorm;
    MATRIX3 matRate;
    MATRIX4 *pmatDmat = &MatDmatOfXfChain(pxf, 0);
    matRate = *(MATRIX3 *)pmatDmat;
    MATRIX3 *pmatWorld = (MATRIX3 *)&pxf->matWorld;
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
struct M3 { U r[3]; M3(){} M3(const M3 &m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];} M3 &operator=(const M3 &m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return *this;} };
struct LO { M3 matrix; U position; M3 rotation; char pad[0x24]; int kind; short state; char pad9A[6]; LO *parent; int stamp; char padA8[8]; U velocity; };
struct VTE { short delta,pad; qword (*fn)(void *); };
struct P { char pad[0xC]; VTE *vt; char pad10[0x4C]; LO *object; };
extern "C" M3 InvertMatrix3Cofactor(const M3 *);
extern "C" void NormalizeRotateMatrix3__FP7MATRIX3(M3 *);
extern "C" void *func_002333B4(void *,int,int) __asm__("memset");
extern "C" void SlotUnsettle(P *,LO *);
static inline M3 Mul(const M3 &a,const M3 &b) return r
{
    qword a0=a.r[0].q,a1=a.r[1].q,a2=a.r[2].q;
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[0].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[0].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[1].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[1].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[2].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[2].q));
}
extern "C" int func_00230310(int);
extern "C" void func_002303A0(P *,LO *,int);
extern "C" qword ConvertXfVec(LO *,LO *,qword);
extern "C" void func_001F4BF8(P *);
extern "C" void func_001F5078(P *);
extern "C" void func_001F8668(P *,long);
static inline qword Sub(qword a,const qword *pb) { qword r,b; __asm__("lqc2 %1,%3\n\tvsub.xyzw %0,%2,%1":"=j"(r),"=&j"(b):"j"(a),"m"(*pb));return r; }
static inline qword Transform(const M3 &m,qword v) { qword r;
 __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r):"j"(m.r[0].q),"j"(m.r[1].q),"j"(m.r[2].q),"j"(v));return r;
}
static inline void Clear(U *v) { v->i[2]=0;v->i[1]=0;v->i[0]=0; }
/** @sly2-annot addr=0x1F46D8
 * Purpose: SO vt36: set rotation-XF (kind 6) angular velocity from world w
 * Info: proposed name SetSoAngularVelocity (T1); class SO slot36; TU so
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void SetSoAngularVelocity(P *p,qword position)
{
    LO *object=p->object;
    if(!object || object->kind!=6)return;
    if(!func_00230310(object->state))func_002303A0(p,object,1);
    const M3 *rotation=&object->rotation;
    Clear(&object->velocity);
    M3 normalized;
    qword origin=p->vt[34].fn((char *)p+p->vt[34].delta);
    normalized=*rotation;
    NormalizeRotateMatrix3__FP7MATRIX3(&normalized);
    M3 relative=Mul(*rotation,InvertMatrix3Cofactor(&normalized));
    object->velocity.q=ConvertXfVec(0,object->parent,Transform(relative,Sub(position,&origin)));
    SlotUnsettle(p,object);
    func_001F4BF8(p);
    func_001F5078(p);
    func_001F8668(p,0);
}
} // namespace M_SetSoAngularVelocity

// 0x1F4848 func_001F4848
/** @sly2-annot addr=0x1F4848
 * Purpose: Convert position XF velocity (+0xB0) between XF spaces (0x22FCC8) - result discarded by m2c
 * Info: proposed name GetSoPosXfVelocityLocal (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F4848(SO *pso)
{
    XF *pxf;
    pxf = pso->pxfUnk58;
    ConvertXfVec(pxf->pxfParent, pso->pxfRoot, *(qword *)&pxf->vUnkB0);
}

// 0x1F4870 func_001F4870
/** @sly2-annot addr=0x1F4870
 * Purpose: Same for rotation XF (+0x5C)
 * Info: proposed name GetSoRotXfVelocityLocal (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F4870(SO *pso)
{
    XF *pxf;
    pxf = pso->pxfUnk5c;
    ConvertXfVec(pxf->pxfParent, pso->pxfRoot, *(qword *)&pxf->vUnkB0);
}

// 0x1F4898 func_001F4898
namespace M_func_001F4898 { extern "C" {
struct O; struct P; struct Q;
typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x98]; short k; char pad2[0xA0 - 0x9A]; int nA0; char pad3[0xB0 - 0xA4]; qword q; };
struct P { char pad[0x58]; struct O *po; };
struct Q { char pad[0x60]; int n60; };
int func_00230310(int);
void func_002303A0(void *, struct O *, int);
qword ConvertXfVec(int, int, qword);
void SlotUnsettle(void *, struct O *);
void func_001F4BF8(void *);
void func_001F5078(void *);
void func_001F8668(void *, int);
/** @sly2-annot addr=0x1F4898
 * Purpose: Set position XF velocity from a vector given in root XF space, refresh, mark phys
 * Info: proposed name SetSoPosXfVelocityLocal (T2); TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4898(struct P *p, qword v)
{
    struct O *po;
    if (!func_00230310(p->po->k)) func_002303A0(p, p->po, 1);
    po = p->po;
    po->q = ConvertXfVec(((struct Q *)p)->n60, po->nA0, v);
    SlotUnsettle(p, p->po);
    func_001F4BF8(p);
    func_001F5078(p);
    func_001F8668(p, 0);
}
} } // namespace M_func_001F4898

// 0x1F4930 func_001F4930
namespace M_func_001F4930 { extern "C" {
struct O; struct P; struct Q;
typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x98]; short k; char pad2[0xA0 - 0x9A]; int nA0; char pad3[0xB0 - 0xA4]; qword q; };
struct P { char pad[0x5C]; struct O *po; };
struct Q { char pad[0x60]; int n60; };
int func_00230310(int);
void func_002303A0(void *, struct O *, int);
qword ConvertXfVec(int, int, qword);
void SlotUnsettle(void *, struct O *);
void func_001F4BF8(void *);
void func_001F5078(void *);
void func_001F8668(void *, int);
/** @sly2-annot addr=0x1F4930
 * Purpose: Same for rotation XF
 * Info: proposed name SetSoRotXfVelocityLocal (T2); TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4930(struct P *p, qword v)
{
    struct O *po;
    if (!func_00230310(p->po->k)) func_002303A0(p, p->po, 1);
    po = p->po;
    po->q = ConvertXfVec(((struct Q *)p)->n60, po->nA0, v);
    SlotUnsettle(p, p->po);
    func_001F4BF8(p);
    func_001F5078(p);
    func_001F8668(p, 0);
}
} } // namespace M_func_001F4930

// 0x1F49C8 func_001F49C8
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
    void *pE8;
};
extern "C" void func_001A6B88(X *, X *);
/** @sly2-annot addr=0x1F49C8
 * Purpose: SO vt38: LO::MatchLoXf then copy velocities via vt33..36 when SO has physics
 * Info: proposed name MatchSoXf (T2); class SO slot38; TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F49C8(X *p, X *q)
{
    func_001A6B88(p, q);
    if (p->pE8) {
        p->Set35(q->Get33());
        p->Set36(q->Get34());
    }
}
} // namespace M_func_001F49C8

// 0x1F4A80 func_001F4A80
namespace M_func_001F4A80 {
union U; struct M4; struct VTE; struct A; struct P;
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct M4 {U r[4];M4(){} M4(const M4&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];r[3]=m.r[3];} M4&operator=(const M4&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];r[3]=m.r[3];return *this;}};
struct VTE { short d; short pad; void (*pfn)(); };
struct A { char pad[0x70]; qword pos; };
struct P { char pad[0xC]; struct VTE *pvt; char pad2[0xE8 - 0x10]; void *pe; };
extern "C" void func_001A6BC8(struct P *p, struct A *pa);
extern "C" M4 *func_0022D148(M4 *,A *,A *);
static inline M4 BuildVelocity(A *owner) return result { func_0022D148(&result,owner,0); }
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__ __volatile__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
typedef void (*SETVECTOR)(void *,qword);
static inline SETVECTOR ReadSetter(const VTE &entry){
 SETVECTOR function;
 asm("lw %0,%1":"=r"(function):"m"(entry.pfn));
 return function;
}
/** @sly2-annot addr=0x1F4A80
 * Purpose: SO vt39: LO::SetXf, then derive linear/angular velocity from XF rate matrix (0x22D148) via
 *   vt35/vt36
 * Info: proposed name SetSoXf (T2); class SO slot39; TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F4A80(struct P *p, struct A *pa)
{
    M4 m;
    qword q;
    func_001A6BC8(p, pa);
    if (!p->pe) return;
    { M4 t = BuildVelocity(pa); m = t; }
    VTE *vt=p->pvt;
    int delta=vt[35].d;
    q = Make(m.r[1].f[2],m.r[2].f[0],m.r[0].f[1]);
    SETVECTOR setpos=ReadSetter(vt[35]);
    {
        register qword r __asm__("vf6");
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w" : "+j"(r) : "j"(m.r[0].q), "j"(m.r[1].q), "j"(m.r[2].q), "j"(m.r[3].q), "j"(pa->pos));
    U v; v.q = q;
    qword rr;__asm__("qmfc2.ni %0,%1":"=r"(rr):"j"(r));
    setpos((char*)p+delta,rr);
    }
    {
        ((void (*)(void *, qword))p->pvt[36].pfn)((char *)p + p->pvt[36].d, q);
    }
}
} // namespace M_func_001F4A80

// 0x1F4B58 func_001F4B58
/** @sly2-annot addr=0x1F4B58
 * Purpose: Set tree flag bit0 and add tree to dirty list g_sw.dlSoTreeQueue1
 * Info: proposed name MarkSoTreeBoundsDirty (T2); TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
/* Queues the object's tree on list 1 (g_sw.dlSoTreeQueue1) once. */
extern "C" void func_001F4B58(SO *pso)
{
    SOTREE *ptree;
    ptree = pso->ptree;
    if (ptree != 0) {
        if ((uchar)ptree->fQueued1 == 0) {
            ptree->fQueued1 = 1;
            AppendDlEntry(&g_sw.dlSoTreeQueue1, pso->ptree);
        }
    }
}

// 0x1F4BA8 func_001F4BA8
namespace M_func_001F4BA8 { extern "C" {
struct T; struct S;
struct T { char pad[0x70]; ulong fl; };
struct S { char pad[0xEC]; struct T *pt; };
struct DL;
void RemoveDlEntry__FP2DLPv(struct DL *, void *);
/** @sly2-annot addr=0x1F4BA8
 * Purpose: Clear tree flag bit0 and remove from g_sw.dlSoTreeQueue1
 * Info: proposed name ClearSoTreeBoundsDirty (T2); TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4BA8(struct S *p)
{
    struct T *pt = p->pt;
    if (!pt) return;
    if ((pt->fl & 1) == 0) return;
    pt->fl &= ~1;
    RemoveDlEntry__FP2DLPv(&g_sw.dlSoTreeQueue1, p->pt);
}
} } // namespace M_func_001F4BA8

// 0x1F4BF8 func_001F4BF8
/** @sly2-annot addr=0x1F4BF8
 * Purpose: After an SO moved/changed bounds: MarkSoTreeBoundsDirty, then flag its broadphase OXA (+0xEC, flag
 *   x70 bit1) dirty once and queue it on g_sw.dlSoTreeQueue2 so UpdateSwAaox re-sorts its sweep endpoints
 * Info: proposed name InvalidateSoAaox (T2); TU so
 * Notes: v3: superseded MarkSoTreeDirty | top50_round (0.55): Body + callees AppendDlEntry. +0xEC is the OXA
 *   node (AddSwAaobrObject allocates it; RecalcSwOxfFilterForObject tests +0xEC). Per-object counterpart of
 *   Sly 1 bbmark InvalidateSwAaox (psw->fAaoxValid=0). Callers: SetSoVelo
 * Confidence: low (draft) [T2 0.55] - evidence: Sly 1 / Lilly reference; semantic review; sources:
 *   r6_1D5150+top50_round
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/physics-collision.md#bbmark-0x1336f0-0x1344f0,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
/* Queues the object's tree on lists 1 and 2 (g_sw.dlSoTreeQueue2) once. */
extern "C" void func_001F4BF8(SO *pso)
{
    SOTREE *ptree;
    func_001F4B58(pso);
    ptree = pso->ptree;
    if (ptree != 0) {
        if (ptree->fQueued2 == 0) {
            ptree->fQueued2 = 1;
            AppendDlEntry(&g_sw.dlSoTreeQueue2, pso->ptree);
        }
    }
}

// 0x1F4C58 PxfFindSo
namespace M_PxfFindSo { extern "C" {
struct E; struct S;
struct E { char pad[0x94]; int b; int pad2; int a; char pad4[0x60]; };
struct S { char pad[0x50]; int c; struct E *ae; };
/** @sly2-annot addr=0x1F4C58
 * Purpose: Find XF in SO XF array (+0x50/+0x54, 0x100 stride) matching (+0x9C, +0x94) ids
 * Info: proposed name PxfFindSo (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
struct E *PxfFindSo(struct S *p, int a, int b)
{
    int i;
    for (i = 0; i < p->c; i++) { struct E *pe = &p->ae[i]; if (pe->a == a && pe->b == b) return pe; }
    return 0;
}
} } // namespace M_PxfFindSo

// 0x1F4CA0 func_001F4CA0
namespace M_func_001F4CA0 { extern "C" {
struct T; struct S;
struct T { char pad[0x80]; int a80; int a84; void *p88; void *p8C; char pad2[0x100 - 0x90]; };
struct S { char pad[0x10]; struct S *next; char pad2[0x50 - 0x14]; int c; struct T *base; };
/** @sly2-annot addr=0x1F4CA0
 * Purpose: SO vt52: re-point an XF* from base SO's XF block to the clone's (also via parent)
 * Info: proposed name CloneXfRef (T1); class SO slot52; TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4CA0(struct S *pnew, struct S *pold, struct T **pp)
{
    struct T *t = *pp;
    struct T *n;
    int i;
    if (!t) return;
    n = 0;
    i = t - pold->base;
    if (i >= 0 && i < pnew->c) {
        n = pnew->base + i;
    } else {
        pold = pold->next;
        i = t - pold->base;
        if (i >= 0 && i < pold->c) n = pnew->next->base + i;
    }
    if (n->p88 == &t->a80) n->p88 = &n->a80;
    if (n->p8C == &t->a84) n->p88 = &n->a84;
    *pp = n;
}
} } // namespace M_func_001F4CA0

// 0x1F4D50 func_001F4D50
/** @sly2-annot addr=0x1F4D50
 * Purpose: Inverse of MarkSoTreeDirty (clear bits 0/1, remove from both lists)
 * Info: proposed name ClearSoTreeDirty (T2); TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F4D50(SO *pso)
{
    SOTREE *ptree;
    func_001F4BA8(pso);
    ptree = pso->ptree;
    if (ptree != 0) {
        if (ptree->fQueued2) {
            ptree->fQueued2 = 0;
            RemoveDlEntry(&g_sw.dlSoTreeQueue2, pso->ptree);
        }
    }
}

// 0x1F4DA8 func_001F4DA8
namespace M_func_001F4DA8 { extern "C" {
struct N;
struct N { int pad[2]; void *p8; char pad2[0xAC - 0xC]; struct N *next; };
struct G;
extern struct G D_003DD490;
void func_001F5078(void *);
void FreeSwXpList__FP2SWP2XP(struct G *, struct N *);
/** @sly2-annot addr=0x1F4DA8
 * Purpose: Unlink contact nodes between pa and pb from list, free them (FreeSwXpList) and invalidate pa
 * Info: proposed name RemoveSoContactsWith (T2); TU so
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4DA8(void *pa, void *pb, struct N **ppn)
{
    struct N *pfree = 0;
    struct N *pn;
loop:
    pn = *ppn;
    if (pn) {
        int i = pn->p8 == pa;
        if (((struct N *)((char *)pn + (i << 6)))->p8 == pb) {
            *ppn = pn->next;
            pn->next = pfree;
            pfree = pn;
        } else {
            ppn = &pn->next;
        }
        goto loop;
    }
    if (pfree) {
        func_001F5078(pa);
        FreeSwXpList__FP2SWP2XP(&D_003DD490, pfree);
    }
}
} } // namespace M_func_001F4DA8

// 0x1F4E28 func_001F4E28
namespace M_func_001F4E28 { extern "C" {
struct VTE; struct VT; struct X; struct N; struct E; struct P;
struct VTE { short d; short p; void (*pfn)(void *, void *); };
struct VT { struct VTE a[55]; };
struct X { char pad[0xC]; struct VT *pvt; };
struct N { void *pv0; struct N *next; int *pn8; struct X *px; };
struct E { char pad[0x60]; struct X *p60; struct N *pn; void *p68; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0xEC - 0x10]; struct E *pe; };
struct G;
extern struct G D_003DD490;
void func_001F4BF8(struct P *);
void FreeSwXpList__FP2SWP2XP(struct G *, void *);
#define VF(q, n, a) (q)->pvt->a[n].pfn((char *)(q) + (q)->pvt->a[n].d, a)
/** @sly2-annot addr=0x1F4E28
 * Purpose: Notify both sides (vt54) for each contact of the tree, free XP lists, clear tree contacts
 * Info: proposed name FreeSoContacts (T2); TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4E28(struct P *p)
{
    struct N *pn;
    if (!p->pe) return;
    func_001F4BF8(p);
    for (pn = p->pe->pn; pn; pn = pn->next) {
        struct X *q = p->pe->p60;
        q->pvt->a[54].pfn((char *)q + q->pvt->a[54].d, pn->px);
        pn->px->pvt->a[54].pfn((char *)pn->px + pn->px->pvt->a[54].d, p->pe->p60);
        FreeSwXpList__FP2SWP2XP(&D_003DD490, pn->pv0);
        *pn->pn8 = 0;
        pn->pv0 = 0;
    }
    p->pvt->a[54].pfn((char *)p + p->pvt->a[54].d, 0);
    FreeSwXpList__FP2SWP2XP(&D_003DD490, p->pe->p68);
    p->pe->p68 = 0;
}
} } // namespace M_func_001F4E28

// 0x1F4F10 func_001F4F10
namespace M_func_001F4F10 { extern "C" {
struct S8; struct N; struct E; struct P;
typedef unsigned long ulong;
struct S8 { long l; } __attribute__((packed));
struct N { int n0; struct N *next; };
struct E { char pad[0x64]; struct N *pn; int pad2; int n6C; };
struct P { char pad[0xEC]; struct E *pe; char pad2[0x148 - 0xF0]; ulong fl; };
struct G;
extern struct G D_003DD490;
extern const struct S8 D_002A8278;
void EnsureSwCallback(struct G *, struct P *, long, int);
/** @sly2-annot addr=0x1F4F10
 * Purpose: SO vt58: if tree touching flags set, schedule touch callback 0x1F7950 (via D_002A8278) with g_sw
 * Info: proposed name ScheduleSoTouchEvents (T2); class SO slot58; TU so
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4F10(struct P *p)
{
    struct E *pe = p->pe;
    struct N *pn;
    if (pe->n6C) {
        EnsureSwCallback(&D_003DD490, p, D_002A8278.l, 0);
    } else if (p->fl & 0x3000000) {
        for (pn = pe->pn; pn; pn = pn->next) {
            if (pn->n0) {
                EnsureSwCallback(&D_003DD490, p, D_002A8278.l, 0);
                break;
            }
        }
    }
}
} } // namespace M_func_001F4F10

// 0x1F4FA8 func_001F4FA8
/** @sly2-annot addr=0x1F4FA8
 * Purpose: SO vt59 default no-op
 * Info: proposed name SO_vt59_Nop (T2); class SO slot59; TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
extern "C" void func_001F4FA8(void)
{
}

// 0x1F4FB0 func_001F4FB0
/** @sly2-annot addr=0x1F4FB0
 * Purpose: SO vt55 default no-op
 * Info: proposed name SO_vt55_Nop (T2); class SO slot55; TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#barrier-0x1320c8-0x132570,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F4FB0(void)
{
}

// 0x1F4FB8 func_001F4FB8
/** @sly2-annot addr=0x1F4FB8
 * Purpose: SO vt56 default no-op
 * Info: proposed name SO_vt56_Nop (T2); class SO slot56; TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F4FB8(void)
{
}

// 0x1F4FC0 func_001F4FC0
/** @sly2-annot addr=0x1F4FC0
 * Purpose: SO vt57 default no-op
 * Info: proposed name SO_vt57_Nop (T2); class SO slot57; TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F4FC0(void)
{
}

// 0x1F4FC8 FIgnoreSoIntersection
namespace M_FIgnoreSoIntersection { extern "C" {
struct VTE; struct VT; struct X;
typedef unsigned long ulong;
struct VTE { short d; short p; int (*pfn)(void *); };
struct VT { struct VTE a[63]; };
struct X { char pad[0xC]; struct VT *pvt; char pad2[0x148 - 0x10]; ulong fl; };
#define VF(p) (p)->pvt->a[62].pfn((char *)(p) + (p)->pvt->a[62].d)
/** @sly2-annot addr=0x1F4FC8
 * Purpose: SO vt61: true if (pa flag +0x148 bit26 or pa fixed) and pb fixed
 * Info: proposed name FIgnoreSoIntersection (T1); class SO slot61; TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int FIgnoreSoIntersection(struct X *pa, struct X *pb)
{
    if (((pa->fl & 0x4000000) || VF(pa)) && VF(pb)) return 1;
    return 0;
}
} } // namespace M_FIgnoreSoIntersection

// 0x1F5040 func_001F5040
/** @sly2-annot addr=0x1F5040
 * Purpose: SO vt62: true if no SOPHYS or both XF modes (+0xA0/+0xC0) == 3
 * Info: proposed name FIsSoFixed (T2); class SO slot62; TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
/* Whether the object is fixed: no physics, or both constraints of kind 3. */
extern "C" int func_001F5040(SO *pso)
{
    SOPHYS *psophys;
    int nKind;
    int f;
    psophys = pso->psophys;
    f = 0;
    if ((psophys == 0) || ((nKind = psophys->constrForce.nKind, nKind == 3) && (psophys->constrTorque.nKind == nKind))) {
        f = 1;
    }
    return f;
}

// 0x1F5078 func_001F5078
/** @sly2-annot addr=0x1F5078
 * Purpose: If the SO is in the broadphase (+0xEC): SetSoGrf(4) (vt47, the awake flag also set by
 *   SlotSetRole/WakeXfSlot) and, if +0xE8, un-settle its position and rotation XF slots (SlotUnsettle
 *   0x230598)
 * Info: proposed name WakeSo (T2); TU so
 * Notes: v3: superseded InvalidateSoPhys | top50_round (0.45): Body: vt[47](4) on the SO, then 0x230598 on
 *   +0x58 (position XF) and +0x5C (rotation XF). 0x230598 maps every at-rest role to its moving counterpart
 *   (decoded jump table D_002AA000: 3->1, 6/10->7, 12->11, 19->18), the inve
 * Confidence: low (draft) [T2 0.45] - evidence: semantic review; sources: r6_1D5150+top50_round
 * See: docs/engine/physics-collision.md#7-key-functions, docs/open-questions.md#physics--navigation,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
/* Wakes the object (LO slot 47 with 4) and unsettles its two physics transforms. */
extern "C" void func_001F5078(SO *pso)
{
    if (pso->ptree != 0) {
        ((void (*)(void *, int))PVTE(pso)[47].pfn)((char *)pso + PVTE(pso)[47].d, 4);
        if (pso->psophys != 0) {
            SlotUnsettle(pso, pso->pxfUnk58);
            SlotUnsettle(pso, pso->pxfUnk5c);
        }
    }
}

// 0x1F50E0 UpdateSoTreeBounds
namespace M_UpdateSoTreeBounds {
struct MATRIX4; struct VECTOR; struct XF; struct GEOM; struct DLE; struct SOPART; struct SOTREE; struct SO; struct SW; struct VU_FLOAT;
// UpdateSoTreeBounds (0x1F50E0): SO virtual 48 - rebuild the bounds of the SO's tree record (SO+0xEC).
// Pass 1, the box of every visible collision part: UpdateSoPartGeom (0x1F38A8) first, then a sphere part adds its
// XF position -/+ radius * D_002A2DE0, a mesh part its world points, and a part whose geometry is stale
// (nStampGeom != nStampWorld) also its local points from nUnk1c on, moved by the part's matrix.
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
struct XF {char pad[0x40];MATRIX4 matWorld;};
struct GEOM {int cpos;VECTOR *apos;int csurf;void *asurf;int cedge;void *aedge;};
struct SOPART;
struct DLE {SOPART *next,*prev;};
struct SOPART
{
    /* 0x00 */ XF *pxf;
    /* 0x04 */ void *pso;
    /* 0x08 */ float gUnk08;          // sphere radius (fUnk49 parts)
    /* 0x0c */ int oid;
    /* 0x10 */ void *plvo;
    /* 0x14 */ short dhp;
    /* 0x16 */ ulong fUnk48:1;
               ulong fUnk49:1;        // the part is a sphere around its XF's position
               ulong fHidden:1;
               ulong unk_0x16:13;
    /* 0x18 */ unsigned grfzpk;
    /* 0x1c */ int nUnk1c;
    /* 0x20 */ MATRIX4 matWorld;
    /* 0x60 */ DLE dle;
    /* 0x68 */ char bspc[0x10];
    /* 0x78 */ GEOM geomLocal;
    /* 0x90 */ GEOM geomWorld;
    /* 0xa8 */ int nStampGeom;
    /* 0xac */ int nStampWorld;
};
struct SOTREE {qword qwMin,qwMax,qwCenter;float sRadius;SOPART *ppartFirst;};
struct SO {char pad[0xEC];SOTREE *ptree;};
struct SW {char pad[0x3CCC];int fUnk3CCC;};
struct VU_FLOAT {qword qw;};
extern "C" { extern qword D_002A2DE0,D_002A2DF0,D_002A2E00; }
extern "C" { extern SW D_003DD490; }
extern "C" void UpdateSoPartGeom(SOPART *);
extern "C" void func_001F4BA8(SO *);

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
static inline qword QwAddScaledF(qword qwBase, qword qwVec, float g)
{
    return QwAddScaled(qwBase, qwVec, VuFloat(g));
}
static inline qword QwTransformPos(MATRIX4 *pmat, qword qwPos)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qwResult)
            : "j"(*(qword *)pmat->mat[0]), "j"(*(qword *)pmat->mat[1]), "j"(*(qword *)pmat->mat[2]), "j"(*(qword *)pmat->mat[3]), "j"(qwPos));
    return qwResult;
}
static inline qword QwSub(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline qword QwMin(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vmini.xyz %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline qword QwMax(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vmax.xyz %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
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
static inline float GDistance(const qword &qwA, const qword &qwB)
{
    return GLength(QwSub(qwA, qwB));
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
static inline void ExtendBox(qword &qwMin, qword &qwMax, const qword &qwPos)
{
    qwMin = QwMin(qwMin, qwPos);
    qwMax = QwMax(qwMax, qwPos);
}

/** @sly2-annot addr=0x1F50E0
 * Purpose: SO vt49: rebuild tree record AABB and bounding sphere from visible parts, set g_sw+0x3CCC, clear
 *   dirty
 * Info: proposed name UpdateSoTreeBounds (T1); class SO slot49; TU so
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/physics-collision.md#1-so-the-solid-object-socpp-0x1f2108-0x1f8dd8-174-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
extern "C" void UpdateSoTreeBounds(SO *pso)
{
    qword qwMin = D_002A2DF0;
    qword qwMax = D_002A2E00;
    SOPART *ppart;
    int i;
    for (ppart = pso->ptree->ppartFirst; ppart; ppart = ppart->dle.next)
    {
        if (ppart->fHidden)
            continue;
        UpdateSoPartGeom(ppart);
        if (ppart->fUnk49)
        {
            float gRadius = ppart->gUnk08;
            qword qwLo = QwAddScaledF(*(qword *)ppart->pxf->matWorld.mat[3], D_002A2DE0, -gRadius);
            qwMin = QwMin(qwMin, qwLo);
            qword qwHi = QwAddScaledF(*(qword *)ppart->pxf->matWorld.mat[3], D_002A2DE0, gRadius);
            qwMax = QwMax(qwMax, qwHi);
        }
        else
        {
            for (i = 0; i < ppart->geomWorld.cpos; i++)
            {
                qword qwPos = *(qword *)&ppart->geomWorld.apos[i];
                ExtendBox(qwMin, qwMax, qwPos);
            }
        }
        if (ppart->nStampGeom != ppart->nStampWorld)
        {
            for (i = ppart->nUnk1c; i < ppart->geomWorld.cpos; i++)
            {
                qword qwPos = QwTransformPos(&ppart->matWorld, *(qword *)&ppart->geomLocal.apos[i]);
                ExtendBox(qwMin, qwMax, qwPos);
            }
        }
    }

    pso->ptree->qwMin = qwMin;
    pso->ptree->qwMax = qwMax;
    VU_FLOAT gHalf = VuFloat(0.5f);
    float sRadius = 0.0f;
    qword qwCenter;
    __asm__("vmulax.xyzw $ACC, %1, %3x\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qwCenter) : "j"(qwMin), "j"(qwMax), "j"(gHalf.qw));
    for (ppart = pso->ptree->ppartFirst; ppart; ppart = ppart->dle.next)
    {
        if (ppart->fUnk49)
        {
            float gDist = GDistance(qwCenter, *(qword *)ppart->pxf->matWorld.mat[3]) + ppart->gUnk08;
            if (sRadius < gDist)
                sRadius = gDist;
        }
        else
        {
            for (i = 0; i < ppart->geomWorld.cpos; i++)
            {
                float gDist = GDistance(qwCenter, *(qword *)&ppart->geomWorld.apos[i]);
                if (sRadius < gDist)
                    sRadius = gDist;
            }
        }
        if (ppart->nStampGeom != ppart->nStampWorld)
        {
            for (i = ppart->nUnk1c; i < ppart->geomWorld.cpos; i++)
            {
                qword qwPos = QwTransformPos(&ppart->matWorld, *(qword *)&ppart->geomLocal.apos[i]);
                float gDist = GDistanceFused(qwCenter, qwPos);
                if (sRadius < gDist)
                    sRadius = gDist;
            }
        }
    }
    pso->ptree->qwCenter = qwCenter;
    pso->ptree->sRadius = sRadius;
    D_003DD490.fUnk3CCC = 1;
    func_001F4BA8(pso);
}

typedef char layout_part_size[(sizeof(SOPART)==0xB0)?1:-1];
typedef char layout_part_matrix[((unsigned)&((SOPART*)0)->matWorld==0x20)?1:-1];
typedef char layout_part_dle[((unsigned)&((SOPART*)0)->dle==0x60)?1:-1];
typedef char layout_part_world[((unsigned)&((SOPART*)0)->geomWorld==0x90)?1:-1];
typedef char layout_part_stamp[((unsigned)&((SOPART*)0)->nStampWorld==0xAC)?1:-1];
typedef char layout_tree_first[((unsigned)&((SOTREE*)0)->ppartFirst==0x34)?1:-1];
} // namespace M_UpdateSoTreeBounds

// 0x1F5480 ExtendSoTreeBounds
namespace M_ExtendSoTreeBounds { extern "C" {
struct B; struct P;
typedef int qword __attribute__((mode(TI)));
struct B { qword min; qword max; qword c; float r; };
struct P { char pad[0xEC]; struct B *pb; };
/** @sly2-annot addr=0x1F5480
 * Purpose: Grow tree AABB/radius to include sphere (c,r)
 * Info: proposed name ExtendSoTreeBounds (T1); TU so
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void ExtendSoTreeBounds(struct P *p, qword c, float r)
{
    qword rr, lo, hi, d, t, one;
    float dist;
    __asm__("pextlw %0, %1, %1\n\tpextlw %0, %0, %0" : "=r"(rr) : "r"(r));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(lo) : "j"(c), "j"(rr));
    qword mn;
    __asm__("vmini.xyz %0, %1, %2" : "=j"(mn) : "j"(p->pb->min), "j"(lo));
    p->pb->min = mn;
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(hi) : "j"(c), "j"(rr));
    qword mx;
    __asm__("vmax.xyz %0, %1, %2" : "=j"(mx) : "j"(p->pb->max), "j"(hi));
    p->pb->max = mx;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(c) : "0"(c), "j"(p->pb->c));
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d), "=&j"(one) : "0"(c));
    __asm__("qmfc2.ni %0, %1" : "=r"(dist) : "j"(d));
    if (p->pb->r < dist) p->pb->r = dist;
}
} } // namespace M_ExtendSoTreeBounds

// 0x1F5510 func_001F5510
namespace M_func_001F5510 { extern "C" {
struct OUT;
typedef int qword __attribute__((mode(TI)));
typedef struct { qword row[3]; } M3;
struct OUT { qword axis; int mode; };
extern int D_002C3C20[];
extern qword D_002C3BA0[];
extern qword D_002A2DD0;
static inline qword Transform(M3 *matrix,qword value) {
    register qword x __asm__("vf2")=matrix->row[0];
    register qword y __asm__("vf1")=matrix->row[1];
    register qword z __asm__("vf3")=matrix->row[2];
    __asm__("vmulax.xyz $ACC,%1,%3x\n\tvmadday.xyz $ACC,%2,%3y\n\tvmaddz.xyz %0,%0,%3z" : "+j"(z) : "j"(x),"j"(y),"j"(value));
    return z;
}
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
/** @sly2-annot addr=0x1F5510
 * Purpose: Constraint table lookup: mode D_002C3C20[i], axis D_002C3BA0[i] transformed by matrix and
 *   normalized
 * Info: proposed name GetSoCnstrAxis (T2); TU so
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F5510(void *unused,M3 *matrix,int index,struct OUT *out) {
    out->mode=D_002C3C20[index];
    out->axis=NormalizeOr(Transform(matrix,D_002C3BA0[index]),D_002A2DD0);
}
} } // namespace M_func_001F5510

// 0x1F55C8 func_001F55C8
namespace M_func_001F55C8 { extern "C" {
struct VTE; struct Q; struct O;
typedef unsigned long ulong;
struct VTE { short d; short pad; void *(*pfn)(void *); };
struct O;
struct Q { char pad[0x60]; struct O *p60; };
struct O { char pad[0xC]; struct VTE *pvt; char pad2[0x28 - 0x10]; ulong fl; char pad3[0xEC - 0x30]; struct Q *pq; };
struct G;
extern struct G D_003DD490;
struct O *PsoFindPhysRoot(struct O *);
int CploFindSwObjectsByClass(struct G *, int, int, struct O *, int, struct O **);
void RemoveSoParts(struct O *);
void RecalcSwOxfFilterForObject(struct G *, struct O *);
void AddSoParts(struct O *);
void func_001F5078(struct O *);
/** @sly2-annot addr=0x1F55C8
 * Purpose: After physics-root change: regroup contact trees of SOs whose phys root changed, re-register
 * Info: proposed name RebuildSoPhysHook (T2); TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F55C8(struct O *p)
{
    struct O *ap[64];
    int i;
    int n;
    if (((p->fl >> 60) & 1) == 0) return;
    if (PsoFindPhysRoot(p) != p->pq->p60) {
        n = CploFindSwObjectsByClass(&D_003DD490, 1, 0x30, p, 0x40, ap);
        for (i = 0; i < n; i++) {
            {
                struct O *po = ap[i];
                if (po->pq) {
                    struct O *q = PsoFindPhysRoot(po);
                    if (q != po->pq->p60) {
                        RemoveSoParts(po);
                        if (!q->pq) {
                            q->pq = (__typeof__(q->pq))q->pvt[53].pfn((char *)q + q->pvt[53].d);
                            RecalcSwOxfFilterForObject(&D_003DD490, q);
                        }
                        po->pq = q->pq;
                        AddSoParts(po);
                    }
                }
            }
        }
    }
    RecalcSwOxfFilterForObject(&D_003DD490, p);
    func_001F5078(p);
}
} } // namespace M_func_001F55C8

// 0x1F56F8 func_001F56F8
/** @sly2-annot addr=0x1F56F8
 * Purpose: SO vt53: create tree record via g_sw (0x1336F0)
 * Info: proposed name SO_PtreeNew (T2); class SO slot53; TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F56F8(SO *pso)
{
    AddSwAaobrObject(&D_003DD490, pso);
}

// 0x1F5720 SetSoCnstrForce
/** @sly2-annot addr=0x1F5720
 * Purpose: Set translation constraint from CNSTR: make pos XF dynamic if needed, store axis/mode in
 *   SOPHYS+0xB0, rebuild phys hook
 * Info: proposed name SetSoCnstrForce (T1); TU so
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void SetSoCnstrForce(SO *pso, CONSTR *pconstr)
{
    XF *pxf;
    if (pso->psophys != 0) {
        pxf = pso->pxfUnk58;
        if ((pxf->pactFirst == 0) && (pxf->nRole != 2)) {
            func_002303A0(pso, pxf, 1);
        }
        func_001F5510(pso, pxf, pconstr, &pso->psophys->constrForce);
        func_001F55C8(pso);
        if (pso->psophys->constrForce.nKind != 3) {
            func_001F8668(pso, 0);
        }
    }
}

// 0x1F57C0 SetSoCnstrTorque
/** @sly2-annot addr=0x1F57C0
 * Purpose: Same for rotation XF into SOPHYS+0x90
 * Info: proposed name SetSoCnstrTorque (T1); TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void SetSoCnstrTorque(SO *pso, CONSTR *pconstr)
{
    XF *pxf;
    if (pso->psophys != 0) {
        pxf = pso->pxfUnk5c;
        if ((pxf->pactFirst == 0) && (pxf->nRole != 2)) {
            func_002303A0(pso, pxf, 1);
        }
        func_001F5510(pso, pxf, pconstr, &pso->psophys->constrTorque);
        func_001F55C8(pso);
        if (pso->psophys->constrTorque.nKind != 3) {
            func_001F8668(pso, 0);
        }
    }
}

// 0x1F5860 GetSoDvGravity
namespace M_GetSoDvGravity { extern "C" {
struct T; struct S;
typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct T { char pad[0x80]; qword v; };
struct S { char pad[0xE8]; struct T *pt; };
/** @sly2-annot addr=0x1F5860
 * Purpose: SOPHYS+0x80 (gravity vector) or zero
 * Info: proposed name GetSoDvGravity (T1); TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
qword GetSoDvGravity(struct S *p) { if (!p->pt) return D_002A2DD0; return p->pt->v; }
} } // namespace M_GetSoDvGravity

// 0x1F5880 SetSoDvGravity
namespace M_SetSoDvGravity { extern "C" {
struct O; struct E; struct P;
typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x90]; int n90; char pad2[4]; short k; };
struct E { char pad[0x80]; qword q; };
struct P { char pad[0x58]; struct O *po; char pad2[0xE8 - 0x5C]; struct E *pe; };
void func_002303A0(struct P *, struct O *, int);
void func_001F5078(struct P *);
/** @sly2-annot addr=0x1F5880
 * Purpose: Make pos XF dynamic if needed and store gravity vector, invalidate
 * Info: proposed name SetSoDvGravity (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void SetSoDvGravity(struct P *p, qword q)
{
    if (p->pe) {
        struct O *po = p->po;
        if (po->n90 == 0 && po->k != 2) func_002303A0(p, po, 1);
        p->pe->q = q;
        func_001F5078(p);
    }
}
} } // namespace M_SetSoDvGravity

// 0x1F58F0 GetSoCpdefi
namespace M_GetSoCpdefi { extern "C" {
struct V; struct O; struct P; struct Out; 
typedef int qword __attribute__((mode(TI)));
struct V { float x, y, z, w; float pad[8]; };
struct O { char pad[0x40]; char sub[0x30]; qword q70; };
struct P { char pad[0x60]; struct O *po; };
struct Out { int n0; int pad[3]; qword q10; qword q20; float g30; float g34; float g38; int n3C; };
extern struct CLOCK D_002DDED0;
qword GetXfVelocity(struct O *, int, qword);
void TiltMatUpright(void *, int, struct V *);
float atan2f(float, float);
/** @sly2-annot addr=0x1F58F0
 * Purpose: SO vt50: camera default info: base pos/velocity, radHome = atan2(-m01,-m00) of upright matrix,
 *   sAdjust 100, grfcpd 0, tMoveLast = g_clock.t
 * Info: proposed name GetSoCpdefi (T1); class SO slot50; TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void GetSoCpdefi(struct P *p, struct Out *po)
{
    struct O *o = p->po;
    struct V v;
    void *ps;
    qword q = o->q70;
    po->q10 = q;
    ps = o->sub;
    po->q20 = GetXfVelocity(o, 0, q);
    TiltMatUpright(ps, 0, &v);
    po->g30 = atan2f(-v.y, -v.x); po->g34 = 100.0f; po->n0 = 0; po->g38 = D_002DDED0.t; po->n3C = 0;
}
} } // namespace M_GetSoCpdefi

// 0x1F5988 func_001F5988
/** @sly2-annot addr=0x1F5988
 * Purpose: SO vt54 default no-op (contact notify)
 * Info: proposed name SO_vt54_Nop (T2); class SO slot54; TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F5988(void)
{
}

// 0x1F5990 func_001F5990
/** @sly2-annot addr=0x1F5990
 * Purpose: SO vt60 default no-op
 * Info: proposed name SO_vt60_Nop (T2); class SO slot60; TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F5990(void)
{
}

// 0x1F5998 func_001F5998
/** @sly2-annot addr=0x1F5998
 * Purpose: SO vt63 default: return 0
 * Info: proposed name SO_vt63_Ret0 (T2); class SO slot63; TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" int func_001F5998(void)
{
    return 0;
}

// 0x1F59A0 func_001F59A0
namespace M_func_001F59A0 {
union U; struct M3; struct OWNER; struct P; struct COEF; 
// func_001F59A0: angular braking and bounded angular-velocity correction.
// coef a = top angular speed, (a-b)/c = angular acceleration, a*a/(2*acceleration) = stopping angle.
// Caps an overspeed, turns the owner toward `orientation` (expressed in its parent space) with a desired
// speed taken from the predicted remaining angle, and adds a force change bounded by acceleration*dt.
// Matched from the codex/lilly drafts (T1623). Notes: notes/1F0000.md.
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct M3 {U rows[3];M3(){} M3(const M3&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];} M3&operator=(const M3&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];return *this;}};
struct SPACE;
struct OWNER {M3 matrix;char pad30[0x70];SPACE *parent;char padA4[0xC];qword velocity;char padC0[0x30];qword force;};
struct P {char pad5C[0x5C];OWNER *owner;};
struct COEF {float a,b,c;};
struct CLOCK {float time,dt;};
extern "C" { extern CLOCK D_002DDED0; }
extern "C" M3 *ConvertXfMat(M3 *,SPACE *,SPACE *,const M3 *);
static inline M3 BuildFrame(SPACE*parent,const M3*orientation) return result {ConvertXfMat(&result,0,parent,orientation);}
extern "C" void NormalizeRotateMatrix3__FP7MATRIX3(M3 *);
extern "C" void MulMatrix3Matrix3(M3 *,const M3 *,const M3 *);
extern "C" void DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(const M3 *,float *,qword *);
extern "C" float GLimitAbs__Fff(float,float);
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Add(qword a,qword b){__asm__("vadd.xyzw %0,%0,%1":"+j"(a):"j"(b));return a;}
// volatile store: the game's later loads (dt, velocity, the epilogue) wait for these sqc2s
static inline void StoreQ(qword v,U *out){__asm__ __volatile__("sqc2 %1,%0":"=m"(out->q):"j"(v));}
static inline qword Scalar(float f,U *out){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));__asm__("sqc2 %1,%0":"=m"(out->q):"j"(q));return q;}
static inline qword Scale(qword source,qword k){qword v=source;__asm__("vmulx.xyzw %0,%0,%1x":"+j"(v):"j"(k));return v;}
static inline qword ScaleFactor(qword v,qword k){__asm__("vmulx.xyzw %0,%1,%0x":"+j"(k):"j"(v));return k;}
static inline qword ScaleNew(qword v,qword k){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=&j"(r):"j"(v),"j"(k));return r;}
static inline qword AddScaled(qword a,qword b,qword k){__asm__("vadda.xyzw ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x":"+j"(a):"j"(b),"j"(k));return a;}
static inline qword DotQ(qword value,qword normal){qword d=value,b=normal,one;__asm__("vmul.xyz %0,%0,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(d),"=&j"(one):"j"(b));return d;}
// x of a qword: the compiler emits the qmfc2, the asm the mtc1 (an asm qmfc2 issues it before the 0.5 load)
static inline float GetX(qword q){float f;__asm__("mtc1 %1,%0":"=f"(f):"r"(q));return f;}
static inline float Length(qword a){qword d,one;float f;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(a));__asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=j"(d):"0"(d));__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(d));return f;}

/** @sly2-annot addr=0x1F59A0
 * Purpose: Angular braking + bounded angular-velocity correction toward an orientation (coef top speed /
 *   accel)
 * Info: proposed name AccelSoTowardMatBounded (T2); TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F59A0(P *p,const M3 *orientation,const COEF *c)
{
    M3 frame,relative;
    U axis,wanted,change,predicted;
    float angle;
    float maximum=c->a;
    float acceleration=(maximum-c->b)/c->c;
    float square=maximum*maximum;
    float stopAngle;
    OWNER *owner=p->owner;
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
    desired=GLimitAbs__Fff((angle/stopAngle)*maximum,maximum);
    speed=GetX(DotQ(owner->velocity,axis.q));
    clock=&D_002DDED0;
    float half=clock->dt*0.5f;
    remaining=angle-(speed+desired)*half;
    if(remaining<stopAngle)desired=GLimitAbs__Fff((remaining/stopAngle)*maximum,maximum);
    U scalar;
    qword w;
    {qword k=Scalar(desired,&scalar);w=Scale(axis.q,k);StoreQ(w,&wanted);}
    {qword v=owner->velocity;qword k=Scalar(clock->dt,&scalar);predicted.q=AddScaled(v,owner->force,k);}
    qword cv=Sub(w,predicted.q);
    change.q=cv;
    length=Length(cv);
    if(length<acceleration*clock->dt)gain=1.0f/clock->dt;
    else gain=acceleration/length;
    StoreQ(ScaleNew(cv,Scalar(gain,&scalar)),&change);
    owner->force=Add(owner->force,change.q);
}

typedef char check_matrix_size[(sizeof(M3)==0x30)?1:-1];
typedef char check_owner_parent[((unsigned)&((OWNER*)0)->parent==0xA0)?1:-1];
typedef char check_owner_velocity[((unsigned)&((OWNER*)0)->velocity==0xB0)?1:-1];
typedef char check_owner_force[((unsigned)&((OWNER*)0)->force==0xF0)?1:-1];
typedef char check_object_owner[((unsigned)&((P*)0)->owner==0x5C)?1:-1];
typedef char check_clock_dt[((unsigned)&((CLOCK*)0)->dt==4)?1:-1];
} // namespace M_func_001F59A0

// 0x1F5C30 AccelSoTowardMatSpring
namespace M_AccelSoTowardMatSpring {
union U; struct M3; struct OWNER; struct P; struct COEF; 
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct M3 {U rows[3];M3(){} M3(const M3&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];} M3&operator=(const M3&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];return *this;}};
struct OWNER {M3 matrix;char pad30[0x70];OWNER *parent;char padA4[0xC];qword velocity;char padC0[0x30];qword force;};
struct P {char pad5C[0x5C];OWNER *owner;};
struct COEF {float a,b,c;};
struct CLOCK {float time,dt;};
extern "C" { extern CLOCK D_002DDED0; }
extern "C" M3 ConvertXfMat(OWNER *,OWNER *,const M3 *);
extern "C" void NormalizeRotateMatrix3__FP7MATRIX3(M3 *);
extern "C" M3 MulMatrix3Matrix3(const M3 *,const M3 *);
extern "C" void DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(const M3 *,float *,qword *);
extern "C" qword ConvertXfVec(OWNER *,OWNER *,qword);
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline float Length(qword a){qword d,one;float f;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a));
__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(d):"0"(d));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(d));return f;}
static inline qword Scalar(float f){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));return q;}
static inline qword AddScaled(qword old,qword v,qword s){__asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x":"+j"(old):"j"(v),"j"(s));return old;}
/** @sly2-annot addr=0x1F5C30
 * Purpose: Spring torque toward orientation (quadratic CLQ on angle) and toward target angular velocity
 *   (clamped 1/dt)
 * Info: proposed name AccelSoTowardMatSpring (T1); TU so
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void AccelSoTowardMatSpring(P*p,const M3*orientation,const COEF*c,qword velocity,const COEF*d)
{
    OWNER *owner=p->owner;
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
            qword k=Scalar(gain);
            {qword result=AddScaled(owner->force,axis.q,k);scalar.q=k;owner->force=result;}
        }
    }
    if(d){
        qword transformed=ConvertXfVec(0,owner->parent,velocity);
        qword direction=Sub(transformed,owner->velocity);
        float length=Length(direction);
        if(0.0001f<length){
            float gain=(d->c*length+d->b)+d->a/length;
            float limit=1.0f/D_002DDED0.dt;
            U scalar;
            qword k;
            if(limit<gain)gain=limit;
            k=Scalar(gain);
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
struct CLOCK {float time,dt;};
extern struct CLOCK D_002DDED0;
qword func_001A62E8(struct S*);
qword ConvertXfVec(void*,void*,qword);
static inline qword Sub(qword x,qword y){register qword a __asm__("vf1")=x,b __asm__("vf2")=y,r __asm__("vf3");__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline float Length(qword value){register qword a __asm__("vf3")=value,r __asm__("vf1"),one __asm__("vf2");float f;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(r),"=&j"(one):"j"(a));
__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(r):"0"(r));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(r));return f;}
static inline qword LoadVector(const V *value){register qword result asm("v0");__asm__("lq %0,%1":"=r"(result):"m"(value->q));return result;}
static inline qword Scalar(float f){register qword r __asm__("$2");__asm__("mfc1 %0,%1":"=r"(r):"f"(f));return r;}
static inline qword AddScaled(qword old,qword direction,qword factor){register qword a __asm__("vf2")=old,b __asm__("vf4")=direction,scale __asm__("vf3")=factor,r __asm__("vf1");
__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=&j"(r):"j"(a),"j"(b),"j"(scale));return r;}
/** @sly2-annot addr=0x1F5E10
 * Purpose: Spring force toward position (CLQ on distance) and toward target velocity (clamped 1/dt)
 * Info: proposed name AccelSoTowardPosSpring (T1); TU so
 * Notes: names_v2 C proposed ActivateCptn__FP4CPTNPv (0.170)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void AccelSoTowardPosSpring(struct S*input,qword position,struct COEF*positionCoefficients,qword velocity,struct COEF*velocityCoefficients)
{
 V argument,point,old1,scale1,speed,old2,scale2;
 register struct S *p __asm__("$17")=input;
 register struct COEF *c __asm__("$16")=positionCoefficients,*d __asm__("$18")=velocityCoefficients;
 register struct OWNER *owner __asm__("$19");
 point.q=position;owner=p->owner;speed.q=velocity;
 if(c){
   qword difference=Sub(point.q,func_001A62E8(p));
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
     float limit=1.0f/D_002DDED0.dt;
     if(limit<value)value=limit;
     scale2.q=Scalar(value);
     {register qword oldValue asm("v0")=owner->force;old2.q=oldValue;}parent=owner->parent;argument.q=LoadVector(&scale2);
     owner->force=AddScaled(old2.q,ConvertXfVec(0,parent,difference),scale2.q);
   }
 }
}
} } // namespace M_AccelSoTowardPosSpring

// 0x1F5FF8 func_001F5FF8
namespace M_func_001F5FF8 { extern "C" {
struct E; struct S;
struct E { int oid; char pad[0x24]; };
struct S { char pad[0xB8]; int c; int pad2; struct E *ae; };
/** @sly2-annot addr=0x1F5FF8
 * Purpose: Find glob (globset +0xB8 array, 0x28 stride) by oid
 * Info: proposed name PglobFindSo (T2); TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct E *func_001F5FF8(struct S *p, int oid)
{
    int i;
    for (i = 0; i < p->c; i++) { struct E *pe = &p->ae[i]; if (pe->oid == oid) return pe; }
    return 0;
}
} } // namespace M_func_001F5FF8

// 0x1F6030 PsopartFindSo
namespace M_PsopartFindSo { extern "C" {
struct E; struct S;
struct E { int a[3]; int oid; char pad[0xA0]; };
struct S { char pad[0xE0]; int c; struct E *ae; };
/** @sly2-annot addr=0x1F6030
 * Purpose: Find SOPART by oid (+0x0C)
 * Info: proposed name PsopartFindSo (T1); TU so
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct E *PsopartFindSo(struct S *p, int oid)
{
    int i;
    for (i = 0; i < p->c; i++) { struct E *pe = &p->ae[i]; if (pe->oid == oid) return pe; }
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


inline int FIsLoInWorld(LO *plo)
{
    return plo->fInWorld != 0;
}










extern XF D_002C44F0;
void AddSoParts(SO *pso) __asm__("AddSoParts");
void CloneGlobset(GLOBSET *pglobset, SO *pso, LO *ploBase) __asm__("func_0016EA80");
void CloneBspc(BSPC *pbspcBase, GEOM *pgeomBase, BSPC *pbspc, GEOM *pgeom) __asm__("func_001191E8");
inline void *PvAllocSwCopy(int cb, void *pvBase)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = 0x10;
    return PvAllocSwCopyImpl(pswalloc, pvBase);
}
inline void *PvAllocStack(int cb)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = 0x10;
    return PvAllocStackImpl(pswalloc);
}
void SO::Clone(LO *ploBase)
{
    SO *psoBase = (SO *)ploBase;
    SOTREE *ptreeSave = ptree;
    LO *ploRemovedFirstSave = ploRemovedFirst;
    DL dlChildSave = dlChild;
    int ixf = 0;
    LO::Clone(ploBase);
    ptree = ptreeSave;
    dlChild = dlChildSave;
    ploRemovedFirst = ploRemovedFirstSave;
    psobounds = 0;
    axf = (XF *)PvAllocSwCopy(cxf * sizeof(XF), axf);
    for (ixf = 0; ixf < cxf; ixf++)
    {
        XF *pxf = &axf[ixf];
        CloneXfRef(ploBase, &pxf->pxfParent);
        switch (pxf->nRole)
        {
        case 4:
            CloneXfRef(ploBase, &pxf->pxfRef);
            break;
        case 13:
        {
            IKLIMB *plimb = (IKLIMB *)PvAllocSwCopy(sizeof(IKLIMB), pxf->pxfRef);
            CloneXfRef(ploBase, &plimb->pxfGoal);
            CloneXfRef(ploBase, &plimb->pxfS);
            CloneXfRef(ploBase, &plimb->pxfE);
            CloneXfRef(ploBase, &plimb->pxfEnd);
            plimb->pxfS->pxfRef = (XF *)plimb;
            plimb->pxfE->pxfRef = (XF *)plimb;
            break;
        }
        case 15:
        {
            XFLINK *pxflink = (XFLINK *)PvAllocSwCopy(sizeof(XFLINK), pxf->pxfRef);
            pxf->pxfRef = (XF *)pxflink;
            if (pxflink->pxfFrom != &D_002C44F0)
            {
                CloneXfRef(ploBase, &pxflink->pxfFrom);
            }
            CloneXfRef(ploBase, &pxflink->pxfTo);
            break;
        }
        case 16:
        {
            CHAINLINK *plink = (CHAINLINK *)pxf->pxfRef;
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
                    plinkNew->pxf->pxfRef = (XF *)plinkNew;
                }
            }
            break;
        }
        }
        if (pxf->pactFirst)
        {
            int cact = 0;
            ACT *pact = pxf->pactFirst;
            do
            {
                pact = pact->pactNext;
                cact++;
            } while (pact);
            STACKSCOPE scope;
            ACT **apact = (ACT **)PvAllocStack(cact * sizeof(ACT *));
            int iact = 0;
            for (pact = pxf->pactFirst; pact; pact = pact->pactNext)
            {
                apact[iact++] = pact;
            }
            pxf->pactFirst = 0;
            for (iact = cact - 1; iact >= 0; iact--)
            {
                ACT *pactNew = PactNewClone(apact[iact], this, pxf);
                AddXfAct(this, pxf, pactNew);
            }
        }
    }
    CloneXfRef(ploBase, &pxfUnk58);
    CloneXfRef(ploBase, &pxfUnk5c);
    CloneXfRef(ploBase, &pxfRoot);
    CloneGlobset(&globset, this, ploBase);
    int cpartExtra = CpartExtra();
    apart = (SOPART *)PvAllocSwCopy((cpart + cpartExtra) * sizeof(SOPART), apart);
    for (int ipart = 0; ipart < cpart; ipart++)
    {
        SOPART *ppart = &apart[ipart];
        CloneXfRef(ploBase, &ppart->pxf);
        ppart->pso = this;
        ClearDle(&ppart->dle);
        GEOM geom;
        CloneGeom(&ppart->geomWorld, 0, &geom);
        ppart->geomWorld = geom;
        SOPART *ppartBase = &psoBase->apart[ipart];
        CloneBspc(&ppartBase->bspc, &ppartBase->geomWorld, &ppart->bspc, &ppart->geomWorld);
        if (ppart->plvo)
        {
            int cb = sizeof(LVO);
            ppart->plvo = (LVO *)PvAllocSwCopy(cb, ppart->plvo);
            ppart->plvo->mpiposvLocal = (VECTOR *)PvAllocSwCopy(ppart->geomLocal.cpos * 0x10, ppart->plvo->mpiposvLocal);
            ppart->plvo->mpiposdvLocal = (VECTOR *)PvAllocSwCopy(ppart->geomLocal.cpos * 0x10, ppart->plvo->mpiposdvLocal);
            ppart->plvo->mpisurfsurfx = (SURFX *)PvAllocSwCopy(ppart->geomLocal.csurf * 0xC, ppart->plvo->mpisurfsurfx);
        }
    }
    if (psophys)
    {
        psophys = (SOPHYS *)PvAllocSwCopy(sizeof(SOPHYS), psophys);
        if (psophys->pregent)
        {
            psophys->pregent = (REGENT *)PvAllocSwCopy(sizeof(REGENT), psophys->pregent);
            psophys->pregent->pso = this;
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
struct L { char pad[0x24]; struct L *next; ulong fl; char pad2[0x64 - 0x30]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; };
void PloCloneLo__FP2LOP2SO(struct L *, struct A *);
/** @sly2-annot addr=0x1F66F0
 * Purpose: SO vt40: vt41 Clone then PloCloneLo each non-removed child
 * Info: proposed name CloneSoHierarchy (T1); class SO slot40; TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void CloneSoHierarchy(struct A *pa, struct L *p)
{
    struct L *q;
    pa->pvt->a[41].pfn((char *)pa + pa->pvt->a[41].d);
    for (q = p->first; q != p->end; q = q->next)
        if (!(q->fl & (1L << 57))) PloCloneLo__FP2LOP2SO(q, pa);
}
} } // namespace M_CloneSoHierarchy

// 0x1F6788 func_001F6788
namespace M_func_001F6788 { extern "C" {
struct B; struct N;
struct B { char pad[0x18]; int n; };
struct N { char pad[0x14]; struct B *pb; int n; char pad2[8]; struct N *next; };
/** @sly2-annot addr=0x1F6788
 * Purpose: Search list (*(struct N * *)&g_sw.dlAsega) for node with n==oid and (m==-1 or owner->n==m)
 * Info: proposed name PsmaFindByOidAndSm (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct N *func_001F6788(int n, int m)
{
    struct N *p;
    int unused[4];
    for (p = (*(struct N * *)&g_sw.dlAsega); p; p = p->next) {
        if (p->n == n) {
            if (m == -1 || p->pb->n == m)
                return p;
        }
    }
    return 0;
}
} } // namespace M_func_001F6788

// 0x1F67E0 func_001F67E0
namespace M_func_001F67E0 { extern "C" {
struct B; struct N;
struct B { char pad[0x18]; int n; };
struct N { char pad[0x1C]; struct N *next; int pad2; struct B *pb; int n; };
/** @sly2-annot addr=0x1F67E0
 * Purpose: Same search over (*(struct N * *)&g_sw.dlSma) (SMA live list per sm.md)
 * Info: proposed name PsmaFindLive (T2); TU so
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct N *func_001F67E0(int n, int m)
{
    struct N *p;
    int unused[4];
    for (p = (*(struct N * *)&g_sw.dlSma); p; p = p->next) {
        if (p->n == n) {
            if (m == -1 || p->pb->n == m)
                return p;
        }
    }
    return 0;
}
} } // namespace M_func_001F67E0

// 0x1F6838 func_001F6838
namespace M_func_001F6838 { extern "C" {
struct T; struct S;
struct T { char pad[0x70]; ulong fl; };
struct S { char pad[0xEC]; struct T *pt; };
/** @sly2-annot addr=0x1F6838
 * Purpose: Bit 2 of tree flags (+0x70), 1 if no tree
 * Info: proposed name FSoTreeFlag2 (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F6838(struct S *p) { if (!p->pt) return 1; return (int)((long)(p->pt->fl << 30) >> 32) & 1; }
} } // namespace M_func_001F6838

// 0x1F6860 func_001F6860
/** @sly2-annot addr=0x1F6860
 * Purpose: Set tree flags bit 2 then FreeSoContacts
 * Info: proposed name SetSoTreeFlag2 (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F6860(SO *pso, int f)
{
    SOTREE *ptree;
    ptree = pso->ptree;
    if (ptree != 0) {
        ptree->fNoUpdate = f;
        func_001F4E28(pso);
    }
}

// 0x1F68A8 func_001F68A8
namespace M_func_001F68A8 { extern "C" {
struct E; struct P;
typedef unsigned long ulong;
struct E { char pad[0x10]; ulong lo : 48; ulong b48 : 1; ulong hi : 15; char pad2[0xB0 - 0x18]; };
struct P { char pad[0xE0]; int c; struct E *ae; };
/** @sly2-annot addr=0x1F68A8
 * Purpose: Set bit48 of every SOPART flag word
 * Info: proposed name SetSoPartsFlag48 (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F68A8(struct P *p, int f)
{
    int i;
    for (i = 0; i < p->c; i++) p->ae[i].b48 = f;
}
} } // namespace M_func_001F68A8

// 0x1F6918 func_001F6918
namespace M_func_001F6918 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong a:26; int b:1; ulong c:37; };
struct G;
extern struct G D_003DD490;
void RecalcSwOxfFilterForObject(struct G *, struct S *);
/** @sly2-annot addr=0x1F6918
 * Purpose: Set +0x148 bit26 and refresh contacts (0x134320)
 * Info: proposed name SetSoFlag26 (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F6918(struct S *p, int f) { p->b = f; RecalcSwOxfFilterForObject(&D_003DD490, p); }
} } // namespace M_func_001F6918

// 0x1F6960 func_001F6960
namespace M_func_001F6960 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong fl; };
/** @sly2-annot addr=0x1F6960
 * Purpose: Get +0x148 bit26
 * Info: proposed name GetSoFlag26 (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F6960(struct S *p) { return (int)((long)(p->fl << 6) >> 32) & 1; }
} } // namespace M_func_001F6960

// 0x1F6978 ShowSoGlobsById
namespace M_ShowSoGlobsById { extern "C" {
struct Q; struct E; struct P;
struct Q { char pad[0x10]; ulong fl; };
struct E { int k; int pad[2]; int fl; int pad2[5]; struct Q *pq; };
struct P { char pad[0xB8]; int c; int pad2; struct E *ae; };
/** @sly2-annot addr=0x1F6978
 * Purpose: For every glob with id k: clear hidden flag 0x800 and clear the linked SOPART hide bit 50
 * Info: proposed name ShowSoGlobsById (T1); TU so
 * Notes: top50_round (0.60): Body; exact inverse of HideSoGlobsById 0x1F69F8. Splice option HideSoPartsById*
 *   family and CID98 stage actions AddCid98ShowGlobs use it
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: r6_1D5150+top50_round
 * See: docs/engine/animation.md#open-questions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void ShowSoGlobsById(struct P *p, int k)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (pe->k == k) {
            pe->fl &= ~0x800;
            if (pe->pq) pe->pq->fl &= ~(1L << 50);
        }
    }
}
} } // namespace M_ShowSoGlobsById

// 0x1F69F8 HideSoGlobsById
namespace M_HideSoGlobsById { extern "C" {
struct Q; struct E; struct P;
struct Q { char pad[0x10]; ulong fl; };
struct E { int k; int pad[2]; int fl; int pad2[5]; struct Q *pq; };
struct P { char pad[0xB8]; int c; int pad2; struct E *ae; };
/** @sly2-annot addr=0x1F69F8
 * Purpose: For every glob with id k: set hidden flag 0x800 and set the linked SOPART hide bit 50
 * Info: proposed name HideSoGlobsById (T1); TU so
 * Notes: top50_round (0.60): Body; inverse of ShowSoGlobsById. Callers OnBentleyGadgetsChanged,
 *   BindLoOidTable, CID98 stages
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: r6_1D5150+top50_round
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void HideSoGlobsById(struct P *p, int k)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (pe->k == k) {
            pe->fl |= 0x800;
            if (pe->pq) pe->pq->fl |= (1L << 50);
        }
    }
}
} } // namespace M_HideSoGlobsById

// 0x1F6A60: dead-code scrap, a nop unit (sly2/scrap.h)
SLY2_SCRAP("nop");

// 0x1F6A68 func_001F6A68
namespace M_func_001F6A68 { extern "C" {
struct C; struct E; struct A;
typedef unsigned long ulong;
struct C { char pad[0x10]; ulong fl; };
struct E { char pad[0xC]; int fl; char pad2[0x14]; struct C *pc; };
struct A { char pad[0xB8]; int c; int pad2; struct E *ae; };
/** @sly2-annot addr=0x1F6A68
 * Purpose: Hide every glob and its linked part
 * Info: proposed name HideSoAllGlobs (T2); TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F6A68(struct A *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        pe->fl |= 0x800;
        if (pe->pc)
            pe->pc->fl |= 1L << 50;
    }
}
} } // namespace M_func_001F6A68

// 0x1F6AC8 ShowSoPartsById
namespace M_ShowSoPartsById { extern "C" {
struct E; struct P;
struct E { int pad[3]; int k; char pad2[0xB0 - 0x10]; };
struct P { char pad[0xE0]; int c; struct E *ae; };
void ShowSoPart(struct P *, struct E *);
/** @sly2-annot addr=0x1F6AC8
 * Purpose: ShowSoPart for parts whose id (+0xC) == k
 * Info: proposed name ShowSoPartsById (T1); TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void ShowSoPartsById(struct P *p, int k)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i]; if (pe->k == k) ShowSoPart(p, pe);
    }
}
} } // namespace M_ShowSoPartsById

// 0x1F6B48 func_001F6B48
namespace M_func_001F6B48 { extern "C" {
struct E; struct P;
struct E { int pad[3]; int k; char pad2[0xB0 - 0x10]; };
struct P { char pad[0xE0]; int c; struct E *ae; };
void HideSoPart(struct P *, struct E *);
/** @sly2-annot addr=0x1F6B48
 * Purpose: HideSoPart for parts whose id == k
 * Info: proposed name HideSoPartsById (T2); TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/animation.md#open-questions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_001F6B48(struct P *p, int k)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i]; if (pe->k == k) HideSoPart(p, pe);
    }
}
} } // namespace M_func_001F6B48

// 0x1F6BC8 func_001F6BC8
namespace M_func_001F6BC8 { extern "C" {
struct E; struct S;
struct E { int a[3]; int n; int b[2]; int x18; char pad[0x94]; };
struct S { char pad[0xE0]; int c; struct E *ae; };
/** @sly2-annot addr=0x1F6BC8
 * Purpose: Set SOPART +0x18 (grfzpk) for parts with id n
 * Info: proposed name SetSoPartsField18ById (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F6BC8(struct S *p, int n, int x) { int i; for (i = 0; i < p->c; i++) { struct E *pe = &p->ae[i]; if (pe->n == n) pe->x18 = x; } }
} } // namespace M_func_001F6BC8

// 0x1F6C08 func_001F6C08
namespace M_func_001F6C08 {
union U; struct TRI; struct MESH; struct EDGE; struct PLANE; struct P; struct FRAME; struct MSG;
// func_001F6C08 (0x1F6C08): build the scratch mesh of P once (p->mesh at +0x10).
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
struct FRAME {FRAME(){}~FRAME(){}};
struct MSG {int bytes,alignment;};
extern "C" void *func_001AE1D0(MSG *);
extern "C" void *func_001AE2A0(MSG *);
extern "C" void *func_001AE378(MSG *);
static inline qword Sub(qword a,qword b) {qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"0"(a),"j"(b));return r;}
static inline qword Cross(qword a,qword b){qword r;__asm__("vopmula.xyz $ACC,%1,%2\n\tvopmsub.xyz %0,%2,%1":"=j"(r):"0"(a),"j"(b));return r;}
static inline float Dot(qword a,qword b){qword r,one;float f;
__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(r),"=&j"(one):"0"(a),"j"(b));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(r));return f;}
/** @sly2-annot addr=0x1F6C08
 * Purpose: Build a soft-mesh scratch record (velocities, accelerations, oriented triangles) for a part once
 * Info: proposed name EnsureSoPartMesh (T2); TU so
 * Notes: names_v2 v1 proposed DisplaceSo__FP2SOi (0.034)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/engine/physics-collision.md#1-so-the-solid-object-socpp-0x1f2108-0x1f8dd8-174-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F6C08(P *p)
{
    if(p->mesh)return;
    U first,second,cross;
    FRAME storage;
    MSG request;
    int triBytes=sizeof(TRI),alignment=16;
    STACKSCOPE_ctor(&storage);
    MSG *allocation=&request;
    request.bytes=triBytes;allocation->alignment=alignment;
    p->mesh=(MESH *)func_001AE1D0(allocation);
    request.bytes=p->pointCount*16;allocation->alignment=alignment;
    p->mesh->velocity=(qword *)func_001AE2A0(allocation);
    request.bytes=p->pointCount*16;allocation->alignment=alignment;
    p->mesh->acceleration=(qword *)func_001AE2A0(allocation);
    request.bytes=p->triCount*triBytes;allocation->alignment=alignment;
    p->mesh->triangles=(TRI *)func_001AE2A0(allocation);
    request.bytes=p->triCount*4;allocation->alignment=alignment;
    int *counts=(int *)func_001AE378(allocation);
    int itri;
    TRI *ptri;
    for(int i=0;i<p->edgeCount;i++) {
        EDGE *edge=&p->edges[i];
        for(int side=0;side<2;side++) {
            itri=edge->face[side];
            ptri=&p->mesh->triangles[itri];
            for(int endpoint=0;endpoint<2;endpoint++) {
                int vertex=edge->vertex[endpoint];
                int j;
                for(j=0;j<counts[itri];j++)if(vertex==ptri->vertex[j])break;
                if(j<counts[itri])continue;
                if(j<3){ptri->vertex[counts[itri]]=vertex;counts[itri]++;}
            }
        }
    }
    U *positions=p->positions;
    for(itri=0;itri<p->triCount;itri++) {
        ptri=&p->mesh->triangles[itri];
        PLANE *plane=&p->planes[itri];
        first.q=Sub(positions[ptri->vertex[1]].q,positions[ptri->vertex[0]].q);
        second.q=Sub(positions[ptri->vertex[2]].q,positions[ptri->vertex[0]].q);
        cross.q=Cross(first.q,second.q);
        if(Dot(cross.q,plane->normal.q)<0.0f){int t=ptri->vertex[1];ptri->vertex[1]=ptri->vertex[2];ptri->vertex[2]=t;}
    }
    STACKSCOPE_ctor_1AE4A0(&storage,2);
}
} // namespace M_func_001F6C08

// 0x1F6EF8 func_001F6EF8
namespace M_func_001F6EF8 { extern "C" {
struct TRI; struct MESH; struct P;
typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f;} V;
struct TRI {int index[3];};
struct MESH {qword *velocity,*acceleration;struct TRI *triangles;};
struct P {void *x0;int x4;float maximum;int xC;struct MESH*mesh;char pad14[0x64];int count;qword *positions;int triCount;void *planes;char pad88[0x20];int state;};
void InitPlaneFromPositions(void*,qword,qword,qword);
static inline qword Scalar(float f){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));return q;}
static inline qword Step(qword a,qword b,qword s){register qword r __asm__("vf5");
__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=&j"(r):"j"(a),"j"(b),"j"(s));return r;}
static inline qword Position(qword p,qword old,qword next,qword half,V *scalar){register qword r __asm__("vf3");
__asm__("sqc2 %5,%1\n\tvadda.xyzw $ACC,%2,$vf0\n\tvmaddax.xyzw $ACC,%3,%5x\n\tvmaddx.xyzw %0,%4,%5x":"=&j"(r),"=m"(scalar->q):"j"(p),"j"(old),"j"(next),"j"(half));return r;}
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline float LengthSquared(qword q){qword r,one;float f;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(r),"=&j"(one):"j"(q));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(r));return f;}
static inline float Root(float x){float r;__asm__ __volatile__("sqrt.s %0,%1":"=f"(r):"f"(x));return r;}
static inline qword *VelocityCursor(qword *p){register qword *initial __asm__("$2")=p;register qword *r __asm__("$4");__asm__("daddu %0,%1,$zero":"=r"(r):"r"(initial));return r;}
static inline int PlaneLess(int a,int b){register int r __asm__("$2");__asm__("slt %0,%1,%2":"=r"(r):"r"(a),"r"(b));return r;}
/** @sly2-annot addr=0x1F6EF8
 * Purpose: Integrate soft-mesh vertices by dt (semi-implicit), track max radius, invalidate on large moves
 * Info: proposed name UpdateSoPartMesh (T2); TU so
 * Notes: names_v2 v1 proposed ImpactSo__FP2SOi (0.167)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F6EF8(struct P*p,float dt)
{
 V scalar;
 register int vertexIndex __asm__("a2")=0;
 qword *velocity=p->mesh->velocity;
 register int initialCount __asm__("$3");
 register qword *acceleration __asm__("$5")=({register qword *r __asm__("$5");__asm__("lw %0,%2\n\tlw %1,%3":"=&r"(initialCount),"=r"(r):"m"(p->count),"m"(p->mesh->acceleration));r;});
 register qword *positions __asm__("s4");
 float maximum=0.0f;
 positions=p->positions;
 if(initialCount>0){
   register qword time __asm__("vf7")=Scalar(dt);
   register int invalid __asm__("$7")=-1;
   register qword half __asm__("vf6")=Scalar(dt*0.5f);
   register qword *position __asm__("$3")=positions;
   register qword *velocityCursor __asm__("$4")=VelocityCursor(velocity);
   register int limit __asm__("$2");
   do{
     register qword old __asm__("vf1")=*velocityCursor,force __asm__("vf2"),point __asm__("vf4")=*position;
     int state;
     qword next,newpoint;
     float square;
     __asm__("sqc2 %4,%2\n\tlqc2 %0,%3\n\tlw %1,%5":"=j"(force),"=r"(state),"=m"(scalar.q):"m"(*acceleration),"j"(time),"m"(p->state));
     next=Step(old,force,time);
     newpoint=Position(point,old,next,half,&scalar);
     if(state!=invalid && 1.0f<LengthSquared(Sub(point,newpoint)))p->state=invalid;
     square=LengthSquared(newpoint);
     *position=newpoint;
     *velocityCursor=next;
     if(maximum<square)maximum=square;
     limit=p->count;
   }while(++velocityCursor,++position,++acceleration,++vertexIndex<limit);
 }
 p->maximum=Root(maximum);
 {
   struct TRI*firstTri=p->mesh->triangles;
   char *firstPlane=(char*)p->planes;
   register int planeIndex __asm__("s3")=0;
   if(p->triCount>0){struct TRI*tri=firstTri;char*plane=firstPlane;do{
     qword a=positions[tri->index[0]],b=positions[tri->index[1]],c=positions[tri->index[2]];
     __asm__("addiu %0,%0,12":"+r"(tri));
     InitPlaneFromPositions(plane,a,b,c);
     plane+=0x20;
   }while(PlaneLess(++planeIndex,p->triCount));}
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

// 0x1F70C0 func_001F70C0
namespace M_func_001F70C0 {
union U; struct M4; struct LO; struct GEO; struct TREE; struct P; struct FRAME; struct MSG;
// Insert zeroed vertices, rebuild transformed geometry, and rebase its tree.
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct M4 {U rows[4];M4(){}M4(const M4&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];rows[3]=m.rows[3];}M4&operator=(const M4&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];rows[3]=m.rows[3];return *this;}};
struct LO {char pad40[0x40];M4 matrix;};
struct GEO {int count;qword *points;int planeCount;void *planes;int x10,x14;};
struct TREE {int count,flag,unused;void *nodes;};
struct P {LO *owner;int flags;float radius;int material;char pad10[0xC];int pinnedCount;char pad20[0x48];TREE tree;GEO local,world;};
struct FRAME {int pad[4];};
extern "C" FRAME *STACKSCOPE_ctor(FRAME *);
extern "C" void STACKSCOPE_ctor_1AE4A0(FRAME *,int) __asm__("_$_10STACKSCOPE");
struct MSG {int bytes,alignment;};
extern "C" void *func_001AE378(MSG *);
extern "C" void CopyAb__FPvT0Ui(void *,const void *,unsigned int);
extern "C" M4 *InvertMatrix4Rigid(M4 *,const M4 *);
extern "C" M4 *__ml__FRC7MATRIX4T0(M4 *,const M4 *,const M4 *);
extern "C" void CloneGeom__FP4GEOMP7MATRIX4T0(GEO *,const M4 *,GEO *);
extern "C" void func_001191E8(TREE *,GEO *,TREE *,GEO *);

static inline float SquaredLength(qword a){qword one;float f;__asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(a),"=&j"(one));__asm__("mtc1 %1,%0":"=f"(f):"r"(a));return f;}
/* volatile: the game's sqrt.s is a barrier for the scheduler (the radius store must precede the next constant). */
static inline float Sqrt(float f){float r;__asm__ volatile("sqrt.s %0,%1":"=f"(r):"f"(f));return r;}
/** @sly2-annot addr=0x1F70C0
 * Purpose: Copy a part's geometry into another space inserting zeroed vertices, rebuild world geom and BSP
 * Info: proposed name CloneSoPartGeomInserting (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F70C0(P *source,LO *space,int inserted,P *dest)
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
    qword *points=(qword *)func_001AE378(allocation);
    int pinned=source->pinnedCount;
    CopyAb__FPvT0Ui(points,temporary.points,pinned*16);
    CopyAb__FPvT0Ui(points+(pinned+inserted),temporary.points+pinned,(temporary.count-pinned)*16);
    temporary.count+=inserted;temporary.points=points;
    {M4 product,inverse;__ml__FRC7MATRIX4T0(&product,(InvertMatrix4Rigid(&inverse,&space->matrix),&inverse),&source->owner->matrix);transform=product;}
    CloneGeom__FP4GEOMP7MATRIX4T0(&temporary,&transform,&dest->local);
    CloneGeom__FP4GEOMP7MATRIX4T0(&dest->local,&space->matrix,&dest->world);
    func_001191E8(&source->tree,&source->world,&dest->tree,&dest->world);
    STACKSCOPE_ctor_1AE4A0(&frame,2);
}
} // namespace M_func_001F70C0

// 0x1F72F8 func_001F72F8
namespace M_func_001F72F8 { extern "C" {
struct O; struct E; struct P;
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct O { char pad[0x70]; qword pos; };
struct E { struct O *po; int pad; float r; int pad2; ulong fl; char pad3[0x74 - 0x18]; void *p74; char pad4[0xB0 - 0x78]; };
struct P { char pad[0xE0]; int c; struct E *ae; };
int PbspPointInBspQuick(void *, int, qword);
/** @sly2-annot addr=0x1F72F8
 * Purpose: SO vt73: true if pos is inside any visible sphere part or part BSP (0x118FB0)
 * Info: proposed name FPosInSo (T2); class SO slot73; TU so
 * Notes: names_v2 v1 proposed ExtendSoBounds__FP2SOP6VECTORf (0.058)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F72F8(struct P *p, qword pos)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (pe->fl & (1L << 50)) continue;
        if (pe->fl & (1L << 49)) {
            qword d, one;
            float r2, d2;
            int t;
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pos), "j"(pe->po->pos));
            r2 = pe->r * pe->r;
            __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "0"(d));
            __asm__("qmfc2.ni %0, %1" : "=r"(t) : "j"(d));
            __asm__("mtc1 %1, %0" : "=f"(d2) : "r"(t));
            { int f = d2 < r2; if (f) return 1; }
        }
        if (pe->p74 && PbspPointInBspQuick(pe->p74, 0x7FFFFFFF, pos)) return 1;
    }
    return 0;
}
} } // namespace M_func_001F72F8

// 0x1F7410 func_001F7410
namespace M_func_001F7410 { extern "C" {
struct O; struct H; struct N; struct E; struct P; struct M;
typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x14]; short s14; short pad2; int n18; };
struct H { struct O *po; int pad; void *p8; void *pC; qword q10; qword q20; qword q30; };
struct N { struct H h[2]; char pad[0xAC - 0x80]; struct N *next; };
struct E { char pad[0x6C]; struct N *pn; };
struct P { char pad[0xEC]; struct E *pe; };
struct M { void *p0; void *p4; void *p8; int nC; int n10; int pad[3]; qword q20; qword q30; char pad2[0x60 - 0x40]; };
struct G;
extern struct G D_003DD490;
void *func_001F7530(void *, int);
void func_002333B4(void *, int, int) __asm__("memset");
int func_001F7598(struct M *);
void FreeSwXpList__FP2SWP2XP(struct G *, void *);
/** @sly2-annot addr=0x1F7410
 * Purpose: SO vt74: for each contact, if other part has zap kinds, build a zap record and InflictZap; free
 *   contact list
 * Info: proposed name InflictSoContactZaps (T2); class SO slot74; TU so
 * Notes: names_v2 v1 proposed UpdateGeomWorld__FP4GEOMT0G9VU_VECTORP7MATRIX3 (0.200)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7410(struct P *p)
{
    struct N *pn;
    for (pn = p->pe->pn; pn; pn = pn->next) {
        int i = pn->h[0].p8 != p;
        struct H *ph = (struct H *)((char *)pn + (i << 6));
        if (ph->po->n18) {
            struct H *pot = (struct H *)((char *)pn + ((1 - i) << 6));
            void *r = func_001F7530(pot->pC, ph->po->n18);
            if (r) {
                struct M m;
                func_002333B4(&m, 0, 0x60);
                m.p0 = p;
                m.p4 = ph->po;
                m.p8 = r;
                m.nC = ph->po->n18;
                m.n10 = ph->po->s14;
                m.q20 = pot->q10;
                m.q30 = ph->q30;
                func_001F7598(&m);
                if (!p->pe) break;
            }
        }
    }
    if (p->pe) {
        FreeSwXpList__FP2SWP2XP(&D_003DD490, p->pe->pn);
        p->pe->pn = 0;
    }
}
} } // namespace M_func_001F7410

// 0x1F7530 func_001F7530
namespace M_func_001F7530 { extern "C" {
struct N;
int GrfzpdFromSoGrfzpk(void *p, void *q);
struct N { char pad[0x10]; struct N *next; };
/** @sly2-annot addr=0x1F7530
 * Purpose: First LO in chain whose zap response for grfzpk is nonzero
 * Info: proposed name PloFindZapResponder (T2); TU so
 * Notes: names_v2 v1 proposed UpdateSoXfWorldHierarchy__FP2SO (0.026)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct N *func_001F7530(struct N *p, void *q)
{
    for (; p; p = p->next) {
        if (GrfzpdFromSoGrfzpk(p, q))
            return p;
    }
    return 0;
}
} } // namespace M_func_001F7530

// 0x1F7588 func_001F7588
/** @sly2-annot addr=0x1F7588
 * Purpose: return arg1->+0x44 != 0
 * Info: proposed name FZprHasField44 (T2); class SO slot75; TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/physics-collision.md#1-so-the-solid-object-socpp-0x1f2108-0x1f8dd8-174-functions,
 *   docs/engine/player.md#2-po-and-poctl-pocpp-0x1c3d70,
 *   docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
extern "C" int func_001F7588(SO *pso, SOWATCH *pwatch)
{
    return pwatch->n44 != 0;
}

// 0x1F7598 func_001F7598
namespace M_func_001F7598 { extern "C" {
struct VT; struct O; struct K; struct N; struct M; struct M2; struct M3;
typedef int qword __attribute__((mode(TI)));
struct O;
struct VT {
    short d; short pad;
    void *pfn;
};
struct O { char pad[0xC]; struct VT *pvt; struct O *pNext; };
struct K {
    struct O *p0; int n4; void *p8; int nC;
    char pad[0x44 - 0x10];
    int n44; struct O *p48; float f4C; char dle[0x10];
} __attribute__((aligned(16)));
struct N { int n0; int n4; int pad8; int nC; char pad[0x48 - 0x10]; struct O *p48; char pad2[0x50 - 0x4C]; struct N *pNext; };
struct M { struct O *po; int n; struct K *pk; };
struct M2 { struct O **ppo; int *pn; };
struct M3 { struct K *pk; };
extern float D_002DDED0;
struct O *PsoFromLo(void *);
int GrfzpdFromSoGrfzpk(struct O *, int);
struct K *PvAllocSlotheapClear(void *);
void ClearDle__FP3DLE(void *);
void AppendDlEntry__FP2DLPv(void *, void *);

#define VCALL(po, slot, T) ((T)(po)->pvt[slot].pfn)((void *)((char *)(po) + (po)->pvt[slot].d)

/** @sly2-annot addr=0x1F7598
 * Purpose: Deliver a zap record to the first SO in the target chain that accepts it: dedupe against the
 *   recent-zap list (*(struct N * *)&g_sw.dlZprHit), compute GrfzpdFromSoGrfzpk, ask target vt75; on accept log a copy with time
 *   and, if damaging (grfzpd&0xF8), send msg 0xB to both and splice events 0xA/0xB; return the SO or NULL
 * Info: proposed name PsoInflictZap (T2); TU so
 * Notes: v3: superseded InflictZap | top50_round (0.55): Body. Sly 1 zap.cpp InflictZpdZap/FInflictSoZap
 *   vocabulary (ZPR record, FInflictSoZap = SO vt). Callers DetonateBomb, InflictBhvguardZap, ZapLasenBeams,
 *   InflictSoContactZaps, SoTouchZapCallback. Returns the zapped SO ->
 * Confidence: low (draft) [T2 0.55] - evidence: Sly 1 / Lilly reference; semantic review; sources:
 *   r6_1D5150+top50_round
 * See: docs/engine/overview.md#6-subsystem-map, docs/engine/physics-collision.md#3-zaps-damage-delivery,
 *   docs/engine/physics-collision.md#7-key-functions
 * @end-sly2-annot */
struct O *func_001F7598(struct K *pk)
{
    int unused[4];
    struct M m;
    struct M2 m2;
    struct O *po;
    for (po = PsoFromLo(pk->p8); po; po = po->pNext) {
        struct N *pn;
        for (pn = (*(struct N * *)&g_sw.dlZprHit); pn; pn = pn->pNext) {
            if (pn->n0 == (int)pk->p0 && pn->n4 == pk->n4 && pn->nC == pk->nC && pn->p48 == po) return 0;
        }
        pk->n44 = GrfzpdFromSoGrfzpk(po, pk->nC);
        if (VCALL(po, 75, int (*)(void *, struct K *)), pk)) {
            struct K *pkNew;
            pk->p48 = po;
            pkNew = PvAllocSlotheapClear((*(char (*)[])&g_sw.slotheapZprHit));
            *pkNew = *pk;
            pkNew->f4C = D_002DDED0;
            ClearDle__FP3DLE(pkNew->dle);
            AppendDlEntry__FP2DLPv(&(*(struct N * *)&g_sw.dlZprHit), pkNew);
            if (pk->n44 & 0xF8) {
                {
                    m.po = po; m.n = (int)pk->p0; m.pk = pk;
                    VCALL(po, 4, void (*)(void *, int, struct M *)), 0xB, &m);
                    VCALL(pk->p0, 4, void (*)(void *, int, struct M *)), 0xB, &m);
                }
                {
                    m2.ppo = &po; m2.pn = &pk->nC;
                    VCALL(pk->p0, 3, void (*)(void *, int, int, int, void *, int)), 0xA, 1, 2, &m2, 0);
                }
                {
                    m2.ppo = (struct O **)pk;
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
struct DL { void *pvFirst; void *pvLast; int ibDle; };
struct DLI { struct DL *pdl; void **ppv; int ibDle; struct DLI *pdliNext; };
extern struct DLI *D_002DD2FC;
extern struct CLOCK D_002DDED0;
struct E { char pad[0x4C]; float t; };
struct P { char pad[0x4118]; char pool[0x18]; struct DL dl; };
void RemoveDlEntry__FP2DLPv(struct DL *, void *);
void func_0011CD98(void *, void *);
static inline void InitDli(struct DLI *pdli, struct DL *pdl)
{
    pdli->pdl = pdl;
    pdli->ppv = (void **)pdl;
    pdli->ibDle = pdl->ibDle;
    pdli->pdliNext = D_002DD2FC;
    D_002DD2FC = pdli;
}
static inline void *PvNextDli(struct DLI *pdli)
{
    void *pv = *pdli->ppv;
    pdli->ppv = (void **)((char *)pv + pdli->ibDle);
    return pv;
}
/** @sly2-annot addr=0x1F77C0
 * Purpose: Drop zap-history entries older than 0.25 s from g_sw list
 * Info: proposed name ExpireSwZapRecords (T1); TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#3-zaps-damage-delivery,
 *   docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void ExpireSwZapRecords(struct P *p)
{
    struct DLI dli;
    struct E *pe;
    InitDli(&dli, &p->dl);
    while ((pe = (__typeof__(pe))PvNextDli(&dli)) != 0) {
        if (D_002DDED0.t < pe->t + 0.25f) continue;
        RemoveDlEntry__FP2DLPv(&p->dl, pe);
        func_0011CD98(p->pool, pe);
    }
    D_002DD2FC = dli.pdliNext;
}
} } // namespace M_ExpireSwZapRecords

// 0x1F78A0 func_001F78A0
namespace M_func_001F78A0 { extern "C" {
/** @sly2-annot addr=0x1F78A0
 * Purpose: Index of lowest set bit among 26 zap kinds
 * Info: proposed name IzpkFromGrfzpk (T2); TU so
 * Notes: names_v2 C proposed LoadMatrixFromPosRot__FP6VECTORP7MATRIX3P7MATRIX4 (0.438)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F78A0(int x)
{
    int i; int m;
    for (i = 0, m = 1; i < 26; i++, m <<= 1) if (x & m) break;
    return i;
}
} } // namespace M_func_001F78A0

// 0x1F78D8 GrfzpdFromSoGrfzpk
namespace M_GrfzpdFromSoGrfzpk { extern "C" {
struct S;
struct S { char pad[0x110]; unsigned char ab[26]; };
/** @sly2-annot addr=0x1F78D8
 * Purpose: OR of per-kind response bytes SO+0x110[26] selected by grfzpk
 * Info: proposed name GrfzpdFromSoGrfzpk (T1); TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int GrfzpdFromSoGrfzpk(struct S *p, int f)
{
    int r = 0; int i; int m;
    for (i = 0, m = 1; i < 26; i++) { if (m & f) r |= p->ab[i]; m <<= 1; }
    return r;
}
} } // namespace M_GrfzpdFromSoGrfzpk

// 0x1F7918 func_001F7918
namespace M_func_001F7918 { extern "C" {
struct S;
struct S { char pad[0x110]; unsigned char ab[26]; };
/** @sly2-annot addr=0x1F7918
 * Purpose: Set SO+0x110[i]=b for each kind bit in grfzpk
 * Info: proposed name SetSoZapResponse (T2); TU so
 * Notes: names_v2 v1 proposed FIgnoreSoIntersection__FP2SOT0 (0.017)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7918(struct S *p, int f, int b)
{
    int i; int m;
    for (i = 0, m = 1; i < 26; i++) { if (m & f) p->ab[i] = b; m <<= 1; }
}
} } // namespace M_func_001F7918

// 0x1F7950 func_001F7950
namespace M_func_001F7950 { extern "C" {
struct VTE; struct VT; struct E; struct P; struct M; struct G2;
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct VTE { short d; short p; void *pfn; };
struct VT { struct VTE a[75]; };
struct E { char pad[0x6C]; int n6C; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0xEC - 0x10]; struct E *pe; char pad3[0x148 - 0xF0]; union { struct { signed char b0, b1; } c; ulong l; } u; };
struct M { void *p0; int n4; void *p8; int nC; int n10; int pad[3]; qword q20; qword q30; char pad2[0x60 - 0x40]; };
struct G2 { void *p; };
extern struct G2 D_002F7900;
void func_001F7A68(struct P *, int);
void func_002333B4(void *, int, int) __asm__("memset");
qword func_001A62E8(struct P *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void func_001F7598(struct M *);
/** @sly2-annot addr=0x1F7950
 * Purpose: Deferred callback: process contact zaps (vt74) then emit self zap depending on zap mode (+0x148
 *   bits 24-25)
 * Info: proposed name SoTouchZapCallback (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7950(struct P *p)
{
    int k;
    struct M m;
    if (!p->pe) return;
    if (p->pe->n6C) ((void (*)(void *))p->pvt->a[74].pfn)((char *)p + p->pvt->a[74].d);
    if (!p->pe) return;
    k = (int)(p->u.l >> 24) & 3;
    if (!k) return;
    func_001F7A68(p, 0);
    func_002333B4(&m, 0, 0x60);
    m.p8 = p;
    m.q20 = func_001A62E8(p);
    m.q30 = ((qword (*)(void *))p->pvt->a[33].pfn)((char *)p + p->pvt->a[33].d);
    if (k == 3 && FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x38)) {
        m.nC = 0x40000; m.n10 = -50; m.p0 = D_002F7900.p;
    } else {
        m.p0 = p; m.nC = 0x20000; m.n10 = p->u.c.b1;
    }
    func_001F7598(&m);
}
} } // namespace M_func_001F7950

// 0x1F7A68 func_001F7A68
namespace M_func_001F7A68 { extern "C" {
struct E; struct P;
typedef unsigned long ulong;
struct E { char pad[0x14]; short s14; short pad2; int n18; char pad3[0xB0 - 0x1C]; };
struct P { char pad[0xE0]; int c; struct E *ae; char pad2[0x148 - 0xE8]; union { signed char b; struct { ulong lo : 24; ulong k : 2; ulong hi : 38; } f; ulong l; } u; };
/** @sly2-annot addr=0x1F7A68
 * Purpose: Set SO zap mode (+0x148 bits 24-25, 0..3): mode 2 turns parts with zap kind 0 into 0x10000
 *   (damaging) with the SO damage byte; modes 0/1/3 turn them back with dhp -10
 * Info: proposed name SetSoZapMode (T2); TU so
 * Notes: names_v2 v1 proposed UpdateSoImpacts__FP2SO (0.072) | top50_round (0.50): Body; callers
 *   ThrowMurrayCarried/TossMurrayCarried/DropMurrayCarried, UpdateBhvhit, SoTouchZapCallback, ResetSoForProxy
 *   (resets to 0): thrown objects become damaging
 * Confidence: low (draft) [T2 0.50] - evidence: semantic review; static naming pass; sources:
 *   r6_1D5150+top50_round
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7A68(struct P *p, int k)
{
    int from, to, s;
    int i;
    if (k == ((int)(p->u.l >> 24) & 3)) return;
    switch (k) {
    case 0: case 1: case 3:
        from = 0x10000; to = 0; s = -10;
        break;
    case 2:
        s = p->u.b; from = 0; to = 0x10000;
        break;
    }
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (pe->n18 == from) { pe->n18 = to; pe->s14 = s; }
    }
    p->u.f.k = (unsigned int)k;
}
} } // namespace M_func_001F7A68

// 0x1F7B40 func_001F7B40
namespace M_func_001F7B40 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong fl; };
/** @sly2-annot addr=0x1F7B40
 * Purpose: +0x148 bits 24-25
 * Info: proposed name GetSoZapMode (T2); TU so
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F7B40(struct S *p) { return (int)((long)(p->fl << 8) >> 32) & 3; }
} } // namespace M_func_001F7B40

// 0x1F7B58 func_001F7B58
/** @sly2-annot addr=0x1F7B58
 * Purpose: +0x148 byte = -n (damage amount, default 10)
 * Info: proposed name SetSoDhp0 (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F7B58(SO *pso, int n)
{
    pso->bUnk148 = -n;
}

// 0x1F7B68 func_001F7B68
/** @sly2-annot addr=0x1F7B68
 * Purpose: return -(+0x148 byte)
 * Info: proposed name GetSoDhp0 (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" int func_001F7B68(SO *pso)
{
    return -pso->bUnk148;
}

// 0x1F7B78 func_001F7B78
/** @sly2-annot addr=0x1F7B78
 * Purpose: +0x149 byte = -n (default 20)
 * Info: proposed name SetSoDhp1 (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F7B78(SO *pso, int n)
{
    pso->bUnk149 = -n;
}

// 0x1F7B88 func_001F7B88
/** @sly2-annot addr=0x1F7B88
 * Purpose: return -(+0x149 byte)
 * Info: proposed name GetSoDhp1 (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" int func_001F7B88(SO *pso)
{
    return -pso->bUnk149;
}

// 0x1F7B98 func_001F7B98
/** @sly2-annot addr=0x1F7B98
 * Purpose: If SO not found in g_sw list 0x187 (0x15E938) add it (0x13E3C8)
 * Info: proposed name EnsureSoInSwList187 (T2); TU so
 * Notes: names_v2 v1 proposed UProjectVector__FP6VECTORN20 (0.438)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
/* Gives the object a CID 0x12 child with oid 0x187 unless it has one. */
extern "C" void func_001F7B98(SO *pso)
{
    if (PloFindSwObject(&D_003DD490, 2, (OID)0x187, pso) == 0) {
        PloNew((CID)0x12, pso, (OID)0x187, -1);
    }
}

// 0x1F7BF0 func_001F7BF0
/** @sly2-annot addr=0x1F7BF0
 * Purpose: return +0xE8
 * Info: proposed name PsophysFromSo (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" SOPHYS *func_001F7BF0(SO *pso)
{
    return pso->psophys;
}

// 0x1F7BF8 FadeSoOut
namespace M_FadeSoOut { extern "C" {
void SetSoFadeRate(void *, float);
void SetSoAlpha(void *, float);
/** @sly2-annot addr=0x1F7BF8
 * Purpose: dur<=0: rate 0, alpha 0 (removes); else rate -1/dur
 * Info: proposed name FadeSoOut (T1); TU so
 * Notes: names_v2 v1 proposed CalculateSoAngularEffectWorld__FP2SOP6VECTORT1 (0.200)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/player.md#jtgoal-0x144638-0x144b28
 * @end-sly2-annot */
void FadeSoOut(void *p, float f)
{
    if (f <= 0.0f) {
        SetSoFadeRate(p, 0.0f);
        SetSoAlpha(p, 0.0f);
    } else {
        SetSoFadeRate(p, -1.0f / f);
    }
}
} } // namespace M_FadeSoOut

// 0x1F7C68 FadeSoIn
namespace M_FadeSoIn { extern "C" {
struct VTE; struct VT; struct P;
typedef unsigned long ulong;
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[9]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x28 - 0x10]; ulong fl; char pad3[0x12C - 0x30]; float g12C; };
void SetSoFadeRate(struct P *, float);
void SetSoAlpha(struct P *, float);
/** @sly2-annot addr=0x1F7C68
 * Purpose: dur<=0: rate 0, alpha 1; else re-Add if removed (alpha 0) and rate 1/dur
 * Info: proposed name FadeSoIn (T1); TU so
 * Notes: names_v2 v1 proposed PresetSoAccel__FP2SOf (0.029)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void FadeSoIn(struct P *p, float f)
{
    if (f <= 0.0f) {
        SetSoFadeRate(p, 0.0f);
        SetSoAlpha(p, 1.0f);
    } else {
        if (p->fl & (1L << 56)) {
            p->pvt->a[8].pfn((char *)p + p->pvt->a[8].d);
            p->g12C = 0.0f;
        }
        SetSoFadeRate(p, 1.0f / f);
    }
}
} } // namespace M_FadeSoIn

// 0x1F7D10 SetSoAlpha
/** @sly2-annot addr=0x1F7D10
 * Purpose: alpha<=0: Remove (vt9) unless class 0x69; else Add (vt8); store +0x12C
 * Info: proposed name SetSoAlpha (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
/* Sets the alpha; at 0 the object is removed (LO Remove, unless it is a CID 0x69), above 0 added (LO Add). */
extern "C" void SetSoAlpha(SO *pso, float uAlpha)
{
    if (uAlpha <= 0.0f) {
        if (FIsBasicDerivedFrom(pso, (CID)0x69) == 0) {
            pso->Remove();
        }
    } else {
        pso->Add();
    }
    pso->uAlpha = uAlpha;
}

// 0x1F7D90 SetSoFadeRate
namespace M_SetSoFadeRate { extern "C" {
struct VTE; struct VT; struct P;
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[9]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x40 - 0x10]; int v40; char pad3[0x130 - 0x44]; float g; };
/** @sly2-annot addr=0x1F7D90
 * Purpose: If rate != 0: Add (vt8) and set grf bit 2; store +0x130
 * Info: proposed name SetSoFadeRate (T1); TU so
 * Notes: names_v2 v1 proposed RotateSoToMat__FP2SOP7MATRIX3 (0.026)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void SetSoFadeRate(struct P *p, float g)
{
    if (g != 0.0f) {
        int v;
        p->pvt->a[8].pfn((char *)p + p->pvt->a[8].d);
        v = p->v40;
        if ((v ^ 1) & 1) *(int *)v |= 2;
        else p->v40 = v | 2;
    }
    p->g = g;
}
} } // namespace M_SetSoFadeRate

// 0x1F7E08 func_001F7E08
namespace M_func_001F7E08 { extern "C" {
struct E; struct P;
struct E { char pad[0xB0]; };
struct P { char pad[0xE0]; int c; struct E *ae; };
/** @sly2-annot addr=0x1F7E08
 * Purpose: Collect parts passing optional predicate into array, return count
 * Info: proposed name CsopartCollectSo (T2); TU so
 * Notes: names_v2 v1 proposed SetSoAngularVelocityVec__FP2SOP6VECTOR (0.438)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F7E08(struct P *p, int (*pfn)(void *, struct E *), void *pv, struct E **ape)
{
    int n = 0;
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (!pfn || pfn(pv, pe)) {
            ape[n++] = pe;
        }
    }
    return n;
}
} } // namespace M_func_001F7E08

// 0x1F7EC8 func_001F7EC8
namespace M_func_001F7EC8 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong a:16; int b:4; ulong c:44; };
/** @sly2-annot addr=0x1F7EC8
 * Purpose: Set 4-bit field +0x148 bits 16-19
 * Info: proposed name SetSoField148_16 (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7EC8(struct S *p, int n) { p->b = n; }
} } // namespace M_func_001F7EC8

// 0x1F7EF0 func_001F7EF0
namespace M_func_001F7EF0 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong a:16; int b:4; ulong c:44; };
/** @sly2-annot addr=0x1F7EF0
 * Purpose: Get 4-bit field +0x148 bits 16-19
 * Info: proposed name GetSoField148_16 (T2); TU so
 * Notes: names_v2 v1 proposed AddSoAcceleration__FP2SOP6VECTOR (0.054)
 * Confidence: low (draft) [T2 0.20] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F7EF0(struct S *p) { return p->b; }
} } // namespace M_func_001F7EF0

// 0x1F7F08 func_001F7F08
namespace M_func_001F7F08 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong fl; };
/** @sly2-annot addr=0x1F7F08
 * Purpose: Get +0x148 bit29
 * Info: proposed name GetSoFlag29 (T2); TU so
 * Notes: names_v2 v1 proposed AddSoAngularAcceleration__FP2SOP6VECTOR (0.054)
 * Confidence: low (draft) [T2 0.20] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F7F08(struct S *p) { return (int)((long)(p->fl << 3) >> 32) & 1; }
} } // namespace M_func_001F7F08

// 0x1F7F20 func_001F7F20
namespace M_func_001F7F20 { extern "C" {
struct S;
struct S { char pad[0x148]; ulong a:29; int b:1; ulong c:34; };
/** @sly2-annot addr=0x1F7F20
 * Purpose: Set +0x148 bit29
 * Info: proposed name SetSoFlag29 (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7F20(struct S *p, int n) { p->b = n; }
} } // namespace M_func_001F7F20

// 0x1F7F48 func_001F7F48
namespace M_func_001F7F48 { extern "C" {
struct A;
typedef unsigned long ulong;
struct A { char pad[0x148]; ulong a:34; ulong f:1; ulong b:1; };
void func_001F55C8(struct A *p);
/** @sly2-annot addr=0x1F7F48
 * Purpose: Set +0x148 bit34 and RebuildSoPhysHook if changed
 * Info: proposed name SetSoPhysHookFlag (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7F48(struct A *p, long f)
{
    if (f != p->f) {
        p->f = f;
        func_001F55C8(p);
    }
}
} } // namespace M_func_001F7F48

// 0x1F7FA0 func_001F7FA0
namespace M_func_001F7FA0 { extern "C" {
struct P;
struct P { char pad[0xA4]; int c; void **ap; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void SetScrollerMasterSpeeds__FP8SCROLLERff(void *, float, float);
/** @sly2-annot addr=0x1F7FA0
 * Purpose: SetScrollerMasterSpeeds(f,g) on every SCROLLER (CID 0x8F) in SO SAA list (+0xA4)
 * Info: proposed name SetSoScrollerSpeeds (T2); TU so
 * Notes: names_v2 v1 proposed ApplySoProxy__FP2SOP5PROXY (0.069)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7FA0(struct P *p, float f, float g)
{
    int i;
    for (i = 0; i < p->c; i++) {
        void *q = p->ap[i];
        if (FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x8F)) SetScrollerMasterSpeeds__FP8SCROLLERff(q, f, g);
    }
}
} } // namespace M_func_001F7FA0

// 0x1F8038 func_001F8038
namespace M_func_001F8038 { extern "C" {
struct P;
struct P { char pad[0xA4]; int c; void **ap; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void SetSaiDuDv(void *, float, float);
/** @sly2-annot addr=0x1F8038
 * Purpose: 0x1EDAD8(f,g) on every CID 0x8F SAA
 * Info: proposed name SetSoSaaParams (T2); TU so
 * Notes: names_v2 v1 proposed GetSoCpdefi__FP2SOfP6CPDEFI (0.016)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8038(struct P *p, float f, float g)
{
    int i;
    for (i = 0; i < p->c; i++) {
        void *q = p->ap[i];
        if (FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x8F)) SetSaiDuDv(q, f, g);
    }
}
} } // namespace M_func_001F8038

// 0x1F80D0 func_001F80D0
namespace M_func_001F80D0 { extern "C" {
struct L;
struct L { char pad[0x24]; struct L *next; char pad2[0x64 - 0x28]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; char pad4[0xA4 - 0x74]; int c; void **ap; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void SetEyesClosed(void *, float);
/** @sly2-annot addr=0x1F80D0
 * Purpose: SetEyesClosed(f) on EYES (CID 0x8E) SAAs, recursively through SO children
 * Info: proposed name SetSoEyesClosed (T2); TU so
 * Notes: names_v2 v1 proposed PsoFirstSoTouching__FP2SO (0.087)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001F80D0(struct L *p, float f)
{
    int i;
    struct L *q;
    for (i = 0; i < p->c; i++) {
        void *pv = p->ap[i];
        if (FIsBasicDerivedFrom__FP5BASIC3CID(pv, 0x8E)) SetEyesClosed(pv, f);
    }
    for (q = p->first; q != p->end; q = q->next)
        if (FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) func_001F80D0(q, f);
}
} } // namespace M_func_001F80D0

// 0x1F8198 PsaaFindSoOid
namespace M_PsaaFindSoOid { extern "C" {
struct T; struct S;
struct T { char pad[0x18]; int n; };
struct S { char pad[0xA4]; int c; struct T **apt; };
/** @sly2-annot addr=0x1F8198
 * Purpose: Find SAA in +0xA4 list with +0x18 == n
 * Info: proposed name PsaaFindSoOid (T1); TU so
 * Notes: verifier CORRECT: PsaaFindSo -> PsaaFindSoOid
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct T *PsaaFindSoOid(struct S *p, int n)
{
    int i;
    for (i = 0; i < p->c; i++) { struct T *pt = p->apt[i]; if (pt->n == n) return pt; }
    return 0;
}
} } // namespace M_PsaaFindSoOid

// 0x1F81E0 func_001F81E0
namespace M_func_001F81E0 { extern "C" {
struct A;
struct A { char pad[0xAC]; int c; int *a; int n; };
/** @sly2-annot addr=0x1F81E0
 * Purpose: +0xB4 = -(index of n in +0xB0 list + 3), or 0
 * Info: proposed name SetSoAnimIndex (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F81E0(struct A *p, int n)
{
    int i;
    if (n == -1) {
        p->n = 0;
        return;
    }
    for (i = 0; i < p->c; i++) {
        if (n == p->a[i]) {
            p->n = -(i + 3);
            return;
        }
    }
    p->n = 0;
}
} } // namespace M_func_001F81E0

// 0x1F8238 func_001F8238
/** @sly2-annot addr=0x1F8238
 * Purpose: +0xB4 = 0
 * Info: proposed name ClearSoB4 (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F8238(SOWATCH *pwatch)
{
    pwatch->nB4 = 0;
}

// 0x1F8240 func_001F8240
namespace M_func_001F8240 { extern "C" {
struct T;
struct T { int a[3]; int n; char pad[0x10]; struct T *pnext; };
/** @sly2-annot addr=0x1F8240
 * Purpose: Search (*(struct T * *)&g_sw.dlTargetrec) list by id
 * Info: proposed name PxxFindGlobalById (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct T *func_001F8240(int n) { struct T *pt; for (pt = (*(struct T * *)&g_sw.dlTargetrec); pt; pt = pt->pnext) if (pt->n == n) return pt; return 0; }
} } // namespace M_func_001F8240

// 0x1F8278 func_001F8278
/** @sly2-annot addr=0x1F8278
 * Purpose: Find, else new 0xAC object (0x20A5F8)
 * Info: proposed name EnsureGlobalById (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
/* Makes a target record (0xAC bytes, func_0020A5F8) for plo unless it has one. */
extern "C" void func_001F8278(LO *plo)
{
    if (func_001F8240(plo) == 0) {
        func_0020A5F8(func_001AE4F8(0xAC), plo);
    }
}

// 0x1F82B8 func_001F82B8
namespace M_func_001F82B8 { extern "C" {
struct K; struct T;
struct K { int a, b; };
struct T { char pad[0x10]; int x; struct K k; };
struct T *func_001F8278(void *);
/** @sly2-annot addr=0x1F82B8
 * Purpose: Ensure object then set x and optional 8-byte key
 * Info: proposed name SetSoSideRecordParams (T2); TU so
 * Notes: verifier PLAUSIBLE: SetGlobalByIdParams -> SetSoSideRecordParams (exact field roles unknown) |
 *   verifier: purpose partly wrong: Find-or-create a 0xAC record in list D_003E145C keyed by the OBJECT
 *   POINTER (0x1F8240 compares rec+0xC to p; 0x20A5F8 inits int -1, floats 1.0/2.0), then set th
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_001F82B8(void *p, int x, struct K *pk) { struct T *pt = func_001F8278(p); pt->x = x; if (pk) pt->k = *pk; }
} } // namespace M_func_001F82B8

// 0x1F8300 func_001F8300
namespace M_func_001F8300 { extern "C" {
struct Q; struct E; struct P; struct CB;
struct Q { void *p0; int pad; float g8; float gC; };
struct E { char pad[0xDC]; struct Q *pq; };
struct P { char pad[0xE8]; struct E *pe; };
struct CB { int cb; int n; };
void *func_001AE2A0(struct CB *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
static inline void *PvAllocMsg(struct CB *pcb, int cb, int n) { pcb->cb = cb; pcb->n = n; return func_001AE2A0(pcb); }
/** @sly2-annot addr=0x1F8300
 * Purpose: Ensure 0x70-byte SOPHYS+0xDC record {pso, g4, g8=75 (150 for CID 0x34), gC=1}
 * Info: proposed name PsowaterEnsure (T2); TU so
 * Notes: names_v2 v1 proposed ApplyConstr__FP6CONSTRP6VECTORN21 (0.028)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct Q *func_001F8300(struct P *p)
{
    if (!p->pe) return 0;
    if (!p->pe->pq) {
        struct CB cb;
        struct Q *q = (__typeof__(q))PvAllocMsg(&cb, 0x70, 0x10);
        q->p0 = p;
        if (FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x34)) q->g8 = 150.0f;
        else q->g8 = 75.0f;
        q->gC = 1.0f;
        p->pe->pq = q;
    }
    return p->pe->pq;
}
} } // namespace M_func_001F8300

// 0x1F83B0 func_001F83B0
/** @sly2-annot addr=0x1F83B0
 * Purpose: record.g8 = f
 * Info: proposed name SetSoPhysParam8 (T2); TU so
 * Notes: names_v2 v1 proposed ApplySoConstraintLocal__FP2SOP6CONSTRP6VECTORN22 (0.004)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F83B0(STEP *pstep, float g)
{
    STEPREC *prec;
    prec = func_001F8300(pstep);
    if (prec != 0) {
        prec->g8 = g;
    }
}

// 0x1F83E0 func_001F83E0
namespace M_func_001F83E0 { extern "C" {
struct U; struct T; struct S;
struct U { char pad[0x8]; float g; };
struct T { char pad[0xDC]; struct U *pu; };
struct S { char pad[0xE8]; struct T *pt; };
/** @sly2-annot addr=0x1F83E0
 * Purpose: record.g8 or 0
 * Info: proposed name GetSoPhysParam8 (T2); TU so
 * Notes: names_v2 v1 proposed AddSoXa__FP2SOP2XA (0.001)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
float func_001F83E0(struct S *p) { struct U *pu = 0; if (p->pt) pu = p->pt->pu; if (pu) return pu->g; return 0.0f; }
} } // namespace M_func_001F83E0

// 0x1F8410 func_001F8410
/** @sly2-annot addr=0x1F8410
 * Purpose: record.gC = f
 * Info: proposed name SetSoPhysParamC (T2); TU so
 * Notes: names_v2 v1 proposed SetSoNoInteract__FP2SOi (0.004)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F8410(STEP *pstep, float g)
{
    STEPREC *prec;
    prec = func_001F8300(pstep);
    if (prec != 0) {
        prec->gC = g;
    }
}

// 0x1F8440 func_001F8440
namespace M_func_001F8440 { extern "C" {
struct U; struct T; struct S;
struct U { char pad[0xC]; float g; };
struct T { char pad[0xDC]; struct U *pu; };
struct S { char pad[0xE8]; struct T *pt; };
/** @sly2-annot addr=0x1F8440
 * Purpose: record.gC or 0
 * Info: proposed name GetSoPhysParamC (T2); TU so
 * Notes: names_v2 v1 proposed ConstrFromCnstr__F5CNSTRP2CTP6VECTOR (0.054)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
float func_001F8440(struct S *p) { struct U *pu = 0; if (p->pt) pu = p->pt->pu; if (pu) return pu->g; return 0.0f; }
} } // namespace M_func_001F8440

// 0x1F8470 func_001F8470
/** @sly2-annot addr=0x1F8470
 * Purpose: record.g4 = f
 * Info: proposed name SetSoPhysParam4 (T2); TU so
 * Notes: names_v2 v1 proposed SetSoCameraStyle__FP2SO3CMK (0.043)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F8470(STEP *pstep, float g)
{
    STEPREC *prec;
    prec = func_001F8300(pstep);
    if (prec != 0) {
        prec->g4 = g;
    }
}

// 0x1F84A0 func_001F84A0
namespace M_func_001F84A0 { extern "C" {
struct U; struct T; struct S;
struct U { char pad[0x4]; float g; };
struct T { char pad[0xDC]; struct U *pu; };
struct S { char pad[0xE8]; struct T *pt; };
/** @sly2-annot addr=0x1F84A0
 * Purpose: record.g4 or 0
 * Info: proposed name GetSoPhysParam4 (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
float func_001F84A0(struct S *p) { struct U *pu = 0; if (p->pt) pu = p->pt->pu; if (pu) return pu->g; return 0.0f; }
} } // namespace M_func_001F84A0

// 0x1F84D0 SetSoIceable__FP2SOi
// SO+0x148 bit 33 (64-bit unit at 0x148). Lilly's port calls this bit fHurtRespawns; the function name says "iceable".
/** @sly2-annot addr=0x1F84D0
 * Purpose: Set +0x148 bit33 (fIceable)
 * Info: proposed name SetSoIceable__FP2SOi (EXISTING); TU so
 * Confidence: high [EXISTING 0.85] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void SetSoIceable(SO *pso, int f) { pso->fHurtRespawns = f; }

// 0x1F8500 GetSoIceable
namespace M_GetSoIceable { extern "C" {
struct S;
struct S { char pad[0x148]; ulong fl; };
/** @sly2-annot addr=0x1F8500
 * Purpose: Return +0x148 bit33
 * Info: proposed name GetSoIceable (T1); TU so
 * Notes: names_v2 v1 proposed SetSoMtlk__FP2SO4MTLK (0.008)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int GetSoIceable(struct S *p) { return (int)(p->fl >> 33) & 1; }
} } // namespace M_GetSoIceable

// 0x1F8510 func_001F8510
namespace M_func_001F8510 { extern "C" {
struct Q; struct E; struct P;
struct Q { int a, b, c; void *pvt; int pad[4]; void *p20; void *p24; int pad2[2]; };
struct E { char pad[0x94]; int n94; int pad2; int n9C; char pad3[0x100 - 0xA0]; };
struct P { char pad[0x50]; int c; struct E *ae; };
extern char D_002CA2C0[];
struct Pool;
struct Q *PvAllocSlotheapClear(struct Pool *);
void func_002333B4(void *, int, int) __asm__("memset");
/** @sly2-annot addr=0x1F8510
 * Purpose: Create a 0x30 handle object (pool (*(struct Pool *)&g_sw.slotheapXfh), vtable 0x2CA2C0 = CID 0x95) for the XF matching
 *   (oid,kind)
 * Info: proposed name PxfhNewSo (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct Q *func_001F8510(struct P *p, int k, int m)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (pe->n94 == m && (k == -1 || pe->n9C == k)) {
            struct Q *q = PvAllocSlotheapClear(&(*(struct Pool *)&g_sw.slotheapXfh));
            func_002333B4(q, 0, 0x30);
            q->pvt = D_002CA2C0;
            q->p20 = p;
            q->p24 = pe;
            q->a = 0;
            q->b = 0;
            return q;
        }
    }
    return 0;
}
} } // namespace M_func_001F8510

// 0x1F85E0 func_001F85E0
namespace M_func_001F85E0 { extern "C" {
struct Q;
struct Q { int a, b, c; void *pvt; int pad[4]; void *p20; void *p24; int pad2[2]; };
extern char D_002CA280[];
struct Pool;
void *PsopartFindSo(void *);
struct Q *PvAllocSlotheapClear(struct Pool *);
void *func_002333B4(void *, int, int) __asm__("memset");
/** @sly2-annot addr=0x1F85E0
 * Purpose: Create a 0x30 handle object (vtable 0x2CA280 = CID 0x96) for the SOPART with given oid
 * Info: proposed name PsoparthNewSo (T2); TU so
 * Notes: names_v2 v1 proposed SendSoMessage__FP2SO5MSGIDPv (0.438)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct Q *func_001F85E0(void *pa)
{
    void *r = PsopartFindSo(pa);
    struct Q *q;
    if (!r) return 0;
    q = PvAllocSlotheapClear(&(*(struct Pool *)&g_sw.slotheapXfh));
    func_002333B4(q, 0, 0x30);
    q->p20 = pa;
    q->p24 = r;
    q->pvt = D_002CA280;
    q->a = 0;
    q->b = 0;
    return q;
}
} } // namespace M_func_001F85E0

// 0x1F8668 func_001F8668
namespace M_func_001F8668 { extern "C" {
struct L; struct G;
typedef unsigned long ulong;
struct L { char pad[0x24]; struct L *next; char pad2[0x64 - 0x28]; struct L *first; char pad3[0x148 - 0x68]; ulong lo : 35; ulong b35 : 1; ulong hi : 28; };
struct G { char pad[0x3E5C]; int f; };
extern struct G D_003DD490;
void func_001D6378(struct L *);
void func_001D6310(struct L *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
/** @sly2-annot addr=0x1F8668
 * Purpose: Set +0x148 bit35 recursively; when g_sw+0x3E5C and enabling register (0x1D6378)
 * Info: proposed name SetSoFlag35Hierarchy (T2); TU so
 * Notes: names_v2 v1 proposed PxpFindSoGround__FP2SOT0Pi (0.047)
 * Confidence: low (draft) [T2 0.30] - evidence: semantic review; static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8668(struct L *p, long f)
{
    struct L *q;
    if (f != p->b35) {
        p->b35 = f;
        if (D_003DD490.f) {
            if (f) func_001D6378(p);
            else func_001D6310(p);
        }
        for (q = p->first; q; q = q->next)
            if (FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) func_001F8668(q, f);
    }
}
} } // namespace M_func_001F8668

// 0x1F8730 func_001F8730
/** @sly2-annot addr=0x1F8730
 * Purpose: cref(+0x10)++
 * Info: proposed name IncrefXfh (T2); class CID148 slot1; TU so
 * Notes: verify r4r6: HANDLE::Decref -> XFH::Decref
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F8730(XFH *pxfh)
{
    pxfh->cref = pxfh->cref + 1;
}

// 0x1F8740 DecrefXfh
/** @sly2-annot addr=0x1F8740
 * Purpose: --cref; at 0 destroy and free to pool g_sw.slotheapXfh
 * Info: proposed name DecrefXfh (T1); class CID148 slot2; TU so
 * Notes: verifier CORRECT: HANDLE::Decref -> XFH::Decref
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
/* Drops a reference; the last one clears the splice frame and sidebag, unsubscribes and frees the handle. */
extern "C" void DecrefXfh(XFH *pxfh)
{
    int cref;
    cref = pxfh->cref - 1;
    pxfh->cref = cref;
    if (cref <= 0) {
        FramePtr_Assign(&pxfh->pframe, 0);
        func_002580A8(&pxfh->psidebag, 0);
        UnsubscribeLoAll(pxfh);
        func_0011CD98(&g_sw.slotheapXfh, pxfh);
    }
}

// 0x1F87A0 func_001F87A0
namespace M_func_001F87A0 { extern "C" {
struct T; struct S;
struct T { char pad[0xAC]; unsigned short n; };
struct S { char pad[0x24]; struct T *pt; };
/** @sly2-annot addr=0x1F87A0
 * Purpose: XF u16 +0xAC * 0.001
 * Info: proposed name GetXfhParamAC (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
float func_001F87A0(struct S *p) { return p->pt->n * 0.001f; }
} } // namespace M_func_001F87A0

// 0x1F87C8 func_001F87C8
namespace M_func_001F87C8 { extern "C" {
struct T; struct S;
struct T { char pad[0xAC]; short n; };
struct S { char pad[0x20]; void *p; struct T *pt; };
void SlotUnsettle(void *, struct T *);
/** @sly2-annot addr=0x1F87C8
 * Purpose: XF +0xAC = g*1000, refresh XF kind
 * Info: proposed name SetXfhParamAC (T2); TU so
 * Notes: names_v2 v1 proposed SetSoNoGravity__FP2SOi (0.077)
 * Confidence: low (draft) [T2 0.20] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F87C8(struct S *p, float g) { p->pt->n = (int)(g * 1000.0f); SlotUnsettle(p->p, p->pt); }
} } // namespace M_func_001F87C8

// 0x1F8808 func_001F8808
namespace M_func_001F8808 { extern "C" {
struct T; struct S;
struct T { char pad[0xAE]; unsigned short n; };
struct S { char pad[0x24]; struct T *pt; };
/** @sly2-annot addr=0x1F8808
 * Purpose: XF u16 +0xAE * 0.001
 * Info: proposed name GetXfhParamAE (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
float func_001F8808(struct S *p) { return p->pt->n * 0.001f; }
} } // namespace M_func_001F8808

// 0x1F8830 func_001F8830
namespace M_func_001F8830 { extern "C" {
struct T; struct S;
struct T { char pad[0xAE]; short n; };
struct S { char pad[0x20]; void *p; struct T *pt; };
void SlotUnsettle(void *, struct T *);
/** @sly2-annot addr=0x1F8830
 * Purpose: XF +0xAE = g*1000, refresh
 * Info: proposed name SetXfhParamAE (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8830(struct S *p, float g) { p->pt->n = (int)(g * 1000.0f); SlotUnsettle(p->p, p->pt); }
} } // namespace M_func_001F8830

// 0x1F8870 func_001F8870
namespace M_func_001F8870 { extern "C" {
struct T; struct S;
struct T { char pad[0xAC]; unsigned short n; };
struct S { char pad[0x24]; struct T *pt; };
/** @sly2-annot addr=0x1F8870
 * Purpose: duplicate of 0x1F87A0 (different vtable)
 * Info: proposed name GetXfhParamAC2 (T2); TU so
 * Notes: verifier: purpose partly wrong: Getter: XF-handle (+0x20 SO, +0x24 XF) -> u16 XF+0xAC * 0.001
 *   (ms->s); setter 0x1F8898. Code identical to 0x1F87A0, but the two refs are Splice property descrip
 * Confidence: low (draft) [T2 0.15] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
float func_001F8870(struct S *p) { return p->pt->n * 0.001f; }
} } // namespace M_func_001F8870

// 0x1F8898 func_001F8898
namespace M_func_001F8898 { extern "C" {
struct T; struct S;
struct T { char pad[0xAC]; short n; };
struct S { char pad[0x20]; void *p; struct T *pt; };
void SlotUnsettle(void *, struct T *);
/** @sly2-annot addr=0x1F8898
 * Purpose: duplicate of 0x1F87C8
 * Info: proposed name SetXfhParamAC2 (T2); TU so
 * Confidence: low (draft) [T2 0.15] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8898(struct S *p, float g) { p->pt->n = (int)(g * 1000.0f); SlotUnsettle(p->p, p->pt); }
} } // namespace M_func_001F8898

// 0x1F88D8 func_001F88D8
/** @sly2-annot addr=0x1F88D8
 * Purpose: optionally refresh XF kind (arg==1) and return XF (+0x24)
 * Info: proposed name PxfFromXfh (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" XF *func_001F88D8(XFH *pxfh, int n)
{
    if (n == 1) {
        SlotUnsettle(pxfh->pso, pxfh->pxf);
    }
    return pxfh->pxf;
}

// 0x1F8910 func_001F8910
namespace M_func_001F8910 { extern "C" {
struct E; struct Q; struct P;
struct E { int k; int pad[2]; int nC; struct E *next; };
struct Q { char pad[0x90]; struct E *pe; };
struct P { char pad[0x20]; void *p20; struct Q *pq; };
void func_00230100(void *, struct Q *, struct E *);
void func_0012E210(struct E *);
void func_002301D0(void *, struct Q *);
void func_002303A0(void *, struct Q *, int);
void SlotUnsettle(void *, struct Q *);
/** @sly2-annot addr=0x1F8910
 * Purpose: Find XF actuator of kind k; remove if n==-1, else update and re-prioritize
 * Info: proposed name PactFindXfh (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct E *func_001F8910(struct P *p, int k, int n, int m)
{
    struct E *pe;
    for (pe = p->pq->pe; pe; pe = pe->next) {
        if (pe->k != k) continue;
        if (n == -1) {
            func_00230100(p->p20, p->pq, pe);
            func_0012E210(pe);
            return 0;
        }
        if (m != pe->nC) {
            pe->nC = m;
            func_002301D0(p->p20, p->pq);
        }
        if (pe == p->pq->pe) {
            func_002303A0(p->p20, p->pq, n);
            SlotUnsettle(p->p20, p->pq);
        }
        return pe;
    }
    return 0;
}
} } // namespace M_func_001F8910

// 0x1F89E0 func_001F89E0
namespace M_func_001F89E0 { extern "C" {
struct R; struct P;
typedef int qword __attribute__((mode(TI)));
struct R { char pad[0x14]; int n14; };
struct P { char pad[0x20]; void *p20; void *p24; };
struct R *func_001F8910(struct P *, int, int, int);
void SetActPositionGoal(struct R *, qword);
void func_0012DC18(void *, void *, int, int, int, qword);
/** @sly2-annot addr=0x1F89E0
 * Purpose: Update/create kind-1 actuator with value n,m,q
 * Info: proposed name SetXfhActuator1 (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F89E0(struct P *p, int n, int m, qword q)
{
    struct R *r = func_001F8910(p, 1, n, m);
    if (r) {
        r->n14 = n;
        SetActPositionGoal(r, q);
    } else if (n != -1) {
        func_0012DC18(p->p20, p->p24, m, n, 2, q);
    }
}
} } // namespace M_func_001F89E0

// 0x1F8A78 func_001F8A78
namespace M_func_001F8A78 { extern "C" {
struct B; struct A;
typedef int qword __attribute__((mode(TI)));
struct B { char pad[0xA0]; int n; };
struct A { char pad[0x24]; struct B *pb; };
qword func_0022FC48(int a, int n, qword q);
void func_001F89E0(struct A *p, int a, int b, qword q);
/** @sly2-annot addr=0x1F8A78
 * Purpose: 0x1F89E0 with vector converted from XF parent space
 * Info: proposed name SetXfhActuator1World (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8A78(struct A *p, int a, int b, qword q)
{
    func_001F89E0(p, a, b, func_0022FC48(0, p->pb->n, q));
}
} } // namespace M_func_001F8A78

// 0x1F8AD8 func_001F8AD8
namespace M_func_001F8AD8 { extern "C" {
struct M3; struct O; struct P;
typedef int qword __attribute__((mode(TI)));
struct M3 { qword a[3]; };
struct O { char pad[0xA0]; int nA0; };
struct P { char pad[0x24]; struct O *po; };
void ConvertXfMat(struct M3 *, int, int);
qword func_0011BFF0(struct M3 *);
void func_001F89E0(struct P *, int, int, qword);
/** @sly2-annot addr=0x1F8AD8
 * Purpose: 0x1F89E0 with value from matrix
 * Info: proposed name SetXfhActuator1Mat (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8AD8(struct P *p, int a, int b)
{
    struct M3 c, t;
    ConvertXfMat(&t, 0, p->po->nA0);
    c = t;
    func_001F89E0(p, a, b, func_0011BFF0(&c));
}
} } // namespace M_func_001F8AD8

// 0x1F8B58 func_001F8B58
namespace M_func_001F8B58 { extern "C" {
struct R; struct P;
struct R { char pad[0x14]; int n14; int pad2; float g1C; float g20; };
struct P { char pad[0x20]; void *p20; void *p24; };
struct R *func_001F8910(struct P *, int, int, int);
void func_0012DCA0(void *, void *, int, int, float, float);
/** @sly2-annot addr=0x1F8B58
 * Purpose: Update/create kind-4 actuator with two floats
 * Info: proposed name SetXfhActuator4 (T2); TU so
 * Notes: names_v2 v1 proposed RebuildSoPhysHook__FP2SO (0.081)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8B58(struct P *p, int n, int m, float f, float g)
{
    struct R *r = func_001F8910(p, 4, n, m);
    if (r) {
        r->n14 = n;
        r->g1C = f;
        r->g20 = g;
    } else if (n != -1) {
        func_0012DCA0(p->p20, p->p24, m, n, f, g);
    }
}
} } // namespace M_func_001F8B58

// 0x1F8BF0 func_001F8BF0
namespace M_func_001F8BF0 { extern "C" {
struct U; struct V; struct T; struct S;
typedef int qword __attribute__((mode(TI)));
struct U { int a, b; int n; float x, y, z, r; int pad; qword q; };
struct V { char pad[0x60]; struct U *pu; };
struct T { char pad[0x98]; short k; char pad2[0xE]; struct V *pv; };
struct S { char pad[0x24]; struct T *pt; };
/** @sly2-annot addr=0x1F8BF0
 * Purpose: For XF kind 16: set n, xyz, angle (deg->rad), q
 * Info: proposed name SetXfhKind16Params (T2); TU so
 * Notes: names_v2 v1 proposed PsoFindSoPhysHook__FP2SOi (0.051)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8BF0(struct S *p, int n, qword q, float x, float y, float z, float deg)
{ struct T *pt = p->pt; if (pt->k == 16) { struct U *pu = pt->pv->pu; pu->n = n; pu->x = x; pu->y = y; pu->z = z; pu->r = deg * 0.01745329424738884f; pu->q = q; } }
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
void p_func_001F3868() __asm__("func_001F3868");
void p_func_001F7950() __asm__("func_001F7950");
extern "C" {
extern const unsigned int D_002A8270[2] = {0xFFFF0000, (unsigned int)p_func_001F3868};
extern const unsigned int D_002A8278[2] = {0xFFFF0000, (unsigned int)p_func_001F7950};
}
} // namespace ANPTR_so
