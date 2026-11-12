// shape.cpp -- shapes (SHAPE : XFM): polylines or Hermite curves through points, each point carrying a parameter
// value (u, "time") and a cumulative arc length (s, "distance"). Shapes are the paths that guards patrol, cameras
// and platforms follow, rails and ropes use, and that convex obstacles are built from. Role: load and clone, the
// per-segment sample cache, conversion between u and s and their limits/wrapping (closed shapes), evaluation of
// position and tangent, closest-point searches, bounds; shape chains (a simulated rope that follows the shape with
// ends pinned to objects); CID 106, a mesh deformed along a shape; and the volume / valid-range tables of a shape
// against the path zones. Naming: "Param" is the parameter u and "Distance" the arc length s (Sly 1 names were
// SFromShapeU = GetShapeDistanceAtParam, UFromShapeS, SMaxShape = GetShapeMaxDistance = total length, ...).
// Entry points: LoadShapeFromBrx, EvaluateShapeAtParam / EvaluateShapeAtDistance, GetShapeDistanceAtParam / GetShapeParamAtDistance,
// FindShapeClosestPoint*, SetShapeChainMode, GetRandomShapeDistanceInRange.
// TU 0x1E5D80..0x1EA568 (56 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_shape
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_shape

#include "sly2/basic.h"
#include "sly2/xfm.h"
#include "sly2/slotheap.h"
#include "sly2/scrap.h"
#include "sly2/classes.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/sw.h"
#include "sly2/clock.h"
#include "sly2/lsg.h"
// The real class behind the per-function object views below, for its virtual calls.
static inline ::LO *AsLo(void *plo) { return (::LO *)plo; }

// 0x1EA410 StaticInit_shape and GlobalConstructors_shape: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

struct SPACE;
extern "C" {
void InitXfm(XFM *pxfm);
float GetShapeParamAtDistance(SHAPE *pshape, float s);
void EvaluateShapeAtParam(SHAPE *pshape, float u, qword *ppos, qword *pdir);
void FindShapeClosestPointLinearFromParam(SHAPE *pshape, qword pos, SPACE *pspace, qword *pposOut, qword *pdirOut, float *puOut, float *pgOut, float u);
void FindShapeClosestPointCurveFromParam(SHAPE *pshape, qword pos, SPACE *pspace, qword *pposOut, qword *pdirOut, float *puOut, float *pgOut, float u);
}
void CopyAb(void *pvDst, void *pvSrc, uint cb);

// Slot 7 (Init): InitXfm; no cached segment (-1) and the segment search hint at 1.
void SHAPE::Init()
{
    SHAPE *pshape = (SHAPE *)this;
    InitXfm(pshape);
    pshape->unknown148 = -1;
    pshape->segmentHint = 1;
}

namespace M_CloneShape {
void CloneXfm(SHAPE *p, LO *ploBase) __asm__("CloneXfm");
void EnsureShapeChain(SHAPE *p, int n) __asm__("EnsureShapeChain");
// Slot 41 (Clone): CloneXfm; if the source had a chain attached, rebuilds one for the clone.
extern "C" void CloneShape(SHAPE *shape, LO *ploBase)
{
    CloneXfm(shape, ploBase);
    if (shape->chain) {
        shape->chain = 0;
        EnsureShapeChain(shape, 0);
    }
}
} // namespace M_CloneShape

namespace M_LoadShapeFromBrx {
void LoadXfmXfFromBrx(SHAPE *, void *) __asm__("LoadXfmXfFromBrx");
int S8Read__18CBinaryInputStream(void *) __asm__("S8Read__18CBinaryInputStream");
int U16Read__18CBinaryInputStream(void *) __asm__("U16Read__18CBinaryInputStream");
void *AllocFromWorldHeap(struct SWALLOC *) __asm__("AllocFromWorldHeap");
void Read__18CBinaryInputStreamiPv(void *, int, void *) __asm__("Read__18CBinaryInputStreamiPv");
void ReadVector__18CBinaryInputStreamP6VECTOR(void *, VECTOR4 *) __asm__("ReadVector__18CBinaryInputStreamP6VECTOR");
void LoadOptionsFromBrx__FPvP18CBinaryInputStream(SHAPE *, void *) __asm__("LoadOptionsFromBrx__FPvP18CBinaryInputStream");
void MeasureShape(SHAPE *) __asm__("MeasureShape");
// Slot 14 (LoadFromBrx): the XFM, the mode (0 linear, 1 curve), closed flag, point count, then the per-point
// parameters, distances and positions (and curve tangents).
// Layout parse-verified: a parser following it reads every byte of 4 levels. Also known as: LoadPathFromBrx (upstream readable / Sly 1 name; LoadPathFromBrx__FP2LOP18CBinaryInputStream).
extern "C" void LoadShapeFromBrx(SHAPE *shape, void *stream)
{
    struct SWALLOC msg;
    int i;
    LoadXfmXfFromBrx(shape, stream);
    shape->curveKind = (CRVK)S8Read__18CBinaryInputStream(stream);
    shape->isClosed = S8Read__18CBinaryInputStream(stream);
    shape->numControlVertices = U16Read__18CBinaryInputStream(stream);
    msg.cb = shape->numControlVertices * 4;
    msg.cbAlign = 0x10;
    shape->vertexParams = (__typeof__(shape->vertexParams))AllocFromWorldHeap(&msg);
    msg.cb = shape->numControlVertices * 4;
    msg.cbAlign = 0x10;
    shape->vertexArcLengths = (__typeof__(shape->vertexArcLengths))AllocFromWorldHeap(&msg);
    msg.cb = shape->numControlVertices * 16;
    msg.cbAlign = 0x10;
    shape->vertexPositions = (__typeof__(shape->vertexPositions))AllocFromWorldHeap(&msg);
    Read__18CBinaryInputStreamiPv(stream, shape->numControlVertices * 4, shape->vertexParams);
    for (i = 0; i < shape->numControlVertices; i++)
        ReadVector__18CBinaryInputStreamP6VECTOR(stream, &shape->vertexPositions[i]);
    if (shape->curveKind) {
        msg.cb = shape->numControlVertices * 16;
        msg.cbAlign = 0x10;
        shape->incomingTangents = (__typeof__(shape->incomingTangents))AllocFromWorldHeap(&msg);
        msg.cb = shape->numControlVertices * 16;
        msg.cbAlign = 0x10;
        shape->outgoingTangents = (__typeof__(shape->outgoingTangents))AllocFromWorldHeap(&msg);
        for (i = 0; i < shape->numControlVertices; i++)
            ReadVector__18CBinaryInputStreamP6VECTOR(stream, &shape->incomingTangents[i]);
        for (i = 0; i < shape->numControlVertices; i++)
            ReadVector__18CBinaryInputStreamP6VECTOR(stream, &shape->outgoingTangents[i]);
    }
    LoadOptionsFromBrx__FPvP18CBinaryInputStream(shape, stream);
    MeasureShape(shape);
}
} // namespace M_LoadShapeFromBrx

namespace M_CacheShapeSegment {
// Cache eleven Hermite samples of one path segment (control points transformed by the path matrix)
// and the cumulative lengths between them.
union U { qword q; float f; };
void EvaluateBezierPos(float duration, float t, float scale, qword start, qword startTangent, qword end, qword endTangent,
                   qword *pos, qword *vel, qword *acc) __asm__("EvaluateBezierPos");

// Rotate v by the 3x3 matrix with rows m0..m2.
extern "C" { static inline qword Dir(qword m0, qword m1, qword m2, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
            : "=j"(r) : "j"(m0), "j"(m1), "j"(m2), "j"(v));
    return r;
} }
// Transform point v by the 4x4 matrix with rows m0..m3.
extern "C" { static inline qword Point(qword m0, qword m1, qword m2, qword m3, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w"
            : "=j"(r) : "j"(m0), "j"(m1), "j"(m2), "j"(m3), "j"(v));
    return r;
} }
// Length of the xyz part of a as a float.
extern "C" { static inline float Len(qword a)
{
    qword one;
    float f;
    __asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "+j"(a), "=&j"(one));
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "+j"(a));
    __asm__("qmfc2.ni %0,%1" : "=r"(f) : "j"(a));
    return f;
} }

// Caches 11 Hermite samples of curve segment i (in world space) and their cumulative lengths, for arc-length
// lookups inside a curve segment. Also known as: CacheSegCrv (upstream readable / Sly 1 name; CacheSegCrv__FP3CRVi).
extern "C" void CacheShapeSegment(SHAPE *shape, int segment)
{
    union U prev, cur;
    // Plain qwords: they live across the call in the loop, so reload spills them to 0x20..0x50 and
    // loads them straight into the argument registers.
    qword start, end, a, b;
    int j;
    float duration;
    if (segment == shape->unknown148)
        return;
    {
        MATRIX4Q *m = &shape->matqWorldCache;
        qword v0 = shape->vertexPosq[segment].q, v1 = shape->vertexPosq[segment + 1].q, o0 = shape->outgoingTangentq[segment].q, i1 = shape->incomingTangentq[segment + 1].q;
        qword m1 = m->r[1].q, m2 = m->r[2].q, m3 = m->r[3].q, m0 = shape->matqWorldCache.r[0].q;
        shape->segmentLengths[0] = 0;
        // Computed in argument order; the scheduler then emits them as b, end, start, a.
        start = Point(m0, m1, m2, m3, v0);
        a = Dir(m0, m1, m2, o0);
        end = Point(m0, m1, m2, m3, v1);
        b = Dir(m0, m1, m2, i1);
    }
    duration = shape->vertexParams[segment + 1] - shape->vertexParams[segment];
    for (j = 0; j < 11; j++) {
        float frac = (float)j * 0.1f;
        EvaluateBezierPos(duration, frac * duration, 1.0f, start, a, end, b, &cur.q, 0, 0);
        if (j > 0)
            shape->segmentLengths[j] = shape->segmentLengths[j - 1] + Len(VuSub(prev.q, cur.q));
        prev.q = cur.q;
    }
    shape->unknown148 = segment;
}
} // namespace M_CacheShapeSegment

namespace M_MeasureShape {
void InvalidateShapeCache(SHAPE*) __asm__("InvalidateShapeCache");
void CacheShapeSegment(SHAPE*,int) __asm__("CacheShapeSegment");
void SyncChainDistances(void*) __asm__("SyncChainDistances");
// Transform point a by the 4x4 matrix m.
extern "C" { static inline qword Point(MATRIX4Q *m,qword a) {
 qword r;
 __asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w":"=j"(r):"j"(m->r[0].q),"j"(m->r[1].q),"j"(m->r[2].q),"j"(m->r[3].q),"j"(a));
 return r;
} }
// Copy a VU0 register (vmove).
extern "C" { static inline qword Move(qword a) { qword r; __asm__("vmove.xyzw %0,%1":"=j"(r):"j"(a));return r; } }
// Length of the xyz part of a, in the x lane.
extern "C" { static inline qword LenVec(qword a) {
 qword d,one;
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=j"(one):"j"(a));
 __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(d));
 return d;
} }
// Recomputes the cumulative-distance array from the world-space points (linear) or the cached per-segment lengths
// (curve). Also known as: RecalcCrv (upstream readable / Sly 1 name; RecalcCrv__FP3CRV).
extern "C" void MeasureShape(SHAPE *shape)
{
 int i;
 InvalidateShapeCache(shape);
 shape->matqWorldCache=*(MATRIX4Q*)AsLo(shape)->GetWorldMatrix();
 if (shape->curveKind==0) {
  float *d0=shape->vertexArcLengths; float total=0.0f;
  qword prev;
  prev=Point(&shape->matqWorldCache,shape->vertexPosq[0].q);
  d0[0]=total;
  for(i=1;i<shape->numControlVertices;i++) {
   qword cur=Point(&shape->matqWorldCache,shape->vertexPosq[i].q);
   qword lenv=LenVec(VuSub(prev,cur));
   float len;
   prev=Move(cur);
   __asm__("mtc1 %1,%0":"=f"(len):"r"(lenv));
   total+=len;
   shape->vertexArcLengths[i]=total;
  }
 } else {
  int j;
  shape->vertexArcLengths[0]=0;
  for(j=0;j<shape->numControlVertices-1;j++) {
   CacheShapeSegment(shape,j);
   shape->vertexArcLengths[j+1]=shape->vertexArcLengths[j]+shape->segmentLengths[10];
  }
 }
 if(shape->chain)SyncChainDistances(shape->chain);
}
} // namespace M_MeasureShape

// Invalidates the segment cache (search hint 0, cached segment -1). Also known as: RefreshCrv (upstream readable / Sly 1 name; RefreshCrv__FP3CRV).
extern "C" void InvalidateShapeCache(SHAPE *shape)
{
    shape->segmentHint = 0;
    shape->unknown148 = -1;
}

namespace M_func_001E6340 {
void StepChainRk4(SHAPECHAIN *q) __asm__("StepChainRk4");
void StepChain(SHAPECHAIN *q) __asm__("StepChain");
void InheritLoFlag4(SHAPE *p) __asm__("InheritLoFlag4");
// Slot 17: when a chain is attached, steps its simulation (StepChainRk4 for modes 0/3, StepChain for 1/2);
// otherwise the base LO transform update. Also known as: UpdatePath17 (upstream readable / Sly 1 name; UpdatePath17__FP2LO).
extern "C" void UpdateShapeXfWorld(SHAPE *shape)
{
    if (shape->chain) {
        switch (shape->chain->mode) {
        case 0:
        case 3:
            StepChainRk4(shape->chain);
            break;
        case 1:
        case 2:
            StepChain(shape->chain);
            break;
        }
    } else {
        InheritLoFlag4(shape);
    }
}
} // namespace M_func_001E6340

namespace M_SFromShapeU {
int FindShapeSegmentAtParam(SHAPE *p, float *pa, float *pb) __asm__("FindShapeSegmentAtParam");
void CacheShapeSegment(SHAPE *p, int i) __asm__("CacheShapeSegment");
// Linear interpolation (1-t)*a + t*b.
extern "C" { static __inline__ float GLerp(float a, float b, float t) { return (1.0f - t) * a + t * b; } }
// Arc length at parameter u, interpolated from the distance table (sub-sampled inside curve segments). Also known as: SFromCrvG (upstream readable / Sly 1 name; SFromCrvG__FP3CRV).
extern "C" float GetShapeDistanceAtParam(SHAPE *shape)
{
    float a, b;
    float g;
    int i = FindShapeSegmentAtParam(shape, &a, &b);
    if (shape->curveKind == 0) { g = GLerp(shape->vertexArcLengths[i], shape->vertexArcLengths[i + 1], a / b); } else { float u; int j; CacheShapeSegment(shape, i); u = a * 10.0f / b; j = (int)u; g = GLerp(shape->segmentLengths[j], shape->segmentLengths[j + 1], u - j); g += shape->vertexArcLengths[i]; }
    return g;
}
} // namespace M_SFromShapeU

namespace M_UFromShapeS {

int FindShapeSegmentAtDistance(SHAPE *p, float *pa, float *pb, float time) __asm__("FindShapeSegmentAtDistance");
void CacheShapeSegment(SHAPE *p, int i) __asm__("CacheShapeSegment");

// Linear interpolation (1-t)*a + t*b.
extern "C" { static __inline__ float GLerp(float a, float b, float t)
{
    return (1.0f - t) * a + t * b;
} }

// Return value through $f4 (register-pinned move for matching).
extern "C" { static inline float ReturnValue(float value)
{
    register float input __asm__("$f4") = value;
    float result;
    __asm__("mov.s %0,%1" : "=f"(result) : "f"(input));
    return result;
} }

// Parameter u at arc length s (the inverse of GetShapeDistanceAtParam). Also known as: GFromCrvS (upstream readable / Sly 1 name; GFromCrvS__FP3CRVf).
extern "C" float GetShapeParamAtDistance(SHAPE *shape, float distance)
{
    float a, b;
    float g;
    int i = FindShapeSegmentAtDistance(shape, &a, &b, distance);

    if (shape->curveKind == 0) {
        {
            float fraction = a / b;
            float upper = *(volatile float *)&shape->vertexParams[i + 1];
            float lower = *(volatile float *)&shape->vertexParams[i];
            g = GLerp(lower, upper, fraction);
        }
    } else {
        int j;
        CacheShapeSegment(shape, i);
        for (j = 1; j < 10; j++)
            if (a < shape->segmentLengths[j]) break;
        j--;
        {
            float part = (a - shape->segmentLengths[j]) / (shape->segmentLengths[j + 1] - shape->segmentLengths[j]);
            float index = j;
            part = (index + part) * 0.1f;
            g = GLerp(shape->vertexParams[i], shape->vertexParams[i + 1], part);
        }
    }
    return ReturnValue(g);
}
} // namespace M_UFromShapeS

// First point's parameter. Also known as: GMinCrv (upstream readable / Sly 1 name; GMinCrv__FP3CRV).
extern "C" float GetShapeMinParam(SHAPE *shape)
{
    return shape->vertexParams[0];
}

// First point's distance. Also known as: SMinCrv (upstream readable / Sly 1 name; SMinCrv__FP3CRV).
extern "C" float GetShapeMinDistance(SHAPE *shape)
{
    return shape->vertexArcLengths[0];
}

// Last point's parameter. Also known as: GMaxCrv (upstream readable / Sly 1 name; GMaxCrv__FP3CRV).
extern "C" float GetShapeMaxParam(SHAPE *shape)
{
    return shape->vertexParams[shape->numControlVertices - 1];
}

