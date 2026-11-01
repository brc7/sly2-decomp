float fmodf(float x, float y);

/** @sly2-annot addr=0x11D898
 * Purpose: fmod returning a non-negative result
 * Info: proposed name GModPositive (T1); TU util
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2+verdicts_A1
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
float func_0011D898(float gDividend, float gDivisor)
{
    float result = fmodf(gDividend, gDivisor);
    if (result < 0.0f) {
        result += gDivisor;
    }
    return result;
}
