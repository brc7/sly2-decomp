// cnvo.cpp -- OBSTACLE, the convex polygon SHAPE ("convex volume", CNVO): a closed outline in the XY plane used as
// a navigation obstacle. Role: its load / clone / hierarchy overrides, the winding and convexity test of its
// outline, its side planes and segment clipping against them, point-inside and overlap tests, registration as an
// obstacle in the path zone that contains it, the search for free points/ranges along a shape that avoid the
// obstacles, and the merging of overlapping polygons into one convex hull (a pooled "merged" OBSTACLE owner).
// Entry points: LoadCid14FromBrx, PostCid14Load, ClipEdgeToConvexObstacle, IsPosInConvexObstacle, DoConvexObstaclesOverlap, FindShapeFreePoint,
// MergeCid14Obstacles, UpdatePendingCid14Obstacles.
// TU 0x1BC050..0x1BE398 (31 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_cnvo
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_cnvo
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/sw.h"
#include "sly2/classes.h"
static inline ::LO *AsLo(void *plo) { return (::LO *)plo; }   /* the real class behind the views below, for its virtual calls */
#include "sly2/vu0.h"
#include "sly2/dli.h"
#include "sly2/port.h"   /* SLY2_EE_LAYOUT */
#if defined(SLY2_EE_LAYOUT)
/* Entry i of a BASIC-derived object's g++ 2.95 vtable (vptr at +0xC), for the hand-written virtual calls below. */
#define SLY2_VTE(p, i) ((*(VTE **)((char *)(p) + 0xC))[i])
#endif

// 0x1BE240 StaticInit_cnvo and GlobalConstructors_cnvo: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

union U { qword q; float f[4]; };
/* ClassifyCid14Polygon results used here: a convex outline in the expected winding, or convex but wound the other
   way (PostCid14Load reverses those). Other values: not convex (inferred). */
enum { WINDING_CONVEX = 3, WINDING_CONVEX_REVERSED = 4 };

/* The 0x60-byte clipped segment (same record as ClsgClipEdgeToBsp's). */
struct LSG {
    qword apos[2];
    qword anormal[2];
    float au[2];
    int lsgk;
    void *apsurf[2];
    void *pedge;
    int iiposSwap;
    LSG *plsgNext;
};

// Allocation request: byte count and alignment.
struct AREQV { int cb; int align; AREQV(int c, int a) { cb = c; align = a; } };

struct VU_QWORD;
extern "C" VU_QWORD *InitVuTemp(VU_QWORD *);
// VU temporary initialised through InitVuTemp.
struct VU_QWORD { qword qw; VU_QWORD() { (void)InitVuTemp(this); } };
struct VU_FLOAT { qword qw; };
// Wrap a qword as a VU_VECTOR.
struct VU_VECTOR { qword qw; VU_VECTOR(const qword &q) { qw = q; } };

extern "C" {
/* cnvo's .data (address order; the 24 ints after D_002C0AC4 are named by no code: claimed by layout, the rest
   of the gap before rtcam's .data: 24 words, rows of 4 by their pattern; type unknown). */
float D_002C0AB0 = 1.0f, D_002C0AB4 = 1.0f, D_002C0AB8 = 0.2f, D_002C0ABC = 10.0f, D_002C0AC0 = 50.0f, D_002C0AC4 = 200.0f;
int D_002C0AC8[24] = {1, 1, 2, 1, 4, 2, 8, 4, 2, 2, 2, 2, 4, 2, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4};
extern const float D_002A76D0;   /* defined below where the ELF has it (.rodata order) */
extern const float D_002A76D4;   /* defined below where the ELF has it (.rodata order) */
extern qword g_normalZ, D_002A2DD0;
extern char D_003DD020[];

void InitShape(OBSTACLE *p);
void InitDl__FP2DLi(DL *pdl, int ib);
void LoadShapeFromBrx(OBSTACLE *p);
void *AllocClearedFromWorldHeap(SWALLOC *pmsg);
void *AllocFromWorldHeap(SWALLOC *preq);
void *AllocFromStack(const AREQV &r);
void CloneShape(OBSTACLE *p, LO *ploBase);
void AddLoHierarchy(OBSTACLE *p);
void RemoveLoHierarchy(OBSTACLE *p);
void AppendDlEntry__FP2DLPv(DL *pdl, void *pv);
void RemoveDlEntry__FP2DLPv(DL *pdl, void *pv);
int IsEntryInDl__FP2DLPv(DL *pdl, void *pv);
int IsDlEmpty__FP2DL(DL *pdl);
void PostLoLoad(OBSTACLE *p);
float GetShapeMaxParam(OBSTACLE *p);
void MeasureShape(OBSTACLE *p);
void *memset(void *, int, unsigned);
int FindShapeSegmentAtDistance(OBSTACLE *p, float g, int a, int b);
void EvaluateShapeAtDistance(OBSTACLE *p, qword *pv, float g, int n);
int IsEdgeBlockedByPathzone(void *pv, qword a, qword b, int k);
float GetShapeMaxDistance(OBSTACLE *p);
float GetShapeMinDistance(OBSTACLE *p);
float WrapShapeDistance(OBSTACLE *p, float g);
int IsPosInPathzone(void *b, qword v, int c);
qword GetLoWorldPos(OBSTACLE *p);
void *FindPathzoneAtPos(qword pos, int n);
void AddPathzoneObstacle(void *r, OBSTACLE *p);
void RemovePathzoneObstacle(void *ppathzone, OBSTACLE *p);
int FindPathzoneObstacles(void *a, void *b, qword q2, qword q3, int c, OBSTACLE **buf);
void InitPlaneFromNormalPos(qword *pplane, qword n, qword pos);
void FindShapeClosestPoint(OBSTACLE *x, qword q, void *g, qword *a, qword *b, float *c, float *pf);
OBSTACLE *CreateLo__F3CIDP2SO3OIDi(int a, SO *b, int c, int d);
void func_002333B4(void *p, int c, int n) __asm__("memset");
void qsort(void *base, __SIZE_TYPE__ n, __SIZE_TYPE__ cb, int (*cmp)(const void *, const void *));
void CopyAb__FPvT0Ui(void *dst, const void *src, int cb);

int ComparePosXY(qword a, qword b);
int GetTurnSignXY(qword a, qword b, qword c);
int ClassifyCid14Polygon(OBSTACLE *p);
int WrapCid14PointIndex(OBSTACLE *p, int n);
int ClipEdgeToConvexObstacle(OBSTACLE *pcnvo, qword pos1, qword pos2, int clsgMax, LSG *alsg);
int IsPosInConvexObstacle(OBSTACLE *p, qword pos);
int FindCid14PathzoneSpan(OBSTACLE *p, void *pv, int fRev, float g0, float g1);
int ClipShapeToPathzone(OBSTACLE *s, void *x, float *pt, int mode, float t);
int FindShapePointInPathzone(OBSTACLE *a, void *b, float g, float range, float *pf, int c);
void RegisterCid14Obstacle(OBSTACLE *p);
void UnregisterCid14Obstacle(OBSTACLE *p);
void DetachCid14FromOwner(OBSTACLE *p);
OBSTACLE *FindFirstObstacleHitBySegment(void *a, void *b, qword q2, qword q3);
void BuildCid14EdgePlanes(OBSTACLE *p);
int FindShapeFreeRange(OBSTACLE *p, OBSTACLE *s, int fForward, qword *pq, float *pt, float t);
OBSTACLE *NewMergedCid14Obstacle(void);
int ComparePosX(const void *a, const void *b);
void BuildConvexHull(OBSTACLE *p, int n, qword *pts);
void AddCid14ToMerged(OBSTACLE *p, OBSTACLE *po);
}
/* The callers' own argument order (floats first): the order in which g++ loads the argument registers follows it. */
int func_001BCAC0F(OBSTACLE *p, void *pv, float g0, float g1, int fRev) __asm__("FindCid14PathzoneSpan");
int func_001BD560F(OBSTACLE *p, OBSTACLE *s, float t, int fForward, qword *pq, float *pt) __asm__("FindShapeFreeRange");