// Last point's distance: the shape's total length. Also known as: SMaxCrv (upstream readable / Sly 1 name; SMaxCrv__FP3CRV).
extern "C" float GetShapeMaxDistance(SHAPE *shape)
{
    return shape->vertexArcLengths[shape->numControlVertices - 1];
}

namespace M_UWrapShape {
float ModPositive__Fff(float g, float gMax) __asm__("ModPositive__Fff");
// On a closed shape wraps u into the parameter range; open shapes return u unchanged. Also known as: GWrapCrv (upstream readable / Sly 1 name; GWrapCrv__FP3CRVf).
extern "C" float WrapShapeParam(SHAPE *shape, float param)
{
    float gMin, gMax;
    if (!shape->isClosed) return param;
    gMin = shape->vertexParams[0];
    gMax = shape->vertexParams[shape->numControlVertices - 1];
    if (gMin <= param && param <= gMax) return param;
    return gMin + ModPositive__Fff(param - gMin, gMax - gMin);
}
} // namespace M_UWrapShape

namespace M_SWrapShape {
float ModPositive__Fff(float g, float gMax) __asm__("ModPositive__Fff");
// On a closed shape wraps s into [0, length]; open shapes return s unchanged. Also known as: SWrapCrv (upstream readable / Sly 1 name; SWrapCrv__FP3CRVf).
extern "C" float WrapShapeDistance(SHAPE *shape, float distance)
{
    float gMax;
    if (!shape->isClosed) return distance;
    gMax = shape->vertexArcLengths[shape->numControlVertices - 1];
    if (0.0f <= distance && distance <= gMax) return distance;
    return ModPositive__Fff(distance, gMax);
}
} // namespace M_SWrapShape

namespace M_SClampShape {
float WrapShapeDistance(SHAPE *p, float g) __asm__("WrapShapeDistance");
// 0x1E6720: dead-code scrap (sly2/scrap.h)
extern "C" { SLY2_SCRAP("addiu $sp, $sp, 0x30"); }

// Closed shapes wrap s, open shapes clamp it to [GetShapeMinDistance, GetShapeMaxDistance]. Also known as: SClampCrv (upstream readable / Sly 1 name; SClampCrv__FP3CRVf).
extern "C" float ClampShapeDistance(SHAPE *shape, float distance)
{
    if (shape->isClosed) return WrapShapeDistance(shape, distance);
    return Clamp(distance, GetShapeMinDistance(shape), GetShapeMaxDistance(shape));
}
} // namespace M_SClampShape

namespace M_EvaluateShapeU {
struct M { qword a,b,c,d; };
union U { qword q; float f; };
int FindShapeSegmentAtParam(SHAPE*,float,float*,float*) __asm__("FindShapeSegmentAtParam");
void EvaluateBezierPos(float,float,float,qword,qword,qword,qword,qword*,qword*,qword*) __asm__("EvaluateBezierPos");
// Load the tangent vector at p into vf2.
extern "C" { static inline qword ReadTangent(const qword*p){register qword a __asm__("vf2");__asm__("lqc2 %0,%1":"=j"(a):"m"(*p));return a;} }
// Load the second linear point at p into vf2.
extern "C" { static inline qword ReadLinearSecond(const qword*p){register qword value __asm__("vf2");__asm__("lqc2 %0,%1":"=j"(value):"m"(*p));return value;} }
// Return next - first (vf2/vf1).
extern "C" { static inline qword Sub(qword next,qword first){register qword a __asm__("vf2")=next,b __asm__("vf1")=first;__asm__("vsub.xyzw %0,%0,%1":"+j"(a):"j"(b));return a;} }
// Return a*w0 + b*w1.
extern "C" { static inline qword Lerp(qword a,qword b,qword w0,qword w1) { __asm__("vmulax.xyzw $ACC,%1,%0x\n\tvmaddx.xyzw %0,%2,%3x":"+j"(w0):"j"(a),"j"(b),"j"(w1));return w0; } }
// Evaluate a point on the Hermite segment: transform value by the basis matrix m (register-pinned).
extern "C" { static inline qword HermitePoint(struct M*m,qword value){register qword keep __asm__("vf2")=m->a,row1 __asm__("vf4")=m->b,row2 __asm__("vf3")=m->c,translation __asm__("vf1")=m->d,point __asm__("vf5")=value;__asm__("vmulax.xyz $ACC,%0,%4x\n\tvmadday.xyz $ACC,%1,%4y\n\tvmaddaz.xyz $ACC,%2,%4z\n\tvmaddw.xyz %0,%3,$vf0w":"+j"(keep):"j"(row1),"j"(row2),"j"(translation),"j"(point));return keep;} }
// Transform point a by the 4x4 matrix m, keeping keep live in its register.
extern "C" { static inline qword PointKept(qword keep,struct M *m,qword a) {__asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w":"+j"(keep):"j"(m->a),"j"(m->b),"j"(m->c),"j"(m->d),"j"(a));return keep;} }
// Broadcast the high blend weight f into vf4.
extern "C" { static inline qword HighWeight(float f){register qword value __asm__("vf4");__asm__("qmtc2.ni %1,%0":"=j"(value):"r"(f));return value;} }
// Broadcast the low blend weight f into vf1 and spill it to home.
extern "C" { static inline qword LowWeight(float f,union U*home){register qword value __asm__("vf1");__asm__("qmtc2.ni %2,%0\n\tsqc2 %0,%1":"=j"(value),"=m"(home->q):"r"(f));return value;} }
// Load the first point into vf3 and spill weight to home.
extern "C" { static inline qword FirstPointAndWeight(const qword*first,qword weight,union U*home){register qword point __asm__("vf3");__asm__("lqc2 %0,%2\n\tsqc2 %3,%1":"=&j"(point),"=m"(home->q):"m"(*first),"j"(weight));return point;} }
// Byte offset of point i (i * 16).
extern "C" { static inline unsigned PointOffset(int i){register unsigned bytes __asm__("$3");__asm__("sll %0,%1,4":"=r"(bytes):"r"(i));return bytes;} }
// Load the first tangent point at p into vf1.
extern "C" { static inline qword TangentFirst(const qword*p){register qword point __asm__("vf1");__asm__("lqc2 %0,%1":"=j"(point):"m"(*p));return point;} }
// Copy the segment index into callee-saved $18.
extern "C" { static inline int SegmentIndex(int value){register int index __asm__("$18");__asm__("daddu %0,%1,$zero":"=r"(index):"r"(value));return index;} }
// Position and tangent at parameter u (linear interpolation or Hermite evaluation), in world space. Also known as: EvalCrv (upstream readable / Sly 1 name; EvalCrv__FP3CRVfP6VECTORT2).
void EvaluateShapeAtParam(SHAPE *shape,float param,qword *posOut,qword *dirOut) __asm__("EvaluateShapeAtParam"); void EvaluateShapeAtParam(SHAPE *shape,float param,qword *posOut,qword *dirOut)
{
    struct M m;
    float elapsed,duration;
    register qword row0 __asm__("vf5"),row1 __asm__("vf4"),row2 __asm__("vf3");
    register qword tangent __asm__("vf2");
    int i=SegmentIndex(FindShapeSegmentAtParam(shape,param,&elapsed,&duration));
    m=* (struct M *)AsLo(shape)->GetWorldMatrix();
    if (shape->curveKind==0) {
        if (posOut) {
            union U scalar;float t=elapsed/duration;
            qword *linearPoint=(qword*)(PointOffset(i)+(uptr)shape->vertexPosq);
            qword w1=HighWeight(t),w0=LowWeight(1.0f-t,&scalar);
            {qword first=FirstPointAndWeight(linearPoint,w1,&scalar),second=ReadLinearSecond(linearPoint+1);qword blend=Lerp(first,second,w0,w1);*posOut=PointKept(blend,&m,blend);}
        }
        if (!dirOut) return;
        {register unsigned offset __asm__("$2")=(unsigned)i<<4;
        register qword *base __asm__("$3")=(qword *)shape->vertexPosq;
        register qword *first __asm__("$4"),*second __asm__("$2");
        __asm__("lqc2 %0,%2\n\taddu %1,%3,%4":"=j"(row0),"=r"(first):"m"(m.a),"r"(offset),"r"(base));
        __asm__("lqc2 %0,%2\n\tdaddu %1,%3,$zero":"=j"(row1),"=r"(second):"m"(m.b),"r"(first));
        tangent=Sub(ReadTangent(second+1),TangentFirst(first));}
    } else {
        EvaluateBezierPos(duration,elapsed,1.0f,shape->vertexPosq[i].q,shape->outgoingTangentq[i].q,shape->vertexPosq[i+1].q,shape->incomingTangentq[i+1].q,posOut,dirOut,0);
        if (posOut) *posOut=HermitePoint(&m,*posOut);
        if (!dirOut) return;
        row0=m.a;
        tangent=ReadTangent(dirOut);row1=m.b;
    }
    row2=m.c;
    {register qword d __asm__("vf1"),one __asm__("vf6");
    __asm__("vmulax.xyz $ACC,%4,%2x\n\tvmadday.xyz $ACC,%5,%2y\n\tvmaddz.xyz %2,%6,%2z\n\tvmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z\n\tvsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q\n\tvdiv $Q,$vf0w,%0x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q\n\tsqc2 %0,%3":"=&j"(d),"=&j"(one),"+j"(tangent),"=m"(*dirOut):"j"(row0),"j"(row1),"j"(row2));}
}

#define FIELD_OFFSET(t,m) ((unsigned)&(((t*)0x10000)->m)-0x10000)
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char qword_Alignment[__alignof__(qword)==16?1:-1];
typedef char int_Width[sizeof(int)==4?1:-1];
typedef char float_Width[sizeof(float)==4?1:-1];
typedef char pointer_Width[sizeof(void*)==4?1:-1];
typedef char M_Size[sizeof(struct M)==0x40?1:-1];
typedef char M_Alignment[__alignof__(struct M)==16?1:-1];
typedef char U_Size[sizeof(union U)==16?1:-1];
typedef char U_Alignment[__alignof__(union U)==16?1:-1];
typedef char M_a_Offset[FIELD_OFFSET(struct M,a)==0?1:-1];
typedef char M_b_Offset[FIELD_OFFSET(struct M,b)==0x10?1:-1];
typedef char M_c_Offset[FIELD_OFFSET(struct M,c)==0x20?1:-1];
typedef char M_d_Offset[FIELD_OFFSET(struct M,d)==0x30?1:-1];
typedef char U_q_Offset[FIELD_OFFSET(union U,q)==0?1:-1];
typedef char U_f_Offset[FIELD_OFFSET(union U,f)==0?1:-1];
} // namespace M_EvaluateShapeU
#undef FIELD_OFFSET

namespace M_func_001E69A0 {
struct X; struct SPACE;
struct S { struct X *a0; struct SPACE *a4; int pad[2]; qword v; };
void EvaluateShapeAtParam(struct X *, float, qword *, qword *) __asm__("EvaluateShapeAtParam");
qword ApplyConstrNoTransform(struct SPACE *, qword) __asm__("ApplyConstrNoTransform");
// Squared length of the xyz part of a (x lane, vf1).
extern "C" { static inline qword SquareBits(qword a) {
    register qword d __asm__("vf1"),one __asm__("vf3");
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(a));
    return d;
} }
// Move a GPR qword into vf4 and return its squared length (x lane).
extern "C" { static inline qword ConvertedSquareBits(qword a) {
    register qword value __asm__("vf4");
    register qword square __asm__("vf1"),one __asm__("vf2");
    __asm__("qmtc2.ni %1,%0":"=j"(value):"r"(a));
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(square),"=&j"(one):"j"(value));
    return square;
} }
// Minimizer callback: squared distance from the query point (in its space) to the shape's point at u.
extern "C" float GetShapeDistanceSquaredAtParam(void *context,float param){
    struct S *p=(struct S *)context;
    qword v;register qword bits __asm__("$2");float result;
    EvaluateShapeAtParam(p->a0,param,&v,0);
    if(!p->a4){
        register qword point __asm__("vf1")=p->v;
        register qword path __asm__("vf2")=v;
        __asm__("vsub.xyzw %0,%0,%1":"+j"(point):"j"(path));
        bits=SquareBits(point);
    }
    else {
        register qword point __asm__("vf2")=p->v;
        register qword path __asm__("vf1")=v;
        register qword delta __asm__("vf3");
        __asm__("vsub.xyzw %0,%1,%2":"=j"(delta):"j"(path),"j"(point));
        bits=ConvertedSquareBits(ApplyConstrNoTransform(p->a4,delta));
    }
    __asm__ __volatile__("mtc1 %1,%0":"=f"(result):"r"(bits));
    return result;
}
} // namespace M_func_001E69A0

namespace M_func_001E6A30 {
// FindShapeClosestPointLinearFromParam (w2lane4, wave 2): closest point on a path (chain of control points under the path's own matrix) to a query point,
// searched from a starting time. FindShapeSegmentAtParam turns the starting time into a segment index; the loop projects the point on that
// segment (segment vector rotated by the matrix, point minus the transformed start) and walks to the next / previous segment (wrapping
// when the path is cyclic) until the projection falls inside a segment. Afterwards the position, direction, time and value outputs
// are produced from the final segment and fraction.
// Matching notes (found with the -da dumps; the dumps explain every non-obvious choice):
//  * `fraction` is reused for the final time (`fraction = (1-fraction)*t0 + fraction*t1`), which is what puts it in $f12 (the argument
//    register of GetShapeDistanceAtParam) without any pin.
//  * `basis=&matrix` is declared inside the loop (and again in the direction block): loop.c hoists it to the loop preheader, which
//    is where the target has it (after the matrix copy, in the delay slot of the jump into the loop) and what makes it outrank `space`
//    in global allocation (space $s4, basis $s3). Rows 1 and 2 are read through `basis`, row 0 and the translation row directly.
//  * `previous = index; continue;` is written in both branches (each gets its own delay-slot copy), and `backward = 1` / `forward = 1`
//    are set after the if/else of their branch.
//  * `segment` is a memory union (it is stored and reloaded around the ApplyConstrNoTransform calls); `NormalizeTo` takes it by value, which
//    is the unread temp copy stored to the temp slot (sp+0x50, shared with the VU_FLOAT temps of the position block).
//  * Dot helpers list their input once and `LengthOf` lists the operand once ("j"(v.q) with %2 used twice): two operand
//    references would make the loaded segment outrank the length in local allocation and swap vf1/vf2.
//  * The size and the projection are converted with the single-asm `mtc1 %1,%0 : "r"(vu)` form (ToFloatR): the qmfc2 is then a
//    reload move scheduled before the in-place normalize, and the distance bits land in $a0 instead of $v0.
//  * Normalize output is tied to the length operand ("+j"), DotInto ties the result to the second operand (the point delta).
// Qword wrapper convertible to and from qword.
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
struct M4 { QWROW rows[4]; };
struct SPACE;

int FindShapeSegmentAtParam(SHAPE *, float, float *, float *) __asm__("FindShapeSegmentAtParam");
qword ApplyConstrNoTransform(SPACE *, qword) __asm__("ApplyConstrNoTransform");
float GetShapeDistanceAtParam(SHAPE *, float) __asm__("GetShapeDistanceAtParam");

struct VU_FLOAT {
    qword q;
    // Broadcast a float into a VU0 register.
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1,%0" : "=j"(q) : "r"(f)); }
};
// Transform point v by the 4x4 matrix m.
static inline VV TransformPoint(const M4 &m, VV v)
{
    qword r;
    __asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w"
            : "=j"(r) : "j"(m.rows[0].q), "j"(m.rows[1].q), "j"(m.rows[2].q), "j"(m.rows[3].q), "j"(v));
    return r;
}
// Rotate v by the rows of m (row 0) and b (rows 1 and 2).
static inline VV Rotate(const M4 &m, const M4 *b, VV v)
{
    qword r = v;
    __asm__("vmulax.xyz $ACC,%1,%0x\n\tvmadday.xyz $ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z"
            : "+j"(r) : "j"(m.rows[0].q), "j"(b->rows[1].q), "j"(b->rows[2].q));
    return r;
}
// Same idiom as sly2/vu0.h VuSub; kept local because its callers pass mixed operand types (the template wants one).
static inline VV VuSub(VV a, VV b)
{
    qword r;
    __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
// Return v1 * f1 + v2 * f2.
static inline VV VecCombo(VU_FLOAT f1, VV v1, VU_FLOAT f2, VV v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1), "j"(f2.q), "j"(v2));
    return r;
}
// Dot product of the xyz parts of a and b (x lane), computed in b's register.
static inline VV DotInto(VV a, VV b)
{
    qword one;
    __asm__("vmul.xyz %0,%2,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "+j"(b), "=j"(one) : "j"(a));
    return b;
}
// Square root of the x lane of a.
static inline VV Root(VV a)
{
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "+j"(a));
    return a;
}
// Length of the xyz part of v (x lane).
static inline VV LengthOf(const QWROW &v)
{
    qword d, one;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "=j"(d), "=j"(one) : "j"(v.q));
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "+j"(d));
    return d;
}
// Divide a by the x lane of length.
static inline VV NormalizeTo(QWROW a, VV length)
{
    qword r = length;
    __asm__("vdiv $Q,$vf0w,%0x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q" : "+j"(r) : "j"(a.q));
    return r;
}
// Unit vector along a.
static inline VV Normalize(VV a)
{
    qword length = Root(VuDot(a, a));
    __asm__("vdiv $Q,$vf0w,%0x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q" : "+j"(length) : "j"(a));
    return length;
}
// Closest point on a linear shape to a position, walking segments from a starting parameter.
void FindShapeClosestPointLinearFromParam(SHAPE *p, qword point, SPACE *space, qword *outPosition, qword *outDirection, float *outTime, float *outValue, float initial) __asm__("FindShapeClosestPointLinearFromParam"); void FindShapeClosestPointLinearFromParam(SHAPE *p, qword point, SPACE *space, qword *outPosition, qword *outDirection,
                              float *outTime, float *outValue, float initial)
{
    QWROW segment;
    M4 matrix;
    float elapsed, duration;
    int backward = 0, forward = 0, previous;
    int index = FindShapeSegmentAtParam(p, initial, &elapsed, &duration);
    matrix = *(M4 *)AsLo(p)->GetWorldMatrix();
    float fraction;
    for (;;) {
        const M4 *basis = &matrix;
        qword start = ((qword *)p->vertexPosq)[index], end = ((qword *)p->vertexPosq)[index + 1];
        segment.q = Rotate(matrix, basis, VuSub(end, start));
        qword delta = VuSub(point, TransformPoint(matrix, start));
        if (space) {
            delta = ApplyConstrNoTransform(space, delta);
            segment.q = ApplyConstrNoTransform(space, segment.q);
        }
        qword length = LengthOf(segment);
        float size = QwordX(length);
        segment.q = NormalizeTo(segment, length);
        float distance = QwordX(DotInto(segment.q, delta));
        fraction = 0.0f;
        if (distance < 0.0f) {
            if (forward) { fraction = 1.0f; index = previous; break; }
            if (index > 0) --index;
            else {
                if (!p->isClosed) { fraction = 0.0f; break; }
                index = p->numControlVertices - 2;
            }
            backward = 1;
            previous = index;
            continue;
        } else if (size < distance) {
            if (backward) { fraction = 0.0f; index = previous; break; }
            if (index < p->numControlVertices - 2) ++index;
            else {
                if (!p->isClosed) { fraction = 1.0f; break; }
                index = 0;
            }
            forward = 1;
            previous = index;
            continue;
        } else {
            fraction = distance / size;
            break;
        }
    }
    if (outPosition) {
        *outPosition = TransformPoint(matrix, VecCombo(VU_FLOAT(1.0f - fraction), ((qword *)p->vertexPosq)[index],
                                                       VU_FLOAT(fraction), ((qword *)p->vertexPosq)[index + 1]));
    }
    if (outDirection) {
        const M4 *basis = &matrix;
        qword tangent = Rotate(matrix, basis, VuSub(((qword *)p->vertexPosq)[index + 1], ((qword *)p->vertexPosq)[index]));
        *outDirection = Normalize(tangent);
    }
    fraction = (1.0f - fraction) * p->vertexParams[index] + fraction * p->vertexParams[index + 1];
    if (outTime) *outTime = fraction;
    if (outValue) *outValue = GetShapeDistanceAtParam(p, fraction);
}
} // namespace M_func_001E6A30

