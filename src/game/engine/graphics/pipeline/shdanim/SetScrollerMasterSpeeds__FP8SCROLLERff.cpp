struct SCROLLER { char pad[0x60]; float su; float sv; };
/** @sly2-annot addr=0x1EE678
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void SetScrollerMasterSpeeds(SCROLLER *pscroller, float su, float sv)
{
    pscroller->su = su;
    pscroller->sv = sv;
}
