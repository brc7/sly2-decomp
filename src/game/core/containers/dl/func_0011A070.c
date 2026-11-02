typedef struct SL { void *pvFirst; void *pvLast; int ibNext; } SL;

/** @sly2-annot addr=0x11A070
 * Purpose: Append to singly linked list (tail pointer)
 * Info: proposed name AppendSlEntry (T1); TU dl
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/core-library.md#7-key-functions, docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/core-library.md#dl-0x11a050-0x11a550
 * @end-sly2-annot */
void func_0011A070(SL *psl, void *pv)
{
    if (psl->pvLast == 0) {
        psl->pvLast = pv;
        psl->pvFirst = pv;
    } else {
        *(void **)((char *)psl->pvLast + psl->ibNext) = pv;
        psl->pvLast = pv;
    }
}