namespace M_func_001E6DB8 {
typedef int qword __attribute__((mode(TI)));
struct SPACE;
struct CTX { SHAPE *p; struct SPACE *space; int pad[2]; qword q; };
extern float D_002A8130 __asm__("D_002A8130");
extern float D_002A8134 __asm__("D_002A8134");
float GetShapeDistanceSquaredAtParam(void *pv, float g) __asm__("GetShapeDistanceSquaredAtParam");
void MinimizeRange__FPFPvf_fPvffffPfT6(float (*pfn)(void *, float), void *pv, float g, float dg, float gMin, float gMax, float *pg, float *pgOut) __asm__("MinimizeRange__FPFPvf_fPvffffPfT6");
float WrapShapeParam(SHAPE *p, float g) __asm__("WrapShapeParam");
void EvaluateShapeAtParam(SHAPE *p, float g, qword *a, qword *b) __asm__("EvaluateShapeAtParam");
float GetShapeDistanceAtParam(SHAPE *p, float g) __asm__("GetShapeDistanceAtParam");
// Closest point on a curved shape by minimizing the distance around a starting parameter (step = segment / 10).
void FindShapeClosestPointCurveFromParam(SHAPE *shape, qword pos, struct SPACE *space, qword *posOut, qword *dirOut, float *paramOut, float *distanceOut, float startParam) __asm__("FindShapeClosestPointCurveFromParam"); void FindShapeClosestPointCurveFromParam(SHAPE *shape, qword pos, struct SPACE *space, qword *posOut, qword *dirOut, float *paramOut, float *distanceOut, float startParam)
{
    struct CTX ctx;
    float gRes;
    ctx.space = space; ctx.q = pos; ctx.p = shape;
    MinimizeRange__FPFPvf_fPvffffPfT6(GetShapeDistanceSquaredAtParam, &ctx, startParam, (shape->vertexParams[1] - shape->vertexParams[0]) * 0.1f, shape->isClosed ? D_002A8130 : shape->vertexParams[0], shape->isClosed ? D_002A8134 : shape->vertexParams[shape->numControlVertices - 1], &gRes, 0);
    gRes = WrapShapeParam(shape, gRes);
    if (posOut || dirOut) EvaluateShapeAtParam(shape, gRes, posOut, dirOut);
    if (paramOut) *paramOut = gRes;
    if (distanceOut) *distanceOut = GetShapeDistanceAtParam(shape, gRes);
}
} // namespace M_func_001E6DB8

// Dispatches on the shape's mode to the linear or curve closest-point search from a starting parameter. Also known as: NearestCrv (upstream readable / Sly 1 name; NearestCrv__FP3CRV).
extern "C" void FindShapeClosestPointFromParam(SHAPE *shape, qword pos, SPACE *space, qword *posOut, qword *dirOut, float *paramOut, float *distanceOut, float startParam)
{
    if (shape->curveKind == 0) {
        FindShapeClosestPointLinearFromParam(shape, pos, space, posOut, dirOut, paramOut, distanceOut, startParam);
        return;
    }
    FindShapeClosestPointCurveFromParam(shape, pos, space, posOut, dirOut, paramOut, distanceOut, startParam);
}

// Position and tangent at arc length s.
extern "C" void EvaluateShapeAtDistance(SHAPE *shape, float distance, qword *posOut, qword *dirOut)
{
    EvaluateShapeAtParam(shape, GetShapeParamAtDistance(shape, distance), posOut, dirOut);
}

namespace M_FindShapeClosestPoint {
// FindShapeClosestPoint (w2lane4, wave 2): closest point of a path (a chain of control points, transformed by the path's
// own matrix) to a query point. mode 0 walks every segment, projects the point on it (clamped to the segment), keeps the
// nearest candidate and reports its position, direction, interpolated time and the path value at that time. mode 1 only
// measures the distances to the control points and hands the nearest vertex time to FindShapeClosestPointFromParam.
// Matching notes (found by reading the local-alloc / global-alloc / reload dumps):
//  * Spill slots of start / end / point follow the order in which the three pseudos get their stack home, i.e. the order in which
//    they are first address-wrapped while expanding the loop head: `Sub(end=TransformPoint(..), start)` then `Sub(point, start)`
//    gives start 0x60, end 0x70, point 0x80 (writing `end=..; Sub(end,start)` first reverses start and end).
//  * The quotient's two dot products convert the VU result with an explicit qmfc2 (Bits) + mtc1 (FromBits): that keeps the Dot result
//    a VU-class pseudo, so local-alloc gives it a real VU register and the segment direction lands in vf3 (and the matrix rows reload
//    in the original order). Length() instead lists the VU value as an "r" operand of the mtc1 asm (ToFloatR), which keeps its Dot
//    result in a GPR pair with a reload-chosen VU register; the two spellings are not interchangeable.
//  * Normalize ties its output to the length operand ("+j"), Dot's `one` is not early-clobber (the second dot reuses vf3).
//  * The empty asm on `time` is allocation-only (see the comment at its use).
// Qword wrapper convertible to and from qword.
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
struct M4 { qword a, b, c, d; };
struct SPACE;

extern float D_002A8138 __asm__("D_002A8138");
qword ApplyConstrNoTransform(SPACE *, qword) __asm__("ApplyConstrNoTransform");
float GetShapeDistanceAtParam(SHAPE *, float) __asm__("GetShapeDistanceAtParam");
void FindShapeClosestPointFromParam(SHAPE *, qword, SPACE *, qword *, qword *, float *, float *, float) __asm__("FindShapeClosestPointFromParam");

struct VU_FLOAT {
    qword q;
    // Broadcast a float into a VU0 register.
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1,%0" : "=j"(q) : "r"(f)); }
};
// Transform point v by the 4x4 matrix m.
static inline VV TransformPoint(const M4 &m, VV v)
{
    qword r;
    __asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w"
            : "=j"(r) : "j"(m.a), "j"(m.b), "j"(m.c), "j"(m.d), "j"(v));
    return r;
}
// Same idiom as sly2/vu0.h VuSub; kept local because its callers pass mixed operand types (the template wants one).
static inline VV VuSub(VV a, VV b)
{
    qword r;
    __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
// Return v1 * f1 + v2 * f2.
static inline VV VecCombo(VU_FLOAT f1, VV v1, VU_FLOAT f2, VV v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1), "j"(f2.q), "j"(v2));
    return r;
}
// x lane of v as raw int bits.
static inline int Bits(qword v) { int b; __asm__("qmfc2.ni %0,%1" : "=r"(b) : "j"(v)); return b; }
// Reinterpret int bits as a float.
static inline float FromBits(int b) { float f; __asm__("mtc1 %1,%0" : "=f"(f) : "r"(b)); return f; }
// x lane of v as a float.
static inline float ToFloat(qword v) { return FromBits(Bits(v)); }
// Square root of the x lane of a.
static inline VV Root(VV a)
{
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "+j"(a));
    return a;
}
// Length of the xyz part of a as a float.
static inline float Length(VV a) { return QwordX(Root(VuDot(a, a))); }
// Unit vector along a.
static inline VV Normalize(VV a)
{
    qword length = Root(VuDot(a, a));
    __asm__("vdiv $Q,$vf0w,%0x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q" : "+j"(length) : "j"(a));
    return length;
}
// Global closest point on the shape to a position (segment scan, or nearest vertex then refinement); outputs the
// point, tangent, parameter and distance. Also known as: NearestCrvAll (upstream readable / Sly 1 name; NearestCrvAll__FP3CRVG9VU_VECTORP6VECTORT2PfT4T2).
extern "C" void FindShapeClosestPoint(SHAPE *p, qword point, SPACE *space, qword *outPosition, qword *outDirection,
                              float *outTime, float *outValue)
{
    M4 matrix;
    QWROW closest;
    qword start, end;
    matrix = *(M4 *)AsLo(p)->GetWorldMatrix();
    switch (p->curveKind) {
    case 0: {
        start = TransformPoint(matrix, ((qword *)p->vertexPosq)[0]);
        float best = D_002A8138, time = -1.0f;
        for (int index = 1; index < p->numControlVertices; index++) {
            // allocation-only: an in-place use/set of `time` makes global-alloc rank it ahead of `best`
            // (time gets f22, best f23); it emits no instruction.
            __asm__("" : "+f"(time));
            qword direction = VuSub(end = TransformPoint(matrix, ((qword *)p->vertexPosq)[index]), start);
            qword offset = VuSub(point, start);
            if (space) {
                direction = ApplyConstrNoTransform(space, direction);
                offset = ApplyConstrNoTransform(space, offset);
            }
            float t = ToFloat(VuDot(offset, direction)) / ToFloat(VuDot(direction, direction));
            t = Clamp(t, 0.0f, 1.0f);
            float u = 1.0f - t;
            closest.q = VecCombo(VU_FLOAT(u), start, VU_FLOAT(t), end);
            qword delta = VuSub(point, closest.q);
            if (space) delta = ApplyConstrNoTransform(space, delta);
            float distance = Length(delta);
            if (distance < best) {
                best = distance;
                time = u * p->vertexParams[index - 1] + t * p->vertexParams[index];
                if (outPosition) *outPosition = closest.q;
                if (outDirection) *outDirection = Normalize(VuSub(end, start));
            }
            start = end;
        }
        if (outTime) *outTime = time;
        if (outValue) *outValue = GetShapeDistanceAtParam(p, time);
        break;
    }
    case 1: {
        float best = D_002A8138;
        int closestIndex = -1;
        for (int index = 0; index < p->numControlVertices; index++) {
            qword displacement = VuSub(point, TransformPoint(matrix, ((qword *)p->vertexPosq)[index]));
            if (space) displacement = ApplyConstrNoTransform(space, displacement);
            float distance = Length(displacement);
            if (distance < best) { best = distance; closestIndex = index; }
        }
        FindShapeClosestPointFromParam(p, point, space, outPosition, outDirection, outTime, outValue, p->vertexParams[closestIndex]);
        break;
    }
    }
}
} // namespace M_FindShapeClosestPoint

namespace M_FindShapeClosestPointFromS {
float GetShapeParamAtDistance(void *p, float seed) __asm__("GetShapeParamAtDistance");
void FindShapeClosestPointFromParam(void *p, qword q, void *a, qword *b, qword *c, float *d, float *e, float g) __asm__("FindShapeClosestPointFromParam");
// Closest-point search started from arc length s. Sly 1: FindCrvlClosestPointFromS / FindCrvcClosestPointFromS. Also known as: NearestCrvS (upstream readable / Sly 1 name; NearestCrvS__FP3CRVG9VU_VECTORPvP6VECTORT3PfT5f).
extern "C" void FindShapeClosestPointFromDistance(void *shape, qword pos, void *space, qword *posOut, qword *dirOut, float *paramOut, float *distanceOut, float startDistance)
{
    FindShapeClosestPointFromParam(shape, pos, space, posOut, dirOut, paramOut, distanceOut, GetShapeParamAtDistance(shape, startDistance));
}
} // namespace M_FindShapeClosestPointFromS

namespace M_func_001E7410 {
extern float D_002A813C __asm__("D_002A813C");
void EvaluateShapeAtParam(SHAPE *p, int a, int b, float g) __asm__("EvaluateShapeAtParam");
float GetShapeDistanceAtParam(SHAPE *p, float g) __asm__("GetShapeDistanceAtParam");
// Picks the vertex whose parameter minimizes a callback and evaluates position, direction and distance there.
// (inferred)
extern "C" void FindShapeBestVertex(SHAPE *shape, float (*score)(float, void *), void *context, int a, int b, float *paramOut, float *distanceOut)
{
    int i;
    float gBest = 0.0f;
    float dMin = D_002A813C;
    for (i = 0; i < shape->numControlVertices; i++) {
        float g = shape->vertexParams[i];
        float d = score(g, context);
        if (d < dMin) {
            dMin = d;
            gBest = g;
        }
    }
    if (a || b) EvaluateShapeAtParam(shape, a, b, gBest);
    if (paramOut) *paramOut = gBest;
    if (distanceOut) *distanceOut = GetShapeDistanceAtParam(shape, gBest);
}
} // namespace M_func_001E7410

namespace M_IsegFindShapeU {
float WrapShapeParam(SHAPE *p, float g) __asm__("WrapShapeParam");
// Index of the segment containing u (searching from the cached hint), with the offset into it and its span. Also known as: ISegCrvG (upstream readable / Sly 1 name; ISegCrvG__FP3CRVPfT1f).
extern "C" int FindShapeSegmentAtParam(SHAPE *shape, float *offsetOut, float *spanOut, float param)
{
    int i;
    param = WrapShapeParam(shape, param);
    i = shape->segmentHint;
    if (shape->vertexParams[i] < param) { int n = shape->numControlVertices - 1; while (i < n) { if (param < shape->vertexParams[i]) break; i++; } i--; } else { while (i > 0) { if (shape->vertexParams[i] <= param) break; i--; } } { float g0 = shape->vertexParams[i]; if (offsetOut) *offsetOut = param - g0; if (spanOut) *spanOut = shape->vertexParams[i + 1] - g0; }
    shape->segmentHint = i;
    return i;
}
} // namespace M_IsegFindShapeU

namespace M_IsegFindShapeS {
float WrapShapeDistance(SHAPE *p, float g) __asm__("WrapShapeDistance");

// a < b (slt) in a pinned register.
extern "C" { static inline int Before(int a, int b)
{
    register int result __asm__("$2");
    __asm__("slt %0,%1,%2" : "=r"(result) : "r"(a), "r"(b));
    return result;
} }

// Copy the loop bound into $5.
extern "C" { static inline int LoopEnd(int value)
{
    register int result __asm__("$5");
    __asm__("daddu %0,%1,$zero" : "=r"(result) : "r"(value));
    return result;
} }

// Load p->numPoints into $3.
extern "C" { static inline int Count(SHAPE *p)
{
    register int value __asm__("$3");
    __asm__("lw %0,%1" : "=r"(value) : "m"(p->numControlVertices));
    return value;
} }

// Index of the segment containing arc length s. Also known as: ISegCrvS (upstream readable / Sly 1 name; ISegCrvS__FP3CRVPfT1f).
extern "C" int FindShapeSegmentAtDistance(SHAPE *shape, float *offsetOut, float *spanOut, float distance)
{
    int i;
    distance = WrapShapeDistance(shape, distance);
    i = shape->segmentHint;
    if (shape->vertexArcLengths[i] < distance) {
        int limit = Count(shape) - 1;
        if (!Before(i, limit)) goto forwardDone;
        if (distance < shape->vertexArcLengths[i]) goto forwardDone;
        {
            int end = LoopEnd(limit);
            do {
                ++i;
                if (i >= end) break;
                if (distance < shape->vertexArcLengths[i]) break;
            } while (1);
        }
forwardDone:
        --i;
    } else {
        for (; i > 0; i--)
            if (shape->vertexArcLengths[i] <= distance) break;
    }
    if (offsetOut) *offsetOut = distance - shape->vertexArcLengths[i];
    if (spanOut) *spanOut = shape->vertexArcLengths[i + 1] - shape->vertexArcLengths[i];
    shape->segmentHint = i;
    return i;
}
} // namespace M_IsegFindShapeS

