struct SMPA;
float RadNormalize(float rad);
float GSmoothA(float gCur, float dgCur, float gTarget, float dt, SMPA *psmpa, float *pdgNext);

/** @sly2-annot addr=0x11D340
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
float RadSmoothA(float radCur, float dradCur, float radTarget, float dt, SMPA *psmpa, float *pdradNext)
{
    float rad;
    rad = RadNormalize(radTarget - radCur);
    rad = GSmoothA(0.0f, dradCur, rad, dt, psmpa, pdradNext);
    rad = RadNormalize(radCur + rad);
    return rad;
}