// Fill the heap message and allocate cb zeroed bytes (kind nKind) from the world heap.
static __inline__ void *PvAllocMsg(SWALLOC *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->cbAlign = nKind; return AllocClearedFromWorldHeap(pmsg); }
// Allocate cb bytes, 16-byte aligned, from the frame stack.
static inline void *Alloc(int cb) { return AllocFromStack(AREQV(cb, 0x10)); }
/* The world matrix: virtual slot 31. */
static inline MATRIX4Q *GetMat(OBSTACLE *p) { return (MATRIX4Q *)AsLo(p)->GetWorldMatrix(); }
// Transform point v by the 4x4 matrix m.
static inline qword MulPt(MATRIX4Q *m, qword v) { qword r; __asm__("vmulax.xyz $ACC, %1, %2x\n\tvmadday.xyz $ACC, %3, %2y\n\tvmaddaz.xyz $ACC, %4, %2z\n\tvmaddw.xyz %0, %5, $vf0w" : "=j"(r) : "j"(m->r[0].q), "j"(v), "j"(m->r[1].q), "j"(m->r[2].q), "j"(m->r[3].q)); return r; }

// Slot 7 (Init): the SHAPE init plus the polygon's defaults.
void OBSTACLE::Init()
{
    OBSTACLE *p = (OBSTACLE *)this;
    InitShape(p);
    p->planeStamp = -1;
    InitDl__FP2DLi(&p->members, SLY2_OFFSETOF(OBSTACLE, dleMerged));
}

// Slot 14 (LoadFromBrx): LoadShapeFromBrx, then allocates one side plane per outline edge (count - 1).
// Layout parse-verified: a parser following it reads every byte of 4 levels.
extern "C" void LoadCid14FromBrx(OBSTACLE *polygon)
{
    SWALLOC msg;
    LoadShapeFromBrx(polygon);
    polygon->edgePlanes = (qword *)PvAllocMsg(&msg, (polygon->numControlVertices << 4) - 0x10, 0x10);
}

// Slot 41 (Clone): CloneShape and a copy of the side-plane array.
extern "C" void CloneCid14(OBSTACLE *polygon, LO *ploBase)
{
    DLE a = polygon->dleMerged;
    DLE b = polygon->dlePending;
    SWALLOC req;
    CloneShape(polygon, ploBase);
    polygon->dleMerged = a;
    polygon->dlePending = b;
    polygon->edgePlanes = (qword *)PvAllocMsg(&req, polygon->numControlVertices * 16 - 16, 0x10);
}

// Slot 11: AddLoHierarchy, then queues the polygon on the pending-obstacle list g_sw.dlCid14Pending (registered
// later by UpdatePendingCid14Obstacles).
extern "C" void AddCid14Hierarchy(OBSTACLE *polygon)
{
    AddLoHierarchy(polygon);
    AppendDlEntry__FP2DLPv((DL *)&g_sw.dlCid14Pending, polygon);
}

// Slot 12: RemoveLoHierarchy, detaches the polygon from its merged owner, unregisters the obstacle and leaves the
// pending list.
// Also known as: RemoveNavPoly (upstream readable / Sly 1 name; RemoveNavPoly__FP2LO).
extern "C" void RemoveCid14Hierarchy(OBSTACLE *polygon)
{
    DL *pdl;
    RemoveLoHierarchy(polygon);
    if (polygon->owner) DetachCid14FromOwner(polygon->owner);
    UnregisterCid14Obstacle(polygon);
    pdl = (DL *)&g_sw.dlCid14Pending;
    if (IsEntryInDl__FP2DLPv(pdl, polygon)) RemoveDlEntry__FP2DLPv(pdl, polygon);
}

// Lexicographic comparison of two positions by x, then y (sort callback for the hull).
int ComparePosXY(qword a, qword b)
{
    if (QwordX(a) < QwordX(b)) return -1;
    if (QwordX(b) < QwordX(a)) return 1;
    if (QwordY(a) < QwordY(b)) return -1;
    if (QwordY(b) < QwordY(a)) return 1;
    return 0;
}