namespace M_func_001E77B0 {
void FindShapeClosestPoint(void *a, int b, int c, qword *pq, int d, int e, int f) __asm__("FindShapeClosestPoint");
// Only the closest point of FindShapeClosestPoint.
extern "C" qword GetShapeClosestPoint(void *a, int b)
{
    qword q;
    FindShapeClosestPoint(a, b, 0, &q, 0, 0, 0);
    return q;
}
} // namespace M_func_001E77B0

namespace M_DuGetShapeSearchIncrement {

// 0x1E77E0: dead-code scrap (sly2/scrap.h)
extern "C" { SLY2_SCRAP("addiu $sp, $sp, 0xa0"); }

// Parameter step for searches: (uMax - uMin) / (count * 10). Also known as: DgStepCrv (upstream readable / Sly 1 name; DgStepCrv__FP3CRV).
extern "C" float GetShapeParamSearchStep(SHAPE *shape)
{
    float a = GetShapeMaxParam(shape);
    return (a - GetShapeMinParam(shape)) / (shape->numControlVertices * 10.0f);
}
} // namespace M_DuGetShapeSearchIncrement

namespace M_func_001E7840 {
void *func_001AE4F8(int cb) __asm__("__builtin_new");
SHAPECHAIN *InitChain(void *pv, SHAPE *p) __asm__("InitChain");
void MeasureShape(SHAPE *p) __asm__("MeasureShape");
void SetLoBusyKind(SHAPE *p, int n) __asm__("SetLoBusyKind");
// Creates the shape's chain (rope simulation) if missing, measures it and marks the LO busy; f = 1 re-arms an
// existing chain. Also known as: PropeEnsure (upstream readable / Sly 1 name; PropeEnsure__FP2LOi).
extern "C" SHAPECHAIN *EnsureShapeChain(SHAPE *shape, int rearm)
{
    if (!shape->chain) {
        shape->chain = InitChain(func_001AE4F8(sizeof(SHAPECHAIN)), shape);
        MeasureShape(shape);
        SetLoBusyKind(shape, 0);
        { sptr v = (sptr)shape->ploupd; if ((v ^ 1) & 1) ((LOUPD *)v)->grf &= ~0x20; else shape->unknown40 = v & ~0x20; }
        { sptr v = (sptr)shape->ploupd; if ((v ^ 1) & 1) ((LOUPD *)v)->grf |= 0xC; else shape->unknown40 = v | 0xC; }
    } else if (rearm == 1) {
        SetLoBusyKind(shape, 0);
        { sptr v = (sptr)shape->ploupd; if ((v ^ 1) & 1) ((LOUPD *)v)->grf |= 0x4; else shape->unknown40 = v | 0x4; }
    }
    return shape->chain;
}
} // namespace M_func_001E7840

namespace M_func_001E7930 {
void *EnsureShapeChain(void *p, int n) __asm__("EnsureShapeChain");
void SetChainEndObject(void *p, int a, int b) __asm__("SetChainEndObject");
// Pins chain end 0 to an object (or the shape's start when 0).
extern "C" void SetShapeChainStartObject(void *shape, int object)
{
    SetChainEndObject(EnsureShapeChain(shape, 1), 0, object);
}
} // namespace M_func_001E7930

namespace M_func_001E7968 {
void *EnsureShapeChain(void *p, int n) __asm__("EnsureShapeChain");
void SetChainEndObject(void *p, int a, int b) __asm__("SetChainEndObject");
// Pins chain end 1 to an object (or the shape's end when 0).
extern "C" void SetShapeChainEndObject(void *shape, int object)
{
    SetChainEndObject(EnsureShapeChain(shape, 1), 1, object);
}
} // namespace M_func_001E7968

namespace M_func_001E79A0 {
SHAPECHAIN *EnsureShapeChain(SHAPE *p, int n) __asm__("EnsureShapeChain");
// Current length of the shape's chain (last sample; 0 without points).
extern "C" float GetShapeChainLength(SHAPE *shape)
{
    float g;
    if (shape->numControlVertices > 0) g = EnsureShapeChain(shape, 0)->distances[shape->numControlVertices - 1];
    else g = 0.0f;
    return g;
}
} // namespace M_func_001E79A0

namespace M_func_001E79F0 {
void *EnsureShapeChain(void *p, int n) __asm__("EnsureShapeChain");
void AddChainSample(void *p, float g) __asm__("AddChainSample");
// Ensures the chain and appends a sample at g.
extern "C" void AddShapeChainSample(void *shape, float param)
{
    AddChainSample(EnsureShapeChain(shape, 1), param);
}
} // namespace M_func_001E79F0

namespace M_SetShapeChainMode {
SHAPECHAIN *EnsureShapeChain(SHAPE *p, int f) __asm__("EnsureShapeChain");
void SetLoBusyKind(SHAPE *p, int n) __asm__("SetLoBusyKind");
// Sets the chain's simulation mode; -1 disables updates, any other value ensures the chain and enables them.
extern "C" void SetShapeChainMode(SHAPE *shape, int mode)
{
    if (mode != -1) {
        EnsureShapeChain(shape, 1);
        SetLoBusyKind(shape, 0);
        { sptr v = (sptr)shape->ploupd; if ((v ^ 1) & 1) ((LOUPD *)v)->grf &= ~0x20; else shape->unknown40 = v & ~0x20; }
        { sptr v = (sptr)shape->ploupd; if ((v ^ 1) & 1) ((LOUPD *)v)->grf |= 0xC; else shape->unknown40 = v | 0xC; }
    } else {
        SetLoBusyKind(shape, 0);
        { sptr v = (sptr)shape->ploupd; if ((v ^ 1) & 1) ((LOUPD *)v)->grf |= 0x20; else shape->unknown40 = v | 0x20; }
        { sptr v = (sptr)shape->ploupd; if ((v ^ 1) & 1) ((LOUPD *)v)->grf &= ~0xC; else shape->unknown40 = v & ~0xC; }
    }
    if (shape->chain) shape->chain->mode = mode;
}
} // namespace M_SetShapeChainMode

namespace M_func_001E7B38 {
// Signed distance from a to b along the shape, taken the shorter way round on closed shapes.
extern "C" float GetShapeDistanceBetween(SHAPE *shape, float from, float to)
{
    float d = to - from;
    if (shape->isClosed) {
        float r = GetShapeMaxDistance(shape);
        if (d > r * 0.5f) d -= r;
        else if (d < r * -0.5f) d += r;
    }
    return d;
}
} // namespace M_func_001E7B38

namespace M_func_001E7BB8 {
// World bounds from transformed path points and segment controls.
union U {qword q;float f[4];};
extern void UpdateXfmMatWorld(SHAPE *) __asm__("UpdateXfmMatWorld");
// Transform point p by the matrix rows r0..r3 (old only influences register allocation).
extern "C" { static inline qword Point(qword old,qword r0,qword r1,qword r2,qword r3,qword p){__asm__("vmulax.xyz ACC,%1,%5x\n\tvmadday.xyz ACC,%2,%5y\n\tvmaddaz.xyz ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w":"+j"(old):"j"(r0),"j"(r1),"j"(r2),"j"(r3),"j"(p));return old;} }
// World-space axis-aligned bounds of the transformed points and curve control points.
extern "C" void GetShapeBounds(SHAPE *shape,qword*minOut,qword*maxOut){
 register qword r0 __asm__("vf2"),r1 __asm__("vf3"),r2 __asm__("vf4"),r3 __asm__("vf11");
 register qword maximum __asm__("vf5"),minimum __asm__("vf6");
 register qword *first __asm__("$5");
 UpdateXfmMatWorld(shape);
 {register struct MATRIX4Q*m __asm__("$2");
 __asm__("lw %0,%2\n\taddiu %1,%3,%4":"=&r"(first),"=r"(m):"m"(shape->vertexPosq),"r"(shape),"I"(SLY2_OFFSETOF(SHAPE, matqWorld)));
 __asm__("lqc2 %0,0x30(%8)\n\tlqc2 %1,0x10(%8)\n\tlqc2 %2,0x20(%8)\n\tlqc2 %3,%7":"=j"(r3),"=j"(r1),"=j"(r2),"=j"(r0):"m"(m->r[3].q),"m"(m->r[1].q),"m"(m->r[2].q),"m"(shape->matqWorld.r[0].q),"r"(m));}
 {register qword value __asm__("vf1")=*first;register int mode __asm__("$2");
 __asm__ volatile("vmulax.xyz ACC,%2,%6x\n\tvmadday.xyz ACC,%3,%6y\n\tvmaddaz.xyz ACC,%4,%6z\n\tvmaddw.xyz %0,%5,$vf0w\n\tlw %1,%7":"=&j"(maximum),"=r"(mode):"j"(r0),"j"(r1),"j"(r2),"j"(r3),"j"(value),"m"(shape->curveKind));
 minimum=maximum;
 if(mode==0){
  register int n __asm__("$4")=shape->numControlVertices;
  register int greater __asm__("$2");
  __asm__("addiu %0,$zero,%2\n\tslt %0,%0,%1":"=&r"(greater):"r"(n),"I"(1));
  if(greater){
   register qword a __asm__("vf8"),b __asm__("vf7"),c __asm__("vf3"),d __asm__("vf2");
   register qword *point __asm__("$3");
   register int remaining __asm__("$2");
   __asm__("vmove.xyzw %0,%3\n\taddiu %1,%4,%6\n\tvmove.xyzw %2,%5":"=j"(a),"=r"(point),"=j"(b):"j"(r0),"r"(first),"j"(r1),"I"(sizeof(qword)));
   remaining=n-1;
   __asm__("vmove.xyzw %0,%2\n\tvmove.xyzw %1,%3":"=j"(c),"=j"(d):"j"(r2),"j"(r3));
   do {register qword value __asm__("vf1")=*point;
    --remaining;value=Point(value,a,b,c,d,value);++point;
    __asm__("vmax.xyz %0,%0,%2\n\tvmini.xyz %1,%1,%2":"+&j"(maximum),"+j"(minimum):"j"(value));
   }while(remaining);
  }
 }else{
  register int bound __asm__("$7");
  register qword previous __asm__("vf7");
  __asm__("lw $2,%1\n\taddiu %0,$2,%2":"=r"(bound):"m"(shape->numControlVertices),"I"(-1):"$2");
  previous=maximum;
  if(bound>0){
   register qword a __asm__("vf10"),b __asm__("vf9"),c __asm__("vf8");
   register qword *incoming __asm__("$6"),*outgoing __asm__("$4"),*point __asm__("$5");
   register float *time __asm__("$3");
   register float positive __asm__("$f3"),negative __asm__("$f2");
   register int remaining __asm__("$2");
   __asm__("lw $2,%4\n\tvmove.xyzw %0,%6\n\tlw %1,%5\n\tvmove.xyzw %2,%7\n\taddiu %3,$2,%8":"=j"(a),"=r"(time),"=j"(b),"=r"(incoming):"m"(shape->incomingTangentq),"m"(shape->vertexParams),"j"(r0),"j"(r1),"I"(sizeof(qword)):"$2");
   positive=0.333333343f;
#ifdef SLY2_EE_ASM
   __asm__("vmove.xyzw %0,%2\n\tli.s %1,%3":"=j"(c),"=f"(negative):"j"(r2),"F"(-0.333333343f):"$1");
#else
   c = r2; negative = -0.333333343f;
#endif
   {register float lower __asm__("$f0");
   point=first;
   __asm__("lw %0,%4\n\taddiu %1,%1,%7\n\tdaddu %2,%5,$zero\n\tlwc1 %3,%6":"=&r"(outgoing),"+&r"(point),"=&r"(remaining),"=f"(lower):"m"(shape->outgoingTangentq),"r"(bound),"m"(time[0]),"I"(sizeof(qword)));
   goto curve_body;
   do {
    lower=time[0];
curve_body: {union U scalar;
    register qword old __asm__("vf4"),value __asm__("vf1"),plus __asm__("vf3"),minus __asm__("vf2");
    register float interval __asm__("$f1");
    register int bits __asm__("$7");
    __asm__("vmove.xyzw %0,%3\n\tlwc1 %1,%4\n\taddiu %2,%2,%5":"=j"(old),"=f"(interval),"+r"(remaining):"j"(previous),"m"(time[1]),"I"(-1));
    value=*point;
    __asm__("addiu %0,%0,%3\n\tsub.s %1,%1,%2":"+r"(time),"+f"(interval):"f"(lower),"I"(sizeof(float)));
    __asm__("vmulax.xyz ACC,%1,%5x\n\tvmadday.xyz ACC,%2,%5y\n\tvmaddaz.xyz ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w":"+j"(previous):"j"(a),"j"(b),"j"(c),"j"(r3),"j"(value));
    __asm__("addiu %0,%0,%5\n\tmul.s %1,%2,%3\n\tmul.s %2,%2,%4":"+r"(point),"=&f"(lower),"+f"(interval):"f"(positive),"f"(negative),"I"(sizeof(qword)));
    __asm__("mfc1 %0,%1":"=r"(bits):"f"(lower));
    __asm__("qmtc2.ni %2,%0\n\tsqc2 %0,%1":"=j"(plus),"=m"(scalar.q):"r"(bits));
    __asm__("mfc1 %0,%1":"=r"(bits):"f"(interval));
    __asm__("qmtc2.ni %3,%0\n\tlqc2 %1,%4\n\tsqc2 %0,%2":"=j"(minus),"=j"(value),"=m"(scalar.q):"r"(bits),"m"(*outgoing));
    __asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"+j"(value):"j"(a),"j"(b),"j"(c));
    __asm__("vadda.xyzw ACC,%0,$vf0\n\tvmaddx.xyzw %0,%2,%3x\n\taddiu %1,%1,%4":"+j"(old),"+r"(outgoing):"j"(value),"j"(plus),"I"(sizeof(qword)));
    __asm__("vmax.xyz %0,%0,%3\n\tvmini.xyz %1,%1,%3\n\tlqc2 %2,%4":"+&j"(maximum),"+&j"(minimum),"=j"(value):"j"(old),"m"(*incoming));
    __asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"+j"(value):"j"(a),"j"(b),"j"(c));++incoming;
    __asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%0x":"+j"(minus):"j"(previous),"j"(value));
    __asm__("vmax.xyz %0,%0,%2\n\tvmini.xyz %1,%1,%2\n\tvmini.xyz %1,%1,%3\n\tvmax.xyz %0,%0,%3":"+&j"(maximum),"+&j"(minimum):"j"(minus),"j"(previous));
   }
   }while(remaining);
   }
  }
 }
 }
 *minOut=minimum;*maxOut=maximum;
}

#define OFF(T,f) ((unsigned)&((T*)0)->f)
typedef char bound_layout_0[(sizeof(qword)==16)?1:-1];
typedef char bound_layout_1[(__alignof__(qword)==16)?1:-1];
typedef char bound_layout_2[(sizeof(union U)==16)?1:-1];
typedef char bound_layout_3[(__alignof__(union U)==16)?1:-1];
typedef char bound_layout_4[(sizeof(struct MATRIX4Q)==64)?1:-1];
typedef char bound_layout_5[(__alignof__(struct MATRIX4Q)==16)?1:-1];
typedef char bound_layout_6[(sizeof(((struct MATRIX4Q*)0)->r)==64)?1:-1];
typedef char bound_layout_7[(sizeof(((struct MATRIX4Q*)0)->r[0].q)==16)?1:-1];
typedef char bound_layout_8[(OFF(struct MATRIX4Q,r)==0)?1:-1];
typedef char bound_layout_17[(sizeof(void*)==4)?1:-1];
typedef char bound_layout_18[(sizeof(int)==4)?1:-1];
typedef char bound_layout_19[(sizeof(float)==4)?1:-1];
} // namespace M_func_001E7BB8
#undef OFF

