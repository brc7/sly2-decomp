// bsp.cpp -- BSP trees of planes (collision volumes): clipping an edge to a BSP, simplifying a tree around a point or
// a polygon, point queries, cloning a tree, and planes (from three points, from normal and point, transformed). VU0
// asm. TU 0x118668..0x1195E0 (12 functions, address order) and the inline functions behind two linkonce copies
// (0x2564A8, 0x2564B8), defined at the end.
// A BSP node holds a plane and two children; null children are outside / inside leaves. Collision code clips edges
// against these volumes, the culling and query code locates points, and level loading clones trees between
// geometries. Entry points: ClsgClipEdgeToBsp, PruneBspBySphere, CollectBspSurfacesTouchingPolygon,
// FindBspLeafContainingPointFast, FindBspLeafContainingPoint, CloneBspc, InitPlaneFromPositions / InitPlaneFromNormalPos,
// TransformPlane.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_bsp
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_bsp
#include "sly2/xfm.h"
#include "sly2/slotheap.h"
#include "sly2/scrap.h"
#include "sly2/vu0.h"
#include "sly2/util.h"

// 0x119488 StaticInit_bsp and GlobalConstructors_bsp: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned  size_t;
extern "C" {
void* memset(void* ptr, int value, size_t num);
}
/* SURF, EDGE and BSP are sly2/xfm.h's. The code reads a surface's plane (normal, distance) as one quadword, and a
   node's psurf as a bare plane or a pointer to rebase: */
static inline qword *PplaneOfSurf(SURF *psurf) { return (qword *)psurf; }
enum { SURF_COLLECTED = 0x4000 };   /* SURF::grf: already collected by CollectBspSurfacesTouchingPolygon */
typedef char assert_BSP_size[sizeof(BSP) == 12 ? 1 : -1];
/* A plane (normal, distance). */
typedef struct PLANE { float x, y, z, d; } __attribute__((aligned(16))) PLANE;
/* A VU0 temporary with an out-of-line empty constructor (InitVuTemp below). */
class VUF {
public:
    VUF() __asm__("InitVuTemp");
    qword q;
};
union VUFU { qword q; float f[4]; };   /* FindBspLeafContainingPointFast calls that constructor by hand */
extern "C" inline void *InitVuTemp(void *p);   /* inline, defined after its callers (end of file) */

