typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct DLE { void *pvNext; void *pvPrev; } DLE;
typedef struct DLI { DL *pdl; void **ppv; int ibDle; struct DLI *pdliNext; } DLI;
#define PDLE(pdl, pv) ((DLE *)((char *)(pv) + (pdl)->ibDle))

extern DLI *g_pdliFirst;

void RemoveDlEntry__FP2DLPv(DL *pdl, void *pv)
{
    DLE *pdle = PDLE(pdl, pv);
    DLI *pdli = g_pdliFirst;
    void *pvT;

    while (pdli) {
        if ((DLE *)pdli->ppv == pdle) {
            void *pvPrev = pdle->pvPrev;
            if (pvPrev) {
                pdli->ppv = (void **)PDLE(pdl, pvPrev);
            } else {
                pdli->ppv = (void **)pdl;
            }
        }
        pdli = pdli->pdliNext;
    }

    pvT = pdle->pvPrev;
    if (pvT) {
        PDLE(pdl, pvT)->pvNext = pdle->pvNext;
    } else {
        pdl->pvFirst = pdle->pvNext;
    }

    pvT = pdle->pvNext;
    if (pvT) {
        PDLE(pdl, pvT)->pvPrev = pdle->pvPrev;
    } else {
        pdl->pvLast = pdle->pvPrev;
    }

    pdle->pvNext = 0;
    pdle->pvPrev = 0;
}
