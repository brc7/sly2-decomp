typedef struct SL { void *pvFirst; void *pvLast; int ibNext; } SL;

/** @sly2-annot addr=0x11A098
 * Purpose: Prepend to singly linked list
 * Info: proposed name PrependSlEntry (T1); TU dl
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#dl-0x11a050-0x11a550
 * @end-sly2-annot */
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
