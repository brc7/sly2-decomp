// path.cpp -- navigation for AI movement. A path zone (PATHZONE, CID 17, an XFM) holds a navigation graph (CG):
// walkable triangles (CGT) with a 2D BSP for point location, and vertices (CGV) with edge costs for A*. Role: locate
// the triangle under a point, clip a segment against walkable space, find and simplify a path (A* plus line-of-sight
// pruning), choose the path zone that contains a point (the world's zone BSP), keep each zone's dynamic obstacles
// (convex polygons from cnvo.cpp) in sweep-and-prune event arrays, and route between zones through path links
// (PATHLINK, CID 124). Entry points: FindPathPoints, FindPathzonePathCurve, FindPathzoneAtPos, IsPosInPathzone,
// IsPosFreeInPathzone, AddPathzoneObstacle / RemovePathzoneObstacle, FindPathlinkRoute, PostLoadSwPathzones.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_path
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_path
#include "sly2/basic.h"
#include "sly2/xfm.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/classes.h"
#include "sly2/vu0.h"
#include "sly2/dli.h"
#include "sly2/sw.h"
#include "sly2/port.h"
#include "sly2/lsg.h"

// 0x1C27C8 StaticInit_path and GlobalConstructors_path: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* The world's zone BSP (SW::pregtree, 0x30 bytes on the EE): its nodes and planes (LoadZoneBspFromBrx), the zones by
   index, and the path links bucketed by zone (BuildPathzoneLinkLists, BindSwPathzones). Defined here, the only TU that
   allocates or reads one. */
/* A node of the zone BSP (8 bytes on the EE): its plane and two children (the side in front of the plane first); a
   child with bit 15 set is a leaf, its low 15 bits index REGTREE::zones (FindPathzoneAtPos). */
struct REGBSPNODE
{
    VECTOR4 *plane;
    unsigned short children[2];
};
/* The path links whose near end lies in one zone: indices into REGTREE::links (BuildPathzoneLinkLists; read by
   FindPathlinkRoute). */
struct REGLINKLIST
{
    int count;
    int *linkIndices;
};
struct REGTREE
{
    int numNodes;
    REGBSPNODE *nodes;
    int numPlanes;
    VECTOR4 *planes;
    int numZones;
    int *zoneIds;                /* the leaf zones' oids, resolved into zones by BindSwPathzones */
    PATHZONE **zones;
    REGLINKLIST *linkLists;      /* per zone */
    int numLinks;
    PATHLINK **links;            /* every path link (CID 124), numbered through PATHLINK::linkIndex */
    int linkVersion;             /* the world's navigation version (SW::unknown3D74), bumped when a link is added or removed */
    int isBound;
};
typedef char assert_REGTREE_size[sizeof(REGTREE) == 0x30 ? 1 : -1];   /* EE */
typedef char assert_REGTREE_linkVersion[SLY2_OFFSETOF(REGTREE, linkVersion) == 0x28 ? 1 : -1];   /* EE */
/* PATHLINK names its zones by basic.h's forward name CID17; the class is PATHZONE. */
static inline PATHZONE *PathzoneOf(CID17 *zone) { return (PATHZONE *)zone; }


