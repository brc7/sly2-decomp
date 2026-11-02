// FindShapeClosestPoint (w2lane4, wave 2): closest point of a path (a chain of control points, transformed by the path's
// own matrix) to a query point. mode 0 walks every segment, projects the point on it (clamped to the segment), keeps the
// nearest candidate and reports its position, direction, interpolated time and the path value at that time. mode 1 only
// measures the distances to the control points and hands the nearest vertex time to FindShapeClosestPointFromU.
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
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
struct M4 { qword a, b, c, d; };
struct VTE { short delta, pad; void *fn; };
struct SPACE;
struct PATH {
    char padC[0xC]; VTE *vt; char pad10[0xD0];
    int mode, cyclic, count; float *times, *values; qword *points;
};
extern "C" {
extern float D_002A8138;
qword ApplyConstr_1FA970(SPACE *, qword);
float SFromShapeU(PATH *, float);
void FindShapeClosestPointFromU(PATH *, qword, SPACE *, qword *, qword *, float *, float *, float);
}
struct VU_FLOAT {
    qword q;
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1,%0" : "=j"(q) : "r"(f)); }
};
static inline VV TransformPoint(const M4 &m, VV v)
{
    qword r;
    __asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w"
            : "=j"(r) : "j"(m.a), "j"(m.b), "j"(m.c), "j"(m.d), "j"(v));
    return r;
}
static inline VV Sub(VV a, VV b)
{
    qword r;
    __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline VV VecCombo(VU_FLOAT f1, VV v1, VU_FLOAT f2, VV v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1), "j"(f2.q), "j"(v2));
    return r;
}
static inline VV Dot(VV a, VV b)
{
    qword r, one;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "=j"(r), "=j"(one) : "j"(a), "j"(b));
    return r;
}
static inline int Bits(qword v) { int b; __asm__("qmfc2.ni %0,%1" : "=r"(b) : "j"(v)); return b; }
static inline float FromBits(int b) { float f; __asm__("mtc1 %1,%0" : "=f"(f) : "r"(b)); return f; }
static inline float ToFloat(qword v) { return FromBits(Bits(v)); }
static inline VV Root(VV a)
{
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "+j"(a));
    return a;
}
static inline float ToFloatR(qword v) { float f; __asm__("mtc1 %1,%0" : "=f"(f) : "r"(v)); return f; }
static inline float Length(VV a) { return ToFloatR(Root(Dot(a, a))); }
static inline VV Normalize(VV a)
{
    qword length = Root(Dot(a, a));
    __asm__("vdiv $Q,$vf0w,%0x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q" : "+j"(length) : "j"(a));
    return length;
}
static inline float Clamp(float g, float lo, float hi)
{
    if (g < lo) g = lo;
    else if (g > hi) g = hi;
    return g;
}
/** @sly2-annot addr=0x1E6F60
 * Purpose: Global closest point on the shape to a position (segment scan, or nearest vertex then refine);
 *   outputs pos, dir, u, s
 * Info: proposed name FindShapeClosestPoint (T1); TU shape
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
extern "C" void FindShapeClosestPoint(PATH *p, qword point, SPACE *space, qword *outPosition, qword *outDirection,
                              float *outTime, float *outValue)
{
    M4 matrix;
    U closest;
    qword start, end;
    VTE &v = p->vt[31];
    matrix = *((M4 *(*)(void *))v.fn)((char *)p + v.delta);
    switch (p->mode) {
    case 0: {
        start = TransformPoint(matrix, p->points[0]);
        float best = D_002A8138, time = -1.0f;
        for (int index = 1; index < p->count; index++) {
            // allocation-only: an in-place use/set of `time` makes global-alloc rank it ahead of `best`
            // (time gets f22, best f23); it emits no instruction.
            __asm__("" : "+f"(time));
            qword direction = Sub(end = TransformPoint(matrix, p->points[index]), start);
            qword offset = Sub(point, start);
            if (space) {
                direction = ApplyConstr_1FA970(space, direction);
                offset = ApplyConstr_1FA970(space, offset);
            }
            float t = ToFloat(Dot(offset, direction)) / ToFloat(Dot(direction, direction));
            t = Clamp(t, 0.0f, 1.0f);
            float u = 1.0f - t;
            closest.q = VecCombo(VU_FLOAT(u), start, VU_FLOAT(t), end);
            qword delta = Sub(point, closest.q);
            if (space) delta = ApplyConstr_1FA970(space, delta);
            float distance = Length(delta);
            if (distance < best) {
                best = distance;
                time = u * p->times[index - 1] + t * p->times[index];
                if (outPosition) *outPosition = closest.q;
                if (outDirection) *outDirection = Normalize(Sub(end, start));
            }
            start = end;
        }
        if (outTime) *outTime = time;
        if (outValue) *outValue = SFromShapeU(p, time);
        break;
    }
    case 1: {
        float best = D_002A8138;
        int closestIndex = -1;
        for (int index = 0; index < p->count; index++) {
            qword displacement = Sub(point, TransformPoint(matrix, p->points[index]));
            if (space) displacement = ApplyConstr_1FA970(space, displacement);
            float distance = Length(displacement);
            if (distance < best) { best = distance; closestIndex = index; }
        }
        FindShapeClosestPointFromU(p, point, space, outPosition, outDirection, outTime, outValue, p->times[closestIndex]);
        break;
    }
    }
}
