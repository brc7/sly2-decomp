typedef int qword __attribute__((mode(TI)));
/** @sly2-annot addr=0x11E290
 * Purpose: Scalar projection of a on b; optionally returns parallel and perpendicular components
 * Info: proposed name SProjectVector (T1); TU vec
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#vec-0x11e198-0x11f440
 * @end-sly2-annot */
float func_0011E290(qword a, qword b, qword *ppar, qword *pperp)
{
    qword d, one, p;
    float g;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(p) : "j"(b), "j"(d));
    if (ppar) {
        __asm__("sq %1, %0" : "=m"(*ppar) : "r"(p));
    }
    if (pperp) {
        qword r;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(p));
        *pperp = r;
    }
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(d));
    return g;
}
