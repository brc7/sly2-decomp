struct E { int x0; unsigned short x4; unsigned short x6; };
void CopyAb__FPvT0Ui(void *dst, void *src, int cb);
/** @sly2-annot addr=0x1C18B8
 * Purpose: Delete the begin/end event pair of key from a sorted event array
 * Info: proposed name RemoveObstacleEvents (T2); TU path
 * Notes: names_v2 v1 proposed ThrowJt__FP2JTP6VECTORff (0.005)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void func_001C18B8(int n, struct E *a, int key)
{
    int i, iFirst, iLast;
    struct E *pe;
    i = 0;
    pe = a;
    goto test;
    for (;;) {
        pe++;
        i++;
    test:
        if (i >= n) break;
        if (pe->x6 == key) {
            if (pe->x4 != 0) {
                iFirst = i;
                continue;
            }
            iLast = i;
            break;
        }
    }
    CopyAb__FPvT0Ui(&a[iFirst], &a[iFirst] + 1, (iLast - iFirst - 1) * sizeof(struct E));
    CopyAb__FPvT0Ui(&a[iLast] - 1, &a[iLast] + 1, (n - iLast - 1) * sizeof(struct E));
}
