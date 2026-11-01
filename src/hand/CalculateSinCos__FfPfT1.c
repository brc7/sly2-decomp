static inline float sqrtf_(float x) { float r; __asm__("sqrt.s %0, %1" : "=&f"(r) : "f"(x)); return r; }
static inline float GClamp(float g, float lo, float hi)
{
    if (g < lo) return lo;
    if (g > hi) return hi;
    return g;
}

/** @sly2-annot addr=0x11D608
 * Purpose: sin and cos of an angle (polynomial, clamped)
 * Info: proposed name CalculateSinCos (T1); TU util
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources:
 *   r1_116918+verdicts_tu_r1r2+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8,
 *   docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
void CalculateSinCos__FfPfT1(float rad, float *psin, float *pcos)
{
    float g = (rad + 1.5707963705062866f) * 0.31830987334251404f;
    int n = (int)g;
    float x2, s, c;
    if (g < 0.0f)
        n--;
    rad -= (float)n * 3.1415927410125732f;
    if (n & 1)
        rad = -rad;
    x2 = rad * rad;
    s = rad;
    rad *= x2;
    s += rad * -0.16666656732559204f;
    rad *= x2;
    s += rad * 0.008333025500178337f;
    rad *= x2;
    s += rad * -0.00019807413627859205f;
    rad *= x2;
    s += rad * 2.601886990305502e-06f;
    s = GClamp(s, -1.0f, 1.0f);
    c = sqrtf_(1.0f - s * s);
    if (n & 1)
        c = -c;
    *pcos = c;
    *psin = s;
}
