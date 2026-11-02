// FindClosestPointOnLine (Claude agent, lane A22, 2026-10-23): matched from build/hand_wip/FindClosestPointOnLine.v4.c.
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
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
    __asm__("vmul.xyz %0, %0, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "0"(a), "j"(b));
    /* "$4" clobber: a0 (pos's argument register) is dead here; scheduling only, it gives pos's qmtc2 one more dependent so it wins the tie */
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(d) : "$4");
    return g;
}
/** @sly2-annot addr=0x11ED40
 * Purpose: Closest point to pos on the infinite line through p0,p1 (t unclamped); optional point, t, distance
 * Info: proposed name FindClosestPointOnLine (T1); TU vec
 * Notes: v3: superseded ClosestPointOnSegment | top50_round (0.65): [extra: knock-on fix] FIX of current
 *   ClosestPointOnSegment: body has no clamp (0x11EE08 is the clamped segment version) | v2 draft was
 *   ClosestPointOnSegment
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: r1_116918+top50_round
 * See: docs/reference/functions/core-library.md#vec-0x11e198-0x11f440
 * @end-sly2-annot */
void FindClosestPointOnLine(qword pos, qword p0, qword p1, qword *ppos, float *pt, float *pdist)
{
    register qword p0r __asm__("vf5") = p0; /* the one register pin: p0 copy shared by both Subs and the vadda */
    qword d = Sub(p1, p0r);
    qword d2;
    float n, t;
    __asm__("" : "=j"(d2) : "0"(d)); /* empty asm: the d copy (vmove) both dots read */
    __asm__("" : "+j"(pos)); /* empty asm, scheduling only: raises the priority of pos's qmtc2 to tie with the d vsub */
    n = Dot(Sub(pos, p0r), d2);
    t = n / Dot(d2, d2);
    if (pt)
        *pt = t;
    if (ppos || pdist) {
        qword s, buf;
        qword c, q;
        __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(t));
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(c) : "j"(p0r), "j"(d), "j"(s));
        __asm__ __volatile__("sqc2 %1, %0" : "=m"(buf) : "j"(s)); /* dead spill of s, as in the target */
        if (ppos) {
            __asm__("" : "=r"(q) : "0"(c)); /* empty asm: qmfc2 c into a GPR for the store */
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
