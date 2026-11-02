struct ASEG; struct ALO; struct ASEGA;
enum SEEK { SEEK_0 };
ASEGA *PasegaFindAseg(ASEG *paseg, ALO *palo);
void ApplyAseg(ASEG *paseg, ALO *paloRoot, float t, float svt, int grfapl, ASEGA **ppasega);
void SeekAsega(ASEGA *pasega, SEEK seek, float t, float svt);   /* func_00130D30 */
/** @sly2-annot addr=0x1304E0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8
 * @end-sly2-annot */
void EnsureAseg(ASEG *paseg, ALO *paloRoot, SEEK seek, float t, float svt, int grfapl, ASEGA **ppasega)
{
    ASEGA *pasega;
    if (!ppasega) ppasega = &pasega;
    pasega = PasegaFindAseg(paseg, paloRoot);
    if (pasega) *ppasega = pasega;
    else ApplyAseg(paseg, paloRoot, 0.0f, 1.0f, grfapl, ppasega);
    SeekAsega(*ppasega, seek, t, svt);
}
