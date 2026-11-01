typedef int qword __attribute__((mode(TI)));
struct S { int a[5]; int n14; };
void *func_001BF1F8(struct S *p, int n, qword q);
qword func_001BFDB0(void *p, qword q);
/** @sly2-annot addr=0x1C0320
 * Purpose: PcgtPointInCbspSafe then ClosestPointOnCgt; returns found
 * Info: proposed name FindClosestPointInCg (T1); TU path
 * Notes: names_v2 v1 proposed LoadJtFromBrx__FP2JTP18CBinaryInputStream (0.001)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
int func_001C0320(struct S *p, qword q, qword *pq)
{
    void *pv = func_001BF1F8(p, p->n14, q);
    if (!pv) return 0;
    *pq = func_001BFDB0(pv, q);
    return 1;
}
