int NRand(void);
/** @sly2-annot addr=0x11D448
 * Purpose: Uniform float in [gMin, gMax]
 * Info: proposed name GRandInRange (T1); TU util
 * Confidence: high [T1 0.88] - evidence: static analysis; sources: r1_116918+verdicts_A1
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
float GRandInRange__Fff(float gMin, float gMax)
{
    if (gMin == gMax)
        return gMin;
    return gMin + (gMax - gMin) * (float)(NRand() % 0x16A1B) * 1.078760305972537e-05f;
}
