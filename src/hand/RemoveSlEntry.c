typedef struct SL { void *pvFirst; void *pvLast; int ibNext; } SL;
typedef struct SLI { SL *psl; void **ppv; int ibNext; struct SLI *psliNext; } SLI;

extern SLI *g_psliFirst;

/** @sly2-annot addr=0x11A118
 * Purpose: Unlink entry (linear search for predecessor) and fix up active iterators
 * Info: proposed name RemoveSlEntry (T1); TU dl
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#dl-0x11a050-0x11a550
 * @end-sly2-annot */
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
