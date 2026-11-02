typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct DLE { void *pvNext; void *pvPrev; } DLE;
typedef struct DLI { DL *pdl; void **ppv; int ibDle; struct DLI *pdliNext; } DLI;
#define PDLE(pdl, pv) ((DLE *)((char *)(pv) + (pdl)->ibDle))

void AppendDlEntry__FP2DLPv(DL *pdl, void *pv);
void PrependDlEntry__FP2DLPv(DL *pdl, void *pv);

/** @sly2-annot addr=0x11A280
 * Purpose: Insert before pvNext (NULL -> append, first -> prepend)
 * Info: proposed name InsertDlEntryBefore (T1); TU dl
 * Notes: names_v2 v1 proposed PrependDlEntry__FP2DLPv (0.013)
 * Confidence: high [T1 0.88] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/core-library.md#dl-0x11a050-0x11a550
 * @end-sly2-annot */
void InsertDlEntryBefore__FP2DLPvT1(DL *pdl, void *pvNext, void *pv)
{
    if (!pvNext) {
        AppendDlEntry__FP2DLPv(pdl, pv);
    } else if (pvNext == pdl->pvFirst) {
        PrependDlEntry__FP2DLPv(pdl, pv);
    } else {
        DLE *pdle = PDLE(pdl, pv);
        DLE *pdleNext = PDLE(pdl, pvNext);
        void *pvPrev = pdleNext->pvPrev;
        DLE *pdlePrev = PDLE(pdl, pvPrev);
        pdle->pvNext = pvNext;
        pdle->pvPrev = pvPrev;
        pdleNext->pvPrev = pv;
        pdlePrev->pvNext = pv;
    }
}
