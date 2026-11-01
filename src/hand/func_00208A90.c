struct S { char pad[0x3C0]; int n; int a[1]; };
void *func_002332B8(void *d, void *s, int n);
/** @sly2-annot addr=0x208A90
 * Purpose: Remove cid from +0x3C0
 * Info: proposed name RemoveStepguardForceClass (T2); TU stepguard
 * Notes: names_v2 C proposed UpdateBusyLasenSenseTimes__Fv (0.438)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00208A90(struct S *p, int x)
{
    int i;
    for (i = 0; i < p->n; i++) {
        if (p->a[i] == x) {
            func_002332B8(p->a + i, p->a + i + 1, (p->n - i) * 4 - 4);
            p->n--;
        }
    }
}
