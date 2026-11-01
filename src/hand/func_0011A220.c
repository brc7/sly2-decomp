typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct DLE { void *pvNext; void *pvPrev; } DLE;
typedef struct DLI { DL *pdl; void **ppv; int ibDle; struct DLI *pdliNext; } DLI;
#define PDLE(pdl, pv) ((DLE *)((char *)(pv) + (pdl)->ibDle))

/** @sly2-annot addr=0x11A220
 * Purpose: Append to doubly linked list
 * Info: proposed name AppendDlEntry (T1); TU dl
 * Confidence: high [T1 0.92] - evidence: static analysis; sources: r1_116918+verify_side
 * See: docs/engine/core-library.md#2-containers,
 *   docs/reference/functions/core-library.md#dl-0x11a050-0x11a550
 * @end-sly2-annot */
void func_0011A220(DL *pdl, void *pv)
{
    DLE *pdle = PDLE(pdl, pv);
    if (!pdl->pvLast) {
        pdl->pvLast = pv;
        pdl->pvFirst = pv;
    } else {
        DLE *pdleLast = PDLE(pdl, pdl->pvLast);
        pdle->pvPrev = pdl->pvLast;
        pdleLast->pvNext = pv;
        pdl->pvLast = pv;
    }
}
