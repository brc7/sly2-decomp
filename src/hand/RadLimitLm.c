typedef struct LM { float gMin; float gMax; } LM;
float RadNormalize__Ff(float rad);
float GLimitAbs__Fff(float g, float gLimit);

/** @sly2-annot addr=0x11D978
 * Purpose: Clamps an angle into the angular range plm (around its midpoint, wrap-aware)
 * Info: proposed name RadLimitLm (T1); TU util
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
float RadLimitLm(LM *plm, float rad)
{
    float radMid = (plm->gMax + plm->gMin) * 0.5f;
    float radHalf = (plm->gMax - plm->gMin) * 0.5f;
    return RadNormalize__Ff(radMid + GLimitAbs__Fff(RadNormalize__Ff(rad - radMid), radHalf));
}
