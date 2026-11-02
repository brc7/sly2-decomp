struct LM { float gMin; float gMax; };
struct EMITVX { int c; float dg; float drad; float g; float rad; };
float RadNormalize(float rad) asm("RadNormalize__Ff");
float GRandInRange(float gMin, float gMax) asm("GRandInRange__Fff");
/** @sly2-annot addr=0x15AE38
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void CalculateEmitvx(int c, LM *plm, int cMax, EMITVX *pemitvx)
{
    int n;
    float dg;
    if (c > 0)
        pemitvx->c = c < cMax ? c : cMax;
    else
        pemitvx->c = cMax;
    n = (cMax + (pemitvx->c - 1)) / pemitvx->c;
    dg = RadNormalize(plm->gMax - plm->gMin);
    pemitvx->drad = 6.2831855f / pemitvx->c;
    pemitvx->dg = dg / n;
    pemitvx->g = plm->gMin + pemitvx->dg * 0.5f;
    pemitvx->rad = GRandInRange(0.0f, 6.2831855f);
}
