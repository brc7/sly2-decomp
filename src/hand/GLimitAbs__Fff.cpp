/** @sly2-annot addr=0x11CFE0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
float GLimitAbs(float g, float gLimit)
{
    if (g > gLimit) {
        g = gLimit;
    } else if (g < -gLimit) {
        g = -gLimit;
    }
    return g;
}
