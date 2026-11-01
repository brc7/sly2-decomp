struct WR; struct WRE { char pad[0x64]; float x; };
enum ENSK { ENSK_Get, ENSK_Set };
WRE *PwreGetWrCircle(WR *pwr, ENSK ensk);
/** @sly2-annot addr=0x22C518
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/animation.md#wr-0x22b720-0x22c818
 * @end-sly2-annot */
void SetWrWavelength(WR *pwr, float sWavelength)
{
    WRE *pwre = PwreGetWrCircle(pwr, ENSK_Set);
    if (sWavelength == 0.0f)
        pwre->x = 0.0f;
    else
        pwre->x = 1.0f / sWavelength;
}
