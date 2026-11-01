typedef struct SL { void *pvFirst; void *pvLast; int ibNext; } SL;

void ClearDle__FP3DLE(SL *psl)
{
    psl->pvLast = 0;
    psl->pvFirst = 0;
}
