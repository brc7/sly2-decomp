/* func_0011EA60 (Claude agent, lane A28, 2026-10-24): from fresh9 (13). ba via a vsub without the "memory" clobber (qmtc2 b order), and every dot readback via F3, a mtc1 asm with an allocation-only "j"(q) input next to the "r"(q) one, so the dot result stays a VU pseudo and gcc emits the qmfc2 itself. No pins. */
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
static inline qword Sub(qword a, qword b)
{
    qword r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b) : "memory");
    return r;
}
static inline qword SubN(qword a, qword b)
{
    qword r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword Cross(qword a, qword b)
{
    qword r;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b) : "memory");
    return r;
}
static inline qword CrossT(qword a, qword b)
{
    __asm__("vopmula.xyz $ACC, %0, %1\n\tvopmsub.xyz %0, %1, %0" : "+j"(a) : "j"(b) : "memory");
    return a;
}
static inline qword DotQ(qword a, qword b)
{
    qword d, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b) : "memory");
    return d;
}
static inline qword DotN(qword a, qword b)
{
    qword d, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    return d;
}
static inline float Dot(qword a, qword b)
{
    int n;
    float g;
    __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(n), "=f"(g) : "j"(DotQ(a, b)) : "memory");
    return g;
}
static inline float F3(qword q)
{
    float g;
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q), "j"(q)); /* "j"(q): allocation-only operand */
    return g;
}
static inline float DotS(qword a, qword b)
{
    return F3(DotQ(a, b));
}
/** @sly2-annot addr=0x11EA60
 * Purpose: Vector geometry helper (edge cross products and dot readbacks against a triangle/segment)
 * Info: proposed name FClipLineToTriangle (T2); TU vec
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#vec-0x11e198-0x11f440
 * @end-sly2-annot */
int func_0011EA60(qword a, qword b, qword c, qword d, float *puv, qword *pnormal)
{
    qword ca = Sub(c, a);
    qword ba = SubN(b, a);
    qword dc = Sub(d, c);
    qword n = Cross(ba, dc);
    qword ca2, n2, c1, lq, c2;
    float lenSq;
    float g, s;
    __asm__("vmove.xyzw %0, %1" : "=j"(ca2) : "j"(ca) : "memory");
    __asm__("vmove.xyzw %0, %1" : "=j"(n2) : "j"(n), "j"(ca) : "memory"); /* "j"(ca): allocation-only operand */
    c1 = Cross(ca2, ba);
    lq = DotQ(n2, n2);
    __asm__("vopmula.xyz $ACC, %0, %2\n\tvopmsub.xyz %0, %2, %0" : "=j"(c2) : "0"(ca2), "j"(dc) : "memory");
    {
        int k;
        __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(k), "=f"(lenSq) : "j"(lq) : "memory");
    }
    if (lenSq < 1e-6f)
        return 0;
    if (puv) {
        qword du = DotN(c2, n2);
        qword dv = DotN(c1, n2);
        puv[0] = F3(du) / lenSq;
        puv[1] = F3(dv) / lenSq;
    }
    g = DotS(ca, n2);
    s = 1.0f;
    if (g < 0.0f)
        s = -1.0f;
    if (pnormal) {
        float l;
        U t;
        qword r;
        __asm__("sqrt.s %0, %1" : "=&f"(l) : "f"(lenSq));
        __asm__("qmtc2.ni %1, %0" : "=j"(t.q) : "r"(s / l));
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(n), "j"(t.q));
        *pnormal = r;
    }
    return 1;
}
