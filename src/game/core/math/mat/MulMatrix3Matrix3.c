// Claude agent, lane A19, 2026-10-23: matched from fresh7 (3). One pin ($5), one commented empty asm, allocation-only asm operands.
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
void func_0023320C(void *pvDst, void *pvSrc, int cb);
static inline M3 MulM3(M3 *pa, M3 *pb, M3 *pd)
{
    M3 m;
    qword v4;
    qword v7;
    qword v8;
    __asm__(
        "lqc2 $vf6, 0x20(%4)\n\t"
        "lqc2 $vf5, 0x20(%3)\n\t"
        "lqc2 $vf3, 0x10(%4)\n\t"
        "lqc2 $vf2, 0x0(%3)\n\t"
        "lqc2 $vf1, 0x10(%3)\n\t"
        "lqc2 %2, 0x0(%4)\n\t"
        "vmulaz.xyzw $ACC, %2, $vf2z\n\t"
        "vmaddaz.xyzw $ACC, $vf3, $vf1z\n\t"
        "vmaddz.xyzw %1, $vf6, $vf5z\n\t"
        "vmulax.xyzw $ACC, %2, $vf2x\n\t"
        "vmaddax.xyzw $ACC, $vf3, $vf1x\n\t"
        "vmaddx.xyzw %0, $vf6, $vf5x\n\t"
        "vmulay.xyzw $ACC, %2, $vf2y\n\t"
        "vmadday.xyzw $ACC, $vf3, $vf1y\n\t"
        "vmaddy.xyzw %2, $vf6, $vf5y"
        : "+j"(v7), "+j"(v8), "=j"(v4) /* "+j" on v7/v8: allocation-only (ranks v4 first for vf4) */
        : "r"(pa), "r"(pb), "m"(*pa), "m"(*pb), "r"(pd) /* allocation-only operand */ : "vf1", "vf2", "vf3", "vf5", "vf6");
    m.r[0] = v7;
    m.r[1] = v4;
    m.r[2] = v8;
    return m;
}
/** @sly2-annot addr=0x11BC00
 * Purpose: 3x3 matrix product into *pm (VU rows)
 * Info: proposed name MulMatrix3Matrix3 (T1); TU mat
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
M3 *MulMatrix3Matrix3(M3 *pm, M3 *pa, M3 *pb)
{
    M3 m;
    register M3 *pt __asm__("$5") = &m; /* pin */
    __asm__("" : "+r"(pa) : "r"(pt)); /* empty asm: keeps a1 = sp ahead of the MulM3 asm (emulates the original schedule) */
    m = MulM3(pa, pb, pm);
    func_0023320C(pm, pt, sizeof(M3));
    return pm;
}