// Sign of the 2D cross product of (a - b) and (b - c) with an epsilon: -1, 0 or 1 for a right turn, straight, left.
int GetTurnSignXY(qword a, qword b, qword c)
{
    float g = (QwordX(a) - QwordX(b)) * (QwordY(b) - QwordY(c)) - (QwordY(a) - QwordY(b)) * (QwordX(b) - QwordX(c));
    if (g < -D_002C0AB0) return -1;
    if (D_002C0AB0 < g) return 1;
    return 0;
}

// Classifies the outline from its turn signs: too few points, convex clockwise, convex counter-clockwise, or not
// convex.
int ClassifyCid14Polygon(OBSTACLE *polygon)
{
    int cFlip, i, sPrev, sgn;
    MATRIX4Q m;
    if (polygon->numControlVertices < 2)
        return 2;
    cFlip = 0;
    i = 0;
    sPrev = 0;
    m = *GetMat(polygon);
    sgn = ComparePosXY(MulPt(&m, polygon->vertexPosq[0].q), MulPt(&m, polygon->vertexPosq[1].q));
    for (; i < polygon->numControlVertices - 1; i++) {
        int i0 = WrapCid14PointIndex(polygon, i);
        int i1 = WrapCid14PointIndex(polygon, i0 + 1);
        int i2 = WrapCid14PointIndex(polygon, i1 + 1);
        qword c = MulPt(&m, polygon->vertexPosq[i2].q);
        qword b = MulPt(&m, polygon->vertexPosq[i1].q);
        qword a = MulPt(&m, polygon->vertexPosq[i0].q);
        int s = ComparePosXY(b, c);
        int t;
        if (s) {
            if (s == -sgn)
                cFlip++;
        }
        sgn = s;
        t = GetTurnSignXY(a, b, c);
        if (t) {
            if (t == -sPrev)
                return 0;
            sPrev = t;
        }
    }
    if (cFlip >= 3)
        return sPrev == 0;
    switch (sPrev) {
    case -1:
        return 4;
    case 1:
        return 3;
    }
    return 2;
}

// Wraps a point index into [0, count - 1) for the closed outline (the last point repeats the first).
// Also known as: IpolyWrap (upstream readable / Sly 1 name; IpolyWrap__FP7NAVPOLYi).
int WrapCid14PointIndex(OBSTACLE *polygon, int index)
{
    int c = polygon->numControlVertices;
    int r = index % c;
    if (r < 0) r = r + (c - 1);
    else if (r == c - 1) r = 0;
    return r;
}

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) V4;
// Swap two vectors.
static __inline__ void SwapV(V4 *a, V4 *b) { V4 t = *a; *a = *b; *b = t; }
// Swap two floats.
static __inline__ void SwapG(float *a, float *b) { float t = *a; *a = *b; *b = t; }
// Slot 15 (PostLoad): PostLoLoad, classifies the outline and reverses or re-measures it when needed so it is convex
// with a known winding.
// Also known as: ReverseLoxPath (upstream readable / Sly 1 name; ReverseLoxPath__FP2LO).
extern "C" void PostCid14Load(OBSTACLE *polygon)
{
    int r, half, i, j;
    float tMax;
    PostLoLoad(polygon);
    r = ClassifyCid14Polygon(polygon);
    polygon->winding = r;
    if (r == WINDING_CONVEX) return;   /* redundant with the next test, but part of the codegen */
    if (r != WINDING_CONVEX_REVERSED) return;
    half = polygon->numControlVertices / 2;
    tMax = GetShapeMaxParam(polygon);
    for (i = 0; i < half; i++) {
        j = polygon->numControlVertices - i - 1;
        SwapV(&((V4 *)polygon->vertexPosq)[i], &((V4 *)polygon->vertexPosq)[j]);
        SwapG(&polygon->vertexParams[i], &polygon->vertexParams[j]);
        polygon->vertexParams[i] = tMax - polygon->vertexParams[i];
        polygon->vertexParams[j] = tMax - polygon->vertexParams[j];
    }
    MeasureShape(polygon);
    polygon->winding = WINDING_CONVEX;
}

