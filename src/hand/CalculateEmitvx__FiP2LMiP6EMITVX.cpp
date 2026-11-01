struct LM { float gMin; float gMax; };
struct EMITVX { int c; float dg; float drad; float g; float rad; };
float RadNormalize(float rad) asm("RadNormalize__Ff");
float GRandInRange(float gMin, float gMax) asm("GRandInRange__Fff");
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
