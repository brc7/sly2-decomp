struct PINGPONG { char pad[0x1C]; int cframe; char pad2[0x60 - 0x20]; float dframe; float iframe; float dtPause; };
/** @sly2-annot addr=0x1EDF28
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
float UCompletePingpong(PINGPONG *ppingpong)
{
    float absDframe = ppingpong->dframe;
    float progress;

    if (absDframe < 0.0f)
    {
        absDframe = -absDframe;
        progress = (float)(ppingpong->cframe * 2) - ppingpong->iframe;
    }
    else
    {
        progress = ppingpong->iframe;
    }

    return (progress / absDframe) /
           (((float)(ppingpong->cframe * 2) / absDframe) + ppingpong->dtPause);
}