// ClipSegmentBsp: ported from Lilly's personal Sly 2 decomp (src/P2/bsp.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// 0x00118668 ClsgClipEdgeToBsp__FP3BSPT0UxUxiP3LSG  (MATCHES Sly 2; Sly 1's ClsgClipEdgeToBsp with qword positions, no mpibspn and the node limit)
// Clips the edge pos1 - pos2 to the BSP: the pieces of it on the negative side of every plane (= inside the volume) go to alsg (at most clsgMax),
// the return value is how many. A work stack of CEB (0x20 bytes: pbsp = node to look at, pbspNeg = the node whose negative side this piece is on,
// NULL = outside, g = where along the edge the piece starts (0 = pos1, 1 = pos2), psurf + normal = the plane it starts at) walks the tree: the piece
// [ceb.g, pceb->g] (ceb is the current one, pceb the top of the stack = where it ends) is cut by each node's plane (g1 / g2 = distance of pos1 / pos2 to
// it): both ends behind -> the negative child; both in front -> the positive child; otherwise it is split at t = g1 / (g1 - g2) (a new stack entry for the
// far part, pushed when t is inside the piece) and the near part goes on by the side of the middle (gMid, ties decided by the far end gTop).
// A leaf (pbsp == NULL) with pbspNeg set is a piece inside the volume: it is merged into the last LSG when it starts where that one ended
// (ceb.g == plsgLast->au[1]), else a new LSG (0x60 bytes, cleared) is made: apos[2] = pos1 * (1 - g) + pos2 * g, anormal[2], au[2], apsurf[2].
// How it matches (T1120): (1) the LOOP NEST. `while (ceb.pbsp == NULL)` inside `while (1)`, and pbsp is never copied to a local (every use re-reads ceb.pbsp
// from the stack: the game has no register for it). With the oracle's loop.c threshold (1 + n_non_fixed_regs = 95) the 0xAAAAAAAB constant of `plsgOut - alsg`
// (pointer difference of 0x60-byte elements = x * 0xAAAAAAAB >> 5) is hoisted out of the INNER loop only when that has < 93 real insns: then the multiply
// is strength-reduced there with a `mult` initialisation. The game's inner loop is bigger because its VU0 weights go through a helper with by-value VU_FLOAT
// parameters (BlendVectors: four parameter copies per call that cse2 removes later, 101 insns here): K is then hoisted by the OUTER pass together with the
// matching one of the return, and both counts become registers that step by 0x20 with initial value 0 (s5 / s6; the plsgLast giv s0 = s4 is the inner
// pass's only reduction). (2) `pceb = aceb;` first and every aceb[0] access through pceb (the game's s1 is both). (3) The third plane case is its own
// `else if (g1 > 0 && g2 > 0)`; the split case ends in an if / else whose arms are the same assignments again (jump2 merges the tails). (4) The dot
// products are the non-tied form (`%2w`: the plane is not overwritten); the first reads g1 with a plain qmfc2 (VfToFloat: the sqc2 home store lands between
// qmfc2 and mtc1), the second is ONE asm with the qmfc2 + mtc1 inside and its outputs listed vfOne first: vfOne2 is then stored (and dies) before
// the branch and gets vf1, dot2 (stored after it) vf2.
struct VU_QWORD
{
    qword qw;
    VU_QWORD() __asm__("InitVuTemp");
};
struct LSG
{
      qword apos[2];
      qword anormal[2];
      float au[2];
      LSGK lsgk;
      SURF *apsurf[2];
      EDGE *pedge;
      int iiposSwap;
      LSG *plsgNext;
};
struct CEB
{
      BSP *pbsp;
      BSP *pbspNeg;
      float g;
      SURF *psurf;
      qword normal;
};
struct VU_FLOAT
{
    qword qw;
};
// Broadcast a float into a VU0 register.
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "r"(g));
    return v;
}
// pbsp, or null when it lies at or past pbspLimit.
static inline BSP *BspOrNullPastLimit(BSP *pbsp, BSP *pbspLimit)
{
    if (pbsp >= pbspLimit)
    {
        pbsp = 0;
    }
    return pbsp;
}
// Return qw1*g1 + qw2*g2.
static inline qword BlendVectors(VU_FLOAT g1, qword qw1, VU_FLOAT g2, qword qw2)
{
    qword qw;
    __asm__("vmulax.xyzw $ACC, %1, %3x\n\tvmaddx.xyzw %0, %2, %4x" : "=j"(qw) : "j"(qw1), "j"(qw2), "j"(g1.qw), "j"(g2.qw));
    return qw;
}
int ClsgClipEdgeToBsp(BSP *pbspRoot, BSP *pbspLimit, qword pos1, qword pos2, int clsgMax, LSG *alsg) __asm__("ClipSegmentBsp");
// Clips the edge pos1-pos2 against a BSP volume and writes the pieces inside it to alsg (at most clsgMax);
// returns their count. See the long comment above for the algorithm.
// Also known as: ClipSegmentBsp (upstream readable / Sly 1 name; ClipSegmentBsp__FP6BSPREFP6VECTORG9VU_VECTORT2iP3LSG).
int ClsgClipEdgeToBsp(BSP *pbspRoot, BSP *pbspLimit, qword pos1, qword pos2, int clsgMax, LSG *alsg)
{
    CEB ceb;
    CEB aceb[32];
    CEB *pceb;
    LSG *plsgLast;
    LSG *plsgOut;
    pceb = aceb;
    plsgLast = 0;
    plsgOut = alsg;
    memset(&ceb, 0, sizeof(CEB));
    memset(pceb, 0, sizeof(CEB));
    ceb.pbsp = BspOrNullPastLimit(pbspRoot, pbspLimit);
    pceb->g = 1.0f;
    while (1)
    {
        while (ceb.pbsp == 0)
        {
            if (ceb.pbspNeg != 0)
            {
                if (plsgLast != 0 && ceb.g == plsgLast->au[1])
                {
                    plsgLast->apos[1] = BlendVectors(VuFloat(1.0f - pceb->g), pos1, VuFloat(pceb->g), pos2);
                    plsgLast->anormal[1] = pceb->normal;
                    plsgLast->au[1] = pceb->g;
                    plsgLast->apsurf[1] = pceb->psurf;
                }
                else
                {
                    if (plsgOut - alsg >= clsgMax)
                    {
                        return clsgMax;
                    }
                    plsgLast = plsgOut;
                    plsgOut++;
                    memset(plsgLast, 0, sizeof(LSG));
                    plsgLast->apos[0] = BlendVectors(VuFloat(1.0f - ceb.g), pos1, VuFloat(ceb.g), pos2);
                    plsgLast->apos[1] = BlendVectors(VuFloat(1.0f - pceb->g), pos1, VuFloat(pceb->g), pos2);
                    plsgLast->anormal[0] = ceb.normal;
                    plsgLast->anormal[1] = pceb->normal;
                    plsgLast->au[0] = ceb.g;
                    plsgLast->au[1] = pceb->g;
                    plsgLast->apsurf[0] = ceb.psurf;
                    plsgLast->apsurf[1] = pceb->psurf;
                }
            }
            if (pceb <= aceb)
            {
                return plsgOut - alsg;
            }
            ceb = *pceb;
            pceb--;
        }
        qword plane = *(qword *)ceb.pbsp->psurf;
        VU_QWORD dot1;
        VU_QWORD vfOne1;
        __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %2w"
                : "=j"(dot1), "=j"(vfOne1) : "j"(plane), "j"(pos1));
        float g1 = VuToFloat(dot1.qw);
        VU_QWORD dot2;
        VU_QWORD vfOne2;
        float g2;
        int iScratch;
        __asm__("vmul.xyz %1, %4, %5\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddaz.x $ACC, %0, %1z\n\tvmsubw.x %1, %0, %4w\n\tqmfc2 %3, %1\n\tmtc1 %3, %2"
                : "=j"(vfOne2), "=j"(dot2), "=f"(g2), "=&r"(iScratch) : "j"(plane), "j"(pos2));
        if (g1 <= 0.0f && g2 <= 0.0f)
        {
            ceb.pbspNeg = ceb.pbsp;
            ceb.pbsp = BspOrNullPastLimit(ceb.pbsp->pbspNeg, pbspLimit);
        }
        else if (g1 > 0.0f && g2 > 0.0f)
        {
            ceb.pbspNeg = 0;
            ceb.pbsp = BspOrNullPastLimit(ceb.pbsp->pbspPos, pbspLimit);
        }
        else
        {
            float t = g1 / (g1 - g2);
            float gMid = g2 * ceb.g + (1.0f - ceb.g) * g1;
            float gTop = g2 * pceb->g + (1.0f - pceb->g) * g1;
            if (ceb.g < t && t < pceb->g)
            {
                pceb++;
                pceb->g = t;
                pceb->normal = plane;
                pceb->psurf = ceb.pbsp->psurf;
                if (gMid <= 0.0f)
                {
                    pceb->pbspNeg = 0;
                    pceb->pbsp = BspOrNullPastLimit(ceb.pbsp->pbspPos, pbspLimit);
                }
                else
                {
                    pceb->pbspNeg = ceb.pbsp;
                    pceb->pbsp = BspOrNullPastLimit(ceb.pbsp->pbspNeg, pbspLimit);
                }
            }
            if (gMid < 0.0f || (gMid == 0.0f && gTop <= 0.0f))
            {
                ceb.pbspNeg = ceb.pbsp;
                ceb.pbsp = BspOrNullPastLimit(ceb.pbsp->pbspNeg, pbspLimit);
            }
            else
            {
                ceb.pbspNeg = 0;
                ceb.pbsp = BspOrNullPastLimit(ceb.pbsp->pbspPos, pbspLimit);
            }
        }
    }
}

