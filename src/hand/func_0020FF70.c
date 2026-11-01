typedef struct QW { int a[4]; } QW;
void func_0020FFF0(void *p, QW *pq, int a, int b, int c, int d);
/** @sly2-annot addr=0x20FF70
 * Purpose: Apply every RSMG record in an array (calls 0x20FFF0 each)
 * Info: proposed name TriggerRsmg (T1); TU sw
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020FF70(void *p, int c, QW *pq, int a, int b)
{
    int i;
    for (i = 0; i < c; i++) {
        func_0020FFF0(p, &pq[i], a, b, 0, 0);

    }
}
