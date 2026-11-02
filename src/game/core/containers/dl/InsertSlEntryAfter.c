typedef struct SL { void *pvFirst; void *pvLast; int ibNext; } SL;

void PrependSlEntry(SL *psl, void *pv);
void func_0011A070(SL *psl, void *pv);

/** @sly2-annot addr=0x11A0C0
 * Purpose: Insert after pvPrev (NULL -> prepend, last -> append)
 * Info: proposed name InsertSlEntryAfter (T1); TU dl
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#dl-0x11a050-0x11a550
 * @end-sly2-annot */
void InsertSlEntryAfter(SL *psl, void *pvPrev, void *pv)
{
    if (pvPrev == 0) {
        PrependSlEntry(psl, pv);
    } else if (pvPrev == psl->pvLast) {
        func_0011A070(psl, pv);
    } else {
        void **ppvPrev = (void **)((char *)pvPrev + psl->ibNext);
        *(void **)((char *)pv + psl->ibNext) = *ppvPrev;
        *ppvPrev = pv;
    }
}
