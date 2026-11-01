typedef int qword __attribute__((mode(TI)));
typedef struct PLANE { float x, y, z, d; } __attribute__((aligned(16))) PLANE;

void InitPlaneFromNormalPos(PLANE *pplane, qword vecNormal, qword vecPos)
{
    __asm__ __volatile__(
        "qmtc2.ni %1, $vf1\n"
        "qmtc2.ni %2, $vf4\n"
        "vmul.xyz $vf2, $vf1, $vf4\n"
        "vaddw.x $vf3, $vf0, $vf0w\n"
        "vadday.x ACC, $vf2, $vf2y\n"
        "vmaddz.x $vf2, $vf3, $vf2z\n"
        "sqc2 $vf1, 0x0(%0)\n"
        "qmfc2.ni $2, $vf2\n"
        "sw $2, 0xC(%0)\n"
        : : "r"(pplane), "r"(vecNormal), "r"(vecPos) : "$2", "memory");
}
