typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
M4 InvertMatrix4Rigid(M4 *pm)
{
    M4 m;
    register qword v2 __asm__("vf2") = pm->r[1];
    register qword v1 __asm__("vf1") = pm->r[2];
    register qword v3 __asm__("vf3") = pm->r[0];
    register qword v4 __asm__("vf4") = pm->r[3];
    register qword v5 __asm__("vf5");
    register qword v6 __asm__("vf6");
    register qword v7 __asm__("vf7");
    register qword v8 __asm__("vf8");
    __asm__(
        "vopmula.xyz $ACC, $vf2, $vf1\n\t"
        "vopmsub.xyz $vf5, $vf1, $vf2\n\t"
        "vopmula.xyz $ACC, $vf1, $vf3\n\t"
        "vopmsub.xyz $vf6, $vf3, $vf1\n\t"
        "vopmula.xyz $ACC, $vf3, $vf2\n\t"
        "vopmsub.xyz $vf7, $vf2, $vf3\n\t"
        "vmul.xyz $vf10, $vf3, $vf5\n\t"
        "vmulax.w $ACC, $vf0, $vf10x\n\t"
        "vmadday.w $ACC, $vf0, $vf10y\n\t"
        "vmaddz.w $vf10, $vf0, $vf10z\n\t"
        "vadd.xyz $vf10, $vf0, $vf5\n\t"
        "vadd.xyz $vf9, $vf0, $vf6\n\t"
        "vsub.xyz $vf4, $vf0, $vf4\n\t"
        "vdiv $Q, $vf0w, $vf10w\n\t"
        "vaddx.y $vf5, $vf0, $vf6x\n\t"
        "vaddx.z $vf5, $vf0, $vf7x\n\t"
        "vaddy.x $vf6, $vf0, $vf10y\n\t"
        "vaddy.z $vf6, $vf0, $vf7y\n\t"
        "vaddz.x $vf7, $vf0, $vf10z\n\t"
        "vaddz.y $vf7, $vf0, $vf9z\n\t"
        "vmulq.xyz $vf5, $vf5, $Q\n\t"
        "vsub.w $vf5, $vf0, $vf0\n\t"
        "vmulq.xyz $vf6, $vf6, $Q\n\t"
        "vsub.w $vf6, $vf0, $vf0\n\t"
        "vmulq.xyz $vf7, $vf7, $Q\n\t"
        "vsub.w $vf7, $vf0, $vf0\n\t"
        "vmulax.xyz $ACC, $vf5, $vf4x\n\t"
        "vmadday.xyz $ACC, $vf6, $vf4y\n\t"
        "vmaddz.xyz $vf8, $vf7, $vf4z\n\t"
        "vaddx.w $vf8, $vf0, $vf0x"
        : "=j"(v5), "=j"(v6), "=j"(v7), "=j"(v8), "+j"(v4) : "j"(v1), "j"(v2), "j"(v3) : "vf9", "vf10");
    m.r[0] = v5;
    m.r[1] = v6;
    m.r[2] = v7;
    m.r[3] = v8;
    return m;
}