extern BSP D_002BBB10;      /* the "inside" leaf */
extern BSP D_002BBB20;      /* the "outside" leaf */
// p, or null when it lies at or past pnodeMax.
static inline BSP *NodeOrNullPastLimit(BSP *p, BSP *pnodeMax)
{
    if (!(p < pnodeMax))
        p = 0;
    return p;
}
// Builds a reduced copy of a BSP for queries near pos: planes farther than eps from pos are skipped (the walk
// follows the side pos is on), and only the planes within eps are copied as new nodes into *outNodes (capacity
// *capacity, both advanced). Returns 0 or 1 when the result is a plain leaf (the two leaf kinds), 2 when a subtree
// was emitted (its root is the last node written). Used as a broad-phase simplification (inferred).
// Also known as: ClipBspPlane (upstream readable / Sly 1 name; ClipBspPlane__FP6BSPREFP6VECTORG9VU_VECTORfPiPP4NODE).
extern "C" int PruneBspBySphere(BSP *node, BSP *nodeLimit, qword pos, float eps, int *capacity, BSP **outNodes)
{
    float g;
    while (node) {
        qword plane = *PplaneOfSurf(node->psurf);
        VUF d;
        VUF one;
        qword dq, oq;
        __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %0w"
                : "=j"(dq), "=j"(oq) : "j"(plane), "j"(pos));
        int n;
        __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(n), "=f"(g) : "j"(dq));
        one.q = oq;
        d.q = dq;
        if (g > eps) {
            node = NodeOrNullPastLimit(node->pbspPos, nodeLimit);
        } else if (g < -eps) {
            node = NodeOrNullPastLimit(node->pbspNeg, nodeLimit);
        } else {
            int negResult, posResult;
            BSP *negChild = NodeOrNullPastLimit(node->pbspNeg, nodeLimit);
            if (negChild) {
                negResult = PruneBspBySphere(negChild, nodeLimit, pos, eps, capacity, outNodes);
                negChild = *outNodes - 1;
            } else {
                negResult = 0;
            }
            BSP *posChild = NodeOrNullPastLimit(node->pbspPos, nodeLimit);
            if (posChild) {
                posResult = PruneBspBySphere(posChild, nodeLimit, pos, eps, capacity, outNodes);
                posChild = *outNodes - 1;
            } else {
                posResult = 1;
            }
            if (negResult == posResult && posResult != 2)
                return posResult;
            if (*capacity <= 0)
                return 1;
            BSP *newNode = (*outNodes)++;
            (*capacity)--;
            newNode->psurf = node->psurf;
            switch (negResult) {
            case 0: newNode->pbspNeg = 0; break;
            case 1: newNode->pbspNeg = &D_002BBB10; break;
            case 2: newNode->pbspNeg = negChild; break;
            }
            switch (posResult) {
            case 0: newNode->pbspPos = &D_002BBB20; break;
            case 1: newNode->pbspPos = 0; break;
            case 2: newNode->pbspPos = posChild; break;
            }
            return 2;
        }
    }
    return g > 0.0f;
}

