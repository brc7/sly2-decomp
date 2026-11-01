typedef int qword __attribute__((mode(TI)));
typedef struct M4 { qword r[4]; } M4;
typedef struct P { char pad[0xA0]; struct P *xA0; } P;
void *func_002333B4(void *, int, int);
void func_00230710(P *, M4 *, M4 *, float);
M4 __ml__FRC7MATRIX4T0(M4 *, M4 *);

static inline void AddM4(M4 *d, M4 *a, M4 *b)
{
    int i;
    for (i = 0; i < 4; i++) {
        qword r;
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a->r[i]), "j"(b->r[i]));
        d->r[i] = r;
    }
}

/** @sly2-annot addr=0x230A98
 * Purpose: adds rows then 0x230710
 * Info: TU xf
 * Notes: descriptive only: xf: blend matrices | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r8_21BB88
 * @end-sly2-annot */
void func_00230A98(P *first, P *end, M4 *pA, M4 *pB, float f)
{
    M4 m1, m2, a, b, s, t1, t2;
    P *p;

    if (pB)
        b = *pB;
    else
        func_002333B4(&b, 0, 0x40);
    if (pA)
        a = *pA;
    else {
        float *pf = (float *)&a;
        func_002333B4(&a, 0, 0x40);
        pf[0] = pf[5] = pf[10] = pf[15] = 1.0f;
    }
    for (p = first; p != end; p = p->xA0) {
        func_00230710(p, &m1, &m2, f);
        t1 = __ml__FRC7MATRIX4T0(&m1, &b);
        t2 = __ml__FRC7MATRIX4T0(&m2, &a);
        AddM4(&s, &t1, &t2);
        b = s;
        s = __ml__FRC7MATRIX4T0(&m1, &a);
        a = s;
    }
    if (pA)
        *pA = a;
    if (pB)
        *pB = b;
}
