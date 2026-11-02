typedef struct SL { void *pvFirst; void *pvLast; int ibNext; } SL;

/** @sly2-annot addr=0x11A1B8
 * Purpose: True if entry is linked (next != NULL or it is the tail)
 * Info: proposed name FFindSlEntry (T1); TU dl
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/core-library.md#7-key-functions, docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/core-library.md#dl-0x11a050-0x11a550
 * @end-sly2-annot */
int FFindSlEntry(SL *psl, void *pv)
{
    int f = 0;
    if (*(void **)((char *)pv + psl->ibNext) != 0 || psl->pvLast == pv) {
        f = 1;
    }
    return f;
}
