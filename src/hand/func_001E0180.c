struct T { char pad[0x10]; int n; };
struct O { char pad[0x28]; struct T *pt; char pad2[0xE4-0x2C]; struct O *pNext; };
struct S { char pad[0x5C]; struct O *pFirst; };
/** @sly2-annot addr=0x1E0180
 * Purpose: Find a running JSG of this relay whose block label (prelaycmdFirst->oidLabel) == oid, skipping
 *   pjsgSkip
 * Info: proposed name PjsgFindRelayBlock (T1); TU relay
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
struct O *func_001E0180(struct S *s, int n, struct O *pSkip)
{
    int unused[4];
    struct O *p;
    for (p = s->pFirst; p; p = p->pNext) {
        if (p != pSkip && p->pt->n == n) return p;
    }
    return 0;
}