// Walks a BSP with a polygon (pointCount points): uses the min/max plane distance over the points to descend into
// the children it straddles, collecting the surfaces of the nodes it touches into (*outCount, *outSurfaces).
extern "C" int CollectBspSurfacesTouchingPolygon(BSP *node, BSP *nodeLimit, int pointCount, qword *points, int *outCount, SURF ***outSurfaces)
{
    if (pointCount == 0)
        return 1;
    while (node) {
        float gMax, gMin;
        {
            qword plane = *PplaneOfSurf(node->psurf);
            qword pos = points[0];
            VUF d;
            VUF one;
            qword dq, oq;
            __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %0w"
                    : "=j"(dq), "=j"(oq) : "j"(plane), "j"(pos));
            register int n __asm__("$3");
            __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(n), "=f"(gMax) : "j"(dq));
            one.q = oq;
            d.q = dq;
            gMin = gMax;
        }
        for (int i = 1; i < pointCount; i++) {
            qword plane = *PplaneOfSurf(node->psurf);
            qword pos = points[i];
            VUF d;
            VUF one;
            qword dq, oq;
            __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %0w"
                    : "=j"(dq), "=j"(oq) : "j"(plane), "j"(pos));
            int n;
            float g;
            __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(n), "=f"(g) : "j"(dq));
            gMin = StdMin(gMin, g);
            gMax = StdMax(gMax, g);
            one.q = oq;
            d.q = dq;
        }
        if (gMin > 1.0e-4f) {
            node = NodeOrNullPastLimit(node->pbspPos, nodeLimit);
            if (!node)
                return 1;
        } else if (gMax < -1.0e-4f) {
            node = NodeOrNullPastLimit(node->pbspNeg, nodeLimit);
            if (!node)
                return 0;
        } else {
            int negResult, posResult;
            BSP *negChild = NodeOrNullPastLimit(node->pbspNeg, nodeLimit);
            if (negChild)
                negResult = CollectBspSurfacesTouchingPolygon(negChild, nodeLimit, pointCount, points, outCount, outSurfaces);
            else
                negResult = 0;
            BSP *posChild = NodeOrNullPastLimit(node->pbspPos, nodeLimit);
            if (posChild)
                posResult = CollectBspSurfacesTouchingPolygon(posChild, nodeLimit, pointCount, points, outCount, outSurfaces);
            else
                posResult = 1;
            if (negResult == posResult && negResult != 2)
                return negResult;
            if (*outCount <= 0)
                return 1;
            SURF *surf = node->psurf;
            if (!(surf->grf & SURF_COLLECTED)) {
                SURF **slot = (*outSurfaces)++;
                *slot = surf;
                surf->grf |= SURF_COLLECTED;
            }
            return 2;
        }
    }
    return 1;
}

