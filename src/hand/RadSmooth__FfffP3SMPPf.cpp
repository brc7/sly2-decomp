struct SMP;
float RadNormalize(float rad);
float GSmooth(float gCur, float gTarget, float dt, SMP *psmp, float *pdgNext);

/** @sly2-annot addr=0x11D2D0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
float RadSmooth(float radCur, float radTarget, float dt, SMP *psmp, float *pdradNext)
{
    float rad;
    rad = RadNormalize(radTarget - radCur);
    rad = GSmooth(0.0f, rad, dt, psmp, pdradNext);
    rad = RadNormalize(radCur + rad);
    return rad;
}
