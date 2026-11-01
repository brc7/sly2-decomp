float cosf(float) __asm__("func_00244E10");
void FitLinearFunction(float x0, float y0, float x1, float y1, float *pdu, float *pru);
/** @sly2-annot addr=0x1A38A8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void ConvertAngleStrength(float deg0, float g0, float deg1, float g1, float *pdu, float *pru)
{
    const float kDegToHalfRad = 0.008726647f;
    const float kStrengthScale = 1.0f / 255.0f;

    float strength0 = cosf(deg0 * kDegToHalfRad);
    strength0 += strength0 * strength0 * strength0;

    float strength1 = cosf(deg1 * kDegToHalfRad);
    strength1 += strength1 * strength1 * strength1;

    FitLinearFunction(strength0, g0 * kStrengthScale, strength1, g1 * kStrengthScale, pdu, pru);
}
