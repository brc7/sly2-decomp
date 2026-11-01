struct SCROLLER { char pad[0x24]; short du; short dv; char pad2[0x50 - 0x28]; float duSpeed; float dvSpeed; float duMax; float dvMax; };
/** @sly2-annot addr=0x1EE5B8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
float UCompleteScroller(SCROLLER *pscroller)
{
    float uComp = 0.0f;

    if (pscroller->duSpeed != 0.0f)
        uComp = ((pscroller->du * (1.0f / 4096.0f)) / pscroller->duMax) * 0.5f;
    else
        uComp = 0.5f;

    if (pscroller->dvSpeed != 0.0f)
        uComp += ((pscroller->dv * (1.0f / 4096.0f)) / pscroller->dvMax) * 0.5f;
    else
        uComp += 0.5f;

    return uComp;
}
