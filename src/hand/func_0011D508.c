static inline float sqrtf_(float x) { float r; __asm__("sqrt.s %0, %1" : "=&f"(r) : "f"(x)); return r; }
#define sqrtf sqrtf_
float fabsf(float x);

/** @sly2-annot addr=0x11D508
 * Purpose: Real roots of a x^2 + b x + c (count returned, roots written)
 * Info: proposed name CSolveQuadratic (T1); TU util
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources:
 *   r1_116918+verdicts_tu_r1r2+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
int func_0011D508(float a, float b, float c, float *ax)
{
    float alpha;
    float beta;

    c *= 4.0f;
    alpha = b * b - a * c;
    if (alpha < 0.0f)
        return 0;
    a = a + a;
    alpha = sqrtf(alpha) / a;
    beta = b / a;
    if (fabsf(alpha) < 0.0001f) {
        *ax = -beta;
        return 1;
    }
    ax[0] = -beta + alpha;
    ax[1] = -beta - alpha;
    if (ax[1] < ax[0]) {
        ax[0] = -beta - alpha;
        ax[1] = -beta + alpha;
    }
    return 2;
}
