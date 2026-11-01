typedef struct SL { void *pvFirst; void *pvLast; int ibNext; } SL;

void PrependSlEntry(SL *psl, void *pv)
{
    void **ppvNext = (void **)((char *)pv + psl->ibNext);
    if (psl->pvFirst == 0) {
        psl->pvLast = pv;
        psl->pvFirst = pv;
    } else {
        *ppvNext = psl->pvFirst;
        psl->pvFirst = pv;
    }
}
