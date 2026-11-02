struct WR; struct WRE;
enum ENSK { ENSK_0 };
enum WREK { WREK_Circle = 0, WREK_Bend = 1 };
WRE *PwreGetWrCur(WR *pwr, ENSK ensk, WREK wrek);
/** @sly2-annot addr=0x22C4F8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/animation.md#wr-0x22b720-0x22c818
 * @end-sly2-annot */
WRE *PwreGetWrBend(WR *pwr, ENSK ensk)
{
    return PwreGetWrCur(pwr, ensk, WREK_Bend);
}
