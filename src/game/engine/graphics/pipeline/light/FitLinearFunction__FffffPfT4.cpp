int FFloatsNear(float a, float b, float eps) __asm__("FFloatsNear__Ffff");
/** @sly2-annot addr=0x1A3730
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void FitLinearFunction(float x0, float y0, float x1, float y1, float *pdu, float *pru)
{
    if (FFloatsNear(x0, x1, 0.0001f))
    {
        *pru = 0.0f;
        *pdu = y0;
        return;
    }

    float a = (y1 - y0) / (x1 - x0);
    *pru = a;
    *pdu = y0 - a * x0;
}