// ClipEdgeToConvexObstacle: clips the segment pos1-pos2 to the convex volume pcnvo (its cplane - 1 planes, built by BuildCid14EdgePlanes)
// and writes the parts of it OUTSIDE the volume to alsg (at most clsgMax LSGs); returns how many. Convex-volume twin of
// ClsgClipEdgeToBsp (ClipSegmentBsp), same LSG record (0x60 bytes: apos[2], anormal[2], au[2], apsurf[2]).
// gIn / gOut = where the segment enters / leaves the volume (0 = pos1, 1 = pos2), normalIn / normalOut = those planes.
// If the segment never enters (gOut <= gIn) it is emitted whole; else [0, gIn] and [gOut, 1] are emitted with the
// negated plane normal at the cut.
// How it matches (T1830, no pins; lilly5/lilly7 were the earlier near-misses, see notes/1B0000.md):
// - the game keeps a register copy of the plane for the else branch (`lq s1,0x70` in the 2nd ctor's delay slot). A plain
//   `qword normal = plane` dies in cse/gcse copy-propagation; constructing a VU_VECTOR from it (inline ctor taking a
//   const qword &) keeps the copy, and the other uses (dot asms, if branch) read the spilled plane at sp+0x70.
// - in the else branch `normalIn = normal.qw` comes AFTER the gIn update: that makes the copy live 2 insns longer, so its
//   global-alloc priority drops below i's (i gets s0, the copy s1, as in the game).
// - GXOfQw: the qmfc2 after the 2nd dot product is a compiler move (unit vu0, so the sqc2 home stores don't pair with it)
//   and the mtc1 is asm. Its extra "j" inputs are allocation-only: the duplicate "j"(vf) keeps dot2 VU-class (else it is
//   stored with sq), the two "j"(vfLive) raise vfOne2's local-alloc priority so it gets vf1 like vfOne1.
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2.ni %1, %0" : "=j"(v.qw) : "r"(g));
    return v;
}
// x lane of vf as a float (the "j" inputs only influence allocation).
static inline float GXOfQw(const VU_QWORD &vf, const VU_QWORD &vfLive)
{
    float g;
    __asm__("mtc1 %1,%0" : "=f"(g) : "r"(vf.qw), "j"(vf.qw), "j"(vfLive.qw), "j"(vfLive.qw)); /* "j" inputs: allocation only (see header) */
    return g;
}
// Return qw1*g1 + qw2*g2.
static inline qword QwMix(VU_FLOAT g1, const qword &qw1, VU_FLOAT g2, const qword &qw2)
{
    qword qw;
    __asm__("vmulax.xyzw $ACC, %1, %3x\n\tvmaddx.xyzw %0, %2, %4x" : "=j"(qw) : "j"(qw1), "j"(qw2), "j"(g1.qw), "j"(g2.qw));
    return qw;
}
// Return -v.
static inline qword QwNeg(qword v) { qword qw; __asm__("vsub.xyzw %0, $vf0, $vf0\n\tvsub.xyzw %0, %0, %1" : "=&j"(qw) : "j"(v)); return qw; }
// Clips a segment against the polygon's side planes, writing the inside span(s) as LSG entries; returns the count.
// Also known as: ChitPoly (upstream readable / Sly 1 name; ChitPoly__FP7NAVPOLYG9VU_VECTORT1iP7POLYHIT).
int ClipEdgeToConvexObstacle(OBSTACLE *polygon, qword start, qword end, int maxSegments, LSG *segments)
{
    BuildCid14EdgePlanes(polygon);
    float gIn = 0.0f;
    qword normalIn = VuZero<qword>();
    float gOut = 1.0f;
    qword normalOut = normalIn;
    int i;
    for (i = 0; i < polygon->numControlVertices - 1; i++) {
        qword plane = polygon->edgePlanes[i];
        VU_VECTOR normal(plane);
        VU_QWORD dot1;
        VU_QWORD vfOne1;
        __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %2w"
                : "=j"(dot1), "=j"(vfOne1) : "j"(plane), "j"(start));
        float g1 = VuToFloat(dot1.qw);
        VU_QWORD dot2;
        VU_QWORD vfOne2;
        __asm__("vmul.xyz %1, %2, %3\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddaz.x $ACC, %0, %1z\n\tvmsubw.x %1, %0, %2w"
                : "=j"(vfOne2), "=j"(dot2) : "j"(plane), "j"(end));
        float g2 = GXOfQw(dot2, vfOne2);
        g1 += D_002C0AB4;
        g2 += D_002C0AB4;
        if (g1 < 0.0f && g2 < 0.0f)
            continue;
        if (g1 > 0.0f && g2 > 0.0f) {
            gIn = gOut = 0.0f;
            break;
        }
        float t = g1 / (g1 - g2);
        if (g1 < 0.0f) {
            gOut = (gOut < t) ? gOut : t;
            normalOut = plane;
        } else {
            gIn = (t < gIn) ? gIn : t;
            normalIn = normal.qw;
        }
        if (gOut <= gIn)
            break;
    }
    int clsg = 0;
    LSG *plsg;
    if (gOut <= gIn && clsg < maxSegments) {
        plsg = &segments[clsg++];
        memset(plsg, 0, sizeof(LSG));
        plsg->apos[0] = start;
        plsg->au[1] = 1.0f;
        plsg->apos[1] = end;
        plsg->au[0] = 0.0f;
    } else {
        if (gIn > 0.0f && clsg < maxSegments) {
            plsg = &segments[clsg++];
            memset(plsg, 0, sizeof(LSG));
            plsg->apos[0] = start;
            plsg->apos[1] = QwMix(VuFloat(1.0f - gIn), start, VuFloat(gIn), end);
            plsg->anormal[1] = QwNeg(normalIn);
            plsg->au[0] = 0.0f;
            plsg->au[1] = gIn;
            plsg->apsurf[1] = polygon;
        }
        if (gOut < 1.0f && clsg < maxSegments) {
            plsg = &segments[clsg++];
            memset(plsg, 0, sizeof(LSG));
            plsg->apos[0] = QwMix(VuFloat(1.0f - gOut), start, VuFloat(gOut), end);
            plsg->anormal[0] = QwNeg(normalOut);
            plsg->apos[1] = end;
            plsg->au[0] = gOut;
            plsg->au[1] = 1.0f;
            plsg->apsurf[0] = polygon;
        }
    }
    return clsg;
}

union UF { qword q; float f; };
// Signed distance from pos to plane (plane.xyz . pos - plane.w).
static __inline__ float GDistPlane(qword plane, qword pos)
{
    UF d, one;
    qword td, tone;
    float dist;
    InitVuTemp((VU_QWORD *)&d);
    InitVuTemp((VU_QWORD *)&one);
    __asm__("vmul.xyz %0, %3, %4\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %3w\n\tqmfc2.ni %2, %0" : "=j"(td), "=&j"(tone), "=r"(dist) : "j"(plane), "j"(pos));
    one.q = tone;
    d.q = td;
    return dist;
}
// Returns 1 when a point is inside the polygon (in front of no side plane).
// Also known as: FPolyContains (upstream readable / Sly 1 name; FPolyContains__FP7NAVPOLYG9VU_VECTOR).
int IsPosInConvexObstacle(OBSTACLE *polygon, qword pos)
{
    int i;
    BuildCid14EdgePlanes(polygon);
    for (i = 0; i < polygon->numControlVertices - 1; i++) {
        if (0.0f < GDistPlane(polygon->edgePlanes[i], pos))
            return 0;
    }
    return 1;
}

