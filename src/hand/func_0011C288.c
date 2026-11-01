typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
typedef struct { qword r[3]; } M3;
typedef struct { float m[4][4]; } M4;
static inline qword Mulx(qword a, float g)
{
    U s;
    qword r;
    __asm__("qmtc2.ni %1, %0" : "=j"(s.q) : "r"(g));
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(a), "j"(s.q));
    return r;
}
/** @sly2-annot addr=0x11C288
 * Purpose: 4x4 from 3x3 rotation uniformly scaled by g plus position
 * Info: proposed name LoadMatrixFromPosRotScalar (T1); TU mat
 * Notes: names_v2 v1 proposed DecomposeRotateMatrixPanTilt__FP7MATRIX3PfT1 (0.001)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8
 * @end-sly2-annot */
M4 *func_0011C288(M4 *pm, qword pos, M3 *pmat, float g)
{
    *(qword *)pm->m[0] = Mulx(pmat->r[0], g);
    *(qword *)pm->m[1] = Mulx(pmat->r[1], g);
    *(qword *)pm->m[2] = Mulx(pmat->r[2], g);
    *(qword *)pm->m[3] = pos;
    pm->m[0][3] = pm->m[1][3] = pm->m[2][3] = 0.0f;
    pm->m[3][3] = 1.0f;
    return pm;
}
