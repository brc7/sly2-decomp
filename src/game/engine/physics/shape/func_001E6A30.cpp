// func_001E6A30 (w2lane4, wave 2): closest point on a path (chain of control points under the path's own matrix) to a query point,
// searched from a starting time. IsegFindShapeU turns the starting time into a segment index; the loop projects the point on that
// segment (segment vector rotated by the matrix, point minus the transformed start) and walks to the next / previous segment (wrapping
// when the path is cyclic) until the projection falls inside a segment. Afterwards the position, direction, time and value outputs
// are produced from the final segment and fraction.
// Matching notes (found with the -da dumps; the dumps explain every non-obvious choice):
//  * `fraction` is reused for the final time (`fraction = (1-fraction)*t0 + fraction*t1`), which is what puts it in $f12 (the argument
//    register of SFromShapeU) without any pin.
//  * `basis=&matrix` is declared inside the loop (and again in the direction block): loop.c hoists it to the loop preheader, which
//    is where the target has it (after the matrix copy, in the delay slot of the jump into the loop) and what makes it outrank `space`
//    in global allocation (space $s4, basis $s3). Rows 1 and 2 are read through `basis`, row 0 and the translation row directly.
//  * `previous = index; continue;` is written in both branches (each gets its own delay-slot copy), and `backward = 1` / `forward = 1`
//    are set after the if/else of their branch.
//  * `segment` is a memory union (it is stored and reloaded around the ApplyConstr_1FA970 calls); `NormalizeTo` takes it by value, which
//    is the unread temp copy stored to the temp slot (sp+0x50, shared with the VU_FLOAT temps of the position block).
//  * Dot helpers list their input once and `LengthOf` lists the operand once ("j"(v.q) with %2 used twice): two operand
//    references would make the loaded segment outrank the length in local allocation and swap vf1/vf2.
//  * The size and the projection are converted with the single-asm `mtc1 %1,%0 : "r"(vu)` form (ToFloatR): the qmfc2 is then a
//    reload move scheduled before the in-place normalize, and the distance bits land in $a0 instead of $v0.
//  * Normalize output is tied to the length operand ("+j"), DotInto ties the result to the second operand (the point delta).
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
struct M4 { U rows[4]; };
struct VTE { short delta, pad; void *fn; };
struct SPACE;
struct PATH {
    char padC[0xC]; VTE *vt; char pad10[0xD4];
    int cyclic, count; float *times; int padF0; qword *points;
};
extern "C" {
int IsegFindShapeU(PATH *, float, float *, float *);
qword ApplyConstr_1FA970(SPACE *, qword);
float SFromShapeU(PATH *, float);
}
struct VU_FLOAT {
    qword q;
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1,%0" : "=j"(q) : "r"(f)); }
};
static inline VV TransformPoint(const M4 &m, VV v)
{
    qword r;
    __asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w"
            : "=j"(r) : "j"(m.rows[0].q), "j"(m.rows[1].q), "j"(m.rows[2].q), "j"(m.rows[3].q), "j"(v));
    return r;
}
static inline VV Rotate(const M4 &m, const M4 *b, VV v)
{
    qword r = v;
    __asm__("vmulax.xyz $ACC,%1,%0x\n\tvmadday.xyz $ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z"
            : "+j"(r) : "j"(m.rows[0].q), "j"(b->rows[1].q), "j"(b->rows[2].q));
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
static inline VV DotInto(VV a, VV b)
{
    qword one;
    __asm__("vmul.xyz %0,%2,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "+j"(b), "=j"(one) : "j"(a));
    return b;
}
static inline VV Root(VV a)
{
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "+j"(a));
    return a;
}
static inline float ToFloatR(qword v) { float f; __asm__("mtc1 %1,%0" : "=f"(f) : "r"(v)); return f; }
static inline VV LengthOf(const U &v)
{
    qword d, one;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "=j"(d), "=j"(one) : "j"(v.q));
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "+j"(d));
    return d;
}
static inline VV NormalizeTo(U a, VV length)
{
    qword r = length;
    __asm__("vdiv $Q,$vf0w,%0x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q" : "+j"(r) : "j"(a.q));
    return r;
}
static inline VV Normalize(VV a)
{
    qword length = Root(Dot(a, a));
    __asm__("vdiv $Q,$vf0w,%0x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q" : "+j"(length) : "j"(a));
    return length;
}
/** @sly2-annot addr=0x1E6A30
 * Purpose: Closest point on a linear shape, walking segments from a start parameter
 * Info: proposed name FindShapeClosestPointLinearFromU (T2); TU shape
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
extern "C" void func_001E6A30(PATH *p, qword point, SPACE *space, qword *outPosition, qword *outDirection,
                              float *outTime, float *outValue, float initial)
{
    U segment;
    M4 matrix;
    float elapsed, duration;
    int backward = 0, forward = 0, previous;
    int index = IsegFindShapeU(p, initial, &elapsed, &duration);
    matrix = *((M4 *(*)(void *))p->vt[31].fn)((char *)p + p->vt[31].delta);
    float fraction;
    for (;;) {
        const M4 *basis = &matrix;
        qword start = p->points[index], end = p->points[index + 1];
        segment.q = Rotate(matrix, basis, Sub(end, start));
        qword delta = Sub(point, TransformPoint(matrix, start));
        if (space) {
            delta = ApplyConstr_1FA970(space, delta);
            segment.q = ApplyConstr_1FA970(space, segment.q);
        }
        qword length = LengthOf(segment);
        float size = ToFloatR(length);
        segment.q = NormalizeTo(segment, length);
        float distance = ToFloatR(DotInto(segment.q, delta));
        fraction = 0.0f;
        if (distance < 0.0f) {
            if (forward) { fraction = 1.0f; index = previous; break; }
            if (index > 0) --index;
            else {
                if (!p->cyclic) { fraction = 0.0f; break; }
                index = p->count - 2;
            }
            backward = 1;
            previous = index;
            continue;
        } else if (size < distance) {
            if (backward) { fraction = 0.0f; index = previous; break; }
            if (index < p->count - 2) ++index;
            else {
                if (!p->cyclic) { fraction = 1.0f; break; }
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
        *outPosition = TransformPoint(matrix, VecCombo(VU_FLOAT(1.0f - fraction), p->points[index],
                                                       VU_FLOAT(fraction), p->points[index + 1]));
    }
    if (outDirection) {
        const M4 *basis = &matrix;
        qword tangent = Rotate(matrix, basis, Sub(p->points[index + 1], p->points[index]));
        *outDirection = Normalize(tangent);
    }
    fraction = (1.0f - fraction) * p->times[index] + fraction * p->times[index + 1];
    if (outTime) *outTime = fraction;
    if (outValue) *outValue = SFromShapeU(p, fraction);
}
