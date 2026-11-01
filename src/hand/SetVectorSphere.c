typedef int qword __attribute__((mode(TI)));
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
void CalculateSinCos__FfPfT1(float g, float *ps, float *pc);
/** @sly2-annot addr=0x11E208
 * Purpose: Vector from spherical (pan, tilt, r)
 * Info: proposed name SetVectorSphere (T1); TU vec
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#vec-0x11e198-0x11f440
 * @end-sly2-annot */
qword SetVectorSphere(float radPan, float radTilt, float r)
{
    union { qword q; float f[4]; } v;
    float sp, cp, st, ct;
    CalculateSinCos__FfPfT1(radPan, &sp, &cp);
    CalculateSinCos__FfPfT1(radTilt, &st, &ct);
    v.q = Make(ct * cp * r, ct * sp * r, st * r);
    return v.q;
}
