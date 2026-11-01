/* plane from 3 points; qmfc2+sw in one asm puts the d store first; normal copied via float temps (load x,y,z, store y,z,x) */
typedef int qword __attribute__((mode(TI)));
typedef struct { float x, y, z; } VECTOR;
typedef struct { VECTOR normal; float d; } PLANE;
typedef union { qword q; VECTOR v; } UV;
static inline qword Sub(qword a, qword b)
{
    qword r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword Cross(qword a, qword b)
{
    qword r;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword Dot3(qword a, qword b)
{
    qword d, one;
    __asm__("vmul.xyz %0, %2, %3
	vaddw.x %1, $vf0, $vf0w
	vadday.x $ACC, %0, %0y
	vmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    return d;
}
static inline qword Normalize(qword v)
{
    qword d, l, r;
    d = Dot3(v, v);
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l), "j"(v));
    return r;
}
/** @sly2-annot addr=0x119308
 * Purpose: Plane through 3 points: normal = normalize((b-a)x(c-a)), d = n.a
 * Info: proposed name InitPlaneFromPositions (T1); TU bsp
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#bsp-0x118668-0x1195e0
 * @end-sly2-annot */
void InitPlaneFromPositions(PLANE *pplane, qword a, qword b, qword c)
{
    UV u;
    qword d;
    int n;
    u.q = Normalize(Cross(Sub(b, a), Sub(c, a)));
    d = Dot3(u.q, a);
    __asm__("qmfc2.ni %0, %2\n\tsw %0, %1" : "=r"(n), "=m"(pplane->d) : "j"(d));
    { float tx = u.v.x, ty = u.v.y, tz = u.v.z; pplane->normal.y = ty; pplane->normal.z = tz; pplane->normal.x = tx; }
}
