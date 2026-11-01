struct LM { float gMin; float gMax; };

/** @sly2-annot addr=0x11D948
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
float GLimitLm(LM *plm, float g)
{
    if (g < plm->gMin) {
        return plm->gMin;
    }
    if (g > plm->gMax) {
        return plm->gMax;
    }
    return g;
}
