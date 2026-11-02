struct WR; struct ONZ; struct WRE { char pad[0x30]; char onz[1]; };
enum ENSK { ENSK_Get, ENSK_Set };
WRE *PwreGetWrBend(WR *pwr, ENSK ensk);
void AddOnzOnze(ONZ *ponz, float uAmpl, float gFreq, float gPhase, float uRandom);
/** @sly2-annot addr=0x22C5F0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/animation.md#wr-0x22b720-0x22c818
 * @end-sly2-annot */
void AddWrBendNoise(WR *pwr, float uAmpl, float gFreq, float gPhase, float uRandom)
{
    WRE *pwre = PwreGetWrBend(pwr, ENSK_Set);
    AddOnzOnze((ONZ *)pwre->onz, uAmpl, gFreq, gPhase, uRandom);
}