// Transform point v by the 4x4 matrix pm.
static inline qword MulPos(MATRIX4Q *pm, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(r) : "j"(pm->r[0].q), "j"(pm->r[1].q), "j"(pm->r[2].q), "j"(pm->r[3].q), "j"(v));
    return r;
}
// Walks the polygon's edges against the path zone (IsEdgeBlockedByPathzone) to find the span of the outline that
// lies inside walkable space. (inferred)
// Also known as: NPolyClearSide (upstream readable / Sly 1 name; NPolyClearSide__FP7NAVPOLYP9NAVREGIONiff).
int FindCid14PathzoneSpan(OBSTACLE *polygon, void *zone, int isReversed, float start, float end)
{
    int i0, r, pass, k, i1;
    i0 = FindShapeSegmentAtDistance(polygon, start, 0, 0);
    i1 = FindShapeSegmentAtDistance(polygon, end, 0, 0);
    for (pass = 0; pass < 2; pass++) {
        int i, iEnd, a;
        MATRIX4Q m;
        qword v0, v1, q, v;
        switch (pass) {
        case 0: r = 0; k = 0; break;
        case 1: r = 1; k = 1; break;
        }
        if (isReversed) {
            a = WrapCid14PointIndex(polygon, i0 + 1);
            iEnd = WrapCid14PointIndex(polygon, i1 + 1);
        } else {
            a = i0;
            iEnd = i1;
        }
        i = a;
        m = *GetMat(polygon);
        EvaluateShapeAtDistance(polygon, &v0, start, 0);
        q = v0;
        while (i != iEnd) {
            v = MulPos(&m, polygon->vertexPosq[i].q);
            if (IsEdgeBlockedByPathzone(zone, q, v, k))
                goto next;
            if (isReversed)
                i = WrapCid14PointIndex(polygon, i + 1);
            else
                i = WrapCid14PointIndex(polygon, i - 1);
            q = v;
        }
        EvaluateShapeAtDistance(polygon, &v1, end, 0);
        if (!IsEdgeBlockedByPathzone(zone, q, v1, k))
            return r;
    next:;
    }
    return 2;
}

// Finds the parameter range of a shape that lies inside its path zone; returns a status code. (inferred)
// Also known as: NPolyWay (upstream readable / Sly 1 name; NPolyWay__FP7NAVPOLYP9NAVREGIONPfif).
int ClipShapeToPathzone(OBSTACLE *shape, void *zone, float *paramOut, int mode, float param)
{
    float at[2];
    float tB, d, f;
    int fShort, a, b;
    if (!FindShapePointInPathzone(shape, zone, param, GetShapeMaxDistance(shape) * 0.5f, &at[0], 0) &&
        !FindShapePointInPathzone(shape, zone, param, GetShapeMaxDistance(shape) * 0.5f, &at[0], 1)) return 4;
    param = at[0];
    if (!FindShapePointInPathzone(shape, zone, *paramOut, GetShapeMaxDistance(shape) * 0.5f, &at[1], 0) &&
        !FindShapePointInPathzone(shape, zone, *paramOut, GetShapeMaxDistance(shape) * 0.5f, &at[1], 1)) return 4;
    tB = at[1];
    *paramOut = tB;
    d = tB - param;
    if (d < 0.0f) d += GetShapeMaxDistance(shape);
    switch (mode) {
    case 2: f = D_002C0AB8 + 1.0f; break;
    case 1: f = 1.0f - D_002C0AB8; break;
    default: f = 1.0f; break;
    }
    fShort = (d < GetShapeMaxDistance(shape) * f * 0.5f) ? 1 : 0;
    a = func_001BCAC0F(shape, zone, param, tB, 1);
    b = func_001BCAC0F(shape, zone, param, tB, 0);
    if (b < a) return 1;
    if (a < b) return 2;
    if (a != 2) return fShort ? 2 : 1;
    return 4;
}

// Searches along a shape for a point inside the path zone; returns 1 and the point when found. (inferred)
// Also known as: FPolyPointOnRegion (upstream readable / Sly 1 name; FPolyPointOnRegion__FP7NAVPOLYP9NAVREGIONffPfPv).
int FindShapePointInPathzone(OBSTACLE *shape, void *zone, float param, float range, float *paramOut, int c)
{
    float d, t;
    U v0, v1;
    for (d = 0.0f; d < range; d += D_002C0ABC) {
        t = WrapShapeDistance(shape, param - d);
        EvaluateShapeAtDistance(shape, &v0.q, t, 0);
        if (IsPosInPathzone(zone, v0.q, c)) {
            if (paramOut) *paramOut = t;
            return 1;
        }
        t = WrapShapeDistance(shape, param + d);
        EvaluateShapeAtDistance(shape, &v1.q, t, 0);
        if (IsPosInPathzone(zone, v1.q, c)) {
            if (paramOut) *paramOut = t;
            return 1;
        }
    }
    return 0;
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A76D0 __asm__("D_002A76D0") = -3.40282347e+38f; }
namespace RO_ORDER { extern const float r_D_002A76D4 __asm__("D_002A76D4") = 3.40282347e+38f; }
// Registers the polygon as an obstacle: transforms the outline to world space, finds its path zone
// (FindPathzoneAtPos), computes its bounds and calls AddPathzoneObstacle.
void RegisterCid14Obstacle(OBSTACLE *polygon)
{
    STACKSCOPE ctx;
    int i;
    qword *a = (qword *)Alloc((polygon->numControlVertices << 4) - 1);
    MATRIX4Q m = *GetMat(polygon);
    void *r;
    {
        qword v0 = MulPt(&m, polygon->vertexPosq[0].q);
        polygon->boundsMin.q = v0;
        polygon->boundsMax.q = v0;
        a[0] = v0;
    }
    a[polygon->numControlVertices - 1] = polygon->boundsMin.q;
    for (i = 1; i < polygon->numControlVertices - 1; i++) {
        qword v = MulPt(&m, polygon->vertexPosq[i].q);
        polygon->boundsMin.q = VuMin(polygon->boundsMin.q, v);
        polygon->boundsMax.q = VuMax(polygon->boundsMax.q, v);
        a[i] = v;
    }
    polygon->boundsMin.f[2] = D_002A76D0;
    polygon->boundsMax.f[2] = D_002A76D4;
    r = FindPathzoneAtPos(GetLoWorldPos(polygon), 0);
    if (!r)
        return;
    AddPathzoneObstacle(r, polygon);
}

// Removes the polygon's obstacle registration if it has one.
// Also known as: RemovePolyFromRegion (upstream readable / Sly 1 name; RemovePolyFromRegion__FP7NAVPOLY).
void UnregisterCid14Obstacle(OBSTACLE *polygon)
{
    void *ppathzone = polygon->pathzone;
    if (ppathzone != 0) {
        RemovePathzoneObstacle(ppathzone, polygon);
        polygon->pathzone = 0;
    }
}

