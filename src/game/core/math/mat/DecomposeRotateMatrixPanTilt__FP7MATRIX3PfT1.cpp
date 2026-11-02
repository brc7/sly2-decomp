struct MATRIX3 { float mat[3][4]; } __attribute__((aligned(16)));
extern "C" float atan2f(float y, float x);
static inline float sqrtf_(float x) { float r; __asm__ __volatile__("sqrt.s %0, %1" : "=&f"(r) : "f"(x)); return r; }

/** @sly2-annot addr=0x11BC88
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8
 * @end-sly2-annot */
void DecomposeRotateMatrixPanTilt(MATRIX3 *pmat, float *ppan, float *ptilt)
{
    *ppan = atan2f(pmat->mat[0][1], pmat->mat[0][0]);
    *ptilt = atan2f(pmat->mat[0][2], sqrtf_(pmat->mat[0][0] * pmat->mat[0][0] + pmat->mat[0][1] * pmat->mat[0][1]));
}