// 0x118FA8: dead-code scrap, a nop unit (sly2/scrap.h)
SLY2_SCRAP("nop");

// FindBspLeafContainingPointFast (Claude agent, lane A8, 2026-10-23): BSP-style plane walk. From v6: one asm doing qmfc2.ni + mtc1 (scratch "=&r"), and the negative child clipped into a separate `child = 0; if (negChild < nodeLimit) child = negChild;` (gives the target v1->v0 copy), the positive one straight into node. oq pinned to vf2.
// Returns the BSP leaf containing pos; points within 1e-4 of a plane go to the negative child (fast version).
// Also known as: IbspFindPoint (upstream readable / Sly 1 name; IbspFindPoint__FP6BSPREFP6VECTORG9VU_VECTOR).
extern "C" BSP *FindBspLeafContainingPointFast(BSP *node, BSP *nodeLimit, qword pos)
{
    VUFU d;
    VUFU one;
    if (!node)
        return 0;
    VUFU *pone = &one;
    for (;;) {
        qword plane;
        __asm__("lqc2 %0, %1" : "=j"(plane) : "m"(*PplaneOfSurf(node->psurf)));
        qword dq;
        register qword oq __asm__("vf2"); /* pin: the target allocates the one-vector to vf2 (the dot result then gets vf1) */
        float g;
        InitVuTemp(&d);
        InitVuTemp(pone);
        __asm__("vmul.xyz %0, %0, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %0w"
                : "=j"(dq), "=j"(oq) : "0"(plane), "j"(pos));
        { int t; __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(t), "=f"(g) : "j"(dq)); }
        one.q = oq;
        d.q = dq;
        if (g < 1.0e-4f) {
            BSP *negChild = node->pbspNeg;
            BSP *child = 0;
            if (negChild < nodeLimit)
                child = negChild;
            if (!child)
                return node;
            node = child;
        } else {
            BSP *posChild = node->pbspPos;
            node = 0;
            if (posChild < nodeLimit)
                node = posChild;
            if (!node)
                return 0;
        }
    }
}

// (Claude agent, lane A9, 2026-10-23) BSP point search. On the on-plane path ONE local p holds the checked right child, the
// recursive result and then the checked left child, so it stays in v0 (no if-conversion straight into a0).
// Returns the BSP leaf containing pos; a point within 1e-4 of a plane tries both children (positive first).
// Also known as: IbspFindPointEps (upstream readable / Sly 1 name; IbspFindPointEps__FP6BSPREFP6VECTORG9VU_VECTOR).
extern "C" BSP *FindBspLeafContainingPoint(BSP *node, BSP *nodeLimit, qword pos)
{
    while (node) {
        qword plane = *PplaneOfSurf(node->psurf);
        VUF d;
        VUF one;
        qword dq, oq;
        float g;
        __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %0w"
                : "=j"(dq), "=j"(oq) : "j"(plane), "j"(pos));
        int n;
        __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(n), "=f"(g) : "j"(dq));
        one.q = oq;
        d.q = dq;
        if (g < -1.0e-4f) {
            BSP *p = NodeOrNullPastLimit(node->pbspNeg, nodeLimit);
            if (!p)
                return node;
            node = p;
        } else if (g > 1.0e-4f) {
            BSP *p = NodeOrNullPastLimit(node->pbspPos, nodeLimit);
            if (!p)
                return 0;
            node = p;
        } else {
            BSP *p = NodeOrNullPastLimit(node->pbspPos, nodeLimit);
            if (p) {
                p = FindBspLeafContainingPoint(p, nodeLimit, pos);
                if (p)
                    return p;
            }
            p = NodeOrNullPastLimit(node->pbspNeg, nodeLimit);
            if (!p)
                return node;
            node = p;
        }
    }
    return 0;
}

