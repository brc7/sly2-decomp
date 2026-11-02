typedef int qword __attribute__((mode(TI)));
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
void CalculateSinCos__FfPfT1(float g, float *ps, float *pc);
/** @sly2-annot addr=0x11E1A0
 * Purpose: Vector from cylindrical (rad, r, z) via CalculateSinCos, returned by value
 * Info: proposed name SetVectorCylind (T1); TU vec
 * Notes: names_v2 v1 proposed ConvertUserRgbToUserHsv__FP6VECTORT0 (0.000)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8,
 *   docs/reference/functions/core-library.md#vec-0x11e198-0x11f440
 * @end-sly2-annot */
qword SetVectorCylind(float rad, float r, float z)
{
    union { qword q; float f[4]; } v; float gA, gB; CalculateSinCos__FfPfT1(rad, &gA, &gB); v.q = Make(gB * r, gA * r, z); return v.q;
}
