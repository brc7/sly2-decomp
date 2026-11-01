typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
/** @sly2-annot addr=0x11B480
 * Purpose: Rotation matrix about an axis from (sin, cos)
 * Info: proposed name LoadRotateMatrixSinCos (T1); TU mat
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8
 * @end-sly2-annot */
M3 func_0011B480(qword s, qword c, qword axis)
{
    M3 m;
    register qword v1 __asm__("vf1");
    register qword v2 __asm__("vf2");
    register qword v3 __asm__("vf3");
    register qword v6 __asm__("vf6") = c;
    register qword v7 __asm__("vf7") = s;
    register qword v8 __asm__("vf8") = axis;
    __asm__(
        "vsubx.w $vf5, $vf0, $vf6x\n\t"
        "vsub.w $vf1, $vf0, $vf0\n\t"
        "vsub.w $vf2, $vf0, $vf0\n\t"
        "vsub.w $vf3, $vf0, $vf0\n\t"
        "vmulw.xyz $vf5, $vf8, $vf5w\n\t"
        "vmulx.xyz $vf4, $vf8, $vf7x\n\t"
        "vmulx.xyz $vf1, $vf5, $vf8x\n\t"
        "vmuly.xyz $vf2, $vf5, $vf8y\n\t"
        "vmulz.xyz $vf3, $vf5, $vf8z\n\t"
        "vaddx.x $vf1, $vf1, $vf6x\n\t"
        "vaddx.y $vf2, $vf2, $vf6x\n\t"
        "vaddx.z $vf3, $vf3, $vf6x\n\t"
        "vaddz.y $vf1, $vf1, $vf4z\n\t"
        "vsubz.x $vf2, $vf2, $vf4z\n\t"
        "vsuby.z $vf1, $vf1, $vf4y\n\t"
        "vaddy.x $vf3, $vf3, $vf4y\n\t"
        "vaddx.z $vf2, $vf2, $vf4x\n\t"
        "vsubx.y $vf3, $vf3, $vf4x"
        : "=j"(v1), "=j"(v2), "=j"(v3) : "j"(v6), "j"(v7), "j"(v8) : "vf4", "vf5");
    m.r[0] = v1;
    m.r[1] = v2;
    m.r[2] = v3;
    return m;
}
