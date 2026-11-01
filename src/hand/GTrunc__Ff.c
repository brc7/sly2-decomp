typedef unsigned int uint;

/** @sly2-annot addr=0x11D7F0
 * Purpose: Truncate a float toward zero via exponent bit manipulation
 * Info: proposed name GTrunc (T1); TU util
 * Notes: agent qualifier: float | C++ overload of existing GTrunc__Fd (distinct mangled names) | overload of
 *   GTrunc at 0x11D740 (distinct mangled symbols)
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2+verdicts_A1
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
float GTrunc__Ff(float g)
{
    uint u = *(uint *)&g;
    int exp = ((u >> 23) & 0xFF) - 0x7F;
    if (exp < 0) {
        return 0.0f;
    }
    if (exp < 23) {
        uint mask = (1UL << (23 - exp)) - 1;
        uint frac = u & 0x7FFFFF;
        if ((frac & mask) == mask) {
            *(uint *)&g = (u & 0xFF800000) | (frac & ~mask & 0x7FFFFF);
            g += 1.0f;
        } else {
            *(uint *)&g = (u & 0xFF800000) | (frac & ~mask & 0x7FFFFF);
        }
    }
    return g;
}
