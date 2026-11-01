typedef int qword __attribute__((mode(TI)));
typedef union { qword q; int n; } U;
typedef struct { qword r[3]; } M3;
void CalculateSinCos__FfPfT1(float g, int *ps, int *pc);
M3 LoadRotateMatrixSinCos(qword s, qword c, qword axis);
void func_0023320C(void *pvDst, void *pvSrc, int cb);
static inline qword QwFromN(int n) { U u; __asm__("" : "=r"(u.q) : "0"(n)); return u.q; }
/* c's conversion. "+r"(a) is allocation-only and the "$4" clobber (a dead argument register) is scheduling-only:
   they put the axis copy (a3) and the hidden return pointer (a0) after this asm, as in the target. */
#define QWFROMN2(dst, n, a) { U u; __asm__("" : "=r"(u.q), "+r"(a) : "0"(n) : "$4"); dst = u.q; }
M3 *func_0011B500(M3 *pm, float g, qword axis)
{
    int s, c;
    M3 m;
    CalculateSinCos__FfPfT1(g, &s, &c);
    { qword qs = QwFromN(s), qc; QWFROMN2(qc, c, axis); m = LoadRotateMatrixSinCos(qs, qc, axis); }
    func_0023320C(pm, &m, sizeof(M3));
    return pm;
}
