typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
/** @sly2-annot addr=0x11B340
 * Purpose: 3x3 inverse by cofactors (row cross products / determinant) returned by value
 * Info: proposed name InvertMatrix3Cofactor (T1); TU mat
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8
 * @end-sly2-annot */
M3 InvertMatrix3Cofactor(M3 *pm)
{
    M3 m;
    register qword v1 __asm__("vf1") = pm->r[1];
    register qword v2 __asm__("vf2") = pm->r[0];
    register qword v3 __asm__("vf3") = pm->r[2];
    register qword v4 __asm__("vf4");
    register qword v5 __asm__("vf5");
    register qword v6 __asm__("vf6");
    __asm__(
        "vopmula.xyz $ACC, $vf1, $vf3\n\t"
        "vopmsub.xyz $vf4, $vf3, $vf1\n\t"
        "vopmula.xyz $ACC, $vf3, $vf2\n\t"
        "vopmsub.xyz $vf5, $vf2, $vf3\n\t"
        "vopmula.xyz $ACC, $vf2, $vf1\n\t"
        "vopmsub.xyz $vf6, $vf1, $vf2\n\t"
        "vmul.xyz $vf9, $vf2, $vf4\n\t"
        "vmulax.w $ACC, $vf0, $vf9x\n\t"
        "vmadday.w $ACC, $vf0, $vf9y\n\t"
        "vmaddz.w $vf9, $vf0, $vf9z\n\t"
        "vadd.xyz $vf8, $vf0, $vf4\n\t"
        "vadd.xyz $vf7, $vf0, $vf5\n\t"
        "vdiv $Q, $vf0w, $vf9w\n\t"
        "vaddx.y $vf4, $vf0, $vf5x\n\t"
        "vaddx.z $vf4, $vf0, $vf6x\n\t"
        "vaddy.x $vf5, $vf0, $vf8y\n\t"
        "vaddy.z $vf5, $vf0, $vf6y\n\t"
        "vaddz.x $vf6, $vf0, $vf8z\n\t"
        "vaddz.y $vf6, $vf0, $vf7z\n\t"
        "vwaitq\n\t"
        "vmulq.xyz $vf4, $vf4, $Q\n\t"
        "vmulq.xyz $vf5, $vf5, $Q\n\t"
        "vmulq.xyz $vf6, $vf6, $Q"
        : "=j"(v4), "=j"(v5), "=j"(v6) : "j"(v1), "j"(v2), "j"(v3) : "vf7", "vf8", "vf9");
    m.r[0] = v4;
    m.r[1] = v5;
    m.r[2] = v6;
    return m;
}
