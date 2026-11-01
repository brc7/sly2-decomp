// FindClosestPointOnLineSegment: closest point on segment p0-p1 to pos (Claude agent). The dead stack copy of the t vector is a
// volatile sqc2 asm (keeps it ahead of beqz); the ppos copy goes vf->gpr via a gcc reload (empty asm with "0") so
// reorg can steal its qmfc2 into the delay slot.
typedef int qword __attribute__((mode(TI)));
static inline qword Sub(qword a, qword b)
{
    qword r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline float Dot(qword a, qword b)
{
    qword d, one;
    float g;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(d));
    return g;
}
static inline qword DotV(qword a, qword b)
{
    qword d, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    return d;
}
static inline float ToF(qword d)
{
    float g;
    register int r __asm__("v0");
    __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(r), "=f"(g) : "j"(d));
    return g;
}
static inline float GClamp(float g, float lo, float hi)
{
    if (g < lo) g = lo;
    else if (g > hi) g = hi;
    return g;
}
void FindClosestPointOnLineSegment(qword pos, qword p0, qword p1, qword *ppos, float *pt, float *pdist)
{
    qword d = Sub(p1, p0);
    qword e0 = Sub(pos, p0);
    qword vd = DotV(d, d);
    qword vn = DotV(e0, d);
    float t = GClamp(ToF(vn) / ToF(vd), 0.0f, 1.0f);
    if (pt)
        *pt = t;
    if (ppos || pdist) {
        qword s, buf;
        qword c, q;
        __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(t));
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(c) : "j"(p0), "j"(d), "j"(s));
        __asm__ __volatile__("sqc2 %1, %0" : "=m"(buf) : "j"(s));
        if (ppos) {
            __asm__("" : "=r"(q) : "0"(c)); /* emits nothing: forces gcc movti vf->gpr (qmfc2.ni) */
            *ppos = q;
        }
        if (pdist) {
            qword e, one, l;
            int g;
            e = Sub(pos, c);
            __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(e), "=j"(one) : "0"(e));
            __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(e));
            __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
            *(int *)pdist = g;
        }
    }
}
