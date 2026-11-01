typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct DLE { void *pvNext; void *pvPrev; } DLE;
typedef struct DLI { DL *pdl; void **ppv; int ibDle; struct DLI *pdliNext; } DLI;
#define PDLE(pdl, pv) ((DLE *)((char *)(pv) + (pdl)->ibDle))

void AppendDlEntry__FP2DLPv(DL *pdl, void *pv);
void PrependDlEntry__FP2DLPv(DL *pdl, void *pv);

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
