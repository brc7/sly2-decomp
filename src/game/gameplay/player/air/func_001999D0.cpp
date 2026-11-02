// func_001999D0: contact direction of a JT against a shape feature, then FCheckJtEdgeNormal(p, point, *out).
//   kind 1 (vertex): sum of FCheckJtSingleEdge directions over the edges at that vertex (func_001665F0, max 64)
//   kind 2 (face):   face must be flat enough (plane z >= 0.7); sum of the directions of both edge lists whose
//                    segment passes func_00118668 against the actor's tree
//   kind 3 (edge):   FCheckJtSingleEdge directly
// A sum is normalized with D_002A2DA0 as the fallback; no accepted direction returns 0.
// What mattered (w3lane2): the return-style Normalize with the mtc1-asm X() compare from GetPipeGoalMat (the asm
// between the fallback load and the bc1f stops reorg's backward delay-slot search, so the divide's reload fills
// it), a two-input Dot so Len(v) = Sqrt(Dot(v, v)), and block-scoped case locals (case 2 reuses case 1's slots).
typedef int qword __attribute__((mode(TI)));
typedef unsigned short ushort;
union U { qword q; float f[4]; int i[4]; };
struct EDGE;
struct NODE;
struct FACE { U plane; EDGE *first, *second; int flags; };
struct EDGE { ushort vertices[2], face, other; EDGE *next, *nextOther; };
struct GEOMETRY { char pad0[4]; U *points; char pad8[4]; FACE *faces; };
struct QUERY { int pad0, count, pad8; NODE *tree; };
struct SHAPE { char pad0[0x68]; QUERY query; char pad78[0x18]; GEOMETRY geometry; };
struct ACTOR { char pad0[0x258]; SHAPE *shape; };
struct JT { ACTOR *actor; };
struct SPAN { U first, last, firstPlane, lastPlane; float from, to; int flags; FACE *firstFace, *lastFace; };
extern "C" {
extern U D_002A2DD0, D_002A2DA0;   // zero vector, fallback direction
int func_001665F0(GEOMETRY *, QUERY *, int, int, EDGE **);
int FCheckJtSingleEdge__FP2JTP2SOP4EDGEP6VECTOR(JT *, SHAPE *, EDGE *, U *);   // FCheckJtSingleEdge
int func_00118668(NODE *, int, qword, qword, int, SPAN *);
int FCheckJtEdgeNormal(JT *, qword, qword);
}

static inline qword Add(qword a, qword b)
{
    __asm__("vadd.xyzw %0, %0, %1" : "+j"(a) : "j"(b));
    return a;
}
static inline qword Dot(qword a, qword b)
{
    qword d, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    return d;
}
static inline qword Sqrt(qword d)
{
    qword l;
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
    return l;
}
static inline qword Len(qword v) { return Sqrt(Dot(v, v)); }
static inline qword Div(qword v, qword l)
{
    qword r;
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l), "j"(v));
    return r;
}
static inline float X(qword q)
{
    float f;
    __asm__("mtc1 %1,%0" : "=f"(f) : "r"(q));
    return f;
}
static inline qword Normalize(qword v, qword fallback)
{
    qword l = Len(v);
    if (X(l) < 1.0e-4f)
        return fallback;
    return Div(v, l);
}

/** @sly2-annot addr=0x1999D0
 * Purpose: Contact direction vs a shape feature: vertex -> sum FCheckJtSingleEdge over edges at the point
 *   (CpedgeFromGeomPoint, max 64) + BSP clip test; edge -> FCheckJtSingleEdge; then FCheckJtEdgeNormal
 * Info: proposed name FCheckJtEdgeGrab (T1); TU air
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4b_rest+verdicts_B
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
extern "C" int func_001999D0(JT *p, SHAPE *shape, qword point, int kind, void *info, U *out)
{
    switch (kind) {
    case 1: {
        EDGE *edges[64];
        U direction;
        int found = 0;
        qword sum = D_002A2DD0.q;
        GEOMETRY *geometry = &shape->geometry;
        int count = func_001665F0(geometry, &shape->query, (U *)info - geometry->points, 64, edges);
        for (int i = 0; i < count; i++) {
            if (FCheckJtSingleEdge__FP2JTP2SOP4EDGEP6VECTOR(p, shape, edges[i], &direction)) {
                found = 1;
                sum = Add(sum, direction.q);
            }
        }
        if (!found)
            return 0;
        out->q = Normalize(sum, D_002A2DA0.q);
        break;
    }
    case 3:
        if (!FCheckJtSingleEdge__FP2JTP2SOP4EDGEP6VECTOR(p, shape, (EDGE *)info, out))
            return 0;
        break;
    case 2: {
        FACE *face = (FACE *)info;
        if (face->plane.f[2] < 0.7f)
            return 0;
        int found = 0;
        qword sum = D_002A2DD0.q;
        U direction;
        SPAN spans[2];
        for (EDGE *edge = face->first; edge; edge = edge->next) {
            if (FCheckJtSingleEdge__FP2JTP2SOP4EDGEP6VECTOR(p, shape, edge, &direction)) {
                U *points = shape->geometry.points;
                if (func_00118668(p->actor->shape->query.tree, 0x7FFFFFFF, points[edge->vertices[0]].q,
                                  points[edge->vertices[1]].q, 2, spans) > 0) {
                    found = 1;
                    sum = Add(sum, direction.q);
                }
            }
        }
        for (EDGE *edge = face->second; edge; edge = edge->nextOther) {
            if (FCheckJtSingleEdge__FP2JTP2SOP4EDGEP6VECTOR(p, shape, edge, &direction)) {
                U *points = shape->geometry.points;
                if (func_00118668(p->actor->shape->query.tree, 0x7FFFFFFF, points[edge->vertices[0]].q,
                                  points[edge->vertices[1]].q, 2, spans) > 0) {
                    found = 1;
                    sum = Add(sum, direction.q);
                }
            }
        }
        if (!found)
            return 0;
        out->q = Normalize(sum, D_002A2DA0.q);
        break;
    }
    }
    return FCheckJtEdgeNormal(p, point, out->q);
}