// Removes the polygon from its merged owner's member list (with a safe list iterator) and queues it again.
// Also known as: DissolveMerged (upstream readable / Sly 1 name; DissolveMerged__FP2LO).
void DetachCid14FromOwner(OBSTACLE *polygon)
{
    DLI dli;
    OBSTACLE *pv;
    InitDli(&dli, &polygon->members);
    while ((pv = (OBSTACLE *)PvNextDli(&dli)) != 0) {
        RemoveDlEntry__FP2DLPv(&polygon->members, pv);
        pv->owner = 0;
        if (!IsEntryInDl__FP2DLPv((DL *)&g_sw.dlCid14Pending, pv)) AppendDlEntry__FP2DLPv((DL *)&g_sw.dlCid14Pending, pv);
    }
    AsLo(polygon)->Remove();
    AppendDlEntry__FP2DLPv((DL *)&g_sw.dlCid14Free, polygon);
    g_pdliFirst = dli.m_pdliNext;
}

// Among the obstacles overlapping a segment, returns the one the segment enters first.
// Also known as: PpolyOnSegment (upstream readable / Sly 1 name; PpolyOnSegment__FP9NAVREGIONP6VECTORG9VU_VECTORT2).
OBSTACLE *FindFirstObstacleHitBySegment(void *a, void *b, qword start, qword end)
{
    OBSTACLE *buf[0x80];
    LSG r[2];
    OBSTACLE *best = 0;
    float min;
    int i, n;
    n = FindPathzoneObstacles(a, b, start, end, 0x80, buf);
    min = 1.0f;
    for (i = 0; i < n; i++) {
        OBSTACLE *e = buf[i];
        if (!ClipEdgeToConvexObstacle(e, start, end, 2, r) || r[0].au[0] != 0.0f) {
            best = e;
            break;
        }
        if (r[0].au[1] >= min) continue;
        min = r[0].au[1];
        best = e;
    }
    return best;
}

// Unit vector along n.
static inline qword Normalize(qword n)
{
    qword l, one, r;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(n));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(l));
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l), "j"(n));
    return r;
}
// Rebuilds the polygon's side planes in world space when its transform changed since the last build.
void BuildCid14EdgePlanes(OBSTACLE *polygon)
{
    int i;
    int k = polygon->parentTransform ? polygon->parentTransform->worldStamp : 0;
    if (polygon->planeStamp != k) {
        MATRIX4Q m;
        qword v;
        i = 1;
        m = *GetMat(polygon);
        v = MulPt(&m, polygon->vertexPosq[0].q);
        if (i < polygon->numControlVertices) {
            do {
                qword v0 = v;
                v = MulPt(&m, polygon->vertexPosq[i].q);
                InitPlaneFromNormalPos(&polygon->edgePlanes[i - 1], Normalize(VuCross(VuSub(v, v0), g_normalZ)), v0);
            } while (++i < polygon->numControlVertices);
        }
        polygon->planeStamp = k;
    }
}

// Steps along a shape from a parameter until it leaves the obstacles; returns the free range found. (inferred)
// Also known as: NPathLeavesPoly (upstream readable / Sly 1 name; NPathLeavesPoly__FP7NAVPOLYP3CRViP6VECTORPff).
int FindShapeFreeRange(OBSTACLE *obstacle, OBSTACLE *shape, int isForward, qword *posOut, float *paramOut, float param)
{
    qword av[2];
    LSG ahit[2];
    float dt;
    int fWrapped;
    int c;
    EvaluateShapeAtDistance(shape, &av[1], param, 0);
    dt = D_002C0AC0;
    if (!isForward) dt = -dt;
    fWrapped = 0;
    for (;;) {
        av[0] = av[1];
        param += dt;
        if (param <= GetShapeMinDistance(shape)) {
            if (fWrapped) break;
            fWrapped = 1;
            if (shape->isClosed) param = WrapShapeDistance(shape, param);
            else param = GetShapeMinDistance(shape);
        } else if (GetShapeMaxDistance(shape) <= param) {
            if (fWrapped) break;
            fWrapped = 1;
            if (shape->isClosed) param = WrapShapeDistance(shape, param);
            else param = GetShapeMaxDistance(shape);
        }
        EvaluateShapeAtDistance(shape, &av[1], param, 0);
        c = ClipEdgeToConvexObstacle(obstacle, av[0], av[1], 2, ahit);
        if (c > 0) {
            if (paramOut) *paramOut = param;
            if (posOut) *posOut = (ahit - 1)[c].apos[0];
            return -1;
        }
    }
    return 3;
}

// Finds a free point on a shape around parameter g that avoids every obstacle (searching both directions). (inferred)
// Also known as: NNavWay (upstream readable / Sly 1 name; NNavWay__FG9VU_VECTORP6VECTORP9NAVREGIONP3CRViPP7NAVPOLYPff).
extern "C" int FindShapeFreePoint(qword pos, void *a1, void *a2, OBSTACLE *shape, int direction, OBSTACLE **blockerOut, float *paramOut, float param)
{
    float t;
    U v, v2;
    float f0, f1;
    OBSTACLE *x;
    int r;
    if (direction) t = WrapShapeDistance(shape, param + D_002C0AC4);
    else t = WrapShapeDistance(shape, param - D_002C0AC4);
    EvaluateShapeAtDistance(shape, &v.q, t, 0);
    x = FindFirstObstacleHitBySegment(a1, a2, pos, v.q);
    if (blockerOut) *blockerOut = x;
    if (!x) return 0;
    r = func_001BD560F(x, shape, t, direction, &v2.q, 0);
    if (r != -1) return r;
    FindShapeClosestPoint(x, pos, D_003DD020, 0, 0, 0, &f0);
    FindShapeClosestPoint(x, v2.q, D_003DD020, 0, 0, 0, &f1);
    if (paramOut) *paramOut = f1;
    else paramOut = &f1;
    return ClipShapeToPathzone(x, a1, paramOut, -1, f0);
}