typedef struct MSG { int size; int alignment; } MSG;   /* a world-heap request (bis.cpp's view) */
typedef struct ITEM { char ab[0x20]; } ITEM;
typedef struct OWNER { char pad[0xC]; ITEM *items; } OWNER;
typedef struct TREE { int nodeCount; int unknown4; int unknown8; BSP *nodes; } TREE;   /* bis.cpp's names */
extern "C" void *AllocFromWorldHeap(MSG *pmsg);
extern char D_002BBAF0[];
#define REBASE(p, baseSrc, baseDst) ((void *)((char *)(baseDst) + ((char *)(p) - (char *)(baseSrc))))
// Copies a BSP collection from one geometry to another, rebasing every internal node and plane pointer from the
// source owner's storage to the destination's.
extern "C" void CloneBspc(TREE *srcTree, OWNER *srcOwner, TREE *dstTree, OWNER *dstOwner)
{
    int i;
    MSG msg;
    dstTree->nodeCount = srcTree->nodeCount;
    dstTree->unknown4 = srcTree->unknown4;
    msg.size = srcTree->nodeCount * sizeof(BSP);
    msg.alignment = 0x10;
    dstTree->nodes = (BSP *)AllocFromWorldHeap(&msg);
    for (i = 0; i < dstTree->nodeCount; i++) {
        BSP *srcNode = &srcTree->nodes[i];
        BSP *dstNode = &dstTree->nodes[i];
        if ((void *)srcNode->psurf == D_002BBAF0) {
            dstNode->psurf = srcNode->psurf;   /* the shared "outside" sentinel plane is not rebased */
        } else {
            dstNode->psurf = (SURF *)REBASE(srcNode->psurf, srcOwner->items, dstOwner->items);
        }
        dstNode->pbspNeg = srcNode->pbspNeg ? (BSP *)REBASE(srcNode->pbspNeg, srcTree->nodes, dstTree->nodes) : 0;
        dstNode->pbspPos = srcNode->pbspPos ? (BSP *)REBASE(srcNode->pbspPos, srcTree->nodes, dstTree->nodes) : 0;
    }
}
#undef REBASE

/* plane from 3 points; qmfc2+sw in one asm puts the d store first; normal copied via float temps (load x,y,z, store y,z,x) */
typedef struct { float x, y, z; } V3;
typedef union { qword q; V3 v; } UV;
// Unit vector along v.
static inline qword Normalize(qword v)
{
    qword d, l, r;
    d = VuDot(v, v);
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l), "j"(v));
    return r;
}
// Initialises a plane through three points: normal = normalize((b - a) x (c - a)), distance = normal . a.
// Also known as: PlaneFromPoints (upstream readable / Sly 1 name; PlaneFromPoints__FP6VECTORG9VU_VECTORN21).
extern "C" void InitPlaneFromPositions(PLANE *outPlane, qword a, qword b, qword c)
{
    UV u;
    qword d;
    int n;
    u.q = Normalize(VuCross(VuSub(b, a), VuSub(c, a)));
    d = VuDot(u.q, a);
    __asm__("qmfc2.ni %0, %2\n\tsw %0, %1" : "=r"(n), "=m"(outPlane->d) : "j"(d));
    { float tx = u.v.x, ty = u.v.y, tz = u.v.z; outPlane->y = ty; outPlane->z = tz; outPlane->x = tx; }
}

// Initialises a plane from a unit normal and a point on it (distance = normal . point).
// Also known as: PlaneFromNormalPos (upstream readable / Sly 1 name; PlaneFromNormalPos__FP6VECTORG9VU_VECTORT1).
extern "C" void InitPlaneFromNormalPos(PLANE *outPlane, qword normal, qword point)
{
    __asm__ __volatile__(
        "qmtc2.ni %1, $vf1\n"
        "qmtc2.ni %2, $vf4\n"
        "vmul.xyz $vf2, $vf1, $vf4\n"
        "vaddw.x $vf3, $vf0, $vf0w\n"
        "vadday.x ACC, $vf2, $vf2y\n"
        "vmaddz.x $vf2, $vf3, $vf2z\n"
        "sqc2 $vf1, 0x0(%0)\n"
        "qmfc2.ni $2, $vf2\n"
        "sw $2, 0xC(%0)\n"
        : : "r"(outPlane), "r"(normal), "r"(point) : "$2", "memory");
}

