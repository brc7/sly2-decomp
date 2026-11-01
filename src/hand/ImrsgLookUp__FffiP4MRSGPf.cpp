struct MRSG { float t; float dt; };
/** @sly2-annot addr=0x138370
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/animation.md#blend-0x137a30-0x138f08
 * @end-sly2-annot */
int ImrsgLookUp(float t, float tMax, int cmrsg, MRSG *amrsg, float *pdt)
{
    int i;
    float dt;
    for (i = 0; ; amrsg++, i++) {
        if (i >= cmrsg) goto done;
        if (amrsg->dt == 0.0f) continue;
        dt = t - amrsg->t;
        if (dt < 0.0f) dt += tMax;
        if (dt < amrsg->dt + 0.0001f) break;
    }
    if (pdt) *pdt = dt;
done:
    return i;
}