namespace M_func_001E7D90 {
// Bounding radius from transformed path vertices and Bezier controls.
union U {qword q;float f[4];};
struct BASIS3 {qword r[3];};
extern void UpdateXfmMatWorld(SHAPE *) __asm__("UpdateXfmMatWorld");
// Load the point at source into vf1.
extern "C" { static inline qword ReadPoint(const qword*source){register qword value __asm__("vf1");__asm__("lqc2 %0,%1":"=j"(value):"m"(*source));return value;} }
// Rotate a in place by the 3x3 matrix rows m0..m2.
extern "C" { static inline qword DirInPlace(qword m0,qword m1,qword m2,qword a){__asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"+j"(a):"j"(m0),"j"(m1),"j"(m2));return a;} }
// Squared length of the xyz part of a as a float.
extern "C" { static inline float LenSq(qword a){qword one;float f;__asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(a),"=&j"(one));__asm__("mtc1 %1,%0":"=f"(f):"r"(a));return f;} }
/* returns the larger (C++: an inline taking &squared would put the pinned register on the stack) */
extern "C" { static inline float MaxR(float g,float value){if(g<value)g=value;return g;} }
// Broadcast f into vf4 and store it to *out.
extern "C" { static inline qword Scalar(float f,union U*out){register int bits __asm__("$10");register qword q __asm__("vf4");__asm__("mfc1 %0,%1":"=r"(bits):"f"(f));__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(bits));out->q=q;return out->q;} }
// Square root of f.
extern "C" { static inline float Sqrt(float f){return __builtin_sqrtf(f);} }
// Bounding radius from the transformed vertices and Bezier controls (after the world matrix update).
extern "C" float UpdateShapeBoundingSphere(SHAPE *shape)
{
    register float squared __asm__("$f2");
    UpdateXfmMatWorld(shape);squared=0.0f;
    if(shape->curveKind==0){
        register int remaining __asm__("$3")=shape->numControlVertices;
        if(remaining>0){
            register qword r0 __asm__("vf3"),r2 __asm__("vf5"),r1 __asm__("vf4");
            register qword *point __asm__("$4");
            register struct MATRIX4Q *basis __asm__("$2")=&shape->matqWorld;
            __asm__("lqc2 %0,%4\n\tlqc2 %1,0x20(%8)\n\tlqc2 %2,0x10(%8)\n\tlw %3,%7":"=j"(r0),"=j"(r2),"=j"(r1),"=r"(point):"m"(shape->matqWorld.r[0].q),"m"(basis->r[2].q),"m"(basis->r[1].q),"m"(shape->vertexPosq),"r"(basis));
            do {qword v=DirInPlace(r0,r1,r2,*point);squared=MaxR(squared,LenSq(v));point++;}while(--remaining);
        }
    }else{
        register struct MATRIX4Q *incomingBasis __asm__("$8");
        register qword r0 __asm__("vf3"),r1 __asm__("vf4"),r2 __asm__("vf5");
        register qword firstPoint __asm__("vf1"),previous __asm__("vf6");
        register qword *first __asm__("$5")=(qword *)shape->vertexPosq;
        register int i __asm__("$4"),limit __asm__("$2");
        __asm__("addiu $2,%6,%9\n\tlqc2 %0,0x10($2)\n\tdaddu %1,$2,$zero\n\tlqc2 %2,0x20($2)\n\tdaddu %3,$zero,$zero\n\tlqc2 %4,%8\n\tlqc2 %5,%7"
          :"=j"(r1),"=&r"(incomingBasis),"=j"(r2),"=&r"(i),"=j"(firstPoint),"=j"(r0)
          :"r"(shape),"m"(*(const struct BASIS3*)&shape->matqWorld),"m"(*first),"I"(SLY2_OFFSETOF(SHAPE, matqWorld)):"$2");
        __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
          :"=j"(previous):"j"(r0),"j"(r1),"j"(r2),"j"(firstPoint));
        __asm__("lw %0,%3\n\tvmul.xyz $vf1,%2,%2\n\tvaddw.x $vf2,$vf0,$vf0w\n\tvadday.x ACC,$vf1,$vf1y\n\tvmaddz.x $vf1,$vf2,$vf1z\n\tqmfc2.ni $3,$vf1\n\taddiu %0,%0,%4\n\tmtc1 $3,%1"
          :"=r"(limit),"=f"(squared):"j"(previous),"m"(shape->numControlVertices),"I"(-1):"$3","vf1","vf2");
        if(limit>0){
            register float positive __asm__("$f4"),negative __asm__("$f3");
            register qword a __asm__("vf10"),b __asm__("vf9"),c __asm__("vf8");
            register qword *outgoing __asm__("$7"),*point __asm__("$6");
            register float *time __asm__("$3");
            register int iterations __asm__("$9");
            positive=0.333333343f;
            __asm__("vmove.xyzw %0,%1":"=j"(a):"j"(r0));
            negative=-0.333333343f;
            __asm__("vmove.xyzw %0,%3\n\tlw %1,%4\n\tvmove.xyzw %2,%5"
              :"=j"(b),"=r"(outgoing),"=j"(c):"j"(r1),"m"(shape->outgoingTangentq),"j"(r2));
            __asm__("lw %0,%3\n\tdaddu %1,%4,$zero\n\taddiu %2,%5,%6"
              :"=&r"(time),"=&r"(iterations),"=r"(point):"m"(shape->vertexParams),"r"(limit),"r"(first),"I"(sizeof(qword)));
            do{
                union U scalar;
                register qword old __asm__("vf5"),value __asm__("vf1"),next __asm__("vf7"),k __asm__("vf4");
                register float interval __asm__("$f1"),measured __asm__("$f0");
                register unsigned offset __asm__("$5");
                register qword *incoming __asm__("$2");
                register qword row0 __asm__("vf5"),row1 __asm__("vf3"),row2 __asm__("vf2");
                __asm__("lwc1 $f0,%4\n\tvmove.xyzw %0,%6\n\tlwc1 %1,%5\n\tsll %2,%7,%9\n\tlqc2 %3,%8\n\tsub.s %1,%1,$f0"
                  :"=j"(old),"=f"(interval),"=&r"(offset),"=j"(value)
                  :"m"(time[0]),"m"(time[1]),"j"(previous),"r"(i),"m"(*point),"I"(4):"$f0");
                {register int bits __asm__("$10");
                __asm__("vmulax.xyz ACC,%3,%6x\n\tvmadday.xyz ACC,%4,%6y\n\tvmaddz.xyz %0,%5,%6z\n\tvmove.xyzw %1,%0\n\tmul.s $f0,%7,%8\n\tmfc1 %2,$f0"
                  :"=j"(next),"=j"(previous),"=r"(bits)
                  :"j"(a),"j"(b),"j"(c),"j"(value),"f"(interval),"f"(positive):"$f0");
                __asm__("qmtc2.ni %1,%0":"=j"(k):"r"(bits));scalar.q=k;}
                value=ReadPoint(outgoing);
                __asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z"
                  :"+j"(value):"j"(a),"j"(b),"j"(c));
                __asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%0,%2x":"+j"(value):"j"(old),"j"(k));
                __asm__("vmul.xyz $vf2,%1,%1\n\tvaddw.x $vf3,$vf0,$vf0w\n\tvadday.x ACC,$vf2,$vf2y\n\tvmaddz.x $vf2,$vf3,$vf2z\n\tqmfc2.ni $2,$vf2\n\tmtc1 $2,%0"
                  :"=f"(measured):"j"(value):"$2","vf2","vf3");
                squared=MaxR(squared,measured);
                __asm__("mul.s %0,%3,%4\n\tlw %1,%5\n\tlqc2 %2,%6\n\taddu %1,%7,%1"
                  :"=f"(measured),"=&r"(incoming),"=j"(row0)
                  :"f"(interval),"f"(negative),"m"(shape->incomingTangentq),"m"(shape->matqWorld.r[0].q),"r"(offset));
                k=Scalar(measured,&scalar);
                __asm__("lqc2 %0,0x10(%6)\n\tlqc2 %1,0x10(%7)\n\tlqc2 %2,0x20(%6)"
                  :"=j"(row1),"=j"(value),"=j"(row2)
                  :"m"(incomingBasis->r[1].q),"m"(incoming[1]),"m"(incomingBasis->r[2].q),"r"(incomingBasis),"r"(incoming));
                __asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z"
                  :"+j"(value):"j"(row0),"j"(row1),"j"(row2));
                __asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%0,%2x":"+j"(value):"j"(next),"j"(k));
                __asm__("vmul.xyz $vf2,%1,%1\n\tvaddw.x $vf3,$vf0,$vf0w\n\tvadday.x ACC,$vf2,$vf2y\n\tvmaddz.x $vf2,$vf3,$vf2z\n\tqmfc2.ni $2,$vf2\n\tmtc1 $2,%0"
                  :"=f"(measured):"j"(value):"$2","vf2","vf3");
                squared=MaxR(squared,measured);
                __asm__("vmul.xyz $vf1,%1,%1\n\tvaddw.x $vf2,$vf0,$vf0w\n\tvadday.x ACC,$vf1,$vf1y\n\tvmaddz.x $vf1,$vf2,$vf1z\n\tqmfc2.ni $10,$vf1\n\tmtc1 $10,%0"
                  :"=f"(measured):"j"(previous):"$10","vf1","vf2");
                squared=MaxR(squared,measured);
                ++i;++outgoing;++time;++point;
            }while(i<iterations);
        }
    }
    return Sqrt(squared);
}

#define OFF(T,f) ((unsigned)&((T*)0)->f)
typedef char verified_radius_0[(sizeof(qword)==16)?1:-1];
typedef char verified_radius_1[(__alignof__(qword)==16)?1:-1];
typedef char verified_radius_2[(sizeof(union U)==16)?1:-1];
typedef char verified_radius_3[(sizeof(struct MATRIX4Q)==0x40)?1:-1];
typedef char verified_radius_4[(OFF(struct MATRIX4Q,r)==0)?1:-1];
typedef char verified_radius_13[(sizeof(void*)==4)?1:-1];
typedef char verified_radius_14[(sizeof(int)==4)?1:-1];
typedef char verified_radius_15[(sizeof(float)==4)?1:-1];
typedef char verified_radius_16[(__alignof__(struct MATRIX4Q)==16)?1:-1];
typedef char verified_radius_17[(__alignof__(union U)==16)?1:-1];
typedef char verified_radius_18[(sizeof(((struct MATRIX4Q*)0)->r)==64)?1:-1];
typedef char verified_radius_19[(sizeof(((struct MATRIX4Q*)0)->r[0].q)==16)?1:-1];

typedef char radius_basis_prefix_size[(sizeof(struct BASIS3)==48)?1:-1];
typedef char radius_basis_prefix_align[(__alignof__(struct BASIS3)==16)?1:-1];
} // namespace M_func_001E7D90
#undef OFF

/* The chain mode, or -1. */
// The chain's simulation mode, or -1 without a chain.
extern "C" int GetShapeChainMode(SHAPE *shape)
{
    SHAPECHAIN *pchain = shape->chain;
    int n = -1;
    if (pchain != 0) {
        n = pchain->mode;
    }
    return n;
}

namespace M_SRandomShapeRange {
// Store upper to *endpoint and load the float whose high half is highHalf (lui + mtc1), in one asm block.
extern "C" { static inline float StoreEndpointAndUpperBits(float *endpoint,float upper,unsigned highHalf)
{
    register unsigned bits __asm__("$1");
    register float bound __asm__("$f12");
    __asm__(".set noat\n\tswc1 %3,%0\n\tlui %2,%4\n\tmtc1 %2,%1\n\t.set at"
            :"=m"(*endpoint),"=f"(bound),"=&r"(bits):"f"(upper),"i"(highHalf));
    return bound;
} }
extern int D_002C3650 __asm__("D_002C3650");
float RandomFloatInRange__Fff(float, float) __asm__("RandomFloatInRange__Fff");
float WrapShapeDistance(SHAPE *, float) __asm__("WrapShapeDistance");
// Random arc length within +-range of center, wrapped or clamped to the shape; the distribution is chosen by a table. Also known as: SRandCrvNear (upstream readable / Sly 1 name; SRandCrvNear__FP3CRVff).
extern "C" float GetRandomShapeDistanceInRange(SHAPE *shape, float center, float range)
{
    float r[2];
    float t,u,upper,minimum;
    {register float upper __asm__("$f0"),lower __asm__("$f1");
    if (shape->isClosed) {
        lower = center - range;
        upper = center + range;
    } else {
        float m = GetShapeMaxDistance(shape);
        float c = Clamp(center, range, m - range);
        lower = StdMax(c - range, 0.0f);
        upper = StdMin(c + range, m);
    }
    minimum=StoreEndpointAndUpperBits(&r[1],upper,0xBF800000u>>16);
    r[0] = lower;
    }
    t = RandomFloatInRange__Fff(minimum, 1.0f);
    switch (D_002C3650) {
    case 0:
        break;
    case 1:
        t = t * __builtin_fabsf(t);
        break;
    case 2:
        if (0.0f < t) upper = t * t * (3.0f - (t + t));
        else upper = t * t * (-3.0f - (t + t));
        t = upper;
        break;
    case 3:
        if (0.0f < t) upper = SqrtFloat(t);
        else upper = -SqrtFloat(-t);
        t = upper;
        break;
    }
    t = t * 0.5f + 0.5f;
    u = (1.0f - t) * r[0] + t * r[1];
    if (shape->isClosed)
        u = WrapShapeDistance(shape, u);
    return u;
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_int[(sizeof(int)==4)?1:-1];
typedef char layout_float[(sizeof(float)==4)?1:-1];
typedef char layout_P_closed[((unsigned)&((SHAPE*)0)->isClosed==0xE4)?1:-1];
} // namespace M_SRandomShapeRange

namespace M_InitChain {
extern char D_002C44F0[] __asm__("D_002C44F0");   /* g_xfWorld */
void *AllocClearedFromWorldHeap(struct SWALLOC*) __asm__("AllocClearedFromWorldHeap");
void SyncChainDistances(SHAPECHAIN*) __asm__("SyncChainDistances");
// Transform point a by the 4x4 matrix m.
extern "C" { static inline qword Point(MATRIX4Q *m,qword a) {qword r;__asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w":"=j"(r):"j"(m->r[0].q),"j"(m->r[1].q),"j"(m->r[2].q),"j"(m->r[3].q),"j"(a));return r;} }
// Initialises a chain for a shape: a private copy of the points and distances and the rest lengths. Also known as: InitRope (upstream readable / Sly 1 name; InitRope__FP4ROPEP2LO).
extern "C" SHAPECHAIN *InitChain(SHAPECHAIN *chain,SHAPE *path) {
 struct SWALLOC size;MATRIX4Q *m;int i;
 chain->pshape=path;chain->mode=-1;
 size.cb=path->numControlVertices*16;size.cbAlign=16;chain->positions=(__typeof__(chain->positions))AllocClearedFromWorldHeap(&size);
 size.cb=path->numControlVertices*16;size.cbAlign=16;chain->velocities=(__typeof__(chain->velocities))AllocClearedFromWorldHeap(&size);
 size.cb=path->numControlVertices*4;size.cbAlign=16;chain->distances=(__typeof__(chain->distances))AllocClearedFromWorldHeap(&size);
 chain->forceq.f[2]=-1750.0f;chain->damping=3.0f;chain->bend=500.0f;chain->bendDamping=5.0f;
 chain->forceq.i[0]=0;chain->forceq.i[1]=0;
 chain->capacity=chain->pshape->numControlVertices;chain->step=300.0f;
 m=(MATRIX4Q *)AsLo(chain->pshape)->GetWorldMatrix();
 for(i=0;i<chain->pshape->numControlVertices;i++)chain->positions[i].q=Point(m,((qword *)chain->pshape->vertexPosq)[i]);
 chain->pxfFirst=chain->pxfLast=(XF *)D_002C44F0;
 if(chain->pshape->numControlVertices>0) {
  chain->start=chain->positions[0].q;chain->end=chain->positions[chain->pshape->numControlVertices-1].q;
 }
 SyncChainDistances(chain);
 return chain;
}
} // namespace M_InitChain

namespace M_func_001E8378 {
/* StepChainRk4 (Lilly's range): one RK4-style step of a spring rope (CHAIN, same struct as matched StepChain).
 * Matching tricks (T1869, see notes/190000-1F0000_lanes.md):
 *  - the 4 empty volatile asms at the end of the pass loop add 4 RTL insns (no code) so the hoisted start/startVel/end/endVel
 *    copies get vf12..14 in the game's order (global-alloc live length);
 *  - NormalizeV takes the previous length as a dummy "f" input and &fallback as a dummy "r" input: sched1 then issues
 *    mtc1, addiu, qmtc2 in the game's order;
 *  - proj2 is computed into a local first and stored into the U after the shared `damping` VU_FLOAT, so its home store has the
 *    higher LUID (sched1 tie-break) and issues after the damping store.
 * Repeated "j" inputs are allocation-only (local-alloc priority). */
union U { qword q; float f[4]; };
// Qword wrapper convertible to and from qword.
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
struct VU_FLOAT {
    qword q;
    // Broadcast a float into a VU0 register.
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); }
    // Wrap an existing qword.
    VU_FLOAT(const qword &v) { q = v; }
    // Broadcast raw int bits into a VU0 register.
    VU_FLOAT(int w) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(w)); }
    // Broadcast the float word at pf into a VU0 register.
    VU_FLOAT(const float *pf) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "rV"(*(const int *)pf)); }  /* a float word from memory */
};
// Wrap a qword as a VU_VECTOR.
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
struct SPACE { char pad[0x40]; MATRIX4Q matrix; };

struct CHAIN
{
    qword force;
    qword pad10;
    qword start;
    qword end;
    SPACE *first;
    SPACE *last;
    SHAPE *path;
    int mode;
    int pad50;
    U *position;
    U *velocity;
    float *distance;
    float damping;
    float stretch, stretchDamping;
    float bend, bendDamping;
    float shape, shapeDamping;
};
extern float D_002C3658[4] __asm__("D_002C3658");
extern float D_002C3668[4] __asm__("D_002C3668");
extern qword D_002A2DA0 __asm__("D_002A2DA0");
extern qword g_normalZ __asm__("g_normalZ");

qword GetXfVelocity(SPACE *, SPACE *, qword) __asm__("GetXfVelocity");
void CopyAb__FPvT0Ui(void *, const void *, unsigned) __asm__("CopyAb__FPvT0Ui");
int ClipEdgeToSphere(qword posCenter, float sRadius, qword pos0, qword pos1, int clsgMax, LSG *alsg) __asm__("ClipEdgeToSphere");
MATRIX4Q *InvertMatrix4Rigid(MATRIX4Q *, MATRIX4Q *) __asm__("InvertMatrix4Rigid");
void StepChainRk4(CHAIN *) __asm__("StepChainRk4");


