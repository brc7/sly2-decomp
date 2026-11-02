typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
extern qword D_002A2DB0;
extern qword D_002A2DC0;
void func_0011B500(M3 *pm, float g, qword axis);
void *func_0023320C(void *pvDst, void *pvSrc, int cb);
static inline M3 MulM3(M3 *pa, M3 *pb)
{
    M3 m;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(m.r[0]) : "j"(pa->r[0]), "j"(pa->r[1]), "j"(pa->r[2]), "j"(pb->r[0]));
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(m.r[1]) : "j"(pa->r[0]), "j"(pa->r[1]), "j"(pa->r[2]), "j"(pb->r[1]));
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(m.r[2]) : "j"(pa->r[0]), "j"(pa->r[1]), "j"(pa->r[2]), "j"(pb->r[2]));
    return m;
}
/** @sly2-annot addr=0x11B720
 * Purpose: R = Rz(pan) * Ry(-tilt)
 * Info: proposed name LoadRotateMatrixPanTilt (T1); TU mat
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8,
 *   docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8
 * @end-sly2-annot */
M3 *LoadRotateMatrixPanTilt(M3 *pm, float g1, float g2)
{
    M3 c;
    M3 a;
    M3 b;
    func_0011B500(&a, g1, D_002A2DC0);
    func_0011B500(&b, -g2, D_002A2DB0);
    c = MulM3(&a, &b);
    func_0023320C(pm, &c, sizeof(M3));
    return pm;
}