// Returns 1 when two convex polygons overlap (separating-axis test on their side planes).
// Also known as: FPolysOverlap (upstream readable / Sly 1 name; FPolysOverlap__FP7NAVPOLYT0).
extern "C" int DoConvexObstaclesOverlap(OBSTACLE *polygonA, OBSTACLE *polygonB)
{
    int i;
    MATRIX4Q m = *GetMat(polygonA);
    qword v = MulPt(&m, polygonA->vertexPosq[0].q);
    LSG hit;
    i = 1;
    if (i < polygonA->numControlVertices) {
        do {
            qword w = MulPt(&m, polygonA->vertexPosq[i].q);
            if (ClipEdgeToConvexObstacle(polygonB, v, w, 1, &hit))
                break;
            v = w;
        } while (++i < polygonA->numControlVertices);
    }
    if (i < polygonA->numControlVertices)
        return 1;
    return IsPosInConvexObstacle(polygonA, MulPt(GetMat(polygonB), polygonB->vertexPosq[0].q)) != 0;
}

// Creates a merged obstacle object (a new OBSTACLE) that will own several member polygons. (inferred)
// Also known as: PpathNewMerged (upstream readable / Sly 1 name; PpathNewMerged__Fv).
OBSTACLE *NewMergedCid14Obstacle(void)
{
    OBSTACLE *p;
    int i, j;
    SWALLOC req;
    float *pm;
    p = (OBSTACLE *)((DL *)&g_sw.dlCid14Free)->head;
    if (!p) {
        for (i = 0; i < 16; i++) {
            OBSTACLE *q = CreateLo__F3CIDP2SO3OIDi(0xE, 0, 0x2CD, -1);
            q->curveKind = (CRVK)0;
            q->isClosed = 1;
            q->isMerged = 1;
            pm = q->localMatrix.mat[0];
            func_002333B4(pm, 0, 0x40);
            pm[15] = 1.0f;
            pm[10] = 1.0f;
            pm[5] = 1.0f;
            pm[0] = 1.0f;
            req.cb = 0x80;
            req.cbAlign = 0x10;
            q->vertexParams = (float *)AllocFromWorldHeap(&req);
            for (j = 0; j < 32; j++) q->vertexParams[j] = (float)j;
            req.cb = 0x80;
            req.cbAlign = 0x10;
            q->vertexArcLengths = (float *)AllocFromWorldHeap(&req);
            req.cb = 0x200;
            req.cbAlign = 0x10;
            q->vertexPosq = (QWROW *)AllocFromWorldHeap(&req);
            req.cb = 0x1F0;
            req.cbAlign = 0x10;
            q->edgePlanes = (qword *)AllocFromWorldHeap(&req);
            AsLo(q)->Remove();
            AppendDlEntry__FP2DLPv((DL *)&g_sw.dlCid14Free, q);
        }
        p = (OBSTACLE *)((DL *)&g_sw.dlCid14Free)->head;
    }
    RemoveDlEntry__FP2DLPv((DL *)&g_sw.dlCid14Free, p);
    p->planeStamp = -1;
    AsLo(p)->Add();
    p->owner = 0;
    return p;
}

// Compares two positions by x (sort callback). (Sly 1-derived name: NCmpWkr.)
// Also known as: NCmpWkr (upstream readable / Sly 1 name; NCmpWkr__FP3WKRT0).
int ComparePosX(const void *a, const void *b)
{
    int r = -1;
    if (*(const float *)a > *(const float *)b)
        r = 1;
    return r;
}

// z component of the cross product (b - a) x (c - b): the turn direction of the three points.
static inline float CrossZ(qword a, qword b, qword c)
{
    qword d, e, r; float z;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(b), "j"(a));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(e) : "j"(c), "j"(b));
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(d), "j"(e));
    __asm__("qmfc2.ni %0, %1\n\tpextuw %0, $0, %0" : "=r"(z) : "j"(r));
    return z;
}
// Computes the 2D convex hull of a point set (sort by x, then the lower and upper chains of the monotone-chain
// algorithm) and stores it as the polygon's outline (at most 32 points).
void BuildConvexHull(OBSTACLE *polygon, int count, qword *points)
{
    int i, k, t;   /* k: hull size; t: size of the lower chain */
    qword *hull, *pv;
    qsort(points, count, 0x10, ComparePosX);
    k = 2;
    {
        STACKSCOPE ctx;
        hull = (qword *)Alloc(0x800);
        CopyAb__FPvT0Ui(hull, points, 0x20);
        for (i = 2; i < count; i++) {
            hull[k++] = points[i];
            while (k >= 3) {
                pv = &hull[k - 2];
                if (CrossZ(pv[-1], pv[0], pv[1]) < 0.0f) {
                    pv[0] = pv[1];
                    k--;
                } else
                    goto lowerConvex;   /* not `break` (see docs/MATCHING_NOTES.md) */
            }
            lowerConvex:;
        }
        t = k;
        hull[k++] = points[count - 2];
        for (i = count - 3; i >= 0; i--) {
            hull[k++] = points[i];
            while (t < k) {
                pv = &hull[k - 2];
                if (CrossZ(pv[-1], pv[0], pv[1]) < 0.0f) {
                    pv[0] = pv[1];
                    k--;
                } else
                    goto upperConvex;   /* not `break` (see docs/MATCHING_NOTES.md) */
            }
            upperConvex:;
        }
        { int cMax = 32; polygon->numControlVertices = (k > cMax - 1) ? cMax : k; }
        CopyAb__FPvT0Ui(polygon->vertexPosq, hull, polygon->numControlVertices << 4);
    }
}

// Attaches a polygon to a merged owner and unregisters its individual obstacle.
// Also known as: AddPolyMember (upstream readable / Sly 1 name; AddPolyMember__FP2LOT0).
void AddCid14ToMerged(OBSTACLE *polygon, OBSTACLE *owner)
{
    DL *pdl;
    polygon->owner = owner;
    AppendDlEntry__FP2DLPv(&owner->members, polygon);
    UnregisterCid14Obstacle(polygon);
    pdl = (DL *)&g_sw.dlCid14Pending;
    if (IsEntryInDl__FP2DLPv(pdl, polygon)) RemoveDlEntry__FP2DLPv(pdl, polygon);
}

