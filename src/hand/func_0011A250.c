typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct DLE { void *pvNext; void *pvPrev; } DLE;
typedef struct DLI { DL *pdl; void **ppv; int ibDle; struct DLI *pdliNext; } DLI;
#define PDLE(pdl, pv) ((DLE *)((char *)(pv) + (pdl)->ibDle))

/** @sly2-annot addr=0x11A250
 * Purpose: Prepend
 * Info: proposed name PrependDlEntry (T1); TU dl
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#dl-0x11a050-0x11a550
 * @end-sly2-annot */
void func_0011A250(DL *pdl, void *pv)
{
    DLE *pdle = PDLE(pdl, pv);
    if (!pdl->pvFirst) {
        pdl->pvLast = pv;
        pdl->pvFirst = pv;
    } else {
        DLE *pdleFirst = PDLE(pdl, pdl->pvFirst);
        pdle->pvNext = pdl->pvFirst;
        pdleFirst->pvPrev = pv;
        pdl->pvFirst = pv;
    }
}
