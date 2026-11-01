typedef struct SL { void *pvFirst; void *pvLast; int ibNext; } SL;
/** @sly2-annot addr=0x11A060
 * Purpose: pvFirst = pvLast = NULL
 * Info: proposed name ClearSl (T1); TU dl
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#dl-0x11a050-0x11a550
 * @end-sly2-annot */
void ClearSl(SL *psl) { psl->pvLast = 0; psl->pvFirst = 0; }
