/* EvaluateBlendChnPosVel (Claude agent, laneC29 2026-10-24): MATCH from v7 (4). ONE register pin (wb in $16); every other extra asm operand below is allocation-only and emits nothing: the "r"(i) operands stand in for the loop weighting of i, the extra "j" operands on the Madd asms raise the VF priorities of a/v/b, and the earlyclobber "=&j" with an unused "j"(a.q) input keeps s2 out of a's register. */
/* wip (Claude agent, 2026-10-17): aligned 28 (was 52). for-loop gives target s-reg numbering and a0 reuse at loop head; E.w read as int (lw->qmtc2); vadda operand order fixed. Left: loop.c hoists 0.0f (f22, frame 0x100) and iSeg*8 (s4) out of the loop; target keeps them inside. */
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } VU;
extern qword D_002A2DD0;
struct VTE { short d; short pad; void *pfn; };
struct O { struct VTE *vt; };
struct SEG { float g0; float g4; };
struct CL { char pad[0x74]; float x74; };
struct E { union { float w; int iw; } u; struct CL *p4; };
struct A { char pad[0x74]; float x74; char pad2[0xC0 - 0x78]; int xC0; int xC4; char *xC8; };
struct B { int x0; void *x4; struct SEG *x8; };
struct P { int pad; struct A *p4; struct B *x8; struct O **xC; qword x10; };
int func_00138370(void *, struct SEG *, float *, float, float);
static __inline__ qword ToVf(int g)
{
    qword s;
    __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(g));
    return s;
}
static __inline__ qword Madd2(qword a, qword v, qword s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a), "j"(v), "j"(s), "j"(v), "j"(v), "j"(a), "j"(a)); /* allocation-only (Claude agent): the 2nd "j"(v) pair and the "j"(a) pair are unused, they only raise v30's and b's VF allocation priority */
    return r;
}
static __inline__ qword Madd(qword a, qword v, qword s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a), "j"(v), "j"(s), "j"(a)); /* allocation-only (Claude agent): the 2nd "j"(a) is unused, it only raises a's VF allocation priority */
    return r;
}

/** @sly2-annot addr=0x138970
 * Purpose: Blended-channel actuator virtual (p, ppos, prot, t, rate): weighted sum of each bl channel's
 *   position/rotation at time t (MRSG-mapped)
 * Info: proposed name EvaluateBlendChnPosVel (T1); TU blend
 * Notes: superseded: EvaluateBlendChnPosRot (verdicts_followups) | verifier better_name (PLAUSIBLE row): Slot
 *   in a non-CID AC vtable (0x2CB5C4; neighbours GetAcConstTimes, EvaluateBlendChnPose/Scalar). Weighted sum
 *   over bl entries of child AC vt3(t', rate') outputs
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest+verdicts_followups
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/reference/functions/animation.md#blend-0x137a30-0x138f08
 * @end-sly2-annot */
void EvaluateBlendChnPosVel(struct P *p, qword *ppos, qword *prot, float t, float rate)
{
    float u;
    int iSeg;
    struct SEG *pseg;
    VU a, b;
    VU v20, v30, w;
    int i;
    struct A *pa;
    if (p->x8 == 0) {
        u = t / p->p4->x74;
    } else {
        float tLocal;
        iSeg = func_00138370(p->x8->x4, p->x8->x8, &tLocal, t, p->p4->x74);
        pseg = &p->x8->x8[iSeg];
        u = tLocal / pseg->g4;
    }
    i = 0;
    pa = p->p4;
    b.q = a.q = D_002A2DD0;
    do {
    if (pa->xC4 <= 0)
        goto done;
top:
    {
        int off = i * pa->xC0;
        char *base = pa->xC8;
        struct E *pe;
        struct O *po;
        float zero = 0.0f;
        pe = (struct E *)(base + off);
        if (pe->u.w == zero)
            goto next;
        po = p->xC[i];
        if (!po) {
            a.q = p->x10;
            b.f[2] = zero;
            b.f[1] = zero;
            b.f[0] = zero;
        } else {
            float g0, g1;
            if (!p->x8) {
                g1 = rate * pe->p4->x74 / pa->x74;
                g0 = u * pe->p4->x74;
            } else {
                int is = iSeg;
                struct SEG *ps;
                float gMax;
                ps = (struct SEG *)((char *)pe + p->x8->x0) + is;
                gMax = pe->p4->x74;
                g0 = ps->g0 + u * ps->g4;
                g1 = rate * ps->g4 / pseg->g4;
                if (gMax < g0)
                    g0 -= gMax;
            }
            {
                struct VTE *e = &po->vt[3];
                ((void (*)(void *, float, float, VU *, VU *))e->pfn)((char *)po + e->d, g0, g1, &v20, &v30);
            }
        }
        {
            qword s1, s2;
            s1 = ToVf(pe->u.iw);
            a.q = Madd(a.q, v20.q, s1);
            { register int wb asm("$16") = pe->u.iw; /* register pin (Claude agent): target reloads pe->w into pe's dying register s0 */ __asm__("qmtc2.ni %1, %0" : "=&j"(s2) : "r"(wb), "r"(i), "r"(i), "r"(i), "j"(a.q)); /* the three "r"(i) operands are allocation-only (Claude agent): they stand in for the loop weighting of i's refs, so i gets s1 before p; the "&" earlyclobber and the unused "j"(a.q) input are allocation-only too (they keep s2 out of a's VF register) */ }
            w.q = s1;
            b.q = Madd2(b.q, v30.q, s2);
            w.q = s2;
        }
    }
next:
    pa = p->p4;
    if (++i < pa->xC4)
        goto top;
    } while (0);
done:
    if (ppos)
        *ppos = a.q;
    if (prot)
        *prot = b.q;
}
