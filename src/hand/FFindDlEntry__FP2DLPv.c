typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct DLE { void *pvNext; void *pvPrev; } DLE;
typedef struct DLI { DL *pdl; void **ppv; int ibDle; struct DLI *pdliNext; } DLI;
#define PDLE(pdl, pv) ((DLE *)((char *)(pv) + (pdl)->ibDle))

int FFindDlEntry__FP2DLPv(DL *pdl, void *pv)
{
    return PDLE(pdl, pv)->pvNext || pdl->pvLast == pv;
}
