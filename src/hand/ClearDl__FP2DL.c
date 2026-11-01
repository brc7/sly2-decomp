typedef struct SL { void *pvFirst; void *pvLast; int ibNext; } SL;

void ClearDl__FP2DL(SL *psl)
{
    psl->pvLast = 0;
    psl->pvFirst = 0;
}