extern "C" {
void InitXfm(void *pxfm);
void *memset(void *pv, int n, unsigned int cb);
CGT *FindNavTriangleQuick(CG *pcg, qword pos, int grf);
int FindClosestPointInCg(CG *pcg, qword pos, qword *ppos, int flags);
int ClipEdgeToCbsp(CG *pcg, qword posFrom, qword posTo, int clsgMax, void *alsg, int grf);
int IsEdgeBlockedByCbsp(CG *pcg, qword posFrom, qword posTo, int grf);
}
void InitDl(DL *pdl, int ibDle);

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_PcgtPointInCbspQuick {
// Dot product of the xyz parts of a and b as a float.
extern "C" { static __inline__ float Dot(qword a, qword b)
{
    qword d, one;
    float f;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
} }



// Walks the graph's 2D BSP and returns the triangle containing pos, or 0 (Sly 1 PcgtPointInCbspQuick; the flags
// argument is new in Sly 2).
// Also known as: ItriNav (upstream readable / Sly 1 name; ItriNav__FP9NAVREGIONG9VU_VECTORi).
CGT *FindNavTriangleQuick(CG *graph, qword pos, int flags) __asm__("FindNavTriangleQuick"); CGT *FindNavTriangleQuick(CG *graph, qword pos, int flags)
{
    union { qword q; float f; } u;
    CBSP *pn, *pnEnd, *anode;
    int mask;
    u.q = QwordFromFloats<qword>(QwordX(pos), QwordY(pos), -1.0f);
    pn = graph->bspNodes;
    pnEnd = &graph->bspNodes[graph->numBspNodes];
    anode = graph->bspNodes;
    mask = (flags & 1) ? 0x8000 : 0x4000;
    for (; pn != pnEnd; ) {
        unsigned int k;
        /* The C file indexed a[cond ? 1 : 0]; C fused the scale into the arms (0 or 2 bytes) and added it to the
           array's address. As C++ the bool condition is not folded that way, so the byte offset and the base are
           spelled out. This form exists to reproduce the original codegen. */
        { int ib = (0.0f < Dot(pn->plane, u.q)) ? 2 : 0; unsigned short *pa = pn->e.child; k = *(unsigned short *)((char *)pa + ib); }
        if (k & 0x8000) {
            if ((k & mask) == 0)
                return 0;
            k &= 0x3FFF;
            return &graph->triangles[k];
        }
        pn = anode + k;
    }
    return 0;
}
} // namespace M_PcgtPointInCbspQuick

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_PcgtPointInCbspSafe {
// Dot product of the xyz parts of a and b as a float.
extern "C" { static __inline__ float Dot(qword a, qword b)
{
    qword d, one;
    float f;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
} }



float D_002C0C00 __asm__("D_002C0C00") = 0.01f;   /* path's .data starts here: the plane-side epsilon */
namespace PATH_DATA { float r_D_002C0C04 __asm__("D_002C0C04") = 50.0f; }   /* FindPathPoints's route spacing (declared const there) */
// Tolerant BSP walk: returns the triangle containing pos or, failing that, the nearest one (Sly 1
// PcgtPointInCbspSafe).
// Also known as: ItriNavFrom (upstream readable / Sly 1 name; ItriNavFrom__FP9NAVREGIONP4CBSPG9VU_VECTORi).
extern "C" CGT *FindNavTriangleSafe(CG *graph, CBSP *node, qword pos, int flags)
{
    union { qword q; float f; } u;
    CBSP *pnEnd, *anode;
    int mask;
    qword q;
    q = QwordFromFloats<qword>(QwordX(pos), QwordY(pos), -1.0f);
    u.q = q;
    pnEnd = &graph->bspNodes[graph->numBspNodes];
    anode = graph->bspNodes;
    mask = (flags & 1) ? 0x8000 : 0x4000;
    while (node != pnEnd) {
        float d = Dot(node->plane, q);
        unsigned int k;
        if (d < -D_002C0C00) {
            k = node->e.child[0];
        } else if (D_002C0C00 < d) {
            k = node->e.child[1];
        } else {
            k = node->e.child[1];
            if (k & 0x8000) {
                if (k & mask) {
                    k &= 0x3FFF;
                    return &graph->triangles[k];
                }
            } else {
                CBSP *pnT = anode + k;
                if (pnT != pnEnd) {
                    CGT *pl = FindNavTriangleSafe(graph, pnT, pos, flags);
                    if (pl)
                        return pl;
                }
            }
            k = node->e.child[0];
        }
        if (k & 0x8000) {
            if (!(k & mask))
                return 0;
            k &= 0x3FFF;
            return &graph->triangles[k];
        }
        node = anode + k;
    }
    return 0;
}
} // namespace M_PcgtPointInCbspSafe

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_CbskFromG__Ff {
// Classifies a signed plane distance: 0 front, 1 back, 2 on the plane (within an epsilon). Sly 1 name: CbskFromG.
extern "C" int ClassifyPlaneDistance__Ff(float distance)
{
extern float D_002C0C00;
    if (D_002C0C00 < distance) return 0;
    if (distance < -D_002C0C00) return 1;
    return 2;
}
} // namespace M_CbskFromG__Ff

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_ClsgClipEdgeToCbsp {
// ClipEdgeToCbsp: walk the segment from -> to (XY only) through the 2D BSP of a navigation graph and write the
// leaf spans it crosses into out[] (at most `capacity` rows); returns the number of rows (wave-4 lane 6).
// Matched siblings: FindNavTriangleQuick / FindNavTriangleSafe (same GRAPH/CBSP layout and Make/GetX/GetY/Dot helpers),
// ClassifyPlaneDistance (plane side test), caller IsEdgeBlockedByCbsp (ROW layout).
// Keys:
//  * The row count is `cursor - out` (ROW = 96 bytes). The exact division by 96 is a multiply by inverse(3) and an
//    arithmetic shift by 5, so loop.c reduces both uses to registers stepping by 32 and only `sra 5` survives.
//  * The two homogeneous endpoints are VU_VECTORs built by a constructor whose asm writes straight into the member,
//    and GetX/GetY take const references: this gives the original's spill-slot order from/fromPlane/to/toPlane.
//  * Lerp returns a VU_VECTOR; the extra struct copies keep the leaf loop above loop.c's hoisting threshold, so the
//    flags & 2 test and the constant 3 stay inside it as in the original.
//  * The child -> state code is written out three times (pushed state, near child after a crossing, child without a
//    crossing); jump2 cross-jumps the identical tails. Only the pushed state clears `leaf` for an interior child.
//  * The pushed plane is stored field by field; cse reuses the stored 0.0f for the `dl <= 0.0f` test.



struct STATE { CBSP *node; CGT *leaf; float t; QWROW plane; };
/* One clipped span: sly2/lsg.h's LSG (its size and layout on every target), with the navigation triangle crossed
   in LSG::apsurf[0]'s slot. */
union ROW { ::LSG lsg; struct { QWROW a, b, planeA, planeB; float t0, t1; int flags; CGT *leaf; }; }; typedef char assert_ROW_leaf[SLY2_OFFSETOF(ROW, leaf) == SLY2_OFFSETOF(::LSG, apsurf) && SLY2_OFFSETOF(ROW, flags) == SLY2_OFFSETOF(::LSG, lsgk) ? 1 : -1];
typedef char STATE_Size[sizeof(STATE) == 0x20 ? 1 : -1];
typedef char ROW_Size[sizeof(ROW) == 0x60 ? 1 : -1];
extern void *func_002333B4(void *, int, unsigned int) __asm__("memset"); // memset
extern int ClassifyPlaneDistance__Ff(float) __asm__("ClassifyPlaneDistance__Ff");                      // 0: above +eps, 1: below -eps, 2: on the plane

// x lane of q as a float.
static inline float GetX(const qword &q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
// y lane of q as a float (prot3w rotates it into the low word).
static inline float GetY(const qword &q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
// volatile (as in many matched VU helpers): keeps both qmfc2 moves after the second dot product.
static inline qword Dot(const qword &a, const qword &b)
{
    qword d, one;
    __asm__ volatile("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
                     : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    return d;
}
// Float broadcast into a VU0 register on construction.
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1,%0" : "=j"(q) : "r"(f)); } };
struct VU_VECTOR {
    qword q;
    // Wrap a qword as a VU_VECTOR.
    VU_VECTOR(const qword &v) { q = v; }
    VU_VECTOR(float x, float y, float z)
    {
        qword t;
        __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
                : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    }
};
// Return v1 * f1 + v2 * f2.
static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC,%2,%1x\n\tvmaddx.xyzw %0,%4,%3x" : "=j"(r) : "j"(f1.q), "j"(v1.q), "j"(f2.q), "j"(v2.q));
    return r;
}
// Linear interpolation (1-t)*a + t*b.
static inline VU_VECTOR Lerp(const qword &a, const qword &b, float t)
{
    return VecCombo(VU_FLOAT(1.0f - t), VU_VECTOR(a), VU_FLOAT(t), VU_VECTOR(b));
}

// Clips a segment against the graph's BSP, writing the walkable leaf spans it crosses as LSG rows; returns the
// count (Sly 1 ClsgClipEdgeToCbsp).
// Also known as: CsegNav (upstream readable / Sly 1 name; CsegNav__FP9NAVREGIONG9VU_VECTORT1iP6NAVSEGi).
int ClipEdgeToCbsp(CG *g, qword from, qword to, int capacity, ROW *out, int flags) __asm__("ClipEdgeToCbsp"); int ClipEdgeToCbsp(CG *g, qword from, qword to, int capacity, ROW *out, int flags)
{
    STATE st[33]; // st[0] is the current state, st[1..32] the stack of pending far sides
    STATE *top;
    ROW *prev = 0;
    ROW *cursor = out;
    CBSP *nodes = g->bspNodes;
    CBSP *end = nodes + g->numBspNodes;
    CGT *leaves = g->triangles;
    int mask = (flags & 1) ? 0x8000 : 0x4000;

    func_002333B4(&st[0], 0, sizeof(STATE));
    st[0].node = nodes;
    top = &st[1];
    func_002333B4(top, 0, sizeof(STATE));
    top->t = 1.0f;
    VU_VECTOR fromPlane(GetX(from), GetY(from), -1.0f);
    VU_VECTOR toPlane(GetX(to), GetY(to), -1.0f);
    for (;;) {
        if (st[0].node == end) {
            do {
                if (st[0].leaf) {
                    if (prev && st[0].t == prev->t1 && ((flags & 2) || st[0].leaf == prev->leaf)) {
                        // extend the previous span
                        prev->b.q = Lerp(from, to, top->t).q;
                        prev->t1 = top->t;
                        prev->planeB = top->plane;
                    } else {
                        float lo, hi;
                        if (cursor - out >= capacity)
                            return capacity;
                        prev = cursor++;
                        func_002333B4(prev, 0, sizeof(ROW));
                        lo = st[0].t;
                        hi = top->t;
                        prev->flags = 3;
                        prev->a.q = Lerp(from, to, lo).q;
                        prev->b.q = Lerp(from, to, hi).q;
                        prev->t0 = lo;
                        prev->t1 = hi;
                        prev->planeA = st[0].plane;
                        prev->planeB = top->plane;
                        prev->leaf = st[0].leaf;
                    }
                }
                if (top <= &st[1])
                    return cursor - out;
                st[0] = *top--;
            } while (st[0].node == end);
        }
        {
            qword q0 = Dot(st[0].node->plane, fromPlane.q);
            qword q1 = Dot(st[0].node->plane, toPlane.q);
            float d0 = VuToFloat(q0);
            float d1 = VuToFloat(q1);
            int s0 = ClassifyPlaneDistance__Ff(d0);
            int s1 = ClassifyPlaneDistance__Ff(d1);
            int neg, pos;
            unsigned int child;
            neg = 0;
            if (s0 == 1 || s1 == 1)
                neg = 1;
            pos = 0;
            if (s0 == 0 || s1 == 0)
                pos = 1;
            if (!neg || !pos) {
                child = st[0].node->e.child[pos];
                if (child & 0x8000) {
                    st[0].node = end;
                    if (child & mask) {
                        child &= 0x3FFF;
                        st[0].leaf = &leaves[child];
                    } else
                        st[0].leaf = 0;
                } else {
                    st[0].node = &nodes[child];
                }
            } else {
                // the segment crosses the plane: split at `cross`, push the far side
                float cross = d0 / (d0 - d1);
                float lo = st[0].t, hi = top->t;
                float dl = lo * d1 + (1.0f - lo) * d0;
                float dh = hi * d1 + (1.0f - hi) * d0;
                int nearNeg;
                if (lo < cross && cross < hi) {
                    unsigned int c;
                    top++;
                    top->t = cross;
                    top->plane.f[0] = st[0].node->e.x;
                    top->plane.f[1] = st[0].node->e.y;
                    top->plane.f[2] = 0.0f;
                    top->plane.f[3] = st[0].node->e.z;
                    c = st[0].node->e.child[dl <= 0.0f];
                    if (c & 0x8000) {
                        top->node = end;
                        if (c & mask) {
                            c &= 0x3FFF;
                            top->leaf = &leaves[c];
                        } else
                            top->leaf = 0;
                    } else {
                        top->leaf = 0;
                        top->node = &nodes[c];
                    }
                }
                nearNeg = 0;
                if (dl < 0.0f || (dl == 0.0f && dh <= 0.0f))
                    nearNeg = 1;
                child = st[0].node->e.child[!nearNeg];
                if (child & 0x8000) {
                    st[0].node = end;
                    if (child & mask) {
                        child &= 0x3FFF;
                        st[0].leaf = &leaves[child];
                    } else
                        st[0].leaf = 0;
                } else {
                    st[0].node = &nodes[child];
                }
            }
        }
    }
}
} // namespace M_ClsgClipEdgeToCbsp

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_FClipEdgeToCbsp {



union ROW{::LSG lsg;struct{QWROW begin,end,beginPlane,endPlane;float low,high;int flags;CGT*leaf;};};   /* sly2/lsg.h LSG, the triangle in apsurf[0]'s slot */
extern int ClipEdgeToCbsp(CG*,qword,qword,int,ROW*,int) __asm__("ClipEdgeToCbsp");
typedef char U_Size[sizeof(QWROW)==16?1:-1];
typedef char ROW_Size[sizeof(ROW)==96?1:-1];
typedef char ROW_Low_Offset[(unsigned int)&((ROW*)0)->low==0x40?1:-1];
typedef char ROW_High_Offset[(unsigned int)&((ROW*)0)->high==0x44?1:-1];
// Returns 1 when the segment leaves walkable space (no span, or the span does not cover [0, 1]).
// Also known as: FNavBlocked (upstream readable / Sly 1 name; FNavBlocked__FP9NAVREGIONG9VU_VECTORT1i).
int IsEdgeBlockedByCbsp(CG*g,qword from,qword to,int flags) __asm__("IsEdgeBlockedByCbsp"); int IsEdgeBlockedByCbsp(CG*g,qword from,qword to,int flags){
 ROW spans[2];
 int count=ClipEdgeToCbsp(g,from,to,2,spans,flags|2);
 int result=0;
 if(!count||spans[0].low!=0.0f||spans[0].high!=1.0f)result=1;
 return result;
}
} // namespace M_FClipEdgeToCbsp

// Index of a graph vertex from its pointer.
int GetCgVertexIndex(CG *graph, CGV *vertex)
{
    return vertex - graph->acgv;
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_FindPathAStar {
// Allocation request: byte count and alignment.
struct AREQ { int cb; int align; AREQ(int c, int a) { cb = c; align = a; } };




struct NODE { CGV *pv; float f; float g; NODE *parent; int depth; NODE *next; };

void *AllocFromStack(const AREQ &r) __asm__("AllocFromStack");
void *AllocClearedFromStack(const AREQ &r) __asm__("AllocClearedFromStack");


// Allocate cb bytes, 16-byte aligned, from the frame stack.
static inline void *Alloc(int cb) { return AllocFromStack(AREQ(cb, 0x10)); }
// Allocate cb zeroed bytes, 16-byte aligned, from the frame stack.
static inline void *AllocB(int cb) { return AllocClearedFromStack(AREQ(cb, 0x10)); }
// Distance between a and b in the XY plane.
static inline float Dist2(const qword &a, const qword &b)
{
    qword d;
    float r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(r) : "j"(d));
    return r;
}

// A* search over the graph's vertices from a start to a goal triangle; fills the predecessor chain. Sly 1 name.
// Also known as: AstarNav (upstream readable / Sly 1 name; AstarNav__FP9NAVREGIONPvT1iPiT1).
extern "C" void FindPathAStar(CG *graph, CGV *start, CGV *goal, int maxVertices, int *count, CGV **path)
{
    NODE node;
    STACKSCOPE ctx;
    int *afClosed = (int *)AllocB(graph->ccgv << 2);
    NODE *pnHead;
    NODE *pn;
    path[0] = start;
    pnHead = 0;
    node.depth = 1;
    node.pv = start;
    node.f = Dist2(start->pos, goal->pos);
    node.g = 0.0f;
    node.parent = 0;
    pn = &node;
    if (start != goal) {
        do {
            CGV *pvCur = pn->pv;
            int i;
            for (i = 0; i < pvCur->numEdges; i++) {
                CGE *pe = &pvCur->edges[i];
                CGV *pv = pe->pcgv;
                int fSkip;
                NODE *pnNew, **ppn, *pnT;
                if (pv == goal) {
                    fSkip = 0;
                } else {
                    int iv = GetCgVertexIndex(graph, pv);
                    if (afClosed[iv] == 0) {
                        afClosed[iv] = 1;
                        fSkip = 0;
                    } else {
                        fSkip = 1;
                    }
                }
                if (fSkip)
                    continue;
                pnNew = (NODE *)Alloc(0x18);
                pnNew->pv = pv;
                pnNew->g = pn->g + pe->cost;
                pnNew->f = pnNew->g + Dist2(pvCur->pos, goal->pos);
                pnNew->parent = pn;
                pnNew->depth = pn->depth + 1;
                ppn = &pnHead;
                for (pnT = pnHead; pnT; pnT = pnT->next) {
                    if (pnNew->f < pnT->f)
                        break;
                    ppn = &pnT->next;
                }
                pnNew->next = pnT;
                *ppn = pnNew;
            }
            pn = pnHead;
            if (!pn)
                break;
            pnHead = pn->next;
        } while (pn->pv != goal);
    }
    if (pn) {
        *count = pn->depth;
        for (; pn; pn = pn->parent)
            path[pn->depth - 1] = pn->pv;
    } else {
        *count = 0;
    }
}
} // namespace M_FindPathAStar

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_SimplifyApcgvNeighbor {
struct CTX { int a[4]; };
void STACKSCOPE_ctor(struct CTX *c) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(struct CTX *c, int n) __asm__("_$_10STACKSCOPE");
void *AllocFromStack(struct SWALLOC *r) __asm__("AllocFromStack");
void CopyAb__FPvT0Ui(void *dst, void *src, int cb) __asm__("CopyAb__FPvT0Ui");
int IsEdgeBlockedByCbsp(void *ctx, qword a, qword b, int n) __asm__("IsEdgeBlockedByCbsp");
// Fill the request and allocate cb bytes (alignment nKind) from the frame stack.
extern "C" { static __inline__ void *PvAllocReq(struct SWALLOC *preq, int cb, int nKind) { preq->cb = cb; preq->cbAlign = nKind; return AllocFromStack(preq); } }
// Drops intermediate path vertices that can be skipped with a clear line of sight (IsEdgeBlockedByCbsp).
// Also known as: SmoothNav (upstream readable / Sly 1 name; SmoothNav__FP9NAVREGIONPiPPG9VU_VECTOR).
extern "C" void SimplifyPathByLineOfSight(void *ctx, int *count, qword **points)
{
    struct CTX c;
    struct SWALLOC req;
    qword **tmp, **prev, **cur, **next, **end;
    int n, k;
    STACKSCOPE_ctor(&c);
    n = *count;
    end = points + n;
    tmp = (__typeof__(tmp))PvAllocReq(&req, n * (int)sizeof(qword *), 0x10);
    tmp[0] = points[0];
    k = 1;
    prev = points;
    cur = points + 1;
    next = points + 2;
    while (next < end) {
        if (IsEdgeBlockedByCbsp(ctx, **prev, **next, 0)) {
            tmp[k] = *cur;
            prev = cur;
            cur = next++;
            k++;
        } else {
            cur = next++;
        }
    }
    tmp[k] = points[n - 1];
    k++;
    if (k < n && k >= 3) SimplifyPathByLineOfSight(ctx, &k, tmp);
    CopyAb__FPvT0Ui(points, tmp, k * (int)sizeof(qword *));
    *count = k;
    STACKSCOPE_ctor_1AE4A0(&c, 2);
}
} // namespace M_SimplifyApcgvNeighbor

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_ClosestPointOnCgt {
// GetClosestPointOnNavTriangle: closest point of a navigation triangle to p (wave-3 lane 4). T holds pointers to the three corners.
// If p lies outside an edge in the XY plane (z of cross(p - v0, v1 - v0) > 0, tested a-b, b-c, c-a in turn), the result
// is the closest point on that edge (FindClosestPointOnLineSegment); otherwise p is projected along g_normalZ (world up) onto the
// triangle's plane.
// Keys:
//  * The inside block works on VU_VECTOR copies of the first and third corners made right after the loads. The
//    const-reference conversion constructor makes them struct pass-through copies that cse1 and gcse's copy propagation
//    leave alone, so they are live together with a/c through the edge tests and get their own VU registers: the
//    original's `vmove vf8,vf6` / `vmove vf7,vf3` (plain `qword aa = a` copies are always propagated away).
//  * The plane normal's z is read with pextuw on an "r" input and the dot product with mtc1 on an "r" input, so gcc
//    emits the qmfc2 moves itself as reloads. Then the inside block needs only the v0/v1 spill registers, which keeps
//    reload's round-robin choice for the three edge tests at v1, v0, v1, and lets sched2 put the mtc1 before the lqc2.
union U { qword q; float f[4]; };

// Wrap a qword as a VU_VECTOR.
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };

extern qword g_normalZ __asm__("g_normalZ"); // (0, 0, 1, 0)
void FindClosestPointOnLineSegment(qword pos, qword p0, qword p1, qword *ppos, float *pt, float *pdist) __asm__("FindClosestPointOnLineSegment");
extern "C" { qword GetClosestPointOnNavTriangle(CGT *t, qword p); }

// z lane of a as a float.
static inline float Z(qword a) { float z; __asm__("qmfc2.ni %0,%1\n\tpextuw %0,$zero,%0" : "=r"(z) : "j"(a)); return z; }
// Dot product of the xyz parts of a and b (x lane).
static inline qword Dot(qword a, qword b)
{
    qword d = a, one;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "0"(d), "j"(b));
    return d;
}
// Return a + b * g.
static inline qword AddScaled(qword a, qword b, float g)
{
    U u; qword s, r;
    __asm__("qmtc2.ni %1,%0" : "=j"(s) : "r"(g));
    u.q = s;
    __asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x" : "=j"(r) : "j"(a), "j"(b), "j"(u.q));
    return r;
}

// Closest point of a navigation triangle to p (clamped to an edge, or projected vertically inside it).
// Also known as: PosNearestTri (upstream readable / Sly 1 name; PosNearestTri__FP9NAVREGIONG9VU_VECTOR).
extern "C" qword GetClosestPointOnNavTriangle(CGT *triangle, qword pos)
{
    U result;
    qword a = triangle->vertices[0]->pos, b = triangle->vertices[1]->pos, c = triangle->vertices[2]->pos;
    qword ab = VuSub(b, a);
    VU_VECTOR va(a), vc(c);
    if (0.0f < Z(VuCross(VuSub(pos, a), ab))) FindClosestPointOnLineSegment(pos, a, b, &result.q, 0, 0);
    else if (0.0f < Z(VuCross(VuSub(pos, b), VuSub(c, b)))) FindClosestPointOnLineSegment(pos, b, c, &result.q, 0, 0);
    else if (0.0f < Z(VuCross(VuSub(pos, c), VuSub(a, c)))) FindClosestPointOnLineSegment(pos, c, a, &result.q, 0, 0);
    else {
        qword n = VuCross(ab, VuSub(vc.q, va.q));
        qword dv = Dot(VuSub(va.q, pos), n);
        float nz = QwordZ(n);
        float dot = QwordX(dv);
        float g = dot / nz;
        result.q = AddScaled(pos, g_normalZ, g);
    }
    return result.q;
}
} // namespace M_ClosestPointOnCgt

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_CposFindPath {
// FindPathPoints: build a route between two points over the navigation graph (wave-3 lane 4, from w2lane6.v1).
// Finds the triangles containing both points, projects the points onto them (GetClosestPointOnNavTriangle), links a temporary start
// vertex to the start triangle's corners and the goal vertex to the goal triangle's corners, runs the path search
// (FindPathAStar) and the path simplifier (SimplifyPathByLineOfSight), copies the route out, drops points closer than the minimum
// spacing, and returns a two-point route (offset along +X) when only one point is left.
// Keys of the last fixes (tail block):
//  * D_002C0C04 is a `const float` read by value through VU_FLOAT(const float) whose qmtc2 input has a never-chosen "V"
//    alternative: the inliner substitutes the global's unchanging MEM for the const parameter, so the word load is an
//    input reload (lui v1 / lw v1) and the read has no anti-dependence on the out[1] store. That drops the qmtc2 asm's
//    sched1 priority below %hi(D_002A2DA0), which gives the axis address v0 in local-alloc, as in the original.







// Allocation request; Set(n) fills it for n bytes, 16-byte aligned.
struct AREQ { int cb, align; AREQ() {} AREQ *Set(int n) { cb = n; align = 16; return this; } };
extern const float D_002C0C04 __asm__("D_002C0C04"); // minimum spacing between route points (50.0f)
extern qword D_002A2DA0 __asm__("D_002A2DA0");       // (1, 0, 0, 0)

CGT *FindNavTriangleSafe(CG *, CBSP *, qword, int) __asm__("FindNavTriangleSafe");
qword GetClosestPointOnNavTriangle(CGT *, qword) __asm__("GetClosestPointOnNavTriangle");
void STACKSCOPE_ctor(void *) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(void *, int) __asm__("_$_10STACKSCOPE");
void *AllocFromStack(AREQ *) __asm__("AllocFromStack");
void FindPathAStar(CG *, CGV *, CGV *, int, int *, CGV **) __asm__("FindPathAStar");
void SimplifyPathByLineOfSight(CG *, int *, CGV **) __asm__("SimplifyPathByLineOfSight");
int FindPathPoints(CG *, qword, qword, int, QWROW *) __asm__("FindPathPoints");

// the unused second output keeps gcse from hoisting the qmtc2 out of the loops (not single_set)
static inline qword ToVF(qword v) { qword r; int dummy; __asm__("qmtc2.ni %2,%0" : "=j"(r), "=r"(dummy) : "r"(v)); return r; }
// Distance between a and b in the XY plane, returned as raw float bits.
static inline int Dist2DI(qword a, qword b)
{
    qword d; int r;
    __asm__("vsub.xyzw %0,%1,%2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(r) : "j"(d));
    return r;
}
// Distance between a and b in the XY plane.
static inline float Dist2D(qword a, qword b)
{
    qword d; float r;
    __asm__("vsub.xyzw %0,%1,%2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(r) : "j"(d));
    return r;
}
struct VU_FLOAT {
    qword q;
    // never-chosen "V" alternative: allocation-only (the word stays a MEM until reload)
    VU_FLOAT(const float f) { __asm__("qmtc2.ni %1, %0" : "=j,j"(q) : "r,V"(f)); }
};
// Return p + axis * s.
static inline qword AddScaled(qword p, qword axis, VU_FLOAT s)
{
    qword r;
    __asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x" : "=j"(r) : "j"(p), "j"(axis), "j"(s.q));
    return r;
}

// Finds a route between two points over the graph: locates both triangles, runs A*, simplifies, copies at most
// cposMax points out with a minimum spacing. Returns the point count (Sly 1 CposFindPath).
// Also known as: CposNavPath (upstream readable / Sly 1 name; CposNavPath__FP9NAVREGIONG9VU_VECTORT1iP7VECTOR4).
int FindPathPoints(CG *graph, qword from, qword to, int capacity, QWROW *out)
{
    CGT *first = FindNavTriangleSafe(graph, graph->bspNodes, from, 1);
    if (!first) return 0;
    qword start = GetClosestPointOnNavTriangle(first, from);
    CGT *last = FindNavTriangleSafe(graph, graph->bspNodes, to, 1);
    if (!last) return 0;
    qword finish = GetClosestPointOnNavTriangle(last, to);
    QWROW *write;
    int count;
    if (first == last) {
        out[0].q = from;
        out[1].q = start;
        count = 3;
        write = out + 1;
        out[2].q = finish;
    } else {
        STACKSCOPE mark;
        write = out + 1;
        CGV **path = (CGV **)AllocFromStack(AREQ().Set(0x200));
        CGV initial;
        initial.pos = start;
        initial.numEdges = 3;
        initial.edges = (CGE *)AllocFromStack(AREQ().Set(0x18));
        qword sv = ToVF(start);
        for (int i = 0; i < 3; i++) {
            CGV *v = first->vertices[i];
            CGE *e = &initial.edges[i];
            e->pcgv = v;
            register int c asm("$4"); // the single pin: the original keeps this qmfc2 result in a0
            c = Dist2DI(sv, v->pos);
            *(volatile int *)&e->cost = c;   /* the distance's float bits through a GPR; volatile: dbr keeps the store out of the loop branch delay slot */
        }
        CGV goal;
        goal.pos = finish;
        goal.numEdges = 0;
        goal.edges = 0;
        qword fv = ToVF(finish);
        CGV *pg = &goal;
        for (int i = 0; i < 3; i++) {
            CGV *v = last->vertices[i];
            int n = v->numEdges;
            v->numEdges = n + 1;
            CGE *e = v->edges + n;
            e->pcgv = pg;
            *(volatile int *)&e->cost = Dist2DI(v->pos, fv);
        }
        int found;
        FindPathAStar(graph, &initial, pg, 128, &found, path);
        if (found >= 3) SimplifyPathByLineOfSight(graph, &found, path);
        int n = found; if (n > capacity) n = capacity; count = n;
        for (int i = 0; i < count; i++) out[i].q = path[i]->pos;
        for (int i = 0; i < 3; i++) last->vertices[i]->numEdges--;
    }
    {
        QWROW *end = out + count;
        qword prev = out[0].q;
        QWROW *rd = write;
        QWROW *wr = write;
        if (wr < end) {
            float thr = D_002C0C04;
            do {
                int accept = thr < Dist2D(rd->q, prev);
                if (accept) {
                    QWROW *s = rd++;
                    QWROW *d = wr++;
                    prev = s->q;
                    d->q = prev;
                } else rd++;
            } while (rd < end);
        }
        count = wr - out;
    }
    if (count == 1) {
        qword p = out[0].q;
        VU_FLOAT s(D_002C0C04);
        out[1].q = p;
        count = 2;
        out[0].q = AddScaled(p, D_002A2DA0, s);
    }
    return count;
}
} // namespace M_CposFindPath

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C02D0 {

int FindPathPoints(CG *graph, qword from, qword to, int capacity, QWROW *out) __asm__("FindPathPoints");
void MeasureShape(SHAPE *path) __asm__("MeasureShape");
// Finds a path (max 16 points) in a path zone into a path curve object and rebuilds the curve; returns 0 when
// fewer than 2 points were found.
// Also known as: FNavPathCrv (upstream readable / Sly 1 name; FNavPathCrv__FP9NAVREGIONiiP3CRV).
extern "C" int FindPathzonePathCurve(PATHZONE *zone, qword from, qword to, SHAPE *path)
{
    int n = FindPathPoints(&zone->cg, from, to, 0x10, path->vertexPosq);
    if (n < 2) return 0;
    path->numControlVertices = n;
    MeasureShape(path);
    return 1;
}
} // namespace M_func_001C02D0

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_FindClosestPointInCg {

CGT *FindNavTriangleSafe(CG *graph, CBSP *node, qword pos, int flags) __asm__("FindNavTriangleSafe");
qword GetClosestPointOnNavTriangle(CGT *triangle, qword q) __asm__("GetClosestPointOnNavTriangle");
// Closest point of the graph to pos (nearest triangle, then the closest point on it); returns 1 when found.
// Also known as: FNavPoint (upstream readable / Sly 1 name; FNavPoint__FP9NAVREGIONG9VU_VECTORP6VECTOR).
int FindClosestPointInCg(CG *graph, qword pos, qword *closest, int flags) __asm__("FindClosestPointInCg"); int FindClosestPointInCg(CG *graph, qword pos, qword *closest, int flags)
{
    CGT *pv = FindNavTriangleSafe(graph, graph->bspNodes, pos, flags);
    if (!pv) return 0;
    *closest = GetClosestPointOnNavTriangle(pv, pos);
    return 1;
}
} // namespace M_FindClosestPointInCg

// CID 17 slot 7 (Init): InitXfm, ids -1 and the obstacle list.
void PATHZONE::Init()
{
    PATHZONE *ppathzone = this;
    InitXfm(ppathzone);
    ppathzone->unknown110 = -1;
    InitDl(&ppathzone->dlRegent, SLY2_OFFSETOF(REGENT, dlePathzone));
    ppathzone->zoneIndex = -1;
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_LoadPathzoneFromBrx {
struct A { int cb; int align; };







void LoadXfmFromBrx(PATHZONE *p, void *pbis) __asm__("LoadXfmFromBrx");
int U16Read__18CBinaryInputStream(void *pbis) __asm__("U16Read__18CBinaryInputStream");
int U8Read__18CBinaryInputStream(void *pbis) __asm__("U8Read__18CBinaryInputStream");
void ReadVector__18CBinaryInputStreamP6VECTOR(void *pbis, qword *pv) __asm__("ReadVector__18CBinaryInputStreamP6VECTOR");
float F32Read__18CBinaryInputStream(void *pbis) __asm__("F32Read__18CBinaryInputStream");
void *AllocClearedFromWorldHeap(struct A *pa) __asm__("AllocClearedFromWorldHeap");
void *AllocFromWorldHeap(struct A *pa) __asm__("AllocFromWorldHeap");
void ComputeCgEdgeCosts(CG *p) __asm__("ComputeCgEdgeCosts");
// CID 17 slot 14 (LoadFromBrx): the XFM, the vertices (position + edge list), triangles and BSP nodes; then
// ComputeCgEdgeCosts. Layout parse-verified: a parser following it reads every byte of 4 levels.
// Also known as: LoadMeshZoneFromBrx (upstream readable / Sly 1 name; LoadMeshZoneFromBrx__FP2LOP18CBinaryInputStream).
extern "C" void LoadPathzoneFromBrx(PATHZONE *zone, void *stream)
{
    struct A a;
    int i, j, k;
    LoadXfmFromBrx(zone, stream);
    zone->cg.ccgv = U16Read__18CBinaryInputStream(stream);
    a.cb = zone->cg.ccgv * (int)sizeof(CGV);
    a.align = 0x10;
    zone->cg.acgv = (__typeof__(zone->cg.acgv))AllocClearedFromWorldHeap(&a);
    zone->cg.numTriangles = U16Read__18CBinaryInputStream(stream);
    a.cb = zone->cg.numTriangles * (int)sizeof(CGT);
    a.align = 0x10;
    zone->cg.triangles = (__typeof__(zone->cg.triangles))AllocClearedFromWorldHeap(&a);
    for (i = 0; i < zone->cg.ccgv; i++) {
        CGV *pd = &zone->cg.acgv[i];
        ReadVector__18CBinaryInputStreamP6VECTOR(stream, &pd->pos);
        pd->numEdges = U8Read__18CBinaryInputStream(stream);
        a.cb = pd->numEdges * (int)sizeof(CGE) + (int)sizeof(CGE);   /* one spare entry */
        a.align = 0x10;
        pd->edges = (__typeof__(pd->edges))AllocFromWorldHeap(&a);
        for (j = 0; j < pd->numEdges; j++) {
            CGE *pe = &pd->edges[j];
            pe->pcgv = &zone->cg.acgv[U16Read__18CBinaryInputStream(stream)];
        }
    }
    for (k = 0; k < zone->cg.numTriangles; k++) {
        CGV **ppv = zone->cg.triangles[k].vertices;
        for (i = 0; i < 3; i++)
            *ppv++ = &zone->cg.acgv[U16Read__18CBinaryInputStream(stream)];
    }
    zone->cg.numBspNodes = U16Read__18CBinaryInputStream(stream);
    a.cb = zone->cg.numBspNodes * (int)sizeof(CBSP);
    a.align = 0x10;
    zone->cg.bspNodes = (__typeof__(zone->cg.bspNodes))AllocClearedFromWorldHeap(&a);
    for (i = 0; i < zone->cg.numBspNodes; i++) {
        CBSP *pp = &zone->cg.bspNodes[i];
        pp->e.x = F32Read__18CBinaryInputStream(stream);
        pp->e.y = F32Read__18CBinaryInputStream(stream);
        pp->e.z = F32Read__18CBinaryInputStream(stream);
        pp->e.child[0] = U16Read__18CBinaryInputStream(stream);
        pp->e.child[1] = U16Read__18CBinaryInputStream(stream);
    }
    ComputeCgEdgeCosts(&zone->cg);
    if (zone->oid == 0x2CA)
        g_sw.pcid17Default = (CID17 *)zone;
    zone->version = ++g_sw.pathzoneVersion;
}
} // namespace M_LoadPathzoneFromBrx

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_InitDefaultPathzoneCg {
struct A { int cb; int align; };


struct VS { union { float af[4]; qword q; } pos; int c; int ai[3]; };   /* floats first: initializable */

struct FS { int ai[3]; };


/* The default navigation graph (path's .data): four corner vertices of a 200000-unit square, each with its
 * neighbour indices (-1: unused), and its two triangles. */
struct VS D_002C0C10[4] __asm__("D_002C0C10") = {
    {{{-100000.0f, -100000.0f, 0.0f, 0.0f}}, 3, {1, 2, 3}},
    {{{100000.0f, -100000.0f, 0.0f, 0.0f}}, 2, {0, 2, -1}},
    {{{100000.0f, 100000.0f, 0.0f, 0.0f}}, 3, {0, 1, 3}},
    {{{-100000.0f, 100000.0f, 0.0f, 0.0f}}, 2, {0, 2, -1}},
};
struct FS D_002C0C90[2] __asm__("D_002C0C90") = {{{0, 1, 2}}, {{0, 2, 3}}};
void *AllocClearedFromWorldHeap(struct A *pa) __asm__("AllocClearedFromWorldHeap");
void *AllocFromWorldHeap(struct A *pa) __asm__("AllocFromWorldHeap");
void ComputeCgEdgeCosts(CG *p) __asm__("ComputeCgEdgeCosts");
// Builds a trivial graph for a level without zones: 4 vertices, 2 triangles and one diagonal BSP node from tables.
extern "C" void InitDefaultPathzoneCg(PATHZONE *zone)
{
    struct A a;
    int i, j, k;
    CBSP *pp;
    float g;
    zone->cg.ccgv = 4;
    a.cb = 4 * (int)sizeof(CGV);
    a.align = 0x10;
    zone->cg.acgv = (__typeof__(zone->cg.acgv))AllocClearedFromWorldHeap(&a);
    for (i = 0; i < zone->cg.ccgv; i++) {
        struct VS *ps = &D_002C0C10[i];
        CGV *pd = &zone->cg.acgv[i];
        pd->pos = ps->pos.q;
        pd->numEdges = ps->c;
        a.cb = pd->numEdges * (int)sizeof(CGE) + (int)sizeof(CGE);   /* one spare entry */
        a.align = 0x10;
        pd->edges = (__typeof__(pd->edges))AllocFromWorldHeap(&a);
        for (j = 0; j < pd->numEdges; j++)
            { CGE *pe = &pd->edges[j]; pe->pcgv = &zone->cg.acgv[ps->ai[j]]; }
    }
    zone->cg.numTriangles = 2;
    a.cb = 2 * (int)sizeof(CGT);
    a.align = 0x10;
    zone->cg.triangles = (__typeof__(zone->cg.triangles))AllocClearedFromWorldHeap(&a);
    for (k = 0; k < zone->cg.numTriangles; k++) {
        CGV **ppv = zone->cg.triangles[k].vertices;
        int *pi = D_002C0C90[k].ai;
        for (i = 0; i < 3; i++)
            *ppv++ = &zone->cg.acgv[*pi++];
    }
    zone->cg.numBspNodes = 1;
    a.cb = (int)sizeof(CBSP);
    a.align = 0x10;
    pp = (__typeof__(pp))AllocClearedFromWorldHeap(&a);
    zone->cg.bspNodes = pp;
    g = SqrtFloat(2.0f) * 0.5f;
    pp->e.child[0] = 0xC001;
    pp->e.child[1] = 0xC000;
    pp->e.z = 0;
    pp->e.x = g;
    pp->e.y = -g;
    ComputeCgEdgeCosts(&zone->cg);
    zone->version = ++g_sw.pathzoneVersion;
}
} // namespace M_InitDefaultPathzoneCg

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_HookupCg {



// Distance between a and b in the XY plane, returned as raw float bits.
extern "C" { static inline int Dist(qword a, qword b)
{
    qword d;
    register int f __asm__("$2");
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0, %1, %1" : "=j"(d) : "j"(d));
    __asm__("vaddy.x %0, %1, %1y" : "=j"(d) : "j"(d));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
} }
// Computes the graph's edge costs (XY distance between connected vertices). Sly 1 name: HookupCg.
extern "C" void ComputeCgEdgeCosts(CG *graph)
{
    int i, j;
    for (i = 0; i < graph->ccgv; i++) {
        CGV *pe = &graph->acgv[i];
        for (j = 0; j < pe->numEdges; j++) {
            register CGE *field __asm__("$3") = pe->edges;
            field += j;
            *(int *)&field->cost = Dist(pe->pos, field->pcgv->pos);   /* the float bits through a GPR (Dist) */
        }
    }
}
} // namespace M_HookupCg

// Returns 1 when pos lies on a walkable triangle of the zone.
extern "C" int IsPosInPathzone(PATHZONE *zone, qword pos, int flags)
{
    return FindNavTriangleQuick(&zone->cg, pos, flags) != 0;
}

// Closest walkable point of the zone to pos.
extern "C" int FindPathzoneClosestPoint(PATHZONE *zone, qword pos, qword *closest, int flags)
{
    return FindClosestPointInCg(&zone->cg, pos, closest, flags);
}

// Clips a segment against the zone's walkable space (ClipEdgeToCbsp on the zone's graph).
extern "C" int ClipEdgeToPathzone(PATHZONE *zone, qword from, qword to, int maxSegments, void *segments, int flags)
{
    return ClipEdgeToCbsp(&zone->cg, from, to, maxSegments, segments, flags);
}

// Returns 1 when a segment leaves the zone's walkable space.
extern "C" int IsEdgeBlockedByPathzone(PATHZONE *zone, qword from, qword to, int flags)
{
    return IsEdgeBlockedByCbsp(&zone->cg, from, to, flags);
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C09A0 {
int FindPathzoneObstacles(PATHZONE *p, int a, qword q0, qword q1, int c, void **ap) __asm__("FindPathzoneObstacles");
int IsPosInConvexObstacle(void *pv, qword q) __asm__("IsPosInConvexObstacle");
// Returns 1 when a point is walkable in the zone and not inside any registered obstacle.
extern "C" int IsPosFreeInPathzone(PATHZONE *zone, qword pos,int flags)
{
    void *ap[0x80];
    int n, i;
    if (!FindNavTriangleQuick(&zone->cg,pos,flags)) return 0;
    n = FindPathzoneObstacles(zone, 0, pos, pos, 0x80, ap);
    for (i = 0; i < n; i++) {
        if (IsPosInConvexObstacle(ap[i], pos)) return 0;
    }
    return 1;
}
} // namespace M_func_001C09A0

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C0A40 {
struct S { int a[0x30]; };
void *ClipEdgeToPathzone(void *p, qword a, qword b, int c, int d, int e) __asm__("ClipEdgeToPathzone");
int FindPathzoneObstacles(void *p, int z, qword a, qword b, int c, void **ap) __asm__("FindPathzoneObstacles");
void *ClipEdgeToConvexObstacle(void *p, qword a, qword b, int n, struct S *ps) __asm__("ClipEdgeToConvexObstacle");
void *MergeSegmentLists(void *p, int d, void *r, struct S *ps, int c, int d2, int one) __asm__("MergeSegmentLists");
// Clips a segment to the zone, then against its obstacles; returns the blocking obstacle (or 0) and the clipped end.
// (inferred)
// Also known as: AtFreeNav (upstream readable / Sly 1 name; AtFreeNav__FP9NAVREGIONG9VU_VECTORT1ii).
extern "C" void *FindPathzoneSegmentBlocker(void *zone, qword from, qword to, int c, int d)
{
    void *ap[0x80];
    struct S s;
    void *pv;
    int n, i;
    pv = ClipEdgeToPathzone(zone, from, to, c, d, 2);
    if (!pv) return 0;
    n = FindPathzoneObstacles(zone, 0, from, to, 0x80, ap);
    for (i = 0; i < n; i++) {
        void *r = ClipEdgeToConvexObstacle(ap[i], from, to, 2, &s);
        pv = MergeSegmentLists(pv, d, r, &s, c, d, 1);
        if (!pv) goto out;   /* not `break`: that changes the loop's codegen */
    }
out:
    return pv;
}
} // namespace M_func_001C0A40

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A76DC __asm__("D_002A76DC") = 3.40282347e+38f; }
// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C0B50 {
union U {qword q;float f[4];};


extern const float D_002A76DC __asm__("D_002A76DC");   /* defined below where the ELF has it (.rodata order) */

qword GetClosestPointOnNavTriangle(CGT*,qword) __asm__("GetClosestPointOnNavTriangle");
int FindPathzoneObstacles(PATHZONE*,void*,qword,qword,int,void**) __asm__("FindPathzoneObstacles");
int IsPosInConvexObstacle(void*,qword) __asm__("IsPosInConvexObstacle");
qword ConvertPosBetweenLos(void*,void*,qword) __asm__("ConvertPosBetweenLos");
int ClipEdgeToPathzone(PATHZONE*,qword,qword,int,LSG*,int) __asm__("ClipEdgeToPathzone");
void FindClosestPointOnLineSegment(qword,qword,qword,qword*,float*,float*) __asm__("FindClosestPointOnLineSegment");

// Spill vertex to home and store point's z into home->f[2]; returns that z as a float.
extern "C" { static inline float InitialEndpoint(union U*home,qword vertex,qword point){register qword word __asm__("$3");register float coordinate __asm__("$f0");__asm__("pextuw %0,$zero,%2\n\tsq %3,%1":"=&r"(word),"=m"(home->q):"r"(point),"r"(vertex));__asm__("mtc1 %2,%0\n\tswc1 %0,%1":"=f"(coordinate),"=m"(home->f[2]):"r"(word));return coordinate;} }
// Load p->numControlVertices.
extern "C" { static inline int InitialCount(const CID14*p){int value;__asm__("lw %0,%1":"=r"(value):"m"(p->numControlVertices));return value;} }
// Closest point to pos that is walkable in the zone and outside every obstacle.
// Also known as: FKeepOnNav (upstream readable / Sly 1 name; FKeepOnNav__FP9NAVREGIONG9VU_VECTORP6VECTOR).
extern "C" int FindPathzoneClosestFreePoint(PATHZONE*zone,qword point,qword*out){
 void*objects[128];
 union U a,b;
 union {qword q;int i[4];}best;
 LSG hits[1];
 union U nearest;
 float distance;
 CGT*leaf;CID14*poly;
 qword projected,first;
 int n,i;
 float z,bestDistance;
 leaf=FindNavTriangleQuick(&zone->cg,point,1);
 if(!leaf)return 0;
 projected=GetClosestPointOnNavTriangle(leaf,point);poly=0;
 {int j=0;
 n=FindPathzoneObstacles(zone,0,projected,projected,128,objects);
 for(;j<n;j++){if(IsPosInConvexObstacle(objects[j],point)){poly=(__typeof__(poly))objects[j];break;}}
 }
 if(!poly){*out=projected;return 1;}
 best.i[2]=0;best.i[1]=0;best.i[0]=0;
 bestDistance=D_002A76DC;
 first=ConvertPosBetweenLos(poly,0,poly->vertexPosq[0].q);
 z=InitialEndpoint(&a,first,projected);
 i=1;if(i<InitialCount(poly)){do{
  int c;qword*h;
  b.q=ConvertPosBetweenLos(poly,0,poly->vertexPosq[i].q);b.f[2]=z;
  c=ClipEdgeToPathzone(zone,a.q,b.q,1,hits,2);
  {int k;h=&hits[0].apos[1];for(k=0;k<c;k++){
   FindClosestPointOnLineSegment(projected,h[-1],h[0],&nearest.q,0,&distance);
   if(distance<bestDistance){bestDistance=distance;*(qword*)&best=nearest.q;}
   h=(qword*)((char*)h+sizeof(LSG));
  }}
  a=b;i++;
 }while(i<poly->numControlVertices);}
 FindPathzoneClosestPoint(zone,*(qword*)&best,out,0);
 return 1;
}



} // namespace M_func_001C0B50

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_PpathzoneFromPos {
typedef union { qword q; float f; } U;
void InitVuTemp(void *p) __asm__("InitVuTemp");
// 1 if pos is strictly in front of the plane (plane.xyz . pos - plane.w > 0), else 0.
extern "C" { static __inline__ int ISide(qword plane, qword pos)
{
    U d, one;
    qword td, tone;
    float dist;
    int i;
    InitVuTemp(&d);
    InitVuTemp(&one);
    i = 1;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %2w" : "=j"(td), "=&j"(tone) : "j"(plane), "j"(pos));
    __asm__("mtc1 %1, %0" : "=f"(dist) : "r"(td));
    one.q = tone;
    d.q = td;
    if (0.0f < dist) {
    } else
        i = 0;
    return i;
} }
// Walks the world's zone BSP (SW +0x45CC) to the path zone containing pos; falls back to the world's default zone.
// Also known as: PregionAt (upstream readable / Sly 1 name; PregionAt__FG9VU_VECTORi).
extern "C" void *FindPathzoneAtPos(qword pos, int flags)
{
    REGTREE *pb = g_sw.pregtree;
    REGBSPNODE *pn, *pnEnd, *an;
    PATHZONE **zones;
    unsigned int c;
    if (!pb)
        return 0;
    pn = pb->nodes;
    pnEnd = pn + pb->numNodes;
    zones = pb->zones;
    an = pn;
    while (pn != pnEnd) {
        c = pn->children[ISide(*(qword *)pn->plane, pos)];
        if (c & 0x8000)
            goto leaf;   /* bit 15: a leaf, the rest is the zone index (the leaf return stays last: block order) */
        pn = &an[c];
    }
    if (!flags && pb->numZones)
        return 0;
    return g_sw.pcid17Default;
leaf:
    return zones[c & 0x7FFF];
}
} // namespace M_PpathzoneFromPos

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_LoadSwZoneBspFromBrx {
int U32Read__18CBinaryInputStream(void *pbis) __asm__("U32Read__18CBinaryInputStream");
void *AllocClearedFromWorldHeap(struct SWALLOC *pmsg) __asm__("AllocClearedFromWorldHeap");
void LoadZoneBspFromBrx(void *p, int n, void *pbis) __asm__("LoadZoneBspFromBrx");
// Reads the world's zone BSP from the level stream when present.
// Layout parse-verified: a parser following it reads every byte of 4 levels.
// Also known as: LoadCmeshFromBrx (upstream readable / Sly 1 name; LoadCmeshFromBrx__FP2SWP18CBinaryInputStream).
extern "C" void LoadSwZoneBspFromBrx(SW *world, void *stream)
{
    int n = U32Read__18CBinaryInputStream(stream);
    if (n) {
        struct SWALLOC msg;
        msg.cb = sizeof(REGTREE);
        msg.cbAlign = 0x10;
        world->pregtree = (REGTREE *)AllocClearedFromWorldHeap(&msg);
        LoadZoneBspFromBrx(world->pregtree, n, stream);
    }
}
} // namespace M_LoadSwZoneBspFromBrx

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_LoadZoneBspFromBrx {
struct A { int cb; int align; };
int U32Read__18CBinaryInputStream(void *pbis) __asm__("U32Read__18CBinaryInputStream");
int U16Read__18CBinaryInputStream(void *pbis) __asm__("U16Read__18CBinaryInputStream");
void ReadVector4__18CBinaryInputStreamP7VECTOR4(void *pbis, VECTOR4 *pv) __asm__("ReadVector4__18CBinaryInputStreamP7VECTOR4");
int S32Read__18CBinaryInputStream(void *pbis) __asm__("S32Read__18CBinaryInputStream");
void *AllocFromWorldHeap(struct A *pa) __asm__("AllocFromWorldHeap");
void *AllocClearedFromWorldHeap(struct A *pa) __asm__("AllocClearedFromWorldHeap");
// Reads a zone BSP: nodes (plane index and two children), planes and leaf zone ids.
// Layout parse-verified: a parser following it reads every byte of 4 levels.
extern "C" void LoadZoneBspFromBrx(REGTREE *bsp, int count, void *stream)
{
    struct A a;
    int i, j;
    bsp->numNodes = count;
    bsp->numPlanes = U32Read__18CBinaryInputStream(stream);
    bsp->numZones = U32Read__18CBinaryInputStream(stream);
    a.cb = bsp->numNodes * (int)sizeof(REGBSPNODE);
    a.align = 0x10;
    bsp->nodes = (__typeof__(bsp->nodes))AllocFromWorldHeap(&a);
    a.cb = bsp->numPlanes * (int)sizeof(VECTOR4);
    a.align = 0x10;
    bsp->planes = (__typeof__(bsp->planes))AllocFromWorldHeap(&a);
    a.cb = bsp->numZones * (int)sizeof(PATHZONE *);
    a.align = 0x10;
    bsp->zones = (PATHZONE **)AllocClearedFromWorldHeap(&a);
    a.cb = bsp->numZones * (int)sizeof(int);
    a.align = 0x10;
    bsp->zoneIds = (__typeof__(bsp->zoneIds))AllocFromWorldHeap(&a);
    for (i = 0; i < bsp->numNodes; i++) {
        REGBSPNODE *pe = &bsp->nodes[i];
        pe->plane = &bsp->planes[U16Read__18CBinaryInputStream(stream)];
        pe->children[0] = U16Read__18CBinaryInputStream(stream);
        pe->children[1] = U16Read__18CBinaryInputStream(stream);
    }
    for (j = 0; j < bsp->numPlanes; j++)
        ReadVector4__18CBinaryInputStreamP7VECTOR4(stream, &bsp->planes[j]);
    for (j = 0; j < bsp->numZones; j++)
        bsp->zoneIds[j] = S32Read__18CBinaryInputStream(stream);
}
} // namespace M_LoadZoneBspFromBrx

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_PostLoadSwPathzones {
void *CreateLo__F3CIDP2SO3OIDi(int a, SO *b, int c, int d) __asm__("CreateLo__F3CIDP2SO3OIDi");
void InitDefaultPathzoneCg(void *p) __asm__("InitDefaultPathzoneCg");
void *AllocClearedFromWorldHeap(struct SWALLOC *pmsg) __asm__("AllocClearedFromWorldHeap");
void BindSwPathzones(void *p) __asm__("BindSwPathzones");
// After level load: with no zone BSP creates a default path zone (oid 0x2CA) with the trivial graph; then binds the
// zones and their links.
// Also known as: InitSwNav (upstream readable / Sly 1 name; InitSwNav__FP2SW).
extern "C" void PostLoadSwPathzones(SW *world)
{
    if (!world->pregtree) {
        struct SWALLOC msg;
        world->pcid17Default = (CID17 *)CreateLo__F3CIDP2SO3OIDi(0x11, 0, 0x2CA, -1);
        InitDefaultPathzoneCg(world->pcid17Default);
        msg.cb = sizeof(REGTREE);
        msg.cbAlign = 0x10;
        world->pregtree = (REGTREE *)AllocClearedFromWorldHeap(&msg);
    }
    BindSwPathzones(world->pregtree);
}
} // namespace M_PostLoadSwPathzones

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C1138 {
LO *FindSwObject__FP2SWi3OIDP2LO(void *p, int a, int id, void *scope) __asm__("FindSwObject__FP2SWi3OIDP2LO");
int FindSwObjectsByClass(void *p, int a, int b, void *scope, int d, LO **ap) __asm__("FindSwObjectsByClass");
void *AllocCopyFromWorldHeap(const struct SWALLOC *pal, const void *pv) __asm__("AllocCopyFromWorldHeap");
/* The four helpers below pin registers and instructions for the match; off the EE they are plain C. */
#ifdef SLY2_EE_ASM
// Zero in $7 (the null scope argument of the following call).
extern "C" { static __inline__ void*QueryScope(void){register void*value __asm__("$7");__asm__("daddu %0,$zero,$zero":"=r"(value));return value;} }
// Fill the allocation request for count 4-byte links with the given alignment and set owner->numLinks = count.
extern "C" { static __inline__ const struct SWALLOC*PrepareRequest(struct SWALLOC*request,REGTREE*owner,int count,int alignment){register int requiredAlignment __asm__("$7")=alignment;register const struct SWALLOC*result __asm__("$4");register unsigned bytes __asm__("$2");__asm__("sll %1,%5,2\n\tdaddu %0,%4,$zero\n\tsw %1,%2\n\tsw %6,4(%4)":"=&r"(result),"=&r"(bytes),"=m"(request->cb),"=m"(request->cbAlign):"r"(request),"r"(count),"r"(requiredAlignment));owner->numLinks=count;return result;} }
// High address page of the world g_sw.
extern "C" { static __inline__ char*WorldPage(void){register char*page __asm__("$19");__asm__("lui %0,%%hi(%1)":"=r"(page):"i"(&g_sw));return page;} }
// Address of the world g_sw from its high page.
extern "C" { static __inline__ void*WorldAddress(char*page){void*world;__asm__("addiu %0,%1,%%lo(%2)":"=r"(world):"r"(page),"i"(&g_sw));return world;} }
#else
extern "C" { static __inline__ void*QueryScope(void){return 0;} }
extern "C" { static __inline__ const struct SWALLOC*PrepareRequest(struct SWALLOC*request,REGTREE*owner,int count,int alignment){request->cb=count*(int)sizeof(PATHLINK *);request->cbAlign=alignment;owner->numLinks=count;return request;} }
extern "C" { static __inline__ char*WorldPage(void){return (char *)&g_sw;} }
extern "C" { static __inline__ void*WorldAddress(char*page){return page;} }
#endif
// Resolves the BSP's leaf zone ids to PATHZONE objects and collects every path link (CID 124), numbering both.
extern "C" void BindSwPathzones(REGTREE *world)
{
    int i,n;register int c __asm__("$6");
    LO *ap[0x400];
    struct SWALLOC al;
    char*worldPage=WorldPage();
    for (i = 0; i < world->numZones; i++) {
        PATHZONE *plo = (PATHZONE *)FindSwObject__FP2SWi3OIDP2LO(WorldAddress(worldPage), 5, world->zoneIds[i], 0);
        plo->zoneIndex = i;
        world->zones[i] = plo;
    }
    n = FindSwObjectsByClass(WorldAddress(worldPage), 0x205, 0x7C, QueryScope(), 0x400, ap);
    {register int alignment __asm__("$7")=0x10;
    c = 0x400;
    if (n < 0x400) c = n;
    world->links=(PATHLINK**)AllocCopyFromWorldHeap(PrepareRequest(&al,world,c,alignment),ap);}
    {
        int j;
        for (j = 0; j < world->numLinks; j++) world->links[j]->linkIndex = j;
    }
    world->isBound = 1;
}

} // namespace M_func_001C1138

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C1260 {
struct H { int a[4]; };
struct A { int cb; int align; };
int STACKSCOPE_ctor(struct H *p) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(struct H *p, int n) __asm__("_$_10STACKSCOPE");
int *AllocClearedFromStack(struct A *pa) __asm__("AllocClearedFromStack");
void *AllocFromWorldHeap(struct A *pa) __asm__("AllocFromWorldHeap");
// Allocate the request zeroed, 16-byte aligned, from the frame stack.
extern "C" { static inline int *Alloc1(struct A *pa) { pa->align = 0x10; return AllocClearedFromStack(pa); } }
// Allocate the request 16-byte aligned from the world heap.
extern "C" { static inline void *Alloc2(struct A *pa) { pa->align = 0x10; return AllocFromWorldHeap(pa); } }
// Buckets the path links by the zone of their first endpoint, for FindPathlinkRoute.
extern "C" void BuildPathzoneLinkLists(REGTREE *world)
{
    struct H h;
    struct A a;
    int *ac;
    int i, j, k;
    STACKSCOPE_ctor(&h);
    a.cb = world->numZones * (int)sizeof(int);
    ac = Alloc1(&a);
    for (i = 0; i < world->numLinks; i++) {
        PATHLINK *q = world->links[i];
        if (q->startZone && q->endZone)
            ac[PathzoneOf(q->startZone)->zoneIndex]++;
    }
    a.cb = world->numZones * (int)sizeof(REGLINKLIST);
    world->linkLists = (__typeof__(world->linkLists))Alloc2(&a);
    for (j = 0; j < world->numZones; j++) {
        REGLINKLIST *pb = &world->linkLists[j];
        pb->count = 0;
        a.cb = ac[j] * (int)sizeof(int);
        pb->linkIndices = (__typeof__(pb->linkIndices))Alloc2(&a);
    }
    for (k = 0; k < world->numLinks; k++) {
        PATHLINK *q = world->links[k];
        if (q->startZone && q->endZone) {
            REGLINKLIST *pb = &world->linkLists[PathzoneOf(q->startZone)->zoneIndex];
            pb->linkIndices[pb->count++] = k;
        }
    }
    STACKSCOPE_ctor_1AE4A0(&h, 2);
}
} // namespace M_func_001C1260

/* The obstacle query cursor (FindPathzoneObstacles' C): one membership bit per obstacle along x and z, the zone
   version it was built for, and the min/max positions in the x and z event arrays. */
struct PATHSEARCH { int xMembership[4]; int zMembership[4]; int version; int xMin; int xMax; int zMin; int zMax; int unknown34[3]; };
// Resets a 0x40-byte sweep cursor over a zone's sorted obstacle events: no bits, an invalid version (-1), and every
// position at the last real event (numEvents - 2, before the end sentinel).
extern "C" void InitObstacleQueryCursor(PATHZONE *zone, PATHSEARCH *cursor)
{
    int xLast;
    int zLast;
    memset(cursor, 0, 0x40);
    xLast = zone->cg.numEvents - 2;
    cursor->xMax = xLast;
    cursor->xMin = xLast;
    zLast = zone->cg.numEvents - 2;
    cursor->version = -1;
    cursor->zMax = zLast;
    cursor->zMin = zLast;
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C14A8 {
/* (Claude agent) Event bitset sweep: walk ae[] backward/forward from i toward time g, toggling grf bits of events whose kind==f. */
struct E { float t; unsigned short k; unsigned short bit; };
// Moves the cursor through sorted begin/end events, toggling each obstacle's membership bit.
extern "C" int SweepObstacleEvents(struct E *events, int index, float coordinate, unsigned int *membership, int f)
{
    if (coordinate <= events[index].t) {
        for (;;) {
            struct E *pe = &events[index];
            if (pe->t < coordinate) break;
            if (pe->k == f) {
                unsigned int bit = pe->bit;
                unsigned int w = bit / 32;
                unsigned int b = bit - w * 32;
                if (!f) *(unsigned int *)((int)membership + (w << 2)) |= 1U << b;
                else *(unsigned int *)((int)membership + (w << 2)) -= 1U << b;
            }
            index--;
        }
    } else {
        index++;
        for (;;) {
            struct E *pe = &events[index];
            if (coordinate < pe->t) break;
            if (pe->k == f) {
                unsigned int bit = pe->bit;
                unsigned int w = bit / 32;
                unsigned int b = bit - w * 32;
                if (f) *(unsigned int *)((int)membership + (w << 2)) |= 1U << b;
                else *(unsigned int *)((int)membership + (w << 2)) -= 1U << b;
            }
            index++;
        }
        index--;
    }
    return index;
}
} // namespace M_func_001C14A8

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C15F8 {
struct E { float f; int x; };
int SweepObstacleEvents(struct E *a, int i, float g, void *p, int f) __asm__("SweepObstacleEvents");
// Advances the min and max cursors of one axis to a new query range.
extern "C" void SweepObstacleRange(struct E *events, int *minCursor, int *maxCursor, void *p, float rangeMin, float rangeMax)
{
    int i1 = *minCursor;
    int i2 = *maxCursor;
    if (events[i1].f < rangeMin) {
        i2 = SweepObstacleEvents(events, i2, rangeMax, p, 1);
        i1 = SweepObstacleEvents(events, i1, rangeMin, p, 0);
    } else {
        i1 = SweepObstacleEvents(events, i1, rangeMin, p, 0);
        i2 = SweepObstacleEvents(events, i2, rangeMax, p, 1);
    }
    *minCursor = i1;
    *maxCursor = i2;
}
} // namespace M_func_001C15F8

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C1700 {
union U {qword q;unsigned int words[4];};
struct C {qword x,z;int version;int xmin,xmax,zmin,zmax;};

void InitObstacleQueryCursor(struct PATHZONE*,struct C*) __asm__("InitObstacleQueryCursor");
void SweepObstacleRange(void*,int*,int*,qword*,float,float) __asm__("SweepObstacleRange");
// Bitwise AND of two 128-bit values (pand).
extern "C" { static inline qword And(qword a,qword b){qword r;__asm__("pand %0,%1,%2":"=r"(r):"r"(a),"r"(b));return r;} }
// Sweep-and-prune query: collects the obstacles whose x and y intervals overlap the segment's box; returns the obstacleCapacity.
// Also known as: CpolyNear (upstream readable / Sly 1 name; CpolyNear__FP9NAVREGIONP6VECTORG9VU_VECTORT2iPP7NAVPOLY).
extern "C" int FindPathzoneObstacles(struct PATHZONE*zone,struct C*cursor,qword from,qword to,int max,void**out){
 struct C local;
 union U mask,copy;
 qword hi,lo;
 int i,n;
 float hx,lx;
 if(!zone->cg.numEvents)return 0;
 if(!cursor){cursor=&local;cursor->version=-1;}
 if(cursor->version!=zone->version)InitObstacleQueryCursor(zone,cursor);
 lo=VuMin(from,to);hi=VuMax(from,to);
 SweepObstacleRange(zone->cg.xEvents,&cursor->xmin,&cursor->xmax,&cursor->x,QwordX(lo),QwordX(hi));
 SweepObstacleRange(zone->cg.zEvents,&cursor->zmin,&cursor->zmax,&cursor->z,QwordY(lo),QwordY(hi));
 copy.q=And(cursor->x,cursor->z);mask=copy;
 {void**cursor=out;
 for(i=0,n=0;i<zone->cg.obstacleCapacity;i++){
  if(mask.words[i/32]&(1<<(i%32))){
   if(max<n)break;
   *cursor++=zone->cg.obstacles[i];n++;
  }
 }
 }
 cursor->version=zone->version;
 return n;
}
} // namespace M_func_001C1700

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C18B8 {
struct E { int coordinate; unsigned short isBegin; unsigned short obstacleIndex; };
void CopyAb__FPvT0Ui(void *dst, void *src, int cb) __asm__("CopyAb__FPvT0Ui");
// Deletes the begin/end event pair of an obstacle from a sorted event array.
extern "C" void RemoveObstacleEvents(int count, struct E *events, int key)
{
    int i, iFirst, iLast;
    struct E *pe;
    for (i = 0, pe = events; i < count; pe++, i++) {
        if (pe->obstacleIndex != key)
            continue;
        if (pe->isBegin) {
            iFirst = i;
            continue;
        }
        iLast = i;
        break;
    }
    CopyAb__FPvT0Ui(&events[iFirst], &events[iFirst] + 1, (iLast - iFirst - 1) * sizeof(struct E));
    CopyAb__FPvT0Ui(&events[iLast] - 1, &events[iLast] + 1, (count - iLast - 1) * sizeof(struct E));
}
} // namespace M_func_001C18B8

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C1968 {
struct E;
struct S : XFM { char pad[0xF8 - 0xE0]; int numObstacles; int obstacleCapacity; int numEvents; struct E *xEvents; struct E *zEvents; int *obstacles; int unknown110; int version; };
void RemoveObstacleEvents(int n, struct E *a, int key) __asm__("RemoveObstacleEvents");
// Unregisters an obstacle from both axes of its zone and bumps the world's navigation version.
extern "C" void RemovePathzoneObstacle(struct S *zone, int key)
{
    int i;
    for (i = 0; i < zone->obstacleCapacity; i++) {
        if (zone->obstacles[i] == key) break;
    }
    zone->obstacles[i] = 0;
    RemoveObstacleEvents(zone->numEvents, zone->xEvents, i);
    RemoveObstacleEvents(zone->numEvents, zone->zEvents, i);
    zone->numObstacles--;
    zone->numEvents -= 2;
    zone->version = ++g_sw.pathzoneVersion;
}
} // namespace M_func_001C1968

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C1A38 {
/* Insert a begin event (g0, kind 1) and an end event (g1, kind 0) into a time-sorted array of 8-byte events
   (lane 10).  The memmove goes through an inline helper: its parameters get their own pseudos, which gives the
   target's "pointer copied into a0/a1, then a0 += 16" argument setup. */
struct E { float t; unsigned short k; unsigned short bit; };
void *memmove(void *dst, const void *src, unsigned int cb) __asm__("memmove");
// memmove cb bytes.
extern "C" { static inline void Move(void *pvDst, void *pvSrc, unsigned cb) { memmove(pvDst, pvSrc, cb); } }
// Inserts an obstacle's begin/end events into a sorted event array.
extern "C" void InsertObstacleEvents(int count, struct E *events, int bit, float begin, float end)
{
    int i, j;
    for (i = 0; i < count; i++)
        if (begin <= events[i].t) break;
    for (j = i; j < count; j++)
        if (end < events[j].t) break;
    Move(&events[j + 2], &events[j], (count - j) * 8U);
    events[j + 1].t = end;
    events[j + 1].k = 0;
    events[j + 1].bit = bit;
    Move(&events[i + 1], &events[i], (j - i) * 8U);
    events[i].bit = bit;
    events[i].t = begin;
    events[i].k = 1;
}
} // namespace M_func_001C1A38

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C1BA0 {
struct E;
struct O { char pad[0x1A0]; union { qword q; float g[4]; } a; union { qword q; float g[4]; } b; char p1[0x1DC - 0x1C0]; struct S *pathzone; };
struct S : XFM { char pad[0xF8 - 0xE0]; int numObstacles; int obstacleCapacity; int numEvents; struct E *xEvents; struct E *zEvents; struct O **obstacles; int unknown110; int version; };
struct M { struct O *po; void *pv; };
int FindPathzoneObstacles(struct S *p, int z, qword a, qword b, int c, void **ap) __asm__("FindPathzoneObstacles");
int DoConvexObstaclesOverlap(struct O *po, void *pv) __asm__("DoConvexObstaclesOverlap");
void MergeCid14Obstacles(int n, struct M *pm) __asm__("MergeCid14Obstacles");
void GrowPathzoneObstacleArrays(struct S *p) __asm__("GrowPathzoneObstacleArrays");
void InsertObstacleEvents(int c, struct E *ae, int bit, float g0, float g1) __asm__("InsertObstacleEvents");
// Registers an obstacle in a zone unless it overlaps an existing one (then the polygon gets message 2 to merge),
// growing the arrays as needed.
// Also known as: AddPolyToRegion (upstream readable / Sly 1 name; AddPolyToRegion__FP9NAVREGIONP7NAVPOLY).
extern "C" void AddPathzoneObstacle(struct S *zone, struct O *obstacle)
{
    void *ap[0x20];
    int n, i;
    n = FindPathzoneObstacles(zone, 0, obstacle->a.q, obstacle->b.q, 0x20, ap);
    for (i = 0; i < n; i++) {
        if (DoConvexObstaclesOverlap(obstacle, ap[i])) {
            struct M m;
            m.po = obstacle;
            m.pv = ap[i];
            MergeCid14Obstacles(2, &m);
            return;
        }
    }
    if (zone->numObstacles >= zone->obstacleCapacity)
        GrowPathzoneObstacleArrays(zone);
    {
        int j;
        zone->numObstacles++;
        for (j = 0; j < zone->numObstacles; j++)
            if (!zone->obstacles[j]) break;
        zone->obstacles[j] = obstacle;
        InsertObstacleEvents(zone->numEvents, zone->xEvents, j, obstacle->a.g[0], obstacle->b.g[0]);
        InsertObstacleEvents(zone->numEvents, zone->zEvents, j, obstacle->a.g[1], obstacle->b.g[1]);
    }
    obstacle->pathzone = zone;
    zone->numEvents += 2;
    zone->version = ++g_sw.pathzoneVersion;
}
} // namespace M_func_001C1BA0

namespace RO_ORDER { extern const float r_D_002A76E0 __asm__("D_002A76E0") = -3.40282347e+38f; }
namespace RO_ORDER { extern const float r_D_002A76E4 __asm__("D_002A76E4") = 3.40282347e+38f; }
// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C1D20 {
struct E { float t; unsigned short k; unsigned short bit; };
struct S : XFM { char pad[0xFC - 0xE0]; int obstacleCapacity; int numEvents; struct E *xEvents; struct E *zEvents; void **obstacles; };
struct A { int cb; int align; };
extern const float D_002A76E0 __asm__("D_002A76E0");   /* defined below where the ELF has it (.rodata order) */
extern const float D_002A76E4 __asm__("D_002A76E4");   /* defined below where the ELF has it (.rodata order) */
void *AllocFromWorldHeap(struct A *pa) __asm__("AllocFromWorldHeap");
void CopyAb__FPvT0Ui(void *dst, void *src, int cb) __asm__("CopyAb__FPvT0Ui");
void *func_002333B4(void *p, int c, int cb) __asm__("memset");
// Grows a zone's obstacle and event arrays (0 -> 16 -> 128) with sentinel events.
extern "C" void GrowPathzoneObstacleArrays(struct S *zone)
{
    struct A a;
    int n;
    int cb;
    struct E *ae1, *ae2;
    void **apv;
    switch (zone->obstacleCapacity) {
    case 0: n = 0x10; break;
    case 0x10: n = 0x80; break;
    case 0x80: break;
    }
    cb = n * 16 + 16;
    a.cb = cb;
    a.align = 0x10;
    ae1 = (__typeof__(ae1))AllocFromWorldHeap(&a);
    a.cb = cb;
    a.align = 0x10;
    ae2 = (__typeof__(ae2))AllocFromWorldHeap(&a);
    a.cb = n * 4;
    a.align = 0x10;
    apv = (__typeof__(apv))AllocFromWorldHeap(&a);
    CopyAb__FPvT0Ui(ae1, zone->xEvents, zone->numEvents * 8);
    CopyAb__FPvT0Ui(ae2, zone->zEvents, zone->numEvents * 8);
    CopyAb__FPvT0Ui(apv, zone->obstacles, zone->obstacleCapacity * 4);
    { int cz = (n - zone->obstacleCapacity) * 4; func_002333B4((void *)(zone->obstacleCapacity * 4 + (int)apv), 0, cz); }
    zone->zEvents = ae2;
    zone->obstacles = apv;
    zone->obstacleCapacity = n;
    zone->xEvents = ae1;
    if (zone->numEvents == 0) {
        zone->xEvents[0].t = D_002A76E0;
        zone->xEvents[0].k = 1;
        zone->xEvents[0].bit = 0xFF;
        zone->xEvents[1].t = D_002A76E4;
        zone->xEvents[1].k = 0;
        zone->xEvents[1].bit = 0xFF;
        zone->zEvents[0].t = D_002A76E0;
        zone->zEvents[0].k = 1;
        zone->zEvents[0].bit = 0xFF;
        zone->zEvents[1].t = D_002A76E4;
        zone->zEvents[1].k = 0;
        zone->zEvents[1].bit = 0xFF;
        zone->numEvents = 2;
    }
}
} // namespace M_func_001C1D20

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C1EC0 {
extern char D_003DD020[] __asm__("D_003DD020");
void FindShapeClosestPoint(void *p, qword a, void *b, qword *pq, qword *c, float *d, float *e) __asm__("FindShapeClosestPoint");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
// Position of a path-link endpoint: the closest point on its shape, else its object's position, else the stored
// vector.
// Also known as: PosGoal (upstream readable / Sly 1 name; PosGoal__FP4GOALG9VU_VECTOR).
extern "C" qword GetPathlinkEndpointPos(PATHLINKEND *endpoint, qword pos)
{
    qword q;
    if (endpoint->shape) {
        FindShapeClosestPoint(endpoint->shape, pos, D_003DD020, &q, 0, 0, 0);
    } else if (endpoint->object) {
        q = GetLoWorldPos(endpoint->object);
    } else {
        q = endpoint->pos;
    }
    return q;
}
} // namespace M_func_001C1EC0

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C1F28 {
float GetShapeMaxDistance(void *p) __asm__("GetShapeMaxDistance");
void EvaluateShapeAtDistance(void *p, qword *pq, int n, float g) __asm__("EvaluateShapeAtDistance");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
// Position of a path-link endpoint using the shape's midpoint.
// Also known as: PosRecord (upstream readable / Sly 1 name; PosRecord__FP4GOAL).
extern "C" qword GetPathlinkEndpointCenter(PATHLINKEND *endpoint)
{
    qword q;
    if (endpoint->shape) {
        EvaluateShapeAtDistance(endpoint->shape, &q, 0, GetShapeMaxDistance(endpoint->shape) * 0.5f);
    } else if (endpoint->object) {
        q = GetLoWorldPos(endpoint->object);
    } else {
        q = endpoint->pos;
    }
    return q;
}
} // namespace M_func_001C1F28

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C1FA0 {
struct LO : ::LO { char pad[0x218 - 0x50]; qword *pq; };
int IsBasicDerivedFrom__FP5BASIC3CID(struct LO *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
qword GetPathlinkEndpointPos(void *p, qword q) __asm__("GetPathlinkEndpointPos");
// Returns 1 when an object is within radius of an endpoint (always for class 0x40 objects). (inferred)
// Also known as: FNearRecord (upstream readable / Sly 1 name; FNearRecord__FP4GOALP5GUARDf).
extern "C" int IsNearPathlinkEndpoint(void *endpoint, struct LO *object, float radius)
{
    qword a, b, d;
    float f;
    if (IsBasicDerivedFrom__FP5BASIC3CID(object, 0x40)) return 1;
    a = *object->pq;
    b = GetPathlinkEndpointPos(endpoint, a);
    __asm__("vsub.xyzw %0, %1, %2\n\tvmul.xy %0, %0, %0\n\tvaddy.x %0, %0, %0y\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f <= radius;
}
} // namespace M_func_001C1FA0

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_InitPathlink {
void InitLo(LO *p) __asm__("InitLo");
} // namespace M_InitPathlink
// CID 124 slot 7 (Init): InitLo, then no kind and no index yet.
void PATHLINK::Init() { M_InitPathlink::InitLo(this); kind = -1; linkIndex = -1; }

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_AddPathlinkHierarchy {
void AddLoHierarchy(void *p) __asm__("AddLoHierarchy");
// CID 124 slot 11: AddLoHierarchy, then bumps the world's navigation version.
extern "C" void AddPathlinkHierarchy(void *pathlink)
{
    SW *pg;
    int c;
    AddLoHierarchy(pathlink);
    pg = &g_sw;
    c = pg->unknown3D74 + 1;
    pg->unknown3D74 = c;
    pg->pregtree->linkVersion = c;
}
} // namespace M_AddPathlinkHierarchy

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_RemovePathlinkHierarchy {
void RemoveLoHierarchy(void *p) __asm__("RemoveLoHierarchy");
// CID 124 slot 12: RemoveLoHierarchy, then bumps the world's navigation version.
extern "C" void RemovePathlinkHierarchy(void *pathlink)
{
    SW *pg;
    int c;
    RemoveLoHierarchy(pathlink);
    pg = &g_sw;
    c = pg->unknown3D74 + 1;
    pg->unknown3D74 = c;
    pg->pregtree->linkVersion = c;
}
} // namespace M_RemovePathlinkHierarchy

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C20E8 {
int IsBasicDerivedFrom__FP5BASIC3CID(void *q, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
// Appends an endpoint to a path link (a shape for class 10, otherwise an object); max 6.
// Also known as: AddNavLinkRecord (upstream readable / Sly 1 name; AddNavLinkRecord__FP7NAVLINKP2LO).
extern "C" void AddPathlinkEndpointObject(PATHLINK *pathlink, LO *object)
{
    if ((unsigned)pathlink->numEnds < 6) {
        PATHLINKEND *pe = &pathlink->aend[pathlink->numEnds++];
        if (IsBasicDerivedFrom__FP5BASIC3CID(object, 10)) pe->shape = (SHAPE *)object; else pe->object = object;
    }
}
} // namespace M_func_001C20E8

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C2148 {
// Appends a fixed-position endpoint to a path link; max 6.
extern "C" void AddPathlinkEndpointPos(PATHLINK *pathlink, qword pos)
{
    if ((unsigned)pathlink->numEnds < 6) { PATHLINKEND *pe = &pathlink->aend[pathlink->numEnds++]; pe->pos = pos; }
}
} // namespace M_func_001C2148

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_PostPathlinkLoad {
void PostLoLoad(LO *p) __asm__("PostLoLoad");
qword GetPathlinkEndpointCenter(PATHLINKEND *pe) __asm__("GetPathlinkEndpointCenter");
CID17 *FindPathzoneAtPos(qword q, int n) __asm__("FindPathzoneAtPos");
// CID 124 slot 15 (PostLoad): PostLoLoad, caches the first and last endpoint positions and their path zones.
// Also known as: ResolveNavLink (upstream readable / Sly 1 name; ResolveNavLink__FP7NAVLINK).
extern "C" void PostPathlinkLoad(PATHLINK *pathlink)
{
    PostLoLoad(pathlink);
    pathlink->startPos = GetPathlinkEndpointCenter(&pathlink->aend[0]);
    pathlink->startZone = FindPathzoneAtPos(pathlink->startPos, 0);
    pathlink->endPos = GetPathlinkEndpointCenter(/*B*/&pathlink->aend[pathlink->numEnds] - 1/*E*/);
    pathlink->endZone = FindPathzoneAtPos(pathlink->endPos, 0);
}
} // namespace M_PostPathlinkLoad

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_FindPathlinkRoute {
// FindPathlinkRoute: A* path search over the navigation polygons (lilly5, T1831; = lilly4 with the minimal probe).
// BYTE-IDENTICAL (matchtool MATCH), but NOT promotable under AGENT_GUIDE "Not accepted": the target's never-touched
// object at sp+0x30 is still an invented `EMPTY e` (without it every SImode home is 4 bytes low: 18/321, aligned 0).
// T1831: the probe only needs a user-declared DESTRUCTOR (the cleanup makes it addressable -> slot at its declaration):
// `struct EMPTY { ~EMPTY() {} };` and `struct { int n; ~X() {} }` both MATCH; ctor-only empty class = no slot (18/321);
// an unused local whose address goes to an inline helper that ignores it is purged (18/321). So the real object is a
// never-touched RAII-style object (inline empty dtor) or a volatile. Rest of the code is lilly4 (StoreX estimate).
typedef int qword __attribute__((mode(TI)));
/* The search runs over the world's zone BSP (REGTREE): zones are the nodes (PATHZONE::zoneIndex numbers them), path
   links the edges (REGTREE::linkLists buckets them by their near zone; a link leads from startPos to endPos, into
   endZone). */
typedef PATHZONE NAV;
typedef PATHLINK EDGE;
struct NODE { NAV *pnav; EDGE *pedge; NODE *pnodeParent; float gCost; float gEstimate; int state; NODE *pnodeNext; NODE *pnodePrev; };


void *STACKSCOPE_ctor(void *pframe) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(void *pframe, int n) __asm__("_$_10STACKSCOPE");
void *AllocClearedFromStack(SWALLOC *pmsg) __asm__("AllocClearedFromStack");
void *AllocFromStack(SWALLOC *pmsg) __asm__("AllocFromStack");
void *func_002333B4(void *pv, int b, unsigned int cb) __asm__("memset");
qword GetLoWorldPos(void *pv) __asm__("GetLoWorldPos");
NAV *FindPathzoneAtPos(qword pos, int f) __asm__("FindPathzoneAtPos");
void ClearDl__FP2DL(DL *pdl) __asm__("ClearDl__FP2DL");
void AppendDlEntry__FP2DLPv(DL *pdl, void *pv) __asm__("AppendDlEntry__FP2DLPv");
void RemoveDlEntry__FP2DLPv(DL *pdl, void *pv) __asm__("RemoveDlEntry__FP2DLPv");
void InsertDlEntryBefore__FP2DLPvT1(DL *pdl, void *pvNext, void *pv) __asm__("InsertDlEntryBefore__FP2DLPvT1");
int IsDlEmpty__FP2DL(DL *pdl) __asm__("IsDlEmpty__FP2DL");
int CanUsePathlink(REGTREE *pgraph, EDGE *pedge, void *pvActor) __asm__("CanUsePathlink");
int IsPosInPathzone(NAV *pnav, qword pos, int f) __asm__("IsPosInPathzone");


// Allocate cb zeroed bytes, 16-byte aligned, from the frame stack.
static inline void *AllocZeroedFromHeap(int cb) { SWALLOC msg; msg.cb = cb; msg.cbAlign = 0x10; return AllocClearedFromStack(&msg); }
// Allocate cb bytes, 16-byte aligned, from the frame stack.
static inline void *PvAlloc(int cb) { SWALLOC msg; msg.cb = cb; msg.cbAlign = 0x10; return AllocFromStack(&msg); }
// Length of the xyz part of v, in the x lane.
static inline qword QwLength(qword v)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(qw), "=j"(vfOne) : "j"(v), "j"(v));

    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
// Unknown object at sp+0x30 with an empty destructor (see the header comment)
struct EMPTY { ~EMPTY() {} };
// Path zone containing pos.
static inline NAV *PnavFind(const qword &pos) { return FindPathzoneAtPos(pos, 0); }
// Append a node to a doubly linked list.
static inline void AppendNode(DL *pdl, NODE *pnode) { AppendDlEntry__FP2DLPv(pdl, pnode); }
// Remove a node from a doubly linked list.
static inline void RemoveNode(DL *pdl, NODE *pnode) { RemoveDlEntry__FP2DLPv(pdl, pnode); }
// Insert pnode before pnodeNext in a doubly linked list.
static inline void InsertNode(DL *pdl, NODE *pnodeNext, NODE *pnode) { InsertDlEntryBefore__FP2DLPvT1(pdl, pnodeNext, pnode); }
// Distance between a and b (xyz).
static inline float GDistX(const qword &a, const qword &b) { return QwordX(QwLength(VuSub(a, b))); }

// Store the x lane of q to *p.
static inline void StoreX(float *p, qword q) { __asm__("sw %1,%0" : "=m"(*p) : "r"(q)); }
// A* over path zones connected by path links, from an actor's zone to a goal; returns the chain of links.
// Also known as: CNavLinkPath (upstream readable / Sly 1 name; CNavLinkPath__FP5GUARDP6VECTORG9VU_VECTORiiPP4EDGE).
extern "C" int FindPathlinkRoute(REGTREE *graph, void *actor, qword goal, int isExact, int maxEdges, EDGE **edges)
{
    STACKSCOPE scope;
    NODE *anode;
    int *afAllowed;
    anode = (NODE *)AllocZeroedFromHeap(graph->numZones * sizeof(NODE));
    afAllowed = (int *)PvAlloc(graph->numLinks * sizeof(int));
    func_002333B4(afAllowed, 0xFF, graph->numLinks * sizeof(int));
    qword posStart = GetLoWorldPos(actor);
    NAV *pnavStart = PnavFind(posStart);
    NAV *pnavGoal = PnavFind(goal);

    if (!pnavStart || !pnavGoal) {
        return 0;
    }
    DL dlOpen;
    DL dlClosed;
    ClearDl__FP2DL(&dlOpen);
    InitDl(&dlOpen, SLY2_OFFSETOF(NODE, pnodeNext));
    ClearDl__FP2DL(&dlClosed);
    InitDl(&dlClosed, SLY2_OFFSETOF(NODE, pnodeNext));
    EMPTY e;
    NODE *pnode = &anode[pnavStart->zoneIndex];
    pnode->pnav = pnavStart;
    StoreX(&pnode->gEstimate, QwLength(VuSub(posStart, goal)));


    pnode->state = 2;
    AppendNode(&dlOpen, pnode);
    while (!IsDlEmpty__FP2DL(&dlOpen)) {
        pnode = (NODE *)dlOpen.head;
        if (isExact) {
            if (pnode->pnav == pnavGoal)
                break;
        } else if (IsPosInPathzone(pnode->pnav, goal, 1)) {
            break;
        }
        RemoveNode(&dlOpen, pnode);
        pnode->state = 1;
        AppendNode(&dlClosed, pnode);
        qword posFrom;
        if (pnode->pedge)
            posFrom = pnode->pedge->endPos;
        else
            posFrom = posStart;
        REGLINKLIST *padj = &graph->linkLists[pnode->pnav->zoneIndex];
        for (int i = 0; i < padj->count; i++) {
            int iedge = padj->linkIndices[i];
            EDGE *pedge = graph->links[iedge];
            if (afAllowed[iedge] == -1)
                afAllowed[iedge] = CanUsePathlink(graph, pedge, actor);
            if (!afAllowed[iedge])
                continue;

            float gDist = GDistX(posFrom, pedge->startPos) + GDistX(pedge->startPos, pedge->endPos);
            NAV *pnavDest = PathzoneOf(pedge->endZone);
            float gCost = pnode->gCost + gDist;
            NODE *pnodeDest = &anode[pnavDest->zoneIndex];
            float gEstimate = gCost + GDistX(pedge->endPos, goal);
            if (pnodeDest->state && pnodeDest->gEstimate <= gEstimate)
                continue;
            switch (pnodeDest->state) {
            case 0:
                break;
            case 1:
                RemoveNode(&dlClosed, pnodeDest);
                break;
            case 2:
                RemoveNode(&dlOpen, pnodeDest);
                break;
            }
            pnodeDest->pnav = pnavDest;
            pnodeDest->pedge = pedge;
            pnodeDest->gCost = gCost;
            pnodeDest->pnodeParent = pnode;
            pnodeDest->gEstimate = gEstimate;
            pnodeDest->state = 2;
            NODE *pnodeNext;
            for (pnodeNext = (NODE *)dlOpen.head; pnodeNext; pnodeNext = pnodeNext->pnodeNext) {
                if (pnodeNext->gEstimate > pnodeDest->gEstimate)
                    break;
            }
            InsertNode(&dlOpen, pnodeNext, pnodeDest);
        }
    }
    int c = 0;
    if (!IsDlEmpty__FP2DL(&dlOpen)) {
        if (!isExact && !pnode->pnodeParent) {
            edges[0] = 0;
            c = 1;
        } else {
            int cpedge = 0;
            NODE *pnodeT;
            for (NODE *pnodeCount = pnode; pnodeCount->pnodeParent; pnodeCount = pnodeCount->pnodeParent)
                cpedge++;
            if (cpedge < maxEdges)
                c = cpedge;
            else
                c = maxEdges;
            for (pnodeT = pnode; pnodeT->pnodeParent; pnodeT = pnodeT->pnodeParent) {
                if (cpedge <= maxEdges)
                    edges[cpedge - 1] = pnodeT->pedge;
                cpedge--;
            }
        }
    }
    return c;
}
} // namespace M_FindPathlinkRoute

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001C26E0 {
/* The actor asking (inferred: a guard-like object): the zones it may enter and the link kinds it may use. */
struct G : STEP { char pad[0xE94 - 0x2C0]; int numAllowedZones; int allowedZones[84]; int allowedLinkKinds; };
// Returns 1 when the actor may traverse the path link: the link is in the world, the actor allows its kind (kind 1
// needs bit 0 as well as bit 1), and, when the actor has a zone list, both end zones are on it. (inferred)
// Also known as: FNavLinkAllowed (upstream readable / Sly 1 name; FNavLinkAllowed__FP7NAVLINKP5GUARDPv).
extern "C" int CanUsePathlink(void *zoneBsp, PATHLINK *pathlink, struct G *actor)
{
    int requiredKinds, i, hasFrom, hasTo, numZones;
    if ((pathlink->isInWorld & 1) == 0) return 0;
    requiredKinds = (pathlink->kind == 1) ? 3 : 2;
    if ((requiredKinds & actor->allowedLinkKinds) != requiredKinds) return 0;
    numZones = actor->numAllowedZones;
    if (numZones > 0) {
        int *zones = actor->allowedZones;
        hasFrom = 0; hasTo = 0;
        for (i = 0; i < numZones; i++) {
            int zone = zones[i];
            if ((sptr)pathlink->startZone == zone) hasFrom = 1;   /* the actor's list holds zone pointers as ints */
            if ((sptr)pathlink->endZone == zone) hasTo = 1;
            if (hasFrom && hasTo) break;
        }
        if (!hasFrom || !hasTo) return 0;
    }
    return 1;
}
} // namespace M_func_001C26E0

