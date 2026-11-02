void qsort(void *, int, int, int (*)(const void *, const void *));
int func_0013B7A0(const void *, const void *);
/** @sly2-annot addr=0x13B7B8
 * Purpose: qsort then remove duplicates in place
 * Info: proposed name SortUniqueBombVictims (T2); TU bomb
 * Notes: names_v2 C proposed SetAloRotationMatchesVelocity__FP3ALOff3ACK (0.438)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#bomb-0x13a9e8-0x13bdf0
 * @end-sly2-annot */
void func_0013B7B8(void *unused, int *pc, int *a) {
    int n = *pc;
    int *pDst, *pSrc, *pMax;
    if (n < 2) return;
    qsort(a, n, 4, func_0013B7A0);
    pDst = a;
    pMax = a + n;
    for (pSrc = a; pSrc < pMax; pSrc++) {
        if (*pDst != *pSrc) *++pDst = *pSrc;
    }
    *pc = pDst - a + 1;
}
