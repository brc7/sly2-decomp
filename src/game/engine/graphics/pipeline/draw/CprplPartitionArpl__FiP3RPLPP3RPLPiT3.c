typedef struct RPL { int k; float z; int a, b; } RPL;
extern int NCmpPrplReverseZ__FPP3RPLT0(const void *, const void *);
extern int NCmpPrplZ__FPP3RPLT0(const void *, const void *);
extern void qsort(void *, int, int, int (*)(const void *, const void *));
/** @sly2-annot addr=0x157AE0
 * Purpose: Bucket-sorts the frame RPL array by kind (26 kinds, counting sort) into aprpl, z-sorts some
 *   buckets, returns end index
 * Info: proposed name CprplPartitionArpl (T1); TU draw
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r3_1539A8+verdicts_A1
 * See: docs/engine/rendering.md#3-drawsw-sort-prologue-and-pass-dispatch-srchandfunc_00157fa0c-cstatic,
 *   docs/engine/rendering.md#13-key-functions, docs/reference/functions/rendering.md#draw-0x157a50-0x1585d8
 * @end-sly2-annot */
int CprplPartitionArpl__FiP3RPLPP3RPLPiT3(int c, RPL *arpl, RPL **aprpl, int *aiFirst, int *ac)
{
    int ai[26];
    int i;
    RPL *prpl;
    for (i = 25; i >= 0; i--)
        ac[i] = 0;
    {int j; for (j = 0, prpl = arpl; j < c; j++, prpl++)
        ac[prpl->k]++;}
    aiFirst[0] = 0;
    ai[0] = 0;
    for (i = 1; i < 26; i++) {
        int t;
        aiFirst[i] = t = ac[i - 1];
        ac[i] += t;
        ai[i] = aiFirst[i];
    }
    {int k; for (k = 0; k < c; k++, arpl++)
        aprpl[ai[arpl->k]++] = arpl;}
    qsort(aprpl + aiFirst[5], ac[5] - aiFirst[5], 4, NCmpPrplReverseZ__FPP3RPLT0);
    qsort(aprpl + aiFirst[6], ac[6] - aiFirst[6], 4, NCmpPrplReverseZ__FPP3RPLT0);
    qsort(aprpl + aiFirst[10], ac[10] - aiFirst[10], 4, NCmpPrplReverseZ__FPP3RPLT0);
    qsort(aprpl + aiFirst[1], ac[1] - aiFirst[1], 4, NCmpPrplReverseZ__FPP3RPLT0);
    qsort(aprpl + aiFirst[2], ac[2] - aiFirst[2], 4, NCmpPrplReverseZ__FPP3RPLT0);
    qsort(aprpl + aiFirst[25], ac[25] - aiFirst[25], 4, NCmpPrplReverseZ__FPP3RPLT0);
    qsort(aprpl + aiFirst[21], ac[21] - aiFirst[21], 4, NCmpPrplZ__FPP3RPLT0);
    return ac[25];
}