// Sets a plane from a normal and its distance.
extern "C" void SetPlane(PLANE *outPlane, qword normal, float distance)
{
    *(qword *)outPlane = normal;
    outPlane->d = distance;
}

// 0x1193D0: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("swc1 $f0, 0x4($4)");

// (Claude agent, lane A9, 2026-10-23) from the old draft (21): the dot is ONE asm tied to pos ("0"(pos)), and the float
// readback is an mtc1 asm taking the qword (gcc emits the qmfc2 itself). Together they fix every VF number.
struct VECTORQ;
extern "C" VECTORQ *DefaultConstructVector(VECTORQ *);
typedef int sqword __attribute__((mode(TI)));   /* signed: the linkonce copy is AssignQword__FPxx */
inline sqword *AssignQword(sqword *p, sqword v);   /* inline, defined after its caller (end of file) */
/* A VECTOR held in a qword with its out-of-line empty constructor (DefaultConstructVector). */
struct VECTORQ { qword q; VECTORQ() { (void)DefaultConstructVector(this); } };
struct MAT { qword r[4]; };
// Returns `plane` transformed by a matrix: the normal rotated, a point on the plane moved, and the distance
// recomputed.
extern "C" qword TransformPlane(MAT *matrix, qword plane)
{
    union { qword q; float f[4]; } u;
    qword n, t;
    VECTORQ v;
    v.q = plane;
    n = VuMulMatrix3(matrix->r[0], matrix->r[1], matrix->r[2], v.q);
    t = matrix->r[3];
    AssignQword((sqword *)&u.q, plane);
    float d = u.f[3];
    qword pos, p2;
    qword s;
    s = VuFromFloat<qword>(d);
    u.q = s;
    __asm__("vadda.xyzw $ACC, %2, $vf0\n\tvmaddx.xyzw %0, %1, %3x" : "=j"(pos) : "j"(n), "j"(t), "j"(s));
    p2 = VuDot(pos, n);
    float g;
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(p2));
    SetPlane((PLANE *)&u.q, n, g);
    return u.q;
}

// 0x2564A8 / 0x2564B8: a qword store returning its destination, and VUF's empty constructor returning this: inline,
// defined after their callers, so g++ calls them and emits the out-of-line copies (linkonce), as the original did.
// 0x2564B8 keeps its C symbol (cnvo, cm and path call it by that name, and VUF/VU_QWORD's constructors are
// bound to it).
// Stores a quadword through a pointer and returns the pointer (an inline assignment kept out of line in the
// linkonce area).
inline sqword *AssignQword(sqword *p, sqword v) { *p = v; return p; }

// The empty constructor of the VU temporaries VUF / VU_QWORD: returns its argument (an out-of-line inline copy).
extern "C" inline void *InitVuTemp(void *p)
{
    return p;
}

// ---- bsp's .data (0x2BBAD0..0x2BBB30): the two sentinel planes (normal 0, distance -1e8 / +1e8, each padded to 32
// bytes) and the "inside" / "outside" leaves that point at them (BSP's third word; each leaf padded to 16 bytes). The
// trees read from a level end in these leaves (bis.cpp's reader stores D_002BBAF0 as a leaf's plane, CloneBspc
// compares against it, mark.cpp links to the outside leaf). Names by address; under asm labels so that no declaration
// above has to agree with the definitions' types.
namespace BSP_DATA {

extern float d_D_002BBAD0[8] __asm__("D_002BBAD0");
extern float d_D_002BBAF0[8] __asm__("D_002BBAF0");

float d_D_002BBAD0[8] __attribute__((aligned(16))) = {0.0f, 0.0f, 0.0f, -1.0e8f, 0.0f, 0.0f, 0.0f, 0.0f};
float d_D_002BBAF0[8] __attribute__((aligned(16))) = {0.0f, 0.0f, 0.0f, 1.0e8f, 0.0f, 0.0f, 0.0f, 0.0f};
struct BSPLEAF { BSP bsp; } __attribute__((aligned(16)));   /* a leaf node padded to a quadword */

extern BSPLEAF d_D_002BBB10 __asm__("D_002BBB10");
extern BSPLEAF d_D_002BBB20 __asm__("D_002BBB20");

BSPLEAF d_D_002BBB10 = {{0, 0, (SURF *)d_D_002BBAD0}};
BSPLEAF d_D_002BBB20 = {{0, 0, (SURF *)d_D_002BBAF0}};
}
