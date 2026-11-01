typedef struct SL { void *pvFirst; void *pvLast; int ibNext; } SL;
typedef struct SLI { SL *psl; void **ppv; int ibNext; struct SLI *psliNext; } SLI;

extern SLI *g_psliFirst;

void RemoveSlEntry(SL *psl, void *pv)
{
    void **ppvNext = (void **)((char *)pv + psl->ibNext);
    void **ppv = &psl->pvFirst;
    SLI *psli;

    while (pv != *ppv) {
        ppv = (void **)((char *)*ppv + psl->ibNext);
    }

    for (psli = g_psliFirst; psli; psli = psli->psliNext) {
        if (psli->ppv == ppvNext) {
            psli->ppv = ppv;
        }
    }

    *ppv = *ppvNext;
    if (psl->pvFirst == 0) {
        psl->pvLast = 0;
    } else if (*ppv == 0) {
        psl->pvLast = (char *)ppv - psl->ibNext;
    }
    *ppvNext = 0;
}
