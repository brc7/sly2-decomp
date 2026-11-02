// Cache eleven Hermite samples of one path segment (control points transformed by the path matrix)
// and the cumulative lengths between them.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f; };
struct M4 { qword a, b, c, d; };
struct PATH {
    char pad[0xEC];
    float *times;       // 0xEC
    float *dist;        // 0xF0
    qword *points;      // 0xF4
    char padF8[8];
    struct M4 mat;      // 0x100
    qword *in;          // 0x140, incoming tangents
    qword *out;         // 0x144, outgoing tangents
    int segment;        // 0x148, segment currently cached
    float lengths[11];  // 0x14C
};
void EvaluateBezierPos(float duration, float t, float scale, qword start, qword startTangent, qword end, qword endTangent,
                   qword *pos, qword *vel, qword *acc);

static inline qword Dir(qword m0, qword m1, qword m2, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
            : "=j"(r) : "j"(m0), "j"(m1), "j"(m2), "j"(v));
    return r;
}
static inline qword Point(qword m0, qword m1, qword m2, qword m3, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w"
            : "=j"(r) : "j"(m0), "j"(m1), "j"(m2), "j"(m3), "j"(v));
    return r;
}
static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline float Len(qword a)
{
    qword one;
    float f;
    __asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "+j"(a), "=&j"(one));
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "+j"(a));
    __asm__("qmfc2.ni %0,%1" : "=r"(f) : "j"(a));
    return f;
}

/** @sly2-annot addr=0x1E5FC8
 * Purpose: Cache 11 Hermite samples of curve segment i (transformed by the shape matrix) and their cumulative
 *   lengths (+0x14C), segment id +0x148
 * Info: proposed name CacheShapeSegment (T1); TU shape
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void CacheShapeSegment(struct PATH *p, int i)
{
    union U prev, cur;
    // Plain qwords: they live across the call in the loop, so reload spills them to 0x20..0x50 and
    // loads them straight into the argument registers.
    qword start, end, a, b;
    int j;
    float duration;
    if (i == p->segment)
        return;
    {
        struct M4 *m = &p->mat;
        qword v0 = p->points[i], v1 = p->points[i + 1], o0 = p->out[i], i1 = p->in[i + 1];
        qword m1 = m->b, m2 = m->c, m3 = m->d, m0 = p->mat.a;
        p->lengths[0] = 0;
        // Computed in argument order; the scheduler then emits them as b, end, start, a.
        start = Point(m0, m1, m2, m3, v0);
        a = Dir(m0, m1, m2, o0);
        end = Point(m0, m1, m2, m3, v1);
        b = Dir(m0, m1, m2, i1);
    }
    duration = p->times[i + 1] - p->times[i];
    for (j = 0; j < 11; j++) {
        float frac = (float)j * 0.1f;
        EvaluateBezierPos(duration, frac * duration, 1.0f, start, a, end, b, &cur.q, 0, 0);
        if (j > 0)
            p->lengths[j] = p->lengths[j - 1] + Len(Sub(prev.q, cur.q));
        prev.q = cur.q;
    }
    p->segment = i;
}
