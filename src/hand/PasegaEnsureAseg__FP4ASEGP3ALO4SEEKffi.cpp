struct ASEG; struct ALO; struct ASEGA; enum SEEK { SEEK_0 };
void EnsureAseg(ASEG *, ALO *, SEEK, float, float, int, ASEGA **);
/** @sly2-annot addr=0x130598
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8
 * @end-sly2-annot */
ASEGA *PasegaEnsureAseg(ASEG *paseg, ALO *paloRoot, SEEK seek, float t, float svt, int grfapl)
{ ASEGA *pasega; EnsureAseg(paseg, paloRoot, seek, t, svt, grfapl, &pasega); return pasega; }
