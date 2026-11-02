// Recompute the bounds of an object from its list of child objects: run the due children's update, accumulate the
// min/max of their two corner vectors, optionally grow by the object's size, then derive the center and radius and notify.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct DLI { DL *pdl; void **ppv; int ibDle; struct DLI *pdliNext; } DLI;
typedef struct VT { struct { short d; short pad; void (*pfn)(void *); } a[16]; } VT;
typedef struct S {
    char pad0[0x14]; float time; char pad18[0x70 - 0x18]; qword cornerA; qword cornerB; char pad90[0xEC - 0x90];
    VT *vt; char padF0[0x154 - 0xF0]; float size; int pad158; DL children; char pad168[8];
    union U lo; union U hi; union U center; float radius;
} S;
extern DLI *D_002DD2FC;
extern struct { float t; } D_002DDED0;
extern const float D_002A77D0, D_002A77D4;
void func_001F4B58(S *);
static inline void InitDli(DLI *pdli, DL *pdl)
{
    pdli->pdl = pdl;
    pdli->ppv = (void **)pdl;
    pdli->ibDle = pdl->ibDle;
    pdli->pdliNext = D_002DD2FC;
    D_002DD2FC = pdli;
}
static inline void *PvNextDli(DLI *pdli)
{
    void *pv = *pdli->ppv;
    pdli->ppv = (void **)((char *)pv + pdli->ibDle);
    return pv;
}
static inline qword Min(qword a, qword b) { qword r; __asm__("vmini.xyz %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Max(qword a, qword b) { qword r; __asm__("vmax.xyz %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
// (a + b) * k.x.  The second "j"(k) operand is allocation-only: it gives k the register priority the original had.
static inline qword Mid(qword a, qword b, qword k) { qword r; __asm__("vmulax.xyzw $ACC,%1,%3x\n\tvmaddx.xyzw %0,%2,%4x" : "=j"(r) : "j"(a), "j"(b), "j"(k), "j"(k)); return r; }
// Length of a vector in two steps (sum of squares, then sqrt).  The "memory" clobbers keep the stores around the steps in source order.
#define LEN1(d, one, v) __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "j"(v) : "memory")
#define LEN2(d) __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "+j"(d) : : "memory")
/** @sly2-annot addr=0x1CDF88
 * Purpose: RIPG vt slot 17: iterate rips, run due ones' project step, accumulate min/max bounds, grow by
 *   size, set center/radius
 * Info: proposed name ProjectRipgTransform (T1); class RIPG slot17; TU rip
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r5_1AE130+verdicts_A2
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void ProjectRipgTransform(S *p)
{
    DLI dli;
    union U lo, hi, t, half;
    // y/z of each bound are written through pointers (the original did too: it changes how the DLI fields are reloaded)
    union U *plo = &lo, *phi = &hi;
    S *q;
    InitDli(&dli, &p->children);
    plo->f[1] = plo->f[2] = D_002A77D0;
    lo.f[0] = D_002A77D0;
    phi->f[1] = phi->f[2] = D_002A77D4;
    hi.f[0] = D_002A77D4;
    while ((q = (S *)PvNextDli(&dli)) != 0) {
        if (D_002DDED0.t >= q->time)
            q->vt->a[4].pfn((char *)q + q->vt->a[4].d);
        lo.q = Min(lo.q, Min(q->cornerA, q->cornerB));
        hi.q = Max(hi.q, Max(q->cornerA, q->cornerB));
    }
    if (p->size != 0) {
        t.f[0] = t.f[1] = t.f[2] = p->size;
        lo.q = Sub(lo.q, t.q);
        hi.q = Sub(hi.q, t.q);
    }
    if (p->vt) {
        float h = 0.5f;
        int r;
        qword k, mid, d, one, lv = lo.q, hv = hi.q;
        __asm__("qmtc2.ni %1,%0" : "=j"(k) : "r"(h));
        mid = Mid(lv, hv, k);
        half.q = k;
        LEN1(d, one, Sub(mid, hv));
        p->lo.q = lv;
        LEN2(d);
        t.q = mid;
        __asm__("qmfc2.ni %0,%1" : "=r"(r) : "j"(d) : "memory");
        p->hi.q = hv;
        *(int *)&p->radius = r;
        p->center.q = mid;
        func_001F4B58(p);
    }
    D_002DD2FC = dli.pdliNext;
}
