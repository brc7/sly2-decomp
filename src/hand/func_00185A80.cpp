// func_00185A80 (Claude agent, lane G16, 2026-10-24): MATCH, no register pins, VU helper asms only.
// From laneG13v1 (12). Keys: (1) loop 2: the plane.w read before a qmfc2 helper with a "memory" clobber, then an
// mtc1 helper with allocation-only "f"(w) operands (12 -> 9). (2) loop 3 as a fresh block-scoped `int k`/`int m`
// with the weights read through Lilly's VuFloatFromMemWord ("rV": reload makes the lw) (9 -> 1). (3) an early
// `if (n == 0) return v;`: the two returns' qmfc2 moves are cross-jumped into one block before the epilogue, so the
// qmfc2 is no longer scheduled after `lq s0` (1 -> 0).
typedef int qword __attribute__((mode(TI)));
struct P { int count; int pad[3]; qword planes[6]; };
extern "C" {
extern float D_002A568C;
void SolveInequalities(int n, float *matrix, float *rhs, float *weights, float eps);
}
static inline qword DotQ(qword a, qword b)
{
    qword d, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
        : "=j"(d), "=j"(one) : "j"(a), "j"(b) : "memory");
    return d;
}
struct VU_FLOAT { qword q; };
static inline VU_FLOAT VuFloatFromMemWord(int n) { VU_FLOAT v; __asm__("qmtc2.ni %1, %0" : "=j"(v.q) : "rV"(n)); return v; }
static inline qword AddScaled(qword a, qword b, VU_FLOAT s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a), "j"(b), "j"(s.q));
    return r;
}
static inline qword *Addr(qword *q) { return q; }
static inline int Bits(qword d)
{
    int r;
    __asm__("qmfc2.ni %0, %1" : "=r"(r) : "j"(d));
    return r;
}
static inline int BitsM(qword d)
{
    int r;
    /* "memory": keeps the plane.w lwc1 before the qmfc2 (target: qmfc2 one cycle later, addiu before the mtc1). */
    __asm__("qmfc2.ni %0, %1" : "=r"(r) : "j"(d) : "memory");
    return r;
}
/* mtc1 from the qmfc2 bits; the three "f"(w) operands are allocation-only (emit nothing): extra refs raise w's
   local-alloc priority so w gets f0 and x f1 like the target. */
static inline float FloatFromBitsW(int b, float w)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(b), "f"(w), "f"(w), "f"(w));
    return x;
}
extern "C" qword func_00185A80(void *unused, P *p, qword v)
{
    int matrix[36];
    float rhs[8];
    int weights[8];
    int i, j, n;
    int *pm;
    n = p->count;
    if (n == 0)
        return v;
    {
        pm = matrix;
        __asm__("" : "+r"(pm)); /* empty, allocation-only: unknown alias base so the count reloads follow the matrix stores */
        for (i = 0; i < n; i++) {
            for (j = 0; j < p->count; j++)
                *pm++ = Bits(DotQ(*Addr(&p->planes[i]), p->planes[j]));
            n = p->count;
        }
        pm = (int *)rhs;
        for (i = 0; i < p->count; i++) {
            qword *pl = Addr(&p->planes[i]);
            qword point;
            __asm__("" : "=j"(point) : "0"(v)); /* empty: a VF copy of v (loop.c hoists it: the target's vmove) */
            qword d = DotQ(point, *pl);
            float w = ((float *)pl)[3];
            *(float *)pm++ = w + FloatFromBitsW(BitsM(d), w);
        }
        SolveInequalities(p->count, (float *)matrix, rhs, (float *)weights, D_002A568C);
        int k = p->count;
        for (int m = 0; m < k; m++)
            v = AddScaled(v, p->planes[m], VuFloatFromMemWord(weights[m]));
    }
    return v;
}
