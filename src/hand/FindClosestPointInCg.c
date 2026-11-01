typedef int qword __attribute__((mode(TI)));
struct S { int a[5]; int n14; };
void *PcgtPointInCbspSafe(struct S *p, int n, qword q);
qword ClosestPointOnCgt(void *p, qword q);
int FindClosestPointInCg(struct S *p, qword q, qword *pq)
{
    void *pv = PcgtPointInCbspSafe(p, p->n14, q);
    if (!pv) return 0;
    *pq = ClosestPointOnCgt(pv, q);
    return 1;
}
