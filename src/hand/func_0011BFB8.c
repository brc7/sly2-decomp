typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
void DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(void *pmat, float *pg, U *pv);
/** @sly2-annot addr=0x11BFB8
 * Purpose: DecomposeRotateMatrixRad then returns rad*normal as a vector
 * Info: proposed name DecomposeRotateMatrixVector (T2); TU mat
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8
 * @end-sly2-annot */
qword func_0011BFB8(void *pmat)
{
    U v;
    U s;
    int g;
    qword r;
    DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(pmat, (float *)&g, &v);
    __asm__("qmtc2.ni %1, %0" : "=j"(s.q) : "r"(g));
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v.q), "j"(s.q));
    return r;
}
