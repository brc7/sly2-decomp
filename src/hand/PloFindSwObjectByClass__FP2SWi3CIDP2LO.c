int CploFindSwObjectsByClass(void *a, int b, int c, int d, int f, int *pn);
/** @sly2-annot addr=0x15ED78
 * Purpose: First LO of a class: CploFindSwObjectsByClass(grffso|0x200, cid, ctx, 1, &plo)
 * Info: proposed name PloFindSwObjectByClass (T1); TU find
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#find-0x15e380-0x15ef00
 * @end-sly2-annot */
int PloFindSwObjectByClass__FP2SWi3CIDP2LO(void *a, int b, int c, int d)
{
    int n;
    if (!CploFindSwObjectsByClass(a, b | 0x200, c, d, 1, &n))
        return 0;
    return n;
}
