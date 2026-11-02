struct S { char pad[0x3A8]; int n; int a[1]; };
void *func_002332B8(void *d, void *s, int n);
/** @sly2-annot addr=0x208948
 * Purpose: Remove cid from +0x3A8
 * Info: proposed name RemoveStepguardIgnoreClass (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00208948(struct S *p, int x)
{
    int i;
    for (i = 0; i < p->n; i++) {
        if (p->a[i] == x) {
            func_002332B8(p->a + i, p->a + i + 1, (p->n - i) * 4 - 4);
            p->n--;
        }
    }
}
