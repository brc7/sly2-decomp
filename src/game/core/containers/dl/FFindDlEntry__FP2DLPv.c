typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct DLE { void *pvNext; void *pvPrev; } DLE;
typedef struct DLI { DL *pdl; void **ppv; int ibDle; struct DLI *pdliNext; } DLI;
#define PDLE(pdl, pv) ((DLE *)((char *)(pv) + (pdl)->ibDle))

/** @sly2-annot addr=0x11A380
 * Purpose: True if pv is linked (next or tail)
 * Info: proposed name FFindDlEntry (T1); TU dl
 * Notes: names_v2 C proposed AdaptAct__FP3ACT (0.438)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r1_116918+verify_side
 * See: docs/reference/functions/core-library.md#dl-0x11a050-0x11a550
 * @end-sly2-annot */
int FFindDlEntry__FP2DLPv(DL *pdl, void *pv)
{
    return PDLE(pdl, pv)->pvNext || pdl->pvLast == pv;
}
