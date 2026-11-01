struct MATRIX3 { float mat[3][4]; } __attribute__((aligned(16)));
struct VECTOR { float x, y, z, w; } __attribute__((aligned(16)));
extern VECTOR D_002A2DA0;
extern "C" float acosf(float g);
static inline float SqrtF(float x) { float r; __asm__ __volatile__("sqrt.s %0, %1" : "=&f"(r) : "f"(x)); return r; }

/** @sly2-annot addr=0x11BCF8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR (EXISTING); TU mat
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: pre-existing symbol in symbol_addrs.txt
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8
 * @end-sly2-annot */
void DecomposeRotateMatrixRad(MATRIX3 *pmat, float *prad, VECTOR *pvec)
{
    float c = (pmat->mat[0][0] + pmat->mat[1][1] + pmat->mat[2][2] - 1.0f) * 0.5f;
    if (c > 0.999999f) {
        *prad = 0.0f;
        *pvec = D_002A2DA0;
        return;
    }

    float gRem = 1.0f;
    float gDiv = gRem - c;
    int i;
    for (i = 0; i < 2; i++) {
        float g = (pmat->mat[i][i] - c) / gDiv;
        float gAbs = __builtin_fabsf(g);
        if (gAbs > gRem)
            gAbs = gRem;
        else if (gAbs < 0.0f)
            gAbs = 0.0f;
        (&pvec->x)[i] = SqrtF(gAbs);
        gRem -= gAbs;
    }
    pvec->z = SqrtF(gRem);

    if (c < -0.9999f) {
        *prad = 3.14159265f;
        float x = pvec->x;
        float y = pvec->y;
        float z = pvec->z;
        if (y < x) {
            if (z < x) {
                if (pmat->mat[0][1] < 0.0f)
                    pvec->y = -y;
                if (pmat->mat[0][2] < 0.0f)
                    pvec->z = -pvec->z;
            } else {
                if (pmat->mat[2][0] < 0.0f)
                    pvec->x = -x;
                if (pmat->mat[2][1] < 0.0f)
                    pvec->y = -pvec->y;
            }
        } else if (z < y) {
            if (pmat->mat[1][0] < 0.0f)
                pvec->x = -x;
            if (pmat->mat[1][2] < 0.0f)
                pvec->z = -pvec->z;
        } else {
            if (pmat->mat[2][0] < 0.0f)
                pvec->x = -x;
            if (pmat->mat[2][1] < 0.0f)
                pvec->y = -pvec->y;
        }
    } else {
        *prad = acosf(c);
        if (pmat->mat[1][0] > pmat->mat[0][1])
            pvec->z = -pvec->z;
        if (pmat->mat[2][0] < pmat->mat[0][2])
            pvec->y = -pvec->y;
        if (pmat->mat[2][1] > pmat->mat[1][2])
            pvec->x = -pvec->x;
    }
}