// Transform point v by the 4x4 matrix m.
static inline VV TransformPoint(const MATRIX4Q &m, VV v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(r) : "j"(m.r[0].q), "j"(m.r[1].q), "j"(m.r[2].q), "j"(m.r[3].q), "j"(v));
    return r;
}
// Same idiom as sly2/vu0.h VuSub; kept local because the shared template changes StepChainRk4 (48/838 instructions off).
static inline VV VuSub(VV a, VV b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
/* float out of a VF register: asm mtc1, the compiler emits the qmfc2 (reload); the one-cost asm decides the sched2 ties */
// Same idiom as sly2/vu0.h QwordX; kept local because the shared template changes StepChainRk4 (10/838 instructions off).
static inline float QwordX(qword q) { float g; __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q)); return g; }
// Same idiom as sly2/vu0.h VuSqrtX; kept local because the shared template changes StepChainRk4 (10/838 instructions off).
static inline VV VuSqrtX(VV v) { qword r; __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(v)); return r; }
// Same idiom as sly2/vu0.h VuDot; kept local because its callers pass mixed operand types (the template wants one).
static inline VV VuDot(VV a, VV b)
{
    qword r, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(r), "=j"(one) : "j"(a), "j"(b));
    return r;
}
// Length of v (x lane).
static inline VV Length(VV v) { return VuSqrtX(VuDot(v, v)); }
// Same idiom as sly2/vu0.h VuDivByX; kept local because its callers pass mixed operand types (the template wants one).
static inline VV VuDivByX(VV v, VV l) { qword r; __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l), "j"(v)); return r; }
// Same idiom as sly2/vu0.h VuScaleByX; kept local because its callers pass mixed operand types (the template wants one).
static inline VV VuScaleByX(VV v, VV g) { qword r; __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(g)); return r; }
/* base + v * g */
static inline VV AddScaled(VV base, VU_FLOAT g, VV v)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %3, %2x" : "=j"(r) : "j"(base), "j"(g.q), "j"(v));
    return r;
}
/* AddScaled2 for the integration step: the extra base/g inputs are allocation only (refs -> local-alloc priority:
   k/2 before the velocity read, accP before it too -> k/2 vf2, accP vf3, velocity vf4 as in the game) */
static inline VV AddScaled2I(VV base, VU_FLOAT g, VV v1, VV v2)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %3, %2x\n\tvmaddx.xyzw %0, %4, %2x" : "=j"(r) : "j"(base), "j"(g.q), "j"(v1), "j"(v2), "j"(base), "j"(base), "j"(g.q), "j"(g.q), "j"(g.q));
    return r;
}
/* VecCombo3 for the two stretch springs; the repeated v2 / v1 input is allocation only (raises proj1 / dir2 in local-alloc's
   priority order so the stretch block gets the game's vf registers: proj1 vf4, dir2 vf3, g1 vf5) */
static inline VV VecCombo3S1(VV base, VU_FLOAT g1, VV v1, VU_FLOAT g2, VV v2)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %3, %2x\n\tvmaddx.xyzw %0, %5, %4x"
            : "=j"(r) : "j"(base), "j"(g1.q), "j"(v1), "j"(g2.q), "j"(v2), "j"(v2));
    return r;
}
// Return base + v1*g1 + v2*g2.
static inline VV VecCombo3S2(VV base, VU_FLOAT g1, VV v1, VU_FLOAT g2, VV v2)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %3, %2x\n\tvmaddx.xyzw %0, %5, %4x"
            : "=j"(r) : "j"(base), "j"(g1.q), "j"(v1), "j"(g2.q), "j"(v2), "j"(v1));
    return r;
}
/* Sub with b repeated (allocation only): the newV[i] reload outranks the proj2 chain, so it gets vf1 and proj2 vf2 */
static inline VV SubX(VV a, VV b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b), "j"(b)); return r; }
/* base + v1 * g1 + v2 * g2 */
static inline VV VecCombo3(VV base, VU_FLOAT g1, VV v1, VU_FLOAT g2, VV v2)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %3, %2x\n\tvmaddx.xyzw %0, %5, %4x"
            : "=j"(r) : "j"(base), "j"(g1.q), "j"(v1), "j"(g2.q), "j"(v2));
    return r;
}
/* base + (v1 + v2) * g */
static inline VV AddScaled2(VV base, VU_FLOAT g, VV v1, VV v2)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %3, %2x\n\tvmaddx.xyzw %0, %4, %2x"
            : "=j"(r) : "j"(base), "j"(g.q), "j"(v1), "j"(v2));
    return r;
}
/* float -> VF without a VU_FLOAT object: no asm-output temp, so no home store (the game has none for the 2nd spring's g) */
static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1.q), "j"(f2.q), "j"(v2.q));
    return r;
}
/* normalize *pv in place (fallback when shorter than 0.0001); returns the original length */
static inline float Normalize(U *pv, const qword &fallback)
{
    VV len = Length(pv->q);
    if (VuToFloat(len) < 0.0001f)
        pv->q = fallback;
    else
        pv->q = VuDivByX(pv->q, len);
    return QwordX(len);
}
/* Normalize for a vector re-read from memory after a join (dir2): the game loads it into a GPR (lq) and reload copies it into
   the dot's VF input (qmtc2, no RTL insn of its own); the dot hands the VF copy on to the divide as a tied output.
   Allocation only: the two "r"(v) operands keep v in a GPR (the target's lq $v0 + qmtc2 $v0,$vf3). */
static inline float NormalizeV(U *pv, const qword &fallback, float prev)
{
    qword v = pv->q;
    qword d, one, vf;
    __asm__("vmul.xyz %1, %3, %3\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z"
            : "=j"(one), "=j"(d), "=j"(vf) : "2"(v), "r"(v), "r"(v), "r"(&fallback), "f"(prev));   /* "r"(v) x2: allocation only */
    VV len = VuSqrtX(d);
    if (VuToFloat(len) < 0.0001f)
        pv->q = fallback;
    else
        pv->q = VuDivByX(vf, len);
    return QwordX(len);
}
// True when a and b are closer than s.
static inline int FWithin(VV a, VV b, float s) { return QwordX(Length(VuSub(a, b))) < s; }

// Store v to pd.
static inline void Store(U *pd, VV v) { pd->q = v; }
// The qword in p.
static inline const qword &Q(const U *p) { return p->q; }
// Return a - b.
static inline VV SubP(const U *a, const U *b) { return VuSub(a->q, b->q); }
// Store the interpolation (1-t)*pa + t*b into pd.
static inline void Blend(U *pd, const U *pa, const qword &b, float t) { pd->q = VecCombo(VU_FLOAT(1.0f - t), VU_VECTOR(pa->q), VU_FLOAT(t), VU_VECTOR(b)); }
// Store the interpolation (1-t)*pa + t*pb into pd.
static inline void LerpP(U *pd, const U *pa, const U *pb, float t) { pd->q = VecCombo(VU_FLOAT(1.0f - t), VU_VECTOR(pa->q), VU_FLOAT(t), VU_VECTOR(pb->q)); }

// One fourth-order (RK4-style) step of the spring rope chain. Also known as: StepRopeSpring (upstream readable / Sly 1 name; StepRopeSpring__FP4ROPE).
void StepChainRk4(CHAIN *chain)
{
    U newP[64], newV[64], accP[64], accV[64], force[64];
    int i;
    int count = chain->path->numControlVertices;
    VV start = TransformPoint(chain->first->matrix, chain->start);
    VV startVel = GetXfVelocity(chain->first, 0, start);
    VV end = TransformPoint(chain->last->matrix, chain->end);
    VV endVel = GetXfVelocity(chain->last, 0, end);
    if ((unsigned)count > 64)
        return;
    CopyAb__FPvT0Ui(accP, chain->position, count * 16);
    CopyAb__FPvT0Ui(accV, chain->velocity, count * 16);
    for (int pass = 0; pass < 4; pass++) {
        float w = D_002C3658[pass];
        float h = w * g_clock.deltaTime;
        Blend(&newP[0], &chain->position[0], start, w);
        Blend(&newV[0], &chain->velocity[0], startVel, w);
        Blend(&newP[count - 1], &chain->position[count - 1], end, w);
        Blend(&newV[count - 1], &chain->velocity[count - 1], endVel, w);
        for (i = 1; i < count - 1; i++) {
            Store(&newV[i], AddScaled(chain->velocity[i].q, VU_FLOAT(h), force[i].q));
            Store(&newP[i], AddScaled2(chain->position[i].q, VU_FLOAT(h * 0.5f), chain->velocity[i].q, newV[i].q));
        }
        float scale = 0.75f - 0.25f / (count - 2);
        float rest = chain->distance[count - 1];
        VV chord = SubP(&newP[0], &newP[count - 1]);
        float amplitude = scale * SqrtFloat(StdMax(rest * rest - QwordX(VuDot(chord, chord)), 0.0f));
        for (i = 1; i < count - 1; i++) {
            VV F = AddScaled(chain->force, VU_FLOAT(-chain->damping), newV[i].q);
            if (chain->stretch != 0.0f || chain->stretchDamping != 0.0f) {
                U dir1, dir2, proj1, proj2;
                dir1.q = SubP(&newP[i - 1], &newP[i]);
                dir2.q = SubP(&newP[i + 1], &newP[i]);
                float len1 = Normalize(&dir1, D_002A2DA0);
                float len2 = NormalizeV(&dir2, D_002A2DA0, len1);
                float rest1 = chain->distance[i] - chain->distance[i - 1];
                float rest2 = chain->distance[i + 1] - chain->distance[i];
                proj1.q = VuScaleByX(dir1.q, VuDot(SubP(&newV[i - 1], &newV[i]), dir1.q));
                VV pr2 = VuScaleByX(dir2.q, VuDot(SubX(newV[i + 1].q, newV[i].q), dir2.q));
                VU_FLOAT g1(chain->stretch * (len1 - rest1));
                /* damping built per spring: cse merges the two "r" qmtc2s and reload_cse drops the 2nd home store (same vf, same slot);
                   that dropped store still lengthens the g1 store chain in sched1, so g1 is born before the dir2 reload like the game */
                VU_FLOAT damping(&chain->stretchDamping);
                proj2.q = pr2;
                F = VecCombo3S1(F, g1, dir1.q, damping, proj1.q);
                F = VecCombo3S2(F, VU_FLOAT(VuFromFloat<qword>(chain->stretch * (len2 - rest2))), dir2.q, damping, proj2.q);
            }
            if (chain->bend != 0.0f || chain->bendDamping != 0.0f) {
                U mid, midV, dir, proj;
                float r = (chain->distance[i] - chain->distance[i - 1]) / (chain->distance[i + 1] - chain->distance[i - 1]);
                LerpP(&mid, &newP[i - 1], &newP[i + 1], r);
                LerpP(&midV, &newV[i - 1], &newV[i + 1], r);
                dir.q = SubP(&mid, &newP[i]);
                float len = Normalize(&dir, g_normalZ);
                proj.q = VuScaleByX(dir.q, VuDot(SubP(&midV, &newV[i]), dir.q));
                F = VecCombo3(F, VU_FLOAT(chain->bend * len), dir.q, VU_FLOAT(&chain->bendDamping), proj.q);
            }
            if (chain->shape != 0.0f || chain->shapeDamping != 0.0f) {
                U mid, midV, dir, proj;
                float r = chain->distance[i] / chain->distance[count - 1];
                LerpP(&mid, &newP[0], &newP[count - 1], r);
                LerpP(&midV, &newV[0], &newV[count - 1], r);
                dir.q = SubP(&mid, &newP[i]);
                float len = Normalize(&dir, g_normalZ);
                proj.q = VuScaleByX(dir.q, VuDot(SubP(&midV, &newV[i]), dir.q));
                float r4 = r * 4.0f;
                F = VecCombo3(F, VU_FLOAT(chain->shape * (len - amplitude * r4 * (1.0f - r))), dir.q, VU_FLOAT(&chain->shapeDamping), proj.q);
            }
            float wk = D_002C3668[pass];
            float k = wk * g_clock.deltaTime;
            Store(&accV[i], AddScaled(accV[i].q, VU_FLOAT(k), F));
            Store(&accP[i], AddScaled2I(accP[i].q, VU_FLOAT(k * 0.5f), chain->velocity[i].q, newV[i].q));
            Store(&force[i], F);
        }
        /* 4 empty asms: +4 RTL insns in the pass loop (live-length probe, emits nothing) */
        __asm__ __volatile__("");
        __asm__ __volatile__("");
        __asm__ __volatile__("");
        __asm__ __volatile__("");
    }
    if (chain->mode == 3) {
        LSG lsg;
        int j0 = 1;
        chain->position[0].q = start;
        chain->velocity[0].q = startVel;
        for (i = 1; i < count - 1; i++) {
            float step = chain->distance[i] - chain->distance[i - 1];
            if (FWithin(end, chain->position[i - 1].q, step))
                break;
            for (int j = j0; j < count; j++) {
                float t;
                if (!ClipEdgeToSphere(chain->position[i - 1].q, step, Q(&accP[j - 1]), Q(&accP[j]), 1, &lsg)) {
                    chain->position[i] = accP[j - 1];
                    t = 0.0f;
                } else if (lsg.au[1] < 1.0f) {
                    t = lsg.au[1];
                    chain->position[i].q = lsg.apos[1];
                } else
                    continue;
                chain->path->vertexArcLengths[i] = step;
                j0 = j;
                LerpP(&chain->velocity[i], &accV[j - 1], &accV[j], t);
                break;
            }
        }
        chain->position[i].q = end;
        chain->velocity[i].q = endVel;
        count = i + 1;
        chain->distance[i] = chain->distance[i - 1] + QwordX(Length(VuSub(chain->position[i].q, chain->position[i - 1].q)));
        chain->path->vertexArcLengths[i] = chain->distance[i];
        chain->path->numControlVertices = count;
    } else {
        chain->position[0].q = start;
        chain->velocity[0].q = startVel;
        chain->position[count - 1].q = end;
        chain->velocity[count - 1].q = endVel;
        CopyAb__FPvT0Ui(&chain->position[1], &accP[1], (count - 2) * 16);
        CopyAb__FPvT0Ui(&chain->velocity[1], &accV[1], (count - 2) * 16);
    }
    MATRIX4Q inverse;
    {
        SHAPE *path = chain->path;
        InvertMatrix4Rigid(&inverse, (MATRIX4Q *)AsLo(path)->GetWorldMatrix());
    }
    for (i = 0; i < count; i++)
    {
        SHAPE *path = chain->path;
        ((U *)path->vertexPosq)[i].q = TransformPoint(inverse, chain->position[i].q);
    }
    if (chain->mode != 3) {
        chain->path->vertexArcLengths[0] = 0.0f;
        for (i = 1; i < count; i++)
            chain->path->vertexArcLengths[i] = chain->path->vertexArcLengths[i - 1] + QwordX(Length(VuSub(((U *)chain->path->vertexPosq)[i - 1].q, ((U *)chain->path->vertexPosq)[i].q)));
    }
    InvalidateShapeCache(chain->path);
}
} // namespace M_func_001E8378

namespace M_func_001E9090 {
/* StepChain: one simulation step of a rope-like path chain (wave-4 lane 2, from w3lane9's rewrite).
 *
 * The first point is the start offset transformed by the first space's matrix; its velocity comes from
 * GetXfVelocity. Every later point is integrated (accel = force - damping * v, v += accel * dt,
 * pos += (v_old + v_new) * dt/2), then pulled back to the rest distance |dist[i] - dist[i-1]| from the previous
 * point along the normalized displacement (fallback direction D_002A2DA0 when shorter than 0.0001), and the
 * part of the relative velocity along that direction is removed. With mode 2 the last point is pinned to the
 * end offset in the last space and AddChainSample extends the path. Finally every point is transformed into
 * the path's local space (inverse of its world matrix, virtual slot 31) and InvalidateShapeCache refreshes the path.
 *
 * Two register-choice devices (everything else is plain C++ plus one asm per VU operation):
 *  - DotVf1 hard-wires the dot product's 1.0 scratch to $vf1 (clobbered), used for the velocity projection only.
 *    With an allocated "=j" scratch, local-alloc gives the tied difference/result quantity (10 refs over 5
 *    insns, priority 3.0) vf1 before the scratch (priority 2.0); the target has them the other way round. This
 *    stands in for the function's one register pin.
 *  - AddScaled lists its vector input twice (allocation only): the velocity read then outranks the tied
 *    force/acceleration quantity (8 vs 6 refs), so it gets vf1 as in the target.
 */
union U { qword q; float f[4]; };
// Qword wrapper convertible to and from qword.
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
// Float broadcast into a VU0 register on construction.
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct SPACE { char pad[0x40]; MATRIX4Q matrix; };
struct CHAIN
{
    qword force;
    qword pad10;
    qword start;            // offset in the first space
    qword end;              // offset in the last space
    SPACE *first;
    SPACE *last;
    SHAPE *path;
    int mode;               // 2: the last point is pinned to the end offset
    int pad50;
    U *position;
    U *velocity;
    float *distance;        // rest distance along the chain
    float damping;
};
extern qword D_002A2DA0 __asm__("D_002A2DA0");    // fallback direction

qword GetXfVelocity(SPACE *, SPACE *, qword) __asm__("GetXfVelocity");
MATRIX4Q *InvertMatrix4Rigid(MATRIX4Q *, MATRIX4Q *) __asm__("InvertMatrix4Rigid");   // invert
void AddChainSample(CHAIN *, float) __asm__("AddChainSample");
void StepChain(CHAIN *) __asm__("StepChain");


// Transform point v by the 4x4 matrix m.
static inline VV TransformPoint(const MATRIX4Q &m, VV v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(r) : "j"(m.r[0].q), "j"(m.r[1].q), "j"(m.r[2].q), "j"(m.r[3].q), "j"(v));
    return r;
}
// Same idiom as sly2/vu0.h VuSub; kept local because the shared template changes StepChain (62/215 instructions off).
static inline VV VuSub(VV a, VV b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
/* Dot with the 1.0 scratch in $vf1 (see the header comment) */
static inline VV DotVf1(VV a, VV b)
{
    qword r;
    __asm__("vmul.xyz %0, %1, %2\n\tvaddw.x $vf1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, $vf1, %0z"
            : "=j"(r) : "j"(a), "j"(b) : "vf1");
    return r;
}
// Length of v (x lane).
static inline VV Length(VV v) { return VuSqrtX(VuDot(v, v)); }
/* base + v * g */
static inline VV AddScaled(VV base, VU_FLOAT g, VV v)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %3, %2x"
            : "=j"(r) : "j"(base), "j"(g.q), "j"(v), "j"(v));   /* second "j"(v): allocation only */
    return r;
}
// Unit vector along v, or fallback when its length is below 1e-4.
static inline VV NormalizeSafe(VV v, VV fallback)
{
    VV len = Length(v);
    if (VuToFloat(len) < 0.0001f)
        return fallback;
    return VuDivByX(v, len);
}
/* base + v1 * g1 + v2 * g2 */
static inline VV VecCombo3(VV base, VU_FLOAT g1, VV v1, VU_FLOAT g2, VV v2)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %3, %2x\n\tvmaddx.xyzw %0, %5, %4x"
            : "=j"(r) : "j"(base), "j"(g1.q), "j"(v1), "j"(g2.q), "j"(v2));
    return r;
}

