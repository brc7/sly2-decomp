float GModPositive__Fff(float gDividend, float gDivisor);

/** @sly2-annot addr=0x11CF60
 * Purpose: Wraps an angle into [-pi, pi) using GModPositive
 * Info: proposed name RadNormalize (T1); TU util
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_A1
 * See: docs/engine/core-library.md#6-source-files, docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
float RadNormalize__Ff(float rad)
{
    if (rad < -3.14159265359f || 3.14159265359f < rad) {
        float modpos = GModPositive__Fff(rad + 3.14159265359f, 2 * 3.14159265359f);
        rad = modpos - 3.14159265359f;
    }
    return rad;
}
