typedef int qword __attribute__((mode(TI)));
typedef struct PLANE { float x, y, z, d; } __attribute__((aligned(16))) PLANE;

/** @sly2-annot addr=0x119390
 * Purpose: Plane from normal and a point on it (d = n.pos), VU asm
 * Info: proposed name InitPlaneFromNormalPos (T1); TU bsp
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#bsp-0x118668-0x1195e0
 * @end-sly2-annot */
void func_00119390(PLANE *pplane, qword vecNormal, qword vecPos)
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