// One Verlet-style step of the rope chain with distance constraints and pinned ends. Also known as: StepRopeTrail (upstream readable / Sly 1 name; StepRopeTrail__FP4ROPE).
void StepChain(CHAIN *chain)
{
    float dt = g_clock.deltaTime;
    VV prev = TransformPoint(chain->first->matrix, chain->start);
    int n = chain->path->numControlVertices;
    chain->position[0].q = prev;
    U *velocity = chain->velocity;
    VV prevVel = GetXfVelocity(chain->first, 0, prev);
    velocity[0].q = prevVel;
    int i;
    for (i = 1; i < n; i++)
    {
        VV accel = AddScaled(chain->force, VU_FLOAT(-chain->damping), chain->velocity[i].q);
        VV vel = AddScaled(chain->velocity[i].q, VU_FLOAT(dt), accel);
        VV pos = VecCombo3(chain->position[i].q, VU_FLOAT(dt * 0.5f), chain->velocity[i].q, VU_FLOAT(dt * 0.5f), vel);
        VV delta = VuSub(pos, prev);
        VV dir = NormalizeSafe(delta, D_002A2DA0);
        float proj = -QwordX(DotVf1(VuSub(vel, prevVel), dir));
        float d = Abs(chain->distance[i] - chain->distance[i - 1]);
        prev = AddScaled(prev, VU_FLOAT(d), dir);
        prevVel = AddScaled(vel, VU_FLOAT(proj), dir);
        chain->position[i].q = prev;
        chain->velocity[i].q = prevVel;
    }

    if (chain->mode == 2)
    {
        chain->position[n - 1].q = TransformPoint(chain->last->matrix, chain->end);
        AddChainSample(chain, chain->distance[n - 2] + VuToFloat(Length(VuSub(chain->position[n - 2].q, chain->position[n - 1].q))));
        n = chain->path->numControlVertices;
    }
    MATRIX4Q inverse;
    {
        SHAPE *path = chain->path;
        InvertMatrix4Rigid(&inverse, (MATRIX4Q *)AsLo(path)->GetWorldMatrix());
    }
    for (i = 0; i < n; i++)
    {
        SHAPE *path = chain->path;
        ((U *)path->vertexPosq)[i].q = TransformPoint(inverse, chain->position[i].q);
    }
    InvalidateShapeCache(chain->path);
}
} // namespace M_func_001E9090

namespace M_func_001E93F0 {
struct M4 { qword r[3]; qword pos; };
extern char D_002C44F0[] __asm__("D_002C44F0");   /* g_xfWorld */
void EvaluateShapeAtParam(SHAPE *, ::qword *, int, float) __asm__("EvaluateShapeAtParam");
// Attaches chain end i to an object's transform (slot 32), or to the shape's start/end point. Also known as: SetRopeEnd (upstream readable / Sly 1 name; SetRopeEnd__FP4ROPEiP2LO).
extern "C" void SetChainEndObject(SHAPECHAIN *chain, int end, LO *object)
{
    chain->aploEnd[end] = object;
    if (object) {
        struct M4 m;
        chain->apxfEnd[end] = object->GetLocalTransform((MATRIX4 *)&m);
        *(chain->aposEnd + end) = m.pos;
    } else {
        float f;
        chain->apxfEnd[end] = (XF *)D_002C44F0;
        if (end == 0)
            f = GetShapeMinParam(chain->pshape);
        else
            f = GetShapeMaxParam(chain->pshape);
        EvaluateShapeAtParam(chain->pshape, &chain->aposEnd[end], 0, f);
    }
}
} // namespace M_func_001E93F0

namespace M_func_001E94B0 {
struct AR { int cb; int cbAlign; };
SHAPE *CreateLo__F3CIDP2SO3OIDi(int k, SO *n, void *pv, int m) __asm__("CreateLo__F3CIDP2SO3OIDi");
void func_002333B4(void *p, int b, int cb) __asm__("memset");
void *AllocFromWorldHeap(struct AR *par) __asm__("AllocFromWorldHeap");
// Creates a SHAPE (CID 10) with c samples (parameters 0..c-1) and empty distance/point arrays.
extern "C" SHAPE *NewSampledShape(void *pv, int count)
{
    struct AR ar;
    int i;
    SHAPE *p = CreateLo__F3CIDP2SO3OIDi(10, 0, pv, -1);
    MATRIX4 *pm;
    p->curveKind = (CRVK)0;
    p->isClosed = 0;
    pm = &p->localMatrix;
    func_002333B4(pm, 0, 0x40);
    pm->mat[3][3] = 1.0f; pm->mat[2][2] = 1.0f; pm->mat[1][1] = 1.0f; pm->mat[0][0] = 1.0f;
    ar.cb = count * 4;
    ar.cbAlign = 0x10;
    p->vertexParams = (__typeof__(p->vertexParams))AllocFromWorldHeap(&ar);
    for (i = 0; i < count; i++) p->vertexParams[i] = i;
    ar.cb = count * 4;
    ar.cbAlign = 0x10;
    p->vertexArcLengths = (__typeof__(p->vertexArcLengths))AllocFromWorldHeap(&ar);
    ar.cb = count * 16;
    ar.cbAlign = 0x10;
    p->vertexPositions = (VECTOR4 *)AllocFromWorldHeap(&ar);
    return p;
}
} // namespace M_func_001E94B0

namespace M_func_001E95B0 {
/* AddChainSample (Claude agent, wave-2 lane 7): append a time sample to a sampled path (two U arrays a/b, a
 * path with positions/distances, a time array). If the new time is earlier than the last sample the tail is
 * truncated; otherwise samples are inserted until the new time is within a step of the last one or the
 * capacity is reached, interpolating a/b/pos/dist for the sample that becomes the second-to-last.
 *
 * Loop trick: the target loop is a real loop (its refs are weighted x2 for register allocation, which is what
 * puts the right-hand pointer of each Lerp in $a2 and n*4 in $a3) but loop.c did NOT optimize it (it hoists the
 * 1.0f and the %hi of the step constant otherwise). The first sample is tested before the loop and the loop is
 * entered with a goto to its first label, so loop.c sees a jump into the loop and leaves it alone. */
extern float D_002C3678 __asm__("D_002C3678");
extern void AddChainSample(SHAPECHAIN*,float) __asm__("AddChainSample");
// Float broadcast into a VU0 register on construction.
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
// Wrap a qword as a VU_VECTOR.
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
// Return v1 * f1 + v2 * f2.
static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1.q), "j"(f2.q), "j"(v2.q));
    return r;
}
/* pd = p0 * cf + p1 * f */
static inline void Lerp(QWROW*pd,const QWROW*p0,const QWROW*p1,float f,float cf){pd->q=VecCombo(VU_FLOAT(cf),VU_VECTOR(p0->q),VU_FLOAT(f),VU_VECTOR(p1->q));}
// Appends a time sample to a chain's sampled path (truncating when time goes back, interpolating inserted samples
// every `step`); a full chain (capacity reached) drops to mode 0. Also known as: SetRopeLength (upstream readable / Sly 1 name; SetRopeLength__FP4ROPEf).
void AddChainSample(SHAPECHAIN*chain,float time){
 int count=chain->pshape->numControlVertices;
 int n;
 if(time<chain->distances[count-1]){
  int i=count;
  for(;;){--i;if(i<0)break;if(chain->distances[i]<time)break;}
  i++;
  chain->pshape->numControlVertices=i+1;
  chain->distances[i]=time;
  goto refresh;
 }
 n=count;
 if(time<chain->distances[count-2]+D_002C3678*chain->step){chain->distances[count-1]=time;goto finished;}
 goto top;
 for(;;){
 top:
  if(n<chain->capacity){
   n++;
   chain->positions[n-1]=chain->positions[n-2];
   chain->velocities[n-1]=chain->velocities[n-2];
   chain->distances[n-1]=time;
   chain->pshape->vertexPosq[n-1]=chain->pshape->vertexPosq[n-2];
   chain->pshape->vertexArcLengths[n-1]=chain->pshape->vertexArcLengths[n-2];
   chain->distances[n-2]=chain->distances[n-3]+chain->step;
   float f=(chain->distances[n-2]-chain->distances[n-3])/(chain->distances[n-1]-chain->distances[n-3]);
   float cf=1-f;
   Lerp(&chain->positions[n-2],&chain->positions[n-3],&chain->positions[n-1],f,cf);
   Lerp(&chain->velocities[n-2],&chain->velocities[n-3],&chain->velocities[n-1],f,cf);
   Lerp(&chain->pshape->vertexPosq[n-2],&chain->pshape->vertexPosq[n-3],&chain->pshape->vertexPosq[n-1],f,cf);
   chain->pshape->vertexArcLengths[n-2]=cf*chain->pshape->vertexArcLengths[n-3]+f*chain->pshape->vertexArcLengths[n-1];
   if(time<chain->distances[n-2]+D_002C3678*chain->step){chain->distances[n-1]=time;break;}
  }else{chain->mode=0;break;}
 }
finished:
 chain->pshape->numControlVertices=n;
refresh:
 InvalidateShapeCache(chain->pshape);
}
} // namespace M_func_001E95B0

// Copies the shape's distance array into its chain. Also known as: CopyRopeDistances (upstream readable / Sly 1 name; CopyRopeDistances__FP4ROPE).
extern "C" void SyncChainDistances(SHAPECHAIN *chain)
{
    SHAPE *pshape = chain->pshape;
    CopyAb(chain->distances, pshape->vertexArcLengths, pshape->numControlVertices * 4);
}

namespace M_func_001E9858 {
/* PostCid106Load: rebuild every mesh entry's sample array in path-local coordinates.
 *
 * For each mesh node and each of its entries: copy the entry's vertices to a scratch array, allocate the
 * output sample array, then for every vertex
 *   world = vertex * owner->mat                         (point transform)
 *   FindShapeClosestPoint finds the closest hit/direction/distance on the path for `world`
 *   BuildOrthonormalMatrixZ builds a basis from (direction, axis); it is transposed (= inverted)
 *   sample = (world - hit) * inverse basis, with .w = distance.
 *
 * Notes on getting the exact code (plain C++ plus the VU0 / MMI asm that the original needs):
 *  - `axis`, `next` and `world` are ordinary locals.  Nothing is saved to a struct: the compiler spills them
 *    (stack slots 0x8C0, 0x8D0, 0x8E0) and reloads them around the calls, and that spill is what gives the
 *    `lq v0,0x20(v0) ; sq v0,0x8C0(sp)` pair at function entry.
 *  - TransposeInto is split into two asm statements (interleave, then recombine): the four early-clobber
 *    temporaries are then allocated before the inputs (v0,v1,a0,a1 / inputs a2,a3,t0), as in the original.
 *  - No register pins: with the plain loop variables the allocation priorities put the node pointer in s4,
 *    the `&work.phase` pointer in s7.
 */
union U { qword q; float f[4]; };
struct M3 { U a, b, c; };
struct M4 { qword a, b, c, d; };
struct E { int n; char pad4[8]; U *vertices; char pad10[0x1C - 0x10]; U *samples; };
struct MESH { char pad[0x14]; };
struct OWNER { char pad[0x40]; M4 mat; };
struct STATE { int pad; OWNER *object; char pad8[0x28 - 8]; };
struct NODE { MESH *mesh; int n; E *entries; NODE *next; };
struct CONFIG { void *owner; char pad4[0xC]; U zero; float limit; char pad24[0x18]; MESH *meshes; STATE *states; void *ae; };
struct S { char pad[0x80]; CONFIG config; char padD0[0xDC - 0xD0]; NODE *head; char padE0[0x150 - 0xE0]; void *path; };
union PHASE { SWALLOC request; U hit; };
struct WORK { PHASE phase; U direction; M3 basis, temporary, inverse; float distance; };
extern char D_002C44F0 __asm__("D_002C44F0");


void PostSoLoad(S *) __asm__("PostSoLoad");
M3 *GetLoWorldMatrix(S *) __asm__("GetLoWorldMatrix");
void ResetGlobAttachment(CONFIG *, int) __asm__("ResetGlobAttachment");
void CopyAb__FPvT0Ui(void *, const void *, unsigned) __asm__("CopyAb__FPvT0Ui");   /* memcpy */
void *AllocFromWorldHeap(SWALLOC *) __asm__("AllocFromWorldHeap");                            /* allocate (size, align) */
void FindShapeClosestPoint(void *, qword, void *, qword *, qword *, float *, float *) __asm__("FindShapeClosestPoint");
M3 *BuildOrthonormalMatrixZ(M3 *, qword, qword) __asm__("BuildOrthonormalMatrixZ");
void PostCid106Load(S *) __asm__("PostCid106Load");


/* v * 4x4 (translation row applied through vf0.w) */
static inline qword Point(M4 *m, qword v)
{
    qword r = m->a;
    __asm__("vmulax.xyz $ACC,%0,%4x\n\tvmadday.xyz $ACC,%1,%4y\n\tvmaddaz.xyz $ACC,%2,%4z\n\tvmaddw.xyz %0,%3,$vf0w"
            : "+j"(r) : "j"(m->b), "j"(m->c), "j"(m->d), "j"(v));
    return r;
}
/* v * 3x3 */
static inline qword Transform(const M3 &m, qword v)
{
    __asm__("vmulax.xyz $ACC,%1,%0x\n\tvmadday.xyz $ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z"
            : "+j"(v) : "j"(m.a.q), "j"(m.b.q), "j"(m.c.q));
    return v;
}
// Return world - hit.
static inline qword PointDifference(qword world, qword hit)
{
    qword value = world;
    __asm__("vsub.xyzw %0,%0,%1" : "+j"(value) : "j"(hit));
    return value;
}
/* r = transpose of the 3x3 matrix m (MMI interleave/recombine) */
static inline void TransposeInto(M3 &r, const M3 &m)
{
    qword lo, hi, lo1, hi1;
    qword a, b, d;
    __asm__("pextlw %0,%5,%4\n\tpextuw %1,%5,%4\n\tpextlw %2,$zero,%6\n\tpextuw %3,$zero,%6"
            : "=&r"(lo), "=&r"(hi), "=&r"(lo1), "=&r"(hi1) : "r"(m.a.q), "r"(m.b.q), "r"(m.c.q));
    __asm__("pcpyld %0,%4,%3\n\tpcpyud %1,%3,%4\n\tpcpyld %2,%6,%5"
            : "=&r"(a), "=&r"(b), "=&r"(d) : "r"(lo), "r"(lo1), "r"(hi), "r"(hi1));
    r.a.q = a; r.b.q = b; r.c.q = d;
}

// CID 106 slot 15 (PostLoad): PostSoLoad, then stores each mesh vertex as an offset in the path-local frame and a
// distance along the shape. Also known as: PostLoadRopeMesh (upstream readable / Sly 1 name; PostLoadRopeMesh__FP2LO).
void PostCid106Load(S *cid106)
{
    U vertices[128];
    WORK w;
    PostSoLoad(cid106);
    if (!cid106->path || !cid106->head) return;
    qword axis = GetLoWorldMatrix(cid106)->c.q;
    NODE *head = cid106->head;
    for (NODE *node = head; node; node = node->next) {
        int index = node->mesh - cid106->config.meshes;
        STATE *state = &cid106->config.states[index];
        OWNER *object = state->object;
        state->object = (OWNER *)&D_002C44F0;
        int j = 0;
        ResetGlobAttachment(&cid106->config, index);
        if (j < node->n) {
            PHASE *phase = &w.phase;
            do {
                E *e = &node->entries[j];
                int next = j + 1;
                int k = 0;
                CopyAb__FPvT0Ui(vertices, e->vertices, e->n * 16);
                w.phase.request.cb = e->n * 16;
                phase->request.cbAlign = 16;
                U *out = (U *)AllocFromWorldHeap(&phase->request);
                e->samples = out;
                for (k = 0; k < e->n; k++) {
                    qword world = Point(&object->mat, vertices[k].q);
                    FindShapeClosestPoint(cid106->path, world, 0, &phase->hit.q, &w.direction.q, &w.distance, 0);
                    BuildOrthonormalMatrixZ(&w.temporary, w.direction.q, axis);
                    qword difference = PointDifference(world, w.phase.hit.q);
                    w.basis = w.temporary;
                    TransposeInto(w.temporary, w.basis);
                    w.inverse = w.temporary;
                    out->q = Transform(w.inverse, difference);
                    out->f[3] = w.distance;
                    out++;
                }
                j = next;
            } while (j < node->n);
        }
    }
}

#define OFF(T, f) ((unsigned)&((T *)0)->f)
typedef char guard_E[(sizeof(E) == 0x20 && OFF(E, vertices) == 0xC && OFF(E, samples) == 0x1C) ? 1 : -1];
typedef char guard_NODE[(sizeof(NODE) == 16 && OFF(NODE, entries) == 8 && OFF(NODE, next) == 0xC) ? 1 : -1];
typedef char guard_S[(OFF(S, config.meshes) == 0xBC && OFF(S, config.states) == 0xC0 && OFF(S, head) == 0xDC && OFF(S, path) == 0x150) ? 1 : -1];
typedef char guard_OWNER[(OFF(OWNER, mat) == 0x40 && sizeof(STATE) == 40 && OFF(STATE, object) == 4 && sizeof(MESH) == 20) ? 1 : -1];
typedef char guard_WORK[(sizeof(WORK) == 0xC0 && OFF(WORK, direction) == 0x10 && OFF(WORK, temporary) == 0x50 && OFF(WORK, distance) == 0xB0) ? 1 : -1];
} // namespace M_func_001E9858
#undef OFF