/* MergeCid14Obstacles: merge a set of convex polygons into one new polygon (hull of all their transformed points), re-centre
   it on the average point, then reparent the old polygons' children to the new one (lane 10).
   Notes on the compile (all general tricks, see build/lane_notes/lane10.md):
   - first loop is a plain `for`; the `if(n>0){do..while}` form makes gcse hoist `lui %hi(D_002A2DD0)` to the function entry.
   - the destination pointer `pw` is explicit: loop.numPoints will not strength-reduce `apos + k` by itself (benefit 4 == add cost 4).
   - MulPtP uses a plain "=j" output (an in/out "+j" or an earlyclobber changes the VF allocation).
   - px lives in $s1 for both polygon loops in the original; without the pin it swaps with the polygon index (s0/s1)
     (single register pin; GetMat is written out by hand because an inline helper's parameter copy fights the pin). */
/* point * 4x4 matrix (xyz only, w from row 3) */
static inline qword MulPtP(MATRIX4Q*m,qword*point){
 qword out;
 __asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w":"=j"(out):"j"(m->r[0].q),"j"(m->r[1].q),"j"(m->r[2].q),"j"(m->r[3].q),"j"(*point));return out;
}
// Merges overlapping convex polygons: collects their points, builds the convex hull and registers it as one new
// polygon owned by a merged object.
// Also known as: MergePolys (upstream readable / Sly 1 name; MergePolys__FiPP7NAVPOLY).
extern "C" void MergeCid14Obstacles(int count, OBSTACLE **polygons)
{
    int c = 0;
    int i, j, k;
    qword *apos;
    OBSTACLE *pNew;
    for (int i0 = 0; i0 < count; i0++) c += polygons[i0]->numControlVertices - 1;
    STACKSCOPE ctx;
    apos = (qword *)Alloc(c << 4);
    register OBSTACLE *px __asm__("$17");   /* the one allowed pin: original keeps px in $s1 (polygon index in $s0) */
    int polygon = 0;
    if (count > 0) {
    k = -16;
    do {
        px = polygons[polygon];
#if defined(SLY2_EE_LAYOUT)
        /* GetWorldMatrix (slot 31) by hand: a real call changes MergeCid14Obstacles (36 off) */
        MATRIX4Q m=*((MATRIX4Q *(*)(OBSTACLE *))SLY2_VTE(px, 31).pfn)((OBSTACLE *)((char *)px + SLY2_VTE(px, 31).d));
#else
        MATRIX4Q m=*(MATRIX4Q *)px->GetWorldMatrix();
#endif
        j = 0;
        if (j < px->numControlVertices - 1) {
            qword *pw = (qword*)((uptr)k + (uptr)apos);
            do { k += 16; *++pw = MulPtP(&m,&px->vertexPosq[j].q); j++; } while (j < px->numControlVertices - 1);
        }
    } while (++polygon < count);
    }
    pNew = NewMergedCid14Obstacle();
    BuildConvexHull(pNew, c, apos);
    {
        qword sum = D_002A2DD0;
        qword k2;
        union { qword q; float f[4]; } u;   /* the scalar's stack home (dead store in the original) */
        for(int sumIndex=pNew->numControlVertices-2;sumIndex>=0;--sumIndex)
            __asm__("vadd.xyzw %0,%0,%1":"+j"(sum):"j"(pNew->vertexPosq[sumIndex].q));
        __asm__("qmtc2.ni %1, %0" : "=j"(k2) : "r"((float)(pNew->numControlVertices - 1)));
        /* sum /= (c-1) via vdiv/vmulq; the extra "+j"(k2) is allocation/ordering only: it keeps the dead store of k2 after the vmulq */
        __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %0, $Q" : "+j"(sum), "+j"(k2) : "j"(k2));
#if defined(SLY2_EE_LAYOUT)
        /* TranslateToPos (slot 29) by hand: the pfn load must precede the u store */
        void (*pfn)(OBSTACLE *, qword) = (void (*)(OBSTACLE *, qword))SLY2_VTE(pNew, 29).pfn;
        pfn((OBSTACLE *)((char *)pNew + SLY2_VTE(pNew, 29).d), (u.q = k2, sum));
#else
        pNew->TranslateToPos((u.q = k2, sum));
#endif
        int pointIndex = pNew->numControlVertices;
        while (--pointIndex >= 0) {
            qword point=pNew->vertexPosq[pointIndex].q;
            __asm__("vsub.xyzw %0,%0,%1":"+j"(point):"j"(sum));
            pNew->vertexPosq[pointIndex].q=point;
        }
    }
    for (i = 0; i < count; i++) {
        px = polygons[i];
        if (px->isMerged) {
            DLI dli;
            OBSTACLE *pv;
            InitDli(&dli, &px->members);
            while ((pv = (OBSTACLE *)PvNextDli(&dli)) != 0) {
                RemoveDlEntry__FP2DLPv(&px->members, pv);
                pv->owner = 0;
                AddCid14ToMerged(pv, pNew);
            }
#if defined(SLY2_EE_LAYOUT)
            ((void (*)(OBSTACLE *))SLY2_VTE(px, 9).pfn)((OBSTACLE *)((char *)px + SLY2_VTE(px, 9).d));   /* Remove (slot 9) by hand, as above */
#else
            px->Remove();
#endif
            AppendDlEntry__FP2DLPv((DL *)&g_sw.dlCid14Free, px);
            g_pdliFirst = dli.m_pdliNext;
        } else {
            AddCid14ToMerged(px, pNew);
        }
    }
    pNew->isClosed = 1;
    MeasureShape(pNew);
    pNew->winding = ClassifyCid14Polygon(pNew);
}

// Drains g_sw.dlCid14Pending, registering each queued polygon as an obstacle (merging where they overlap).
// Also known as: UpdateNavPolys (upstream readable / Sly 1 name; UpdateNavPolys__Fv).
extern "C" void UpdatePendingCid14Obstacles(void)
{
    while (!IsDlEmpty__FP2DL((DL *)&g_sw.dlCid14Pending)) {
        void *pv = ((DL *)&g_sw.dlCid14Pending)->head;
        RemoveDlEntry__FP2DLPv((DL *)&g_sw.dlCid14Pending, pv);
        RegisterCid14Obstacle((OBSTACLE *)pv);
    }
}
