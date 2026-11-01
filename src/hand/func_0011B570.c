typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
typedef struct { qword r[3]; } M3;
extern M3 D_002A2D00;
M3 *func_0011B500(M3 *pm, float g, qword axis);
void *func_0023320C(void *pvDst, void *pvSrc, int cb);
/** @sly2-annot addr=0x11B570
 * Purpose: Rotation matrix from a rotation vector (angle = |v|, axis = v/|v|)
 * Info: proposed name LoadRotateMatrix (T1); TU mat
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8,
 *   docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70
 * @end-sly2-annot */
M3 *func_0011B570(M3 *pm, qword v)
{
    qword d, one, l;
    float g;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(l));
    if (g < 1.0e-4f) {
        func_0023320C(pm, &D_002A2D00, sizeof(M3));
    } else {
        M3 m;
        U s;
        qword n;
        __asm__("qmtc2.ni %1, %0" : "=j"(s.q) : "r"(g));
        __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(n) : "j"(s.q), "j"(v));
        func_0011B500(&m, g, n);
        func_0023320C(pm, &m, sizeof(M3));
    }
    return pm;
}