namespace M_func_001E9AC0 {
typedef int qword __attribute__((mode(TI)));
struct M3 { qword a,b,c; };
struct V { float x,y,z,w; };
union U { qword q; float f[4]; };
struct E { int n; char pad4[8]; V *out; char pad10[0x1C-0x10]; U *samples; };
struct NODE { int pad; int n; E *entries; NODE *next; };
struct S { char pad[0xDC]; NODE *head; char padE0[0x150-0xE0]; void *path; };

void RenderSoSelf(S*) __asm__("RenderSoSelf");
float GetShapeMaxParam(void*) __asm__("GetShapeMaxParam");
M3 *GetLoWorldMatrix(S*) __asm__("GetLoWorldMatrix");
void EvaluateShapeAtParam(void*,float,qword*,qword*) __asm__("EvaluateShapeAtParam");
M3 BuildOrthonormalMatrixZ(qword,qword) __asm__("BuildOrthonormalMatrixZ");
void RenderCid106Self(S*) __asm__("RenderCid106Self");

// Rotate a by the 3x3 matrix m.
static inline qword Transform(M3 *m,qword a) { __asm__("vmulax.xyz $ACC,%1,%0x\n\tvmadday.xyz $ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"+j"(a):"j"(m->a),"j"(m->b),"j"(m->c));return a; }
// Store the low packed word directly; higher words remain available for Y/Z extraction.
static inline void StoreX(float *p,qword q) {__asm__("sw %1,%0":"=m"(*p):"r"(q));}
// z lane of q as a float.
static inline float GetZ(qword q) {float f;__asm__("pextuw %0,$zero,%1":"=r"(f):"r"(q));return f;}
// y lane of q as a float.
static inline float GetY(qword q) {float f;__asm__("prot3w %0,%1":"=r"(f):"r"(q));return f;}
// Smaller of a and b.
static inline float StdMinLocal(float a,float b) {return b<a?b:a;} // util.h StdMin's spelling; kept local: folding it moves RenderCid106Self's lines (1 instruction off)
} // namespace M_func_001E9AC0
// CID 106 slot 19 (RenderSelf): RenderSoSelf with the mesh deformed along the shape (EvaluateShapeAtParam per sample). Also known as: PlaceRopeMeshScene (upstream readable / Sly 1 name; PlaceRopeMeshScene__FP3ALO).
void CID106::RenderSelf()
{
 using namespace M_func_001E9AC0;
 using M_func_001E9AC0::qword; using M_func_001E9AC0::S;
 S *p = (S *)this;
 RenderSoSelf(p);
 float end=GetShapeMaxParam(p->path);
 qword axis=GetLoWorldMatrix(p)->c;
 NODE *node;
 for(node=p->head;node;node=node->next) {
  int j;
  for(j=0;j<node->n;j++) {
   E *e=&node->entries[j];
   U *src=e->samples; V *out=e->out;
   int k;
   for(k=0;k<e->n;k++) {
    U pos,dir;
    EvaluateShapeAtParam(p->path,StdMinLocal(end,src->f[3]),&pos.q,&dir.q);
    M3 m;
    M3 tmp=BuildOrthonormalMatrixZ(dir.q,axis);
    m=tmp;
    qword result=VuAdd(Transform(&m,src->q),pos.q);   /* qualified: LO::Add hides it in a member */
    src++;
    StoreX(&out->x,result);
    out->y=GetY(result);
    out->z=GetZ(result);
    out++;
   }
  }
 }
 RenderSoSelf(p);
}

namespace M_func_001E9C30 {

void *AllocClearedFromWorldHeap(struct SWALLOC *pmsg) __asm__("AllocClearedFromWorldHeap");
void BuildShapeVolumeSegments(SHAPERANGES *pranges, SHAPE *p) __asm__("BuildShapeVolumeSegments");
// Allocates and builds the shape's volume/segment table once. (inferred)
extern "C" void EnsureShapeVolumeSegments(SHAPE *shape)
{
    struct SWALLOC msg;
    if (!shape->pranges) {
        msg.cb = sizeof(SHAPERANGES);
        msg.cbAlign = 0x10;
        shape->pranges = (SHAPERANGES *)AllocClearedFromWorldHeap(&msg);
        BuildShapeVolumeSegments(shape->pranges, shape);
    }
}
} // namespace M_func_001E9C30

namespace M_func_001E9C88 {
/* BuildShapeVolumeSegments: samples the path po from its start to its end in steps of D_002C367C, collects the distinct
 * objects found at each sample (FindPathzoneAtPos) into apv[64] and the parameter ranges where consecutive samples
 * do not stay inside one object (IsPosInPathzone) into aseg[64] (joining the first/last range when po->isClosed), then copies
 * both lists to the heap (AllocCopyFromWorldHeap) into ps.
 * Matching note: the &apv argument of the final AllocCopyFromWorldHeap call must go through an inline helper; integrate.c
 * then puts sp+0x200 straight into a1, so gcse has nothing to hoist and the apv[cpv++] cursor (s6) keeps an
 * un-noted init in the loop preheader. The MSG request reuses pos2's stack slot. */
typedef int qword __attribute__((mode(TI)));
extern float D_002C367C __asm__("D_002C367C");
void EvaluateShapeAtDistance(SHAPE *, float, qword *, qword *) __asm__("EvaluateShapeAtDistance");
void *FindPathzoneAtPos(qword, int) __asm__("FindPathzoneAtPos");
int IsPosInPathzone(void *, qword, int) __asm__("IsPosInPathzone");
void *AllocCopyFromWorldHeap(struct SWALLOC *, void *) __asm__("AllocCopyFromWorldHeap");
void CopyAb__FPvT0Ui(void *, const void *, unsigned) __asm__("CopyAb__FPvT0Ui");
// Fill the heap message and reallocate pv as a cb-byte copy (kind nKind) on the world heap.
extern "C" { static inline void *PvReallocMsg(struct SWALLOC *pmsg, int cb, int nKind, void *pv) { pmsg->cb = cb; pmsg->cbAlign = nKind; return AllocCopyFromWorldHeap(pmsg, pv); } }
// Samples the shape and records the distinct path zones (FindPathzoneAtPos) it passes through with their parameter
// ranges. (inferred)
extern "C" void BuildShapeVolumeSegments(SHAPERANGES *shape, SHAPE *table)
{
    LM aseg[64];
    void *apv[64];
    qword pos;
    qword pos2;
    LM *pseg = 0;
    int cpv = 0;
    int cseg = 0;
    float tStart, tPrev, t, tEnd;
    void *pPrev;

    shape->pshape = table;
    tStart = GetShapeMinDistance(table);
    tPrev = tStart;
    t = tStart;
    tEnd = GetShapeMaxDistance(shape->pshape);
    EvaluateShapeAtDistance(shape->pshape, tStart, &pos, 0);
    pPrev = FindPathzoneAtPos(pos, 0);
    for (; t < tEnd; t += D_002C367C) {
        void *pCur;
        int outside;
        EvaluateShapeAtDistance(shape->pshape, t, &pos2, 0);
        pCur = FindPathzoneAtPos(pos2, 0);
        if (pCur) {
            int i;
            for (i = 0; i < cpv; i++)
                if (apv[i] == pCur)
                    break;
            if (i >= cpv && (unsigned)cpv < 64)
                apv[cpv++] = pCur;
        }
        outside = 0;
        if (!pCur || pPrev != pCur || !IsPosInPathzone(pPrev, pos2, 0))
            outside = 1;
        if (outside) {
            if (!pseg && cseg < 64) {
                pseg = &aseg[cseg++];
                pseg->minValue = tPrev;
            }
        } else if (pseg) {
            pseg->maxValue = t;
            pseg = 0;
        }
        tPrev = t;
        pPrev = pCur;
    }
    if (pseg)
        pseg->maxValue = tEnd;
    if (cseg >= 2 && shape->pshape->isClosed && aseg[0].minValue == tStart && aseg[cseg - 1].maxValue == tEnd) {
        aseg[0].minValue = aseg[cseg - 1].minValue - tEnd;
        aseg[cseg - 1].maxValue = aseg[0].maxValue + tEnd;
    }
    shape->crangeOutside = cseg;
    shape->arangeOutside = (__typeof__(shape->arangeOutside))PvReallocMsg((struct SWALLOC *)&pos2, cseg * sizeof(LM), 0x10, aseg);
    shape->crangeFree = cseg;
    CopyAb__FPvT0Ui(shape->arangeFree, aseg, cseg * 8);
    shape->cpathzone = cpv;
    shape->appathzone = (__typeof__(shape->appathzone))PvReallocMsg((struct SWALLOC *)&pos2, cpv * sizeof(void *), 0x10, apv);
    shape->pathzoneVersion = -1;
}
} // namespace M_func_001E9C88

namespace M_func_001E9F50 {
void *FindPathzoneAtPos(qword q, int n) __asm__("FindPathzoneAtPos");
int FindPathzoneObstacles(void *p, int n, qword q, qword q2, int c, void **ap) __asm__("FindPathzoneObstacles");
int IsPosInConvexObstacle(void *p, qword q) __asm__("IsPosInConvexObstacle");
// Finds the path zone at pos that passes the obstacle test (IsPosInConvexObstacle), or 0. (inferred) Also known as: PpolyAtPoint (upstream readable / Sly 1 name; PpolyAtPoint__FG9VU_VECTOR).
extern "C" void *FindValidObjectAtPos(qword pos)
{
    void *ap[0x40];
    void *p = FindPathzoneAtPos(pos, 0);
    int c, i;
    if (!p) return 0;
    c = FindPathzoneObstacles(p, 0, pos, pos, 0x40, ap);
    for (i = 0; i < c; i++) {
        void *pT = ap[i];
        if (IsPosInConvexObstacle(pT, pos)) return pT;
    }
    return 0;
}
} // namespace M_func_001E9F50

namespace M_func_001EA000 {
// BuildShapeValidRanges (lane 5): matched. Samples an object's path over [tStart, tEnd] and collects the time segments where
// FindValidObjectAtPos finds a valid position into ps->aseg10 (FindShapeFreeRange == 3 means "no bound found", so tStart/tEnd are
// used), intersects them with ps->aseg through MergeSegmentLists and fixes up a wrap-around pair at the end.
// Notes for whoever touches this again:
//  - `pseg` (the aseg10 pointer used by the wrap-around fix-up) must be assigned after the copy loops, outside the block
//    that uses it, so cse does not fold pseg[0] into 0x10(ps); gcse then keeps it as a separate spilled copy.
//  - `nIter` is a dead iteration counter. It only adds a few insns before gcse, which resizes gcse's expression hash
//    table (101 -> 103 buckets) and so renumbers the PRE reaching registers; that decides the order of the two spill
//    slots (0xC1C/0xC20) for &ax[0] and the aseg10 copy. Without it the two slots are swapped.
typedef int qword __attribute__((mode(TI)));
struct VEC { float x, y, z, w; } __attribute__((aligned(16)));
struct X { char pad[0x40]; float a; float b; char pad2[0x18]; } __attribute__((aligned(16)));
extern float D_002C367C __asm__("D_002C367C");
void EvaluateShapeAtDistance(SHAPE *, float, qword *, qword *) __asm__("EvaluateShapeAtDistance");
void *FindValidObjectAtPos(qword) __asm__("FindValidObjectAtPos");
int FindShapeFreeRange(void *, SHAPE *, float, int, qword *, float *) __asm__("FindShapeFreeRange");
void *func_002333B4(void *, int, unsigned int) __asm__("memset");
int MergeSegmentLists(int, struct X *, int, struct X *, int, struct X *, int) __asm__("MergeSegmentLists");

// Samples the shape over [t0, t1], collects the parameter ranges that are valid (walkable, not blocked) and
// intersects them with the existing ranges. (inferred) Also known as: UpdatePathrec (upstream readable / Sly 1 name; UpdatePathrec__FP7PATHREC).
extern "C" void BuildShapeValidRanges(SHAPERANGES *shape)
{
    struct VEC pos;
    struct X ax[16];
    struct X ay[16];
    float segStart, segEnd;
    LM *aseg;
    LM *asegSrc;
    int cseg, c, i, j, k, n, nIter = 0;
    LM *pseg;
    float tStart, t, tEnd;
    {
        int k;
        for (k = 0; k < shape->cpathzone; k++)
            if (shape->pathzoneVersion < shape->appathzone[k]->version)
                break;
        if (k >= shape->cpathzone)
            return;
    }
    aseg = shape->arangeFree;
    cseg = 0;
    tStart = GetShapeMinDistance(shape->pshape);
    t = tStart;
    tEnd = GetShapeMaxDistance(shape->pshape);
    asegSrc = shape->arangeOutside;
    c = shape->crangeOutside;
    while (t < tEnd) {
        void *pv;
        EvaluateShapeAtDistance(shape->pshape, t, (qword *)&pos, 0);
        pv = FindValidObjectAtPos(*(qword *)&pos);
        if (pv) {
            int fSave = shape->pshape->isClosed;
            LM *pseg;
            int fNew;
            int idx;
            shape->pshape->isClosed = 0;
            if (FindShapeFreeRange(pv, shape->pshape, t, 0, 0, &segStart) == 3)
                segStart = tStart;
            if (FindShapeFreeRange(pv, shape->pshape, t, 1, 0, &segEnd) == 3)
                segEnd = tEnd;
            shape->pshape->isClosed = fSave;
            pseg = &aseg[cseg];
            fNew = 1;
            while (aseg < pseg) {
                LM *previous = pseg - 1;
                if (previous->maxValue < segStart)
                    break;
                pseg = previous;
                if (pseg->minValue < segStart) {
                    fNew = 0;
                    break;
                }
            }
            idx = pseg - aseg;
            cseg = idx + 1;
            if (!((unsigned)cseg < 17))
                cseg = idx;
            if (fNew)
                pseg->minValue = segStart;
            pseg->maxValue = segEnd;
            t = segEnd;
        }
        nIter++;
        t += D_002C367C;
    }
    for (i = 0; i < c; i++) {
        func_002333B4(&ax[i], 0, 0x60);
        ax[i].a = asegSrc[i].minValue;
        ax[i].b = asegSrc[i].maxValue;
    }
    for (j = 0; j < cseg; j++) {
        func_002333B4(&ay[j], 0, 0x60);
        ay[j].a = aseg[j].minValue;
        ay[j].b = aseg[j].maxValue;
    }
    pseg = shape->arangeFree;
    n = MergeSegmentLists(cseg, ay, c, ax, 0x10, ay, 0);
    for (k = 0; k < n; k++) {
        aseg[k].minValue = ay[k].a;
        aseg[k].maxValue = ay[k].b;
    }
    if (n >= 2 && shape->pshape->isClosed) {
        // wrap-around: the first segment starts at tStart and the last ends at tEnd, so shift them across the seam
        if (pseg[0].minValue == tStart && pseg[n - 1].maxValue == tEnd) {
            pseg[0].minValue = pseg[n - 1].minValue - tEnd;
            pseg[n - 1].maxValue = pseg[0].maxValue + tEnd;
        }
    }
    shape->crangeFree = n;
    shape->pathzoneVersion = ++g_sw.pathzoneVersion;
}
} // namespace M_func_001EA000

namespace M_func_001EA3B0 {
// Returns the {min, max} range containing g, or 0. Also known as: FInPathSpan (upstream readable / Sly 1 name; FInPathSpan__FP7PATHRECf).
extern "C" struct LM *FindRangeContaining(SHAPERANGES *table, float value)
{
    int i;
    struct LM *alm = table->arangeFree;
    for (i = 0; i < table->crangeFree; i++) {
        struct LM *plm = &alm[i];
        if (plm->minValue <= value && value <= plm->maxValue) return plm;
    }
    return 0;
}
} // namespace M_func_001EA3B0

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
extern const float D_002A8130 = -3.40282347e+38f;
extern const float D_002A8134 = 3.40282347e+38f;
extern const float D_002A8138 = 3.40282347e+38f;
extern const float D_002A813C = 3.40282347e+38f;
int D_002C3650 = 1;
float D_002C3658[4] = {0.0f, 0.5f, 0.5f, 1.0f};
float D_002C3668[4] = {0.16666667f, 0.33333334f, 0.33333334f, 0.16666667f};
float D_002C3678 = 1.5f;
float D_002C367C = 50.0f;
