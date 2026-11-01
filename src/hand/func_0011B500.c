typedef int qword __attribute__((mode(TI)));
typedef union { qword q; int n; } U;
typedef struct { qword r[3]; } M3;
void func_0011D608(float g, int *ps, int *pc);
M3 func_0011B480(qword s, qword c, qword axis);
void func_0023320C(void *pvDst, void *pvSrc, int cb);
static inline qword QwFromN(int n) { U u; __asm__("" : "=r"(u.q) : "0"(n)); return u.q; }
/* c's conversion. "+r"(a) is allocation-only and the "$4" clobber (a dead argument register) is scheduling-only:
   they put the axis copy (a3) and the hidden return pointer (a0) after this asm, as in the target. */
#define QWFROMN2(dst, n, a) { U u; __asm__("" : "=r"(u.q), "+r"(a) : "0"(n) : "$4"); dst = u.q; }
/** @sly2-annot addr=0x11B500
 * Purpose: Rotation matrix for angle rad about axis (CalculateSinCos 0x11D608 + 0x11B480)
 * Info: proposed name LoadRotateMatrixRad (T1); TU mat
 * Notes: names_v2 B proposed CalculateDmat__FP7MATRIX3N20 (0.856)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
M3 *func_0011B500(M3 *pm, float g, qword axis)
{
    int s, c;
    M3 m;
    func_0011D608(g, &s, &c);
    { qword qs = QwFromN(s), qc; QWFROMN2(qc, c, axis); m = func_0011B480(qs, qc, axis); }
    func_0023320C(pm, &m, sizeof(M3));
    return pm;
}
